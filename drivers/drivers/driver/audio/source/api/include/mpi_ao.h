/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: head file for mpi audio output
 * Author: audio
 */

#ifndef __MPI_AO_H__
#define __MPI_AO_H__

#include "ao_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_ao_init(td_void);

td_s32 mpi_ao_deinit(td_void);

td_s32 mpi_ao_get_def_attr(ao_snd_id snd_id, ao_attr *attr);

td_s32 mpi_ao_open(td_handle *ao, ao_snd_id snd_id, const ao_attr *attr);

td_s32 mpi_ao_close(td_handle ao);

td_s32 mpi_ao_register_event_proc(td_handle ao, audio_event_proc proc, td_void *context);

td_s32 mpi_ao_set_bypass(td_handle ao, td_bool enable);

td_s32 mpi_ao_get_port_enable(td_handle ao, ao_port_id port, td_bool *enable);

td_s32 mpi_ao_set_port_enable(td_handle ao, ao_port_id port, td_bool enable);

td_s32 mpi_ao_get_mute(td_handle ao, ao_port_id port, td_bool *enable);

td_s32 mpi_ao_set_mute(td_handle ao, ao_port_id port, td_bool enable);

td_s32 mpi_ao_get_volume(td_handle ao, ao_port_id port, ao_gain *gain);

td_s32 mpi_ao_set_volume(td_handle ao, ao_port_id port, const ao_gain *gain,
    const audio_toggle_config *toggle_config);

td_s32 mpi_ao_get_channel_mode(td_handle ao, ao_port_id port, ao_track_mode *mode);

td_s32 mpi_ao_set_channel_mode(td_handle ao, ao_port_id port, ao_track_mode mode);

td_s32 mpi_ao_get_all_track_mute(td_handle ao, td_bool *enable);

td_s32 mpi_ao_set_all_track_mute(td_handle ao, td_bool enable);

td_s32 mpi_ao_attach_output(td_handle ao, ao_port_id port, td_handle output);

td_s32 mpi_ao_detach_output(td_handle ao, ao_port_id port, td_handle output);

td_s32 mpi_ao_set_aef_profile(td_handle ao, ao_aef_profile aef_profile);

td_s32 mpi_ao_get_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, td_void *param, td_u32 param_size);

td_s32 mpi_ao_set_aef_param(td_handle ao, ao_port_id port, td_u32 cmd, const td_void *param, td_u32 param_size);

td_s32 mpi_ao_get_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool *enable);

td_s32 mpi_ao_set_port_aef_enable(td_handle ao, ao_port_id port, aef_type effect_type, td_bool enable);

td_s32 mpi_ao_set_spk_cali_coef(td_handle sound, const td_u8 *filter_coefs, td_u32 coef_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_AO_H__ */
