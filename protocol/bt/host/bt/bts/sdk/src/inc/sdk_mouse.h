/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk mouse header
 * Author: @CompanyNameTag
 * Create: 2022-06-11
 */
#ifndef _SDK_MOUSE_H_
#define _SDK_MOUSE_H_

#ifdef CORE
#undef CORE
#endif


#include "chip_core_definition.h"
#include "sdk_common.h"
#include "securec.h"
#include "sdk_log.h"
#include "bth_stack_hid.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 鼠标数据发送方式
typedef enum {
    LOW_SPEED_TX_MODE, // 0：default,通过MCU->HOST->Controller方式发送键值
    HIGH_SPEED_TX_MODE // 1：low delay,通过MCU->EM->Controller方式发送键值
} bth_mouse_tx_mode;

#ifdef BTH_MOUSE_MANAGER_SUPPORT
#define BLE_MCU_TOG_UP_ADDR      0x52000280 // MCU TOG UP ADDR
#define BLE_MCU_TOG_DOWN_ADDR    0x52000284 // MCU TOG DOWN ADDR
#define BLE_MCU_INT_CLEAR_ADDR   0x52000288 // MCU INT CLEAR ADDR
#define BLE_MCU_INT_ENABLE       0x8
#define BLE_MCU_TOG_POS          0x0
#define BLE_MCU_TOG_DEFAULT      0x0
#define BLE_MOUSE_EM_TX_KEY_BASE_ADDR 0x5941a920 // 键值发送时使用的EM地址，MCU直接写
#define BLE_MOUSE_EM_RX_KEY_BASE_ADDR 0x5941a960 // 键值接收时使用的EM地址，MCU直接读
#define BLE_MOUSE_EM_TX_KEY_SIZE    12
#define BLE_MOUSE_EM_RX_KEY_SIZE    12
#define BLE_MOUSE_ACL_HANDLE_OFFSET 4
#define BLE_MOUSE_KEY_OFFSET        7
#define BLE_MOUSE_KEY_LEN_OFFSET    6
#define BLE_MOUSE_KEY_SIZE          5

#define ble_mouse_inversion(NUM) \
    if ((NUM) == 0) { \
        (NUM) = 1; \
    } else { \
        (NUM) = 0; \
    } \

int bt_at_cmd_4k_mouse_input(ble_hid_high_mouse_event_st *mouse_val);
uint8_t bt_get_current_mouse_mode(void);

#endif /* BTH_MOUSE_MANAGER_SUPPORT */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_MOUSE_H_ */
