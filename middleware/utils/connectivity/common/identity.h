/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: identifier function header
 * Author:
 * Create:
 */
#ifndef LIB_IDENTITY_H
#define LIB_IDENTITY_H

#include <stdint.h>
#include "error_types.h"
#include "chip_core_definition.h"

/** @defgroup connectivity_libs_connectivity_identity CONNECTIVITY Identity
  * @ingroup  connectivity_libs_connectivity
  * @{
  */
#define MAX_VERSION_STRING                 10

typedef enum {
    IMAGE_VERSION_SSB = 0,      /* !< SSB image version  */
    IMAGE_VERSION_RECOVERY = 1, /* !< RECOVERY image version  */
    IMAGE_VERSION_BT = 2, /* !< BT image version */
    IMAGE_VERSION_HIFI = 3,     /* !< HIFI image version */
#if (GNSS_EXIST == YES)
    IMAGE_VERSION_GNSS,         /* !< GNSS image version */
#endif
    IMAGE_VERSION_MAX,          /* !< not been used */
} image_version_e;

/**
 * @brief  Get the firmware version.
 * @param  image image to get version string of.
 * @param  ver_len the length of the version_string
 * @param  ver SW version information(minimum size of 10 bytes).
 * @param  ver_max_len the length of version_string buff(minimum 10 bytes).
 * @return RET_TYPE_OK or error value.
 */
ret_type_e get_firmware_version(image_version_e image, uint16_t *ver_len, char *ver, uint16_t ver_max_len);

#if CORE == BT
/**
 * @brief  get the version of ssb recovery bt and hifi
 * @param  image The image to get version string
 * @return null
 */
void get_version_info(image_version_e image);
#endif

/**
 * @brief  get flash unique id.
 * @param  unique_id The flash unique id.
 * @return RET_TYPE_OK or error value.
 */
ret_type_e get_flash_unique_id(uint32_t *unique_id);

/**
 * @brief  version info init in App core.
 */
void version_info_init_in_app(void);

/**
 * @brief  version info init in App core.
 * @param  hardware_id The hardware version of custom.
 */
void set_custom_hardware_ver(uint8_t hardware_id);

/**
  * @}
  */
#endif
