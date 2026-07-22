#ifdef FREERTOS_DEBUG
#include "xlt_memory_pri.h"
#endif
#include "stdbool.h"
#include "stdio.h"
#include "securec.h"
#include "errcode.h"
#include "td_base.h"
#include "sys/time.h"
#include "time.h"
#include "flash.h"
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#endif
#include "app_common_api.h"

#define TIME_MAX                64
#define ERASE_TIMES_MAX         10
#define TOTAL_PERCENT           100
#define TIME_YEAR_OFFSET        70
#define TIME_ZONE_OFF           (8 * 60)
#define TIME_SECSPERMIN         60     /* second per minute */

#ifndef SUPPORT_TEST_COMMON
/* 为节省空间，此值根据实际情况设定 */
#define TEST_FUNC_NUM_MAX       300
#define TEST_FUNC_ARGS_MAX      20

static unsigned int g_testsuit_func_count = 0;
static char *g_testsuite_func_argv[TEST_FUNC_ARGS_MAX];
static int g_testsuite_func_argc;
test_func g_testsuite_func_list[TEST_FUNC_NUM_MAX];

int uapi_test_suite_add_function(char *name, char *description, int (*func)(int argc, char *argv[]))
{
    if (g_testsuit_func_count + 1 >= TEST_FUNC_NUM_MAX) {
        return 1;
    }
    g_testsuite_func_list[g_testsuit_func_count].func_name = name;
    g_testsuite_func_list[g_testsuit_func_count].func_description = description;
    g_testsuite_func_list[g_testsuit_func_count].func = func;
    g_testsuit_func_count++;
    return 0;
}

int add_function(char *name, char *description, int (*func)(int argc, char *argv[]))
{
    return uapi_test_suite_add_function(name, description, func);
}

static char testsuite_cmd_split(char *result[], char res_len, char *string, char split_char)
{
    /* replace split_char with '\0' and return the positions of the start of the strings */
    int i = 0;
    bool new = true;
    while ((*string) != 0) {
        if (new) {
            result[i] = string;
            if (++i >= res_len) {
                return (char)(i - 1);
            }
            new = false;
        }
        if (*string == split_char) {
            *string = '\0';
            new = true;
        }
        string++;
    }
    return (char)i;
}

void test_suite_functions_split_command(char *command)
{
    /* initialise the command array to zeros. */
    for (int i = 0; i < TEST_FUNC_ARGS_MAX; i++) {
        g_testsuite_func_argv[i] = 0;
    }

    g_testsuite_func_argc =
        testsuite_cmd_split(g_testsuite_func_argv, TEST_FUNC_ARGS_MAX, command, ' ');
}

int testsuit_msg_handle(char* buf, int buf_size)
{
    int ret;
    test_suite_functions_split_command(buf);
    if (g_testsuite_func_argc != 0) {
        for (int i = 0; i < g_testsuite_func_argc; i++) {
            printf("%s ", g_testsuite_func_argv[i]);
        }
        printf("\n"); /* 输入命令回显结束换行 */
        for (unsigned int i = 0; i < g_testsuit_func_count; i++) {
            if (strncasecmp(g_testsuite_func_argv[0], g_testsuite_func_list[i].func_name,
                strlen(g_testsuite_func_list[i].func_name)) == 0) {
                g_testsuite_func_list[i].func(
                    g_testsuite_func_argc - 1, g_testsuite_func_argv + 1);
                break;
            }
        }
    }
    return 0;
}
#endif

uint32_t get_sec_time(char* time)
{
    uint32_t sec;
    struct tm tm = { 0 };
    char* ptr = time;
    char* tmp;

    // 年
    tmp = strchr(ptr, '-');
    if (tmp == NULL) { return 0; }
    *tmp = '\0';
    // 年的计算需要减去1900
    tm.tm_year = strtol(ptr, NULL, DECIMAL) - 1900;
    if (tm.tm_year < TIME_YEAR_OFFSET) {
        wstp_print("ERR!year num must bigger than 1970!");
        return 0;
    }
    ptr = tmp + 1;

    // 月
    tmp = strchr(ptr, '-');
    if (tmp == NULL) { return 0; }
    *tmp = '\0';
    // 月的计算需要减1
    tm.tm_mon = strtol(ptr, NULL, DECIMAL) - 1;
    ptr = tmp + 1;

    // 日
    tmp = strchr(ptr, ' ');
    if (tmp == NULL) { return 0; }
    *tmp = '\0';
    tm.tm_mday = strtol(ptr, NULL, DECIMAL);
    ptr = tmp + 1;

    // 时
    tmp = strchr(ptr, ':');
    if (tmp == NULL) { return 0; }
    *tmp = '\0';
    tm.tm_hour = strtol(ptr, NULL, DECIMAL);
    ptr = tmp + 1;

    // 分
    tmp = strchr(ptr, ':');
    if (tmp == NULL) { return 0; }
    *tmp = '\0';
    tm.tm_min = strtol(ptr, NULL, DECIMAL);
    ptr = tmp + 1;

    // 秒
    tm.tm_sec = strtol(ptr, NULL, DECIMAL);
    sec = (uint32_t)__tm_to_secs(&tm);

    return sec;
}

uint32_t set_system_time(char* time)
{
    uint32_t sec = get_sec_time(time);
    struct timeval tv = { 0 };
    struct timezone tz = { 0 };

    if (sec == 0) {
        return ERRCODE_FAIL;
    } else if (sec <= (TIME_ZONE_OFF * TIME_SECSPERMIN)) {
        sec = 0;
    } else {
        sec = sec - (TIME_ZONE_OFF * TIME_SECSPERMIN);
    }
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    tz.tz_dsttime = 0;
    tz.tz_minuteswest = TIME_ZONE_OFF;

    settimeofday(&tv, &tz);
    return ERRCODE_SUCC;
}

uint32_t get_system_time(void)
{
    char timebuf[TIME_MAX] = { 0 };
    struct tm tm;
    struct timeval tv = { 0 };
    char* format_str = " %F %T";

    if (gettimeofday(&tv, NULL)) {
        return ERRCODE_FAIL;
    }

    localtime_r(&tv.tv_sec, &tm);
    if (strftime(timebuf, TIME_MAX - 1, format_str + 1, &tm)) {
        wstp_print("%s", timebuf);
    }

    uint32_t millisec = tv.tv_usec / USEC_TO_MILLI;
    wstp_print(":%03d\n", millisec);

    return ERRCODE_SUCC;
}

#ifdef CFG_DRIVERS_NANDFLASH
void erase_block_test(uint32_t start, uint32_t length)
{
    struct nand_driver_struct *driver = get_nand_flash();
    uint32_t ret;
    uint8_t status;
    for (uint32_t i = start; i < (start + length); i++) {
        ret = driver->erase_block(i);
        uint32_t times = 0;
        while (times < ERASE_TIMES_MAX && ret != 0) {
            PRINT("\r\n erase block fail try again %d \r\n", i);
            ret = driver->erase_block(i);
            times++;
        }
        if (ret != 0) {
            PRINT("\r\n erase block fail %d !!!!!! \r\n", i);
            continue;
        }
    }
}

void nand_read_test(struct nand_driver_struct* driver, uint32_t nand_test_blocks, uint8_t* rbuf)
{
    int ret;
    uint8_t status;
    for (uint32_t i = 0; i < nand_test_blocks * driver->pages_per_block; i++) {
        ret = driver->read_page(i, rbuf, driver->bytes_per_page, rbuf + driver->bytes_per_page,
                                driver->bytes_per_oob, &status, TRANS_DEFAULT);
        if (ret != MC_SUCCESS) {
            PRINT("nand aging test read failed %d \r\n", ret);
            continue;
        }
#ifdef ENABLE_ECC
        for (uint32_t j = 0; j < ((driver->bytes_per_oob / 0x2) + driver->bytes_per_page); j++) {
            if (rbuf[j] != 0xFF) {
                PRINT("nand aging test check erase fail, page = %d, %d, rbuf = 0x%x\r\n", i, j, rbuf[j]);
            }
        }
#else
        for (uint32_t j = 0; j < (driver->bytes_per_oob + driver->bytes_per_page); j++) {
            if (rbuf[j] != 0xFF) {
                PRINT("nand aging test check erase fail, page = %d, %d, rbuf = 0x%x\r\n", i, j, rbuf[j]);
            }
        }
#endif
    }
}

void nand_write_test(struct nand_driver_struct* driver, uint32_t nand_test_blocks, uint8_t* wbuf, uint8_t* rbuf)
{
    int ret;
    uint8_t status;
    for (uint32_t i = 0; i < nand_test_blocks * driver->pages_per_block; i++) {
        ret = driver->write_page(i, wbuf, driver->bytes_per_page, wbuf + driver->bytes_per_page,
                                driver->bytes_per_oob);
        if (ret != MC_SUCCESS) {
            PRINT("nand aging test write failed %d \r\n", ret);
            continue;
        }

        ret = driver->read_page(i, rbuf, driver->bytes_per_page, rbuf + driver->bytes_per_page,
                                driver->bytes_per_oob, &status, TRANS_DEFAULT);
        if (ret != MC_SUCCESS) {
            PRINT("nand aging test read failed %d \r\n", ret);
            continue;
        }
#ifdef ENABLE_ECC
        for (uint32_t j = 0; j < ((driver->bytes_per_oob / 0x2) + driver->bytes_per_page); j++) {
            if (wbuf[j] != rbuf[j]) {
                PRINT("nand aging test check data fail, page = %d, %d, wbuf = 0x%x, rbuf = 0x%x\r\n", i, j,
                    wbuf[j], rbuf[j]);
                break;
            }
        }
#else
        for (uint32_t j = 0; j < (driver->bytes_per_oob + driver->bytes_per_page); j++) {
            if (wbuf[j] != rbuf[j]) {
                PRINT("nand aging test check data fail, page = %d, %d, wbuf = 0x%x, rbuf = 0x%x\r\n", i, j,
                    wbuf[j], rbuf[j]);
                break;
            }
        }
#endif
    }
}

static void nand_rw_test(struct nand_driver_struct* driver, uint32_t nand_test_blocks, uint8_t* wbuf, uint8_t* rbuf)
{
    int ret;
    uint8_t status;

    // 擦除测试块
    erase_block_test(0, nand_test_blocks);

    // 读测试块
    nand_read_test(driver, nand_test_blocks, rbuf);

    // 写测试块
    nand_write_test(driver, nand_test_blocks, wbuf, rbuf);
}

static uint32_t nand_write_set(struct nand_driver_struct* driver, uint8_t* wbuf, uint32_t len)
{
    if (memset_s(wbuf, len, 0x0, len) != EOK) {
        return ERRCODE_FAIL;
    }

#ifdef ENABLE_ECC
    uint32_t length = driver->bytes_per_page + (driver->bytes_per_oob / 2);
    if (memset_s(wbuf, len, 0xFF, length) != EOK) {
        return ERRCODE_FAIL;
    }
    if (memset_s(wbuf, len, 0xA5, driver->bytes_per_page / 0x2) != EOK) {
        return ERRCODE_FAIL;
    }
    if (memset_s(wbuf + driver->bytes_per_page / 0x2, len, 0x5A, driver->bytes_per_page / 0x2) != EOK) {
        return ERRCODE_FAIL;
    }
#else
    if (memset_s(wbuf, len, 0xFF, len) != EOK) {
        return ERRCODE_FAIL;
    }
    if (memset_s(wbuf, len, 0xA5, driver->bytes_per_page / 0x2) != EOK) {
        return ERRCODE_FAIL;
    }
    if (memset_s(wbuf + driver->bytes_per_page / 0x2, len, 0x5A, driver->bytes_per_page / 0x2) != EOK) {
        return ERRCODE_FAIL;
    }
#endif
    return ERRCODE_SUCC;
}

uint32_t nandflash_aging_test(char* para)
{
    uint32_t percent, times, nand_test_blocks;
    uint8_t *wbuf, *rbuf;
    char* pos;
    char* ptr = para;
    struct nand_driver_struct* driver = get_nand_flash();

    pos = strchr((char*)ptr, ',');
    if (pos == NULL) {
        return ERRCODE_FAIL;
    }
    *pos = '\0';

    percent = strtoul(ptr, NULL, 0);
    if (percent > TOTAL_PERCENT) {
        percent = TOTAL_PERCENT;
    }
    pos++;
    times = strtoul(pos, NULL, 0);
    nand_test_blocks = driver->total_block * percent / TOTAL_PERCENT;
    wstp_print("nand aging test, per=%d blocks=%d times=%d\r\n", percent, nand_test_blocks, times);
    wstp_print("nand aging test, bytes_per_page=%d bytes_per_oob=%d\r\n", driver->bytes_per_page,
        driver->bytes_per_oob);

    uint32_t len = driver->bytes_per_oob + driver->bytes_per_page;
    wbuf = (uint8_t*)malloc(len);
    if (wbuf == NULL) {
        return ERRCODE_MALLOC;
    }
    nand_write_set(driver, wbuf, len);

    rbuf = (uint8_t*)malloc(len);
    if (rbuf == NULL) {
        free(wbuf);
        return ERRCODE_MALLOC;
    }

    memset_s(rbuf, len, 0x00, len);
    for (uint32_t t = 0; t < times; t++) {
        PRINT("nand aging test %d times\r\n", (t + 1));
        nand_rw_test(driver, nand_test_blocks, wbuf, rbuf);
    }
    free(wbuf);
    free(rbuf);
    wstp_print("nand aging test over!\r\n");
    return ERRCODE_SUCC;
}
#endif

uint32_t flash_aging_test(char* para)
{
    int ret;
    uint32_t times;
    char* ptr = para;

    uint8_t* wbuf = (uint8_t*)malloc(FLASH_PAGE_SIZE);
    if (wbuf == NULL) {
        return ERRCODE_MALLOC;
    }
    (void)memset_s(wbuf, FLASH_PAGE_SIZE, 0x5a, FLASH_PAGE_SIZE);

    times = strtoul(ptr, NULL, 0);
    wstp_print("norflash aging test start! times=%d\r\n", times);
    uint32_t page_nums = FLASH_RESERVED_LEN / FLASH_PAGE_SIZE;
    for (uint32_t t = 0; t < times; t++) {
        for (uint32_t i = 0; i < page_nums; i++) {
            ret = upg_flash_write(FLASH_RESERVED_OFFSET + i * FLASH_PAGE_SIZE, FLASH_PAGE_SIZE, wbuf, true);
            if (ret != ERRCODE_SUCC) {
                PRINT("norflash aging test write fail! offset=%x ret=%d\r\n",
                    (FLASH_RESERVED_OFFSET + i * FLASH_PAGE_SIZE), ret);
                continue;
            }
        }
        uapi_watchdog_kick();
        wstp_print("norflash aging test write %d times\r\n", t + 1);
    }
    wstp_print("norflash aging test over!\r\n");
    return ERRCODE_SUCC;
}

uint32_t freertos_heap_print(void)
{
#ifdef FREERTOS_DEBUG
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)OS_SYS_MEM_POOL;
    xlt_mem_pool_status status = { 0 };
    if (xlt_mem_info_get(OS_SYS_MEM_POOL, &status) == XLT_NOK) { return ERRCODE_FAIL; }

    wstp_print("\npool addr          pool size    used size     free size    "
        "max free node size   used node num     free node num\n");
    wstp_print("---------------    --------     -------       --------     "
        "--------------       -------------      ------------\n");
    wstp_print("%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x\n", pool_info->pool,
        pool_info->pool_size, status.uw_total_used_size, status.uw_total_free_size, status.uw_max_free_node_size,
        status.uw_used_node_num, status.uw_free_node_num);

    pool_info = (xlt_mem_pool_info *)OS_EXTEND_MEM_ADDR;
    if (xlt_mem_info_get(OS_EXTEND_MEM_ADDR, &status) == XLT_NOK) { return ERRCODE_FAIL; }

    wstp_print("\npool addr          pool size    used size     free size    "
        "max free node size   used node num     free node num\n");
    wstp_print("---------------    --------     -------       --------     "
        "--------------       -------------      ------------\n");
    wstp_print("%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x\n", pool_info->pool,
        pool_info->pool_size, status.uw_total_used_size, status.uw_total_free_size, status.uw_max_free_node_size,
        status.uw_used_node_num, status.uw_free_node_num);
#endif
    return ERRCODE_SUCC;
}