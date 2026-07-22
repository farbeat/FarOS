/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT dft Service module.
 */
#ifndef DFT_SERVICE_H
#define DFT_SERVICE_H

#include "btsrv_task.h"
#include "diag.h"
#include "soc_diag_cmd_id.h"

#define BT_DFT_SHIFT_8 8
#define BT_DFT_SIG_PARA_LEN 4
#define BT_DFT_SIG_SUBCODE 0x33

typedef enum {
    POS_0,
    POS_1,
    POS_2,
    POS_3,
    POS_4,
    POS_5,
    POS_6,
    POS_7,
    POS_8,
    POS_9,
    POS_10,
    POS_11,
    POS_12,
    POS_13,
    POS_14,
    POS_15
} bt_dft_pos_code;

ext_errno bt_dft_reg_callbacks(void);
void bt_wstp_dft_set_dut_mode(uint8_t mode);
void bt_wstp_dft_set_sig_cmd(uint8_t freq, uint8_t mode, uint8_t switch_trx);

void bt_dft_set_dut_mode(const uint8_t *param);
void bt_dft_set_bredr_tx_mode(const uint8_t *param);
void bt_dft_set_bredr_rx_mode(const uint8_t *param);
void bt_dft_set_ble_tx_mode(const uint8_t *param);
void bt_dft_set_ble_rx_mode(const uint8_t *param);
void bt_dft_reset_rx_count(const uint8_t *param);
void bt_dft_read_rx_count(const uint8_t *param);

#endif /* DFT_SERVICE_H */
