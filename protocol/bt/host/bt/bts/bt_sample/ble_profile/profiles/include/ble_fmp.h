/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: BT FMP module.
 */

/**
 * @defgroup bluetooth_ble_fmp FMP API
 * @ingroup bluetooth
 * @{
 */

#ifndef BLE_FMP_H
#define BLE_FMP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Immediate Alert Service UUID.
 * @else
 * @brief  立即警报服务 UUID。
 * @endif
 */
#define FMP_IMMEDIATE_ALERT_SERVICE_UUID 0x1802

/**
 * @if Eng
 * @brief  Alert Level Characteristic UUID.
 * @else
 * @brief  报警级别特征 UUID。
 * @endif
 */
#define FMP_ALERT_LEVEL_CHARACTERISTIC_UUID 0x2A06


/**
 * @if Eng
 * @brief  Alert level values.
 * @else
 * @brief  报警级别特征值。
 * @endif
 */
#define FMP_ALERT_LEVEL_NO_ALERT    0x00
#define FMP_ALERT_LEVEL_MILD_ALERT  0x01
#define FMP_ALERT_LEVEL_HIGH_ALERT  0x02

#ifdef __cplusplus
}
#endif
#endif

/** @} */
