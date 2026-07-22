/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for drv audio system
 * Author: audio
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_sys_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_SYS_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_sys_init(td_void)
{
    core_msg_data msg_data;
    msg_data.cmd = MSG_CORE_SYS_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_deinit(td_void)
{
    core_msg_data msg_data;
    msg_data.cmd = MSG_CORE_SYS_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_set_total_cps(td_handle sys, td_u32 total_cps)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_SET_TOTAL_CPS;
    msg_data.handle = sys;
    msg_data.input_data = &total_cps;
    msg_data.input_data_size = (td_u32)sizeof(total_cps);
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_set_idle_timeout(td_handle sys, td_u32 timeout)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_SET_IDLE_TIMEOUT;
    msg_data.handle = sys;
    msg_data.input_data = &timeout;
    msg_data.input_data_size = (td_u32)sizeof(timeout);
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_get_proc_info(td_handle sys, sys_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_GET_PROC_INFO;
    msg_data.handle = sys;
    msg_data.input_data = TD_NULL;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(sys_proc_item);

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_set_clock_attr(td_handle sys, const audio_clock_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_SET_CLOCK_ATTR;
    msg_data.handle = sys;
    msg_data.input_data = (td_void *)attr;
    msg_data.input_data_size = (td_u32)sizeof(audio_clock_attr);
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_clear_overlay_section(audio_lib_id type)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_CLEAR_OVERLAY_SECTION;
    msg_data.input_data = &type;
    msg_data.input_data_size = (td_u32)sizeof(audio_lib_id);
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

td_s32 drv_msg_sys_set_dsp_log_level(td_u32 log_level)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SYS_SET_DSP_LOG_LEVEL;
    msg_data.input_data = &log_level;
    msg_data.input_data_size = (td_u32)sizeof(td_u32);
    msg_data.output_data_size = 0;

    return drv_sys_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
