/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: Connection Manager Module.
 */
#ifndef __BTSRV_CONNECTION_MANAGER_H__
#define __BTSRV_CONNECTION_MANAGER_H__

#include <td_base.h>
#include <stdbool.h>
#include <soc_osal.h>
#include "bth_stack_gap.h"
#include "stack_def.h"
#include "btsrv_task.h"
#include "bts_br_gap.h"
#include "datatype.h"
#include "bts_def.h"

typedef enum {
    CONN_MSG_ID_SCAN_MODE_TIMEOUT,              /* 扫描模式定时器到期 */
    CONN_MSG_ID_START_BT_DISCOVERY,             /* 启动BT扫描 */
    CONN_MSG_ID_HANDLE_BT_DISCOVERY_COMPLETE,   /* 处理BT扫描结束 */
    CONN_MSG_ID_RCV_CONN_REQ,                   /* 收到连接请求 */
    CONN_MSG_ID_CONNECT_REMOTE_DEVICE,          /* 连接远端设备 */
    CONN_MSG_ID_UPDATE_REMOTE_NAME,             /* 刷新远端设备名称 */
    CONN_MSG_ID_UPDATE_REMOTE_NAME_CFM,         /* 刷新远端设备名称结果确认 */
    CONN_MSG_ID_BTH_REPORT_DISCONNECT,          /* 收到BTH上报断开连接 */
    CONN_MSG_ID_BTH_REPORT_CONNECT,             /* 收到BTH上报连接成功 */
    CONN_MSG_ID_HANDLE_AUTH_FAIL,               /* 鉴权失败处理 */
    CONN_MSG_ID_HANDLE_PAIR_COMPLETE,           /* 配对完成处理 */
    CONN_MSG_ID_DEINIT_DISCONNECT_ALL_ACL,      /* 取消初始化流程，断开所有ACL连接 */
    CONN_MSG_ID_DELETE_CONN_NODE,               /* 删除连接记录 */
    CONN_MSG_ID_DISCONNECT_RMT_DEV,             /* 与远端设备断开连接 */
    CONN_MSG_ID_CREATE_ACL_LINK_CFM,            /* ACL连接结果确认 */
    CONN_MSG_ID_RECONNECT_PHONE_DEVICE,         /* 回连连接手机设备 */

    /* GAP事件上报消息 */
    CONN_MSG_ID_REPORT_GAP_STATE_CHANGED,       /* 蓝牙开关变化上报 */
    CONN_MSG_ID_REPORT_SCAN_MODE_CHANGED,       /* 广播模式变化上报 */
    CONN_MSG_ID_REPORT_DISCOVERY_RESULT,        /* 发现结果上报 */
    CONN_MSG_ID_REPORT_REMOTE_NAME_CHANGED,     /* 远端设备名称变化上报 */
    CONN_MSG_ID_REPORT_REMOTE_COD_CHANGED,      /* 远端设备COD变化上报 */
    CONN_MSG_ID_REPORT_ACL_CONN_STATE,          /* ACL连接状态上报 */
    CONN_MSG_ID_REPORT_LOCAL_NAME_CHANGED,      /* 本地设备名称变化上报 */
    CONN_MSG_ID_REPORT_LOCAL_ADDR_CHANGED,      /* 本地设备地址变化上报 */
    CONN_MSG_ID_REPORT_PAIR_STATUS_CHANGED,     /* 设备配对变化上报 */
    CONN_MSG_ID_REPORT_REMOTE_RSSI,             /* 远端设备RSSI上报 */
} connection_manager_msg_id;

typedef enum {
    BR_LINK_MODE_ACTIVE = 0,
    BR_LINK_MODE_HOLD = 1,
    BR_LINK_MODE_SNIFF = 2,
    BR_LINK_MODE_PARK = 3,
    BR_LINK_MODE_UNCONN = 4,
    BR_LINK_MODE_MAX = 5,
} br_link_mode_type;

/* 连接信息表节点 */
typedef struct {
    uint16_t conn_id;
    bd_addr_t addr;
    uint8_t pair_status;
    uint16_t interval;
    uint16_t latency;
    uint16_t timeout;
    uint8_t ltk_len;
    uint8_t ltk[BTH_BLE_SMP_LTK_LEN];
} gap_ble_conn_list_node;

/* 发送连接管理模块消息 */
#define conn_mgr_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_CONNECTION_MANAGER, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void btsrv_connection_manager_init(void);
void btsrv_connection_manager_deinit(void);
gap_ble_conn_list_node *btsrv_gap_get_conn_node_by_addr(uint8_t *addr);
gap_ble_conn_list_node *btsrv_gap_get_conn_node_by_conn_id(uint16_t conn_id);

bool btsrv_check_conn_list_node_exists(uint32_t dev_hdl);

bool btsrv_check_if_accept_conn_req(const bd_addr_t *bd_addr, uint32_t dev_hdl, int cod, uint8_t link_type);

void btsrv_notify_gap_pair_confirmed(uint32_t dev_hdl, int req_type, int number);

void btsrv_dfx_show_conn_list(void);

void btsrv_dfx_show_ble_conn_list(void);

void btsrv_connect_remote_device(const bd_addr_t *addr, bool phone);
uint32_t bt_set_tx_power(const bd_addr_t *addr, uint8_t pwr_level);
errcode_t btsrv_get_smp_paired_device(uint16_t *paired_device_num, bd_addr_t *addr);
void btsrv_connect_state_change_cbk(uint16_t event, void *arg);
void btsrv_ble_smp_pair_complete_cbk(uint16_t event, void *arg);
void btsrv_ble_smp_pair_keys_notify_cbk(uint16_t event, void *arg);
void bt_srv_conn_callback_register(void);
int32_t btsrv_start_phone_reconnect_timer(const bd_addr_t *addr);
void btsrv_stop_phone_reconnect_timer(void);
bool btsrv_exist_conn_node_of_type(bool phone);
void btsrv_notify_gap_acl_state_changed(const bd_addr_t *addr, gap_acl_state_t state, unsigned int reason);

void btsrv_access_ble_conn_lock(void);
void btsrv_release_ble_conn_lock(void);

#endif /* __BTSRV_CONNECTION_MANAGER_H__ */
