/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio decoders
 * Author: audio
 */

#ifndef __ADEC_STRUCT_H__
#define __ADEC_STRUCT_H__

#include "audio_struct.h"
#include "ha_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* one work output; one debug adp */
#define ADEC_RBUF_MAX_NUM 2
#define ADEC_RBUF_DURA_MS 40

typedef struct {
    ha_codec_id codec_id;
    td_u32 buf_dur_ms;             /* Buffer duration, unit: ms */
    ha_codec_dec_param open_param; /* Decoder open param */
} adec_attr;

typedef struct {
    td_bool bypass;
    td_bool eos;
    td_s32 state;
    td_u32 frame_pms;
    td_u32 frame_size;
    td_u32 frame_cnt;
    td_u32 ibuf_dur_ms;
    td_u32 obuf_dur_ms;

    td_u32 max_pcm_out_size;
    td_u32 proc_data_size;
    td_u32 not_enough_data_cnt;
    td_u32 pkg_loss_cnt;
    td_s32 plc_off_threshold;
    td_bool packet_decode;

    /* spend status */
    td_u32 cur_dec_interval; /* current decode interval(us) */
    td_u32 max_dec_interval; /* maximum decode interval(us) */
    td_u32 min_dec_interval; /* minimum decode interval(us) */
    td_u32 avg_dec_interval; /* average decode interval(us) */
    td_u32 cur_dec_consume; /* current decode consume(us) */
    td_u32 max_dec_consume; /* maximum decode consume(us) */
    td_u32 min_dec_consume; /* minimum decode consume(us) */
    td_u32 avg_dec_consume; /* average decode consume(us) */

    /* run status */
    td_u32 underflow_cnt;
    td_u32 acquire_fail_cnt;
    td_u32 dec_fail_cnt;
    td_u32 dec_success_cnt;
    td_u32 release_fail_cnt;
    td_u32 output_fail_cnt;
    td_u32 info_change_cnt;
    td_u32 change_fail_cnt;
    td_u32 eos_success_cnt;
    td_u32 eos_fail_cnt;

    ha_codec_id codec_id;
    audio_pcm_format pcm_attr;

    audio_ring_buf rbuf_in;
    audio_ring_buf rbuf_out[ADEC_RBUF_MAX_NUM];
} adec_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __ADEC_STRUCT_H__ */
