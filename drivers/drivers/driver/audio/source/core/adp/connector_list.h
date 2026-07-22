/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: connector list
 * Author: audio
 */

#ifndef __CONNECTOR_LIST_H__
#define __CONNECTOR_LIST_H__

#include "osal_list.h"
#include "connector.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_void connector_list_del_all_node(struct osal_list_head *head);
td_void connector_list_del_node(struct osal_list_head *head, connector_handle connector);
td_s32 connector_list_add_node(struct osal_list_head *head, connector_handle connector);
td_s32 connector_list_reset(const struct osal_list_head *head);
td_bool connector_list_check_free(const struct osal_list_head *head, td_u32 size);
td_bool connector_list_check_latency(const struct osal_list_head *head,
    const audio_pcm_format *pcm_format, td_u32 latency);
td_s32 connector_list_send_frame(struct osal_list_head *head, const audio_frame *frame);
td_s32 connector_list_send_stream_data(struct osal_list_head *head, const audio_stream_buf *stream);
td_s32 connector_list_send_stream_info(struct osal_list_head *head, const audio_stream_buf *stream);
td_s32 connector_list_send_stream(struct osal_list_head *head, const audio_stream_buf *stream);
td_s32 connector_list_get_buffer_info(const struct osal_list_head *head,
    audio_ring_buf *data_buffer, td_u32 list_len);
td_s32 connector_list_output_event_callback(struct osal_list_head *head, td_s32 event, td_void *param);
td_s32 connector_list_output_get_delay(struct osal_list_head *head,
    const audio_pcm_format *pcm_format, td_u32 *delay_ms);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CONNECTOR_LIST_H__ */
