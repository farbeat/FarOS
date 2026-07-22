/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: BT PAN Service module.
 */
#ifndef BT_PAN_SERVICE_H
#define BT_PAN_SERVICE_H

#include "btsrv_task.h"
#include "bts_def.h"

#define PAN_POS_2    2
#define PAN_POS_3    3

typedef struct {
    uintptr_t dev_hdl;
    bd_addr_t addr;
} pan_remote_device_info;

/* PAN service 模块消息ID */
enum {
    PAN_MSG_ID_HANDLE_CONN_EVENT,          /* 处理PAN服务连接事件 */
};

/* 发送PAN Service模块消息 */
#define pan_service_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_PAN_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void pan_service_init(void);

#endif