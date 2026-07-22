/*
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: header file for SEA(Speech Enhancement and AI) component
 * Author: audio
 */

#ifndef __AUDIO_SEA_H__
#define __AUDIO_SEA_H__

#include <stdlib.h>
#include <string.h>
#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SEA_ENG_SEL_MAX_NUM  8
#define SEA_INFO_STR_SIZE  32
#define SEA_ITEM_QUERY_CNT 20
#define SEA_ECH_MAX_NUM    4
#define SEA_ECH_AFE_IND    0
#define SEA_ECH_AAI_IND    1


/* Algorithm library type */
typedef enum {
    SEA_LIB_ID_NULL = 0x00,
    SEA_LIB_ID_SEE,
    SEA_LIB_ID_VQE,
    SEA_LIB_ID_AHE,
    SEA_LIB_ID_KWS,
    SEA_LIB_ID_ASR,
    SEA_LIB_ID_VID,
    SEA_LIB_ID_ASD,
    SEA_LIB_ID_HAID,
    SEA_LIB_ID_EXT0,
    SEA_LIB_ID_EXT1,
    SEA_LIB_ID_EXT2,
    SEA_LIB_ID_EXT3,
    SEA_LIB_ID_MAX
} sea_lib_id;

/* AAI(Audio Artificial Intelligence) type */
typedef enum {
    SEA_AAI_NULL = 0x00,
    SEA_AAI_KWS,  /* Keyword Spotting */
    SEA_AAI_ASR,  /* Automatic Speech Recognition */
    SEA_AAI_VID,  /* Voice ID */
    SEA_AAI_ASD,  /* Ambient Sounds Detection */
    SEA_AAI_MAX
} sea_aai_type;

/* AFE(Audio Front-End) type */
typedef enum {
    SEA_AFE_NULL = 0x80,
    SEA_AFE_SEE,  /* Speech Enhancement Engine */
    SEA_AFE_VQE,  /* Voice Quality Engine */
    SEA_AFE_AHE,  /* Augment Hearing Engine */
    SEA_AFE_MAX
} sea_afe_type;

/* SEA event type reported by ::uapi_sea_event_proc */
typedef enum {
    SEA_EVENT_VAD_BEGIN = 0,
    SEA_EVENT_VAD_END,
    SEA_EVENT_STATE_CHANGED,  /* Event param ::sea_state_type */
    SEA_EVENT_KWS_MATCH,      /* Event param ::sea_kws_event_param */
    SEA_EVENT_ASR_MATCH,      /* Event param ::sea_asr_event_param */
    SEA_EVENT_VID_MATCH,      /* Event param ::sea_vid_event_param */
    SEA_EVENT_VID_ENROLL_RET, /* Event param ::sea_vid_enroll_result */
    SEA_EVENT_MAX
} sea_event_type;

typedef enum {
    SEA_NORM_MODE = 0, /* Normal Mode */
    SEA_LP_MODE,       /* Low Power mode */
    SEA_HP_MODE,       /* High Performance mode */
    SEA_MODE_MAX
} sea_work_mode;

/* SEA output track for AI engine or APP */
typedef enum {
    SEA_OUTPUT_REC_SRC = 0, /* Output source for recording */
    SEA_OUTPUT_ASR_SRC,     /* Output source for Voice Kit APP */
    SEA_OUTPUT_VC_SRC,      /* Output source for Voice Call APP */
    SEA_OUTPUT_AH_SRC,      /* Output source for AH output */
    SEA_OUTPUT_KWS_SRC,     /* Output source for Keyword Engine */
    SEA_OUTPUT_MAX
} sea_output_type;

typedef enum {
    SEA_ITEM_PHRASE = 0,
    SEA_ITEM_VOICE_ID,
    SEA_ITEM_MAX
} sea_item_type;

/* defines audio format */
typedef struct {
    td_u32 channels;    /* Number of channels */
    td_u32 bit_depth;   /* ::audio_bit_depth */
    td_u32 sample_rate; /* ::audio_sample_rate */
    td_u32 samples_per_frame;
} sea_pcm_format;

typedef struct {
    td_u8 type;     /* ::sea_eng_type */
    td_u8 lib_id;   /* ::sea_lib_id */
} sea_eng_ind;

typedef union {
    sea_eng_ind eng_ind[SEA_ECH_MAX_NUM];
    td_u16 eng_id[SEA_ECH_MAX_NUM];
} sea_eng_sel;

typedef struct {
    sea_eng_sel eng_sel[SEA_ENG_SEL_MAX_NUM];
    td_u32 eng_sel_cnt;
} sea_eng_caps;

/* Define attributes of SEA instance */
typedef struct {
    td_u32 buf_dur_ms;
    sea_pcm_format in_pcm;
    sea_pcm_format ref_pcm;
    sea_pcm_format out_pcm[SEA_OUTPUT_MAX];
} sea_pcm_attr;

/* defines the audio frame. */
typedef struct {
    td_s32 *data;        /* Buffer for storing PCM data */
    td_u32 size;         /* Data size */
    td_u32 channels;     /* Number of Channels */
    td_u32 bit_depth;    /* Bit Depth */
    td_u32 sample_rate;  /* Sampling Rate */
    td_u32 samples;      /* number of sampling points of the PCM data */
    td_bool interleaved; /* whether the data is interleaved */
    sea_output_type type;
} sea_pcm_frame;

/* SEA Event param for ::SEA_EVENT_KWS_MATCH */
typedef struct {
    td_u32 match_score;
    td_u32 start_offset;
    td_u32 end_offset;
    td_u32 target_angle;
    td_u32 phrase_id;
} sea_kws_event_param;

/* SEA Event param for ::SEA_EVENT_ASR_MATCH */
typedef struct {
    td_u32 skill_id;
    td_u32 voice_id;        /* validate when VID enable */
    td_u8 *data_buf;
    td_u32 data_size;
} sea_asr_event_param;

/* SEA Event param for ::SEA_EVENT_ASR_MATCH */
typedef struct {
    td_u32 voice_id;
} sea_vid_event_param;

/* SEA Event param for ::SEA_EVENT_VID_ENROLL_RET */
typedef struct {
    td_u32 result;   /* Result : if ::AUDIO_SUCCESS, call ::afe_add_vid_entity */
    td_u32 voice_id; /* If result is AUDIO_SUCCESS, voice_id is validate */
} sea_vid_enroll_result;

/* Define union of SEA event param */
typedef union {
    sea_kws_event_param kws;
    sea_asr_event_param asr;
    sea_vid_event_param vid;
} sea_event_param;

typedef struct {
    td_s32 id;
    td_u8 str[SEA_INFO_STR_SIZE];
} sea_info_item;

typedef struct {
    td_u32 index;
    td_u32 total_count;
    td_u32 output_count;
    sea_info_item items[SEA_ITEM_QUERY_CNT];
} sea_item_sets;

typedef struct {
    td_void *addr;
    td_u32 size;
    td_u32 used;
    td_u32 samples;
} sea_frame_buf;

/* Definition of the lib version. */
/* CNcomment: lib 版本定义 */
typedef union {
    struct {
        td_u8 version_major; /* Major version. */ /* CNcomment:主版本号 */
        td_u8 version_minor; /* Minor version. */ /* CNcomment:副版本号 */
        td_u8 revision; /* Revision version. */   /* CNcomment:修订版本 */
        td_u8 step; /* Step version. */           /* CNcomment:阶段性版本 */
    } bits;
    td_u32 u32;
} sea_lib_version;

typedef td_void *sea_handle;
typedef td_s32 (*sea_event_report)(td_void *handle, td_s32 event, td_void *param, td_void *context);
typedef td_s32 (*sea_data_output)(td_void *handle, sea_pcm_frame *frame, td_void *context);

typedef struct sea_entry {
    /* Input, description information about SEA . */
    /* CNcomment:IN SEA描述名字 */
    const td_char *name;

    /* encoder ID. Note: This ID is the identifier of a SEA, and must be unique. */
    /* CNcomment:IN SEA标识，注意: 此标识作为SEA唯一身份标识，必须唯一而不能有重复 */
    const td_u32 sea_id; /* sea_lib_id */

    /* Input, SEA version. */
    /* CNcomment:IN SEA版本描述 */
    const sea_lib_version version;

    /* Input, Description information of the SEA. */
    /* CNcomment:IN SEA详细描述信息 */
    const td_char *description;

    /* OUT pointer to next SEA.manager by client.
    client:The layer of software that invokes the methods of the SEA. */
    /* CNcomment:OUT 下个SEA结构体指针，由客户管理 */
    struct sea_entry *next;

    /* Output, pointer to the dll symbol of an SEA.
    This member variable is maintained by the client that calls the  SEA Therefore,
    the developers of the SEA can ignore this member variable. */
    /* CNcomment:OUT 该成员变量由调用SEA的客户端维护, SEA开发者可以忽略该成员 */
    td_void *module;

    td_s32 (*create)(sea_handle *handle, const sea_eng_sel *eng_sel, const sea_pcm_attr *attr);

    td_s32 (*destroy)(sea_handle handle);

    td_s32 (*proc_frame)(sea_handle handle, sea_pcm_frame *frame);

    td_s32 (*set_enable)(sea_handle handle, td_bool enable);

    td_s32 (*set_scr_buf)(sea_handle handle, td_void *scr_buf, td_u32 scr_size);

    td_s32 (*get_eng_attr)(sea_handle handle, td_u8 type, td_void *eng_attr);

    td_s32 (*set_eng_attr)(sea_handle handle, td_u8 type, const td_void *eng_attr);

    td_s32 (*get_parameter)(sea_handle handle, td_u8 type, td_void *parameter);

    td_s32 (*set_parameter)(sea_handle handle, td_u8 type, const td_void *parameter);

    td_s32 (*set_input_attr)(sea_handle handle,
                             const sea_pcm_format *in_pcm_attr,
                             const sea_pcm_format *ref_pcm_attr);

    td_s32 (*set_output_attr)(sea_handle handle,
                              sea_output_type type,
                              const sea_pcm_format *out_pcm_attr);

    td_s32 (*get_eng_caps)(sea_eng_sel *eng_sel);

    td_s32 (*get_def_pcm_attr)(sea_pcm_attr *attr);

    td_s32 (*get_item_sets)(sea_handle handle, sea_item_type type, td_void *item_sets);

    td_s32 (*reg_data_output)(sea_handle handle, sea_data_output data_output, td_void *context);

    td_s32 (*reg_event_report)(sea_handle handle, sea_event_report event_report, td_void *context);
} sea_entry;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __AUDIO_SEA_H__ */
