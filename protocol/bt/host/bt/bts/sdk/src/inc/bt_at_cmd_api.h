/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: sdk api common
 * Author: @CompanyNameTag
 * Create: 2021-09-27
 */

#ifndef AT_COMMAND_API_H
#define AT_COMMAND_API_H

#include "product.h"
#include "sdk_ble.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BT_AT_CMD_UART_TX_LEN_MAX 512
#define BT_EXT_MAX_ADV_DATA_LEN 255

extern uint32_t g_hfp_hf_pts_mode_setting;

typedef enum {
    BT_AT_PARAM_LEN0,
    BT_AT_PARAM_LEN1,
    BT_AT_PARAM_LEN2,
    BT_AT_PARAM_LEN3,
    BT_AT_PARAM_LEN4,
    BT_AT_PARAM_LEN5,
    BT_AT_PARAM_LEN6,
    BT_AT_PARAM_LEN7,
    BT_AT_PARAM_LEN8,
    BT_AT_PARAM_LEN9,
    BT_AT_PARAM_LEN10,
    BT_AT_PARAM_LEN11,
    BT_AT_PARAM_LEN12,
    BT_AT_PARAM_LEN13,
    BT_AT_PARAM_LEN18 = 18,
    BT_AT_PARAM_LEN19 = 19,
} bt_at_param_len;

typedef enum {
    BT_AT_BLE_PRODUCT_TYPE_KEYMOUSE,
    BT_AT_BLE_PRODUCT_TYPE_MOUSE,
    BT_AT_BLE_PRODUCT_TYPE_KEYBOARD,
    BT_AT_BLE_PRODUCT_TYPE_TEST_MODE,
    BT_AT_BLE_PRODUCT_TYPE_DONGLE_MODE,
    BT_AT_BLE_PRODUCT_TYPE_BYPASS_MODE,
    BT_AT_BLE_PRODUCT_TYPE_MAX,
} bt_at_ble_product_type;

typedef struct {
    uint16_t data_len;
    uint8_t *data;
} bt_at_data_t;

typedef enum {
    BT_AT_PRINT_TX,
    BT_AT_PRINT_RX,
} bt_at_print_type;
 /*
 * App min data length is 8
 * sof(1) + len(2) + control(1) + opcode(2) + crc(2)
 * Mcu min data length is 9
 * tid(1) + sof(1) + len(2) + control(1) + opcode(2) + crc(2)
 */
int bt_at_cmd_write_bdaddr(int argc, char *argv[]);

int bt_at_cmd_read_bdaddr(int argc, char *argv[]);

int bt_at_cmd_name_write(int argc, char *argv[]);

int bt_at_cmd_name_read(int argc, char *argv[]);

int bt_at_cmd_dft_reset(int argc, char *argv[]);

int bt_at_cmd_enter_pairing(int argc, char *argv[]);

int bt_at_cmd_disconnect_link(int argc, char *argv[]);

int bt_at_cmd_ble_connect_peer(int argc, char *argv[]);

int bt_at_cmd_ble_connect_white_list(int argc, char *argv[]);

int bt_at_cmd_ble_add_white_list(int argc, char *argv[]);

int bt_at_cmd_ble_rmv_white_list(int argc, char *argv[]);

int bt_at_cmd_ble_enter_scan(int argc, char *argv[]);

int bt_at_cmd_ble_resolve_addr_enable(int argc, char *argv[]);

int bt_at_cmd_query_con_info(int argc, char *argv[]);

int bt_at_cmd_query_paired_device_info(int argc, char *argv[]);

int bt_at_cmd_ble_dev_info(int argc, char *argv[]);

int bt_at_cmd_ble_disconnect(int argc, char *argv[]);

int bt_at_cmd_ble_adv_enable(int argc, char *argv[]);

int bt_at_cmd_ble_start_adv_by_type(int argc, char *argv[]);

int bt_at_cmd_ble_start_adv_by_adv_data(int argc, char *argv[]);

int bt_at_cmd_ble_start_adv_by_adv_data_and_set_addr(int argc, char *argv[]);

int bts_mpc_adapter_gap_ext_adv_enable(int argc, char *argv[]);

int bt_at_cmd_battery_report(int argc, char *argv[]);

int bt_at_cmd_addr_type_change(int argc, char *argv[]);

int bt_at_cmd_sec_param_set(int argc, char *argv[]);

int bt_at_cmd_read_character(int argc, char *argv[]);

int bt_at_cmd_keyboard_input(int argc, char *argv[]);

int bt_at_cmd_read_multiple_character(int argc, char *argv[]);

#ifdef BTH_KEYBOARD_MANAGER_SUPPORT
int bt_at_cmd_keyboard_input(int argc, char *argv[]);
int bt_at_cmd_keyboard_input_one_word(int argc, char *argv[]);
#endif

#ifdef BTH_MOUSE_MANAGER_SUPPORT
int bt_at_cmd_mouse_input(int argc, char *argv[]);
int bt_at_cmd_high_mouse_input(int argc, char *argv[]);
int bt_at_cmd_high_mouse_input_one(int argc, char *argv[]);
int bt_at_cmd_mouse_rx_int_enable(int argc, char *argv[]);
int bt_at_cmd_mouse_tx_int_enable(int argc, char *argv[]);
int bt_at_cmd_mouse_tx_info_print(int argc, char *argv[]);
int bt_at_cmd_mouse_rx_info_print(int argc, char *argv[]);
int bt_at_cmd_update_mouse_tx_mode(int argc, char *argv[]);
extern void usb_ble_high_mouse_report(uint8_t *data, uint16_t lenth);
#endif

int bt_at_cmd_high_mouse_input(int argc, char *argv[]);
int bt_at_cmd_high_mouse_input_one(int argc, char *argv[]);
int bt_at_cmd_multiple_value_nfy(int argc, char *argv[]);
int bt_at_cmd_multiple_value_read_character(int argc, char *argv[]);
int bt_at_cmd_keyboard_input_one_word(int argc, char *argv[]);
int bts_at_cmd_set_att_key_size(int argc, char *argv[]);
int bts_at_cmd_att_config_set(int argc, char *argv[]);
int bt_at_cmd_connect_para_update(int argc, char *argv[]);

int bt_at_cmd_get_local_appearance(int argc, char *argv[]);

int bt_at_cmd_set_local_appearance(int argc, char *argv[]);

int bt_at_cmd_set_period_advertising(int argc, char *argv[]);

int bt_at_cmd_disable_period_advertising(int argc, char *argv[]);

int bt_at_cmd_set_period_advertising_list(int argc, char *argv[]);

int bt_at_cmd_period_adv_create_sync(int argc, char *argv[]);

int bt_at_cmd_period_termiate_sync(int argc, char *argv[]);

int bt_at_cmd_disable_extern_adv(int argc, char *argv[]);

int bt_at_cmd_set_extern_adv(int argc, char *argv[]);

int bt_at_cmd_disable_extern_adv_by_adv_hdl(int argc, char *argv[]);

int bt_at_cmd_remove_extern_adv(int argc, char *argv[]);

int bt_at_cmd_remove_extern_adv_by_adv_handle(int argc, char *argv[]);

int bt_at_cmd_ble_start_adv_by_adv_handle(int argc, char *argv[]);

int bt_at_cmd_disable_extern_scan(int argc, char *argv[]);

int bt_at_cmd_set_extern_scan(int argc, char *argv[]);

int bt_at_cmd_set_random_addr(int argc, char *argv[]);

int bt_at_cmd_set_passkey_number(int argc, char *argv[]);

int bt_at_cmd_set_pts_mode(int argc, char *argv[]);

int bt_at_cmd_set_global_value(int argc, char *argv[]);

int bt_at_cmd_set_le_conn_param(int argc, char *argv[]);

int bt_at_cmd_set_hci_le_event_mask(int argc, char *argv[]);

int bt_at_cmd_set_le_att_mtu_param(int argc, char *argv[]);

int bt_at_cmd_remove_pair_info(int argc, char *argv[]);

int bt_at_cmd_smp_set_key_from_btc(int argc, char *argv[]);

void ble_at_gap_cmd(int cmd_id, char **params, int params_cnt);

void ble_at_gatt_cmd(int cmd_id, char **params, int params_cnt);

void ble_at_smp_cmd(int cmd_id, char **params, int params_cnt);

#ifdef VERSION_STANDARD
void bt_at_keyboard_input_api_register(void);
#endif

int bt_at_cmd_get_white_list(int argc, char *argv[]);

int bts_at_cmd_l2cap_reg_psm(int argc, char *argv[]);
int bts_at_cmd_l2cap_unreg_psm(int argc, char *argv[]);
int bts_at_cmd_l2cap_credit_channel_req(int argc, char *argv[]);
int bts_at_cmd_l2cap_credit_channel_flow_control(int argc, char *argv[]);
int bts_at_cmd_l2cap_channel_tx(int argc, char *argv[]);
int bts_at_cmd_l2cap_data_send(int argc, char *argv[]);
int bts_at_cmd_l2cap_disconnect(int argc, char *argv[]);
int bts_at_cmd_l2cap_connect(int argc, char *argv[]);
/* Used by other modules of the Acore to print data to the AT. */
void mc_bt_set_at_cmd_print_cbk(sdk_ble_cbk func);
sdk_ble_cbk mc_bt_get_at_cmd_print_cbk(void);
int bts_at_cmd_transparent_to_btc(int argc, char *argv[]);
int bt_at_cmd_set_product_type(int argc, char *argv[]);
int bt_at_cmd_set_default_gatt_services(int argc, char *argv[]);
void bt_at_cmd_common_print_cbk(uint16_t event, void *arg);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* AT_COMMAND_API_H */
