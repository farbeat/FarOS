/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv track
 * Author: audio
 */

#include "securec.h"
#include "mem_addr.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "audio_osal.h"
#include "core_module.h"
#include "drv_sap_ioctl.h"
#include "drv_msg_track.h"
#include "drv_track.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define FADE_OUT_DELAY_MS 100

typedef struct {
    td_handle drv_track;
    td_u64 context;
    td_u64 event_proc;
    td_u32 *pts_reg;
} drv_track_inst;

static drv_track_inst g_drv_track_insts[MOD_TRACK_MAX_NUM];

static td_void drv_track_pts_reg_deinit(drv_track_inst *track_inst, td_u32 track_id)
{
    if (track_inst->pts_reg == TD_NULL) {
        return;
    }

    audio_osal_address_unmap(SOC_SND_TRACK_PTS_ADDR + track_id * sizeof(td_u32), track_inst->pts_reg);
    track_inst->pts_reg = TD_NULL;
}

static td_s32 drv_track_pts_reg_init(drv_track_inst *track_inst, td_u32 track_id)
{
    td_u32 *reg = TD_NULL;

    reg = (td_u32 *)audio_osal_address_map(SOC_SND_TRACK_PTS_ADDR + track_id * sizeof(td_u32), sizeof(td_u32));
    if (reg == TD_NULL) {
        return ERR_TRACK_MEM_ALLOC;
    }

    track_inst->pts_reg = reg;
    return AUDIO_SUCCESS;
}

static td_s32 drv_track_find_inst(drv_track_inst **track_inst, td_handle track)
{
    td_u8 track_ind;
    sap_mod_id mod_id;
    drv_track_inst *track_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)track;
    track_ind = mod_id.bits.index;

    if (track_ind >= MOD_TRACK_MAX_NUM) {
        return ERR_TRACK_INVALID_HANDLE;
    }
    track_inst_p = &g_drv_track_insts[track_ind];

    if (track_inst_p->drv_track != track) {
        return ERR_TRACK_INVALID_HANDLE;
    }

    *track_inst = track_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_track_alloc_inst(drv_track_inst **track_inst, td_handle track)
{
    td_s32 ret;
    td_u32 track_id;
    sap_mod_id mod_id;
    drv_track_inst *track_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)track;
    track_id = mod_id.bits.index;

    if (track_id >= MOD_TRACK_MAX_NUM) {
        return ERR_TRACK_INVALID_HANDLE;
    }
    track_inst_p = &g_drv_track_insts[track_id];

    if (track_inst_p->drv_track != 0) {
        return ERR_TRACK_NOT_AVAILABLE;
    }

    ret = drv_track_pts_reg_init(track_inst_p, track_id);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_track_pts_reg_init, ret);
        sap_err_log_h32(track);
        return ret;
    }

    track_inst_p->drv_track = track;
    *track_inst = track_inst_p;
    return AUDIO_SUCCESS;
}

static td_s32 drv_track_free_inst(td_handle track)
{
    td_u32 track_id;
    sap_mod_id mod_id;
    drv_track_inst *track_inst_p = TD_NULL;

    mod_id.u32 = (td_u32)track;
    track_id = mod_id.bits.index;

    if (track_id >= MOD_TRACK_MAX_NUM) {
        return ERR_TRACK_INVALID_HANDLE;
    }
    track_inst_p = &g_drv_track_insts[track_id];

    if (track_inst_p->drv_track != track) {
        return ERR_TRACK_INVALID_HANDLE;
    }

    drv_track_pts_reg_deinit(track_inst_p, track_id);

    (td_void)memset_s(track_inst_p, sizeof(*track_inst_p), 0, sizeof(drv_track_inst));

    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_init(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_track_init();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_init, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_track_insts, sizeof(g_drv_track_insts), 0, sizeof(g_drv_track_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_deinit(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;

    ret = drv_msg_track_deinit();
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_deinit, ret);
        return ret;
    }

    (td_void)memset_s(&g_drv_track_insts, sizeof(g_drv_track_insts), 0, sizeof(g_drv_track_insts));

    audio_unused(cmd);
    audio_unused(arg);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_def_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    sap_track_open *track_open;

    track_open = (sap_track_open *)arg;
    ret = drv_msg_track_get_def_attr(&track_open->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_get_def_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_open(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_track_open *track_open = TD_NULL;
    drv_track_inst *track_inst = TD_NULL;

    track_open = (sap_track_open *)arg;

    ret = drv_msg_track_open(&drv_track, track_open->h_snd, &track_open->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_open, ret);
        return ret;
    }

    ret = drv_track_alloc_inst(&track_inst, drv_track);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_track_alloc_inst, ret);
        drv_msg_track_close(drv_track);
        return ret;
    }

    track_inst->drv_track = drv_track;
    track_open->h_mod = drv_track;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_close(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_handle *track_handle = TD_NULL;

    track_handle = (sap_drv_handle *)arg;
    drv_track = track_handle->h_mod;

    ret = drv_msg_track_close(drv_track);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_close, ret);
        return ret;
    }

    ret = drv_track_free_inst(drv_track);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_track_free_inst, ret);
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_register_event_proc(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_event *track_event = TD_NULL;
    drv_track_inst *track_inst = TD_NULL;

    track_event = (sap_drv_event *)arg;
    drv_track = track_event->h_mod;
    ret = drv_track_find_inst(&track_inst, drv_track);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_ret(ret);
        return ret;
    }

    track_inst->event_proc = track_event->event_proc;
    track_inst->context = track_event->context;

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_state(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_track_state *track_state = TD_NULL;

    track_state = (sap_track_state *)arg;
    drv_track = track_state->h_mod;

    ret = drv_msg_track_get_state(drv_track, &track_state->state);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_get_state, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_set_state(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_track_state *track_state = TD_NULL;

    track_state = (sap_track_state *)arg;
    drv_track = track_state->h_mod;

    ret = drv_msg_track_set_state(drv_track, track_state->state, &track_state->toggle_config);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_set_state, ret);
        return ret;
    }

    /* 为规避后级太早被销毁导致淡出帧没有播出的问题增加的延时，待正式方案优化 */
    if (track_state->state == TRACK_STATE_STOP || track_state->state == TRACK_STATE_PAUSE) {
        audio_osal_msleep(FADE_OUT_DELAY_MS);
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_track_attr *track_attr = TD_NULL;

    track_attr = (sap_track_attr *)arg;
    drv_track = track_attr->h_mod;

    ret = drv_msg_track_get_attr(drv_track, &track_attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_get_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_set_attr(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_track_attr *track_attr = TD_NULL;

    track_attr = (sap_track_attr *)arg;
    drv_track = track_attr->h_mod;

    ret = drv_msg_track_set_attr(drv_track, &track_attr->attr);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_set_attr, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_mute(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_enable *track_enable = TD_NULL;

    track_enable = (sap_drv_enable *)arg;
    drv_track = track_enable->h_mod;

    ret = drv_msg_track_get_mute(drv_track, &track_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_get_mute, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_set_mute(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_enable *track_enable = TD_NULL;

    track_enable = (sap_drv_enable *)arg;
    drv_track = track_enable->h_mod;

    ret = drv_msg_track_set_mute(drv_track, track_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_set_mute, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_volume(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_ao_gain *track_gain = TD_NULL;

    track_gain = (sap_ao_gain *)arg;
    drv_track = track_gain->h_mod;

    ret = drv_msg_track_get_volume(drv_track, &track_gain->gain);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_get_volume, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_set_volume(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_ao_gain *track_gain = TD_NULL;

    track_gain = (sap_ao_gain *)arg;
    drv_track = track_gain->h_mod;

    ret = drv_msg_track_set_volume(drv_track, &track_gain->gain, &track_gain->toggle_config);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_set_volume, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_set_bypass(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_enable *track_enable = TD_NULL;

    track_enable = (sap_drv_enable *)arg;
    drv_track = track_enable->h_mod;

    ret = drv_msg_track_set_bypass(drv_track, track_enable->enable);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_set_bypass, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_attach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_track = attach_para->h_src;

    ret = drv_msg_track_attach_output(drv_track, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_attach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_detach_output(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    td_handle drv_track;
    sap_drv_attach_para *attach_para = (sap_drv_attach_para *)arg;

    drv_track = attach_para->h_src;

    ret = drv_msg_track_detach_output(drv_track, attach_para->h_sink);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_msg_track_detach_output, ret);
        return ret;
    }

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

td_s32 sap_ioctl_track_get_pts(td_u32 cmd, td_void *arg, td_void *file)
{
    td_s32 ret;
    drv_track_inst *track_inst = TD_NULL;
    sap_track_pts_param *param = (sap_track_pts_param *)arg;

    ret = drv_track_find_inst(&track_inst, param->track);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_track_find_inst, ret);
        sap_err_log_h32(param->track);
        return ret;
    }

    track_check_null_ptr_return(track_inst);
    track_check_null_ptr_return(track_inst->pts_reg);

    param->pts = audio_pts_ms_to_us(*track_inst->pts_reg);

    audio_unused(cmd);
    audio_unused(file);
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
