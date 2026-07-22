/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides rsa hal funcs header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-01, Create file. \n
 */
#ifndef HAL_RSA_H
#define HAL_RSA_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_rsa hal rsa funcs
 * @ingroup  drivers_hal_pke
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of rsa attributes.
 * @else
 * @brief  rsa基础配置参数定义
 * @endif
 */
typedef struct rsa_attr {
    uint32_t ptext_addr;                    /*!< @if Eng rsa plain text addr.
                                                 @else   HAL层rsa明文地址 @endif */
    uint32_t key_addr;                      /*!< @if Eng rsa key text addr.
                                                 @else   HAL层rsa密钥地址 @endif */
    uint32_t output_addr;                   /*!< @if Eng rsa output text addr.
                                                 @else   HAL层rsa输出地址 @endif */
    uint32_t  key_len;                      /*!< @if Eng rsa key len.
                                                 @else   HAL层rsa密钥长度 @endif */
} rsa_attr_t;

/**
 * @if Eng
 * @brief  Callback of rsa rsa.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HASH RSA的回调函数
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_rsa_callback_t)(void);

/**
 * @if Eng
 * @brief  Init device for hal rsa.
 * @retval ERRCODE_SUCC   Success.
 * @param  [in] callback, rsa interrupt callback func, see @ref hal_rsa_callback_t
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层RSA的初始化接口
 * @retval ERRCODE_SUCC   成功.
 * @param  [in] callback, rsa中断回调函数, 参考 @ref hal_rsa_callback_t
 * @retval Other        失败. 详细参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_rsa_init_t)(hal_rsa_callback_t callback);

/**
 * @if Eng
 * @brief  DeInit device for hal rsa.
 * @else
 * @brief  HAL层RSA的去初始化接口
 * @endif
 */
typedef void (*hal_rsa_deinit_t)(void);

/**
 * @if Eng
 * @brief  Start device for hal rsa.
 * @param  [in] rsa_attr rsa attribute. see @ref rsa_attr_t
 * @else
 * @brief  HAL层RSA算法的启动接口
 * @param  [in] rsa_attr rsa配置属性. 参考 @ref rsa_attr_t
 * @endif
 */
typedef void (*hal_rsa_start_t)(rsa_attr_t *rsa_attr);

/**
 * @if Eng
 * @brief  Finish device for hal rsa.
 * @return rsa is finish or not.
 * @else
 * @brief  HAL层RSA算法的结束接口
 * @return rsa是否结束
 * @endif
 */
typedef bool (*hal_rsa_finish_t)(void);

/**
 * @if Eng
 * @brief  Interface between rsa driver and rsa hal.
 * @else
 * @brief  Driver层rsa和HAL层rsa的接口
 * @endif
 */
typedef struct hal_rsa_funcs {
    hal_rsa_init_t   init;               /*!< @if Eng Init rsa interface.
                                              @else   HAL层rsa的初始化接口 @endif */
    hal_rsa_deinit_t deinit;             /*!< @if Eng Deinit rsa interface.
                                              @else   HAL层rsa去初始化接口 @endif */
    hal_rsa_start_t  start;              /*!< @if Eng rsa start interface.
                                              @else   HAL层rsa启动接口 @endif */
    hal_rsa_finish_t finish;             /*!< @if Eng rsa end interface.
                                              @else   HAL层rsa结束接口 @endif */
} hal_rsa_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_rsa_funcs_t into the g_hal_rsa_funcs.
 * @param  [in]  funcs Interface between rsa driver and rsa hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  注册 @ref hal_rsa_funcs_t 到 g_hal_rsa_funcs
 * @param  [in]  funcs Driver层rsa和HAL层rsa的接口实例
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_rsa_register_funcs(hal_rsa_funcs_t *funcs);

/**
 * @if Eng
 * @brief  UnRegister @ref hal_rsa_funcs_t into the g_hal_rsa_funcs.
 * @else
 * @brief  去注册 @ref hal_rsa_funcs_t 到 g_hal_rsa_funcs
 * @endif
 */
void hal_rsa_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between rsa driver and rsa hal, see @ref hal_rsa_funcs_t.
 * @return Interface between rsa driver and rsa hal, see @ref hal_rsa_funcs_t.
 * @else
 * @brief  获取Driver层rsa和HAL层RSA的接口实例，参考 @ref hal_rsa_funcs_t.
 * @return Driver层rsa和HAL层RSA的接口实例，参考 @ref hal_rsa_funcs_t.
 * @endif
 */
hal_rsa_funcs_t *hal_rsa_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
