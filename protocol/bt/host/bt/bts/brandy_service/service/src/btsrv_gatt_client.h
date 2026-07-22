/*
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT service gatt client module.
 */
#ifndef __BTSRV_GATT_CLIENT_H__
#define __BTSRV_GATT_CLIENT_H__

#include "stack_def.h"
#include "btsrv_task.h"
#include "bts_def.h"
#ifdef __cplusplus
extern "C" {
#endif
#define MAX_CONN_NUM 8
#define MAX_CLIENT_NUM 64
#define INVALID_CLIENT_ID 0

typedef enum {
    GATTC_MSG_ID_DISCOVERY_SERVICE,             /* 服务发现结果上报 */
    GATTC_MSG_ID_DISCOVERY_CHARACTER,           /* 特征发现结果上报 */
    GATTC_MSG_ID_DISCOVERY_DESCRIPTOR,          /* 描述符发现结果上报 */
    GATTC_MSG_ID_DISCOVERY_SERVICE_COMPLETE,    /* 服务发现完成上报 */
    GATTC_MSG_ID_DISCOVERY_CHARACTER_COMPLETE,  /* 特征发现完成上报 */
    GATTC_MSG_ID_DISCOVERY_DESCRIPTOR_COMPLETE, /* 描述符发现完成上报 */
    GATTC_MSG_ID_MTU_CHANGE,                    /* MTU大小变化上报 */
    GATTC_MSG_ID_NOTIF_RCV,                     /* 收到通知上报 */
    GATTC_MSG_ID_IND_RCV,                       /* 收到指示上报 */
    GATTC_MSG_ID_READ,                          /* 读结果上报 */
    GATTC_MSG_ID_READ_COMPLETE,                 /* 按照uuid读完成上报 */
    GATTC_MSG_ID_WRITE,                         /* 写请求结果上报 */
} gatt_client_msg_id;

/* 发送GATT Client模块消息 */
#define gatt_client_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_GATT_CLIENT, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void btsrv_gatt_client_init(void);
void btsrv_gatt_client_deinit(void);
#ifdef __cplusplus
}
#endif
#endif /* __BTSRV_GATT_CLIENT_H__ */
