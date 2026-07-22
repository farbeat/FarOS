/*
 * Copyright (c) @CompanyNameMagicTag 2024. All rights reserved.
 * Description: BTS FEATURE module.
 */

/**
 * @defgroup bluetooth_bts_feature BTS FEATURE API
 * @ingroup  bluetooth
 * @{
 */


#ifndef BTS_FEATURE_H
#define BTS_FEATURE_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"
#include "bts_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief Struct of BT stack feature.
 * @else
 * @brief 蓝牙特性位图。
 * @endif
 */
typedef struct {
    uint32_t bt_auto_reconnect_closed : 1;                    /*!< @if Eng Close BT auto reconnect feature.
                                                                   @else   关闭蓝牙自动回连特性。 @endif */
    uint32_t bt_a2dp_snk_closed : 1;                          /*!< @if Eng BT A2DP SNK feature.
                                                                   @else   蓝牙A2DP SNK特性。 @endif */
    uint32_t reserved : 30;                                   /*!< @if Eng Featrues reserved for future.
                                                                   @else   保留字段。 @endif */
} bts_feature_t;

/**
 * @if Eng
 * @brief Set BTS Features supported.
 * @par   Set BTS Features supported.
 * @attention NULL
 * @param  [in] feature #bts_feature_t, a bitmap indicate supported features.
 * @retval #BT_STATUS_SUCCESS   Excute successfully.
 * @retval #Other               Error code, @ref bt_status_t
 * @par Dependency:
 * @li  bts_feature.h
 * @see
 * @else
 * @brief  设置想要使用的特性。
 * @par    设置想要使用的特性。
 * @attention 无
 * @param  [in] 特性位图。 @ref bts_feature_t
 * @retval #BT_STATUS_SUCCESS   执行成功。
 * @retval #Other               错误码, @ref bt_status_t。
 * @par 依赖:
 * @li  bts_feature.h
 * @see
 * @endif
 */
errcode_t bts_set_features(bts_feature_t *feature);

/**
 * @if Eng
 * @brief Use this funtion to get BTS features supported.
 * @par  Use this funtion to get BTS features supported.
 * @attention NULL.
 * @param  NULL.
 * @retval BT features bitmap. see @ref bts_feature_t
 * @par Dependency:
 * @li  bts_feature.h
 * @else
 * @brief  获取BT协议栈支持的特性。
 * @par    获取BT协议栈支持的特性。
 * @attention 无
 * @param  无
 * @retval BT 特性位图。 参考 @ref bts_feature_t
 * @par 依赖:
 * @li  bts_feature.h
 * @endif
 */
bts_feature_t bts_get_features(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
