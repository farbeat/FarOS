/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: core util
 * Author: audio
 */

#ifndef __CORE_UTIL_H__
#define __CORE_UTIL_H__

#include "audio_struct.h"
#include "ao_struct.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 core_check_sample_rate(audio_sample_rate sample_rate);

td_s32 core_check_sample_rate_range(td_u32 sample_rate);

td_s32 core_check_bit_depth(audio_bit_depth bit_depth);

td_s32 core_check_channel(audio_channel channel);

td_s32 core_check_ao_port(ao_port_id port);

td_s32 core_check_bool(td_bool val);

td_s32 core_check_i2s_attr(audio_i2s_attr i2s_attr);

td_u32 sample_size(td_u32 bit_depth);

td_u32 calc_frame_size_per_ms(const audio_pcm_format *pcm_format);

td_u32 calc_frame_size_per_frame(const audio_pcm_format *pcm_format);

td_u32 calc_samples_per_frame(td_u32 frame_size, td_u32 channels, td_u32 bit_depth);

td_u32 calc_sample_size(td_u32 channels, td_u32 bit_depth);

td_u32 calc_pcm_frame_ms(const audio_pcm_format *pcm_format, td_u32 pcm_size);

td_u32 calc_audio_frame_size_per_frame(const audio_frame *frame);

td_u8 audio_crc8(td_u8 *message, td_u8 len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __CORE_UTIL_H__ */
