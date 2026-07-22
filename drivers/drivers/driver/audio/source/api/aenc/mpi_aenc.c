/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: source file for mpi aenc
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

#include "mpi_aenc.h"

#ifdef SAP_AENC_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AENC_DUMP_FILE_SIZE (1024 * 1024)
#define AENC_DUMP_FILE_PATH "/user/stream/aenc_dump.pcm"

#define SAP_AENC_DEV_NAME "/dev/" UMAP_DEVNAME_AENC

#define mpi_aenc_check_inited()         \
    do {                                \
        if (g_mpi_aenc_init_cnt == 0) { \
            return ERR_AENC_NOT_INIT;   \
        }                               \
    } while (0)

static td_s32 g_mpi_aenc_fd = -1;
static td_u32 g_mpi_aenc_init_cnt = 0;
static pthread_mutex_t g_mpi_aenc_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_aenc_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_aenc_mutex);
}

static td_void mpi_aenc_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_aenc_mutex);
}

td_s32 mpi_aenc_init(td_void)
{
    mpi_aenc_lock();

    if (g_mpi_aenc_init_cnt > 0) {
        g_mpi_aenc_init_cnt++;
        mpi_aenc_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_aenc_fd < 0) {
        g_mpi_aenc_fd = audio_open(SAP_AENC_DEV_NAME, O_RDWR, 0);
        if (g_mpi_aenc_fd < 0) {
            g_mpi_aenc_fd = -1;
            mpi_aenc_unlock();
            return ERR_AENC_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_INIT, TD_NULL);
    }

    g_mpi_aenc_init_cnt++;
    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_deinit(td_void)
{
    mpi_aenc_lock();

    if (g_mpi_aenc_init_cnt == 0) {
        mpi_aenc_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_aenc_init_cnt > 1) {
        g_mpi_aenc_init_cnt--;
        mpi_aenc_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_aenc_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_DEINIT, TD_NULL);
        audio_close(g_mpi_aenc_fd);
        g_mpi_aenc_fd = -1;
    }

    g_mpi_aenc_init_cnt--;
    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_load_codec(td_handle aenc, ha_codec_id codec_id, const td_char *name)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    (td_void)memset_s(&drv_load, sizeof(drv_load), 0, sizeof(drv_load));

    if (name != TD_NULL) {
        ret = strcpy_s(drv_load.name, sizeof(drv_load.name), name);
        if (ret != EOK) {
            mpi_aenc_unlock();
            return ret;
        }
    }

    drv_load.h_mod = aenc;
    drv_load.id = (td_u32)codec_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_LOAD_CODEC, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_unload_codec(ha_codec_id codec_id)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    drv_load.id = (td_u32)codec_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_UNLOAD_CODEC, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_open(td_handle *aenc)
{
    td_s32 ret;
    sap_aenc_open aenc_open;

    aenc_check_null_ptr_return(aenc);

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_open.codec_id = 0;
    (td_void)memset_s(&aenc_open.attr, sizeof(aenc_open.attr), 0, sizeof(aenc_open.attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_OPEN, &aenc_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    *aenc = aenc_open.h_mod;
    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_close(td_handle aenc)
{
    td_s32 ret;
    sap_drv_handle aenc_handle;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_handle.h_mod = aenc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_CLOSE, &aenc_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_register_event_proc(td_handle aenc, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event aenc_event;

    aenc_check_null_ptr_return(proc);

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_event.h_mod = aenc;
    aenc_event.context = (td_u64)(td_uintptr_t)context;
    aenc_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_REG_EVENT_PROC, &aenc_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_start(td_handle aenc)
{
    td_s32 ret;
    sap_drv_handle aenc_handle;

    mpi_aenc_check_inited();

#ifdef SAP_AENC_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(aenc) == TD_TRUE) {
        (td_void)mpi_dump_open(aenc, 0, AENC_DUMP_FILE_PATH, AENC_DUMP_FILE_SIZE);
    }
#endif

    mpi_aenc_lock();

    aenc_handle.h_mod = aenc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_START, &aenc_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_stop(td_handle aenc)
{
    td_s32 ret;
    sap_drv_handle aenc_handle;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_handle.h_mod = aenc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_STOP, &aenc_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();

#ifdef SAP_AENC_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(aenc) == TD_TRUE) {
        (td_void)mpi_dump_close(aenc);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_get_attr(td_handle aenc, aenc_attr *attr)
{
    td_s32 ret;
    sap_aenc_attr attr_temp;

    aenc_check_null_ptr_return(attr);

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    memset_s(&attr_temp, sizeof(attr_temp), 0, sizeof(attr_temp));
    attr_temp.h_mod = aenc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_GET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(aenc_attr), &attr_temp.attr, sizeof(attr_temp.attr));

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_set_attr(td_handle aenc, const aenc_attr *attr)
{
    td_s32 ret;
    sap_aenc_attr attr_temp;

    aenc_check_null_ptr_return(attr);

    mpi_aenc_check_inited();

    mpi_aenc_lock();
    attr_temp.h_mod = aenc;
    (td_void)memcpy_s(&attr_temp.attr, sizeof(attr_temp.attr), attr, sizeof(aenc_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_SET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_set_bypass(td_handle aenc, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable aenc_enable;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_enable.h_mod = aenc;
    aenc_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_SET_BYPASS, &aenc_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_attach_output(td_handle aenc, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    attach_para.h_src = aenc;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_detach_output(td_handle aenc, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_aenc_check_inited();
    mpi_aenc_lock();

    attach_para.h_src = aenc;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_aenc_set_param(td_handle aenc, aenc_param_type type, const td_void *param, td_u32 param_size)
{
    td_s32 ret;
    sap_aenc_param aenc_param;

    aenc_check_null_ptr_return(param);
    mpi_aenc_check_inited();
    mpi_aenc_lock();

    aenc_param.h_mod = aenc;
    aenc_param.type = type;
    ret = memcpy_s((td_void *)aenc_param.param, (size_t)AENC_PARAM_SIZE_MAX, param, (size_t)param_size);
    if (ret != EOK) {
        mpi_aenc_unlock();
        return ERR_AENC_MEM_COPY;
    }
    aenc_param.param_size = param_size;

    ret = audio_ioctl(g_mpi_aenc_fd, CMD_IOCTL_AENC_SET_PARAM, &aenc_param);
    if (ret != AUDIO_SUCCESS) {
        mpi_aenc_unlock();
        return ret;
    }

    mpi_aenc_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
