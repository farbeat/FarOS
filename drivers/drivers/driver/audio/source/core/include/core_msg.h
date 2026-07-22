/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for message
 * Author: audio
 */

#ifndef __CORE_MSG_H__
#define __CORE_MSG_H__

#include "audio_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 core_msg_register_module(audio_core_id from, core_msg_module msg_mod, core_msg_func msg_func);

td_s32 core_msg_register_sys_active(audio_core_id to, core_msg_sys_active sys_active);

td_s32 core_msg_register_interrupt(audio_core_id to, core_msg_shoot msg_shoot);

td_s32 core_msg_init(audio_core_id from, audio_core_id to);

td_s32 core_msg_deinit(audio_core_id from, audio_core_id to);

td_s32 core_msg_send(audio_core_id to, core_msg_module msg_mod, core_msg_data *msg_data, td_u32 timeout);

td_s32 core_msg_process(audio_core_id from);

td_bool core_msg_check_empty(audio_core_id from);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CORE_MSG_H__ */
