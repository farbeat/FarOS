/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio decoders and encoders
 * Author: audio
 */

#ifndef __HA_CODEC_H__
#define __HA_CODEC_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Define audio codec common command ID */
/* CNcomment:定义音频 codec 公共CMD ID */
#define HA_CODEC_COMMON_CMD_ID 0xffff

/* Define codec format query command ID */
/* CNcomment:定义 codec 格式查询CMD ID */
#define HA_CODEC_FORMAT_QUERY_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1000)

/* Define  codec decoder open parameter command ID */
/* CNcomment:定义 codec 解码器打开参数CMD ID */
#define HA_CODEC_GET_DEC_PARAM_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1001)

/* Define  codec encoder open parameter command ID */
/* CNcomment:定义 codec 编码器打开参数CMD ID */
#define HA_CODEC_GET_ENC_PARAM_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1002)

/* Define  codec packet query command ID */
/* CNcomment:定义 codec packet解码模式CMD ID */
#define ACODEC_PACKET_MODE_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1003)

/* codec hw decode query command ID */
/* CNcomment:codec hw decode解码模式CMD ID */
#define HA_CODEC_HW_CODEC_QUERY_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1004)

/* Define  codec set outbuf addr command ID  only for hardware codec */
/* CNcomment:定义输出Buffer地址设置CMD ID */
#define HA_CODEC_OUT_BUF_ADDR_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1005)

/* Define dual decode capability support query command ID */
/* CNcomment:定义双路解码能力查询CMD ID */
#define HA_CODEC_DUAL_DECODE_QUERY_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1006)

/* Define dual decode set ad output mode command ID */
/* CNcomment:定义双路解码AD输出模式设置CMD ID */
#define HA_CODEC_AD_OUTPUT_MODE_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1007)

/* Define AD Balance Setting command ID */
/* CNcomment:定义AD 权重设置CMD ID */
#define HA_CODEC_AD_BALANCE_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1008)

/* Define PTS Delay command ID */
/* CNcomment:定义PTS Delay CMD ID */
#define HA_CODEC_PTS_DELAY_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1009)

/* Define  codec set whether work in compress mode command ID */
/* CNcomment:定义是否工作于在线压缩模式设置CMD ID */
#define HA_CODEC_COMPRESS_MODE_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100A)

/* Define  codec get codec output delay ms */
/* CNcomment:定义获取解码器输出延时CMD ID */
#define HA_CODEC_OUTPUT_DELAY_GET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100B)

/* Define  codec get codec output */
/* CNcomment:定义获取编解码器参数 */
#define ACODEC_PRIVATE_PARAM_GET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100C)

#define ACODEC_IN_FRAME_SIZE_GET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100D)
#define ACODEC_OUT_FRAME_SIZE_GET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100E)
#define ACODEC_SCRATCH_BUF_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x100F)
#define ACODEC_SET_PLC_OFF_THR_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1010)
#define ACODEC_GET_PLC_OFF_THR_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1011)
#define ACODEC_SET_HEADER_SYNC_MODE_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1100)
#define ACODEC_IN_FRAME_MODE_SET_CMD ((((td_u32)HA_CODEC_COMMON_CMD_ID) << 16) | 0x1101)

/* Define audio codec maximum channel */
/* CNcomment:定义音频 codec 最大通道 */
#define HA_CODEC_MAX_CHANNEL 8

/* audio format definition */
/* CNcomment:audio format 定义 */
typedef enum {
    /* MPEG audio layer 1, 2. */
    /* CNcomment:MPEG音频第一层、第二层 */
    HA_CODEC_FORMAT_MP2 = 0x000,

    /* MPEG audio layer 1, 2, 3. */
    /* CNcomment:MPEG音频第一层、第二层 、第三层 */
    HA_CODEC_FORMAT_MP3,

    HA_CODEC_FORMAT_AAC,
    HA_CODEC_FORMAT_SBC,
    HA_CODEC_FORMAT_AC3,
    HA_CODEC_FORMAT_DTS,
    HA_CODEC_FORMAT_VORBIS,
    HA_CODEC_FORMAT_DVAUDIO,
    HA_CODEC_FORMAT_WMAV1,
    HA_CODEC_FORMAT_WMAV2,
    HA_CODEC_FORMAT_MACE3,
    HA_CODEC_FORMAT_MACE6,
    HA_CODEC_FORMAT_VMDAUDIO,
    HA_CODEC_FORMAT_SONIC,
    HA_CODEC_FORMAT_SONIC_LS,
    HA_CODEC_FORMAT_FLAC,
    HA_CODEC_FORMAT_MP3ADU,
    HA_CODEC_FORMAT_MP3ON4,
    HA_CODEC_FORMAT_SHORTEN,
    HA_CODEC_FORMAT_ALAC,
    HA_CODEC_FORMAT_WESTWOOD_SND1,
    HA_CODEC_FORMAT_GSM,
    HA_CODEC_FORMAT_QDM2,
    HA_CODEC_FORMAT_COOK,
    HA_CODEC_FORMAT_TRUESPEECH,
    HA_CODEC_FORMAT_TTA,
    HA_CODEC_FORMAT_SMACKAUDIO,
    HA_CODEC_FORMAT_QCELP,
    HA_CODEC_FORMAT_WAVPACK,
    HA_CODEC_FORMAT_DSICINAUDIO,
    HA_CODEC_FORMAT_IMC,
    HA_CODEC_FORMAT_MUSEPACK7,
    HA_CODEC_FORMAT_MLP,

    /* as found in WAV. */
    /* CNcomment:存在WAV格式中 */
    HA_CODEC_FORMAT_GSM_MS,

    HA_CODEC_FORMAT_ATRAC3,
    HA_CODEC_FORMAT_VOXWARE,
    HA_CODEC_FORMAT_APE,
    HA_CODEC_FORMAT_NELLYMOSER,
    HA_CODEC_FORMAT_MUSEPACK8,
    HA_CODEC_FORMAT_SPEEX,
    HA_CODEC_FORMAT_WMAVOICE,
    HA_CODEC_FORMAT_WMAPRO,
    HA_CODEC_FORMAT_WMALOSSLESS,
    HA_CODEC_FORMAT_ATRAC3P,
    HA_CODEC_FORMAT_EAC3,
    HA_CODEC_FORMAT_SIPR,
    HA_CODEC_FORMAT_MP1,
    HA_CODEC_FORMAT_TWINVQ,
    HA_CODEC_FORMAT_TRUEHD,
    HA_CODEC_FORMAT_MP4ALS,
    HA_CODEC_FORMAT_ATRAC1,
    HA_CODEC_FORMAT_BINKAUDIO_RDFT,
    HA_CODEC_FORMAT_BINKAUDIO_DCT,
    HA_CODEC_FORMAT_DRA,
    HA_CODEC_FORMAT_MAT,
    HA_CODEC_FORMAT_AC4,

    HA_CODEC_FORMAT_MSBC,
    HA_CODEC_FORMAT_L2HC,
    HA_CODEC_FORMAT_LC3,
    HA_CODEC_FORMAT_SILK,

    /* various PCM codecs. */
    /* CNcomment:PCM格式 */
    HA_CODEC_FORMAT_PCM = 0x100,
    /* various PCM codecs. */
    /* CNcomment:WFDPCM格式 */
    HA_CODEC_FORMAT_WFDPCM,
    HA_CODEC_FORMAT_PCM_BLURAY = 0x121,

    /* various ADPCM codecs. */
    /* CNcomment:ADPCM格式 */
    HA_CODEC_FORMAT_ADPCM = 0x130,

    /* various AMR codecs. */
    /* CNcomment:AMR格式 */
    HA_CODEC_FORMAT_AMR_NB = 0x160,
    HA_CODEC_FORMAT_AMR_WB,
    HA_CODEC_FORMAT_AMR_AWB,

    /* RealAudio codecs. */
    /* CNcomment:RealAudio格式 */
    HA_CODEC_FORMAT_RA_144 = 0x170,
    HA_CODEC_FORMAT_RA_288,

    /* various DPCM codecs. */
    /* CNcomment:DPCM格式 */
    HA_CODEC_FORMAT_DPCM = 0x180,

    /* various G.7xx codecs. */
    /* CNcomment:G.7xx格式 */
    HA_CODEC_FORMAT_G711 = 0x190,
    HA_CODEC_FORMAT_G722,
    HA_CODEC_FORMAT_G7231,
    HA_CODEC_FORMAT_G726,
    HA_CODEC_FORMAT_G728,
    HA_CODEC_FORMAT_G729AB,

    HA_CODEC_FORMAT_OPUS = 0x1a0,

    /* support multi codecs. */
    /* CNcomment:多种格式 */
    HA_CODEC_FORMAT_MULTI = 0x1f0,

    HA_CODEC_FORMAT_MAX = 0x1ff,
} ha_codec_format;

/* audio codec ID definition */
/* CNcomment:音频编解码器ID定义 */
typedef enum {
    HA_CODEC_ID_PCM,
    HA_CODEC_ID_MP2,
    HA_CODEC_ID_MP3,
    HA_CODEC_ID_SBC,
    HA_CODEC_ID_AAC,
    HA_CODEC_ID_BLYRAYLPCM,
    HA_CODEC_ID_COOK,
    HA_CODEC_ID_DRA,
    HA_CODEC_ID_WMA9STD,
    HA_CODEC_ID_AMRNB,
    HA_CODEC_ID_AMRWB,
    HA_CODEC_ID_G711,
    HA_CODEC_ID_G722,
    HA_CODEC_ID_G726,
    HA_CODEC_ID_ADPCM,
    HA_CODEC_ID_VOICE,
    HA_CODEC_ID_TRUEHD,
    HA_CODEC_ID_AC3PASSTHROUGH,
    HA_CODEC_ID_DTSPASSTHROUGH,
    HA_CODEC_ID_OPUS,
    HA_CODEC_ID_VORBIS,

    HA_CODEC_ID_MSBC,
    HA_CODEC_ID_FLAC,
    HA_CODEC_ID_L2HC,
    HA_CODEC_ID_LC3,
    HA_CODEC_ID_SILK,
    HA_CODEC_ID_APE,

    HA_CODEC_ID_MAX,
} ha_codec_id;

/* *audio channel mode(acmode) */
/* *CNcomment:声道配置信息 */
typedef enum {
    HA_CODEC_CHANNEL_MODE_INVALID = -2,

    /* *object audio */
    HA_CODEC_CHANNEL_MODE_RAW = -1,

    /* *dual mono or one plus one */
    HA_CODEC_CHANNEL_MODE_DUAL_MONO = 0x0,

    /* *C */
    HA_CODEC_CHANNEL_MODE_MONO = 1,
    /* *L R */
    HA_CODEC_CHANNEL_MODE_STEREO = 2,
    HA_CODEC_CHANNEL_MODE_3_0 = 3,
    HA_CODEC_CHANNEL_MODE_2_1 = 4,
    HA_CODEC_CHANNEL_MODE_3_1 = 5,
    HA_CODEC_CHANNEL_MODE_2_2 = 6,

    /* *L R C(3) LFE(.1) LS RS(2) */
    HA_CODEC_CHANNEL_MODE_3_2 = 7,

    /* *only in mp3 format */
    /* *CNcomment:仅mp3格式才有joint stereo模式 */
    HA_CODEC_CHANNEL_MODE_JOINT_STEREO = 8,

    /* *L R C(3) LFE(.1) LS RS LRS RRS(4) */
    HA_CODEC_CHANNEL_MODE_3_4 = 21,

    /* *atmos: 3 front channels, 2 surround channels, LFE channel, 2 height channels. */
    HA_CODEC_CHANNEL_MODE_3_2_2 = 28,

    HA_CODEC_CHANNEL_MODE_MAX,
} ha_codec_channel_mode;

/* Audio Output AD Mode definition */
/* CNcomment:OUTPUT AD MODE 定义 */
typedef enum {
    HA_CODEC_OUTPUT_AD_OFF = 0,
    HA_CODEC_OUTPUT_AD_ON,
    HA_CODEC_OUTPUT_AD_OFF_WITHASSOC,
    HA_CODEC_OUTPUT_AD_MAX
} ha_codec_output_ad_mode;

/* Define query parameter of codec. */
/* CNcomment:定义 codec 查询参数 */
typedef struct {
    td_u32 cmd;
    td_void *param;
} ha_codec_parameter_query;

/* Define get param of codec. */
/* CNcomment:定义 codec 获取参数 */
typedef struct {
    td_u32 codec_id;
    td_void *private_data; /* codec external data */
    td_u32 private_data_size;
    td_void *private_attr; /* codec internal attr */
    td_u32 private_attr_size;
} acodec_get_private_param;

/* Define format query parameter of codec. */
/* CNcomment:定义 codec 格式查询参数 */
typedef struct {
    td_u32 cmd;
    ha_codec_format format;
} ha_codec_format_query_param;

/* Define packet query parameter of codec. */
/* CNcomment:定义 codec packet解码模式查询参数 */
typedef struct {
    /* packet query command. */
    /* CNcomment:packet模式查询命令 */
    td_u32 cmd;

    /* Packet mode.In this mode, the decoder can decode the next packet
     * only after processing an entire packet.
     */
    /* CNcomment:packet模式.在这种模式下，只有处理完当前整个packet后
     * 解码器才能处理下一个packet包
     */
    td_bool packet_decoder;
} ha_codec_packet_decoder_query_param;

/* Define hardware decode query parameter of an  codec. */
/* CNcomment:定义 codec hardware解码模式查询参数 */
typedef struct {
    /* hwdecode query command. */
    /* CNcomment: hwdecode模式查询命令 */
    td_u32 cmd;

    /* TD_TRUE: hwdecode mode else swdecode mode. */
    /* CNcomment:hwdecode模式 */
    td_bool hw_codec;
} ha_codec_hw_decode_query_param;

/* Define query parameter of dual decode capability. */
/* CNcomment:定义双路解码能力查询参数 */
typedef struct {
    td_u32 cmd; /* query dual decode capability command. */     /* CNcomment: 查询双路解码能力命令 */
    td_bool dual_dec_support; /* support dual decode or not. */ /* CNcomment:是否支持双路解码 */
} ha_codec_dual_decode_query_param;

/* Define setting parameter of dual decode ad output mode. */
/* CNcomment:设置双路解码AD输出模式 */
typedef struct {
    td_u32 cmd; /* set AD output mode command. */           /* CNcomment: 设置AD输出模式命令 */
    ha_codec_output_ad_mode ad_mode; /* AD output mode. */ /* CNcomment:AD输出模式 */
} ha_codec_ad_mode_set_param;

/* Define setting parameter of AD Balance. */
/* CNcomment:设置AD 权重 */
typedef struct {
    td_u32 cmd; /* set AD balance command. */            /* CNcomment: 设置AD权重命令 */
    td_s16 ad_balance; /* set the value of AD balance */ /* CNcomment:设置AD权重值 */
} ha_codec_ad_balance_set_param;

/* Define set parameter of whether work in compress mode. */
/* CNcomment:定义是否工作于在线压缩模式设置参数 */
typedef struct {
    /* set the compress mode command. */
    /* CNcomment:设置是否在线压缩模式命令 */
    td_u32 cmd;

    /* whether work in compress mode. */
    /* CNcomment:是否工作于在线压缩模式 */
    td_bool compress_mode;
} ha_codec_compress_mode_set_param;

/* Define get parameter of decoder output delay. */
/* CNcomment:定义获取解码器输出延时参数 */
typedef struct {
    td_u32 cmd; /* get the output delay command. */     /* CNcomment:获取解码器输出延时命令 */
    td_u32 output_delay; /* decoder output delay ms. */ /* CNcomment:解码器的输出延时(毫秒) */
} ha_codec_output_delay_get_param;

/* Define hardware decode set parameter of out buffer address. */
/* CNcomment:定义硬解输出缓存地址设置参数 */
typedef struct {
    /* set outbuf addr command. */
    /* CNcomment: 设置输出Buffer地址命令 */
    td_u32 cmd;

    /* outbuf addr. */
    /* CNcomment:输出Buffer地址 */
    td_u32 out_buf_addr;
} ha_codec_out_buf_addr_set_param;

/* parameter to set codec scratch buffer */
/* CNcomment:设置scratch buf */
typedef struct {
    td_void *scratch_buf;
    td_u32 scratch_buf_size;
} ha_codec_param_set_scratch_buf;

/* Definition of the distribution mode of the channels of a codec. */
/* CNcomment:定义codec 声道分布模式 */
typedef enum {
    /* Unused or empty. */
    /* CNcomment:未使用或为空 */
    HA_CODEC_CHANNEL_NONE = 0x0,

    /* Left front. */
    /* CNcomment:左前 */
    HA_CODEC_CHANNEL_LF = 0x1,

    /* Right front. */
    /* CNcomment:右前 */
    HA_CODEC_CHANNEL_RF = 0x2,

    /* Center front. */
    /* CNcomment:中置 */
    HA_CODEC_CHANNEL_CF = 0x3,

    /* left surround. */
    /* CNcomment:左环绕 */
    HA_CODEC_CHANNEL_LS = 0x4,

    /* Right surround. */
    /* CNcomment:右环绕 */
    HA_CODEC_CHANNEL_RS = 0x5,

    /* Low frequency effects. */
    /* CNcomment:低音 */
    HA_CODEC_CHANNEL_LFE = 0x6,

    /* Back surround. */
    /* CNcomment:后环绕 */
    HA_CODEC_CHANNEL_CS = 0x7,

    /* Left rear. */
    /* CNcomment:左后 */
    HA_CODEC_CHANNEL_LR = 0x8,

    /* Right rear. */
    /* CNcomment:右后 */
    HA_CODEC_CHANNEL_RR = 0x9,

    HA_CODEC_CHANNEL_MAX,
} ha_codec_channel;

/* Definition of the decoding mode of an decoder. */
/* CNcomment:定义解码器解码模式 */
typedef enum {
    /* PCM decoding mode. */
    /* CNcomment:PCM 解码模式 */
    HA_CODEC_DEC_MODE_RAWPCM = 0,

    /* SPIDF61937 passthrough decoding mode only, such as AC3/DTS. */
    /* CNcomment:透传解码模式 */
    HA_CODEC_DEC_MODE_THRU,

    /* PCM and passthrough decoding mode. */
    /* CNcomment:PCM + 透传解码模式 */
    HA_CODEC_DEC_MODE_SIMUL,

    HA_CODEC_DEC_MODE_MAX
} ha_codec_decode_mode;

/* Configuration parameters required by an  codec for creating devices. */
/* CNcomment:解码器创建设备的配置参数 */
typedef struct {
    /* expected output channels.
     * If the number of original channels is not equal to the number of expected output channels,
     * the decoded automatically performs down-mixing or up-mixing,
     * In this way, the number of output channels is equal to the value of desired_out_channels,
     * 1: mono; 2: stereo.
     */
    td_u32 channels;

    /* Bit width of the PCM data.Only 16-bit or 24-bit width is supported.
     * For the data of 16-bit width, 16-bit memory is used
     * For the data of 24-bit width, 32-bit memory is used
     * The active bits of the 24-bit PCM data are upper bits, and the lower eight bits are 0.
     */
    td_u32 bit_depth;

    td_u32 sample_rate; /* expected output sampling rate. */

    /* Input, pointer to private data.If the decoder does not contain private data, this parameter is set to 0. */
    /* CNcomment:IN  指向私有配置结构体的指针，如果解码器没有私有配置，设置为0 */
    td_void *private_data;

    /* Input, size of the private data.If the decoder does not contain private data, this parameter is set to 0. */
    /* CNcomment:IN 私有结构体大小, 如果解码器没有私有配置，设置为0 */
    td_u32 private_data_size;
} ha_codec_dec_param;

/* Input streams of an decoder. */
/* CNcomment:解码器输入码流结构体 */
typedef struct {
    /* Input, presentation time stamp (PTS) (unit:us).Note: The decoder must discard this information. */
    /* CNcomment:当前时间戳(单位：微秒) */
    td_s64 pts;

    /* Input/output, pointer to input streams.The decoder update this pointer after decoding. */
    /* CNcomment:IN/OUT 输入码流指针，解码完成后解码器更新该指针 */
    td_u8 *data;

    /* Input, size (in byte) of input streams. */
    /* CNcomment:IN 输入码流大小. unit: Byte */
    td_u32 size;

    /* Input, the last frame or not. */
    /* CNcomment:IN 是否最后一帧 */
    td_bool eos;
    /* Input, the packet loss. */
    /* CNcomment:IN 是否丢包 */
    td_bool pkg_loss;
} ha_codec_dec_in_packet;

/* Get parameter of an decoder. */
/* CNcomment:获取解码器打开参数 */
typedef struct {
    td_u32 cmd;
    ha_codec_dec_param *decode_param;
} ha_codec_get_dec_param;

/* Definition of the decoder output pts information. */
/* CNcomment:pts输出信息 */
typedef struct {
    union {
        /* left bytes of software decoder. */
        /* CNcomment:软解剩余字节数 */
        td_u32 sw_decoder_bytes_left;

        /* pts read position of software decoder. */
        /* CNcomment:硬解pts读位置 */
        td_u32 hw_decoder_pts_read_pos;
    } un_pts;

    union {
        /* ad left bytes of software decoder. */
        /* CNcomment:软解ad剩余字节数 */
        td_u32 sw_decoder_bytes_left;

        /* ad pts read position of software decoder. */
        /* CNcomment:硬解ad pts读位置 */
        td_u32 hw_decoder_pts_read_pos;
    } un_ad_pts;
} ha_codec_out_pts_info;

/* Output structure of the decoder. */
/* CNcomment:解码器输出结构体 */
typedef struct {
    /* Input, pointer to the decoded PCM data.
    Note: 1) pcm_out_buf must be 32-word aligned.
          2) pcm_out_buf is NULL when the decoder is working in HD_DEC_MODE_THRU mode. */
    /* CNcomment:IN 指向PCM 解码输出的指针. */
    td_s32 *pcm_out_buf;

    /* Input, size (in byte) of the buffer for storing the decoded PCM data
    Note: u32PcmOutBufSize is 0 when the decoder is working in HD_DEC_MODE_THRU mode. */
    /* CNcomment:IN  PCM 解码输出缓冲区大小. unit:Byte */
    td_u32 pcm_out_buf_size;

    /* Input, pointer to the decoded passthrough data.
    Note: 1) ps32BitsOutBuf must be 32-word aligned.
          2) ps32BitsOutBuf is NULL when the decoder is working in HD_DEC_MODE_RAWPCM mode. */
    /* CNcomment:IN 指向透传解码输出的指针. */
    td_s32 *bits_out_buf;

    /* Input, size (in byte) of the buffer for storing the decoded passthrough data (IEC61937)
    Note: u32BitsOutBufSize is 0 when the decoder is working in HD_DEC_MODE_RAWPCM mode. */
    /* CNcomment:IN 透传(IEC61937) 解码输出缓冲区大小. unit:Byte */
    td_u32 bits_out_buf_size;

    /* Output, number of output sampling points after the PCM data is decoded. */
    /* CNcomment:OUT. 解码Pcm 输出样点数 */
    td_u32 pcm_out_samples;

    /* Output, interleaved mode for PCM decoding. */
    /* CNcomment:OUT PCM  解码交织模式 */
    td_bool interleaved;

    /* Output, indicate plc decode */
    /* CNcomment:OUT. 标明plc解码 */
    td_bool compensate_flag;

    /* Output, bit per sampling. */
    /* CNcomment:OUT 位宽 */
    td_u32 bit_per_sample;

    /* Output, size (in byte) of the passthrough frame. */
    /* CNcomment:OUT 透传输出帧大小. unit: Byte */
    td_u32 bits_out_bytes;

    /* Output, number of output channels. */
    /* CNcomment:OUT 输出声道数 */
    td_u32 out_channels;

    /* Output, output sampling rate. */
    /* CNcomment:OUT. 输出采样频率 */
    td_u32 out_sample_rate;

    /* Output, number of original channels. */
    /* CNcomment:OUT 码流原始声道数 */
    td_u32 org_channels;

    /* Output, original sampling rate. */
    /* CNcomment:OUT 码流原始采样频率 */
    td_u32 org_sample_rate;

    /* Output, bit rate (in bit/s) of the compressed streams. */
    /* CNcomment:OUT 码流压缩率unit: bit/S */
    td_u32 bit_rate;

    /* Output, pts information. */
    /* CNcomment:OUT. pts信息 */
    ha_codec_out_pts_info pts_info;

    /* Output, output frame index. */
    /* CNcomment:OUT. 输出帧序号 */
    td_u32 frame_index;
} ha_codec_dec_out_packet;

/* Definition of the decoder. */
/* CNcomment:解码器设备定义 */
typedef struct ha_codec_decode {
    /* Input, description information about a decoder, such as MP3 or AAC. */
    /* CNcomment:IN 解码器描述名字，如MP3,AAC */
    const td_char *name;

    /* Input, decoder ID.Note: This ID is the identifier of a decoder, and must be unique. */
    /* CNcomment:IN 解码器标识，注意: 此标识作为解码器唯一身份标识，必须唯一而不能有重复 */
    const td_u32 codec_id;

    /* Input, decoder version. */
    /* CNcomment:IN 解码器版本描述 */
    const audio_lib_version version;

    /* Input, detailed information about a decoder. */
    /* CNcomment:IN 解码器详细描述信息 */
    const td_char *description;

    /* Output, pointer to the next decoder.This member variable is maintained by the client that calls the  codec,
    Therefore, the developers of the decoder can ignore this member variable. */
    /* CNcomment:OUT  指向下个解码器设备的指针. 该成员变量由调用Codec 的客户端维护,
    解码器开发者可以忽略该成员 */
    struct ha_codec_decode *next;

    /* Output, pointer to the dll symbol of an decoder,
    This member variable is maintained by the client that calls the  codec,
    Therefore, the developers of the decoder can ignore this member variable. */
    /* CNcomment:OUT 该成员变量由调用Codec 的客户端维护, 解码器开发者可以忽略该成员 */
    td_void *module;

    /*
    \brief Initializes a decoder. CNcomment:初始化解码器 CNend
    \attention
    \param[in] open_param pointer of the open params CNcomment:open参数结构体指针 CNend
    \param[out] decoder   pointer of the decoder handle CNcomment:解码器句柄指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*init)(td_void **decoder, const ha_codec_dec_param *open_param);

    /*
    \brief DeInitializes a decoder. CNcomment:去初始化解码器 CNend
    \attention
    \param[in] decoder   pointer of the decoder handle CNcomment:解码器句柄指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*deinit)(td_void *decoder);

    /*
    \brief Configure a decoder dynamically. You can call this API when a decoder works.
    CNcomment:解码器动态配置方法，用户可以在解码器运行时调用该接口 CNend
    \attention
    \param[in] decoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[in] cmd   the cmd code CNcomment:命令字 CNend
    \param[in] config pointer to application allocated structure to be used for initialization by the decoder
    CNcomment:解码器初始化结构体 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*send_cmd)(td_void *decoder, td_u32 cmd, td_void *arg);

    /*
    \brief Maximum size of the buffer required for storing the PCM data decoded by the decoder,
    The memory needs to be allocated based on the size on the client.
    CNcomment:解码器PCM输出所需最大缓冲区size. 客户端需要根据该方法分配内存 CNend
    \attention
    \param[in] decoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[out] out_size pointer to the max size of the pcm audio frame. unit:Byte
    CNcomment:最大PCM输出缓冲区大小 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*get_max_pcm_out_size)(td_void *decoder, td_u32 *out_size);

    /*
    \brief Maximum size of the buffer required for storing the passthrough data decoded by the decoder,
    The memory needs to be allocated based on the size on the client.
    CNcomment:解码器透传输出所需最大缓冲区size.客户端需要根据该方法分配内存 CNend
    \attention
    \param[in] decoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[out] out_size pointer to the max size of the iec61937 audio frame. unit:Byte
    CNcomment:最大透传输出缓冲区大小 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*get_max_bits_out_size)(td_void *decoder, td_u32 *out_size);

    /*
    \brief This method is used to decode a frame. CNcomment:解码器解码一帧方法 CNend
    \attention
    \param[in] h_decoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[in] apkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[out] a_out pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*decode_frame)(td_void *decoder, ha_codec_dec_in_packet *in_pkt, ha_codec_dec_out_packet *out_pkt);
} ha_codec_decode;

/* Parameters for creating an encoder. */
/* CNcomment:编码器创建配置参数 */
typedef struct {
    /* IN interleave or not. */
    /* CNcomment:交织模式 */
    td_bool interleaved;

    /* IN Number of channels (e.g 2 for stereo). */
    /* CNcomment:声道数 */
    td_u32 channels;

    /* IN bitwidth per sample. */
    /* CNcomment:位宽 */
    td_s32 bit_depth;

    /* IN desired samplerate. */
    /* CNcomment:期望采样率 */
    td_u32 sample_rate;

    /* IN Pcm samples per frame for encoder. */
    /* CNcomment:每帧采样点 */
    td_u32 samples_per_frame;

    /* Pointer of decoder private open parameters
     * note: set NULL if there is no private parameters
     */
    /* CNcomment: 解码器私有参数 */
    td_void *private_data;

    /* Size of decoder private open parameters
     * note: set to 0 if there is no private parameters
     */
    /* CNcomment: 解码器私有参数大小 */
    td_u32 private_data_size;
} ha_codec_enc_param;

/* input pcm packet struct */
/* CNcomment: 解码器输入pcm数据结构体 */
typedef struct {
    /* IN PTS (unit:us) Note: encoder should discard this information. */
    /* CNcomment:PTS，单位:微秒 */
    td_s64 pts;

    /* IN/OUT pointer to input auduo pcm data note: encoder would update data after encode. */
    /* CNcomment:输入数据指针 */
    td_u8 *data;

    /* IN/OUT size of the input auduo data. unit: Byte. */
    /* CNcomment:输入数据大小 */
    td_u32 size;
} ha_codec_enc_in_packet;

/* encoder output struct. */
/* CNcomment:编码器输出结构体 */
typedef struct {
    /* the pointer to encoded bitstream output buffer note: ps32BitsOutBuf must  be word32-aligned. */
    /* CNcomment:编码数据输出buffer */
    td_s32 *bits_out_buf;

    /* the buffer size of bitstream output buffer. unit:Byte. */
    /* CNcomment:编码器输出buffer大小 */
    td_u32 bits_out_buf_size;

    /* size of the encoded audio data frame, unit: Byte. */
    /* CNcomment:编码数据帧大小，单位:Byte */
    td_u32 bits_out_bytes;

    /* compress bit rate of the audio stream. */
    /* CNcomment:比特率 */
    td_u32 bit_rate;
} ha_codec_enc_out_packet;

/* encoder struct define. */
/* CNcomment:定义编码器结构体 */
typedef struct ha_codec_encode {
    /* Input, description information about audio encoder. */
    /* CNcomment:IN 编码器描述名字 */
    const td_char *name;

    /* encoder ID. Note: This ID is the identifier of a encoder, and must be unique. */
    /* CNcomment:IN 编码器标识，注意: 此标识作为编码器唯一身份标识，必须唯一而不能有重复 */
    const td_u32 codec_id;

    /* Input, encoder version. */
    /* CNcomment:IN 编码器版本描述 */
    const audio_lib_version version;

    /* Input, Description information of the audio encoder. */
    /* CNcomment:IN 编码器详细描述信息 */
    const td_char *description;

    /* OUT pointer to next encoder.manager by client.
    client:The layer of software that invokes the methods of the encoder. */
    /* CNcomment:OUT 下个编码器结构体指针，由客户管理 */
    struct ha_codec_encode *next;

    /* Output, pointer to the dll symbol of an encoder.
    This member variable is maintained by the client that calls the  codec Therefore,
    the developers of the encoder can ignore this member variable. */
    /* CNcomment:OUT 该成员变量由调用Codec 的客户端维护, 编码器开发者可以忽略该成员 */
    td_void *module;

    /*
    \brief Initializes a encoder. CNcomment:初始化编码器 CNend
    \attention
    \param[in] open_param pointer of the open params CNcomment:open参数结构体指针 CNend
    \param[out] ph_encoder   pointer of the encoder handle CNcomment:编码器句柄指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*init)(td_void **encoder, const ha_codec_enc_param *open_param);

    /*
    \brief DeInitializes a encoder. CNcomment:去初始化编码器 CNend
    \attention
    \param[in] h_encoder   pointer of the encoder handle CNcomment:编码器句柄 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*deinit)(td_void *encoder);

    /*
    \brief Configure a encoder dynamically. You can call this API before a encoder works.
    CNcomment:编码器动态配置方法，用户可以在编码器运行前调用该接口 CNend
    \attention
    \param[in] encoder   the encoder handle CNcomment:编码器句柄 CNend
    \param[in] cmd   the cmd code CNcomment:命令字 CNend
    \param[in] config pointer to application allocated structure to be used for initialization by the encoder
    CNcomment:编码器初始化结构体 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*send_cmd)(td_void *encoder, td_u32 cmd, td_void *arg);

    /*
    \brief Maximum output size of an encoded bitstream CNcomment:编码器输出所需最大缓冲区size CNend
    \attention
    \param[in] encoder   the encoder handle CNcomment:解码器句柄 CNend
    \param[out] out_size pointer to the max size of the encoded audio frame. unit:Byte
    CNcomment:最大编码帧输出缓冲区大小 CNend \retval ::0  SUCCESS CNcomment:成功 CNend \retval
    ::s32 FAILURE CNcomment:失败 CNend \see N/A
    */
    td_s32 (*get_max_bits_out_size)(td_void *encoder, td_u32 *out_size);

    /*
    \brief This method is used to encode a frame. CNcomment:编码器编码一帧方法 CNend
    \attention
    \param[in] encoder   the encoder handle CNcomment:编码器句柄 CNend
    \param[in] in_pkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[out] out_pkt pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*encode_frame)(td_void *encoder, ha_codec_enc_in_packet *in_pkt, ha_codec_enc_out_packet *out_pkt);
} ha_codec_encode;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HA_CODEC_H__ */
