/*
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT service gatt server module.
 */
#ifndef __BTSRV_GATT_SERVER_H__
#define __BTSRV_GATT_SERVER_H__

#include <stdint.h>
#include "btsrv_task.h"
#include "bts_def.h"
#ifdef __cplusplus
extern "C" {
#endif
#define MAX_SERVER_NUM 8

typedef enum {
    GATTS_MSG_ID_REQUEST_READ,          /* 收到远端Client读请求上报 */
    GATTS_MSG_ID_REQUEST_WRITE,         /* 收到远端Client写请求上报 */
    GATTS_MSG_ID_MTU_CHANGE,            /* 服务端MTU发生变化上报 */
    GATTS_MSG_ID_SERVICE_ADD,           /* 服务端添加Service结果上报 */
    GATTS_MSG_ID_INCLUDE_SVC_ADD,       /* 服务端添加Include Service结果上报 */
    GATTS_MSG_ID_CHAR_ADD,              /* 服务端添加Characteristic结果上报 */
    GATTS_MSG_ID_DESCRIPTOR_ADD,        /* 服务端添加Descriptor结果上报 */
    GATTS_MSG_ID_SERVICE_START,         /* 服务端启动Service结果上报 */
    GATTS_MSG_ID_SERVICE_STOP,          /* 服务端停止Service结果上报 */
    GATTS_MSG_ID_SERVICE_DELETE,        /* 服务端删除Service结果上报 */
} gatt_server_msg_id;

enum { /* Attribute Opcode Summary */
    ATT_ERROR_RSP =                              0x01,
    ATT_EXCHANGE_MTU_REQ =                       0x02,
    ATT_EXCHANGE_MTU_RSP =                       0x03,
    ATT_FIND_INFORMATION_REQ =                   0x04,
    ATT_FIND_INFORMATION_RSP =                   0x05,
    ATT_FIND_BY_TYPE_VALUE_REQ =                 0x06,
    ATT_FIND_BY_TYPE_VALUE_RSP =                 0x07,
    ATT_READ_BY_TYPE_REQ =                       0x08,
    ATT_READ_BY_TYPE_RSP =                       0x09,
    ATT_READ_REQ =                               0x0A,
    ATT_READ_RSP =                               0x0B,
    ATT_READ_BLOB_REQ =                          0x0C,
    ATT_READ_BLOB_RSP =                          0x0D,
    ATT_READ_MULTIPLE_REQ =                      0x0E,
    ATT_READ_MULTIPLE_RSP =                      0x0F,
    ATT_READ_BY_GROUPTYPE_REQ =                  0x10,
    ATT_READ_BY_GROUPTYPE_RSP =                  0x11,
    ATT_WRITE_REQ =                              0x12,
    ATT_WRITE_RSP =                              0x13,
    ATT_WRITE_COMMAND =                          0x52, /* 0x12 | ATT_COMMAND */
    ATT_WRITE_COMMAND_SIGNED =                   0xD2, /* 0x12 | ATT_COMMAND | ATT_SIGNED */
    ATT_PREPARE_WRITE_REQ =                      0x16,
    ATT_PREPARE_WRITE_RSP =                      0x17,
    ATT_EXECUTE_WRITE_REQ =                      0x18,
    ATT_EXECUTE_WRITE_RSP =                      0x19,
    ATT_HANDLE_VALUE_NOTIFICATION =              0x1B, /* 27, no 0x1A, 0x1C */
    ATT_HANDLE_VALUE_INDICATION =                0x1D, /* 29 */
    ATT_HANDLE_VALUE_CONFIRMATION =              0x1E, /* 30 */
    ATT_READ_MULTIPLE_VARIABLE_REQ =             0x20,
    ATT_READ_MULTIPLE_VARIABLE_RSP =             0x21,
    ATT_MULTIPLE_HANDLE_VALUE_NOTIFICATION =     0x23
};

/* 发送GATT Server模块消息 */
#define gatt_server_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_GATT_SERVER, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))
/**
 * @ingroup bts_gatt_server
 * @brief  Callback invoked in response to a read request, for system application use only. \n
 *         CNcomment: 服务端收到远端client发送的读请求回调函数，仅提供给系统应用使用，不对三方应用开放。CNend
 *
 * @par Description:
 *           When registered, this callback notifies the upper layer when a read request is received. \n
 *           CNcomment:注册该回调函数之后，BTS在服务端收到读请求后调用该接口上报请求给上层。CNend
 *
 * @attention  1. This function is called in bts context,should not be blocked or do long time waiting. \n
 *                CNcomment:1. 该回调函数运行于bts线程，不能阻塞或长时间等待。CNend \n
 *             2. The memories of <devices> are requested and freed by the bts automatically. \n
 *                CNcomment:2. <devices>由bts申请内存，也由bts释放，回调中不应释放。CNend
 * @param  buff     [IN]  Type  #uint8_t *, data being read. CNcomment:读取的数据。CNend
 * @param  len      [IN]  Type  #uint32_t *, length of read data. CNcomment:读取的数据长度。CNend
 *
 * @retval #void  no return value. CNcomment:无返回值。CNend
 * @par Dependency:
 *            @li bts_gatt_server.h:  bluetooth
 * @see  gatts_callbacks_t
 */

typedef uint32_t (*gatts_service_read)(uint8_t *buff, uint32_t *len);

/**
 * @ingroup bts_gatt_server
 * @brief  Callback invoked in response to a write request, for system application use only. \n
 *         CNcomment: 服务端收到远端client发送的写请求回调函数，仅提供给系统应用使用，不对三方应用开放。CNend
 *
 * @par Description:
 *           When registered, this callback notifies the upper layer when a write request is received. \n
 *           CNcomment:注册该回调函数之后，BTS在服务端收到写请求后调用该接口上报请求给上层。CNend
 *
 * @attention  1. This function is called in bts context,should not be blocked or do long time waiting. \n
 *                CNcomment:1. 该回调函数运行于bts线程，不能阻塞或长时间等待。CNend \n
 *             2. The memories of <devices> are requested and freed by the bts automatically. \n
 *                CNcomment:2. <devices>由bts申请内存，也由bts释放，回调中不应释放。CNend
 * @param  buff     [IN]  Type  #uint8_t *, data being written. CNcomment:写入的数据。CNend
 * @param  len      [IN]  Type  #uint32_t *, length of written data. CNcomment:写入的数据长度。CNend
 *
 * @retval #void  no return value. CNcomment:无返回值。CNend
 * @par Dependency:
 *            @li bts_gatt_server.h:  bluetooth
 * @see  gatts_callbacks_t
 */
typedef uint32_t (*gatts_service_write)(uint8_t *buff, uint32_t len);

/**
 * @ingroup bts_gatt_server
 * @brief  Callback invoked in response to sending notification/indication, for system application use only. \n
 *         CNcomment: 服务端给服务发送通知或指示后回调函数，仅提供给系统应用使用，不对三方应用开放。CNend
 *
 * @par Description:
 *           When registered, this callback notifies the upper layer if a notificaiton/indication is sent. \n
 *           CNcomment:注册该回调函数之后，BTS在服务端给服务发送通知或指示后调用该接口上报结果给上层。CNend
 *
 * @attention  1. This function is called in bts context,should not be blocked or do long time waiting. \n
 *                CNcomment:1. 该回调函数运行于bts线程，不能阻塞或长时间等待。CNend \n
 *             2. The memories of <devices> are requested and freed by the bts automatically. \n
 *                CNcomment:2. <devices>由bts申请内存，也由bts释放，回调中不应释放。CNend
 * @param  buff     [IN]  Type  #uint8_t *, data being notified/indicated. CNcomment:通知/指示数据。CNend
 * @param  len      [IN]  Type  #uint32_t *, length of notified/indicated data. CNcomment:通知/指示数据长度。CNend
 *
 * @retval #void  no return value. CNcomment:无返回值。CNend
 * @par Dependency:
 *            @li bts_gatt_server.h:  bluetooth
 * @see  gatts_callbacks_t
 */
typedef uint32_t (*gatts_service_indicate)(uint8_t *buff, uint32_t len);

/**
 * @ingroup bts_gatt_server
 *
 * Struct of GATT server operation callback function, for system application use only. \n
 * CNcomment:GATT服务端回调操作函数接口定义，仅提供给系统应用使用，不对三方应用开放CNend
 */
typedef struct {
    gatts_service_read read;
    gatts_service_write write;
    gatts_service_indicate indicate;
} gatts_operate_func;

void btsrv_gatt_server_init(void);
void btsrv_gatt_server_deinit(void);

void btsrv_dfx_show_gatts_server_list(void);
void btsrv_dfx_show_gatts_service_list(void);
#ifdef __cplusplus
}
#endif
#endif /* __BTSRV_GATT_SERVER_H__ */
