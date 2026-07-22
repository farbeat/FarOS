/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description:  SSB sec boot
 * Author:
 * Create: 2021-03-28
 */

#ifndef SEC_BOOT_H
#define SEC_BOOT_H

#include <stdint.h>
#include <stdbool.h>
#include "pal_sec_config_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_public Sec Boot Public
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
#define RSA4096_SIGN_LENGTH         512   // equal to 4096 bits
#define RSA4096_PUBKEY_LENGTH       1024
#define RSA4096_RESULT_OFFSET       (RSA4096_SIGN_LENGTH - SHA256_HASH_SIZE)
#define IMGAE_SIGNATURE_CERT_MAGIC  PAL_IMAGE_SIGN_MAGIC_CODE
#define rsa_get_sign_alg(value)     (((value) >> 16) & (((uint32_t)1 << 6) - 1))
#define rsa_get_salt_length(value)  (((value) >> 22) & (((uint32_t)1 << 10) - 1))
#define is_xip_address(addr)        (((addr) >= PAL_FLASH_START) && ((addr) < PAL_FLASH_MAX_END))
#define SUBKEY_CERT_KEY_OFFSET      24

/**
 * @brief Definiation of sec boot mode.
 */
typedef enum {
    SEC_BOOT_SSB_SHA256_CHECK = 0,      //!< rom enable check ssb sha256 function.
    SEC_BOOT_SSB_SIGN_CHECK = 1,        //!< rom enable check ssb image signature function.
    SEC_BOOT_SSB_SIGN_DEBUG = 2,        //!< rom only calculate ssb image signature.
    SEC_BOOT_DISABLE = 3,               //!< sec boot function disable.
} sec_boot_mode_t;

/**
 * @brief Definiation of sec boot signature algorithm type.
 */
typedef enum {
    RSA_PKCS1_SIGN_ALG = 0x00,
    RSA_PSS_SIGN_ALG = 0x01,
    ECC_SIGN_ALG = 0x10,
} sign_alg_type_t;

#if SEC_BOOT_SIGN_CHECK_EN == YES
/**
 * @brief  Get address of global rsa public.
 * @return Address of global rsa public.
 */
uint32_t sec_boot_global_rsa_pubkey_addr_get(void);

/**
 * @brief  Get address of global rsa signature.
 * @return Address of global rsa signature.
 */
uint32_t sec_boot_global_rsa_sign_addr_get(void);

/**
 * @brief  Get address of global rsa result.
 * @return Address of global rsa result.
 */
uint32_t sec_boot_global_rsa_result_addr_get(void);

/**
 * @brief  Checks if the root pubkey sha256 in OTP is valid and the root pubkey matches.
 * @return True if the root pubkey sha256 is valid and the root pubkey matches, otherwise false.
 */
bool sec_boot_root_pubkey_sha256_validate(uint32_t root_pubkey_addr);
#endif

/**
 * @brief  Read OTP_SEC_BOOT_DEBUG_FLAG in OTP, and return sec boot mode.
 * @return Return boot mode.
 */
sec_boot_mode_t sec_boot_mode_get(void);

/**
 * @brief  Read efuse flag to get pkcs1 algorithm disable or not.
 * @return Return true if disbale pkcs1 algorithm, otherwise return false.
 */
bool sec_boot_disable_pkcs1_alg_get(void);

/**
 * @brief  Read efuse flag to get ssb sha256 set or not.
 * @return Return true if ssb sha256 has been set, otherwise return false.
 */
bool sec_boot_ssb_sha256_get(void);

/**
 * @brief  Read MCU ROOT KEY in OTP.
 * @param  key_sha256 The memory address to store the rootkey sha256.
 * @param  length     The lenght of key_sha256.
 * @return True if root key sha256 get success, otherwise false.
 */
bool sec_boot_rootkey_sha256_get(uint8_t *key_sha256, uint16_t length);

/**
 * @brief  Read OTP_SEC_BOOT_DEBUG_FLAG in OTP, and return enable aes decrypt function or not.
 * @return True if get ase decrypy mode success, otherwise false.
 */
bool sec_boot_aes_decrypt_get(void);

/**
 * @brief  Sec boot rsa4096 (pss pading) verify.
 * @param  salt_length  The length of salt.
 * @param  text         The text used to verify.
 * @param  text_len     The lenght of text.
 * @return True if rsa pss verify success, otherwise false.
 */
bool sec_boot_rsa_pss_verify(uint32_t salt_length, uint8_t *text, uint32_t text_len);

/**
 * @brief  Sec boot calculate rsa exp mod.
 * @param  key_addr   The addr of rsa key.
 * @param  rsa_input  The point of rsa input plain data.
 * @param  rsa_result The rsa output .
 * @return True if rsa exp mod cal success, otherwise false.
 */
bool sec_boot_rsa_exp_mod(uint32_t key_addr, uint8_t *rsa_input, uint8_t *rsa_result);

#if SEC_TRNG_ENABLE == YES
/**
 * @brief rsa4096 (pss pading) sign.
 * @param text The text used to sign.
 * @param text_len The lenght of text.
 * @return Return sign result address if success, otherwise return NULL.
 */
uint8_t *sec_boot_rsa_pss_sign(uint8_t *text, uint32_t text_len);
#endif

/**
 * @brief Sec boot to clear rsa key and caculate result.
 */
void sec_boot_clear_rsa_memory(void);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
