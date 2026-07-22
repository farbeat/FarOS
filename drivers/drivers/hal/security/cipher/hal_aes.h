/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides aes hal drivers header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#ifndef HAL_AES_H
#define HAL_AES_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_hal_aes_regs_op aes aes Regs operation
 * @ingroup  drivers_hal_aes
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of AES word mode.
 * @else
 * @brief  AES算法类型定义
 * @endif
 */
typedef enum {
    AES_ECB = 0,
    AES_CBC = 1,
    AES_OFB = 3,
    AES_CTR = 4,
    AES_CCM = 5,
    AES_GCM = 6,
    AES_MAX
} aes_mode_t;

/**
 * @if Eng
 * @brief  Definition of AES key length
 * @else
 * @brief  AES密钥长度类型
 * @endif
 */
typedef enum {
    AES_LENGTH_128 = 0,
    AES_LENGTH_192 = 1,
    AES_LENGTH_256 = 2,
} aes_key_length_t;

/**
 * @if Eng
 * @brief  Definition of AES word mode.
 * @else
 * @brief  AES算法类型定义
 * @endif
 */
typedef union {
    struct {
        uint32_t aes_operation : 1;
        uint32_t aes_mode : 3;
        uint32_t aes_key_length : 2;
        uint32_t aes_key_sel : 1;
        uint32_t aes_key_id : 3;
        uint32_t aes_reserved : 22;
    } aes_cfg_para;
    uint32_t aes_cfg;
} hal_aes_cfg_reg_t;

/**
 * @if Eng
 * @brief  Definition of AES attributes.
 * @else
 * @brief  Aes基础配置参数定义
 * @endif
 */
typedef struct {
    hal_aes_cfg_reg_t input_cfg;                /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES输入基础配置 @endif */
    uint32_t data_len;                          /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES数据长度 @endif */
    uint32_t aad_len;                           /*!< @if Eng Init aes interface.
                                                         @else   HAL层ADD长度 @endif */
    uint32_t key_start_addr;                    /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES密钥输入地址 @endif */
    uint32_t iv_start_addr;                     /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES IV输入地址 @endif */
    uint32_t auth_iv_start_addr;                /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES IV输入地址 @endif */
    uint32_t aad_start_addr;                    /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES AAD输入地址 @endif */
    uint32_t data_in_start_addr;                /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES输入地址 @endif */
    uint32_t data_out_start_addr;               /*!< @if Eng Init aes interface.
                                                         @else   HAL层AES输出地址 @endif */
    uint32_t tag_start_addr;                    /*!< @if Eng Init aes interface.
                                                         @else   HAL层tag输入地址 @endif */
} aes_attr_t;

/**
 * @if Eng
 * @brief  Callback of aes aes.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  AES的回调函数
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_aes_callback_t)(void);

/**
 * @if Eng
 * @brief  Init device for hal aes.
 * @param  [in] callback, aes interrupt callback func, see @ref hal_aes_callback_t
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  HAL层AES的初始化接口
 * @param  [in] callback, aes中断回调函数, 参考 @ref hal_aes_callback_t
 * @retval ERRCODE_SUCC   成功.
 * @retval Other        失败. 详细参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_aes_init_t)(hal_aes_callback_t callback);

/**
 * @if Eng
 * @brief  DeInit device for hal aes.
 * @else
 * @brief  HAL层AES的去初始化接口
 * @endif
 */
typedef void (*hal_aes_deinit_t)(void);

/**
 * @if Eng
 * @brief  Start device for hal aes.
 * @param  [in] aes_attr aes attribute. see @ref aes_attr_t
 * @else
 * @brief  HAL层AES算法的启动接口
 * @param  [in] aes_attr aes配置属性. 参考 @ref aes_attr_t
 * @endif
 */
typedef void (*hal_aes_start_t)(aes_attr_t aes_attr);

/**
 * @if Eng
 * @brief  Finish device for hal aes.
 * @param  [in] aes_mode worke mode of the aes algorithm. see @ref aes_mode_t
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t
 * @return aes is finish or not.
 * @else
 * @brief  HAL层AES算法的结束接口
 * @retval ERRCODE_SUCC   成功.
 * @retval Other          失败. 详细参考 @ref errcode_t
 * @endif
 */
typedef errcode_t (*hal_aes_get_result_t)(aes_mode_t aes_mode, bool *auth_result);

/**
 * @if Eng
 * @brief  Interface between aes driver and aes hal.
 * @else
 * @brief  Driver层HASH和HAL层AES的接口
 * @endif
 */
typedef struct {
    hal_aes_init_t   init;                /*!< @if Eng Init aes interface.
                                               @else   HAL层AES的初始化接口 @endif */
    hal_aes_deinit_t deinit;              /*!< @if Eng Deinit aes interface.
                                               @else   HAL层AES去初始化接口 @endif */
    hal_aes_start_t  start;               /*!< @if Eng aes start interface.
                                               @else   HAL层AES启动接口 @endif */
    hal_aes_get_result_t get_result;      /*!< @if Eng aes get result interface.
                                               @else   HAL层AES获取结果 @endif */
} hal_aes_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_aes_funcs_t into the g_hal_aess_funcs.
 * @param  [in]  funcs Interface between aes driver and aes hal. see @ref hal_aes_funcs_t
 * @retval ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  注册 @ref hal_aes_funcs_t 到 g_hal_aess_funcs
 * @param  [in]  funcs Driver层AES和HAL层AES的接口实例, 参考hal_aes_funcs_t
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t hal_aes_register_funcs(hal_aes_funcs_t *funcs);

/**
 * @if Eng
 * @brief  Unregister @ref hal_aes_funcs_t from the g_hal_aes_funcs.
 * @return ERRCODE_SUCC   Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  从g_hal_aess_funcs注销 @ref hal_aes_funcs_t
 * @return ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
void hal_aes_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between aes driver and aes hal, see @ref hal_aes_funcs_t.
 * @return Interface between aes driver and aes hal, see @ref hal_aes_funcs_t.
 * @else
 * @brief  获取Driver层AES和HAL层AES的接口实例，参考 @ref hal_aes_funcs_t.
 * @return Driver层AES和HAL层AES的接口实例，参考 @ref hal_aes_funcs_t.
 * @endif
 */
hal_aes_funcs_t *hal_aes_get_funcs(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
