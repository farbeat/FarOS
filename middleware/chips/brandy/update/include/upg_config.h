/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: UPG config file for product.
 */
#ifndef UPG_PRODUCT_CONFIG_H
#define UPG_PRODUCT_CONFIG_H

#ifdef YES
#undef YES
#endif
#define YES           1

#ifdef  NO
#undef  NO
#endif
#define NO            0

#define UPG_CFG_DEBUG_ENABLED                   NO
#ifndef UPG_CFG_DEBUG_PRINT_ENABLED
#define UPG_CFG_DEBUG_PRINT_ENABLED             NO
#endif
#define UPG_CFG_DIRECT_FLASH_ACCESS             NO
#define UPG_CFG_PROCESS_NOTIFY_SUPPORT          YES
#ifndef UPG_CFG_VERIFICATION_SUPPORT
#define UPG_CFG_VERIFICATION_SUPPORT            YES
#endif
#define UPG_CFG_VERIFICATION_MODE_RSA           YES
#define UPG_CFG_VERIFICATION_MODE_ECC           NO
#define UPG_CFG_VERIFICATION_MODE_SM2_SM3       NO
#define UPG_CFG_VERIFICATION_MODE_SM3_ONLY      NO
#define UPG_CFG_VERIFICATION_MODE_SHA256_ONLY   NO
#define UPG_CFG_NV_UPGRADE_SUPPORT              NO
#define UPG_CFG_DIFF_UPGRADE_SUPPORT            YES
#define UPG_CFG_IMGAE_DECRYPT_SUPPORT           NO
#define UPG_CFG_PROGRESS_RECOVERY_SUPPORT       YES
#define UPG_CFG_ANTI_ROLLBACK_SUPPORT           NO
#define UPG_CFG_HASH_VERIFY_SUPPORT             NO
#define UPG_CFG_SIGNATURE_VERIFY_SUPPORT        NO
#define UPG_CFG_SUPPORT_FILE_SYSTEM             YES
#define UPG_CFG_MEMORY_DCACHE_ENABLED           NO
#define UPG_CFG_SUPPORT_IMAGE_ON_FILE_SYSTEM    NO
#define UPG_CFG_SUPPORT_RESOURCES_FILE          YES

#include "upg_default_config.h"
#endif /* UPG_PRODUCT_CONFIG_H */
