/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test functions of BT A2DP Service module.
 */
#ifndef SAMPLE_BT_A2DP_SERVICE_H
#define SAMPLE_BT_A2DP_SERVICE_H

#include <stdint.h>
#include "bts_def.h"

void bt_a2dp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
int wstp_a2dp_src_register_callbacks(void);
int wstp_a2dp_src_deregister_callbacks(void);
int wstp_a2dp_src_get_device_connect_state(const bd_addr_t *bd_addr);
int wstp_a2dp_src_get_playing_state(const bd_addr_t *bd_addr);
int wstp_a2dp_src_connect(const bd_addr_t *bd_addr);
int wstp_a2dp_src_disconnect(const bd_addr_t *bd_addr);
void wstp_a2dp_src_get_active_device(void);

#endif /* SAMPLE_BT_A2DP_SERVICE_H */
