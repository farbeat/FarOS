/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for Speech Enhancement & AI
 * Author: audio
 */

#ifndef __AUDIO_SEA_STRUCT_H__
#define __AUDIO_SEA_STRUCT_H__

#include "td_type.h"
#include "audio_sea.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    SEA_STATE_IDLE = 0,  /* IDLE: no voice detected */
    SEA_STATE_WAITING,   /* WAITINE: waiting for voice spot (keyword) */
    SEA_STATE_LISTENING, /* LISTENING: listening for voice directive */
    SEA_STATE_THINKING,  /* UNDERSTANDING: understanding voice directive */
    SEA_STATE_MAX
} sea_state_type;

typedef enum {
    SEA_AFE_OPT_NULL = 0x0000,
    SEA_AFE_OPT_ANR  = 0x0001, /* Automatic Noise Restraint */
    SEA_AFE_OPT_AEC  = 0x0002, /* Acoustic Echo Cancellation */
    SEA_AFE_OPT_AGC  = 0x0004, /* Automatic Gain Control */
    SEA_AFE_OPT_WNR  = 0x0008, /* Wind Noise Reduction */
    SEA_AFE_OPT_BFM  = 0x0010, /* Beamforming */
    SEA_AFE_OPT_BSS  = 0x0020, /* Blind Source Separation */
    SEA_AFE_OPT_DRB  = 0x0040, /* De-Reverb */
    SEA_AFE_OPT_LMT  = 0x0080, /* Limiter */
    SEA_AFE_OPT_EQ   = 0x0100, /* Effect Quality */
    SEA_AFE_OPT_DRC  = 0x0200, /* Dynamic Range Control */
    SEA_AFE_OPT_ALL  = 0xFFFF,
} sea_afe_option;

typedef enum {
    SEA_AFE_AGC_DEF = 0,
    SEA_AFE_AGC_RX = SEA_AFE_AGC_DEF,
    SEA_AFE_AGC_TX,
    SEA_AFE_AGC_MAX,
} sea_afe_agc_type;

typedef enum {
    SEA_AFE_AEC_DEF = 0,
    SEA_AFE_AEC_MONO = SEA_AFE_AEC_DEF,
    SEA_AFE_AEC_STEREO,
    SEA_AFE_AEC_MULTI_CHANNEL,
    SEA_AFE_AEC_AI,
    SEA_AFE_AEC_MAX,
} sea_afe_aec_type;

typedef enum {
    SEA_AFE_AEC_REF_DEF = 0,
    SEA_AFE_AEC_REF_TX_DAC = SEA_AFE_AEC_REF_DEF,
    SEA_AFE_AEC_REF_TX_HDMI,
    SEA_AFE_AEC_REF_ADAPTIVE,
    SEA_AFE_AEC_REF_MAX,
} sea_afe_aec_ref;

typedef enum {
    SEA_AFE_ANR_DEF = 0,
    SEA_AFE_ANR_SCNR = SEA_AFE_ANR_DEF,
    SEA_AFE_ANR_DMNR,
    SEA_AFE_ANR_MAX,
} sea_afe_anr_type;

typedef enum {
    SEA_AFE_BFM_DEF = 0,
    SEA_AFE_BFM_FIX = SEA_AFE_BFM_DEF,
    SEA_AFE_BFM_ADAPTIVE,
    SEA_AFE_BFM_MAX,
} sea_afe_bfm_type;

typedef enum {
    SEA_AFE_BSS_DEF = 0,
    SEA_AFE_BSS_SINGLE = SEA_AFE_BSS_DEF,
    SEA_AFE_BSS_DOUBLE,
    SEA_AFE_BSS_MULTIPLE,
    SEA_AFE_BSS_MAX,
} sea_afe_bss_type;

typedef enum {
    SEA_AFE_DRB_DEF = 0,
    SEA_AFE_DRB_AEC = SEA_AFE_DRB_DEF,
    SEA_AFE_DRB_AI,
    SEA_AFE_DRB_MAX,
} sea_afe_drb_type;

typedef enum {
    SEA_ASR_DM_NULL = 0x0000,
    SEA_ASR_DM_CHAT = 0x0001,       /* Chat domain */
    SEA_ASR_DM_IOT = 0x0002,        /* IoT domain */
    SEA_ASR_DM_APP = 0x0004,        /* Application domain */
    SEA_ASR_DM_MUSIC = 0x0008,      /* Music domain(request by artist, song, album and genre) */
    SEA_ASR_DM_MOVIE = 0x0010,      /* Movie domain(request by artist, movie, tv series, category) */
    SEA_ASR_DM_FOOD = 0x0020,       /* Food domain */
    SEA_ASR_DM_SPORT = 0x0040,      /* Sport&Health domain */
    SEA_ASR_DM_CONTACT = 0x0080,    /* Contact domain */
    SEA_ASR_DM_WEATHER = 0x0100,    /* Weather domain */
    SEA_ASR_DM_CLOTHING = 0x0200,   /* Clothing domain */
    SEA_ASR_DM_NAVIGATION = 0x0400, /* Navigation domain */
    SEA_ASR_DM_MEDICAL = 0x0800,    /* Medical domain */
    SEA_ASR_DM_ALL = 0xFFFF
} sea_asr_domain;

typedef struct {
    td_s16 gain_db;
    td_s16 gain_inequality;
} sea_afe_pga_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_agc_type */
    td_s8 reserved[2];
} sea_afe_agc_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_aec_type */
    td_s8 ref_type;     /* ::sea_afe_aec_ref */
    td_u8 latency;      /* Echo tail length, Unit: 10ms */
} sea_afe_aec_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_anr_type */
    td_s8 reserved[2];
} sea_afe_anr_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_bfm_type */
    td_u8 beam_num;     /* Beam number to be devided, Range: [1~24], recommend 12 */
    td_u8 out_chan_num; /* DOA output beam number, Range: [1~6], recommend 3 */
} sea_afe_bfm_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_bss_type */
    td_s8 out_src_num;  /* Source count to be seperated */
    td_s8 reserved[1];
} sea_afe_bss_cfg;

typedef struct {
    td_s8 enable;       /* TD_TRUE: enable, TD_FALSE: disable */
    td_s8 type;         /* ::sea_afe_drb_type */
    td_u8 latency_ms;   /* Reverberation latency */
    td_s8 reserved[1];
} sea_afe_drb_cfg;

/* Define configure of ::SEA_AFE_VQE */
typedef struct {
    td_u32 options;       /* ::sea_afe_option */
} sea_afe_vqe_attr;

/* Define configure of ::SEA_AFE_SEE */
typedef struct {
    td_u32 options;   /* ::sea_afe_option */
    sea_afe_aec_cfg aec;  /* ::SEA_AFE_OPT_AEC */
    sea_afe_anr_cfg anr;  /* ::SEA_AFE_OPT_ANR */
    sea_afe_agc_cfg agc;  /* ::SEA_AFE_OPT_AGC */
    sea_afe_bfm_cfg bfm;  /* ::SEA_AFE_OPT_BFM */
    sea_afe_bss_cfg bss;  /* ::SEA_AFE_OPT_BSS */
    sea_afe_drb_cfg drb;  /* ::SEA_AFE_OPT_DRB */
} sea_afe_see_attr;

/* Define configure of ::SEA_AFE_AHE */
typedef struct {
    td_s32 volume;
    td_u32 options;   /* ::sea_afe_option */
    sea_afe_anr_cfg anr;  /* ::SEA_AFE_OPT_ANR */
    sea_afe_agc_cfg agc;  /* ::SEA_AFE_OPT_AGC */
    sea_afe_bfm_cfg bfm;  /* ::SEA_AFE_OPT_BFM */
    sea_afe_drb_cfg drb;  /* ::SEA_AFE_OPT_DRB */
} sea_afe_ahe_attr;

/* Define configure of ::SEA_AAI_KWS */
typedef struct {
    td_u16 sensitivity;   /* Unit: %, Rang: 0~100, default 90 */
    td_u16 max_dura_ms;   /* Unit: ms, Rang: 2000~4000, default 2000 */
    td_u16 vad_bgn_thres; /* Unit: ms, Rang: 10~200, default 40 */
    td_u16 vad_end_thres; /* Unit: ms, Rang: 400~1000, default 500 */
    td_char keyword[SEA_INFO_STR_SIZE];
} sea_aai_kws_attr;

/* Define configure of ::SEA_AAI_ASR */
typedef struct {
    td_u16 nlp_enable;    /* NLP switch */
    td_u16 max_dura_ms;   /* Unit: ms, Rang: 4000~60000, default 10000 */
    td_u16 vad_bgn_thres; /* Unit: ms, Rang: 10~200, default 40 */
    td_u16 vad_end_thres; /* Unit: ms, Rang: 400~1000, default 500 */
    td_u32 word_number;   /* Number of words */
    td_u32 domains;       /* ::sea_asr_domain */
} sea_aai_asr_attr;

/* Define configure of ::SEA_AAI_VID */
typedef struct {
    td_u16 sensitivity;   /* Unit: %, Rang: 0~100, default 90 */
    td_u16 min_dura_ms;   /* Unit: ms, Rang: 100~1000, default 500 */
    td_u32 vid_count;     /* Voice ID count */
} sea_aai_vid_attr;

/* Define configure of ::SEA_AAI_ASD */
typedef struct {
    td_u32 latency; /* Unit: ms, Rang: 10~2000, default 100 */
} sea_aai_asd_attr;

/* Define attributes of SEA_AFE/AAI instance */
typedef struct {
    td_u8 type; /* ::sea_afe_type or ::sea_aai_type */
    sea_work_mode mode;
    union {
        sea_afe_see_attr see; /* ::SEA_AFE_SEE */
        sea_afe_vqe_attr vqe; /* ::SEA_AFE_VQE */
        sea_afe_ahe_attr ahe; /* ::SEA_AFE_AHE */
        sea_aai_kws_attr kws; /* ::SEA_AAI_KWS */
        sea_aai_asr_attr asr; /* ::SEA_AAI_ASR */
        sea_aai_vid_attr vid; /* ::SEA_AAI_VID */
        sea_aai_asd_attr asd; /* ::SEA_AAI_ASD */
    } u;
} sea_eng_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __AUDIO_SEA_STRUCT_H__ */
