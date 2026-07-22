/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  ROM SEC BOOT MODULE
 * Author:
 * Create: 2021-03-26
 */
#include "sec_boot_public.h"
#include "sha256.h"
#include "flash.h"
#include "debug_print.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "chip_io.h"
#include "securec.h"
#include "non_os.h"

#define SEC_BOOT_SHIFT_24      24
#define SEC_BOOT_SHIFT_8       8
#define RSA_PUB_KEY_E_OFFSET   (RSA4096_PUBKEY_LENGTH - 4)
#define RSA_PUB_KEY_E_MIN_VAL  3UL
#define PAL_OTP_MCU_SUBKEY_ID_MASK_CRC_LEN 1
#define PAL_OTP_MCU_SUBKEY_CATEGORY_CRC_LEN 1

typedef struct {
    uint32_t subkey_version;  //!< default to 1.
    uint32_t subkey_sign_alg; //!< [15:0] hash alg, 0x0：sha256.
                              //!< [21:16] sign alg, 0x0: RSA_PKCS1; 0x1：RSA_PSS; 0x10：ECC; others:reseverd.
                              //!< [31:22] sign alg param, if RSA_PKCS1：0; if RSA_PSS， the length of salt; if ECC, 0.
    uint32_t subkey_category;
    uint32_t subkey_id;       //!< [4-0] 0-31, id number; other 0.
    uint32_t subkey_len;      //!< if subkey is rsa4096, it's 512.
    uint32_t subkey_signlen;  //!< same as subkey_len.
    uint8_t subkey[RSA4096_PUBKEY_LENGTH]; //!< rsa pubkey (n, e), only support rsa4096.
    uint8_t subkey_sign[RSA4096_SIGN_LENGTH];
} subkey_sign_certificate_t;

static subkey_sign_certificate_t g_subkey_sign_cer __attribute__((aligned(8)));

uint32_t sec_boot_global_subkey_cert_get(void)
{
    return (uint32_t)(&g_subkey_sign_cer);
}

bool sec_boot_subkey_sign_certificate_subkeyid_verify(void)
{
    // 1. read subkeyid_mask in otp, and calculate it's crc and validate
    uint32_t otp_subkeyid_mask = 0xffff;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer((uint8_t *)(&otp_subkeyid_mask),
        PAL_OTP_MCU_SUBKEY_ID_MASK_START, sizeof(uint32_t)) != ERRCODE_SUCC) {
        return false;
    }

    uint8_t otp_crc = 0;
    if (uapi_efuse_read_buffer(&otp_crc, PAL_OTP_MCU_SUBKEY_ID_MASK_CRC,
        PAL_OTP_MCU_SUBKEY_ID_MASK_CRC_LEN) != ERRCODE_SUCC) {
        return false;
    }

    uint8_t calc_crc = 0;
    if (uapi_efuse_calc_crc((uint8_t *)&otp_subkeyid_mask, sizeof(uint32_t), &calc_crc) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (otp_read_buffer((uint8_t *)(&otp_subkeyid_mask),
                        PAL_OTP_MCU_SUBKEY_ID_MASK_START, sizeof(uint32_t)) != OTP_RET_SUCC) {
        return false;
    }

    uint8_t otp_crc = 0;
    if (otp_read_byte(&otp_crc, PAL_OTP_MCU_SUBKEY_ID_MASK_CRC) != OTP_RET_SUCC) {
        return false;
    }

    uint8_t calc_crc = 0;
    if (otp_calc_crc((uint8_t *)&otp_subkeyid_mask, sizeof(uint32_t), &calc_crc) != OTP_RET_SUCC) {
        return false;
    }
#endif
    if (otp_crc != calc_crc) {
        PRINT("crc not equal!!! otp_crc = 0x%x, calc_crc = 0x%x"NEWLINE, otp_crc, calc_crc);
        return false;
    }

    // 2. judge subkeyid is valid or not
    uint32_t subkeyid = g_subkey_sign_cer.subkey_id & 0x1f;
    if ((otp_subkeyid_mask & BIT(subkeyid)) != 0) {
        return false;
    }
    return true;
}

bool sec_boot_subkey_sign_certificate_category_verify(void)
{
    // 1. read subkeyid_mask in otp, and calculate it's crc and verify
    uint32_t otp_subkey_category = 0;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer((uint8_t *)(&otp_subkey_category), PAL_OTP_MCU_SUBKEY_CATEGORY_START,
        sizeof(uint32_t)) != ERRCODE_SUCC) {
        return false;
    }

    uint8_t otp_crc = 0;
    if (uapi_efuse_read_buffer(&otp_crc, PAL_OTP_MCU_SUBKEY_CATEGORY_CRC,
        PAL_OTP_MCU_SUBKEY_CATEGORY_CRC_LEN) != ERRCODE_SUCC) {
        return false;
    }

    uint8_t calc_crc = 0;
    if (uapi_efuse_calc_crc((uint8_t *)&otp_subkey_category, sizeof(uint32_t), &calc_crc) != ERRCODE_SUCC) {
        return false;
    }
#else
    if (otp_read_buffer((uint8_t *)(&otp_subkey_category), PAL_OTP_MCU_SUBKEY_CATEGORY_START,
        sizeof(uint32_t)) != OTP_RET_SUCC) {
        return false;
    }

    uint8_t otp_crc = 0;
    if (otp_read_byte(&otp_crc, PAL_OTP_MCU_SUBKEY_CATEGORY_CRC) != OTP_RET_SUCC) {
        return false;
    }

    uint8_t calc_crc = 0;
    if (otp_calc_crc((uint8_t *)&otp_subkey_category, sizeof(uint32_t), &calc_crc) != OTP_RET_SUCC) {
        return false;
    }
#endif
    if (otp_crc != calc_crc) {
        PRINT("crc not equal!!! otp_crc = 0x%x, calc_crc = 0x%x"NEWLINE, otp_crc, calc_crc);
        return false;
    }

    // 2. judge subkeyid is valid or not
    if (g_subkey_sign_cer.subkey_category != otp_subkey_category) {
        return false;
    }
    return true;
}

// rsa_pkcs1 v1.5 sign verify
static bool sec_boot_subkey_sign_rsapkcs1_verify(void)
{
    uint32_t sign_addr = sec_boot_global_rsa_sign_addr_get();
    uint32_t pubkey_addr = sec_boot_global_rsa_pubkey_addr_get();
    uint32_t result_addr = sec_boot_global_rsa_result_addr_get();

    errno_t sec_ret = memcpy_s((uint8_t *)(uintptr_t)sign_addr, RSA4096_SIGN_LENGTH,
                               g_subkey_sign_cer.subkey_sign, RSA4096_SIGN_LENGTH);
    if (sec_ret != EOK) {
        PRINT("memcpy error!!!"NEWLINE);
        return false;
    }

    if (!sec_boot_rsa_exp_mod(pubkey_addr, (uint8_t*)(uintptr_t)sign_addr, (uint8_t*)(uintptr_t)result_addr)) {
        PRINT("rsa exp mod fail!"NEWLINE);
        return false;
    }

    errcode_t ret;
    uint8_t sha_result[SHA256_HASH_SIZE] = { 0 };
    uint32_t src_len = sizeof(g_subkey_sign_cer) - RSA4096_SIGN_LENGTH;
    uint8_t *src_buf = (uint8_t*)(uintptr_t)(&g_subkey_sign_cer);

    ret = uapi_drv_cipher_sha256(src_buf, src_len, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    /* memcmp return 0 if str1 equals to str2 */
    if (memcmp((uint8_t *)(result_addr + RSA4096_RESULT_OFFSET), sha_result, SHA256_HASH_SIZE) != 0) {
        PRINT("subkey sign check fail!"NEWLINE);
        return false;
    }
    PRINT("subkey sign check success!"NEWLINE);
    return true;
}

static bool sec_boot_subkey_sign_rsapss_verify(uint32_t salt_length)
{
    uint32_t sign_addr = sec_boot_global_rsa_sign_addr_get();
    errno_t sec_ret = memcpy_s((uint8_t *)(uintptr_t)sign_addr, RSA4096_SIGN_LENGTH,
                               g_subkey_sign_cer.subkey_sign, RSA4096_SIGN_LENGTH);
    if (sec_ret != EOK) {
        PRINT("memcpy error!!!"NEWLINE);
        return false;
    }

    return sec_boot_rsa_pss_verify(salt_length, (uint8_t *)&g_subkey_sign_cer,
                                   sizeof(g_subkey_sign_cer) - RSA4096_SIGN_LENGTH);
}

static uint32_t little2big(uint32_t number)
{
    return ((number & 0xff) << SEC_BOOT_SHIFT_24) |
           ((number & 0xff00) << SEC_BOOT_SHIFT_8) |
           ((number & 0xff0000) >> SEC_BOOT_SHIFT_8) |
           ((number >> SEC_BOOT_SHIFT_24) & 0xff);
}

bool sec_boot_subkey_sign_certificate_param_verify(void)
{
    if (!sec_boot_subkey_sign_certificate_subkeyid_verify()) {
        PRINT("subkey id verify error!"NEWLINE);
        return false;
    }

    if (!sec_boot_subkey_sign_certificate_category_verify()) {
        PRINT("subkey category verify error!"NEWLINE);
        return false;
    }
    PRINT("subkey cert param is valid!!!"NEWLINE);
    return true;
}

bool sec_boot_subkey_sign_certificate_verify(uint32_t root_pubkey_addr, uint32_t signature_cert_addr)
{
    // 1. root public key check
    if (!sec_boot_root_pubkey_sha256_validate(root_pubkey_addr)) {
        PRINT("root pubkey verify fail" NEWLINE);
        return false;
    }

    // 2. copy signature certificate copy to g_subkey_sign_cer
    if (!non_os_is_aon_flag_initialised(AON_XIP) && is_xip_address(signature_cert_addr)) {
        uapi_flash_read_data(CHIP_FLASH_ID, (signature_cert_addr - FLASH_START),
                             (uint8_t *)(&g_subkey_sign_cer), sizeof(g_subkey_sign_cer));
    } else {
        errno_t sec_ret = memcpy_s((uint8_t *)(&g_subkey_sign_cer), sizeof(g_subkey_sign_cer),
                                   (uint8_t *)(uintptr_t)signature_cert_addr, sizeof(g_subkey_sign_cer));
        if (sec_ret != EOK) {
            PRINT("memcpy error!!!"NEWLINE);
            return false;
        }
    }

    // 3. judge hash algorithm is sha256
    if ((g_subkey_sign_cer.subkey_sign_alg & 0xffff) != 0) {
        PRINT("subkey_sign_alg check fail! only support sha256!"NEWLINE);
        return false;
    }

    // 4. judge rootkey and subkey rsa e value
    uint32_t rootkey_addr = sec_boot_global_rsa_pubkey_addr_get();
    uint32_t pubkey_e = readl(rootkey_addr + RSA_PUB_KEY_E_OFFSET);
    if (little2big(pubkey_e) <= RSA_PUB_KEY_E_MIN_VAL) {
        PRINT("root key is not safe!"NEWLINE);
        return false;
    }

    pubkey_e = readl(g_subkey_sign_cer.subkey + RSA_PUB_KEY_E_OFFSET);
    if (little2big(pubkey_e) <= RSA_PUB_KEY_E_MIN_VAL) {
        PRINT("subkey cert is not safe!"NEWLINE);
        return false;
    }

    // 5. judge sign algorithm and verify
    if ((g_subkey_sign_cer.subkey_sign_alg == 0) && (!sec_boot_disable_pkcs1_alg_get())) {
        return sec_boot_subkey_sign_rsapkcs1_verify();
    }

    if (rsa_get_sign_alg(g_subkey_sign_cer.subkey_sign_alg) == 0x1) {
        uint32_t salt_len = rsa_get_salt_length(g_subkey_sign_cer.subkey_sign_alg);
        return sec_boot_subkey_sign_rsapss_verify(salt_len);
    }
    return false;
}
