/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Media task process
 * Author:
 * Create:
 */
#if defined(__LITEOS__)
#include "los_memory.h"
#include "los_memory_pri.h"
#ifdef CFG_DRIVERS_MMC
#include "soc/mmc.h"
#endif
#endif

#ifdef CFG_DRIVERS_MMC
#include "hci.h"
#include "soc_mmc.h"
#endif

#if defined(__FREERTOS__)
#ifdef FREERTOS_DEBUG
#include "xlt_memory_pri.h"
#include "task_debug.h"
#endif
#endif

#include "debug_print.h"
#include "td_base.h"
#include "securec.h"
#include "log_file.h"
#include "dfx_adapt_layer.h"
#include "dirent.h"
#include "uart.h"
#include "uapi_crc.h"
#include "tcxo.h"
#include "cmsis_os2.h"
#include "lcd_bus.h"
#include "soc_vau_type.h"
#include "soc_dpu_type.h"
#include "soc_mipi_tx.h"
#include "touch_screen_def.h"
#include "nandflash.h"
#include "efuse.h"
#include "psram_drv.h"
#include "flash.h"
#include "adc.h"
#include "exception.h"
#include "fcntl.h"
#include <sys/stat.h>
#include "common_def.h"
#include "unistd.h"
#include "watchdog.h"
#include "button_porting.h"
#include "hal_reboot.h"
#include "app_msg_manager.h"
#include "sys/statfs.h"
#include "asm/memmap_config.h"
#include "watchdog_porting.h"
#include "thread_init.h"

#include "pm.h"
#include "pm_veto.h"
#include "pm_lp_stub.h"
#include "pm_definition.h"
#include "build_version.h"
#include "at_cmd_api.h"
#include "app_test_fs.h"
#include "app_test_perf.h"
#include "app_common_api.h"
#include "app_tx_process.h"
#include "dfx_feature_config.h"
#include "app_at_process.h"
#include "pm_veto.h"
#include "input_app.h"
#include "lpc_core.h"
#include "diag_msg.h"
#include "pmu_ldo.h"
#include "soc_gmmu_api.h"
#include "pinctrl.h"
#include "non_os_reboot.h"
#include "km.h"
#include "cipher_api.h"
#include "key_deriv_porting.h"
#include "otp_map.h"
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#endif

#if defined(SW_RTT_DEBUG)
#include "SEGGER_RTT.h"
#endif
#ifdef SUPPORT_POWER_MANAGER
#include "power_display_service.h"
#endif
#include "i2c.h"
#include "calendar.h"

#ifdef SUPPORT_FILE_SPLIT_COMBINE
#include "msg_center_protocol.h"
#endif

#ifdef SUPPORT_ALIPAY_SEC
#include "alipay_feature.h"
#endif

#define ARRAY_COUNT(array)      (sizeof(array) / sizeof(array[0]))
#define DIAG_RES_LEN            4
#define FF_VOLUMES_NUM          4
#define ADC_AUTODATA_PRT_SIZE   4
#define ADC_AUTODATA_PRT_INDEX1 1
#define ADC_AUTODATA_PRT_INDEX2 2
#define ADC_AUTODATA_PRT_INDEX3 3
#define ADC_AUTODATA_PRT_INDEX4 4
#define VER_STR_MAX             128
#define FILE_MAX_NUM            32
#define MAX_FILE_PATH_LEN       128
#define TEST_MSG_MAX_SIZE       40
#define LOG_FILE_ARG_NUM        6
#define FS_NODE_INFO_SPACE      16
#define MB_SIZE                 (1024 * 1024)
#define KB_SIZE                 (1024)
#define ONE_SECOND              (1000000)
#define DTCM_RAM_OFF            (400 * 1024)    // dtcm偏移，从最后未使用的dtcm作为测试内存
#define INVALID_HANDLE          (0xFFFFFFFF)

#ifdef CONFIG_PSRAM_SUPPORT
#define LCD_FILL_COLOR_ADDR   0x6D200000
#else
#define LCD_FILL_COLOR_ADDR   0x60280000
#endif

#define POWER_KEY_PARAM_LEN                0x2
#define CALENDAR_PARAM_LEN                 0x6
#ifdef CFG_DRIVERS_MMC
#define FS_VOLUMES_NUM 4
#define FS_VOLUME_STRS "/system/", "/user/", "/update/", "/music/"
extern unsigned int mmc_set_clock_with_way2(struct hci_softc *sc, unsigned int cfg_clock);
uint32_t g_app_emmc_clk = SDIOM_SRC_CLK_FREQ_LVL_98M_VAL;
#endif

#ifdef CFG_DRIVERS_NANDFLASH
#ifndef MEMORY_MINI
#define FS_VOLUMES_NUM 4
#define FS_VOLUME_STRS "/system/", "/update/", "/music/", "/user/"
#else
#define FS_VOLUMES_NUM 1
#define FS_VOLUME_STRS "/user/"
#endif
#endif

#define AT_I2C_PARA_CNT  4
#define AT_I2C_REG_ARRAY_LEN 8

#define AT_PARA_IDX2 2
#define AT_PARA_IDX3 3
#define AT_PARA_IDX4 4
#define AT_PARA_IDX5 5
#define AT_PARA_IDX6 6
#define AT_PARA_IDX7 7
#define AT_PARA_IDX8 8

#define OFFSET_8  8
#define OFFSET_16 16
#define OFFSET_24 24

#define AES_SALT_LENGTH         16
#define AES_IV_LENGTH           16
// AES_SAMPLE_TEST_LEN must be an integer multiple of 32 bytes.
#define AES_SAMPLE_TEST_LEN     64
#define ALIPAY_EXIT_LPM_DELAY_LEN 100

#ifdef SUPPORT_FILE_SPLIT_COMBINE
/* 文件拆分合并测试 */
#define ORIGIN_FILE_PATH "/user/test/ssb.bin"
#define FILE_LIST_PATH "/user/test/flist.txt"
#define COMBINE_FILE_PATH "/user/test/ssb_c.bin"
#endif

#ifdef SUPPORT_AUTO_OTA
extern uint8_t g_auto_ota_upload_mode;
#ifndef ONE_ADDR_LEN
#define ONE_ADDR_LEN 2
#endif
#ifndef ADDR_MAC_LEN
#define ADDR_MAC_LEN 6
#endif
#endif

const uint8_t g_decrypt_aes_salt[AES_SALT_LENGTH] = { 0x0 };
const uint8_t g_decrypt_aes_iv[AES_IV_LENGTH] = { 0x0 };
// The length of the GID must be 32 bytes and cannot be changed.
const uint8_t g_decrypt_aes_gid[OTP_SECURITY_KEY_GID_LEN] = { 0x0 };
// This is the ASCII code of the string to be encrypted.
const uint8_t g_decrypt_aes_encryp[AES_SAMPLE_TEST_LEN] = {};
// This is the ASCII code of the string to be decrypted.
const uint8_t g_decrypt_aes_decryp[AES_SAMPLE_TEST_LEN] = {0x15, 0x56, 0xa7, 0x38, 0x7f, 0xd3, 0x69, 0x72, 0x16,
    0x2, 0xda, 0x36, 0xf3, 0x99, 0xcd, 0xf7, 0x49, 0xf4, 0x2d, 0x15, 0x77, 0xd5, 0x4f, 0xdd, 0x14, 0xd1, 0xc3,
    0x7f, 0x41, 0xd6, 0x99, 0x47, 0x2f, 0xb1, 0x34, 0xdf, 0x87, 0xa0, 0x49, 0x53, 0x23, 0x43, 0xf0, 0x96, 0xed,
    0xbf, 0x5, 0x1d, 0x78, 0xc4, 0xa3, 0xfe, 0xf2, 0xa3, 0xb7, 0x8e, 0x29, 0xd9, 0xd0, 0x86, 0xc5, 0x5f, 0xc4, 0x8f};

static const char * const g_volume_names[FS_VOLUMES_NUM] = { FS_VOLUME_STRS };

#if DSP_L2RAM_AREA_SIZE == 8
extern void *__l2m_dsp_data_end;
#else
extern void *__l2m_gpu_data_end;
#endif

diag_ser_header_t g_mcu_diag_header;    // 用于保存diag回复包头内容
bool g_app_diag_flag = false;           // 用于控制执行功能后是否回复diag消息

pm_id_t g_pm_req_mode_usr = PM_ID_DEBUG;
#ifdef CONFIG_LOW_POWER_TEST
static bool g_dpu_ready_flag = false;
#else
static bool g_dpu_ready_flag = true;
#endif

void wstp_print(const char *str, ...)
{
    static uint8_t s[256];  // 256 means string max length.
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }
#if defined(SW_RTT_DEBUG)
    SEGGER_RTT_Write(0, (const char *)s, str_len);
#else
    if (hal_uart_get_funcs(SW_DEBUG_UART_BUS) == NULL) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, str_len, 0);
#endif
}

void factory_test_print(const uint8_t return_code, const char *str, ...)
{
    static uint8_t s[256];  // 256 means string max length.
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len <= 0) {
        return;
    }
#ifdef SW_UART_DEBUG
    wstp_print("[Factory]|%s.\r\n", s);
#endif
#ifdef FT_BT_UART
    at_cmd_send_abt_result_to_bt((const void *)s, str_len);
#endif
#ifdef FT_DONGLE
    // 发送spp包
    uint8_t table_len = 2 + str_len; // return_code + str_len + payload
    diag_ser_header_t header_temp = {
        .ser_id = DIAG_SER_APP_TEST,
        .cmd_id = FTDONGLERETURN,
        .src = DIAG_FRAME_FID_MCU, // DIAG_FRAME_FID_MCU
        .dst = DIAG_FRAME_FID_PHONE, // DIAG_FRAME_FID_PHONE
        .crc_en = true,
        .ack_en = false,
        .length = 2 + table_len, // moduld + cmd + table_len
    };
    factory_print_table *ft_table = (factory_print_table*)malloc(sizeof(factory_print_table));
    if (ft_table == NULL) {
        return;
    }
    ft_table->return_code = return_code;
    ft_table->payload_len = str_len;
    if (memcpy_s(ft_table->payload, FT_PAYLOAD_LEN_MAX, s, str_len) != EOK) {
        wstp_print("[Factory]|memcpy_s fail!\r\n");
        free(ft_table);
        return;
    }
    send_spp_diag_msg(true, &header_temp, ft_table, table_len);
    free(ft_table);
#endif
}

void print_at_cmd_ret(int ret)
{
    uint32_t irq_sts = osal_irq_lock();
    if (ret == ERRCODE_SUCC) {
        wstp_print("\r\nOK~\r\n");
    } else {
        wstp_print("\r\nERR~\r\n");
    }

#ifdef FT_BT_UART
    if (ret == ERRCODE_SUCC) {
        at_cmd_send_abt_result_to_bt((uint8_t *)"\r\nOK~\r\n", strlen("\r\nOK~\r\n"));
    } else {
        at_cmd_send_abt_result_to_bt((uint8_t *)"\r\nERR~\r\n", strlen("\r\nERR~\r\n"));
    }
#endif
    osal_irq_restore(irq_sts);
}

/* Diag消息获取TLV中V值数据处理接口 */
uint32_t parse_tlv_2value(uint8_t* data, uint16_t data_len, uint8_t** out, uint32_t *out_len)
{
    unused(data_len);
    uint8_t num[WORD_WIDTH] = {0};
    uint8_t* ptr = data;
    uint8_t len;
    uint32_t length = 0;
    int i;

    ptr++;
    // 动态len运算，7bit为1代表剩下0-6bit为真实长度，7bit为0，0-6bit代表后面几个字节为真实长度。
    len = *(uint8_t*)ptr++;
    if (((len & BIT7) >> 7) == 1) {
        length = len & (~BIT7);
    } else {
        // 按照len数值，将真实长度数据小端保存到num中，默认最大长度四个字节，已够用
        for (i = 0; i < len; i++) {
            num[i] = *ptr++;
        }
        length = *(uint32_t*)num;
    }
    *out = ptr;
    *out_len = length;
    return ERRCODE_SUCC;
}

/* diag消息再封装转发app task接口 */
uint32_t mcu_diag_cmd_process(diag_ser_data_t *data)
{
    app_msg_info_t* app_msg;
    uint8_t* ptr;
    uint32_t paylen;
    diag_ser_frame_t* frame = (diag_ser_frame_t*)data->payload;

    if (data == NULL) {
        return ERRCODE_FAIL;
    }

    parse_tlv_2value(frame->tlv, data->header.length, &ptr, &paylen);
    app_msg = (app_msg_info_t*)osal_kmalloc(sizeof(app_msg_info_t) + data->header.length, OSAL_GFP_KERNEL);
    if (app_msg == NULL) {
        wstp_print("mcu_diag_cmd_process malloc fail!!"NEWLINE);
        return ERRCODE_FAIL;
    }
    (void)memset_s(app_msg, sizeof(app_msg_info_t) + data->header.length, 0, sizeof(app_msg_info_t) + data->header.length);
    (void)memcpy_s(&g_mcu_diag_header, sizeof(diag_ser_header_t), &(data->header), sizeof(diag_ser_header_t));
    app_msg->id = data->header.cmd_id;
    app_msg->payload_len = paylen;
    if (app_msg->payload_len > 0 && data != NULL) {
        if (memcpy_s(app_msg->payload, app_msg->payload_len, (const void *)ptr, paylen) != EOK) {
            wstp_print("mcu_diag_cmd_process data memcpy error!"NEWLINE);
            osal_kfree(app_msg);
            return ERRCODE_FAIL;
        }
    }
    g_app_diag_flag = true;
    app_at_msg_send((uint8_t *)app_msg, sizeof(app_msg_info_t) + paylen);
    osal_kfree(app_msg);
    return ERRCODE_SUCC;
}

/* 封装返回结果成Diag数据回发给BTH SPP
param descrip:
   send: 用于控制是否做diag消息回复
   data: 用于传入diag包包头信息
   payload: 回复tlv数据
   pay_len: 回复tlv数据长度
*/
uint32_t send_spp_diag_msg(bool send, diag_ser_header_t *data, uint8_t* payload, uint32_t pay_len)
{
    uint8_t size;
    diag_ser_data_t* diag_data;
    diag_ser_frame_t* frame;
    uint32_t result = ERRCODE_FAIL;

    // send为false，则直接返回
    if (send == false) {
        return ERRCODE_SUCC;
    }

    size = sizeof(diag_ser_data_t) + sizeof(diag_ser_frame_t) + pay_len;
    diag_data = (diag_ser_data_t *)malloc(size);
    if (diag_data == NULL) {
        return ERRCODE_FAIL;
    }

    (void)memcpy_s(&(diag_data->header), sizeof(diag_ser_header_t), data, sizeof(diag_ser_header_t));
    frame = (diag_ser_frame_t*)&(diag_data->payload);
    frame->module_id = data->ser_id;
    frame->cmd_id = data->cmd_id;
    (void)memcpy_s(&(frame->tlv), pay_len, payload, pay_len);
    diag_data->header.crc_en = 1;
    diag_data->header.length = size - sizeof(diag_ser_data_t);
    diag_data->header.dst = DIAG_FRAME_FID_PHONE;
    diag_data->header.src = data->dst;
    result = uapi_diag_service_send_data(diag_data);
    free(diag_data);
    return result;
}

uint32_t mcu_os_sys_reset(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint8_t res[DIAG_RES_LEN] = {0x80, 0x82, 0x00, 0x00};

    send_spp_diag_msg(g_app_diag_flag, &g_mcu_diag_header, res, DIAG_RES_LEN);
    reset_deal_with_fs();

    set_cpu_utils_reset_cause(REBOOT_CAUSE_APPLICATION_SYSRESETREQ);
    hal_reboot_chip();
    return ERRCODE_SUCC;
}

uint32_t mcu_fs_test(uint8_t *para, uint32_t para_len)
{
    test_file_system(para, para_len);
    return ERRCODE_SUCC;
}

uint32_t mcu_os_heap_info(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#if LOSCFG_MEM_TASK_STAT && defined __LITEOS__
    LosMemPoolInfo *heapMan = (LosMemPoolInfo*)OS_EXTEND_MEM_ADDR;
    TSK_INFO_S task_info;
    uint32_t taskId;

    wstp_print("\r\n");
    OsMemInfoPrint(OS_EXTEND_MEM_ADDR);
    wstp_print("\n\t\tstack name  task id  used size  peak size\n");
    for (taskId = 0; taskId <= LOSCFG_BASE_CORE_TSK_LIMIT; taskId++) {
        if (0 != LOS_TaskInfoGet(taskId, &task_info)) {
            // 过滤掉无用的task id
            continue;
        }
        wstp_print("%22s  %-5d    0x%-8x 0x%-8x\n", task_info.acName, taskId,
            heapMan->stat.taskMemstats[taskId].memUsed, heapMan->stat.taskMemstats[taskId].memPeak);
    }
    wstp_print("\r\n\r\n");

    heapMan = (LosMemPoolInfo*)OS_SYS_MEM_ADDR;
    OsMemInfoPrint(OS_SYS_MEM_ADDR);
    wstp_print("\n\t\tstack name  task id  used size  peak size\n");
    for (taskId = 0; taskId <= LOSCFG_BASE_CORE_TSK_LIMIT; taskId++) {
        if (0 != LOS_TaskInfoGet(taskId, &task_info)) {
            // 过滤掉无用的task id
            continue;
        }
        wstp_print("%22s  %-5d    0x%-8x 0x%-8x\n", task_info.acName, taskId,
            heapMan->stat.taskMemstats[taskId].memUsed, heapMan->stat.taskMemstats[taskId].memPeak);
    }
    wstp_print("\r\n");

#endif
#if defined __FREERTOS__ && defined FREERTOS_DEBUG
    freertos_heap_print();
#endif
    return ERRCODE_SUCC;
}

uint32_t mcu_os_stack_info(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

#if LOSCFG_MEM_TASK_STAT && defined __LITEOS__
    TSK_INFO_S task_info;
    uint32_t taskId;

    wstp_print("\n      stack name      task id    prio  status    stack addr     total size   used size   peak size\n"
        "     --------------   -------    ---   ------    ---------      --------     --------    --------\n");
    for (taskId = 0; taskId <= LOSCFG_BASE_CORE_TSK_LIMIT; taskId++) {
        if (0 != LOS_TaskInfoGet(taskId, &task_info)) {
            continue;
        }
        wstp_print("%22s      %-5d   %-2d      %-2d      %-10p    0x%-8x   0x%-8x   0x%-8x\n", task_info.acName,
            task_info.uwTaskID, task_info.usTaskPrio, task_info.usTaskStatus, task_info.uwTopOfStack,
            task_info.uwStackSize, task_info.uwCurrUsed, task_info.uwPeakUsed);
    }
#endif

#if defined __FREERTOS__ && defined FREERTOS_DEBUG
    vTaskInfoPrint();
#endif

    return ERRCODE_SUCC;
}

static uint32_t mcu_fs_format(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    int ret, i;

    /* 解决后台文件打开导致格式化失败的问题，先关闭可能打开的后台文件*/
    for (i = 0; i < FILE_MAX_NUM; i++) {
        close(i);
    }
    /* 如果离线日志打开，则关闭离线日志 */
    uapi_zdiag_set_offline_log_enable(false);

#ifdef CFG_DRIVERS_MMC
    wstp_print("\r\nemmc format,wait a moment!\r\n");
    ret = emmc_drv_format();
    if (ret != ENOERR) {
        ret = ERRCODE_FAIL;
    } else {
        ret = ERRCODE_SUCC;
    }
#endif

#ifdef CFG_DRIVERS_NANDFLASH
    wstp_print("\r\nnandflash format,wait a moment!\r\n");
    nandflash_yaffs_format();
    ret = ERRCODE_SUCC;
#endif

    return ret;
}

static uint32_t mcu_fs_rw_test(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint8_t *ptr = NULL;
    int ret;
    char path[] = "/user/user_test.log";
    FILE *fp = NULL;

    fp = fopen(path, "w+");
    if (fp == NULL) {
        wstp_print("open fail\r\n");
        return ERRCODE_FAIL;
    }
    ret = test_libc_rw(fp);
    fclose(fp);
    remove(path);

    return ret;
}

static uint32_t mcu_fs_set_clk(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef CFG_DRIVERS_MMC
    char *ptr = (char*)para;
    uint8_t mode = 0;
    struct hci_softc *sc;
    if (para == NULL) {
        return ERRCODE_FAIL;
    }

    mode = (uint8_t)strtoul(ptr, NULL, 0);
    sc = get_mmc_resource(0);
    if (mode == 0) {
        wstp_print("set emmc clk to 98M!!!\r\n");
        mmc_set_clock_with_way2(sc, SDIOM_SRC_CLK_FREQ_LVL_98M_VAL);
        g_app_emmc_clk = SDIOM_SRC_CLK_FREQ_LVL_98M_VAL;
    } else if (mode == 1) {
        wstp_print("set emmc clk to 32M!!!\r\n");
        mmc_set_clock_with_way2(sc, SDIOM_SRC_CLK_LOW_FREQ_VAL);
        g_app_emmc_clk = SDIOM_SRC_CLK_LOW_FREQ_VAL;
    } else {
        wstp_print("Input para Error!!!\r\n");
        return ERRCODE_FAIL;
    }
#endif

    return ERRCODE_SUCC;
}

static uint32_t mcu_fs_read_clk(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef CFG_DRIVERS_MMC
    wstp_print("\r\nclock:%d", g_app_emmc_clk);
#endif
    return ERRCODE_SUCC;
}

static uint32_t mcu_fs_check(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint32_t FileSize = 0;
    uint32_t times;
    uint32_t len;
    char FilePath[MAX_FILE_PATH_SIZE] = "/user/";
    FILE *fp = NULL;

    if (para_len == 0) {
        return ERRCODE_FAIL;
    }

    ptr++;
    if (memcpy_s(FilePath + strlen(FilePath), (MAX_FILE_PATH_SIZE - strlen(FilePath) - 1), ptr,
        (strchr(ptr, '"') - ptr)) != EOK) {
            wstp_print("memcpy ERR!,The file path is too long,\
                the total length of the file path should be less than %d\r\n", MAX_FILE_PATH_SIZE);
            return ERRCODE_FAIL;
    }

    ptr += (strchr(ptr, '"') - ptr);
    ptr += TEST_PARA_2;
    FileSize = strtoul(ptr, NULL, 0);
    if (FileSize > FS_TEST_1M_BYTES) { FileSize = FS_TEST_1M_BYTES; }
    times = (FileSize / BYTES_LEN_4K) ? (FileSize / BYTES_LEN_4K) : 1;

    fp = fopen(FilePath, "r");
    if (fp == NULL) {
        wstp_print("open fail!path:%s\r\n", FilePath);
        return ERRCODE_FAIL;
    }

    len = test_libc_read(fp, times);
    fclose(fp);
    if (len == times * BYTES_LEN_4K) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}


static int32_t rm_dir(const char *pathname)
{
    struct dirent *dirent = NULL;
    struct stat stat_info;
    DIR *d = NULL;
    char *fullpath = NULL;
    int ret;

    (void)memset_s(&stat_info, sizeof(stat_info), 0, sizeof(struct stat));
    if (stat(pathname, &stat_info) != 0) {
        wstp_print("read file info fail! %s, err=%d\n", pathname, get_errno());
        return -1;
    }

    if (S_ISREG(stat_info.st_mode)) {
        wstp_print("rm path: %s \n", pathname);
        return unlink(pathname);
    }
    d = opendir(pathname);
    while (1) {
        dirent = readdir(d);
        if (dirent == NULL) {
            break;
        }
        if (strcmp(dirent->d_name, "..") && strcmp(dirent->d_name, ".")) {
            size_t fullpathBufSize = strlen(pathname) + strlen(dirent->d_name) + 2; // 2 is sep eof length
            fullpath = (char *)malloc(fullpathBufSize);
            if (fullpath == NULL) {
                wstp_print("malloc failure!\n");
                (void)closedir(d);
                return -1;
            }
            ret = snprintf_s(fullpath, fullpathBufSize, fullpathBufSize - 1, "%s/%s", pathname, dirent->d_name);
            if (ret < 0) {
                wstp_print("name is too long!\n");
                free(fullpath);
                (void)closedir(d);
                return -1;
            }
            (void)rm_dir(fullpath);
            free(fullpath);
        }
    }
    (void)closedir(d);
    return rmdir(pathname);
}

static uint32_t mcu_fs_rm(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    if (para_len == 0) {
        return ERRCODE_FAIL;
    }

    char *ptr = (char *)para;
    char file_path[MAX_FILE_PATH_SIZE] = "";

    ptr++;
    if (memcpy_s(file_path + strlen(file_path), (MAX_FILE_PATH_SIZE - strlen(file_path) - 1), ptr,
        (strchr(ptr, '"') - ptr)) != EOK) {
            wstp_print("memcpy ERR!,The file path is %s, max len is %d\r\n", para, MAX_FILE_PATH_SIZE);
            return ERRCODE_FAIL;
    }

    wstp_print("excute rm path:%s\r\n", file_path);
    if (rm_dir(file_path) >= 0) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

static uint32_t mcu_get_wstp_version(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    char str[VER_STR_MAX];
#if (defined BUILD_TIME) && (defined TARGET_NAME)
    (void)sprintf_s(str, VER_STR_MAX, "\r\nVersion:%s %s %s\r\n",
                    get_version_string(), TARGET_NAME, BUILD_TIME);
#else
    (void)sprintf_s(str, VER_STR_MAX, "\r\nVersion:%s\r\n", get_version_string());
#endif
    wstp_print(str);

    return ERRCODE_SUCC;
}

static uint32_t mcu_fs_write(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint32_t FileSize = 0;
    uint32_t times;
    uint32_t len;
    char FilePath[MAX_FILE_PATH_SIZE] = "/user/";

    if (para_len == 0) {
        return ERRCODE_FAIL;
    }

    ptr++;
    if (memcpy_s(FilePath + strlen(FilePath), (MAX_FILE_PATH_SIZE - strlen(FilePath) - 1), ptr,
        (strchr(ptr, '"') - ptr)) != EOK) {
            wstp_print("memcpy ERR!,The file path is too long,\
                the total length of the file path should be less than %d\r\n", MAX_FILE_PATH_SIZE);
            return ERRCODE_FAIL;
    }
    ptr += (strchr(ptr, '"') - ptr);
    ptr += TEST_PARA_2;
    FileSize = strtoul(ptr, NULL, 0);
    remove(FilePath);
    if (FileSize > FS_TEST_1M_BYTES) { FileSize = FS_TEST_1M_BYTES; }
    times = (FileSize / BYTES_LEN_4K) ? (FileSize / BYTES_LEN_4K) : 1;

    FILE *fp = NULL;
    fp = fopen(FilePath, "w+");
    if (fp == NULL) {
        wstp_print("open fail!path:%s\r\n", FilePath);
        return ERRCODE_FAIL;
    }

    len = test_libc_write(fp, times);
    fclose(fp);
    if (len == times * BYTES_LEN_4K) {
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_FAIL;
    }
}

#if (CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE == DFX_YES)
static uint32_t mcu_init_logfile(uint8_t *para, store_file_cfg_t *cfg, uint32_t *arr, uint32_t arr_len)
{
    char *ptr = (char *)para;
    char *ptr_end = NULL;

    if (strtoul(ptr, &ptr_end, 0) <= 0) {
        return ERRCODE_FAIL;
    }

    for (uint32_t j = 0; j < arr_len; j++) {
        arr[j] = strtoul(ptr, &ptr_end, 0);
        ptr = ptr_end + 1;
    }

    uint32_t clear_flag = 0;
    uint32_t i = 0;

    cfg->mult_files = arr[i++];
    cfg->file_size = arr[i++];
    cfg->enable_cache = arr[i++];
    cfg->cache_size = arr[i++];
    i++;
    clear_flag = arr[i];
    cfg->path = "/user/test/";
    cfg->name = "log_file_write_0222.bin";
    wstp_print("mult_files = %d, file_size = %d, enable_cache = %d, cache_size = %d clear_flag = %d\r\n",
        cfg->mult_files, cfg->file_size, cfg->enable_cache, cfg->cache_size, clear_flag);

    errcode_t ret = uapi_logfile_init();
    if (ret != ERRCODE_SUCC) {
        wstp_print("init logfile %s failed.\r\n", cfg->name);
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static uint32_t mcu_clear_logfile_dir(store_file_cfg_t *cfg)
{
    static uint8_t full_path[MAX_FILE_PATH_LEN] = {0};
    struct dirent *pdirent = NULL;
    DIR *d = NULL;
    uint32_t ret = ERRCODE_SUCC;

    d = opendir(cfg->path);
    if (d == NULL) {
        if (mkdir(cfg->path, S_IREAD | S_IWRITE) != 0) {
            wstp_print("mkdir %s failed!\r\n", cfg->path);
            ret = ERRCODE_FAIL;
        }
    } else {
        do {
            pdirent = readdir(d);
            if (pdirent == NULL) {
                break;
            }
            wstp_print("file_dir clear start!\r\n");
            if (strncpy_s((char *)full_path, MAX_FILE_PATH_LEN, cfg->path, strlen(cfg->path)) != EOK) {
                ret = ERRCODE_FAIL;
                break;
            }
            if (strcat_s((char *)full_path, MAX_FILE_PATH_LEN, pdirent->d_name) != EOK) {
                ret = ERRCODE_FAIL;
                break;
            }
            if (remove((char *)full_path) != 0) {
                wstp_print("remove %s failed!\r\n", full_path);
                ret = ERRCODE_FAIL;
                break;
            }
        } while (pdirent != NULL);
    }
    (void)closedir(d);
    return ret;
}
#endif /* CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE */

static uint32_t mcu_logfile_write(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#if (CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE == DFX_YES)
    wstp_print("logfile test start!\r\n");
    char test_msg[TEST_MSG_MAX_SIZE] = {0};
    #define TEST_NUM_ID 4
    /*
        arg[0] -- multi_num
        arg[1] -- file_size
        arg[2] -- enable_cache
        arg[3] -- cache_size
        arg[4] -- test timer
        arg[5] -- clear flag
    */
    uint32_t arg[LOG_FILE_ARG_NUM] = {0};
    store_file_cfg_t cfg = {0};

    if (mcu_init_logfile(para, &cfg, arg, LOG_FILE_ARG_NUM) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (arg[LOG_FILE_ARG_NUM - 1] != 0) { /* arg[5] -- clear flag */
        if (mcu_clear_logfile_dir(&cfg) != ERRCODE_SUCC) {
            wstp_print("clear file_dir error!\r\n");
            return ERRCODE_FAIL;
        }
    }

    errcode_t ret = uapi_logfile_open(STORE_DIAG, &cfg);
    if (ret != ERRCODE_SUCC) {
        wstp_print("open log_file %s failed.\r\n", cfg.name);
        return ERRCODE_FAIL;
    }

    for (uint32_t i = 0; i < arg[TEST_NUM_ID]; i++) { /* arg[2] -- test timer */
        (void)memset_s(test_msg, sizeof(test_msg), 0, sizeof(test_msg));
        if (sprintf_s(test_msg, sizeof(test_msg), "abcdef%10d", i + 1) < 0) {
            wstp_print("sprintf_s failed.\r\n");
            return ERRCODE_FAIL;
        }
        ret = uapi_logfile_write(STORE_DIAG, 0, (uint8_t*)&test_msg, strlen(test_msg));
        if (ret != ERRCODE_SUCC) {
            wstp_print("write logfile failed.\r\n");
            return ERRCODE_FAIL;
        }
        dfx_sleep(10); /* 10 : sleep 10ms */
    }

    ret = uapi_logfile_close(STORE_DIAG);
    if (ret != ERRCODE_SUCC) {
        wstp_print("close logfile %s failed.\r\n", cfg.name);
        return ERRCODE_FAIL;
    }
#endif /* CONFIG_DFX_SUPPORT_OFFLINE_LOG_FILE */
    return ERRCODE_SUCC;
}

uint32_t mcu_test_dtcm_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint64_t start_time, end_time;
    uint32_t total_time;
    uint32_t bw;

    char *test_buf = (char*)(APP_DTCM_ORIGIN + DTCM_RAM_OFF);
    uint32_t irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf, BUFF_LEN_48K, APP_DTCM_ORIGIN, BUFF_LEN_48K) != 0) {
        wstp_print("[dctm mem test]dtcm read memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time);
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[dctm mem test]dtcm read time:%dus bw:%dMB/s\r\n", total_time, bw);

    irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf, BUFF_LEN_48K, APP_DTCM_ORIGIN, BUFF_LEN_48K) != 0) {
        wstp_print("[dctm mem test]dtcm write memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time);
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[dctm mem test]dtcm write time:%dus bw:%dMB/s\r\n", total_time, bw);
    return ERRCODE_SUCC;
}

uint32_t mcu_test_l2m_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint64_t start_time, end_time;
    uint32_t total_time;
    uint32_t bw;

#if DSP_L2RAM_AREA_SIZE == 8
    char* l2m_ptr = (char*)(&__l2m_dsp_data_end);
#else
    char* l2m_ptr = (char*)(&__l2m_gpu_data_end);
#endif
    char *test_buf = (char*)(APP_DTCM_ORIGIN + DTCM_RAM_OFF);
    uint32_t irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf, BUFF_LEN_48K, l2m_ptr, BUFF_LEN_48K) != 0) {
        wstp_print("[l2ram mem test]l2ram read memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = end_time - start_time;
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[l2m mem test]l2m read time:%dus bw:%dMB/s\r\n", total_time, bw);

    irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(l2m_ptr, BUFF_LEN_48K, test_buf, BUFF_LEN_48K) != 0) {
        wstp_print("[l2ram mem test]l2ram write memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = end_time - start_time;
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[l2m mem test]l2m write time:%dus bw:%dMB/s\r\n", total_time, bw);

    return ERRCODE_SUCC;
}

uint32_t mcu_test_mmc_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef CFG_DRIVERS_MMC
    char* path = "/user/test_spped.bin";
    wstp_print("emmc fs bw test start!please wait...\r\n");
    fs_test_rw_speed(path, BYTES_LEN_128);
    fs_test_rw_speed(path, BYTES_LEN_512);
    fs_test_rw_speed(path, BYTES_LEN_1K);
    fs_test_rw_speed(path, BYTES_LEN_4K);
    fs_test_rw_speed(path, BYTES_LEN_8K);
#else
    wstp_print("[emmc mem test]The current version does not support EMMC.\r\n");
#endif
    return ERRCODE_SUCC;
}

uint32_t mcu_test_psram_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint64_t start_time, end_time;
    uint32_t total_time;
    uint32_t bw;

#ifdef CONFIG_PSRAM_SUPPORT
    char *test_buf = (char*)malloc(BUFF_LEN_48K);
    if (test_buf == NULL) {
        wstp_print("[psram mem test]buf malloc fail!\r\n");
        return ERRCODE_FAIL;
    }
    char *test_buf1 = (char*)(APP_DTCM_ORIGIN + DTCM_RAM_OFF);

    uint32_t irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf1, BUFF_LEN_48K, test_buf, BUFF_LEN_48K) != 0) {
        wstp_print("[psram mem test]psram read memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time) - TCXO_ERROR;
    bw = ONE_SECOND / total_time * BUFF_LEN_48K / MB_SIZE;
    wstp_print("[psram mem test]psram read time:%dus bw:%dMB/s\r\n", total_time, bw);

    irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf, BUFF_LEN_48K, test_buf1, BUFF_LEN_48K) != 0) {
        wstp_print("[psram mem test]psram write memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time) - TCXO_ERROR;
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[psram mem test]psram write time:%dus bw:%dMB/s\r\n", total_time, bw);
    free(test_buf);
#else
    wstp_print("[psram mem test]The current version does not support PSRAM.\r\n");
#endif
    return ERRCODE_SUCC;
}

uint32_t mcu_test_norflash_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint64_t start_time, end_time;
    uint32_t total_time;
    uint32_t bw;

    char *test_buf = (char*)(APP_DTCM_ORIGIN + DTCM_RAM_OFF);
    uint32_t irq_sts = osal_irq_lock();
    start_time = uapi_tcxo_get_us();
    if (memcpy_s(test_buf, BUFF_LEN_48K, FLASH_RESERVED_ADDR, BUFF_LEN_48K) != 0) {
        wstp_print("[norflash mem test]norflash read memcpy_s fail.\r\n");
    }
    end_time = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
    total_time = (uint32_t)(end_time - start_time);
    bw = (ONE_SECOND / total_time) * BUFF_LEN_48K / (MB_SIZE);
    wstp_print("[norflash mem test]nor flash read time:%dus bw:%dMB/s\r\n", total_time, bw);

    return ERRCODE_SUCC;
}

uint32_t mcu_test_nandflash_baudwidth(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef CFG_DRIVERS_NANDFLASH
    char* path = "/user/test_spped.bin";
    wstp_print("nandflash fs bw test start!please wait...\r\n");
    fs_test_rw_speed(path, BYTES_LEN_128);
    fs_test_rw_speed(path, BYTES_LEN_512);
    fs_test_rw_speed(path, BYTES_LEN_1K);
    fs_test_rw_speed(path, BYTES_LEN_4K);
    fs_test_rw_speed(path, BYTES_LEN_8K);
#else
    wstp_print("[nandflash mem test]The current version does not support nandflash.\r\n");
#endif
    return ERRCODE_SUCC;
}

uint32_t mcu_get_fs_info(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    int i;
    struct statfs file_buf = {0};

    for (i = 0; i < FS_VOLUMES_NUM; i++) {
        /* 获取文件系统状态 */
        if (statfs(g_volume_names[i], &file_buf) != 0) {
            return ERRCODE_FAIL;
        }

        if (file_buf.f_bfree >= FS_NODE_INFO_SPACE) {
            file_buf.f_bfree = file_buf.f_bfree - FS_NODE_INFO_SPACE;
        } else {
            file_buf.f_bfree = 0;
        }

        wstp_print("[FS INFO]%s:\r\n", g_volume_names[i]);
        wstp_print("[FS INFO]total size=%uKB used size=%uKB free size=%uKB\r\n",
                ((uint32_t)(file_buf.f_blocks * file_buf.f_bsize) / KB_SIZE),
                ((uint32_t)((file_buf.f_blocks - file_buf.f_bfree) * file_buf.f_bsize) / KB_SIZE),
                ((uint32_t)(file_buf.f_bfree * file_buf.f_bsize) / KB_SIZE));
    }

    return ERRCODE_SUCC;
}

static void pm_lcd_on_pre_handle(void)
{
    display_resume_power_on();
#ifdef QSPI_DISPLAY
    writew(0x5700c058, 0x0); // sagpio_l22 set gpio mode
#else
    writew(0x5700c058, 0x1); // sagpio_l22 set te mode
#endif
    writew(0x5700c658, 0x2); // sagpio_l22 open ie
#ifndef QSPI_DISPLAY
    pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_ON);
#endif
}

uint32_t mcu_pm_lcd_on(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef SUPPORT_POWER_MANAGER
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    return display_api->turn_on_screen();
#endif
    uapi_pm_request_mode(PM_ID_VIDEO, PM_MODE_PERFORMACE);

    if (lcd_get_status()) {
        wstp_print("\r\nLCD has benn on\r\n");
        return ERRCODE_SUCC;
    }
    pm_lcd_on_pre_handle();
    lcd_bus_api *bus_api = lcd_get_bus_api();
    ext_errno ret = bus_api->bus_resume();
    if (ret != 0) {
        wstp_print("bus resume error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#ifndef QSPI_DISPLAY
    if (g_dpu_ready_flag == false) {
        ret = drv_dpu_resume(NULL);
        if (ret != 0) {
            wstp_print("drv dpu resume error ret=%x\r\n", ret);
            return ERRCODE_FAIL;
        }
    }
#endif
#ifdef SUPPORT_GPU_JPEG
    (void)drv_jpeg_resume(NULL);
#endif
    ret = drv_vau_resume(NULL);
    if (ret != 0) {
        wstp_print("drv vau resume error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#ifdef SUPPORT_GPU_GMMU
    ret = drv_gmmu_resume(NULL);
    if (ret != 0) {
        wstp_print("drv gmmu resume error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#endif
    ret = uapi_lcd_init();
    if (ret != 0 && ret != EXT_ERR_MIPI_ALREADY_INIT) {
        wstp_print("LCD init error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    NotifyScreenOn(); // Instruct the uikit to start the display operation.
#endif
    if (g_dpu_ready_flag == false) {
        touch_screen_resume(); // open touch
    }
#ifdef CONFIG_CLDO1PD_SUPPORT
    pmu_control_cldo1_sleep_bypass(TURN_ON);
#endif
    lcd_set_status(true);
    return ERRCODE_SUCC;
}
uint32_t mcu_pm_into_ship_mode(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uapi_sys_shipmode(0);
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_into_force_ds_mode(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    set_cpu_utils_reset_cause(REBOOT_CAUSE_APPLICATION_SYSRESETREQ);
    writew(0x5702C010, 0x5A5A);
    hal_reboot_chip();
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_req_mode(uint8_t *para, uint32_t para_len)
{
    unused(para_len);

    if (strcmp((char*)para, "lp") == 0) {
        errcode_t ret = uapi_pm_request_mode(g_pm_req_mode_usr, PM_MODE_LP);
        wstp_print("pm requst mode user 0x%x reuest mode lp, ret 0x%x.\r\n", g_pm_req_mode_usr, ret);
    } else if (strcmp((char*)para, "performace") == 0) {
        errcode_t ret = uapi_pm_request_mode(g_pm_req_mode_usr, PM_MODE_PERFORMACE);
        wstp_print("pm requst mode user 0x%x reuest mode perfermance, ret 0x%x.\r\n", g_pm_req_mode_usr, ret);
    } else if (strcmp((char*)para, "normal") == 0) {
        errcode_t ret = uapi_pm_request_mode(g_pm_req_mode_usr, PM_MODE_NORMAL);
        wstp_print("pm requst mode user 0x%x reuest mode normal, ret 0x%x.\r\n", g_pm_req_mode_usr, ret);
    } else if (strcmp((char*)para, "clean") == 0) {
        for (pm_id_t i = 0; i < PM_ID_MAX; i++) {
            uapi_pm_request_mode(i, PM_MODE_LP);
        }
        wstp_print("pm mode back to lp mode for all users.\r\n");
    } else {
        wstp_print("pm request mode is not supported.\r\n");
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

uint32_t mcu_set_pm_req_mode_usr(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    uint8_t pm_id = *(char*)para - '0';

    if (pm_id >= PM_ID_MAX) {
        wstp_print("pm requst mode user 0x%x check fail.\r\n", pm_id);
        return ERRCODE_FAIL;
    }
    g_pm_req_mode_usr = (pm_id_t)pm_id;

    wstp_print("pm requst mode user 0x%x reuest mode.\r\n", g_pm_req_mode_usr);
    return ERRCODE_SUCC;
}

static void pm_print_veto_info(void)
{
    uint8_t i = 0;
    pm_veto_t *pm_veto_ptr = pm_veto_get_info();
    wstp_print("last_veto_id=%d, last_veto_lr=%x, veto_timeout_timestamp=%llu, total_counts=%d\r\n",
               pm_veto_ptr->last_veto_id, pm_veto_ptr->last_veto_lr, pm_veto_ptr->veto_timeout_timestamp,
               pm_veto_ptr->veto_counts.total_counts);
    for (i = 0; i < PM_ID_MAX; i++) {
        wstp_print("sub_counts[%d] = %d\r\n", i, pm_veto_ptr->veto_counts.sub_counts[i]);
    }
}

static void lpc_print_veto_info(void)
{
    uint8_t i = 0;
    volatile lpc_veto_type *lpc_veto_ptr = lpc_get_sys_sleep_veto();
    wstp_print("g_lpc_mcpu_i2c_sleep_vetos=%d\r\n", lpc_get_mcpu_i2c_sleep_veto());
    for (i = 0; i < LPC_ID_MAX; i++) {
        wstp_print("i=%d, lpc_sleep_vetos=%d, lr_pointer=%x\r\n",
                   i, lpc_veto_ptr[i].lpc_sleep_vetos, lpc_veto_ptr[i].lr_pointer);
    }
}

uint32_t mcu_pm_dfx_mode(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uapi_get_current_pm_mode_info();
    pm_print_veto_info();
    lpc_print_veto_info();
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_cldo1_power_down(uint8_t *para, uint32_t para_len)
{
    unused(para);

    if (strcmp((char*)para, "yes") == 0) {
        pmu_control_cldo1_sleep_bypass(TURN_OFF);
        wstp_print("pm request cldo1 power down at sleep.\r\n");
    } else if (strcmp((char*)para, "no") == 0) {
        pmu_control_cldo1_sleep_bypass(TURN_ON);
        wstp_print("pm request cldo1 power on at sleep.\r\n");
    } else {
        wstp_print("pm request cldo1 para is not supported.\r\n");
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static void mcu_aes_get_password(uint32_t keyslot)
{
    errcode_t ret;
     uapi_drv_klad_attr_t test_klad_attr = {
        .klad_cfg = {
            .rootkey_type = UAPI_DRV_KDF_HARD_KEY_TYPE_ABRK_REE,
        },
        .key_cfg = {
            .engine = UAPI_DRV_KLAD_ENGINE_AES,
            .decrypt_support = true,
            .encrypt_support = true,
        },
    };
    uapi_drv_klad_effective_key_t test_klad_key = {
        .kdf_hard_alg = UAPI_DRV_KDF_HARD_ALG_SHA256,
        .key_parity = false,
        .key_size = 16,  //  16 固定的密钥长度
        .salt = (uint8_t *)g_decrypt_aes_salt,
        .salt_length = sizeof(g_decrypt_aes_salt),
    };

    uint32_t klad;
    ret = uapi_drv_klad_create(&klad);
    if (ret != ERRCODE_SUCC) {
        return;
    }

    ret = uapi_drv_klad_attach(klad, UAPI_DRV_KLAD_DEST_MCIPHER, keyslot);
    if (ret != ERRCODE_SUCC) {
        return;
    }

    ret = uapi_drv_klad_set_attr(klad, &test_klad_attr);
    if (ret != ERRCODE_SUCC) {
        (void)uapi_drv_klad_destroy(klad);
        return;
    }

    ret = uapi_drv_klad_set_effective_key(klad, &test_klad_key);
    if (ret != ERRCODE_SUCC) {
        (void)uapi_drv_klad_destroy(klad);
        return;
    }

    ret = uapi_drv_klad_detach(klad, UAPI_DRV_KLAD_DEST_MCIPHER, keyslot);
    if (ret != ERRCODE_SUCC) {
        wstp_print("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        (void)uapi_drv_klad_destroy(klad);
        return;
    }

    (void)uapi_drv_klad_destroy(klad);
}

static errcode_t mcu_test_crypto_cipher_create(uint32_t *cipher, uint32_t *keyslot)
{
    errcode_t ret;
    uapi_drv_cipher_symc_attr_t cipher_attr = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC,
        .symc_type = UAPI_DRV_CIPHER_SYMC_TYPE_NORMAL,
        .is_long_term = true
    };

    ret = uapi_drv_cipher_symc_create(cipher, &cipher_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_drv_keyslot_create(keyslot, UAPI_DRV_KEYSLOT_TYPE_MCIPHER);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    ret =  uapi_drv_cipher_symc_attach(*cipher, *keyslot);
    if (ret != ERRCODE_SUCC) {
        goto cleanup1;
    }

    return ERRCODE_SUCC;

cleanup1:
    (void)uapi_drv_keyslot_destroy(*keyslot);
cleanup0:
    (void)uapi_drv_cipher_symc_destroy(*cipher);

    return ret;
}

static errcode_t mcu_aes_cipher_process(aes_cypto_t crypt, uint8_t *src, uint8_t *dst, uint32_t len)
{
    errcode_t ret = ERRCODE_SUCC;
    uint32_t cipher = INVALID_HANDLE;
    uint32_t keyslot = INVALID_HANDLE;
    uapi_drv_cipher_buf_attr_t src_buf = { .phys_addr = src, .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE, };
    uapi_drv_cipher_buf_attr_t dst_buf = { .phys_addr = dst, .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE, };

    uapi_drv_cipher_symc_ctrl_t ctrl = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC,
        .symc_key_length = UAPI_DRV_CIPHER_SYMC_KEY_128BIT,
        .symc_bit_width = UAPI_DRV_CIPHER_SYMC_BIT_WIDTH_128BIT,
        .iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ONE_PKG,
        .iv_length = sizeof(g_decrypt_aes_iv),
    };

    (void)memcpy_s(ctrl.iv, sizeof(ctrl.iv), g_decrypt_aes_iv, sizeof(g_decrypt_aes_iv));

    ret = mcu_test_crypto_cipher_create(&cipher, &keyslot);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_drv_cipher_symc_set_config(cipher, &ctrl);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    mcu_aes_get_password(keyslot);

    if (crypt == AES_ENCRYPT_TEST) {
        ret = uapi_drv_cipher_symc_encrypt(cipher, &src_buf, &dst_buf, len);
    } else {
        ret = uapi_drv_cipher_symc_decrypt(cipher, &src_buf, &dst_buf, len);
    }

    (void)uapi_drv_cipher_symc_detach(cipher, keyslot);
    (void)uapi_drv_keyslot_destroy(keyslot);
    (void)uapi_drv_cipher_destroy(cipher);
    return ret;
}

uint32_t mcu_aes_decrypt_sample(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint32_t test_len = AES_SAMPLE_TEST_LEN;
    uint8_t *test_src_addr = (uint8_t*)malloc(sizeof(uint8_t) * AES_SAMPLE_TEST_LEN);
    if (test_src_addr == NULL) {
        return ERRCODE_FAIL;
    }
    uint8_t *test_dst_addr = (uint8_t*)malloc(sizeof(uint8_t) * AES_SAMPLE_TEST_LEN);
    if (test_dst_addr == NULL) {
        free(test_src_addr);
        return ERRCODE_FAIL;
    }

    (void)memset_s(test_src_addr, AES_SAMPLE_TEST_LEN, 0, AES_SAMPLE_TEST_LEN);
    (void)memset_s(test_dst_addr, AES_SAMPLE_TEST_LEN, 0, AES_SAMPLE_TEST_LEN);

    wstp_print("mcu aes decrypt sample start.\r\n");
    for (uint32_t i = 0; i < AES_SAMPLE_TEST_LEN; i++) {
        test_src_addr[i] = g_decrypt_aes_decryp[i];
    }
    mcu_aes_cipher_process(AES_DECRYPT_TEST, test_src_addr, test_dst_addr, AES_SAMPLE_TEST_LEN);
    wstp_print("mcu aes decrypt result.\r\n");
    for (uint32_t i = 0; i < AES_SAMPLE_TEST_LEN; i++) {
        wstp_print("i=0x%x, src=0x%02x, dst=0x%02x\r\n", i, test_src_addr[i], test_dst_addr[i]);
    }
    wstp_print("mcu aes decrypt sample end.\r\n");
    free(test_src_addr);
    free(test_dst_addr);
    return ERRCODE_SUCC;
}

uint32_t mcu_aes_encrypt_sample(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint32_t test_len = AES_SAMPLE_TEST_LEN;
    uint8_t *test_src_addr = (uint8_t*)malloc(sizeof(uint8_t) * AES_SAMPLE_TEST_LEN);
    if (test_src_addr == NULL) {
        return ERRCODE_FAIL;
    }
    uint8_t *test_dst_addr = (uint8_t*)malloc(sizeof(uint8_t) * AES_SAMPLE_TEST_LEN);
    if (test_dst_addr == NULL) {
        free(test_src_addr);
        return ERRCODE_FAIL;
    }

    (void)memset_s(test_src_addr, AES_SAMPLE_TEST_LEN, 0, AES_SAMPLE_TEST_LEN);
    (void)memset_s(test_dst_addr, AES_SAMPLE_TEST_LEN, 0, AES_SAMPLE_TEST_LEN);

    wstp_print("mcu aes encrypt sample start.\r\n");
    for (uint32_t i = 0; i < AES_SAMPLE_TEST_LEN; i++) {
        test_src_addr[i] = g_decrypt_aes_encryp[i];
    }
    mcu_aes_cipher_process(AES_ENCRYPT_TEST, test_src_addr, test_dst_addr, AES_SAMPLE_TEST_LEN);
    wstp_print("mcu aes encrypt result.\r\n");
    for (uint32_t i = 0; i < AES_SAMPLE_TEST_LEN; i++) {
        wstp_print("i=0x%x, src=0x%02x, dst=0x%02x\r\n", i, test_src_addr[i], test_dst_addr[i]);
    }
    wstp_print("mcu aes encrypt sample end.\r\n");
    free(test_src_addr);
    free(test_dst_addr);
    return ERRCODE_SUCC;
}

uint32_t mcu_aes_set_gid(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint8_t i;
    uint8_t security_key = 0;
    // After security_Key is set, GID cannot be configured and read.
    (void)uapi_efuse_read_bit(&security_key, OTP_DEBUG_FUSED, OTP_DEBUG_FUSED_SECURIY_KEY_EN_ACCESS_BIT);
    if (security_key == 0x1) {
        wstp_print("gid forbit read, because security_key is closed.\r\n");
        return ERRCODE_FAIL;
    }

    uint8_t *test_efuse_gid = (uint8_t*)malloc(sizeof(uint8_t) * OTP_SECURITY_KEY_GID_LEN);
    if (test_efuse_gid == NULL) {
        return ERRCODE_FAIL;
    }
    (void)memset_s(test_efuse_gid, OTP_SECURITY_KEY_GID_LEN, 0, OTP_SECURITY_KEY_GID_LEN);
    (void)uapi_efuse_read_buffer(test_efuse_gid, OTP_SECURITY_KEY_GID, OTP_SECURITY_KEY_GID_LEN);
    for (i = 0; i < OTP_SECURITY_KEY_GID_LEN; i++) {
        wstp_print("current gid[0x%02x] val[0x%02x]\r\n", i, test_efuse_gid[i]);
    }
    wstp_print("start write into efuse gid.\r\n");
    (void)uapi_efuse_write_buffer(OTP_SECURITY_KEY_GID, g_decrypt_aes_gid, OTP_SECURITY_KEY_GID_LEN);
    wstp_print("write into efuse gid completely.\r\n");
    (void)uapi_efuse_read_buffer(test_efuse_gid, OTP_SECURITY_KEY_GID, OTP_SECURITY_KEY_GID_LEN);
    for (i = 0; i < OTP_SECURITY_KEY_GID_LEN; i++) {
        wstp_print("write into gid[0x%02x] val[0x%02x]\r\n", i, test_efuse_gid[i]);
    }
    (void)uapi_efuse_write_bit(OTP_DEBUG_FUSED, OTP_DEBUG_FUSED_SECURIY_KEY_EN_ACCESS_BIT);
    wstp_print("gid read locked.\r\n");
    free(test_efuse_gid);
    return ERRCODE_SUCC;
}

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
#define ADC_SAMPLE_TIMES 10
#define ADC_SAMPLE_DATA_LEN 10010
uint32_t g_adc_sample_times = 0;
uint32_t g_save_adc_index = 0;
uint32_t *g_save_adc_data = NULL;
static uint32_t adc_auto_sample_show_data(uint8_t *para, uint32_t para_len)
{
    wstp_print("g_save_adc_index = %d\r\n", g_save_adc_index);
    for (uint32_t i = 0; i < g_save_adc_index; i++) {
        wstp_print("%d\r\n", g_save_adc_data[i]);
    }

    g_save_adc_index = 0;
    g_adc_sample_times = 0;
    free(g_save_adc_data);
    g_save_adc_data = NULL;
    return ERRCODE_SUCC;
}

static void adc_autosample_callback(uint8_t channel, uint32_t *buffer, uint32_t length, bool *next)
{
    unused(next);
    if (g_adc_sample_times >= ADC_SAMPLE_TIMES) {
        wstp_print("g_adc_sample_times = %d\r\n", g_adc_sample_times);
        uapi_adc_auto_scan_ch_disable(channel);
        return;
    }

    memcpy_s(g_save_adc_data + g_save_adc_index, length * sizeof(uint32_t), buffer, length * sizeof(uint32_t));
    g_save_adc_index += length;
    g_adc_sample_times++;
    wstp_print("length = %d\r\n", length);
    return;
}

static uint32_t adc_auto_sample_by_diag(uint8_t *para, uint32_t para_len)
{
    char *ptr = (char *)para;
    char *token = NULL;
    char *next_token = NULL;
    uint32_t channel = 0;
    uint32_t freq = 0;
    uint32_t sample_time = 0;
    uint32_t idx = 0;
    uint32_t tmp[ADC_AUTODATA_PRT_INDEX3] = {0};
    token = strtok_s((char*)ptr, ",", &next_token);
    while (token != NULL) {
        tmp[idx++] = strtoul(token, NULL, 10); // 10: 十进制
        token = strtok_s(NULL, ",", &next_token);
    }
    channel = tmp[0];
    freq = tmp[ADC_AUTODATA_PRT_INDEX1];
    sample_time = tmp[ADC_AUTODATA_PRT_INDEX2];

    adc_scan_config_t config = {0};
    config.freq = (uint8_t)freq;
    config.long_sample_time = sample_time;

    g_save_adc_data = (uint32_t *)malloc(sizeof(uint32_t) * ADC_SAMPLE_DATA_LEN);
    if (g_save_adc_data == NULL) {
        wstp_print("malloc g_save_adc_data fail!\r\n");
        return ERRCODE_FAIL;
    }
    (void)uapi_adc_init(ADC_CLOCK_500KHZ);
    (void)uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, true);
    return(uapi_adc_auto_scan_ch_enable((uint8_t)channel, config, adc_autosample_callback));
}

static uint32_t adc_auto_sample_disable(uint8_t *para, uint32_t para_len)
{
    char *ptr = (char *)para;
    uint32_t ret = 0;
    uint32_t channel = strtoul(ptr, NULL, 10); // 10: 十进制
    wstp_print("disable adc auto sample , channel = %x\r\n", channel);

    return(uapi_adc_auto_scan_ch_disable(channel));
}
#endif

static void pm_lcd_off_handle(void)
{
    writew(0x570000c0, 0x0);   // display logic crg reset
    writew(0x57004034, 0x100); // display close power
#ifndef QSPI_DISPLAY
    pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_OFF);
    hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO3, TURN_ON);
#endif
#ifdef CONFIG_CLDO1PD_SUPPORT
    pmu_control_cldo1_sleep_bypass(TURN_OFF);
#endif
}

uint32_t mcu_pm_lcd_off(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
#ifdef SUPPORT_POWER_MANAGER
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    return display_api->turn_off_screen();
#endif
    if (!lcd_get_status()) {
        wstp_print("\r\nLCD has benn off\r\n");
        return ERRCODE_SUCC;
    }
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    NotifyScreenOff(); // Instruct the uikit to close the display operation.
#endif
    osDelay(100);        // Wait 100ms for the uikit to complete display.
    touch_screen_suspend(); // close touch
    ext_errno ret = drv_vau_suspend(NULL);
    if (ret != 0) {
        wstp_print("drv vau suspend error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#ifdef SUPPORT_GPU_JPEG
    (void)drv_jpeg_suspend(NULL);
#endif
#ifndef QSPI_DISPLAY
    ret = drv_dpu_suspend(NULL);
    if (ret != 0) {
        wstp_print("drv dpu suspend error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#endif
#ifdef SUPPORT_GPU_GMMU
    ret = drv_gmmu_suspend(NULL);
    if (ret != 0) {
        wstp_print("drv gmmu suspend error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
#endif
    ret = uapi_lcd_deinit();
    if (ret != 0) {
        wstp_print("lcd deinit error ret=%x\r\n", ret);
        return ERRCODE_FAIL;
    }
    pm_lcd_off_handle();
    lcd_set_status(false);
    g_dpu_ready_flag = false;
    display_suspend_power_off();
    uapi_pm_request_mode(PM_ID_VIDEO, PM_MODE_LP);
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_add_vote(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint8_t cnt = pm_veto_get_id_cnt(PM_ID_DEBUG);
    if (cnt == 0) {
        uapi_pm_add_sleep_veto(PM_ID_DEBUG);
    } else {
        wstp_print("MCU has got pm_id_debug vote\r\n");
    }
    wstp_print("\r\nMCU WAKEUP SUCCESS\r\n");
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_remove_vote(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint8_t cnt = pm_veto_get_id_cnt(PM_ID_DEBUG);
    if (cnt == 1) {
        uapi_pm_remove_sleep_veto(PM_ID_DEBUG);
    } else {
        wstp_print("MCU has delete pm_id_debug vote\r\n");
    }
    wstp_print("\r\nMCU SLEEP SUCCESS\r\n");
    uapi_tcxo_delay_ms(5); // delay 5ms for print debug infor with uart
    return ERRCODE_SUCC;
}

uint32_t mcu_dfx_get_cur_second(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uint32_t cur_second = dfx_get_cur_second();

    wstp_print("cur_second = %d\r\n", cur_second);
    return ERRCODE_SUCC;
}

uint32_t get_ids(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    uint16_t nandflash_id;
    uapi_nandflash_read_id(&nandflash_id);
    wstp_print("[nandflash id:] %x\r\n", nandflash_id);
#endif
    flash_info_t norflash_id;
    uapi_flash_get_info(0, &norflash_id);
    wstp_print("[norflash id:] %x\r\n", norflash_id.flash_id);
    wstp_print("[sdk id:]%s \r\n", uapi_sdk_read_id());
    uint8_t soc_id[20]; // 20 is soc id length
    uapi_soc_read_id(soc_id, 20); // 20 is soc id length
    wstp_print("[soc id:]\r\n");
    for (uint8_t i = 0; i < 20; i += 2) { // 20 is soc id length, 2 means use 16bits
        wstp_print("0x%04X  ", *((uint16_t *)&soc_id[i]));
        if ((i + 2) % 8 == 0) { wstp_print("\r\n"); } // 2&8 will word wrap
    }
    wstp_print("\r\n");
    return ERRCODE_SUCC;
}

uint32_t lcd_test_light(uint8_t *data, uint32_t data_len)
{
    char *ptr = (char *)data;
    uint32_t light_input = strtoul(ptr, NULL, 16);
    wstp_print("light = %x \r\n", light_input);
    uapi_lcd_set_brightness(light_input);
    return ERRCODE_SUCC;
}

uint32_t at_nand_write_read(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t page = strtoul(ptr, NULL, 10);
    wstp_print("page = %d \r\n", page);
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    if (page >= freertos_nandflash_driver->pages_per_block * freertos_nandflash_driver->total_block) {
        wstp_print("page error exceed max page = %d \r\n", page);
        return ERRCODE_FAIL;
    }
    test_nand_flash_read_8(freertos_nandflash_driver, page);
#endif
    return ERRCODE_SUCC;
}

uint32_t at_nand_write_write(uint8_t *data, uint32_t data_len)
{
    PRINT("nand wite\r\n");
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t page = strtoul(ptr, NULL, 10);
    wstp_print("page = %d \r\n", page);
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    if (page >= freertos_nandflash_driver->pages_per_block * freertos_nandflash_driver->total_block) {
        wstp_print("page error exceed max page = %d \r\n", page);
        return ERRCODE_FAIL;
    }
    test_nand_flash_write_width8(freertos_nandflash_driver, page);
#endif
    return ERRCODE_SUCC;
}

uint32_t at_nand_write_erase(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t block = strtoul(ptr, NULL, 10);
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    wstp_print("block = %d total_block = %d\r\n", block, freertos_nandflash_driver->total_block);
    if (block >= freertos_nandflash_driver->total_block) {
        erase_all(freertos_nandflash_driver);
        wstp_print("erase all block\r\n");
        return ERRCODE_SUCC;
    }
    freertos_nandflash_driver->erase_block(block);
#endif
    return ERRCODE_SUCC;
}

uint32_t at_nand_mark_bad(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t block = strtoul(ptr, NULL, 10);
    wstp_print("mark block%d = %x  is bad\r\n", block);
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    if (block >= freertos_nandflash_driver->total_block) {
        wstp_print("exceed max block = %d \r\n", block);
        return ERRCODE_FAIL;
    }
    freertos_nandflash_driver->mark_bad_block(block);
#endif
    return ERRCODE_SUCC;
}

uint32_t at_nand_check_bad(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t block = strtoul(ptr, NULL, 10);
    wstp_print("check block%d = %x  is bad?\r\n", block);
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    if (block >= freertos_nandflash_driver->total_block) {
        wstp_print("exceed max block = %d \r\n", block);
        return ERRCODE_FAIL;
    }
    uint32_t ret = freertos_nandflash_driver->check_bad_block(block);
    wstp_print("is bad? ret = %d\r\n", ret);
#endif
    return ERRCODE_SUCC;
}

uint32_t at_nand_set_mode(uint8_t *data, uint32_t data_len)
{
#ifdef CFG_DRIVERS_NANDFLASH
    char *ptr = (char *)data;
    uint32_t mode = strtoul(ptr, NULL, 10);
    if (mode == 0) {
        wstp_print("set mode to SPEED_HIGH DMA_FOUR_LINE\r\n", mode);
        nand_driver_init(SPEED_HIGH);
        set_trans_type(TRANS_BY_DMA_QUAD);
    } else {
        wstp_print("set mode to SPEED_SLOW CPU_SINGLE_LINE\r\n", mode);
        nand_driver_init(SPEED_SLOW);
        set_trans_type(TRANS_BY_CPU_SINGLE_LINE);
    }
#endif
    return ERRCODE_SUCC;
}

uint32_t lcd_fill(uint8_t *data, uint32_t data_len)
{
    char *ptr = (char *)data;
    uint32_t color_input = strtoul(ptr, NULL, 16);
    wstp_print("color = %x \r\n", color_input);

    td_handle vau_handle;
    ext_vau_surface src_surface = {0};

    src_surface.width            = 454; // 454 screen width
    src_surface.height           = 454; // 454 screen height
    src_surface.phy_addr         = LCD_FILL_COLOR_ADDR;
    src_surface.vir_addr         = (td_uchar *)src_surface.phy_addr;
    src_surface.color_fmt        = EXT_GFX_FMT_ARGB8888;
    src_surface.compress_mode    = EXT_GFX_COMPRESS_NONE;
    src_surface.argb_order       = EXT_GFX_FMT_ORDER_ARGB;
    src_surface.rect.x           = 0;
    src_surface.rect.y           = 0;
    src_surface.rect.width       = src_surface.width;
    src_surface.rect.height      = src_surface.height;
    src_surface.stride           = 1824; /* 454 * 32 / 8 = 1816 16字节对齐得1824 */
    src_surface.is_color_surface = TD_TRUE;
    src_surface.color            = color_input;

#ifdef MIPI_ULPS_SUPPORT
    uapi_mipi_tx_exit_ulps();
#endif

    vau_handle = uapi_vau_create();
    uapi_vau_fill(vau_handle, &src_surface);
    uapi_vau_submit(vau_handle, TD_FALSE, 1000); /* 1000 ms timeout */

    td_handle dpu_handle;
    ext_dpu_layer_info layer_info = {0};
    ext_dpu_surface dpu_surface = {0};

    layer_info.width          = src_surface.width;
    layer_info.height         = src_surface.height;
    layer_info.layer_id       = EXT_DPU_LAYER_ID1;
    layer_info.layer_fmt      = src_surface.color_fmt;
    layer_info.compress_mode  = src_surface.compress_mode;
    layer_info.buf_mode       = EXT_DPU_BUFFER_MODE_NORMAL;

    dpu_surface.width         = src_surface.width;
    dpu_surface.height        = src_surface.height;
    dpu_surface.stride        = src_surface.stride;
    dpu_surface.phy_addr      = src_surface.phy_addr;
    dpu_surface.vir_addr      = src_surface.vir_addr;
    dpu_surface.update_rect   = TD_NULL;
    dpu_surface.color_fmt     = src_surface.color_fmt;
    dpu_surface.compress_mode = src_surface.compress_mode;

    uapi_dpu_create_layer(&layer_info, &dpu_handle);
    uapi_dpu_set_layer_vblank(dpu_handle, TD_TRUE);
    uapi_dpu_set_layer_info(dpu_handle, &dpu_surface);
    uapi_dpu_refresh_layers();

    return ERRCODE_SUCC;
}

uint32_t mcu_pm_no_resume_touch(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *data = (char *)para;
    if (data == NULL) { return ERRCODE_FAIL; }
    uint8_t flag = strtoul(data, NULL, 0);
    if (flag == 1) {
        touch_screen_restart();
    }
    set_touch_resume_flag(flag);
    return ERRCODE_SUCC;
}

uint32_t mcu_pm_set_mcu_dynamic_power(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *data = (char *)para;
    if (data == NULL) { return ERRCODE_FAIL; }
    uint8_t flag = strtoul(data, NULL, 0);
    if (flag == 0) {
        pm_set_mcu_dynamic_power_flag(0);
    } else {
        pm_set_mcu_dynamic_power_flag(1);
    }
    return ERRCODE_SUCC;
}

static void button_callback_press(void)
{
    PRINT("power key press."NEWLINE);
}

static void button_callback_click(void)
{
    PRINT("power key click."NEWLINE);
}

static void button_callback_click_2(void)
{
    PRINT("power key click double."NEWLINE);
}

static void button_callback1(void)
{
    PRINT("power key long press 1s."NEWLINE);
}

static void button_callback2(void)
{
    PRINT("power key long press 2s."NEWLINE);
}

static void button_callback3(void)
{
    PRINT("power key long press 3s."NEWLINE);
}

static void button_callback4(void)
{
    PRINT("power key long press 4s."NEWLINE);
}

static void button_callback5(void)
{
    PRINT("power key long press 5s."NEWLINE);
}

static void button_callback6(void)
{
    PRINT("power key long press 6s."NEWLINE);
}

static void button_callback7(void)
{
    PRINT("power key long press 7s."NEWLINE);
}

static void button_callback8(void)
{
    PRINT("power key long press 8s."NEWLINE);
}

static void button_callback9(void)
{
    PRINT("power key long press 9s."NEWLINE);
}

static void button_callback_1_over(void)
{
    PRINT("power key long press 1s over."NEWLINE);
}

static void button_callback_2_over(void)
{
    PRINT("power key long press 2s over."NEWLINE);
}

static void button_callback_3_over(void)
{
    PRINT("power key long press 3s over."NEWLINE);
}

static void button_callback_4_over(void)
{
    PRINT("power key long press 4s over."NEWLINE);
}

static void button_callback_5_over(void)
{
    PRINT("power key long press 5s over."NEWLINE);
}

static void button_callback_6_over(void)
{
    PRINT("power key long press 6s over."NEWLINE);
}

static void button_callback_7_over(void)
{
    PRINT("power key long press 7s over."NEWLINE);
}

static void button_callback_8_over(void)
{
    PRINT("power key long press 8s over."NEWLINE);
}

static void button_callback_9_over(void)
{
    PRINT("power key long press 9s over."NEWLINE);
}

uint32_t mcu_button_sample(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);

    uapi_button_register_event_cb(button_callback_press, BUTTON_PWR_PRESS);
    uapi_button_register_event_cb(button_callback_click, BUTTON_PWR_CLICKUP);
    uapi_button_register_event_cb(button_callback_click_2, BUTTON_PWR_CLICKUP_2);

    uapi_button_register_event_cb(button_callback1, BUTTON_PWR_LONGPRESS_1S);
    uapi_button_register_event_cb(button_callback2, BUTTON_PWR_LONGPRESS_2S);
    uapi_button_register_event_cb(button_callback3, BUTTON_PWR_LONGPRESS_3S);
    uapi_button_register_event_cb(button_callback4, BUTTON_PWR_LONGPRESS_4S);
    uapi_button_register_event_cb(button_callback5, BUTTON_PWR_LONGPRESS_5S);
    uapi_button_register_event_cb(button_callback6, BUTTON_PWR_LONGPRESS_6S);
    uapi_button_register_event_cb(button_callback7, BUTTON_PWR_LONGPRESS_7S);
    uapi_button_register_event_cb(button_callback8, BUTTON_PWR_LONGPRESS_8S);
    uapi_button_register_event_cb(button_callback9, BUTTON_PWR_LONGPRESS_9S);

    uapi_button_register_event_cb(button_callback_1_over, BUTTON_PWR_LONGPRESS_1S_OVER);
    uapi_button_register_event_cb(button_callback_2_over, BUTTON_PWR_LONGPRESS_2S_OVER);
    uapi_button_register_event_cb(button_callback_3_over, BUTTON_PWR_LONGPRESS_3S_OVER);
    uapi_button_register_event_cb(button_callback_4_over, BUTTON_PWR_LONGPRESS_4S_OVER);
    uapi_button_register_event_cb(button_callback_5_over, BUTTON_PWR_LONGPRESS_5S_OVER);
    uapi_button_register_event_cb(button_callback_6_over, BUTTON_PWR_LONGPRESS_6S_OVER);
    uapi_button_register_event_cb(button_callback_7_over, BUTTON_PWR_LONGPRESS_7S_OVER);
    uapi_button_register_event_cb(button_callback_8_over, BUTTON_PWR_LONGPRESS_8S_OVER);
    uapi_button_register_event_cb(button_callback_9_over, BUTTON_PWR_LONGPRESS_9S_OVER);
    return ERRCODE_SUCC;
}

static uint32_t mcu_power_key_config(uint8_t *para, uint32_t para_len)
{
    char *ptr = (char *)para;
    char *token = NULL;
    char *next_token = NULL;
    uint32_t power_level = 0;
    uint32_t power_type = 0;
    uint32_t idx = 0;
    uint32_t tmp[POWER_KEY_PARAM_LEN] = {0};
    token = strtok_s((char*)ptr, " ", &next_token);
    while (token != NULL) {
        tmp[idx++] = strtoul(token, NULL, 10); // 10: 十进制
        token = strtok_s(NULL, " ", &next_token);
    }
    power_type = tmp[0];
    power_level = tmp[1];
    printf("power type:%d    power_level:%d \r\n", power_type, power_level);
    if (uapi_power_set_mode(power_type) != ERRCODE_SUCC) {
        printf("power type:%d error.", power_type);
    }
    if (uapi_power_time_mode(power_level) != ERRCODE_SUCC) {
        printf("power level:%d error.", power_level);
    }

    return ERRCODE_SUCC;
}

static uint32_t mcu_calendar_set_date(uint8_t *para, uint32_t para_len)
{
    char *ptr = (char *)para;
    char *token = NULL;
    char *next_token = NULL;
    calendar_t date;
    uint32_t idx = 0;
    uint32_t tmp[CALENDAR_PARAM_LEN] = {0};
    token = strtok_s((char*)ptr, " ", &next_token);
    while (token != NULL) {
        tmp[idx++] = strtoul(token, NULL, 10); // 10: 十进制
        token = strtok_s(NULL, " ", &next_token);
    }
    date.year = tmp[0];
    date.mon = tmp[1];
    date.day = tmp[2];
    date.hour = tmp[3];
    date.min = tmp[4];
    date.sec = tmp[5];

    errcode_t ret = uapi_calendar_set_datetime(&date);
    if (ret == ERRCODE_SUCC) {
        printf("Set datetime success: %d-%d-%d %d:%d:%d\r\n", date.year, date.mon, date.day, date.hour, date.min,
            date.sec);
        return ERRCODE_SUCC;
    }
    printf("Set datetime failed, error code = %d \r\n", ret);
    return ret;
}

static uint32_t mcu_calendar_get_date(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    calendar_t date;

    errcode_t ret = uapi_calendar_get_datetime(&date);
    if (ret == ERRCODE_SUCC) {
        printf("Get datetime success: %d-%d-%d %d:%d:%d\r\n", date.year, date.mon, date.day, date.hour, date.min,
            date.sec);
        return ERRCODE_SUCC;
    }
    printf("Get datetime failed, error code = %d \r\n", ret);

    return ret;
}

#ifdef SUPPROT_EXECUTE_CORMARK
static uint32_t mcu_coremark_test(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    uint32_t irq_sts = osal_irq_lock();
    watchdog_turnoff_clk();
    coremark_test();
    watchdog_turnon_clk();
    osal_irq_restore(irq_sts);
    return ERRCODE_SUCC;
}
#endif

static uint32_t mcu_set_app_delay_time(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint32_t delay_time = strtoul(ptr, NULL, 0);
    if (delay_time == 0) {
        delay_time = 10000; // 10000 ms
    }
    wstp_print("app set delay_time = %d\r\n", delay_time);
    set_app_delay_time(delay_time);
    return ERRCODE_SUCC;
}

static uint32_t mcu_set_cldo1_voltage(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint8_t cldo = strtoul(ptr, NULL, 0);
    if (cldo > PMU_LDO_MAX) { cldo = 0; }
    ptr++;
    ptr++;
    uint8_t voltage = strtoul(ptr, NULL, 0);
    if (voltage > 15) { voltage = 14; } // the max voltage of cldo is 15, 14 is the default value.
    wstp_print("app set cldo=%d voltage = %d\r\n", cldo, voltage);
    pmu_ldo_power_on((pmu_ldo_t)cldo, 1);
    if (((pmu_ldo_t)cldo == PMU_LDO_CLDO1) || ((pmu_ldo_t)cldo == PMU_LDO_CLDO1) ||
        ((pmu_ldo_t)cldo == PMU_LDO_CLDO1)) {
        hal_pmu_cldo_enable_bypass((hal_pmu_cldo_t)cldo, 0);
    }
    pmu_ldo_power_vset((pmu_ldo_t)cldo, voltage);
    return ERRCODE_SUCC;
}

static uint32_t mcu_nand_aging_test(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
#ifdef CFG_DRIVERS_NANDFLASH
    return nandflash_aging_test((char*)para);
#else
    return ERRCODE_SUCC;
#endif
}

static uint32_t mcu_norflash_aging_test(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    return flash_aging_test((char*)para);
}

static uint32_t mcu_set_pin_pull(uint8_t *para, uint32_t para_len)
{
    int ret;
    uint32_t pin = 0;
    uint32_t levl = 0;
    char* ptr = (char*)para;
    char* pos = strchr((char*)ptr, ',');
    if (pos == NULL) {
        return ERRCODE_FAIL;
    }
    *pos = '\0';

    pin = strtoul(ptr, NULL, 0);
    if (pin < 0) {
        pin = 0;
    }
    ptr = pos + 1;
    levl = strtoul(ptr, NULL, 0);
    if ((pin_pull_t)levl > PIN_PULL_MAX) {
        levl = 0;
    }
    uapi_pin_set_pull((pin_t)pin, (pin_pull_t)levl);
    return ERRCODE_SUCC;
}

static uint32_t mcu_get_pin_pull(uint8_t *para, uint32_t para_len)
{
    pin_t pin = 0;
    pin_pull_t level = 0;
    wstp_print("all pin levl:\r\n");
    for (uint16_t i = 0; i < PIN_NONE; i++) {
        level = uapi_pin_get_pull((pin_t)i);
        wstp_print("i=%d level=%d\r\n", i, level);
    }
    return ERRCODE_SUCC;
}

static uint32_t mcu_set_time(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    return set_system_time((char*)para);
}

static uint32_t mcu_get_time(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    return get_system_time();
}

#ifdef SUPPORT_POWER_MANAGER
static uint32_t mcu_pms_set_screenoff_time(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    uint32_t type;
    uint32_t timeout;
    char *pos;
    char *ptr = (char *)para;
    errcode_t ret = ERRCODE_SUCC;
    pos = strchr((char *)ptr, ',');
    if (pos == NULL) {
        return ERRCODE_FAIL;
    }
    *pos = '\0';
    type = strtoul(ptr, NULL, 0);
    pos++;
    timeout = strtoul(pos, NULL, 0);
    wstp_print("app set screen off: type = %u, timeut = %u.\r\n", type, timeout);
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    if (type == 0) {
        ret = display_api->set_screen_auto_off_timeout(timeout);
    } else if (type == 1) {
        ret = display_api->set_screen_set_keepon_timeout(timeout);
    } else {
        return ERRCODE_FAIL;
    }
    if (ret != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}


static uint32_t mcu_pms_set_screenoff_enable(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint32_t inputpara = strtoul(ptr, NULL, 0);
    bool enable = true;
    if (inputpara == 0) {
        enable = false;
    }
    wstp_print("app set screen off enable = %u, para = %u. \r\n", enable, inputpara);
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    display_api->set_auto_timeout_function(enable);
    return ERRCODE_SUCC;
}

static uint32_t mcu_pms_set_screen_state(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    wstp_print("app set screen state :%s. \r\n", ptr);
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    if (strcmp(ptr, "ON") == 0) {
        display_api->turn_on_screen();
    } else if (strcmp(ptr, "OFF") == 0) {
        display_api->turn_off_screen();
    } else {
        wstp_print("app set screen input para err.\r\n");
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static uint32_t mcu_pms_get_screen_state(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    screen_state_t state = display_api->get_screen_state();
    wstp_print("app get screen state = %u, on is %u, off is %u. \r\n", state, SCREEN_ON, SCREEN_OFF);
    return ERRCODE_SUCC;
}
#endif

#ifdef FT_DONGLE
uint32_t mcu_check_dongle_connected(uint8_t *para, uint32_t para_len)
{
    unused(para);
    unused(para_len);
    // 通过diag返回包作为factory_tools判断SPP的连接状态依据
    // 仅供factory_tools做判断用，此AT命令不对外用
    wstp_print("Received dongle check msg.\r\n");
    return ERRCODE_SUCC;
}
#endif

uint32_t mcu_i2c_write_byte(i2c_bus_t bus, uint16_t slave_addr, uint32_t reg_addr, uint32_t reg_val)
{
    errcode_t ret;
    i2c_data_t data;

    uint8_t send_buf[AT_I2C_REG_ARRAY_LEN] = { 0 };
    send_buf[0] = (reg_addr >> OFFSET_24) & 0xff;
    send_buf[1] = (reg_addr >> OFFSET_16) & 0xff;
    send_buf[AT_PARA_IDX2] = (reg_addr >> OFFSET_8) & 0xff;
    send_buf[AT_PARA_IDX3] = reg_addr & 0xff;
    send_buf[AT_PARA_IDX4] = reg_val & 0xff;
    send_buf[AT_PARA_IDX5] = (reg_val >> OFFSET_8) & 0xff;
    send_buf[AT_PARA_IDX6] = (reg_val >> OFFSET_16) & 0xff;
    send_buf[AT_PARA_IDX7] = (reg_val >> OFFSET_24) & 0xff;

    data.send_buf = send_buf;
    data.send_len = 8; // 8: 发送长度
    data.receive_buf = NULL;
    data.receive_len = 0;

    ret = uapi_i2c_master_write(bus, slave_addr, &data);
    if (ret != ERRCODE_SUCC) {
        wstp_print("===== Error: mcu_i2c_write_byte: I2C write ret = 0x%x! =====\r\n", ret);
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

int32_t mcu_i2c_read_byte(i2c_bus_t bus, uint16_t slave_addr, uint32_t read_addr, uint32_t *read_val)
{
    errcode_t ret;
    i2c_data_t data;

    uint8_t reg_addr[4] = { 0 };
    reg_addr[0] = (read_addr >> OFFSET_24) & 0xff;
    reg_addr[1] = (read_addr >> OFFSET_16) & 0xff;
    reg_addr[AT_PARA_IDX2] = (read_addr >> OFFSET_8) & 0xff;
    reg_addr[AT_PARA_IDX3] = read_addr & 0xff;

    data.send_buf = reg_addr;
    data.send_len = 4; // 4: 发送长度
    data.receive_buf = (uint8_t *)read_val;
    data.receive_len = 4; // 4: 接收长度

    ret = uapi_i2c_master_write(bus, slave_addr, &data);
    if (ret != ERRCODE_SUCC) {
        wstp_print("===== Error: mcu_i2c_read_byte: I2C write ret = 0x%x! =====\r\n", ret);
        return ERRCODE_FAIL;
    }

    ret = uapi_i2c_master_read(bus, slave_addr, &data);
    if (ret != ERRCODE_SUCC) {
        wstp_print("===== Error: mcu_i2c_read_byte: I2C read ret = 0x%x! =====\r\n", ret);
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

static uint32_t mcu_i2c_write(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    char *token = NULL;
    char *next_token = NULL;
    errcode_t ret;

    uint32_t idx = 0;
    uint32_t bus = 0;
    uint32_t slave_addr = 0;
    uint32_t write_addr = 0;
    uint32_t write_val = 0;

    uint32_t tmp[AT_I2C_PARA_CNT] = {0};
    token = strtok_s((char*)ptr, ",", &next_token);
    while (token != NULL) {
        tmp[idx++] = strtoul(token, NULL, 16); // 16: 十六进制
        token = strtok_s(NULL, ",", &next_token);
    }

    bus = tmp[0];
    slave_addr = tmp[1];
    write_addr = tmp[AT_PARA_IDX2];
    write_val = tmp[AT_PARA_IDX3];

    return mcu_i2c_write_byte((i2c_bus_t)bus, slave_addr, write_addr, write_val);
}

static uint32_t mcu_i2c_read(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    char *ptr = (char *)para;
    char *token = NULL;
    char *next_token = NULL;
    errcode_t ret;
    uint32_t read_val;

    uint32_t idx = 0;
    uint32_t bus = 0;
    uint32_t slave_addr = 0;
    uint32_t read_addr = 0;

    uint32_t tmp[AT_I2C_PARA_CNT] = {0};
    token = strtok_s((char*)ptr, ",", &next_token);
    while (token != NULL) {
        tmp[idx++] = strtoul(token, NULL, 16); // 16: 十六进制
        token = strtok_s(NULL, ",", &next_token);
    }

    bus = tmp[0];
    slave_addr = tmp[1];
    read_addr = tmp[AT_PARA_IDX2];

    ret = mcu_i2c_read_byte((i2c_bus_t)bus, slave_addr, read_addr, &read_val);
    if (ret == ERRCODE_SUCC) {
        wstp_print("Read val: [ 0x%x ]\r\n", read_val);
    }

    return ret;
}
#ifdef SUPPORT_ALIPAY_SEC
static uint32_t mcu_alipay_sec_test(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    wstp_print("mcu_alipay_sec_test start.\r\n");
    uapi_alipay_svr_init();
    const alipay_svr_api_t *alipay_api = alipay_svr_get_api();
    alipay_api->exit_lpm();
    osDelay(ALIPAY_EXIT_LPM_DELAY_LEN);
    yunit_sdk_test();
    alipay_api->enter_lpm();
    wstp_print("mcu_alipay_sec_test end.\r\n");

    return ERRCODE_SUCC;
}

static uint32_t mcu_alipay_debug(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    uapi_alipay_svr_init();
    svr_alipay_debug();
    return ERRCODE_SUCC;
}

static uint32_t mcu_alipay_pl_verify(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    int32_t compat_ret = 0;
    int32_t cert_ret = 0;
    uapi_alipay_svr_init();
    const alipay_svr_api_t *alipay_api = alipay_svr_get_api();
    alipay_api->exit_lpm();
    osDelay(ALIPAY_EXIT_LPM_DELAY_LEN);
    cert_ret = test_get_credential();
    wstp_print("get cert ret=%d.\r\n", cert_ret);
    compat_ret = hal_hardware_compat_check();
    wstp_print("compat check ret=%d.\r\n", compat_ret);
    alipay_api->enter_lpm();
    if (cert_ret == 0 && compat_ret == 0) {
        return ERRCODE_SUCC;
    }
    return ERRCODE_FAIL;
}

static uint32_t mcu_alipay_reset(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    (void)uapi_alipay_svr_reset();
    return ERRCODE_SUCC;
}
#endif

#ifdef SUPPORT_FILE_SPLIT_COMBINE
static uint32_t mcu_file_spilt_test(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    wstp_print("mcu_file_spilt_test start.\r\n");
    msg_center_split_file(ORIGIN_FILE_PATH, FILE_SPILT_MAX_SIZE, FILE_LIST_PATH);
    wstp_print("mcu_file_spilt_test end.\r\n");

    return ERRCODE_SUCC;
}

static uint32_t mcu_file_combine_test(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    wstp_print("mcu_file_combine_test start.\r\n");
    msg_center_combine_file(FILE_LIST_PATH, COMBINE_FILE_PATH);
    wstp_print("mcu_file_combine_test end.\r\n");

    return ERRCODE_SUCC;
}
#endif

#ifdef JS_ENABLE
static uint32_t mcu_get_trans_channel_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    unused(para);
    msg_center_get_trans_channel();

    return ERRCODE_SUCC;
}

static uint32_t mcu_set_trans_channel_test(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    char *ptr = (char *)para;
    uint32_t select = strtoul(ptr, NULL, 10); /* 10进制 */
    msg_center_set_trans_channel(select);

    return ERRCODE_SUCC;
}
#endif

#ifdef SUPPORT_AUTO_OTA
static uint32_t mcu_auto_ota_block(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    unused(para);
     g_auto_ota_upload_mode = 1;// 阻塞等待升级包上传
    wstp_print("auto_ota block!\r\n");
    return ERRCODE_SUCC;
}

static uint32_t mcu_auto_ota_run(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    unused(para);
     g_auto_ota_upload_mode = 0; // 升级包上传完成后auto_ota继续执行
    wstp_print("auto_ota run!\r\n");
    return ERRCODE_SUCC;
}

static uint32_t mcu_auto_ota_del_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    wstp_print("para = %s\r\n", para);
    uint8_t addr[ADDR_MAC_LEN] = {0};
    char tmp[ONE_ADDR_LEN + 1] = {0};
    char *p = (char *)para;

    for (uint8_t i = 0; i < ADDR_MAC_LEN; i++) {
        memcpy_s(tmp, ONE_ADDR_LEN + 1, p, ONE_ADDR_LEN);
        tmp[ONE_ADDR_LEN] = '\0';
        addr[i] = strtol(tmp, NULL, 16); // 16进制
        p = p + ONE_ADDR_LEN;
    }

    wstp_print("mcu_auto_ota_del_mac, mac = %02x%02x%02x%02x%02x%02x\r\n", addr[5], addr[4], addr[3], addr[2], addr[1],
               addr[0]);
    bool res = auto_ota_dev_del(addr);
    wstp_print("auto_ota_dev_del, res = %d\r\n", res);

    return ERRCODE_SUCC;
}

static uint32_t mcu_auto_ota_add_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    wstp_print("enter mcu_auto_ota_add_mac\r\n");
    wstp_print("para = %s\r\n", para);
    uint8_t addr[ADDR_MAC_LEN] = {0};
    char tmp[ONE_ADDR_LEN + 1] = {0};
    char *p = (char *)para;

    for (uint8_t i = 0; i < ADDR_MAC_LEN; i++) {
        memcpy_s(tmp, ONE_ADDR_LEN + 1, p, ONE_ADDR_LEN);
        tmp[ONE_ADDR_LEN] = '\0';
        addr[i] = strtol(tmp, NULL, 16); // 16进制
        p = p + ONE_ADDR_LEN;
    }

    wstp_print("mcu_auto_ota_add_mac, mac = %02x%02x%02x%02x%02x%02x\r\n", addr[5], addr[4], addr[3], addr[2], addr[1],
               addr[0]);
    auto_ota_dev_add(addr);

    return ERRCODE_SUCC;
}

static uint32_t mcu_auto_ota_show_mac(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    unused(para_len);
    unused(para);
    wstp_print("enter mcu_auto_ota_show_mac\r\n");

    auto_ota_dev_show();

    return ERRCODE_SUCC;
}
#endif

#ifdef LOSCFG_KERNEL_PERF
static uint32_t mcu_perf_start(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    return perf_test_start();
}

static uint32_t mcu_perf_stop(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    return perf_test_stop();
}
#endif

#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
static uint32_t mcu_sched_statistis_start(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    OsSchedStatStart();
    return 0;
}

static uint32_t mcu_sched_statistis_stop(uint8_t *para, uint32_t para_len)
{
    unused(para_len);
    unused(para);
    OsSchedStatStop();
    OsSchedStatInfo();
    return 0;
}
#endif

static const app_process_table g_app_funcs[] = {
    { RESET,                mcu_os_sys_reset },
    { FSTEST,               mcu_fs_test },
    { FSFORMAT,             mcu_fs_format },
    { FSRWTEST,             mcu_fs_rw_test },
    { FSSETCLK,             mcu_fs_set_clk },
    { FSREADCLK,            mcu_fs_read_clk },
    { FSWRITE,              mcu_fs_write },
    { FSCHECK,              mcu_fs_check },
    { FSRM,                 mcu_fs_rm },
    { GETWSTPVER,           mcu_get_wstp_version },
    { LOGWRITE,             mcu_logfile_write },
    { HEAPINFO,             mcu_os_heap_info },
    { STACKINFO,            mcu_os_stack_info },
    { TESTDTCMBW,           mcu_test_dtcm_baudwidth },
    { TESTL2MBW,            mcu_test_l2m_baudwidth },
    { TESTMMCBW,            mcu_test_mmc_baudwidth },
    { TESTPSRAMBW,          mcu_test_psram_baudwidth },
    { TESTNORFLASHBW,       mcu_test_norflash_baudwidth },
    { TESTNANDFLASHBW,      mcu_test_nandflash_baudwidth },
    { GETFSINFO,            mcu_get_fs_info },
    { PMRMVOTE,             mcu_pm_remove_vote },
    { PMADDVOTE,            mcu_pm_add_vote },
    { LCDON,                mcu_pm_lcd_on },
    { LCDOFF,               mcu_pm_lcd_off },
    { SHIPMODE,             mcu_pm_into_ship_mode },
    { FORCEDEEPSLEEP,       mcu_pm_into_force_ds_mode },
    { PMMODESET,            mcu_pm_req_mode },
    { PMMODEUSR,            mcu_set_pm_req_mode_usr},
    { PMMODEGET,            mcu_pm_dfx_mode },
    { GETCURSEC,            mcu_dfx_get_cur_second},
    { LCDFILL,              lcd_fill },
    { GETIDS,               get_ids },
    { LCDLIGHT,             lcd_test_light },
    { RESUMETOUCH,          mcu_pm_no_resume_touch },
    { MCUDYMPW,             mcu_pm_set_mcu_dynamic_power },
    { PMCLDO1PD,            mcu_pm_cldo1_power_down },
    { AESDECRYPT,           mcu_aes_decrypt_sample },
    { AESENCRYPT,           mcu_aes_encrypt_sample },
    { AESSETGID,            mcu_aes_set_gid },
    { SETCALENDAR,          mcu_calendar_set_date },
    { GETCALENDAR,          mcu_calendar_get_date },
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    { ADCAUTOSAMPLE,        adc_auto_sample_by_diag },
    { ADCCLOSEAUTOSAMPLE,   adc_auto_sample_disable },
    { ADCSHOWDATA,          adc_auto_sample_show_data },
#endif
    { BUTTON,               mcu_button_sample },
    { POWERKEY,             mcu_power_key_config },
#ifdef SUPPROT_EXECUTE_CORMARK
    { COREMARKTEST,         mcu_coremark_test },
#endif
    { NANDREAD,             at_nand_write_read},
    { NANDWRITE,            at_nand_write_write},
    { NANDERASE,            at_nand_write_erase},
    { NANDMARK,             at_nand_mark_bad},
    { NANDCHECK,            at_nand_check_bad},
    { NANDSETMODE,          at_nand_set_mode},
    { SETAPPDELAY,          mcu_set_app_delay_time },
    { SETCLDO1VOL,          mcu_set_cldo1_voltage },
    { NANDAGINGTEST,        mcu_nand_aging_test },
    { FLASHAGINGTEST,       mcu_norflash_aging_test },
    { SETPINPULL,           mcu_set_pin_pull },
    { GETPINPULL,           mcu_get_pin_pull },
    { TIMESET,              mcu_set_time },
    { TIMEGET,              mcu_get_time },
#ifdef SUPPORT_POWER_MANAGER
    { SETSCREENOFFTIME,     mcu_pms_set_screenoff_time },
    { SETSCREENOFFFUNC,     mcu_pms_set_screenoff_enable },
    { SETSCREENSTATE,       mcu_pms_set_screen_state },
    { GETSCREENSTATE,       mcu_pms_get_screen_state },
#endif
#ifdef FT_DONGLE
    { DONGLECONNECTED,      mcu_check_dongle_connected },
#endif
    { I2CWRITE,             mcu_i2c_write },
    { I2CREAD,              mcu_i2c_read },
#ifdef SUPPORT_ALIPAY_SEC
    { ALIPAYSECTEST,        mcu_alipay_sec_test },
    { ALIPAYDEBUG,          mcu_alipay_debug },
    { ALIPAYVERIFY,         mcu_alipay_pl_verify },
    { ALIPAYRESET,          mcu_alipay_reset },
#endif
#ifdef SUPPORT_FILE_SPLIT_COMBINE
    { FILESPLIT,            mcu_file_spilt_test },
    { FILECOMBINE,          mcu_file_combine_test },
#endif
#ifdef JS_ENABLE
    { GETTRANSCHAN,         mcu_get_trans_channel_test },
    { SETTRANSCHAN,         mcu_set_trans_channel_test },
#endif
#ifdef SUPPORT_AUTO_OTA
    { AUTOOTABLOCK,         mcu_auto_ota_block },
    { AUTOOTARUN,           mcu_auto_ota_run },
    { AUTOOTADELMAC,        mcu_auto_ota_del_mac },
    { AUTOOTAADDMAC,        mcu_auto_ota_add_mac },
    { AUTOOTASHOWMAC,       mcu_auto_ota_show_mac },
#endif
#ifdef LOSCFG_KERNEL_PERF
    { PERFSTART,          mcu_perf_start },
    { PERFSTOP,           mcu_perf_stop },
#endif
#ifdef LOSCFG_DEBUG_SCHED_STATISTICS
    { SCHEDSTART,          mcu_sched_statistis_start },
    { SCHEDSTOP,           mcu_sched_statistis_stop  },
#endif
};

void app_at_process(uint8_t *data, uint32_t data_len)
{
    unused(data_len);
    if (data == NULL) {
        return;
    }
    int32_t ret = ERRCODE_FAIL;
    uint8_t reply_data[DIAG_RES_LEN] = {0};
    app_msg_info_t *msg = (app_msg_info_t *)(data);

    for (uint32_t i = 0; i < ARRAY_COUNT(g_app_funcs); i++) {
        if (msg->id == g_app_funcs[i].id) {
            ret = g_app_funcs[i].func(msg->payload, msg->payload_len);
        }
    }
    print_at_cmd_ret(ret);

    // phone remote control, need reply to spp
    if (g_app_diag_flag) {
        reply_data[0x0] = 0x80;
        reply_data[0x1] = 0x82;
        reply_data[0x2] = 0;
        reply_data[0x3] = (uint8_t)ret;
        ret = send_spp_diag_msg(g_app_diag_flag, &g_mcu_diag_header, reply_data, DIAG_RES_LEN);
        wstp_print("mcu_at_process send msg to spp %s.\r\n", (ret == ERRCODE_SUCC) ? "success" : "failure");
    }

    if (ret != ERRCODE_SUCC) {
        wstp_print("app_at_process func fail! MsgID=0x%x ret=%d "NEWLINE, msg->id, ret);
    }
    g_app_diag_flag = false;
}
