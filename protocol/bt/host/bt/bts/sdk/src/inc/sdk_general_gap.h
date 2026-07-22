/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ble connection general dispatch for gap
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef BLE_CONN_GENERAL_GAP_H
#define BLE_CONN_GENERAL_GAP_H

#include "custdefs.h"
#include "mpc_dispatch.h"
#include "sdk_ble.h"
#include "sdk_list.h"
#include "bth_stack.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BT_AT_SCANNING_PHYS_1M             0x01
#define BT_AT_TGAP_LIM_DISC_SCAN_INT       17   /* 0x0011, 11.250ms, U:0.625 */
#define BT_AT_TGAP_SCAN_SLOW_INTERVAL1     2048 /* 0x0800, 1.280s,   U:0.625 */
#define BT_AT_TGAP_SCAN_SLOW_WINDOW1       17   /* 0x0011, 11.250ms, U:0.625 */
#define BT_AT_TGAP_SCAN_FAST_INTERVAL_MIN  48   /* 0x0030, 30.000ms, U:0.625 */
#define BT_AT_TGAP_SCAN_FAST_WINDOW        48   /* 0x0030, 30.000ms, U:0.625 */

#define BT_AT_TGAP_PRIMARY_ADV_INTERVAL_MIN  48   /* 0x0030, 30.000ms, U:0.625 */
#define BT_AT_TGAP_PRIMARY_ADV_INTERVAL_MAX  96   /* 0x0060, 60.000ms, U:0.625 */
#define BT_AT_TGAP_PRIMARY_ADV_CHANNEL_MAP   0x7
#define BT_AT_EXT_ADV_PHYS_1M                0x01 /* HCI_PRIMARY_ADVERTISING_PHY_1M */
#define BT_AT_FILTER_DUPLICATES_ENABLE       0x01 /* HCI_FILTER_DUPLICATES_DUPLICATE_FILTERING_ENABLED */
#define BT_AT_GAP_LE_EXT_ADV_FLAG_OWN_ADDR   16

enum {
    BT_AT_DISCOVERYMODE_NONE,
    BT_AT_DISCOVERYMODE_LIMITED,
    BT_AT_DISCOVERYMODE_GENERAL
};
enum {
    BT_AT_SPEED_FAST,
    BT_AT_SPEED_SLOW,
    BT_AT_SPEED_FAST_AUTO_SLOW
};
enum {
    BT_AT_SCAN_TYPE_PASSIVE_SCANNING,
    BT_AT_SCAN_TYPE_ACTIVE_SCANNING
};

enum {
    BT_AT_SCANNING_FILTER_POLICY_ACCEPT_ALL =                         0x00,
    BT_AT_SCANNING_FILTER_POLICY_ONLY_WHITE_LIST =                    0x01,
    BT_AT_SCANNING_FILTER_POLICY_ACCEPT_ALL_AND_RPA =                 0x02,
    BT_AT_SCANNING_FILTER_POLICY_ONLY_WHITE_LIST_AND_RPA =            0x03
};

enum {
    BT_AT_LE_DISCOVERYMODE_NONE,
    BT_AT_LE_DISCOVERYMODE_LIMITED,
    BT_AT_LE_DISCOVERYMODE_GENERAL
};

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
} ble_gap_local_data;

uint8_t ble_conn_general_gap_proc(const mpc_message *param);

int8_t mc_ble_smp_send_passkey_number(uint32_t passkey_num);
int8_t mc_ble_smp_set_key_from_btc_atb(uint8_t is_key_from_btc);
int8_t mc_ble_disable_adv_by_handle_atb(uint8_t adv_handle);
int8_t mc_ble_remove_adv_by_handle_atb(uint8_t adv_handle);

int8_t mc_ble_smp_remove_pair_device(uint8_t *addr);
int8_t mc_ble_disconnect_all_links(void);
int8_t mc_ble_smp_pair_remote_device(bth_address *addr);
int8_t mc_ble_smp_remove_pair_info(void);
uint8_t ble_gap_internal_callback_regist(uint16_t module_id,
                                         uint16_t event, sdk_ble_cbk cbk);
uint32_t mc_ble_set_ctkd_atb(bool params);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BLE_CONN_GENERAL_GAP_H */
