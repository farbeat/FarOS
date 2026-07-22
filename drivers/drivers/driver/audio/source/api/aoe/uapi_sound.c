/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: sound user api
 * Author: Audio
 */

#include "soc_uapi_sound.h"
#include "mpi_ao.h"
#include "mpi_track.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "ao_struct.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_snd_init(td_void)
{
    td_s32 ret;

    ret = mpi_track_init();
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = mpi_ao_init();
    if (ret != AUDIO_SUCCESS) {
        (td_void)mpi_track_deinit();
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_snd_deinit(td_void)
{
    td_s32 ret;

    ret = mpi_ao_deinit();
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = mpi_track_deinit();
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_snd_get_default_attr(uapi_snd snd_id, uapi_snd_attr *attr)
{
    return mpi_ao_get_def_attr((ao_snd_id)snd_id, (ao_attr *)(td_void *)attr);
}

td_s32 uapi_snd_open(td_handle *sound, uapi_snd snd_id, const uapi_snd_attr *attr)
{
    return mpi_ao_open(sound, (ao_snd_id)snd_id, (const ao_attr *)(td_void *)attr);
}

td_s32 uapi_snd_close(td_handle sound)
{
    return mpi_ao_close(sound);
}

td_s32 uapi_snd_get_port_enable(td_handle sound, uapi_snd_out_port port, td_bool *enable)
{
    return mpi_ao_get_port_enable(sound, (ao_port_id)port, enable);
}

td_s32 uapi_snd_set_port_enable(td_handle sound, uapi_snd_out_port port, td_bool enable)
{
    return mpi_ao_set_port_enable(sound, (ao_port_id)port, enable);
}

td_s32 uapi_snd_get_mute(td_handle sound, uapi_snd_out_port port, td_bool *mute)
{
    return mpi_ao_get_mute(sound, (ao_port_id)port, mute);
}

td_s32 uapi_snd_set_mute(td_handle sound, uapi_snd_out_port port, td_bool mute)
{
    return mpi_ao_set_mute(sound, (ao_port_id)port, mute);
}

td_s32 uapi_snd_get_volume(td_handle sound, uapi_snd_out_port port, uapi_snd_gain *gain)
{
    return mpi_ao_get_volume(sound, (ao_port_id)port, (ao_gain *)(td_void *)gain);
}

td_s32 uapi_snd_set_volume(td_handle sound, uapi_snd_out_port port, const uapi_snd_gain *gain,
    const uapi_audio_toggle_config *toggle_config)
{
    audio_toggle_config toggle_cfg = {
        .toggle_valid = TD_FALSE,
        .toggle_count = 0,
    };

    if (toggle_config != TD_NULL) {
        toggle_cfg.toggle_valid = toggle_config->toggle_valid;
        toggle_cfg.toggle_count = toggle_config->toggle_count;
    }

    return mpi_ao_set_volume(sound, (ao_port_id)port, (const ao_gain *)(td_void *)gain, &toggle_cfg);
}

td_s32 uapi_snd_get_track_mode(td_handle sound, uapi_snd_out_port port, uapi_snd_track_mode *mode)
{
    return mpi_ao_get_channel_mode(sound, (ao_port_id)port, (ao_track_mode *)(td_void *)mode);
}

td_s32 uapi_snd_set_track_mode(td_handle sound, uapi_snd_out_port port, uapi_snd_track_mode mode)
{
    return mpi_ao_set_channel_mode(sound, (ao_port_id)port, (ao_track_mode)mode);
}

td_s32 uapi_snd_get_all_track_mute(td_handle sound, td_bool *mute)
{
    return mpi_ao_get_all_track_mute(sound, mute);
}

td_s32 uapi_snd_set_all_track_mute(td_handle sound, td_bool mute)
{
    return mpi_ao_set_all_track_mute(sound, mute);
}

td_s32 uapi_snd_attach_output(td_handle sound, uapi_snd_out_port port, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (sound == 0) {
        return ERR_AO_INVALID_HANDLE;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, sound, (td_u32)port);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    ret = mpi_ao_attach_output(sound, (ao_port_id)port, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_snd_detach_output(td_handle sound, uapi_snd_out_port port, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (sound == 0) {
        return ERR_AO_INVALID_HANDLE;
    }

    if (get_sap_mod_type(sound) != SAP_MOD_AO) {
        return ERR_AO_INVALID_HANDLE;
    }

    ret = mpi_ao_detach_output(sound, (ao_port_id)port, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = (td_u32)output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_free_shrb(output);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_snd_get_aef_param(td_handle sound, uapi_snd_out_port port,
    td_u32 cmd, td_void *param, td_u32 param_size)
{
    return mpi_ao_get_aef_param(sound, (ao_port_id)port, cmd, param, param_size);
}

td_s32 uapi_snd_set_aef_param(td_handle sound, uapi_snd_out_port port,
    td_u32 cmd, const td_void *param, td_u32 param_size)
{
    return mpi_ao_set_aef_param(sound, (ao_port_id)port, cmd, param, param_size);
}

td_s32 uapi_snd_set_aef_profile(td_handle sound, uapi_snd_aef_profile aef_profile)
{
    return mpi_ao_set_aef_profile(sound, (ao_aef_profile)aef_profile);
}

td_s32 uapi_snd_get_port_aef_enable(td_handle sound, uapi_snd_out_port out_port,
    uapi_aef_type effect_type, td_bool *enable)
{
    return mpi_ao_get_port_aef_enable(sound, (ao_port_id)out_port, (aef_type)effect_type, enable);
}

td_s32 uapi_snd_set_port_aef_enable(td_handle sound, uapi_snd_out_port out_port,
    uapi_aef_type effect_type, td_bool enable)
{
    return mpi_ao_set_port_aef_enable(sound, (ao_port_id)out_port, (aef_type)effect_type, enable);
}

td_s32 uapi_snd_set_spk_cali_coef(td_handle sound, const td_u8 *filter_coefs, td_u32 coef_len)
{
    return mpi_ao_set_spk_cali_coef(sound, filter_coefs, coef_len);
}

td_s32 uapi_snd_get_track_default_attr(uapi_snd_track_attr *attr)
{
    return mpi_track_get_def_attr((track_attr *)(td_void *)attr);
}

td_s32 uapi_snd_create_track(td_handle *track, td_handle sound, const uapi_snd_track_attr *attr)
{
    return mpi_track_open(track, sound, (const track_attr *)(td_void *)attr);
}

td_s32 uapi_snd_destroy_track(td_handle track)
{
    return mpi_track_close(track);
}

td_s32 uapi_snd_get_track_state(td_handle track, uapi_snd_track_state *state)
{
    return mpi_track_get_state(track, (track_state *)(td_void *)state);
}

td_s32 uapi_snd_track_start(td_handle track, const uapi_audio_toggle_config *toggle_config)
{
    audio_toggle_config toggle_cfg = {
        .toggle_valid = TD_FALSE,
        .toggle_count = 0,
    };

    if (toggle_config != TD_NULL) {
        toggle_cfg.toggle_valid = toggle_config->toggle_valid;
        toggle_cfg.toggle_count = toggle_config->toggle_count;
    }

    return mpi_track_set_state(track, TRACK_STATE_START, &toggle_cfg);
}

td_s32 uapi_snd_track_stop(td_handle track)
{
    audio_toggle_config toggle_cfg = {
        .toggle_valid = TD_FALSE,
        .toggle_count = 0,
    };

    return mpi_track_set_state(track, TRACK_STATE_STOP, &toggle_cfg);
}

td_s32 uapi_snd_track_pause(td_handle track)
{
    audio_toggle_config toggle_cfg = {
        .toggle_valid = TD_FALSE,
        .toggle_count = 0,
    };

    return mpi_track_set_state(track, TRACK_STATE_PAUSE, &toggle_cfg);
}

td_s32 uapi_snd_track_resume(td_handle track)
{
    return uapi_snd_track_start(track, TD_NULL);
}

td_s32 uapi_snd_get_track_mute(td_handle track, td_bool *mute)
{
    return mpi_track_get_mute(track, mute);
}

td_s32 uapi_snd_get_track_play_pts(td_handle track, td_s64 *pts_us)
{
    return mpi_track_get_play_pts(track, pts_us);
}

td_s32 uapi_snd_set_track_mute(td_handle track, td_bool mute)
{
    return mpi_track_set_mute(track, mute);
}

td_s32 uapi_snd_get_track_volume(td_handle track, uapi_snd_gain *gain)
{
    return mpi_track_get_volume(track, (ao_gain *)(td_void *)gain);
}

td_s32 uapi_snd_set_track_volume(td_handle track, const uapi_snd_gain *gain,
    const uapi_audio_toggle_config *toggle_config)
{
    audio_toggle_config toggle_cfg = {
        .toggle_valid = TD_FALSE,
        .toggle_count = 0,
    };

    if (toggle_config != TD_NULL) {
        toggle_cfg.toggle_valid = toggle_config->toggle_valid;
        toggle_cfg.toggle_count = toggle_config->toggle_count;
    }

    return mpi_track_set_volume(track, (const ao_gain *)(td_void *)gain, &toggle_cfg);
}

td_s32 uapi_snd_attach_track_output(td_handle track, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (track == 0 || output == 0) {
        return ERR_TRACK_INVALID_HANDLE;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_alloc_output_shrb(output, track, 0);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    ret = mpi_track_attach_output(track, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_snd_detach_track_output(td_handle track, td_handle output)
{
    td_s32 ret;
    sap_mod_id mod_id;

    if (track == 0 || output == 0) {
        return ERR_TRACK_INVALID_HANDLE;
    }

    if (get_sap_mod_type(track) != SAP_MOD_TRACK) {
        return ERR_TRACK_INVALID_HANDLE;
    }

    ret = mpi_track_detach_output(track, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    mod_id.u32 = output;
    if (mod_id.bits.type == SAP_MOD_ADP) {
        ret = mpi_adp_free_shrb(output);
        if (ret != AUDIO_SUCCESS) {
            return ret;
        }
    }

    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
