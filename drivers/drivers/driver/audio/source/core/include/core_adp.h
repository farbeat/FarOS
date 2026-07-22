/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio data port
 * Author: audio
 */

#ifndef __CORE_ADP_H__
#define __CORE_ADP_H__

#include "adp_struct.h"
#include "core_rbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 core_adp_init(td_void);

td_s32 core_adp_deinit(td_void);

td_bool core_adp_check_no_inst(td_void);

td_s32 core_adp_get_def_attr(audio_adp_type type, audio_adp_attr *attr);

td_s32 core_adp_create(td_handle *adp, audio_adp_type type, const audio_adp_attr *attr);

td_s32 core_adp_clone(td_handle adp, audio_adp_type type);

td_s32 core_adp_destroy(td_handle adp);

td_s32 core_adp_get_attr(td_handle adp, audio_adp_attr *attr);

td_s32 core_adp_set_attr(td_handle adp, const audio_adp_attr *attr);

td_s32 core_adp_alloc_shrb(td_handle adp);

td_s32 core_adp_free_shrb(td_handle adp);

td_s32 core_adp_attach_shrb(td_handle adp);

td_s32 core_adp_detach_shrb(td_handle adp);

td_s32 core_adp_get_rbuf(td_handle adp, rbuf_handle *data_buf, rbuf_handle *info_buf);

td_s32 core_adp_query_free(td_handle adp, td_u32 *free_size);

td_s32 core_adp_query_busy(td_handle adp, td_u32 *busy_size);

td_s32 core_adp_get_buffer(td_handle adp, audio_stream_buf *stream);

td_s32 core_adp_put_buffer(td_handle adp, const audio_stream_buf *stream);

td_s32 core_adp_send_stream(td_handle adp, const audio_stream_buf *stream);

td_s32 core_adp_acquire_stream(td_handle adp, audio_stream_buf *stream);

td_s32 core_adp_release_stream(td_handle adp, const audio_stream_buf *stream);

td_s32 core_adp_send_frame(td_handle adp, const audio_frame *frame);

td_s32 core_adp_acquire_frame(td_handle adp, audio_frame *frame);

td_s32 core_adp_release_frame(td_handle adp, const audio_frame *frame);

td_s32 core_adp_register_event_proc(td_handle adp, audio_event_proc event_proc, td_void *context);

td_s32 core_adp_get_proc_info(td_handle adp, adp_proc_item *proc);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_ADP_H__ */
