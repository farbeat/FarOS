/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:   Application Core OS Priority Initialize Interface for Standard
 * Author:
 * Create: 2022-12-03
 */
#include "debug_print.h"
#include "thread_init.h"
#include "common_def.h"
#include "pm.h"
#include "watchdog.h"
#include "cpu_trace.h"
#include "non_os_reboot.h"
/* after reboot system emmc need write exc info to the file system from sector */
#if defined(SAVE_EXC_INFO) && defined(CFG_DRIVERS_MMC)
#include "exception.h"
#endif
#ifdef CFG_DRIVERS_MMC
#ifdef __LITEOS__
#include "soc/mmc.h"
#endif
#include "soc_mmc.h"
#endif
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#include "soc_lcd_api.h"
#if defined(TPTYPE_TMA525B)
#include "tma525b_drv1.h"
#elif defined(TPTYPE_ZTW523)
#include "ztw523_touch1.h"
#else
#include "ztw523_touch1.h"
#endif
#endif
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#include "yaffs_nand_drv.h"
#endif
#include "thread_pri_init.h"
#if (DSP_EXIST == YES) && !defined(BRANDY_MCU_BT)
#include "soc_uapi_audio_sys.h"
#include "soc_uapi_vendor.h"
#include "soc_uapi_sound.h"
/* Audio codec vendor select according to hardware connection:
 * define AUDIO_SMARTPA_AW88166 : macro to init smartpa aw88166;
 * define AUDIO_SMARTPA_AW88166_I2S_ID UAPI_SND_OUT_PORT_I2S0 : macro to specify i2s id connecting to smartpa aw88166;
 */
#define AUDIO_SMARTPA_AW88262
#define AUDIO_SMARTPA_AW88262_I2S_ID UAPI_SND_OUT_PORT_I2S2

#endif /* DSP_EXIST */
#include "panic.h"
#include "board_init.h"
#include "kernel_init.h"
#ifdef SUPPORT_OHOSFWK
#include "ohosfwk_startup.h"
#endif
#include "clocks_config.h"
#include "board_init.h"
#include "upg.h"
#include "upg_definitions.h"
#include "nv.h"
#include "nv_upg.h"

#define EMMC_BASE_ADDR              0x52062000
#define TASK_PRIORITY_APP           (osPriority_t)(17)
#define TICK_TEN_MS                 (10U)
#define WAIT_BT_BOOTUP_TIME         200U
#define UPG_HEAD_MAGIC              0x55AA55AA
#define UPG_END_MAGIC               0xAA55AA55
#define UPG_FLAG_MAGIC              0xFFFFFFFF

static void thread_main(const void *unused);
void bt_acore_btc_ipc_receive_init(void);

app_task_definition_t g_thread_pri = { { "thread_pri", 0, NULL, 0, NULL, (0x1000), TASK_PRIORITY_APP, 0, 0 },
                                       (osThreadFunc_t)thread_main, NULL, NULL, USE_DTCM_MEM };
store_class g_store_init_select = STORE_CLASS_NAND;

#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
static void sample_gfx_init(void)
{
#if defined(TPTYPE_TMA525B)
    tma525b_host_init_sample();
#elif defined(TPTYPE_ZTW523)
    ztw523_host_init_sample();
#else
    ztw523_host_init_sample();
#endif
    uapi_lcd_init();
#ifdef SUPPORT_GPU_GMMU
    drv_gmmu_module_init();
#endif
    uapi_vau_module_init();
    uapi_dpu_module_init();
#ifdef SUPPORT_GPU_JPEG
    uapi_jpeg_module_init();
#endif
}
#endif

#ifdef CFG_DRIVERS_MMC
static void emmc_init(void)
{
    /**
     * FPGA解复位, 设置CMD_CONFLICT_CHECK
     * 之后才能设置管脚
     */
    writew(EMMC_BASE_ADDR + 0x508, 0);
    init_emmc_pinmux();
    int ret = emmc_drv_context_init();
    if (ret != ENOERR) {
        PRINT("emmc init error:[%d]"NEWLINE, ret);
    }
    mmc_set_first_init_flag(true);
    mmc_set_sleep_init_flag(true);
    PRINT("emmc init success\r\n");

#if defined(SAVE_EXC_INFO)
    if (emmc_exc_info_write() != ERRCODE_SUCC) {
        PRINT("emmc_exc_info_write failed"NEWLINE);
    }
#endif
}

static void emmc_init_handle(void)
{
    emmc_init();
    g_store_init_select = STORE_CLASS_EMMC;
    sdiom_internal_freq_handler_register(mmc_set_internal_clk);
}
#endif

#ifdef CFG_SDIO_EMMC_COEXIST
static void sdio_init(void)
{
    init_sdio_pinmux();
    sdio_drv_context_init();
}
#endif

#ifdef CFG_DRIVERS_NANDFLASH
static void nandflash_init(void)
{
    int32_t ret = 0;
    ret = nandflash_filesystem_init();
    if (ret != 0) {
        PRINT("nandflash init error[%d]"NEWLINE, ret);
    }
    PRINT("nandflash init success\r\n");
}

static void nandflash_init_handle(void)
{
    yaffs_nand_drv_checkptset(YAFFS_CHECKPT_SKIPED);
    nandflash_init();
    g_store_init_select = STORE_CLASS_NAND;
}
#endif

static void board_version_class_print(void)
{
    PRINT("**********startup info**********\r\n");
    PRINT("os init class is %s."NEWLINE, (get_os_init_select() == OS_CLASS_LITEOS) ? "liteos" : "freertos");
    PRINT("store init class is %s."NEWLINE, (g_store_init_select == STORE_CLASS_NAND) ? "nandflash" : "emmc");
    PRINT("display init class is %s."NEWLINE, (get_display_init_select() == DISPLAY_CLASS_QSPI) ? "QSPI" : "MIPI");
    PRINT("board init class is %s."NEWLINE, (get_board_init_select() == BOARD_CLASS_E0B2) ? "E0B2" : "E0B4");
#ifdef CONFIG_PSRAM_SUPPORT
    PRINT("psram density is %s."NEWLINE, (get_psram_density() == PSRAM_DENSITY_16M) ? "16MB" : "32MB");
#endif
    PRINT("**********startup info**********\r\n");
}

#if (DSP_EXIST == YES) && !defined(BRANDY_MCU_BT)
/**
 * The sample code of the vendor,
 * which will be implemented by the customer in the future
 */
#ifdef AUDIO_SMARTPA_AW88262
td_s32 audio_aw88262_init(const uapi_vendor_codec_attr *attr);
td_s32 audio_aw88262_deinit(td_void);

static uapi_vendor_driver g_aw88262_driver = {
    .out_port = AUDIO_SMARTPA_AW88262_I2S_ID,
    .init = audio_aw88262_init,
    .deinit = audio_aw88262_deinit,
    .set_aef_profile = TD_NULL,
};

static td_s32 audio_aw88262_register_driver(void)
{
    return uapi_audio_vendor_register(&g_aw88262_driver);
}
#endif /* AUDIO_SMARTPA_AW88262 */

#ifdef AUDIO_SMARTPA_AW88166
td_s32 audio_aw88166_init(const uapi_vendor_codec_attr *attr);
td_s32 audio_aw88166_deinit(td_void); /* Call when device power off */
td_s32 audio_aw88166_start(const uapi_vendor_codec_attr *attr);
td_s32 audio_aw88166_stop(td_void);
td_s32 audio_aw88166_set_aef_profile(uapi_snd_aef_profile aef_profile);

static uapi_vendor_driver g_aw88166_driver = {
    .out_port = AUDIO_SMARTPA_AW88166_I2S_ID,
    .init = audio_aw88166_start,
    .deinit = audio_aw88166_stop,
    .set_aef_profile = audio_aw88166_set_aef_profile,
};

static td_s32 audio_aw88166_register_driver(void)
{
    const uapi_vendor_codec_attr attr = {};

    audio_aw88166_init(&attr); /* Init aw88166 once when device power on */

    return uapi_audio_vendor_register(&g_aw88166_driver);
}
#endif /* AUDIO_SMARTPA_AW88166 */

static void audio_smartpa_init(void)
{
    uint32_t ret;

#ifdef AUDIO_SMARTPA_AW88262
    ret = (uint32_t)audio_aw88262_register_driver();
    if (ret != 0) {
        PRINT("aw88262 register driver error\r\n");
    }
#endif /* AUDIO_SMARTPA_AW88262 */

#ifdef AUDIO_SMARTPA_AW88166
    ret = (uint32_t)audio_aw88166_register_driver();
    if (ret != 0) {
        PRINT("aw88166 register driver error\r\n");
    }
#endif /* AUDIO_SMARTPA_AW88166 */
}
#endif /* DSP_EXIST */

#if (DSP_EXIST == YES) && !defined(BRANDY_MCU_BT)
static void audio_init()
{
    (void)osDelay(TICK_TEN_MS);
    uapi_watchdog_kick();

    audio_smartpa_init();

    uint32_t ret = (uint32_t)uapi_audio_init();
    if (ret != 0) {
        PRINT("audio init error:[%d]\r\n", ret);
    } else {
        PRINT("audio init OK\r\n");
    }
}
#endif

void rc_calibration_func_notify(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    /* BT核用来通知MCU 32K校准值 */
    writew(GLB_CTL_D_RB_BASE + GLB_GP_D_REG3, 0x0);
    /* 通知BT核 32K时钟来源 */
    writew(GLB_CTL_D_RB_BASE + GLB_GP_D_REG2, CLOCK_SOURCE_RC);
#else
    writew(GLB_CTL_D_RB_BASE + GLB_GP_D_REG2, CLOCK_SOURCE_XO);
#endif
}

static void force_to_sleep(void)
{
    if (reg16_getbits(0x5702C010, 0x8, 0x8) == 0x5A) {  // 特殊模式下，强制进入深睡中
        reg16_clrbits(0x5702C010, 0x8, 0x8);
        uapi_tcxo_delay_ms(WAIT_BT_BOOTUP_TIME);
        hal_pmu_cldo_enable_slp_bypass(HAL_PMU_CLDO1, TURN_OFF);
        uapi_sys_force_mcu_only_deep_sleep_mode(0);
    }
}

#ifdef CONFIG_OTA_UPDATE_SUPPORT
static void upg_check_upgrade(void)
{
    uint32_t fota_address = 0;
    errcode_t ret_val = (errcode_t)upg_get_upgrade_flag_flash_start_addr(&fota_address);
    if (ret_val != ERRCODE_SUCC) {
        return;
    }

    fota_upgrade_flag_area_t upg_flag_info;
    ret_val = (errcode_t)upg_flash_read(fota_address, sizeof(fota_upgrade_flag_area_t), (uint8_t *)(&upg_flag_info));
    if (ret_val != ERRCODE_SUCC) {
        return;
    }

    if (!(upg_flag_info.head_magic == UPG_HEAD_MAGIC &&
          upg_flag_info.head_end_magic == UPG_END_MAGIC &&
          upg_flag_info.complete_flag == UPG_FLAG_MAGIC)) {
        /* 不需要升级直接返回 */
        PRINT("No need to upgrade...\r\n");
        return;
    }

    PRINT("upgrading.......\r\n");
    ret_val = uapi_upg_start();
    if (ret_val != ERRCODE_SUCC) {
        PRINT("upgrade failed, ret = 0x%x\r\n", ret_val);
    } else {
        PRINT("upgrade success\r\n");
    }
    upg_reboot();
}
#endif

static void thread_main(const void *unused)
{
    PRINT("thread_main start "NEWLINE);
    unused(unused);
    rc_calibration_func_notify();
    bt_acore_btc_ipc_receive_init();
    uapi_pm_control(PM_BT_POWER_UP_AND_RUN);
#ifdef FORCE_TO_DEEP_SLEEP
    force_to_sleep();
#endif
    los_vfs_init();
    uint32_t ret = dpal_driver_base_init();
    if (ret != 0) {
        PRINT("dpal driver base init error"NEWLINE);
        panic(PANIC_ASSERT, __LINE__);
    }
#ifdef CFG_DRIVERS_MMC
    emmc_init_handle();
#endif
#ifdef CFG_SDIO_EMMC_COEXIST
    sdio_init();
#endif
#ifdef CFG_DRIVERS_NANDFLASH
    nandflash_init_handle();
#endif

    reboot_cause_t cause = get_cpu_utils_reset_cause();
    if ((cause != REBOOT_CAUSE_APPLICATION_STD_CHIP_WDT_FRST) &&
        (cause != REBOOT_CAUSE_APPLICATION_XIP_CTRL) &&
        (cause != REBOOT_CAUSE_APPLICATION_XIP_CACHE)) {
        cpu_trace_enable_mcpu_repeat();
    } else {
        PRINT("nmi reboot cause: 0x%x, reason: 0x%x"NEWLINE, cause, get_fault_reason());
        // save cputrace memory if chip watchdog reset
        hal_cpu_trace_clock_enable();
        panic(PANIC_ASSERT, cause);
    }

    uapi_pm_control(PM_DISPLAY_POWER_UP_AND_RUN);
    (void)osDelay(TICK_TEN_MS);

#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    sample_gfx_init();
#else
    uapi_pm_control(PM_DISPLAY_POWER_OFF);
#endif

#if (DSP_EXIST == YES) && !defined(BRANDY_MCU_BT)
    audio_init();
#endif
    board_version_class_print();
    uapi_watchdog_kick();
#ifdef CONFIG_OTA_UPDATE_SUPPORT
    upg_check_upgrade();
#endif
    /* nv init and file system init must be before nv_upg_upgrade_task_process */
    (void)nv_upg_upgrade_task_process();
    thread_init();
#if !defined(BRANDY_MCU_BT)
    button_app_init();
    uapi_calendar_init();
#endif
#ifdef SUPPORT_OHOSFWK
    ohos_startup();
#endif
#ifdef CONFIG_SYSTEM_VIEW
    SEGGER_SYSVIEW_Conf();
    // 请在抓取systemview开始的地方调用SEGGER_SYSVIEW_Start接口
#endif
    uapi_pm_remove_sleep_veto(PM_ID_DEBUG);
    set_startup_finish_flag();
}

void thread_pri_init(void)
{
    /* Create tasks */
    g_thread_pri.task_handle = osThreadNew(g_thread_pri.func, NULL, &(g_thread_pri.attr));
    if (g_thread_pri.task_handle == NULL) {
        panic(PANIC_TASK_CREATE_FAILED, __LINE__);
    }
}
