/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: afe hal
 * Author: audio
 */
#ifndef __HAL_AFE_SYS_H__
#define __HAL_AFE_SYS_H__

#include "td_type.h"
#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HAL_DAC_GAIN_MAX 60
#define HAL_DAC_GAIN_MIN (-120)
#define HAL_DAC_GAIN_DEFAULT 0

/* 暂时先只调节模拟部分的PGA增益+BOOST */
#define HAL_ADC_GAIN_MAX 50
#define HAL_ADC_GAIN_MIN 0
#define HAL_ADC_GAIN_DEFAULT 30 /* 媒体需要默认音量较大 */
#define HAL_ADC_GAIN_DECIMAL_MAX 1000
#define HAL_ADC_GAIN_DECIMAL_MIN (-1000)
#define HAL_LPADC_GAIN_DEFAULT 20

typedef enum {
    HAL_AFE_NULL = 0x00,
    HAL_AFE_SYS = 0x01,
    HAL_AFE_AVAD = 0x02,
    HAL_AFE_DAC = 0x04,
    HAL_AFE_ADC = 0x08,
    HAL_AFE_LPADC = 0x10,
    HAL_AFE_DMIC = 0x20,
    HAL_AFE_MAD = 0x40,
    HAL_AFE_ANA = 0x80,
    HAL_AFE_COMM = 0x100,
    HAL_AFE_ALL = 0xFFF,
} hal_afe_type;

typedef enum {
    HAL_AFE_SRC_AIAO = 0x00,
    HAL_AFE_SRC_ANC,
    HAL_AFE_SRC_LPADC,
    HAL_AFE_SRC_MAX,
} hal_afe_src_type;

typedef enum {
    HAL_AFE_CHAN_ANY = 0x00,
    HAL_AFE_CHAN_L0 = 0x01,
    HAL_AFE_CHAN_L1 = 0x02,
    HAL_AFE_CHAN_L2 = 0x04,
    HAL_AFE_CHAN_L3 = 0x08,
    HAL_AFE_CHAN_R0 = 0x10,
    HAL_AFE_CHAN_R1 = 0x20,
    HAL_AFE_CHAN_R2 = 0x40,
    HAL_AFE_CHAN_R3 = 0x80,
    HAL_AFE_CHAN_ALL = 0xFF,
} hal_afe_chan_id;

typedef enum {
    HAL_AFE_MIC_AMIC = 0x00,
    HAL_AFE_MIC_DMIC,
    HAL_AFE_MIC_MAX
} hal_afe_mic_type;

td_s32 hal_afe_sys_init(td_void);

td_void hal_afe_sys_deinit(td_void);

td_void hal_afe_get_regs(hal_afe_type afe_type, td_void **afe_regs);

td_void hal_afe_sys_pon(hal_afe_type type);

td_void hal_afe_sys_pd(hal_afe_type type);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HAL_AFE_SYS_H__ */
