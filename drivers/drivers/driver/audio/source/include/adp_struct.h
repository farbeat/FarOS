/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio data port
 * Author: audio
 */

#ifndef __ADP_STRUCT_H__
#define __ADP_STRUCT_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ADP_BLOCK_NUM 8

/* ADP event type */
typedef enum {
    ADP_EVENT_EOS,
    ADP_EVENT_EMPTY,

    ADP_EVENT_TYPE_MAX,
} adp_event_type;

typedef enum {
    ADP_INPUT = 0, /* Input Port */
    ADP_OUTPUT,    /* Output Port */
    ADP_CONNECTOR, /* Reserved */
    ADP_TYPE_MAX
} audio_adp_type;

typedef struct {
    td_u32 send_success_cnt;
    td_u32 send_fail_cnt;
    spend_status_info spend_status;
} adp_proc_send_status;

typedef struct {
    td_u32 get_success_cnt;
    td_u32 get_fail_cnt;
    td_u32 release_success_cnt;
    td_u32 release_fail_cnt;
    spend_status_info spend_status;
} adp_proc_get_status;

typedef struct {
    audio_adp_type adp_type;
    audio_adp_attr adp_attr;
    td_u32 busy_size;
    td_u32 free_size;

    /* input status */
    adp_proc_send_status send_frm_status;
    adp_proc_send_status send_strm_status;

    /* output status */
    adp_proc_get_status get_frm_status;
    adp_proc_get_status get_strm_status;
    adp_proc_get_status get_buf_status;
} adp_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ADP_STRUCT_H__ */
