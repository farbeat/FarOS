/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT HFP_HF Service module.
 */
#ifndef __BT_HFP_HF_SERVICE_H__
#define __BT_HFP_HF_SERVICE_H__

#include <stdint.h>
#include "btsrv_task.h"
#include "bts_def.h"
#include "stack_gap_if.h"

typedef struct {
    uintptr_t dev_hdl;
    bd_addr_t addr;
} hfp_hf_remote_device_info;

/* HFP-HF service 模块消息ID */
enum {
    HFP_HF_MSG_ID_NOTIFY_SCO_STATE_CHANGED, /* 上报SCO状态变化 */
    HFP_HF_MSG_ID_NOTIFY_CALL_STATE_CHANGED, /* 上报通话状态变化 */
    HFP_HF_MSG_ID_NOTIFY_CALL_LIST_CHANGED, /* 上报通话列表变化 */
    HFP_HF_MSG_ID_NOTIFY_AT_COMMOND_RESULT, /* 上报AT命令结果 */
    HFP_HF_MSG_ID_HANDLE_CONN_EVENT,          /* 处理HFP HF服务连接事件 */
};

/* 发送HFP-HF Service模块消息 */
#define hfp_hf_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_HFP_HF_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void hfp_hf_service_init(void);
void hfp_hf_service_deinit(void);

void hfp_hf_set_actice_dev_addr(const bd_addr_t *addr);
int32_t hfp_hf_codec_setup(const bd_addr_t *bd_addr, uint8_t codec_type);
int hfp_hf_connect(const bd_addr_t *bd_addr);
void hfp_set_rmt_dev_hdl(uint32_t hdl);
void hfp_hf_accept_or_cancel_sco(uint8_t is_accept);
void hfp_hf_connection_event_ind(const bt_sdk_connection_property_stru *conn, uint32_t conn_hdl, uint16_t event);

#endif /* __BT_HFP_HF_SERVICE_H__ */
