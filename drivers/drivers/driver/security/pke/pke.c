/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides pke driver source \n
 * Author: @CompanyNameTag \n
 */

#include "pke.h"
#include "drv_pke.h"


errcode_t uapi_drv_cipher_pke_rsa_sign(const uapi_drv_cipher_pke_rsa_priv_key_t *priv_key,
    uapi_drv_cipher_pke_rsa_scheme_t scheme,
    uapi_drv_cipher_pke_hash_type_t hash_type,
    const uapi_drv_cipher_pke_data_t *input_hash,
    uapi_drv_cipher_pke_data_t *sign)
{
    return uapi_drv_pke_rsa_sign((const drv_pke_rsa_priv_key_t *)priv_key,
        (const drv_pke_rsa_scheme_t)scheme,
        (const drv_pke_hash_type_t)hash_type,
        (const drv_pke_data_t *)input_hash,
        (drv_pke_data_t *)sign);
}

errcode_t uapi_drv_cipher_pke_rsa_verify(const uapi_drv_cipher_pke_rsa_pub_key_t *pub_key,
    uapi_drv_cipher_pke_rsa_scheme_t scheme,
    uapi_drv_cipher_pke_hash_type_t hash_type,
    uapi_drv_cipher_pke_data_t *input_hash,
    const uapi_drv_cipher_pke_data_t *sig)
{
    return uapi_drv_pke_rsa_verify((const drv_pke_rsa_pub_key_t *)pub_key, (const drv_pke_rsa_scheme_t)scheme,
        (drv_pke_hash_type_t)hash_type, (const drv_pke_data_t *)input_hash, (const drv_pke_data_t *)sig);
}

errcode_t uapi_drv_cipher_pke_rsa_public_encrypt(uapi_drv_cipher_pke_rsa_scheme_t scheme,
    uapi_drv_cipher_pke_hash_type_t hash_type,
    const uapi_drv_cipher_pke_rsa_pub_key_t *pub_key,
    const uapi_drv_cipher_pke_data_t *input,
    const uapi_drv_cipher_pke_data_t *label,
    uapi_drv_cipher_pke_data_t *output)
{
    return uapi_drv_pke_rsa_public_encrypt((const drv_pke_rsa_scheme_t)scheme,
        (const drv_pke_hash_type_t)hash_type,
        (const drv_pke_rsa_pub_key_t *)pub_key,
        (const drv_pke_data_t *)input,
        (const drv_pke_data_t *)label,
        (drv_pke_data_t *)output);
}

errcode_t uapi_drv_cipher_pke_rsa_private_decrypt(uapi_drv_cipher_pke_rsa_scheme_t scheme,
    uapi_drv_cipher_pke_hash_type_t hash_type,
    const uapi_drv_cipher_pke_rsa_priv_key_t *priv_key,
    const uapi_drv_cipher_pke_data_t *input,
    const uapi_drv_cipher_pke_data_t *label,
    const uapi_drv_cipher_pke_data_t *output)
{
    return uapi_drv_pke_rsa_private_decrypt((const drv_pke_rsa_scheme_t)scheme,
        (const drv_pke_hash_type_t)hash_type,
        (const drv_pke_rsa_priv_key_t *)priv_key,
        (const drv_pke_data_t *)input,
        (const drv_pke_data_t *)label,
        (drv_pke_data_t *)output);
}

errcode_t uapi_drv_cipher_pke_exp_mod(const uapi_drv_cipher_pke_data_t *n, const uapi_drv_cipher_pke_data_t *k,
    const uapi_drv_cipher_pke_data_t *in, const uapi_drv_cipher_pke_data_t *out)
{
    return uapi_drv_pke_exp_mod((const drv_pke_data_t *)n, (const drv_pke_data_t *)k,
        (const drv_pke_data_t *)in, (drv_pke_data_t *)out);
}
