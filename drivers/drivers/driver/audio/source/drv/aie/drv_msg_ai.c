/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for ai msg
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
#include "drv_msg_ai.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_ai_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_AI_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_ai_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_get_def_attr(audio_ai_port port, audio_ai_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_GET_DEFATTR;
    msg_data.handle = 0;
    msg_data.input_data = &port;
    msg_data.input_data_size = (td_u32)sizeof(port);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(audio_ai_attr);

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_open(td_handle *ai, audio_ai_port port, const audio_ai_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_ai_open ai_open;

    ai_open.port = port;
    ret = memcpy_s(&ai_open.attr, sizeof(ai_open.attr), attr, sizeof(*attr));
    if (ret != EOK) {
        return ret;
    }

    msg_data.cmd = MSG_CORE_AI_OPEN;
    msg_data.input_data = (td_void *)&ai_open;
    msg_data.input_data_size = (td_u32)sizeof(ai_open);
    msg_data.output_data = ai;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_close(td_handle ai)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_CLOSE;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_start(td_handle ai)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_START;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_stop(td_handle ai)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_STOP;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_get_attr(td_handle ai, audio_ai_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_GET_ATTR;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(audio_ai_attr);

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_set_attr(td_handle ai, audio_ai_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_SET_ATTR;
    msg_data.handle = ai;
    msg_data.input_data = attr;
    msg_data.input_data_size = (td_u32)sizeof(audio_ai_attr);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_set_bypass(td_handle ai, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_SET_BYPASS;
    msg_data.handle = ai;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_reset_vad(td_handle ai)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_RESET_VAD;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_get_gain(td_handle ai, ai_gain *gain)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_GET_GAIN;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data = gain;
    msg_data.output_data_size = (td_u32)sizeof(*gain);

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_set_gain(td_handle ai, const ai_gain *gain)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_SET_GAIN;
    msg_data.handle = ai;
    msg_data.input_data = (td_void *)gain;
    msg_data.input_data_size = (td_u32)sizeof(*gain);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_get_mute(td_handle ai, td_bool *mute)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_GET_MUTE;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data = mute;
    msg_data.output_data_size = (td_u32)sizeof(*mute);

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_set_mute(td_handle ai, td_bool mute)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_SET_MUTE;
    msg_data.handle = ai;
    msg_data.input_data = &mute;
    msg_data.input_data_size = (td_u32)sizeof(mute);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_set_mic_cali_volume(td_handle ai, const ai_cali_volume *cali_volume)
{
    core_msg_data msg_data = {
        .cmd = MSG_CORE_AI_SET_MIC_CALI_VOLUME,
        .handle = ai,
        .input_data = (td_void *)cali_volume,
        .input_data_size = (td_u32)sizeof(*cali_volume),
        .output_data_size = 0,
        .output_data = TD_NULL,
    };

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_attach_output(td_handle ai, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_ATTACH_OUTPUT;
    msg_data.handle = ai;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_detach_output(td_handle ai, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_DETACH_OUTPUT;
    msg_data.handle = ai;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_ai_send_msg(&msg_data);
}

td_s32 drv_msg_ai_get_proc_info(td_handle ai, ai_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AI_GET_PROC_INFO;
    msg_data.handle = ai;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(ai_proc_item);

    return drv_ai_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
