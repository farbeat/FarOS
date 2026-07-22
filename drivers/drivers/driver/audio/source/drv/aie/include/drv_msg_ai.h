/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv ai msg
 * Author: audio
 * Create: 2020-06-23
 * Notes: NA
 * History: 2020-06-23 1st version
 */

#ifndef __DRV_MSG_AI_H__
#define __DRV_MSG_AI_H__

#include "audio_msg.h"
#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 drv_msg_ai_init(td_void);
td_s32 drv_msg_ai_deinit(td_void);

td_s32 drv_msg_ai_get_def_attr(audio_ai_port port, audio_ai_attr *attr);
td_s32 drv_msg_ai_open(td_handle *ai, audio_ai_port port, const audio_ai_attr *attr);
td_s32 drv_msg_ai_close(td_handle ai);

td_s32 drv_msg_ai_start(td_handle ai);
td_s32 drv_msg_ai_stop(td_handle ai);
td_s32 drv_msg_ai_get_attr(td_handle ai, audio_ai_attr *attr);
td_s32 drv_msg_ai_set_attr(td_handle ai, audio_ai_attr *attr);
td_s32 drv_msg_ai_set_bypass(td_handle ai, td_bool enable);

td_s32 drv_msg_ai_reset_vad(td_handle ai);
td_s32 drv_msg_ai_get_gain(td_handle ai, ai_gain *gain);
td_s32 drv_msg_ai_set_gain(td_handle ai, const ai_gain *gain);
td_s32 drv_msg_ai_get_mute(td_handle ai, td_bool *mute);
td_s32 drv_msg_ai_set_mute(td_handle ai, td_bool mute);

td_s32 drv_msg_ai_set_mic_cali_volume(td_handle ai, const ai_cali_volume *cali_volume);

td_s32 drv_msg_ai_attach_output(td_handle ai, td_handle output);
td_s32 drv_msg_ai_detach_output(td_handle ai, td_handle output);

td_s32 drv_msg_ai_get_proc_info(td_handle ai, ai_proc_item *proc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_MSG_AO_H__ */
