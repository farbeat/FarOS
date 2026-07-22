/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides keyslot port template header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef KLAD_PORTING_H
#define KLAD_PORTING_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define KEY_SIZE                        16
#define KLAD_CHANNEL_MAX_SUPPORT        0x1

#define KLAD_KEY_LEN                    32
#define KLAD_ADDR_ALIGN                 16

extern uint8_t g_klad_key_addr[KLAD_KEY_LEN];

#define KLAD_KEY_ADDR                   g_klad_key_addr

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif