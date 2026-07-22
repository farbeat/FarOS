/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: source file for mpi anc
 * Author: audio
 */

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include "audio_type.h"
#include "drv_sap_ioctl.h"
#include "mpi_anc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SAP_ANC_DEV_NAME "/dev/" UMAP_DEVNAME_ANC

static td_s32 g_sap_anc_fd = -1;
static td_u32 g_mpi_anc_init_cnt = 0;
static pthread_mutex_t g_mpi_anc_mutex = PTHREAD_MUTEX_INITIALIZER;

#define mpi_anc_check_inited()         \
    do {                               \
        if (g_mpi_anc_init_cnt == 0) { \
            return ERR_ANC_NOT_INIT;   \
        }                              \
    } while (0)

static td_void mpi_anc_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_anc_mutex);
}

static td_void mpi_anc_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_anc_mutex);
}

td_s32 mpi_anc_init(td_void)
{
    mpi_anc_lock();

    if (g_mpi_anc_init_cnt > 0) {
        g_mpi_anc_init_cnt++;
        mpi_anc_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_sap_anc_fd < 0) {
        g_sap_anc_fd = audio_open(SAP_ANC_DEV_NAME, O_RDWR, 0);
        if (g_sap_anc_fd < 0) {
            g_sap_anc_fd = -1;
            mpi_anc_unlock();
            return ERR_ANC_OPEN_FAILED;
        }
    }

    g_mpi_anc_init_cnt++;
    mpi_anc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_anc_deinit(td_void)
{
    mpi_anc_lock();

    if (g_mpi_anc_init_cnt == 0) {
        mpi_anc_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_anc_init_cnt > 1) {
        g_mpi_anc_init_cnt--;
        mpi_anc_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_sap_anc_fd > 0) {
        (td_void)audio_close(g_sap_anc_fd);
        g_sap_anc_fd = -1;
    }

    g_mpi_anc_init_cnt--;
    mpi_anc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_anc_set_config(aha_mode mode, const td_s8 *config, td_u32 config_len)
{
    td_s32 ret;
    sap_anc_config sap_config;

    mpi_anc_check_inited();
    anc_check_null_ptr_return(config);

    mpi_anc_lock();

    sap_config.h_mod = mode;
    sap_config.config_len = config_len;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_sap_anc_fd, CMD_IOCTL_ANC_SET_CONFIG, &sap_config);
    if (ret != AUDIO_SUCCESS) {
        mpi_anc_unlock();
        return ret;
    }

    mpi_anc_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_anc_get_config(aha_mode *mode, td_s8 *config, td_u32 *config_len)
{
    td_s32 ret;
    sap_anc_config sap_config;

    mpi_anc_check_inited();
    anc_check_null_ptr_return(mode);
    anc_check_null_ptr_return(config);
    anc_check_null_ptr_return(config_len);

    mpi_anc_lock();

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_sap_anc_fd, CMD_IOCTL_ANC_GET_CONFIG, &sap_config);
    if (ret != AUDIO_SUCCESS) {
        mpi_anc_unlock();
        return ret;
    }

    mpi_anc_unlock();

    *mode = sap_config.h_mod;
    *config_len = sap_config.config_len;
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
