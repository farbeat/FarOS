/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: 算法公共头文件
 * Author: audio
 */

#ifndef __AUDIO_ALG_H__
#define __AUDIO_ALG_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 公共命令，各个算法都需要实现 */
typedef enum {
    ALG_COMMON_CMD_GET_API_SIZE = 0x0100,
    ALG_COMMON_CMD_API_INIT,

    ALG_COMMON_CMD_MAX,
} alg_common_cmd_id;

/* 算法音频帧 */
typedef struct {
    td_bool interleaved; /* whether the data is interleaved */
    td_u32 channels;     /* Number of Channels */
    td_u32 bit_depth;    /* Bit Depth */
    td_u32 sample_rate;  /* Sampling Rate */
    td_s32 *pcm_buffer;  /* pointer to the buffer for storing the pulse code modulation (PCM) data */
    td_u32 pcm_samples;  /* number of sampling points of the PCM data */
} audio_alg_frame;

/* 算法组件化接口 */
typedef struct {
    const td_char *name;
    td_u32 version;

    td_s32 (*create)(td_void *handle, const td_void *attr);
    td_s32 (*destroy)(td_void *handle);

    td_s32 (*set_config)(td_void *handle, td_u32 cmd, const td_void *cfg);
    td_s32 (*get_config)(td_void *handle, td_u32 cmd, td_void *cfg);

    td_s32 (*process)(td_void *handle, td_void *input, td_void *output);
    
    td_s32 (*reset)(td_void *handle);
} audio_alg_component;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_ALG_H__ */
