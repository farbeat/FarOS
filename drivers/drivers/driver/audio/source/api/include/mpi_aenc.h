/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: head file for mpi aenc
 * Author: audio
 */

#ifndef __MPI_AENC_H__
#define __MPI_AENC_H__

#include "aenc_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_aenc_init(td_void);

td_s32 mpi_aenc_deinit(td_void);

td_s32 mpi_aenc_load_codec(td_handle aenc, ha_codec_id codec_id, const td_char *name);

td_s32 mpi_aenc_unload_codec(ha_codec_id codec_id);

td_s32 mpi_aenc_open(td_handle *aenc);

td_s32 mpi_aenc_close(td_handle aenc);

td_s32 mpi_aenc_register_event_proc(td_handle aenc, audio_event_proc proc, td_void *context);

td_s32 mpi_aenc_start(td_handle aenc);

td_s32 mpi_aenc_stop(td_handle aenc);

td_s32 mpi_aenc_get_attr(td_handle aenc, aenc_attr *attr);

td_s32 mpi_aenc_set_attr(td_handle aenc, const aenc_attr *attr);

td_s32 mpi_aenc_set_bypass(td_handle aenc, td_bool enable);

td_s32 mpi_aenc_attach_output(td_handle aenc, td_handle output);

td_s32 mpi_aenc_detach_output(td_handle aenc, td_handle output);

td_s32 mpi_aenc_set_param(td_handle aenc, aenc_param_type type, const td_void *param, td_u32 param_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_AENC_H__ */
