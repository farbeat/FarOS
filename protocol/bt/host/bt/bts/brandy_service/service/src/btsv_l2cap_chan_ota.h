/*
 * Copyright (c) @CompanyNameMagicTag 2023-2024. All rights reserved.
 */
#ifndef BTSRV_L2CAP_CHAN_OTA_H
#define BTSRV_L2CAP_CHAN_OTA_H
#include <stdint.h>
#include "errcode.h"
#include "bts_def.h"
#include "bth_stack_l2cap.h"

typedef enum {
    BTH_OTA_CHAN_DISCONNECTED = 0,
    BTH_OTA_CHAN_CONNECTED
} bth_ota_chan_status_t;

#define BTH_OTA_CHAN_PSM          0xFF
#define BTH_OTA_CHAN_TYPE         0x03
#define BTH_OTA_CHAN_MPS          1024
#define BTH_OTA_CHAN_MTU          1024
#define BTH_OTA_CHAN_CREDITS      0xFFFF
#define BTH_OTA_CHAN_CREDITS_CNTL 1000
typedef void (bth_ota_chan_data_report) (const uint8_t *data_ptr,  const uint16_t data_len);
typedef void (bth_ota_chan_status_report) (const bth_ota_chan_status_t chan_status);

/**
 * Initializes bth ota channel.
 */
errcode_t bth_ota_init(void);

/**
 * reg bth ota channel cbks.
 * @param notify_cbk the notify callback, when bth chan receive data, this function should be called.
 * @param conn_report the connection event report callback, when bth chan established, this function should be called.
 */
void bth_ota_reg_cbk(bth_ota_chan_data_report *data_report, bth_ota_chan_status_report *status_report);

/**
 * sent ota data to ble channel.
 * @param data_ptr data buffer to wirte.
 * @param data_len length of the data buffer
 */
errcode_t bth_ota_data_send(uint8_t *data_ptr, uint16_t data_len);

#endif