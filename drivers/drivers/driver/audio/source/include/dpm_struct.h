/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for dpm
 * Author: audio
 */

#ifndef __DPM_STRUCT_H__
#define __DPM_STRUCT_H__

#include "dpm.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* one work output; one debug adp */
#define DPM_RBUF_MAX_NUM 2
#define DPM_RBUF_DURA_MS 40
#define DPM_FRAME_MIN_PER_SEC AUDIO_5MS_NUM_PER_SECOND
#define DPM_FRAME_MAX_PER_SEC AUDIO_20MS_NUM_PER_SECOND

typedef struct {
    dpm_id dpm_id;
    dpm_param open_param; /* dpm open param */
} dpm_attr;

typedef struct {
    td_bool bypass;
    td_bool eos;
    td_s32 state;
    td_u32 frame_size;
    td_u32 proc_data_count;
    td_u32 not_enough_data_cnt;

    dpm_id dpm_id;
    audio_pcm_format pcm_attr;

    td_u32 private_data[DPM_PRIVATE_DATA_MAX_SIZE];
    td_u32 private_data_size;

    audio_ring_buf rbuf_in;
    audio_ring_buf rbuf_out[DPM_RBUF_MAX_NUM];
} dpm_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __DPM_STRUCT_H__ */
