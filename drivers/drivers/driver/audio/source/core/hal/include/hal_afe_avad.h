/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: afe hal
 * Author: audio
 */

#ifndef __HAL_AFE_AVAD_H__
#define __HAL_AFE_AVAD_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    HAL_AFE_AVAD_M0_L0 = 0,
    HAL_AFE_AVAD_M0_L1,
    HAL_AFE_AVAD_M0_L2,
    HAL_AFE_AVAD_M0_L3,
    HAL_AFE_AVAD_M1_L0,
    HAL_AFE_AVAD_M1_L1,
    HAL_AFE_AVAD_M1_L2,
    HAL_AFE_AVAD_M1_L3,
    HAL_AFE_AVAD_MODE_MAX,
} hal_afe_avad_mode;

td_s32 hal_afe_avad_init(td_void);

td_void hal_afe_avad_deinit(td_void);

td_s32 hal_afe_avad_open(td_void);

td_s32 hal_afe_avad_set_enable(td_bool enable);

td_void hal_afe_avad_set_mode(hal_afe_avad_mode mode);

td_s32 hal_afe_avad_close(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HAL_AFE_AVAD_H__ */
