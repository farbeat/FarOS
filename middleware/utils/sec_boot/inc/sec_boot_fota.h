/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  SSB sec boot
 * Author:
 * Create: 2020-06-02
 */

#ifndef SSB_SEC_BOOT_H
#define SSB_SEC_BOOT_H

#include <stdbool.h>
#include <stdint.h>
#include "pal_sec_config_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup connectivity_framework_sec_boot_fota Sec Boot Fota
 * @ingroup  connectivity_framework_sec_boot
 * @{
 */
/**
 * @brief Definiation ssb sec boot check return type.
 */
typedef enum {
    SSB_SEC_CHECK_RET_SUCC,
    SSB_SEC_CHECK_TIMEOUT,
    SSB_SEC_CHECK_MEMCPY_FAIL,
    SSB_SEC_CHECK_RET_FAIL,
} ssb_sec_ret_t;

/**
 * @brief  After recovery/bootloader, copy image signature to flash store region from fota swap info region.
 * @param  bin_index  The index of core.
 * @return True if success, otherwise false.
 */
bool sec_boot_fota_save_sign_after_recovery(pal_fota_bin_index_t bin_index);

/**
 * @brief  After full upgrade, verify image sha256, and copy image signature to flash store region.
 *         from fota swap info region.
 * @param  bin_index  The index of core.
 * @return True if success, otherwise false.
 */
bool sec_boot_fota_save_sign_after_full_upgrade(pal_fota_bin_index_t bin_index);

/**
 * @brief Get fota image signature address.
 * @param fota_bin_index The index of core.
 * @return Return the address of image signature.
 */
uint32_t sec_boot_fota_image_sign_addr_get(pal_fota_bin_index_t fota_bin_index);

/**
 * @brief  Security boot verify image signature in swap info region.
 * @param  fota_bin_index  The index of core.
 * @return True indicates that succ, otherwise false.
 */
bool sec_boot_fota_verify_swap_info_sign(pal_fota_bin_index_t fota_bin_index);

/**
 * @brief  Security boot verify recovery image.
 * @return True if vertify success, otherwise false.
 */
bool sec_boot_verify_recovery_image(void);

/**
 * @brief  Security boot verify app/dsp/bt image.
 * @return True vertify success, otherwise false.
 */
bool sec_boot_verify_standard_image(void);

/**
 * @brief  To enable image backup function.
 * @param  fota_bin_index  The index of core.
 * @return True if backup is enable, otherwise false.
 */
bool sec_boot_fota_enable_backup_region(pal_fota_bin_index_t fota_bin_index);
/**
 * @}
 */
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
