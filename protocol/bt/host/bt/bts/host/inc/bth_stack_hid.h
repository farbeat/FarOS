/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: bth mpc stack struct for gap
 * Author: @CompanyNameTag
 * Create: 2022-03-01
 */
#ifndef _BTH_STACK_HID_H_
#define _BTH_STACK_HID_H_

#include "custdefs.h"
#include "stdint.h"

/* General Interface for Stack GAP */
#pragma pack (1)

typedef struct {
    uint8_t button_mask;
    uint8_t x; /* mouse的x坐标 */
    uint8_t y; /* mouse的y坐标 */
    uint8_t wheel;
} ble_hid_mouse_event_st;
typedef struct {
    uint16_t button_mask;
    uint16_t x : 12; /* mouse的x坐标 */
    uint16_t y : 12; /* mouse的y坐标 */
    uint8_t wheel;
    uint8_t ac_pan;
} ble_hid_high_mouse_event_st;

typedef struct {
    uint8_t button_mask;
    uint16_t x : 12; /* mouse的x坐标 */
    uint16_t y : 12; /* mouse的y坐标 */
    uint8_t wheel;
} ble_hid_high_4k_mouse_event_st;

/* 后面可能会挪头文件 先放这里 */
#define BT_AT_MAX_KEY_NUM 6
#define BT_AT_MOUSE_LEN 4 /* sizeof(ble_hid_mouse_event_st) */
#define BT_AT_HIGH_MOUSE_LEN 5 /* sizeof(ble_hid_high_mouse_event_st) */
#define BT_AT_MAX_KEY_LEN 4 /* 0xXX(16进制) or 255(10进制) */
#define BT_AT_MIN_KEY_LEN 1 /* 0(10进制) */
#define BT_AT_MAX_MOUSE_XY_NUM 0xFFF

#pragma pack ()
/* General Interface for Stack GAP */
#endif /* _BTH_STACK_HID_H_ */
