/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  ROM SEC BOOT MODULE
 * Author:
 * Create: 2021-03-26
 */
#include "sec_boot_image_sign_validate.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "debug_print.h"
#include "flash.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif
#include "chip_io.h"
#include "securec.h"
#include "pke.h"
#include "sha256.h"
#include "non_os.h"
#include "flash_common_config.h"

#define RSA4096_SIGN_SALT_LENGTH    SHA256_HASH_SIZE

static bool sec_boot_image_hash_verify(uint8_t *image_hash, uint32_t image_start_addr, uint32_t image_len)
{
    errcode_t ret;
    uint8_t sha_result[SHA256_HASH_SIZE] = { 0 };

    ret = uapi_drv_cipher_sha256((uint8_t*)(uintptr_t)image_start_addr, image_len, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    /* memcmp return 0 if str1 equals to str2 */
    if (memcmp((uint8_t *)image_hash, sha_result, SHA256_HASH_SIZE) != 0) {
        PRINT("image sign verify fail!"NEWLINE);
        return false;
    }
    return true;
}

static bool sec_boot_image_sign_para_verify(standard_image_sign_info_t *sign_info)
{
#ifndef USE_EMBED_FLASH
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    uint32_t flash_size = flash_info.flash_size;
#else
    uint32_t flash_size = FLASH_LEN;
#endif

    if (sign_info->cert_header.magic_number != IMGAE_SIGNATURE_CERT_MAGIC) {
        PRINT("boot error: image sign magic number!!!");
        return false;
    }
    if (sign_info->cert_content.image_len > flash_size) {
        PRINT("boot error: image len is too long!!!");
        return false;
    }

    return true;
}

static uint16_t sec_boot_get_ssb_version_limit(void)
{
    uint32_t ssb_ver_counter = 0;
#ifdef EFUSE_INTERFACE_REPLACE
    (void)uapi_efuse_read_buffer((uint8_t *)&ssb_ver_counter, PAL_OTP_SSB_VERSION_COUNTER_START, sizeof(uint32_t));
#else
    (void)otp_read_buffer((uint8_t *)&ssb_ver_counter, PAL_OTP_SSB_VERSION_COUNTER_START, sizeof(uint32_t));
#endif
    for (uint16_t i = 0; i < PAL_OTP_SSB_VERSION_LENGTH_BIT; i++) {
        if ((ssb_ver_counter & 0x1) == 0) {
            return i;
        }
        ssb_ver_counter = ssb_ver_counter >> 1;
    }
    return (uint16_t)(PAL_OTP_SSB_VERSION_LENGTH_BIT - 1);
}

bool sec_boot_ssb_version_verify(uint32_t ssb_version_addr)
{
    sec_boot_clear_rsa_memory();
    uint16_t min_ver = sec_boot_get_ssb_version_limit();
    uint16_t current_ver = readw(ssb_version_addr);
    if (current_ver >= min_ver) {
        return true;
    }
    PRINT("ssb version is not valid!!! current_ver = 0x%x, min_ver = 0x%x"NEWLINE, current_ver, min_ver);
    return false;
}

bool sec_boot_image_sign_verify(uint32_t image_sign_addr, uint32_t sub_certificate_addr)
{
    // 1. read image sign info
    standard_image_sign_info_t sign_info = { 0 };
    errno_t sec_ret;

    if (!non_os_is_aon_flag_initialised(AON_XIP) && is_xip_address(image_sign_addr)) {
#ifdef USE_EMBED_FLASH
        uapi_eflash_read((image_sign_addr - FLASH_START), (uint32_t *)&sign_info, sizeof(standard_image_sign_info_t));
#else
        uapi_flash_read_data(CHIP_FLASH_ID, (image_sign_addr - FLASH_START), (uint8_t *)&sign_info,
            sizeof(standard_image_sign_info_t));
#endif
    } else {
        sec_ret = memcpy_s((uint8_t *)(uintptr_t)(&sign_info), sizeof(standard_image_sign_info_t),
                           (uint8_t *)(uintptr_t)image_sign_addr, sizeof(standard_image_sign_info_t));
        if (sec_ret != EOK) { return false; }
    }

    // 2. verify image sign info
    if (!sec_boot_image_sign_para_verify(&sign_info)) { return false; }

    // 3. copy rsa image sign and pubkey to global rsa addr
    uint32_t rsa_sign = sec_boot_global_rsa_sign_addr_get();
    uint32_t pubkey_addr = sec_boot_global_rsa_pubkey_addr_get();

    sec_ret = memcpy_s((uint8_t *)(uintptr_t)rsa_sign, RSA4096_SIGN_LENGTH,
                       (uint8_t *)sign_info.image_sign, RSA4096_SIGN_LENGTH);
    if (sec_ret != EOK) { return false; }
    if (!non_os_is_aon_flag_initialised(AON_XIP) && is_xip_address(sub_certificate_addr)) {
#ifndef USE_EMBED_FLASH
        uapi_flash_read_data(CHIP_FLASH_ID, (sub_certificate_addr + SUBKEY_CERT_KEY_OFFSET - FLASH_START),
                             (uint8_t *)(uintptr_t)pubkey_addr, RSA4096_PUBKEY_LENGTH);
#else
        uapi_eflash_read((sub_certificate_addr + SUBKEY_CERT_KEY_OFFSET - FLASH_START),
                         (uint32_t *)(uintptr_t)pubkey_addr, RSA4096_PUBKEY_LENGTH);
#endif
    } else {
        sec_ret = memcpy_s((uint8_t *)(uintptr_t)pubkey_addr, RSA4096_PUBKEY_LENGTH,
                           (uint8_t *)(sub_certificate_addr + SUBKEY_CERT_KEY_OFFSET), RSA4096_PUBKEY_LENGTH);
        if (sec_ret != EOK) { return false; }
    }

    // 4. verify image signature
    bool ret = sec_boot_rsa_pss_verify(RSA4096_SIGN_SALT_LENGTH, (uint8_t *)&sign_info,
                                       (sizeof(standard_image_sign_info_t) - RSA4096_SIGN_LENGTH));

#if defined(BUILD_APPLICATION_ROM)
    uint32_t image_load_addr = PAL_SSB_BOOT_RAM_ORIGIN;
#else
    uint32_t image_load_addr = sign_info.cert_content.image_load_addr;
#endif
    if (ret) {
        ret = sec_boot_image_hash_verify(sign_info.cert_content.image_hash, image_load_addr,
                                         sign_info.cert_content.image_len);
    }
    return ret;
}
