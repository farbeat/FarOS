/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: define the sap drv ioctl
 * Author: audio
 * Create: 2020-01-02
 */

#ifndef __DRV_SAP_IOCTL_H__
#define __DRV_SAP_IOCTL_H__

#include "ao_struct.h"
#include "ai_struct.h"
#include "adp_struct.h"
#include "vad_struct.h"
#include "sea_struct.h"
#include "anc_struct.h"
#include "dpm_struct.h"
#include "haid_struct.h"
#include "adec_struct.h"
#include "aenc_struct.h"
#include "drv_audio_dev.h"
#include "drv_audio_osal_ioctl.h"
#include "cmsis_os.h"
#include "drv_sap_ioctl_cmd.h"

#if (SAP_OS_TYPE == os_type_linux)
#ifndef __KERNEL__
#include <sys/ioctl.h>
#endif /* __KERNEL__ */
#endif /* SAP_OS_TYPE */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SAP_AUDIO_LOCK_TIMEMOUT 3000

#define SAP_LIB_NAME_LEN 128

#define UMAP_MIN_MINOR_SAP UMAP_MIN_MINOR_AO
#define UMAP_MAX_MINOR_SAP UMAP_MAX_MINOR_ADP

#define DEV_ID_SAP_SYS UMAP_MIN_MINOR_SYS
#define DEV_ID_SAP_ADP UMAP_MIN_MINOR_ADP
#define DEV_ID_SAP_AI UMAP_MIN_MINOR_AI
#define DEV_ID_SAP_VAD UMAP_MIN_MINOR_VAD
#define DEV_ID_SAP_SEA UMAP_MIN_MINOR_SEA
#define DEV_ID_SAP_ADEC UMAP_MIN_MINOR_ADEC
#define DEV_ID_SAP_AENC UMAP_MIN_MINOR_AENC
#define DEV_ID_SAP_AO UMAP_MIN_MINOR_AO
#define DEV_ID_SAP_TRACK UMAP_MIN_MINOR_TRACK
#define DEV_ID_SAP_ANC UMAP_MIN_MINOR_ANC
#define DEV_ID_SAP_HAID UMAP_MIN_MINOR_HAID

typedef struct {
    td_handle h_mod;
} sap_drv_handle;

typedef struct {
    td_handle h_src;
    td_handle h_sink;
    audio_adp_type adp_type;
    td_u32 data_port_id;
} sap_drv_attach_para;

typedef struct {
    td_handle h_mod;
    td_bool enable;
} sap_drv_enable;

typedef struct {
    td_handle h_mod;
    td_u32 value;
} sap_drv_u32;

typedef struct {
    td_handle track;
    td_s64 pts; /* us */
} sap_track_pts_param;

typedef struct {
    td_handle h_mod;
    td_u64 context;
    td_u64 event_proc;
} sap_drv_event;

typedef struct {
    td_handle h_mod;
    td_u32 id;
    td_char name[SAP_LIB_NAME_LEN];
} sap_drv_load;

typedef struct {
    td_u32 id;
} sap_drv_id;

typedef struct {
    td_handle h_mod;
    td_bool enable;
    td_u32 init_reg;
    td_u32 core_cid;
    td_u32 drv_cid;
    td_s32 mem_type;    /* ::shrb_mem_type */
    td_s32 fd;
    td_s32 cache;       /* ::td_bool */
    td_u32 size;
    td_u64 phys_addr;
} sap_adp_init;

typedef struct {
    td_handle h_mod;
    audio_adp_type type;
    audio_adp_attr attr;
} sap_adp_open;

typedef struct {
    td_handle h_mod;
    audio_adp_attr attr;
} sap_adp_attr;

typedef struct {
    td_handle h_mod;
    audio_ai_port port;
    audio_ai_attr attr;
} sap_ai_open;

typedef struct {
    td_handle h_mod;
    audio_ai_attr attr;
} sap_ai_attr;

typedef struct {
    td_handle h_mod;
    ai_gain gain;
} sap_ai_gain;

typedef struct {
    td_handle h_mod;
    ai_cali_volume cali_volume;
} sap_ai_cali_volume;

typedef struct {
    td_handle h_mod;
    audio_vad_attr attr;
} sap_vad_attr;

typedef struct {
    td_handle h_mod;
    sea_eng_sel eng_sel;
    sea_pcm_attr attr;
} sap_sea_attr;

typedef struct {
    sea_lib_id lib_id;
    sea_eng_caps eng_caps;
} sap_sea_eng_caps;

typedef struct {
    td_handle h_mod;
    sea_eng_attr attr;
} sap_sea_eng_attr;

typedef struct {
    td_handle h_mod;
    sea_item_type type;
    td_u32 count;
} sap_sea_item_count;

typedef struct {
    td_handle h_mod;
    sea_item_type type;
    sea_item_sets sets;
} sap_sea_item_sets;

typedef struct {
    td_handle h_mod;
    sea_item_type type;
    sea_info_item item;
} sap_sea_item_node;

typedef struct {
    td_handle h_mod;
    td_s8 *config;
    td_u32 config_len;
} sap_haid_config;

typedef struct {
    td_handle h_mod;
    td_s8 *all_config_addr;
    td_u32 all_config_len;
} sap_haid_all_config;

typedef struct {
    td_handle h_mod;
    haid_attr attr;
} sap_haid_attr;

typedef struct {
    td_handle h_mod;
    haid_volume volume;
} sap_haid_volume;

typedef struct {
    td_handle h_mod;
    td_bool enable;
} sap_haid_enable;

typedef struct {
    td_handle h_mod;
    td_u64 context;
    td_u64 event_proc;
} sap_haid_event;

typedef struct {
    td_handle h_mod;
    haid_scene scene;
} sap_haid_scene;

typedef struct {
    td_handle h_mod;
    td_u32 frequence;
    haid_volume volume;
} sap_haid_sinus_tone_attr;

typedef struct {
    td_handle h_mod;
    audio_spk_cali cali_coefs;
} sap_spk_cali_coefs;

typedef struct {
    td_handle h_mod;
    haid_cali_volume cali_volume;
} sap_haid_cali_volume;

typedef struct {
    td_handle h_mod;
    ha_codec_id codec_id;
    adec_attr attr;
} sap_adec_open;

typedef struct {
    td_handle h_mod;
    adec_attr attr;
} sap_adec_attr;

typedef struct {
    td_handle h_mod;
    ha_codec_id codec_id;
    aenc_attr attr;
} sap_aenc_open;

typedef struct {
    td_handle h_mod;
    aenc_attr attr;
} sap_aenc_attr;

typedef struct {
    td_handle h_mod;
    aenc_param_type type;
    td_u8 param[AENC_PARAM_SIZE_MAX];
    td_u32 param_size;
} sap_aenc_param;

typedef struct {
    td_handle h_mod;
    td_handle h_snd;
    track_attr attr;
} sap_track_open;

typedef struct {
    td_handle h_mod;
    track_attr attr;
} sap_track_attr;

typedef struct {
    td_handle h_mod;
    audio_pcm_format format;
} sap_track_pcm_format;

typedef struct {
    td_handle h_mod;
    track_state state;
    audio_toggle_config toggle_config;
} sap_track_state;

typedef struct {
    td_handle h_mod;
    ao_snd_id snd_id;
    ao_attr attr;
} sap_ao_open;

typedef struct {
    td_handle h_mod;
    ao_gain gain;
    audio_toggle_config toggle_config;
} sap_ao_gain;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    td_bool enable;
} sap_ao_port_enable;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    ao_track_mode mode;
} sap_ao_port_mode;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    ao_gain gain;
    audio_toggle_config toggle_config;
} sap_ao_port_gain;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    td_handle h_output;
} sap_ao_port_output;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    td_u32 cmd;
    td_char param[AO_AEF_PARAM_SIZE_MAX];
    td_u32 param_size;
} sap_ao_port_aef_param;

typedef struct {
    td_handle h_mod;
    ao_aef_profile aef_profile;
} sap_ao_port_aef_profile;

typedef struct {
    td_handle h_mod;
    ao_port_id port;
    aef_type effect_type;
    td_bool enable;
} sap_ao_port_aef_enable;

typedef struct {
    td_handle h_mod;
    sea_afe_type type;
    td_char param[SEA_PARAM_SIZE_MAX];
    td_u32 param_size;
} sap_sea_param;

typedef struct {
    aha_mode h_mod;
    td_s8 *config;
    td_u32 config_len;
} sap_anc_config;

#define CMD_IOCTL_SYS_INIT _IO(DEV_ID_SAP_SYS, CMD_SYS_INIT)
#define CMD_IOCTL_SYS_DEINIT _IO(DEV_ID_SAP_SYS, CMD_SYS_DEINIT)
#define CMD_IOCTL_SYS_SET_STATE _IOWR(DEV_ID_SAP_SYS, CMD_SYS_SET_STATE, sap_drv_u32)
#define CMD_IOCTL_SYS_SET_DEBUG_CFG _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_DEBUG_CFG, audio_debug_cfg)
#define CMD_IOCTL_SYS_GET_DEBUG_CFG _IOR(DEV_ID_SAP_SYS, CMD_SYS_GET_DEBUG_CFG, audio_debug_cfg)
#define CMD_IOCTL_SYS_SET_PRODUCT_FORM _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_PRODUCT_FORM, td_u8)
#define CMD_IOCTL_SYS_SET_TWS_MONO_MODE _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_TWS_MONO_MODE, td_bool)
#define CMD_IOCTL_SYS_SET_TWS_MODE _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_TWS_MODE, td_u8)
#define CMD_IOCTL_SYS_SET_TWS_ROLE _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_TWS_ROLE, td_u8)
#define CMD_IOCTL_SYS_SET_DSP_LOG_LEVEL _IOW(DEV_ID_SAP_SYS, CMD_SYS_SET_DSP_LOG_LEVEL, td_u32)

#define CMD_IOCTL_ADP_INIT _IOWR(DEV_ID_SAP_ADP, CMD_ADP_INIT, sap_adp_init)
#define CMD_IOCTL_ADP_DEINIT _IO(DEV_ID_SAP_ADP, CMD_ADP_DEINIT)
#define CMD_IOCTL_ADP_GET_DEF_ATTR _IOWR(DEV_ID_SAP_ADP, CMD_ADP_GET_DEF_ATTR, sap_adp_open)
#define CMD_IOCTL_ADP_OPEN _IOWR(DEV_ID_SAP_ADP, CMD_ADP_OPEN, sap_adp_open)
#define CMD_IOCTL_ADP_CLOSE _IOWR(DEV_ID_SAP_ADP, CMD_ADP_CLOSE, sap_drv_handle)
#define CMD_IOCTL_ADP_ALLOC_SHRB _IOWR(DEV_ID_SAP_ADP, CMD_ADP_ALLOC_SHRB, sap_drv_attach_para)
#define CMD_IOCTL_ADP_FREE_SHRB _IOWR(DEV_ID_SAP_ADP, CMD_ADP_FREE_SHRB, sap_drv_handle)
#define CMD_IOCTL_ADP_MAP _IOWR(DEV_ID_SAP_ADP, CMD_ADP_MAP, sap_drv_handle)
#define CMD_IOCTL_ADP_UNMAP _IOWR(DEV_ID_SAP_ADP, CMD_ADP_UNMAP, sap_drv_handle)
#define CMD_IOCTL_ADP_GET_ATTR _IOWR(DEV_ID_SAP_ADP, CMD_ADP_GET_ATTR, sap_adp_attr)
#define CMD_IOCTL_ADP_SET_ATTR _IOWR(DEV_ID_SAP_ADP, CMD_ADP_SET_ATTR, sap_adp_attr)
#define CMD_IOCTL_ADP_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_ADP, CMD_ADP_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_ADP_DETACH_OUTPUT _IOWR(DEV_ID_SAP_ADP, CMD_ADP_DETACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_ADP_REG_EVENT_PROC _IOWR(DEV_ID_SAP_ADP, CMD_ADP_REG_EVENT_PROC, sap_drv_event)

#define CMD_IOCTL_AI_INIT _IO(DEV_ID_SAP_AI, CMD_AI_INIT)
#define CMD_IOCTL_AI_DEINIT _IO(DEV_ID_SAP_AI, CMD_AI_DEINIT)
#define CMD_IOCTL_AI_GET_DEF_ATTR _IOWR(DEV_ID_SAP_AI, CMD_AI_GET_DEF_ATTR, sap_ai_open)
#define CMD_IOCTL_AI_OPEN _IOWR(DEV_ID_SAP_AI, CMD_AI_OPEN, sap_ai_open)
#define CMD_IOCTL_AI_CLOSE _IOWR(DEV_ID_SAP_AI, CMD_AI_CLOSE, sap_drv_handle)
#define CMD_IOCTL_AI_REG_EVENT_PROC _IOWR(DEV_ID_SAP_AI, CMD_AI_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_AI_START _IOWR(DEV_ID_SAP_AI, CMD_AI_START, sap_drv_handle)
#define CMD_IOCTL_AI_STOP _IOWR(DEV_ID_SAP_AI, CMD_AI_STOP, sap_drv_handle)
#define CMD_IOCTL_AI_GET_ATTR _IOWR(DEV_ID_SAP_AI, CMD_AI_GET_ATTR, sap_ai_attr)
#define CMD_IOCTL_AI_SET_ATTR _IOWR(DEV_ID_SAP_AI, CMD_AI_SET_ATTR, sap_ai_attr)
#define CMD_IOCTL_AI_SET_BYPASS _IOWR(DEV_ID_SAP_AI, CMD_AI_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_AI_RESET_VAD _IOWR(DEV_ID_SAP_AI, CMD_AI_RESET_VAD, sap_drv_handle)
#define CMD_IOCTL_AI_GET_GAIN _IOWR(DEV_ID_SAP_AI, CMD_AI_GET_GAIN, sap_ai_gain)
#define CMD_IOCTL_AI_SET_GAIN _IOWR(DEV_ID_SAP_AI, CMD_AI_SET_GAIN, sap_ai_gain)
#define CMD_IOCTL_AI_GET_MUTE _IOWR(DEV_ID_SAP_AI, CMD_AI_GET_MUTE, sap_drv_enable)
#define CMD_IOCTL_AI_SET_MUTE _IOWR(DEV_ID_SAP_AI, CMD_AI_SET_MUTE, sap_drv_enable)
#define CMD_IOCTL_AI_SET_MIC_CALI_VOLUME _IOWR(DEV_ID_SAP_AI, CMD_AI_SET_MIC_CALI_VOLUME, sap_ai_cali_volume)
#define CMD_IOCTL_AI_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_AI, CMD_AI_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_AI_DETACH_OUTPUT _IOWR(DEV_ID_SAP_AI, CMD_AI_DETACH_OUTPUT, sap_drv_attach_para)

#define CMD_IOCTL_VAD_INIT _IO(DEV_ID_SAP_VAD, CMD_VAD_INIT)
#define CMD_IOCTL_VAD_DEINIT _IO(DEV_ID_SAP_VAD, CMD_VAD_DEINIT)
#define CMD_IOCTL_VAD_GET_DEF_ATTR _IOWR(DEV_ID_SAP_VAD, CMD_VAD_GET_DEF_ATTR, sap_vad_attr)
#define CMD_IOCTL_VAD_OPEN _IOWR(DEV_ID_SAP_VAD, CMD_VAD_OPEN, sap_vad_attr)
#define CMD_IOCTL_VAD_CLOSE _IOWR(DEV_ID_SAP_VAD, CMD_VAD_CLOSE, sap_drv_handle)
#define CMD_IOCTL_VAD_REG_EVENT_PROC _IOWR(DEV_ID_SAP_VAD, CMD_VAD_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_VAD_RESET _IOWR(DEV_ID_SAP_VAD, CMD_VAD_RESET, sap_drv_handle)

#define CMD_IOCTL_SEA_INIT _IO(DEV_ID_SAP_SEA, CMD_SEA_INIT)
#define CMD_IOCTL_SEA_DEINIT _IO(DEV_ID_SAP_SEA, CMD_SEA_DEINIT)
#define CMD_IOCTL_SEA_LOAD_ENG _IOWR(DEV_ID_SAP_SEA, CMD_SEA_LOAD, sap_drv_load)
#define CMD_IOCTL_SEA_UNLOAD_ENG _IOWR(DEV_ID_SAP_SEA, CMD_SEA_UNLOAD, sap_drv_load)
#define CMD_IOCTL_SEA_GET_ENG_CAPS _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_ENG_CAPS, sap_sea_eng_caps)
#define CMD_IOCTL_SEA_GET_DEF_ATTR _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_DEF_ATTR, sap_sea_attr)
#define CMD_IOCTL_SEA_CREATE _IOWR(DEV_ID_SAP_SEA, CMD_SEA_CREATE, sap_sea_attr)
#define CMD_IOCTL_SEA_DESTROY _IOWR(DEV_ID_SAP_SEA, CMD_SEA_DESTROY, sap_drv_handle)
#define CMD_IOCTL_SEA_REG_EVENT_PROC _IOWR(DEV_ID_SAP_SEA, CMD_SEA_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_SEA_START _IOWR(DEV_ID_SAP_SEA, CMD_SEA_START, sap_drv_handle)
#define CMD_IOCTL_SEA_STOP _IOWR(DEV_ID_SAP_SEA, CMD_SEA_STOP, sap_drv_handle)
#define CMD_IOCTL_SEA_GET_ENG_ATTR _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_ENG_ATTR, sap_sea_eng_attr)
#define CMD_IOCTL_SEA_SET_ENG_ATTR _IOWR(DEV_ID_SAP_SEA, CMD_SEA_SET_ENG_ATTR, sap_sea_eng_attr)
#define CMD_IOCTL_SEA_GET_PCM_ATTR _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_PCM_ATTR, sap_sea_attr)
#define CMD_IOCTL_SEA_SET_PCM_ATTR _IOWR(DEV_ID_SAP_SEA, CMD_SEA_SET_PCM_ATTR, sap_sea_attr)
#define CMD_IOCTL_SEA_GET_PARAM _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_PARAM, sap_sea_param)
#define CMD_IOCTL_SEA_SET_PARAM _IOWR(DEV_ID_SAP_SEA, CMD_SEA_SET_PARAM, sap_sea_param)
#define CMD_IOCTL_SEA_SET_BYPASS _IOWR(DEV_ID_SAP_SEA, CMD_SEA_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_SEA_GET_ITEM_COUNT _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_ITEM_COUNT, sap_sea_item_count)
#define CMD_IOCTL_SEA_GET_ITEM_SETS _IOWR(DEV_ID_SAP_SEA, CMD_SEA_GET_ITEM_SETS, sap_sea_item_sets)
#define CMD_IOCTL_SEA_ADD_ITEM _IOWR(DEV_ID_SAP_SEA, CMD_SEA_ADD_ITEM, sap_sea_item_node)
#define CMD_IOCTL_SEA_REMOVE_ITEM _IOWR(DEV_ID_SAP_SEA, CMD_SEA_REMOVE_ITEM, sap_sea_item_node)
#define CMD_IOCTL_SEA_ENABLE_VID_ENROLL _IOWR(DEV_ID_SAP_SEA, CMD_SEA_ENABLE_VID_ENROLL, sap_drv_enable)
#define CMD_IOCTL_SEA_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_SEA, CMD_SEA_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_SEA_DETACH_OUTPUT _IOWR(DEV_ID_SAP_SEA, CMD_SEA_DETACH_OUTPUT, sap_drv_attach_para)

#define CMD_IOCTL_ADEC_INIT _IO(DEV_ID_SAP_ADEC, CMD_ADEC_INIT)
#define CMD_IOCTL_ADEC_DEINIT _IO(DEV_ID_SAP_ADEC, CMD_ADEC_DEINIT)
#define CMD_IOCTL_ADEC_LOAD_CODEC _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_LOAD, sap_drv_load)
#define CMD_IOCTL_ADEC_UNLOAD_CODEC _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_UNLOAD, sap_drv_load)
#define CMD_IOCTL_ADEC_OPEN _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_OPEN, sap_drv_handle)
#define CMD_IOCTL_ADEC_CLOSE _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_CLOSE, sap_drv_handle)
#define CMD_IOCTL_ADEC_REG_EVENT_PROC _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_ADEC_START _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_START, sap_drv_handle)
#define CMD_IOCTL_ADEC_STOP _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_STOP, sap_drv_handle)
#define CMD_IOCTL_ADEC_GET_ATTR _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_GET_ATTR, sap_adec_attr)
#define CMD_IOCTL_ADEC_SET_ATTR _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_SET_ATTR, sap_adec_attr)
#define CMD_IOCTL_ADEC_SET_BYPASS _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_ADEC_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_ADEC_DETACH_OUTPUT _IOWR(DEV_ID_SAP_ADEC, CMD_ADEC_DETACH_OUTPUT, sap_drv_attach_para)

#define CMD_IOCTL_AENC_INIT _IO(DEV_ID_SAP_AENC, CMD_AENC_INIT)
#define CMD_IOCTL_AENC_DEINIT _IO(DEV_ID_SAP_AENC, CMD_AENC_DEINIT)
#define CMD_IOCTL_AENC_LOAD_CODEC _IOWR(DEV_ID_SAP_AENC, CMD_AENC_LOAD, sap_drv_load)
#define CMD_IOCTL_AENC_UNLOAD_CODEC _IOWR(DEV_ID_SAP_AENC, CMD_AENC_UNLOAD, sap_drv_load)
#define CMD_IOCTL_AENC_OPEN _IOWR(DEV_ID_SAP_AENC, CMD_AENC_OPEN, sap_drv_handle)
#define CMD_IOCTL_AENC_CLOSE _IOWR(DEV_ID_SAP_AENC, CMD_AENC_CLOSE, sap_drv_handle)
#define CMD_IOCTL_AENC_REG_EVENT_PROC _IOWR(DEV_ID_SAP_AENC, CMD_AENC_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_AENC_START _IOWR(DEV_ID_SAP_AENC, CMD_AENC_START, sap_drv_handle)
#define CMD_IOCTL_AENC_STOP _IOWR(DEV_ID_SAP_AENC, CMD_AENC_STOP, sap_drv_handle)
#define CMD_IOCTL_AENC_GET_ATTR _IOWR(DEV_ID_SAP_AENC, CMD_AENC_GET_ATTR, sap_aenc_attr)
#define CMD_IOCTL_AENC_SET_ATTR _IOWR(DEV_ID_SAP_AENC, CMD_AENC_SET_ATTR, sap_aenc_attr)
#define CMD_IOCTL_AENC_SET_BYPASS _IOWR(DEV_ID_SAP_AENC, CMD_AENC_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_AENC_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_AENC, CMD_AENC_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_AENC_DETACH_OUTPUT _IOWR(DEV_ID_SAP_AENC, CMD_AENC_DETACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_AENC_SET_PARAM _IOWR(DEV_ID_SAP_AENC, CMD_AENC_SET_PARAM, sap_aenc_param)

#define CMD_IOCTL_TRACK_INIT _IO(DEV_ID_SAP_TRACK, CMD_TRACK_INIT)
#define CMD_IOCTL_TRACK_DEINIT _IO(DEV_ID_SAP_TRACK, CMD_TRACK_DEINIT)
#define CMD_IOCTL_TRACK_GET_DEF_ATTR _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_DEF_ATTR, sap_track_open)
#define CMD_IOCTL_TRACK_OPEN _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_OPEN, sap_track_open)
#define CMD_IOCTL_TRACK_CLOSE _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_CLOSE, sap_drv_handle)
#define CMD_IOCTL_TRACK_REG_EVENT_PROC _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_TRACK_GET_STATE _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_STATE, sap_track_state)
#define CMD_IOCTL_TRACK_SET_STATE _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_SET_STATE, sap_track_state)
#define CMD_IOCTL_TRACK_GET_ATTR _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_ATTR, sap_track_attr)
#define CMD_IOCTL_TRACK_SET_ATTR _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_SET_ATTR, sap_track_attr)
#define CMD_IOCTL_TRACK_GET_MUTE _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_MUTE, sap_drv_enable)
#define CMD_IOCTL_TRACK_SET_MUTE _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_SET_MUTE, sap_drv_enable)
#define CMD_IOCTL_TRACK_GET_VOLUME _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_VOLUME, sap_ao_gain)
#define CMD_IOCTL_TRACK_SET_VOLUME _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_SET_VOLUME, sap_ao_gain)
#define CMD_IOCTL_TRACK_SET_BYPASS _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_TRACK_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_TRACK_DETACH_OUTPUT _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_DETACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_TRACK_GET_PTS _IOWR(DEV_ID_SAP_TRACK, CMD_TRACK_GET_PTS, sap_track_pts_param)

#define CMD_IOCTL_AO_INIT _IO(DEV_ID_SAP_AO, CMD_AO_INIT)
#define CMD_IOCTL_AO_DEINIT _IO(DEV_ID_SAP_AO, CMD_AO_DEINIT)
#define CMD_IOCTL_AO_GET_DEF_ATTR _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_DEF_ATTR, sap_ao_open)
#define CMD_IOCTL_AO_OPEN _IOWR(DEV_ID_SAP_AO, CMD_AO_OPEN, sap_ao_open)
#define CMD_IOCTL_AO_CLOSE _IOWR(DEV_ID_SAP_AO, CMD_AO_CLOSE, sap_drv_handle)
#define CMD_IOCTL_AO_REG_EVENT_PROC _IOWR(DEV_ID_SAP_AO, CMD_AO_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_AO_GET_PORT_ENABLE _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_PORT_ENABLE, sap_ao_port_enable)
#define CMD_IOCTL_AO_SET_PORT_ENABLE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_PORT_ENABLE, sap_ao_port_enable)
#define CMD_IOCTL_AO_GET_MUTE _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_MUTE, sap_ao_port_enable)
#define CMD_IOCTL_AO_SET_MUTE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_MUTE, sap_ao_port_enable)
#define CMD_IOCTL_AO_GET_VOLUME _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_VOLUME, sap_ao_port_gain)
#define CMD_IOCTL_AO_SET_VOLUME _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_VOLUME, sap_ao_port_gain)
#define CMD_IOCTL_AO_GET_TRACK_MODE _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_TRACK_MODE, sap_ao_port_mode)
#define CMD_IOCTL_AO_SET_TRACK_MODE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_TRACK_MODE, sap_ao_port_mode)
#define CMD_IOCTL_AO_GET_ALL_TRACK_MUTE _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_ALL_TRACK_MUTE, sap_drv_enable)
#define CMD_IOCTL_AO_SET_ALL_TRACK_MUTE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_ALL_TRACK_MUTE, sap_drv_enable)
#define CMD_IOCTL_AO_SET_BYPASS _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_AO_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_AO, CMD_AO_ATTACH_OUTPUT, sap_ao_port_output)
#define CMD_IOCTL_AO_DETACH_OUTPUT _IOWR(DEV_ID_SAP_AO, CMD_AO_DETACH_OUTPUT, sap_ao_port_output)
#define CMD_IOCTL_AO_GET_AEF_PARAM _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_AEF_PARAM, sap_ao_port_aef_param)
#define CMD_IOCTL_AO_SET_AEF_PARAM _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_AEF_PARAM, sap_ao_port_aef_param)
#define CMD_IOCTL_AO_SET_AEF_PROFILE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_AEF_PROFILE, sap_ao_port_aef_profile)
#define CMD_IOCTL_AO_GET_PORT_AEF_ENABLE _IOWR(DEV_ID_SAP_AO, CMD_AO_GET_PORT_AEF_ENABLE, sap_ao_port_aef_enable)
#define CMD_IOCTL_AO_SET_PORT_AEF_ENABLE _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_PORT_AEF_ENABLE, sap_ao_port_aef_enable)
#define CMD_IOCTL_AO_SET_SPK_CALI _IOWR(DEV_ID_SAP_AO, CMD_AO_SET_SPK_CALI, sap_spk_cali_coefs)

#define CMD_IOCTL_ANC_SET_CONFIG _IOWR(DEV_ID_SAP_ANC, CMD_ANC_SET_CONFIG, sap_anc_config)
#define CMD_IOCTL_ANC_GET_CONFIG _IOWR(DEV_ID_SAP_ANC, CMD_ANC_GET_CONFIG, sap_anc_config)

#define CMD_IOCTL_HAID_GET_DEFAULT_ATTR _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_DEFAULT_ATTR, haid_attr)
#define CMD_IOCTL_HAID_CREATE _IOWR(DEV_ID_SAP_HAID, CMD_HAID_CREATE, sap_haid_attr)
#define CMD_IOCTL_HAID_DESTROY _IOWR(DEV_ID_SAP_HAID, CMD_HAID_DESTROY, sap_drv_handle)
#define CMD_IOCTL_HAID_SET_ENABLE _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_ENABLE, sap_haid_enable)
#define CMD_IOCTL_HAID_GET_ENABLE _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_ENABLE, sap_haid_enable)
#define CMD_IOCTL_HAID_REGISTER_EVENT_PROC _IOWR(DEV_ID_SAP_HAID, CMD_HAID_REGISTER_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_HAID_SET_VOLUME _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_VOLUME, sap_haid_volume)
#define CMD_IOCTL_HAID_GET_VOLUME _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_VOLUME, sap_haid_volume)
#define CMD_IOCTL_HAID_SET_CONFIG _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_CONFIG, sap_haid_config)
#define CMD_IOCTL_HAID_GET_CONFIG _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_CONFIG, sap_haid_config)
#define CMD_IOCTL_HAID_SET_HARDWARE_CONFIG _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_HARDWARE_CONFIG, sap_haid_config)
#define CMD_IOCTL_HAID_SET_ALL_CONFIG _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_ALL_CONFIG, sap_haid_all_config)
#define CMD_IOCTL_HAID_GET_ALL_CONFIG _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_ALL_CONFIG, sap_haid_all_config)
#define CMD_IOCTL_HAID_SINUS_TONE_START _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SINUS_TONE_START, sap_haid_sinus_tone_attr)
#define CMD_IOCTL_HAID_SINUS_TONE_STOP _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SINUS_TONE_STOP, sap_drv_handle)
#define CMD_IOCTL_HAID_SET_SCENE _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_SCENE, sap_haid_scene)
#define CMD_IOCTL_HAID_GET_SCENE _IOWR(DEV_ID_SAP_HAID, CMD_HAID_GET_SCENE, sap_haid_scene)
#define CMD_IOCTL_HAID_SET_SPK_CALI _IOWR(DEV_ID_SAP_HAID, CMD_HAID_SET_SPK_CALI, sap_spk_cali_coefs)
#define CMD_IOCTL_HAID_SET_MIC_CALI_VOLUME _IOWR(DEV_ID_SAP_AI, CMD_HAID_SET_MIC_CALI_VOLUME, sap_haid_cali_volume)

#if defined(SAP_DPM_SUPPORT)
typedef struct {
    td_handle h_mod;
    dpm_id dpm_id;
    dpm_attr attr;
} sap_dpm_open;

typedef struct {
    td_handle h_mod;
    dpm_attr attr;
} sap_dpm_attr;

#define DEV_ID_SAP_DPM UMAP_MIN_MINOR_DPM
#define CMD_IOCTL_DPM_INIT _IO(DEV_ID_SAP_DPM, CMD_DPM_INIT)
#define CMD_IOCTL_DPM_DEINIT _IO(DEV_ID_SAP_DPM, CMD_DPM_DEINIT)
#define CMD_IOCTL_DPM_LOAD_LIB _IOWR(DEV_ID_SAP_DPM, CMD_DPM_LOAD, sap_drv_load)
#define CMD_IOCTL_DPM_UNLOAD_LIB _IOWR(DEV_ID_SAP_DPM, CMD_DPM_UNLOAD, sap_drv_load)
#define CMD_IOCTL_DPM_OPEN _IOWR(DEV_ID_SAP_DPM, CMD_DPM_OPEN, sap_drv_handle)
#define CMD_IOCTL_DPM_CLOSE _IOWR(DEV_ID_SAP_DPM, CMD_DPM_CLOSE, sap_drv_handle)
#define CMD_IOCTL_DPM_REG_EVENT_PROC _IOWR(DEV_ID_SAP_DPM, CMD_DPM_REG_EVENT_PROC, sap_drv_event)
#define CMD_IOCTL_DPM_START _IOWR(DEV_ID_SAP_DPM, CMD_DPM_START, sap_drv_handle)
#define CMD_IOCTL_DPM_STOP _IOWR(DEV_ID_SAP_DPM, CMD_DPM_STOP, sap_drv_handle)
#define CMD_IOCTL_DPM_GET_ATTR _IOWR(DEV_ID_SAP_DPM, CMD_DPM_GET_ATTR, sap_dpm_attr)
#define CMD_IOCTL_DPM_SET_ATTR _IOWR(DEV_ID_SAP_DPM, CMD_DPM_SET_ATTR, sap_dpm_attr)
#define CMD_IOCTL_DPM_SET_BYPASS _IOWR(DEV_ID_SAP_DPM, CMD_DPM_SET_BYPASS, sap_drv_enable)
#define CMD_IOCTL_DPM_ATTACH_OUTPUT _IOWR(DEV_ID_SAP_DPM, CMD_DPM_ATTACH_OUTPUT, sap_drv_attach_para)
#define CMD_IOCTL_DPM_DETACH_OUTPUT _IOWR(DEV_ID_SAP_DPM, CMD_DPM_DETACH_OUTPUT, sap_drv_attach_para)
#endif /* SAP_DPM_SUPPORT */

#if (SAP_OS_TYPE != os_type_linux)
enum {
    MODULE_SYS_IDX = 0x0,
    MODULE_ADP_IDX,
    MODULE_AI_IDX,
    MODULE_VAD_IDX,
    MODULE_SEA_IDX,
    MODULE_ADEC_IDX,
    MODULE_AENC_IDX,
    MODULE_TRACK_IDX,
    MODULE_AO_IDX,
    MODULE_ANC_IDX,
    MODULE_HAID_IDX,
    MODULE_DPM_IDX,

    MODULE_ID_MAX,
};

td_s32 audio_open(const td_char *path_name, td_s32 flags, td_s32 mode);
td_s32 audio_ioctl(td_s32 module_idx, td_ulong ioctl_cmd, td_void *para);
td_s32 audio_close(td_s32 module_idx);

#else /* linux ioctl */
#ifndef __KERNEL__
#define audio_open(path_name, flags, mode) open((path_name), (flags), (mode))
#define audio_ioctl(module_idx, request, para) ioctl((module_idx), (request), (para))
#define audio_close(module_idx) close((module_idx))
#endif /* __KERNEL__ */

#endif /* SAP_OS_TYPE */

#ifdef SAP_TIME_CONSUME
#define audio_api_time_consum_stat()                      \
    do {                                                  \
        audio_osal_print("Call %-32s takes:", __FUNCTION__); \
    } while (0)

#else
#define audio_api_time_consum_stat()
#endif /* SAP_TIME_CONSUME */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_SAP_IOCTL_H__ */
