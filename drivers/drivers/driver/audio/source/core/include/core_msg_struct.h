/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for message
 * Author: audio
 */

#ifndef __CORE_MSG_STRUCT_H__
#define __CORE_MSG_STRUCT_H__

#include "audio_struct.h"
#include "ao_struct.h"
#include "ai_struct.h"
#include "adp_struct.h"
#include "ha_codec.h"
#include "adec_struct.h"
#include "aenc_struct.h"
#include "sea_struct.h"
#include "haid_struct.h"
#include "anc_struct.h"
#include "dpm_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define CORE_MSG_ADEC_PRIV_DATA_SIZE 512
#define CORE_MSG_AENC_PRIV_DATA_SIZE 512

typedef struct {
    audio_core_id core_id;
    td_u32 event;
    td_u32 param;
} msg_data_sys_event_info;

typedef struct {
    audio_adp_type type;
    audio_adp_attr attr;
} msg_data_adp_open;

typedef struct {
    audio_ai_port port;
    audio_ai_attr attr;
} msg_data_ai_open;

typedef struct {
    ai_event_type event;
    ai_event_param param;
} msg_data_ai_event_info;

typedef struct {
    sea_eng_sel eng_sel;
    sea_pcm_attr attr;
} msg_data_sea_attr;

typedef struct {
    sea_item_type type;
    td_u32 index;
} msg_data_sea_item_index;

typedef struct {
    sea_item_type type;
    sea_info_item node;
} msg_data_sea_item_node;

typedef struct {
    sea_event_type event;
    sea_event_param param;
} msg_data_sea_event_info;

typedef struct {
    audio_adp_type adp_type;
    td_u32 data_port_id;
    td_handle h_mod;
} msg_data_adp_port;

typedef struct {
    adp_event_type event;
    td_u32 param;
} msg_data_adp_event_info;

typedef struct {
    adec_attr attr;
    td_u8 private_data[CORE_MSG_ADEC_PRIV_DATA_SIZE];
    td_u32 private_data_size;
} msg_data_adec_attr;

typedef struct {
    aenc_attr attr;
    td_u8 private_data[CORE_MSG_AENC_PRIV_DATA_SIZE];
    td_u32 private_data_size;
} msg_data_aenc_attr;

typedef struct {
    aenc_param_type type;
    td_u8 param[AENC_PARAM_SIZE_MAX];
    td_u32 param_size;
} msg_data_aenc_param;

typedef struct {
    td_handle sound;
    track_attr attr;
} msg_data_track_open;

typedef struct {
    ao_snd_id snd_id;
    ao_attr attr;
} msg_data_ao_open;

typedef struct {
    ao_port_id port;
    td_bool enable;
} msg_data_ao_port_enable;

typedef struct {
    ao_port_id port;
    ao_track_mode mode;
} msg_data_ao_port_mode;

typedef struct {
    ao_port_id port;
    ao_gain gain;
    audio_toggle_config toggle_config;
} msg_data_ao_port_gain;

typedef struct {
    ao_port_id port;
    td_handle output;
} msg_data_ao_port_output;

typedef struct {
    ao_port_id port;
    td_u32 cmd;
    audio_buffer mmz_buf;
} msg_data_ao_port_aef_param;

typedef struct {
    ao_port_id port;
    aef_type effect_type;
    td_bool enable;
} msg_data_ao_port_aef_enable;

typedef struct {
    td_u32 frequence;
    haid_volume volume;
} msg_data_haid_sinus_tone_attr;

typedef struct {
    td_handle h_mod;
    haid_attr attr;
} msg_data_haid_attr;

typedef struct {
    haid_event_type event;
    haid_event_param param;
} msg_data_haid_event_info;

typedef struct {
    sea_afe_type type;
    td_u32 param_size;
} msg_data_sea_param_get;

typedef struct {
    sea_afe_type type;
    td_char param[SEA_PARAM_SIZE_MAX];
    td_u32 param_size;
} msg_data_sea_param_set;

typedef struct {
    track_state state;
    audio_toggle_config toggle_config;
} msg_data_track_state;

typedef struct {
    ao_gain gain;
    audio_toggle_config toggle_config;
} msg_data_track_gain;

typedef struct {
    dpm_attr attr;
    audio_buffer mmz_buf;
} msg_data_dpm_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CORE_MSG_STRUCT_H__ */
