/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hal aes v2 drivers header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_AES_V2_H
#define HAL_AES_V2_H

#include "hal_aes.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_aes_v2 hal funs
 * @ingroup  drivers_hal_cipher
 * @{
 */

/**
 * @brief  Definition of aes key source.
 */
typedef enum {
    AES_KEY_ID = 0,
    AES_KEY_SOFT = 1,
} aes_key_sel_t;

/**
 * @if Eng
 * @brief  Get hash aes v2 hal func.
 * @retval hash funcs pointer, see @ref hal_aes_funcs_t
 * @else
 * @brief  HAL层AES V2 获取HAL函数接口
 * @retval HAL层AES函数指针, 参考 @ref hal_aes_funcs_t
 * @endif
 */
hal_aes_funcs_t *hal_aes_v2_get_funcs(void);

/**
 * @if Eng
 * @brief  aes irq handler function
 * @else
 * @brief  HAL层aes中断处理函数
 * @endif
 */
void hal_aes_irq_handler(void);

/**
 * @if Eng
 * @brief  aes interrupt clear function
 * @else
 * @brief  HAL层aes中断清理函数
 * @endif
 */
void hal_aes_int_clear(void);

/**
 * @if Eng
 * @brief  aes interrupt enable function
 * @else
 * @brief  HAL层aes中断使能函数
 * @endif
 */
void hal_aes_int_enable(void);

/**
 * @if Eng
 * @brief  aes interrupt disable function
 * @else
 * @brief  HAL层aes中断去使能函数
 * @endif
 */
void hal_aes_int_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
