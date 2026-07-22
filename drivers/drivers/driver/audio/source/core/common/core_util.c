/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: core util
 * Author: audio
 */

#include "audio_struct.h"
#include "audio_debug.h"
#include "core_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 core_check_sample_rate(audio_sample_rate sample_rate)
{
    td_u32 i;
    const td_u32 sample_rate_table[] = {
        SAMPLE_RATE_4K,
        SAMPLE_RATE_8K,
        SAMPLE_RATE_11K,
        SAMPLE_RATE_12K,
        SAMPLE_RATE_16K,
        SAMPLE_RATE_22K,
        SAMPLE_RATE_24K,
        SAMPLE_RATE_32K,
        SAMPLE_RATE_44K,
        SAMPLE_RATE_48K,
        SAMPLE_RATE_88K,
        SAMPLE_RATE_96K,
        SAMPLE_RATE_176K,
        SAMPLE_RATE_192K,
        SAMPLE_RATE_288K,
        SAMPLE_RATE_384K,
        SAMPLE_RATE_768K,
    };

    for (i = 0; i < sizeof(sample_rate_table) / sizeof(sample_rate_table[0]); i++) {
        if (sample_rate_table[i] == sample_rate) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_s32(sample_rate);
    return ERR_SYS_INVALID_PARAM;
}

td_s32 core_check_sample_rate_range(td_u32 sample_rate)
{
    if ((sample_rate < SAMPLE_RATE_4K) || (sample_rate > SAMPLE_RATE_768K)) {
        sap_err_log_s32(sample_rate);
        return ERR_SYS_INVALID_PARAM;
    }

    return AUDIO_SUCCESS;
}

td_s32 core_check_bit_depth(audio_bit_depth bit_depth)
{
    td_u32 i;
    const td_u32 bit_depth_table[] = {
        BIT_DEPTH_16,
        BIT_DEPTH_24,
        BIT_DEPTH_32,
    };

    for (i = 0; i < sizeof(bit_depth_table) / sizeof(bit_depth_table[0]); i++) {
        if (bit_depth_table[i] == bit_depth) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_s32(bit_depth);
    return ERR_SYS_INVALID_PARAM;
}

td_s32 core_check_channel(audio_channel channel)
{
    td_u32 i;
    const td_u32 channel_table[] = {
        AUDIO_CH_1,
        AUDIO_CH_2,
        AUDIO_CH_4,
        AUDIO_CH_6,
        AUDIO_CH_8,
        AUDIO_CH_16,
    };

    for (i = 0; i < sizeof(channel_table) / sizeof(channel_table[0]); i++) {
        if (channel_table[i] == channel) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_s32(channel);
    return ERR_SYS_INVALID_PARAM;
}

td_s32 core_check_ao_port(ao_port_id port)
{
    td_u32 i;
    const td_u32 port_table[] = {
        AO_PORT_DAC0,
        AO_PORT_DAC1,
        AO_PORT_DAC2,
        AO_PORT_DAC3,
        AO_PORT_I2S0,
        AO_PORT_I2S1,
        AO_PORT_I2S2,
        AO_PORT_I2S3,
        AO_PORT_I2S4,
        AO_PORT_SPDIF0,
        AO_PORT_HDMI0,
        AO_PORT_HDMI1,
        AO_PORT_ARC0,
        AO_PORT_ARC1,
        AO_PORT_USB0,
        AO_PORT_USB1,
        AO_PORT_BT0,
        AO_PORT_BT1,
        AO_PORT_CAST0,
        AO_PORT_CAST1,
    };

    for (i = 0; i < sizeof(port_table) / sizeof(port_table[0]); i++) {
        if (port_table[i] == port) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_h32(port);
    return ERR_SYS_INVALID_PARAM;
}

td_s32 core_check_bool(td_bool val)
{
    if (val != TD_FALSE && val != TD_TRUE) {
        sap_err_log_s32(val);
        return ERR_SYS_INVALID_PARAM;
    }

    return AUDIO_SUCCESS;
}

static td_s32 core_check_i2s_mode(audio_i2s_mode i2s_mode)
{
    if (i2s_mode <= I2S_PDM_MODE) {
        return AUDIO_SUCCESS;
    }

    sap_err_log_h32(i2s_mode);
    return ERR_AI_INVALID_PARAM;
}

static td_s32 core_check_i2s_mclk(audio_i2s_mclk i2s_mclk)
{
    if (i2s_mclk < I2S_MCLK_MAX) {
        return AUDIO_SUCCESS;
    }

    sap_err_log_h32(i2s_mclk);
    return ERR_AI_INVALID_PARAM;
}

static td_s32 core_check_i2s_bclk(audio_i2s_bclk i2s_bclk)
{
    td_u32 i;
    const td_u32 i2s_bclk_table[] = {
        I2S_BCLK_1_DIV,
        I2S_BCLK_2_DIV,
        I2S_BCLK_3_DIV,
        I2S_BCLK_4_DIV,
        I2S_BCLK_6_DIV,
        I2S_BCLK_8_DIV,
        I2S_BCLK_12_DIV,
        I2S_BCLK_16_DIV,
        I2S_BCLK_24_DIV,
        I2S_BCLK_32_DIV,
        I2S_BCLK_48_DIV,
        I2S_BCLK_64_DIV,
        I2S_BCLK_96_DIV,
    };

    for (i = 0; i < sizeof(i2s_bclk_table) / sizeof(i2s_bclk_table[0]); i++) {
        if (i2s_bclk_table[i] == i2s_bclk) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_h32(i2s_bclk);
    return ERR_AI_INVALID_PARAM;
}

static td_s32 core_check_i2s_bit_depth(audio_bit_depth i2s_bit_depth)
{
    td_u32 i;
    const td_u32 i2s_bit_depth_table[] = {
        BIT_DEPTH_8,
        BIT_DEPTH_16,
        BIT_DEPTH_18,
        BIT_DEPTH_20,
        BIT_DEPTH_24,
        BIT_DEPTH_32,
    };

    for (i = 0; i < sizeof(i2s_bit_depth_table) / sizeof(i2s_bit_depth_table[0]); i++) {
        if (i2s_bit_depth_table[i] == i2s_bit_depth) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_h32(i2s_bit_depth);
    return ERR_AI_INVALID_PARAM;
}

static td_s32 core_check_i2s_channel(audio_channel i2s_channel)
{
    td_u32 i;
    const td_u32 i2s_channel_table[] = {
        AUDIO_CH_1,
        AUDIO_CH_2,
        AUDIO_CH_4,
        AUDIO_CH_6,
        AUDIO_CH_8,
    };

    for (i = 0; i < sizeof(i2s_channel_table) / sizeof(i2s_channel_table[0]); i++) {
        if (i2s_channel_table[i] == i2s_channel) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_h32(i2s_channel);
    return ERR_AI_INVALID_PARAM;
}

static td_s32 core_check_i2s_pcm_delay(audio_i2s_pcm_delay i2s_pcm_delay)
{
    td_u32 i;
    const td_u32 i2s_pcm_delay_table[] = {
        I2S_PCM_0_DELAY,
        I2S_PCM_1_DELAY,
        I2S_PCM_8_DELAY,
        I2S_PCM_16_DELAY,
        I2S_PCM_17_DELAY,
        I2S_PCM_24_DELAY,
        I2S_PCM_32_DELAY,
    };

    for (i = 0; i < sizeof(i2s_pcm_delay_table) / sizeof(i2s_pcm_delay_table[0]); i++) {
        if (i2s_pcm_delay_table[i] == i2s_pcm_delay) {
            return AUDIO_SUCCESS;
        }
    }

    sap_err_log_h32(i2s_pcm_delay);
    return ERR_AI_INVALID_PARAM;
}

td_s32 core_check_i2s_attr(audio_i2s_attr i2s_attr)
{
    td_s32 ret;
    ret = core_check_bool(i2s_attr.master);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_mode(i2s_attr.i2s_mode);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_mclk(i2s_attr.mclk);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_bclk(i2s_attr.bclk);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_bit_depth(i2s_attr.bit_depth);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_channel(i2s_attr.channels);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_bool(i2s_attr.sample_rise_edge);
    if (ret != AUDIO_SUCCESS) {
        return ERR_SYS_INVALID_PARAM;
    }

    ret = core_check_i2s_pcm_delay(i2s_attr.pcm_delay_cycle);
    if (ret != AUDIO_SUCCESS) {
        return ERR_AI_INVALID_PARAM;
    }
    return AUDIO_SUCCESS;
}

td_u32 sample_size(td_u32 bit_depth)
{
    switch (bit_depth) {
        case BIT_DEPTH_8:
            return sizeof(td_s8);

        case BIT_DEPTH_16:
            return sizeof(td_s16);

        case BIT_DEPTH_24:
        case BIT_DEPTH_32:
            return sizeof(td_s32);

        case BIT_DEPTH_64:
            return sizeof(td_s64);

        default:
            return 0;
    }
}

/* how many bytes per ms */
td_u32 calc_frame_size_per_ms(const audio_pcm_format *pcm_format)
{
    td_u32 sample_size;

    if (pcm_format->sample_rate < SAMPLE_RATE_4K || pcm_format->sample_rate > SAMPLE_RATE_768K) {
        return 0;
    }

    if (pcm_format->channels < AUDIO_CH_MONO || pcm_format->channels > AUDIO_CH_16) {
        return 0;
    }

    sample_size = calc_sample_size(pcm_format->channels, pcm_format->bit_depth);
    return (sample_size * pcm_format->sample_rate) / AUDIO_MS_NUM_PER_SECOND;
}

/* how many bytes per frame */
td_u32 calc_frame_size_per_frame(const audio_pcm_format *pcm_format)
{
    if (pcm_format->channels < AUDIO_CH_MONO || pcm_format->channels > AUDIO_CH_16) {
        return 0;
    }

    /* 帧长校验为0-40ms */
    if (pcm_format->samples_per_frame == 0 ||
        (pcm_format->samples_per_frame > pcm_format->sample_rate / AUDIO_40MS_NUM_PER_SECOND)) {
        return 0;
    }

    return calc_sample_size(pcm_format->channels, pcm_format->bit_depth) * pcm_format->samples_per_frame;
}

/* how many samples per frame */
td_u32 calc_samples_per_frame(td_u32 frame_size, td_u32 channels, td_u32 bit_depth)
{
    td_u32 sample_size = calc_sample_size(channels, bit_depth);
    if (sample_size == 0) {
        return 0;
    }

    return (frame_size / sample_size);
}

td_u32 calc_sample_size(td_u32 channels, td_u32 bit_depth)
{
    return sample_size(bit_depth) * channels;
}

td_u32 calc_pcm_frame_ms(const audio_pcm_format *pcm_format, td_u32 pcm_size)
{
    td_u32 size_per_ms;

    size_per_ms = calc_frame_size_per_ms(pcm_format);
    if (size_per_ms == 0) {
        return 0;
    }

    return pcm_size / size_per_ms;
}

/* how many bytes per frame */
td_u32 calc_audio_frame_size_per_frame(const audio_frame *frame)
{
    if (frame == TD_NULL) {
        return 0;
    }

    if (frame->channels < AUDIO_CH_MONO || frame->channels > AUDIO_CH_16) {
        return 0;
    }

    return calc_sample_size(frame->channels, frame->bit_depth) * frame->pcm_samples;
}

td_u8 audio_crc8(td_u8 *message, td_u8 len)
{
    td_u8 crc;
    td_u8 i;
    td_u8 crc_len = len;
    td_u8 *crc_message = message;
    crc = 0;
    while ((crc_len--) != 0) {
        crc ^= *crc_message++;
        for (i = 0; i < 8; i++) { /* CRC 8 algorithm */
            if ((crc & 0x01) != 0) {
                crc = (crc >> 1) ^ 0x8c;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
