/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv ao msg
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_ao.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_ao_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_AO_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_ao_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_def_attr(ao_snd_id sound, ao_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_DEFATTR;
    msg_data.handle = 0;
    msg_data.input_data = &sound;
    msg_data.input_data_size = (td_u32)sizeof(sound);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_open(td_handle *ao, ao_snd_id sound, const ao_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_ao_open ao_open;

    ao_open.snd_id = sound;
    ret = memcpy_s(&ao_open.attr, sizeof(ao_open.attr), attr, sizeof(*attr));
    if (ret != EOK) {
        return ret;
    }

    msg_data.cmd = MSG_CORE_AO_OPEN;
    msg_data.input_data = &ao_open;
    msg_data.input_data_size = (td_u32)sizeof(ao_open);
    msg_data.output_data = ao;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_close(td_handle ao)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_CLOSE;
    msg_data.handle = ao;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_port_enable(td_handle ao, ao_port_id port, td_bool *enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_PORT_ENABLE;
    msg_data.handle = ao;
    msg_data.input_data = &port;
    msg_data.input_data_size = (td_u32)sizeof(port);
    msg_data.output_data = enable;
    msg_data.output_data_size = (td_u32)sizeof(td_bool);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_port_enable(td_handle ao, ao_port_id port, td_bool enable)
{
    core_msg_data msg_data;
    msg_data_ao_port_enable port_enable;

    port_enable.port = port;
    port_enable.enable = enable;
    msg_data.cmd = MSG_CORE_AO_SET_PORT_ENABLE;
    msg_data.handle = ao;
    msg_data.input_data = &port_enable;
    msg_data.input_data_size = (td_u32)sizeof(port_enable);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_mute(td_handle ao, ao_port_id port, td_bool *enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_MUTE;
    msg_data.handle = ao;
    msg_data.input_data = &port;
    msg_data.input_data_size = (td_u32)sizeof(port);
    msg_data.output_data = enable;
    msg_data.output_data_size = (td_u32)sizeof(td_bool);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_mute(td_handle ao, ao_port_id port, td_bool enable)
{
    core_msg_data msg_data;
    msg_data_ao_port_enable port_enable;

    port_enable.port = port;
    port_enable.enable = enable;
    msg_data.cmd = MSG_CORE_AO_SET_MUTE;
    msg_data.handle = ao;
    msg_data.input_data = &port_enable;
    msg_data.input_data_size = (td_u32)sizeof(port_enable);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_volume(td_handle ao, ao_port_id port, ao_gain *gain)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_VOLUME;
    msg_data.handle = ao;
    msg_data.input_data = &port;
    msg_data.input_data_size = (td_u32)sizeof(port);
    msg_data.output_data = gain;
    msg_data.output_data_size = (td_u32)sizeof(*gain);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_volume(td_handle ao, ao_port_id port,
    const ao_gain *gain, const audio_toggle_config *toggle_config)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_ao_port_gain port_gain;

    port_gain.port = port;
    (td_void)memcpy_s(&port_gain.gain, sizeof(port_gain.gain), gain, sizeof(*gain));
    (td_void)memcpy_s(&port_gain.toggle_config, sizeof(port_gain.toggle_config),
        toggle_config, sizeof(*toggle_config));

    msg_data.cmd = MSG_CORE_AO_SET_VOLUME;
    msg_data.handle = ao;
    msg_data.input_data = &port_gain;
    msg_data.input_data_size = (td_u32)sizeof(port_gain);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_track_mode(td_handle ao, ao_port_id port, ao_track_mode *mode)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_TRACK_MODE;
    msg_data.handle = ao;
    msg_data.input_data = &port;
    msg_data.input_data_size = (td_u32)sizeof(port);
    msg_data.output_data = mode;
    msg_data.output_data_size = (td_u32)sizeof(ao_track_mode);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_track_mode(td_handle ao, ao_port_id port, ao_track_mode mode)
{
    core_msg_data msg_data;
    msg_data_ao_port_mode port_mode;

    port_mode.port = port;
    port_mode.mode = mode;

    msg_data.cmd = MSG_CORE_AO_SET_TRACK_MODE;
    msg_data.handle = ao;
    msg_data.input_data = &port_mode;
    msg_data.input_data_size = (td_u32)sizeof(port_mode);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_all_track_mute(td_handle ao, td_bool *enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_ALL_TRACK_MUTE;
    msg_data.handle = ao;
    msg_data.input_data_size = 0;
    msg_data.output_data = enable;
    msg_data.output_data_size = (td_u32)sizeof(td_bool);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_all_track_mute(td_handle ao, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_SET_ALL_TRACK_MUTE;
    msg_data.handle = ao;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_bypass(td_handle ao, ao_port_id port, td_bool enable)
{
    core_msg_data msg_data;
    msg_data_ao_port_enable port_enable;

    port_enable.port = port;
    port_enable.enable = enable;
    msg_data.cmd = MSG_CORE_AO_SET_BYPASS;
    msg_data.handle = ao;
    msg_data.input_data = &port_enable;
    msg_data.input_data_size = (td_u32)sizeof(port_enable);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_attr(td_handle ao, ao_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_ATTR;
    msg_data.handle = ao;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_attach_output(td_handle ao, ao_port_id port, td_handle output)
{
    core_msg_data msg_data;
    msg_data_ao_port_output port_output;

    port_output.port = port;
    port_output.output = output;

    msg_data.cmd = MSG_CORE_AO_ATTACH_OUTPUT;
    msg_data.handle = ao;
    msg_data.input_data = &port_output;
    msg_data.input_data_size = (td_u32)sizeof(port_output);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_detach_output(td_handle ao, ao_port_id port, td_handle output)
{
    core_msg_data msg_data;
    msg_data_ao_port_output port_output;

    port_output.port = port;
    port_output.output = output;

    msg_data.cmd = MSG_CORE_AO_DETACH_OUTPUT;
    msg_data.handle = ao;
    msg_data.input_data = &port_output;
    msg_data.input_data_size = (td_u32)sizeof(port_output);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_proc_info(td_handle ao, ao_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_GET_PROC_INFO;
    msg_data.handle = ao;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(ao_proc_item);

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_get_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, td_void *param, td_u32 size)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_ao_port_aef_param port_aef_param = { 0 };
    audio_buffer *mmz_buf = &port_aef_param.mmz_buf;

    if (size == 0 || size > AO_AEF_PARAM_SIZE_MMZ_MAX || param == TD_NULL) {
        return ERR_AO_INVALID_PARAM;
    }

    ret = audio_osal_mmz_alloc(mmz_buf, size); /* Use mmz to transfer param data from dsp */
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(size);
        sap_err_log_fun(audio_osal_mmz_alloc, ret);
        return ERR_AO_MEM_ALLOC;
    }

    port_aef_param.port = port;
    port_aef_param.cmd = cmd;

    msg_data.cmd = MSG_CORE_AO_GET_AEF_PARAM;
    msg_data.handle = ao;
    msg_data.input_data = &port_aef_param;
    msg_data.input_data_size = (td_u32)sizeof(port_aef_param);
    msg_data.output_data_size = 0;

    ret = drv_ao_send_msg(&msg_data);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_ret(ret);
        goto mmz_free;
    }

    ret = memcpy_s(param, size, mmz_buf->virt_addr, mmz_buf->size);
    if (ret != EOK) {
        sap_err_log_fun(memcpy_s, ret);
        ret = ERR_AO_MEM_COPY;
    }

mmz_free:
    audio_osal_mmz_release(mmz_buf); /* Free mmz used to transfer param data */

    return ret;
}

td_s32 drv_msg_ao_set_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, const td_void *param, td_u32 size)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_ao_port_aef_param port_aef_param = { 0 };
    audio_buffer *mmz_buf = &port_aef_param.mmz_buf;

    if (size == 0 || size > AO_AEF_PARAM_SIZE_MMZ_MAX || param == TD_NULL) {
        return ERR_AO_INVALID_PARAM;
    }

    ret = audio_osal_mmz_alloc(mmz_buf, size); /* Use mmz to transfer param data to dsp */
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_h32(size);
        sap_err_log_fun(audio_osal_mmz_alloc, ret);
        return ERR_AO_MEM_ALLOC;
    }

    ret = memcpy_s(mmz_buf->virt_addr, mmz_buf->size, param, size);
    if (ret != EOK) {
        sap_err_log_fun(memcpy_s, ret);
        ret = ERR_AO_MEM_COPY;
        goto mmz_free;
    }

    port_aef_param.port = port;
    port_aef_param.cmd = cmd;

    msg_data.cmd = MSG_CORE_AO_SET_AEF_PARAM;
    msg_data.handle = ao;
    msg_data.input_data = &port_aef_param;
    msg_data.input_data_size = (td_u32)sizeof(port_aef_param);
    msg_data.output_data_size = 0;

    ret = drv_ao_send_msg(&msg_data);

mmz_free:
    audio_osal_mmz_release(mmz_buf); /* Free mmz used to transfer param data */

    return ret;
}

td_s32 drv_msg_ao_set_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool enable)
{
    td_s32 ret;
    msg_data_ao_port_aef_enable data = {
        .port = port,
        .effect_type = effect_type,
        .enable = enable,
    };
    core_msg_data msg = {
        .cmd = MSG_CORE_AO_SET_AEF_ENABLE,
        .handle = ao,
        .input_data_size = (td_u32)sizeof(data),
        .input_data = (td_void *)&data,
        .output_data_size = 0,
        .output_data = TD_NULL,
    };

    ret = drv_ao_send_msg(&msg);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ao_send_msg, ret);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 drv_msg_ao_get_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool *enable)
{
    td_s32 ret;
    msg_data_ao_port_aef_enable data = {
        .port = port,
        .effect_type = effect_type,
        .enable = TD_FALSE,
    };
    core_msg_data msg = {
        .cmd = MSG_CORE_AO_GET_AEF_ENABLE,
        .handle = ao,
        .input_data_size = (td_u32)sizeof(data),
        .input_data = (td_void *)&data,
        .output_data_size = (td_u32)sizeof(data),
        .output_data = (td_void *)&data,
    };

    ret = drv_ao_send_msg(&msg);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_fun(drv_ao_send_msg, ret);
        return ret;
    }

    *enable = data.enable;
    return AUDIO_SUCCESS;
}

td_s32 drv_msg_ao_set_aef_profile(td_handle ao, ao_aef_profile aef_profile)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AO_SET_AEF_PROFILE;
    msg_data.handle = ao;
    msg_data.input_data = &aef_profile;
    msg_data.input_data_size = (td_u32)sizeof(ao_aef_profile);
    msg_data.output_data_size = 0;

    return drv_ao_send_msg(&msg_data);
}

td_s32 drv_msg_ao_set_spk_cali(td_handle ao, const audio_spk_cali *flt_coefs)
{
    core_msg_data msg_data = {
        .cmd = MSG_CORE_AO_SET_SPK_CALI,
        .handle = ao,
        .input_data_size = sizeof(audio_spk_cali),
        .input_data = (td_void *)flt_coefs,
        .output_data_size = 0,
    };

    return drv_ao_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
