/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __AI_STRUCT_H__
#define __AI_STRUCT_H__

#include "ao_struct.h"
#include "vad_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @if Eng
 * @brief  maximum microphone num used by audio input.
 * @else
 * @brief  音频输入使用的麦克风数量最大值
 * @endif
 */
#define AI_MIC_NUM_MAX 2

/* one work output; one debug adp */
#define AI_RBUF_MAX_NUM 2
#define AI_FRAME_PER_SEC AUDIO_10MS_NUM_PER_SECOND
#define AI_FRAME_MIN_PER_SEC AUDIO_5MS_NUM_PER_SECOND
#define AI_FRAME_MAX_PER_SEC AUDIO_20MS_NUM_PER_SECOND

#define AI_VOL_MAX 60     /* max 60 dB */
#define AI_VOL_MIN (-120) /* min -120 dB */
#define AI_VOL_STEP_MIN 1 /* 1dB */
#define AI_VOL_PRECI_COEF 1000     /* 0.125dB is 125 */

#define AI_CALIBRATION_VOL_MAX 12    /* max 12dB */
#define AI_CALIBRATION_VOL_MIN (-12) /* min -12dB */

typedef enum {
    AI_EVENT_VAD_VALID = 0, /* user api event */
    AI_EVENT_VAD_TIMEOUT,   /* user api event */

    AI_EVENT_PORT_EXCEPTION,
    AI_EVENT_DMA_OVERLOAD,
    AI_EVENT_REF_CRG_ERROR,
    AI_EVENT_REF_BIT_DEPTH_ERROR,

    AI_EVENT_MAX,
} ai_event_type;

/* Audio input port type */
typedef enum {
    AI_PORT_I2S0 = 0,
    AI_PORT_I2S1,
    AI_PORT_I2S2,
    AI_PORT_I2S3,
    AI_PORT_I2S4,

    AI_PORT_ADC0 = 0x10,
    AI_PORT_ADC1,
    AI_PORT_ADC2,
    AI_PORT_ADC3,

    AI_PORT_SIF0 = 0x20,

    AI_PORT_HDMIRX0 = 0x30,
    AI_PORT_HDMIRX1,
    AI_PORT_HDMIRX2,
    AI_PORT_HDMIRX3,

    AI_PORT_PDM0 = 0x40,
    AI_PORT_PDM1,
    AI_PORT_PDM2,
    AI_PORT_PDM3,

    AI_PORT_LPADC0 = 0x50,
    AI_PORT_LPADC1,

    AI_PORT_VPU0 = 0x60,

    AI_PORT_MAX = 0xff
} audio_ai_port;

/* Defines the ADC work mode */
typedef enum {
    AI_RX_DEFAULT = 0,
    AI_RX_MAD,
    AI_RX_TYPE_MAX
} ai_rx_type;

typedef enum {
    AI_AFE_DMIC = 0,
    AI_AFE_ADC,
    AI_AFE_LPADC,

    AI_AFE_TYPE_MAX,
} ai_afe_type;

/* Defines the ADC work mode */
typedef enum {
    AI_ADC_NORM_MODE = 0,
    AI_ADC_LP_MODE,  /* Low power mode */
    AI_ADC_HP_MODE,  /* High performance mode */

    AI_ADC_MODE_MAX
} ai_adc_mode;

/* Audio ADC port attribute */
typedef struct {
    ai_rx_type rx_type;
    ai_adc_mode mode;
} ai_adc_attr;

/* Audio PDM port attribute */
typedef struct {
    ai_rx_type rx_type;
    audio_i2s_attr i2s_attr; /* I2S Attribute */
} ai_pdm_attr;

/* Audio I2S port attribute */
typedef struct {
    audio_i2s_attr i2s_attr; /* I2S Attribute */
} ai_i2s_attr;

/* High Precision Gain, ranging from -81dB to 18dB, step 0.125dB */
typedef struct {
    td_s32 integer; /* Interger part of high preicision gain */
    td_s32 decimal; /* decimal part of high preicision gain, if -0.125dB, value is -125 */
} ai_gain;

/**
 * @if Eng
 * @brief  microphone calibration volume
 * @else
 * @brief  麦克风的校准音量
 * @endif
 */
typedef struct {
    td_u32 mic_num;  /*!< @if Eng microphone num used by audio input.
                          @else   使用的麦克风数量 @endif */
    ai_gain volume[AI_MIC_NUM_MAX]; /*!< @if Eng microphone calibration volume
                                         @else   每个麦克风的音量校准值 @endif */
} ai_cali_volume;

typedef struct {
    td_bool enable;
    ao_port_id port;
} ai_ref_attr;

typedef union {
    ai_adc_attr adc;
    ai_pdm_attr pdm;
    ai_i2s_attr i2s;
} ai_port_attr;

typedef struct {
    td_bool enable;
    audio_vad_attr attr;
} ai_vad_attr;

/* Audio port attribute */
typedef struct {
    audio_pcm_format pcm_attr;
    ai_ref_attr ref_attr;
    ai_port_attr port_attr;
    ai_vad_attr vad_attr;
} audio_ai_attr;

/* Define union of AI event param */
typedef union {
    td_u32 gain;
    audio_pcm_format pcm_attr;
} ai_event_param;

typedef struct {
    td_u32 dma_cnt;
    td_u32 bus_time_out_cnt; /* update at isr */

    /* rx only */
    td_u32 total_byte_read;
    td_u32 try_read_cnt;
    td_u32 buf_full_cnt;         /* update at rx isr */
    td_u32 buf_full_warning_cnt; /* update at rx isr */
    td_u32 bus_fifo_full_cnt;    /* update at rx isr */
    td_u32 inf_fifo_full_cnt;    /* update at rx isr */
} ai_port_proc_status;

typedef struct {
    ai_port_proc_status proc_status;
    audio_aiao_buf_info buf_info;
    audio_port_run_status run_status;
} ai_port_stauts;

typedef struct {
    td_bool enable;
    td_bool mute;
    ai_gain gain;
    ai_cali_volume cali_volume;
    td_u32 frame_index;
    audio_ai_port port;
    td_s32 chan_num;

    audio_ai_attr ai_attr;
    ai_port_stauts port_status;
} ai_port_proc_item;

typedef struct {
    td_bool enable;
    td_u32 frame_pms;
    audio_ring_buf rbuf[AI_RBUF_MAX_NUM];

    ai_port_proc_item port_proc;
} ai_proc_item;

#define ai_port_is_i2s_type(port)  ((port) <= AI_PORT_I2S4)
#define ai_port_is_pdm_type(port)  ((AI_PORT_PDM0 <= (port)) && ((port) <= AI_PORT_PDM3))
#define ai_port_is_adc_type(port)  ((AI_PORT_ADC0 <= (port)) && ((port) <= AI_PORT_ADC3))
#define ai_port_is_lpadc_type(port)  ((AI_PORT_LPADC0 <= (port)) && ((port) <= AI_PORT_LPADC1))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __AI_STRUCT_H__ */
