/**
 * \file sm3.h
 *
 * \brief SM3 message digest algorithm (hash function)
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef MBEDTLS_SM3_H
#define MBEDTLS_SM3_H
#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"
#include "mbedtls/platform_util.h"

#include <stddef.h>
#include <stdint.h>

#define MBEDTLS_ERR_SM3_BAD_INPUT_DATA                    -0x0076  /**< SM3 input data was malformed. */

#if !defined(MBEDTLS_SM3_ALT)
// Regular implementation
//

/**
 * \brief          SM3 context structure
 */
typedef struct mbedtls_sm3_context
{
    uint32_t MBEDTLS_PRIVATE(total)[2];          /*!< number of bytes processed  */
    uint32_t MBEDTLS_PRIVATE(state)[8];          /*!< intermediate digest state, 32 Bytes, 256 bits */
    unsigned char MBEDTLS_PRIVATE(buffer)[64];   /*!< data block being processed */
}
mbedtls_sm3_context;

#else  /* MBEDTLS_SM3_ALT */
#include PATH_SM3_ALT_H
#endif /* MBEDTLS_SM3_ALT */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Initialize SM3 context
 *
 * \param ctx      The SM3 context to initialize. This must not be \c NULL.
 */
void mbedtls_sm3_init( mbedtls_sm3_context *ctx );

/**
 * \brief          This function clears a SM3 context.
 *
 * \param ctx      The SM3 context to clear. This may be \c NULL, in which
 *                 case this function returns immediately. If it is not \c NULL,
 *                 it must point to an initialized SM3 context.
 */
void mbedtls_sm3_free( mbedtls_sm3_context *ctx );

/**
 * \brief          This function clones the state of a SM3 context.
 *
 * \param dst      The destination context. This must be initialized.
 * \param src      The context to clone. This must be initialized.
 */
void mbedtls_sm3_clone( mbedtls_sm3_context *dst,
                        const mbedtls_sm3_context *src );

/**
 * \brief          This function starts a SM3 checksum calculation.
 *
 * \param ctx      The context to use. This must be initialized.
 *
 * \return         \c 0 on success.
 * \return         A negative error code on failure.
 */
int mbedtls_sm3_starts( mbedtls_sm3_context *ctx );

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SM3 checksum calculation.
 *
 * \param ctx      The SM3 context. This must be initialized
 *                 and have a hash operation started.
 * \param input    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * \param ilen     The length of the input data in Bytes.
 *
 * \return         \c 0 on success.
 * \return         A negative error code on failure.
 */
int mbedtls_sm3_update( mbedtls_sm3_context *ctx,
                            const unsigned char *input,
                            size_t ilen );

/**
 * \brief          This function finishes the SM3 operation, and writes
 *                 the result to the output buffer.
 *
 * \param ctx      The SM3 context. This must be initialized
 *                 and have a hash operation started.
 * \param output   The SM3 checksum result.
 *                 This must be a writable buffer of length \c 32 Bytes.
 *
 * \return         \c 0 on success.
 * \return         A negative error code on failure.
 */
int mbedtls_sm3_finish( mbedtls_sm3_context *ctx,
                            unsigned char output[32] );

/**
 * \brief          This function processes a single data block within
 *                 the ongoing SM3 computation. This function is for
 *                 internal use only.
 *
 * \param ctx      The SM3 context. This must be initialized.
 * \param data     The buffer holding one block of data. This must
 *                 be a readable buffer of length \c 64 Bytes.
 *
 * \return         \c 0 on success.
 * \return         A negative error code on failure.
 */
int mbedtls_internal_sm3_process( mbedtls_sm3_context *ctx,
                                  const unsigned char data[64] );

/**
 * \brief          This function calculates the SM3 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SM3 result is calculated as
 *                 output = SM3(input buffer).
 *
 * \param input    The buffer holding the data. This must be a readable
 *                 buffer of length \p ilen Bytes.
 * \param ilen     The length of the input data in Bytes.
 * \param output   The SM3 checksum result. This must
 *                 be a writable buffer of length \c 32 Bytes.
 */
int mbedtls_sm3( const unsigned char *input,
                      size_t ilen,
                      unsigned char output[32] );

#ifdef __cplusplus
}
#endif

#endif /* mbedtls_sm3.h */
