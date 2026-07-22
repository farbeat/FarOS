/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ble connection general dispatch for hid
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef BLE_CONN_GENERAL_HID_H
#define BLE_CONN_GENERAL_HID_H

#include "sdk_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * local_data hook struct
 * hook_list
 *      |->node_list.......
 *            |->cbk_node->cbk_node->cbk_node
 */
typedef struct {
    struct list_stru hook_list;
    struct list_stru conn_list;
    void *resv;
} ble_hid_local_data;

#define BT_AT_MAX_KEY_NUM 6
#define BT_AT_MAX_KEY_LEN 4 /* 0xXX(16进制) or 255(10进制) */
#define BT_AT_MIN_KEY_LEN 1 /* 0(10进制) */
#define BT_AT_MOUSE_LEN 4 /* sizeof(ble_hid_mouse_event_st) */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BLE_CONN_GENERAL_HID_H */
