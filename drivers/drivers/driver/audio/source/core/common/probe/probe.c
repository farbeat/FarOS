/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: probe of sys status
 * Author: audio
 */

#include "probe.h"
#include "chip_type.h"
#include "mem_addr.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "securec.h"

typedef struct {
    probe_status_info core_status[DSP_CORE_NUM];
} probe_context;

static volatile probe_context *g_probe_ctx = TD_NULL;

td_s32 probe_status_init(td_void)
{
    audio_func_enter();

    if (g_probe_ctx == TD_NULL) {
        g_probe_ctx = (volatile probe_context *)audio_osal_address_map(SOC_IPC_PROBE_STATUS_ADDR,
            sizeof(probe_context));
        if (g_probe_ctx == TD_NULL) {
            audio_log_err("probe_context map failed.");
            return AUDIO_FAILURE;
        }
    }

    audio_func_exit();
    return AUDIO_SUCCESS;
}

td_void probe_status_deinit(td_void)
{
    audio_func_enter();

    if (g_probe_ctx != TD_NULL) {
        audio_osal_address_unmap(SOC_IPC_PROBE_STATUS_ADDR, (td_void *)g_probe_ctx);
        g_probe_ctx = TD_NULL;
    }

    audio_func_exit();
}

td_void probe_add_heartbeat(audio_core_id core_id)
{
    volatile td_u32 *heartbeat = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        heartbeat = &(g_probe_ctx->core_status[DSP_CORE0].heartbeat);
    } else {
        heartbeat = &(g_probe_ctx->core_status[DSP_CORE1].heartbeat);
    }
    *heartbeat += 1;
}

td_u32 probe_get_heartbeat(audio_core_id core_id)
{
    if (g_probe_ctx == TD_NULL) {
        return 0;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        return g_probe_ctx->core_status[DSP_CORE0].heartbeat;
    } else {
        return g_probe_ctx->core_status[DSP_CORE1].heartbeat;
    }
}

td_void probe_update_run_step(audio_core_id core_id, audio_run_step run_step)
{
    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        g_probe_ctx->core_status[DSP_CORE0].run_step = run_step;
    } else {
        g_probe_ctx->core_status[DSP_CORE1].run_step = run_step;
    }
}

td_void probe_update_sys_state(audio_core_id core_id, audio_sys_state sys_state)
{
    volatile probe_status_info *status = TD_NULL;
    volatile td_u32 *cnt = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        g_probe_ctx->core_status[DSP_CORE0].sys_state = sys_state;
        status = &(g_probe_ctx->core_status[DSP_CORE0]);
    } else {
        g_probe_ctx->core_status[DSP_CORE1].sys_state = sys_state;
        status = &(g_probe_ctx->core_status[DSP_CORE1]);
    }

    switch (sys_state) {
        case AUDIO_SYS_ACTIVE:
            cnt = &(status->sys_active);
            break;

        case AUDIO_SYS_ACTIVE_STANDBY:
            cnt = &(status->sys_active_standby);
            break;

        case AUDIO_SYS_DEEP_STANDBY:
            cnt = &(status->sys_deep_standby);
            break;

        default:
            return;
    }
	*cnt += 1;
}

td_void probe_add_log_success(audio_core_id core_id)
{
    volatile td_u32 *log_success = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        log_success = &(g_probe_ctx->core_status[DSP_CORE0].log_success);
    } else {
        log_success = &(g_probe_ctx->core_status[DSP_CORE1].log_success);
    }
    *log_success += 1;
}

td_void probe_add_log_busy(audio_core_id core_id)
{
    volatile td_u32 *log_busy = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        log_busy = &(g_probe_ctx->core_status[DSP_CORE0].log_busy);
    } else {
        log_busy = &(g_probe_ctx->core_status[DSP_CORE1].log_busy);
    }
    *log_busy += 1;
}

td_void probe_add_log_len(audio_core_id core_id, td_u32 log_len)
{
    volatile td_u32 *cur_len = TD_NULL;
    volatile td_u32 *total_len = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        cur_len = &(g_probe_ctx->core_status[DSP_CORE0].log_cur_len);
        total_len = &(g_probe_ctx->core_status[DSP_CORE0].log_total_len);
    } else {
        cur_len = &(g_probe_ctx->core_status[DSP_CORE1].log_cur_len);
        total_len = &(g_probe_ctx->core_status[DSP_CORE1].log_total_len);
    }
    *cur_len = log_len;
    *total_len += log_len;
}

td_void probe_add_msg_success(audio_core_id core_id)
{
    volatile td_u32 *msg_success = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        msg_success = &(g_probe_ctx->core_status[DSP_CORE0].msg_success);
    } else {
        msg_success = &(g_probe_ctx->core_status[DSP_CORE1].msg_success);
    }
    *msg_success += 1;
}

td_void probe_add_msg_illegal_param(audio_core_id core_id)
{
    volatile td_u32 *illegal_param = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        illegal_param = &(g_probe_ctx->core_status[DSP_CORE0].msg_illegal_param);
    } else {
        illegal_param = &(g_probe_ctx->core_status[DSP_CORE1].msg_illegal_param);
    }
    *illegal_param += 1;
}

td_void probe_add_msg_ack_timeout(audio_core_id core_id)
{
    volatile td_u32 *ack_timeout = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        ack_timeout = &(g_probe_ctx->core_status[DSP_CORE0].msg_ack_timeout);
    } else {
        ack_timeout = &(g_probe_ctx->core_status[DSP_CORE1].msg_ack_timeout);
    }
    *ack_timeout += 1;
}

td_void probe_add_msg_underflow(audio_core_id core_id)
{
    volatile td_u32 *underflow = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        underflow = &(g_probe_ctx->core_status[DSP_CORE0].msg_underflow);
    } else {
        underflow = &(g_probe_ctx->core_status[DSP_CORE1].msg_underflow);
    }
    *underflow += 1;
}

td_void probe_add_msg_crc_fail(audio_core_id core_id)
{
    volatile td_u32 *crc_fail = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        crc_fail = &(g_probe_ctx->core_status[DSP_CORE0].msg_crc_fail);
    } else {
        crc_fail = &(g_probe_ctx->core_status[DSP_CORE1].msg_crc_fail);
    }
    *crc_fail += 1;
}

td_void probe_add_msg_int_fail(audio_core_id core_id)
{
    volatile td_u32 *msg_int_fail = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        msg_int_fail = &(g_probe_ctx->core_status[DSP_CORE0].msg_int_fail);
    } else {
        msg_int_fail = &(g_probe_ctx->core_status[DSP_CORE1].msg_int_fail);
    }
    *msg_int_fail += 1;
}

td_void probe_add_msg_int_retrigger(audio_core_id core_id)
{
    volatile td_u32 *msg_int_retrigger = TD_NULL;

    if (g_probe_ctx == TD_NULL) {
        return;
    }

    if (core_id == AUDIO_CORE_DSP0) {
        msg_int_retrigger = &(g_probe_ctx->core_status[DSP_CORE0].msg_int_retrigger);
    } else {
        msg_int_retrigger = &(g_probe_ctx->core_status[DSP_CORE1].msg_int_retrigger);
    }
    *msg_int_retrigger += 1;
}

