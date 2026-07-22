/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __DRV_AI_H__
#define __DRV_AI_H__

#include "ai_struct.h"
#include "audio_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_ai_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_reset_vad(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_get_gain(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_set_gain(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_get_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_set_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_set_mic_cali_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ai_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 drv_msg_ai_module_init(audio_core_id core_id);

td_s32 ai_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_AI_H__ */
