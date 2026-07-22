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
#include "audio_intf.h"
#include "mpi_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SAP_SYS_DEV_NAME "/dev/" UMAP_DEVNAME_SYS

#define mpi_sys_check_inited()         \
    do {                               \
        if (g_mpi_sys_init_cnt == 0) { \
            return ERR_ADEC_NOT_INIT;  \
        }                              \
    } while (0)

static td_s32 g_mpi_sys_fd = -1;
static td_u32 g_mpi_sys_init_cnt = 0;
static pthread_mutex_t g_mpi_sys_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_sys_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_sys_mutex);
}

static td_void mpi_sys_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_sys_mutex);
}

static td_s32 mpi_sys_init(td_void)
{
    if (g_mpi_sys_fd < 0) {
        g_mpi_sys_fd = audio_open(SAP_SYS_DEV_NAME, O_RDWR, 0);
        if (g_mpi_sys_fd < 0) {
            g_mpi_sys_fd = -1;
            return ERR_SYS_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_INIT, TD_NULL);
    }

    return AUDIO_SUCCESS;
}

static td_s32 mpi_sys_deinit(td_void)
{
    if (g_mpi_sys_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_DEINIT, TD_NULL);
        audio_close(g_mpi_sys_fd);
        g_mpi_sys_fd = -1;
    }

    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_init(td_void)
{
    td_s32 ret;
    mpi_sys_lock();

    if (g_mpi_sys_init_cnt > 0) {
        g_mpi_sys_init_cnt++;
        mpi_sys_unlock();
        return AUDIO_SUCCESS;
    }

#if (SAP_OS_TYPE != os_type_linux)
    ret = sap_drv_mod_init();
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }
#endif /* SAP_OS_TYPE */

    ret = mpi_sys_init();
    if (ret != AUDIO_SUCCESS) {
#if (SAP_OS_TYPE != os_type_linux)
        sap_drv_mod_exit();
#endif /* SAP_OS_TYPE */
        mpi_sys_unlock();
        return ret;
    }

    g_mpi_sys_init_cnt++;
    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_deinit(td_void)
{
    td_s32 ret;
    mpi_sys_lock();

    if (g_mpi_sys_init_cnt == 0) {
        mpi_sys_unlock();
        return AUDIO_SUCCESS;
    } else if (g_mpi_sys_init_cnt > 1) {
        g_mpi_sys_init_cnt--;
        mpi_sys_unlock();
        return AUDIO_SUCCESS;
    }

    ret = mpi_sys_deinit();
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

#if (SAP_OS_TYPE != os_type_linux)
    sap_drv_mod_exit();
#endif /* SAP_OS_TYPE */

    g_mpi_sys_init_cnt--;
    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_debug_cfg(const audio_debug_cfg *cfg)
{
    td_s32 ret;

    mpi_sys_check_inited();
    mpi_sys_lock();

    if (g_mpi_sys_fd < 0) {
        g_mpi_sys_fd = audio_open(SAP_SYS_DEV_NAME, O_RDWR, 0);
        if (g_mpi_sys_fd < 0) {
            g_mpi_sys_fd = -1;
            mpi_sys_unlock();
            return ERR_SYS_INIT_FAILED;
        }
    }

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_DEBUG_CFG, (td_void *)cfg);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_get_debug_cfg(audio_debug_cfg *cfg)
{
    td_s32 ret;

    mpi_sys_check_inited();
    mpi_sys_lock();

    if (g_mpi_sys_fd < 0) {
        g_mpi_sys_fd = audio_open(SAP_SYS_DEV_NAME, O_RDWR, 0);
        if (g_mpi_sys_fd < 0) {
            g_mpi_sys_fd = -1;
            mpi_sys_unlock();
            return ERR_SYS_INIT_FAILED;
        }
    }

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_GET_DEBUG_CFG, cfg);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_product_form(audio_product_form product_form)
{
    td_s32 ret;
    td_u8 param = product_form;

    mpi_sys_check_inited();
    mpi_sys_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_PRODUCT_FORM, (td_void *)&param);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_tws_mono_mode(td_bool mono)
{
    td_s32 ret;

    mpi_sys_check_inited();
    mpi_sys_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_TWS_MONO_MODE, (td_void *)&mono);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_tws_mode(audio_tws_mode tws_mode)
{
    td_s32 ret;
    td_u8 param = tws_mode;

    mpi_sys_check_inited();
    mpi_sys_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_TWS_MODE, (td_void *)&param);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_tws_role(audio_tws_role tws_role)
{
    td_s32 ret;
    td_u8 param = tws_role;

    mpi_sys_check_inited();
    mpi_sys_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_TWS_ROLE, (td_void *)&param);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_audio_set_dsp_log_level(td_u32 log_level)
{
    td_s32 ret;
    td_u32 param = log_level;

    mpi_sys_check_inited();
    mpi_sys_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sys_fd, CMD_IOCTL_SYS_SET_DSP_LOG_LEVEL, (td_void *)&param);
    if (ret != AUDIO_SUCCESS) {
        mpi_sys_unlock();
        return ret;
    }

    mpi_sys_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
