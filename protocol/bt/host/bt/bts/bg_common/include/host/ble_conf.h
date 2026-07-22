/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BLE module macro conf
 */
#ifndef BLE_CONF_H
#define BLE_CONF_H

#include "ivt_config.h"
#include "product.h"
#include "product_bth.h"
#include "datatype.h"

/* --------------------------------------------------------------------------- */
/*                                 featrue conf                                */
/* --------------------------------------------------------------------------- */
extern BOOL g_config_align_onebyte;
extern BOOL g_config_le_runtime_service_change;
extern BOOL g_config_le_eatt_service_support;
extern BOOL g_config_smp_lesc;
extern uint8_t g_config_smp_mode;

extern uint8_t g_config_gap_use_static_addr;

extern BOOL g_config_att_trnas_save_copy;
extern BOOL g_config_gap_le_features_init_ready;
extern BOOL g_config_hc2host_fc_enable;
extern BOOL g_config_fixed_public_secret_key;
extern BOOL g_pts_bth_smp;
extern BOOL g_config_le_privacy;
extern BOOL g_config_le_master_use_privacy;
extern BOOL g_config_le_m_can_use_privacy;
extern BOOL g_config_le_slave_use_privacy;
extern BOOL g_config_le_s_can_use_privacy;
extern BOOL g_config_hci_log_enable;
/* --------------------------------------------------------------------------- */
/*                                 variable conf                               */
/* --------------------------------------------------------------------------- */
/* gap */
extern uint32_t g_buff_new_max_len;
extern uint8_t  g_gap_min_encry_keysize;
extern uint8_t  g_gap_max_encry_keysize;
extern uint8_t  g_gatt_min_encry_keysize;
extern uint8_t  g_gatt_item_need_auth_check;
extern uint8_t  g_gatt_item_need_pend_flag;
extern uint16_t g_gap_group_vendor_specific;
extern uint8_t  g_gap_cr_value_len;
#ifdef ENABLE_CHANGE_DEVICE_NAME
extern uint16_t g_bluetooth_di_specification;
#endif

extern uint16_t g_gap_extended_advertising_duration_ms;
extern uint16_t g_gap_extended_scan_duration_ms;
extern uint16_t g_gap_extended_scan_period_ms;
extern uint32_t g_gap_extended_scan_gap_ms;

extern uint32_t g_gap_ops_set_local_br_addr;

/* gatt */
extern uint16_t g_gatt_appearance_category_type;
extern uint16_t g_gatt_properties_device_name;
extern uint16_t g_gatt_properties_appearance;
extern uint16_t g_gatt_properties_connection_parameters;
extern uint16_t g_gatt_properties_central_address_resolution;
extern uint16_t g_att_mtu_le_max;
extern uint16_t g_att_mtu_le_default;

extern uint8_t g_battery_service_battery_level;

/* att */
extern uint16_t g_att_min_connection_interval;
extern uint16_t g_att_max_connection_interval;
extern uint16_t g_att_slave_latency;
extern uint16_t g_att_connection_timeout;
extern uint8_t  g_att_characteristic_space;
extern uint16_t  g_att_security_keyword;

/* HCI */
extern uint32_t g_hci_max_len;
extern uint8_t  g_hci_qos_realtime_flush_max;
extern uint8_t  g_hci_max_adv_data_len;
extern uint32_t g_hci_local_start_timer_timeout;
extern uint32_t g_hci_start_encryption_timeout;
extern uint32_t g_hci_periodic_advertising_create_sync_timeout;
extern uint32_t g_hci_sync_conn_transmit_bandwidth;
extern uint32_t g_hci_sync_conn_receive_bandwidth;
extern uint16_t g_hci_create_conn_supervision_timeout_delta;
extern uint8_t  g_hci_passkey_length;
extern uint8_t  g_hci_sync_data_packet_len;
extern uint16_t g_hci_sync_data_packet_num;
extern uint8_t g_bgle_coex;
extern uint16_t g_l2cap_security_keyword;
extern uint16_t g_l2cap_cid_userset;

void ble_conf_init(void);
void bth_set_l2cap_sec_keyword(uint16_t sec_mode);
void bth_set_l2cap_cid_userset(uint16_t cid);
void bth_set_pts_mode(uint8_t pts_mode);
BOOL bth_is_pts_mode(void);
uint16_t bth_get_att_sec_keyword(void);
void bth_set_att_pend_check_flag(uint8_t pend_mode);
uint8_t bth_get_att_pend_check_flag(void);
void bth_set_att_auth_check_flag(uint8_t auth_mode);
uint8_t bth_get_att_pend_check_flag(void);

void bth_conf_set_hci_log_status(uint8_t enable);
void bth_add_global_heap_memused(uint32_t size);
void bth_minus_global_heap_memused(uint32_t size);
uint32_t bth_get_global_heap_memused(void);
void bth_printf_global_heap_memused(void);
void bth_conf_set_heap_statistic(uint8_t enable);
void bth_conf_set_heap_statistic_printf(uint8_t enable);
void bth_conf_set_heap_print_period(uint32_t value);
void bth_conf_set_hci_receive_printf(uint8_t enable);
void bth_conf_set_mips_print_period(uint32_t value);
uint16_t bth_conf_set_le_resolution_enable(uint8_t enable);
uint8_t *bth_get_gatt_device_name(void);
uint8_t bth_get_gatt_device_name_len(void);
BOOL bth_conf_get_le_resolution_enable(void);
/* HCI LE Connection Parameters */
extern uint16_t g_hci_tgap_slave_latency;

extern uint16_t g_hci_tgap_max_latency_min;
extern uint16_t g_hci_tgap_max_latency_max;

extern uint16_t g_hci_tgap_scan_fast_interval_min;
extern uint16_t g_hci_tgap_scan_fast_interval_max;
extern uint16_t g_hci_tgap_scan_fast_window;

extern uint16_t g_hci_tgap_initial_conn_interval_min;
extern uint16_t g_hci_tgap_initial_conn_interval_max;

extern uint16_t g_hci_tgap_scan_fast_interval_coded;
extern uint16_t g_hci_tgap_scan_fast_window_coded;

extern uint16_t g_hci_tgap_initial_conn_interval_coded_min;
extern uint16_t g_hci_tgap_initial_conn_interval_coded_max;

extern uint16_t g_hci_tgap_supervision_to_min;
extern uint16_t g_hci_tgap_supervision_to_max;
extern uint16_t g_hci_tgap_supervision_to_delta;

extern uint16_t g_hci_gtap_min_ce_len;
extern uint16_t g_hci_gtap_max_ce_len;

extern uint16_t g_l2cap_min_connection_interval;
extern uint16_t g_l2cap_max_connection_interval;
extern uint16_t g_l2cap_link_timeout;

extern BOOL g_bth_global_heap_print_enable;
extern BOOL g_bth_global_mips_statistic_enable;
extern uint8_t g_global_hci_recv_data_print_enable;
extern uint32_t g_bth_global_heap_print_period;
extern BOOL g_bth_global_heap_statistic_enable;
extern uint32_t g_bth_global_mips_print_period;
/* --------------------------------------------------------------------------- */
/*                                 macro conf                                  */
/* --------------------------------------------------------------------------- */
#define CONFIG_LE 1
#define CFG_BT_LE_SUPPORTED 1
#if (defined(CHIP_SUQIN) && (CHIP_SUQIN == 1))
#undef CONFIG_HW_MEMORY_TRACE
#undef CONFIG_HW_MIPS_TRACE
#else
#define CONFIG_HW_MEMORY_TRACE 1
#define CONFIG_HW_MIPS_TRACE   1
#endif
#define SMP_CONFIG_OOB_SUPPORT 0

#undef CFG_BT_BR_SUPPORTED
#undef CFG_BR_HFP_SUPPORTED
#undef CFG_BR_PBAP_SUPPORTED
#undef CFG_BR_AVDTP_SUPPORTED
#undef CFG_BR_A2DP_SUPPORTED
#undef CFG_BR_AVRCP_SUPPORTED
#undef CFG_BR_RFCOMM_SUPPORTED

#undef CONFIG_AVDTP_HEADSET_PATCH
#undef CONFIG_SUPPORT_AAC
#undef CFG_BR_SPP_SUPPORTED

/*
 * / gap
 */
// Range: 0x0006 to 0x0C80, Time = N * 1.25 ms, Time Range: 7.5 ms to 4 s
#define HCI_LE_CREATE_CONN_INTERVAL_MIN  0x6
#define HCI_LE_CREATE_CONN_INTERVAL_MAX  0xC80

// Range: 0x0006 to 0x0C80, Time = N * 1.25 ms, Time Range: 7.5 ms to 4 s
#define HCI_LE_EXT_CREATE_CONN_INTERVAL_MIN  0x6
#define HCI_LE_EXT_CREATE_CONN_INTERVAL_MAX  0xC80

/*
 * / ATT
 */
#define CONFIG_ATT 1
#ifdef CONFIG_TWS
#undef CONFIG_ATT
#endif
#define CONFIG_EATT_SUPPORTED YES
#define CONFIG_EATT 1

/*
 * / SMP
 */
#define CONFIG_SMP_SLAVE_PEERKEYS_MAX 10

/*
 * / HCI
 */
#define CONFIG_HCI_LE_SCAN_REQUEST_EVENT_ENABLED

#define CONFIG_HOST_ACL_PKT_SIZE 512
#define CONFIG_HOST_ACL_PKT_NUM 8
#define BTH_HOST_PRODUCT_TEST_MODE 3
#endif
