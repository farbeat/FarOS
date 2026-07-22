/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: ble connection general dispatch for l2cap
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef BLE_CONN_GENERAL_L2CAP_H
#define BLE_CONN_GENERAL_L2CAP_H

#include "sdk_list.h"
#include "bth_stack.h"
#include "mpc_dispatch.h"
#include "bth_stack_l2cap.h"
#include "sdk_ble.h"

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
    struct list_stru psm_list;
    struct list_stru conn_list;
    void *resv;
} ble_l2cap_local_data;

enum {
    SDK_L2CAP_CONNECT_INIT,
    SDK_L2CAP_CONNECT_CREATING_INITIATOR,
    SDK_L2CAP_CONNECT_CREATING_RESPONDER,
    SDK_L2CAP_CONNECT_CREATED_INITIATOR,
    SDK_L2CAP_CONNECT_CREATED_RESPONDER,
    SDK_L2CAP_CONNECT_DISCONNECT,
};

uint8_t ble_l2cap_internal_callback_regist(uint16_t module_id, uint16_t event, sdk_ble_cbk cbk);
uint8_t ble_conn_general_l2cap_proc(const mpc_message *param);
void ble_l2cap_internal_cbk(uint16_t event, void *arg);

bth_l2cap_credit_conn *ble_l2cap_le_find_conn(uint16_t cid);
bth_l2cap_credit_conn *ble_l2cap_le_add_conn(void);
bth_l2cap_psm_node *ble_l2cap_le_find_psm(uint16_t psm);
bth_l2cap_psm_node *ble_l2cap_le_add_psm(void);
bth_l2cap_psm_node *ble_l2cap_le_del_psm(uint16_t psm);
bth_l2cap_credit_conn *ble_l2cap_le_find_conn_by_addr(bth_address *addr);
bth_l2cap_credit_conn *ble_l2cap_le_find_conn_by_psm(uint16_t psm);

uint8_t ble_l2cap_reg_psm_hlp(bth_l2cap_psm_node *param);
uint8_t ble_l2cap_unreg_psm_hlp(bth_l2cap_psm_node *param);
uint8_t ble_l2cap_credit_conn_req_hlp(bth_l2cap_credit_conn_ev *param);
uint8_t ble_l2cap_ch_tx_hlp(bth_l2cap_ch_tx *param);
uint8_t ble_l2cap_ch_tx_hlp_by_psm(bth_l2cap_ch_tx *param);
uint8_t ble_l2cap_ch_data_send(bth_l2cap_ch_tx *param);
uint8_t mc_ble_l2cap_credit_conn_rsp_atb(bth_l2cap_credit_conn_ev *param);
uint8_t mc_ble_l2cap_credit_channel_flow_control_atb(bth_l2cap_credit_flow_cntl *param);
uint8_t mc_ble_l2cap_disconnect_atb(bth_l2cap_disconnect *param);
uint8_t ble_l2cap_conn_req_hlp(bth_l2cap_credit_conn_ev *param);
uint8_t mc_ble_l2cap_set_l2cap_config(bth_l2cap_config_param *param);
void bts_at_cmd_l2cap_sdk_reg_cbk(void);
void ble_l2cap_main_internal_reg_cbk(void);
typedef void (*l2cap_le_cbk)(uint16_t, void *);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BLE_CONN_GENERAL_L2CAP_H */
