/**
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved. \n
 * Description: common UNF header file for audio \n
 * Author: @CompanyNameTag \n
 */

#ifndef __SOC_UAPI_AUDIO_H__
#define __SOC_UAPI_AUDIO_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define UAPI_AUDIO_DUMP_MASK_AI 0x1
#define UAPI_AUDIO_DUMP_MASK_SEA 0x2
#define UAPI_AUDIO_DUMP_MASK_AENC 0x4
#define UAPI_AUDIO_DUMP_MASK_ADEC 0x8
#define UAPI_AUDIO_DUMP_MASK_TRACK 0x10
#define UAPI_AUDIO_DUMP_MASK_AO 0x20
#define UAPI_AUDIO_DUMP_MASK_MAX (UAPI_AUDIO_DUMP_MASK_AO << 1)

#define UAPI_AUDIO_PROC_MASK_UART 0x1
#define UAPI_AUDIO_PROC_MASK_DIAG_SHELL 0x2
#define UAPI_AUDIO_PROC_MASK_MAX (UAPI_AUDIO_PROC_MASK_DIAG_SHELL << 1)

/**
 * @defgroup soc_uapi_audio soc_uapi_audio
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Defines the Sample Rate.
 * @else
 * @brief  定义音频采样率枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_SAMPLE_RATE_UNKNOWN = 0,    /*!< @if Eng Unknown sampling rate.
                                                @else   未知采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_8K   = 8000,    /*!< @if Eng 8 kHz sampling rate.
                                                @else   8K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_11K  = 11025,   /*!< @if Eng 11.025 kHz sampling rate.
                                                @else   11.025K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_12K  = 12000,   /*!< @if Eng 12 kHz sampling rate.
                                                @else   12K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_16K  = 16000,   /*!< @if Eng 16 kHz sampling rate.
                                                @else   16K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_22K  = 22050,   /*!< @if Eng 22.050 kHz sampling rate.
                                                @else   22.050K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_24K  = 24000,   /*!< @if Eng 24 kHz sampling rate.
                                                @else   24K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_32K  = 32000,   /*!< @if Eng 32 kHz sampling rate.
                                                @else   32K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_44K  = 44100,   /*!< @if Eng 44.1 kHz sampling rate.
                                                @else   44.1K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_48K  = 48000,   /*!< @if Eng 48 kHz sampling rate.
                                                @else   48K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_88K  = 88200,   /*!< @if Eng 88.2 kHz sampling rate.
                                                @else   88.2K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_96K  = 96000,   /*!< @if Eng 96 kHz sampling rate.
                                                @else   96K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_176K = 176400,  /*!< @if Eng 176 kHz sampling rate.
                                                @else   176K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_192K = 192000,  /*!< @if Eng 192 kHz sampling rate.
                                                @else   192K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_288K = 288000,  /*!< @if Eng 288 kHz sampling rate.
                                                @else   288K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_384K = 384000,  /*!< @if Eng 384 kHz sampling rate.
                                                @else   384K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_768K = 768000,  /*!< @if Eng 768 kHz sampling rate.
                                                @else   768K采样频率 @endif */
    UAPI_AUDIO_SAMPLE_RATE_MAX,
} uapi_audio_sample_rate;

/**
 * @if Eng
 * @brief  Defines the bit depth during audio sampling.
 * @else
 * @brief  定义音频采样位宽枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_BIT_DEPTH_UNKNOWN = 0, /*!< @if Eng Unknown bit depth.
                                           @else   未知采样位宽 @endif */
    UAPI_AUDIO_BIT_DEPTH_8  = 8,      /*!< @if Eng 8-bit depth.
                                           @else   8位采样位宽 @endif */
    UAPI_AUDIO_BIT_DEPTH_16 = 16,     /*!< @if Eng 16-bit depth.
                                           @else   16位采样位宽 @endif */
    UAPI_AUDIO_BIT_DEPTH_24 = 24,     /*!< @if Eng 24-bit depth.
                                           @else   24位采样位宽 @endif */
    UAPI_AUDIO_BIT_DEPTH_32 = 32,     /*!< @if Eng 32-bit depth.
                                           @else   32位采样位宽 @endif */

    UAPI_AUDIO_BIT_DEPTH_MAX,
} uapi_audio_bit_depth;

/**
 * @if Eng
 * @brief  Defines the I2S(Inter-IC Sound) mode.
 * @else
 * @brief  定义音频I2S接口模式枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_I2S_STD_MODE = 0, /*!< @if Eng I2S standard mode.
                                      @else   I2S标准模式 @endif */
    UAPI_AUDIO_I2S_PCM_MODE,     /*!< @if Eng PCM mode.
                                      @else   PCM模式 @endif */
    UAPI_AUDIO_I2S_TDM_MODE,     /*!< @if Eng TDM mode.
                                      @else   TDM分时复用模式 @endif */
    UAPI_AUDIO_I2S_PDM_MODE,     /*!< @if Eng PDM mode.
                                      @else   PDM模式 @endif */

    UAPI_AUDIO_I2S_MODE_MAX
} uapi_audio_i2s_mode;

/**
 * @if Eng
 * @brief  Defines the I2S(Inter-IC Sound) master clock.
 * @else
 * @brief  定义音频I2S接口主时钟枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_I2S_MCLK_128_FS = 0, /*!< @if Eng Mclk 128*fs.
                                         @else   mclk配置为fs的128倍 @endif */
    UAPI_AUDIO_I2S_MCLK_256_FS,     /*!< @if Eng Mclk 256*fs.
                                         @else   mclk配置为fs的256倍 @endif */
    UAPI_AUDIO_I2S_MCLK_384_FS,     /*!< @if Eng Mclk 384*fs.
                                         @else   mclk配置为fs的384倍 @endif */
    UAPI_AUDIO_I2S_MCLK_512_FS,     /*!< @if Eng Mclk 512*fs.
                                         @else   mclk配置为fs的512倍 @endif */
    UAPI_AUDIO_I2S_MCLK_768_FS,     /*!< @if Eng Mclk 768*fs.
                                         @else   mclk配置为fs的768倍 @endif */
    UAPI_AUDIO_I2S_MCLK_1024_FS,    /*!< @if Eng Mclk 1024*fs.
                                         @else   mclk配置为fs的1024倍 @endif */
    UAPI_AUDIO_I2S_MCLK_1536_FS,    /*!< @if Eng Mclk 1536*fs.
                                         @else   mclk配置为fs的1536倍 @endif */

    UAPI_AUDIO_I2S_MCLK_MAX
} uapi_audio_i2s_mclk_sel;

/**
 * @if Eng
 * @brief  Defines the I2S(Inter-IC Sound) bclk clock.
 * @else
 * @brief  定义音频I2S接口bclk枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_I2S_BCLK_1_DIV  = 1,  /*!< @if Eng Bclk,mclk/1.
                                          @else   bclk配置为mclk的1分频 @endif */
    UAPI_AUDIO_I2S_BCLK_2_DIV  = 2,  /*!< @if Eng Bclk,mclk/2.
                                          @else   bclk配置为mclk的2分频 @endif */
    UAPI_AUDIO_I2S_BCLK_3_DIV  = 3,  /*!< @if Eng Bclk,mclk/3.
                                          @else   bclk配置为mclk的3分频 @endif */
    UAPI_AUDIO_I2S_BCLK_4_DIV  = 4,  /*!< @if Eng Bclk,mclk/4.
                                          @else   bclk配置为mclk的4分频 @endif */
    UAPI_AUDIO_I2S_BCLK_6_DIV  = 6,  /*!< @if Eng Bclk,mclk/6.
                                          @else   bclk配置为mclk的6分频 @endif */
    UAPI_AUDIO_I2S_BCLK_8_DIV  = 8,  /*!< @if Eng Bclk,mclk/8.
                                          @else   bclk配置为mclk的8分频 @endif */
    UAPI_AUDIO_I2S_BCLK_12_DIV = 12, /*!< @if Eng Bclk,mclk/12.
                                          @else   bclk配置为mclk的12分频 @endif */
    UAPI_AUDIO_I2S_BCLK_24_DIV = 24, /*!< @if Eng Bclk,mclk/24.
                                          @else   bclk配置为mclk的24分频 @endif */
    UAPI_AUDIO_I2S_BCLK_32_DIV = 32, /*!< @if Eng Bclk,mclk/32.
                                          @else   bclk配置为mclk的32分频 @endif */
    UAPI_AUDIO_I2S_BCLK_48_DIV = 48, /*!< @if Eng Bclk,mclk/48.
                                          @else   bclk配置为mclk的48分频 @endif */
    UAPI_AUDIO_I2S_BCLK_64_DIV = 64, /*!< @if Eng Bclk,mclk/64.
                                          @else   bclk配置为mclk的64分频 @endif */
    UAPI_AUDIO_I2S_BCLK_96_DIV = 96, /*!< @if Eng Bclk,mclk/96.
                                          @else   bclk配置为mclk的96分频 @endif */

    UAPI_AUDIO_I2S_BCLK_MAX
} uapi_audio_i2s_bclk_sel;

/**
 * @if Eng
 * @brief  Audio channels.
 * @else
 * @brief  音频声道枚举
 * @endif
 */
typedef enum {
    UAPI_AUDIO_CHANNEL_0  = 0,
    UAPI_AUDIO_CHANNEL_1  = 1,
    UAPI_AUDIO_CHANNEL_2  = 2,
    UAPI_AUDIO_CHANNEL_4  = 4,
    UAPI_AUDIO_CHANNEL_6  = 6,
    UAPI_AUDIO_CHANNEL_8  = 8,
    UAPI_AUDIO_CHANNEL_12 = 12,
    UAPI_AUDIO_CHANNEL_16 = 16,

    UAPI_AUDIO_CHANNEL_MAX,
} uapi_audio_channel;

/**
 * @if Eng
 * @brief  Defines the I2S(Inter-IC Sound) data valid after frame sync clock at PCM mode.
 * @else
 * @brief  定义音频I2S接口，数据有效延迟周期，仅PCM模式有效
 * @endif
 */
typedef enum {
    UAPI_AUDIO_I2S_PCM_0_DELAY  = 0,  /*!< @if Eng 0 bclk cycles delay.
                                           @else   0 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_1_DELAY  = 1,  /*!< @if Eng 1 bclk cycles delay.
                                           @else   1 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_8_DELAY  = 8,  /*!< @if Eng 8 bclk cycles delay.
                                           @else   8 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_16_DELAY = 16, /*!< @if Eng 16 bclk cycles delay.
                                           @else   16 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_17_DELAY = 17, /*!< @if Eng 17 bclk cycles delay.
                                           @else   17 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_24_DELAY = 24, /*!< @if Eng 24 bclk cycles delay.
                                           @else   24 bclk 周期延时 @endif */
    UAPI_AUDIO_I2S_PCM_32_DELAY = 32, /*!< @if Eng 32 bclk cycles delay.
                                           @else   32 bclk 周期延时 @endif */

    UAPI_AUDIO_I2S_PCM_DELAY_MAX
} uapi_audio_i2s_pcm_delay;

/**
 * @if Eng
 * @brief  Defines the I2S(inter-IC sound) attribute.
 * @else
 * @brief  定义音频I2S接口属性
 * @endif
 */
typedef struct {
    td_bool master;               /*!< @if Eng True: master, false: slave.
                                       @else   真：作为主设备，假：作为从设备 */
    uapi_audio_i2s_mode i2s_mode;
    uapi_audio_i2s_mclk_sel mclk;
    uapi_audio_i2s_bclk_sel bclk;
    uapi_audio_bit_depth bit_depth;
    uapi_audio_channel channels;
    td_bool pcm_sample_rise_edge; /*!< @if Eng Only works for PCM mode. \n
                                               true: sample data at positive edge of bclk, \n
                                               false: sample data at negative edge of bclk \n
                                       @else   仅适用于PCM模式 \n
                                               真: 在bclk的上升沿采样数据， \n
                                               假: 在bclk的下降沿采样数据 @endif */
    uapi_audio_i2s_pcm_delay pcm_delay_cycle;
} uapi_audio_i2s_attr;

/**
 * @if Eng
 * @brief  Defines the information about audio frames.
 * @else
 * @brief  定义有关音频帧信息
 * @endif
 */
typedef struct {
    td_bool interleaved; /*!< @if Eng Whether the data is interleaved.
                              @else   数据是否交织 @endif */
    td_u32 channels;     /*!< @if Eng Number of channels.
                              @else   声道数 @endif */
    td_u32 bit_depth;    /*!< @if Eng bit_depth: (PCM) data depth, and format of storing the output data \n
                                      if the data depth is 16 bits, 16-bit word memory is used. \n
                                      if the data depth is greater than 16 bits, 32-bit word memory is used, \n
                                      and data is stored as left-aligned data. \n
                                      that is, the valid data is at upper bits. \n
                              @else   bit_depth: (PCM) 数据位宽设置. 输出存放格式 \n
                                      等于16bit: 占用16bit word内存 \n
                                      大于16bit: 占用32bit word内存, 数据左对齐方式存放(有效数据在高位) @endif */
    td_u32 sample_rate;  /*!< @if Eng Sampling rate.
                              @else   采样率 @endif */
    td_s64 pts;          /*!< @if Eng Presentation time stamp (PTS, unit: us).
                              @else   时间戳(单位：微秒) @endif */
    td_s32 *pcm_buffer;  /*!< @if Eng Pointer to the buffer for storing the pulse code modulation (PCM) data
                              @else   PCM数据缓冲指针 @endif */
    td_s32 *bits_buffer; /*!< @if Eng Pointer to the buffer for storing the stream data.
                              @else   透传数据缓冲指针 @endif */
    td_u32 bits_bytes;   /*!< @if Eng data size.
                              @else   数据长度 @endif */
    td_u32 pcm_samples;  /*!< @if Eng Number of sampling points of the PCM data.
                              @else   PCM数据采样点数量 @endif */
    td_u32 frame_index;  /*!< @if Eng Frame ID.
                              @else   帧序号 @endif */
    td_bool eos;         /*!< @if Eng End of stream.
                              @else   流结束标志 @endif */
    td_bool pkg_loss;    /*!< @if Eng The packet loss.
                              @else   是否丢包 @endif */
} uapi_audio_frame;

/**
 * @if Eng
 * @brief  Define the Stream Buffer.
 * @else
 * @brief  定义流缓冲区
 * @endif
 */
typedef struct {
    td_u8 *data;      /*!< @if Eng Data Buffer.
                           @else   数据缓冲区 @endif */
    td_u32 size;      /*!< @if Eng Data Size.
                           @else   数据大小 @endif */
    td_s64 pts;       /*!< @if Eng Presentation time stamp (PTS, unit: us).
                           @else   时间戳（单位：微秒） @endif */
    td_bool eos;      /*!< @if Eng End of stream.
                           @else   流结束标志 @endif */
    td_bool pkg_loss; /*!< @if Eng The packet loss.
                           @else   是否丢包 @endif */
} uapi_stream_buf;

/**
 * @if Eng
 * @brief  Define the audio format, refer to EIA-CEA-861-G Table 31.
 * @else
 * @brief  定义音频编码格式，来源于EIA-CEA-861-G 表31
 * @endif
 */
typedef enum {
    UAPI_AUDIO_FORMAT_STREAM = 0, /*!< @if Eng Audio coding type, refer to stream header, default type.
                                       @else   音频编码类型，参考数据流，缺省格式 @endif */
    UAPI_AUDIO_FORMAT_PCM,        /*!< @if Eng Audio coding PCM type.
                                       @else   音频编码PCM格式 @endif */
    UAPI_AUDIO_FORMAT_AC3,        /*!< @if Eng Audio coding dolby digital(ac3)type.
                                       @else   音频编码dolby digital(ac3)格式 @endif */
    UAPI_AUDIO_FORMAT_MPEG1,      /*!< @if Eng Audio coding MPEG1 type.
                                       @else   音频编码MPEG1格式 @endif */
    UAPI_AUDIO_FORMAT_MP3,        /*!< @if Eng Audio coding MP3 type.
                                       @else   音频编码MP3格式 @endif */
    UAPI_AUDIO_FORMAT_MPEG2,      /*!< @if Eng Audio coding MPEG2 type.
                                       @else   音频编码MPEG2格式 @endif */
    UAPI_AUDIO_FORMAT_AAC_LC,     /*!< @if Eng Audio coding AAC type.
                                       @else   音频编码AAC格式 @endif */
    UAPI_AUDIO_FORMAT_DTS,        /*!< @if Eng Audio coding DTS type.
                                       @else   音频编码DTS格式 @endif */
    UAPI_AUDIO_FORMAT_ATRAC,      /*!< @if Eng Audio coding ATRAC type.
                                       @else   音频编码ATRAC格式 @endif */
    UAPI_AUDIO_FORMAT_DSD,        /*!< @if Eng Audio coding One Bit Audio type.
                                       @else   音频编码One Bit Audio格式 @endif */
    UAPI_AUDIO_FORMAT_EAC3,       /*!< @if Eng Audio coding dolby digital plus type.
                                       @else   音频编码dolby digital plus(e-ac3)格式 @endif */
    UAPI_AUDIO_FORMAT_DTS_HD,     /*!< @if Eng Audio coding DTS HD type.
                                       @else   音频编码DTS HD格式 @endif */
    UAPI_AUDIO_FORMAT_MAT,        /*!< @if Eng Audio coding MAT(DVD Forum MLP)type.
                                       @else   音频编码MAT(DVD Forum MLP)格式 @endif */
    UAPI_AUDIO_FORMAT_DST,        /*!< @if Eng Audio coding DST type.
                                       @else   音频编码DST格式 @endif */
    UAPI_AUDIO_FORMAT_WMA_PRO,    /*!< @if Eng Audio coding WMA PRO type.
                                       @else   音频编码WMA PRO格式 @endif */
    UAPI_AUDIO_FORMAT_CXT,        /*!< @if Eng Audio coding refer to CXT.
                                       @else   音频编码参考CXT @endif */
} uapi_audio_format;

/**
 * @if Eng
 * @brief  Define the audio format, refer to EIA-CEA-861-G Table 33.
 * @else
 * @brief  定义音频编码格式，来源于EIA-CEA-861-G 表33
 * @endif
 */
typedef enum {
    UAPI_AUDIO_FORMAT_EXT_CT = 0x0,                    /*!< @if Eng Refer to Audio Coding Type (CT) field in \n
                                                                    Data Byte 1. \n
                                                            @else   参考数据字节 1 中的音频编码类型 (CT) 字段 @endif */
    UAPI_AUDIO_FORMAT_EXT_HE_AAC,                      /*!< @if Eng The next three CXT values are defined in \n
                                                                    CEA-861-E only. \n
                                                                    They do not exist in older versions, \n
                                                                    and in CEA-861-F and CEA-861-F \n
                                                                    they are defined as 'Not in use'. \n
                                                            @else   接下来的三个 CXT 值仅在 CEA-861-E 中定义 \n
                                                                    它们在旧版本以及 CEA-861-F 和 CEA-861-F 中不存在 \n
                                                                    它们被定义为“未使用”。 @endif */
    UAPI_AUDIO_FORMAT_EXT_HE_AAC_V2,
    UAPI_AUDIO_FORMAT_EXT_MPEG_SURROUND,
    UAPI_AUDIO_FORMAT_EXT_MPEG4_HE_AAC = 0x4,          /*!< @if Eng The following CXT values are only defined in \n
                                                                    CEA-861-F.\n
                                                            @else   以下 CXT 值仅在 CEA-861-F 中定义 @endif */
    UAPI_AUDIO_FORMAT_EXT_MPEG4_HE_AAC_V2,
    UAPI_AUDIO_FORMAT_EXT_MPEG4_AAC_LC,
    UAPI_AUDIO_FORMAT_EXT_DRA,
    UAPI_AUDIO_FORMAT_EXT_MPEG4_HE_AAC_SURROUND,       /*!< @if Eng MPEG-4 HE AAC + MPEG Surround.
                                                            @else   MPEG-4 HE AAC + MPEG 环绕声 @endif */
    UAPI_AUDIO_FORMAT_EXT_MPEG4_AAC_LC_SURROUND = 0xa, /*!< @if Eng MPEG-4 AAC LC + MPEG Surround.
                                                            @else   MPEG-4 AAC LC + MPEG 环绕声 @endif */
    UAPI_AUDIO_FORMAT_EXT_MPEGH_3D,                    /*!< @if Eng The following CXT values are only defined in \n
                                                                    CEA-861-G. \n
                                                                    Audio Extension Type Code 11 (MPEG-H 3D Audio). \n
                                                            @else   以下 CXT 值仅在 CEA-861-G 中定义 \n
                                                                    音频扩展类型代码 11（MPEG-H 3D 音频） @endif */
    UAPI_AUDIO_FORMAT_EXT_AC4,                         /*!< @if Eng Audio Extension Type Code 12 (AC-4).
                                                            @else   音频扩展类型代码 12(AC-4)CNend @endif */
    UAPI_AUDIO_FORMAT_EXT_LPCM_3D,                     /*!< @if Eng Audio Extension Type Code 13 (L-PCM 3D Audio).
                                                            @else   音频扩展类型代码 13（L-PCM 3D 音频）@endif */
} uapi_audio_format_ext;

/**
 * @if Eng
 * @brief  Defines pcm format.
 * @else
 * @brief  定义pcm格式
 * @endif
 */
typedef struct {
    td_u32 channels;
    td_u32 bit_depth;
    td_u32 sample_rate;
    td_u32 sample_per_frame; /*!< @if Eng Samples per frame.
                                  @else   每帧采样点数 @endif */
} uapi_audio_pcm_format;

typedef struct {
    td_u8 dump_mask;
    td_u8 proc_mask;
} uapi_audio_debug_cfg;

/**
 * @if Eng
 * @brief  audio product form
 * @else
 * @brief  音频产品形态
 * @endif
 */
typedef enum {
    UAPI_AUDIO_PRODUCT_FORM_TWS_LEFT,   /*!< @if Eng product form is only left channel.
                                             @else   产品形态为真无线耳机左耳 @endif */
    UAPI_AUDIO_PRODUCT_FORM_TWS_RIGHT,  /*!< @if Eng product form is only right channel.
                                             @else   产品形态为真无线耳机右耳 @endif */
    UAPI_AUDIO_PRODUCT_FORM_NON_TWS,    /*!< @if Eng product form is not tws.
                                             @else   产品形态不是真无线耳机 @endif */

    UAPI_AUDIO_PRODUCT_FORM_MAX,
} uapi_audio_product_form;

/**
 * @if Eng
 * @brief  audio TWS mode
 * @else
 * @brief  真无线耳机模式
 * @endif
 */
typedef enum {
    UAPI_AUDIO_TWS_MODE_NON_TWS,  /*!< @if Eng single-ear mode.
                                       @else   单耳模式 @endif */

    UAPI_AUDIO_TWS_MODE_FORWARD,  /*!< @if Eng TWS mode is forward.
                                       @else   转发模式 @endif */

    UAPI_AUDIO_TWS_MODE_LISTEN,   /*!< @if Eng TWS mode is listen.
                                       @else   协同模式 @endif */

    UAPI_AUDIO_TWS_MODE_DUAL_TRANSMIT, /*!< @if Eng TWS mode is dual transmit.
                                            @else   双发模式 @endif */

    UAPI_AUDIO_TWS_MODE_MAX,
} uapi_audio_tws_mode;

/**
 * @if Eng
 * @brief  audio TWS role
 * @else
 * @brief  真无线耳机角色
 * @endif
 */
typedef enum {
    UAPI_AUDIO_TWS_ROLE_MASTER,   /*!< @if Eng TWS role is master.
                                       @else 真无线耳机主耳 @endif */

    UAPI_AUDIO_TWS_ROLE_SLAVE,    /*!< @if Eng TWS role is slave.
                                       @else 真无线耳机副耳 @endif */

    UAPI_AUDIO_TWS_ROLE_FREEMAN,  /*!< @if Eng TWS role is freeman.
                                       @else 真无线耳机自由播放 @endif */

    UAPI_AUDIO_TWS_ROLE_MAX,
} uapi_audio_tws_role;

/**
 * @if Eng
 * @brief  Audio toggle config.
 * @else
 * @brief  音频toggle配置
 * @endif
 */
typedef struct {
    td_bool toggle_valid; /*!< @if Eng false, the config will take effect immediately;
                                       true,  the config will take effect in the toggle_count interrupt;
                               @else   false, 配置将立即生效；
                                       true,  配置将在指定的toggle计数器toggle_count处生效 @endif */

    td_u32 toggle_count; /*!< @if Eng Used when toggle_valid is true,
                                      the config will take effect in the toggle_count interrupt.
                              @else   toggle_valid为真时有效，配置将在指定的toggle计数器toggle_count处生效 @endif */
} uapi_audio_toggle_config;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_AUDIO_H__ */
