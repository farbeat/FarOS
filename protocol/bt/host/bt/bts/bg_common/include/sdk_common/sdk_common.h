/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: sdk common
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef _SDK_COMMON_H_
#define _SDK_COMMON_H_

#include "custdefs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef ACORE_DEBUG
// Close Acore Debug Log switch
#define BT_ACORE_DEBUG 1
#endif

#define arr2value16(x) (((*(x)) << 8) | (*((x) + 1)))
#define value162arr(x, value) \
do { \
    *(x) = (uint8_t)(((value) >> 8) & 0xff); \
    *((x) + 1) = (uint8_t)((value) & 0xff); \
} while (0)

#define bt_mpc_data_check_crc(data, flag) \
do { \
    uint16_t len; \
    uint8_t *data_ptr = NULL; \
    data_ptr = (uint8_t *)(data) + MPC_OFFS_SOF_ID; \
    len = arr2value16((uint8_t *)(data) + MPC_OFFS_LENGTH); \
    len += MPC_SOF_LEN + MPC_LENGTH_LEN; \
    *(flag) = check_crc16(data_ptr, len); \
} while (0)

#define unused_param(x) (void)(x)
/*
 * bth a core error code
 */
typedef enum {
    BG_SDK_SUCC = 0x00,
    BG_SDK_FAIL = 0x01,
    BG_SDK_ALLOC_MEM_FAIL = 0x02,
} bg_sdk_err_code;

typedef enum {
    BTH_A_SUCC = 0x00,
    BTH_A_FAIL = 0x01,
    BTH_A_ALLOC_MEM_FAIL = 0x02,
} bth_a_err_code;

uint16_t sdk_co_read16p(void const *buf);
uint32_t sdk_co_read32p(void const *buf);
void sdk_co_write32p(void const *ptr32, uint32_t value);
uint16_t calc_crc16(const uint8_t *pkt, uint16_t pkt_len);
uint32_t check_crc16(uint8_t *pkt, uint16_t off);
int sdk_memcmp(const void *src, const void *dst, int count);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* __SDK_COMMON_H_ */
