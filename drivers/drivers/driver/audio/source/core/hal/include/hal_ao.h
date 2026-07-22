/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: aiao interface of module
 * Author: audio
 */

#ifndef __HAL_AO_H__
#define __HAL_AO_H__

#include "audio_struct.h"
#include "ao_struct.h"
#include "core_rbuf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HAL_AO_BUFFER_MS 40

typedef struct hal_ao_inst* ao_port_handle;

td_s32 hal_ao_init(td_void);

td_void hal_ao_de_init(td_void);

td_s32 hal_ao_add_inst(ao_port_id ao_port);

td_void hal_ao_del_inst(ao_port_handle port_handle);

ao_port_handle hal_ao_get_port_handle(ao_port_id ao_port);

td_s32 hal_ao_port_open(ao_port_handle port_handle,
                        const ao_out_port_attr *port_attr, const audio_pcm_format *pcm_attr);

td_s32 hal_ao_port_close(ao_port_handle port_handle);

td_s32 hal_ao_port_start(ao_port_handle port_handle);

td_s32 hal_ao_port_stop(ao_port_handle port_handle);

td_s32 hal_ao_set_ref_rbuf_handle(ao_port_handle port_handle, rbuf_handle rbuf);

rbuf_handle hal_ao_get_ref_rbuf_handle(ao_port_handle port_handle);

rbuf_handle hal_ao_get_port_rbuf_handle(ao_port_handle port_handle);

td_s32 hal_ao_port_get_pcm_attr(ao_port_handle port_handle, audio_pcm_format *pcm_attr);

td_s32 hal_ao_port_check_interleave(ao_port_handle port_handle, td_bool *interleave);

td_s32 hal_ao_get_port_status(ao_port_handle port_handle, ao_out_port_status *port_status);

td_u32 hal_ao_get_aiao_port_by_ao_port(ao_port_id ao_port);

td_s32 hal_ao_get_edma_ch(ao_port_handle port_handle, td_u32 *edma_ch);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HAL_AO_H__ */
