/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash hal sha v2 drivers header file. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_SHA_V2_H
#define HAL_SHA_V2_H

#include "hal_sha.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_sha_v2 Hash Sha V2
 * @ingroup  drivers_hal_sha
 * @{
 */

/**
 * @if Eng
 * @brief  Get hash sha v2 hal func.
 * @retval hash funcs pointer, see @ref hal_sha_funcs_t
 * @else
 * @brief  HAL层HASH SHA V2 获取HAL函数接口
 * @retval HAL层HASH函数指针, 参考 @ref hal_sha_funcs_t
 * @endif
 */
hal_sha_funcs_t *hal_sha_v2_get_funcs(void);

/**
 * @if Eng
 * @brief  sha irq handler function
 * @else
 * @brief  HAL层sha中断处理函数
 * @endif
 */
void hal_sha_irq_handler(void);

/**
 * @if Eng
 * @brief  sha interrupt clear function
 * @else
 * @brief  HAL层sha中断清理函数
 * @endif
 */
void hal_sha_int_clear(void);

/**
 * @if Eng
 * @brief  sha interrupt enable function
 * @else
 * @brief  HAL层sha中断使能函数
 * @endif
 */
void hal_sha_int_enable(void);

/**
 * @if Eng
 * @brief  sha interrupt disable function
 * @else
 * @brief  HAL层sha中断去使能函数
 * @endif
 */
void hal_sha_int_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
