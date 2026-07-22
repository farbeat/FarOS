/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:   Application Core OS Initialize Interface for Standard
 * Author:
 * Create: 2021-03-09
 */
#include "thread_init.h"
#include "tcxo.h"
#include "log_common.h"
#include "log_def.h"
#include "log_uart.h"
#include "log_file.h"
#ifdef __LITEOS__
#include "los_status.h"
#endif
#include "pmu_interrupt.h"
#include "log_reg_dump.h"
#include "watchdog.h"
#include "preserve.h"
#include "hal_reboot.h"
#include "pinctrl_porting.h"
#include "pm_sleep.h"
#include "pm_lpc.h"
#include "pm_control.h"
#include "dfx_system_init.h"
#include "cpu_load.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_print.h"

#ifdef SUPPORT_AT_CMD
#include "at_cmd_api.h"
#endif

#ifdef SUPPORT_AT_GRAPHIC
#include "graphic_msg_manager.h"
#include "graphic_rx_process.h"
#endif

#ifdef SUPPORT_AT_APP
#include "app_msg_manager.h"
#include "app_rx_process.h"
#endif

#ifdef SUPPORT_AT_AUDIO
#include "audio_msg_manager.h"
#include "audio_rx_process.h"
#endif

#ifdef SUPPORT_AT_BT_MANAGER
#include "bt_manager_msg_manager.h"
#include "bt_manager_rx_process.h"
#endif

#ifdef SUPPORT_AT_MEDIA
#include "media_msg_manager.h"
#include "media_rx_process.h"
#endif

#include "clocks_config.h"
#ifdef SUPPORT_POWER_MANAGER
#include "power_display_service.h"
#endif

#if defined(__FREERTOS__)
#include "FreeRTOS.h"
#if defined(FREERTOS_DEBUG)
#include "debug_cmd.h"
#endif
#endif

#ifdef SUPPORT_MEDIA_AUDIO_MANAGER
#include "audio_manager_c_wrapper.h"
#endif
#include "clocks_config.h"
#include "sys/time.h"
#include "time.h"
#include "log_panic.h"
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#endif
#include "exception_dump.h"
#ifdef SUPPORT_AUTO_OTA
#include "auto_ota_service.h"
#endif

#if SUPPORT_LWIP
#include "lwip/tcpip.h"
#endif

#define EMMC_BASE_ADDR              0x52062000

#define TASK_COMMON_APP_DELAY_MS    5000
#define TASK_SAVE_FILE_DELAY_MS     2000

#define TASK_PRIORITY_APP           (osPriority_t)(17)
#define TASK_PRIORITY_LOG           (osPriority_t)(18)
#define TASK_PRIORITY_CMD           (osPriority_t)(17)
#define TASK_PRIORITY_BRANDY_APP    (osPriority_t)(15)
#define TASK_PRIORITY_SDK           (osPriority_t)(31)
#define TASK_PRIORITY_DFX_MSG       (osPriority_t)(18)
#define TASK_PRIORITY_TRANSMIT      (osPriority_t)(20)
#define TASK_PRIORITY_SAVE_FILE     (osPriority_t)(36)
#define TASK_PRIORITY_AUTO_OTA      (osPriority_t)(36)

#define BTH_SDK_STACK_SIZE          0xC00
#define AT_GRAPHIC_STACK_SIZE       0xC00
#define AT_APP_STACK_SIZE           0x1000
#define AT_AUDIO_STACK_SIZE         0x1000
#define AT_BT_MANAGER_STACK_SIZE    0x1000
#define AT_MEDIA_STACK_SIZE         0x1000
#define DFX_MSG_STACK_SIZE          0x2000
#define PM_DISPLAY_MSG_STACK_SIZE   0xc00
#define AT_OHOSFWK_STACK_SIZE       0xC00
#define APP_MAIN_STACK_SIZE         0xC00
#define SAVE_FILE_STACK_SIZE        0xC00
#define AUTO_OTA_STACK_SIZE         0xC00
#define LWIP_MAIN_STACK_SIZE        0xC00

#define TIME_MAX                64
#define USEC_TO_MILLI           1000
#define TCXO_TICKS_PER_MILLISECOND  32000

#if SUPPORT_LWIP
static void lwip_main(void);
#endif

static void app_main(const void *unused);
void cmd_processor_entry(const void *args);
void cmd_main_add_functions(void);
void bt_acore_task_main(const void *pvParams);
int msg_process_thread(void *data);
errcode_t thread_msg_event_init(void);
int transmit_process_thread(void *data);
errcode_t transmit_thread_msg_init(void);
void system_time_cali(void);
#ifdef SUPPORT_32K_MEAS
void uapi_32k_systick_measurements(void);
#endif
osSemaphoreId_t g_semaphore_save = NULL;
void save_file_main(void);

uint32_t g_app_delay_ms = TASK_COMMON_APP_DELAY_MS;
app_task_definition_t g_app_tasks[] = {
    { { "app", 0, NULL, 0, NULL, APP_MAIN_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)app_main, NULL, NULL, USE_DTCM_MEM },
#if SUPPORT_LWIP
    { { "lwip", 0, NULL, 0, NULL, LWIP_MAIN_STACK_SIZE, TASK_PRIORITY_BRANDY_APP, 0, 0 },
      (osThreadFunc_t)lwip_main, NULL, NULL, USE_DTCM_MEM },
#endif
#if (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO)
    { { "log", 0, NULL, 0, NULL, (0xC00), TASK_PRIORITY_LOG, 0, 0 },
      (osThreadFunc_t)log_main, NULL, NULL, USE_DTCM_MEM },
#endif
    { { "bt_sdk", 0, NULL, 0, NULL, BTH_SDK_STACK_SIZE, TASK_PRIORITY_SDK, 0, 0 },
      (osThreadFunc_t)bt_acore_task_main, NULL, NULL, USE_DTCM_MEM },
#ifdef SUPPORT_AT_CMD
    { { "at_cmd", 0, NULL, 0, NULL, 0xC00, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)at_cmd_task_process_handler, NULL,
      (CreateQueueFunction_t)at_cmd_task_queue_init, USE_DTCM_MEM },
#endif
#if defined(SUPPORT_AT_GRAPHIC)
    { { "at_graphic", 0, NULL, 0, NULL, AT_GRAPHIC_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)graphic_at_task_process_handler, NULL,
      (CreateQueueFunction_t)graphic_at_task_queue_init, USE_EXTEND_MEM },
#endif
#ifdef SUPPORT_AT_APP
    { { "at_app", 0, NULL, 0, NULL, AT_APP_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)app_at_task_process_handler, NULL,
      (CreateQueueFunction_t)app_at_task_queue_init, USE_EXTEND_MEM },
#endif
#ifdef SUPPORT_AT_AUDIO
    { { "at_audio", 0, NULL, 0, NULL, AT_AUDIO_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)audio_at_task_process_handler, NULL,
      (CreateQueueFunction_t)audio_at_task_queue_init, USE_DTCM_MEM },
#endif
#ifdef SUPPORT_AT_BT_MANAGER
    { { "at_bt_manager", 0, NULL, 0, NULL, AT_BT_MANAGER_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)bt_manager_task_process_handler, NULL,
      (CreateQueueFunction_t)bt_manager_at_task_queue_init, USE_DTCM_MEM },
#endif
#ifdef SUPPORT_AT_MEDIA
    { { "at_media", 0, NULL, 0, NULL, AT_MEDIA_STACK_SIZE, TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)media_at_task_process_handler, NULL,
      (CreateQueueFunction_t)media_at_task_queue_init, USE_DTCM_MEM },
#endif
#ifdef HSO_SUPPORT
    { { "dfx_msg", 0, NULL, 0, NULL, DFX_MSG_STACK_SIZE, TASK_PRIORITY_DFX_MSG, 0, 0 },
      (osThreadFunc_t)msg_process_thread, NULL,
      (CreateQueueFunction_t)thread_msg_event_init, USE_DTCM_MEM },
#endif
#if (CONFIG_DFX_SUPPORT_TRANSMIT_FILE == DFX_YES && !defined(MEMORY_MINI))
    { { "transmit", 0, NULL, 0, NULL, DFX_MSG_STACK_SIZE, TASK_PRIORITY_TRANSMIT, 0, 0 },
      (osThreadFunc_t)transmit_process_thread, NULL,
      (CreateQueueFunction_t)transmit_thread_msg_init, USE_DTCM_MEM },
#endif
#if defined(SUPPORT_POWER_MANAGER) && !defined(SUPPORT_AT_OHOSFWK)
    /* OHOS场景，pms不需要独立的队列和线程，事件由UI线程接管处理 */
    { { "power_display", 0, NULL, 0, NULL, PM_DISPLAY_MSG_STACK_SIZE, TASK_PRIORITY_TRANSMIT, 0, 0 },
      (osThreadFunc_t)power_display_msg_thread, NULL,
      (CreateQueueFunction_t)power_display_task_init, USE_DTCM_MEM },
#endif
    { { "save_file", 0, NULL, 0, NULL, SAVE_FILE_STACK_SIZE, TASK_PRIORITY_SAVE_FILE, 0, 0 },
      (osThreadFunc_t)save_file_main, NULL, NULL, USE_DTCM_MEM },
#ifdef SUPPORT_AUTO_OTA
    { { "auto_ota", 0, NULL, 0, NULL, AUTO_OTA_STACK_SIZE, TASK_PRIORITY_AUTO_OTA, 0, 0 },
      (osThreadFunc_t)auto_ota_task_main, NULL, NULL, USE_DTCM_MEM },
#endif
};

osSemaphoreId_t get_save_file_semaphore(void)
{
    return g_semaphore_save;
}

void save_file_main(void)
{
    if (osKernelGetState() == osKernelRunning) {
        dump_norflash_trace_to_file();
    }
    g_semaphore_save = osSemaphoreNew(1, 0, NULL);
    if (!g_semaphore_save) {
        panic(PANIC_LOG, LOG_PANIC_MUTEX_NOT_READY);
        return;
    }
    for (;;) {
        // If there has been log indicator get run again
        if (osKernelGetState() == osKernelRunning) {
            if (osSemaphoreAcquire(g_semaphore_save, osWaitForever) != osOK) {
                panic(PANIC_LOG, LOG_PANIC_MUTEX_NOT_READY);
            }
#ifdef CFG_DRIVERS_NANDFLASH
            nand_driver_init(SPEED_SLOW);
            set_trans_type(TRANS_BY_CPU_SINGLE_LINE);
#endif
#if defined SAVE_EXC_INFO
            save_preserve_file_to_fs();
            save_btram_file_to_fs();
            save_btrace_file_to_fs();
#endif
            osDelay(TASK_SAVE_FILE_DELAY_MS);
            hal_reboot_chip();
        }
        osDelay(1000);
    }
}

void set_app_delay_time(uint32_t time)
{
    g_app_delay_ms = time;
}

void thread_init(void)
{
    /* Create queues before tasks in case they are used during initialisation. */
    for (uint8_t i = 0; i < (sizeof(g_app_tasks) / sizeof(app_task_definition_t)); i++) {
        if (g_app_tasks[i].create_queue_handle) {
            g_app_tasks[i].create_queue_handle();
        }
    }

    /* Create tasks */
    for (uint8_t i = 0; i < (sizeof(g_app_tasks) / sizeof(app_task_definition_t)); i++) {
        if (g_app_tasks[i].use_mem == USE_EXTEND_MEM) {
            g_app_tasks[i].attr.stack_mem = memalign(16, g_app_tasks[i].attr.stack_size); // 16字节对齐
#if defined(__FREERTOS__)
            g_app_tasks[i].attr.cb_size = sizeof(StaticTask_t);
            g_app_tasks[i].attr.cb_mem = memalign(16, sizeof(StaticTask_t));  // 16字节对齐
#endif
        }
        g_app_tasks[i].task_handle = osThreadNew(g_app_tasks[i].func, NULL, &(g_app_tasks[i].attr));
        if (g_app_tasks[i].task_handle == NULL) {
            panic(PANIC_TASK_CREATE_FAILED, i);
        }
    }
}

static void print_system_sleep_info(void)
{
#if ENABLE_LOW_POWER == YES
    uint32_t deepsleep_time, lightsleep_time;
    uint32_t deepsleep_cnt, lightsleep_cnt;

    sleep_info_t *sleep_info = pm_get_sleep_info();

    deepsleep_time = (uint32_t)(sleep_info->sleep_history[PM_SLEEP_DS].total_slp_time);
    deepsleep_cnt = (uint32_t)(sleep_info->sleep_history[PM_SLEEP_DS].total_slp_count);

    lightsleep_time = (uint32_t)(sleep_info->sleep_history[PM_SLEEP_LS].total_slp_time);
    lightsleep_cnt = (uint32_t)(sleep_info->sleep_history[PM_SLEEP_LS].total_slp_count);

    uint32_t sys_slp = readw(0x57004088);
    PRINT("sys_slp: 0x%x\r\n", sys_slp);
    reg16_setbit(0x57004088, 1); // clr slp history
    reg16_setbit(0x57004088, 0); // clr slp history

    bool m_core_sleep = pmu_msystem_sleep_history();
    pmu_msystem_sleep_history_clear();
    PRINT("M core deepsleep time:%d, lightsleep time:%d, m sleep flag:%d"NEWLINE,
        deepsleep_time, lightsleep_time, (uint8_t)m_core_sleep);
    PRINT("M core deepsleep cnt:%d, lightsleep cnt:%d"NEWLINE, deepsleep_cnt, lightsleep_cnt);
    PRINT("M core last veto id:%d, last veto count:%d"NEWLINE, sleep_info->sleep_veto.last_veto_id,
        sleep_info->sleep_veto.last_veto_count);

    oml_pf_log_print1(LOG_BCORE_PLT_DRIVER_REBOOT, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO, "sys_slp: 0x%d", sys_slp);
    oml_pf_log_print4(LOG_BCORE_PLT_DRIVER_REBOOT, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO,
        "App run time:%d, app deepsleep time:%d, lightsleep time:%d, m sleep flag:%d",
        (uint32_t)los_system_system_run_time(), deepsleep_time, lightsleep_time, (uint8_t)m_core_sleep);
#endif
}

#ifdef SUPPORT_32K_MEAS
/* 使用32M tcxo 测量 systick的32K频率值 */
void uapi_32k_systick_measurements(void)
{
    uint64_t cali_ms = 500;  // 单次测量时长（ms）
    uint64_t start_count = 0;
    uint64_t total_count = 0;
    uint64_t freq_32k = 0;

    uint32_t irq = osal_irq_lock();
    start_count = uapi_tcxo_get_count();
    uapi_systick_delay_ms(cali_ms);
    total_count = uapi_tcxo_get_count() - start_count;
    osal_irq_restore(irq);

    freq_32k = (STANDARD_32K_FREQUENCY * cali_ms * TCXO_TICKS_PER_MILLISECOND) / (total_count);
    PRINT("32M_count = %llu, after calculation 32K freq = %llu."NEWLINE, total_count, freq_32k);
    oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_REBOOT, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO,
                      "32M_count = %llu, after calculation 32K freq = %llu.", total_count, freq_32k);
    return;
}
#endif

void system_time_cali(void)
{
    struct timeval tv = { 0 };

    if (gettimeofday(&tv, NULL)) {
        return;
    }
    return;
}


static void app_main(const void *unused)
{
    unused(unused);
    dfx_set_log_enable();
#ifdef SUPPORT_MEDIA_AUDIO_MANAGER
    bool success = AudioManagerInit();
    PRINT("audio manager init %s\r\n", success ? "ok" : "error");
#endif
    oml_pf_log_print0(LOG_BCORE_PLT, LOG_NUM_DEBUG, LOG_LEVEL_INFO, "App main");
#if (USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO)
    log_exception_dump_reg_check();
#endif
    uapi_pm_request_mode(PM_ID_DEBUG, PM_MODE_LP);
    PRINT("app_main run start."NEWLINE);
    while (1) {  //lint !e716 Main Loop
        (void)osDelay(g_app_delay_ms);
        uapi_watchdog_kick();
        uapi_logfile_fsync(STORE_DIAG);
#ifdef SUPPORT_32K_MEAS
        uapi_32k_systick_measurements();
#endif
        system_time_cali();

#ifndef XTS_SUPPORT
        PRINT("app_main run start."NEWLINE);
        print_system_sleep_info();
        oml_pf_log_print0(LOG_BCORE_PLT_DRIVER_REBOOT, LOG_NUM_DRIVER_LPC, LOG_LEVEL_INFO, "App run ok.");
#endif
    }
}

#if SUPPORT_LWIP
static void lwip_main(void)
{
    PRINT("lwip_main run start."NEWLINE);
    tcpip_init(NULL, NULL);
}
#endif
