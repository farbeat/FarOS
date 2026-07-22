/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio encoder.
 * Author: audio
 */

#ifndef __DRV_AENC_H__
#define __DRV_AENC_H__

#include "audio_struct.h"
#include "ha_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_aenc_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_load_codec(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_unload_codec(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_aenc_set_param(td_u32 cmd, td_void *arg, td_void *file);

td_s32 aenc_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_AENC_H__ */
