/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT HFP_AG Service module.
 */
#ifndef BT_HFP_AG_SERVICE_H
#define BT_HFP_AG_SERVICE_H

#include <stdint.h>
#include "bts_def.h"
#include "btsrv_task.h"
#include "stack_gap_if.h"

typedef struct {
    uintptr_t dev_hdl;
    bd_addr_t addr;
} hfp_ag_remote_device_info;

/* hfp ag service 模块消息ID */
enum {
    HFP_AG_MSG_ID_HANDLE_CONN_EVENT,          /* 处理HFP AG服务连接事件 */
};

/* 发送hfp ag service模块消息 */
#define hfp_ag_service_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_HFP_AG_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void hfp_ag_service_init(void);
void hfp_ag_service_deinit(void);
void hfp_ag_connection_event_ind(const bt_sdk_connection_property_stru *conn, uint32_t conn_hdl, uint16_t event);
#endif /* BT_HFP_AG_SERVICE_H */
