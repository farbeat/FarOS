/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides PBKDF2 source code \n
 * Author: @CompanyNameTag \n
 */

#include "cipher_api.h"
#include "common_def.h"
#include "securec.h"


#define SHA256_BYTES        32
#define SHA256_WORDS        8

#define SHA256_BUF_BYTES    64
#define SHA256_BUF_WORDS    16

#define PADDING_SIZE_MAX    64
#define U64_BYTES           8

#define DIGEST_TEMP_LEN     SHA256_WORDS
#define LEN_64              64
#define BIT_WIDTH_32        32

static const uint8_t g_sha256_padding[PADDING_SIZE_MAX] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const uint32_t g_sha256_kk[LEN_64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

typedef struct {
    uint64_t total_length;
    union {
        uint8_t  bytes[SHA256_BYTES];
        uint32_t words[SHA256_WORDS];
    } hash;
    union {
        uint8_t  bytes[SHA256_BUF_BYTES];
        uint32_t words[SHA256_BUF_WORDS];
    } buf;
} sha256_context_t;

typedef struct {
    sha256_context_t sha256_ctx;
    uint8_t ipad[PADDING_SIZE_MAX];
    uint8_t opad[PADDING_SIZE_MAX];
} hmac_sha256_context_t;


static inline uint32_t rotl(uint32_t x, uint32_t n)
{
    return ((x << n) | (x >> (BIT_WIDTH_32 - n)));
}

static inline uint32_t rotr(uint32_t x, uint32_t n)
{
    return ((x >> n) | (x << (BIT_WIDTH_32 - n)));
}

static inline uint32_t bytes_swap_32(uint32_t x)
{
    return ((rotr(x, 8) & 0xff00ff00L) | (rotl(x, 8) & 0x00ff00ffL));   /* hash alg param: 8. */
}

static inline uint32_t f1(uint32_t x, uint32_t y, uint32_t z)
{
    return (z ^ (x & (y ^ z)));
}

static inline uint32_t f3(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) | (z & (x | y)));
}

static inline uint32_t s0_256(uint32_t x)
{
    return (rotl(x, 10) ^ rotl(x, 19) ^ rotl(x, 30));   /* hash alg param: 10, 19, 30. */
}

static inline uint32_t s1_256(uint32_t x)
{
    return (rotl(x, 7) ^ rotl(x, 21) ^ rotl(x, 26));    /* hash alg param: 7, 21, 26. */
}

static inline uint32_t d0_256(uint32_t x)
{
    return (rotl(x, 14) ^ rotl(x, 25) ^ (x >> 3));    /* hash alg param: 14, 25, 3. */
}

static inline uint32_t d1_256(uint32_t x)
{
    return (rotl(x, 13) ^ rotl(x, 15) ^ (x >> 10));   /* hash alg param: 13, 15, 10. */
}

static inline uint64_t bytes_swap_64(uint64_t x)
{
    uint32_t a = (uint32_t)(x >> BIT_WIDTH_32);
    uint32_t b = (uint32_t)x;

    return (((uint64_t)bytes_swap_32(b) << BIT_WIDTH_32) | (uint64_t)bytes_swap_32(a));
}

static void drv_sha256_process(sha256_context_t *ctx)
{
    uint32_t ww[LEN_64];
    uint32_t digest_temp[DIGEST_TEMP_LEN];
    uint32_t i = 0;
    uint32_t tt1, tt2;

    for (i = 0; i < DIGEST_TEMP_LEN; i++) {
        digest_temp[i] = ctx->hash.words[i];
    }

    for (i = 0; i < LEN_64; i++) {
        if (i < SHA256_BUF_WORDS) {
            ww[i] = bytes_swap_32(ctx->buf.words[i]);
        } else {
            ww[i] = ww[i - 16] + ww[i - 7] + d0_256(ww[i - 15]) + d1_256(ww[i - 2]); /* hash alg param: 2, 7, 16, 15. */
        }

        tt1 = digest_temp[7] + s1_256(digest_temp[4]) +             /* array index: 4, 7 */
            f1(digest_temp[4], digest_temp[5], digest_temp[6]) +    /* array index: 4, 5, 6 */
            g_sha256_kk[i] + ww[i];
        tt2 = s0_256(digest_temp[0]) + f3(digest_temp[0], digest_temp[1], digest_temp[2]);  /* array index: 0, 1, 2 */

        digest_temp[7] = digest_temp[6];        /* array index: 6, 7 */
        digest_temp[6] = digest_temp[5];        /* array index: 5, 6 */
        digest_temp[5] = digest_temp[4];        /* array index: 4, 5 */
        digest_temp[4] = digest_temp[3] + tt1;  /* array index: 3, 4 */
        digest_temp[3] = digest_temp[2];        /* array index: 2, 3 */
        digest_temp[2] = digest_temp[1];        /* array index: 1, 2 */
        digest_temp[1] = digest_temp[0];        /* array index: 0, 1 */
        digest_temp[0] = tt1 + tt2;
    }

    for (i = 0; i < DIGEST_TEMP_LEN; i++) {
        ctx->hash.words[i] += digest_temp[i];
    }
}

static void drv_sha256_start(sha256_context_t *ctx)
{
    ctx->total_length = 0;
    ctx->hash.words[0] = 0x6a09e667L;   /* 0: 0x6a09e667L */
    ctx->hash.words[1] = 0xbb67ae85L;   /* 1: 0xbb67ae85L */
    ctx->hash.words[2] = 0x3c6ef372L;   /* 2: 0x3c6ef372L */
    ctx->hash.words[3] = 0xa54ff53aL;   /* 3: 0xa54ff53aL */
    ctx->hash.words[4] = 0x510e527fL;   /* 4: 0x510e527fL */
    ctx->hash.words[5] = 0x9b05688cL;   /* 5: 0x9b05688cL */
    ctx->hash.words[6] = 0x1f83d9abL;   /* 6: 0x1f83d9abL */
    ctx->hash.words[7] = 0x5be0cd19L;   /* 7: 0x5be0cd19L */
}

static void drv_sha256_context_copy(sha256_context_t *ctx, sha256_context_t *ctx_temp)
{
    ctx->total_length = ctx_temp->total_length;
    (void)memcpy_s(ctx->hash.bytes, sizeof(ctx->hash.bytes), ctx_temp->hash.bytes, sizeof(ctx_temp->hash.bytes));
}

static void drv_sha256_update(sha256_context_t *ctx, const uint8_t *data, uint32_t data_len)
{
    uint32_t length = data_len;
    uint32_t left = (uint32_t)(ctx->total_length & 0x3F);

    ctx->total_length += length;
    while ((length--) > 0) {
        ctx->buf.bytes[left++] = *(data++);
        if (left == 64) {   /* block size: 64 */
            drv_sha256_process(ctx);
            left = 0;
        }
    }
}

static void drv_sha256_final(sha256_context_t *ctx, uint8_t *hash, uint32_t hash_len)
{
    uint32_t low = (uint32_t)(ctx->total_length & 0x3F);
    uint32_t padn = (low < 56) ? (56 - low) : (120 - low);  /* pad 56 or 120 */
    uint64_t length = ctx->total_length;

    length <<= 3; /* bytes to bits, leftshift 3. */
    length = bytes_swap_64(length);

    drv_sha256_update(ctx, g_sha256_padding, padn);
    drv_sha256_update(ctx, (const uint8_t *)&length, sizeof(uint64_t));

    if ((hash != NULL) && (hash_len >= SHA256_BYTES)) {
        for (uint32_t i = 0; i < SHA256_BYTES; i += 4) {    /* unit: 4 */
            *(hash++) = ctx->hash.bytes[i + 3];     /* index: 3 */
            *(hash++) = ctx->hash.bytes[i + 2];     /* index: 2 */
            *(hash++) = ctx->hash.bytes[i + 1];     /* index: 1 */
            *(hash++) = ctx->hash.bytes[i    ];     /* index: 0 */
        }
    }
}

static void drv_sha256(const uint8_t *data, uint32_t data_len, uint8_t *hash, uint32_t hash_len)
{
    sha256_context_t ctx;

    drv_sha256_start(&ctx);
    drv_sha256_update(&ctx, data, data_len);
    drv_sha256_final(&ctx, hash, hash_len);

    (void)memset_s(&ctx, sizeof(ctx), 0, sizeof(ctx));
}

static void drv_hmac_sha256_start(hmac_sha256_context_t *ctx, const uint8_t *key, uint32_t key_len)
{
    uint32_t len = key_len;
    if (len > SHA256_BYTES) {
        drv_sha256(key, len, ctx->sha256_ctx.buf.bytes, SHA256_BYTES);
        len = SHA256_BYTES;
        key = ctx->sha256_ctx.buf.bytes;
    }

    memset_s(ctx->ipad, sizeof(ctx->ipad), 0x36, sizeof(ctx->ipad));
    memset_s(ctx->opad, sizeof(ctx->opad), 0x5C, sizeof(ctx->opad));

    for (uint32_t i = 0; i < len; i++) {
        ctx->ipad[i] ^= key[i];
        ctx->opad[i] ^= key[i];
    }

    drv_sha256_start(&ctx->sha256_ctx);
    drv_sha256_update(&ctx->sha256_ctx, ctx->ipad, PADDING_SIZE_MAX);
}

static void pkcs5_pbkdf2_hmac(const uapi_drv_cipher_kdf_pbkdf2_param_t *param, uint8_t *out, const uint32_t out_len)
{
    hmac_sha256_context_t ctx_ipad;
    sha256_context_t ctx, ctx_opad;
    uint8_t md1[SHA256_BYTES];

    drv_hmac_sha256_start(&ctx_ipad, param->password, param->plen);
    drv_sha256_start(&ctx_opad);
    drv_sha256_update(&ctx_opad, ctx_ipad.opad, PADDING_SIZE_MAX);

    memset_s(out, out_len, 0, out_len);

    uint32_t key_length = out_len;
    uint8_t c[4] = {0x00, 0x00, 0x00, 0x01};

    for (uint32_t iter_count = 0; iter_count < param->count; iter_count++) {
        drv_sha256_context_copy(&ctx, &ctx_ipad.sha256_ctx);
        if (iter_count == 0) {
            drv_sha256_update(&ctx, param->salt, param->slen);
            drv_sha256_update(&ctx, c, sizeof(c));
        } else {
            drv_sha256_update(&ctx, md1, sizeof(md1));
        }
        drv_sha256_final(&ctx, md1, sizeof(md1));

        drv_sha256_context_copy(&ctx, &ctx_opad);
        drv_sha256_update(&ctx, md1, sizeof(md1));
        drv_sha256_final(&ctx, md1, sizeof(md1));

        for (uint32_t i = 0; i < key_length; i++) {
            out[i] ^= md1[i];
        }
    }

    memset_s(&ctx,      sizeof(ctx),      0, sizeof(ctx));
    memset_s(&ctx_ipad, sizeof(ctx_ipad), 0, sizeof(ctx_ipad));
    memset_s(&ctx_opad, sizeof(ctx_opad), 0, sizeof(ctx_opad));
}

errcode_t uapi_drv_cipher_pbkdf2(const uapi_drv_cipher_kdf_pbkdf2_param_t *param, uint8_t *out, const uint32_t out_len)
{
    if ((param == NULL) || (param->password == NULL) || (param->salt == NULL) || (out == NULL)) {
        return ERRCODE_FAIL;
    }

    if (param->hash_type != UAPI_DRV_CIPHER_HASH_TYPE_SHA256) {
        return ERRCODE_FAIL;
    }

    pkcs5_pbkdf2_hmac(param, out, out_len);
    return ERRCODE_SUCC;
}
