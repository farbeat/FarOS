/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash hal key_deriv v2 drivers header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_KEY_DERIV_V2_H
#define HAL_KEY_DERIV_V2_H

#include "hal_key_deriv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_key_deriv_api Key deriv api
 * @ingroup  drivers_hal_km
 * @{
 */

/**
 * @if Eng
 * @brief  Get key_deriv v2 hal func.
 * @retval key deriv funcs pointer, see @ref hal_key_deriv_funcs_t
 * @else
 * @brief  HAL层 key_deriv V2 获取HAL函数接口
 * @retval HAL层HASH函数指针, 参考 @ref hal_key_deriv_funcs_t
 * @endif
 */
hal_key_deriv_funcs_t *hal_key_deriv_v2_get_funcs(void);

/**
 * @if Eng
 * @brief  key_deriv irq handler function
 * @else
 * @brief  HAL层key_deriv中断处理函数
 * @endif
 */
void hal_key_deriv_irq_handler(void);

/**
 * @if Eng
 * @brief  key_deriv interrupt clear function
 * @else
 * @brief  HAL层key_deriv中断清理函数
 * @endif
 */
void hal_key_deriv_int_clear(void);

/**
 * @if Eng
 * @brief  key_deriv interrupt enable function
 * @else
 * @brief  HAL层key_deriv中断使能函数
 * @endif
 */
void hal_key_deriv_int_enable(void);

/**
 * @if Eng
 * @brief  key_deriv interrupt disable function
 * @else
 * @brief  HAL层key_deriv中断去使能函数
 * @endif
 */
void hal_key_deriv_int_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
