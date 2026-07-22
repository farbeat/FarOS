/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: head file for mpi audio track
 * Author: audio
 */

#ifndef __MPI_TRACK_H__
#define __MPI_TRACK_H__

#include "ao_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_track_init(td_void);

td_s32 mpi_track_deinit(td_void);

td_s32 mpi_track_get_def_attr(track_attr *attr);

td_s32 mpi_track_open(td_handle *track, td_handle sound, const track_attr *attr);

td_s32 mpi_track_close(td_handle track);

td_s32 mpi_track_register_event_proc(td_handle track, audio_event_proc proc, td_void *context);

td_s32 mpi_track_set_bypass(td_handle track, td_bool enable);

td_s32 mpi_track_get_state(td_handle track, track_state *state);

td_s32 mpi_track_set_state(td_handle track, track_state state, const audio_toggle_config *toggle_config);

td_s32 mpi_track_get_attr(td_handle track, track_attr *attr);

td_s32 mpi_track_set_attr(td_handle track, const track_attr *attr);

td_s32 mpi_track_get_mute(td_handle track, td_bool *enable);

td_s32 mpi_track_set_mute(td_handle track, td_bool enable);

td_s32 mpi_track_get_volume(td_handle track, ao_gain *gain);

td_s32 mpi_track_set_volume(td_handle track, const ao_gain *gain, const audio_toggle_config *toggle_config);

td_s32 mpi_track_attach_output(td_handle track, td_handle output);

td_s32 mpi_track_detach_output(td_handle track, td_handle output);

td_s32 mpi_track_get_play_pts(td_handle track, td_s64 *pts);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_TRACK_H__ */
