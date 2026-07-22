/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: sdk api common
 * Author: @CompanyNameTag
 * Create: 2021-09-27
 */

#ifndef AT_COMMAND_API_BRANDY_H
#define AT_COMMAND_API_BRANDY_H

#include "sdk_ble.h"
#include "chip_definitions.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(CHIP_BRANDY)

#define BT_AT_DISC_PHONE_FLAG 1
#define BT_AT_ADDR_TYPE_PUBLIC 0
#define BT_AT_ADDR_TYPE_STATIC 1
#define BT_AT_ADDR_TYPE_MAX 3
#define BT_AT_ADDRESS_CAP_LE 1
#define BT_AT_ADDRESS_CAP_BREDR 2
#define BT_AT_PAIRING_STAUS_LEN 1
#define BT_AT_ADDR_TYPE_LEN 2
#define BT_AT_SC_PARAM_INVALID 0xFF

// gap
int bt_at_cmd_bredr_creat_acl(int argc, char *argv[]);
int bt_at_cmd_bredr_sec_mode_set(int argc, char *argv[]);
int bt_at_cmd_bredr_visual_mode_set(int argc, char *argv[]);
int bt_at_cmd_bredr_bondable_mode_set(int argc, char *argv[]);
int bt_at_cmd_bredr_disconnect_acl(int argc, char *argv[]);
int bt_at_cmd_bredr_gap_enable_discovery(int argc, char *argv[]);
int bt_at_cmd_bredr_gap_pair_rmtdev(int argc, char *argv[]);
int bt_at_cmd_bredr_gap_io_set(int argc, char *argv[]);
int bt_at_cmd_bredr_gap_unpair_rmtdev(int argc, char *argv[]);
int bt_at_cmd_bredr_set_bt_default(int argc, char *argv[]);
int bt_at_cmd_bredr_set_secu_level(int argc, char *argv[]);
int bt_at_cmd_bredr_encryptlink(int argc, char *argv[]);
int bt_at_cmd_bredr_add_remote_dev(int argc, char *argv[]);
// pbap
int bt_at_cmd_bredr_pbap_creat_obex_conn(int argc, char *argv[]);
int bt_at_cmd_bredr_pbap_stop_obex_conn(int argc, char *argv[]);
// hfp
int bt_at_cmd_hfp_audio_conn_trans(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_answer_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_cancel_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_tag_phone_num_req(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_hold_incoming_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_accept_held_incoming_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_reject_held_incoming_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_transmit_dtmf_code(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_voice_recognition_req(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_mic_vol_ctl(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_speaker_vol_ctl(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_create_conn(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_dial(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_dial_second(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_last_num_redial(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_get_current_call(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_get_ag_features(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_3_way_calling_chld_0(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_3_way_calling_chld_1(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_3_way_calling_chld_2(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_3_way_calling_chld_3(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_3_way_calling_chld_4(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_disconn(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_disable_nrec(int argc, char *argv[]);
int bt_at_cmd_hfp_hf_get_subscriber_number(int argc, char *argv[]);
// sdp
int bt_at_sdp_ss_add_attr(int argc, char *argv[]);
int bt_at_sdp_ssa_add_attr(int argc, char *argv[]);
int bt_at_sdp_ssa_add_addi_desc(int argc, char *argv[]);
int bt_at_sdp_add_record(int argc, char *argv[]);
// sdp
int bt_at_cmd_sdp_sa_add_attr(int argc, char *argv[]);
int bt_at_cmd_sdp_ssa_add_attr(int argc, char *argv[]);
int bt_at_cmd_sdp_ssa_add_addi_desc(int argc, char *argv[]);
int bt_at_cmd_sdp_add_record(int argc, char *argv[]);
int bt_at_cmd_sdp_rm_record(int argc, char *argv[]);
// avrcp
int bt_at_cmd_avrcp_conn_set(int argc, char *argv[]);
int bt_at_cmd_avctp_cont_brow_set(int argc, char *argv[]);
int bt_at_cmd_avctp_cont_brow_rel(int argc, char *argv[]);
int bt_at_cmd_avrcp_set_absolute_volume(int argc, char *argv[]);
int bt_at_cmd_avrcp_register_notification(int argc, char *argv[]);
int bt_at_cmd_avrcp_pass_through(int argc, char *argv[]);
int bt_at_cmd_avrcp_sub_unit_info(int argc, char *argv[]);
int bt_at_cmd_avrcp_unit_info(int argc, char *argv[]);
int bt_at_cmd_avrcp_folder_items_get(int argc, char *argv[]);
int bt_at_cmd_avrcp_browsed_player_set(int argc, char *argv[]);
int bt_at_cmd_avrcp_addressed_player_set(int argc, char *argv[]);
int bt_at_cmd_avrcp_total_num_of_items_get(int argc, char *argv[]);
int bt_at_cmd_avrcp_search(int argc, char *argv[]);
int bt_at_cmd_register_sep(int argc, char *argv[]);
int bt_at_cmd_avrcp_pass_through_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_set_absolute_vol_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_capabilities_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_player_app_set_cmd_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_vol_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_tg_set_vol_changed(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_track_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_ct_event_get_folder_items_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_change_path(int argc, char *argv[]);
int bt_at_cmd_avrcp_play_item(int argc, char *argv[]);
int bt_at_cmd_avrcp_add_to_now_playing(int argc, char *argv[]);
int bt_at_cmd_avrcp_set_browsed_player_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_set_addressed_player_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_play_status_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_element_attr_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_tg_reject_register_notification(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_player_app_set_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_folder_items_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_folder_items_other_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_addr_player_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_event_available_players_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_tg_event_uid_changed_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_folder_items_extern_one_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_folder_items_extern_two_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_folder_items_extern_three_rsp(int argc, char *argv[]);
int bt_at_cmd_avrcp_state_init(int argc, char *argv[]);
int bt_at_cmd_avrcp_get_element_attr_fragment_rsp(int argc, char *argv[]);
/* avctp */
int bt_at_cmd_avctp_connect(int argc, char *argv[]);
int bt_at_cmd_avctp_disconnect(int argc, char *argv[]);
int bt_at_cmd_avctp_ct_send_nfr(int argc, char *argv[]);
int bt_at_cmd_avctp_ct_send_fra_cmd(int argc, char *argv[]);
int bt_at_cmd_avctp_tg_register_callback(int argc, char *argv[]);
/* A2DP */
int bt_at_a2dp_src_register_callback(int argc, char *argv[]);
int bt_at_a2dp_src_conn(int argc, char *argv[]);
int bt_at_a2dp_src_disconn(int argc, char *argv[]);
int bt_at_a2dp_src_start_stream(int argc, char *argv[]);
int bt_at_a2dp_src_set_codec(int argc, char *argv[]);
int bt_at_a2dp_src_suspend_stream(int argc, char *argv[]);
int bt_at_a2dp_src_send_stream(int argc, char *argv[]);
/* AVDTP */
int bt_at_avdtp_get_conf(int argc, char *argv[]);
int bt_at_avdtp_register_cap(int argc, char *argv[]);
int bt_at_avdtp_abort(int argc, char *argv[]);
/* pbap */
int bt_at_cmd_pbap_create_connect(int argc, char *argv[]);
int bt_at_cmd_pbap_disconnect(int argc, char *argv[]);
int bt_at_cmd_pbap_pull_phone_book(int argc, char *argv[]);
int bt_at_cmd_pbap_pull_card_list(int argc, char *argv[]);
int bt_at_cmd_pbap_pull_pb_entry(int argc, char *argv[]);
int bt_at_cmd_pbap_set_pb_req(int argc, char *argv[]);
int bt_at_cmd_pbap_abort_pull_action(int argc, char *argv[]);
/* rfcomm */
int bt_at_cmd_rfcomm_connect_req(int argc, char *argv[]);
int bt_at_cmd_rfcomm_disconnect_req(int argc, char *argv[]);
int bt_at_cmd_rfcomm_conn_rsp(int argc, char *argv[]);
int bt_at_cmd_rfcomm_data_req(int argc, char *argv[]);
int bt_at_cmd_rfcomm_rls_req(int argc, char *argv[]);
int bt_at_cmd_rfcomm_reg_server(int argc, char *argv[]);
/* spp */
void bth_dft_command_register_brandy_spp(void);

/* PAN */
int bt_at_cmd_pan_connnect(int argc, char *argv[]);
int bt_at_cmd_pan_disconnnect(int argc, char *argv[]);
int bt_at_cmd_pan_filter_protocol(int argc, char *argv[]);
int bt_at_cmd_pan_filter_multiaddr(int argc, char *argv[]);
int bt_at_cmd_pan_ethernet_send(int argc, char *argv[]);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* AT_COMMAND_API_H */
