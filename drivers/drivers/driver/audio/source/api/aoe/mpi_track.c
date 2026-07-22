/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for mpi track
 * Author: audio
 */

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include "securec.h"
#include "audio_type.h"
#include "drv_sap_ioctl.h"

#include "mpi_track.h"

#ifdef SAP_TRACK_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TRACK_DUMP_FILE_SIZE (1024 * 1024)
#define TRACK_DUMP_FILE_PATH "/user/stream/track_dump.pcm"

#define SAP_TRACK_DEV_NAME "/dev/" UMAP_DEVNAME_TRACK

#define mpi_track_check_inited()         \
    do {                                 \
        if (g_mpi_track_init_cnt == 0) { \
            return ERR_TRACK_NOT_INIT;   \
        }                                \
    } while (0)

static td_s32 g_mpi_track_fd = -1;
static td_u32 g_mpi_track_init_cnt = 0;
static pthread_mutex_t g_mpi_track_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_track_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_track_mutex);
}

static td_void mpi_track_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_track_mutex);
}

td_s32 mpi_track_init(td_void)
{
    mpi_track_lock();

    if (g_mpi_track_init_cnt > 0) {
        g_mpi_track_init_cnt++;
        mpi_track_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_track_fd < 0) {
        g_mpi_track_fd = audio_open(SAP_TRACK_DEV_NAME, O_RDWR, 0);
        if (g_mpi_track_fd < 0) {
            g_mpi_track_fd = -1;
            mpi_track_unlock();
            return ERR_TRACK_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_INIT, TD_NULL);
    }

    g_mpi_track_init_cnt++;
    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_deinit(td_void)
{
    mpi_track_lock();

    if (g_mpi_track_init_cnt == 0) {
        mpi_track_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_track_init_cnt > 1) {
        g_mpi_track_init_cnt--;
        mpi_track_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_track_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_DEINIT, TD_NULL);
        audio_close(g_mpi_track_fd);
        g_mpi_track_fd = -1;
    }

    g_mpi_track_init_cnt--;
    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_def_attr(track_attr *attr)
{
    td_s32 ret;
    sap_track_open track_open;

    track_check_null_ptr_return(attr);

    mpi_track_check_inited();
    mpi_track_lock();

    track_open.h_snd = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_DEF_ATTR, &track_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(track_attr), &track_open.attr, sizeof(track_open.attr));

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_open(td_handle *track, td_handle sound, const track_attr *attr)
{
    td_s32 ret;
    sap_track_open track_open;

    track_check_null_ptr_return(track);
    track_check_null_ptr_return(attr);

    mpi_track_check_inited();
    mpi_track_lock();

    track_open.h_snd = sound;
    (td_void)memcpy_s(&track_open.attr, sizeof(track_open.attr), attr, sizeof(track_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_OPEN, &track_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    *track = track_open.h_mod;
    mpi_track_unlock();

#ifdef SAP_TRACK_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(*track) == TD_TRUE) {
        (td_void)mpi_dump_open(*track, 0, TRACK_DUMP_FILE_PATH, TRACK_DUMP_FILE_SIZE);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_track_close(td_handle track)
{
    td_s32 ret;
    sap_drv_handle track_handle;

    mpi_track_check_inited();

#ifdef SAP_TRACK_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(track) == TD_TRUE) {
        (td_void)mpi_dump_close(track);
    }
#endif

    mpi_track_lock();

    track_handle.h_mod = track;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_CLOSE, &track_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_register_event_proc(td_handle track, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event track_event;

    track_check_null_ptr_return(proc);

    mpi_track_check_inited();
    mpi_track_lock();

    track_event.h_mod = track;
    track_event.context = (td_u64)(td_uintptr_t)context;
    track_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_REG_EVENT_PROC, &track_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_set_bypass(td_handle track, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable track_enable;

    mpi_track_check_inited();
    mpi_track_lock();

    track_enable.h_mod = track;
    track_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_SET_BYPASS, &track_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_state(td_handle track, track_state *state)
{
    td_s32 ret;
    sap_track_state param;

    track_check_null_ptr_return(state);

    mpi_track_check_inited();
    mpi_track_lock();

    param.h_mod = track;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_STATE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }
    *state = param.state;

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_set_state(td_handle track, track_state state, const audio_toggle_config *toggle_config)
{
    td_s32 ret;
    sap_track_state param;

    track_check_null_ptr_return(toggle_config);
    mpi_track_check_inited();
    mpi_track_lock();

    param.h_mod = track;
    param.state = state;
    (td_void)memcpy_s(&param.toggle_config, sizeof(param.toggle_config), toggle_config, sizeof(*toggle_config));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_SET_STATE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_attr(td_handle track, track_attr *attr)
{
    td_s32 ret;
    sap_track_attr track_attr;

    track_check_null_ptr_return(attr);

    mpi_track_check_inited();
    mpi_track_lock();

    track_attr.h_mod = track;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_ATTR, &track_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(track_attr), &track_attr.attr, sizeof(track_attr.attr));

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_set_attr(td_handle track, const track_attr *attr)
{
    td_s32 ret;
    sap_track_attr track_attr;

    track_check_null_ptr_return(attr);

    mpi_track_check_inited();
    mpi_track_lock();

    track_attr.h_mod = track;
    ret = memcpy_s(&track_attr.attr, sizeof(track_attr.attr), attr, sizeof(track_attr));
    if (ret != EOK) {
        mpi_track_unlock();
        return ret;
    }

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_SET_ATTR, &track_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_mute(td_handle track, td_bool *enable)
{
    td_s32 ret;
    sap_drv_enable track_enable;

    track_check_null_ptr_return(enable);

    mpi_track_check_inited();
    mpi_track_lock();

    track_enable.h_mod = track;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_MUTE, &track_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }
    *enable = track_enable.enable;

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_set_mute(td_handle track, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable track_enable;

    mpi_track_check_inited();
    mpi_track_lock();

    track_enable.h_mod = track;
    track_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_SET_MUTE, &track_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_volume(td_handle track, ao_gain *gain)
{
    td_s32 ret;
    sap_ao_gain track_gain;

    track_check_null_ptr_return(gain);

    mpi_track_check_inited();
    mpi_track_lock();

    track_gain.h_mod = track;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_VOLUME, &track_gain);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }
    (td_void)memcpy_s(gain, sizeof(ao_gain), &track_gain.gain, sizeof(track_gain.gain));

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_set_volume(td_handle track, const ao_gain *gain, const audio_toggle_config *toggle_config)
{
    td_s32 ret;
    sap_ao_gain track_gain;

    track_check_null_ptr_return(gain);
    track_check_null_ptr_return(toggle_config);

    mpi_track_check_inited();
    mpi_track_lock();

    track_gain.h_mod = track;
    (td_void)memcpy_s(&track_gain.gain, sizeof(track_gain.gain), gain, sizeof(ao_gain));
    (td_void)memcpy_s(&track_gain.toggle_config, sizeof(track_gain.toggle_config),
        toggle_config, sizeof(*toggle_config));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_SET_VOLUME, &track_gain);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_attach_output(td_handle track, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_track_check_inited();
    mpi_track_lock();

    attach_para.h_src = track;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_detach_output(td_handle track, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_track_check_inited();
    mpi_track_lock();

    attach_para.h_src = track;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_track_get_play_pts(td_handle track, td_s64 *pts)
{
    td_s32 ret;
    sap_track_pts_param param = {
        .track = track,
        .pts = 0,
    };

    track_check_null_ptr_return(pts);

    if (track == 0) {
        return ERR_TRACK_INVALID_PARAM;
    }

    mpi_track_check_inited();
    mpi_track_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_track_fd, CMD_IOCTL_TRACK_GET_PTS, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_track_unlock();
        return ret;
    }

    mpi_track_unlock();

    *pts = param.pts;
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
