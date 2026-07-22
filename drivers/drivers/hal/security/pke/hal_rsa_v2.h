/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa v2 hal api header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_RSA_V2_H
#define HAL_RSA_V2_H

#include <stdint.h>
#include <stdbool.h>
#include "hal_rsa.h"

#define RSAV2_KEY_LEN_1024    128
#define RSAV2_KEY_LEN_2048    256
#define RSAV2_KEY_LEN_4096    512

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa_v2 hal rsa v2 funs
 * @ingroup  drivers_hal_pke
 * @{
 */

/**
 * @if Eng
 * @brief  Get RSA v2 hal func. see @ref hal_rsa_funcs_t
 * @retval RSA v2 hal func
 * @else
 * @brief  获取HAL层RSA V2函数 @ref hal_rsa_funcs_t
 * @retval RSA V2 hal 函数
 * @endif
 */
hal_rsa_funcs_t *hal_rsa_v2_get_funcs(void);

/**
 * @if Eng
 * @brief  rsa irq handler function
 * @else
 * @brief  HAL层rsa中断处理函数
 * @endif
 */
void hal_rsa_irq_handler(void);

/**
 * @if Eng
 * @brief  rsa interrupt clear function
 * @else
 * @brief  HAL层rsa中断清除函数
 * @endif
 */
void hal_rsa_int_clear(void);

/**
 * @if Eng
 * @brief  rsa interrupt enable function
 * @else
 * @brief  HAL层rsa中断使能函数
 * @endif
 */
void hal_rsa_int_enable(void);

/**
 * @if Eng
 * @brief  rsa interrupt disable function
 * @else
 * @brief  HAL层RSA中断去使能函数
 * @endif
 */
void hal_rsa_int_disable(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
