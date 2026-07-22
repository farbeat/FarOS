/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  SSB sec boot
 * Author:
 * Create: 2021-03-28
 */

#ifndef SEC_BOOT_SUBKEY_CERT_VALIDATE_H
#define SEC_BOOT_SUBKEY_CERT_VALIDATE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_cert_validate Sec Boot Subkey Certificate Validate
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
/**
 * @brief  Get the address of global subkey certificate.
 * @return The address of global subkey certificate.
 */
uint32_t sec_boot_global_subkey_cert_get(void);

/**
 * @brief  Compare certificate attribute with SubKeyID in OTP, and return match or not.
 * @return True if certificate attribute is ok, otherwise false.
 */
bool sec_boot_subkey_sign_certificate_subkeyid_verify(void);

/**
 * @brief  Compare certificate attribute with SubKeyCategory in OTP, and return match or not.
 * @return True if certificate attribute is ok, otherwise false.
 */
bool sec_boot_subkey_sign_certificate_category_verify(void);

/**
 * @brief  Compare certificate attribute with SubKeyCategory, SubKeyID in OTP, and return match or not.
 * @return True if certificate attribute is ok, otherwise false.
 */
bool sec_boot_subkey_sign_certificate_param_verify(void);

/**
 * @brief  Vertify subkey sign certificate is valid or not baseon rootpubkey.
 * @param  root_pubkey_addr    The address of root public key.
 * @param  signature_cert_addr The address of signature certificate.
 * @return True if subkey signature verify success valid, otherwise false.
 */
bool sec_boot_subkey_sign_certificate_verify(uint32_t root_pubkey_addr, uint32_t signature_cert_addr);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
