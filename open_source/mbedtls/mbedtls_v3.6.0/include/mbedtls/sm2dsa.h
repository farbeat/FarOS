/**
 * \file sm2dsa.h
 *
 * \brief This file contains SM2 ECDSA definitions and functions.
 *
 * The SM2 Elliptic Curve Digital Signature Algorithm (ECDSA) is
 * defined in GB/T 32918.2-2016.
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

#ifndef MBEDTLS_SM2DSA_H
#define MBEDTLS_SM2DSA_H
#include "mbedtls/private_access.h"

#include "mbedtls/build_info.h"

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief               This function computes message hash for the SM2DSA signature
 *                      and verification.
 *
 * \param grp           The context for the elliptic curve to use.
 *                      This must be initialized and have group parameters
 *                      set, for example through mbedtls_ecp_group_load().
 *                      with MBEDTLS_ECP_DP_SM2P256V1.
 * \param pk            The public key. This must be initialized.
 *                      This must not be \c NULL.
 * \param id            The device id. It must be readable. This must not be \c NULL.
 * \param id_len        The id length must less than or equal to 0x1FFF.
 * \param buf           The buffer holding the input data.
 *                      It must be readable and of size \p blen Bytes.
 * \param blen          The length of \p buf in Bytes.
 * \param hash          The hash buffer where the output data will be written.
 *                      It must be writeable.
 * \param hashlen       The length of \p hash in Bytes.
 * \param md_alg        The hash algorithm used to hash the original data.
 *
 * \return              \c 0 on success.
 * \return              An \c MBEDTLS_ERR_ECP_XXX
 *                      or \c MBEDTLS_ERR_MD_XXX error code on failure.
 */
int mbedtls_sm2dsa_calc_hash( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pk,
                              const unsigned char *id, size_t id_len,
                              const unsigned char *buf, size_t blen,
                              unsigned char *hash, unsigned char hashlen,
                              mbedtls_md_type_t md_alg );

/**
 * \brief               This function computes device-info hash z for the SM2DSA signature
 *                      and verification. This function is for internal use only.
 *
 * \param grp           The context for the elliptic curve to use.
 *                      This must be initialized and have group parameters
 *                      set, for example through mbedtls_ecp_group_load().
 *                      with MBEDTLS_ECP_DP_SM2P256V1.
 * \param pk            The public key. This must be initialized.
 *                      This must not be \c NULL.
 * \param id            The device id. It must be readable. This must not be \c NULL.
 * \param id_len        The id length must less than or equal to 0x1FFF.
 * \param z             The device-info hash buffer where the output data will be written.
 *                      It must be writeable.
 * \param zlen          The length of \p device-info hash in Bytes.
 * \param md_info       The information structure of the message-digest algorithm
 *                      to use.
 *
 * \return              \c 0 on success.
 * \return              An \c MBEDTLS_ERR_ECP_XXX
 *                      or \c MBEDTLS_ERR_MD_XXX error code on failure.
 */
int sm2dsa_calc_z_internal( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pk,
                            const unsigned char *id, size_t id_len,
                            unsigned char *z, size_t zlen,
                            const mbedtls_md_info_t *md_info );

/**
 * \brief               This function computes the SM2DSA signature of a
 *                      previously-hashed message. This function is for
 *                      internal use only.
 *
 * \note                If the bitlength of the message hash is larger than the
 *                      bitlength of the group order, then the hash is truncated
 *                      as defined in <em>Standards for Efficient Cryptography Group
 *                      (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                      4.1.3, step 5.
 *
 * \see                 ecp.h
 *
 * \param grp           The context for the elliptic curve to use.
 *                      This must be initialized and have group parameters
 *                      set, for example through mbedtls_ecp_group_load().
 * \param r             The MPI context in which to store the first part
 *                      the signature. This must be initialized.
 * \param s             The MPI context in which to store the second part
 *                      the signature. This must be initialized.
 * \param d             The private signing key. This must be initialized
 *                      and setup, for example through mbedtls_ecp_gen_privkey().
 * \param buf           The hashed content to be signed. This must be a readable
 *                      buffer of length \p blen Bytes. It may be \c NULL if
 *                      \p blen is zero.
 * \param blen          The length of \p buf in Bytes.
 * \param f_rng         The RNG function. This must not be \c NULL.
 * \param p_rng         The RNG parameter to be passed to \p f_rng. This may be
 *                      \c NULL if \p f_rng doesn't need a context argument.
 * \param f_rng_blind   The RNG function used for blinding. This must not be
 *                      \c NULL.
 * \param p_rng_blind   The RNG context to be passed to \p f_rng. This may be
 *                      \c NULL if \p f_rng doesn't need a context parameter.
 *
 * \return              \c 0 on success.
 * \return              An \c MBEDTLS_ERR_ECP_XXX
 *                      or \c MBEDTLS_MPI_XXX error code on failure.
 */
int sm2dsa_sign_internal( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                          const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                          int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
                          int (*f_rng_blind)(void *, unsigned char *, size_t), void *p_rng_blind );

/**
 * \brief               This function verifies the SM2DSA signature of a
 *                      previously-hashed message. This function is for
 *                      internal use only.
 *
 * \note                If the bitlength of the message hash is larger than the
 *                      bitlength of the group order, then the hash is truncated as
 *                      defined in <em>Standards for Efficient Cryptography Group
 *                      (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                      4.1.4, step 3.
 *
 * \see                 ecp.h
 *
 * \param grp           The ECP group to use.
 *                      This must be initialized and have group parameters
 *                      set, for example through mbedtls_ecp_group_load().
 * \param buf           The hashed content that was signed. This must be a readable
 *                      buffer of length \p blen Bytes. It may be \c NULL if
 *                      \p blen is zero.
 * \param blen          The length of \p buf in Bytes.
 * \param Q             The public key to use for verification. This must be
 *                      initialized and setup.
 * \param r             The first integer of the signature.
 *                      This must be initialized.
 * \param s             The second integer of the signature.
 *                      This must be initialized.
 *
 * \return              \c 0 on success.
 * \return              #MBEDTLS_ERR_ECP_BAD_INPUT_DATA if the signature
 *                      is invalid.
 * \return              An \c MBEDTLS_ERR_ECP_XXX or \c MBEDTLS_MPI_XXX
 *                      error code on failure for any other reason.
 */
int sm2dsa_verify_internal( mbedtls_ecp_group *grp,
                            const unsigned char *buf, size_t blen,
                            const mbedtls_ecp_point *Q, const mbedtls_mpi *r,
                            const mbedtls_mpi *s);

#ifdef __cplusplus
}
#endif

#endif /* sm2dsa.h */
