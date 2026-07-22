/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv aenc msg
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
#include "drv_msg_aenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_aenc_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_AENC_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_aenc_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_open(td_handle *aenc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_OPEN;
    msg_data.input_data_size = 0;
    msg_data.output_data = aenc;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_close(td_handle aenc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_CLOSE;
    msg_data.handle = aenc;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_start(td_handle aenc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_START;
    msg_data.handle = aenc;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_stop(td_handle aenc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_STOP;
    msg_data.handle = aenc;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_get_attr(td_handle aenc, aenc_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_aenc_attr msg_attr;

    msg_data.cmd = MSG_CORE_AENC_GET_ATTR;
    msg_data.handle = aenc;
    msg_data.input_data_size = 0;
    msg_data.output_data = &msg_attr;
    msg_data.output_data_size = (td_u32)sizeof(msg_attr);
    ret = drv_aenc_send_msg(&msg_data);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    attr->codec_id = msg_attr.attr.codec_id;
    attr->frame_mode_output = msg_attr.attr.frame_mode_output;
    attr->max_trans_unit_size = msg_attr.attr.max_trans_unit_size;
    attr->open_param.interleaved = msg_attr.attr.open_param.interleaved;
    attr->open_param.samples_per_frame = msg_attr.attr.open_param.samples_per_frame;
    attr->open_param.bit_depth = msg_attr.attr.open_param.bit_depth;
    attr->open_param.sample_rate = msg_attr.attr.open_param.sample_rate;
    attr->open_param.channels = msg_attr.attr.open_param.channels;
    if (attr->open_param.private_data != TD_NULL && attr->open_param.private_data_size > 0) {
        ret = memcpy_s(attr->open_param.private_data, attr->open_param.private_data_size, msg_attr.private_data,
            msg_attr.private_data_size);
        if (ret != EOK) {
            sap_err_log_ret(ret);
            return ERR_ADP_MEM_COPY;
        }
    }
    return AUDIO_SUCCESS;
}

td_s32 drv_msg_aenc_set_attr(td_handle aenc, aenc_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_aenc_attr msg_attr;

    (td_void)memcpy_s(&msg_attr.attr, sizeof(msg_attr.attr), attr, sizeof(aenc_attr));

    if (attr->open_param.private_data_size > CORE_MSG_AENC_PRIV_DATA_SIZE) {
        sap_fatal_log_h32(attr->open_param.private_data_size);
        return ERR_AENC_INVALID_PARAM;
    }

    if (attr->open_param.private_data_size > 0 && attr->open_param.private_data != TD_NULL) {
        ret = memcpy_s(msg_attr.private_data, sizeof(msg_attr.private_data), attr->open_param.private_data,
            attr->open_param.private_data_size);
        if (ret != EOK) {
            sap_err_log_ret(ret);
            return ERR_ADP_MEM_COPY;
        }
        msg_attr.private_data_size = attr->open_param.private_data_size;
        msg_attr.attr.open_param.private_data = msg_attr.private_data;
        msg_attr.attr.open_param.private_data_size = msg_attr.private_data_size;
    } else {
        msg_attr.private_data_size = 0;
        msg_attr.attr.open_param.private_data = TD_NULL;
        msg_attr.attr.open_param.private_data_size = 0;
    }

    msg_data.cmd = MSG_CORE_AENC_SET_ATTR;
    msg_data.handle = aenc;
    msg_data.input_data = &msg_attr;
    msg_data.input_data_size = (td_u32)sizeof(msg_attr);
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_set_bypass(td_handle aenc, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_SET_BYPASS;
    msg_data.handle = aenc;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_attach_output(td_handle aenc, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_ATTACH_OUTPUT;
    msg_data.handle = aenc;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_detach_output(td_handle aenc, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_DETACH_OUTPUT;
    msg_data.handle = aenc;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_get_proc_info(td_handle aenc, aenc_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_GET_PROC_INFO;
    msg_data.handle = aenc;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(aenc_proc_item);

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_get_lib_type(ha_codec_id codec_id, audio_lib_type *lib_type)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_AENC_GET_LIB_TYPE;
    msg_data.input_data = &codec_id;
    msg_data.input_data_size = (td_u32)sizeof(ha_codec_id);
    msg_data.output_data = lib_type;
    msg_data.output_data_size = (td_u32)sizeof(audio_lib_type);

    return drv_aenc_send_msg(&msg_data);
}

td_s32 drv_msg_aenc_set_param(td_handle aenc, aenc_param_type type, const td_void *param, td_u32 param_size)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_aenc_param *aenc_param;

    aenc_param = (msg_data_aenc_param *)audio_osal_malloc(sizeof(msg_data_aenc_param));
    if (aenc_param == TD_NULL) {
        return ERR_AENC_MEM_ALLOC;
    }

    aenc_param->type = type;
    ret = memcpy_s((td_void *)aenc_param->param, sizeof(aenc_param->param), param, (size_t)param_size);
    if (ret != EOK) {
        audio_osal_free(aenc_param);
        return ret;
    }
    aenc_param->param_size = param_size;

    msg_data.cmd = MSG_CORE_AENC_SET_PARAM;
    msg_data.handle = aenc;
    msg_data.input_data = aenc_param;
    msg_data.input_data_size = (td_u32)sizeof(msg_data_aenc_param);
    msg_data.output_data_size = 0;

    ret = drv_aenc_send_msg(&msg_data);
    audio_osal_free(aenc_param);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
