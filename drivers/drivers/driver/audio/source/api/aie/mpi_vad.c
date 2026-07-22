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

#include "mpi_vad.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SAP_VAD_DEV_NAME "/dev/" UMAP_DEVNAME_VAD

#define mpi_vad_check_inited()         \
    do {                               \
        if (g_mpi_vad_init_cnt == 0) { \
            return ERR_AI_NOT_INIT;    \
        }                              \
    } while (0)

static td_s32 g_mpi_vad_fd = -1;
static td_u32 g_mpi_vad_init_cnt = 0;
static pthread_mutex_t g_mpi_vad_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_vad_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_vad_mutex);
}

static td_void mpi_vad_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_vad_mutex);
}

td_s32 mpi_vad_init(td_void)
{
    mpi_vad_lock();

    if (g_mpi_vad_init_cnt > 0) {
        g_mpi_vad_init_cnt++;
        mpi_vad_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_vad_fd < 0) {
        g_mpi_vad_fd = audio_open(SAP_VAD_DEV_NAME, O_RDWR, 0);
        if (g_mpi_vad_fd < 0) {
            g_mpi_vad_fd = -1;
            mpi_vad_unlock();
            return ERR_AI_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_INIT, TD_NULL);
    }

    g_mpi_vad_init_cnt++;
    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_deinit(td_void)
{
    mpi_vad_lock();

    if (g_mpi_vad_init_cnt == 0) {
        mpi_vad_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_vad_init_cnt > 1) {
        g_mpi_vad_init_cnt--;
        mpi_vad_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_vad_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_DEINIT, TD_NULL);
        audio_close(g_mpi_vad_fd);
        g_mpi_vad_fd = -1;
    }

    g_mpi_vad_init_cnt--;
    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_get_def_attr(audio_vad_attr *attr)
{
    td_s32 ret;
    sap_vad_attr vad_attr;

    ai_check_null_ptr_return(attr);

    mpi_vad_check_inited();
    mpi_vad_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_GET_DEF_ATTR, &vad_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_vad_unlock();
        return ret;
    }
    ret = memcpy_s(attr, sizeof(*attr), &vad_attr.attr, sizeof(vad_attr.attr));
    if (ret != EOK) {
        mpi_vad_unlock();
        return ret;
    }

    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_open(td_handle *vad, const audio_vad_attr *attr)
{
    td_s32 ret;
    sap_vad_attr vad_attr;

    ai_check_null_ptr_return(vad);

    mpi_vad_check_inited();

    if (attr == TD_NULL) {
        (td_void)memset_s(&vad_attr, sizeof(vad_attr), 0, sizeof(vad_attr));

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_GET_DEF_ATTR, &vad_attr);
    } else {
        (td_void)memcpy_s(&vad_attr.attr, sizeof(vad_attr.attr), attr, sizeof(*attr));
    }

    mpi_vad_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_OPEN, &vad_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_vad_unlock();
        return ret;
    }
    mpi_vad_unlock();

    *vad = vad_attr.h_mod;
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_close(td_handle vad)
{
    td_s32 ret;
    sap_drv_handle vad_handle;

    mpi_vad_check_inited();
    mpi_vad_lock();

    vad_handle.h_mod = vad;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_CLOSE, &vad_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_vad_unlock();
        return ret;
    }

    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_register_event_proc(td_handle vad, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event vad_event;

    ai_check_null_ptr_return(proc);

    mpi_vad_check_inited();
    mpi_vad_lock();

    vad_event.h_mod = vad;
    vad_event.context = (td_u64)(td_uintptr_t)context;
    vad_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_REG_EVENT_PROC, &vad_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_vad_unlock();
        return ret;
    }

    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_vad_reset(td_handle vad)
{
    td_s32 ret;
    sap_drv_handle vad_handle;

    mpi_vad_check_inited();
    mpi_vad_lock();

    vad_handle.h_mod = vad;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_vad_fd, CMD_IOCTL_VAD_RESET, &vad_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_vad_unlock();
        return ret;
    }

    mpi_vad_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
