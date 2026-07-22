/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test functions of BT AVRCP Service module.
 */
#ifndef SAMPLE_BT_AVRCP_SERVICE_H
#define SAMPLE_BT_AVRCP_SERVICE_H

#include <stdint.h>
#include "bts_def.h"

void bt_avrcp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
void wstp_avrcp_register_callbacks(void);
void wstp_avrcp_disregister_callbacks(void);
int wstp_avrcp_tg_connect(const bd_addr_t *bd_addr);
int wstp_avrcp_tg_disconnect(const bd_addr_t *bd_addr);
int wstp_avrcp_tg_get_dev_conn_state(const bd_addr_t *bd_addr);
void wstp_avrcp_tg_notify_state(const uint8_t *params, int32_t params_cnt);
void wstp_avrcp_tg_set_loc_inf(const uint8_t *params, int32_t params_cnt);
void wstp_avrcp_ct_conn_and_reg(const bd_addr_t *bd_addr);
int wstp_avrcp_ct_disconnect(const bd_addr_t *bd_addr);
void wstp_avrcp_ct_press_button(const bd_addr_t *bd_addr, int32_t key_operation);
int wstp_bts_avrcp_ct_set_abs_volume(const bd_addr_t *bd_addr, uint8_t volume);
int wstp_avrcp_ct_get_tg_player_stat(const bd_addr_t *bd_addr);
void wstp_avrcp_ct_get_element_attributes(const bd_addr_t *bd_addr);
int wstp_avrcp_ct_get_support_event(const bd_addr_t *bd_addr);
void wstp_avrcp_ct_reg_support_event(const bd_addr_t *bd_addr, uint8_t event_id, uint32_t interval);
int wstp_avrcp_ct_get_conn_stat(const bd_addr_t *bd_addr);
int wstp_avrcp_ct_get_support_companies(const bd_addr_t *bd_addr);

#endif /* SAMPLE_BT_A2DP_SERVICE_H */
