/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ab msg driver
 * Author: audio
 */

#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_ab.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_ab_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_AB_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_ab_get_proc_info(ab_proc_item *proc)
{
    core_msg_data msg_data = {
        .cmd = MSG_CORE_AB_GET_PROC_INFO,
        .handle = 0,
        .input_data = TD_NULL,
        .input_data_size = 0,
        .output_data = proc,
        .output_data_size = sizeof(ab_proc_item),
    };

    return drv_ab_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
