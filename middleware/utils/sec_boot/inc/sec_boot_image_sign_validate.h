/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  SSB sec boot
 * Author:
 * Create: 2021-03-28
 */

#ifndef SEC_BOOT_IMAGE_SIGN_VALIDATE_H
#define SEC_BOOT_IMAGE_SIGN_VALIDATE_H

#include <stdint.h>
#include <stdbool.h>
#include "sec_boot_public.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_sign_validate Sec Boot Image Signature Validate
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
#define NONCE_LEN                   8
#define RSA4096_SIGN_LENGTH         512
#define SHA256_HASH_SIZE            32

/**
 * @brief  The header definiation of image signature certificate .
 */
typedef struct {
    uint32_t magic_number;  //!< Magic number for signature, value is IMGAE_SIGNATURE_CERT_MAGIC.
    uint32_t cert_version;  //!< Default value is 1, not used.
    uint32_t cert_size;     //!< The size of certificate.
    uint32_t cert_sign_alg; //!< [15:0] Hash algorithm, 0x0：sha256.
                            //!< [21:16] Sign algorithm, 0x0: RSA_PKCS1; 0x1：RSA_PSS; 0x10：ECC; others:reseverd.
                            //!< [31:22] Sign algorithm parameter, if RSA_PKCS1：0; if RSA_PSS，the salt length;
                            //!< if ECC, 0.
} certificate_header_t;

/**
 * @brief  The content definiation image signature certificate.
 */
typedef struct {
    uint32_t sw_version;                  //!< Default value is 1, not used, reserved.
    uint8_t nonce[NONCE_LEN];             //!< Random number, not used, reserved.
    uint8_t image_hash[SHA256_HASH_SIZE]; //!< The hash of image.
    uint32_t image_load_addr;             //!< The flash address of image.
    uint32_t image_len;                   //!< The length of image.
    uint32_t is_aes_code_encrypted;       //!< The image is encrypted or not, current not used.
} certificate_content_t;

/**
 * @brief  The definiation image signature certificate.
 */
typedef struct {
    certificate_header_t cert_header;
    certificate_content_t cert_content;
    uint8_t image_sign[RSA4096_SIGN_LENGTH];
} standard_image_sign_info_t;

/**
 * @brief Verify image signature used in Security boot process.
 * @param  image_sign_addr       image signature address.
 * @param  sub_certificate_addr  sub certificate address.
 * @return True if verify ok, otherwise false.
 */
bool sec_boot_image_sign_verify(uint32_t image_sign_addr, uint32_t sub_certificate_addr);

/**
 * @brief  Read ssb version counter in efuse, and judge ssb version is validate or not.
 * @param  ssb_version_addr ssb image version information address.
 * @return True if verify ok, otherwise false.
 */
bool sec_boot_ssb_version_verify(uint32_t ssb_version_addr);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
