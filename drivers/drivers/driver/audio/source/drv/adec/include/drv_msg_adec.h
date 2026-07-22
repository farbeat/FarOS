/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv adec msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_ADEC_H__
#define __DRV_MSG_ADEC_H__

#include "ha_codec.h"
#include "audio_msg.h"
#include "adec_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_msg_adec_init(td_void);

td_s32 drv_msg_adec_deinit(td_void);

td_s32 drv_msg_adec_open(td_handle *adec);

td_s32 drv_msg_adec_close(td_handle adec);

td_s32 drv_msg_adec_start(td_handle adec);

td_s32 drv_msg_adec_stop(td_handle adec);

td_s32 drv_msg_adec_get_attr(td_handle adec, adec_attr *attr);

td_s32 drv_msg_adec_set_attr(td_handle adec, adec_attr *attr);

td_s32 drv_msg_adec_set_bypass(td_handle adec, td_bool enable);

td_s32 drv_msg_adec_attach_output(td_handle adec, td_handle output);

td_s32 drv_msg_adec_detach_output(td_handle adec, td_handle output);

td_s32 drv_msg_adec_get_proc_info(td_handle adec, adec_proc_item *proc);

td_s32 drv_msg_adec_get_lib_type(ha_codec_id codec_id, audio_lib_type *lib_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_ADEC_H__ */
