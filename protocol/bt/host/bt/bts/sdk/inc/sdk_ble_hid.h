/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: host sdk stack api for hid
 * Author: @CompanyNameTag
 * Create: 2022-03-14
 */
#ifndef _SDK_BLE_HID_H_
#define _SDK_BLE_HID_H_


#include "sdk_ble.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    SDK_BLE_HID_EVENT_MAX,
} sdk_ble_hid_cbk_event;

/* 发送键值 */
uint8_t ble_conn_hid_keyboard_input(uint8_t *key_list, uint8_t key_num);
/* 发送单个键值，自动发送抬起所有键指令 */
uint8_t ble_conn_hid_keyboard_input_one_word(uint8_t *key_list, uint8_t key_num);

/* 发送键值，str方式输入 */
uint8_t ble_conn_hid_keyboard_input_str(int key_num, char *key_list[]);
/* 发送单个键值，自动发送抬起所有键指令，str方式输入 */
uint8_t ble_conn_hid_keyboard_input_one_word_str(int key_num, char *key_list[]);

int bt_at_cmd_high_mouse_input(int argc, char *argv[]);
int bt_at_cmd_high_mouse_input_one(int argc, char *argv[]);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_BLE_HID_H_ */
