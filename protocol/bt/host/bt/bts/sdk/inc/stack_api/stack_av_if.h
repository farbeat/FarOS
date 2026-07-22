/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: stack a2dp interface
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_AV_IF_H
#define STATCK_AV_IF_H

#include "ipc.h"
#include "stack_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ************** Macro Definition ***************** */
/* dev_type */
#define BTSDK_A2DP_PLAYER 0x0001
#define BTSDK_A2DP_HEADPHONE 0x0001

/* sep_type */
#define BTSDK_AUDIOSRC 0x00
#define BTSDK_AUDIOSNK 0x01

#define BTSDK_MAX_USERMEMORY 100
#define BTSDK_CODECCAPS_LEN 0x10
#define BTSDK_REGSRC_NUM 0x03
#define BTSDK_REGSNK_NUM 0x02

/* codec_type */
#define BTSDK_A2DP_SBC 0x00
#define BTSDK_A2DP_MPEG12 0x01
#define BTSDK_A2DP_MPEG24 0x02 /* that is AAC */
#define BTSDK_A2DP_SBC_PAYLOAD_TYPE 0x60
#define BTSDK_A2DP_MPEG12_PAYLOAD_TYPE 0x61

/* codec_priority */
#define BTSDK_A2DP_CODEC_PRIORITY_1 1
#define BTSDK_A2DP_CODEC_PRIORITY_2 2
#define BTSDK_A2DP_CODEC_PRIORITY_3 3

/* codec_num */
#define BTSDK_CODEC_NUM_1 1
#define BTSDK_CODEC_NUM_2 2
#define BTSDK_CODEC_NUM_3 3

/* content_protect */
#define BTSDK_CONTENT_PROTECT_1 1
#define BTSDK_CONTENT_PROTECT_2 2
#define BTSDK_CONTENT_PROTECT_3 3

/* MPEG12 capabilitis */
/* chnl_mode */
#define BTSDK_A2DP_MPEG12_JOINTSTEREO 0x01
#define BTSDK_A2DP_MPEG12_STEREO 0x02
#define BTSDK_A2DP_MPEG12_DUAL 0x04
#define BTSDK_A2DP_MPEG12_MONO 0x08
/* crc */
#define BTSDK_A2DP_MPEG12_CRCSUPPORT 0x01
/* layer */
#define BTSDK_A2DP_MPEG12_LAYER1 0x04
#define BTSDK_A2DP_MPEG12_LAYER2 0x02
#define BTSDK_A2DP_MPEG12_LAYER3 0x01
/* sample_frequency */
#define BTSDK_A2DP_MPEG12_FS48000 0x01
#define BTSDK_A2DP_MPEG12_FS44100 0x02
#define BTSDK_A2DP_MPEG12_FS32000 0x04
#define BTSDK_A2DP_MPEG12_FS24000 0x08
#define BTSDK_A2DP_MPEG12_FS22050 0x10
#define BTSDK_A2DP_MPEG12_FS16000 0x20
/* mpf */
#define BTSDK_A2DP_MPEG12_MPF2SUPPORT 0x01
/* bitrate */
#define BTSDK_A2DP_MPEG12_BITRATE0000 0x0001 /* free */
#define BTSDK_A2DP_MPEG12_BITRATE0001 0x0002 /* 32Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0010 0x0004 /* 40Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0011 0x0008 /* 48Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0100 0x0010 /* 56Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0101 0x0020 /* 64Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0110 0x0040 /* 80Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE0111 0x0080 /* 96Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1000 0x0100 /* 112Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1001 0x0200 /* 128Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1010 0x0400 /* 160Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1011 0x0800 /* 192Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1100 0x1000 /* 224Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1101 0x2000 /* 256Kbps for MPEG-1 layer 3 */
#define BTSDK_A2DP_MPEG12_BITRATE1110 0x4000 /* 320Kbps for MPEG-1 layer 3 */
/* vbr */
#define BTSDK_A2DP_MPEG12_VBRSUPPORT 0x01

enum { /* mask in struct A2DPAPP_SEPStru */
    BTSDK_A2DP_SEPMASK_ISSRCSVC = 0x10,
    BTSDK_A2DP_SEPMASK_ISSNKSVC = 0x20
};

/* SBC capabilitis */
enum {                              /* Octet1 */
    BTSDK_A2DP_SBC_SF_16000 = 0x80, /* sampling_freq, do not need to shift again, already in macro */
    BTSDK_A2DP_SBC_SF_32000 = 0x40,
    BTSDK_A2DP_SBC_SF_44100 = 0x20, /* SRC must support 44.1 or 48, SNK must support both */
    BTSDK_A2DP_SBC_SF_48000 = 0x10,
    BTSDK_A2DP_SBC_SF_ALL = 0xF0,
    BTSDK_A2DP_SBC_CHMODE_MONO = 0x08, /* channel_mode */
    BTSDK_A2DP_SBC_CHMODE_DUAL_CHANNEL = 0x04,
    BTSDK_A2DP_SBC_CHMODE_STEREO = 0x02,
    BTSDK_A2DP_SBC_CHMODE_JOINT_STEREO = 0x01,
    BTSDK_A2DP_SBC_CHMODE_ALL = 0x0F
};

enum {                             /* Octet2 */
    BTSDK_A2DP_SBC_BLOCK_4 = 0x80, /* block_len, do not need to shift */
    BTSDK_A2DP_SBC_BLOCK_8 = 0x40,
    BTSDK_A2DP_SBC_BLOCK_12 = 0x20,
    BTSDK_A2DP_SBC_BLOCK_16 = 0x10,
    BTSDK_A2DP_SBC_BLOCK_ALL = 0xF0,
    BTSDK_A2DP_SBC_SUBBAND_4 = 0x08, /* subbands, do not need to shift */
    BTSDK_A2DP_SBC_SUBBAND_8 = 0x04,
    BTSDK_A2DP_SBC_SUBBAND_ALL = 0x0C,
    BTSDK_A2DP_SBC_SNR = 0x02,      /* allocation_method, do not need to shift */
    BTSDK_A2DP_SBC_LOUDNESS = 0x01, /* Default, most headphone only support Loudness */
    BTSDK_A2DP_SBC_ALLOCATION_ALL = 0x03
};

#define BTSDK_A2DP_SBC_MINBITPOOL 0x02
#define BTSDK_A2DP_SBC_MAXBITPOOL 0x44

/* MPEG24(AAC) capabilitis */
/* MPEG AAC Sampling Frequency */
#define BTSDK_A2DP_AAC_SF8000 0x0800
#define BTSDK_A2DP_AAC_SF11025 0x0400
#define BTSDK_A2DP_AAC_SF12000 0x0200
#define BTSDK_A2DP_AAC_SF16000 0x0100
#define BTSDK_A2DP_AAC_SF22050 0x0080
#define BTSDK_A2DP_AAC_SF24000 0x0040
#define BTSDK_A2DP_AAC_SF32000 0x0020
#define BTSDK_A2DP_AAC_SF44100 0x0010
#define BTSDK_A2DP_AAC_SF48000 0x0008
#define BTSDK_A2DP_AAC_SF64000 0x0004
#define BTSDK_A2DP_AAC_SF88200 0x0002
#define BTSDK_A2DP_AAC_SF96000 0x0001
#define BTSDK_A2DP_AAC_SFSEPALL 0x0FFF
/* MPEG AAC Object type */
#define BTSDK_A2DP_AAC_MPEG2_LC 0x80
#define BTSDK_A2DP_AAC_MPEG4_LC 0x40
#define BTSDK_A2DP_AAC_MPEG4_LTP 0x20
#define BTSDK_A2DP_AAC_MPEG4_SCALABLE 0x10
#define BTSDK_A2DP_AAC_OBJECT_SEPALL 0xF0
/* MPEG AAC Channels */
#define BTSDK_A2DP_AAC_CH_1 0x02
#define BTSDK_A2DP_AAC_CH_2 0x01
/* MPEG AAC VBR Support */
#define BTSDK_A2DP_AAC_VBR 0x01
#define BTSDK_A2DP_AAC_NOVBR 0x00

#define BTSDK_A2DP_AUDIOCARD_NAME_LEN 0x80
#define BTSDK_A2DP_CODECCAPS_LEN 0x10

/* rtfc Mode */
#define BTSDK_A2DP_RTFC_MODE_BASIC 0x00
#define BTSDK_A2DP_RTFC_MODE_STREAMING 0x04

/* FCS Types */
#define BTSDK_FCS_TYPE_NOFCS 0x00
#define BTSDK_FCS_TYPE_16BITFCS 0x01

/* SEP type */
#define BTSDK_A2DP_TSEP_SRC 0x00
#define BTSDK_A2DP_TSEP_SNK 0x01
#define BTSDK_A2DP_CONNECT_MASK_SRC 0x02 /* Source, otherwise is sink */

#define BTSDK_A2DP_CODEC_SBC 0x00 /* now only support sbc codec. */
#define BTSDK_A2DP_CODEC_AAC 0x02
#define BTSDK_A2DP_CODEC_NONA2DP 0xFF

enum {
    BTSDK_A2DP_MEDIATYPE_AUDIO = 0x00,
    BTSDK_A2DP_MEDIATYPE_VIDEO = 0x01,
    BTSDK_A2DP_MEDIATYPE_MULTIMEDIA = 0x02
};

enum {                                 /* Service Category */
    BTSDK_A2DP_SC_MEDIA_TRANSPORT = 1, /* 0000,0001 */
    BTSDK_A2DP_SC_REPORTING,           /* 0000,0010 */
    BTSDK_A2DP_SC_RECOVERY,            /* 0000,0011 */
    BTSDK_A2DP_SC_CONTENT_PROTECTION,  /* 0000,0100 */
    BTSDK_A2DP_SC_HEADER_COMPRESSION,  /* 0000,0101 */
    BTSDK_A2DP_SC_MULTIPLEXING,        /* 0000,0110 */
    BTSDK_A2DP_SC_MEDIA_CODEC,         /* 0000,0111 */
    BTSDK_A2DP_SC_DELAY_REPORTING      /* 0000,1000 */
};

enum {
    BTSDK_A2DP_SBC_ALLOC_METHOD_LOUNDNESS,
    BTSDK_A2DP_SBC_ALLOC_METHOD_SNR
};

enum {
    BTSDK_A2DP_SBC_CHANNEL_MODE_MONO,
    BTSDK_A2DP_SBC_CHANNEL_MODE_DUAL_CHANNEL,
    BTSDK_A2DP_SBC_CHANNEL_MODE_STEREO,
    BTSDK_A2DP_SBC_CHANNEL_MODE_JOINT_STEREO,
};

/* A2DP stream state */
enum {
    BTSDK_A2DP_STREAM_UNKNOWN, /* no a2dp */
    BTSDK_A2DP_STREAM_CREATE,
    BTSDK_A2DP_STREAM_OPENED,
    BTSDK_A2DP_STREAM_START,
    BTSDK_A2DP_STREAM_SUSPENDED,
    BTSDK_A2DP_STREAM_CLOSED,
    BTSDK_A2DP_STREAM_START_REQ,
};

/* AVDTP Codec type */
typedef enum {
    BTSDK_AVDTP_CODEC_TYPE_SBC,
    BTSDK_AVDTP_CODEC_TYPE_MPEG1_2_AUDIO,
    BTSDK_AVDTP_CODEC_TYPE_MPEG2_4_AAC,
    BTSDK_AVDTP_CODEC_TYPE_ATRAC,
    BTSDK_AVDTP_CODEC_TYPE_OPUS,
    BTSDK_AVDTP_CODEC_TYPE_H263,
    BTSDK_AVDTP_CODEC_TYPE_MPEG4_VSP,
    BTSDK_AVDTP_CODEC_TYPE_H263_PROF3,
    BTSDK_AVDTP_CODEC_TYPE_H263_PROF8,
    BTSDK_AVDTP_CODEC_TYPE_LHDC,
    BTSDK_AVDTP_CODEC_TYPE_ERR
} service_avdtp_codec_type;

/* Sbc config */
typedef struct {
    uint32_t sample_f;
    uint8_t chan_mode;
    uint8_t block_len;
    uint8_t sub_band;
    uint8_t alloc_method;
    uint8_t min_bit_pool;
    uint8_t max_bit_pool;
} sdk_sbc_cfg_stru;

/* aac config */
typedef struct {
    uint32_t sample_rate;
    uint32_t bit_rate;
    uint8_t channel;
    uint8_t object_type;
    uint8_t rfa;
    uint8_t vbr;
} sdk_aac_cfg_stru;

/* Stream config */
typedef struct {
    uint16_t payload;
    uint8_t code_c; // BTSDK_AVDTP_CODEC_TYPE_SBC or other value
    union {
        sdk_sbc_cfg_stru sbc;
        sdk_aac_cfg_stru aac;
    } info;
} sdka2dp_stream_cfg_stru;

/* Stream error */
typedef struct {
    void *stream_handle;
    uint8_t role;
    uint8_t event;
    uint8_t err;
} sdka2dp_stream_err_stru;

/* Stream mtu & frame config */
typedef struct {
    uint16_t stream_mtu;
    uint16_t frame_size;
    uint8_t num_frame;
} sdka2dp_stream_mtu_stru;

struct btsdk_a2dp_reg_cbk_stru {
    uint8_t media_type;
    void *cbk;
};

struct btsdk_a2dp_service_cap_stru {
    uint8_t len;                  /* Service Capability Length */
    uint8_t service_capability[1]; /* Service Category(1), LOSC(1), Data(x), This is for SEP GetCapability */
};

/* remote sep capacities */
typedef struct {
    void *rmt_hdl;
    uint8_t seid;
    struct btsdk_a2dp_service_cap_stru sc;
} sdka2dp_rmt_sep_cap_stru;

struct btsdk_a2dp_set_config_stru {
    void *stream_handle;
    uint16_t media_mtu;
    uint8_t bd[BTSDK_BDADDR_LEN];
    uint8_t media_type; /* Media_type(4bits)+TSEP(1bit)+RFA(3bits) */
    struct btsdk_a2dp_service_cap_stru sc;
};

struct btsdk_a2dp_open_ind_stru {
    void *stream_handle;
    uint16_t media_mtu;
    uint16_t cid;
    uint8_t initiator;
    uint8_t media_type;
    uint8_t remote_bd[BTSDK_BDADDR_LEN];
};

struct btsdk_a2dp_start_suspend_ind_stru {
    void *stream_handle;
    uint8_t mask;
};

struct btsdk_a2dp_stream_done_stru {
    void *stream_handle;
    uint8_t mask;
    uint8_t reason;
};

struct btsdk_a2dp_sep_stru {
    uint8_t seid;                        /* 6bits, seid of this SEP element */
    uint8_t mask;                        /* e.g: INUSE */
    /* Media_type(4bits)+TSEP(1bit, SNK=1)+RFA(3bits), This is for SEP Discovery */
    uint8_t media_type;
    struct btsdk_a2dp_service_cap_stru sc; /* Service Capability */
};

struct btsdk_a2dp_codec_caps_stru {
    uint8_t codec_type;                           /* Codec type */
    uint8_t codec_priority;                       /* the priority of the Codec */
    uint8_t codec_caps[BTSDK_A2DP_CODECCAPS_LEN]; /* indicates the Codec information, be configured such as struct
                                                    btsdk_a2dp_sbc_user_info_stru, depend on codec_type */
};

typedef struct {
    uint32_t size;
    uint16_t mask;
    uint16_t dev_type;        /* the local device type, can be PLAYER or HEADPHONE */
    uint16_t trans_mask;      /* transport service mask */
    uint16_t content_protect; /* the content protection type RFD */
    uint8_t sep_type;         /* SRC or SNK */
    uint8_t codec_num;
    uint8_t audio_card[BTSDK_A2DP_AUDIOCARD_NAME_LEN]; /* the audio card name used */
    struct btsdk_a2dp_codec_caps_stru codec[1];
} bt_sdk_locala2dp_server_attr_stru, *p_bt_sdk_locala2dp_server_attr_stru;

/* Structure for the local_attrib of A2DP_SRC(SNK)ClntStart A2DP_SRC(SNK)SvrStart */
struct btsdk_a2dp_local_attrib_stru {
    uint16_t dev_type;                 /* The local device type, can be PLAYER or HEADPHONE */
    uint16_t trans_mask;               /* Transport service mask */
    uint8_t sep_type;                  /* SRC or SNK */
    uint8_t local_seid;                /* For Dynamic Regisiter SEP */
    uint8_t codec_num;
    uint8_t rfa;
    /* The audio card name used, 0x80 means max length of audiocard name
    */
    uint8_t audio_card[BTSDK_A2DP_AUDIOCARD_NAME_LEN];
    struct btsdk_a2dp_codec_caps_stru codec[1];
};

/* Possible flags for member 'mask' in _BtSdkLocalA2DPServerAttrStru */
#define BTSDK_LA2DPSAM_DEVTYPE 0x0001
#define BTSDK_LA2DPSAM_CONTENTPROTECT 0x0002
#define BTSDK_LA2DPSAM_SEPTYPE 0x0004
#define BTSDK_LA2DPSAM_CODEC 0x0008
#define BTSDK_LA2DPSAM_AUDIOCARD 0x0010
#define BTSDK_LA2DPSAM_ALL 0x001f

struct btsdk_a2dp_mpeg24user_info_stru { /* the structure of MPEG4 AAC ISO 14496-3 codec_caps */
    uint16_t sample_frequency;
    uint8_t object_type;
    uint8_t channels;
    uint32_t bit_rate;
    uint8_t rfa;
    uint8_t vbr;
};

struct btsdk_a2dp_sbc_user_info_stru { /* the structure of SBC codec_caps */
    uint8_t chnl_mode;
    uint8_t sample_frequency;
    uint8_t alloc_method;
    uint8_t subband;
    uint8_t block_length;
    uint8_t min_bitpool;
    uint8_t max_bitpool;
};

struct btsdk_a2dp_sbc_configed_caps_info_stru { /* LITTLE_ENDIAN */
    uint8_t chnl_mode;
    uint8_t sample_frequency;
    uint8_t alloc_method;
    uint8_t subband;
    uint8_t block_length;
    uint8_t min_bitpool;
    uint8_t max_bitpool;
};

struct btsdk_a2dp_mpeg12configed_caps_info_stru {
    uint8_t chnl_mode;
    uint8_t sample_frequency;
    uint8_t bitrate_idx;
    uint8_t layer;
    uint8_t crc;
    uint8_t mpf;
    uint8_t vbr;
    uint8_t rfa;
    uint8_t padding;
};

typedef struct {
    uint32_t stream_hdl;
    uint8_t state;
    uint8_t src;
    uint16_t len;
    uint8_t param[0];
} a2dp_src_stream_chg_params;

typedef struct {
    uint32_t dev_hdl;
    uint8_t addr[BTSDK_BDADDR_LEN];
} remote_device_s;

typedef struct {
    void *stream_handle;
    uint8_t src;
    uint8_t res[3];
    sdka2dp_stream_cfg_stru stream_cfg;
} a2dp_stream_info_s;

typedef void (*btsdk_a2dp_vaudio_event_func)(uint16_t event, uint32_t handle, uint8_t *ev_param);
typedef void (*btsdk_a2dp_rmt_sep_cap_cbk)(sdka2dp_rmt_sep_cap_stru *cap);
typedef void (*btsdk_a2dp_stream_state_chg_cbk)(void *stream_hdl, uint8_t state, uint16_t len, uint8_t *param);
typedef void (*btsdk_a2dp_stream_cfg_chg_cbk)(void *stream_hdl, sdka2dp_stream_cfg_stru *config);
typedef void (*btsdk_a2dp_stream_reject_cbk)(sdka2dp_stream_err_stru *err);

typedef struct {
    btsdk_a2dp_rmt_sep_cap_cbk rmt_sep_cap_cbk;
    btsdk_a2dp_stream_state_chg_cbk stream_state_chg_cbk;
    btsdk_a2dp_stream_cfg_chg_cbk stream_cfg_chg_cbk;
    btsdk_a2dp_stream_reject_cbk stream_reject_cbk;
} btsdka2dp_cbk;

int32_t btsdk_a2dp_set_audio_shmid(uint32_t stream_hdl, bool enable, uint32_t share_mem_id);
int32_t btsdk_a2dp_stream_start(void *stream_hdl);
int32_t btsdk_a2dp_stream_suspend(void *stream_hdl);
int32_t a2dp_src_set_stream_codec_req(bool reconfig, const void *stream_hdl, const void *cfg, uint32_t size);
int32_t btsdk_registera2dpsnk_service(uint16_t len, const uint8_t *audio_card);
int32_t btsdk_unregistera2dpsnk_service(void);
void btsdk_a2dp_reg_cbk(const btsdka2dp_cbk *a2dp_cbk);
void btsdk_a2dp_snk_reg_cbk(const btsdka2dp_cbk *a2dp_cbk);
int32_t btsdk_a2dp_stream_start_accept(void *stream_hdl);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_AV_IF_H */
