/**
 * Copyright (c) CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: mbedtls harden alg alt internal source file.
 * Author: CompanyName
 * Create: 2023-05-10
*/
#ifndef CCM_ALT_H
#define CCM_ALT_H
#include "crypto_cipher_common_struct.h"

#if defined(MBEDTLS_CCM_ALT)

/**
 * \brief    The CCM context-type alt definition. The CCM context is passed
 *           to the APIs called.
 */

typedef struct {
    unsigned int kslot_handle;
    crypto_symc_ccm_ctx store_ctx;
    unsigned int data_len;        /* Crypto Data Length In Bytes. */
    unsigned int tag_len;         /* Tag Length In Bytes. */
    unsigned int total_aad_len;
    unsigned int processed_aad_len;
    unsigned int processed_data_len;
    unsigned char iv[16];
    unsigned int iv_length;
    unsigned char key[32];
    unsigned int key_len;
    int is_decrypt; // flag of crypt
    unsigned long long phys_addr;
    void *virt_addr;
} mbedtls_ccm_context;

#endif

#endif