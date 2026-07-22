/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: user header file for audio decoders and encoders \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_ACODEC_H__
#define __SOC_UAPI_ACODEC_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup soc_uapi_acodec soc_uapi_acodec
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Audio codec ID definition.
 * @else
 * @brief  音频编解码器ID定义
 * @endif
 */
typedef enum {
    UAPI_ACODEC_ID_PCM,
    UAPI_ACODEC_ID_MP2,
    UAPI_ACODEC_ID_MP3,
    UAPI_ACODEC_ID_SBC,
    UAPI_ACODEC_ID_AAC,
    UAPI_ACODEC_ID_BLYRAYLPCM,
    UAPI_ACODEC_ID_COOK,
    UAPI_ACODEC_ID_DRA,
    UAPI_ACODEC_ID_WMA9STD,
    UAPI_ACODEC_ID_AMRNB,
    UAPI_ACODEC_ID_AMRWB,
    UAPI_ACODEC_ID_G711,
    UAPI_ACODEC_ID_G722,
    UAPI_ACODEC_ID_G726,
    UAPI_ACODEC_ID_ADPCM,
    UAPI_ACODEC_ID_VOICE,
    UAPI_ACODEC_ID_TRUEHD,
    UAPI_ACODEC_ID_AC3PASSTHROUGH,
    UAPI_ACODEC_ID_DTSPASSTHROUGH,
    UAPI_ACODEC_ID_OPUS,
    UAPI_ACODEC_ID_VORBIS,

    UAPI_ACODEC_ID_MSBC,
    UAPI_ACODEC_ID_FLAC,
    UAPI_ACODEC_ID_L2HC,
    UAPI_ACODEC_ID_LC3,
    UAPI_ACODEC_ID_SILK,
    UAPI_ACODEC_ID_APE,

    UAPI_ACODEC_ID_MAX,
} uapi_acodec_id;

/**
 * @if Eng
 * @brief  Definition of the distribution mode of the channels of a codec.
 * @else
 * @brief  定义codec声道分布模式
 * @endif
 */
typedef enum {
    UAPI_ACODEC_CHANNEL_NONE = 0x0, /*!< @if Eng Unused or empty
                                         @else   未使用或为空 @endif */
    UAPI_ACODEC_CHANNEL_LF = 0x1,   /*!< @if Eng Left front
                                         @else   左前 @endif */
    UAPI_ACODEC_CHANNEL_RF = 0x2,   /*!< @if Eng Right front
                                         @else   右前 @endif */
    UAPI_ACODEC_CHANNEL_CF = 0x3,   /*!< @if Eng Center front
                                         @else   中置 @endif */
    UAPI_ACODEC_CHANNEL_LS = 0x4,   /*!< @if Eng Left surround
                                         @else   左环绕 @endif */
    UAPI_ACODEC_CHANNEL_RS = 0x5,   /*!< @if Eng Right surround
                                         @else   右环绕 @endif */
    UAPI_ACODEC_CHANNEL_LFE = 0x6,  /*!< @if Eng Low frequency effects
                                         @else   低音 @endif */
    UAPI_ACODEC_CHANNEL_CS = 0x7,   /*!< @if Eng Back surround
                                         @else   后环绕 @endif */
    UAPI_ACODEC_CHANNEL_LR = 0x8,   /*!< @if Eng Left rear
                                         @else   左后 @endif */
    UAPI_ACODEC_CHANNEL_RR = 0x9,   /*!< @if Eng Right rear
                                         @else   右后 @endif */

    UAPI_ACODEC_CHANNEL_MAX,
} uapi_acodec_channel;

/**
 * @if Eng
 * @brief  Configuration parameters required by an codec for creating devices.
 * @else
 * @brief  解码器创建设备的配置参数
 * @endif
 */
typedef struct {
    td_u32 channels;          /*!< @if Eng Number of channels
                                   @else   声道数 @endif */
    td_u32 bit_depth;         /*!< @if Eng (PCM) Data depth, and format of storing the output data. \n
                                           If the data depth is 16 bits, 16-bit word memory is used. \n
                                           If the data depth is greater than 16 bits, 32-bit word memory is used, \n
                                           and data is stored as left-aligned data. (the valid data is at upper bits)
                                   @else   (PCM) 数据位宽设置. 输出存放格式 \n
                                           等于16bit: 占用16bit word内存 \n
                                           大于16bit: 占用32bit word内存, \n
                                           数据左对齐方式存放(有效数据在高位) @endif */
    td_u32 sample_rate;       /*!< @if Eng Sampling rate
                                   @else   采样率 @endif */
    td_void *private_data;    /*!< @if Eng Input, pointer to private data. \n
                                           If the decoder does not contain private data, this parameter is set to NULL.
                                   @else   IN 指向私有配置结构体的指针 \n
                                           如果解码器没有私有配置，设置为NULL @endif */
    td_u32 private_data_size; /*!< @if Eng Input, size of the private data. \n
                                           If the decoder does not contain private data, this parameter is set to 0.
                                   @else   IN 私有结构体大小 \n
                                           如果解码器没有私有配置，设置为0 @endif */
} uapi_acodec_dec_param;

/**
 * @if Eng
 * @brief  Parameters for creating an encoder
 * @else
 * @brief  编码器创建配置参数
 * @endif
 */
typedef struct {
    td_bool interleaved;      /*!< @if Eng IN Interleave or not
                                   @else   交织模式 @endif */
    td_u32 channels;          /*!< @if Eng Number of channels
                                   @else   声道数 @endif */
    td_u32 bit_depth;         /*!< @if Eng (PCM) Data depth, and format of storing the output data. \n
                                           If the data depth is 16 bits, 16-bit word memory is used. \n
                                           If the data depth is greater than 16 bits, 32-bit word memory is used, \n
                                           and data is stored as left-aligned data. (the valid data is at upper bits)
                                   @else   (PCM) 数据位宽设置. 输出存放格式 \n
                                           等于16bit: 占用16bit word内存 \n
                                           大于16bit: 占用32bit word内存, \n
                                           数据左对齐方式存放(有效数据在高位) @endif */
    td_u32 sample_rate;       /*!< @if Eng Sampling rate
                                   @else   采样率 @endif */
    td_u32 samples_per_frame; /*!< @if Eng IN Pcm samples per frame for encoder
                                   @else   每帧采样点 @endif */
    td_void *private_data;    /*!< @if Eng Pointer of decoder private open parameters \n
                                           note: set NULL if there is no private parameters
                                   @else   解码器私有参数 \n
                                           注意: 若没有私有参数，设置为NULL @endif */
    td_u32 private_data_size; /*!< @if Eng Size of decoder private open parameters \n
                                           note: set to 0 if there is no private parameters
                                   @else   解码器私有参数大小 \n
                                           注意: 若没有私有参数，设置为0 @endif */
} uapi_acodec_enc_param;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __SOC_UAPI_ACODEC_H__ */
