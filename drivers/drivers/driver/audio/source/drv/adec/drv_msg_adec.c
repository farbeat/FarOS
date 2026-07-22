/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv adec msg
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
#include "drv_msg_adec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_adec_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_ADEC_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_adec_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_open(td_handle *adec)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_OPEN;
    msg_data.input_data_size = 0;
    msg_data.output_data = adec;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_close(td_handle adec)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_CLOSE;
    msg_data.handle = adec;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_start(td_handle adec)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_START;
    msg_data.handle = adec;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_stop(td_handle adec)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_STOP;
    msg_data.handle = adec;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_get_attr(td_handle adec, adec_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_GET_ATTR;
    msg_data.handle = adec;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(adec_attr);

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_set_attr(td_handle adec, adec_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_adec_attr msg_attr = {
        .private_data_size = attr->open_param.private_data_size,
    };

    if (attr->open_param.private_data_size > CORE_MSG_ADEC_PRIV_DATA_SIZE) {
        sap_fatal_log_h32(attr->open_param.private_data_size);
        return ERR_ADEC_INVALID_PARAM;
    }

    if (attr->open_param.private_data_size > 0 && attr->open_param.private_data != TD_NULL) {
        ret = memcpy_s(msg_attr.private_data, sizeof(msg_attr.private_data),
            attr->open_param.private_data, attr->open_param.private_data_size);
        if (ret != EOK) {
            return ret;
        }
    }

    (td_void)memcpy_s(&msg_attr.attr, sizeof(msg_attr.attr), attr, sizeof(adec_attr));

    msg_data.cmd = MSG_CORE_ADEC_SET_ATTR;
    msg_data.handle = adec;
    msg_data.input_data = &msg_attr;
    msg_data.input_data_size = (td_u32)sizeof(msg_attr);
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_set_bypass(td_handle adec, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_SET_BYPASS;
    msg_data.handle = adec;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_attach_output(td_handle adec, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_ATTACH_OUTPUT;
    msg_data.handle = adec;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_detach_output(td_handle adec, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_DETACH_OUTPUT;
    msg_data.handle = adec;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_get_proc_info(td_handle adec, adec_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_GET_PROC_INFO;
    msg_data.handle = adec;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(adec_proc_item);

    return drv_adec_send_msg(&msg_data);
}

td_s32 drv_msg_adec_get_lib_type(ha_codec_id codec_id, audio_lib_type *lib_type)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADEC_GET_LIB_TYPE;
    msg_data.input_data = &codec_id;
    msg_data.input_data_size = (td_u32)sizeof(ha_codec_id);
    msg_data.output_data = lib_type;
    msg_data.output_data_size = (td_u32)sizeof(audio_lib_type);

    return drv_adec_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
