/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT A2DP Service module.
 */
#ifndef __BT_PBAP_SERVICE_H__
#define __BT_PBAP_SERVICE_H__

#include "stack_def.h"
#include "bts_def.h"
#include "btsrv_task.h"

/* 发送PBAP Service模块消息 */
#define pbap_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_PBAP_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

void pbap_set_actice_dev_addr(const bd_addr_t *addr);
void pbap_set_rmt_dev_hdl(uint32_t hdl);
void pbap_reset_conn_status(void);
void pbap_service_init(void);
void pbap_conn_event_callback(const bd_addr_t *addr, uint16_t event);

#endif /* __BT_PBAP_SERVICE_H__ */
