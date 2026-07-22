/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ble connection general dispatch for gatt
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef BLE_CONN_GENERAL_GATT_H
#define BLE_CONN_GENERAL_GATT_H

#include "sdk_list.h"
#include "bth_stack.h"
#include "bth_stack_gatt.h"
#include "sdk_ble_gatt.h"
#include "mpc_dispatch.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    sdk_ble_gatt_cbk_event event;
    uint16_t module_id;
    void    *context;
} ble_gatt_tl_task_node;

/* * local_data struct
 * tl_list ble_gatt_tl_data
 *      |->conn_list 1.......
 *            | bd address
 *            |->task_node->task_node->task_node(ble_gatt_tl_task_node)
 *      |->conn_list 2.......
 *            | bd address
 *            |->task_node->task_node->task_node(ble_gatt_tl_task_node)
 */
typedef struct {
    bth_address bd_addr;
    sdk_ble_gatt_role role;
    struct list_stru tl_task;
} ble_gatt_tl_data;

typedef struct {
    // local as gatt server, save server event and cbk info, ble_general_cbk_table struct
    struct list_stru server_event_cbk_list;
    // local as gatt client, save client event and cbk info, ble_general_cbk_table struct
    struct list_stru client_event_cbk_list;
    // work as gatt client, sava a core api event ckb and cbk context
    struct list_stru tl_list;
} ble_gatt_local_data;

extern ble_gatt_local_data g_sdk_gatt_local_data;
#define sdk_gatt_local_data() g_sdk_gatt_local_data
#define sdk_gatt_local_server() (sdk_gatt_local_data().server_event_cbk_list)
#define sdk_gatt_local_server_head() (sdk_gatt_local_server().head)
#define sdk_gatt_local_client() (sdk_gatt_local_data().client_event_cbk_list)
#define sdk_gatt_local_client_head() (g_sdk_gatt_local_data.client_event_cbk_list.head)
#define sdk_gatt_local_tl() (sdk_gatt_local_data().tl_list)
#define sdk_gatt_local_tl_head() (sdk_gatt_local_tl().head)

bool ble_gatt_addr_compare(bth_address *s, bth_address *r);
typedef void (gatt_func) (void *context, uint8_t ev, void *arg); /* GATT callback function */
uint8_t ble_conn_general_gatt_proc(const mpc_message *param);
void ble_gatt_internal_reg_cbk(void);
uint8_t ble_gatt_internal_callback_register(uint16_t module_id, uint16_t event, sdk_ble_cbk cbk);
void ble_gatt_internal_cbk(uint16_t event, void *arg);
int8_t mc_ble_gatt_create_server_req_atb(void);
int8_t mc_ble_gatt_create_service_req_atb(bth_gatt_add_server *param);
int8_t mc_ble_gatt_add_include_service_req_atb(bth_gatt_add_include *param);
int8_t mc_ble_gatt_add_characteristic_req_atb(bth_gatt_add_characteristic *param);
int8_t mc_ble_gatt_add_descriptor_req_atb(bth_gatt_add_descriptor *param);
int8_t mc_ble_gatt_start_service_req_atb(bth_gatt_start_service *param);
int8_t mc_ble_gatt_stop_service_req_atb(bth_gatt_service *param);
int8_t mc_ble_gatt_send_response_req_atb(bth_gatt_send_response *param);
int8_t mc_ble_gatt_notify_characteristic_changed_req_atb(bth_gatt_notify_characteristic *param);
int8_t mc_ble_gatt_notify_crt_changed_by_uuid_req_atb(bth_gatt_notify_characteristic_by_uuid *param);
int8_t mc_ble_gatt_update_descriptor_value_change_req_atb(bth_gatt_update_descriptor *param);
int8_t mc_ble_gatt_discovery_service_req_atb(bth_gatt_discovery_service *param);
int8_t mc_ble_gatt_read_character_req_atb(bth_gatt_read_character_request *param);
int8_t mc_ble_gatt_read_multiple_character_req_atb(bth_gatt_read_multiple_character_request *param);
int8_t mc_ble_gatt_multiple_value_read_character_req_atb(bth_gatt_read_multiple_character_request *param);
int8_t mc_ble_gatt_multiple_value_ntf_req_atb(bth_gatt_multiple_value_nfy_request *param);
int8_t mc_ble_gatt_read_characteristics_by_uuid_req_atb(bth_gatt_discovery_character_by_uuid *param);
int8_t mc_ble_gatt_discovery_service_req_atb(bth_gatt_discovery_service *param);
int8_t mc_ble_gatt_find_include_service_req_atb(bth_gatt_find_include_service *param);
int8_t mc_ble_gatt_discovery_characteristics_req_atb(bth_gatt_discovery_character *param);
int8_t mc_ble_gatt_discovery_characteristics_by_uuid_req_atb(bth_gatt_discovery_character_by_uuid *param);
int8_t mc_ble_gatt_discovery_descriptor_req_atb(bth_gatt_discovery_descriptor *param);
int8_t mc_ble_gatt_write_characteristic_value_req_atb(bth_gatt_write_characteristic *param);
int8_t mc_ble_gatt_reliable_write_characteristic_value_req_atb(bth_gatt_write_characteristic *param);
int8_t mc_ble_gatt_read_descriptor_req_atb(bth_gatt_read_descriptor *param);
int8_t mc_ble_gatt_mtu_config_req_atb(bth_gatt_mtu_config *param);
int8_t mc_ble_gatt_write_descriptor_value_req_atb(bth_gatt_write_descriptor *param);
int8_t mc_ble_gatt_discovery_descriptor_by_handle_req_atb(bth_gatt_discovery_descriptor_by_handle *param);
int8_t mc_ble_gatt_set_gap_ui_attributes_req_atb(bth_gatt_set_gap_ui *param);
int8_t mc_ble_gatt_get_gap_ui_attributes_req_atb(void);
int8_t mc_ble_gatt_get_hid_report_map_req_atb(bth_gatt_read_character_request *param);
int8_t mc_ble_gatt_set_default_mtu_value_atb(bth_le_att_mtu_param *mtu_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BLE_CONN_GENERAL_GATT_H */
