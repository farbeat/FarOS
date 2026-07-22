/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: head file for mpi ao
 * Author: audio
 */

#ifndef __MPI_AI_H__
#define __MPI_AI_H__

#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_ai_init(td_void);

td_s32 mpi_ai_deinit(td_void);

td_s32 mpi_ai_get_def_attr(audio_ai_port port, audio_ai_attr *attr);

td_s32 mpi_ai_open(td_handle *ai, audio_ai_port port, const audio_ai_attr *attr);

td_s32 mpi_ai_close(td_handle ai);

td_s32 mpi_ai_register_event_proc(td_handle ai, audio_event_proc proc, td_void *context);

td_s32 mpi_ai_start(td_handle ai);

td_s32 mpi_ai_stop(td_handle ai);

td_s32 mpi_ai_get_attr(td_handle ai, audio_ai_attr *attr);

td_s32 mpi_ai_set_attr(td_handle ai, const audio_ai_attr *attr);

td_s32 mpi_ai_get_gain(td_handle ai, ai_gain *gain);

td_s32 mpi_ai_set_gain(td_handle ai, const ai_gain *gain);

td_s32 mpi_ai_get_mute(td_handle ai, td_bool *mute);

td_s32 mpi_ai_set_mute(td_handle ai, td_bool mute);

td_s32 mpi_ai_set_bypass(td_handle ai, td_bool enable);

td_s32 mpi_ai_reset_vad(td_handle ai);

td_s32 mpi_ai_set_mic_cali_volume(td_handle ai, const ai_cali_volume *cali_volume);

td_s32 mpi_ai_attach_output(td_handle ai, td_handle output);

td_s32 mpi_ai_detach_output(td_handle ai, td_handle output);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_AI_H__ */
