/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio data port
 * Author: audio
 */

#ifndef __DRV_ADP_H__
#define __DRV_ADP_H__

#include "adp_struct.h"
#include "audio_struct.h"
#include "audio_errno.h"
#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_adp_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_alloc_shrb(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_free_shrb(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_map(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_unmap(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_adp_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 drv_msg_adp_module_init(audio_core_id core_id);

td_s32 adp_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_ADP_H__ */
