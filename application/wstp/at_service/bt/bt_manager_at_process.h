/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd register func \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-20， Create file. \n
 */
#ifndef BT_MANAGER_AT_SERVICE_H
#define BT_MANAGER_AT_SERVICE_H

#include <stdint.h>
#include <stdio.h>
#include "errcode.h"
#include "diag_service.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define BT_APK_MSG_MIN_LEN 9
#define BT_APK_MSG_TLV_LEN 11
#define BT_MANAGER_SHIFT_8 8

#define BT_APK_RSP_GENERAL_LEN 13
#define BT_APK_RSP_PAYLOAD_LEN 4

#define BT_HFP_MAX_NUM_LEN 32
#define BT_HFP_MIN_NUM_LEN 1

/* apk指令传输使用的TLV */
typedef enum {
    BT_APK_TLV_0 = 0x00,
    BT_APK_TLV_80 = 0x80,
    BT_APK_TLV_81,
    BT_APK_TLV_82,
    BT_APK_TLV_83,
    BT_APK_TLV_84,
    BT_APK_TLV_85,
    BT_APK_TLV_86,
    BT_APK_TLV_87,
    BT_APK_TLV_88,
    BT_APK_TLV_89,
} bt_apk_tlv_t;

void bt_manager_at_init(void);
void bt_manager_at_process(uint8_t *data, uint32_t data_len);
void bt_manager_apk_process(uint8_t *data, uint32_t data_len);
uint32_t bt_diag_cmd_process(diag_ser_data_t *data);
void bt_wstp_dft_set_dut_mode(uint8_t mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif