/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: head file for mpi adp
 * Author: audio
 */

#ifndef __MPI_ADP_H__
#define __MPI_ADP_H__

#include "audio_struct.h"
#include "adp_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 mpi_adp_init(td_void);

td_s32 mpi_adp_deinit(td_void);

td_s32 mpi_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr);

td_s32 mpi_adp_open(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr);

td_s32 mpi_adp_close(td_handle adp);

td_s32 mpi_adp_alloc_output_shrb(td_handle adp, td_handle mod, td_u32 data_port_id);

td_s32 mpi_adp_alloc_input_shrb(td_handle adp, td_handle mod, td_u32 data_port_id);

td_s32 mpi_adp_free_shrb(td_handle adp);

td_s32 mpi_adp_map(td_handle adp);

td_s32 mpi_adp_unmap(td_handle adp);

td_s32 mpi_adp_get_attr(td_handle adp, audio_adp_attr *attr);

td_s32 mpi_adp_set_attr(td_handle adp, const audio_adp_attr *attr);

td_s32 mpi_adp_attach_output(td_handle adp, td_handle output);

td_s32 mpi_adp_detach_output(td_handle adp, td_handle output);

td_s32 mpi_adp_register_event_proc(td_handle adp, audio_event_proc proc, td_void *context);

td_s32 mpi_adp_query_free(td_handle adp, td_u32 *free_size);

td_s32 mpi_adp_query_busy(td_handle adp, td_u32 *busy_size);

td_s32 mpi_adp_get_buffer(td_handle adp, audio_stream_buf *stream);

td_s32 mpi_adp_put_buffer(td_handle adp, const audio_stream_buf *stream);

td_s32 mpi_adp_send_stream(td_handle adp, const audio_stream_buf *stream);

td_s32 mpi_adp_acquire_stream(td_handle adp, audio_stream_buf *stream);

td_s32 mpi_adp_release_stream(td_handle adp, const audio_stream_buf *stream);

td_s32 mpi_adp_send_frame(td_handle adp, const audio_frame *frame);

td_s32 mpi_adp_acquire_frame(td_handle adp, audio_frame *frame);

td_s32 mpi_adp_release_frame(td_handle adp, const audio_frame *frame);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_ADP_H__ */
