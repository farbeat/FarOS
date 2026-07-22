/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for mpi adec
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

#include "mpi_adec.h"

#ifdef SAP_ADEC_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ADEC_DUMP_FILE_SIZE (1024 * 1024)
#define ADEC_DUMP_FILE_PATH "/user/stream/adec_dump.pcm"

#define SAP_ADEC_DEV_NAME "/dev/" UMAP_DEVNAME_ADEC

#define mpi_adec_check_inited()         \
    do {                                \
        if (g_mpi_adec_init_cnt == 0) { \
            return ERR_ADEC_NOT_INIT;   \
        }                               \
    } while (0)

static td_s32 g_mpi_adec_fd = -1;
static td_u32 g_mpi_adec_init_cnt = 0;
static pthread_mutex_t g_mpi_adec_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_adec_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_adec_mutex);
}

static td_void mpi_adec_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_adec_mutex);
}

td_s32 mpi_adec_init(td_void)
{
    mpi_adec_lock();

    if (g_mpi_adec_init_cnt > 0) {
        g_mpi_adec_init_cnt++;
        mpi_adec_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_adec_fd < 0) {
        g_mpi_adec_fd = audio_open(SAP_ADEC_DEV_NAME, O_RDWR, 0);
        if (g_mpi_adec_fd < 0) {
            g_mpi_adec_fd = -1;
            mpi_adec_unlock();
            return ERR_ADEC_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_INIT, TD_NULL);
    }

    g_mpi_adec_init_cnt++;
    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_deinit(td_void)
{
    mpi_adec_lock();

    if (g_mpi_adec_init_cnt == 0) {
        mpi_adec_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_adec_init_cnt > 1) {
        g_mpi_adec_init_cnt--;
        mpi_adec_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_adec_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_DEINIT, TD_NULL);
        audio_close(g_mpi_adec_fd);
        g_mpi_adec_fd = -1;
    }

    g_mpi_adec_init_cnt--;
    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_load_codec(td_handle adec, ha_codec_id codec_id, const td_char *name)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_adec_check_inited();
    mpi_adec_lock();

    (td_void)memset_s(&drv_load, sizeof(sap_drv_load), 0, sizeof(drv_load));

    if (name != TD_NULL) {
        ret = strcpy_s(drv_load.name, sizeof(drv_load.name), name);
        if (ret != EOK) {
            mpi_adec_unlock();
            return ret;
        }
    }

    drv_load.h_mod = adec;
    drv_load.id = (td_u32)codec_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_LOAD_CODEC, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_unload_codec(ha_codec_id codec_id)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_adec_check_inited();
    mpi_adec_lock();

    drv_load.id = (td_u32)codec_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_UNLOAD_CODEC, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_open(td_handle *adec)
{
    td_s32 ret;
    sap_adec_open adec_open;

    adec_check_null_ptr_return(adec);

    mpi_adec_check_inited();
    mpi_adec_lock();

    adec_open.codec_id = 0;
    (td_void)memset_s(&adec_open.attr, sizeof(adec_open.attr), 0, sizeof(adec_open.attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_OPEN, &adec_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    *adec = adec_open.h_mod;
    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_close(td_handle adec)
{
    td_s32 ret;
    sap_drv_handle adec_handle;

    mpi_adec_check_inited();
    mpi_adec_lock();

    adec_handle.h_mod = adec;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_CLOSE, &adec_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_register_event_proc(td_handle adec, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event adec_event;

    adec_check_null_ptr_return(proc);

    mpi_adec_check_inited();
    mpi_adec_lock();

    adec_event.h_mod = adec;
    adec_event.context = (td_u64)(td_uintptr_t)context;
    adec_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_REG_EVENT_PROC, &adec_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_start(td_handle adec)
{
    td_s32 ret;
    sap_drv_handle adec_handle;

    mpi_adec_check_inited();

#ifdef SAP_ADEC_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(adec) == TD_TRUE) {
        (td_void)mpi_dump_open(adec, 0, ADEC_DUMP_FILE_PATH, ADEC_DUMP_FILE_SIZE);
    }
#endif

    mpi_adec_lock();

    adec_handle.h_mod = adec;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_START, &adec_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_stop(td_handle adec)
{
    td_s32 ret;
    sap_drv_handle adec_handle;

    mpi_adec_check_inited();
    mpi_adec_lock();

    adec_handle.h_mod = adec;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_STOP, &adec_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();

#ifdef SAP_ADEC_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(adec) == TD_TRUE) {
        (td_void)mpi_dump_close(adec);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_get_attr(td_handle adec, adec_attr *attr)
{
    td_s32 ret;
    sap_adec_attr attr_temp;

    adec_check_null_ptr_return(attr);

    mpi_adec_check_inited();
    mpi_adec_lock();

    attr_temp.h_mod = adec;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_GET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(adec_attr), &attr_temp.attr, sizeof(attr_temp.attr));

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_set_attr(td_handle adec, const adec_attr *attr)
{
    td_s32 ret;
    sap_adec_attr attr_temp;

    adec_check_null_ptr_return(attr);

    mpi_adec_check_inited();

    mpi_adec_lock();
    attr_temp.h_mod = adec;
    (td_void)memcpy_s(&attr_temp.attr, sizeof(attr_temp.attr), attr, sizeof(adec_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_SET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_set_bypass(td_handle adec, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable adec_enable;

    mpi_adec_check_inited();
    mpi_adec_lock();

    adec_enable.h_mod = adec;
    adec_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_SET_BYPASS, &adec_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_attach_output(td_handle adec, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_adec_check_inited();
    mpi_adec_lock();

    attach_para.h_src = adec;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adec_detach_output(td_handle adec, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_adec_check_inited();
    mpi_adec_lock();

    attach_para.h_src = adec;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adec_fd, CMD_IOCTL_ADEC_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_adec_unlock();
        return ret;
    }

    mpi_adec_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
