/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: sdk private header
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef _SDK_PRIVATE_H_
#define _SDK_PRIVATE_H_

#ifdef CORE
#undef CORE
#endif

#include "chip_core_definition.h"
#define CORE APPS
#include "cmsis_os2.h"
#include "product_bth.h"
#include "ipc.h"
#include "ipc_protocol.h"
#include "log_common.h"
#include "bt_acore_log.h"
#include "sdk_common.h"
#include "securec.h"
#include "sdk_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

/**< attach 2 uint8 to uint16  */
#define tou16(h_u8, l_u8) (uint16_t)(((uint8_t)(h_u8) << 8) | (uint8_t)(l_u8))

/* attach two uint16 num to a uint32 num */
#define tou32(h_u16, l_u16) (uint32_t)(((uint16_t)(h_u16) << 16) | (uint16_t)(l_u16))

#ifndef msb16
/* The upper 8 bits of a 16 bit value */
#define msb16(a) (((a) & 0xFF00) >> 8)
#endif

#ifndef lsb16
/* The lower 8 bits (of a 16 bit value) */
#define lsb16(a) ((a) & 0x00FF)
#endif

void *bt_os_new(uint32_t size);
void bt_os_free(void* buf);

uint8_t bt_sdk_at_str_to_num32(uint8_t *str, uint32_t *num);
uint8_t bt_sdk_at_str_to_num16(uint8_t *str, uint16_t *num);
uint8_t bt_sdk_at_str_to_num8(uint8_t *str, uint8_t *num);
uint8_t bt_sdk_at_str_to_addr(uint8_t *str, uint8_t *addr, uint8_t addr_len);
uint8_t bt_sdk_at_str_to_data(uint8_t *str, uint16_t data_len, uint8_t *data);
uint8_t bt_sdk_at_num_to_str(uint32_t num, char *data, uint8_t max_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* _SDK_PRIVATE_H_ */
