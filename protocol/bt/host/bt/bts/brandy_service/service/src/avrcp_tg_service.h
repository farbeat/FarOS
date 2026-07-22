/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT AVRCP TG Service module.
 */
#ifndef __BT_AVRCP_TG_SERVICE_H__
#define __BT_AVRCP_TG_SERVICE_H__

#include "btsrv_task.h"

void avrcp_tg_service_init(void);
void avrcp_tg_service_deinit(void);
void avrcp_tg_handle_msg(const ext_sys_queue_msg *msg);

void avrcp_tg_register_callback(void);
void avrcp_tg_deregister_callback(void);

#endif /* __BT_AVRCP_TG_SERVICE_H__ */
