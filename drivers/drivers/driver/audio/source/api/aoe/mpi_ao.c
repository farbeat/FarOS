/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for mpi ao
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
#include "audio_debug.h"

#include "mpi_ao.h"

#ifdef SAP_AO_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AO_DUMP_FILE_SIZE (1024 * 1024)
#define AO_DUMP_PORT0_FILE_PATH "/user/stream/ao_dump0.pcm"
#define AO_DUMP_PORT1_FILE_PATH "/user/stream/ao_dump1.pcm"

#define SAP_AO_DEV_NAME "/dev/" UMAP_DEVNAME_AO

#define mpi_ao_check_inited()         \
    do {                              \
        if (g_mpi_ao_init_cnt == 0) { \
            return ERR_AO_NOT_INIT;   \
        }                             \
    } while (0)

static td_s32 g_mpi_ao_fd = -1;
static td_u32 g_mpi_ao_init_cnt = 0;
static pthread_mutex_t g_mpi_ao_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_ao_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_ao_mutex);
}

static td_void mpi_ao_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_ao_mutex);
}

td_s32 mpi_ao_init(td_void)
{
    mpi_ao_lock();

    if (g_mpi_ao_init_cnt > 0) {
        g_mpi_ao_init_cnt++;
        mpi_ao_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_ao_fd < 0) {
        g_mpi_ao_fd = audio_open(SAP_AO_DEV_NAME, O_RDWR, 0);
        if (g_mpi_ao_fd < 0) {
            g_mpi_ao_fd = -1;
            mpi_ao_unlock();
            return ERR_AO_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_INIT, TD_NULL);
    }

    g_mpi_ao_init_cnt++;
    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_deinit(td_void)
{
    mpi_ao_lock();

    if (g_mpi_ao_init_cnt == 0) {
        mpi_ao_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_ao_init_cnt > 1) {
        g_mpi_ao_init_cnt--;
        mpi_ao_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_ao_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_DEINIT, TD_NULL);
        audio_close(g_mpi_ao_fd);
        g_mpi_ao_fd = -1;
    }

    g_mpi_ao_init_cnt--;
    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_def_attr(ao_snd_id snd_id, ao_attr *attr)
{
    td_s32 ret;
    sap_ao_open ao_open;

    ao_check_null_ptr_return(attr);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_open.snd_id = snd_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_DEF_ATTR, &ao_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(*attr), &ao_open.attr, sizeof(ao_open.attr));

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_open(td_handle *ao, ao_snd_id snd_id, const ao_attr *attr)
{
    td_s32 ret;
    sap_ao_open ao_open;

    ao_check_null_ptr_return(ao);
    ao_check_null_ptr_return(attr);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_open.snd_id = snd_id;
    (td_void)memcpy_s(&ao_open.attr, sizeof(ao_open.attr), attr, sizeof(*attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_OPEN, &ao_open);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    *ao = ao_open.h_mod;
    mpi_ao_unlock();

#ifdef SAP_AO_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(*ao) == TD_TRUE) {
        (td_void)mpi_dump_open(*ao, attr->port_attr[0].out_port, AO_DUMP_PORT0_FILE_PATH, AO_DUMP_FILE_SIZE);
        if (attr->port_num > 1) {
            (td_void)mpi_dump_open(*ao, attr->port_attr[1].out_port, AO_DUMP_PORT1_FILE_PATH, AO_DUMP_FILE_SIZE);
        }
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_close(td_handle ao)
{
    td_s32 ret;
    sap_drv_handle ao_handle;

    mpi_ao_check_inited();

#ifdef SAP_AO_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(ao) == TD_TRUE) {
        /* for port 0 */
        (td_void)mpi_dump_close(ao);
        /* for port 1, if exist */
        (td_void)mpi_dump_close(ao);
    }
#endif

    mpi_ao_lock();

    ao_handle.h_mod = ao;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_CLOSE, &ao_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_register_event_proc(td_handle ao, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event ao_event;

    ao_check_null_ptr_return(proc);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_event.h_mod = ao;
    ao_event.context = (td_u64)(td_uintptr_t)context;
    ao_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_REG_EVENT_PROC, &ao_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_bypass(td_handle ao, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable ao_enable;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_BYPASS, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_port_enable(td_handle ao, ao_port_id port, td_bool *enable)
{
    td_s32 ret;
    sap_ao_port_enable ao_enable;

    ao_check_null_ptr_return(enable);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.port = port;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_PORT_ENABLE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    *enable = ao_enable.enable;

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_port_enable(td_handle ao, ao_port_id port, td_bool enable)
{
    td_s32 ret;
    sap_ao_port_enable ao_enable;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.port = port;
    ao_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_PORT_ENABLE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_mute(td_handle ao, ao_port_id port, td_bool *enable)
{
    td_s32 ret;
    sap_ao_port_enable ao_enable;

    ao_check_null_ptr_return(enable);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.port = port;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_MUTE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    *enable = ao_enable.enable;

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_mute(td_handle ao, ao_port_id port, td_bool enable)
{
    td_s32 ret;
    sap_ao_port_enable ao_enable;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.port = port;
    ao_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_MUTE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_volume(td_handle ao, ao_port_id port, ao_gain *gain)
{
    td_s32 ret;
    sap_ao_port_gain param;

    ao_check_null_ptr_return(gain);

    mpi_ao_check_inited();
    mpi_ao_lock();

    param.h_mod = ao;
    param.port = port;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_VOLUME, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    (td_void)memcpy_s(gain, sizeof(*gain), &param.gain, sizeof(param.gain));

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_volume(td_handle ao, ao_port_id port, const ao_gain *gain,
    const audio_toggle_config *toggle_config)
{
    td_s32 ret;
    sap_ao_port_gain param;

    ao_check_null_ptr_return(gain);
    ao_check_null_ptr_return(toggle_config);

    mpi_ao_check_inited();
    mpi_ao_lock();

    param.h_mod = ao;
    param.port = port;
    (td_void)memcpy_s(&param.gain, sizeof(param.gain), gain, sizeof(*gain));
    (td_void)memcpy_s(&param.toggle_config, sizeof(param.toggle_config), toggle_config, sizeof(*toggle_config));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_VOLUME, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_channel_mode(td_handle ao, ao_port_id port, ao_track_mode *mode)
{
    td_s32 ret;
    sap_ao_port_mode ao_mode;

    ao_check_null_ptr_return(mode);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_mode.h_mod = ao;
    ao_mode.port = port;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_TRACK_MODE, &ao_mode);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    *mode = ao_mode.mode;

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_channel_mode(td_handle ao, ao_port_id port, ao_track_mode mode)
{
    td_s32 ret;
    sap_ao_port_mode ao_mode;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_mode.h_mod = ao;
    ao_mode.port = port;
    ao_mode.mode = mode;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_TRACK_MODE, &ao_mode);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_all_track_mute(td_handle ao, td_bool *enable)
{
    td_s32 ret;
    sap_drv_enable ao_enable;

    ao_check_null_ptr_return(enable);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_ALL_TRACK_MUTE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }
    *enable = ao_enable.enable;

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_all_track_mute(td_handle ao, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable ao_enable;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_enable.h_mod = ao;
    ao_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_ALL_TRACK_MUTE, &ao_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_attach_output(td_handle ao, ao_port_id port, td_handle output)
{
    td_s32 ret;
    sap_ao_port_output ao_output;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_output.h_mod = ao;
    ao_output.port = port;
    ao_output.h_output = output;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_ATTACH_OUTPUT, &ao_output);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_detach_output(td_handle ao, ao_port_id port, td_handle output)
{
    td_s32 ret;
    sap_ao_port_output ao_output;

    mpi_ao_check_inited();
    mpi_ao_lock();

    ao_output.h_mod = ao;
    ao_output.port = port;
    ao_output.h_output = output;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_DETACH_OUTPUT, &ao_output);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_aef_profile(td_handle ao, ao_aef_profile aef_profile)
{
    td_s32 ret;
    sap_ao_port_aef_profile ao_aef_profile;

    if (ao == 0) {
        return ERR_AO_INVALID_HANDLE;
    }

    mpi_ao_lock();

    ao_aef_profile.h_mod = ao;
    ao_aef_profile.aef_profile = aef_profile;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_AEF_PROFILE, &ao_aef_profile);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, td_void *param, td_u32 param_size)
{
    td_s32 ret;
    sap_ao_port_aef_param ioctl_attr;

    ao_check_null_ptr_return(param);

    if (param_size > AO_AEF_PARAM_SIZE_MAX) {
        return ERR_AO_INVALID_PARAM;
    }

    mpi_ao_check_inited();
    mpi_ao_lock();

    ioctl_attr.h_mod = ao;
    ioctl_attr.cmd = cmd;
    ioctl_attr.port = port;
    ioctl_attr.param_size = param_size;

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_AEF_PARAM, &ioctl_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    ret = memcpy_s(param, param_size, ioctl_attr.param, ioctl_attr.param_size);
    if (ret != EOK) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();

    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, const td_void *param, td_u32 param_size)
{
    td_s32 ret;
    sap_ao_port_aef_param ioctl_attr;

    ao_check_null_ptr_return(param);

    if (param_size > AO_AEF_PARAM_SIZE_MAX) {
        return ERR_AO_INVALID_PARAM;
    }

    mpi_ao_check_inited();
    mpi_ao_lock();

    ioctl_attr.h_mod = ao;
    ioctl_attr.port = port;
    ioctl_attr.cmd = cmd;

    ret = memcpy_s(ioctl_attr.param, AO_AEF_PARAM_SIZE_MAX, param, param_size);
    if (ret != EOK) {
        mpi_ao_unlock();
        return ret;
    }

    ioctl_attr.param_size = param_size;

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_AEF_PARAM, &ioctl_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();

    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_get_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool *enable)
{
    td_s32 ret;
    sap_ao_port_aef_enable param = {
        .h_mod = ao,
        .port = port,
        .effect_type = effect_type,
        .enable = TD_FALSE,
    };

    ao_check_null_ptr_return(enable);

    mpi_ao_check_inited();
    mpi_ao_lock();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_GET_PORT_AEF_ENABLE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();

    *enable = param.enable;
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool enable)
{
    td_s32 ret;
    sap_ao_port_aef_enable param = {
        .h_mod = ao,
        .port = port,
        .effect_type = effect_type,
        .enable = enable,
    };

    mpi_ao_check_inited();
    mpi_ao_lock();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_PORT_AEF_ENABLE, &param);
    if (ret != AUDIO_SUCCESS) {
        mpi_ao_unlock();
        return ret;
    }

    mpi_ao_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_ao_set_spk_cali_coef(td_handle sound, const td_u8 *filter_coefs, td_u32 coef_len)
{
    td_s32 ret;
    sap_spk_cali_coefs cali_filter_coefs;

    ao_check_null_ptr_return(filter_coefs);
    mpi_ao_check_inited();

    if (coef_len == 0 || coef_len > AUDIO_SPK_CALI_COEF_LEN_MAX) {
        return ERR_AO_INVALID_PARAM;
    }

    cali_filter_coefs.h_mod = sound;
    cali_filter_coefs.cali_coefs.coef_len = coef_len;
    ret = memcpy_s(cali_filter_coefs.cali_coefs.coefs, AUDIO_SPK_CALI_COEF_LEN_MAX, filter_coefs, coef_len);
    if (ret != EOK) {
        return ret;
    }

    mpi_ao_lock();
    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_ao_fd, CMD_IOCTL_AO_SET_SPK_CALI, &cali_filter_coefs);
    mpi_ao_unlock();

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
