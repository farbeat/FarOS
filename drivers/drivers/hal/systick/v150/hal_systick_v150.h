/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V150 HAL systick \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01, Create file. \n
 */
#ifndef HAL_SYSTICK_V150_H
#define HAL_SYSTICK_V150_H

#include "hal_systick.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_systick_v150 Systick V150
 * @ingroup  drivers_hal_systick
 * @{
 */

/**
 * @if Eng
 * @brief  Get interface between Systick driver and Systick hal.
 * @else
 * @brief  获取Driver层Systick和HAL层Systick的接口
 * @endif
 */
hal_systick_funcs_t *hal_systick_v150_funcs_get(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif