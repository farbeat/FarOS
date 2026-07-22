/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for drv ao msg
 * Author: audio
 * Create: 2020-07-30
 * Notes: NA
 * History: 2020-07-30 1st version
 */

#ifndef __DRV_MSG_AO_H__
#define __DRV_MSG_AO_H__

#include "ao_struct.h"
#include "audio_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define AO_AEF_PARAM_SIZE_MMZ_MAX (16 << 10) /* 16k */

td_s32 drv_msg_ao_init(td_void);

td_s32 drv_msg_ao_deinit(td_void);

td_s32 drv_msg_ao_get_def_attr(ao_snd_id sound, ao_attr *attr);

td_s32 drv_msg_ao_open(td_handle *ao, ao_snd_id sound, const ao_attr *attr);

td_s32 drv_msg_ao_close(td_handle ao);

td_s32 drv_msg_ao_get_port_enable(td_handle ao, ao_port_id port, td_bool *enable);

td_s32 drv_msg_ao_set_port_enable(td_handle ao, ao_port_id port, td_bool enable);

td_s32 drv_msg_ao_get_mute(td_handle ao, ao_port_id port, td_bool *enable);

td_s32 drv_msg_ao_set_mute(td_handle ao, ao_port_id port, td_bool enable);

td_s32 drv_msg_ao_get_volume(td_handle ao, ao_port_id port, ao_gain *gain);

td_s32 drv_msg_ao_set_volume(td_handle ao, ao_port_id port,
    const ao_gain *gain, const audio_toggle_config *toggle_config);

td_s32 drv_msg_ao_get_track_mode(td_handle ao, ao_port_id port, ao_track_mode *mode);

td_s32 drv_msg_ao_set_track_mode(td_handle ao, ao_port_id port, ao_track_mode mode);

td_s32 drv_msg_ao_get_all_track_mute(td_handle ao, td_bool *enable);

td_s32 drv_msg_ao_set_all_track_mute(td_handle ao, td_bool enable);

td_s32 drv_msg_ao_set_bypass(td_handle ao, ao_port_id port, td_bool enable);

td_s32 drv_msg_ao_get_attr(td_handle ao, ao_attr *attr);

td_s32 drv_msg_ao_attach_output(td_handle ao, ao_port_id port, td_handle output);

td_s32 drv_msg_ao_detach_output(td_handle ao, ao_port_id port, td_handle output);

td_s32 drv_msg_ao_get_proc_info(td_handle ao, ao_proc_item *proc);

td_s32 drv_msg_ao_get_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, td_void *param, td_u32 size);

td_s32 drv_msg_ao_set_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, const td_void *param, td_u32 size);

td_s32 drv_msg_ao_set_aef_profile(td_handle ao, ao_aef_profile aef_profile);

td_s32 drv_msg_ao_set_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool enable);

td_s32 drv_msg_ao_get_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool *enable);

td_s32 drv_msg_ao_set_spk_cali(td_handle ao, const audio_spk_cali *flt_coefs);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DRV_MSG_AO_H__ */
