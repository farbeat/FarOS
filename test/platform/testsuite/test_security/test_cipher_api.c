/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 * Description: Test cipher source \n
 * Author: @CompanyNameTag \n
 * Date: 2023-07-10 \n
 */

#include "test_cipher.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "km.h"
#include "cipher_api.h"
#include "securec.h"
#include "test_print.h"


#define TEST_AES_KEY_SIZE_128   16
#define TEST_AES_KEY_SIZE_256   32

#define TEST_DATA_LEN           32
#define TEST_TAG_LEN            16
#define TEST_ALIGNED_ADDR_SIZE  32

#ifndef INVALID_HANDLE
#define INVALID_HANDLE          0xFFFFFFFF
#endif


static uint8_t g_test_cipher_result[TEST_DATA_LEN] __attribute__((aligned(TEST_ALIGNED_ADDR_SIZE))) = {0};
static uint8_t g_test_cipher_data[TEST_DATA_LEN];
static uint8_t g_test_cipher_tag[TEST_TAG_LEN];


static uint8_t g_aes_key[] = {
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C,
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3D
};

static uint8_t g_aes_iv[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static uint8_t g_aes_src[] = {
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
};

static uint8_t g_aes_gcm_key[] = {
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08,
    0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08,
};

static uint8_t g_aes_ccm_key[] = {
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
};

static uint8_t g_aes_ecb_cipher_128[] = {
    0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97,
    0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
};

static uint8_t g_aes_ecb_cipher_256[] = {
    0x63, 0xB7, 0x8D, 0x44, 0xDF, 0xD4, 0xEF, 0x21, 0xB1, 0x01, 0x5D, 0xBF, 0xB4, 0xA0, 0xD3, 0x5F,
    0x63, 0xB7, 0x8D, 0x44, 0xDF, 0xD4, 0xEF, 0x21, 0xB1, 0x01, 0x5D, 0xBF, 0xB4, 0xA0, 0xD3, 0x5F
};

static uint8_t g_aes_cbc_cipher_128[] = {
    0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46, 0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D,
    0x4C, 0xBB, 0xC8, 0x58, 0x75, 0x6B, 0x35, 0x81, 0x25, 0x52, 0x9E, 0x96, 0x98, 0xA3, 0x8F, 0x44
};

static uint8_t g_aes_cbc_cipher_256[] = {
    0xC3, 0xBE, 0x4C, 0xEF, 0x2C, 0x67, 0xBC, 0x8B, 0x4B, 0x91, 0x2F, 0x85, 0x1C, 0xBD, 0x09, 0x8F,
    0xF3, 0xCF, 0x17, 0x91, 0x4F, 0x12, 0x34, 0x49, 0xB7, 0xEE, 0xEF, 0x47, 0x81, 0x6F, 0x24, 0x92
};

static uint8_t g_aes_ctr_cipher_128[] = {
    0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20, 0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
    0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C, 0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75
};

static uint8_t g_aes_ctr_cipher_256[] = {
    0x15, 0x40, 0xA2, 0x4A, 0x82, 0x09, 0x13, 0x9F, 0x13, 0x8B, 0x52, 0x6D, 0x9F, 0x36, 0xF0, 0x74,
    0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8, 0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9
};

static uint8_t g_aes_ofb_cipher_128[] = {
    0x3B, 0x3F, 0xD9, 0x2E, 0xB7, 0x2D, 0xAD, 0x20, 0x33, 0x34, 0x49, 0xF8, 0xE8, 0x3C, 0xFB, 0x4A,
    0xB2, 0x65, 0x64, 0x38, 0x26, 0xD2, 0xBC, 0x09, 0x82, 0xB6, 0x43, 0x67, 0xF3, 0x72, 0x41, 0x5E,
};

static uint8_t g_aes_ofb_cipher_256[] = {
    0x15, 0x40, 0xA2, 0x4A, 0x82, 0x09, 0x13, 0x9F, 0x13, 0x8B, 0x52, 0x6D, 0x9F, 0x36, 0xF0, 0x74,
    0x21, 0x7B, 0xE2, 0x46, 0x98, 0xDB, 0x55, 0xC0, 0x53, 0xB3, 0x22, 0x8B, 0xE2, 0xA0, 0x78, 0x8F
};

static uint8_t g_aes_gcm_cipher_128[] = {
    0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C, 0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75,
    0xFD, 0x2B, 0x31, 0x6B, 0x2B, 0xC3, 0x0F, 0xD4, 0x50, 0xF3, 0xFE, 0x89, 0x7F, 0x1E, 0x31, 0x1A
};

static uint8_t g_aes_gcm_cipher_256[] = {
    0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8, 0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9,
    0x03, 0x4C, 0xD6, 0xF3, 0xD1, 0x65, 0x91, 0xC9, 0x30, 0xB5, 0xB2, 0x6A, 0xC7, 0x6E, 0x53, 0x64
};

static uint8_t g_aes_gcm_tag_128[] = {
    0x63, 0xE8, 0x86, 0x40, 0x57, 0xCE, 0xF3, 0x9B, 0xC1, 0x41, 0x95, 0x25, 0xC2, 0x78, 0x02, 0x74
};

static uint8_t g_aes_gcm_tag_256[] = {
    0xF6, 0xFB, 0x4B, 0xBC, 0xFA, 0x74, 0xD2, 0xCD, 0x24, 0x56, 0x27, 0xFA, 0x7E, 0x5B, 0xED, 0x84
};

static uint8_t g_aes_ccm_cipher_128[] = {
    0xC4, 0xE0, 0x30, 0xAC, 0xA9, 0xA3, 0x0C, 0x3C, 0x33, 0x0C, 0x35, 0xF5, 0x08, 0x64, 0xB4, 0x75,
    0xFD, 0x2B, 0x31, 0x6B, 0x2B, 0xC3, 0x0F, 0xD4, 0x50, 0xF3, 0xFE, 0x89, 0x7F, 0x1E, 0x31, 0x1A
};

static uint8_t g_aes_ccm_cipher_256[] = {
    0x42, 0x09, 0xB5, 0x9D, 0x26, 0x52, 0xCF, 0xA8, 0x24, 0x75, 0xBF, 0x77, 0x60, 0x63, 0x7C, 0xE9,
    0x03, 0x4C, 0xD6, 0xF3, 0xD1, 0x65, 0x91, 0xC9, 0x30, 0xB5, 0xB2, 0x6A, 0xC7, 0x6E, 0x53, 0x64
};

static uint8_t g_aes_ccm_tag_128[] = {
    0x41, 0x1E, 0xCA, 0x70, 0x0B, 0x5C, 0xD3, 0x8B, 0x02, 0x4A, 0x59, 0x80, 0x42, 0x5C, 0xF8, 0xAC
};

static uint8_t g_aes_ccm_tag_256[] = {
    0xC8, 0xC1, 0x0B, 0x37, 0x3E, 0xD8, 0xA9, 0xA5, 0x63, 0xF5, 0xE8, 0x65, 0x35, 0xD5, 0x10, 0x7B
};

static uint8_t g_aes_ccm_gcm_aad[] = {
    0x00, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
};


typedef struct test_cipher_info {
    uapi_drv_cipher_buf_attr_t src;
    uint32_t src_len;
    uapi_drv_cipher_buf_attr_t dst;
    uint32_t dst_len;
    uint8_t  *iv;
    uint32_t iv_len;
    uint8_t  *key;
    uint32_t key_len;
    uint8_t  *expected_tag;
    uint32_t expected_tag_len;
    uapi_drv_klad_engine_t crypto_alg;
    uapi_drv_cipher_symc_config_aes_ccm_gcm_t ccm_gcm;
    uapi_drv_cipher_symc_attr_t attr;
    uapi_drv_cipher_symc_ctrl_t ctrl;
    bool is_clear_key;
} test_cipher_info_t;


static test_cipher_info_t g_test_aes_info = {
    .src = {
        .phys_addr = (uintptr_t)g_aes_src,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
    },
    .src_len = sizeof(g_aes_src),
    .dst = {
        .phys_addr = (uintptr_t)NULL,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
    },
    .iv = g_aes_iv,
    .iv_len = sizeof(g_aes_iv),
    .key = g_aes_key,
    .key_len = TEST_AES_KEY_SIZE_128,
    .expected_tag = NULL,
    .expected_tag_len = 0,
    .crypto_alg = UAPI_DRV_KLAD_ENGINE_AES,
    .attr = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC,
        .symc_type = UAPI_DRV_CIPHER_SYMC_TYPE_NORMAL,
        .is_long_term = true,
    },
    .ctrl = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC,
        .symc_key_length = UAPI_DRV_CIPHER_SYMC_KEY_128BIT,
        .key_parity = UAPI_DRV_CIPHER_SYMC_KEY_EVEN,
        .symc_bit_width = UAPI_DRV_CIPHER_SYMC_BIT_WIDTH_128BIT,
        .iv_change_flag = UAPI_DRV_CIPHER_SYMC_IV_CHANGE_ONE_PKG,
    },
    .is_clear_key = true,
};

static test_cipher_info_t g_test_aes_ccm_gcm_info = {
    .src = {
        .phys_addr = (uintptr_t)NULL,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
    },
    .src_len = 0,
    .dst = {
        .phys_addr = (uintptr_t)NULL,
        .buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
    },
    .dst_len = 0,
    .iv = NULL,
    .iv_len = 0,
    .key = g_aes_key,
    .key_len = TEST_AES_KEY_SIZE_128,
    .expected_tag = NULL,
    .expected_tag_len = 0,
    .crypto_alg = UAPI_DRV_KLAD_ENGINE_AES,
    .ccm_gcm = {
        .aad_buf.phys_addr = (uintptr_t)NULL,
        .aad_buf.buf_sec = UAPI_DRV_CIPHER_BUF_NONSECURE,
        .aad_len = 0,
        .data_len = 0,
        .tag_len = 0,
    },
    .attr = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_GCM,
        .symc_type = UAPI_DRV_CIPHER_SYMC_TYPE_NORMAL,
        .is_long_term = true,
    },
    .ctrl = {
        .symc_alg = UAPI_DRV_CIPHER_SYMC_ALG_AES,
        .work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_GCM,
        .symc_key_length = UAPI_DRV_CIPHER_SYMC_KEY_128BIT,
        .key_parity = UAPI_DRV_CIPHER_SYMC_KEY_EVEN,
        .symc_bit_width = UAPI_DRV_CIPHER_SYMC_BIT_WIDTH_128BIT,
        .iv_change_flag = UAPI_DRV_CIPHER_SYMC_GCM_IV_CHANGE_START,
    },
};


static errcode_t test_cipher_set_clear_key(uint32_t keyslot, uint8_t *key, uint32_t key_len,
    bool key_parity, uapi_drv_klad_engine_t engine)
{
    uapi_drv_klad_attr_t attr = {0};
    attr.key_cfg.engine = engine;
    attr.key_cfg.decrypt_support = true;
    attr.key_cfg.encrypt_support = true;

    uapi_drv_klad_clear_key_t clear_key;
    clear_key.key_length = key_len;
    clear_key.key = key;
    clear_key.key_parity = key_parity;

    uint32_t klad;
    errcode_t ret = uapi_drv_klad_create(&klad);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        return ret;
    }

    ret = uapi_drv_klad_attach(klad, UAPI_DRV_KLAD_DEST_MCIPHER, keyslot);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup;
    }

    ret = uapi_drv_klad_set_attr(klad, &attr);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup;
    }

    ret = uapi_drv_klad_set_clear_key(klad, &clear_key);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup;
    }

    ret = uapi_drv_klad_detach(klad, UAPI_DRV_KLAD_DEST_MCIPHER, keyslot);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup;
    }

    ret = uapi_drv_klad_destroy(klad);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup;
    }

cleanup:
    (void)uapi_drv_klad_destroy(klad);
    return ret;
}

static errcode_t test_cipher_common(test_cipher_info_t *test_info)
{
    uapi_drv_cipher_buf_attr_t src_buf, dst_buf;
    src_buf.phys_addr = test_info->src.phys_addr;
    src_buf.buf_sec = test_info->src.buf_sec;
    dst_buf.phys_addr = (uintptr_t)g_test_cipher_result;
    dst_buf.buf_sec = test_info->dst.buf_sec;

    uint32_t length = test_info->src_len;
    uint32_t cipher = INVALID_HANDLE;
    errcode_t ret = uapi_drv_cipher_symc_create(&cipher, &test_info->attr);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        return ret;
    }

    uint32_t keyslot = INVALID_HANDLE;
    ret = uapi_drv_keyslot_create(&keyslot, UAPI_DRV_KEYSLOT_TYPE_MCIPHER);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup2;
    }

    ret = uapi_drv_cipher_symc_attach(cipher, keyslot);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup1;
    }

    ret = uapi_drv_cipher_symc_set_config(cipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    ret = test_cipher_set_clear_key(keyslot, test_info->key, test_info->key_len, false, test_info->crypto_alg);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("encrypt src", (const uint8_t *)src_buf.phys_addr, length);

    ret = uapi_drv_cipher_symc_encrypt(cipher, &src_buf, &dst_buf, length);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("encrypt result", g_test_cipher_result, length);
    if (test_info->dst.phys_addr != 0) {
        if (memcmp((const void *)test_info->dst.phys_addr, g_test_cipher_result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

    ret = uapi_drv_cipher_symc_set_config(cipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    src_buf.phys_addr = test_info->dst.phys_addr;
    src_buf.buf_sec = test_info->dst.buf_sec;
    dst_buf.phys_addr = (uintptr_t)g_test_cipher_result;
    dst_buf.buf_sec = test_info->src.buf_sec;

    print_buffer("decrypt src", (const uint8_t *)src_buf.phys_addr, length);

    ret = uapi_drv_cipher_symc_decrypt(cipher, &src_buf, &dst_buf, length);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("decrypt result", g_test_cipher_result, length);
    print_buffer("src data", (const uint8_t *)test_info->src.phys_addr, length);

    if (test_info->src.phys_addr != 0) {
        if (memcmp((const void *)test_info->src.phys_addr, g_test_cipher_result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

cleanup0:
    (void)uapi_drv_cipher_symc_detach(cipher, keyslot);
cleanup1:
    (void)uapi_drv_keyslot_destroy(keyslot);
cleanup2:
    (void)uapi_drv_cipher_symc_destroy(cipher);

    return ret;
}

static void test_param_config_key_type(test_cipher_info_t *test_info, bool is_clear_key, uint32_t key_len)
{
    test_info->is_clear_key = is_clear_key;
    test_info->key_len = key_len;
    test_info->key = g_aes_key;

    if (test_info->crypto_alg == UAPI_DRV_KLAD_ENGINE_AES) {
        test_info->ctrl.symc_key_length = (key_len == TEST_AES_KEY_SIZE_128) ?
            UAPI_DRV_CIPHER_SYMC_KEY_128BIT : UAPI_DRV_CIPHER_SYMC_KEY_256BIT;
    }
}

static errcode_t test_aes_param_config(test_cipher_info_t *test_info)
{
    if (test_info->ctrl.work_mode != UAPI_DRV_CIPHER_SYMC_WORK_MODE_ECB) {
        (void)memcpy_s(test_info->ctrl.iv, sizeof(test_info->ctrl.iv), test_info->iv, test_info->iv_len);
        test_info->ctrl.iv_length = test_info->iv_len;
        print_buffer("iv", test_info->ctrl.iv, sizeof(test_info->ctrl.iv));
    }

    return test_cipher_common(test_info);
}

static errcode_t test_ccm_gcm_common(test_cipher_info_t *test_info)
{
    uint8_t tag[TEST_TAG_LEN] = {0};
    uint32_t tag_len = test_info->expected_tag_len;

    uapi_drv_cipher_buf_attr_t src_buf, dst_buf;
    src_buf.phys_addr = test_info->src.phys_addr;
    src_buf.buf_sec = test_info->src.buf_sec;
    dst_buf.phys_addr = (uintptr_t)g_test_cipher_result;
    dst_buf.buf_sec = test_info->dst.buf_sec;

    uint32_t length = test_info->src_len;
    uint32_t cipher = INVALID_HANDLE;
    errcode_t ret = uapi_drv_cipher_symc_create(&cipher, &test_info->attr);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        return ret;
    }

    uint32_t keyslot = INVALID_HANDLE;
    ret = uapi_drv_keyslot_create(&keyslot, UAPI_DRV_KEYSLOT_TYPE_MCIPHER);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup2;
    }

    ret = uapi_drv_cipher_symc_attach(cipher, keyslot);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup1;
    }

    ret = uapi_drv_cipher_symc_set_config(cipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    ret = test_cipher_set_clear_key(keyslot, test_info->key, test_info->key_len, false, test_info->crypto_alg);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("encrypt src", (const uint8_t *)src_buf.phys_addr, length);

    ret = uapi_drv_cipher_symc_encrypt(cipher, &src_buf, &dst_buf, length);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("encrypt result", g_test_cipher_result, length);
    print_buffer("expected result", (const uint8_t *)test_info->dst.phys_addr, length);

    ret = uapi_drv_cipher_symc_get_tag(cipher, tag, tag_len);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("tag", tag, tag_len);
    print_buffer("expect tag", test_info->expected_tag, tag_len);

    if (test_info->dst.phys_addr != 0) {
        if (memcmp((const void *)test_info->dst.phys_addr, g_test_cipher_result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

    if (memcmp(tag, test_info->expected_tag, tag_len) != 0) {
        ret = ERRCODE_FAIL;
        goto cleanup0;
    }

    ret = uapi_drv_cipher_symc_set_config(cipher, &test_info->ctrl);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    src_buf.phys_addr = test_info->dst.phys_addr;
    src_buf.buf_sec = test_info->dst.buf_sec;
    dst_buf.phys_addr = (uintptr_t)g_test_cipher_result;
    dst_buf.buf_sec = test_info->src.buf_sec;

    print_buffer("decrypt src", (const uint8_t *)src_buf.phys_addr, length);

    ret = uapi_drv_cipher_symc_decrypt(cipher, &src_buf, &dst_buf, length);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("decrypt result", g_test_cipher_result, length);
    print_buffer("src data", (const uint8_t *)test_info->src.phys_addr, length);

    (void)memset_s(tag, tag_len, 0, tag_len);
    ret = uapi_drv_cipher_symc_get_tag(cipher, tag, tag_len);
    if (ret != ERRCODE_SUCC) {
        PRINT("[ERR] %d, %s (%d)\n", ret, __FUNCTION__, __LINE__);
        goto cleanup0;
    }

    print_buffer("tag", tag, tag_len);
    print_buffer("expect tag", test_info->expected_tag, tag_len);

    if (test_info->src.phys_addr != 0) {
        if (memcmp((const void *)test_info->src.phys_addr, g_test_cipher_result, length) != 0) {
            ret = ERRCODE_FAIL;
            goto cleanup0;
        }
    }

cleanup0:
    (void)uapi_drv_cipher_symc_detach(cipher, keyslot);
cleanup1:
    (void)uapi_drv_keyslot_destroy(keyslot);
cleanup2:
    (void)uapi_drv_cipher_symc_destroy(cipher);

    return ret;
}

static errcode_t test_ccm_gcm_param_config(test_cipher_info_t *test_info)
{
    (void)memcpy_s(test_info->ctrl.iv, sizeof(test_info->ctrl.iv), test_info->iv, test_info->iv_len);
    test_info->ctrl.iv_length = test_info->iv_len;
    print_buffer("iv", test_info->ctrl.iv, sizeof(test_info->ctrl.iv));

    return test_ccm_gcm_common(test_info);
}

static int cipher_ecb_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes ecb test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ecb_cipher_128, sizeof(g_aes_ecb_cipher_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ecb_cipher_256, sizeof(g_aes_ecb_cipher_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_info, sizeof(g_test_aes_info));

    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.dst_len = sizeof(g_test_aes_info);
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_ECB;

    if (test_aes_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int cipher_cbc_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes cbc test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_cbc_cipher_128, sizeof(g_aes_cbc_cipher_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_cbc_cipher_256, sizeof(g_aes_cbc_cipher_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_info, sizeof(g_test_aes_info));

    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.dst_len = sizeof(g_test_aes_info);
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CBC;

    if (test_aes_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int cipher_ctr_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes ctr test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ctr_cipher_128, sizeof(g_aes_ctr_cipher_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ctr_cipher_256, sizeof(g_aes_ctr_cipher_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_info, sizeof(g_test_aes_info));

    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.dst_len = sizeof(g_test_aes_info);
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CTR;

    if (test_aes_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int cipher_ofb_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes ofb test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ofb_cipher_128, sizeof(g_aes_ofb_cipher_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ofb_cipher_256, sizeof(g_aes_ofb_cipher_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_info, sizeof(g_test_aes_info));

    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.dst_len = sizeof(g_test_aes_info);
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_OFB;

    if (test_aes_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int cipher_gcm_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes gcm test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_gcm_cipher_128, sizeof(g_aes_gcm_cipher_128));
        (void)memcpy_s(g_test_cipher_tag, sizeof(g_test_cipher_tag),
            g_aes_gcm_tag_128, sizeof(g_aes_gcm_tag_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_gcm_cipher_256, sizeof(g_aes_gcm_cipher_256));
        (void)memcpy_s(g_test_cipher_tag, sizeof(g_test_cipher_tag),
            g_aes_gcm_tag_256, sizeof(g_aes_gcm_tag_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_ccm_gcm_info, sizeof(g_test_aes_ccm_gcm_info));
    test_info.src.phys_addr = (uintptr_t)g_aes_src;
    test_info.src_len = sizeof(g_aes_src);
    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.src_len = sizeof(g_test_cipher_data);
    test_info.expected_tag = g_test_cipher_tag;
    test_info.expected_tag_len = sizeof(g_test_cipher_tag);
    test_info.iv = g_aes_iv;
    test_info.iv_len = sizeof(g_aes_iv);
    test_info.ccm_gcm.aad_buf.phys_addr = (uintptr_t)g_aes_ccm_gcm_aad;
    test_info.ccm_gcm.aad_len = sizeof(g_aes_ccm_gcm_aad);
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_GCM;
    test_info.ctrl.param = (void *)&(test_info.ccm_gcm);

    print_buffer("key", test_info.key, test_info.key_len);

    if (test_ccm_gcm_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

static int cipher_ccm_test(int argc, char *argv[])
{
    unused(argc);

    PRINT("******************** aes ccm test *********************\n");

    uint32_t key_len = (uint32_t)strtol(argv[0], NULL, 0);
    key_len = (key_len == TEST_AES_KEY_SIZE_256) ? TEST_AES_KEY_SIZE_256 : TEST_AES_KEY_SIZE_128;

    PRINT("key len: %d\n", key_len);

    if (key_len == TEST_AES_KEY_SIZE_128) {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ccm_cipher_128, sizeof(g_aes_ccm_cipher_128));
        (void)memcpy_s(g_test_cipher_tag, sizeof(g_test_cipher_tag),
            g_aes_ccm_tag_128, sizeof(g_aes_ccm_tag_128));
    } else {
        (void)memcpy_s(g_test_cipher_data, sizeof(g_test_cipher_data),
            g_aes_ccm_cipher_256, sizeof(g_aes_ccm_cipher_256));
        (void)memcpy_s(g_test_cipher_tag, sizeof(g_test_cipher_tag),
            g_aes_ccm_tag_256, sizeof(g_aes_ccm_tag_256));
    }

    test_cipher_info_t test_info;
    (void)memcpy_s(&test_info, sizeof(test_info), &g_test_aes_ccm_gcm_info, sizeof(g_test_aes_ccm_gcm_info));
    test_info.src.phys_addr = (uintptr_t)g_aes_src;
    test_info.src_len = sizeof(g_aes_src);
    test_info.dst.phys_addr = (uintptr_t)g_test_cipher_data;
    test_info.src_len = sizeof(g_test_cipher_data);
    test_info.expected_tag = g_test_cipher_tag;
    test_info.expected_tag_len = sizeof(g_test_cipher_tag);
    test_info.iv = g_aes_iv;
    test_info.iv_len = sizeof(g_aes_iv);
    test_info.ccm_gcm.aad_buf.phys_addr = (uintptr_t)g_aes_ccm_gcm_aad;
    test_info.ccm_gcm.aad_len = sizeof(g_aes_ccm_gcm_aad);
    test_info.ccm_gcm.tag_len = test_info.expected_tag_len;
    test_info.ccm_gcm.data_len = test_info.src_len;
    test_param_config_key_type(&test_info, true, key_len);
    test_info.ctrl.work_mode = UAPI_DRV_CIPHER_SYMC_WORK_MODE_CCM;
    test_info.ctrl.param = (void *)&(test_info.ccm_gcm);

    print_buffer("key", test_info.key, test_info.key_len);

    if (test_ccm_gcm_param_config(&test_info) != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

void add_cipher_test_case(void)
{
    uapi_test_suite_add_function("cipher_ecb_test", "cipher ecb test.", cipher_ecb_test);
    uapi_test_suite_add_function("cipher_cbc_test", "cipher cbc test.", cipher_cbc_test);
    uapi_test_suite_add_function("cipher_ctr_test", "cipher ctr test.", cipher_ctr_test);
    uapi_test_suite_add_function("cipher_ofb_test", "cipher ofb test.", cipher_ofb_test);
    uapi_test_suite_add_function("cipher_gcm_test", "cipher gcm test.", cipher_gcm_test);
    uapi_test_suite_add_function("cipher_ccm_test", "cipher ccm test.", cipher_ccm_test);
}
