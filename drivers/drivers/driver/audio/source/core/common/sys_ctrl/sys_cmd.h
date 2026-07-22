/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: SYS_CMD
 * Author: audio
 */

#ifndef __SYS_CMD_H__
#define __SYS_CMD_H__

#include "audio_type.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 sys_cmd_init(td_void);
td_void sys_cmd_deinit(td_void);
td_void sys_cmd_set_state(audio_core_id core_id, audio_sys_state state);
td_void sys_cmd_get_state(audio_core_id core_id, audio_sys_state *state);
td_void sys_cmd_set_cmd_done(audio_core_id core_id, td_bool cmd_done);
td_void sys_cmd_get_cmd_done(audio_core_id core_id, td_bool *cmd_done);
td_void sys_cmd_set_cmd_ret(audio_core_id core_id, td_u8 cmd_ret);
td_void sys_cmd_get_cmd_ret(audio_core_id core_id, td_u8 *cmd_ret);
td_s32 sys_cmd_check_cmd_done(audio_core_id core_id);

td_void sys_audio_tws_config_reg_init(td_void);

td_void sys_audio_tws_config_deinit(td_void);

td_void sys_audio_tws_config_init(td_void);

td_s32 sys_audio_get_product_form(audio_product_form *product_form);

td_s32 sys_audio_set_product_form(audio_product_form product_form);

td_s32 sys_audio_get_tws_mono_mode(td_bool *mono);

td_s32 sys_audio_set_tws_mono_mode(td_bool mono);

td_s32 sys_audio_get_tws_mode(audio_tws_mode *tws_mode);

td_s32 sys_audio_set_tws_mode(audio_tws_mode tws_mode);

td_s32 sys_audio_get_tws_role(audio_tws_role *tws_role);

td_s32 sys_audio_set_tws_role(audio_tws_role tws_role);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
