/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT HID Service Server module.
 */

/**
 * @defgroup bluetooth_bts_hid_server HID SERVER API
 * @ingroup  bluetooth
 * @{
 */
#ifndef BLE_HID_SERVER_H
#define BLE_HID_SERVER_H

#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Human Interface Device UUID */
#define BLE_UUID_HUMAN_INTERFACE_DEVICE 0x1812
/* HID Information UUID */
#define BLE_UUID_HID_INFORMATION 0x2A4A
/* Report Map UUID */
#define BLE_UUID_REPORT_MAP 0x2A4B
/* HID Control Point UUID */
#define BLE_UUID_HID_CONTROL_POINT 0x2A4C
/* Boot Keyboard Input Report UUID */
#define BLE_UUID_BOOT_KEYBOARD_INPUT_REPORT 0x2A22
/* Boot Keyboard Output Report UUID */
#define BLE_UUID_BOOT_KEYBOARD_OUTPUT_REPORT 0x2A32
/* Report UUID */
#define BLE_UUID_REPORT 0x2A4D
/* Protocol Mode UUID */
#define BLE_UUID_PROTOCOL_MODE 0x2A4E
/* Client Characteristic Configuration UUID */
#define BLE_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION 0x2902
/* External Report Reference UUID */
#define BLE_UUID_EXTERNAL_REPORT_REFERENCE 0x2907
/* Report Reference UUID */
#define BLE_UUID_REPORT_REFERENCE 0x2908
/* HID character value handle offset */
#define BLE_HID_CHARACTER_INPUT_REPORT_VALUE_HANDLE_OFFSET 8

/**
 * @if Eng
 * @brief  Use this funtion to register callback function of framework.
 * @par Description:Use this funtion to register callback function of framework.
 * @attention  NULL
 * @param  NULL
 * @retval NULL
 * @par Dependency:
 * @li ble_hid_server.h
 * @else
 * @brief  注册上层应用的回调。
 * @par 说明:注册上层应用的回调。
 * @attention  NULL
 * @param  NULL
 * @retval NULL
 * @par 依赖:
 * @li ble_hid_server.h
 * @endif
 */
void ble_hid_server_register_callbacks(void);

/**
 * @if Eng
 * @brief  Use this funtion to init HID service.
 * @par Description:add HID service, character and character descriptor, then start service.
 * @attention  NULL
 * @param  NULL
 * @retval NULL
 * @par Dependency:
 * @li ble_hid_server.h
 * @else
 * @brief  初始化HID服务。
 * @par 说明:添加HID服务，特征，特征描述符，然后启动服务。
 * @attention  NULL
 * @param  NULL
 * @retval NULL
 * @par 依赖:
 * @li ble_hid_server.h
 * @endif
 */
void ble_hid_server_init(void);

/**
 * @if Eng
 * @brief  Notify remote device the press button value.
 * @par Description:Notify remote device the press button value..
 * @attention  NULL
 * @param  [in]  value  Press button value.
 * @param  [in]  len    Length of press button value。
 * @retval ERRCODE_BT_SUCCESS    Excute successfully
 * @retval ERRCODE_BT_FAIL       Execute fail
 * @par Dependency:
 * @li ble_hid_server.h
 * @else
 * @brief  通知对端按键变化。
 * @par 说明：通知对端按键变化。
 * @attention  NULL
 * @param  [in]  data   按键值。
 * @param  [in]  len    按键值数据长度。
 * @retval ERRCODE_BT_SUCCESS    执行成功
 * @retval ERRCODE_BT_FAIL       执行失败
 * @par 依赖:
 * @li ble_hid_server.h
 * @endif
 */
errcode_t ble_hid_server_send_input_report(const uint8_t *data, uint8_t len);

/**
 * @if Eng
 * @brief  Hid add characters and descriptors.
 * @par Description:Hid add characters and descriptors.
 * @attention  NULL
 * @param  [in]  server_id  server id.
 * @param  [in]  srvc_handle    service handle.
 * @retval NULL
 * @par Dependency:
 * @li ble_hid_server.h
 * @else
 * @brief  HID 服务添加特征和描述符。
 * @par 说明：HID 服务添加特征和描述符。
 * @attention  NULL
 * @param  [in]  server_id   服务id。
 * @param  [in]  srvc_handle    服务handle。
 * @retval NULL
 * @par 依赖:
 * @li ble_hid_server.h
 * @endif
 */
void ble_hid_add_characters_and_descriptors(uint8_t server_id, uint16_t srvc_handle);

void ble_hid_add_characters_input_report_cbk(uint16_t srvc_handle);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif
