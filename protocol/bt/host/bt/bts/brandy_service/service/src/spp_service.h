/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT Serial Port Profile Service module.
 */
#ifndef __BT_SPP_SERVICE_H__
#define __BT_SPP_SERVICE_H__

#include "btsrv_task.h"

/* SPP service 模块消息ID */
enum {
    SPP_MSG_ID_HANDLE_SPP_CONNECT,
    SPP_MSG_ID_HANDLE_SPP_DISCONNECT,
    SPP_MSG_ID_HANDLE_SPP_CONNECTION_EVENT,
};

/* 发送A2DP Service模块消息 */
#define spp_service_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_SPP_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void spp_service_init(void);
void spp_service_deinit(void);
int32_t spp_read(int32_t client_id, char *buf, const uint32_t buf_len);

#endif /* __BT_SPP_SERVICE_H__ */
