/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv dpm msg
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_dpm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_dpm_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_DPM_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_dpm_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_open(td_handle *dpm)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_OPEN;
    msg_data.input_data_size = 0;
    msg_data.output_data = dpm;
    msg_data.output_data_size = (td_u32)sizeof(td_handle);

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_close(td_handle dpm)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_CLOSE;
    msg_data.handle = dpm;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_start(td_handle dpm)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_START;
    msg_data.handle = dpm;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_stop(td_handle dpm)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_STOP;
    msg_data.handle = dpm;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_get_attr(td_handle dpm, dpm_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_dpm_attr msg_attr;
    audio_buffer *mmz_buf = &msg_attr.mmz_buf;
    dpm_param *in_param = &attr->open_param;

    (td_void)memset_s(mmz_buf, sizeof(*mmz_buf), 0, sizeof(audio_buffer));

    if (in_param->private_data != TD_NULL && in_param->private_data_size > 0) {
        ret = audio_osal_mmz_alloc(mmz_buf, in_param->private_data_size); /* Use mmz to transfer private data to dsp */
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_h32(in_param->private_data_size);
            sap_err_log_fun(audio_osal_mmz_alloc, ret);
            return ret;
        }
    }

    msg_data.cmd = MSG_CORE_DPM_GET_ATTR;
    msg_data.handle = dpm;
    msg_data.input_data_size = 0;
    msg_data.output_data = &msg_attr;
    msg_data.output_data_size = (td_u32)sizeof(msg_attr);
    ret = drv_dpm_send_msg(&msg_data);
    if (ret != AUDIO_SUCCESS) {
        sap_err_log_ret(ret);
    }

    (td_void)memcpy_s(attr, sizeof(dpm_attr), &msg_attr.attr, sizeof(msg_attr.attr));

    if (in_param->private_data != TD_NULL && in_param->private_data_size > 0) {
        ret = memcpy_s(in_param->private_data, in_param->private_data_size, mmz_buf->virt_addr, mmz_buf->size);
        audio_osal_mmz_release(mmz_buf); /* Free mmz used to transfer private data */
        if (ret != EOK) {
            sap_err_log_fun(memcpy_s, ret);
            ret = ERR_DPM_MEM_COPY;
        }
    }

    return ret;
}

td_s32 drv_msg_dpm_set_attr(td_handle dpm, dpm_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_dpm_attr msg_attr;
    dpm_param *in_param = &attr->open_param;
    audio_buffer *mmz_buf = &msg_attr.mmz_buf;

    (td_void)memcpy_s(&msg_attr.attr, sizeof(msg_attr.attr), attr, sizeof(dpm_attr));
    (td_void)memset_s(mmz_buf, sizeof(*mmz_buf), 0, sizeof(audio_buffer));

    if (in_param->private_data_size > 0 && in_param->private_data != TD_NULL) {
        ret = audio_osal_mmz_alloc(mmz_buf, in_param->private_data_size); /* Use mmz to transfer private data to dsp */
        if (ret != AUDIO_SUCCESS) {
            sap_err_log_h32(in_param->private_data_size);
            sap_err_log_fun(audio_osal_mmz_alloc, ret);
            return ret;
        }

        ret = memcpy_s(mmz_buf->virt_addr, mmz_buf->size, in_param->private_data, in_param->private_data_size);
        if (ret != EOK) {
            sap_err_log_fun(memcpy_s, ret);
            ret = ERR_DPM_MEM_COPY;
            goto mmz_free;
        }
    }

    msg_data.cmd = MSG_CORE_DPM_SET_ATTR;
    msg_data.handle = dpm;
    msg_data.input_data = &msg_attr;
    msg_data.input_data_size = (td_u32)sizeof(msg_attr);
    msg_data.output_data_size = 0;

    ret = drv_dpm_send_msg(&msg_data);

mmz_free:
    if (in_param->private_data_size > 0 && in_param->private_data != TD_NULL) {
        audio_osal_mmz_release(mmz_buf); /* Free mmz used to transfer private data */
    }

    return ret;
}

td_s32 drv_msg_dpm_set_bypass(td_handle dpm, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_SET_BYPASS;
    msg_data.handle = dpm;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_attach_output(td_handle dpm, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_ATTACH_OUTPUT;
    msg_data.handle = dpm;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_detach_output(td_handle dpm, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_DETACH_OUTPUT;
    msg_data.handle = dpm;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_get_proc_info(td_handle dpm, dpm_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_GET_PROC_INFO;
    msg_data.handle = dpm;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(dpm_proc_item);

    return drv_dpm_send_msg(&msg_data);
}

td_s32 drv_msg_dpm_get_lib_type(dpm_id dpm_id, audio_lib_type *lib_type)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_DPM_GET_LIB_TYPE;
    msg_data.input_data = &dpm_id;
    msg_data.input_data_size = (td_u32)sizeof(dpm_id);
    msg_data.output_data = lib_type;
    msg_data.output_data_size = (td_u32)sizeof(audio_lib_type);

    return drv_dpm_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
