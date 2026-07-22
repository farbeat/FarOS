/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: bth mpc stack struct for gap
 * Author:
 * Create: 2022-03-01
 */
#ifndef _BTH_STACK_GAP_H_
#define _BTH_STACK_GAP_H_

#include "bth_stack.h"
#include "product_bth.h"

/* General Interface for Stack GAP */
#pragma pack (1)
#define BTH_GAP_BLE_ADV_HANDLE_DEFAULT 0x01
#define BTH_GAP_BLE_ADV_INTERIVAL_FAST 0xA0
#define BTH_GAP_BLE_ADV_FOREVER_DURATION 0
#define BTH_GAP_BLE_ADV_ALIPAY_SUPPORTED 0x01
#define BTH_GAP_BLE_ADV_HIBEACON_SUPPORTED 0x02
#define BTH_GAP_BLE_ADV_SELFIND_SUPPORTED 0x04
#define BTH_GAP_BLE_ADV_KEYBOARD_SUPPORTED 0x08
#define BTH_GAP_BLE_ADV_MOUSE_SUPPORTED 0x10
#define BTH_GAP_BLE_ADV_INTERVAL_MAX 0xFFFF /* ms */
#define BTH_GAP_BLE_ADV_INTERVAL_MIN 0x06
#define BTH_HCI_LE_EVENT_MASK_LEN 8
#define BTH_BLE_SMP_LTK_LEN 16

typedef enum {
    BTH_GAP_BLE_ADV_UNDIR_CONN = 0,
    BTH_GAP_BLE_ADV_DIR_CONN,
    BTH_GAP_BLE_ADV_UNDIR_SCAN,
    BTH_GAP_BLE_ADV_NON_UNDIR_NONCONN,
    BTH_GAP_BLE_ADV_DIR_CONN_LOW_DUTY,
    BTH_GAP_BLE_ADV_INVALID_TYPE
} bth_gap_adv_type_em;
typedef enum {
    BTH_GAP_BLE_ADV_CHANNEL_MAP_37,
    BTH_GAP_BLE_ADV_CHANNEL_MAP_38,
    BTH_GAP_BLE_ADV_CHANNEL_MAP_39,
    BTH_GAP_BLE_ADV_CHANNEL_MAP_DEFAULT /* ADV_CHANNEL_MAP_37 | ADV_CHANNEL_MAP_38 | ADV_CHANNEL_MAP_39 */
} bth_gap_adv_channel_em;
typedef enum {
    BTH_GAP_BLE_FLT_ANY_SCAN_ANY_CONNECT = 0,
    BTH_GAP_BLE_FLT_WHITE_SCAN_ANY_CONNECT,
    BTH_GAP_BLE_FLT_ANY_SCAN_WHITE_CONNECT,
    BTH_GAP_BLE_FLT_WHITE_SCAN_WHITE_CONNECT
} bth_gap_ble_flt_policy_em;
typedef enum {
    BTH_GAP_BLE_ADV_PHY_1M = 0x01,
    BTH_GAP_BLE_ADV_PHY_2M,
    BTH_GAP_BLE_ADV_PHY_CODED
} bth_gap_ble_adv_phy_em;

typedef enum  {
    BTH_EIR_FLAGS_LE_LIMITED_DISCOVERABLE_MODE =                             0x01,
    BTH_EIR_FLAGS_LE_GENERAL_DISCOVERABLE_MODE =                             0x02,
    BTH_EIR_FLAGS_BREDR_NOT_SUPPORTED =                                      0x04,
    BTH_EIR_FLAGS_LE_AND_BREDR_TO_SAME_DEVICE_CAPABLE_CONTROLLER =           0x08,
    BTH_EIR_FLAGS_LE_AND_BREDR_TO_SAME_DEVICE_CAPABLE_HOST =                 0x10
} bth_gap_ble_eir_flags;
typedef enum  {
    BTH_GAP_BLE_SECURITY_MODE1_LEVEL1, /* No security */
    BTH_GAP_BLE_SECURITY_MODE1_LEVEL2, /* Unauthenticated pairing and encryption */
    BTH_GAP_BLE_SECURITY_MODE1_LEVEL3, /* Authenticated Pairing and encryption */
    BTH_GAP_BLE_SECURITY_MODE1_LEVEL4, /* Authenticated ECDH Pairing and encryption */
    BTH_GAP_BLE_SECURITY_MODE2_LEVEL1, /* Unauthenticated pairing and data signing */
    BTH_GAP_BLE_SECURITY_MODE2_LEVEL2 /* Authenticated pairing and data signing */
} bth_gap_ble_sec_mode;

typedef struct {
    uint8_t adv_handle;
    bth_gap_adv_type_em adv_type;
    bth_gap_adv_channel_em adv_channel_map;
    uint32_t adv_interval;
    bth_gap_ble_flt_policy_em adv_filter_policy;
    bth_gap_ble_adv_phy_em adv_phy;
    uint8_t tx_power_level;
    uint16_t duration;
    uint8_t supported_adv_para;
    uint16_t adv_data_len;
    uint8_t adv_data[0];
} bth_gap_ext_adv_node;

typedef struct {
    uint8_t event_type;
    uint8_t data_status;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t primary_phy;
    uint8_t secondary_phy;
    uint8_t adv_sid;
    uint8_t tx_power;
    uint8_t rssi;
    uint16_t periodic_adv_interval;
    uint8_t direct_addr_type;
    uint8_t direct_addr[6];
    uint8_t data_length;
    uint8_t data[1];
} bth_gap_ext_adv_report_node;

typedef struct {
    bth_address rmt_addr;
    uint16_t conn_handle;
    uint16_t mtu_size;
    uint8_t conn_state;
    uint8_t os_type;
    uint8_t pair_status;
    uint8_t disc_reason;
} bth_gap_conn_status_node;

typedef struct {
    uint8_t filter_policy;
    uint8_t adv_sid;
    uint8_t adv_addr_type;
    uint8_t peer_addr[6];
    uint16_t skip;
    uint16_t sync_timeout;
} bth_periodic_adv_sync_parm;

typedef struct {
    uint8_t addr_number;
    uint8_t adv_sid;
    bth_address addrs[0];
} bth_periodic_adv_list;

/* Parameters of Periodic Advertising */
typedef struct {
    uint8_t add_tx_power;
    uint16_t periodic_adv_interval_min;
    uint16_t periodic_adv_interval_max;
} bth_periodic_adv_params;

typedef struct {
    uint16_t data_length;
    uint8_t prd_adv_data[0];
} bth_periodic_adv_data;

typedef struct {
    bth_address addr;
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t slave_latency;
    uint16_t timeout_multiplier;
} bth_gap_connection_param;

/* Parameters of Periodic Advertising Create Sync */
typedef struct {
    uint8_t add_dev;
    bth_periodic_adv_list dev_list;
} bth_gap_set_adv_list;

typedef struct {
    uint8_t filter_policy;
    uint8_t adv_sid;
    uint8_t adv_addr_type;
    uint8_t peer_addr[6];
    uint16_t skip;
    uint16_t sync_timeout;
} bth_gap_periodic_adv_sync_param;

typedef struct {
    uint8_t sync_id;
    bth_periodic_adv_sync_parm adv_sync_param;
} bth_gap_create_adv_syn_param;

typedef struct {
    uint8_t adv_handle;
    bth_periodic_adv_params adv_param;
    bth_periodic_adv_data adv_data;
} bth_gap_set_periodic_adv_param;

typedef struct {
    uint8_t adv_handle;
    uint8_t adv_type;
    uint8_t adv_event_flag;
    uint8_t primary_adv_channel_map;
    uint32_t primary_adv_interval_min;
    uint32_t primary_adv_interval_max;
    uint8_t own_addr[6];
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t adv_filter_policy;
    uint8_t adv_tx_power;
    uint8_t primary_adv_phy;
    uint8_t secondary_adv_max_skip;
    uint8_t secondary_adv_phy;
    uint8_t advertising_sid;
    uint8_t scan_request_notification_enable;
} bth_gap_ext_adv_param;

typedef struct {
    uint8_t fragment_preference;
    uint8_t eir_flag;
    uint16_t adv_data_length;
    uint16_t scan_rsp_data_length;
    uint8_t *adv_data;
    uint8_t *scan_rsp_data;
} bth_gap_ext_adv_data;

typedef struct {
    uint16_t duration;
    uint8_t max_extended_adv_events;
    uint8_t priority;
} bth_gap_ext_adv_en;

typedef struct {
    bth_gap_ext_adv_param ext_adv_param;
    bth_gap_ext_adv_data ext_adv_data;
    bth_gap_ext_adv_en ext_adv_en;
} bth_gap_set_extern_adv;

typedef struct {
    uint8_t status;
    uint8_t adv_handle;
    uint16_t co_handle;
    uint8_t num_cmp_ext_adv_events;
} bth_gap_adv_set_terminated;

/* Parameters of Extended Scan */
typedef struct {
    uint8_t scan_filter_policy;
    uint8_t scan_phys;
    uint8_t scan_type_1m;
    uint8_t scan_type_coded;
    uint16_t scan_interval_1m;
    uint16_t scan_window_1m;
    uint16_t scan_interval_coded;
    uint16_t scan_window_coded;
} bth_gap_ext_scan_param;

typedef struct {
    uint8_t filter_duplicate;
    uint8_t limited;
    uint16_t duration;
    uint16_t period;
} bth_gap_ext_scan_en;

typedef struct {
    bth_gap_ext_scan_param ext_scan_param;
    bth_gap_ext_scan_en ext_scan_en;
} bth_gap_set_extern_scan;

typedef struct {
    uint16_t conn_handle;
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
    uint16_t phy_options;
} bth_gap_le_set_phy_t;

typedef struct {
    bth_address addr;
    bth_gap_le_set_phy_t phys;
} bth_gap_le_phys_ampid_addr;

typedef struct {
    uint16_t conn_handle;
    uint16_t maxtxoctets;
    uint16_t maxtxtime;
} bth_gap_le_set_data_length;

typedef struct {
    bth_address addr;
    bth_gap_le_set_data_length len;
} bth_gap_le_data_length_ampid_addr;

typedef struct {
    bth_address addr;
    uint8_t mask; /* etc, HCI_CANODE_MASK_WHITELIST */
} bth_gap_white_addr_node;

typedef struct {
    uint32_t count;
    bth_gap_white_addr_node addr_list[BTH_BTS_WHITE_ADDR_COUNT_MAX];
} bth_gap_white_addr_list;

/* Default Parameters of Connect */
typedef struct {
    uint16_t conn_interval_min;
    uint16_t conn_interval_max;

    uint16_t conn_interval_coded_min;
    uint16_t conn_interval_coded_max;
} bth_hci_tgap_le_conn_param;

typedef struct {
    uint8_t isset; // 1: set the new mask, 0: unset the mask
    uint8_t mask[BTH_HCI_LE_EVENT_MASK_LEN];
} bth_hci_tgap_le_set_event_mask;

typedef struct {
    uint32_t rmt_dev_hdl;
    uint32_t passkey_number;
} bth_smp_passkey_rsp;

typedef struct {
    uint32_t rmt_dev_hdl;
    uint8_t result;
} bth_smp_pair_result;

typedef struct {
    uint8_t bondable;
    uint8_t io_capability;
    uint8_t sc_enable;
    uint8_t sc_mode;
} bth_smp_sec_param;

enum {
    BTH_GLOBAL_PTS_MODE = 1,
    BTH_GLOBAL_L2CAP_SEC_KEY_WORD = 2,
    BTH_GLOBAL_L2CAP_USERCID = 3,
    BTH_GLOBAL_HCI_LOG_STATUS = 4,
    BTH_GLOBAL_HCI_RECV_PRINTF = 5,
    BTH_GLOBAL_HEAP_STATITIC = 6,
    BTH_GLOBAL_HEAP_STATITIC_PRINTF = 7,
    BTH_GLOBAL_HEAP_STATITIC_PERIOD = 8,
    BTH_GLOBAL_THREAD_MIPS_STATITIC = 9,
    BTH_GLOBAL_THREAD_MIPS_PERIOD = 10,
};

typedef struct {
    uint32_t global_value_flag;
    uint32_t global_value;
} bth_set_global_value;

typedef struct {
    uint8_t addr_type;
    uint8_t use_private_addr;
} bth_gap_addr_info;

typedef struct {
    uint32_t handle;
    int8_t rssi;
    uint8_t status;
    uint16_t reserved;
} bth_gap_read_remote_rssi;

typedef struct {
    bth_address addr;
    uint8_t ltk_len;
    uint8_t ltk[BTH_BLE_SMP_LTK_LEN];
} bth_smp_keys_info;

#pragma pack ()

/* General Interface for Stack GAP */
#endif /* _BTH_STACK_GAP_H_ */
