/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides symc def header\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef SYMC_DEF_H
#define SYMC_DEF_H

#include <stdint.h>
#include "errcode.h"
#include "symc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ENCRYPT                         0x0
#define DECRYPT                         0x1

#define CIPHER_INVALID_HANDLE           0xFFFFFFFF
#define SYMC_KEY_SLOT_INVALID           0xFFFFFFFF

#define SYMC_IV_STATE_START             0
#define SYMC_IV_STATE_UPDATE            1
#define SYMC_IV_STATE_FINISH            2

#define AES_BLOCK_SIZE                  16
#define AES_IV_LEN                      4

typedef union {
    struct {
        uint8_t j0[AES_IV_SIZE];
        uint32_t iv_ghash[AES_IV_SIZE / DRV_WORD_WIDTH];
        uint32_t updated_len;
        uint8_t *aad;
        uint32_t aad_len;
    } gcm;
    struct {
        uint8_t s0[AES_IV_SIZE];
        uint32_t iv_mac[AES_IV_SIZE / DRV_WORD_WIDTH];
        uint8_t *aad;
        uint32_t aad_len;
        uint32_t updated_len;
        uint32_t tag_len;
        uint32_t total_len;
    } ccm;
} symc_extend_info_t;

typedef struct {
    drv_cipher_alg_t alg;
    drv_cipher_work_mode_t mode;
    drv_cipher_bit_width_t width;
    drv_cipher_key_length_t klen;
    uint8_t dfa_enable;
    uint8_t block_size;
    uint8_t key_sel;
    uint8_t reserved;
} symc_config_t;

typedef struct {
    uint32_t iv[AES_IV_SIZE / DRV_WORD_WIDTH];
    uintptr_t src_addr;
    uint32_t buffer_length;
    uintptr_t dest_addr;
    uint8_t work_ss_flag;
    uint8_t work_ds_flag;
    uint8_t is_odd_key;
    uint8_t iv_state;
    uint32_t work_alg;
    uint32_t work_mode;
    symc_extend_info_t extend;
} symc_node_context_t;

typedef struct {
    symc_config_t config;
    uint32_t keyslot_id;
    uint32_t timeout_ms;
    uint8_t lock_mode;
    uint8_t operation;
    uint16_t reserved;
    symc_node_context_t node_context;
} symc_hard_context_t;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
