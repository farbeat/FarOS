/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 *
 * Description: dut data trans module.
 *
 * Create: 2021-05-31
 */
#ifndef __MPC_DISPATCH_H_
#define __MPC_DISPATCH_H_

#include "ipc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MPC_OFFS_TRANS_ID     (0)
#define MPC_OFFS_SOF_ID       (1)
#define MPC_OFFS_LENGTH       (2)
#define MPC_OFFS_CONTROL      (4)
#define MPC_OFFS_OPCODE       (5)
#define MPC_OFFS_OPCODE_SID   (5)
#define MPC_OFFS_OPCODE_CID   (6)
#define MPC_OFFS_PAYLOAD      (7)

#define MPC_TRANS_LEN         (1)
#define MPC_SOF_LEN           (1)
#define MPC_LENGTH_LEN        (2)
#define MPC_CONTROL_LEN       (1)
#define MPC_OPCODE_LEN        (2)
#define MPC_CRC_LEN           (2)
#define MPC_FRAME_HDR_LEN (MPC_TRANS_LEN + MPC_SOF_LEN + MPC_LENGTH_LEN)
#define MPC_HDR_LEN (MPC_FRAME_HDR_LEN + MPC_CONTROL_LEN + MPC_OPCODE_LEN + MPC_CRC_LEN)
#define MPC_SAIA_MIN_PKT_LEN  (8)

#define MPC_SOF_NORMAL        (0x5A)
#define MPC_SOF_CONTRL_CMD    (0xAA)
#define MPC_SOF_FILE_TRANS    (0xCC)

#define MPC_NO_FSN            (0x00)
#define MPC_FSN_MASK          (0x03)


typedef struct {
    uint8_t trans_id;
    uint8_t sof;
    uint8_t service_id;
    uint8_t command_id;
} mpc_header;

typedef struct {
    mpc_header header;
    uint16_t payload_length;
    uint8_t src_transid;
    uint8_t resv;
    uint8_t payload[0];
} mpc_message;

typedef void (*mpc_function)(const mpc_message *param);

typedef struct {
    uint8_t op;
    mpc_function func;
} table_function;
/*
 * bt error code enum
 */
typedef enum {
    MPC_BT_SUCC = 0x00,
    MPC_BT_FAIL = 0x01,
} mpc_bt_err_code;

void mc_bt_factory_test_svr(const mpc_message *param);
void bt_device_mgr_svr_proc(const mpc_message *param);

#define NO_NEED_FOR_DESKTOP_116X YES
#if (NO_NEED_FOR_DESKTOP_116X == NO)
void bt_phone_alert_mgr_svr_proc(const mpc_message *param);
#endif
void ble_connect_svr_proc(const mpc_message *param);
void gle_manager_svr_proc(const mpc_message *param);
#if (NO_NEED_FOR_DESKTOP_116X == NO)
void ble_ancs_svr_proc(const mpc_message *param);
void bt_pay_svr_proc(const mpc_message *param);
void bt_synergy_connect_svr_proc(const mpc_message *param);
#endif
#if ((BTH_WEAR_ENABLE_AUDIO_SINK == YES) || (BTH_WEAR_ENABLE_AUDIO_GATEWAY == YES))
void bt_audio_control_svr_proc(const mpc_message *param);
void bt_audio_svc_svr_proc(const mpc_message *param);
#endif
void bt_manager_svr_proc(const mpc_message *param);
void bt_desktop_svr_proc(const mpc_message *param);

#if defined(CHIP_BRANDY)
void bt_mpc_gap_confirm_proc(const mpc_message *param);
void bt_mpc_gap_event_proc(const mpc_message *param);
void bt_spp_acore_service_proc(const mpc_message *param);
void bt_mpc_a2dp_service_proc(const mpc_message *param);
void bt_pbap_acore_service_proc(const mpc_message *param);
void bt_mpc_avrcp_service_proc(const mpc_message *param);
void bt_hfp_acore_service_proc(const mpc_message *param);
void bt_mpc_pan_service_proc(const mpc_message *param);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* end of __MPC_DISPATCH_H_ */
