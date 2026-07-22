/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BLE GATT Service Server CallBack module.
 */

#ifndef BLE_GATTS_CALLBACKS_H
#define BLE_GATTS_CALLBACKS_H

#include "errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

errcode_t ble_server_register_gatt_callbacks(void);
errcode_t ble_client_register_gatt_callbacks(void);
uint32_t ble_gatt_mtu_exchange_result_get(void);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif
