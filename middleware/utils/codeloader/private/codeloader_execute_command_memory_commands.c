/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:    BT CODELOADER MEMORY COMMANDS MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "codeloader_execute_command_memory_commands.h"
#include "non_os.h"
#include "codeloader_command_processor_utils.h"
#include "codeloader_configuration.h"
#include "codeloader.h"
#include "memory_core.h"
#include "sha256/sha256.h"
#include <string.h>
#include "cpu_utils.h"
#include "ssb_config_common.h"
#include "codeloader_locking.h"
#include "panic.h"
#if defined(BUILD_APPLICATION_SSB)
#include "hal_sectors.h"
#endif
#include "flash_common_config.h"
#include "flash.h"
#include "flash_porting.h"
#include "errcode.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif

#define CODELOADER_MAX_READ_LENGTH    4096
uint8_t g_codeloader_read_data_buf[CODELOADER_MAX_READ_LENGTH + 1];
#define CHECKSUM_LENGTH 32

/**
 * Erase core input parameters
 */
typedef struct {
    core_images_e cimage;
} __attribute__((packed)) codeloader_erase_core_in_para_t; //lint !e959
/**
 * Set flash limits input parameters, flash pages every core.
 */
typedef struct {
    uint16_t bt_code_length;
    uint16_t dsp_code_length;
    uint16_t apps_code_length;
    uint16_t recovery_code_length;
#if CODELOADER_SINGLE_PARTITION_EXP == YES
    uint16_t extern0_code_length;
#endif
#if (CHIP_LIBRA == 1)
    uint16_t extern0_code_length;
    uint16_t extern1_code_length;
    uint16_t extern1_ssb_code_length;
#endif
} __attribute__((packed)) codeloader_set_flash_limits_v2_t;

/**
 * Validate checksum input values
 */
typedef struct {
    core_images_e cimage;
    uint8_t checksum[CHECKSUM_LENGTH];
} __attribute__((packed)) codeloader_validate_checksum_t; //lint !e959
/**
 * Set memory config input parameters
 */
typedef struct {
    core_images_e cimage;
    uint32_t flash_length;
    uint32_t ram_length;
} __attribute__((packed)) codeloader_set_memory_config_t; //lint !e959

/**
 * Set temporal lock to the codeloader input parameters
 */
typedef struct {
    cores_t core;
} __attribute__((packed)) codeloader_set_temporal_lock_t; //lint !e959

/**
 * Set permanent lock to the codeloader input parameters
 */
typedef struct {
    cores_t core;
} __attribute__((packed)) codeloader_set_permanent_lock_t; //lint !e959
typedef struct {
    uint16_t range;
} __attribute__((packed)) codeloader_set_apps_kv_extra_pages_t; //lint !e959

typedef struct {
    uint16_t range;
} __attribute__((packed)) codeloader_set_nv_scan_range_t; //lint !e959

typedef struct {
    uint16_t pages;
} __attribute__((packed)) codeloader_set_apps_image_pages_t; //lint !e959

/**
 * Particular errors for validate checksum command
 */
//lint -esym(751, codeloader_validate_checksum_error_code_t)
typedef enum {
    CODELOADER_COMMAND_VALIDATE_CHECKSUM_ERROR_CHECKSUM_DOES_NOT_MATCH = -1,
} codeloader_validate_checksum_error_code_t;

/**
 * Particular errors for set memory config command
 */
//lint -esym(751, codeloader_set_mem_cfg_error_code_t)
typedef enum {
    CODELOADER_COMMAND_SET_MEMORY_CONFIG_ERROR_CODE_FLASH_TOO_BIG = -1,
    CODELOADER_COMMAND_SET_MEMORY_CONFIG_ERROR_CODE_RAM_TOO_BIG = -2,
} codeloader_set_mem_cfg_error_code_t;

/**
 * Particular errors for erase core command
 */
//lint -esym(751, codeloader_erase_core_error_code_t)
typedef enum {
    CODELOADER_COMMAND_ERASE_CORE_ERROR_CODE_PROBLEM_ERASING = -1,
} codeloader_erase_core_error_code_t;

/**
 * Read data from assigned addr
 */
typedef struct {
    uint32_t addr;
    uint32_t length;
} __attribute__((packed)) codeloader_read_data_t;

/**
 * Erases the program region for a given core.
 * @param core core to erase
 * @return true if the write function to the flash was good
 */
static bool codeloader_command_processor_erase_core_helper_erase(core_images_e cimage);

//lint -esym(459, codeloader_command_processor_validate_checksum)
/* Validate checksum command */
int8_t codeloader_command_processor_validate_checksum(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    // using cfbb flash needs to switch to xip when validating checksum
#ifndef USE_EMBED_FLASH
    uapi_flash_switch_to_xip_mode(CHIP_FLASH_ID);
#endif
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_validate_checksum_t *input_parameters = NULL;
    core_images_e cimage;
    memory_map *core_memory_map = NULL;
    sha256_context_t md;
    char sha256[CHECKSUM_LENGTH] = { 0 };

    // check parameters
    if (input_buffer->length != sizeof(codeloader_validate_checksum_t)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        return return_code;
    }

    input_parameters = (codeloader_validate_checksum_t *)input_buffer->buffer;  //lint !e826

    // remap codeloader core to internal core number
    cimage = input_parameters->cimage;
    if (codeloader_command_processor_utils_cimage_is_valid(cimage)) {
        if (codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage))) {
            core_memory_map = memory_get_cached_core_map(cimage);
            if (core_memory_map == NULL) { return CODELOADER_COMMAND_VALIDATE_CHECKSUM_ERROR_CHECKSUM_DOES_NOT_MATCH; }
            (void)sha256_init(&md);                             // always returns success
            // if it fails we will have an incorrect SHA and fail
            (void)SHA256Update(&md, (unsigned char *)((uintptr_t)(core_memory_map->code_addr - FLASH_START + \
                               flash_config_get_start_addr())), core_memory_map->code_length);
            (void)sha256_final(&md, (unsigned char *)sha256, CHECKSUM_LENGTH);    // we don't care if it fails
            /* 0 == compare succeeded */
            if (memcmp(sha256, input_parameters->checksum, CHECKSUM_LENGTH) != 0) {
                return_code = CODELOADER_COMMAND_VALIDATE_CHECKSUM_ERROR_CHECKSUM_DOES_NOT_MATCH;
            }
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    return return_code;
}

static int8_t set_memory_config_pre_check_stage1(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer, bool *keep_ram)
{
    UNUSED(output_buffer);
    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_set_memory_config_t *input_parameters = NULL;

    // check parameters
    if (input_buffer->length != sizeof(codeloader_set_memory_config_t)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    input_parameters = (codeloader_set_memory_config_t *)input_buffer->buffer;  //lint !e826

    // remap codeloader core to internal core number
    core_images_e cimage = input_parameters->cimage;

    if (!codeloader_command_processor_utils_cimage_is_valid(cimage)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        return return_code;
    }

    if (!codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage)) ||
        !codeloader_unlocked_check(CODELOADER_LOCK_WRITE_MEMORY_CONFIG)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        return return_code;
    }

    // flash set request
    uint32_t flash_length = bswap(input_parameters->flash_length);
    // RAM requested
    uint32_t ram_length = bswap(input_parameters->ram_length);
    *keep_ram = (ram_length == 0xFFFFFFFF);
    // check ram the start and lengths are either the keep value or multiple of 4
    if (!(*keep_ram) && (ram_length % 4 != 0)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        return return_code;
    }

    memory_map *core_memory_map = memory_get_cached_core_map(cimage);
    // check flash fits
    if ((core_memory_map == NULL) || (flash_length > core_memory_map->code_section_length)) {
        return_code = CODELOADER_COMMAND_SET_MEMORY_CONFIG_ERROR_CODE_FLASH_TOO_BIG;
    }

    return return_code;
}

static int8_t set_memory_config_pre_check_stage2(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_set_memory_config_t *input_parameters = NULL;
    core_images_e other_slave_cimage = CORE_IMAGES_MAX_NUMBER;

    input_parameters = (codeloader_set_memory_config_t *)input_buffer->buffer;  //lint !e826
    uint32_t ram_length = bswap(input_parameters->ram_length);
    core_images_e cimage = input_parameters->cimage;
    switch (cimage) {
        case CORE_IMAGES_BT:
            if (ram_length > SSB_BOOT_RAM_LENGTH) {
                return_code = CODELOADER_COMMAND_SET_MEMORY_CONFIG_ERROR_CODE_RAM_TOO_BIG;
            }
            break;
        case CORE_IMAGES_EXTERN0:
        case CORE_IMAGES_EXTERN1:
        case CORE_IMAGES_EXTERN1_SSB:
        case CORE_IMAGES_PROTOCOL:
            other_slave_cimage = CORE_IMAGES_APPS;
            break;
        case CORE_IMAGES_APPS:
        case CORE_IMAGES_RECOVERY:
            other_slave_cimage = CORE_IMAGES_PROTOCOL;
            break;
        default:
            // Already checked above - core has to be 0-2
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;  //lint !e527
            break;
    }

    UNUSED(other_slave_cimage);
    return return_code;
}

//lint -esym(459, codeloader_command_processor_set_memory_config)
/* Set memory config command */
#ifdef SUPPORT_PARTITION_INFO
int8_t codeloader_command_processor_set_memory_config(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(input_buffer);

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#else
int8_t codeloader_command_processor_set_memory_config(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    bool keep_ram = false;
    codeloader_set_memory_config_t *input_parameters = NULL;
    memory_map *other_slave_core_memory_map = NULL;

    int8_t return_code = set_memory_config_pre_check_stage1(input_buffer, output_buffer, &keep_ram);
    input_parameters = (codeloader_set_memory_config_t *)input_buffer->buffer;  //lint !e826
    core_images_e cimage = input_parameters->cimage;
    if ((return_code == CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) && (!keep_ram)) {
        UNUSED(set_memory_config_pre_check_stage2);
    }

    uint32_t flash_length = bswap(input_parameters->flash_length);
    uint32_t ram_length = bswap(input_parameters->ram_length);
    memory_map *core_memory_map = memory_get_cached_core_map(cimage);
    if (core_memory_map == NULL) { return CODELOADER_COMMAND_GENERAL_RETURN_CODE_NULL_BUFFER; }
    if (return_code != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) { return return_code; }
    memory_update_cached_core_code_region(cimage, core_memory_map->code_addr, flash_length,
                                          core_memory_map->code_section_length);
    if ((!keep_ram) && (cimage < CORE_IMAGES_MAX_NUMBER)) {
        if (cimage == CORE_IMAGES_APPS) {   // apps goes first in RAM and then protocol
            other_slave_core_memory_map = memory_get_cached_core_map(CORE_IMAGES_PROTOCOL);
            if (other_slave_core_memory_map != NULL) {
                mem_update_cached_core_ram_region(CORE_IMAGES_APPS, SHARED_MEM_START, ram_length);
                mem_update_cached_core_ram_region(CORE_IMAGES_PROTOCOL, SHARED_MEM_START + ram_length,
                                                  other_slave_core_memory_map->ram_length);
            }
        } else if ((cimage == CORE_IMAGES_BT) || (cimage == CORE_IMAGES_RECOVERY)) {
            mem_update_cached_core_ram_region(cimage,
                                              (SSB_BOOT_RAM_ORIGIN + SSB_BOOT_RAM_LENGTH) - ram_length, ram_length);
            memory_update_cached_core_otp_region(cimage, BT_OTP_OFFSET,
                                                 MAX(BT_OTP_MIN_LENGTH, BT_OTP_LENGTH));  //lint !e506
        } else {
            // the core goes second in the RAM so we asume it keeps its start address
            mem_update_cached_core_ram_region(cimage, core_memory_map->ram_addr, ram_length);
        }
    }
    if (!memory_write_back_all_core_maps()) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}
#endif

#ifdef SUPPORT_PARTITION_INFO
int8_t codeloader_processor_set_flash_limits_v2(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(input_buffer);

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#elif !(defined TARGET_LOCATION_ROM)
static void codeloader_command_set_core_region(core_images_e cimage, uint32_t length, uint32_t start_address)
{
    uint32_t core_code_length;
    memory_map *core_map = NULL;
    core_map = memory_get_cached_core_map(cimage);
    if (core_map == NULL) { return; }
    if ((cimage == CORE_IMAGES_RECOVERY) || (cimage == CORE_IMAGES_BT)) {
        core_code_length = ((core_map->code_length <= length) ? core_map->code_length : 0);
    } else {
        // if the new length exceeds the code length or the start address has changed
        // set the code length to 0 otherwise leave it as it was
        core_code_length = (((core_map->code_length <= length) &&
                             (core_map->code_addr == start_address)) ? core_map->code_length : 0);
    }
    memory_update_cached_core_code_region(cimage, start_address, core_code_length, length);
}

#if CODELOADER_SINGLE_PARTITION_EXP == YES
static bool codeloader_command_processor_set_flash_limits_common_helper(
    uint16_t bt_code_length, uint16_t dsp_code_length, uint16_t apps_code_length, uint16_t recovery_code_length,
    uint16_t extern0_code_length)
{
    uint32_t start_address, core_section_length;
    bool ret_value = true;
    uint16_t apps_area_pages;

    // set recover region
    start_address = RECOVERY_FLASH_REGION_START;
    core_section_length = FLASH_PAGE_SIZE * recovery_code_length;
    codeloader_command_set_core_region(CORE_IMAGES_RECOVERY, core_section_length, start_address);

    // set bt region
    start_address = BT_FLASH_REGION_START;
    core_section_length = FLASH_PAGE_SIZE * bt_code_length;
    codeloader_command_set_core_region(CORE_IMAGES_BT, core_section_length, start_address);

    // set dsp region
    start_address = DSP_FLASH_REGION_START;
    core_section_length = FLASH_PAGE_SIZE * dsp_code_length;
    codeloader_command_set_core_region(CORE_IMAGES_PROTOCOL, core_section_length, start_address);
 
    // set extern0 region
    start_address = DSP_CODEC_FLASH_REGION_START;
    core_section_length = FLASH_PAGE_SIZE * extern0_code_length;
    codeloader_command_set_core_region(CORE_IMAGES_EXTERN0, core_section_length, start_address);

    // set apps region
    apps_area_pages = (uint16_t)APP_FLASH_REGION_PAGES;
    start_address = (uint32_t)APP_FLASH_REGION_START;

    // check apps images will fit into allocated area
    if (apps_code_length > apps_area_pages) {
        ret_value = false;
    } else {
        core_section_length = FLASH_PAGE_SIZE * apps_code_length;
        codeloader_command_set_core_region(CORE_IMAGES_APPS, core_section_length, start_address);
    }
    return ret_value;
}
#else
/* Set flash limits command */
static bool codeloader_command_processor_set_flash_limits_common_helper(
    codeloader_set_flash_limits_v2_t *flash_limit)
{
    if (flash_limit == NULL) {
        return false;
    }
    uint32_t start_address, core_section_length;
    bool ret_value = true;
    uint16_t apps_area_pages;

    // Core Images always starts at end of the SSB.
    start_address = (uint32_t)((uintptr_t)(g_hal_sectors_flash_region_end)) - \
                                         FLASH_START + flash_config_get_start_addr();
    // make sure we can never put system images in the Application Area
    if ((start_address + ((flash_limit->bt_code_length + flash_limit->recovery_code_length +
        flash_limit->dsp_code_length + flash_limit->extern0_code_length + flash_limit->extern1_code_length +
        flash_limit->extern1_ssb_code_length) * FLASH_PAGE_SIZE)) > APP_FLASH_REGION_START) {
        ret_value = false;
    } else {
        // set sec ssb region
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->extern1_ssb_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_EXTERN1_SSB, core_section_length, start_address);

        // set security region
        start_address = BT_FLASH_REGION_START;
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->bt_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_BT, core_section_length, start_address);

        // set protocol region
        start_address = DSP_FLASH_REGION_START;
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->dsp_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_PROTOCOL, core_section_length, start_address);

        // set gnss region
        start_address = GNSS_FLASH_REGION_START;
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->extern0_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_EXTERN0, core_section_length, start_address);

        // set recover region
        start_address = RECOVERY_FLASH_REGION_START;
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->recovery_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_RECOVERY, core_section_length, start_address);

        // set sec region
        start_address = SEC_FLASH_REGION_START;
        core_section_length = FLASH_PAGE_SIZE * (flash_limit->extern1_code_length);
        codeloader_command_set_core_region(CORE_IMAGES_EXTERN1, core_section_length, start_address);

        // set apps region
        apps_area_pages = (uint16_t)APP_FLASH_REGION_PAGES;
        start_address = (uint32_t)APP_FLASH_REGION_START;
        // check apps images will fit into allocated area
        if (flash_limit->apps_code_length > apps_area_pages) {
            ret_value = false;
        } else {
            core_section_length = FLASH_PAGE_SIZE * (flash_limit->apps_code_length);
            codeloader_command_set_core_region(CORE_IMAGES_APPS, core_section_length, start_address);
        }
    }
    return ret_value;
}
#endif

static uint16_t codeloader_get_ssb_size(void)
{
    return (uint16_t)g_hal_sectors_flash_region_pages;
}

static uint16_t codeloader_get_code_flash_size(flash_limit_e cimage)
{
    switch (cimage) {
        case FLASH_LIMIT_BT:
            return (uint16_t)BT_FLASH_REGION_PAGES;
        case FLASH_LIMIT_DSP:
            return (uint16_t)DSP_FLASH_REGION_PAGES;
        case FLASH_LIMIT_APP:
            return (uint16_t)APP_FLASH_REGION_PAGES;
        case FLASH_LIMIT_RECV:
            return (uint16_t)(RECOVERY_FLASH_REGION_PAGES + DTB_IMAGE_PAGES + RESERVE_IMAGE_PAGES);
#if SUPPORT_SINGLE_DSP_DUAL_IMAGE == YES
        case FLASH_LIMIT_EXTERN0:
            return (uint16_t)HIFI0_CODEC_PAGES;
#else
        case FLASH_LIMIT_EXTERN0:
#if CHIP_SOCMN1 || CHIP_BS25 || CHIP_BS21
            return (uint16_t)HIFI1_IMAGE_PAGES;
#elif (CHIP_LIBRA == 1)
            return (uint16_t)GNSS_FLASH_REGION_PAGES;
#endif
#endif
#if (CHIP_LIBRA == 1)
        case FLASH_LIMIT_EXTERN1:
            return (uint16_t)SEC_FLASH_REGION_PAGES;
        case FLASH_LIMIT_SEC_SSB:
            return (uint16_t)SEC_SSB_FLASH_REGION_PAGES;
#endif
        default:
            return (uint16_t)g_hal_sectors_flash_region_pages;
    }
}

#if CODELOADER_SINGLE_PARTITION_EXP == YES
int8_t codeloader_processor_set_flash_limits_v2(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(input_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    // check permissions
    if (!codeloader_check_permissions(cpu_utils_core_images_to_cores(CORE_IMAGES_BT)) ||
        !codeloader_unlocked_check(CODELOADER_LOCK_WRITE_FLASH_LIMITS)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        return return_code;
    }

    // set flash limits
    codeloader_set_flash_limits_v2_t *input_parameters = (
        codeloader_set_flash_limits_v2_t *)input_buffer->buffer;  //lint !e826
    input_parameters->bt_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_BT);
    input_parameters->dsp_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_DSP);
    input_parameters->apps_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_APP);
    input_parameters->recovery_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_RECV);
    input_parameters->extern0_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_EXTERN0);

    // if running codeloader on the chip BS32 we need to make allowance for the SSB residing in Flash.
    uint16_t total_code_length = codeloader_get_ssb_size() + input_parameters->bt_code_length +
                               input_parameters->dsp_code_length + input_parameters->apps_code_length +
                               input_parameters->recovery_code_length + input_parameters->extern0_code_length;
    if ((total_code_length <= (flash_config_get_pages() - SYSTEM_RESERVED_FLASH_PAGES))) {
        if (codeloader_command_processor_set_flash_limits_common_helper(input_parameters->bt_code_length,
            input_parameters->dsp_code_length, input_parameters->apps_code_length,
            input_parameters->recovery_code_length, input_parameters->extern0_code_length)) {
            if (!memory_write_back_all_core_maps()) {
                return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
            }
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    return return_code;
}
#else
int8_t codeloader_processor_set_flash_limits_v2(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(input_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    // check permissions
    if (!codeloader_check_permissions(cpu_utils_core_images_to_cores(CORE_IMAGES_BT)) ||
        !codeloader_unlocked_check(CODELOADER_LOCK_WRITE_FLASH_LIMITS)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        return return_code;
    }

    // set flash limits
    codeloader_set_flash_limits_v2_t *input_parameters = (
        codeloader_set_flash_limits_v2_t *)input_buffer->buffer;  //lint !e826
    input_parameters->bt_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_BT);
    input_parameters->dsp_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_DSP);
    input_parameters->apps_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_APP);
    input_parameters->recovery_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_RECV);
    input_parameters->extern0_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_EXTERN0);
    input_parameters->extern1_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_EXTERN1);
    input_parameters->extern1_ssb_code_length = codeloader_get_code_flash_size(FLASH_LIMIT_SEC_SSB);

    // if running codeloader on the chip BS32 we need to make allowance for the SSB residing in Flash.
    uint16_t total_code_length = codeloader_get_ssb_size() + input_parameters->bt_code_length +
                               input_parameters->dsp_code_length + input_parameters->apps_code_length +
                               input_parameters->recovery_code_length + input_parameters->extern0_code_length +
                               input_parameters->extern1_code_length + input_parameters->extern1_ssb_code_length;
    if ((total_code_length <= (flash_config_get_pages() - SYSTEM_RESERVED_FLASH_PAGES))
        && (input_parameters->recovery_code_length > 0)) {
        if (codeloader_command_processor_set_flash_limits_common_helper(input_parameters)) {
            if (!memory_write_back_all_core_maps()) {
                return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
            }
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    return return_code;
}
#endif
#endif  // !(defined TARGET_LOCATION_ROM)

// remap codeloader core to internal core number
static int codeloader_command_remap_internal_core_number(core_images_e cimage)
{
    uint32_t int_core_number;
    int_core_number = (cimage == CORE_IMAGES_BT &&
                       codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_BT)) || (
                       cimage == CORE_IMAGES_PROTOCOL &&
                       codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_PROTOCOL)) || (
                       cimage == CORE_IMAGES_APPS &&
                       codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_APPS)) || (
                       cimage == CORE_IMAGES_RECOVERY &&
                       codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_RECOVER));
#if SECURITY_EXIST == NO
    return ((int_core_number || (cimage == CORE_IMAGES_EXTERN0 &&
                                 codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN0))) &&
                                 codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage)));
#else
    return ((int_core_number || ((cimage == CORE_IMAGES_EXTERN0 &&
                                 codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN0)) || (
                                 cimage == CORE_IMAGES_EXTERN1 &&
                                 codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN1)) || (
                                 cimage == CORE_IMAGES_EXTERN1_SSB &&
                                 codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CIMAGE_EXTERN1)))) &&
             codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage)));
#endif
}

//lint -esym(459, codeloader_command_processor_erase_core)
/* Erase command */
int8_t codeloader_command_processor_erase_core(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    codeloader_erase_core_in_para_t *input_parameters = NULL;
    core_images_e cimage;

    if (input_buffer->length == 1) {
        input_parameters = (codeloader_erase_core_in_para_t *)input_buffer->buffer;  //lint !e826

        // remap codeloader core to internal core number
        cimage = input_parameters->cimage;
        if (codeloader_command_remap_internal_core_number(cimage)) {
            // erase core
            if (!codeloader_command_processor_erase_core_helper_erase(cimage)) {
                return_code = CODELOADER_COMMAND_ERASE_CORE_ERROR_CODE_PROBLEM_ERASING;
            }
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}

//lint -esym(459, codeloader_command_processor_set_permanent_lock)
/* Set a permanent lock to the codeloader for a specific core */
int8_t codeloader_command_processor_set_permanent_lock(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    codeloader_set_permanent_lock_t *input_parameters = NULL;
    cores_t core;

    if (input_buffer->length == sizeof(codeloader_set_permanent_lock_t)) {
        input_parameters = (codeloader_set_permanent_lock_t *)input_buffer->buffer;  //lint !e826

        core = input_parameters->core;

        if (((core == CORES_BT_CORE &&
              codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_BT)) || (
              core == CORES_PROTOCOL_CORE &&
              codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_PROTOCOL)) || (
              core == CORES_APPS_CORE && codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_APPS))) && (
              codeloader_check_permissions(core))) {
            // Set the permanent lock in OTP!
            codeloader_configuration_permanent_lock_permissions(core);
            // Update the internal status
            codeloader_update_permissions(core, false);
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}

//lint -esym(459, codeloader_command_processor_set_temporal_lock)
/* Set a temporal lock to the codeloader for a specific core */
int8_t codeloader_command_processor_set_temporal_lock(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_set_temporal_lock_t *input_parameters = NULL;
    cores_t core;
    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    if (input_buffer->length == sizeof(codeloader_set_temporal_lock_t)) {
        input_parameters = (codeloader_set_temporal_lock_t *)input_buffer->buffer;  //lint !e826

        core = input_parameters->core;
        if (((core == CORES_BT_CORE &&
              codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_BT)) || (
              core == CORES_PROTOCOL_CORE &&
              codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_PROTOCOL)) || (
              core == CORES_APPS_CORE && codeloader_unlocked_check(CODELOADER_LOCK_WRITE_CODELOADER_LOCK_APPS))) && (
              codeloader_check_permissions(core))) {
            // Set the temporal lock
            codeloader_configuration_temporary_lock_permissions(core);
            // Update the internal status
            codeloader_update_permissions(core, false);
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}

/*
 *  Private function definitions
 */
#ifdef SUPPORT_PARTITION_INFO
static bool codeloader_command_processor_erase_core_helper_erase(core_images_e cimage)
{
    partition_ids_t id;
    partition_information_t info;

    id = cpu_utils_core_iamge_to_partition_id(cimage);
    if (id == PARTITION_MAX_CNT) {
        return false;
    }

    errcode_t ret_val = uapi_partition_get_info(id, &info);
    if (ret_val != ERRCODE_SUCC || info.type != PARTITION_BY_ADDRESS) {
        return false;
    }

    if (uapi_flash_block_erase(CHIP_FLASH_ID, info.part_info.addr_info.addr,
                               info.part_info.addr_info.size, true) != ERRCODE_SUCC) {
        return false;
    }

    return true;
}
#else
static bool codeloader_command_processor_erase_core_helper_erase(core_images_e cimage)
{
    memory_map *core_memory_map;
    core_memory_map = memory_get_cached_core_map(cimage);
    if (core_memory_map == NULL) {
        panic(PANIC_CODELOADER, __LINE__);
        return false;  //lint !e527  unreachable code
    }

    // erase from core_memory_map->code_addr to core_memory_map->code_length
    // core_memory_map->code_addr will be zero if the map is invalid
    if ((core_memory_map->code_addr > 0) && (core_memory_map->code_section_length > 0)) {
        if (cimage == CORE_IMAGES_BT) {
            codeloader_set_security_core_updated();
        }
#ifndef USE_EMBED_FLASH
        if (uapi_flash_block_erase(CHIP_FLASH_ID, core_memory_map->code_addr - FLASH_START,
                                   core_memory_map->code_section_length, true) != ERRCODE_SUCC) {
#else
        if (uapi_eflash_erase(core_memory_map->code_addr - FLASH_START,
                              core_memory_map->code_section_length) != ERRCODE_SUCC) {
#endif
            return false;
        }
    }
    return true;
}
#endif

int8_t codeloader_command_processor_read_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    codeloader_read_data_t *in_parameters = NULL;
    output_buffer->length = 0;

    if (input_buffer->length != sizeof(codeloader_read_data_t)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    in_parameters = (codeloader_read_data_t *)input_buffer->buffer;
    in_parameters->addr = bswap(in_parameters->addr);
    in_parameters->length = bswap(in_parameters->length);

#ifndef USE_EMBED_FLASH
    uint32_t read_count;
    flash_info_t flash_info = {0};
    uint32_t flash_start = FLASH_START;
    uint8_t flash_id = CHIP_FLASH_ID;
#ifdef SUPPORT_TWO_FLASH
    flash_porting_switch_flash_id(&flash_id, &flash_start, in_parameters->addr);
#endif
    uapi_flash_get_info(flash_id, &flash_info);
    if ((in_parameters->length > CODELOADER_MAX_READ_LENGTH) || (in_parameters->addr < flash_start) ||
        (in_parameters->addr + in_parameters->length > flash_info.flash_size + flash_start)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    read_count = uapi_flash_read_data(flash_id, in_parameters->addr - flash_start, g_codeloader_read_data_buf + 1,
        in_parameters->length);
    if (read_count != in_parameters->length) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
    }
#else
    if ((in_parameters->length > CODELOADER_MAX_READ_LENGTH) || in_parameters->addr < EMBED_FLASH_START) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
    }
    errcode_t ret = uapi_eflash_read(in_parameters->addr - EMBED_FLASH_START,
        (uint32_t *)(g_codeloader_read_data_buf + 1), in_parameters->length);
    if (ret != ERRCODE_SUCC) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS;
    }
#endif

    /* add return_code */
    g_codeloader_read_data_buf[0] = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    output_buffer->length = in_parameters->length + 1;
    output_buffer->buffer = g_codeloader_read_data_buf;

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
