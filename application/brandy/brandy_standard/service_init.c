
/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Application core board init function for standard
 * Author:
 * Create:
 */

#ifdef __LITEOS__
#include "partition.h"
#endif
#include "pm.h"
#include "cpu_utils.h"
#include "log_common.h"
#include "log_uart.h"
#include "exception.h"
#include "log_oml_exception.h"
#ifdef HSO_SUPPORT
#include "dfx_system_init.h"
#endif
#include "debug_print.h"
#ifdef SUPPORT_AT_GRAPHIC
#include "graphic_cmd_manager.h"
#endif
#ifdef SUPPORT_AT_JSINSTALL
#include "ohosfwk_at_process.h"
#endif
#ifdef SUPPORT_AT_APP
#include "app_cmd_manager.h"
#endif
#ifdef SUPPORT_AT_AUDIO
#include "audio_cmd_manager.h"
#endif
#ifdef SUPPORT_AT_BT_MANAGER
#include "bt_manager_cmd_manager.h"
#endif
#ifdef SUPPORT_AT_MEDIA
#include "media_cmd_manager.h"
#endif
#include "log_file.h"
#ifdef CONFIG_OTA_UPDATE_SUPPORT
#include "upg.h"
#endif
static void register_at_service(void)
{
#ifdef SUPPORT_AT_GRAPHIC
    uint32_t graphic_cmd_count;
    at_cmd_table_t *graphic_table = get_graphic_cmd_table(&graphic_cmd_count);
    at_cmd_add_moudle(graphic_table, graphic_cmd_count);
#endif
#ifdef SUPPORT_AT_JSINSTALL
    uint32_t ohosfwk_cmd_count;
    at_cmd_table_t *ohosfwk_table = get_ohosfwk_cmd_table(&ohosfwk_cmd_count);
    at_cmd_add_moudle(ohosfwk_table, ohosfwk_cmd_count);
#endif
#ifdef SUPPORT_AT_APP
    uint32_t app_cmd_count;
    at_cmd_table_t *app_table = get_app_cmd_table(&app_cmd_count);
    at_cmd_add_moudle(app_table, app_cmd_count);
#endif
#ifdef SUPPORT_AT_AUDIO
    uint32_t audio_cmd_count;
    at_cmd_table_t *audio_table = get_audio_cmd_table(&audio_cmd_count);
    at_cmd_add_moudle(audio_table, audio_cmd_count);
#endif
#ifdef SUPPORT_AT_BT_MANAGER
    uint32_t bt_manager_cmd_count;
    at_cmd_table_t *bt_manager_table = get_bt_manager_cmd_table(&bt_manager_cmd_count);
    at_cmd_add_moudle(bt_manager_table, bt_manager_cmd_count);
#endif
#ifdef SUPPORT_AT_MEDIA
    uint32_t media_cmd_count;
    at_cmd_table_t *media_table = get_media_cmd_table(&media_cmd_count);
    at_cmd_add_moudle(media_table, media_cmd_count);
#endif
}

#ifdef CONFIG_OTA_UPDATE_SUPPORT
static void upg_putc(const char c)
{
    printf("%c", c);
}
#endif

void service_init(void)
{
#if defined(FREERTOS_TEST)
    // freertos demo test
    extern int main_full(void);
    (void)main_full();
    for (;;) {
    }
#endif

    /* dfx init step 1: log init */
    log_init();
    log_init_after_rtos();
    log_uart_init_after_rtos();

    cpu_utils_init();
    hal_register_exception_dump_callback(log_exception_dump);
    uapi_partition_init();
    PRINT("partition init success"NEWLINE);

#ifdef CONFIG_OTA_UPDATE_SUPPORT
    upg_func_t upg_func = {0};
    upg_func.malloc = malloc;
    upg_func.free = free;
    upg_func.serial_putc = upg_putc;
    (void)uapi_upg_init(&upg_func);
    PRINT("upg init success\r\n");
#endif

    uapi_nv_init();
#ifdef SUPPORT_AT_BT_MANAGER
    set_calibration_from_nv();
#endif
    /* dfx init step 2: dfx init */
#ifdef HSO_SUPPORT
    dfx_system_init();
#endif
    /* power manager init */
    uapi_pm_init();
#ifdef SUPPORT_POWER_MANAGER
    uapi_power_display_init();
#endif

    register_at_service();
}
