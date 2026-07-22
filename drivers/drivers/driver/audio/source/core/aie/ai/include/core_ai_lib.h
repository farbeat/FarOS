/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio input
 * Author: audio
 */

#ifndef __CORE_AI_LIB_H__
#define __CORE_AI_LIB_H__

#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef td_void *ai_dev_handle;

typedef td_s32 (*ai_dev_event_report)(td_void *handle, td_s32 event, td_void *param, td_void *context);
typedef td_s32 (*ai_dev_data_output)(td_void *handle, audio_frame *frame, td_void *context);

typedef struct {
    audio_ai_port ai_port;
    ai_rx_type rx_type;
    audio_i2s_attr *i2s_attr;
    ai_ref_attr *ref_attr;
    audio_pcm_format *pcm_attr;
} ai_rx_open_attr;

typedef struct {
    const td_char *name;
    const ai_rx_type type;
    td_s32 (*init)(td_void);
    td_s32 (*deinit)(td_void);
    td_s32 (*get_ref_attr)(ao_port_id port, audio_pcm_format *pcm_attr);
    td_s32 (*open)(ai_dev_handle *handle, const ai_rx_open_attr *open_attr);
    td_s32 (*close)(ai_dev_handle handle);
    td_s32 (*read)(ai_dev_handle handle, audio_frame *frame);
    td_s32 (*set_enable)(ai_dev_handle handle, td_bool enable);
    td_s32 (*set_ref_bypass)(ai_dev_handle handle, td_bool enable);
    td_s32 (*set_attr)(ai_dev_handle handle, const audio_i2s_attr *i2s_attr, const ai_ref_attr *ref_attr);
    td_s32 (*reg_event_report)(ai_dev_handle handle, ai_dev_event_report event_report, td_void *context);
    td_s32 (*reg_data_output)(ai_dev_handle handle, ai_dev_data_output data_output);
    td_s32 (*get_status)(ai_dev_handle handle, ai_port_stauts *status);
} ai_dev_rx;

typedef struct {
    const td_char *name;
    const ai_afe_type type;
    td_s32 (*init)(td_void);
    td_s32 (*deinit)(td_void);
    td_s32 (*open)(ai_dev_handle *handle, audio_ai_port port, const audio_pcm_format *pcm_attr, ai_rx_type rx_type);
    td_s32 (*close)(ai_dev_handle handle);
    td_s32 (*set_enable)(ai_dev_handle handle, td_bool enable);
    td_s32 (*set_attr)(ai_dev_handle handle, const audio_pcm_format *pcm_attr);
    td_s32 (*set_volume)(ai_dev_handle handle, const ai_gain *gain);
    td_s32 (*get_volume)(ai_dev_handle handle, ai_gain *gain);
} ai_dev_afe;

typedef struct {
    const td_char *name;
    const vad_dev_type type;
    td_s32 (*init)(td_void);
    td_s32 (*deinit)(td_void);
    td_s32 (*open)(ai_dev_handle *handle, audio_ai_port port,
                   const audio_pcm_format *pcm_attr, const audio_vad_attr *vad_attr);
    td_s32 (*close)(ai_dev_handle handle);
    td_s32 (*set_enable)(ai_dev_handle handle, td_bool enable);
    td_s32 (*set_attr)(ai_dev_handle handle, const audio_pcm_format *pcm_attr, const audio_vad_attr *vad_attr);
    td_s32 (*reg_event_report)(ai_dev_handle handle, ai_dev_event_report event_report, td_void *context);
} ai_dev_vad;

td_void ai_dev_get_type(audio_ai_port port, audio_ai_attr *attr,
                        ai_rx_type *rx_type, ai_afe_type *afe_type, vad_dev_type *vad_type);

td_s32 ai_dev_register_rx_driver(ai_dev_rx *dev_rx);
td_s32 ai_dev_register_afe_driver(ai_dev_afe *dev_afe);
td_s32 ai_dev_register_vad_driver(ai_dev_vad *dev_vad);

ai_dev_rx* ai_dev_get_rx_driver(ai_rx_type type);
ai_dev_afe* ai_dev_get_afe_driver(ai_afe_type type);
ai_dev_vad* ai_dev_get_vad_driver(vad_dev_type type);

#ifdef SAP_AFE_AVAD
td_void ai_dev_load_avad(td_void);
#else
#define ai_dev_load_avad()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CORE_AI_LIB_H__ */
