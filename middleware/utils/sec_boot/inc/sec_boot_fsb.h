/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BOOTLOADER SEC BOOT MODULE API
 * Author:
 * Create: 2020-11-26
 */

#ifndef ROM_SEC_BOOT_H
#define ROM_SEC_BOOT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_fsb Sec Boot Fsb
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
/**
 * @brief  Get ssb backup enable flag, check it is enable or not.
 * @return True if ssb backup is valid, otherwise false.
 */
bool sec_boot_ssb_backup_flag_get(void);

/**
 * @brief  Set bakcup flag, enable image sign backup region and subkey cert backup region.
 */
void sec_boot_ssb_backup_flag_set(void);

/**
 * @brief  Get romloader flag, check it is enable or not.
 * @return True if romloader is valid, otherwise false.
 */
bool sec_boot_romloader_flag_get(void);

/**
 * @brief  Set romloader flag, enable image sign, subkey cert and rootkey download.
 */
void sec_boot_romloader_flag_set(void);

/**
 * @brief Verify subkey certificate and ssb image signature, return success or not.
 * @param root_pubkey_addr     The address of root public key.
 * @param sign_cert_addr       The address of subkey certificate.
 * @param ssb_image_sign_addr  The address of ssb image signature.
 * @return True if verify success, otherwise false.
 */
bool sec_boot_cert_and_ssb_sign_verify(uint32_t root_pubkey_addr,
                                       uint32_t sign_cert_addr,
                                       uint32_t ssb_image_sign_addr);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif