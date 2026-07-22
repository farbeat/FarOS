/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv track msg
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_track.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_track_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_TRACK_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_track_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_get_def_attr(track_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_GET_DEFATTR;
    msg_data.handle = 0;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(track_attr);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_open(td_handle *track, td_handle sound, const track_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_track_open track_open;

    track_open.sound = sound;
    ret = memcpy_s(&track_open.attr, sizeof(track_open.attr), attr, sizeof(*attr));
    if (ret != EOK) {
        return ret;
    }

    msg_data.cmd = MSG_CORE_TRACK_OPEN;
    msg_data.input_data = &track_open;
    msg_data.input_data_size = (td_u32)sizeof(track_open);
    msg_data.output_data = track;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_close(td_handle track)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_CLOSE;
    msg_data.handle = track;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_get_state(td_handle track, track_state *state)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_GET_STATE;
    msg_data.handle = track;
    msg_data.input_data_size = 0;
    msg_data.output_data = state;
    msg_data.output_data_size = (td_u32)sizeof(track_state);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_set_state(td_handle track, track_state state, const audio_toggle_config *toggle_config)
{
    msg_data_track_state data;
    core_msg_data msg_data;

    data.state = state;
    (td_void)memcpy_s(&data.toggle_config, sizeof(data.toggle_config),
        toggle_config, sizeof(*toggle_config));

    msg_data.cmd = MSG_CORE_TRACK_SET_STATE;
    msg_data.handle = track;
    msg_data.input_data = &data;
    msg_data.input_data_size = (td_u32)sizeof(data);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_get_attr(td_handle track, track_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_GET_ATTR;
    msg_data.handle = track;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(track_attr);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_set_attr(td_handle track, track_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_SET_ATTR;
    msg_data.handle = track;
    msg_data.input_data = attr;
    msg_data.input_data_size = (td_u32)sizeof(track_attr);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_get_mute(td_handle track, td_bool *mute)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_GET_MUTE;
    msg_data.handle = track;
    msg_data.input_data_size = 0;
    msg_data.output_data = mute;
    msg_data.output_data_size = (td_u32)sizeof(td_bool);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_set_mute(td_handle track, td_bool mute)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_SET_MUTE;
    msg_data.handle = track;
    msg_data.input_data = &mute;
    msg_data.input_data_size = (td_u32)sizeof(mute);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_get_volume(td_handle track, ao_gain *gain)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_GET_VOLUME;
    msg_data.handle = track;
    msg_data.input_data_size = 0;
    msg_data.output_data = gain;
    msg_data.output_data_size = (td_u32)sizeof(*gain);

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_set_volume(td_handle track, ao_gain *gain, const audio_toggle_config *toggle_config)
{
    core_msg_data msg_data;
    msg_data_track_gain track_gain;

    (td_void)memcpy_s(&track_gain.gain, sizeof(track_gain.gain), gain, sizeof(*gain));
    (td_void)memcpy_s(&track_gain.toggle_config, sizeof(track_gain.toggle_config),
        toggle_config, sizeof(*toggle_config));
    msg_data.cmd = MSG_CORE_TRACK_SET_VOLUME;
    msg_data.handle = track;
    msg_data.input_data = &track_gain;
    msg_data.input_data_size = (td_u32)sizeof(track_gain);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_set_bypass(td_handle track, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_SET_BYPASS;
    msg_data.handle = track;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_attach_output(td_handle track, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_ATTACH_OUTPUT;
    msg_data.handle = track;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

td_s32 drv_msg_track_detach_output(td_handle track, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_TRACK_DETACH_OUTPUT;
    msg_data.handle = track;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_track_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
