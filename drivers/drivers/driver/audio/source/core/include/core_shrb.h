/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for ring buffer
 * Author: audio
 */

#ifndef __CORE_SHRB_H__
#define __CORE_SHRB_H__

#include "audio_struct.h"
#include "core_util.h"
#include "core_rbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SHRB_BUFFER_MAX_NUM 8

typedef struct sap_core_shrb_inst* shrb_handle;

typedef enum {
    SHRB_MEM_NULL = 0,
    SHRB_MEM_MMZ,
    SHRB_MEM_MMU,
    SHRB_MEM_MAP,
    SHRB_MEM_MAX,
} shrb_mem_type;

typedef struct {
    td_u32 data_buf_size;
    td_u32 data_buf_tail_size;
    td_u32 info_buf_size;
} shrb_attr;

typedef struct {
    rbuf_handle info_buf;
    rbuf_handle data_buf;
} shrb_buffer;

typedef struct {
    td_s32 mem_type; /* ::shrb_mem_type */
    td_s32 cache;    /* ::td_bool */
    td_u32 size;
    td_s32 fd;
    td_u64 phys_addr;
    td_u64 dma_buf;
} shrb_init_cfg;

td_void core_shrb_get_init_cfg(td_u32 init_reg, shrb_init_cfg *init_cfg);

td_void core_shrb_set_init_cfg(td_u32 init_reg, const shrb_init_cfg *init_cfg);

td_s32 core_shrb_init(audio_core_id core_id, shrb_init_cfg *init_cfg);

td_s32 core_shrb_deinit(td_void);

audio_core_id core_shrb_get_core_id(td_void);

td_s32 core_shrb_alloc(shrb_handle *handle, td_u32 mod_id, const shrb_attr *attr);

td_s32 core_shrb_free(shrb_handle shrb);

td_s32 core_shrb_get_id(td_u32 mod_id, shrb_handle *shrb);

td_s32 core_shrb_get_rbuf(shrb_handle shrb, shrb_buffer *buf);

td_s32 core_shrb_attach(shrb_handle shrb);

td_s32 core_shrb_detach(shrb_handle shrb);

td_s32 core_shrb_query_size(shrb_handle shrb, td_u32 *data_size, td_u32 *info_size);

td_s32 core_shrb_query_free(const shrb_buffer *buf, td_u32 *free_size);

td_s32 core_shrb_query_busy(const shrb_buffer *buf, td_u32 *data_size);

td_s32 core_shrb_get_buffer(const shrb_buffer *buf, audio_stream_buf *stream);

td_s32 core_shrb_put_buffer(const shrb_buffer *buf, const audio_stream_buf *stream);

td_s32 core_shrb_send_frame(const shrb_buffer *buf, const audio_frame *frame);

td_s32 core_shrb_send_stream(const shrb_buffer *buf, const audio_stream_buf *stream);

td_s32 core_shrb_acquire_frame(const shrb_buffer *buf, audio_frame *frame);

td_s32 core_shrb_release_frame(const shrb_buffer *buf, const audio_frame *frame);

td_s32 core_shrb_acquire_stream(const shrb_buffer *buf, audio_stream_buf *stream);

td_s32 core_shrb_release_stream(const shrb_buffer *buf, const audio_stream_buf *stream);

td_s32 core_shrb_get_eos(shrb_handle shrb, td_bool *eos);
td_s32 core_shrb_set_eos(shrb_handle shrb, td_bool eos);

td_s32 core_shrb_query_mod_ids(td_u32 *mod_ids, td_u32 *mod_cnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_SHRB_H__ */
