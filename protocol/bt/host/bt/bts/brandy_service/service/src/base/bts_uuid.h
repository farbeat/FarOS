/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: BT UUID module.
 */
#ifndef __BTS_UUID_H__
#define __BTS_UUID_H__

#include <stdbool.h>
#include <stdint.h>
#include "bts_def.h"

#define UUID_LEN_2 2
#define UUID_LEN_4 4
#define UUID_LEN_16 16
#define GATT_UUID_OFFSET 12

#define BASE_UUID { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB }

bool bts_uuid_is_valid(const bt_uuid_t *uuid);
bool bts_uuid_is_equal(const bt_uuid_t *uuid1, const bt_uuid_t *uuid2);
uint8_t *bts_init_128bit_uuid(const bt_uuid_t *uuid);
void bts_data_to_uuid_len2(uint16_t uuid_data, bt_uuid_t *out_uuid);
void bts_print_uuid(bt_uuid_t *uuid);
#endif /* __BTS_UUID_H__ */
