/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Battery Service Server module.
 */

/**
 * @defgroup bluetooth_bts_bas_server BAS SERVER API
 * @ingroup  bluetooth
 * @{
 */

#ifndef BLE_BAS_SERVER_H
#define BLE_BAS_SERVER_H
#include "ble_bas.h"
#include "errcode.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Battery level max value is 100.
 * @else
 * @brief  电量值的最大值为100。
 * @endif
 */
#define BLE_BAS_BATTERY_LEVEL_MAX 100

/**
 * @if Eng
 * @brief  Battery level default value is 85.
 * @else
 * @brief  电量值的默认值为85。
 * @endif
 */
#define BLE_BAS_BATTERY_LEVEL_DEFAULT 85

/**
 * @if Eng
 * @brief  Battery Service data.
 * @else
 * @brief  电池服务数据。
 * @endif
 */
typedef struct {
    uint8_t battery_level;
    uint16_t start_hdl;              /*!< @if Eng Bas service attribute handle. @else bas服务属性handle @endif */
    ble_bas_func_result_cbk_t cbk;   /*!< @if Eng Bas service init execute result. @else 服务初始化函数执行结果 @endif */
} ble_bas_data_t;

/**
 * @if Eng
 * @brief  Use this funtion to init battery service.
 * @par Description:add battery service,battery level character and character descriptor,then start service.
 * @attention  NULL
 * @param  [in]  cbk       register execute result callback
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_server.h | ble_bas.h
 * @else
 * @brief  初始化电池服务。
 * @par 说明:添加电池服务，电量特征，电量特征的描述符，然后启动电池服务。
 * @attention  NULL
 * @param  [in]  cbk       注册执行结果回调函数
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_server.h | ble_bas.h
 * @endif
 */
void ble_bas_server_init(ble_bas_func_result_cbk_t cbk);

/**
 * @if Eng
 * @brief  Use this funtion to get battery level caching.
 * @par Description:get battery level caching value,caching record battery level change,.
 * @attention  NULL
 * @param  NULL
 * @retval uint8_t battery level value
 * @par Dependency:
 * @li ble_bas_server.h
 * @else
 * @brief  获取电量缓存值。
 * @par 说明:获取电量缓存值,缓存用于记录电量值变化。
 * @attention  NULL
 * @param  NULL
 * @retval uint8_t 电量值
 * @par 依赖:
 * @li ble_bas_server.h
 * @endif
 */
uint8_t ble_bas_server_battery_level_get(void);

/**
 * @if Eng
 * @brief  Set caching value and When battery level is changed, notify remote device.
 * @par Description:set caching value and When battery level is changed,notify remote device.
 * @attention  NULL
 * @param  [in]  value,battery level,value range is 0 to 100.
 * @retval ERRCODE_BT_SUCCESS Excute successfully
 * @retval ERRCODE_BT_FAIL    When value is out of range or value is not changed,execute fail
 * @par Dependency:
 * @li ble_bas_server.h
 * @else
 * @brief  设置缓存值，当电量值变化时，通知对端。
 * @par 说明:设置缓存值，当电量值变化时，通知对端。
 * @attention  NULL
 * @param  [in]  value,电量值,值范围为0到100.
 * @retval ERRCODE_BT_SUCCESS 执行成功
 * @retval ERRCODE_BT_FAIL    当值超过范围或者值没有变化时，执行失败
 * @par 依赖:
 * @li ble_bas_server.h
 * @endif
 */
errcode_t ble_bas_server_battery_level_notify(uint8_t value);

/**
 * @if Eng
 * @brief  The callback of bas service start.
 * @par Description:The callback of bas service start.
 * @attention  NULL
 * @param  [in]  server_id  Gatt server id.
 * @param  [in]  handle    Bas service handle.
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_server.h
 * @else
 * @brief  启动BAS服务回调函数。
 * @par 说明：启动BAS服务回调函数。
 * @attention  NULL
 * @param  [in]  server_id   Gatt server id。
 * @param  [in]  handle    BAS服务handle。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_server.h
 * @endif
 */
void ble_bas_start_service_cbk(uint8_t server_id, uint16_t handle);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif