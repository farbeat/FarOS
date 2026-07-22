/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: BT FMP-LOCATOR module.
 */

/**
 * @defgroup bluetooth_ble_fmp_locator FMP LOCATOR API
 * @ingroup bluetooth
 * @{
 */

#ifndef BLE_FMP_LOCATOR_H
#define BLE_FMP_LOCATOR_H

#include <stdint.h>
#include "bts_gatt_client.h"
#include "ble_fmp.h"
#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Create connection with a FMP target.
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @retval ERRCODE_BT_SUCCESS    Success.
 * @retval Others               Failure.
 * @else
 * @brief  与 FMP 目标创建连接。
 * @param [in] client_id gatt client注册返回client id。
 * @param [in] conn_id   执行BLE连接时回调返回的连接ID。
 * @retval ERRCODE_BT_SUCCESS     成功返回#ERRCODE_BT_SUCCESS
 * @retval Others                失败返回其他值
 * @endif
 */
errcode_t fmp_locator_connect(uint8_t client_id, uint16_t conn_id);

/**
 * @if Eng
 * @brief  Send Alert Command to a FMP target.
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  alert_level    The Alert level. eg #FMP_ALERT_LEVEL_HIGH_ALERT
 * @retval ERRCODE_BT_SUCCESS    Success.
 * @retval Others               Failure.
 * @par Dependency:
 * @li ble_fmp.h
 * @else
 * @brief  向 FMP 目标发送报警指令。
 * @param [in] client_id        gatt client注册返回client id。
 * @param [in] conn_id          执行BLE连接时回调返回的连接ID。
 * @param  [in]  alert_level    报警级别。 例如 #FMP_ALERT_LEVEL_HIGH_ALERT
 * @retval ERRCODE_BT_SUCCESS     成功返回#ERRCODE_BT_SUCCESS
 * @retval Others                失败返回其他值
 * @par 依赖：
 * @li ble_fmp.h
 * @endif
 */
errcode_t fmp_locator_send_alert(uint8_t client_id, uint16_t conn_id, uint8_t alert_level);

/* 根据连接ID清理fmp服务连接设备信息 */
void ble_fmp_locator_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id);

/* 清理fmp服务连接设备信息 */
void ble_fmp_locator_clear_remote_all(void);

/* 发现fmp服务回调函数 */
void fmp_locator_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service);

/* 发现fmp特征回调函数 */
void fmp_locator_discovery_characteristic_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character);

#ifdef __cplusplus
}
#endif
#endif

/** @} */
