/*
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: BT profile stubs for brandy-mcu-bt (no audio stack)
 */
#include "td_base.h"
#include "sample_a2dp_service.h"
#include "sample_a2dp_snk_service.h"
#include "sample_avrcp_service.h"
#include "sample_hfp_hf_service.h"
#include "sample_a2dp_service.h"
#include "sample_avrcp_service.h"
#include "sample_hfp_ag_service.h"

#define MCU_BT_STUB_INT int
#define MCU_BT_STUB_VOID void

MCU_BT_STUB_INT wstp_a2dp_src_register_callbacks(void) { return 0; }
MCU_BT_STUB_INT wstp_a2dp_src_deregister_callbacks(void) { return 0; }
MCU_BT_STUB_INT wstp_a2dp_src_get_device_connect_state(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_src_get_playing_state(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_src_connect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_src_disconnect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_a2dp_src_get_active_device(void) { }

MCU_BT_STUB_INT wstp_a2dp_snk_register_callbacks(void) { return 0; }
MCU_BT_STUB_INT wstp_a2dp_snk_deregister_callbacks(void) { return 0; }
MCU_BT_STUB_INT wstp_a2dp_snk_get_device_connect_state(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_snk_get_playing_state(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_snk_connect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_a2dp_snk_disconnect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_a2dp_snk_get_active_device(void) { }

MCU_BT_STUB_VOID wstp_avrcp_register_callbacks(void) { }
MCU_BT_STUB_VOID wstp_avrcp_disregister_callbacks(void) { }
MCU_BT_STUB_INT wstp_avrcp_tg_connect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_avrcp_tg_disconnect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_avrcp_tg_get_dev_conn_state(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_avrcp_tg_notify_state(const uint8_t *params, int32_t params_cnt) { (void)params; (void)params_cnt; }
MCU_BT_STUB_VOID wstp_avrcp_tg_set_loc_inf(const uint8_t *params, int32_t params_cnt) { (void)params; (void)params_cnt; }
MCU_BT_STUB_VOID wstp_avrcp_ct_conn_and_reg(const bd_addr_t *bd_addr) { (void)bd_addr; }
MCU_BT_STUB_INT wstp_avrcp_ct_disconnect(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_avrcp_ct_press_button(const bd_addr_t *bd_addr, int32_t key_operation) { (void)bd_addr; (void)key_operation; }
MCU_BT_STUB_INT wstp_bts_avrcp_ct_set_abs_volume(const bd_addr_t *bd_addr, uint8_t volume) { (void)bd_addr; (void)volume; return 0; }
MCU_BT_STUB_INT wstp_avrcp_ct_get_tg_player_stat(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_avrcp_ct_get_element_attributes(const bd_addr_t *bd_addr) { (void)bd_addr; }
MCU_BT_STUB_INT wstp_avrcp_ct_get_support_event(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_VOID wstp_avrcp_ct_reg_support_event(const bd_addr_t *bd_addr, uint8_t event_id, uint32_t interval)
{
    (void)bd_addr; (void)event_id; (void)interval;
}
MCU_BT_STUB_INT wstp_avrcp_ct_get_conn_stat(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }
MCU_BT_STUB_INT wstp_avrcp_ct_get_support_companies(const bd_addr_t *bd_addr) { (void)bd_addr; return 0; }

MCU_BT_STUB_VOID wstp_hfp_hf_reg_callback(void) { }
MCU_BT_STUB_VOID sample_hfp_hf_set_spk_volume(uint8_t volume) { (void)volume; }

void bt_a2dp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    (void)cmd_id; (void)params; (void)params_cnt;
}

void bt_avrcp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    (void)cmd_id; (void)params; (void)params_cnt;
}

void bt_hfp_hf_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    (void)cmd_id; (void)params; (void)params_cnt;
}

void bt_hfp_ag_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    (void)cmd_id; (void)params; (void)params_cnt;
}
