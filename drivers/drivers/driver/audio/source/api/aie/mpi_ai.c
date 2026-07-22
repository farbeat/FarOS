/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: source file for mpi ai
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

#include "mpi_ai.h"

#ifdef SAP_AI_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AI_DUMP_FILE_SIZE (1024 * 1024)
#define AI_DUMP_FILE_PATH "/user/stream/ai_dump.pcm"

#define SAP_AI_DEV_NAME "/dev/" UMAP_DEVNAME_AI

#define mpi_ai_check_inited()         \
    do {                              \
        if (g_mpi_ai_init_cnt == 0) { \
            return ERR_AI_NOT_INIT;   \
        }                             \
    } while (0)

static td_s32 g_mpi_ai_fd = -1;
static td_u32 g_mpi_ai_init_cnt = 0;
static pthread_mutex_t g_mpi_ai_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_ai_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_ai_mutex);
}

static td_void mpi_ai_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_ai_mutex);
}

td_s32 mpi_ai_init(td_void)
{
    mpi_ai_lock();

    if (g_mpi_ai_init_cnt > 0) {
        g_mpi_ai_init_cnt++;
        mpi_ai_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_ai_fd < 0) {
        g_mpi_ai_fd = audio_open(SAP_AI_DEV_NAME, O_RDWR, 0);
        if (g_mpi_ai_fd < 0) {
            g_mpi_ai_fd = -1;
            mpi_ai_unlock();
            return ERR_AI_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_INIT, TD_NULL);
    }

    g_mpi_ai_init_cnt++;
    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_deinit(td_void)
{
    mpi_ai_lock();

    if (g_mpi_ai_init_cnt == 0) {
        mpi_ai_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_ai_init_cnt > 1) {
        g_mpi_ai_init_cnt--;
        mpi_ai_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_ai_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_DEINIT, TD_NULL);
        audio_close(g_mpi_ai_fd);
        g_mpi_ai_fd = -1;
    }

    g_mpi_ai_init_cnt--;
    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_get_def_attr(audio_ai_port port, audio_ai_attr *attr)
{
    td_s32 ret;
    sap_ai_open ai_open;

    ai_check_null_ptr_return(attr);

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_open.port = port;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_GET_DEF_ATTR, &ai_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(audio_ai_attr), &ai_open.attr, sizeof(ai_open.attr));

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_open(td_handle *ai, audio_ai_port port, const audio_ai_attr *attr)
{
    td_s32 ret;
    sap_ai_open param = {
        .h_mod = 0,
        .port = port,
    };

    ai_check_null_ptr_return(ai);

    mpi_ai_check_inited();

    if (attr == TD_NULL) {
        ret = mpi_ai_get_def_attr(port, &param.attr);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    } else {
        (td_void)memcpy_s(&param.attr, sizeof(param.attr), attr, sizeof(*attr));
    }

    mpi_ai_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_OPEN, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }
    mpi_ai_unlock();

    *ai = param.h_mod;

#ifdef SAP_AI_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(*ai) == TD_TRUE) {
        (td_void)mpi_dump_open(*ai, 0, AI_DUMP_FILE_PATH, AI_DUMP_FILE_SIZE);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_close(td_handle ai)
{
    td_s32 ret;
    sap_drv_handle ai_handle;

    mpi_ai_check_inited();

#ifdef SAP_AI_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(ai) == TD_TRUE) {
        (td_void)mpi_dump_close(ai);
    }
#endif

    mpi_ai_lock();

    ai_handle.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_CLOSE, &ai_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_register_event_proc(td_handle ai, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event ai_event;

    ai_check_null_ptr_return(proc);

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_event.h_mod = ai;
    ai_event.context = (td_u64)(td_uintptr_t)context;
    ai_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_REG_EVENT_PROC, &ai_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_start(td_handle ai)
{
    td_s32 ret;
    sap_drv_handle ai_handle;

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_handle.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_START, &ai_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_stop(td_handle ai)
{
    td_s32 ret;
    sap_drv_handle ai_handle;

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_handle.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_STOP, &ai_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_get_attr(td_handle ai, audio_ai_attr *attr)
{
    td_s32 ret;
    sap_ai_attr ai_attr;

    ai_check_null_ptr_return(attr);

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_attr.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_GET_ATTR, &ai_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(audio_ai_attr), &ai_attr.attr, sizeof(ai_attr.attr));

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_set_attr(td_handle ai, const audio_ai_attr *attr)
{
    td_s32 ret;
    sap_ai_attr ai_attr;

    ai_check_null_ptr_return(attr);

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_attr.h_mod = ai;
    (td_void)memcpy_s(&ai_attr.attr, sizeof(ai_attr.attr), attr, sizeof(audio_ai_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_SET_ATTR, &ai_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_get_gain(td_handle ai, ai_gain *gain)
{
    td_s32 ret;
    sap_ai_gain param;

    ai_check_null_ptr_return(gain);

    mpi_ai_check_inited();
    mpi_ai_lock();

    param.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_GET_GAIN, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }
    (td_void)memcpy_s(gain, sizeof(*gain), &param.gain, sizeof(param.gain));

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_set_gain(td_handle ai, const ai_gain *gain)
{
    td_s32 ret;
    sap_ai_gain param;

    ai_check_null_ptr_return(gain);

    mpi_ai_check_inited();
    mpi_ai_lock();

    param.h_mod = ai;
    (td_void)memcpy_s(&param.gain, sizeof(param.gain), gain, sizeof(*gain));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_SET_GAIN, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_get_mute(td_handle ai, td_bool *mute)
{
    td_s32 ret;
    sap_drv_enable param;

    ai_check_null_ptr_return(mute);

    mpi_ai_check_inited();
    mpi_ai_lock();

    param.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_GET_MUTE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    *mute = param.enable;
    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_set_mute(td_handle ai, td_bool mute)
{
    td_s32 ret;
    sap_drv_enable param;

    mpi_ai_check_inited();
    mpi_ai_lock();

    param.h_mod = ai;
    param.enable = mute;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_SET_MUTE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_set_bypass(td_handle ai, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable ai_enable;

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_enable.h_mod = ai;
    ai_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_SET_BYPASS, &ai_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_reset_vad(td_handle ai)
{
    td_s32 ret;
    sap_drv_handle ai_handle;

    mpi_ai_check_inited();
    mpi_ai_lock();

    ai_handle.h_mod = ai;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_RESET_VAD, &ai_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_set_mic_cali_volume(td_handle ai, const ai_cali_volume *cali_volume)
{
    td_s32 ret;
    sap_ai_cali_volume sap_cali_volume;

    ai_check_null_ptr_return(cali_volume);
    mpi_ai_check_inited();

    sap_cali_volume.h_mod = ai;
    (td_void)memcpy_s(&sap_cali_volume.cali_volume, sizeof(sap_cali_volume.cali_volume),
        cali_volume, sizeof(*cali_volume));

    mpi_ai_lock();
    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_SET_MIC_CALI_VOLUME, &sap_cali_volume);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_attach_output(td_handle ai, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_ai_check_inited();
    mpi_ai_lock();

    attach_para.h_src = ai;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ai_detach_output(td_handle ai, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_ai_check_inited();
    mpi_ai_lock();

    attach_para.h_src = ai;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ai_fd, CMD_IOCTL_AI_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_ai_unlock();
        return ret;
    }

    mpi_ai_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
