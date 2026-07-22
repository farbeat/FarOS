/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 HAL Watchdog \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26, Create file. \n
 */
#ifndef HAL_WATCHDOG_V100_H
#define HAL_WATCHDOG_V100_H

#include "hal_watchdog.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_watchdog_v100 Watchdog V100
 * @ingroup  drivers_hal_watchdog
 * @{
 */

/**
 * @if Eng
 * @brief  Get functions of the Watchdog v100.
 * @else
 * @brief  获取 Watchdog v100 的实例
 * @endif
 */
hal_watchdog_funcs_t *hal_watchdog_v100_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the Watchdog interrupt request.
 * @param  [in]  param request param
 * @else
 * @brief  Watchdog中断处理函数
 * @param  [in]  param 中断请求的参数
 * @endif
 */
void hal_watchdog_irq_handler(uintptr_t param);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
