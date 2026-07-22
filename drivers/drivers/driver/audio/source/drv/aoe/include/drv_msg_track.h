/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv track msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_TRACK_H__
#define __DRV_MSG_TRACK_H__

#include "ao_struct.h"
#include "audio_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 drv_msg_track_init(td_void);

td_s32 drv_msg_track_deinit(td_void);

td_s32 drv_msg_track_get_def_attr(track_attr *attr);

td_s32 drv_msg_track_open(td_handle *track, td_handle sound, const track_attr *attr);

td_s32 drv_msg_track_close(td_handle track);

td_s32 drv_msg_track_get_state(td_handle track, track_state *state);

td_s32 drv_msg_track_set_state(td_handle track, track_state state, const audio_toggle_config *toggle_config);

td_s32 drv_msg_track_get_attr(td_handle track, track_attr *attr);

td_s32 drv_msg_track_set_attr(td_handle track, track_attr *attr);

td_s32 drv_msg_track_get_mute(td_handle track, td_bool *mute);

td_s32 drv_msg_track_set_mute(td_handle track, td_bool mute);

td_s32 drv_msg_track_get_volume(td_handle track, ao_gain *gain);

td_s32 drv_msg_track_set_volume(td_handle track, ao_gain *gain, const audio_toggle_config *toggle_config);

td_s32 drv_msg_track_set_bypass(td_handle track, td_bool enable);

td_s32 drv_msg_track_attach_output(td_handle track, td_handle output);

td_s32 drv_msg_track_detach_output(td_handle track, td_handle output);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_TRACK_H__ */
