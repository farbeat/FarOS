/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv dpm msg
 * Author: audio
 */

#include "dpm.h"
#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_module.h"
#include "drv_sap_ioctl.h"
#include "drv_overlay_load.h"
#include "drv_audio_osal.h"
#include "drv_msg_dpm.h"
#include "drv_proc_dpm.h"
#include "drv_dpm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_handle drv_dpm;
    td_u64 context;
    td_u64 event_proc;
} drv_dpm_inst;

static drv_dpm_inst g_drv_dpm_insts[MOD_DPM_MAX_NUM];

static td_s32 drv_dpm_find_inst(drv_dpm_inst **dpm_inst, td_handle dpm)
{
    td_u8 dpm_ind;
    sap_mod_id mod_id;
    drv_dpm_inst *dpm_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)dpm;
    dpm_ind = mod_id.bits.index;

    if (dpm_ind >= MOD_DPM_MAX_NUM) {
        return ERR_DPM_INVALID_HANDLE;
    }
    dpm_inst_p = &g_drv_dpm_insts[dpm_ind];

    if (dpm_inst_p->drv_dpm != dpm) {
        return ERR_DPM_INVALID_HANDLE;
    }

    *dpm_inst = dpm_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_dpm_alloc_inst(drv_dpm_inst **dpm_inst, td_handle dpm)
{
    td_u8 dpm_ind;
    drv_dpm_inst *dpm_inst_p = TD_NULL;
    sap_mod_id mod_id;

    mod_id.u32 = (td_u32)dpm;
    dpm_ind = mod_id.bits.index;

    if (dpm_ind >= MOD_DPM_MAX_NUM) {
        return ERR_DPM_INVALID_HANDLE;
    }
    dpm_inst_p = &g_drv_dpm_insts[dpm_ind];

    if (dpm_inst_p->drv_dpm != 0) {
        return ERR_DPM_NOT_AVAILABLE;
    }

    dpm_inst_p->drv_dpm = dpm;
    *dpm_inst = dpm_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_dpm_free_inst(td_handle dpm)
{
    td_u8 dpm_ind;
    sap_mod_id mod_id;
    drv_dpm_inst *dpm_inst_t = TD_NULL;

    mod_id.u32 = (td_u32)dpm;
    dpm_ind = mod_id.bits.index;

    if (dpm_ind >= MOD_DPM_MAX_NUM) {
        return ERR_DPM_INVALID_HANDLE;
    }
    dpm_inst_t = &g_drv_dpm_insts[dpm_ind];

    if (dpm_inst_t->drv_dpm != dpm) {
        return ERR_DPM_INVALID_HANDLE;
    }

    (td_void)memset_s(dpm_inst_t, sizeof(*dpm_inst_t), 0, sizeof(drv_dpm_inst));

    return AUDIO_SUCCESS;
}

static audio_lib_id drv_dpm_get_lib_id(dpm_id dpm_plugin_id)
{
    td_u32 i;
    const struct {
        dpm_id dpm_plugin_id;
        audio_lib_id lib_id;
    } tab[] = {
        {DPM_ID_FIR, AUDIO_LIB_DPM_FIR},
    };

    for (i = 0; i < sizeof(tab) / sizeof(tab[0]); i++) {
        if (tab[i].dpm_plugin_id == dpm_plugin_id) {
            return tab[i].lib_id;
        }
    }

    return AUDIO_LIB_MAX;
}

td_s32 sap_ioctl_dpm_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_dpm_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_init, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_dpm_insts, sizeof(g_drv_dpm_insts), 0, sizeof(g_drv_dpm_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_dpm_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_deinit, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_dpm_insts, sizeof(g_drv_dpm_insts), 0, sizeof(g_drv_dpm_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

static td_s32 drv_dpm_load_lib(td_handle dpm, dpm_id dpm_id)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;

    ret = drv_msg_dpm_get_lib_type(dpm_id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_dpm_get_lib_id(dpm_id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(dpm_id);
        return ERR_DPM_NOT_SUPPORTED;
    }

    ret = drv_overlay_request_firmware(dpm, OVERLAY_CODEC);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_request_firmware, ret);
        return ret;
    }

    ret = drv_overlay_load_lib_program(AUDIO_CORE_DSP0, lib_id, dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_lib_program, ret);
        drv_overlay_release_firmware(dpm, OVERLAY_CODEC);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_load_lib(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    ret = drv_msg_dpm_get_lib_type((dpm_id)drv_load->id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_dpm_get_lib_id((dpm_id)drv_load->id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(drv_load->id);
        return ERR_DPM_NOT_SUPPORTED;
    }

    ret = drv_overlay_request_firmware(drv_load->h_mod, OVERLAY_CODEC);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_request_firmware, ret);
        return ret;
    }

    ret = drv_overlay_load_lib_program(AUDIO_CORE_DSP0, lib_id, drv_load->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_lib_program, ret);
        drv_overlay_release_firmware(drv_load->h_mod, OVERLAY_CODEC);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_unload_lib(td_u32 cmd, td_void *arg, td_void *file)
{
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    drv_overlay_release_firmware(drv_load->h_mod, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_dpm_open *dpm_open = TD_NULL;
    drv_dpm_inst *dpm_inst = TD_NULL;

    dpm_open = (sap_dpm_open *)arg;

    ret = drv_msg_dpm_open(&drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_open, ret);
        return ret;
    }

    ret = drv_dpm_alloc_inst(&dpm_inst, drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_dpm_alloc_inst, ret);
        drv_msg_dpm_close(drv_dpm);
        return ret;
    }

    dpm_inst->drv_dpm = drv_dpm;
    dpm_open->h_mod = drv_dpm;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_handle *dpm_handle = TD_NULL;

    dpm_handle = (sap_drv_handle *)arg;
    drv_dpm = dpm_handle->h_mod;

    ret = drv_msg_dpm_close(drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_close, ret);
        return ret;
    }

    ret = drv_dpm_free_inst(drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_dpm_free_inst, ret);
    }

    drv_overlay_release_firmware(drv_dpm, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_event *dpm_event = TD_NULL;
    drv_dpm_inst *dpm_inst = TD_NULL;

    dpm_event = (sap_drv_event *)arg;
    drv_dpm = dpm_event->h_mod;
    ret = drv_dpm_find_inst(&dpm_inst, drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_dpm);
        sap_err_log_fun(drv_dpm_find_inst, ret);
        return ret;
    }

    dpm_inst->event_proc = dpm_event->event_proc;
    dpm_inst->context = dpm_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_start(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_handle *dpm_handle = TD_NULL;

    dpm_handle = (sap_drv_handle *)arg;
    drv_dpm = dpm_handle->h_mod;

    ret = drv_msg_dpm_start(drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_start, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_stop(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_handle *dpm_handle = TD_NULL;

    dpm_handle = (sap_drv_handle *)arg;
    drv_dpm = dpm_handle->h_mod;

    ret = drv_msg_dpm_stop(drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_stop, ret);
        return ret;
    }

    drv_overlay_release_firmware(drv_dpm, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_dpm_attr *attr = TD_NULL;

    attr = (sap_dpm_attr *)arg;
    drv_dpm = attr->h_mod;

    ret = drv_msg_dpm_get_attr(drv_dpm, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_get_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_dpm_attr *attr = TD_NULL;

    attr = (sap_dpm_attr *)arg;
    drv_dpm = attr->h_mod;

    ret = drv_msg_dpm_stop(drv_dpm);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_stop, ret);
        return ret;
    }

    ret = drv_dpm_load_lib(drv_dpm, attr->attr.dpm_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_dpm_load_lib, ret);
        return ret;
    }

    ret = drv_msg_dpm_set_attr(drv_dpm, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_set_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_set_bypass(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_enable *dpm_enable = TD_NULL;

    dpm_enable = (sap_drv_enable *)arg;
    drv_dpm = dpm_enable->h_mod;

    ret = drv_msg_dpm_set_bypass(drv_dpm, dpm_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_dpm = attach_para->h_src;

    ret = drv_msg_dpm_attach_output(drv_dpm, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_attach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_dpm_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_dpm;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_dpm = attach_para->h_src;

    ret = drv_msg_dpm_detach_output(drv_dpm, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_dpm_detach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 dpm_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    td_u8 dpm_ind;
    drv_dpm_inst *dpm_inst = TD_NULL;
    dpm_proc_item proc;

    audio_unused(private);

    for (dpm_ind = 0; dpm_ind < MOD_DPM_MAX_NUM; dpm_ind++) {
        dpm_inst = &g_drv_dpm_insts[dpm_ind];
        if (dpm_inst->drv_dpm == 0) {
            continue;
        }

        (td_void)memset_s(&proc, sizeof(proc), 0, sizeof(proc));
        ret = drv_msg_dpm_get_proc_info(dpm_inst->drv_dpm, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_msg_dpm_get_proc_info, ret);
            continue;
        }

        ret = drv_proc_dpm_show(file, dpm_inst->drv_dpm, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_proc_dpm_show, ret);
            continue;
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
