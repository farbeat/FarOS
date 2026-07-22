/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv aenc msg
 * Author: audio
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_module.h"
#include "drv_sap_ioctl.h"
#include "drv_overlay_load.h"
#include "drv_audio_osal.h"
#include "drv_msg_aenc.h"
#include "drv_proc_aenc.h"
#include "drv_aenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_handle drv_aenc;
    td_u64 context;
    td_u64 event_proc;
    core_overlay_id overlay_id;
} drv_aenc_inst;

static drv_aenc_inst g_drv_aenc_insts[MOD_AENC_MAX_NUM];

static td_s32 drv_aenc_find_inst(drv_aenc_inst **aenc_inst, td_handle aenc)
{
    td_u8 aenc_ind;
    sap_mod_id mod_id;
    drv_aenc_inst *aenc_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)aenc;
    aenc_ind = mod_id.bits.index;

    if (aenc_ind >= MOD_AENC_MAX_NUM) {
        return ERR_AENC_INVALID_HANDLE;
    }
    aenc_inst_p = &g_drv_aenc_insts[aenc_ind];

    if (aenc_inst_p->drv_aenc != aenc) {
        return ERR_AENC_INVALID_HANDLE;
    }

    *aenc_inst = aenc_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_aenc_alloc_inst(drv_aenc_inst **aenc_inst, td_handle aenc)
{
    td_u8 aenc_ind;
    drv_aenc_inst *aenc_inst_p = TD_NULL;
    sap_mod_id mod_id;

    mod_id.u32 = (td_u32)aenc;
    aenc_ind = mod_id.bits.index;

    if (aenc_ind >= MOD_AENC_MAX_NUM) {
        return ERR_AENC_INVALID_HANDLE;
    }
    aenc_inst_p = &g_drv_aenc_insts[aenc_ind];

    if (aenc_inst_p->drv_aenc != 0) {
        return ERR_AENC_NOT_AVAILABLE;
    }

    aenc_inst_p->drv_aenc = aenc;
    *aenc_inst = aenc_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_aenc_free_inst(td_handle aenc)
{
    td_u8 aenc_ind;
    sap_mod_id mod_id;
    drv_aenc_inst *aenc_inst_t = TD_NULL;

    mod_id.u32 = (td_u32)aenc;
    aenc_ind = mod_id.bits.index;

    if (aenc_ind >= MOD_AENC_MAX_NUM) {
        return ERR_AENC_INVALID_HANDLE;
    }
    aenc_inst_t = &g_drv_aenc_insts[aenc_ind];

    if (aenc_inst_t->drv_aenc != aenc) {
        return ERR_AENC_INVALID_HANDLE;
    }

    (td_void)memset_s(aenc_inst_t, sizeof(*aenc_inst_t), 0, sizeof(drv_aenc_inst));

    return AUDIO_SUCCESS;
}

static audio_lib_id drv_aenc_get_lib_id(ha_codec_id codec_id)
{
    td_u32 i;
    const struct {
        ha_codec_id acodec_id;
        audio_lib_id lib_id;
    } tab[] = {
        {HA_CODEC_ID_PCM, AUDIO_LIB_ENC_PCM},
        {HA_CODEC_ID_MSBC, AUDIO_LIB_ENC_SBC},
        {HA_CODEC_ID_SBC, AUDIO_LIB_ENC_SBC},
        {HA_CODEC_ID_MP3, AUDIO_LIB_ENC_MP3},
        {HA_CODEC_ID_AAC, AUDIO_LIB_DEC_AAC},
        {HA_CODEC_ID_OPUS, AUDIO_LIB_ENC_OPUS},
        {HA_CODEC_ID_SILK, AUDIO_LIB_ENC_SILK},
        {HA_CODEC_ID_LC3, AUDIO_LIB_ENC_LC3},
        {HA_CODEC_ID_L2HC, AUDIO_LIB_ENC_L2HC},
        {HA_CODEC_ID_AMRWB, AUDIO_LIB_ENC_AMRWB},
    };

    for (i = 0; i < sizeof(tab) / sizeof(tab[0]); i++) {
        if (tab[i].acodec_id == codec_id) {
            return tab[i].lib_id;
        }
    }

    return AUDIO_LIB_MAX;
}

td_s32 sap_ioctl_aenc_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_aenc_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_init, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_aenc_insts, sizeof(g_drv_aenc_insts), 0, sizeof(g_drv_aenc_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_aenc_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_deinit, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_aenc_insts, sizeof(g_drv_aenc_insts), 0, sizeof(g_drv_aenc_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

static td_s32 drv_aenc_load_codec(td_handle aenc, ha_codec_id codec_id)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;
    drv_aenc_inst *aenc_inst = TD_NULL;

    ret = drv_msg_aenc_get_lib_type(codec_id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_aenc_get_lib_id(codec_id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(codec_id);
        return ERR_AENC_NOT_SUPPORTED;
    }

    ret = drv_aenc_find_inst(&aenc_inst, aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(aenc);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    /* Only amrwb enc uses extra codec section */
    aenc_inst->overlay_id = (codec_id == HA_CODEC_ID_AMRWB) ? OVERLAY_EXTCODEC : OVERLAY_CODEC;
    ret = drv_overlay_request_firmware(aenc, aenc_inst->overlay_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_request_firmware, ret);
        return ret;
    }

    ret = drv_overlay_load_lib_program(AUDIO_CORE_DSP0, lib_id, aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_lib_program, ret);
        drv_overlay_release_firmware(aenc, aenc_inst->overlay_id);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_load_codec(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;
    drv_aenc_inst *aenc_inst = TD_NULL;
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    ret = drv_msg_aenc_get_lib_type((ha_codec_id)drv_load->id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_aenc_get_lib_id((ha_codec_id)drv_load->id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(drv_load->id);
        return ERR_AENC_NOT_SUPPORTED;
    }

    ret = drv_aenc_find_inst(&aenc_inst, drv_load->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_load->h_mod);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    /* Only amrwb enc uses extra codec section */
    aenc_inst->overlay_id = ((ha_codec_id)drv_load->id == HA_CODEC_ID_AMRWB) ? OVERLAY_EXTCODEC : OVERLAY_CODEC;
    ret = drv_overlay_request_firmware(drv_load->h_mod, aenc_inst->overlay_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_request_firmware, ret);
        return ret;
    }

    ret = drv_overlay_load_lib_program(AUDIO_CORE_DSP0, lib_id, drv_load->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_lib_program, ret);
        drv_overlay_release_firmware(drv_load->h_mod, aenc_inst->overlay_id);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_unload_codec(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    drv_aenc_inst *aenc_inst = TD_NULL;
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    ret = drv_aenc_find_inst(&aenc_inst, drv_load->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_load->h_mod);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    drv_overlay_release_firmware(drv_load->h_mod, aenc_inst->overlay_id);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_aenc_open *aenc_open = TD_NULL;
    drv_aenc_inst *aenc_inst = TD_NULL;

    aenc_open = (sap_aenc_open *)arg;

    ret = drv_msg_aenc_open(&drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_open, ret);
        return ret;
    }

    ret = drv_aenc_alloc_inst(&aenc_inst, drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_aenc_alloc_inst, ret);
        drv_msg_aenc_close(drv_aenc);
        return ret;
    }

    aenc_inst->drv_aenc = drv_aenc;
    aenc_open->h_mod = drv_aenc;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_handle *aenc_handle = TD_NULL;
    drv_aenc_inst *aenc_inst = TD_NULL;

    aenc_handle = (sap_drv_handle *)arg;
    drv_aenc = aenc_handle->h_mod;

    ret = drv_msg_aenc_close(drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_close, ret);
        return ret;
    }

    ret = drv_aenc_find_inst(&aenc_inst, drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_aenc);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    drv_overlay_release_firmware(drv_aenc, aenc_inst->overlay_id);

    ret = drv_aenc_free_inst(drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_aenc_free_inst, ret);
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_event *aenc_event = TD_NULL;
    drv_aenc_inst *aenc_inst = TD_NULL;

    aenc_event = (sap_drv_event *)arg;
    drv_aenc = aenc_event->h_mod;
    ret = drv_aenc_find_inst(&aenc_inst, drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_aenc);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    aenc_inst->event_proc = aenc_event->event_proc;
    aenc_inst->context = aenc_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_start(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_handle *aenc_handle = TD_NULL;

    aenc_handle = (sap_drv_handle *)arg;
    drv_aenc = aenc_handle->h_mod;

    ret = drv_msg_aenc_start(drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_start, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_stop(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_handle *aenc_handle = TD_NULL;
    drv_aenc_inst *aenc_inst = TD_NULL;

    aenc_handle = (sap_drv_handle *)arg;
    drv_aenc = aenc_handle->h_mod;

    ret = drv_msg_aenc_stop(drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_stop, ret);
        return ret;
    }

    ret = drv_aenc_find_inst(&aenc_inst, drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(drv_aenc);
        sap_err_log_fun(drv_aenc_find_inst, ret);
        return ret;
    }

    drv_overlay_release_firmware(drv_aenc, aenc_inst->overlay_id);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_aenc_attr *attr = TD_NULL;

    attr = (sap_aenc_attr *)arg;
    drv_aenc = attr->h_mod;

    ret = drv_msg_aenc_get_attr(drv_aenc, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_get_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_aenc_attr *attr = TD_NULL;

    attr = (sap_aenc_attr *)arg;
    drv_aenc = attr->h_mod;

    ret = drv_msg_aenc_stop(drv_aenc);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_stop, ret);
        return ret;
    }

    ret = drv_aenc_load_codec(drv_aenc, attr->attr.codec_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_aenc_load_codec, ret);
        return ret;
    }

    ret = drv_msg_aenc_set_attr(drv_aenc, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_set_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_set_bypass(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_enable *aenc_enable = TD_NULL;

    aenc_enable = (sap_drv_enable *)arg;
    drv_aenc = aenc_enable->h_mod;

    ret = drv_msg_aenc_set_bypass(drv_aenc, aenc_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_aenc = attach_para->h_src;

    ret = drv_msg_aenc_attach_output(drv_aenc, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_attach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_aenc = attach_para->h_src;

    ret = drv_msg_aenc_detach_output(drv_aenc, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_detach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_aenc_set_param(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_aenc;
    sap_aenc_param *attr = TD_NULL;

    attr = (sap_aenc_param *)arg;
    drv_aenc = attr->h_mod;

    ret = drv_msg_aenc_set_param(drv_aenc, attr->type, attr->param, attr->param_size);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_aenc_set_param, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 aenc_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    td_bool offline_flag = TD_FALSE;
    td_u8 aenc_ind;
    drv_aenc_inst *aenc_inst = TD_NULL;
    aenc_proc_item proc;

    if (private != TD_NULL) {
        offline_flag = *(td_bool *)private;
    }

    for (aenc_ind = 0; aenc_ind < MOD_AENC_MAX_NUM; aenc_ind++) {
        aenc_inst = &g_drv_aenc_insts[aenc_ind];
        if (aenc_inst->drv_aenc == 0) {
            continue;
        }

        (td_void)memset_s(&proc, sizeof(proc), 0, sizeof(proc));
        ret = drv_msg_aenc_get_proc_info(aenc_inst->drv_aenc, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_msg_aenc_get_proc_info, ret);
            continue;
        }

        if (offline_flag == TD_FALSE) {
            ret = drv_proc_aenc_show(file, aenc_inst->drv_aenc, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_proc_aenc_show, ret);
                continue;
            }
        } else {
#ifndef SW_UART_DEBUG
            ret = drv_diag_log_proc_aenc_show(aenc_inst->drv_aenc, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_diag_log_proc_aenc_show, ret);
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
