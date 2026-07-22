/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: head file for mpi vad
 * Author: audio
 */

#ifndef __MPI_VAD_H__
#define __MPI_VAD_H__

#include "vad_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_vad_init(td_void);

td_s32 mpi_vad_deinit(td_void);

td_s32 mpi_vad_get_def_attr(audio_vad_attr *attr);

td_s32 mpi_vad_open(td_handle *vad, const audio_vad_attr *attr);

td_s32 mpi_vad_close(td_handle vad);

td_s32 mpi_vad_register_event_proc(td_handle vad, audio_event_proc proc, td_void *context);

td_s32 mpi_vad_reset(td_handle vad);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_VAD_H__ */
