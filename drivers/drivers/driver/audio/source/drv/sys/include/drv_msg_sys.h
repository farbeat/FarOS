/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv sys msg
 * Author: audio
 */

#ifndef __DRV_MSG_SYS_H__
#define __DRV_MSG_SYS_H__

#include "audio_msg.h"
#include "audio_struct.h"
#include "sys_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_msg_sys_init(td_void);

td_s32 drv_msg_sys_deinit(td_void);

td_s32 drv_msg_sys_set_total_cps(td_handle sys, td_u32 total_cps);

td_s32 drv_msg_sys_set_idle_timeout(td_handle sys, td_u32 timeout);

td_s32 drv_msg_sys_get_proc_info(td_handle sys, sys_proc_item *proc);

td_s32 drv_msg_sys_set_clock_attr(td_handle sys, const audio_clock_attr *attr);

td_s32 drv_msg_sys_clear_overlay_section(audio_lib_id type);

td_s32 drv_msg_sys_set_dsp_log_level(td_u32 log_level);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_MSG_SYS_H__ */
