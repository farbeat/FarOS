/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: host sdk stack api for gatt
 * Author: @CompanyNameTag
 * Create: 2022-03-10
 */
#ifndef SDK_BLE_GATT_EVENT_H
#define SDK_BLE_GATT_EVENT_H

#include "sdk_ble.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SDK_UUID_SHORT    1
#define SDK_UUID_LONG     0

#define UUID_SIZE 16
#define BD_ADDR_SIZE 6

typedef enum {
    /* server event */
    SDK_BLE_GATT_SERVER_EVENT_CREATE_SERVER = 0,
    SDK_BLE_GATT_SERVER_EVENT_CREATE_SERVICE,
    SDK_BLE_GATT_SERVER_EVENT_ADD_INCLUDE,
    SDK_BLE_GATT_SERVER_EVENT_ADD_CHARACTER,
    SDK_BLE_GATT_SERVER_EVENT_ADD_DESCRIPTOR,
    SDK_BLE_GATT_SERVER_EVENT_START_SERVICE,
    SDK_BLE_GATT_SERVER_EVENT_GET_UI_ATTRIBUTES,
    SDK_BLE_GATT_SERVER_EVENT_MTU_CHANGED_CBK,
    SDK_BLE_GATT_SERVER_EVENT_WRITE_IND,
    SDK_BLE_GATT_SERVER_EVENT_READ_IND,
    SDK_BLE_GATT_SERVER_EVENT_MAX,

    /* client event */
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_SERVICE,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_SERVICE_BY_UUID,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_INCLUDE,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_INCLUDE_BY_UUID,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_SERVICE_COMP,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_CHARACTER,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_CHARACTER_BY_UUID,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_CHARACTER_COMP,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_DESC,
    SDK_BLE_GATT_CLIENT_EVENT_DISCOVERY_DESC_COMP,
    SDK_BLE_GATT_CLIENT_EVENT_READ_CHARACTER,
    SDK_BLE_GATT_CLIENT_EVENT_READ_CHARACTER_COMP,
    SDK_BLE_GATT_CLIENT_EVENT_WRITE_CHARACTER,
    SDK_BLE_GATT_CLIENT_EVENT_READ_DESCRIPTOR,
    SDK_BLE_GATT_CLIENT_EVENT_WRITE_DESCRIPTOR,
    SDK_BLE_GATT_CLIENT_EVENT_CHARACTER_VALUE_NOTIFICATION,
    SDK_BLE_GATT_GET_HID_REPORT_MAP,
    SDK_BLE_GATT_CLIENT_EVENT_MAX,
} sdk_ble_gatt_cbk_event;

typedef struct {
    uint8_t addr[BD_ADDR_SIZE];
    uint16_t report_hdl;
    uint8_t uuid[UUID_SIZE];
    uint16_t type;
    uint16_t sevice_evt;
    uint16_t len;
    uint8_t data[0];
} sdk_ble_gatt_out_report_stru;

typedef enum {
    SDK_BLE_GATT_SERVER,
    SDK_BLE_GATT_CLIENT
} sdk_ble_gatt_role;

typedef void (*sdk_gatt)(uint16_t event, void *arg); /* Gatt callback function */
uint8_t ble_gatt_callback_register(sdk_ble_gatt_cbk_event event, sdk_ble_cbk cbk);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* SDK_BLE_GATT_EVENT_H */
