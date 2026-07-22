/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: define the drv hal interface
 * Author: audio
 * Create: 2020-01-02
 */

#ifndef __DRV_HAL_H__
#define __DRV_HAL_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    td_u32 irq_no;  /* No. of interrupt for DSP inform CPU to process IPC message */
    td_u32 reg_int; /* Register for producing #irg_no interrupt signal  */
    td_u32 reg_int_clr; /* Register for clearing #irq_no interrupt signal */
    td_u32 reg_int_mask; /* Register for filtering #irq_no interrupt signal */
    td_u32 int_msg_bit; /* IPC messsage bit in #reg_int */
    td_u32 int_vad_bit; /* AVAD event bit in #reg_int */
    td_u32 int_clr_bit; /* IPC messsage bit in #reg_int_clr */
} dsp_int_cfg;

td_void hal_dsp_get_int_cfg(audio_core_id core_id, dsp_int_cfg *int_cfg);

td_u32 hal_dsp_get_core_num(td_void);

td_s32 hal_dsp_init(audio_core_id core_id);

td_s32 hal_dsp_deinit(audio_core_id core_id);

td_s32 hal_dsp_boot_prepare(audio_core_id core_id);

td_s32 hal_dsp_boot_up(audio_core_id core_id);

td_s32 hal_dsp_reset(audio_core_id core_id);

td_void hal_dsp_set_power_on(audio_core_id core_id, td_bool enable);

td_void hal_dsp_set_clock_attr(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_HAL_H__ */
