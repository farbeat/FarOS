/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for hearing aid
 * Author: audio
 */

#ifndef __HAID_STRUCT_H__
#define __HAID_STRUCT_H__

#include "td_type.h"
#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @if Eng
 * @brief  maximum microphone num used by hearing aid.
 * @else
 * @brief  助听使用的麦克风数量最大值
 * @endif
 */
#define HAID_MIC_NUM_MAX 2

#define HAID_CONFIG_SIZE_MAX 530

#define HAID_VOL_MAX 60     /* max 60 dB */
#define HAID_VOL_MIN (-120) /* min -120 dB */
#define HAID_VOL_STEP_MIN 1 /* 1dB */
#define HAID_VOL_PRECI_COEF 1000     /* 0.125dB is 125 */

#define HAID_CALIBRATION_VOL_MAX 12    /* max 12dB */
#define HAID_CALIBRATION_VOL_MIN (-12) /* min -12dB */

typedef enum {
    HAID_EXIT,
    HAID_RUNNING,
    SINUS_TONE_RUNNING,
} haid_state;

typedef enum {
    HAID_EVENT_SCENE_CHANGE,
    HAID_EVENT_TYPE_MAX,
} haid_event_type;

typedef union {
    td_u32 value;
} haid_event_param;

typedef enum {
    HAID_SCENE_AUTO = 0,
    HAID_SCENE_TRANSPORTATION,
    HAID_SCENE_SOCIALIZING,
    HAID_SCENE_PARTY,
    HAID_SCENE_QUIET,

    HAID_SCENE_MAX,
} haid_scene;

typedef struct {
    audio_ai_port ai_port[HAID_MIC_NUM_MAX];
    ao_port_id ao_port;

    audio_pcm_format in_pcm;
    audio_pcm_format out_pcm;
} haid_attr;

/* High Precision Gain, ranging from -81dB to 18dB, step 0.125dB */
typedef struct {
    td_s32 integer; /* Interger part of high preicision gain */
    td_s32 decimal; /* decimal part of high preicision gain, if -0.125dB, value is -125 */
} haid_volume;

/**
 * @if Eng
 * @brief  hearing aid microphone calibration volume
 * @else
 * @brief  助听麦克风的校准音量
 * @endif
 */
typedef struct {
    td_u32 mic_num;  /*!< @if Eng microphone num used by hearing aid.
                          @else   助听使用的麦克风数量 @endif */
    haid_volume volume[HAID_MIC_NUM_MAX]; /*!< @if Eng microphone calibration volume
                                               @else   每个麦克风的音量校准值 @endif */
} haid_cali_volume;

typedef struct {
    td_handle h_mod;
    td_s8 config[HAID_CONFIG_SIZE_MAX];
    td_u32 config_len;
} haid_config;

typedef struct {
    td_handle h_mod;
    td_u32 all_config_addr;
    td_u32 all_config_len;
} haid_all_config;

typedef struct {
    haid_state state;
    haid_volume volume;
    haid_cali_volume cali_volume;
    haid_scene scene;
    audio_pcm_format in_format;
    audio_pcm_format out_format;
} haid_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HAID_STRUCT_H__ */
