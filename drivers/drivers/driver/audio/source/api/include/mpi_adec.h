/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: head file for mpi adec
 * Author: audio
 */

#ifndef __MPI_ADEC_H__
#define __MPI_ADEC_H__

#include "adec_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_adec_init(td_void);

td_s32 mpi_adec_deinit(td_void);

td_s32 mpi_adec_load_codec(td_handle adec, ha_codec_id codec_id, const td_char *name);

td_s32 mpi_adec_unload_codec(ha_codec_id codec_id);

td_s32 mpi_adec_open(td_handle *adec);

td_s32 mpi_adec_close(td_handle adec);

td_s32 mpi_adec_register_event_proc(td_handle adec, audio_event_proc proc, td_void *context);

td_s32 mpi_adec_start(td_handle adec);

td_s32 mpi_adec_stop(td_handle adec);

td_s32 mpi_adec_get_attr(td_handle adec, adec_attr *attr);

td_s32 mpi_adec_set_attr(td_handle adec, const adec_attr *attr);

td_s32 mpi_adec_set_bypass(td_handle adec, td_bool enable);

td_s32 mpi_adec_attach_output(td_handle adec, td_handle output);

td_s32 mpi_adec_detach_output(td_handle adec, td_handle output);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_ADEC_H__ */
