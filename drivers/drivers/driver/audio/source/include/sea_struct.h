/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for Speech Enhancement & AI
 * Author: audio
 */

#ifndef __SEA_STRUCT_H__
#define __SEA_STRUCT_H__

#include "audio_struct.h"
#include "audio_sea.h"
#include "audio_sea_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (SAP_CHIP_TYPE == socmn1)
#define SEA_LIB_MAX_NUM       1
#else
#define SEA_LIB_MAX_NUM       2
#endif
#define SEA_LIB_SEA_IND       0
#define SEA_LIB_AAI_IND       1
#define SEA_RBUF_MAX_NUM      2
#define SEA_RBUF_DUR_MS       40
#define SEA_PARAM_SIZE_MAX    512
#define SEA_FRAME_MIN_PER_SEC AUDIO_5MS_NUM_PER_SECOND
#define SEA_FRAME_MAX_PER_SEC AUDIO_20MS_NUM_PER_SECOND

typedef struct {
    td_u32 sea_id;
    sea_lib_version sea_version;
} sea_version_info;

typedef struct {
    td_bool enable;
    audio_event_proc event_proc;

    sea_version_info version_info[SEA_ECH_MAX_NUM];

    /* attr */
    sea_pcm_attr pcm_attr;
    sea_eng_attr eng_attr[SEA_ECH_MAX_NUM];

    /* input */
    audio_ring_buf rbuf_in;
    td_u32 in_frame_pms;

    /* output */
    audio_ring_buf rbuf_out[SEA_OUTPUT_MAX][SEA_RBUF_MAX_NUM];
    td_u32 out_frame_pms[SEA_OUTPUT_MAX];

    /* debug info */
    td_u32 send_try_cnt;
    td_u32 send_empty_cnt;
    td_u32 send_fail_cnt;
    td_u32 send_ok_cnt;
    td_u32 org_match_cnt;
    td_u32 user_match_cnt;
} sea_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __SEA_STRUCT_H__ */
