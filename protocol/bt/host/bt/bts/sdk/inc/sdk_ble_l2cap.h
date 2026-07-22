/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: host sdk stack api for l2cap
 * Author: @CompanyNameTag
 * Create: 2021-09-27
 */
#ifndef _SDK_BLE_L2CAP_H_
#define _SDK_BLE_L2CAP_H_


#include "sdk_ble.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define SDK_BLE_FIX_LEPSM_MAX 0x7F
#define SDK_BLE_DYNAMIC_LEPSM_MIN 0x80
#define SDK_BLE_DYNAMIC_LEPSM_MAX 0xFF

typedef struct {
    uint16_t data_len;
    uint8_t data[0];
} sdk_ble_data_t;

typedef enum {
    SDK_BLE_L2CAP_EVENT_REG_PSM = 0,
    SDK_BLE_L2CAP_EVENT_CREDIT_CH_REQ,
    SDK_BLE_L2CAP_EVENT_CREDIT_CH_RSP,
    SDK_BLE_L2CAP_EVENT_CREDIT_CH_RX,
    SDK_BLE_L2CAP_EVENT_CREDIT_CH_TX,
    SDK_BLE_L2CAP_EVENT_CREDIT_FLOW_CTRL,
    SDK_BLE_L2CAP_EVENT_DISCONNECT,
    SDK_BLE_L2CAP_EVENT_MAX,
} sdk_ble_l2cap_cbk_event;

void sdk_ble_l2cap_callback_regist(sdk_ble_l2cap_cbk_event event, sdk_ble_cbk cbk);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_BLE_L2CAP_H_ */
