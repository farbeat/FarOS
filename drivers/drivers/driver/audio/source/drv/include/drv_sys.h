/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio system
 * Author: audio
 */

#ifndef __DRV_SYS_H__
#define __DRV_SYS_H__

#include "audio_struct.h"
#include "audio_errno.h"
#include "sys_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_sys_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_debug_cfg(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_get_debug_cfg(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_product_form(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_tws_mono_mode(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_tws_mode(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_tws_role(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sys_set_dsp_log_level(td_u32 cmd, td_void *arg, td_void *file);

td_s32 drv_msg_sys_module_init(audio_core_id core_id);

td_s32 sys_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_SYS_H__ */
