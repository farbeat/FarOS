/*
 * Copyright (c) @CompanyNameMagicTag. 2021-2021. All rights reserved.
 * Description: UPG common functions header file
 */

#ifndef UPG_COMMON_PORTING_H
#define UPG_COMMON_PORTING_H

#include "memory_config_common.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined (MEMORY_MINI) || defined (SUPPORT_AUTO_OTA_SERVER)
#define UPG_FILE_PATH                   "/user/"
#define UPG_RES_INDEX_PATH              "/user/res_index.bin"
#define UPG_FILE_NAME                   "/user/update.fwpkg"
#else
#define UPG_FILE_PATH                   "/update/"
#define UPG_RES_INDEX_PATH              "/update/res_index.bin"
#define UPG_FILE_NAME                   "/update/update.fwpkg"
#endif

#define UPG_FLASH_SIZE                  (FLASH_MAX_END - FLASH_START)
#define UPG_FLASH_PAGE_SIZE             FLASH_PAGE_SIZE /* Bytes per sector */
#define FOTA_DATA_STATUS_AREA_LEN       UPG_FLASH_PAGE_SIZE
#define FOTA_DATA_BUFFER_AREA_LEN       UPG_FLASH_PAGE_SIZE
#define FOTA_DATA_FLAG_AREA_LEN         UPG_FLASH_PAGE_SIZE
#define UPG_UPGRADE_FLAG_LENGTH (FOTA_DATA_STATUS_AREA_LEN + FOTA_DATA_BUFFER_AREA_LEN + FOTA_DATA_FLAG_AREA_LEN)

#define UPG_META_DATA_LENGTH            0x0

#define PARAMS_PARTITION_IMAGE_ID       0x4B87A52D
#define PARAMS_PARTITION_START_ADDR     PARTITION_TABLE_REGION_START
#define PARAMS_PARTITION_LENGTH         PARTITION_TABLE_REGION_LENGTH

#define EFUSE_REE_SSB_VERSION           0xF0
#define EFUSE_REE_RECOVERT_VERSION      0xF0
#define EFUSE_REE_APP_VERSION           0xF0
#define EFUSE_REE_BT_VERSION            0xF0
#define EFUSE_REE_DSP_MAIN_VERSION      0xF0
#define EFUSE_REE_DSP_OVERLAY_VERSION   0xF0

/**
* @ingroup  iot_update
* @brief Upgrade the backup area of each image before start the upgrade.
*        CNcomment:升级开始前对各个镜像的备区升级。CNend
*
* @retval #ERRCODE_SUCC         Success.
* @retval #Other     Failure. For details, see soc_errno.h.
*/
errcode_t upg_image_backups_update(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* UPG_COMMON_PORTING_H */