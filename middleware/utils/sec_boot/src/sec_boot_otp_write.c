/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  ROM SEC BOOT MODULE
 * Author:
 * Create: 2021-03-26
 */
#include "sec_boot_otp_write.h"
#include "sha256.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "debug_print.h"
#include "pal_sec_config_port.h"
#include "chip_io.h"

#define OTP_SHIFT_24          24UL
#define PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN 1
#define PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC_LEN 1
#define DEFAULT_WRITE_LEN     1
#define PAL_OTP_SSB_SHA256_CRC_LEN 1

#ifdef EFUSE_INTERFACE_REPLACE
static errcode_t sec_boot_ssb_check(sec_boot_mode_t mode, uint8_t byte, errcode_t *otp_ret)
#else
static errcode_t sec_boot_ssb_check(sec_boot_mode_t mode, uint8_t byte, otp_ret_t *otp_ret)
#endif
{
    switch (mode) {
        case SEC_BOOT_SSB_SHA256_CHECK:
            if ((byte & BIT(PAL_OTP_SEC_BOOT_EN_BIT)) == 0) {
#ifdef EFUSE_INTERFACE_REPLACE
                *otp_ret = uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
                *otp_ret |= uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
#else
                *otp_ret = otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
                *otp_ret |= otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
#endif
            }
            break;
        case SEC_BOOT_SSB_SIGN_CHECK:
            if ((byte & BIT(PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT)) == 0) {
#ifdef EFUSE_INTERFACE_REPLACE
                *otp_ret = uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT);
                *otp_ret |= uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT);
#else
                *otp_ret = otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT);
                *otp_ret |= otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT);
#endif
            }
            if ((byte & BIT(PAL_OTP_SEC_BOOT_EN_BIT)) == 0) {
#ifdef EFUSE_INTERFACE_REPLACE
                *otp_ret = uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
                *otp_ret |= uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
#else
                *otp_ret = otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
                *otp_ret |= otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_EN_BIT);
#endif
            }
            break;
        case SEC_BOOT_SSB_SIGN_DEBUG:
#ifdef EFUSE_INTERFACE_REPLACE
            *otp_ret = uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_DEBUG_BIT);
            *otp_ret |= uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_DEBUG_BIT);
#else
            *otp_ret = otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_DEBUG_BIT);
            *otp_ret |= otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_DEBUG_BIT);
#endif
            break;
        default:
            break;
    }
    return ERRCODE_SUCC;
}

sec_boot_error_code_t sec_boot_mode_set(sec_boot_mode_t mode)
{
#if SEC_BOOT_SIGN_CHECK_EN == YES
    sec_boot_mode_t current_mode = sec_boot_mode_get();
    if (current_mode <= mode) {
        PRINT("Don't allow to lower sec boot level!"NEWLINE);
        return SEC_BOOT_ERROR;
    }

    uint8_t otp_byte, double_byte;
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_ret = ERRCODE_SUCC;
    otp_ret |= uapi_efuse_read_buffer(&otp_byte, PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
    otp_ret |= uapi_efuse_read_buffer(&double_byte,
        PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
#else
    otp_ret_t otp_ret = OTP_RET_SUCC;
    otp_ret |= otp_read_byte(&otp_byte, PAL_OTP_SEC_BOOT_DEBUG_FLAG);
    otp_ret |= otp_read_byte(&double_byte, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG);
#endif
    if (otp_ret != ERRCODE_SUCC) {
        PRINT("Read otp fail!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
    otp_byte &= double_byte;
    sec_boot_ssb_check(mode, otp_byte, &otp_ret);
    if (otp_ret != ERRCODE_SUCC) {
        PRINT("Write byte to otp fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#else
    UNUSED(mode);
#endif
    return SEC_BOOT_SUCCESS;
}

sec_boot_error_code_t sec_boot_aes_decrypt_set(void)
{
#if (OTP_HAS_WRITE_PERMISSION == YES) && (SEC_IAMGE_AES_DECRYPT_EN == YES)
    if (sec_boot_aes_decrypt_get()) {
        PRINT("Aes decrypt function already enable!!!"NEWLINE);
        return SEC_BOOT_SUCCESS;
    }
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_ret = uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_AES_EN_BIT);
    otp_ret |= uapi_efuse_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_AES_EN_BIT);
#else
    otp_ret_t otp_ret = otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_AES_EN_BIT);
    otp_ret |= otp_write_bit(PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_AES_EN_BIT);
#endif
    if (otp_ret != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
#endif
    return SEC_BOOT_SUCCESS;
}

static sec_boot_error_code_t hash_check(uint8_t *result, uint8_t *crc)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_calc_crc(result, SHA256_HASH_SIZE, crc) != ERRCODE_SUCC) {
        PRINT("calculate crc error!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (uapi_efuse_write_buffer(PAL_OTP_MCU_ROOT_PUBKEY_SHA256_START, result, SHA256_HASH_SIZE) != ERRCODE_SUCC) {
        PRINT("otp write buffer fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (uapi_efuse_write_buffer(PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC, crc,
        PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC_LEN) != ERRCODE_SUCC) {
        PRINT("otp write crc fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#else
    if (otp_calc_crc(result, SHA256_HASH_SIZE, crc) != OTP_RET_SUCC) {
        PRINT("calculate crc error!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (otp_write_buffer(PAL_OTP_MCU_ROOT_PUBKEY_SHA256_START, result, SHA256_HASH_SIZE) != OTP_RET_SUCC) {
        PRINT("otp write buffer fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (otp_write_byte(PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC, *crc) != OTP_RET_SUCC) {
        PRINT("otp write crc fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#endif
    return SEC_BOOT_SUCCESS;
}

sec_boot_error_code_t sec_boot_rootkey_hash_otp_write(const uint8_t *root_key, uint32_t length)
{
#if (OTP_HAS_WRITE_PERMISSION == YES) && (SEC_BOOT_SIGN_CHECK_EN == YES)
    if (length != RSA4096_PUBKEY_LENGTH) {
        return SEC_BOOT_ERROR;
    }

    uint8_t crc = 0;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer(&crc, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC,
        PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC_LEN) != ERRCODE_SUCC) {
        PRINT("otp read fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#else
    if (otp_read_byte(&crc, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC) != OTP_RET_SUCC) {
        PRINT("otp read fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#endif
    if (crc != 0) {
        PRINT("Rootkey hash has been writed!!!"NEWLINE);
        return SEC_BOOT_REPEAT_SET;
    }

    uint8_t sha_result[SHA256_HASH_SIZE] = {0};
    errcode_t ret = uapi_drv_cipher_sha256(root_key, RSA4096_PUBKEY_LENGTH, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    ret = hash_check(sha_result, &crc);
    if (ret != ERRCODE_SUCC) {
        return false;
    }
#else
    UNUSED(root_key);
    UNUSED(length);
#endif
    return SEC_BOOT_SUCCESS;
}

static sec_boot_error_code_t hash_otp_write(uint8_t *result, uint8_t *crc, uint32_t otp_addr)
{
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_calc_crc(result, SHA256_HASH_SIZE, crc) != ERRCODE_SUCC) {
        PRINT("calculate crc error!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
    if (uapi_efuse_write_buffer((otp_addr + sizeof(uint32_t)), result, SHA256_HASH_SIZE) != ERRCODE_SUCC) {
        PRINT("otp write buffer fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (uapi_efuse_write_buffer((otp_addr + sizeof(uint32_t) + SHA256_HASH_SIZE),
        crc, DEFAULT_WRITE_LEN) != ERRCODE_SUCC) {
        PRINT("otp write crc fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#else
    if (otp_calc_crc(result, SHA256_HASH_SIZE, crc) != OTP_RET_SUCC) {
        PRINT("calculate crc error!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (otp_write_buffer((otp_addr + sizeof(uint32_t)), result, SHA256_HASH_SIZE) != OTP_RET_SUCC) {
        PRINT("otp write buffer fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }

    if (otp_write_byte((otp_addr + sizeof(uint32_t) + SHA256_HASH_SIZE), *crc) != OTP_RET_SUCC) {
        PRINT("otp write crc fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
#endif
    return SEC_BOOT_SUCCESS;
}

static uint32_t hash_operation_write(uint32_t image_start_addr, uint32_t length, uint32_t otp_addr, uint8_t crc)
{
    errcode_t ret;
    uint8_t sha_result[SHA256_HASH_SIZE] = {0};

    ret = uapi_drv_cipher_sha256((uint8_t*)(uintptr_t)image_start_addr, length, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    ret = hash_otp_write(sha_result, &crc, otp_addr);
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    return ret;
}

sec_boot_error_code_t sec_boot_ssb_image_hash_otp_write(uint32_t image_start_addr, uint32_t length, uint32_t otp_addr)
{
#if (OTP_HAS_WRITE_PERMISSION == YES) && (SEC_BOOT_SIGN_CHECK_EN == YES)
    uint8_t otp_data = 0;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer(&otp_data, otp_addr, DEFAULT_WRITE_LEN) != ERRCODE_SUCC) {
#else
    if (otp_read_byte(&otp_data, otp_addr) != OTP_RET_SUCC) {
#endif
        PRINT("otp read fail!!!"NEWLINE);
        return SEC_BOOT_OTP_FAIL;
    }
    if (otp_data != 0) {
        PRINT("ssb image hash has been writed!!!"NEWLINE);
        return SEC_BOOT_REPEAT_SET;
    }
    uint32_t ssb_length = length;
    uint8_t ssb_length_crc;
    uint8_t otp_crc = 0;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_calc_crc((uint8_t *)(&ssb_length), (sizeof(uint32_t) - 1), &ssb_length_crc) != ERRCODE_SUCC) {
#else
    if (otp_calc_crc((uint8_t *)(&ssb_length), (sizeof(uint32_t) - 1), &ssb_length_crc) != OTP_RET_SUCC) {
#endif
        return SEC_BOOT_OTP_FAIL;
    }
    uint32_t ssb_length_with_chk = (((uint32_t)ssb_length_crc) << OTP_SHIFT_24);
    ssb_length_with_chk |= ssb_length;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_write_buffer(otp_addr, (const uint8_t *)&ssb_length_with_chk, sizeof(uint32_t)) != ERRCODE_SUCC) {
#else
    if (otp_write_buffer(otp_addr, (const uint8_t *)&ssb_length_with_chk, sizeof(uint32_t)) != OTP_RET_SUCC) {
#endif
        return SEC_BOOT_OTP_FAIL;
    }
    errcode_t ret = hash_operation_write(image_start_addr, length, otp_addr, otp_crc);
    if (ret != ERRCODE_SUCC) {
        return false;
    }
#else
    UNUSED(image_start_addr);
    UNUSED(length);
    UNUSED(otp_addr);
#endif
    return SEC_BOOT_SUCCESS;
}

static sec_boot_error_code_t fsb_hasl_otp_write(uint8_t *fsb_len, uint8_t *result)
{
    uint8_t fsb_len_crc, fsb_sha_crc;
    uint32_t fsb_len_with_chk;
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_calc_crc(fsb_len, PAL_OTP_SSB_CODE_SIZE_LENGTH, &fsb_len_crc) != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    fsb_len_with_chk = (((uint32_t)fsb_len_crc) << 24UL);
    fsb_len_with_chk |= (uint32_t)(*fsb_len);
    if (uapi_efuse_write_buffer(PAL_OTP_SSB_CODE_SIZE_START, (uint8_t *)&fsb_len_with_chk,
        sizeof(uint32_t)) != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }

    if (uapi_efuse_calc_crc(result, SHA256_HASH_SIZE, &fsb_sha_crc) != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    if (uapi_efuse_write_buffer(PAL_OTP_SSB_SHA256_START, result, SHA256_HASH_SIZE) != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    if (uapi_efuse_write_buffer(PAL_OTP_SSB_SHA256_CRC, &fsb_sha_crc, PAL_OTP_SSB_SHA256_CRC_LEN) != ERRCODE_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
#else
    if (otp_calc_crc(fsb_len, PAL_OTP_SSB_CODE_SIZE_LENGTH, &fsb_len_crc) != OTP_RET_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    fsb_len_with_chk = (((uint32_t)fsb_len_crc) << 24UL);
    fsb_len_with_chk |= (uint32_t)(*fsb_len);
    if (otp_write_buffer(PAL_OTP_SSB_CODE_SIZE_START, (uint8_t *)&fsb_len_with_chk,
                         sizeof(uint32_t)) != OTP_RET_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }

    if (otp_calc_crc(result, SHA256_HASH_SIZE, &fsb_sha_crc) != OTP_RET_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    if (otp_write_buffer(PAL_OTP_SSB_SHA256_START, result, SHA256_HASH_SIZE) != OTP_RET_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
    if (otp_write_byte(PAL_OTP_SSB_SHA256_CRC, fsb_sha_crc) != OTP_RET_SUCC) {
        return SEC_BOOT_OTP_FAIL;
    }
#endif
    return SEC_BOOT_SUCCESS;
}

sec_boot_error_code_t sec_boot_fsb_hash_otp_write(uint32_t fsb_image_start, uint32_t fsb_image_len)
{
#if (OTP_HAS_WRITE_PERMISSION == YES)
    if (sec_boot_ssb_sha256_get()) {
        PRINT("Fsb hash has been writed!!!"NEWLINE);
        return SEC_BOOT_REPEAT_SET;
    }

    errcode_t ret;
    uint8_t sha_result[SHA256_HASH_SIZE] = {0};

    ret = uapi_drv_cipher_sha256((uint8_t *)(uintptr_t)fsb_image_start, fsb_image_len, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    ret = fsb_hasl_otp_write((uint8_t *)&fsb_image_len, sha_result);
    if (ret != ERRCODE_SUCC) {
        return false;
    }
#else
    UNUSED(fsb_image_start);
    UNUSED(fsb_image_len);
#endif
    return SEC_BOOT_SUCCESS;
}
