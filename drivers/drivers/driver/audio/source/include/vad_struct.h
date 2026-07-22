/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __VAD_STRUCT_H__
#define __VAD_STRUCT_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    VAD_EVENT_VAD_VALID = 0, /* user api event */
    VAD_EVENT_VAD_TIMEOUT,   /* user api event */

    VAD_EVENT_MAX,
} vad_event_type;

typedef enum {
    VAD_DEV_AVAD = 0,
    VAD_DEV_MAD,

    VAD_DEV_TYPE_MAX
} vad_dev_type;

/* Defines the VAD work mode */
typedef enum {
    VAD_NORM_MODE = 0,
    VAD_LP_MODE,  /* Low power mode */
    VAD_HP_MODE,  /* High performance mode */

    VAD_MODE_MAX
} vad_work_mode;

/* Defines the Audio digital VAD attribute */
typedef struct {
    td_bool always_on;
    td_u32 timeout;
    vad_dev_type vad_type;
    vad_work_mode vad_mode;
} audio_vad_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __VAD_STRUCT_H__ */
