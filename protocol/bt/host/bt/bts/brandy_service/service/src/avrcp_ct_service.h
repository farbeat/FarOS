/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT AVRCP CT Service module.
 */
#ifndef __BT_AVRCP_CT_SERVICE_H__
#define __BT_AVRCP_CT_SERVICE_H__

#include "btsrv_task.h"
#include "bts_def.h"

void avrcp_ct_service_init(void);

void avrcp_ct_service_deinit(void);

void avrcp_ct_handle_msg(const ext_sys_queue_msg *msg);

int avrcp_ct_set_absolute_volume(const bd_addr_t *bd_addr, unsigned char volume);
void avrcp_ct_event_rsp_cbk_proc(uint32_t dev_hdl, uint16_t rsp_type, uint8_t *param);

#endif /* __BT_AVRCP_TG_SERVICE_H__ */
