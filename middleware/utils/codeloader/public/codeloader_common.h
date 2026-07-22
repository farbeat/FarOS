/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER COMMON DEFINITIONS
 * Author:
 * Date:
 */

#ifndef SRC_APPLICATION_CODELOADER_COMMON_DEFINITIONS_H
#define SRC_APPLICATION_CODELOADER_COMMON_DEFINITIONS_H

#include <stdint.h>

/**
 * @brief  Codeloader generic buffer handler
 */
typedef struct {
    uint32_t length;
    uint8_t *buffer;
} codeloader_common_generic_buffer_t;

/**
 * @brief  Codeloader Reception buffer handler
 */
typedef struct {
    volatile uint32_t length;
    uint8_t *buffer;
} codeloader_rx_buffer_t;

#define CODELOADER_TX_BUFFER_SIZE 8704  // 8.5KB
#define bswap(x)  (((x>>24)&0x000000FFUL) | ((x<<24)&0xFF000000UL)  | \
                    ((x>>8)&0x0000FF00UL) | ((x<<8)&0x00FF0000UL))

#define bitswap_16bit(x)          ((((x) >> 8) & 0x00FFUL) | (((x) << 8) & 0xFF00UL))

#endif
