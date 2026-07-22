/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv sea
 * Author: audio
 */

#ifndef __DRV_SEA_H__
#define __DRV_SEA_H__

#include "sea_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sap_ioctl_sea_init(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_deinit(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_load_engine(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_unload_engine(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_eng_caps(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_def_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_create(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_destroy(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_register_event_proc(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_start(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_stop(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_eng_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_set_eng_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_pcm_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_set_pcm_attr(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_param(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_set_param(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_set_bypass(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_item_count(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_get_item_sets(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_add_item(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_remove_item(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_enable_vid_enroll(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_attach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 sap_ioctl_sea_detach_output(td_u32 cmd, td_void *arg, td_void *file);

td_s32 drv_msg_sea_module_init(audio_core_id core_id);

td_s32 sea_read_proc(td_void *file, td_void *private);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_SEA_H__ */
