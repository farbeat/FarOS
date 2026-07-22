/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: header file for audio sys
 * Author: audio
 */

#ifndef __SYS_STRUCT_H__
#define __SYS_STRUCT_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MALLOC_BLOCK_NUM 60
#define MCPS_BLOCK_NUM 10
#define SHARE_MEM_BLOCK_NUM 9
#define STATIC_BLOCK_NUM 8
#define DYNAMIC_BLOCK_NUM 3

#define MCPS_TO_KCPS_RATIO 1000
#define MALLOC_NAME_LEN 16
#define SEGMENT_NAME_LEN 6

typedef enum {
    SYS_EVENT_CPS_CHANGE = 0,
    SYS_EVENT_NO_SERVICE,
    SYS_EVENT_NO_STREAM,
    SYS_EVENT_OFFLINE_PROC,
    SYS_EVENT_TYPE_MAX,
} sys_event_type;

typedef enum {
    SYS_STATE_DEEP_STANDBY = 0,
    SYS_STATE_ACTIVE_STANDBY,
    SYS_STATE_ACTIVE,
    SYS_STATE_MAX
} sys_state_type;

typedef struct {
    td_u32 total_tog_bias;
    td_u32 cur_tog_bias;
    td_u32 max_tog_bias;
    td_u32 min_tog_bias;
    td_u32 avg_tog_bias;
} tog_bias_block;

typedef struct {
    td_u32 mod_id;
    td_u32 peak;
    td_u32 ave;
} mcps_block;

typedef struct {
    td_u32 start_addr;
    td_u32 size;
} malloc_block;

typedef struct {
    td_u32 addr;
    td_u32 data_size;
    td_u32 info_size;
} share_mem_block;

typedef struct {
    td_char name[SEGMENT_NAME_LEN];
    td_u32 size;
    td_u32 used;
} segment_block;

typedef struct {
    /* debug */
    audio_debug_cfg debug_cfg;

    /* sync_status */
    td_u32 irq_count;
    tog_bias_block tog_bias;

    /* profile */
    td_u32 total_kcps;
    td_u32 idle_kcps;
    td_u32 msg_kcps;
    td_u32 driver_kcps;
    mcps_block mcps_list[MCPS_BLOCK_NUM];

    /* memory */
    td_u32 mem_total;
    td_u32 mem_used;
    segment_block static_list[STATIC_BLOCK_NUM];
    segment_block dynamic_list[DYNAMIC_BLOCK_NUM];

    /* heap */
    td_u32 heap_ptr;
    td_u32 heap_cached;

    /* malloc */
    td_u32 malloc_used;
    td_u32 malloc_cnt;
    malloc_block malloc_list[MALLOC_BLOCK_NUM];

    /* share memory */
    td_u32 share_mem_total;
    td_u32 share_mem_used;
    share_mem_block share_mem_list[SHARE_MEM_BLOCK_NUM];
} sys_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __SYS_STRUCT_H__ */
