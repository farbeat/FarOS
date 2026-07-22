/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 */
#ifndef BTSRV_L2CAP_CHAN_H
#define BTSRV_L2CAP_CHAN_H
#include <stdint.h>
#include "errcode.h"
#include "bts_def.h"
#include "bth_stack_l2cap.h"

typedef enum {
    BTH_L2CAP_CHAN_OTA = 1,
    BTH_L2CAP_CHAN_MAX
} bth_chan_id_t;

typedef struct bth_l2cap_chan {
    bth_chan_id_t id;
    uint16_t psm;
} bth_l2cap_chan_t;

/**
 * Initializes a bth data transmission channel.
 * @param bth_chan_id id of the channel
 * @param credits number of l2cap channel credits
 */
errcode_t bth_chan_credit_cntl(bth_chan_id_t bth_chan_id, uint16_t credits);

/**
 * Initializes a bth data transmission channel.
 * @param bth_chan_id id of the channel
 * @param param bth l2cap psm node.
 */
errcode_t bth_chan_init(const bth_chan_id_t bth_chan_id, bth_l2cap_psm_node *param);

/**
 * Initializes a bth data transmission channel.
 * @param bth_chan_id id of the channel
 */
errcode_t bth_chan_deinit(const bth_chan_id_t bth_chan_id);

/**
 * write data to bth data channel.
 * @param bth_chan_id id of the channel
 * @param data_ptr data buffer to wirte.
 * @param data_len length of the data buffer
 */
errcode_t bth_chan_data_write(const bth_chan_id_t bth_chan_id, const uint8_t *data_ptr, const uint16_t data_len);

#endif