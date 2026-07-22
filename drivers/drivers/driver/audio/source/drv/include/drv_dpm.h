/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio encoder.
 * Author: audio
 */

#ifndef __DRV_DPM_H__
#define __DRV_DPM_H__

#include "audio_struct.h"
#include "dpm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_dpm_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_load_lib(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_unload_lib(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_dpm_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 dpm_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_DPM_H__ */
