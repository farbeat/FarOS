/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pke driver \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#include "securec.h"
#include "errcode.h"
#include "cipher_api.h"
#include "trng.h"
#include "systick.h"
#include "common_def.h"
#include "hal_rsa.h"
#include "drv_pke.h"
#include "rsa_porting.h"


#define RSA_PADLEN_1            1
#define RSA_PADLEN_2            2
#define RSA_PADLEN_3            3
#define RSA_PADLEN_8            8
#define RSA_PADLEN_11           11
#define RSA_PADLEN_19           19
#define RSA_PADLEN_51           51
#define RSA_PAD_X12             0x12
#define RSA_PAD_X55             0x55
#define RSA_PAD_XBC             0xBC

#define WORD_SIZE               4
#define RSA4096_SIGN_LENGTH     512
#define RSA4096_PUBKEY_LENGTH   1024
#define RSA_TIMEOUT_MS          2000ULL

/* result size */
#define HASH_SIZE_SHA_256       32
#define HASH_SIZE_SHA_512       64
#define HASH_SIZE_SHA_MAX       64

#define SHA1_BLOCK_LENGTH       64
#define SHA256_BLOCK_LENGTH     64
#define SHA512_BLOCK_LENGTH     128
#define SHA256_RESULT_LENGTH    32
#define SHA512_RESULT_LENGTH    64

#define WORD_INDEX_0            0
#define WORD_INDEX_1            1
#define WORD_INDEX_2            2
#define WORD_INDEX_3            3

/* Boundary value 1. */
#define BOUND_VALUE_1           1

/* multiple value */
#define MULTIPLE_VALUE_1        1
#define MULTIPLE_VALUE_2        2

#define SHIFT_4BITS             4
#define SHIFT_8BITS             8
#define SHIFT_16BITS            16
#define SHIFT_24BITS            24
#define MAX_LOW_2BITS           3
#define MAX_LOW_3BITS           7
#define MAX_LOW_4BITS           0xF
#define MAX_LOW_8BITS           0xFF
#define BYTE_BITS               8

/*! Define the max block size for hash calculation */
#define PKE_MAX_HASH_BLOCK_SIZE 128

#define BYTES_PER_WORD          4

typedef struct {
    uapi_drv_cipher_hash_type_t hash_type;
    uint32_t hash_len;
    uint8_t *lhash_data;
    uint8_t *asn1_data;
    uint32_t asn1_len;
} rsa_pkcs1_hash_info_t;

typedef struct {
    drv_pke_hash_type_t hash_type;
    uint32_t block_len;
    uint32_t result_len;
} pke_hash_properties_type_t;

typedef struct {
    uint32_t klen;
    uint32_t em_bit;
    uint8_t *em;
    uint32_t em_len;
    uint8_t *hash;
    uint32_t hash_len;
    uint8_t *data;
    uint32_t data_len;
} rsa_pkcs1_pack_t;

static const uint8_t g_asn1_rsa256[] = {
    0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
    0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
    0x00, 0x04, 0x20,
};

static const uint8_t g_asn1_rsa512[] = {
    0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
    0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05,
    0x00, 0x04, 0x40,
};

static const uint8_t g_empty_l_rsa256[] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55,
};

static const uint8_t g_empty_l_rsa512[] = {
    0xcf, 0x83, 0xe1, 0x35, 0x7e, 0xef, 0xb8, 0xbd,
    0xf1, 0x54, 0x28, 0x50, 0xd6, 0x6d, 0x80, 0x07,
    0xd6, 0x20, 0xe4, 0x05, 0x0b, 0x57, 0x15, 0xdc,
    0x83, 0xf4, 0xa9, 0x21, 0xd3, 0x6c, 0xe9, 0xce,
    0x47, 0xd0, 0xd1, 0x3c, 0x5d, 0x85, 0xf2, 0xb0,
    0xff, 0x83, 0x18, 0xd2, 0x87, 0x7e, 0xec, 0x2f,
    0x63, 0xb9, 0x31, 0xbd, 0x47, 0x41, 0x7a, 0x81,
    0xa5, 0x38, 0x32, 0x7a, 0xf9, 0x27, 0xda, 0x3e,
};

static const pke_hash_properties_type_t g_pke_hash_properties[] = {
    {DRV_PKE_HASH_TYPE_SHA256, SHA256_BLOCK_LENGTH, SHA256_RESULT_LENGTH},
    {DRV_PKE_HASH_TYPE_SHA512, SHA512_BLOCK_LENGTH, SHA512_RESULT_LENGTH}
};

static uint8_t g_l_hash[HASH_SIZE_SHA_512] = {0x0};

static const uint8_t g_bits[BYTE_BITS * MULTIPLE_VALUE_2] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

static void rsa_pre_process(void);
static void rsa_post_process(void);
static uint32_t rsa_get_bit_num(const uint8_t *big_num, uint32_t num_len);
static errcode_t rsa_init(void);
static void rsa_deinit(void);
static errcode_t rsa_calculate(rsa_attr_t *rsa_attr);
static errcode_t hash_check(const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash);
static errcode_t get_hash_prop(const uapi_drv_cipher_hash_type_t hash_type, uint32_t *block_size);
static errcode_t get_random(uint8_t *rand, const uint32_t size);
static errcode_t pkcs1_v15_encrypt(const rsa_pkcs1_pack_t *pack);
static errcode_t pkcs1_v15_decrypt(rsa_pkcs1_pack_t *pack);
static errcode_t do_hash_update(uint32_t h_handle, const drv_pke_data_t *arr, const uint32_t len, uint32_t block_size);
static errcode_t pke_calc_hash(const drv_pke_data_t *arr, const uint32_t len,
    const uapi_drv_cipher_hash_type_t hash_type, drv_pke_data_t *hash);
static errcode_t pkcs1_get_hash(const drv_pke_hash_type_t hash_type, const drv_pke_data_t *label,
    rsa_pkcs1_hash_info_t *hash_info);
static errcode_t pkcs1_mgf(const rsa_pkcs1_hash_info_t *hash_info, const uint8_t *seed, const uint32_t seed_len,
    uint8_t *mask, const uint32_t mask_len);
static errcode_t pkcs1_oaep_encrypt(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack,
    const drv_pke_data_t *label);
static errcode_t pkcs1_oaep_decrypt(const drv_pke_hash_type_t hash_type, rsa_pkcs1_pack_t *pack,
    const drv_pke_data_t *label);
static errcode_t rsa_encrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type, uint8_t *em,
    const uint32_t klen, const drv_pke_data_t *msg, const drv_pke_data_t *label);
static errcode_t rsa_decrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type, uint8_t *em,
    const uint32_t klen, const drv_pke_data_t *label, drv_pke_data_t *msg);
static errcode_t rsa_exp_mod(const uint8_t *n, const uint8_t *key, const uint32_t len, const drv_pke_data_t *input,
    drv_pke_data_t *output);
static errcode_t pkcs1_v15_sign(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack);
static errcode_t pkcs1_pss_hash(const rsa_pkcs1_hash_info_t *hash_info, const uint8_t *m_hash, const uint32_t klen,
    const drv_pke_data_t *salt, uint8_t *hash);
static errcode_t pkcs1_pss_sign(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack);
static errcode_t rsa_sign(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    uint8_t *em, const uint32_t klen, const uint32_t em_bit, const drv_pke_data_t *msg);
static errcode_t pkcs1_v15_verify(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack);
static errcode_t pkcs1_pss_verify(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack);
static errcode_t rsa_verify(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    uint8_t *em, const uint32_t klen, const uint32_t em_bit, const drv_pke_data_t *msg);

errcode_t uapi_drv_pke_rsa_public_encrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    const drv_pke_rsa_pub_key_t *pub_key, const drv_pke_data_t *input, const drv_pke_data_t *label,
    drv_pke_data_t *output)
{
    if (pub_key == NULL || output == NULL || input == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (output->data == NULL || (input->data == NULL && input->length != 0)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (scheme >= DRV_PKE_RSA_SCHEME_MAX) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (pub_key->n == NULL || pub_key->e == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    errcode_t ret;
    static uint8_t em[DRV_PKE_LEN_4096];
    uint32_t klen = pub_key->len;
    drv_pke_data_t data_in = {0};
    if ((klen != DRV_PKE_LEN_2048) && (klen != DRV_PKE_LEN_4096)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (memset_s(em, klen, 0x00, klen) != EOK) {
        return ERRCODE_MEMSET;
    }

    ret = rsa_encrypt(scheme, hash_type, em, klen, input, label);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    data_in = (drv_pke_data_t) {.length = klen, .data = em};
    ret = rsa_exp_mod(pub_key->n, pub_key->e, klen, &data_in, output);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcmp(input->data, output->data, input->length) == 0) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_pke_rsa_private_decrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    const drv_pke_rsa_priv_key_t *priv_key, const drv_pke_data_t *input, const drv_pke_data_t *label,
    drv_pke_data_t *output)
{
    if (output == NULL || (output->data == NULL && output->length != 0)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (input == NULL || input->data == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (scheme >= DRV_PKE_RSA_SCHEME_MAX) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (priv_key == NULL || priv_key->n == NULL || priv_key->d == NULL || priv_key->n_len != input->length) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    errcode_t ret;
    static uint8_t em[DRV_PKE_LEN_4096];
    uint32_t klen = priv_key->n_len;
    drv_pke_data_t data_out = {0};
    if ((klen != DRV_PKE_LEN_2048) && (klen != DRV_PKE_LEN_4096)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (memset_s(em, klen, 0x00, klen) != EOK) {
        return ERRCODE_MEMSET;
    }

    data_out = (drv_pke_data_t) {.length = klen, .data = em};
    ret = rsa_exp_mod(priv_key->n, priv_key->d, klen, input, &data_out);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = rsa_decrypt(scheme, hash_type, em, klen, label, output);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcmp(input->data, output->data, output->length) == 0) {
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_pke_rsa_sign(const drv_pke_rsa_priv_key_t *priv_key, const drv_pke_rsa_scheme_t scheme,
    const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash, drv_pke_data_t *sign)
{
    if (priv_key == NULL || priv_key->n == NULL || priv_key->d == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (input_hash == NULL || input_hash->data == NULL || sign == NULL || sign->data == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (hash_check(hash_type, input_hash) != ERRCODE_SUCC) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (scheme >= DRV_PKE_RSA_SCHEME_MAX) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    uint8_t em[DRV_PKE_LEN_4096] = {0};
    uint32_t klen = priv_key->n_len;
    uint32_t em_bit = 0;
    drv_pke_data_t in = {0};
    if ((klen != DRV_PKE_LEN_2048) && (klen != DRV_PKE_LEN_4096)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    em_bit = rsa_get_bit_num((const uint8_t*)priv_key->n, klen);
    errcode_t ret = rsa_sign(scheme, hash_type, em, klen, em_bit, input_hash);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    in = (drv_pke_data_t) {.length = klen, .data = em};
    ret = rsa_exp_mod(priv_key->n, priv_key->d, priv_key->n_len, &in, sign);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcmp(input_hash->data, sign->data, input_hash->length) == 0) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_drv_pke_rsa_verify(const drv_pke_rsa_pub_key_t *pub_key, drv_pke_rsa_scheme_t scheme,
    const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash, const drv_pke_data_t *sign)
{
    if (pub_key == NULL || pub_key->n == NULL || pub_key->e == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (input_hash == NULL || input_hash->data == NULL || sign == NULL || sign->data == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (hash_check(hash_type, input_hash) != ERRCODE_SUCC) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (scheme >= DRV_PKE_RSA_SCHEME_MAX) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if ((pub_key->len != DRV_PKE_LEN_2048) && (pub_key->len != DRV_PKE_LEN_4096)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    errcode_t ret;
    static uint8_t em[DRV_PKE_LEN_4096];
    uint8_t *pem = em;
    uint32_t klen = pub_key->len;
    uint32_t em_bit = 0;
    drv_pke_data_t out = {0};
    if (memset_s(em, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096) != EOK) {
        return ERRCODE_MEMSET;
    }

    out = (drv_pke_data_t) {.length = klen, .data = em};
    ret = rsa_exp_mod(pub_key->n, pub_key->e, pub_key->len, sign, &out);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcmp(sign->data, pem, klen) == 0) {
        return ERRCODE_FAIL;
    }

    em_bit = rsa_get_bit_num((const uint8_t*)pub_key->n, klen);
    ret = rsa_verify(scheme, hash_type, em, klen, em_bit, input_hash);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_drv_pke_exp_mod(const drv_pke_data_t *n, const drv_pke_data_t *k, const drv_pke_data_t *in,
    drv_pke_data_t *out)
{
    errcode_t ret;
    uint32_t klen;
    if (n == NULL || n->data == NULL || k == NULL || k->data == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if (in == NULL || in->data == NULL || out == NULL || out->data == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    klen = n->length;
    if ((klen != k->length) || (klen != in->length) || (klen != out->length)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    if ((klen != DRV_PKE_LEN_2048) && (klen != DRV_PKE_LEN_4096)) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    ret = rsa_exp_mod(n->data, k->data, n->length, in, out);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static uint32_t rsa_get_bit_num(const uint8_t *big_num, uint32_t num_len)
{
    uint32_t i;
    uint8_t num = 0;
    uint32_t big_num_val = 0;

    for (i = 0; i < num_len; i++) {
        num = g_bits[(big_num[i] & (MAX_LOW_8BITS - MAX_LOW_4BITS)) >> SHIFT_4BITS];
        if (num > 0) {
            big_num_val = (num_len - i - BOUND_VALUE_1) * BYTE_BITS + num + WORD_SIZE;
            break;
        }

        num = g_bits[big_num[i] & MAX_LOW_4BITS];
        if (num > 0) {
            big_num_val = (num_len - i - BOUND_VALUE_1) * BYTE_BITS + num;
            break;
        }
    }
    return big_num_val;
}

static void rsa_pre_process(void)
{
    rsa_port_set_soft_reset(0x1);
    rsa_port_set_clk_en(0x1);
    rsa_set_endian(RSA_BIG_ENDIAN_32BIT);
}

static void rsa_post_process(void)
{
    rsa_port_set_soft_reset(0x0);
    rsa_port_set_clk_en(0x0);
}

static errcode_t rsa_init(void)
{
    errcode_t ret;
    rsa_pre_process();
    rsa_port_register_hal_funcs();
    hal_rsa_funcs_t *hal_funcs = rsa_port_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    ret = hal_funcs->init(NULL);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static void rsa_deinit(void)
{
    hal_rsa_funcs_t *hal_funcs = rsa_port_get_funcs();
    hal_funcs->deinit();
    rsa_port_unregister_hal_funcs();
    rsa_post_process();
}

static errcode_t rsa_calculate(rsa_attr_t *rsa_attr)
{
    errcode_t ret = ERRCODE_SUCC;
    hal_rsa_funcs_t *hal_funcs = rsa_port_get_funcs();

    hal_funcs->start(rsa_attr);

    uint64_t start_time = uapi_systick_get_ms();
    while (!hal_funcs->finish()) {
        if ((uapi_systick_get_ms() - start_time) >= RSA_TIMEOUT_MS) {
            ret = ERRCODE_PKE_TIMEOUT;
            break;
        }
    }

    rsa_deinit();
    return ret;
}

static errcode_t hash_check(const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash)
{
    uint32_t i;
    errcode_t ret = ERRCODE_FAIL;
    for (i = 0; i < sizeof(g_pke_hash_properties) / sizeof(pke_hash_properties_type_t); i++) {
        if (g_pke_hash_properties[i].hash_type == hash_type &&
            g_pke_hash_properties[i].result_len == input_hash->length) {
            ret = ERRCODE_SUCC;
            break;
        }
    }

    return ret;
}

static errcode_t get_hash_prop(const uapi_drv_cipher_hash_type_t hash_type, uint32_t *block_size)
{
    drv_pke_hash_type_t pke_hash_type;
    switch (hash_type) {
        case UAPI_DRV_CIPHER_HASH_TYPE_SHA1:
            pke_hash_type = DRV_PKE_HASH_TYPE_SHA1;
            break;

        case UAPI_DRV_CIPHER_HASH_TYPE_SHA224:
            pke_hash_type = DRV_PKE_HASH_TYPE_SHA224;
            break;

        case UAPI_DRV_CIPHER_HASH_TYPE_SHA256:
            pke_hash_type = DRV_PKE_HASH_TYPE_SHA256;
            break;

        case UAPI_DRV_CIPHER_HASH_TYPE_SHA384:
            pke_hash_type = DRV_PKE_HASH_TYPE_SHA384;
            break;

        case UAPI_DRV_CIPHER_HASH_TYPE_SHA512:
            pke_hash_type = DRV_PKE_HASH_TYPE_SHA512;
            break;

        case UAPI_DRV_CIPHER_HASH_TYPE_SM3:
            pke_hash_type = DRV_PKE_HASH_TYPE_SM3;
            break;

        default:
            return ERRCODE_PKE_INVALID_PARAMETER;
    }

    uint32_t i;
    errcode_t ret = ERRCODE_PKE_INVALID_PARAMETER;
    for (i = 0; i < sizeof(g_pke_hash_properties) / sizeof(pke_hash_properties_type_t); i++) {
        if (g_pke_hash_properties[i].hash_type == pke_hash_type) {
            *block_size = g_pke_hash_properties[i].block_len;
            ret = ERRCODE_SUCC;
            break;
        }
    }

    return ret;
}

static errcode_t get_random(uint8_t *rand, const uint32_t size)
{
    errcode_t ret;
    uint32_t k = 0;
    uint32_t randnum = 0;
    static uint8_t randlist[DRV_PKE_LEN_4096];
    if (memset_s(randlist, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096) != EOK) {
        return ERRCODE_MEMSET;
    }

    for (k = 0; k < (size + WORD_SIZE - 1) / WORD_SIZE; k++) {
        ret = uapi_drv_cipher_trng_get_random((uint32_t *)(randlist + k * WORD_SIZE));
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    k = 0;
    while (k < size) {
        if (randlist[k] != 0x00) {
            k++;
            continue;
        }
        ret = uapi_drv_cipher_trng_get_random(&randnum);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
        randlist[k] = (uint8_t)(randnum & 0xFF);
    }

    if (memcpy_s(rand, size, randlist, size) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

/* EM = 00 || 02 || PS || 00 || M */
static errcode_t pkcs1_v15_encrypt(const rsa_pkcs1_pack_t *pack)
{
    errcode_t ret;
    uint8_t *p = NULL;
    uint32_t idx = 0;
    p = pack->em;
    p[idx++] = 0x00;
    p[idx++] = 0x02;

    /* PS */
    ret = get_random(&p[idx], pack->em_len - pack->data_len - RSA_PADLEN_3);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    idx += pack->em_len - pack->data_len - RSA_PADLEN_3;

    p[idx++] = 0x00;
    /* M */
    if (pack->data_len != 0) {
        if (memcpy_s(&p[idx], pack->data_len, pack->data, pack->data_len) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }

    return ERRCODE_SUCC;
}

/* check EM = 00 || 02 || PS || 00 || M */
static errcode_t pkcs1_v15_decrypt(rsa_pkcs1_pack_t *pack)
{
    uint8_t *p = NULL;
    uint32_t idx = 0;
    uint32_t len = 0;

    p = pack->em;
    if (p[idx++] != 0x00) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    if (p[idx++] != 0x02) {
        return ERRCODE_PKE_INVALID_PADDING;
    }
    /* PS */
    while ((p[idx] != 0x00) && (idx < (pack->em_len - 1))) {
        idx++;
    }
    idx++;

    len = pack->em_len - idx;
    if (len > pack->em_len - RSA_PADLEN_11) {
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    /* M */
    if (len != 0) {
        if (memcpy_s(pack->data, pack->data_len, &p[idx], len) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }
    pack->data_len = len; // record length of plain text for return

    return ERRCODE_SUCC;
}

static errcode_t do_hash_update(uint32_t h_handle, const drv_pke_data_t *arr, const uint32_t len, uint32_t block_size)
{
    errcode_t ret;
    uint32_t i = 0;
    uint32_t cur = 0;
    uint32_t offset = 0;
    static uint32_t block_mem[PKE_MAX_HASH_BLOCK_SIZE / BYTES_PER_WORD]; /* u32 makes block_mem aligned by 4-byte */
    uapi_drv_cipher_buf_attr_t h_buf = {
        .phys_addr = (uintptr_t)block_mem,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE
    };

    if (memset_s((uint8_t *)block_mem, PKE_MAX_HASH_BLOCK_SIZE, 0, PKE_MAX_HASH_BLOCK_SIZE) != EOK) {
        return ERRCODE_MEMSET;
    }

    for (i = 0, cur = 0, offset = 0; i < len;) {
        if (cur + arr[i].length - offset < block_size) {
            if (memcpy_s((uint8_t *)block_mem + cur, PKE_MAX_HASH_BLOCK_SIZE - cur,
                arr[i].data + offset, arr[i].length - offset) != EOK) {
                return ERRCODE_MEMCPY;
            }
            cur += arr[i].length - offset;
            offset = 0;
            i++;
        } else {
            if (memcpy_s((uint8_t *)block_mem + cur, PKE_MAX_HASH_BLOCK_SIZE - cur,
                arr[i].data + offset, block_size - cur) != EOK) {
                return ERRCODE_MEMCPY;
            }
            if (cur + arr[i].length - offset == block_size) {
                offset = 0;
                i++;
            } else {
                offset += block_size - cur;
            }

            cur = 0;
            ret = uapi_drv_cipher_hash_update(h_handle, &h_buf, block_size);
            if (ret != ERRCODE_SUCC) {
                return ret;
            }
        }
    }

    if (cur != 0) {
        ret = uapi_drv_cipher_hash_update(h_handle, &h_buf, cur);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    return ERRCODE_SUCC;
}

static errcode_t pke_calc_hash(const drv_pke_data_t *arr, const uint32_t len,
    const uapi_drv_cipher_hash_type_t hash_type, drv_pke_data_t *hash)
{
    errcode_t ret;
    uint32_t h_handle = 0;
    uint32_t block_size = 0;
    uapi_drv_cipher_hash_attr_t hash_attr = {.hash_type = hash_type};

    (void)get_hash_prop(hash_type, &block_size);

    ret = uapi_drv_cipher_hash_start(&h_handle, &hash_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = do_hash_update(h_handle, arr, len, block_size);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_drv_cipher_hash_finish(h_handle, hash->data, &(hash->length));
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static errcode_t pkcs1_get_hash(const drv_pke_hash_type_t hash_type, const drv_pke_data_t *label,
    rsa_pkcs1_hash_info_t *hash_info)
{
    switch (hash_type) {
        case DRV_PKE_HASH_TYPE_SHA256:
            hash_info->hash_type = UAPI_DRV_CIPHER_HASH_TYPE_SHA256;
            hash_info->hash_len = HASH_SIZE_SHA_256;
            hash_info->asn1_data = (uint8_t *)g_asn1_rsa256;
            hash_info->asn1_len = (uint32_t)sizeof(g_asn1_rsa256);
            hash_info->lhash_data = (uint8_t *)g_empty_l_rsa256;
            break;
        case DRV_PKE_HASH_TYPE_SHA512:
            hash_info->hash_type = UAPI_DRV_CIPHER_HASH_TYPE_SHA512;
            hash_info->hash_len = HASH_SIZE_SHA_512;
            hash_info->asn1_data = (uint8_t *)g_asn1_rsa512;
            hash_info->asn1_len = (uint32_t)sizeof(g_asn1_rsa512);
            hash_info->lhash_data = (uint8_t *)g_empty_l_rsa512;
            break;
        default:
            return ERRCODE_PKE_UNSUPPORTED;
    }

    if (label != NULL && label->data != NULL && label->length != 0) {
        hash_info->lhash_data = g_l_hash;
        drv_pke_data_t h_hash = {
            .data = hash_info->lhash_data,
            .length = hash_info->hash_len
        };
        errcode_t ret = pke_calc_hash(label, 1, hash_info->hash_type, &h_hash);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    return ERRCODE_SUCC;
}

static errcode_t pkcs1_mgf(const rsa_pkcs1_hash_info_t *hash_info, const uint8_t *seed, const uint32_t seed_len,
    uint8_t *mask, const uint32_t mask_len)
{
    errcode_t ret;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t out_len = 0;
    uint8_t cnt[WORD_SIZE];
    uint8_t md[HASH_SIZE_SHA_MAX];
    drv_pke_data_t hash = {0};
    static uint8_t seed_buf[DRV_PKE_LEN_4096];
    uapi_drv_cipher_hash_attr_t h_type = {0};

    if (memset_s(cnt, WORD_SIZE, 0x00, WORD_SIZE) != EOK) {
        return ERRCODE_MEMSET;
    }

    if (memset_s(md, HASH_SIZE_SHA_MAX, 0x00, HASH_SIZE_SHA_MAX) != EOK) {
        return ERRCODE_MEMSET;
    }
    if (memset_s(seed_buf, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096) != EOK) {
        return ERRCODE_MEMSET;
    }

    if (memcpy_s(seed_buf, DRV_PKE_LEN_4096, seed, seed_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    h_type.hash_type = hash_info->hash_type;
    hash.length = HASH_SIZE_SHA_MAX;
    hash.data = md;

    drv_pke_data_t arr[2] = {0};
    arr[0].data = seed_buf;
    arr[0].length = seed_len;
    arr[1].data = cnt;
    arr[1].length = (uintptr_t)sizeof(cnt);

    for (i = 0; out_len < mask_len; i++) {
        cnt[WORD_INDEX_0] = (uint8_t)((i >> SHIFT_24BITS) & MAX_LOW_8BITS);
        cnt[WORD_INDEX_1] = (uint8_t)((i >> SHIFT_16BITS) & MAX_LOW_8BITS);
        cnt[WORD_INDEX_2] = (uint8_t)((i >> SHIFT_8BITS)) & MAX_LOW_8BITS;
        cnt[WORD_INDEX_3] = (uint8_t)(i & MAX_LOW_8BITS);

        /* H = Hash(seedbuf || cnt) */
        ret = pke_calc_hash(arr, sizeof(arr) / sizeof(drv_pke_data_t), h_type.hash_type, &hash);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }

        for (j = 0; (j < hash_info->hash_len) && (out_len < mask_len); j++) {
            mask[out_len++] ^= md[j];
        }
    }

    return ERRCODE_SUCC;
}

/* oaep set EM = 0x00 || maskedSeed || maskedDB */
static errcode_t pkcs1_oaep_encrypt(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack,
    const drv_pke_data_t *label)
{
    uint32_t idx = 0;
    rsa_pkcs1_hash_info_t hash_info = {0};

    errcode_t ret = pkcs1_get_hash(hash_type, label, &hash_info);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    uint32_t hash_len = hash_info.hash_len;
    if (pack->data_len > pack->klen - 2 * hash_len - 2) { /* 2 */
        return ERRCODE_PKE_INVALID_PARAMETER;
    }

    uint8_t *p = pack->em;
    uint8_t *seed = p + 1;
    uint8_t *mask_db = p + hash_len + 1;

    /* 1. set data[0] = 0 */
    p[idx++] = 0x00;

    /* 2. set data[1, hash_len + 1] = random */
    ret = get_random(&p[idx], hash_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    idx += hash_len;

    /* 3. set data[hash_len + 1, 2hash_len + 1] = lHash */
    if (memcpy_s(p + idx, hash_len, hash_info.lhash_data, hash_len) != EOK) {
        return ERRCODE_MEMCPY;
    }
    idx += hash_len;
    /* 4. set PS with 0x00 */
    uint32_t tmp_len = pack->klen - pack->data_len - 2 * hash_len - 2; /* 2 */
    if (memset_s(p + idx, tmp_len, 0x00, tmp_len) != EOK) {
        return ERRCODE_MEMSET;
    }
    idx += tmp_len;

    /* 5. set 0x01 after PS */
    p[idx++] = 0x01;

    /* 6. set M */
    if (pack->data_len != 0) {
        if (memcpy_s(p + idx, pack->data_len, pack->data, pack->data_len) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }

    /* 7. MGF: seed -> mask_db */
    tmp_len = pack->klen - hash_len - 1;
    ret = pkcs1_mgf(&hash_info, seed, hash_len, mask_db, tmp_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* 8. MGF: mask_db -> mask_seed */
    ret = pkcs1_mgf(&hash_info, mask_db, tmp_len, seed, hash_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static errcode_t pkcs1_oaep_decrypt(const drv_pke_hash_type_t hash_type, rsa_pkcs1_pack_t *pack,
                                    const drv_pke_data_t *label)
{
    uint32_t idx = 0;
    rsa_pkcs1_hash_info_t hash_info = {0};

    errcode_t ret = pkcs1_get_hash(hash_type, label, &hash_info);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    uint32_t hash_len = hash_info.hash_len;
    uint8_t *p = pack->em;
    uint8_t *seed = p + 1;
    uint8_t *db = p + hash_len + 1;

    /* 1. check data[0] = 0 */
    if (p[idx++] != 0x00) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    /* 2. MGF: mask_db -> seed */
    uint32_t tmp_len = pack->klen - hash_len - 1;
    ret = pkcs1_mgf(&hash_info, db, tmp_len, seed, hash_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* 3. MGF: seed -> db */
    ret = pkcs1_mgf(&hash_info, seed, hash_len, db, tmp_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* 4. check data[hash + 1, 2hash + 1] */
    idx += hash_len;
    if (memcmp(p + idx, hash_info.lhash_data, hash_len) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    /* 5. remove PS */
    idx += hash_len;
    for (; idx < pack->klen - 1; idx++) {
        if (p[idx] != 0x00) {
            break;
        }
    }

    /* 6. check 0x01 */
    if (p[idx++] != 0x01) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    /* 7. check data length */
    tmp_len = pack->klen - idx;
    if (tmp_len != 0) {
        if (memcpy_s(pack->data, pack->data_len, p + idx, tmp_len) != EOK) {
            return ERRCODE_MEMCPY;
        }
    }
    pack->data_len = tmp_len; // record length of plain text for return

    return ERRCODE_SUCC;
}

static errcode_t rsa_encrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type, uint8_t *em,
    const uint32_t klen, const drv_pke_data_t *msg, const drv_pke_data_t *label)
{
    rsa_pkcs1_pack_t en_pack = {
        .klen = klen,
        .em_bit = 0,
        .em = em,
        .em_len = klen,
        .hash = NULL,
        .hash_len = 0,
        .data = msg->data,
        .data_len = msg->length
    };

    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        return pkcs1_v15_encrypt(&en_pack);
    } else {
        return pkcs1_oaep_encrypt(hash_type, &en_pack, label);
    }
}

static errcode_t rsa_decrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type, uint8_t *em,
    const uint32_t klen, const drv_pke_data_t *label, drv_pke_data_t *msg)
{
    errcode_t ret;
    rsa_pkcs1_pack_t de_pack = {
        .klen = klen,
        .em_bit = 0,
        .em = em,
        .em_len = klen,
        .hash = NULL,
        .hash_len = 0,
        .data = msg->data,
        .data_len = msg->length
    };

    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        ret = pkcs1_v15_decrypt(&de_pack);
    } else {
        ret = pkcs1_oaep_decrypt(hash_type, &de_pack, label);
    }

    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    msg->length = de_pack.data_len;
    return ERRCODE_SUCC;
}

static errcode_t rsa_exp_mod(const uint8_t *n, const uint8_t *key, const uint32_t len, const drv_pke_data_t *input,
    drv_pke_data_t *output)
{
    errcode_t ret;
    rsa_attr_t rsa_attr;

    ret = rsa_init();
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = rsa_set_attr(&rsa_attr, n, key, len, input->data, input->length);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = rsa_calculate(&rsa_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcpy_s(output->data, len, (uint32_t*)(uintptr_t)rsa_attr.output_addr, len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

/* EM = 00 || 01 || PS || 00 || T */
static errcode_t pkcs1_v15_sign(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack)
{
    uint8_t *p = NULL;
    uint32_t idx = 0;
    rsa_pkcs1_hash_info_t hash_info = {0};
    // 此处label传入NULL,且hash_type前面校验过,返回值可忽略
    (void)pkcs1_get_hash(hash_type, NULL, &hash_info);

    p = pack->em;
    p[idx++] = 0x00;
    p[idx++] = 0x01;

    /* PS */
    while (idx < (pack->em_len - hash_info.hash_len - hash_info.asn1_len - 1)) {
        p[idx++] = 0xFF;
    }

    p[idx++] = 0x00;

    /* T */
    if (memcpy_s(&p[idx], hash_info.asn1_len, hash_info.asn1_data, hash_info.asn1_len) != EOK) {
        return ERRCODE_MEMCPY;
    }
    idx += hash_info.asn1_len;
    if (memcpy_s(&p[idx], hash_info.hash_len, pack->hash, hash_info.hash_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

/* H = Hash( M' ) = Hash( Padding1 || mHash || salt ) */
static errcode_t pkcs1_pss_hash(const rsa_pkcs1_hash_info_t *hash_info, const uint8_t *m_hash, const uint32_t klen,
    const drv_pke_data_t *salt, uint8_t *hash)
{
    uint8_t ps[8];
    const uint32_t h_len = HASH_SIZE_SHA_MAX;
    drv_pke_data_t h_hash = {0};
    uapi_drv_cipher_hash_attr_t h_type = {0};

    h_hash.length = h_len;
    h_hash.data = hash;
    h_type.hash_type = hash_info->hash_type;

    if (memset_s(ps, RSA_PADLEN_8, 0x00, RSA_PADLEN_8) != EOK) {
        return ERRCODE_MEMSET;
    }

    errcode_t ret;
    /* H = Hash(PS || MHash || SALT) */
    drv_pke_data_t arr[3] = {0}; // 3 elements in total
    arr[0].data = ps; // 0 th element is ps
    arr[0].length = RSA_PADLEN_8; // 0 element is PS
    arr[1].data = (uint8_t *)m_hash; // 1 element is MHash
    arr[1].length = klen; // 1 element is MHash
    arr[2].data = (uint8_t *)salt->data; // 2 element is SALT
    arr[2].length = salt->length; // 2 element is SALT
    ret = pke_calc_hash(arr, sizeof(arr) / sizeof(drv_pke_data_t), h_type.hash_type, &h_hash);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static errcode_t pkcs1_pss_sign(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack)
{
    errcode_t ret;
    rsa_pkcs1_hash_info_t hash_info = {0};
    uint8_t salt_data[HASH_SIZE_SHA_MAX];
    uint32_t salt_len = 0;
    uint32_t tmp_len = 0;
    uint32_t ms_bits = 0;
    uint8_t *masked_db = NULL;
    uint8_t *masked_seed = NULL;
    drv_pke_data_t salt = {0};

    (void)pkcs1_get_hash(hash_type, NULL, &hash_info);

    salt_len = hash_info.hash_len;
    ms_bits = (pack->em_bit - BOUND_VALUE_1) & MAX_LOW_3BITS;
    ret = get_random((uint8_t*)salt_data, salt_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    masked_db = pack->em;
    masked_seed = pack->em + pack->em_len - pack->hash_len - 1; /* 1 byte for bound. */
    salt.data = salt_data;
    salt.length = salt_len;
    ret = pkcs1_pss_hash(&hash_info, pack->hash, pack->hash_len, &salt, masked_seed);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    tmp_len = pack->em_len - pack->hash_len - salt_len - RSA_PADLEN_2;
    if (memset_s(masked_db, tmp_len, 0x00, tmp_len) != EOK) {
        return ERRCODE_MEMSET;
    }

    masked_db[tmp_len++] = 0x01;
    if (memcpy_s(masked_db + tmp_len, salt_len, salt_data, salt_len) != EOK) {
        return ERRCODE_MEMCPY;
    }

    tmp_len = pack->em_len - pack->hash_len - 1;
    ret = pkcs1_mgf(&hash_info, masked_seed, pack->hash_len, masked_db, tmp_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    pack->em[0] &= MAX_LOW_8BITS >> (BYTE_BITS - ms_bits);
    pack->em[pack->em_len - 1] = RSA_PAD_XBC; /* 1 byte for bound. */
    return ERRCODE_SUCC;
}

static errcode_t rsa_sign(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    uint8_t *em, const uint32_t klen, const uint32_t em_bit, const drv_pke_data_t *msg)
{
    rsa_pkcs1_pack_t sg_pack = {
        .klen = klen,
        .em_bit = em_bit,
        .em = em,
        .em_len = klen,
        .hash = msg->data,
        .hash_len = msg->length,
        .data = NULL,
        .data_len = 0
    };

    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        return pkcs1_v15_sign(hash_type, &sg_pack);
    } else {
        return pkcs1_pss_sign(hash_type, &sg_pack);
    }
}

/* check EM = 00 || 01 || PS || 00 || T */
static errcode_t pkcs1_v15_verify(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack)
{
    uint8_t *p = NULL;
    uint32_t idx = 0;
    rsa_pkcs1_hash_info_t hash_info = {0};

    (void)pkcs1_get_hash(hash_type, NULL, &hash_info);

    p = pack->em;
    if (p[idx++] != 0x00) {
        return ERRCODE_PKE_INVALID_PADDING;
    }
    if (p[idx++] != 0x01) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    /* PS */
    while (idx < (pack->em_len - hash_info.hash_len - hash_info.asn1_len - 1)) {
        if (p[idx++] != 0xFF) {
            return ERRCODE_PKE_INVALID_PADDING;
        }
    }

    if (p[idx++] != 0x00) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    /* T */
    if (memcmp(&p[idx], hash_info.asn1_data, hash_info.asn1_len) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }
    if (memcmp(hash_info.asn1_data, &p[idx], hash_info.asn1_len) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    idx += hash_info.asn1_len;
    if (memcmp(&p[idx], pack->hash, hash_info.hash_len) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }
    if (memcmp(pack->hash, &p[idx], hash_info.hash_len) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    return ERRCODE_SUCC;
}

static errcode_t pkcs1_pss_verify(const drv_pke_hash_type_t hash_type, const rsa_pkcs1_pack_t *pack)
{
    errcode_t ret;
    rsa_pkcs1_hash_info_t hash_info = {0};
    static uint8_t em[DRV_PKE_LEN_4096];
    uint8_t hash[HASH_SIZE_SHA_MAX];
    uint8_t *phash = hash;
    uint32_t salt_len = 0;
    uint32_t tmp_len = 0;
    drv_pke_data_t salt = {0};

    if (memset_s(em, DRV_PKE_LEN_4096, 0x00, DRV_PKE_LEN_4096) != EOK) {
        return ERRCODE_MEMSET;
    }

    (void)pkcs1_get_hash(hash_type, NULL, &hash_info);

    if (pack->em[pack->em_len - 1] != RSA_PAD_XBC) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    uint32_t ms_bits = (pack->em_bit - BOUND_VALUE_1) & MAX_LOW_3BITS;
    if ((pack->em[0] & (MAX_LOW_8BITS << ms_bits)) != 0) {
        return ERRCODE_PKE_INVALID_PADDING;
    }

    (void)memcpy_s(em, DRV_PKE_LEN_4096, pack->em, pack->em_len);

    uint8_t *masked_db = em;
    uint8_t *masked_seed = em + pack->em_len - pack->hash_len - 1; /* 1 byte for bound. */
    tmp_len = pack->em_len - pack->hash_len - 1;
    ret = pkcs1_mgf(&hash_info, masked_seed, pack->hash_len, masked_db, tmp_len);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    em[0] &= MAX_LOW_8BITS >> (BYTE_BITS - ms_bits);

    while (masked_db < masked_seed - 1 && *masked_db == 0) {
        masked_db++;
    }

    if (*masked_db++ != 0x01) {
        return ERRCODE_FAIL;
    }

    salt_len = masked_seed - masked_db;
    salt.length = salt_len;
    salt.data = masked_db;

    ret = pkcs1_pss_hash(&hash_info, pack->hash, pack->hash_len, &salt, (uint8_t*)hash);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    if (memcmp(phash, masked_seed, hash_info.hash_len) != 0) {
        return ERRCODE_FAIL;
    }

    if (memcmp(masked_seed, hash, hash_info.hash_len) != 0) { /* reduntancy check */
        return ERRCODE_FAIL;
    }

    return ERRCODE_SUCC;
}

static errcode_t rsa_verify(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    uint8_t *em, const uint32_t klen, const uint32_t em_bit, const drv_pke_data_t *msg)
{
    rsa_pkcs1_pack_t pack = {
        .klen = klen,
        .em_bit = em_bit,
        .em = em,
        .em_len = klen,
        .hash = msg->data,
        .hash_len = msg->length,
        .data = NULL,
        .data_len = 0
    };

    if (scheme == DRV_PKE_RSA_SCHEME_PKCS1_V15) {
        return pkcs1_v15_verify(hash_type, &pack);
    } else {
        return pkcs1_pss_verify(hash_type, &pack);
    }
}
