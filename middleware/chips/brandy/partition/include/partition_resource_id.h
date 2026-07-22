/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides configuration definitions related to partitions. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-01, Create file. \n
 */
#ifndef PARTITION_RESOURCE_ID_H
#define PARTITION_RESOURCE_ID_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* partition image id, must be same as flash partition bin */
#define PARTITION_IMAGE_ID  0x4B87A52D

/**
 * @defgroup middleware_chips_partition Partition
 * @ingroup  middleware_chips
 * @{
 */

/**
 * @if Eng
 * @brief  Partition ids.
 * @else
 * @brief  分区ID
 * @endif
 */
typedef enum {
    PARTITION_SSB_IMAGE = 0x0,
    PARTITION_SSB_BACKUP = 0x1,
    PARTITION_ROOTPUBKEY_SIGNATURE = 0x10,
    PARTITION_SIGNATURE_CERTIFICATE = 0x11,
    PARTITION_SSB_IMAGE_SIGNATURE = 0x12,
    PARTITION_SIGNATURE_CERTIFICATE_BACKUP = 0x13,
    PARTITION_SSB_IMAGE_SIGNATURE_BACKUP = 0x14,
    PARTITION_ALGORITHM_SIGNATURE = 0x15,
    PARTITION_RECOVERY_IMAGE_SIGNATURE = 0x16,
    PARTITION_BT_IMAGE_SIGNATURE = 0x17,
    PARTITION_DSP_MAIN_IMAGE_SIGNATURE = 0x18,
    PARTITION_DSP_OVERLAY_IMAGE_SIGNATURE = 0x19,
    PARTITION_ACPU_IMAGE_SIGNATURE = 0x1A,
    PARTITION_RECOVERY_IMAGE = 0x20,
    PARTITION_BT_IMAGE = 0x21,
    PARTITION_DSP_MAIN_IMAGE = 0x22,
    PARTITION_DSP_OVERLAY_IMAGE = 0x23,
    PARTITION_ACPU_IMAGE = 0x24,
    PARTITION_NV_DATA = 0x30,
    PARTITION_FOTA_DATA = 0x31,

    PARTITION_MAX_CNT = 20 /*!< @if Eng Maximum number of partitions.
        The value is recorded only as the number of partitions and is not used to determine the ID validity.
                                @else   分区的数量，这个枚举只用来记录分区的数量，不作为分区ID有效性的判断 @endif */
} partition_ids_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* PARTITION_RESOURCE_ID_H */
