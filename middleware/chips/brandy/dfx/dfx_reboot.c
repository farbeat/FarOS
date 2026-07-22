/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: reboot module.
 * Author: @CompanyNameTag
 */

#include <stdint.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "errcode.h"
#include "hal_reboot.h"
#include "non_os_reboot.h"
#ifdef SUPPORT_POWER_MANAGER
#include "power_display_service.h"
#endif
#include "dfx_reboot.h"

#define SELF_HEAL_WAIT_TIME 100
#define REBOOT_WAIT_SCREEN_OFF_TIME 200

static process_callback_func g_reboot_process_cb = NULL;
static process_callback_func g_shutdown_process_cb = NULL;

static void default_process_pre_action(void)
{
#if defined(SUPPORT_POWER_MANAGER)
    const power_display_svr_api_t *display_api = power_display_svr_get_api();
    if (display_api->get_screen_state() == SCREEN_ON) {
        display_api->turn_off_screen();
    }
#endif
    osDelay(REBOOT_WAIT_SCREEN_OFF_TIME);
}

void uapi_register_pre_reboot_process_cb(process_callback_func callback)
{
    g_reboot_process_cb = callback;
    return;
}

void uapi_register_pre_shutdown_process_cb(process_callback_func callback)
{
    g_shutdown_process_cb = callback;
    return;
}

void uapi_system_reboot(reboot_type_t type)
{
    unused(type);
    if (g_reboot_process_cb != NULL) {
        g_reboot_process_cb();
    } else {
        default_process_pre_action();
    }
    reset_deal_with_fs();
    set_cpu_utils_reset_cause(REBOOT_CAUSE_APPLICATION_SYSRESETREQ);
    hal_reboot_chip();
    return;
}

void uapi_system_shutdown(uint32_t arg)
{
    unused(arg);
    if (g_shutdown_process_cb != NULL) {
        g_shutdown_process_cb();
    } else {
        default_process_pre_action();
    }
    uapi_sys_shipmode(0);
    uapi_tcxo_delay_ms(SELF_HEAL_WAIT_TIME);
    return;
}
