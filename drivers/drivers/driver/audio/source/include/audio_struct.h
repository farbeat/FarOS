/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio structs
 * Author: audio
 */

#ifndef __AUDIO_STRUCT_H__
#define __AUDIO_STRUCT_H__

#include "chip_type.h"
#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AUDIO_40MS_NUM_PER_SECOND 25
#define AUDIO_20MS_NUM_PER_SECOND 50
#define AUDIO_10MS_NUM_PER_SECOND 100
#define AUDIO_5MS_NUM_PER_SECOND 200
#define AUDIO_MS_NUM_PER_SECOND 1000
#define AUDIO_US_NUM_PER_MS 1000
#define AUDIO_US_NUM_PER_SECOND 1000000

#define AUDIO_CLK_12288K 12288000
#define AUDIO_CLK_24576K 24576000

#define AUDIO_XRUN_LOG_TIMES  4

#define AUDIO_DSP_BUILD_DAY_LEN 6
#define AUDIO_DSP_BUILD_YEAR_LEN 4
#define AUDIO_DSP_BUILD_TIME_LEN 8
#define AUDIO_DSP_VERSION_LEN 4

#define sap_min(a, b) ((a) < (b) ? (a) : (b))
#define sap_max(a, b) ((a) > (b) ? (a) : (b))
#define audio_mem_align(addr, align) (((td_u32)(addr) + ((align) - 1)) & ~((align) - 1))
#define audio_mem_align_down(addr, align) (((td_u32)(addr)) & ~((align) - 1))

#define AB_ATTACH_MAX_NUM 16
#define AENC_PRIVATE_DATA_MAX_SIZE 16
#define DPM_PRIVATE_DATA_MAX_SIZE 16

#define AUDIO_DUMP_MASK_AI 0x1
#define AUDIO_DUMP_MASK_SEA 0x2
#define AUDIO_DUMP_MASK_AENC 0x4
#define AUDIO_DUMP_MASK_ADEC 0x8
#define AUDIO_DUMP_MASK_TRACK 0x10
#define AUDIO_DUMP_MASK_AO 0x20
#define AUDIO_DUMP_MASK_DPM 0x40
#define AUDIO_DUMP_MASK_MAX (AUDIO_DUMP_MASK_DPM << 1)

#define AUDIO_PROC_MASK_UART 0x1
#define AUDIO_PROC_MASK_DIAG_SHELL 0x2
#define AUDIO_PROC_MASK_MAX (AUDIO_PROC_MASK_DIAG_SHELL << 1)

typedef enum {
    SAMPLE_RATE_NULL = 0,      /* unknown */
    SAMPLE_RATE_4K = 4000,     /* 4 kHz sampling rate */
    SAMPLE_RATE_8K = 8000,     /* 8 kHz sampling rate */
    SAMPLE_RATE_11K = 11025,   /* 11.025 kHz sampling rate */
    SAMPLE_RATE_12K = 12000,   /* 12 kHz sampling rate */
    SAMPLE_RATE_16K = 16000,   /* 16 kHz sampling rate */
    SAMPLE_RATE_22K = 22050,   /* 22.050 kHz sampling rate */
    SAMPLE_RATE_24K = 24000,   /* 24 kHz sampling rate */
    SAMPLE_RATE_32K = 32000,   /* 32 kHz sampling rate */
    SAMPLE_RATE_44K = 44100,   /* 44.1 kHz sampling rate */
    SAMPLE_RATE_48K = 48000,   /* 48 kHz sampling rate */
    SAMPLE_RATE_88K = 88200,   /* 88.2 kHz sampling rate */
    SAMPLE_RATE_96K = 96000,   /* 96 kHz sampling rate */
    SAMPLE_RATE_176K = 176400, /* 176 kHz sampling rate */
    SAMPLE_RATE_192K = 192000, /* 192 kHz sampling rate */
    SAMPLE_RATE_288K = 288000, /* 288 kHz sampling rate */
    SAMPLE_RATE_384K = 384000, /* 384 kHz sampling rate */
    SAMPLE_RATE_768K = 768000, /* 768 kHz sampling rate */
    SAMPLE_RATE_MAX
} audio_sample_rate;

/* defines the bit depth during audio sampling. */
typedef enum {
    BIT_DEPTH_NULL = 0, /* unknown */
    BIT_DEPTH_8 = 8,    /* 8-bit depth */
    BIT_DEPTH_16X2_LH = 15,  /* 16-bit depth */
    BIT_DEPTH_16 = 16,  /* 16-bit depth */
    BIT_DEPTH_18 = 18,  /* 18-bit depth */
    BIT_DEPTH_20 = 20,  /* 20-bit depth */
    BIT_DEPTH_24 = 24,  /* 24-bit depth */
    BIT_DEPTH_32 = 32,  /* 32-bit depth */
    BIT_DEPTH_64 = 64,  /* 64-bit depth */

    BIT_DEPTH_MAX
} audio_bit_depth;

/* defines audio channels. */
typedef enum {
    AUDIO_CH_NULL = 0,
    AUDIO_CH_1 = 1,
    AUDIO_CH_MONO = AUDIO_CH_1,
    AUDIO_CH_2 = 2,
    AUDIO_CH_STEREO = AUDIO_CH_2,
    AUDIO_CH_4 = 4,
    AUDIO_CH_6 = 6,
    AUDIO_CH_8 = 8,
    AUDIO_CH_16 = 16,
    AUDIO_CH_MAX
} audio_channel;

/* defines the I2S(inter-IC sound) mode. */
typedef enum {
    I2S_STD_MODE = 0, /* I2S standard mode */
    I2S_PCM_MODE,     /* pcm mode */
    I2S_TDM_MODE,     /* TDM mode */
    I2S_PDM_MODE,     /* PDM mode */
    I2S_MODE_MAX
} audio_i2s_mode;

/* defines the I2S(inter-IC sound) master clock. */
typedef enum {
    I2S_MCLK_128_FS = 0, /* mclk 128*fs */
    I2S_MCLK_256_FS,     /* mclk 256*fs */
    I2S_MCLK_384_FS,     /* mclk 384*fs */
    I2S_MCLK_512_FS,     /* mclk 512*fs */
    I2S_MCLK_768_FS,     /* mclk 768*fs */
    I2S_MCLK_1024_FS,    /* mclk 1024*fs */
    I2S_MCLK_1536_FS,    /* mclk 1536*fs */
    I2S_MCLK_MAX
} audio_i2s_mclk;

/* defines the I2S(inter-IC sound) bclk clock. */
typedef enum {
    I2S_BCLK_1_DIV = 1,   /* bclk mclk/1 */
    I2S_BCLK_2_DIV = 2,   /* bclk mclk/2 */
    I2S_BCLK_3_DIV = 3,   /* bclk mclk/3 */
    I2S_BCLK_4_DIV = 4,   /* bclk mclk/4 */
    I2S_BCLK_6_DIV = 6,   /* bclk mclk/6 */
    I2S_BCLK_8_DIV = 8,   /* bclk mclk/8 */
    I2S_BCLK_12_DIV = 12, /* bclk mclk/12 */
    I2S_BCLK_16_DIV = 16, /* bclk mclk/16 */
    I2S_BCLK_24_DIV = 24, /* bclk mclk/24 */
    I2S_BCLK_32_DIV = 32, /* bclk mclk/32 */
    I2S_BCLK_48_DIV = 48, /* bclk mclk/48 */
    I2S_BCLK_64_DIV = 64, /* bclk mclk/64 */
    I2S_BCLK_96_DIV = 96, /* bclk mclk/96 */
    I2S_BCLK_MAX
} audio_i2s_bclk;

/* defines the I2S(inter-IC sound) data valid after frame sync clock at PCM mode. */
typedef enum {
    I2S_PCM_0_DELAY = 0,   /* 0 bclk cycles delay */
    I2S_PCM_1_DELAY = 1,   /* 1 bclk cycles delay */
    I2S_PCM_8_DELAY = 8,   /* 8 bclk cycles delay */
    I2S_PCM_16_DELAY = 16, /* 16 bclk cycles delay */
    I2S_PCM_17_DELAY = 17, /* 17 bclk cycles delay */
    I2S_PCM_24_DELAY = 24, /* 24 bclk cycles delay */
    I2S_PCM_32_DELAY = 32, /* 32 bclk cycles delay */
    I2S_PCM_DELAY_MAX
} audio_i2s_pcm_delay;

/* EIA-CEA-861-G Table 31 */
typedef enum {
    AUDIO_FORMAT_STREAM = 0, /* Default type, refer to stream header */
    AUDIO_FORMAT_PCM,        /* Audio coding PCM type */
    AUDIO_FORMAT_AC3,        /* Audio coding dolby digital(ac3) type */
    AUDIO_FORMAT_MPEG1,      /* Audio coding MPEG1 type */
    AUDIO_FORMAT_MP3,        /* Audio coding MP3 type */
    AUDIO_FORMAT_MPEG2,      /* Audio coding MPEG2 type */
    AUDIO_FORMAT_AAC_LC,     /* Audio coding AAC type */
    AUDIO_FORMAT_DTS,        /* Audio coding DTS type */
    AUDIO_FORMAT_ATRAC,      /* Audio coding ATRAC type */
    AUDIO_FORMAT_DSD,        /* Audio coding One Bit Audio type */
    AUDIO_FORMAT_EAC3,       /* Audio coding dolby digital plus type */
    AUDIO_FORMAT_DTS_HD,     /* Audio coding DTS HD type */
    AUDIO_FORMAT_MAT,        /* Audio coding MAT(DVD Forum MLP) type */
    AUDIO_FORMAT_DST,        /* Audio coding DST type */
    AUDIO_FORMAT_WMA_PRO,    /* Audio coding WMA PRO type */
    AUDIO_FORMAT_CXT,        /* Audio coding refer to CXT */
} audio_format;

/* EIA-CEA-861-G Table 33 */
typedef enum {
    /* Refer to Audio Coding Type (CT) field in Data Byte 1 */
    AUDIO_FORMAT_EXT_CT = 0x0,

    /* The next three CXT values are defined in CEA-861-E only.
     * They do not exist in older versions, and in CEA-861-F and CEA-861-F
     * they are defined as 'Not in use'. */
    AUDIO_FORMAT_EXT_HE_AAC,
    AUDIO_FORMAT_EXT_HE_AAC_V2,
    AUDIO_FORMAT_EXT_MPEG_SURROUND,

    /* The following CXT values are only defined in CEA-861-F. */
    AUDIO_FORMAT_EXT_MPEG4_HE_AAC = 0x4,
    AUDIO_FORMAT_EXT_MPEG4_HE_AAC_V2,
    AUDIO_FORMAT_EXT_MPEG4_AAC_LC,
    AUDIO_FORMAT_EXT_DRA,

    AUDIO_FORMAT_EXT_MPEG4_HE_AAC_SURROUND,       /* MPEG-4 HE AAC + MPEG Surround */
    AUDIO_FORMAT_EXT_MPEG4_AAC_LC_SURROUND = 0xa, /* MPEG-4 AAC LC + MPEG Surround */

    /* The following CXT values are only defined in CEA-861-G. */
    AUDIO_FORMAT_EXT_MPEGH_3D, /* Audio Extension Type Code 11 (MPEG-H 3D Audio) */
    AUDIO_FORMAT_EXT_AC4,      /* Audio Extension Type Code 12 (AC-4) */
    AUDIO_FORMAT_EXT_LPCM_3D,  /* Audio Extension Type Code 13 (L-PCM 3D Audio) */
} audio_format_ext;

typedef enum {
    AUDIO_LIB_NULL = 0x0,

    AUDIO_LIB_DSP0_APP,
    AUDIO_LIB_DSP1_APP,

    AUDIO_LIB_HAID_NULL,
    AUDIO_LIB_HAID,
    AUDIO_LIB_HAID_MAX,

    AUDIO_LIB_DEC_NULL,
    AUDIO_LIB_DEC_PCM,
    AUDIO_LIB_DEC_MP3,
    AUDIO_LIB_DEC_AAC,
    AUDIO_LIB_DEC_SBC,
    AUDIO_LIB_DEC_MSBC,
    AUDIO_LIB_DEC_FLAC,
    AUDIO_LIB_DEC_LC3,
    AUDIO_LIB_DEC_SILK,
    AUDIO_LIB_DEC_L2HC,
    AUDIO_LIB_DEC_OPUS,
    AUDIO_LIB_DEC_VORBIS,
    AUDIO_LIB_DEC_AMRWB,
    AUDIO_LIB_DEC_MAX,

    AUDIO_LIB_ENC_NULL,
    AUDIO_LIB_ENC_PCM,
    AUDIO_LIB_ENC_MP3,
    AUDIO_LIB_ENC_AAC,
    AUDIO_LIB_ENC_SBC,
    AUDIO_LIB_ENC_MSBC,
    AUDIO_LIB_ENC_LC3,
    AUDIO_LIB_ENC_SILK,
    AUDIO_LIB_ENC_L2HC,
    AUDIO_LIB_ENC_OPUS,
    AUDIO_LIB_ENC_AMRWB,
    AUDIO_LIB_ENC_MAX,

    AUDIO_LIB_SEA_AFE_NULL,
    AUDIO_LIB_SEA_AFE_SEE,
    AUDIO_LIB_SEA_AFE_EXT0,
    AUDIO_LIB_SEA_AFE_EXT1,
    AUDIO_LIB_SEA_AFE_MAX,

    AUDIO_LIB_SEA_AAI_NULL,
    AUDIO_LIB_SEA_AAI_KWS,
    AUDIO_LIB_SEA_AAI_ASR,
    AUDIO_LIB_SEA_AAI_ASD,
    AUDIO_LIB_SEA_AAI_EXT0,
    AUDIO_LIB_SEA_AAI_EXT1,
    AUDIO_LIB_SEA_AAI_MAX,

    AUDIO_LIB_AEF_NULL,
    AUDIO_LIB_AEF,
    AUDIO_LIB_AEF_MAX,

    AUDIO_LIB_DPM_NULL,
    AUDIO_LIB_DPM_FIR,
    AUDIO_LIB_DPM_MAX,

    AUDIO_LIB_MAX,
} audio_lib_id;

typedef enum {
    AUDIO_LIB_TYPE_BUILT_IN,
    AUDIO_LIB_TYPE_OVERLAY,

    AUDIO_LIB_TYPE_MAX,
} audio_lib_type;

typedef enum {
    AUDIO_CORE_NULL = 0,
    AUDIO_CORE_CPU_K,
    AUDIO_CORE_CPU_U,
    AUDIO_CORE_MCU0,
    AUDIO_CORE_DSP0,
    AUDIO_CORE_DSP1,
    AUDIO_CORE_NPU0,
    AUDIO_CORE_MAX,
} audio_core_id;

typedef enum {
    AUDIO_STATE_IDLE = 0,
    AUDIO_STATE_RUNNING,
    AUDIO_STATE_STOPPED,
    AUDIO_STATE_MAX
} audio_state;

typedef enum {
    AUDIO_SYS_DEEP_STANDBY = 0,
    AUDIO_SYS_ACTIVE,
    AUDIO_SYS_ACTIVE_STANDBY,
    AUDIO_SYS_STATE_MAX
} audio_sys_state;

typedef enum {
    AUDIO_DSP_FREQ_31M = 31000000,
    AUDIO_DSP_FREQ_64M = 64000000,
    AUDIO_DSP_FREQ_147M = 147456000,
    AUDIO_DSP_FREQ_176M = 176947200,
    AUDIO_DSP_FREQ_196M = 196608000,
    AUDIO_DSP_FREQ_294M = 294912000,
} audio_dsp_freq;

typedef enum {
    AUDIO_CLOCK_XO,
    AUDIO_CLOCK_PLL,
    AUDIO_CLOCK_MAX,
} audio_clock_type;

typedef enum {
    AUDIO_LIB_LOAD,
    AUDIO_LIB_UNLOAD,
    AUDIO_LIB_LOAD_TYPE_MAX,
} audio_lib_load_type;

typedef struct {
    audio_clock_type clock_type;
    td_u32 clock_freq;
} audio_clock_attr;

/* defines the I2S(inter-IC sound) attribute. */
typedef struct {
    td_bool master;    /* true: master, false: slave */
    audio_i2s_mode i2s_mode;
    audio_i2s_mclk mclk;
    audio_i2s_bclk bclk;
    audio_bit_depth bit_depth;
    audio_channel channels;
    td_bool sample_rise_edge; /* true: positive, false: negative */

    /* Only work for PCM mode */
    audio_i2s_pcm_delay pcm_delay_cycle;
} audio_i2s_attr;

/* defines the audio frame. */
typedef struct {
    td_bool interleaved;  /* whether the data is interleaved */
    td_u32 channels;     /* Number of Channels */
    td_u32 bit_depth;    /* Bit Depth */
    td_u32 sample_rate;  /* Sampling Rate */
    td_s64 pts;          /* presentation time stamp (PTS, unit: us) */
    td_s32 *pcm_buffer;  /* pointer to the buffer for storing the pulse code modulation (PCM) data */
    td_s32 *bits_buffer; /* pointer to the buffer for storing the stream data */
    td_u32 bits_bytes;   /* IEC61937 data size */
    td_u32 pcm_samples;  /* number of sampling points of the PCM data */
    td_u32 frame_index;  /* Frame ID */

    /* eos */
    /* CNcomment: eos */
    td_bool eos;
    /* the packet loss */
    /* CNcomment: 是否丢包 CNend */
    td_bool pkg_loss;
} audio_frame;

/* defines audio format */
typedef struct {
    td_u32 channels;    /* Number of channels */
    td_u32 bit_depth;   /* ::audio_bit_depth */
    td_u32 sample_rate; /* ::audio_sample_rate */
    td_u32 samples_per_frame;
} audio_pcm_format;

typedef struct {
    td_u8 *data; /* Data Buffer */
    td_u32 size; /* Data Size */
    td_s64 pts;  /* us */
    td_bool eos;
    td_bool pkg_loss; /* the packet loss CNcomment: 是否丢包 CNend */
} audio_stream_buf;

/* defines the audio buffer */
typedef struct {
    td_s32 fd;        /* buffer file handle, set to -1 if not support */
    td_u32 size;      /* buffer size */
    td_s32 cache;     /* buffer has cache for CPU */
    td_u8 *virt_addr; /* kernel virtual addr */
    td_u64 phys_addr; /* physic addr */
    td_void *dma_buf; /* kernel dma buffer */
} audio_buffer;

/* defines the audio buffer */
typedef struct {
    td_handle handle;
    td_u32 buf_addr;
    td_u32 size;
    td_u32 read_pos;
    td_u32 write_pos;
    td_u32 overrun_cnt; /* write overrun count */
    td_u32 tail_size;

    /* info buf */
    td_u32 info_addr;
    td_u16 info_num; /* total number */
    td_u16 info_used; /* used number */
} audio_ring_buf;

typedef struct {
    td_u32 buff_addr;
    td_u32 buff_size;
    td_u32 buff_wptr;
    td_u32 buff_rptr;
    td_u32 period_buf_size;
    td_u32 threshold_size;
} audio_aiao_buf_info;

typedef struct {
    td_u32 xrun_cnt;
    td_u32 start_pos;
} xrun_stage_info;

typedef struct {
    td_u32 xrun_stage_cnt;
    xrun_stage_info max_xrun[AUDIO_XRUN_LOG_TIMES];  /* maximum xrun, sort by count */
    xrun_stage_info last_xrun[AUDIO_XRUN_LOG_TIMES]; /* latest xrun, sort by time */
} audio_port_run_status;

#define ADP_DEFAULT_INFO_FRAMES 16
#define ADP_DEFAULT_INFO_BUF_SIZE (ADP_DEFAULT_INFO_FRAMES * sizeof(audio_frame))

/* defines the audio adp buffer */
typedef struct {
    td_u32 rbuf_size;
    td_u32 tail_size;
    td_u32 info_buf_size;
} audio_adp_attr;

/* audio event */
typedef enum {
    AUDIO_EVENT_PROCESS = 0x5a5aff00, /* module process */
    AUDIO_EVENT_FRAME_INFO_CHANGE, /* report new stream info to output connector */

    AUDIO_EVENT_GET_EOS,    /* get eos from input connector */
    AUDIO_EVENT_REPORT_EOS, /* report eos to input connector */

    AUDIO_EVENT_REPORT_PTS, /* report eos to output connector */
    AUDIO_EVENT_GET_DELAY,  /* get delay of output connector */

    AUDIO_EVENT_REPORT_SCENE, /* report audio scene to input connector */

    AUDIO_EVENT_REPORT_INPUT_EMPTY, /* report input buffer empty to input connector */

    AUDIO_EVENT_TYPE_MAX,
} audio_event_type;

/* Definition of the lib version. */
/* CNcomment: lib 版本定义 */
typedef union {
    struct {
        td_u8 version_major; /* Major version. */ /* CNcomment:主版本号 */
        td_u8 version_minor; /* Minor version. */ /* CNcomment:副版本号 */
        td_u8 revision; /* Revision version. */   /* CNcomment:修订版本 */
        td_u8 step; /* Step version. */           /* CNcomment:阶段性版本 */
    } bits;
    td_u32 version;
} audio_lib_version;

typedef td_s32 (*audio_event_proc)(td_handle handle, td_s32 event, td_void *param, td_void *context);

/* attach info item  */
typedef struct {
    td_handle src_handle;
    td_u32 src_mod;
    td_u32 src_id;

    td_u32 data_type;

    td_handle dest_handle;
    td_u32 dest_mod;
    td_u32 dest_id;
} ab_attach_item;

/* base info item */
typedef struct {
    td_char day[AUDIO_DSP_BUILD_DAY_LEN + 1];
    td_char year[AUDIO_DSP_BUILD_YEAR_LEN + 1];
    td_char time[AUDIO_DSP_BUILD_TIME_LEN + 1];
    td_char dsp_version[AUDIO_DSP_VERSION_LEN + 1];
} ab_version_info;

typedef struct {
    audio_clock_attr clock_attr;
} ab_status_info;

/* running step */
typedef enum {
    AUDIO_STEP_SYSTEM_INIT = 0,
    AUDIO_STEP_CMD_INIT,

    AUDIO_STEP_CMD_PROCESS,
    AUDIO_STEP_MSG_PROCESS,
    AUDIO_STEP_DRIVER_PROCESS,
    AUDIO_STEP_SYSTEM_IDLE,

    AUDIO_STEP_CMD_DEINIT,
    AUDIO_STEP_SYSTEM_DEINIT,

    AUDIO_STEP_MAX,
} audio_run_step;

typedef struct {
    /* run status */
    td_u32 heartbeat;
    audio_run_step run_step;

    /* sys status */
    audio_sys_state sys_state;
    td_u32 sys_active;
    td_u32 sys_active_standby;
    td_u32 sys_deep_standby;

    /* log status */
    td_u32 log_success;
    td_u32 log_busy;
    td_u32 log_total_len;
    td_u32 log_cur_len;

    /* msg status */
    td_u32 msg_success;
    td_u32 msg_illegal_param;
    td_u32 msg_ack_timeout;
    td_u32 msg_underflow;
    td_u32 msg_crc_fail;

    /* msg int status */
    td_u32 msg_int_fail;
    td_u32 msg_int_retrigger;
} probe_status_info;

typedef struct {
    td_u8 dump_mask;
    td_u8 proc_mask;
} audio_debug_cfg;

typedef struct {
    td_u32 cur_interval; /* current interval(us) */
    td_u32 max_interval; /* maximum interval(us) */
    td_u32 min_interval; /* minimum interval(us) */
    td_u32 avg_interval; /* average interval(us) */
} spend_status_info;

/**
 * @if Eng
 * @brief  Audio scene
 * @else
 * @brief  音频场景定义
 * @endif
 */
typedef enum {
    AUDIO_SCENE_UNKNOWN = 0, /*!< @if Eng Audio scene unknown.
                                  @else   未知的音频场景 @endif */
    AUDIO_SCENE_BT_MUSIC,   /*!< @if Eng Bluetooth Music.
                                 @else   蓝牙音乐场景 @endif */
    AUDIO_SCENE_BT_VOIP,    /*!< @if Eng Bluetooth voip call.
                                 @else   蓝牙通话场景 @endif */

    AUDIO_SCENE_NUM_MAX,
} audio_scene;

/**
 * @if Eng
 * @brief  audio product form
 * @else
 * @brief  音频产品形态
 * @endif
 */
typedef enum {
    AUDIO_PRODUCT_FORM_TWS_LEFT,   /*!< @if Eng product form is only left channel.
                                        @else   产品形态为真无线耳机左耳 @endif */

    AUDIO_PRODUCT_FORM_TWS_RIGHT,  /*!< @if Eng product form is only right channel.
                                        @else   产品形态为真无线耳机右耳 @endif */

    AUDIO_PRODUCT_FORM_NON_TWS,    /*!< @if Eng product form is not tws.
                                        @else   产品形态不是真无线耳机 @endif */

    AUDIO_PRODUCT_FORM_MAX,
} audio_product_form;

/**
 * @if Eng
 * @brief  audio TWS mode
 * @else
 * @brief  真无线耳机模式
 * @endif
 */
typedef enum {
    AUDIO_TWS_MODE_NON_TWS,  /*!< @if Eng single-ear mode.
                                  @else   单耳模式 @endif */

    AUDIO_TWS_MODE_FORWARD,  /*!< @if Eng TWS mode is forward.
                                  @else   转发模式 @endif */

    AUDIO_TWS_MODE_LISTEN,   /*!< @if Eng TWS mode is listen.
                                  @else   协同模式 @endif */

    AUDIO_TWS_MODE_DUAL_TRANSMIT, /*!< @if Eng TWS mode is dual transmit.
                                       @else   双发模式 @endif */

    AUDIO_TWS_MODE_MAX,
} audio_tws_mode;

/**
 * @if Eng
 * @brief  audio TWS role
 * @else
 * @brief  真无线耳机角色
 * @endif
 */
typedef enum {
    AUDIO_TWS_ROLE_MASTER,   /*!< @if Eng TWS role is master.
                                  @else 真无线耳机主耳 @endif */

    AUDIO_TWS_ROLE_SLAVE,    /*!< @if Eng TWS role is slave.
                                  @else 真无线耳机副耳 @endif */

    AUDIO_TWS_ROLE_FREEMAN,  /*!< @if Eng TWS role is freeman.
                                  @else 真无线耳机自由播放 @endif */

    AUDIO_TWS_ROLE_MAX,
} audio_tws_role;

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
} audio_toggle_config;

#define AUDIO_SPK_CALI_COEF_LEN_MAX 96

typedef struct {
    td_u32 coef_len;
    td_u8 coefs[AUDIO_SPK_CALI_COEF_LEN_MAX];
} audio_spk_cali;

typedef struct {
    audio_product_form product_form;
    td_bool tws_mono_mode;
    audio_tws_mode tws_mode;
    audio_tws_role tws_role;
} ab_tws_info;

/* proc item of audio base module */
typedef struct {
    ab_attach_item attach_list[AB_ATTACH_MAX_NUM];

    ab_version_info version_info;
    ab_status_info status_info;
    ab_tws_info tws_info;
} ab_proc_item;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __AUDIO_STRUCT_H__ */
