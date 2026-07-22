/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv sea msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_SEA_H__
#define __DRV_MSG_SEA_H__

#include "audio_msg.h"
#include "sea_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_msg_sea_init(td_void);

td_s32 drv_msg_sea_deinit(td_void);

td_s32 drv_msg_sea_get_eng_caps(sea_lib_id lib_id, sea_eng_caps *eng_caps);

td_s32 drv_msg_sea_get_def_pcm_attr(sea_eng_sel *eng_sel, sea_pcm_attr *attr);

td_s32 drv_msg_sea_create(td_handle *sea, sea_eng_sel *eng_sel, const sea_pcm_attr *attr);

td_s32 drv_msg_sea_destroy(td_handle sea);

td_s32 drv_msg_sea_start(td_handle sea);

td_s32 drv_msg_sea_stop(td_handle sea);

td_s32 drv_msg_sea_get_eng_attr(td_handle sea, sea_eng_attr *attr);

td_s32 drv_msg_sea_set_eng_attr(td_handle sea, sea_eng_attr *attr);

td_s32 drv_msg_sea_get_pcm_attr(td_handle sea, sea_pcm_attr *attr);

td_s32 drv_msg_sea_set_pcm_attr(td_handle sea, sea_pcm_attr *attr);

td_s32 drv_msg_sea_get_param(td_handle sea, td_u32 type, td_void *param, td_u32 size);

td_s32 drv_msg_sea_set_param(td_handle sea, td_u32 type, const td_void *param, td_u32 size);

td_s32 drv_msg_sea_set_bypass(td_handle sea, td_bool enable);

td_s32 drv_msg_sea_get_item_count(td_handle sea, sea_item_type type, td_u32 *count);

td_s32 drv_msg_sea_get_item_sets(td_handle sea, sea_item_type type, sea_item_sets *sets);

td_s32 drv_msg_sea_add_item(td_handle sea, sea_item_type type, sea_info_item *item);

td_s32 drv_msg_sea_remove_item(td_handle sea, sea_item_type type, sea_info_item *item);

td_s32 drv_msg_sea_enable_vid_enroll(td_handle sea, td_bool enable);

td_s32 drv_msg_sea_attach_output(td_handle sea, sea_output_type type, td_handle output);

td_s32 drv_msg_sea_detach_output(td_handle sea, sea_output_type type, td_handle output);

td_s32 drv_msg_sea_get_proc_info(td_handle sea, sea_proc_item *proc);

td_s32 drv_msg_sea_query_lib_status(sea_lib_id lib_id, audio_lib_load_type load_type, td_bool *need_action);

td_s32 drv_msg_sea_update_lib_status(sea_lib_id lib_id, audio_lib_load_type load_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_SEA_H__ */
