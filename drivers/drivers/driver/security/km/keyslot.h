/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides key slot driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef KEYSLOT_H
#define KEYSLOT_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup drivers_driver_km Keyslot
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief Keyslot types supported by the KM module
 * @else
 * @brief KM模块支持的keyslot类型
 * @endif
 */
typedef enum {
    DRV_KEYSLOT_TYPE_MCIPHER = 0x00,
    DRV_KEYSLOT_TYPE_HMAC,
    DRV_KEYSLOT_TYPE_MAX,
    DRV_KEYSLOT_TYPE_INVALID = 0xffffffff,
} drv_keyslot_type_t;

/**
 * @if Eng
 * @brief  Create a Keyslot Handle
 * @param  [out] hkeyslot keyslot Handle
 * @param  [in]  keyslot_type keyslot type
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  创建keyslot句柄。
 * @param  [out] hkeyslot keyslot句柄
 * @param  [in]  keyslot_type keyslot类型
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_keyslot_create(uintptr_t *hkeyslot, drv_keyslot_type_t keyslot_type);

/**
 * @if Eng
 * @brief  Destroy the keylot handle
 * @param  [in] hkeyslot keyslot Handle
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  销毁keyslot句柄。
 * @param  [in] hkeyslot keyslot句柄
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_keyslot_destroy(uintptr_t hkeyslot);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
