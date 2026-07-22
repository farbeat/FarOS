/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv aenc msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_AENC_H__
#define __DRV_MSG_AENC_H__

#include "ao_struct.h"
#include "audio_msg.h"
#include "aenc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


td_s32 drv_msg_aenc_init(td_void);

td_s32 drv_msg_aenc_deinit(td_void);

td_s32 drv_msg_aenc_open(td_handle *aenc);

td_s32 drv_msg_aenc_close(td_handle aenc);

td_s32 drv_msg_aenc_start(td_handle aenc);

td_s32 drv_msg_aenc_stop(td_handle aenc);

td_s32 drv_msg_aenc_get_attr(td_handle aenc, aenc_attr *attr);

td_s32 drv_msg_aenc_set_attr(td_handle aenc, aenc_attr *attr);

td_s32 drv_msg_aenc_set_bypass(td_handle aenc, td_bool enable);

td_s32 drv_msg_aenc_attach_output(td_handle aenc, td_handle output);

td_s32 drv_msg_aenc_detach_output(td_handle aenc, td_handle output);

td_s32 drv_msg_aenc_get_proc_info(td_handle aenc, aenc_proc_item *proc);

td_s32 drv_msg_aenc_get_lib_type(ha_codec_id codec_id, audio_lib_type *lib_type);

td_s32 drv_msg_aenc_set_param(td_handle aenc, aenc_param_type type, const td_void *param, td_u32 param_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_AENC_H__ */
