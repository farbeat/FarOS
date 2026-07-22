/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: afe hal
 * Author: audio
 */

#ifndef __HAL_AFE_LPADC_H__
#define __HAL_AFE_LPADC_H__

#include "hal_afe_sys.h"
#include "ai_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

td_s32 hal_afe_lpadc_init(td_void);

td_void hal_afe_lpadc_deinit(td_void);

td_s32 hal_afe_lpadc_get_chan_sel(td_u32 *chan_sel, ai_rx_type rx_type, td_u32 channels);

td_s32 hal_afe_lpadc_open(td_u32 chan_sel, audio_sample_rate sample_rate, audio_bit_depth bit_width);

td_s32 hal_afe_lpadc_close(td_void);

td_s32 hal_afe_lpadc_set_enable(td_bool enable);

td_s32 hal_afe_lpadc_set_gain(td_s32 gain);

td_s32 hal_afe_lpadc_get_gain(td_s32 *gain);

td_s32 hal_afe_lpadc_set_pcm_attr(audio_sample_rate sample_rate, audio_bit_depth bit_width);

td_s32 hal_afe_lpadc_set_mic_cali_volume(td_u32 chan_sel, td_s32 cali_volume);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HAL_AFE_LPADC_H__ */
