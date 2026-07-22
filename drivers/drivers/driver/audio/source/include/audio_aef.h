/*
 * Copyright (c) CompanyNameMagicTag 2012-2020. All rights reserved.
 * Description: define audio common data structure
 * Author: AuthorNameMagicTag
 * Create: 2012-12-22
 */

#ifndef __AUDIO_AEF_H__
#define __AUDIO_AEF_H__

#include "osal_list.h"
#include "audio_struct.h"
#include "sws_aef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* Audio effect type */
/* CNcomment: 音效类型枚举 */
typedef enum {
    /* invalid audio effect */
    /* CNcomment: 无效音效 */
    AEF_TYPE_NULL = 0x000,

    /* Dolby audio effect */
    /* CNcomment: Dolby音效 */
    AEF_TYPE_DOLBY = 0x001,

    /* SRS audio effect */
    /* CNcomment: SRS音效 */
    AEF_TYPE_SRS3D = 0x010,

    /* Customer audio effect */
    /* CNcomment: 客户自研音效 */
    AEF_TYPE_CUSTOMER = 0x020,

    /* Customer A audio effect */
    /* CNcomment: 客户A的自研音效 */
    AEF_TYPE_CUSTOMER_A = 0x021,

    /* LP audio effect */
    /* CNcomment: LP音效 */
    AEF_TYPE_LP = 0x100,

    /* DBX audio effect */
    /* CNcomment: DBX音效 */
    AEF_TYPE_DBX = 0x110,

    /* DBX2 audio effect */
    /* CNcomment: DBX2音效 */
    AEF_TYPE_DBX2 = 0x111,

    /* SPECTRUM */
    /* CNcomment: SPECTRUM组件 */
    AEF_TYPE_SPECTRUM = 0x120,

    /* SWS3D V1.0 audio effect */
    /* CNcomment: SWS3D V1.0音效 */
    AEF_TYPE_SWS1 = 0x130,

    /* SWS3D V2.0 audio effect */
    /* CNcomment: SWS3D V2.0音效 */
    AEF_TYPE_SWS2 = 0x131,

    /* SWS3D V3.0 audio effect */
    /* CNcomment: SWS3D V3.0音 */
    AEF_TYPE_SWS3 = 0x132,

    /* KDE audio effect */
    /* CNcomment: 卡拉OK音效 */
    AEF_TYPE_KDE = 0x150,

    /* SOOVEN audio effect */
    /* CNcomment: SOOVEN音效 */
    AEF_TYPE_SOOVEN = 0x160,

    /* SOOVEN2 audio effect */
    /* CNcomment: SOOVEN2音效 */
    AEF_TYPE_SOOVEN2 = 0x161,

    /* SOOVEN3 audio effect */
    /* CNcomment: SOOVEN3音效 */
    AEF_TYPE_SOOVEN3 = 0x162,

    /* global audio effect */
    /* CNcomment: 全局音效 */
    SND_AEF_TYPE_GLOBAL = 0x170,

    /* supplemental1 audio effect */
    /* CNcomment: 补充1音效，补充全局音效的单一特性§ */
    SND_AEF_TYPE_SUPPLEMENTAL1 = 0x171,

    /* supplemental2 audio effect */
    /* CNcomment: 补充2音效，补充全局音效的单一特性，Reserved */
    SND_AEF_TYPE_SUPPLEMENTAL2 = 0x172,

    /* SMARTPA audio effect */
    /* CNcomment: SMARTPA音效 */
    AEF_TYPE_SMARTPA = 0x180,

	/* SAID audio effect */
    /* CNcomment: 助眠音效 */
    AEF_TYPE_SAID = 0x190,
} aef_type;

/* Configuration parameters required by an audio effect for creating devices. */
/* CNcomment: 音效创建设备的配置参数 */
typedef struct {
    /* [IN]pointer to private data */
    /* CNcomment:[IN]指向私有配置结构体的指针 */
    td_void *private_data;

    /* [IN]size of the private data. */
    /* CNcomment:[IN]私有结构体大小 */
    td_u32 private_data_size;
} aef_param;

typedef enum {
    AEF_CMD_ALG_PARAM = 0x0000,
    AEF_CMD_CALIBRATION_STATE = 0x0001,
    AEF_CMD_CALI_RE = 0x0002,
    AEF_CMD_CALI_F0 = 0x0003,
    AEF_CMD_NOISE_STATE = 0x0004,
} aef_param_cmd;

typedef union {
    audio_sws_mobile_para sws_cfg;
    audio_pcm_format aw_cfg;
    audio_pcm_format haid_cfg;
} effect_cfg;

typedef td_void *aef_handle;
typedef struct {
    struct osal_list_head node;
    const td_char *name;
    aef_type type;
    td_u32 version;

    td_s32 (*get_default_open_param)(td_void *open_param, td_u32 param_size);
    td_s32 (*create)(const td_void *aef_attr, td_void **aef);
    td_s32 (*destroy)(td_void *aef);
    td_s32 (*set_config)(td_void *aef, td_u32 cmd, const td_void *cfg);
    td_s32 (*get_config)(td_void *aef, td_u32 cmd, td_void *cfg);
    td_s32 (*set_parameter)(td_void *aef, td_u32 cmd, const td_void *parameter, td_u32 param_size);
    td_s32 (*get_parameter)(td_void *aef, td_u32 cmd, td_void *parameter, td_u32 param_size);

    td_s32 (*set_enable)(td_void *aef, td_bool enable);
    td_s32 (*get_enable)(td_void *aef, td_bool *enable);
    td_s32 (*get_max_pcm_in_size)(td_void *aef, td_u32 *in_size);
    td_s32 (*get_max_pcm_out_size)(td_void *aef, td_u32 *out_size);
    td_s32 (*get_input_pcm_attr)(td_void *aef, audio_pcm_format *pcm_attr);
    td_s32 (*proc_frame)(td_void *aef, audio_frame *in_frame, audio_frame *out_frame);
} aef_component;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __AUDIO_AEF_H__ */
