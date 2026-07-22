/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test sdio source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16£¬ Create file. \n
 */
#include "mmc.h"
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "cmsis_os2.h"
#include "securec.h"
#include "tcxo.h"
#include "systick.h"
#include "pinctrl_porting.h"
#include "gpio.h"
#include "host.h"
#include "chip_io.h"
#include "test_sdio_host.h"

#define SDIO_WRITE_BYTES 2
#define SDIO_READ_BYTES 3
#define SDIO_READ_FLAG  0
#define SDIO_WRITE_FLAG 1
#define DEC_BASE             10
#define HEX_BASE             16
#define ASCII_NUMBER_OFFSET  48
#define FUNC_NUM 1

#if CHIP_FPGA
int g_manf_id   = 0x12d1;
#else
int g_manf_id   = 0x0296;
#endif
int g_device_id = 0x5347;

char write_buf[] = "abcdefghijklmnopqrstuvwxyz1234567890";

static unsigned long long int emmc_parse_nums(const char *nums)
{
    if (nums == NULL || strlen(nums) == 0) {
        PRINT("%s: addr is NULL\n", __FUNCTION__);
        return 0;
    }
    const unsigned long len = strlen(nums);
    PRINT("%s: num = %s, len = %d\n", __FUNCTION__, nums, len);

    unsigned long long int cnt = 0;
    for (int i = 0; i < len; i++) {
        if (nums[i] < '0' || nums[i] > '9') {
            cnt = 1;
            break;
        }
        int num = 0;
        if (nums[i] >= '0' && nums[i] <= '9') {
            num = (int)nums[i] - ASCII_NUMBER_OFFSET;
        }
        cnt = cnt * 10 + num;
    }
    PRINT("cnt = %llu\r\n", cnt);
    return cnt;
}

static int sdio_host_init(void)
{
#if CHIP_FPGA
    /* FPGA½â¸´Î» */
    writew(GLB_CTL_M_RB_BASE + 0xc8, 0x1);
    writew(GLB_CTL_M_RB_BASE + 0xc8, 0x3);
#endif
    init_sdio_pinmux();
    sdio_drv_context_init();
    return 0;
}

static int test_sdio_host(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t ret = sdio_host_init();
    if (ret != 0) {
        PRINT("sdio_host_init error ret = %d\r\n", ret);
        return -1;
    }
    PRINT("sdio_host_init success!\r\n");
    return 0;
}

static int test_sdio_write_byte(int argc, char *argv[])
{
    UNUSED(argc);
    int func_num = FUNC_NUM;
    int manf_id = g_manf_id;
    int device_id = g_device_id;
    unsigned long long int addr = emmc_parse_nums(argv[0]);
    int err;
    uint8_t byte = emmc_parse_nums(argv[1]);
    struct sdio_func* func = sdio_get_func(1, manf_id, device_id);
    sdio_write_byte(func, byte, addr, &err);
    PRINT("err = %d\r\n", err);
    return err;
}

static int test_sdio_read_byte(int argc, char *argv[])
{
    UNUSED(argc);
    unsigned long long addr = emmc_parse_nums(argv[0]);
    int err;
    uint8_t byte;
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    byte = sdio_read_byte(func, addr, &err);
    PRINT("sdio_read_byte result: 0x%x, err = %d\r\n", byte, err);
    return err;
}

static void check_func_support_csa(bool *issupport_csa, bool *isenable_csa)
{
    int err;
    unsigned int fbr_addr = FUNC_NUM * 0x100 + 0x0;
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    uint8_t byte = sdio_func0_read_byte(func, fbr_addr, &err);
    PRINT("byte = 0x%x\n", byte);
    if (byte & 0x40) {
        PRINT("func %d support csa!\n", FUNC_NUM);
        *issupport_csa = true;
    } else {
        PRINT("func %d don't support csa!\n", FUNC_NUM);
        *issupport_csa = false;
    }
    if (byte & 0x20) {
        PRINT("func %d have enable csa!\n", FUNC_NUM);
        *isenable_csa = true;
    } else {
        PRINT("func %d is not enable csa!\n", FUNC_NUM);
        *isenable_csa = false;
    }
}

static int test_sdio_write_incr_block(int argc, char *argv[])
{
    UNUSED(argc);
    unsigned long long size = emmc_parse_nums(argv[0]);
    unsigned long long block_count = emmc_parse_nums(argv[1]);
    bool issupport_csa = false;
    bool isenable_csa = false;
    unsigned int addr = FUNC_NUM * 0x100 + 0xf;
    if (size <= 0) {
        PRINT("size is illegal\n");
        return -1;
    }
    char *src = (char *)malloc((unsigned long)size + 1);
    if (src == NULL) {
        return -1;
    }
    if (memset_s(src, size + 1, 0, size + 1) != 0) {
        free(src);
        return -1;
    }
    if (memcpy_s(src, size, write_buf, size) != 0) {
        free(src);
        return -1;
    }
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    check_func_support_csa(&issupport_csa, &isenable_csa);
    if (!issupport_csa) {
        PRINT("func %d dont support csa, then return success\n", FUNC_NUM);
        free(src);
        return 0;
    } else if (!issupport_csa && issupport_csa) {
        PRINT("please enable csa\n");
        free(src);
        return -1;
    }
    int ret = sdio_write_incr_block(func, addr, src, block_count);
    free(src);
    return ret;
}

static int test_sdio_read_incr_block(int argc, char *argv[])
{
    UNUSED(argc);
    unsigned long long int size = emmc_parse_nums(argv[0]);
    unsigned long long int block_count = emmc_parse_nums(argv[1]);
    bool issupport_csa = false;
    bool isenable_csa = false;
    unsigned int addr = FUNC_NUM * 0x100 + 0xf;
    if (size <= 0) {
        PRINT("size is illegal\n");
        return -1;
    }
    char* read_buf = (char *)malloc(size);
    if (read_buf == NULL) {
        return -1;
    }
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    check_func_support_csa(&issupport_csa, &isenable_csa);
    if (!issupport_csa) {
        PRINT("func %d dont support csa, then return success\n", FUNC_NUM);
        free(read_buf);
        return 0;
    } else if (!issupport_csa && issupport_csa) {
        PRINT("please enable csa\n");
        free(read_buf);
        return -1;
    }
    int ret = sdio_read_incr_block(func, read_buf, addr, block_count);
    PRINT("test_sdio_read_incr_block result:\n%s\n", read_buf);
    free(read_buf);
    return ret;
}

static int test_sdio_write_fifo_block(int argc, char *argv[])
{
    UNUSED(argc);
    uint32_t index;
    uint32_t addr = 0x0;
    uint64_t size = emmc_parse_nums(argv[0]);
    uint64_t counts = emmc_parse_nums(argv[1]);
    uint64_t loop_times = emmc_parse_nums(argv[2]);
    uint64_t start_time;
    uint64_t end_time;

    int ret = -1;
    if (size < 0) {
        PRINT("size is illegal\n");
        return -1;
    }

    char *src = (char *)malloc(size + 1);
    if (src == NULL) {
        return -1;
    }

    if (memset_s(src, size + 1, 0, size + 1) != 0) {
        free(src);
        return -1;
    }

    for (index = 0; index < size; ++index) {
        src[index] = (index%26) + 'a';
    }

    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    start_time = uapi_tcxo_get_ms();
    for (index = 0; index < loop_times; ++index) {
        ret = sdio_write_fifo_block(func, addr, src, counts);
        if (ret != 0) {
            printf("index:[%u], write err:[%u]\r\n", index, ret);
            break;
        }
    }
    end_time = uapi_tcxo_get_ms();
    free(src);

    printf("sdio host write [%llu]B, cost:[%llu]ms, avg speed:[%f]KB/s\r\n",
        counts*index, end_time - start_time,
        ((float)counts*index)/1024 / ((float)(end_time - start_time)/1000));

    return ret;
}

static int test_sdio_read_fifo_block(int argc, char *argv[])
{
    UNUSED(argc);
    uint32_t index;
    uint32_t addr = 0x0;
    uint64_t size = emmc_parse_nums(argv[0]);
    uint64_t counts = emmc_parse_nums(argv[1]);
    uint64_t loop_times = emmc_parse_nums(argv[2]);
    uint64_t start_time;
    uint64_t end_time;
    int ret = -1;

    if (size < 0) {
        PRINT("size is illegal\n");
        return -1;
    }

    char* read_buf = (char *)malloc(size + 1);
    if (read_buf == NULL) {
        PRINT("read_buf is empty!\r\n");
        return -1;
    }

    if (memset_s(read_buf, size + 1, 0, size + 1) != 0) {
        free(read_buf);
        return -1;
    }

    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    start_time = uapi_tcxo_get_ms();
    for (index = 0; index < loop_times; ++index) {
        ret = sdio_read_fifo_block(func, read_buf, addr, counts);
        if (ret != 0) {
            printf("index:[%u], read err:[%u]\r\n", index, ret);
            break;
        }
    }
    end_time = uapi_tcxo_get_ms();
    free(read_buf);

    printf("sdio host read [%llu]B, cost:[%llu]ms, avg speed:[%f]KB/s\r\n",
        counts*index, end_time - start_time,
        ((float)counts*index)/1024 / ((float)(end_time - start_time)/1000));

    printf("test_sdio_read_fifo_block result: %s\r\n", read_buf+64);

    return ret;
}

static int test_sdio_rescan(int argc, char *argv[])
{
    UNUSED(argc);
    return sdio_rescan((int)emmc_parse_nums(argv[0]));
}

static int test_sdio_readv(int argc, char *argv[])
{
    UNUSED(argc);
    unsigned long long int addr = emmc_parse_nums(argv[0]);
    unsigned long long int size = emmc_parse_nums(argv[1]);
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    struct mmc_sg sg;
    if (size < 0) {
        PRINT("size is illegal\n");
        return -1;
    }
    sg.data = memalign(CACHE_ALIGNED_SIZE, size + 1);
    if (sg.data == NULL) {
        return -1;
    }
    if (memset_s(sg.data, size + 1, 0, size + 1) != 0) {
        free(sg.data);
        return -1;
    }
    sg.length = size + 1;
    int ret = sdio_readv_incr_block(func, addr, &sg, 1);
    PRINT("sg.data = %s\n", sg.data);
    free(sg.data);
    return ret;
}

static int test_sdio_writev(int argc, char *argv[])
{
    UNUSED(argc);
    unsigned long long int addr = emmc_parse_nums(argv[0]);
    unsigned long long int size = emmc_parse_nums(argv[1]);
    struct sdio_func* func = sdio_get_func(FUNC_NUM, g_manf_id, g_device_id);
    struct mmc_sg sg;
    if (size < 0) {
        PRINT("size is illegal\n");
        return -1;
    }
    sg.data = memalign(CACHE_ALIGNED_SIZE, size + 1);
    if (sg.data == NULL) {
        return -1;
    }
    if (memcpy_s(sg.data, size, write_buf, size) != 0) {
        free(sg.data);
        return -1;
    }
    sg.length = size + 1;
    int ret = sdio_writev_incr_block(func, addr, &sg, 1);
    free(sg.data);
    return ret;
}

static int test_sdio_host_erase(int argc, char *argv[])
{
    int func_num = FUNC_NUM;
    int manf_id = g_manf_id;
    int device_id = g_device_id;
    unsigned long long int addr = emmc_parse_nums(argv[0]);
    unsigned long long int size = emmc_parse_nums(argv[1]);
    unsigned long long int block_count = emmc_parse_nums(argv[2]);
    int ret = -1;
    if (size < 0) {
        PRINT("size is illegal\n");
        return -1;
    }
    char *src = (char *)malloc(size + 1);
    if (src  == NULL) {
        return -1;
    }
    if (memset_s(src, size + 1, 0, size + 1) != 0) {
        free(src);
        return -1;
    }
    struct sdio_func* func = sdio_get_func(func_num, manf_id, device_id);
    ret = sdio_write_fifo_block(func, addr, src, block_count);
    free(src);
    return 0;
}

void add_sdio_host_case(void)
{
    uapi_test_suite_add_function("sdio_host_init", "sdio host init", test_sdio_host);
    uapi_test_suite_add_function("test_sdio_host_write_fifo", "sdio_write_fifo_block", test_sdio_write_fifo_block);
    uapi_test_suite_add_function("test_sdio_host_read_fifo",  "sdio_read_fifo_block", test_sdio_read_fifo_block);
    uapi_test_suite_add_function("test_sdio_rescan", "test_sdio_rescan", test_sdio_rescan);

    uapi_test_suite_add_function("test_sdio_host_write", "sdio_write_incr_block", test_sdio_write_incr_block);
    uapi_test_suite_add_function("test_sdio_host_read", "sdio_read_incr_block", test_sdio_read_incr_block);
    uapi_test_suite_add_function("test_sdio_host_erase", "sdio_host_read", test_sdio_host_erase);
    uapi_test_suite_add_function("sdio_read_byte",  "sdio_read_byte",  test_sdio_read_byte);
    uapi_test_suite_add_function("sdio_write_byte", "sdio_write_byte", test_sdio_write_byte);
    uapi_test_suite_add_function("test_sdio_readv", "test_sdio_readv", test_sdio_readv);
    uapi_test_suite_add_function("test_sdio_writev", "test_sdio_writev", test_sdio_writev);
}
