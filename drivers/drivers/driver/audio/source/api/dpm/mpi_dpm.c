/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: source file for mpi dpm
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

#include "mpi_dpm.h"

#ifdef SAP_DPM_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DPM_DUMP_FILE_SIZE (1024 * 1024)
#define DPM_DUMP_FILE_PATH "/user/stream/dpm_dump.pcm"

#define SAP_DPM_DEV_NAME "/dev/" UMAP_DEVNAME_DPM

#define mpi_dpm_check_inited()         \
    do {                                \
        if (g_mpi_dpm_init_cnt == 0) { \
            return ERR_DPM_NOT_INIT;   \
        }                               \
    } while (0)

static td_s32 g_mpi_dpm_fd = -1;
static td_u32 g_mpi_dpm_init_cnt = 0;
static pthread_mutex_t g_mpi_dpm_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_dpm_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_dpm_mutex);
}

static td_void mpi_dpm_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_dpm_mutex);
}

td_s32 mpi_dpm_init(td_void)
{
    mpi_dpm_lock();

    if (g_mpi_dpm_init_cnt > 0) {
        g_mpi_dpm_init_cnt++;
        mpi_dpm_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_dpm_fd < 0) {
        g_mpi_dpm_fd = audio_open(SAP_DPM_DEV_NAME, O_RDWR, 0);
        if (g_mpi_dpm_fd < 0) {
            g_mpi_dpm_fd = -1;
            mpi_dpm_unlock();
            return ERR_DPM_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_INIT, TD_NULL);
    }

    g_mpi_dpm_init_cnt++;
    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_deinit(td_void)
{
    mpi_dpm_lock();

    if (g_mpi_dpm_init_cnt == 0) {
        mpi_dpm_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_dpm_init_cnt > 1) {
        g_mpi_dpm_init_cnt--;
        mpi_dpm_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_dpm_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_DEINIT, TD_NULL);
        audio_close(g_mpi_dpm_fd);
        g_mpi_dpm_fd = -1;
    }

    g_mpi_dpm_init_cnt--;
    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_load_lib(td_handle dpm, dpm_id dpm_id, const td_char *name)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    (td_void)memset_s(&drv_load, sizeof(drv_load), 0, sizeof(drv_load));

    if (name != TD_NULL) {
        ret = strcpy_s(drv_load.name, sizeof(drv_load.name), name);
        if (ret != EOK) {
            mpi_dpm_unlock();
            return ret;
        }
    }

    drv_load.h_mod = dpm;
    drv_load.id = (td_u32)dpm_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_LOAD_LIB, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_unload_lib(dpm_id dpm_id)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    drv_load.id = (td_u32)dpm_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_UNLOAD_LIB, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_open(td_handle *dpm)
{
    td_s32 ret;
    sap_dpm_open dpm_open;

    dpm_check_null_ptr_return(dpm);

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    dpm_open.dpm_id = 0;
    (td_void)memset_s(&dpm_open.attr, sizeof(dpm_open.attr), 0, sizeof(dpm_open.attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_OPEN, &dpm_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    *dpm = dpm_open.h_mod;
    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_close(td_handle dpm)
{
    td_s32 ret;
    sap_drv_handle dpm_handle;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    dpm_handle.h_mod = dpm;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_CLOSE, &dpm_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_register_event_proc(td_handle dpm, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event dpm_event;

    dpm_check_null_ptr_return(proc);

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    dpm_event.h_mod = dpm;
    dpm_event.context = (td_u64)(td_uintptr_t)context;
    dpm_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_REG_EVENT_PROC, &dpm_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_start(td_handle dpm)
{
    td_s32 ret;
    sap_drv_handle dpm_handle;

    mpi_dpm_check_inited();

#ifdef SAP_DPM_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(dpm) == TD_TRUE) {
        (td_void)mpi_dump_open(dpm, 0, DPM_DUMP_FILE_PATH, DPM_DUMP_FILE_SIZE);
    }
#endif

    mpi_dpm_lock();

    dpm_handle.h_mod = dpm;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_START, &dpm_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_stop(td_handle dpm)
{
    td_s32 ret;
    sap_drv_handle dpm_handle;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    dpm_handle.h_mod = dpm;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_STOP, &dpm_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();

#ifdef SAP_DPM_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(dpm) == TD_TRUE) {
        (td_void)mpi_dump_close(dpm);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_get_attr(td_handle dpm, dpm_attr *attr)
{
    td_s32 ret;
    sap_dpm_attr attr_temp;

    dpm_check_null_ptr_return(attr);

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    memset_s(&attr_temp, sizeof(attr_temp), 0, sizeof(attr_temp));
    attr_temp.h_mod = dpm;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_GET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(dpm_attr), &attr_temp.attr, sizeof(attr_temp.attr));

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_set_attr(td_handle dpm, const dpm_attr *attr)
{
    td_s32 ret;
    sap_dpm_attr attr_temp;

    dpm_check_null_ptr_return(attr);

    mpi_dpm_check_inited();

    mpi_dpm_lock();
    attr_temp.h_mod = dpm;
    (td_void)memcpy_s(&attr_temp.attr, sizeof(attr_temp.attr), attr, sizeof(dpm_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_SET_ATTR, &attr_temp);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_set_bypass(td_handle dpm, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable dpm_enable;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    dpm_enable.h_mod = dpm;
    dpm_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_SET_BYPASS, &dpm_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_attach_output(td_handle dpm, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    attach_para.h_src = dpm;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_dpm_detach_output(td_handle dpm, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_dpm_check_inited();
    mpi_dpm_lock();

    attach_para.h_src = dpm;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_dpm_fd, CMD_IOCTL_DPM_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_dpm_unlock();
        return ret;
    }

    mpi_dpm_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
