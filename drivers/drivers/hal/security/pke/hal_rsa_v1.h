/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides rsa v1 hal api header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-06, Create file. \n
 */
#ifndef HAL_RSA_V1_H
#define HAL_RSA_V1_H

#include <stdint.h>
#include <stdbool.h>
#include "hal_rsa.h"

#define RSAV1_KEY_LEN_1024    128
#define RSAV1_KEY_LEN_2048    256
#define RSAV1_KEY_LEN_4096    512

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa_v1 hal rsa v1 funs
 * @ingroup  drivers_hal_pke
 * @{
 */

/**
 * @if Eng
 * @brief  Get RSA v1 hal func. see @ref hal_rsa_funcs_t
 * @retval RSA v1 hal func
 * @else
 * @brief  获取HAL层RSA v1函数 @ref hal_rsa_funcs_t
 * @retval RSA v1 hal 函数
 * @endif
 */
hal_rsa_funcs_t *hal_rsa_v1_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
