/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: sdk api common
 */
#ifndef AT_COMMAND_API_BRANDY_ADAPTER_H
#define AT_COMMAND_API_BRANDY_ADAPTER_H

#include "sdk_ble.h"
#include "chip_definitions.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(CHIP_BRANDY)
int mc_sdk_cmd_avrcp_conn_set(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_set_absolute_volume(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_register_notification(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_pass_through(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avctp_cont_brow_set(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avctp_cont_brow_rel(void);
int mc_sdk_cmd_avrcp_sub_unit_info(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_unit_info(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_folder_items_get(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_browsed_player_set(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_addressed_player_set(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_total_num_of_items_get(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_search(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_register_sep(void);
int mc_sdk_cmd_avrcp_pass_through_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_set_absolute_vol_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_capabilities_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_event_player_app_set_cmd_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_event_vol_changed_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_tg_set_vol_changed(void);
int mc_sdk_cmd_avrcp_event_track_changed_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_ct_event_get_folder_items_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_change_path(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_play_item(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_add_to_now_playing(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_set_browsed_player_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_set_addressed_player_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_play_status_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_element_attr_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_tg_reject_register_notification(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_event_player_app_set_changed_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_folder_items_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_folder_items_other_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_event_addr_player_changed_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_event_available_players_changed_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_tg_event_uid_changed_rsp(void);
int mc_sdk_cmd_avrcp_get_folder_items_extern_one_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_folder_items_extern_two_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_get_folder_items_extern_three_rsp(uint8_t *data, uint16_t len);
int mc_sdk_cmd_avrcp_state_init(void);
int mc_sdk_cmd_avrcp_get_element_attr_fragment_rsp(uint8_t *data, uint16_t len);
/* hfp */
int mc_sdk_cmd_hfp_audio_conn_trans_atb(void);
int mc_sdk_cmd_hfp_dial_atb(void);
int mc_sdk_cmd_hfp_dial_second_atb(void);
int mc_sdk_cmd_hfp_last_num_redial_atb(void);
int mc_sdk_cmd_hfp_answer_call_atb(void);
int mc_sdk_cmd_hfp_cancel_call_atb(void);
int mc_sdk_cmd_hfp_tag_phone_num_req_atb(void);
int mc_sdk_cmd_hfp_hold_incoming_call_atb(void);
int mc_sdk_cmd_hfp_accept_held_incoming_call_atb(void);
int mc_sdk_cmd_hfp_reject_held_incoming_call_atb(void);
int mc_sdk_cmd_hfp_get_current_call(void);
int mc_sdk_cmd_hfp_get_ag_features_atb(void);
int mc_sdk_cmd_hfp_3_way_calling_chld_0_atb(void);
int mc_sdk_cmd_hfp_3_way_calling_chld_3_atb(void);
int mc_sdk_cmd_hfp_3_way_calling_chld_4_atb(void);
int mc_sdk_cmd_hfp_disconn_atb(void);
int mc_sdk_cmd_hfp_get_subscriber_number_atb(void);
int mc_sdk_cmd_hfp_disable_nrec_atb(void);
int mc_sdk_cmd_hfp_transmit_dtmf_code_atb(uint8_t *data, uint16_t len);
int mc_sdk_cmd_hfp_voice_recognition_req_atb(uint8_t *data, uint16_t len);
int mc_sdk_cmd_hfp_mic_vol_ctl_atb(uint8_t *data, uint16_t len);
int mc_sdk_cmd_hfp_speaker_vol_ctl_atb(uint8_t *data, uint16_t len);
int mc_sdk_cmd_hfp_create_conn_atb(uint8_t *data, uint16_t len);
int mc_sdk_cmd_hfp_3_way_calling_chld_1_atb(uint8_t *data, uint8_t len);
int mc_sdk_cmd_hfp_3_way_calling_chld_2_atb(uint8_t *data, uint8_t len);
int mc_sdk_cmd_sdp_sa_add_attr_atb(void);
int mc_sdk_cmd_sdp_ssa_add_attr_atb(void);
int mc_sdk_cmd_sdp_ssa_add_addi_desc_atb(void);
int mc_sdk_cmd_sdp_add_record_atb(void);
int mc_sdk_cmd_sdp_rm_record_atb(void);
/* avctp */
int mc_sdk_cmd_avctp_connect_req(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_avctp_disconnect_req(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_avctp_ct_send_nfr_req(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_avctp_ct_send_fra_req(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_avctp_tg_reg_cmd_cbk(bool enable);
/* A2DP */
int mc_sdk_cmd_a2dp_src_conn(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_a2dp_src_disconn(uint32_t stream_hdl);
int mc_sdk_cmd_a2dp_src_suspend_stream(uint32_t stream_hdl);
int mc_sdk_cmd_a2dp_src_send_stream(uint8_t codec_type, uint32_t stream_handle);
/* AVDTP */
int mc_sdk_cmd_avdtp_get_conf(uint32_t stream_hdl);
int mc_sdk_cmd_avdtp_register_cap(uint8_t service_category);
int mc_sdk_cmd_avdtp_abort(uint32_t stream_hdl);
/* pbap */
int mc_sdk_cmd_pbap_connect_req(uint8_t *addr, uint32_t chnl_psm);
int mc_sdk_cmd_pbap_disconnect_req(void);
int mc_sdk_cmd_pbap_pull_phone_book(uint16_t max_count, uint8_t format, uint8_t flag,
    uint8_t call, uint8_t selector, uint8_t selector_operator, const char *path);
int mc_sdk_cmd_pbap_pull_card_list(uint16_t max_count, uint8_t search_attr, uint8_t call,
    uint8_t selector, uint8_t *search_value, const char *path);
int mc_sdk_cmd_pbap_pull_card_entry(uint8_t format, uint8_t flag1, const char *vcf_name, uint8_t flag2);
int mc_sdk_cmd_pbap_set_path_req(char *path);
int mc_sdk_cmd_pbap_abort_req(void);
/* RFCOMM */
int mc_sdk_cmd_rfcomm_connect(uint8_t *bd, uint8_t channel);
int mc_sdk_cmd_rfcomm_disconnect(void);
int mc_sdk_cmd_rfcomm_conn_rsp(uint8_t *bd, uint8_t server_chnl);
int mc_sdk_cmd_rfcomm_data_req(void);
int mc_sdk_cmd_rfcomm_rls_req(uint8_t status);
int mc_sdk_cmd_rfcomm_reg_server(uint8_t channel);

/* PAN */
int mc_sdk_cmd_pan_connnect(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_pan_disconnnect(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_pan_filter_protocol(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_pan_filter_multiaddr(uint8_t *addr, uint16_t param_len);
int mc_sdk_cmd_pan_ethernet_send(uint8_t *data, uint16_t param_len);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* AT_COMMAND_API_H */
