/*
 * Copyright (c) @CompanyNameMagicTag 2019. All rights reserved.
 * Description: bth log functions
 */

#ifndef BT_LOG_H
#define BT_LOG_H
#include "log_printf.h"
#include "log_buffer.h"
#include "hiaudio_user_event.h"
#include "hiaudio_user_error.h"

#define ADDR_1ST_INDEX 0
#define ADDR_2ND_INDEX 1
#define ADDR_3RD_INDEX 2
#define ADDR_5TH_INDEX 4
#define ADDR_6TH_INDEX 5
#define addr0(addr) ((addr)[ADDR_1ST_INDEX])
#define addr1(addr) ((addr)[ADDR_2ND_INDEX])
#define addr2(addr) ((addr)[ADDR_3RD_INDEX])
#define addr4(addr) ((addr)[ADDR_5TH_INDEX])
#define addr5(addr) ((addr)[ADDR_6TH_INDEX])
#define le_addr_to_br0(addr)                                                    \
({                                                                              \
    uint8_t *rmt_addr = twsm_tran_le_addr2br_addr_in_hitws(addr);                       \
    (rmt_addr == NULL) ? addr[ADDR_1ST_INDEX] : rmt_addr[ADDR_1ST_INDEX];         \
})

#define le_addr_to_br1(addr)                                                    \
({                                                                              \
    uint8_t *rmt_addr = twsm_tran_le_addr2br_addr_in_hitws(addr);                       \
    (rmt_addr == NULL) ? addr[ADDR_2ND_INDEX] : rmt_addr[ADDR_2ND_INDEX];         \
})

#define le_addr_to_br2(addr)                                                    \
({                                                                              \
    uint8_t *rmt_addr = twsm_tran_le_addr2br_addr_in_hitws(addr);                       \
    (rmt_addr == NULL) ? addr[ADDR_3RD_INDEX] : rmt_addr[ADDR_3RD_INDEX];         \
})

#define le_addr_to_br4(addr)                                                    \
({                                                                              \
    uint8_t *rmt_addr = twsm_tran_le_addr2br_addr_in_hitws(addr);                       \
    (rmt_addr == NULL) ? addr[ADDR_5TH_INDEX] : rmt_addr[ADDR_5TH_INDEX];         \
})

#define le_addr_to_br5(addr)                                                    \
({                                                                              \
    uint8_t *rmt_addr = twsm_tran_le_addr2br_addr_in_hitws(addr);                       \
    (rmt_addr == NULL) ? addr[ADDR_6TH_INDEX] : rmt_addr[ADDR_6TH_INDEX];         \
})

#define INVALID_VALUE 0

#define bth_dbg_log(subMod, log_num, fmt, count, args...)                       \
    BASE_PRINT_DEBUG(CONNECT(subMod, log_num), fmt, count, ##args)
#define bth_info_log(subMod, log_num, fmt, count, args...)                      \
    BASE_PRINT_INFO(CONNECT(subMod, log_num), fmt, count, ##args)
#define bth_warn_log(subMod, log_num, fmt, count, args...)                      \
    BASE_PRINT_WARN(CONNECT(subMod, log_num), fmt, count, ##args)
#define bth_err_log(subMod, log_num, fmt, count, args...)                       \
    BASE_PRINT_ERR(CONNECT(subMod, log_num), fmt, count, ##args)

// BTH SERVICE LOG FUNCTION BEGIN
#define gaps_dbg_log(log_num, fmt, count, args...)                              \
    bth_dbg_log(LOG_BTH_SERVICE_GAP_SERVICE, log_num, fmt, count, ##args)
#define gaps_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_GAP_SERVICE, log_num, fmt, count, ##args)
#define gaps_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_GAP_SERVICE, log_num, fmt, count, ##args)
#define gaps_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_GAP_SERVICE, log_num, fmt, count, ##args)
#define profs_dbg_log(log_num, fmt, count, args...)                             \
    bth_dbg_log(LOG_BTH_SERVICE_PROFILE_SERVICE, log_num, fmt, count, ##args)
#define profs_info_log(log_num, fmt, count, args...)                            \
    bth_info_log(LOG_BTH_SERVICE_PROFILE_SERVICE, log_num, fmt, count, ##args)
#define profs_warn_log(log_num, fmt, count, args...)                            \
    bth_warn_log(LOG_BTH_SERVICE_PROFILE_SERVICE, log_num, fmt, count, ##args)
#define profs_err_log(log_num, fmt, count, args...)                             \
    bth_err_log(LOG_BTH_SERVICE_PROFILE_SERVICE, log_num, fmt, count, ##args)
#define connm_dbg_log(log_num, fmt, count, args...)                             \
    bth_dbg_log(LOG_BTH_SERVICE_CONN_MGR, log_num, fmt, count, ##args)
#define connm_info_log(log_num, fmt, count, args...)                            \
    bth_info_log(LOG_BTH_SERVICE_CONN_MGR, log_num, fmt, count, ##args)
#define connm_warn_log(log_num, fmt, count, args...)                            \
    bth_warn_log(LOG_BTH_SERVICE_CONN_MGR, log_num, fmt, count, ##args)
#define connm_err_log(log_num, fmt, count, args...)                             \
    bth_err_log(LOG_BTH_SERVICE_CONN_MGR, log_num, fmt, count, ##args)
#define hitwsm_dbg_log(log_num, fmt, count, args...)                            \
    bth_dbg_log(LOG_BTH_SERVICE_HITWS_MGR, log_num, fmt, count, ##args)
#define hitwsm_info_log(log_num, fmt, count, args...)                           \
    bth_info_log(LOG_BTH_SERVICE_HITWS_MGR, log_num, fmt, count, ##args)
#define hitwsm_warn_log(log_num, fmt, count, args...)                           \
    bth_warn_log(LOG_BTH_SERVICE_HITWS_MGR, log_num, fmt, count, ##args)
#define hitwsm_err_log(log_num, fmt, count, args...)                            \
    bth_err_log(LOG_BTH_SERVICE_HITWS_MGR, log_num, fmt, count, ##args)
#define jitwsm_dbg_log(log_num, fmt, count, args...)                            \
    bth_dbg_log(LOG_BTH_SERVICE_JITWS_MGR, log_num, fmt, count, ##args)
#define jitwsm_info_log(log_num, fmt, count, args...)                           \
    bth_info_log(LOG_BTH_SERVICE_JITWS_MGR, log_num, fmt, count, ##args)
#define jitwsm_warn_log(log_num, fmt, count, args...)                           \
    bth_warn_log(LOG_BTH_SERVICE_JITWS_MGR, log_num, fmt, count, ##args)
#define jitwsm_err_log(log_num, fmt, count, args...)                            \
    bth_err_log(LOG_BTH_SERVICE_JITWS_MGR, log_num, fmt, count, ##args)
#define dtsm_dbg_log(log_num, fmt, count, args...)                              \
    bth_dbg_log(LOG_BTH_SERVICE_DTS_MGR, log_num, fmt, count, ##args)
#define dtsm_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_DTS_MGR, log_num, fmt, count, ##args)
#define dtsm_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_DTS_MGR, log_num, fmt, count, ##args)
#define dtsm_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_DTS_MGR, log_num, fmt, count, ##args)
#define datam_dbg_log(log_num, fmt, count, args...)                             \
    bth_dbg_log(LOG_BTH_SERVICE_DATA_MGR, log_num, fmt, count, ##args)
#define datam_info_log(log_num, fmt, count, args...)                            \
    bth_info_log(LOG_BTH_SERVICE_DATA_MGR, log_num, fmt, count, ##args)
#define datam_warn_log(log_num, fmt, count, args...)                            \
    bth_warn_log(LOG_BTH_SERVICE_DATA_MGR, log_num, fmt, count, ##args)
#define datam_err_log(log_num, fmt, count, args...)                             \
    bth_err_log(LOG_BTH_SERVICE_DATA_MGR, log_num, fmt, count, ##args)
#define uxadp_dbg_log(log_num, fmt, count, args...)                             \
    bth_dbg_log(LOG_BTH_SERVICE_UX_ADPATER, log_num, fmt, count, ##args)
#define uxadp_info_log(log_num, fmt, count, args...)                            \
    bth_info_log(LOG_BTH_SERVICE_UX_ADPATER, log_num, fmt, count, ##args)
#define uxadp_warn_log(log_num, fmt, count, args...)                            \
    bth_warn_log(LOG_BTH_SERVICE_UX_ADPATER, log_num, fmt, count, ##args)
#define uxadp_err_log(log_num, fmt, count, args...)                             \
    bth_err_log(LOG_BTH_SERVICE_UX_ADPATER, log_num, fmt, count, ##args)
#define nearby_dbg_log(log_num, fmt, count, args...)                            \
    bth_dbg_log(LOG_BTH_SERVICE_NEARBY, log_num, fmt, count, ##args)
#define nearby_info_log(log_num, fmt, count, args...)                           \
    bth_info_log(LOG_BTH_SERVICE_NEARBY, log_num, fmt, count, ##args)
#define nearby_warn_log(log_num, fmt, count, args...)                           \
    bth_warn_log(LOG_BTH_SERVICE_NEARBY, log_num, fmt, count, ##args)
#define nearby_err_log(log_num, fmt, count, args...)                            \
    bth_err_log(LOG_BTH_SERVICE_NEARBY, log_num, fmt, count, ##args)
#define icarry_dbg_log(log_num, fmt, count, args...)                            \
    bth_dbg_log(LOG_BTH_SERVICE_ICARRY, log_num, fmt, count, ##args)
#define icarry_info_log(log_num, fmt, count, args...)                           \
    bth_info_log(LOG_BTH_SERVICE_ICARRY, log_num, fmt, count, ##args)
#define icarry_warn_log(log_num, fmt, count, args...)                           \
    bth_warn_log(LOG_BTH_SERVICE_ICARRY, log_num, fmt, count, ##args)
#define icarry_err_log(log_num, fmt, count, args...)                            \
    bth_err_log(LOG_BTH_SERVICE_ICARRY, log_num, fmt, count, ##args)
#define spa_dbg_log(log_num, fmt, count, args...)                               \
    bth_dbg_log(LOG_BTH_SERVICE_SPA, log_num, fmt, count, ##args)
#define spa_info_log(log_num, fmt, count, args...)                              \
    bth_info_log(LOG_BTH_SERVICE_SPA, log_num, fmt, count, ##args)
#define spa_warn_log(log_num, fmt, count, args...)                              \
    bth_warn_log(LOG_BTH_SERVICE_SPA, log_num, fmt, count, ##args)
#define spa_err_log(log_num, fmt, count, args...)                               \
    bth_err_log(LOG_BTH_SERVICE_SPA, log_num, fmt, count, ##args)
#define ipc_dbg_log(log_num, fmt, count, args...)                               \
    bth_dbg_log(LOG_BTH_SERVICE_IPC, log_num, fmt, count, ##args)
#define ipc_info_log(log_num, fmt, count, args...)                              \
    bth_info_log(LOG_BTH_SERVICE_IPC, log_num, fmt, count, ##args)
#define ipc_warn_log(log_num, fmt, count, args...)                              \
    bth_warn_log(LOG_BTH_SERVICE_IPC, log_num, fmt, count, ##args)
#define ipc_err_log(log_num, fmt, count, args...)                               \
    bth_err_log(LOG_BTH_SERVICE_IPC, log_num, fmt, count, ##args)
#define ftm_dbg_log(log_num, fmt, count, args...)                               \
    bth_dbg_log(LOG_BTH_SERVICE_FACTORY, log_num, fmt, count, ##args)
#define ftm_info_log(log_num, fmt, count, args...)                              \
    bth_info_log(LOG_BTH_SERVICE_FACTORY, log_num, fmt, count, ##args)
#define ftm_warn_log(log_num, fmt, count, args...)                              \
    bth_warn_log(LOG_BTH_SERVICE_FACTORY, log_num, fmt, count, ##args)
#define ftm_err_log(log_num, fmt, count, args...)                               \
    bth_err_log(LOG_BTH_SERVICE_FACTORY, log_num, fmt, count, ##args)
#define twsm_dbg_log(log_num, fmt, count, args...)                              \
    bth_dbg_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define twsm_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define twsm_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define twsm_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define audio_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define audio_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define audio_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)

#define gattm_dbg_log(log_num, fmt, count, args...)                              \
    bth_dbg_log(LOG_BTH_SERVICE_GATT_MGR, log_num, fmt, count, ##args)
#define gattm_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_GATT_MGR, log_num, fmt, count, ##args)
#define gattm_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_GATT_MGR, log_num, fmt, count, ##args)
#define gattm_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_GATT_MGR, log_num, fmt, count, ##args)

// BTH STACK ADAPTER LOG FUNCTION BEGIN
#define sadp_dbg_log(log_num, fmt, count, args...)                              \
    bth_dbg_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define sadp_info_log(log_num, fmt, count, args...)                             \
    bth_info_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define sadp_warn_log(log_num, fmt, count, args...)                             \
    bth_warn_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)
#define sadp_err_log(log_num, fmt, count, args...)                              \
    bth_err_log(LOG_BTH_SERVICE_TWS_MGR, log_num, fmt, count, ##args)

// BTH STACK LOG FUNCTION BEGIN
#define h2c_dbg_log(log_num, fmt, count, args...)                               \
    bth_dbg_log(LOG_BTH_H2C, log_num, fmt, count, ##args)
#define h2c_info_log(log_num, fmt, count, args...)                              \
    bth_info_log(LOG_BTH_H2C, log_num, fmt, count, ##args)
#define h2c_warn_log(log_num, fmt, count, args...)                              \
    bth_warn_log(LOG_BTH_H2C, log_num, fmt, count, ##args)
#define h2c_err_log(log_num, fmt, count, args...)                               \
    bth_err_log(LOG_BTH_H2C, log_num, fmt, count, ##args)
#define c2h_dbg_log(log_num, fmt, count, args...)                               \
    bth_dbg_log(LOG_BTH_C2H, log_num, fmt, count, ##args)
#define c2h_info_log(log_num, fmt, count, args...)                              \
    bth_info_log(LOG_BTH_C2H, log_num, fmt, count, ##args)
#define c2h_warn_log(log_num, fmt, count, args...)                              \
    bth_warn_log(LOG_BTH_C2H, log_num, fmt, count, ##args)
#define c2h_err_log(log_num, fmt, count, args...)                               \
    bth_err_log(LOG_BTH_C2H, log_num, fmt, count, ##args)
#define stack_gap_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_GAP, log_num, fmt, count, ##args)
#define stack_gap_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_GAP, log_num, fmt, count, ##args)
#define stack_gap_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_GAP, log_num, fmt, count, ##args)
#define stack_gap_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_GAP, log_num, fmt, count, ##args)
#define stack_sdp_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_SDP, log_num, fmt, count, ##args)
#define stack_sdp_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_SDP, log_num, fmt, count, ##args)
#define stack_sdp_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_SDP, log_num, fmt, count, ##args)
#define stack_sdp_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_SDP, log_num, fmt, count, ##args)
#define stack_smp_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_SMP, log_num, fmt, count, ##args)
#define stack_smp_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_SMP, log_num, fmt, count, ##args)
#define stack_smp_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_SMP, log_num, fmt, count, ##args)
#define stack_smp_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_SMP, log_num, fmt, count, ##args)
#define stack_a2dp_dbg_log(log_num, fmt, count, args...)                        \
    bth_dbg_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_a2dp_info_log(log_num, fmt, count, args...)                       \
    bth_info_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_a2dp_warn_log(log_num, fmt, count, args...)                       \
    bth_warn_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_a2dp_err_log(log_num, fmt, count, args...)                        \
    bth_err_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_hfp_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_HFP, log_num, fmt, count, ##args)
#define stack_hfp_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_HFP, log_num, fmt, count, ##args)
#define stack_hfp_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_HFP, log_num, fmt, count, ##args)
#define stack_hfp_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_HFP, log_num, fmt, count, ##args)
#define stack_avrcp_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_AVRCP, log_num, fmt, count, ##args)
#define stack_avrcp_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_AVRCP, log_num, fmt, count, ##args)
#define stack_avrcp_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_AVRCP, log_num, fmt, count, ##args)
#define stack_avrcp_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_AVRCP, log_num, fmt, count, ##args)
#define stack_spp_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_SPP, log_num, fmt, count, ##args)
#define stack_spp_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_SPP, log_num, fmt, count, ##args)
#define stack_spp_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_SPP, log_num, fmt, count, ##args)
#define stack_spp_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_SPP, log_num, fmt, count, ##args)
#define stack_gatt_dbg_log(log_num, fmt, count, args...)                        \
    bth_dbg_log(LOG_BTH_STACK_GATT, log_num, fmt, count, ##args)
#define stack_gatt_info_log(log_num, fmt, count, args...)                       \
    bth_info_log(LOG_BTH_STACK_GATT, log_num, fmt, count, ##args)
#define stack_gatt_warn_log(log_num, fmt, count, args...)                       \
    bth_warn_log(LOG_BTH_STACK_GATT, log_num, fmt, count, ##args)
#define stack_gatt_err_log(log_num, fmt, count, args...)                        \
    bth_err_log(LOG_BTH_STACK_GATT, log_num, fmt, count, ##args)
#define stack_avdtp_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_AVDTP, log_num, fmt, count, ##args)
#define stack_avdtp_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_AVDTP, log_num, fmt, count, ##args)
#define stack_avdtp_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_AVDTP, log_num, fmt, count, ##args)
#define stack_avdtp_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_AVDTP, log_num, fmt, count, ##args)
#define stack_avctp_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_AVCTP, log_num, fmt, count, ##args)
#define stack_avctp_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_AVCTP, log_num, fmt, count, ##args)
#define stack_avctp_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_AVCTP, log_num, fmt, count, ##args)
#define stack_avctp_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_AVCTP, log_num, fmt, count, ##args)
#define stack_rfcomm_dbg_log(log_num, fmt, count, args...)                      \
    bth_dbg_log(LOG_BTH_STACK_RFCOMM, log_num, fmt, count, ##args)
#define stack_rfcomm_info_log(log_num, fmt, count, args...)                     \
    bth_info_log(LOG_BTH_STACK_RFCOMM, log_num, fmt, count, ##args)
#define stack_rfcomm_warn_log(log_num, fmt, count, args...)                     \
    bth_warn_log(LOG_BTH_STACK_RFCOMM, log_num, fmt, count, ##args)
#define stack_rfcomm_err_log(log_num, fmt, count, args...)                      \
    bth_err_log(LOG_BTH_STACK_RFCOMM, log_num, fmt, count, ##args)
#define stack_l2cap_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_L2CAP, log_num, fmt, count, ##args)
#define stack_l2cap_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_L2CAP, log_num, fmt, count, ##args)
#define stack_l2cap_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_L2CAP, log_num, fmt, count, ##args)
#define stack_l2cap_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_L2CAP, log_num, fmt, count, ##args)
#define stack_hci_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_HCI, log_num, fmt, count, ##args)
#define stack_hci_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_HCI, log_num, fmt, count, ##args)
#define stack_hci_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_HCI, log_num, fmt, count, ##args)
#define stack_hci_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_HCI, log_num, fmt, count, ##args)
#define stack_ascp_dbg_log(log_num, fmt, count, args...)                        \
    bth_dbg_log(LOG_BTH_STACK_ASCP, log_num, fmt, count, ##args)
#define stack_ascp_info_log(log_num, fmt, count, args...)                       \
    bth_info_log(LOG_BTH_STACK_ASCP, log_num, fmt, count, ##args)
#define stack_ascp_warn_log(log_num, fmt, count, args...)                       \
    bth_warn_log(LOG_BTH_STACK_ASCP, log_num, fmt, count, ##args)
#define stack_ascp_err_log(log_num, fmt, count, args...)                        \
    bth_err_log(LOG_BTH_STACK_ASCP, log_num, fmt, count, ##args)
#define stack_asmp_dbg_log(log_num, fmt, count, args...)                        \
    bth_dbg_log(LOG_BTH_STACK_ASMP, log_num, fmt, count, ##args)
#define stack_asmp_info_log(log_num, fmt, count, args...)                       \
    bth_info_log(LOG_BTH_STACK_ASMP, log_num, fmt, count, ##args)
#define stack_asmp_warn_log(log_num, fmt, count, args...)                       \
    bth_warn_log(LOG_BTH_STACK_ASMP, log_num, fmt, count, ##args)
#define stack_asmp_err_log(log_num, fmt, count, args...)                        \
    bth_err_log(LOG_BTH_STACK_ASMP, log_num, fmt, count, ##args)
#define stack_jitws_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_JITWS, log_num, fmt, count, ##args)
#define stack_jitws_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_JITWS, log_num, fmt, count, ##args)
#define stack_jitws_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_JITWS, log_num, fmt, count, ##args)
#define stack_jitws_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_JITWS, log_num, fmt, count, ##args)
#define stack_hitws_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_HITWS, log_num, fmt, count, ##args)
#define stack_hitws_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_HITWS, log_num, fmt, count, ##args)
#define stack_hitws_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_HITWS, log_num, fmt, count, ##args)
#define stack_hitws_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_HITWS, log_num, fmt, count, ##args)
#define stack_dadp_dbg_log(log_num, fmt, count, args...)                        \
    bth_dbg_log(LOG_BTH_STACK_DADP, log_num, fmt, count, ##args)
#define stack_dadp_info_log(log_num, fmt, count, args...)                       \
    bth_info_log(LOG_BTH_STACK_DADP, log_num, fmt, count, ##args)
#define stack_dadp_warn_log(log_num, fmt, count, args...)                       \
    bth_warn_log(LOG_BTH_STACK_DADP, log_num, fmt, count, ##args)
#define stack_dadp_err_log(log_num, fmt, count, args...)                        \
    bth_err_log(LOG_BTH_STACK_DADP, log_num, fmt, count, ##args)
#define stack_fsm_dbg_log(log_num, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_FSM, log_num, fmt, count, ##args)
#define stack_fsm_info_log(log_num, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_FSM, log_num, fmt, count, ##args)
#define stack_fsm_warn_log(log_num, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_FSM, log_num, fmt, count, ##args)
#define stack_fsm_err_log(log_num, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_FSM, log_num, fmt, count, ##args)
#define stack_common_dbg_log(log_num, fmt, count, args...)                      \
    bth_dbg_log(LOG_BTH_STACK_COMMON, log_num, fmt, count, ##args)
#define stack_common_info_log(log_num, fmt, count, args...)                     \
    bth_info_log(LOG_BTH_STACK_COMMON, log_num, fmt, count, ##args)
#define stack_common_warn_log(log_num, fmt, count, args...)                     \
    bth_warn_log(LOG_BTH_STACK_COMMON, log_num, fmt, count, ##args)
#define stack_common_err_log(log_num, fmt, count, args...)                      \
    bth_err_log(LOG_BTH_STACK_COMMON, log_num, fmt, count, ##args)
#define stack_gle_tm_dbg_log(log_num, fmt, count, args...)                      \
    bth_dbg_log(LOG_BTH_STACK_GLE_TM, log_num, fmt, count, ##args)
#define stack_gle_tm_info_log(log_num, fmt, count, args...)                     \
    bth_info_log(LOG_BTH_STACK_GLE_TM, log_num, fmt, count, ##args)
#define stack_gle_tm_warn_log(log_num, fmt, count, args...)                     \
    bth_warn_log(LOG_BTH_STACK_GLE_TM, log_num, fmt, count, ##args)
#define stack_gle_tm_err_log(log_num, fmt, count, args...)                      \
    bth_err_log(LOG_BTH_STACK_GLE_TM, log_num, fmt, count, ##args)

#define stack_gle_dbg_log(logNum, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_STACK_GLE, logNum, fmt, count, ##args)
#define stack_gle_info_log(logNum, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_STACK_GLE, logNum, fmt, count, ##args)
#define stack_gle_warn_log(logNum, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_STACK_GLE, logNum, fmt, count, ##args)
#define stack_gle_err_log(logNum, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_STACK_GLE, logNum, fmt, count, ##args)

#define gle_ahi_dbg_log(logNum, fmt, count, args...)                         \
    bth_dbg_log(LOG_BTH_GLE_AHIB, logNum, fmt, count, ##args)
#define gle_ahi_info_log(logNum, fmt, count, args...)                        \
    bth_info_log(LOG_BTH_GLE_AHIB, logNum, fmt, count, ##args)
#define gle_ahi_warn_log(logNum, fmt, count, args...)                        \
    bth_warn_log(LOG_BTH_GLE_AHIB, logNum, fmt, count, ##args)
#define gle_ahi_err_log(logNum, fmt, count, args...)                         \
    bth_err_log(LOG_BTH_GLE_AHIB, logNum, fmt, count, ##args)

/* BNEP */
#define stack_bnep_dbg_log(log_num, fmt, count, args...)                       \
    bth_dbg_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_bnep_info_log(log_num, fmt, count, args...)                      \
    bth_info_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_bnep_warn_log(log_num, fmt, count, args...)                      \
    bth_warn_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
#define stack_bnep_err_log(log_num, fmt, count, args...)                       \
    bth_err_log(LOG_BTH_STACK_A2DP, log_num, fmt, count, ##args)
/* BNEP */

// BTH CHR FUNCTION
#if (defined(CONFIG_TWS))
#define bth_service_ue_chr_evt(info1, info2, info3)                            \
    chr_record_ue(EVENT_BT_SEVICE_MANAGER, info1, info2, info3)
#define bth_service_chr_evt(info1, info2, info3)                               \
    massdata_record_system_event(EVENT_BT_SEVICE_MANAGER, info1, info2, info3)
#define bth_service_chr_err(info2, info3)                                      \
    massdata_record_system_error(ERROR_BLUETOOTH, ERROR_BT_SERVICE, info2, info3)
#define bth_stack_chr_err(info2, info3)                                        \
    massdata_record_system_error(ERROR_BLUETOOTH, ERROR_BT_STACK, info2, info3)
#else
#define bth_service_ue_chr_evt(info1, info2, info3)
#define bth_service_chr_evt(info1, info2, info3)
#define bth_service_chr_err(info2, info3)
#define bth_stack_chr_err(info2, info3)
#endif

#endif
