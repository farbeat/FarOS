/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __CORE_AI_H__
#define __CORE_AI_H__

#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef td_s32 (*ai_data_proc)(td_handle ai, audio_frame *frame, td_void *context);

td_s32 core_ai_get_default_attr(audio_ai_port port, audio_ai_attr *attr);

td_s32 core_ai_create(td_handle *ai, audio_ai_port port, audio_ai_attr *attr);

td_s32 core_ai_destroy(td_handle ai);

td_s32 core_ai_register_event_proc(td_handle ai, audio_event_proc event_proc, td_void *context);

td_s32 core_ai_register_data_proc(td_handle ai, ai_data_proc data_proc, td_void *context);

td_s32 core_ai_start(td_handle ai);

td_s32 core_ai_stop(td_handle ai);

td_s32 core_ai_get_attr(td_handle ai, audio_ai_attr *attr);

td_s32 core_ai_set_attr(td_handle ai, audio_ai_attr *attr);

td_s32 core_ai_get_volume(td_handle ai, ai_gain *gain);

td_s32 core_ai_set_volume(td_handle ai, const ai_gain *attr);

td_s32 core_ai_get_mute(td_handle ai, td_bool *mute);

td_s32 core_ai_set_mute(td_handle ai, td_bool mute);

td_s32 core_ai_set_mic_cali_volume(td_handle ai, const ai_cali_volume *cali_volume);

td_s32 core_ai_set_bypass(td_handle sea, td_bool enable);

td_s32 core_ai_reset_vad(td_handle ai);

td_s32 core_ai_get_proc_info(td_handle ai, ai_proc_item *proc);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_AI_H__ */
