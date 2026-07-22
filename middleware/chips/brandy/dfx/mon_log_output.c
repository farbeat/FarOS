/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 * Description: log output monitor.
 */

#include <stdint.h>
#include <stdbool.h>
#include <log_module_id.h>
#include <diag_log.h>
#include <diag_msg.h>
#include <osal_timer.h>
#include "common_def.h"
#include "mon_log_output.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#define MODULEID_COUNT (MODULEID_BUTT + 2)
STATIC uint32_t g_mon_logcnt_max[MODULEID_COUNT] = { 0,
                                                     MAX_LOGCNT_BT,
                                                     0,
                                                     MAX_LOGCNT_DSP,
                                                     MAX_LOGCNT_PF,
                                                     MAX_LOGCNT_MEDIA,
                                                     0,
                                                     MAX_LOGCNT_APP,
                                                     MAX_LOGCNT_GPU,
                                                     MAX_LOGCNT_GUI,
                                                     0,
                                                     MAX_LOGCNT_BTH,
                                                     MAX_LOGCNT_OHOS,
                                                     0,
                                                     MAX_LOGCNT_BT_STATUS,
                                                     MAX_LOGCNT_BT_OTA };
STATIC uint32_t g_mon_logcnt_ctrl[MODULEID_COUNT] = { 0 };
STATIC osal_timer g_logcnt_timer = { 0 };
STATIC bool g_logcnt_en = false;

STATIC void mon_logcnt_enable(void)
{
    for (uint32_t i = 0; i < MODULEID_COUNT; i++) {
        g_mon_logcnt_ctrl[i] = 0;
    }

    osal_timer_start(&g_logcnt_timer);
}

STATIC void mon_log_output_timer_alert(unsigned long data)
{
    unused(data);
    mon_logcnt_enable();
}

STATIC bool mon_logcnt_process(uint8_t mid, uint8_t level)
{
    unused(level);
    if (mid >= MODULEID_COUNT) {
        return false;
    }

    if ((g_logcnt_en == true) && (g_mon_logcnt_max[mid] < g_mon_logcnt_ctrl[mid])) {
        return false;
    }

    g_mon_logcnt_ctrl[mid]++;
    return true;
}

void mon_log_output_ctrl_init(void)
{
    g_logcnt_timer.handler = mon_log_output_timer_alert;
    g_logcnt_timer.data = 0;
    g_logcnt_timer.interval = MON_LOG_CNT_CLEAR_TIMEOUT;

    if (osal_timer_init(&g_logcnt_timer) < 0) {
        uapi_diag_error_log(0, "Create logcnt timer failed!");
        return;
    }

    /* register log file hook */
    uapi_diag_register_msg_flow_control_hook(mon_logcnt_process);

    osal_timer_start(&g_logcnt_timer);
}

uint32_t *mon_log_output_get_counter(void)
{
    return &g_mon_logcnt_ctrl;
}

void mon_log_output_enable(bool enable)
{
    if (g_logcnt_en == enable) {
        return; /* Same settings, do nothing */
    }

    g_logcnt_en = enable;

    if (enable == true) {
        mon_logcnt_enable(); /* start over counting */
    } else {
        osal_timer_stop(&g_logcnt_timer); /* continue counting, but do not clear */
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

