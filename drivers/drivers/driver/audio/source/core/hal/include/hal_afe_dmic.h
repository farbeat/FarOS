/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: dmic hal header file
 * Author: audio
 */

#ifndef __HAL_AFE_DMIC_H__
#define __HAL_AFE_DMIC_H__

#include "hal_afe_sys.h"
#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


td_s32 hal_afe_dmic_init(td_void);

td_void hal_afe_dmic_deinit(td_void);

td_s32 hal_afe_dmic_get_chan_sel(td_u32 *chan_sel, ai_rx_type rx_type, td_u32 channels);

td_s32 hal_afe_dmic_open(td_u32 chan_sel, audio_sample_rate sample_rate, audio_bit_depth bit_depth);

td_s32 hal_afe_dmic_close(td_void);

td_s32 hal_afe_dmic_set_enable(td_bool enable);

td_s32 hal_afe_dmic_set_gain(td_s32 integer, td_s32 decimal);

td_s32 hal_afe_dmic_get_gain(td_s32 *integer, td_s32 *decimal);

td_s32 hal_afe_dmic_set_pcm_attr(audio_sample_rate sample_rate, audio_bit_depth bit_depth);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HAL_AFE_DMIC_H__ */
