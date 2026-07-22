/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  SSB sec boot
 * Author:
 * Create: 2021-03-28
 */

#ifndef SEC_BOOT_OTP_WRITE_H
#define SEC_BOOT_OTP_WRITE_H

#include "sec_boot_public.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_otp_write Sec Boot Otp Write
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
/**
 * @brief  Definiation sec boot return type.
 */
typedef enum {
    SEC_BOOT_SUCCESS = 0,
    SEC_BOOT_OTP_FAIL = 1,
    SEC_BOOT_REPEAT_SET = 2,
    SEC_BOOT_ERROR = 3,
} sec_boot_error_code_t;

/**
 * @brief  Set security boot mode by write otp.
 * @param  mode The security boot mode to set.
 * @return SEC_BOOT_SUCCESS if set success, otherwise false.
 */
sec_boot_error_code_t sec_boot_mode_set(sec_boot_mode_t mode);

/**
 * @brief  Set aes decrypt funcation enable.
 * @return SEC_BOOT_SUCCESS if set success, otherwise false.
 */
sec_boot_error_code_t sec_boot_aes_decrypt_set(void);

/**
 * @brief  Write root public key hash to efuse, to enable security boot funcation.
 * @param  root_key  The pointer to root public key.
 * @param  length    The lenght of root public key.
 * @return SEC_BOOT_SUCCESS if write success, otherwise false.
 */
sec_boot_error_code_t sec_boot_rootkey_hash_otp_write(const uint8_t *root_key, uint32_t length);

/**
 * @brief Write ssb image size and ssb image hash information to efuse.
 * @param image_start_addr The start addr of ssb image.
 * @param length           The ssb image length.
 * @param otp_addr         The otp start address of OTP_SSB_CODE_SIZE_START.
 * @return SEC_BOOT_SUCCESS if write success, otherwise false.
 */
sec_boot_error_code_t sec_boot_ssb_image_hash_otp_write(uint32_t image_start_addr, uint32_t length, uint32_t otp_addr);

/**
 * @brief  Write root public key hash to efuse, to enable security boot funcation.
 * @param  fsb_image_start  image start address.
 * @param  fsb_image_len    image length.
 * @return SEC_BOOT_SUCCESS if write success, otherwise false.
 */
sec_boot_error_code_t sec_boot_fsb_hash_otp_write(uint32_t fsb_image_start, uint32_t fsb_image_len);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
