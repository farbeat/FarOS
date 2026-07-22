/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test functions of BT A2DP Service module.
 */
#ifndef SAMPLE_BT_A2DP_SNK_SERVICE_H
#define SAMPLE_BT_A2DP_SNK_SERVICE_H

#include "bts_def.h"

int wstp_a2dp_snk_register_callbacks(void);
int wstp_a2dp_snk_deregister_callbacks(void);
int wstp_a2dp_snk_get_device_connect_state(const bd_addr_t *bd_addr);
int wstp_a2dp_snk_get_playing_state(const bd_addr_t *bd_addr);
int wstp_a2dp_snk_connect(const bd_addr_t *bd_addr);
int wstp_a2dp_snk_disconnect(const bd_addr_t *bd_addr);
void wstp_a2dp_snk_get_active_device(void);

#endif /* SAMPLE_BT_A2DP_SNK_SERVICE_H */
