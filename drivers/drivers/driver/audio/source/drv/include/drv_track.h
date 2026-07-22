/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio track
 * Author: audio
 */

#ifndef __DRV_TRACK_H__
#define __DRV_TRACK_H__

#include "audio_struct.h"
#include "ao_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_track_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_state(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_set_state(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_set_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_set_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_set_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_track_get_pts(td_u32 cmd, td_void *arg, td_void *file);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_TRACK_H__ */
