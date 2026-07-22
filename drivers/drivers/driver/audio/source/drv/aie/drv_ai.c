/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv ai
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "core_module.h"
#include "drv_sap_ioctl.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_ai.h"
#include "drv_proc_ai.h"
#include "drv_ai.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_handle drv_ai;
    td_u64 context;
    td_u64 event_proc;
} drv_ai_inst;

static drv_ai_inst g_drv_ai_insts[MOD_AI_MAX_NUM];

static td_s32 drv_ai_find_inst(drv_ai_inst **ai_inst, td_handle ai)
{
    td_u8 ai_ind;
    sap_mod_id mod_id;
    drv_ai_inst *ai_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)ai;
    ai_ind = mod_id.bits.index;

    if (ai_ind >= MOD_AI_MAX_NUM) {
        return ERR_AI_INVALID_HANDLE;
    }
    ai_inst_p = &g_drv_ai_insts[ai_ind];

    if (ai_inst_p->drv_ai != ai) {
        return ERR_AI_INVALID_HANDLE;
    }

    *ai_inst = ai_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_ai_alloc_inst(drv_ai_inst **ai_inst, td_handle ai)
{
    td_u8 ai_ind;
    drv_ai_inst *ai_inst_p = TD_NULL;
    sap_mod_id mod_id;

    mod_id.u32 = (td_u32)ai;
    ai_ind = mod_id.bits.index;

    if (ai_ind >= MOD_AI_MAX_NUM) {
        return ERR_AI_INVALID_HANDLE;
    }
    ai_inst_p = &g_drv_ai_insts[ai_ind];

    if (ai_inst_p->drv_ai != 0) {
        return ERR_AI_NOT_AVAILABLE;
    }

    ai_inst_p->drv_ai = ai;
    *ai_inst = ai_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_ai_free_inst(td_handle ai)
{
    td_u8 ai_ind;
    sap_mod_id mod_id;
    drv_ai_inst *ai_inst_t = TD_NULL;

    mod_id.u32 = (td_u32)ai;
    ai_ind = mod_id.bits.index;

    if (ai_ind >= MOD_AI_MAX_NUM) {
        return ERR_AI_INVALID_HANDLE;
    }
    ai_inst_t = &g_drv_ai_insts[ai_ind];

    if (ai_inst_t->drv_ai != ai) {
        return ERR_AI_INVALID_HANDLE;
    }

    (td_void)memset_s(ai_inst_t, sizeof(*ai_inst_t), 0, sizeof(drv_ai_inst));

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_ai_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_init, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_ai_insts, sizeof(g_drv_ai_insts), 0, sizeof(g_drv_ai_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_ai_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_deinit, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_ai_insts, sizeof(g_drv_ai_insts), 0, sizeof(g_drv_ai_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_get_def_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_ai_open *ai_open;

    ai_open = (sap_ai_open *)arg;
    ret = drv_msg_ai_get_def_attr(ai_open->port, &ai_open->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_get_def_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_ai_open *ai_open = TD_NULL;
    drv_ai_inst *ai_inst = TD_NULL;

    ai_open = (sap_ai_open *)arg;

    ret = drv_msg_ai_open(&drv_ai, ai_open->port, &ai_open->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_ret(ret);
        return ret;
    }

    ret = drv_ai_alloc_inst(&ai_inst, drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ai_alloc_inst, ret);
        drv_msg_ai_close(drv_ai);
        return ret;
    }

    ai_inst->drv_ai = drv_ai;
    ai_open->h_mod = drv_ai;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_handle *ai_handle = TD_NULL;

    ai_handle = (sap_drv_handle *)arg;
    drv_ai = ai_handle->h_mod;

    ret = drv_msg_ai_close(drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_close, ret);
        return ret;
    }

    ret = drv_ai_free_inst(drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ai_free_inst, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_event *ai_event = TD_NULL;
    drv_ai_inst *ai_inst = TD_NULL;

    ai_event = (sap_drv_event *)arg;
    drv_ai = ai_event->h_mod;
    ret = drv_ai_find_inst(&ai_inst, drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_ai);
        sap_err_log_fun(drv_ai_find_inst, ret);
        return ret;
    }

    ai_inst->event_proc = ai_event->event_proc;
    ai_inst->context = ai_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_start(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_handle *ai_handle = TD_NULL;

    ai_handle = (sap_drv_handle *)arg;
    drv_ai = ai_handle->h_mod;

    ret = drv_msg_ai_start(drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_start, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_stop(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_handle *ai_handle = TD_NULL;

    ai_handle = (sap_drv_handle *)arg;
    drv_ai = ai_handle->h_mod;

    ret = drv_msg_ai_stop(drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_stop, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_ai_attr *ai_attr = TD_NULL;

    ai_attr = (sap_ai_attr *)arg;
    drv_ai = ai_attr->h_mod;

    ret = drv_msg_ai_get_attr(drv_ai, &ai_attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_get_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_ai_attr *ai_attr = TD_NULL;

    ai_attr = (sap_ai_attr *)arg;
    drv_ai = ai_attr->h_mod;

    ret = drv_msg_ai_set_attr(drv_ai, &ai_attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_set_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_set_bypass(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_enable *ai_enable = TD_NULL;

    ai_enable = (sap_drv_enable *)arg;
    drv_ai = ai_enable->h_mod;

    ret = drv_msg_ai_set_bypass(drv_ai, ai_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_set_bypass, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_reset_vad(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_handle *ai_handle = TD_NULL;

    ai_handle = (sap_drv_handle *)arg;
    drv_ai = ai_handle->h_mod;

    ret = drv_msg_ai_reset_vad(drv_ai);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_reset_vad, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_get_gain(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_ai_gain *ai_gain = TD_NULL;

    ai_gain = (sap_ai_gain *)arg;
    drv_ai = ai_gain->h_mod;

    ret = drv_msg_ai_get_gain(drv_ai, &ai_gain->gain);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_get_gain, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_set_gain(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_ai_gain *ai_gain = TD_NULL;

    ai_gain = (sap_ai_gain *)arg;
    drv_ai = ai_gain->h_mod;

    ret = drv_msg_ai_set_gain(drv_ai, &ai_gain->gain);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_set_gain, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_get_mute(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_enable *ai_enable = TD_NULL;

    ai_enable = (sap_drv_enable *)arg;
    drv_ai = ai_enable->h_mod;

    ret = drv_msg_ai_get_mute(drv_ai, &ai_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_get_mute, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_set_mute(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_enable *ai_enable = TD_NULL;

    ai_enable = (sap_drv_enable *)arg;
    drv_ai = ai_enable->h_mod;

    ret = drv_msg_ai_set_mute(drv_ai, ai_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_set_mute, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_set_mic_cali_volume(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_ai_cali_volume *sap_cali_volume = (sap_ai_cali_volume *)arg;

    ret = drv_msg_ai_set_mic_cali_volume(sap_cali_volume->h_mod, &sap_cali_volume->cali_volume);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_set_mic_cali_volume, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_ai = attach_para->h_src;

    ret = drv_msg_ai_attach_output(drv_ai, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_attach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_ai_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_ai;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_ai = attach_para->h_src;

    ret = drv_msg_ai_detach_output(drv_ai, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_ai_detach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

static td_s32 drv_msg_ai_report_event(core_msg_data *msg_data)
{
    td_s32 ret;
    msg_data_ai_event_info *ai_event = (msg_data_ai_event_info *)msg_data->input_data;
    td_handle ai_handle = msg_data->handle;
    drv_ai_inst *ai_inst = TD_NULL;
    audio_event_proc event_proc = TD_NULL;

    ret = drv_ai_find_inst(&ai_inst, ai_handle);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_ret(ret);
        return ret;
    }

    event_proc = (audio_event_proc)(td_uintptr_t)(ai_inst->event_proc);
    if (event_proc != TD_NULL) {
        event_proc(ai_handle, ai_event->event, &(ai_event->param), (td_void *)(td_uintptr_t)ai_inst->context);
    }

    return AUDIO_SUCCESS;
}

static const core_msg_module_func g_drv_msg_ai_func[MSG_DRV_AI_CMD_MAX] = {
    [MSG_DRV_AI_REPORT_EVENT] = {drv_msg_ai_report_event}
};

static td_s32 drv_msg_ai_callback(core_msg_data *msg_data)
{
    if (msg_data->cmd >= MSG_DRV_AI_CMD_MAX) {
        sap_err_log_u32(msg_data->cmd);
        return ERR_AI_INVALID_PARAM;
    }

    return g_drv_msg_ai_func[msg_data->cmd].func(msg_data);
}

td_s32 drv_msg_ai_module_init(audio_core_id core_id)
{
    td_s32 ret;
    ret = core_msg_register_module(core_id, MSG_CORE_AI_MODULE, drv_msg_ai_callback);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(core_msg_register_module, ret);
        return ERR_AI_INIT_FAILED;
    }
    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 ai_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    td_bool offline_flag = TD_FALSE;
    td_u8 ai_id;
    drv_ai_inst *ai_inst = TD_NULL;
    ai_proc_item proc;

    if (private != TD_NULL) {
        offline_flag = *(td_bool *)private;
    }

    for (ai_id = 0; ai_id < MOD_AI_MAX_NUM; ai_id++) {
        ai_inst = &g_drv_ai_insts[ai_id];
        if (ai_inst->drv_ai == 0) {
            continue;
        }

        (td_void)memset_s(&proc, sizeof(proc), 0, sizeof(proc));
        ret = drv_msg_ai_get_proc_info(ai_inst->drv_ai, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_msg_ai_get_proc_info, ret);
            continue;
        }

        if (offline_flag == TD_FALSE) {
            ret = drv_proc_ai_show(file, ai_inst->drv_ai, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_proc_ai_show, ret);
                continue;
            }
        } else {
#ifndef SW_UART_DEBUG
            ret = drv_diag_log_proc_ai_show(ai_inst->drv_ai, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_diag_log_proc_ai_show, ret);
                continue;
            }
#endif
        }
    }

    return AUDIO_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
