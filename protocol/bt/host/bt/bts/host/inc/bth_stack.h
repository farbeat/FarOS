/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: bth stack struct
 * Author: @CompanyNameTag
 * Create: 2022-03-01
 */
#ifndef _BTH_STACK_H_
#define _BTH_STACK_H_

#include "custdefs.h"
#include "stdint.h"

#ifndef BD_ADDR_LEN
#define BD_ADDR_LEN 6
#endif
#pragma pack (1)

#define BD_CONFIG_SYNC_DATA_SIZE_MAX 1000

typedef enum {
    BTH_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS =                             0x00,
    BTH_ADDRESS_TYPE_RANDOM_DEVICE_ADDRESS =                             0x01,
    BTH_ADDRESS_TYPE_PUBLIC_IDENTITY_ADDRESS =                           0x02,
    BTH_ADDRESS_TYPE_RANDOM_STATIC_IDENTITY_ADDRESS =                    0x03
} bth_address_type;

typedef enum {
    BTH_ADDRESS_CAP_LE =                  0x01,
    BTH_ADDRESS_CAP_BREDR =               0x02,
    BTH_ADDRESS_CAP_IDENTITY =            0x04,
    BTH_ADDRESS_CAP_MAIN =                (BTH_ADDRESS_CAP_BREDR | BTH_ADDRESS_CAP_LE),
    BTH_ADDRESS_CAP_80211 =               0x08,
    BTH_ADDRESS_CAP_UWB =                 0x10,
    BTH_ADDRESS_CAP_AMP =                 (BTH_ADDRESS_CAP_80211 | BTH_ADDRESS_CAP_UWB),

    BTH_ADDRESS_CAP_LELINK_UPDATE =       0x80,
    BTH_ADDRESS_CAP_DONOT_CHANGE_NOTIFY = 0x40,
    BTH_ADDRESS_CAP_INFO =                (BTH_ADDRESS_CAP_LELINK_UPDATE | BTH_ADDRESS_CAP_DONOT_CHANGE_NOTIFY)
} bth_address_cap;

typedef enum {
    BLE_UNPAIRED = 0x00,
    BLE_PAIR_SUCCESS,
    BLE_PAIR_FAILED
} ble_pair_status;

typedef enum {
    BLE_STATE_DISCONNECTED = 0x00, /* disconnected */
    BLE_STATE_CONNECTED,           /* connected */
    BLE_STATE_CONNECTING,           /* connected */
} ble_conn_state;

typedef struct {
    uint16_t data_len;
    uint8_t data[0];
} bth_data_t;

typedef struct {
    uint8_t bd[BD_ADDR_LEN];              /* 6octets, 48bits */
    uint8_t atype;                        /* enum bth_address_type */
    uint8_t cap;                          /* enum bth_address_cap */
} bth_address;

#pragma pack ()
#include "bth_stack_hid.h"

#endif /* _BTH_STACK_H_ */
