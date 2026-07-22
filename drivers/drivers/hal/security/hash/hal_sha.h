/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash hal drivers header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_SHA_H
#define HAL_SHA_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_sha_api Hash Sha Api
 * @ingroup  drivers_hal_sha
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of hash sha compute mode type
 * @else
 * @brief  HASH SHA计算类型
 * @endif
 */
typedef enum {
    SHA1 = 0x00,
    SHA224,
    SHA256,
    SHA384,
    SHA512,
    SHA_MAX,
    SHA_INVALID = 0xffffffff,
} sha_mode_t;

/**
 * @if Eng
 * @brief  Definition of HASH SHA attributes.
 * @else
 * @brief  HASH sha基础配置参数定义
 * @endif
 */
typedef struct sha_attr {
    uint32_t input_addr;                        /*!< @if Eng SHA input address at the HAL layer
                                                     @else   HAL层SHA输入地址 @endif */
    uint32_t output_addr;                       /*!< @if Eng SHA output address at the HAL layer.
                                                     @else   HAL层SHA输出地址 @endif */
    uint32_t data_length;                       /*!< @if Eng Length of data processed by SHA.
                                                     @else   SHA处理数据长度 @endif */
    uint32_t  total_length;                     /*!< @if Eng Total length of SHA data.
                                                     @else   SHA数据总长度 @endif */
    sha_mode_t sha_mode;                        /*!< @if Eng SHA Algorithm Type.
                                                     @else   SHA算法类型 @endif */
    bool is_iv_exist;                           /*!< @if Eng Indicates whether the initial value exists.
                                                     @else   iv是否存在的标志 @endif */
    bool is_last_package;                       /*!< @if Eng Indicates whether the packet is the last packet for sha
                                                             calculation.
                                                     @else   是否sha计算的最后一包 @endif */
} sha_attr_t;

/**
 * @if Eng
 * @brief  Callback of hash sha.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HASH SHA的回调函数
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_sha_callback_t)(void);

/**
 * @if Eng
 * @brief  Init device for hal sha.
 * @retval ERRCODE_SUCC   Success.
 * @param  [in] callback sha interrupt callback func, see @ref hal_sha_callback_t
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层SHA的初始化接口
 * @retval ERRCODE_SUCC   成功.
 * @param  [in] callback sha中断回调函数, 参考 @ref hal_sha_callback_t
 * @retval Other        失败. 详细参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_sha_init_t)(hal_sha_callback_t callback);

/**
 * @if Eng
 * @brief  DeInit device for hal sha.
 * @else
 * @brief  HAL层SHA的去初始化接口
 * @endif
 */
typedef void (*hal_sha_deinit_t)(void);

/**
 * @if Eng
 * @brief  Start device for hal sha.
 * @param  [in] sha_attr hash attribute. see @ref sha_attr_t
 * @else
 * @brief  HAL层SHA算法的启动接口
 * @param  [in] sha_attr hash配置属性. 参考 @ref sha_attr_t
 * @endif
 */
typedef void (*hal_sha_start_t)(sha_attr_t sha_attr);

/**
 * @if Eng
 * @brief  Finish device for hal sha.
 * @return hash is finish or not.
 * @else
 * @brief  HAL层SHA算法的结束接口
 * @return hash是否结束
 * @endif
 */
typedef bool (*hal_sha_finish_t)(void);

/**
 * @if Eng
 * @brief  Interface between pke driver and pke hal.
 * @else
 * @brief  Driver层HASH和HAL层SHA的接口
 * @endif
 */
typedef struct {
    hal_sha_init_t   init;                /*!< @if Eng Init pke interface.
                                               @else   HAL层SHA的初始化接口 @endif */
    hal_sha_deinit_t deinit;              /*!< @if Eng Deinit pke interface.
                                               @else   HAL层SHA去初始化接口 @endif */
    hal_sha_start_t  start;               /*!< @if Eng pke start interface.
                                               @else   HAL层SHA启动接口 @endif */
    hal_sha_finish_t finish;              /*!< @if Eng pke end interface.
                                               @else   HAL层SHA结束接口 @endif */
} hal_sha_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_sha_funcs_t into the g_hal_hashs_funcs.
 * @param  [in]  funcs Interface between hash driver and hash hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  注册 @ref hal_sha_funcs_t 到 g_hal_hashs_funcs
 * @param  [in]  funcs Driver层hash和HAL层SHA的接口实例
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_sha_register_funcs(hal_sha_funcs_t *funcs);

/**
 * @if Eng
 * @brief  UnRegister @ref hal_sha_funcs_t into the g_hal_hashs_funcs.
 * @else
 * @brief  去注册 @ref hal_sha_funcs_t 到 g_hal_hashs_funcs
 * @endif
 */
void hal_sha_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between hash driver and hash hal, see @ref hal_sha_funcs_t.
 * @return Interface between hash driver and hash hal, see @ref hal_sha_funcs_t.
 * @else
 * @brief  获取Driver层hash和HAL层SHA的接口实例，参考 @ref hal_sha_funcs_t.
 * @return Driver层hash和HAL层SHA的接口实例，参考 @ref hal_sha_funcs_t.
 * @endif
 */
hal_sha_funcs_t *hal_sha_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
