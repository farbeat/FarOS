/*
 * Copyright (c) CompanyNameMagicTag 2012-2021. All rights reserved.
 * Description: header file for audio output control
 * Author: audio
 */

#ifndef __AO_STRUCT_H__
#define __AO_STRUCT_H__

#include "audio_aef.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AO_PORT_MAX_NUM 2
#define AO_TRACK_MAX_NUM 2
#define AO_VOL_MAX 18    /* max 18 dB */
#define AO_VOL_MIN (-81) /* min -81 dB */
#define AO_VOL_DECIMAL_MAX 1000    /* 0.125dB is 125 */
#define AO_VOL_DECIMAL_MIN (-1000) /* -0.125dB is -125 */
#define AO_VOL_DECIMAL_STEP 125 /* 0.125dB is 125, -0.125dB is -125 */

#define AO_AEF_PARAM_SIZE_MAX  1024

/* one aip; one debug adp */
#define TRACK_RBUF_MAX_NUM 2
#define TRACK_RBUF_DUR_MS 40
#define TRACK_FADE_IN_MS 10
#define TRACK_FADE_OUT_MS 10

/* The ID of the audio output (AO) device. */
typedef enum {
    AO_SND_0 = 0,
    AO_SND_1,
    AO_SND_2,

    AO_SND_ID_MAX
} ao_snd_id;

/* High Precision Gain, ranging from -81dB to 18dB, step 0.125dB */
typedef struct {
    td_s32 integer; /* Interger part of high preicision gain */
    td_s32 decimal; /* decimal part of high preicision gain, if -0.125dB, value is -125 */
} ao_gain;

typedef enum {
    AO_PORT_DAC0 = 0x00,
    AO_PORT_DAC1,
    AO_PORT_DAC2,
    AO_PORT_DAC3,

    AO_PORT_I2S0 = 0x10,
    AO_PORT_I2S1,
    AO_PORT_I2S2,
    AO_PORT_I2S3,
    AO_PORT_I2S4,

    AO_PORT_SPDIF0 = 0x20,

    AO_PORT_HDMI0 = 0x30,
    AO_PORT_HDMI1,

    AO_PORT_ARC0 = 0x40,
    AO_PORT_ARC1,

    AO_PORT_USB0 = 0x50,
    AO_PORT_USB1,

    AO_PORT_BT0 = 0x60,
    AO_PORT_BT1,

    AO_PORT_CAST0 = 0x70,
    AO_PORT_CAST1,

    AO_PORT_ALL = 0x7fff,

    AO_PORT_MAX,
} ao_port_id;

/* Defines the mode of audio channels. */
/* CNcomment: 定义音频声道模式枚举 */
typedef enum {
    /* Stereo */
    /* CNcomment: 立体声 */
    AO_TRACK_STEREO = 0,

    /* Data is output after being mixed in the audio-left channel and audio-right channel. */
    /* CNcomment: 左右声道混合后输出 */
    AO_TRACK_DOUBLE_MONO,

    /* The audio-left channel and audio-right channel output the data of the audio-left channel. */
    /* CNcomment: 左右声道输出左声道数据 */
    AO_TRACK_DOUBLE_LEFT,

    /* The audio-left channel and audio-right channel output the data of the audio-right channel. */
    /* CNcomment: 左右声道输出右声道数据 */
    AO_TRACK_DOUBLE_RIGHT,

    /* Data is output after being exchanged in the audio-left channel and audio-right channel. */
    /* CNcomment: 左右声道数据交换输出 */
    AO_TRACK_EXCHANGE,

    /* Only the data in the audio-right channel is output. */
    /* CNcomment: 只输出右声道数据 */
    AO_TRACK_ONLY_RIGHT,

    /* Only the data in the audio-left channel is output. */
    /* CNcomment: 只输出左声道数据 */
    AO_TRACK_ONLY_LEFT,

    /* Mute */
    /* CNcomment: 静音 */
    AO_TRACK_MUTE,

    AO_TRACK_MODE_MAX,
} ao_track_mode;

typedef enum {
    TRACK_STATE_STOP = 0,
    TRACK_STATE_START,
    TRACK_STATE_PAUSE,

    TRACK_STATE_MAX,
} track_state;

/* Internal Audio DAC outport attribute */
typedef struct {
    td_u32 reserved;
} ao_dac_attr;

/* Audio I2S outport attribute */
typedef struct {
    audio_i2s_attr attr;
} ao_i2s_attr;

/* Audio outport attribute */
typedef struct {
    ao_port_id out_port;
    union {
        ao_dac_attr dac_attr;
        ao_i2s_attr i2s_attr;
    } u;
} ao_out_port_attr;

/* defines audio sound device attribute */
typedef struct {
    audio_channel channels;
    audio_bit_depth bit_depth;
    audio_sample_rate sample_rate;
    td_u32 port_num;                               /* Outport number attached sound */
    ao_out_port_attr port_attr[AO_PORT_MAX_NUM]; /* Outports attr attached sound */
} ao_attr;

/**
 * @if Eng
 * @brief  Audio Sound track type
 * @else
 * @brief  音频声卡通道类型
 * @endif
 */
typedef enum {
    AO_TRACK_TYPE_MASTER = 0, /*!< @if Eng Master Track.
                                   @else   主通道 @endif */
    AO_TRACK_TYPE_SLAVE,      /*!< @if Eng Slave Track.
                                   @else   副通道 @endif */
    AO_TRACK_TYPE_BT_MUSIC,   /*!< @if Eng Bluetooth Music Track.
                                   @else   蓝牙音乐通道 @endif */
    AO_TRACK_TYPE_BT_VOIP,    /*!< @if Eng Bluetooth voip call Track.
                                   @else   蓝牙通话通道 @endif */

    AO_TRACK_TYPE_MAX,
} ao_track_type;

/* Audio output attribute */
typedef struct {
    ao_track_type track_type; /*!< @if Eng Audio Sound track type
                                   @else   音频声卡通道类型 @endif */
    td_u32 fade_in_ms;  /* Fade in time(unit:ms) */
    td_u32 fade_out_ms; /* Fade out time(unit:ms) */
    td_u32 buf_dur_ms;  /* Buffer duration, unit: ms */
} track_attr;

typedef enum {
    AO_AEF_PROFILE_NONE = 0,
    AO_AEF_PROFILE_MUSIC,
    AO_AEF_PROFILE_VOIP,
    AO_AEF_PROFILE_RINGTONE,

    AO_AEF_PROFILE_MAX,
} ao_aef_profile;

typedef struct {
    td_u32 dma_cnt;
    td_u32 bus_time_out_cnt; /* update at isr */

    /* tx only */
    td_u32 total_byte_write;
    td_u32 try_write_cnt;
    td_u32 buf_empty_cnt;         /* update at tx isr */
    td_u32 buf_empty_warning_cnt; /* update at tx isr */
    td_u32 bus_fifo_empty_cnt;    /* update at tx isr */
    td_u32 inf_fifo_empty_cnt;    /* update at tx isr */
    td_u32 inf_empty_cnt_real;    /* update at tx isr */
} ao_port_proc_status;

typedef struct {
    td_u32 frame_size;
    td_u32 aip_num;
    td_u32 aop_num;
    audio_pcm_format pcm_attr;
} engine_proc_item;

typedef struct {
    td_handle handle;
    td_u32 port;
    td_u8 state;
    td_u8 bypass;
    td_u8 port_enable;
    td_u8 mute;
    ao_gain gain;
    td_u8 track_mode;

    td_u8 aef_bypass;
    td_u32 effect_fail;
    aef_type effect_type;
    ao_aef_profile aef_profile;

    td_bool said_enable;
    td_u32 said_beat;

    audio_pcm_format pcm_attr;

    ao_port_proc_status tx_status;
    audio_aiao_buf_info tx_buf;
    audio_port_run_status run_status;
} aop_proc_item;

typedef struct {
    td_handle handle;
    td_u8 state;
    td_bool bypass;
    td_bool eos;
    td_u32 input_pts;
    td_u32 play_pts;

    td_u32 in_frame_pms;
    td_u32 out_frame_pms;

    ao_gain gain;

    audio_pcm_format in_pcm_attr;
    audio_pcm_format out_pcm_attr;

    audio_ring_buf rbuf_in;
    audio_ring_buf rbuf_out[TRACK_RBUF_MAX_NUM];
} aip_proc_item;

typedef struct {
    td_bool bypass;
    td_u32 snd_id;
    ao_attr attr;
    td_handle engine;

    engine_proc_item engine_proc;
    aip_proc_item aip_proc[AO_TRACK_MAX_NUM];
    aop_proc_item aop_proc[AO_PORT_MAX_NUM];
} ao_proc_item;

typedef struct {
    ao_port_proc_status proc_status;
    audio_aiao_buf_info buf_info;
    audio_port_run_status run_status;
} ao_out_port_status;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __AO_STRUCT_H__ */
