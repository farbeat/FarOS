/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 */
#ifndef __MPC_SEND_H_
#define __MPC_SEND_H_

#include "ipc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

ipc_status_t bt_acore_mpc_send_2_bt(ipc_payload *data, uint32_t data_len);
ipc_status_t bt_acore_mpc_send_2_bt_timeout(ipc_payload *data, uint32_t data_len, uint32_t timeout_msec);
ipc_status_t bt_acore_mpc_send_timeout(ipc_payload *data, uint32_t data_len, uint32_t timeout_msec);
int8_t bt_acore_mpc_send_2_bt_para(uint8_t sid, uint8_t cid, uint8_t *para, uint16_t para_len);
int8_t bt_acore_mpc_send_2_bt_bypass(uint8_t *para, uint16_t para_len);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* __SDK_SEND_H_ */
