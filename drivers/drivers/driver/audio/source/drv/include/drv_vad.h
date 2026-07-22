/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: header file for drv vad
 * Author: audio
 */

#ifndef __DRV_VAD_H__
#define __DRV_VAD_H__

#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_vad_intr_proc(td_void);

td_s32 drv_vad_set_enable(td_bool enable);

td_s32 sap_ioctl_vad_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_vad_reset(td_u32 cmd, td_void *arg, td_void *file);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_VAD_H__ */
