/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for haid driver
 * Author: audio
 */

#ifndef __DRV_HAID_H__
#define __DRV_HAID_H__

#include "td_type.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_haid_get_default_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_create(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_destroy(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_get_enable(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_get_volume(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_config(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_get_config(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_hardware_config(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_all_config(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_get_all_config(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_sinus_tone_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_sinus_tone_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_scene(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_get_scene(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_spk_cali(td_u32 cmd, const td_void *arg, td_void *file);

td_s32 sap_ioctl_haid_set_mic_cali_volume(td_u32 cmd, const td_void *arg, td_void *file);

td_s32 drv_msg_haid_module_init(audio_core_id core_id);

td_s32 haid_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_HAID_H__ */
