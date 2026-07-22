/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 * Description: Test symc source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-11, Create file. \n
 */
#include "securec.h"
#include "test_print.h"
#include "test_suite_errors.h"
#include "test_suite.h"
#include "symc.h"
#include "km.h"
#include "test_cipher.h"

#define DATA_LEN             512
#define INVALID_HANDLE       (0xFFFFFFFF)
#define CACHE_LINE_SIZE      32
#define AES_KEY_128BIT       16
#define AES_KEY_192BIT       24
#define AES_KEY_256BIT       32

#define DUMP_BYTES_ONCE_LEN  16
#define DUMP_BYTES_TIMES     3

static uint8_t g_aes_key[] = {
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C,
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3D
};

static uint8_t g_aes_iv[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static uint8_t g_aes_src[] = {
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
    0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
    0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
};

static uint8_t g_aes_gcm_key[] = {
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C,
    0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08,
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C,
    0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08,
};

static uint8_t  g_aes_ccm_key[] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
};

typedef struct {
    drv_cipher_buff_attr_t src;
    uint32_t src_len;
    drv_cipher_buff_attr_t dst;
    uint32_t dst_len;
    uint8_t *iv;
    uint32_t iv_len;
    uint8_t *key;
    uint32_t key_len;
    uint8_t *expect_tag;
    uint32_t expect_tag_len;
    uapi_drv_klad_engine_t crypto_alg;
    union {
        drv_cipher_config_aes_t aes_config;
        drv_cipher_config_aes_gcm_t aes_gcm_config;
        drv_cipher_config_aes_ccm_t aes_ccm_config;
    } cipher_config_t;
    drv_cipher_attr_t cipher_attr;
    drv_cipher_config_t ctrl;
    bool is_clear_key;
} cipher_test_info_t;

cipher_test_info_t g_aes_test_info = {
    .src = {
        .address = g_aes_src,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .src_len = sizeof(g_aes_src),
    .dst = {
        .address = NULL,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .iv = g_aes_iv,
    .iv_len = sizeof(g_aes_iv),
    .key = g_aes_key,
    .key_len = AES_KEY_128BIT, /* key */
    .expect_tag = NULL,
    .expect_tag_len = 0,
    .crypto_alg = UAPI_DRV_KLAD_ENGINE_AES, /* needed or can be replaced by compare other parameters */
    .cipher_config_t.aes_config = {
        .key_len  = DRV_CIPHER_KEY_128BIT,
        .bit_width = DRV_CIPHER_BIT_WIDTH_128BIT,
        .iv_change_flag = DRV_CIPHER_IV_CHANGE_ONE_PKG,
        .key_parity = false
    },
    .cipher_attr = {
        .cipher_type = DRV_CIPHER_TYPE_NORMAL,
    },
    .ctrl = {
        .alg = DRV_CIPHER_ALG_AES,             /* < Cipher algorithm */
        .work_mode = DRV_CIPHER_WORK_MODE_CBC, /* < Operating mode */
        .dfa = DRV_CIPHER_DFA_DISABLE,         /* Against DFA attack enable flag (0xa: close DFA; 0x5:open DFA). */
        .key_sel = DRV_DIPHER_KEY_SOFT
    },
    .is_clear_key = false, /* is_clear_key */
};

static cipher_test_info_t g_aes_ccm_test_info = {
    .src = {
        .address = NULL,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .src_len = 0,
    .dst =  {
        .address = NULL,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .iv = NULL,
    .iv_len = 0,
    .key = g_aes_key,
    .key_len = AES_KEY_128BIT, /* key */
    .expect_tag = NULL,
    .expect_tag_len = 0,
    .crypto_alg = UAPI_DRV_KLAD_ENGINE_AES, /* needed or can be replaced by compare other parameters */
    .cipher_config_t.aes_ccm_config = {
        .key_len = DRV_CIPHER_KEY_128BIT,
        .key_parity = false,
        .iv_change_flag = DRV_CIPHER_CCM_IV_CHANGE_START,
        .iv_len = 0,
        .aad = NULL,
        .aad_len = 0,
        .tag_len = 0,
        .total_len = 0
    },
    .cipher_attr = {
        .cipher_type = DRV_CIPHER_TYPE_NORMAL,
    },
    .ctrl = {
        .alg = DRV_CIPHER_ALG_AES,             /* < Cipher algorithm */
        .work_mode = DRV_CIPHER_WORK_MODE_CCM, /* < Operating mode */
        .dfa = DRV_CIPHER_DFA_DISABLE,         /* Against DFA attack enable flag (0xa: close DFA; 0x5:open DFA). */
        .key_sel = DRV_DIPHER_KEY_SOFT
    },
    .is_clear_key = true, /* is_clear_key */
};

static cipher_test_info_t g_aes_gcm_test_info = {
    .src = {
        .address = NULL,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .src_len = 0,
    .dst =  {
        .address = NULL,
        .buf_sec = DRV_CIPHER_BUFFER_NONSECURE
    },
    .dst_len = 0,
    .key = g_aes_key,
    .key_len = AES_KEY_128BIT, /* key */
    .expect_tag = NULL,
    .expect_tag_len = 0,
    .crypto_alg = UAPI_DRV_KLAD_ENGINE_AES, /* needed or can be replaced by compare other parameters */
    .cipher_config_t.aes_gcm_config = {
        .key_len = DRV_CIPHER_KEY_128BIT,
        .key_parity = false,
        .iv_change_flag = DRV_CIPHER_GCM_IV_CHANGE_START,
        .iv = NULL,
        .iv_len = 0,
        .aad = NULL,
        .aad_len = 0,
    },
    .cipher_attr = {
        .cipher_type = DRV_CIPHER_TYPE_NORMAL,
    },
    .ctrl = {
        .alg = DRV_CIPHER_ALG_AES,             /* < Cipher algorithm */
        .work_mode = DRV_CIPHER_WORK_MODE_GCM, /* < Operating mode */
        .dfa = DRV_CIPHER_DFA_DISABLE,         /* Against DFA attack enable flag (0xa: close DFA; 0x5:open DFA). */
        .key_sel = DRV_DIPHER_KEY_SOFT
    },
    .is_clear_key = true, /* is_clear_key */
};


static errcode_t cipher_set_clear_key(uint32_t keyslot, unsigned char *key, unsigned int keylen,
                                      bool key_parity, uapi_drv_klad_engine_t alg)
{
    errcode_t ret;
    uint32_t klad;
    uapi_drv_klad_clear_key_t clear_key;
    uapi_drv_klad_dest_t dest = UAPI_DRV_KLAD_DEST_MCIPHER;
    uapi_drv_klad_attr_t attr = { 0 };

    attr.key_cfg.engine          = (uapi_drv_klad_engine_t)alg;
    attr.key_cfg.decrypt_support = 0x1;
    attr.key_cfg.encrypt_support = 0x1;

    clear_key.key_length   = keylen; /* 16 or 32 */
    clear_key.key          = key;
    clear_key.key_parity   = key_parity;

    /* get klad */
    ret = uapi_drv_klad_create(&klad);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = uapi_drv_klad_attach(klad, dest, keyslot);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ret = uapi_drv_klad_set_attr(klad, &attr);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ret = uapi_drv_klad_set_clear_key(klad, &clear_key);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ret = uapi_drv_klad_detach(klad, dest, keyslot);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ret = uapi_drv_klad_destroy(klad);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;

exit:
    (void)uapi_drv_klad_destroy(klad);
    return ERRCODE_FAIL;
}

errcode_t cipher_common(cipher_test_info_t *test_info)
{
    errcode_t ret;
    uint32_t hcipher = INVALID_HANDLE;
    uint32_t hkeyslot = INVALID_HANDLE;

    uint32_t length;
    drv_cipher_buff_attr_t src_buf;
    drv_cipher_buff_attr_t dest_buf;
    static uint8_t result[DATA_LEN] __attribute__ ((aligned (CACHE_LINE_SIZE))) = {0};

    src_buf.address = test_info->src.address;
    src_buf.buf_sec = test_info->src.buf_sec;
    dest_buf.address = result;
    dest_buf.buf_sec = test_info->dst.buf_sec;
    length = test_info->src_len;

    ret = uapi_drv_cipher_create(&hcipher, &test_info->cipher_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* create keyslot */
    ret = uapi_drv_keyslot_create(&hkeyslot, UAPI_DRV_KEYSLOT_TYPE_MCIPHER);
    if (ret != ERRCODE_SUCC) {
        goto cleanup2;
    }

    /* attach keyslot and cipher */
    ret = uapi_drv_cipher_attach(hcipher, hkeyslot);
    if (ret != ERRCODE_SUCC) {
        goto cleanup1;
    }

    /* encrypt test */
    ret = uapi_drv_cipher_set_config(hcipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    /* diferrence in key_len(128/256)(can be control by input param) && content_key/clear_key && AES */
    ret = cipher_set_clear_key(hkeyslot, test_info->key, test_info->key_len,
        ((drv_cipher_config_aes_t*)test_info->ctrl.param)->key_parity, test_info->crypto_alg);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("encrypt src", src_buf.address, length);

    ret = uapi_drv_cipher_encrypt(hcipher, &src_buf, &dest_buf, length, 0);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("encrypt result", result, length);

    if (test_info->dst.address != NULL) {
        if (memcmp(test_info->dst.address, result, length) != ERRCODE_SUCC) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

    /* decrypt test */
    ret = uapi_drv_cipher_set_config(hcipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    src_buf.address = test_info->dst.address;
    src_buf.buf_sec = test_info->dst.buf_sec;
    dest_buf.address = result;
    dest_buf.buf_sec = test_info->src.buf_sec;

    print_buffer("decrypt src", src_buf.address, length);

    ret = uapi_drv_cipher_decrypt(hcipher, &src_buf, &dest_buf, length, 0);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("decrypt result", result, length);
    print_buffer("src data", test_info->src.address, length);

    if (test_info->src.address != NULL) {
        if (memcmp(test_info->src.address, result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

cleanup0:
    (void)uapi_drv_cipher_detach(hcipher, hkeyslot);
cleanup1:
    (void)uapi_drv_keyslot_destroy(hkeyslot);
cleanup2:
    (void)uapi_drv_cipher_destroy(hcipher);

    return ret;
}

errcode_t cipher_gcm_ccm_common(cipher_test_info_t *test_info)
{
    errcode_t ret;
    uint32_t hcipher = INVALID_HANDLE;
    uint32_t hkeyslot = INVALID_HANDLE;

    drv_cipher_buff_attr_t src_buf;
    drv_cipher_buff_attr_t dest_buf;
    uint32_t length;
    uint8_t tag[16] = {0};
    uint32_t tag_len = test_info->expect_tag_len;
    static uint8_t result[DATA_LEN] __attribute__ ((aligned (CACHE_LINE_SIZE))) = {0};

    src_buf.address = test_info->src.address;
    src_buf.buf_sec = test_info->src.buf_sec;
    dest_buf.address = result;
    dest_buf.buf_sec = test_info->dst.buf_sec;
    length = test_info->src_len;

    print_buffer("input data", src_buf.address, length);

    ret = uapi_drv_cipher_create(&hcipher, &test_info->cipher_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    /* create keyslot */
    ret = uapi_drv_keyslot_create(&hkeyslot, UAPI_DRV_KEYSLOT_TYPE_MCIPHER);
    if (ret != ERRCODE_SUCC) {
        goto cleanup2;
    }

    /* attach keyslot and cipher */
    ret = uapi_drv_cipher_attach(hcipher, hkeyslot);
    if (ret != ERRCODE_SUCC) {
        goto cleanup1;
    }

    /* encrypt test */
    if (test_info->ctrl.work_mode == DRV_CIPHER_WORK_MODE_GCM) {
        ret = cipher_set_clear_key(hkeyslot, test_info->key, test_info->key_len,
            ((drv_cipher_config_aes_gcm_t *)test_info->ctrl.param)->key_parity,
            test_info->crypto_alg);
    } else {
        ret = cipher_set_clear_key(hkeyslot, test_info->key, test_info->key_len,
            ((drv_cipher_config_aes_ccm_t *)test_info->ctrl.param)->key_parity,
            test_info->crypto_alg);
    }

    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    ret = uapi_drv_cipher_set_config(hcipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    ret = uapi_drv_cipher_encrypt(hcipher, &src_buf, &dest_buf, length, 0);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("encrypt result", result, length);
    print_buffer("expect encrypt result", test_info->dst.address, length);

    /* unique for gcm */
    ret = uapi_drv_cipher_get_tag(hcipher, tag, &tag_len);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("tag", tag, tag_len);
    print_buffer("expect tag", test_info->expect_tag, tag_len);

    if (test_info->dst.address != NULL) {
        if (memcmp(test_info->dst.address, result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

    if (memcmp(tag, test_info->expect_tag, tag_len) != 0) {
        ret = ERRCODE_FAIL;
        goto cleanup0;
    }

    /* decrypt test */
    ret = uapi_drv_cipher_set_config(hcipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    src_buf.address = test_info->dst.address;
    src_buf.buf_sec = test_info->dst.buf_sec;
    dest_buf.address = result;
    dest_buf.buf_sec = test_info->src.buf_sec;

    ret = uapi_drv_cipher_decrypt(hcipher, &src_buf, &dest_buf, length, 0);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("decrypt result", result, length);

    (void)memset_s(&tag, sizeof(tag), 0, sizeof(tag));
    ret = uapi_drv_cipher_get_tag(hcipher, tag, &tag_len);
    if (ret != ERRCODE_SUCC) {
        goto cleanup0;
    }

    print_buffer("tag", tag, tag_len);
    print_buffer("expect tag", test_info->expect_tag, tag_len);

    if (memcmp(tag, test_info->expect_tag, tag_len) != 0) {
        ret = ERRCODE_FAIL;
        goto cleanup0;
    }

    if (test_info->src.address != NULL) {
        if (memcmp(test_info->src.address, result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

cleanup0:
    (void)uapi_drv_cipher_detach(hcipher, hkeyslot);
cleanup1:
    (void)uapi_drv_keyslot_destroy(hkeyslot);
cleanup2:
    (void)uapi_drv_cipher_destroy(hcipher);

    return ret;
}

void param_config_key_type(cipher_test_info_t *test_info, const bool is_clear_key, const uint32_t key_len)
{
    test_info->is_clear_key = is_clear_key;
    test_info->key_len = key_len;
    test_info->key = g_aes_key;

    if (key_len == 16) { /* 16 bytes */
        if (test_info->crypto_alg == UAPI_DRV_KLAD_ENGINE_AES) {
            test_info->cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_128BIT;
        }
    } else { /* 32 bytes */
        if (test_info->crypto_alg == UAPI_DRV_KLAD_ENGINE_AES) {
            test_info->cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_256BIT;
        }
    }
}

static errcode_t aes_param_config(cipher_test_info_t *aes_test_info)
{
    if (aes_test_info->ctrl.work_mode != DRV_CIPHER_WORK_MODE_ECB) {
        memcpy_s(aes_test_info->cipher_config_t.aes_config.iv, DRV_CIPHER_AES_IV_LEN_IN_BYTES,
            aes_test_info->iv, aes_test_info->iv_len);
        print_buffer("memcpy result", aes_test_info->cipher_config_t.aes_config.iv,
            sizeof(aes_test_info->cipher_config_t.aes_config.iv));
    }
    aes_test_info->ctrl.param = &(aes_test_info->cipher_config_t.aes_config);
    return cipher_common(aes_test_info);
}

static errcode_t ccm_param_config(cipher_test_info_t* ccm_test_info)
{
    memcpy_s(ccm_test_info->cipher_config_t.aes_ccm_config.iv_ctr,
             sizeof(ccm_test_info->cipher_config_t.aes_ccm_config.iv_ctr), ccm_test_info->iv, ccm_test_info->iv_len);
    ccm_test_info->cipher_config_t.aes_ccm_config.iv_len = ccm_test_info->iv_len;
    ccm_test_info->cipher_config_t.aes_ccm_config.tag_len = ccm_test_info->expect_tag_len;
    ccm_test_info->cipher_config_t.aes_ccm_config.total_len = ccm_test_info->src_len;

    ccm_test_info->ctrl.param = &(ccm_test_info->cipher_config_t.aes_ccm_config);
    return cipher_gcm_ccm_common(ccm_test_info);
}

static errcode_t gcm_param_config(cipher_test_info_t* gcm_test_info)
{
    gcm_test_info->ctrl.param = &(gcm_test_info->cipher_config_t.aes_gcm_config);
    return cipher_gcm_ccm_common(gcm_test_info);
}

void cipher_param_config_input(cipher_test_info_t *test_info, const uint8_t *src, const uint32_t src_len,
                               const uint8_t *dst, const uint32_t dst_len)
{
    test_info->src.address = (uint8_t*)src;
    test_info->src_len = src_len;
    test_info->dst.address = (uint8_t*)dst;
    test_info->dst_len = dst_len;
}

int cipher_ecb_test_drv(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    PRINT("***************** aes ecb drv test ******************\n");
    errcode_t ret;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);
    static uint8_t aes_dst[32] = {0};

    static uint8_t aes_dst_128[] = {
        0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
        0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97,
        0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
        0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
    };

    static uint8_t aes_dst_256[] = {
        0x63, 0xB7, 0x8D, 0x44, 0xDF, 0xD4, 0xEF, 0x21,
        0xB1, 0x01, 0x5D, 0xBF, 0xB4, 0xA0, 0xD3, 0x5F,
        0x63, 0xB7, 0x8D, 0x44, 0xDF, 0xD4, 0xEF, 0x21,
        0xB1, 0x01, 0x5D, 0xBF, 0xB4, 0xA0, 0xD3, 0x5F
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_128, sizeof(aes_dst_128));
    } else {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_256, sizeof(aes_dst_256));
    }

    cipher_test_info_t aes_ecb_test_info;
    (void)memcpy_s(&aes_ecb_test_info, sizeof(aes_ecb_test_info), &g_aes_test_info, sizeof(g_aes_test_info));

    aes_ecb_test_info.dst.address = aes_dst;
    aes_ecb_test_info.dst_len = sizeof(aes_dst);
    param_config_key_type(&aes_ecb_test_info, true, AES_KEY_128BIT);
    aes_ecb_test_info.ctrl.work_mode = DRV_CIPHER_WORK_MODE_ECB;
    aes_ecb_test_info.key_len = key_len;
    if (key_len == AES_KEY_256BIT) {
        aes_ecb_test_info.cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    ret = aes_param_config(&aes_ecb_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int cipher_cbc_test_drv(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    PRINT("***************** aes cbc drv test ******************\n");
    errcode_t ret;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);
    static uint8_t aes_dst[32] = {0};

    static uint8_t aes_dst_128[] = {
        0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46,
        0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D,
        0x4C, 0xBB, 0xC8, 0x58, 0x75, 0x6B, 0x35, 0x81,
        0x25, 0x52, 0x9E, 0x96, 0x98, 0xA3, 0x8F, 0x44
    };

    static uint8_t aes_dst_256[] = {
        0xC3, 0xBE, 0x4C, 0xEF, 0x2C, 0x67, 0xBC, 0x8B,
        0x4B, 0x91, 0x2F, 0x85, 0x1C, 0xBD, 0x09, 0x8F,
        0xF3, 0xCF, 0x17, 0x91, 0x4F, 0x12, 0x34, 0x49,
        0xB7, 0xEE, 0xEF, 0x47, 0x81, 0x6F, 0x24, 0x92
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_128, sizeof(aes_dst_128));
    } else {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_256, sizeof(aes_dst_256));
    }

    cipher_test_info_t aes_cbc_test_info;
    (void)memcpy_s(&aes_cbc_test_info, sizeof(aes_cbc_test_info), &g_aes_test_info, sizeof(g_aes_test_info));

    aes_cbc_test_info.dst.address = aes_dst;
    aes_cbc_test_info.dst_len = sizeof(aes_dst);
    aes_cbc_test_info.key_len = key_len;
    if (key_len == AES_KEY_256BIT) {
        aes_cbc_test_info.cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    ret = aes_param_config(&aes_cbc_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int cipher_ctr_test_drv(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    PRINT("***************** aes ctr drv test ******************\n");
    errcode_t ret;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);
    static uint8_t aes_dst[32] = {0};

    static uint8_t aes_dst_128[] = {
        0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20,
        0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
        0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C,
        0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75
    };

    static uint8_t aes_dst_256[] = {
        0x15, 0x40, 0xA2, 0x4A, 0x82, 0x09, 0x13, 0x9F,
        0x13, 0x8B, 0x52, 0x6D, 0x9F, 0x36, 0xF0, 0x74,
        0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8,
        0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_128, sizeof(aes_dst_128));
    } else {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_256, sizeof(aes_dst_256));
    }

    cipher_test_info_t aes_ctr_test_info;
    memcpy_s(&aes_ctr_test_info, sizeof(aes_ctr_test_info), &g_aes_test_info, sizeof(g_aes_test_info));

    aes_ctr_test_info.dst.address = aes_dst;
    aes_ctr_test_info.dst_len = sizeof(aes_dst);
    aes_ctr_test_info.ctrl.work_mode = DRV_CIPHER_WORK_MODE_CTR;
    aes_ctr_test_info.key_len = key_len;
    if (key_len == AES_KEY_256BIT) {
        aes_ctr_test_info.cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    ret = aes_param_config(&aes_ctr_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int cipher_ofb_test_drv(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    PRINT("***************** aes ofb drv test ******************\n");
    errcode_t ret;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);
    static uint8_t aes_dst[32] = {0};

    static uint8_t aes_dst_128[] = {
        0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20,
        0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
        0xB2, 0x65, 0x64, 0x38, 0x26, 0xD2, 0xBC, 0x09,
        0x82, 0xB6, 0x43, 0x67, 0xF3, 0x72, 0x41, 0x5E,
    };

    static uint8_t aes_dst_256[] = {
        0x15, 0x40, 0xA2, 0x4A, 0x82, 0x09, 0x13, 0x9F,
        0x13, 0x8B, 0x52, 0x6D, 0x9F, 0x36, 0xF0, 0x74,
        0x21, 0x7B, 0xE2, 0x46, 0x98, 0xDB, 0x55, 0xC0,
        0x53, 0xB3, 0x22, 0x8B, 0xE2, 0xA0, 0x78, 0x8F
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_128, sizeof(aes_dst_128));
    } else {
        (void)memcpy_s(aes_dst, sizeof(aes_dst), aes_dst_256, sizeof(aes_dst_256));
    }

    cipher_test_info_t aes_ofb_test_info;
    memcpy_s(&aes_ofb_test_info, sizeof(aes_ofb_test_info), &g_aes_test_info, sizeof(g_aes_test_info));

    aes_ofb_test_info.dst.address = aes_dst;
    aes_ofb_test_info.dst_len = sizeof(aes_dst);
    param_config_key_type(&aes_ofb_test_info, true, AES_KEY_128BIT);
    aes_ofb_test_info.ctrl.work_mode = DRV_CIPHER_WORK_MODE_OFB;
    aes_ofb_test_info.key_len = key_len;
    if (key_len == AES_KEY_256BIT) {
        aes_ofb_test_info.cipher_config_t.aes_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    ret =  aes_param_config(&aes_ofb_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int cipher_ccm_test_drv(int argc, char *argv[])
{
    UNUSED(argc);

    PRINT("***************** aes ccm drv test ******************\n");
    errcode_t ret;
    cipher_test_info_t aes_ccm_test_info;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);

    static uint8_t aes_ccm_dst[32] = {0};
    static uint8_t expect_tag[16] = {0};

    static uint8_t aes_ccm_dst_128[] = {
        0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C,
        0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75,
        0xFD, 0x2B, 0x31, 0x6B, 0x2B, 0xC3, 0x0F, 0xD4,
        0x50, 0xF3, 0xFE, 0x89, 0x7F, 0x1E, 0x31, 0x1A
    };

    static uint8_t aes_ccm_dst_256[] = {
        0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8,
        0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9,
        0x03, 0x4C, 0xD6, 0xF3, 0xD1, 0x65, 0x91, 0xC9,
        0x30, 0xB5, 0xB2, 0x6A, 0xC7, 0x6E, 0x53, 0x64
    };

    static uint8_t expect_tag_16[] = {
        0x41, 0x1E, 0xCA, 0x70, 0x0B, 0x5C, 0xD3, 0x8B,
        0x02, 0x4A, 0x59, 0x80, 0x42, 0x5C, 0xF8, 0xAC
    };

    static uint8_t expect_tag_32[] = {
        0xC8, 0xC1, 0x0B, 0x37, 0x3E, 0xD8, 0xA9, 0xA5,
        0x63, 0xF5, 0xE8, 0x65, 0x35, 0xD5, 0x10, 0x7B
    };

    static uint8_t aad[] = {
        0x00, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_ccm_dst, sizeof(aes_ccm_dst), aes_ccm_dst_128, sizeof(aes_ccm_dst_128));
        (void)memcpy_s(expect_tag, sizeof(expect_tag), expect_tag_16, sizeof(expect_tag_16));
    } else {
        (void)memcpy_s(aes_ccm_dst, sizeof(aes_ccm_dst), aes_ccm_dst_256, sizeof(aes_ccm_dst_256));
        (void)memcpy_s(expect_tag, sizeof(expect_tag), expect_tag_32, sizeof(expect_tag_32));
    }

    memcpy_s(&aes_ccm_test_info, sizeof(aes_ccm_test_info), &g_aes_ccm_test_info, sizeof(g_aes_ccm_test_info));

    cipher_param_config_input(&aes_ccm_test_info, g_aes_src, sizeof(g_aes_src), aes_ccm_dst, sizeof(aes_ccm_dst));
    aes_ccm_test_info.iv = g_aes_iv;
    aes_ccm_test_info.iv_len = sizeof(g_aes_iv);
    aes_ccm_test_info.expect_tag = expect_tag;
    aes_ccm_test_info.expect_tag_len = sizeof(expect_tag);
    aes_ccm_test_info.cipher_config_t.aes_ccm_config.aad = aad;
    aes_ccm_test_info.cipher_config_t.aes_ccm_config.aad_len = sizeof(aad);
    aes_ccm_test_info.key_len = key_len;
    if (aes_ccm_test_info.key_len == AES_KEY_256BIT) {
        aes_ccm_test_info.cipher_config_t.aes_ccm_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    print_buffer("key", aes_ccm_test_info.key, aes_ccm_test_info.key_len);

    ret = ccm_param_config(&aes_ccm_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int cipher_gcm_test_drv(int argc, char *argv[])
{
    UNUSED(argc);

    PRINT("***************** aes gcm drv test ******************\n");
    errcode_t ret;
    cipher_test_info_t aes_gcm_test_info;
    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == AES_KEY_256BIT) ? AES_KEY_256BIT : AES_KEY_128BIT;
    PRINT("key len: %d\n", key_len);
    static uint8_t aes_gcm_dst[32] = {0};
    static uint8_t expect_tag[16] = {0};

    static uint8_t aes_gcm_dst_128[] = {
        0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C,
        0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75,
        0xFD, 0x2B, 0x31, 0x6B, 0x2B, 0xC3, 0x0F, 0xD4,
        0x50, 0xF3, 0xFE, 0x89, 0x7F, 0x1E, 0x31, 0x1A
    };

    static uint8_t aes_gcm_dst_256[] = {
        0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8,
        0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9,
        0x03, 0x4C, 0xD6, 0xF3, 0xD1, 0x65, 0x91, 0xC9,
        0x30, 0xB5, 0xB2, 0x6A, 0xC7, 0x6E, 0x53, 0x64
    };

    static uint8_t expect_tag_16[] = {
        0x63, 0xE8, 0x86, 0x40, 0x57, 0xCE, 0xF3, 0x9B,
        0xC1, 0x41, 0x95, 0x25, 0xC2, 0x78, 0x02, 0x74
    };

    static uint8_t expect_tag_32[] = {
        0xF6, 0xFB, 0x4B, 0xBC, 0xFA, 0x74, 0xD2, 0xCD,
        0x24, 0x56, 0x27, 0xFA, 0x7E, 0x5B, 0xED, 0x84
    };

    static uint8_t aad[] = {
        0x00, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
    };

    if (key_len == AES_KEY_128BIT) {
        (void)memcpy_s(aes_gcm_dst, sizeof(aes_gcm_dst), aes_gcm_dst_128, sizeof(aes_gcm_dst_128));
        (void)memcpy_s(expect_tag, sizeof(expect_tag), expect_tag_16, sizeof(expect_tag_16));
    } else {
        (void)memcpy_s(aes_gcm_dst, sizeof(aes_gcm_dst), aes_gcm_dst_256, sizeof(aes_gcm_dst_256));
        (void)memcpy_s(expect_tag, sizeof(expect_tag), expect_tag_32, sizeof(expect_tag_32));
    }

    memcpy_s(&aes_gcm_test_info, sizeof(aes_gcm_test_info), &g_aes_gcm_test_info, sizeof(g_aes_gcm_test_info));

    cipher_param_config_input(&aes_gcm_test_info, g_aes_src, sizeof(g_aes_src), aes_gcm_dst, sizeof(aes_gcm_dst));
    aes_gcm_test_info.expect_tag = expect_tag;
    aes_gcm_test_info.expect_tag_len = sizeof(expect_tag);
    aes_gcm_test_info.cipher_config_t.aes_gcm_config.iv = g_aes_iv;
    aes_gcm_test_info.cipher_config_t.aes_gcm_config.iv_len = sizeof(g_aes_iv);
    aes_gcm_test_info.cipher_config_t.aes_gcm_config.aad = aad;
    aes_gcm_test_info.cipher_config_t.aes_gcm_config.aad_len = sizeof(aad);
    aes_gcm_test_info.key_len = key_len;
    if (aes_gcm_test_info.key_len == AES_KEY_256BIT) {
        aes_gcm_test_info.cipher_config_t.aes_gcm_config.key_len = DRV_CIPHER_KEY_256BIT;
    }

    print_buffer("key", aes_gcm_test_info.key, aes_gcm_test_info.key_len);

    ret = gcm_param_config(&aes_gcm_test_info);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

void add_cipher_test_case_drv(void)
{
    uapi_test_suite_add_function("cipher_ecb_test_drv", "cipher ecb drv test.", cipher_ecb_test_drv);
    uapi_test_suite_add_function("cipher_cbc_test_drv", "cipher cbc drv test.", cipher_cbc_test_drv);
    uapi_test_suite_add_function("cipher_ofb_test_drv", "cipher ofb drv test.", cipher_ofb_test_drv);
    uapi_test_suite_add_function("cipher_ctr_test_drv", "cipher ctr drv test.", cipher_ctr_test_drv);
    uapi_test_suite_add_function("cipher_gcm_test_drv", "cipher gcm drv test.", cipher_gcm_test_drv);
    uapi_test_suite_add_function("cipher_ccm_test_drv", "cipher ccm drv test.", cipher_ccm_test_drv);
}