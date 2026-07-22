/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio output
 * Author: audio
 */

#ifndef __DRV_AO_H__
#define __DRV_AO_H__

#include "audio_struct.h"
#include "ao_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_ao_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_open(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_close(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_port_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_port_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_track_mode(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_track_mode(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_all_track_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_all_track_mute(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 ao_read_proc(td_void *file, td_void *private);

td_s32 sap_ioctl_ao_get_aef_param(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_aef_param(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_aef_profile(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_get_port_aef_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_port_aef_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_ao_set_spk_cali(td_u32 cmd, td_void *arg, td_void *file);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_AO_H__ */
