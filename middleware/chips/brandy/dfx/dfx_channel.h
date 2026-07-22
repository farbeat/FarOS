/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: dfx uart channel
 * This file should be changed only infrequently and with great care.
 */
#ifndef DFX_CHANNEL_H
#define DFX_CHANNEL_H
#include <stdint.h>
#include "errcode.h"
#include "diag.h"
#include "bts_gatt_server.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void diag_uart_rx_proc(uint8_t *buffer, uint16_t length);
errcode_t diag_register_channel(void);
errcode_t diag_cmd_uart_config(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t diag_cmd_set_offline_log(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
void diag_gatt_write_req_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_write_cb_t *write_cb_para,
    errcode_t status);
void diag_gatt_set_handle(uint8_t server_id, uint16_t conn_id, uint16_t handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif