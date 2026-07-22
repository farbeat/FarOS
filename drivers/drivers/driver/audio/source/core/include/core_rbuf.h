/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for ring buffer
 * Author: audio
 */

#ifndef __CORE_RBUF_H__
#define __CORE_RBUF_H__

#include "audio_type.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct core_ring_buffer* rbuf_handle;

#define CORE_RBUF_MAX_NUM 20

typedef enum {
    RING_BUF_ALLOC_EXTERNAL = 0, /* use alloc */
    RING_BUF_ALLOC_INTERNAL,     /* ring buf alloc */

    RING_BUF_ALLOC_MAX
} rbuf_alloc_type;

typedef enum {
    RBUF_CPY_DSP_TODSP = 0x0, /* dsp area copy to dsp area */
    RBUF_CPY_DSP_TOARM,       /* dsp area copy to not dsp area */
    RBUF_CPY_ARM_TODSP,       /* not dsp area copy to dsp area */
    RBUF_CPY_ARM_TOARM,       /* not dsp area copy to not dsp area */
} rbuf_copy_mode;

typedef struct {
    rbuf_alloc_type alloc_type;
    rbuf_copy_mode copy_mode;
    td_u8 *data;
    td_u32 size;
    td_u32 tail_size;
    td_u32 *read_pos;
    td_u32 *write_pos;
} ring_buf;

td_s32 ring_buf_create(rbuf_handle *handle, const ring_buf *attr);

td_s32 ring_buf_destroy(rbuf_handle rbuf);
td_s32 ring_buf_reset(rbuf_handle rbuf);

td_s32 ring_buf_set_read_pos(rbuf_handle rbuf, td_u32 read_pos);

td_s32 ring_buf_read(rbuf_handle rbuf, td_u8 *buf, td_u32 size);

td_s32 ring_buf_freeze_read(rbuf_handle rbuf, td_u8 *buf, td_u32 size);

td_s32 ring_buf_write(rbuf_handle rbuf, const td_u8 *data, td_u32 size);

td_u32 ring_buf_query_busy(rbuf_handle rbuf);

td_u32 ring_buf_get_delay(const rbuf_handle rbuf, const audio_pcm_format *pcm_format);

td_u32 ring_buf_query_free(rbuf_handle rbuf);

td_s32 ring_buf_get_info(rbuf_handle rbuf, td_u32 *buf_addr, td_u32 *buf_len, td_u32 *tail_len);

td_s32 ring_buf_get_pos(const struct core_ring_buffer *rbuf, td_u32 *read_pos, td_u32 *write_pos);

td_u32 ring_buf_linear_query_busy(rbuf_handle rbuf);

td_u32 ring_buf_linear_query_free(rbuf_handle rbuf);

td_s32 ring_buf_linear_get_buffer(rbuf_handle rbuf, td_u32 *data_buf, td_u32 *buf_size);

td_s32 ring_buf_linear_put_buffer(rbuf_handle rbuf, td_u32 data_size);

td_s32 ring_buf_linear_acquire_data(rbuf_handle rbuf, td_u32 *data_buf, td_u32 *buf_size);

td_s32 ring_buf_linear_release_data(rbuf_handle rbuf, td_u32 data_size);

td_s32 ring_buf_flush(rbuf_handle rbuf);

td_u32 ring_buf_read_with_action(rbuf_handle rbuf,
    td_u32 (*action)(td_void *to, const td_void *from, td_u32 size),
    td_u8 *to, td_u32 len);

td_s32 ring_buf_peek(rbuf_handle rbuf, td_u8 *buf, td_u32 size);

td_s32 ring_buf_poke(rbuf_handle rbuf, td_u32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CORE_RBUF_H__ */
