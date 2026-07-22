/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT AVRCP Service Mgr module.
 */
#ifndef __BT_AVRCP_SERVICE_MGR_H__
#define __BT_AVRCP_SERVICE_MGR_H__

#include "stack_def.h"
#include "btsrv_task.h"
#include "bts_def.h"
#include "stdbool.h"
/* AVRCP SRV service 模块消息ID */
enum avrcp_srv_msg_hdl_id {
    AVRCP_SRV_CT_MSG_ID,
    AVRCP_CT_MSG_ID_HANDLE_REMOTE_SEP_CAP,
    AVRCP_CT_MSG_ID_HANDLE_CONN_STATE_CHG,
    AVRCP_CT_MSG_ID_HANDLE_PASS_THROUGH_CHG,
    AVRCP_CT_MSG_ID_HANDLE_SET_VOLUME,
    AVRCP_SRV_TG_MSG_ID,
    AVRCP_TG_MSG_ID_HANDLE_REMOTE_SEP_CAP,
    AVRCP_TG_MSG_ID_HANDLE_CONN_STATE_CHG,
    AVRCP_TG_MSG_ID_HANDLE_PASS_THROUGH_CHG,
    AVRCP_TG_MSG_ID_HANDLE_SET_VOLUME,
    AVRCP_TG_MSG_ID_HANDLE_REG_NOTIFY_EVENT,
    AVRCP_SRV_MSG_ID_HANDLE_DELETE_SERVICE,
    AVRCP_SRV_MSG_ID_HANDLE_CT_EVENT_RSP_CBK,
};

enum avrcp_bts_button_value {
    AVRCP_SRV_PRESS_VALUE,
    AVRCP_SRV_RELEASED_VALUE,
    AVRCP_SRV_INIT_VALUE
};

enum avrcp_msg_pos {
    AVRCP_SRV_PARAM0,
    AVRCP_SRV_PARAM1,
    AVRCP_SRV_PARAM2,
    AVRCP_SRV_PARAM3,
};

typedef struct {
    uint32_t size; /* Size of the content stored in the data */
    uint8_t data[1]; /* Actual data */
} avrcp_evt_param_inf;

typedef void (*avrcp_srv_msg_handle_func)(uint32_t dev_hdl, uint8_t value);
typedef struct {
    uint32_t id;
    avrcp_srv_msg_handle_func func;
} avrcp_serv_hdl_func_type;
/* 发送AVRCP Service模块消息 */
#define avrcp_srv_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_AVRCP_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

#define avrcp_srv_ct_send_msg(p1, p2, p3)             \
    btsrv_send_simple_msg(BTSRV_MSG_ID_AVRCP_SERVICE, \
        (uintptr_t)(AVRCP_SRV_CT_MSG_ID), (uintptr_t)(p1), (uintptr_t)(p2), (uintptr_t)(p3))

#define avrcp_srv_tg_send_msg(p1, p2, p3)             \
    btsrv_send_simple_msg(BTSRV_MSG_ID_AVRCP_SERVICE, \
        (uintptr_t)(AVRCP_SRV_TG_MSG_ID), (uintptr_t)(p1), (uintptr_t)(p2), (uintptr_t)(p3))

enum avrcp_bts_conn_enum {
    AVRCP_SRV_PEER_CONN_CT,
    AVRCP_SRV_PEER_CONN_TG,
    AVRCP_SRV_PEER_CONN_MAX_TYPE,
    AVRCP_SRV_DISCONN_CT,
    AVRCP_SRV_DISCONN_TG,
};

void avrcp_srv_service_init(void);
void avrcp_srv_service_deinit(void);
int32_t avrcp_srv_connect(const bd_addr_t *bd_addr, uint16_t avrcp_srv_class);
int32_t avrcp_srv_disconnect(const bd_addr_t *bd_addr, uint16_t peer_srv_class);
int avrcp_srv_get_device_connect_state(const bd_addr_t *bd_addr, uint16_t peer_srv_calss);
uint8_t avrcp_srv_get_conn_stat(const bd_addr_t *bd_addr, uint16_t peer_srv_calss);
bool avrcp_srv_is_conn(const bd_addr_t *bd_addr, uint16_t peer_srv_calss);
void avrcp_srv_set_connection_state(const bd_addr_t *bd_addr, uint16_t peer_srv_calss, uint8_t conn_stat);
bd_addr_t *avrcp_srv_get_active_addr(uint32_t dev_hdl);
bool avrcp_srv_is_active_addr(const bd_addr_t *bd_addr);
uint32_t avrcp_srv_get_dev_hdl(const bd_addr_t *bd_addr);
void avrcp_srv_set_conn_inf(uint32_t conn_hdl, uint16_t event, uint8_t *conn_inf);
#endif /* __BT_AVRCP_TG_SERVICE_H__ */
