/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: BT FMP-TARGET module.
 */

/**
 * @defgroup bluetooth_ble_fmp_target FMP TARGET API
 * @ingroup bluetooth
 * @{
 */

#ifndef BLE_FMP_TARGET_H
#define BLE_FMP_TARGET_H

#include <stdint.h>
#include "ble_fmp.h"
#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Callback invoked when recieving an immediate alert from FMP Locater.
 * @param  [in]  value    alert level characteristic value. eg #FMP_ALERT_LEVEL_HIGH_ALERT
 * @retval #void    no return value.
 * @par Dependency:
 * @li ble_fmp.h
 * @else
 * @brief  收到对端报警请求后的回调函数。
 * @param  [in]  value    报警级别特征值。例如 #FMP_ALERT_LEVEL_HIGH_ALERT
 * @retval #void    无返回值。
 * @par 依赖：
 * @li ble_fmp.h
 * @endif
 */
typedef void (*fmp_target_evt_handler_t)(uint8_t value);

/**
 * @if Eng
 * @brief  Function for initializing the FMP Target.
 * @param  [in]  handler  Event handler to be called for handling write request.
 * @retval ERRCODE_BT_SUCCESS    Success.
 * @retval Others               Failure.
 * @else
 * @brief  初始化 FMP Target。
 * @param  [in]  handler  收到写请求后的事件处理钩子。
 * @retval ERRCODE_BT_SUCCESS     成功返回#ERRCODE_BT_SUCCESS
 * @retval Others                失败返回其他值
 * @endif
 */
errcode_t fmp_target_init(fmp_target_evt_handler_t evt_hdl);

/**
 * @if Eng
 * @brief  Function for add the FMP Target characteristic.
 * @param  [in]  server_id  Gatt server id.
 * @param  [in]  srvc_handle  Fmp target service handle.
 * @retval ERRCODE_BT_SUCCESS    Success.
 * @retval Others               Failure.
 * @else
 * @brief  初始化 FMP Target。
 * @param  [in]  server_id  Gatt server id.
 * @param  [in]  srvc_handle  Fmp 服务handle.
 * @retval ERRCODE_BT_SUCCESS     成功返回#ERRCODE_BT_SUCCESS
 * @retval Others                失败返回其他值
 * @endif
 */
errcode_t fmp_target_add_alc_characteristic(uint8_t server_id, uint16_t srvc_handle);

void fmp_target_add_characteristic_cbk(uint16_t service_handle, uint16_t character_handle, uint16_t value_handle);

void fmp_target_start_service_cbk(uint8_t server_id, uint16_t handle);

void fmp_target_write_req_cbk(uint8_t server_id, uint16_t handle, uint8_t value);

#ifdef __cplusplus
}
#endif
#endif

/** @} */
