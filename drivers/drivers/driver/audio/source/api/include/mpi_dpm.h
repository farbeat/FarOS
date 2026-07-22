/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: head file for mpi dpm
 * Author: audio
 */

#ifndef __MPI_DPM_H__
#define __MPI_DPM_H__

#include "dpm_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_dpm_init(td_void);

td_s32 mpi_dpm_deinit(td_void);

td_s32 mpi_dpm_load_lib(td_handle dpm, dpm_id dpm_id, const td_char *name);

td_s32 mpi_dpm_unload_lib(dpm_id dpm_id);

td_s32 mpi_dpm_open(td_handle *dpm);

td_s32 mpi_dpm_close(td_handle dpm);

td_s32 mpi_dpm_register_event_proc(td_handle dpm, audio_event_proc proc, td_void *context);

td_s32 mpi_dpm_start(td_handle dpm);

td_s32 mpi_dpm_stop(td_handle dpm);

td_s32 mpi_dpm_get_attr(td_handle dpm, dpm_attr *attr);

td_s32 mpi_dpm_set_attr(td_handle dpm, const dpm_attr *attr);

td_s32 mpi_dpm_set_bypass(td_handle dpm, td_bool enable);

td_s32 mpi_dpm_attach_output(td_handle dpm, td_handle output);

td_s32 mpi_dpm_detach_output(td_handle dpm, td_handle output);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_DPM_H__ */
