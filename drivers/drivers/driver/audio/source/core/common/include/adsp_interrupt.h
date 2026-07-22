/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: dsp interrupt define and api
 * Author: audio
 */

#ifndef __ADSP_INTERRUPT_H__
#define __ADSP_INTERRUPT_H__

#include "audio_type.h"
#include "interrupt_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
td_void dsp_int_en(td_u32 int_id);
td_void dsp_int_disable(td_u32 int_id);
td_void dsp_interrupt_on(td_u32 int_id);
td_void dsp_interrupt_off(td_u32 int_id);
td_void dsp_int_off_all(td_void);
td_void dsp_interrupt_disable_all(td_void);
td_void dsp_int_clc_all(td_void);
td_void dsp_interrupt_clear(td_u32 int_id);
td_void dsp_interrupt_trigger(td_u32 int_id);
td_void dsp_interrupt_setup(td_u32 int_id, td_void (*handler_interrupt)(td_void));
td_void dsp_set_exception_handler(td_s32 exccause, td_void (*handler_exception)(td_void));

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ADSP_INTERRUPT_H__ */
