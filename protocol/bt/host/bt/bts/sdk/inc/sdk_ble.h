/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: host sdk stack api for l2cap
 * Author: @CompanyNameTag
 * Create: 2021-09-27
 */
#ifndef _SDK_BLE_H_
#define _SDK_BLE_H_

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef enum {
    SDK_BLE_OK,
    SDK_BLE_FAIL,
    SDK_BLE_PARAM_WRONG,
} sdk_ble_errcode;

typedef void (*sdk_ble_cbk)(uint16_t event, void *arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_BLE_H_ */
