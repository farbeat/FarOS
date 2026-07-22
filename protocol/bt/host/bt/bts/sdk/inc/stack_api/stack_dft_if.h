/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 */
#ifndef STATCK_DFT_IF_H
#define STATCK_DFT_IF_H
#include "ipc.h"
#include "mpc_dispatch.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************** Macro Definition ******************/

/*************** Structure Definition ******************/
typedef struct {
    uint8_t on_off;
} bt_dft_dut_params;

typedef struct {
    uint8_t length;
    uint8_t subcode;
    uint8_t freq;
    uint8_t mode;
    uint8_t switch_trx;
} bt_dft_sig_params;

typedef struct {
    uint16_t ocf;
    uint8_t param_len;
    uint8_t param[];
} bt_dft_set_vendor_params;

typedef struct {
    uint8_t on_off;
    uint8_t tx_power;
    uint8_t tx_channel;
    uint8_t pkt_type;
    uint8_t pkt_len[2];
    uint8_t br_edr;
    uint8_t payload;
} bt_dft_bt_tx_params;

typedef struct {
    uint8_t on_off;
    uint8_t rx_channel;
    uint8_t pkt_type;
    uint8_t pkt_len[2];
    uint8_t br_edr;
    uint8_t payload;
} bt_dft_bt_rx_params;

typedef struct {
    uint8_t on_off;
    uint8_t tx_channel;
    uint8_t pkt_type;
    uint8_t pkt_len;
    uint8_t tx_phy;
} bt_dft_ble_tx_params;

typedef struct {
    uint8_t on_off;
    uint8_t rx_channel;
    uint8_t rx_phy;
} bt_dft_ble_rx_params;

uint8_t dft_set_dut_mode_req(bt_dft_dut_params para);
uint8_t dft_set_sig_cmd_req(bt_dft_sig_params para);
uint8_t dft_set_vendor_cmd_req(bt_dft_set_vendor_params *para);
uint8_t dft_set_bredr_tx_mode_req(bt_dft_bt_tx_params para);
uint8_t dft_set_bredr_rx_mode_req(bt_dft_bt_rx_params para);
uint8_t dft_set_ble_tx_mode_req(bt_dft_ble_tx_params para);
uint8_t dft_set_ble_rx_mode_req(bt_dft_ble_rx_params para);
uint8_t dft_reset_rx_count_req(void);
uint8_t dft_read_rx_count_req(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_DFT_IF_H */
