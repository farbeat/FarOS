/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Battery Service Client module.
 */

/**
 * @defgroup bluetooth_bts_bas_client BAS CLIENT API
 * @ingroup  bluetooth
 * @{
 */

#ifndef BLE_BAS_CLIENT_H
#define BLE_BAS_CLIENT_H
#include "errcode.h"
#include "osal_list.h"
#include "bts_gatt_client.h"
#include "ble_bas.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @if Eng
 * @brief  Enum of batttery level notify switch.
 * @else
 * @brief  电量通知开关枚举值。
 * @endif
 */
typedef enum {
    BAS_CLIENT_CCCD_NOTIFY_DISABLE, /*!< @if Eng disable notify @else 关闭通知 @endif */
    BAS_CLIENT_CCCD_NOTIFY_ENABLE,  /*!< @if Eng enable notify @else 开启通知 @endif */
} bas_client_cccd_notify_enable_t;

/**
 * @if Eng
 * @brief  Enum of batttery level descriptor.
 * @else
 * @brief  电量特征的描述符枚举值。
 * @endif
 */
typedef enum {
    BAS_CHARACTERISTIC_FORMAT_DESCRIPTOR,               /*!< @if Eng Characteristic Presentation Format
                                                             @else   特征表示格式描述符 @endif */
    BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR, /*!< @if Eng Client Characteristic Configuration
                                                             @else   客户端特征配置描述符 @endif */
    BAS_CHAR_BUTT,
} bas_client_character_descriptor_t;

/**
 * @if Eng
 * @brief  Characteristic Presentation Format struct.
 * @else
 * @brief  电量特征表示格式结构。
 * @endif
 */
typedef struct {
    uint8_t format;            /*!< @if Eng Format @else 特征格式 @endif */
    uint8_t exponent;          /*!< @if Eng The exponent field is only used on integer format types,
                                            the actual value is a combination of the Characteristic Value
                                            and the value 10 to the power Exponent,For example,
                                            if the Exponent is 2 and the Characteristic Value is 23,
                                            the actual value would be 2300.
                                    @else   该指数仅用于整数类型，实际值计算时为特征值与10的指数相乘的结果。 @endif */
    bt_uuid_t unit;            /*!< @if Eng Unit,for example battery level unit is percentage,value is UUID
                                            {@link bth_gatt_uuid}.
                                    @else   单位，例如电量的单位是百分比。 @endif */
    uint8_t name_space;        /*!< @if Eng The Name Space field is used to identify the organization,
                                            that is responsible for defining the enumerations
                                            for the description field.
                                    @else 命名空间用于标识一个组织，默认是蓝牙SIG,参照蓝牙SIG官网 @endif */
    bt_uuid_t description;     /*!< @if Eng The Description is an enumerated value as defined
                                            from the organization identified by the Name Space field.
                                            value is UUID {@link bth_gatt_uuid}
                                    @else 描述是由命名空间组织定义标识，参照蓝牙SIG官网。 @endif */
} bas_client_cfd_t;

/**
 * @if Eng
 * @brief  When uppler layer read characteristic presentation format,register this callback function.
 * @par Description:After read characteristic presentation format,call this function and return value to upper layer.
 * @attention  NULL
 * @param  [in]  cfd_info characteristic presentation format {@link bas_client_cfd_t}
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  上层应用读取特征表示格式描述符时，需要注册的回调函数
 * @par 说明:读取特征表示格式描述符完成后，调用该回调函数，将值返回给上层应用。
 * @attention  NULL
 * @param  [in]  cfd_info 特征表示格式描述符，参照{@link bas_client_cfd_t}。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
typedef void (*ble_bas_cfd_read_cbk_t)(bas_client_cfd_t cfd_info);

/**
 * @if Eng
 * @brief  Characteristic presentation format descriptor info of Batter level.
 * @else
 * @brief  电量特征的特征表示格式描述符信息。
 * @endif
 */
typedef struct {
    uint16_t descriptor_handle;  /*!< @if Eng descriptor handle. @else 描述符句柄。 @endif */
    bas_client_cfd_t cfd;        /*!< @if Eng descriptor data. @else 描述符数据。 @endif */
    ble_bas_cfd_read_cbk_t cbk;  /*!< @if Eng read descriptor callback function @else 读取描述符时的回调函数。 @endif */
} bas_client_cfd_info_t;

/**
 * @if Eng
 * @brief  When uppler layer read client characteristic configuration,register this callback function.
 * @par Description:After read client characteristic configuration,call this function and return value to upper layer.
 * @attention  NULL
 * @param  [in]  value  client characteristic configuration value.
                        {@link BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT}
                        {@link BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION}
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  上层应用读取客户端特征配置描述符时，需要注册的回调函数
 * @par 说明:读取客户端特征配置描述符完成后，调用该回调函数，将值返回给上层应用。
 * @attention  NULL
 * @param  [in]  value  客户端特征配置描述符的值。
                        {@link BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_DEFAULT}
                        {@link BLE_BAS_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATION}
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
typedef void (*ble_bas_cccd_read_cbk_t)(uint16_t value);

/**
 * @if Eng
 * @brief  Client characteristic configuration descriptor info of Batter level.
 * @else
 * @brief  电量特征的客户端特征配置描述符信息。
 * @endif
 */
typedef struct {
    uint16_t descriptor_handle;   /*!< @if Eng descriptor handle. @else 描述符句柄。 @endif */
    uint16_t value;               /*!< @if Eng descriptor data. @else 描述符数据。 @endif */
    ble_bas_cccd_read_cbk_t cbk;  /*!< @if Eng read descriptor callback function @else 读取描述符时的回调函数。 @endif */
} bas_client_cccd_info_t;

/**
 * @if Eng
 * @brief  When peer device battler level changed,notify client device,so register this callback function.
 * @par Description:When peer device battler level changed,notify client device,so register this callback function.
 * @attention  NULL
 * @param  [in]  value  battery level, value range is 0 to 100.
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  当对端设备电量值变化时，通知客户端，因此上层应用需要注册该回调函数，用来接收数据。
 * @par 说明:当对端设备电量值变化时，通知客户端，因此上层应用需要注册该回调函数，用来接收数据。
 * @attention  NULL
 * @param  [in]  value  value  电量值，值范围为0至100。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
typedef void (*ble_bas_notify_cbk_t)(uint8_t value);

/**
 * @if Eng
 * @brief  When uppler layer read battery level,register this callback function.
 * @par Description:After read battery level,call this function and return value to upper layer.
 * @attention  NULL
 * @param  [in]  value  battery level, value range is 0 to 100.
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  上层应用读取电量值时，需要注册的回调函数
 * @par 说明:读取电量值完成后，调用该回调函数，将值返回给上层应用。
 * @attention  NULL
 * @param  [in]  value  电量值，值范围为0至100。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
typedef void (*ble_bas_character_read_cbk_t)(uint8_t value);

/**
 * @if Eng
 * @brief  Battery level character info.
 * @else
 * @brief  电量特征信息。
 * @endif
 */
typedef struct {
    uint16_t declare_handle;                /*!< @if Eng Declare handle. @else 特征声明句柄。 @endif */
    uint16_t value_handle;                  /*!< @if Eng Value handle. @else 特征值句柄。 @endif */
    uint8_t value;                          /*!< @if Eng Value. @else 特征值。 @endif */
    ble_bas_notify_cbk_t notify_cbk;        /*!< @if Eng Notify callback function. @else 通知回调函数。 @endif */
    ble_bas_character_read_cbk_t read_cbk;  /*!< @if Eng Read callback function. @else 读取回调函数。 @endif */
    bas_client_cfd_info_t cfd_info;         /*!< @if Eng Characteristic presentation format descriptor info
                                                         {@link bas_client_cfd_info_t}.
                                                 @else   特征表示格式描述符信息。 @endif */
    bas_client_cccd_info_t cccd_info;       /*!< @if Eng Client characteristic configuration descriptor info
                                                         {@link bas_client_cccd_info_t}.
                                                 @else   客户端特征配置描述符信息。 @endif */
    uint8_t properties;      /*!< @if Eng character properties { @ref gatt_characteristic_property_t }.
                                  @else   特征特性 { @ref gatt_characteristic_property_t }。@endif */
} bas_client_character_info_t;

/**
 * @if Eng
 * @brief  Peer device info.
 * @else
 * @brief  对端设备相关信息。
 * @endif
 */
typedef struct {
    struct osal_list_head node;
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t start_hdl;                     /*!< @if Eng start handle. @else 开始句柄 @endif */
    uint16_t end_hdl;                       /*!< @if Eng end handle. @else 结束句柄 @endif */
    bas_client_character_info_t character;  /*!< @if Eng battery level character info
                                                         {@link bas_client_character_info_t}.
                                                 @else   电量特征信息。 @endif */
    ble_bas_func_result_cbk_t cbk;   /*!< @if Eng Bas service init execute result. @else 服务发现执行结果 @endif */
} ble_bas_client_t;

/**
 * @if Eng
 * @brief  Discovery remote device battery service.
 * @par Description:Discovery remote device battery service.
 * @attention  NULL
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  cbk            The callback of result。
 * @retval NULL
 * @par Dependency:
 * @li ble_bas_client.h | ble_bas.h
 * @else
 * @brief  发现远端设备的电池服务。
 * @par 说明:发现远端设备的电池服务。
 * @attention  NULL
 * @param [in] client_id        gatt client注册返回client id。
 * @param [in] conn_id          执行BLE连接时回调返回的连接ID。
 * @param [in] cbk              结果回调。
 * @retval NULL
 * @par 依赖:
 * @li ble_bas_client.h | ble_bas.h
 * @endif
 */
errcode_t ble_bas_client_discovery_service(uint8_t client_id, uint16_t conn_id, ble_bas_func_result_cbk_t cbk);

/**
 * @if Eng
 * @brief  Read remote device battery level.
 * @par Description:Read remote device battery level.
 * @attention  NULL
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  read_cbk       register read battery level callback
 * @param  [in]  notify_cbk     register notify battery level callback
 * @retval {@link ERRCODE_BT_SUCCESS}          read success.
 * @retval others                              read fail.
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  读远端设备的电量。
 * @par 说明:读远端设备的电量。
 * @attention  NULL
 * @param [in] client_id        gatt client注册返回client id。
 * @param [in] conn_id          执行BLE连接时回调返回的连接ID。
 * @param  [in]  read_cbk       注册读电量值回调函数
 * @param  [in]  notify_cbk     注册通知电量值回调函数
 * @retval {@link ERRCODE_BT_SUCCESS}          读取成功.
 * @retval others                              读取失败.
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
errcode_t ble_bas_client_read_battery_level(uint8_t client_id, uint16_t conn_id,
    ble_bas_character_read_cbk_t read_cbk, ble_bas_notify_cbk_t notify_cbk);

/**
 * @if Eng
 * @brief  Read characteristic presentation format descriptor.
 * @par Description:Read remote device battery level characteristic presentation format descriptor
 * @attention  NULL
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  cfd_cbk       register read characteristic presentation format descriptor callback
 * @retval ERRCODE_BT_SUCCESS Excute successfully
 * @retval ERRCODE_BT_FAIL    When battery level character is not present,execute fail
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  读特征表示格式描述符。
 * @par 说明:读远端设备的电量特征描述符,特征表示格式描述符。
 * @attention  NULL
 * @param [in] client_id        gatt client注册返回client id。
 * @param [in] conn_id          执行BLE连接时回调返回的连接ID。
 * @param  [in]  cfd_cbk        注册读取特征表示格式描述符回调函数
 * @retval ERRCODE_BT_SUCCESS 执行成功
 * @retval ERRCODE_BT_FAIL    当没有读取到对端设备的电量特征时，执行失败。
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
errcode_t ble_bas_client_read_format_descriptor(uint8_t client_id, uint16_t conn_id,
    ble_bas_cfd_read_cbk_t cfd_cbk);

/**
 * @if Eng
 * @brief  Read client characteristic configuration descriptor..
 * @par Description:Read remote device battery level client characteristic configuration descriptor.
 * @attention  NULL
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  cccd_cbk      register read client characteristic configuration descriptor callback
 * @retval ERRCODE_BT_SUCCESS Excute successfully
 * @retval ERRCODE_BT_FAIL    When battery level character is not present,execute fail
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  读客户端特征配置描述符。
 * @par 说明:读远端设备的电量特征描述符,客户端特征配置描述符。
 * @attention  NULL
 * @param [in] client_id        gatt client注册返回client id。
 * @param [in] conn_id          执行BLE连接时回调返回的连接ID。
 * @param  [in]  cccd_cbk       注册读取客户端特征配置描述符回调函数
 * @retval ERRCODE_BT_SUCCESS 执行成功
 * @retval ERRCODE_BT_FAIL    当没有读取到对端设备的电量特征时，执行失败。
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
errcode_t ble_bas_client_read_ccc_descriptor(uint8_t client_id, uint16_t conn_id,
    ble_bas_cccd_read_cbk_t cccd_cbk);

/**
 * @if Eng
 * @brief  Close or open remote device battery level change notification.
 * @par Description:Close or open remote device battery level change notification.
 * @attention  NULL
 * @param  [in]  client_id      BLE gatt client register id
 * @param  [in]  conn_id        BLE connect id
 * @param  [in]  enable_flag       notification switch,{@link bas_client_cccd_notify_enable_t}
 * @retval ERRCODE_BT_SUCCESS Excute successfully
 * @retval ERRCODE_BT_FAIL    When client characteristic configuration descriptor is not supported,execute fail
 * @par Dependency:
 * @li ble_bas_client.h
 * @else
 * @brief  打开或关闭远端设备电量值变化通知。
 * @par 说明:打开或关闭远端设备电量值变化通知。
 * @attention  NULL
 * @param [in] client_id   gatt client注册返回client id。
 * @param [in] conn_id     执行BLE连接时回调返回的连接ID。
 * @param  [in]  cbk       通知开关，参照{@link bas_client_cccd_notify_enable_t}
 * @retval ERRCODE_BT_SUCCESS 执行成功
 * @retval ERRCODE_BT_FAIL    当对端设备不支持客户端特征配置描述符时，执行失败。
 * @par 依赖:
 * @li ble_bas_client.h
 * @endif
 */
errcode_t ble_bas_client_write_ccc_descriptor(uint8_t client_id, uint16_t conn_id,
    bas_client_cccd_notify_enable_t enable_flag);

/* 根据连接ID清理bas服务连接设备信息 */
void ble_bas_client_clear_remote_by_conn(uint8_t client_id, uint16_t conn_id);

/* 清理bas服务连接设备信息 */
void ble_bas_client_clear_remote_all(void);

/* 发现BAS服务回调函数 */
void ble_bas_client_discovery_service_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service);

/* 发现BAS特征回调函数 */
void ble_bas_client_discovery_character_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character);

/* 发现BAS描述符回调函数 */
void ble_bas_client_discovery_descriptor_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor);

/* 读BAS回调函数 */
void ble_bas_client_read_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status);

/* 特征值通知回调函数 */
void ble_bas_client_notify_characteristic_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif