/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: host sdk stack api for gap
 * Author: @CompanyNameTag
 * Create: 2022-03-14
 */
#ifndef _SDK_BLE_GAP_H_
#define _SDK_BLE_GAP_H_


#include "sdk_ble.h"
#include "bth_stack_gap.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    SDK_BLE_GAP_EVENT_EXT_ADV_RPT = 0,
    SDK_BLE_GAP_EVENT_CONN_STATUS_CHANGED,
    SDK_BLE_GAP_EVENT_GET_WHITE_LIST,
    SDK_BLE_GAP_EVENT_GET_APPERANCE,
    SDK_BLE_SMP_EVENT_PASSKEY_REQ,
    SDK_BLE_SMP_EVENT_PASSKEY_NOTIFY,
    SDK_BLE_SMP_EVENT_PAIR_COMPLETE,
    SDK_BLE_SMP_EVENT_PAIR_KEYS_NOTIFY,
    SDK_BLE_GAP_EVENT_ADV_COMPLETE,
    SDK_BLE_GAP_EVENT_BLE_ENABLE,
    SDK_BLE_GAP_EVENT_READ_REMOTE_RSSI,
    SDK_BLE_GAP_EVENT_CONN_PARAM_UPDATE,
    SDK_BLE_GAP_EVENT_MAX,
    SDK_BLE_APP_EVENT_ANY = 0xFFFF,
} sdk_ble_gap_cbk_event;

typedef struct {
    uint8_t status;
    uint16_t connection_handle;
    uint16_t conn_interval;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} sdk_ble_connection_param_t;

void ble_gap_internal_cbk(uint16_t event, void *arg);

bth_gap_conn_status_node *ble_gap_get_conn_node_by_handle(uint16_t conn_handle);

/* 对上API：发起广播 */
uint8_t ble_conn_gap_start_adv(void);
/* 对上API：停止广播 */
uint8_t ble_conn_gap_stop_adv(void);
/* 对上API：获取链路状态和配对状态：0：未连接、未配对；1：已连接、已配对 */
uint8_t ble_conn_gap_get_status(uint8_t *rmt_addr, uint8_t *conn_state, uint8_t *pair_status);

uint8_t ble_gap_callback_regist(sdk_ble_gap_cbk_event event, sdk_ble_cbk cbk);

/* start adv use default paramters */
uint8_t mc_sdk_ble_start_adv_atb(bth_gap_set_extern_adv *adv_param);

/* disable adv */
uint8_t mc_sdk_ble_stop_adv_atb(uint8_t adv_handle);

/* start adv use user paramters */
uint8_t mc_sdk_ble_set_conn_param_atb(uint8_t *adv_param, uint32_t adv_param_len);

/* start extern scan */
uint8_t mc_sdk_ble_set_ext_scan_atb(bth_gap_set_extern_scan *scan_param);

/* stop extern scan */
uint8_t mc_sdk_ble_dis_ext_scan_atb(void);

/* set le phy */
uint8_t mc_sdk_ble_set_phy(bth_gap_le_phys_ampid_addr *param);

/* set data length */
uint8_t mc_sdk_ble_set_data_length(bth_gap_le_data_length_ampid_addr *param);

/* connect reomte peer */
uint8_t mc_sdk_ble_conn_peer_atb(uint8_t *addr, uint8_t length);

/* disconnect reomte peer */
uint8_t mc_sdk_ble_disconn_peer_atb(bth_address *rmt_addr);

/* set addr */
uint8_t mc_sdk_ble_set_local_addr_atb(uint8_t *addr, uint8_t len);

/* set the local name */
uint8_t mc_sdk_ble_set_local_name_atb(uint8_t *name, uint8_t len);

uint8_t mc_sdk_ble_set_local_apperance_atb(uint16_t appr);

uint8_t mc_sdk_ble_update_connect_params_atb(bth_gap_connection_param *con_params);

uint8_t mc_sdk_ble_disconnect_link_atb(bth_gap_conn_status_node *conn_node);

uint8_t mc_sdk_ble_set_sec_param_atb(bth_smp_sec_param *sec_param);

/* set bypass cbk func */
void mc_bt_set_data_bypass_cbk(sdk_ble_cbk func);

uint8_t mc_sdk_ble_gatt_gap_reg_atb(void);

uint32_t mc_ble_read_rssi_atb(uint16_t connect_handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_BLE_GAP_H_ */
