/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: aiao interface of module
 * Author: audio
 * Create: 2020-02-06
 */

#ifndef __HAL_AI_H__
#define __HAL_AI_H__

#include "audio_struct.h"
#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef td_void (*hal_ai_intr_proc)(td_s32 port, ai_event_type event, td_void *param, td_void *iproc_data);

typedef struct hal_ai_inst* ai_port_handle;

td_s32 hal_ai_init(td_void);

td_void hal_ai_de_init(td_void);

td_s32 hal_ai_add_inst(audio_ai_port ai_port, ai_rx_type rx_type, td_bool ref_en);

td_void hal_ai_del_inst(ai_port_handle port_handle);

ai_port_handle hal_ai_get_port_handle(audio_ai_port ai_port);

td_s32 hal_ai_port_open(ai_port_handle port_handle, const audio_i2s_attr *i2s_attr,
    const audio_pcm_format *pcm_attr);

td_s32 hal_ai_port_close(ai_port_handle port_handle);

td_void hal_ai_set_intr_proc(ai_port_handle port_handle, hal_ai_intr_proc intr_proc, td_void *iproc_data);

td_s32 hal_ai_port_start(ai_port_handle port_handle);

td_s32 hal_ai_port_stop(ai_port_handle port_handle);

td_void hal_ai_port_register_dump_handle(ai_port_handle port_handle,
    td_void (*dump)(const td_s32 *data, const audio_pcm_format *pcm_format));

td_s32 hal_ai_port_read_data(ai_port_handle port_handle, td_u8 *buf, td_u32 size);

td_s32 hal_ai_get_attr(ai_port_handle port_handle, audio_i2s_attr *i2s_attr);

td_s32 hal_ai_set_attr(ai_port_handle port_handle, const audio_i2s_attr *i2s_attr,
    const audio_pcm_format *pcm_attr, const ai_ref_attr *ref_attr);

td_s32 hal_ai_ref_open(ai_port_handle port_handle, ao_port_id port_id);

td_void hal_ai_ref_close(ai_port_handle port_handle);

td_s32 hal_ai_get_port_status(ai_port_handle port_handle, ai_port_stauts *port_status);

td_s32 hal_ai_port_get_pcm_attr(ai_port_handle port_handle, audio_pcm_format *ai_attr, audio_pcm_format *ref_attr);

td_s32 hal_ai_port_check_interleave(ai_port_handle port_handle, td_bool *interleave);

td_s32 hal_ai_port_get_rw_pos(ai_port_handle port_handle, td_u32 *read_pos, td_u32 *write_pos);

td_void hal_ai_regist_intr_func(ai_port_handle port_handle, td_s32 (*isr_proc)(td_s32 *data));

td_s32 hal_ai_get_edma_ch(ai_port_handle port_handle, td_u32 *edma_ch);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HAL_AI_H__ */
