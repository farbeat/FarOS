/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: source file for mpi adp
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
#include "mem_addr.h"
#include "core_shrb.h"
#include "core_adp.h"
#include "drv_sap_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SAP_ADP_DEV_NAME "/dev/" UMAP_DEVNAME_ADP

#define mpi_adp_check_inited()         \
    do {                               \
        if (g_mpi_adp_init_cnt == 0) { \
            return ERR_ADP_NOT_INIT;   \
        }                              \
    } while (0)

#define mpi_adp_check_host(en)            \
    do {                                  \
        if (ADP_HOST_FLAG != (en)) {      \
            return ERR_ADP_NOT_AVAILABLE; \
        }                                 \
    } while (0)

#define mpi_adp_is_host(flag) (ADP_HOST_FLAG == (flag))

static td_s32 g_mpi_adp_fd = -1;
static td_u32 g_mpi_adp_init_cnt = 0;
#ifndef BRANDY_PROT
static pthread_mutex_t g_mpi_adp_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static td_void mpi_adp_lock(td_void)
{
#ifndef BRANDY_PROT
    (td_void)pthread_mutex_lock(&g_mpi_adp_mutex);
#endif
}

static td_void mpi_adp_unlock(td_void)
{
#ifndef BRANDY_PROT
    (td_void)pthread_mutex_unlock(&g_mpi_adp_mutex);
#endif
}

td_s32 mpi_adp_init(td_void)
{
    td_s32 ret;
    const td_u32 init_reg = SOC_IPC_SHM_BASE_ADDR;
    shrb_init_cfg shrb_cfg;
    sap_adp_init adp_init;

    mpi_adp_lock();

    if (g_mpi_adp_init_cnt > 0) {
        g_mpi_adp_init_cnt++;
        mpi_adp_unlock();
        return AUDIO_SUCCESS;
    }

    if (mpi_adp_is_host(TD_TRUE)) {
        if (g_mpi_adp_fd < 0) {
            g_mpi_adp_fd = audio_open(SAP_ADP_DEV_NAME, O_RDWR, 0);
            if (g_mpi_adp_fd < 0) {
                g_mpi_adp_fd = -1;
                mpi_adp_unlock();
                return ERR_ADP_INIT_FAILED;
            }
        }
        adp_init.enable = TD_TRUE;
        adp_init.init_reg = init_reg;
        adp_init.core_cid = AUDIO_CORE_DSP0;
        adp_init.drv_cid = AUDIO_CORE_CPU_K;

        audio_api_time_consum_stat();

        ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_INIT, &adp_init);
        if (ret != AUDIO_SUCCESS) {
            mpi_adp_unlock();
            return ret;
        }

        shrb_cfg.mem_type = adp_init.mem_type;
        shrb_cfg.fd = adp_init.fd;
        shrb_cfg.cache = adp_init.cache;
        shrb_cfg.size = adp_init.size;
        shrb_cfg.phys_addr = adp_init.phys_addr;
    } else {
        core_shrb_get_init_cfg(init_reg, &shrb_cfg);
    }

    core_shrb_init(SHRB_CORE_U, &shrb_cfg);

    ret = core_adp_init();
    if (ret != AUDIO_SUCCESS) {
        core_shrb_deinit();
        mpi_adp_unlock();
        return ret;
    }

    g_mpi_adp_init_cnt++;
    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_deinit(td_void)
{
    td_s32 ret;

    mpi_adp_lock();

    if (g_mpi_adp_init_cnt == 0) {
        mpi_adp_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_adp_init_cnt > 1) {
        g_mpi_adp_init_cnt--;
        mpi_adp_unlock();
        return AUDIO_SUCCESS;
    }

    ret = core_adp_deinit();
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    core_shrb_deinit();

    if (mpi_adp_is_host(TD_TRUE)) {
        if (g_mpi_adp_fd > 0) {
            audio_api_time_consum_stat();

            audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_DEINIT, TD_NULL);
            audio_close(g_mpi_adp_fd);
            g_mpi_adp_fd = -1;
        }
    }

    g_mpi_adp_init_cnt--;
    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr)
{
    td_s32 ret;
    sap_adp_open adp_open;

    adp_check_null_ptr_return(attr);

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_open.type = type;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_GET_DEF_ATTR, &adp_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    (td_void)memcpy_s(attr, sizeof(audio_adp_attr), &adp_open.attr, sizeof(adp_open.attr));

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_open(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr)
{
    td_s32 ret;
    sap_adp_open adp_info;
    sap_drv_handle adp_handle;

    adp_check_null_ptr_return(adp);
    adp_check_null_ptr_return(attr);

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_info.type = type;
    (td_void)memcpy_s(&adp_info.attr, sizeof(adp_info.attr), attr, sizeof(audio_adp_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_OPEN, &adp_info);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    ret = core_adp_clone(adp_info.h_mod, adp_info.type);
    if (ret != AUDIO_SUCCESS) {
        adp_handle.h_mod = adp_info.h_mod;

        audio_api_time_consum_stat();

        (td_void)audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_CLOSE, &adp_handle);
        mpi_adp_unlock();
        return ret;
    }

    *adp = adp_info.h_mod;
    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_close(td_handle adp)
{
    td_s32 ret;
    sap_drv_handle adp_info;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_info.h_mod = adp;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_CLOSE, &adp_info);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    ret = core_adp_destroy(adp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return AUDIO_SUCCESS;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

static td_s32 mpi_adp_alloc_shrb(td_handle adp, td_handle mod, audio_adp_type adp_type, td_u32 data_port_id)
{
    td_s32 ret;
    sap_drv_handle adp_handle;
    sap_drv_attach_para attach_para;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    attach_para.h_src = adp;
    attach_para.h_sink = mod;
    attach_para.adp_type = adp_type;
    attach_para.data_port_id = data_port_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_ALLOC_SHRB, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    ret = core_adp_attach_shrb(adp);
    if (ret != AUDIO_SUCCESS) {
        adp_handle.h_mod = adp;

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_FREE_SHRB, &adp_handle);
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_alloc_output_shrb(td_handle adp, td_handle mod, td_u32 data_port_id)
{
    return mpi_adp_alloc_shrb(adp, mod, ADP_OUTPUT, data_port_id);
}

td_s32 mpi_adp_alloc_input_shrb(td_handle adp, td_handle mod, td_u32 data_port_id)
{
    return mpi_adp_alloc_shrb(adp, mod, ADP_INPUT, data_port_id);
}

td_s32 mpi_adp_free_shrb(td_handle adp)
{
    td_s32 ret;
    sap_drv_handle adp_handle;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    ret = core_adp_detach_shrb(adp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    adp_handle.h_mod = adp;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_FREE_SHRB, &adp_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_map(td_handle adp)
{
    td_s32 ret;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_FALSE);
    mpi_adp_lock();

    ret = core_adp_clone(adp, ADP_CONNECTOR);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    ret = core_adp_attach_shrb(adp);
    if (ret != AUDIO_SUCCESS) {
        core_adp_destroy(adp);
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_unmap(td_handle adp)
{
    td_s32 ret;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_FALSE);
    mpi_adp_lock();

    ret = core_adp_detach_shrb(adp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    ret = core_adp_destroy(adp);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_get_attr(td_handle adp, audio_adp_attr *attr)
{
    td_s32 ret;
    sap_adp_attr adp_info;

    adp_check_null_ptr_return(attr);

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_info.h_mod = adp;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_GET_ATTR, &adp_info);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    (td_void)core_adp_set_attr(adp_info.h_mod, &adp_info.attr);

    (td_void)memcpy_s(attr, sizeof(audio_adp_attr), &adp_info.attr, sizeof(adp_info.attr));

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_set_attr(td_handle adp, const audio_adp_attr *attr)
{
    td_s32 ret;
    sap_adp_attr adp_info;

    adp_check_null_ptr_return(attr);

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_info.h_mod = adp;
    (td_void)memcpy_s(&adp_info.attr, sizeof(adp_info.attr), attr, sizeof(audio_adp_attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_SET_ATTR, &adp_info);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    (td_void)core_adp_set_attr(adp_info.h_mod, &adp_info.attr);

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_attach_output(td_handle adp, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    attach_para.h_src = adp;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_detach_output(td_handle adp, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    attach_para.h_src = adp;
    attach_para.h_sink = output;
    attach_para.data_port_id = 0;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_register_event_proc(td_handle adp, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event adp_event;

    adp_check_null_ptr_return(proc);

    mpi_adp_check_inited();
    mpi_adp_check_host(TD_TRUE);
    mpi_adp_lock();

    adp_event.h_mod = adp;
    adp_event.context = (td_u64)(td_uintptr_t)context;
    adp_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_adp_fd, CMD_IOCTL_ADP_REG_EVENT_PROC, &adp_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_query_free(td_handle adp, td_u32 *free_size)
{
    td_s32 ret;

    adp_check_null_ptr_return(free_size);

    mpi_adp_check_inited();
    mpi_adp_lock();

    *free_size = 0;

    ret = core_adp_query_free(adp, free_size);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_query_busy(td_handle adp, td_u32 *busy_size)
{
    td_s32 ret;

    adp_check_null_ptr_return(busy_size);

    mpi_adp_check_inited();
    mpi_adp_lock();

    *busy_size = 0;

    ret = core_adp_query_busy(adp, busy_size);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_get_buffer(td_handle adp, audio_stream_buf *stream)
{
    td_s32 ret;

    adp_check_null_ptr_return(stream);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_get_buffer(adp, stream);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_put_buffer(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;

    adp_check_null_ptr_return(stream);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_put_buffer(adp, stream);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_send_stream(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;

    adp_check_null_ptr_return(stream);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_send_stream(adp, stream);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_acquire_stream(td_handle adp, audio_stream_buf *stream)
{
    td_s32 ret;

    adp_check_null_ptr_return(stream);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_acquire_stream(adp, stream);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_release_stream(td_handle adp, const audio_stream_buf *stream)
{
    td_s32 ret;

    adp_check_null_ptr_return(stream);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_release_stream(adp, stream);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_send_frame(td_handle adp, const audio_frame *frame)
{
    td_s32 ret;

    adp_check_null_ptr_return(frame);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_send_frame(adp, frame);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_acquire_frame(td_handle adp, audio_frame *frame)
{
    td_s32 ret;

    adp_check_null_ptr_return(frame);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_acquire_frame(adp, frame);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_adp_release_frame(td_handle adp, const audio_frame *frame)
{
    td_s32 ret;

    adp_check_null_ptr_return(frame);

    mpi_adp_check_inited();
    mpi_adp_lock();

    ret = core_adp_release_frame(adp, frame);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_unlock();
        return ret;
    }

    mpi_adp_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
