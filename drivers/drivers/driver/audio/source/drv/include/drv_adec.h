/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: adec driver
 * Author: audio
 */

#ifndef __DRV_ADEC_H__
#define __DRV_ADEC_H__

#include "audio_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_adec_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_load_codec(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_unload_codec(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adec_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 adec_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_ADEC_H__ */
