/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: connector
 * Author: audio
 */

#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct sap_connector* connector_handle;

td_s32 connector_close(td_handle input, td_handle output);
td_s32 connector_open(td_handle input, td_u32 data_port_id, td_handle output);
td_s32 connector_reset(const connector_handle connector);
td_s32 connector_get_buffer_info(const connector_handle connector, audio_ring_buf *data_buffer);
td_bool connector_check_free(const connector_handle connector, td_u32 data_size);
td_bool connector_check_data(const connector_handle connector, td_u32 data_size);
td_bool connector_check_latency(const connector_handle connector, const audio_pcm_format *pcm_format, td_u32 latency);
td_s32 connector_read_data(const connector_handle connector, td_u8 *data, td_u32 size);
td_s32 connector_peek_frame_info(const connector_handle connector, audio_frame *frame);
td_s32 connector_send_frame(const connector_handle connector, const audio_frame *frame);
td_s32 connector_send_stream_data(const connector_handle connector, const audio_stream_buf *stream);
td_s32 connector_send_stream_info(const connector_handle connector, const audio_stream_buf *stream);
td_s32 connector_send_stream(const connector_handle connector, const audio_stream_buf *stream);
td_s32 connector_acquire_stream(const connector_handle connector, audio_stream_buf *stream, td_bool packet_stream);
td_s32 connector_release_stream(const connector_handle connector, const audio_stream_buf *stream);
td_s32 connector_input_event_callback(connector_handle connector, td_s32 event, td_void *param);
td_s32 connector_output_event_callback(connector_handle connector, td_s32 event, td_void *param);
td_s32 connector_get_delay(connector_handle connector, const audio_pcm_format *pcm_format, td_u32 *delay_ms);
td_s32 connector_get_output_delay(connector_handle connector, const audio_pcm_format *pcm_format, td_u32 *delay_ms);
td_s32 connector_get_attach_info(ab_attach_item *attach_list, td_u32 max_item_num, td_u32 *active_num);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CONNECTOR_H__ */
