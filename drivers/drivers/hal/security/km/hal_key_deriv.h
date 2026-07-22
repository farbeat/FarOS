/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides key_deriv hal funcs \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_KEY_DERIV_H
#define HAL_KEY_DERIV_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_key_deriv hal hal_deriv Api
 * @ingroup  drivers_hal_km
 * @{
 */

typedef enum {
    ROOT_KEY_HUK = 0,
    ROOT_KEY_GID = 1,
} root_key_type_t;

typedef enum {
    DERIV_KEY_ID_0 = 0,
    DERIV_KEY_ID_1 = 1,
    DERIV_KEY_ID_2 = 2,
    DERIV_KEY_ID_3 = 3,
    DERIV_KEY_ID_4 = 4,
    DERIV_KEY_ID_5 = 5,
    DERIV_KEY_ID_6 = 6,
    DERIV_KEY_ID_7 = 7,
    DERIV_KEY_ID_MAX,
} deriv_key_id_t;

/**
 * @if Eng
 * @brief  Definition of key_deriv attributes.
 * @else
 * @brief  key_deriv基础配置参数定义
 * @endif
 */
typedef struct {
    uint16_t deriv_num;
    root_key_type_t key_sel;
    deriv_key_id_t key_id;
    uintptr_t key_addr;
} key_deriv_attr_t;

/**
 * @if Eng
 * @brief  Callback of hal key_deriv.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  key_deriv 的回调函数
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_key_deriv_callback_t)(void);

/**
 * @if Eng
 * @brief  Init device for hal key_deriv.
 * @retval ERRCODE_SUCC   Success.
 * @param  [in] callback key_deriv interrupt callback func, see @ref hal_key_deriv_callback_t
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层key_deriv的初始化接口
 * @retval ERRCODE_SUCC   成功.
 * @param  [in] callback key_deriv中断回调函数, 参考 @ref hal_key_deriv_callback_t
 * @retval Other        失败. 详细参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_key_deriv_init_t)(hal_key_deriv_callback_t callback);

/**
 * @if Eng
 * @brief  DeInit device for hal key_deriv.
 * @else
 * @brief  HAL层key_deriv的去初始化接口
 * @endif
 */
typedef void (*hal_key_deriv_deinit_t)(void);

/**
 * @if Eng
 * @brief  Start device for hal key_deriv.
 * @else
 * @brief  HAL层key_deriv算法的启动接口
 * @endif
 */
typedef void (*hal_key_deriv_start_t)(key_deriv_attr_t *key_deriv_attr);

/**
 * @if Eng
 * @brief  Finish device for hal key_deriv.
 * @return key_deriv is finish or not.
 * @else
 * @brief  HAL层key_deriv算法的结束接口
 * @return 是否结束
 * @endif
 */
typedef bool (*hal_key_deriv_get_result_t)(deriv_key_id_t key_id);

/**
 * @if Eng
 * @brief  Interface between pke driver and pke hal.
 * @else
 * @brief  Driver层key_deriv和HAL层key_deriv的接口
 * @endif
 */
typedef struct {
    hal_key_deriv_init_t   init;             /*!< @if Eng Init key_deriv interface.
                                                  @else   HAL层key_deriv的初始化接口 @endif */
    hal_key_deriv_deinit_t deinit;           /*!< @if Eng Deinit key_deriv interface.
                                                  @else   HAL层key_deriv去初始化接口 @endif */
    hal_key_deriv_start_t  start;            /*!< @if Eng key_deriv start interface.
                                                  @else   HAL层key_deriv启动接口 @endif */
    hal_key_deriv_get_result_t get_result;   /*!< @if Eng key_deriv get result interface.
                                                  @else   HAL层key_deriv获取结果接口 @endif */
} hal_key_deriv_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_key_deriv_funcs_t into the g_hal_key_deriv_funcs.
 * @param  [in]  funcs Interface between key_deriv driver and key_deriv hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  注册 @ref hal_key_deriv_funcs_t 到 g_hal_key_deriv_funcs
 * @param  [in]  funcs Driver层key_deriv和HAL层key_deriv的接口实例
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_key_deriv_register_funcs(hal_key_deriv_funcs_t *funcs);

/**
 * @if Eng
 * @brief  UnRegister @ref hal_key_deriv_funcs_t into the g_hal_key_deriv_funcs.
 * @else
 * @brief  去注册 @ref hal_key_deriv_funcs_t 到 g_hal_key_deriv_funcs
 * @endif
 */
void hal_key_deriv_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between key_deriv driver and key_deriv hal, see @ref hal_key_deriv_funcs_t.
 * @return Interface between key_deriv driver and key_deriv hal, see @ref hal_key_deriv_funcs_t.
 * @else
 * @brief  获取Driver层和HAL层key_deriv的接口实例，参考 @ref hal_key_deriv_funcs_t.
 * @return Driver层和HAL层key_deriv的接口实例，参考 @ref hal_key_deriv_funcs_t.
 * @endif
 */
hal_key_deriv_funcs_t *hal_key_deriv_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif