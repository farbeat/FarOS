/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv audio data port
 * Author: audio
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_adp_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_ADP_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_adp_init(td_u32 core_id, td_u32 init_reg)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_INIT;
    msg_data.handle = (td_u32)core_id;
    msg_data.input_data = (td_u32 *)&init_reg;
    msg_data.input_data_size = (td_u32)sizeof(init_reg);
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_GET_DEF_ATTR;
    msg_data.input_data = (td_void *)&type;
    msg_data.input_data_size = (td_u32)sizeof(type);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_open(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_adp_open adp_open;

    adp_open.type = type;
    ret = memcpy_s(&adp_open.attr, sizeof(adp_open.attr), attr, sizeof(audio_adp_attr));
    if (ret != EOK) {
        return ERR_ADP_MEM_COPY;
    }

    msg_data.cmd = MSG_CORE_ADP_OPEN;
    msg_data.input_data = (td_void *)&adp_open;
    msg_data.input_data_size = (td_u32)sizeof(adp_open);
    msg_data.output_data = adp;
    msg_data.output_data_size = (td_u32)sizeof(*adp);

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_close(td_handle adp)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_CLOSE;
    msg_data.handle = adp;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_alloc_shrb(td_handle adp, td_handle mod, audio_adp_type adp_type, td_u32 data_port_id)
{
    core_msg_data msg_data;
    msg_data_adp_port data_port;

    data_port.h_mod = mod;
    data_port.adp_type = adp_type;
    data_port.data_port_id = data_port_id;
    msg_data.cmd = MSG_CORE_ADP_ALLOC_SHRB;
    msg_data.handle = adp;
    msg_data.input_data = &data_port;
    msg_data.input_data_size = (td_u32)sizeof(data_port);
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_free_shrb(td_handle adp)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_FREE_SHRB;
    msg_data.handle = adp;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_attach_shrb(td_handle adp)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_ATTACH_SHRB;
    msg_data.handle = adp;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_detach_shrb(td_handle adp)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_DETACH_SHRB;
    msg_data.handle = adp;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_get_attr(td_handle adp, audio_adp_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_GET_ATTR;
    msg_data.handle = adp;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_set_attr(td_handle adp, audio_adp_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_SET_ATTR;
    msg_data.handle = adp;
    msg_data.input_data = attr;
    msg_data.input_data_size = (td_u32)sizeof(*attr);
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_sync_attr(td_handle adp, td_handle mod,
                             audio_adp_type adp_type, td_u32 data_port_id, audio_adp_attr *attr)
{
    core_msg_data msg_data;
    msg_data_adp_port data_port;

    data_port.h_mod = mod;
    data_port.adp_type = adp_type;
    data_port.data_port_id = data_port_id;
    msg_data.cmd = MSG_CORE_ADP_SYNC_ATTR;
    msg_data.handle = adp;
    msg_data.input_data = &data_port;
    msg_data.input_data_size = (td_u32)sizeof(data_port);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_attach_output(td_handle adp, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_ATTACH_OUTPUT;
    msg_data.handle = adp;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

td_s32 drv_msg_adp_detach_output(td_handle adp, td_handle output)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_ADP_DETACH_OUTPUT;
    msg_data.handle = adp;
    msg_data.input_data = &output;
    msg_data.input_data_size = (td_u32)sizeof(output);
    msg_data.output_data_size = 0;

    return drv_adp_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
