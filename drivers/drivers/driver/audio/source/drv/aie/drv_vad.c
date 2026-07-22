/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv vad
 * Author: audio
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "drv_sap_ioctl.h"
#include "core_util.h"
#include "core_module.h"
#include "include/core_ai_lib.h"
#include "drv_vad.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_handle inst_id;
    ai_dev_vad *func;
    ai_dev_handle handle;
    td_void *context;
    audio_event_proc event_proc;
} drv_vad_inst;

static drv_vad_inst g_vad_inst = {
    .inst_id = 0,
    .func = TD_NULL,
    .handle = TD_NULL,
    .context = TD_NULL,
    .event_proc = TD_NULL,
};

td_s32 drv_vad_intr_proc(td_void)
{
    drv_vad_inst *vad_inst = &g_vad_inst;

    if (vad_inst->inst_id == 0) {
        return ERR_AI_NOT_AVAILABLE;
    }

    if (vad_inst->event_proc != TD_NULL) {
        vad_inst->event_proc(vad_inst->inst_id, VAD_EVENT_VAD_VALID, TD_NULL, vad_inst->context);
    }

    return AUDIO_SUCCESS;
}

td_s32 drv_vad_set_enable(td_bool enable)
{
    drv_vad_inst *vad_inst = &g_vad_inst;

    if (vad_inst->inst_id == 0) {
        return ERR_AI_NOT_AVAILABLE;
    }

    return vad_inst->func->set_enable(vad_inst->handle, enable);
}

td_s32 sap_ioctl_vad_init(td_u32 cmd, td_void *arg, td_void *file)
{
    drv_vad_inst *vad_inst = &g_vad_inst;

    (td_void)memset_s(vad_inst, sizeof(*vad_inst), 0, sizeof(*vad_inst));
    vad_inst->func = TD_NULL;
    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    drv_vad_inst *vad_inst = &g_vad_inst;
    (td_void)memset_s(vad_inst, sizeof(*vad_inst), 0, sizeof(*vad_inst));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_get_def_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    sap_vad_attr *vad_attr = (sap_vad_attr *)arg;

    vad_attr->h_mod = 0;
    vad_attr->attr.always_on = TD_FALSE;
    vad_attr->attr.timeout = 0;
    vad_attr->attr.vad_type = VAD_DEV_AVAD;
    vad_attr->attr.vad_mode = VAD_NORM_MODE;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

static td_s32 vad_check_dev_type(vad_dev_type dev_type)
{
    if (dev_type >= VAD_DEV_TYPE_MAX) {
        return ERR_VAD_INVALID_PARAM;
    }

    return AUDIO_SUCCESS;
}

static td_s32 vad_check_work_mode(vad_work_mode work_mode)
{
    if (work_mode >= VAD_MODE_MAX) {
        return ERR_VAD_INVALID_PARAM;
    }

    return AUDIO_SUCCESS;
}
static td_s32 vad_check_parameter(sap_vad_attr *vad_attr)
{
    td_s32 ret;

    ret = core_check_bool(vad_attr->attr.always_on);
    if (ret != AUDIO_SUCCESS) {
        return ERR_VAD_INVALID_PARAM;
    }

    ret = vad_check_dev_type(vad_attr->attr.vad_type);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = vad_check_work_mode(vad_attr->attr.vad_mode);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_mod_id vad_id;
    drv_vad_inst *vad_inst = &g_vad_inst;
    sap_vad_attr *vad_attr = (sap_vad_attr *)arg;

    ai_check_null_ptr_return(vad_inst->func);
    if (vad_inst->inst_id != 0) {
        return ERR_VAD_OPEN_FAILED;
    }

    ret = vad_check_parameter(vad_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = vad_inst->func->open(&vad_inst->handle, AI_PORT_ADC0, TD_NULL, &vad_attr->attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    vad_id.u32 = 0;
    vad_id.bits.type = SAP_MOD_VAD;
    vad_id.bits.index = 0;

    vad_inst->inst_id = (td_handle)vad_id.u32;
    vad_attr->h_mod = vad_inst->inst_id;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_vad;
    sap_drv_handle *vad_handle = (sap_drv_handle *)arg;
    drv_vad_inst *vad_inst = &g_vad_inst;

    ai_check_null_ptr_return(vad_inst->func);

    drv_vad = vad_handle->h_mod;
    if (vad_inst->inst_id != drv_vad) {
        return ERR_AI_INVALID_HANDLE;
    }

    if (vad_inst->handle == TD_NULL) {
        vad_inst->inst_id = 0;
        vad_inst->context = TD_NULL;
        vad_inst->event_proc = TD_NULL;
        return AUDIO_SUCCESS;
    }

    ret = vad_inst->func->close(vad_inst->handle);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    vad_inst->inst_id = 0;
    vad_inst->handle = TD_NULL;
    vad_inst->context = TD_NULL;
    vad_inst->event_proc = TD_NULL;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_handle drv_vad;
    drv_vad_inst *vad_inst = &g_vad_inst;
    sap_drv_event *vad_event = (sap_drv_event *)arg;

    ai_check_null_ptr_return(vad_inst->func);

    drv_vad = vad_event->h_mod;
    if (vad_inst->inst_id != drv_vad) {
        return ERR_AI_INVALID_HANDLE;
    }

    if (vad_inst->handle == TD_NULL) {
        return ERR_AI_INVALID_HANDLE;
    }

    vad_inst->event_proc = (audio_event_proc)(td_uintptr_t)vad_event->event_proc;
    vad_inst->context = (td_void *)(td_uintptr_t)vad_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_vad_reset(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_vad;
    drv_vad_inst *vad_inst = &g_vad_inst;
    sap_drv_handle *vad_handle = (sap_drv_handle *)arg;

    ai_check_null_ptr_return(vad_inst->func);

    drv_vad = vad_handle->h_mod;
    if (vad_inst->inst_id != drv_vad) {
        return ERR_AI_INVALID_HANDLE;
    }

    if (vad_inst->handle == TD_NULL) {
        return ERR_AI_INVALID_HANDLE;
    }

    ret = vad_inst->func->set_enable(vad_inst->handle, TD_TRUE);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
