/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv adec msg
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_module.h"
#include "drv_sap_ioctl.h"
#include "drv_overlay_load.h"
#include "drv_audio_osal.h"
#include "drv_msg_adec.h"
#include "drv_proc_adec.h"
#include "drv_adec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_handle drv_adec;
    td_u64 context;
    td_u64 event_proc;
} drv_adec_inst;

static drv_adec_inst g_drv_adec_insts[MOD_ADEC_MAX_NUM];

static td_s32 drv_adec_find_inst(drv_adec_inst **adec_inst, td_handle adec)
{
    td_u8 adec_ind;
    sap_mod_id mod_id;
    drv_adec_inst *adec_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)adec;
    adec_ind = mod_id.bits.index;

    if (adec_ind >= MOD_ADEC_MAX_NUM) {
        return ERR_ADEC_INVALID_HANDLE;
    }
    adec_inst_p = &g_drv_adec_insts[adec_ind];

    if (adec_inst_p->drv_adec != adec) {
        return ERR_ADEC_INVALID_HANDLE;
    }

    *adec_inst = adec_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_adec_alloc_inst(drv_adec_inst **adec_inst, td_handle adec)
{
    td_u8 adec_ind;
    drv_adec_inst *adec_inst_p = TD_NULL;
    sap_mod_id mod_id;

    mod_id.u32 = (td_u32)adec;
    adec_ind = mod_id.bits.index;

    if (adec_ind >= MOD_ADEC_MAX_NUM) {
        return ERR_ADEC_INVALID_HANDLE;
    }
    adec_inst_p = &g_drv_adec_insts[adec_ind];

    if (adec_inst_p->drv_adec != 0) {
        return ERR_ADEC_NOT_AVAILABLE;
    }

    adec_inst_p->drv_adec = adec;
    *adec_inst = adec_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_adec_free_inst(td_handle adec)
{
    td_u8 adec_ind;
    sap_mod_id mod_id;
    drv_adec_inst *adec_inst_t = TD_NULL;

    mod_id.u32 = (td_u32)adec;
    adec_ind = mod_id.bits.index;

    if (adec_ind >= MOD_ADEC_MAX_NUM) {
        return ERR_ADEC_INVALID_HANDLE;
    }
    adec_inst_t = &g_drv_adec_insts[adec_ind];

    if (adec_inst_t->drv_adec != adec) {
        return ERR_ADEC_INVALID_HANDLE;
    }

    (td_void)memset_s(adec_inst_t, sizeof(*adec_inst_t), 0, sizeof(drv_adec_inst));

    return AUDIO_SUCCESS;
}

static audio_lib_id drv_adec_get_lib_id(ha_codec_id codec_id)
{
    td_u32 i;
    const struct {
        ha_codec_id acodec_id;
        audio_lib_id lib_id;
    } tab[] = {
        {HA_CODEC_ID_PCM, AUDIO_LIB_DEC_PCM},
        {HA_CODEC_ID_MSBC, AUDIO_LIB_DEC_SBC},
        {HA_CODEC_ID_SBC, AUDIO_LIB_DEC_SBC},
        {HA_CODEC_ID_MP3, AUDIO_LIB_DEC_MP3},
        {HA_CODEC_ID_AAC, AUDIO_LIB_DEC_AAC},
        {HA_CODEC_ID_OPUS, AUDIO_LIB_DEC_OPUS},
        {HA_CODEC_ID_SILK, AUDIO_LIB_DEC_SILK},
        {HA_CODEC_ID_LC3, AUDIO_LIB_DEC_LC3},
        {HA_CODEC_ID_L2HC, AUDIO_LIB_DEC_L2HC},
        {HA_CODEC_ID_FLAC, AUDIO_LIB_DEC_FLAC},
        {HA_CODEC_ID_VORBIS, AUDIO_LIB_DEC_VORBIS},
        {HA_CODEC_ID_AMRWB, AUDIO_LIB_DEC_AMRWB},
    };

    for (i = 0; i < sizeof(tab) / sizeof(tab[0]); i++) {
        if (tab[i].acodec_id == codec_id) {
            return tab[i].lib_id;
        }
    }

    return AUDIO_LIB_MAX;
}

td_s32 sap_ioctl_adec_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_adec_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_init, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_adec_insts, sizeof(g_drv_adec_insts), 0, sizeof(g_drv_adec_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_adec_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_deinit, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_adec_insts, sizeof(g_drv_adec_insts), 0, sizeof(g_drv_adec_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

static td_s32 drv_adec_load_codec(td_handle adec, ha_codec_id codec_id)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;
    sap_mod_id mod_id;

    mod_id.u32 = adec;

    ret = drv_msg_adec_get_lib_type(codec_id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_adec_get_lib_id(codec_id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(codec_id);
        return ERR_ADEC_NOT_SUPPORTED;
    }

    ret = drv_overlay_request_firmware(adec, OVERLAY_CODEC);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_request_firmware, ret);
        return ret;
    }

    ret = drv_overlay_load_lib_program(AUDIO_CORE_DSP0, lib_id, adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_overlay_load_lib_program, ret);
        drv_overlay_release_firmware(adec, OVERLAY_CODEC);
        return ERR_ADEC_INVALID_HANDLE;
    }

    audio_unused(mod_id);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_load_codec(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    audio_lib_id lib_id;
    audio_lib_type lib_type = AUDIO_LIB_TYPE_MAX;
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    ret = drv_msg_adec_get_lib_type((ha_codec_id)drv_load->id, &lib_type);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_get_lib_type, ret);
        return ret;
    }

    if (lib_type == AUDIO_LIB_TYPE_BUILT_IN) {
        return AUDIO_SUCCESS;
    }

    lib_id = drv_adec_get_lib_id((ha_codec_id)drv_load->id);
    if (lib_id >= AUDIO_LIB_MAX) {
        sap_err_log_u32(lib_id);
        sap_err_log_u32(drv_load->id);
        return ERR_ADEC_NOT_SUPPORTED;
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
        return ERR_ADEC_INVALID_HANDLE;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_unload_codec(td_u32 cmd, td_void *arg, td_void *file)
{
    sap_drv_load *drv_load = (sap_drv_load *)arg;

    drv_overlay_release_firmware(drv_load->h_mod, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_adec_open *adec_open = TD_NULL;
    drv_adec_inst *adec_inst = TD_NULL;

    adec_open = (sap_adec_open *)arg;

    ret = drv_msg_adec_open(&drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_open, ret);
        return ret;
    }

    ret = drv_adec_alloc_inst(&adec_inst, drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adec_alloc_inst, ret);
        drv_msg_adec_close(drv_adec);
        return ret;
    }

    adec_inst->drv_adec = drv_adec;
    adec_open->h_mod = drv_adec;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_drv_handle *adec_handle = TD_NULL;

    adec_handle = (sap_drv_handle *)arg;
    drv_adec = adec_handle->h_mod;

    ret = drv_msg_adec_close(drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_close, ret);
        return ret;
    }

    ret = drv_adec_free_inst(drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adec_free_inst, ret);
        return ret;
    }

    drv_overlay_release_firmware(drv_adec, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_drv_event *adec_event = TD_NULL;
    drv_adec_inst *adec_inst = TD_NULL;

    adec_event = (sap_drv_event *)arg;
    drv_adec = adec_event->h_mod;
    ret = drv_adec_find_inst(&adec_inst, drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adec_find_inst, ret);
        return ret;
    }

    adec_inst->event_proc = adec_event->event_proc;
    adec_inst->context = adec_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_start(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adec_handle = (sap_drv_handle *)arg;

    ret = drv_msg_adec_start(adec_handle->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_start, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_stop(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_drv_handle *adec_handle = (sap_drv_handle *)arg;

    ret = drv_msg_adec_stop(adec_handle->h_mod);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_stop, ret);
        return ret;
    }

    drv_overlay_release_firmware(adec_handle->h_mod, OVERLAY_CODEC);
    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_adec_attr *attr = TD_NULL;

    attr = (sap_adec_attr *)arg;
    drv_adec = attr->h_mod;

    ret = drv_msg_adec_get_attr(drv_adec, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_get_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_adec_attr *attr = TD_NULL;

    attr = (sap_adec_attr *)arg;
    drv_adec = attr->h_mod;

    ret = drv_msg_adec_stop(drv_adec);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_stop, ret);
        return ret;
    }

    ret = drv_adec_load_codec(drv_adec, attr->attr.codec_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_adec_load_codec, ret);
        return ret;
    }

    ret = drv_msg_adec_set_attr(drv_adec, &attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_set_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_set_bypass(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_drv_enable *adec_enable = TD_NULL;

    adec_enable = (sap_drv_enable *)arg;
    drv_adec = adec_enable->h_mod;

    ret = drv_msg_adec_set_bypass(drv_adec, adec_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_adec = attach_para->h_src;

    ret = drv_msg_adec_attach_output(drv_adec, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_attach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_adec_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_adec;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_adec = attach_para->h_src;

    ret = drv_msg_adec_detach_output(drv_adec, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_adec_detach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

#ifdef SAP_PROC_SUPPORT
td_s32 adec_read_proc(td_void *file, td_void *private)
{
    td_s32 ret;
    td_bool offline_flag = TD_FALSE;
    td_u8 adec_ind;
    drv_adec_inst *adec_inst = TD_NULL;
    adec_proc_item proc;

    if (private != TD_NULL) {
        offline_flag = *(td_bool *)private;
    }

    for (adec_ind = 0; adec_ind < MOD_ADEC_MAX_NUM; adec_ind++) {
        adec_inst = &g_drv_adec_insts[adec_ind];
        if (adec_inst->drv_adec == 0) {
            continue;
        }

        (td_void)memset_s(&proc, sizeof(proc), 0, sizeof(proc));
        ret = drv_msg_adec_get_proc_info(adec_inst->drv_adec, &proc);
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_fun(drv_msg_adec_get_proc_info, ret);
            continue;
        }

        if (offline_flag == TD_FALSE) {
            ret = drv_proc_adec_show(file, adec_inst->drv_adec, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_proc_adec_show, ret);
                continue;
            }
        } else {
#ifndef SW_UART_DEBUG
            ret = drv_diag_log_proc_adec_show(adec_inst->drv_adec, &proc);
            if (ret != AUDIO_SUCCESS) {
                sap_err_log_fun(drv_diag_log_proc_adec_show, ret);
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
