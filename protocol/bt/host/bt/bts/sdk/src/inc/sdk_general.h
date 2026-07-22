/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ble connection general dispatch
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef BLE_CONN_GENERAL_H
#define BLE_CONN_GENERAL_H

#include "sdk_list.h"
#include "sdk_ble.h"
#include "sdk_ble_gap.h"
#include "sdk_general_l2cap.h"
#include "sdk_general_gatt.h"
#include "sdk_general_gap.h"
#include "sdk_general_hid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

enum {
    SDK_MOUDLE_GROUP_EXTERNAL = 0,
    SDK_MOUDLE_GROUP_INTERNAL,

    /* reserved extension */

    SDK_MOUDLE_GROUP_MAX,
};

enum {
    SDK_MOUDLE_ID_EXTERNAL = 0,
    SDK_MOUDLE_ID_INTERNAL_GAP,
    SDK_MOUDLE_ID_INTERNAL_L2CAP,
    SDK_MOUDLE_ID_INTERNAL_GATT,
    SDK_MOUDLE_ID_INTERNAL_SMP,
    SDK_MOUDLE_ID_INTERNAL_TEST_GAP,
    SDK_MOUDLE_ID_INTERNAL_TEST_L2CAP,
    SDK_MOUDLE_ID_INTERNAL_TEST_GATT,
    SDK_MOUDLE_ID_INTERNAL_TEST_SMP,
    SDK_MOUDLE_ID_INTERNAL_AT_GAP,
    SDK_MOUDLE_ID_INTERNAL_AT_L2CAP,
    SDK_MOUDLE_ID_INTERNAL_AT_GATT,
    SDK_MOUDLE_ID_INTERNAL_AT_SMP,

    /* reserved extension */

    SDK_MOUDLE_ID_MAX,
};

typedef struct {
    uint16_t table_id;
    uint16_t type;
    struct list_stru node_list;
} ble_general_cbk_table;

typedef struct {
    uint16_t event;
    sdk_ble_cbk cbk;
} ble_general_cbk_node;

typedef struct {
    uint16_t event;
    void *cbk;
} sdk_ble_general_cbk;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BLE_CONN_GENERAL_H */
