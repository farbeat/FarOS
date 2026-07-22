/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Battery Service module.
 */

/**
 * @defgroup bluetooth_bts_bas BAS API
 * @ingroup  bluetooth
 * @{
 */

#ifndef BLE_BAS_H
#define BLE_BAS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Battery Service UUID.
 * @else
 * @brief  电池服务UUID。
 * @endif
 */
#define BLE_BAS_SERVICE_UUID 0x180F

/**
 * @if Eng
 * @brief  Battery Level UUID.
 * @else
 * @brief  电量值UUID。
 * @endif
 */
#define BLE_BAS_BATTERY_LEVEL_UUID 0x2A19

/**
 * @if Eng
 * @brief  Characteristic Presentation Format Descriptor UUID.
 * @else
 * @brief  特征表示格式描述符UUID。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_UUID 0x2904

/**
 * @if Eng
 * @brief  Client Characteristic Configuration Descriptor UUID.
 * @else
 * @brief  客户端特征配置描述符UUID。
 * @endif
 */
#define BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_UUID 0x2902

/**
 * @if Eng
 * @brief  Format of Characteristic Presentation Format is unsigned 8­bit integer.
 * @else
 * @brief  特征表示格式描述符的格式为无符号8位二进制整数。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_FORMAT 0x04

/**
 * @if Eng
 * @brief  Exponent of Characteristic Presentation Format is zero,
           The exponent field is only used on integer format types,
           the actual value is a combination of the Characteristic Value and the value 10 to the power Exponent,
           battery level range is 0 to 100,so zero is enough.
 * @else
 * @brief  特征表示格式描述符的指数为0,该指数仅用于整数类型，
           实际值计算时为特征值与10的指数相乘，电量值范围为0-100，不需要指数，该字段设置为0即可。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_EXPONENT 0x00

/**
 * @if Eng
 * @brief  Unit of Characteristic Presentation Format UUID, percentage.
 * @else
 * @brief  特征表示格式描述符的单位UUID，代表百分比。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_UNIT_UUID 0x27AD

/**
 * @if Eng
 * @brief  Namespace of Characteristic Presentation Format is Bluetooth SIG.
           The Name Space field is used to identify the organization,
           that is responsible for defining the enumerations for the description field
           Bluetooth organization define is Bluetooth SIG.
 * @else
 * @brief  特征表示格式描述符的命名空间为蓝牙SIG。命名空间用于标识一个组织，该组织负责定义描述字段列表。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_NAME_SPACE 0x01

/**
 * @if Eng
 * @brief  Description of Characteristic Presentation Format UUID,internal.
           The Description is an enumerated value as defined from the organization identified by the Name Space field.
 * @else
 * @brief  特征表示格式描述符的描述为内部。由命名空间组织定义，参照蓝牙SIG官网。
 * @endif
 */
#define BLE_BAS_CHARACTERISTIC_FORMAT_DESCRIPTION_UUID 0x010F

/**
 * @if Eng
 * @brief  Default value of Client Characteristic Configuration.
 * @else
 * @brief  客户端特征配置描述符的默认值。
 * @endif
 */
#define BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT 0x0000

/**
 * @if Eng
 * @brief  Client Characteristic Configuration value is notification.
 * @else
 * @brief  客户端特征配置描述符的值为通知。
 * @endif
 */
#define BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION 0x0001

/**
 * @if Eng
 * @brief  Function execute result notify upper application.
 * @par Description:Function execute result notify upper application.
 * @attention  NULL
 * @param  [in]  result  execute result, ERRCODE_BT_SUCCESS,ERRCODE_BT_FAIL.
 * @retval NULL
 * @par Dependency:
 * @li ble_bas.h
 * @else
 * @brief  函数执行结果告知上层应用。
 * @par 说明:函数执行结果告知上层应用。
 * @attention  NULL
 * @param  [in]  result  执行结果, ERRCODE_BT_SUCCESS 成功，ERRCODE_BT_FAIL 失败。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas.h
 * @endif
 */
typedef void (*ble_bas_func_result_cbk_t)(uint8_t result);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif