/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER FLASH COMMANDS MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "codeloader_execute_command_flash_writing_commands.h"
#include "bootloader_configuration.h"
#include "codeloader.h"
#include "codeloader_command_processor_utils.h"
#include "flash_common_config.h"
#include "securec.h"
#include "codeloader_locking.h"
#include "cpu_utils.h"
#include "rtc.h"
#include "irmalloc.h"
#include "uart.h"
#include "flash.h"
#ifdef USE_EMBED_FLASH
#include "eflash.h"
#endif
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_inner.h"
#endif
#ifdef CFG_DRIVERS_MMC
#include "drv_mmc_adapter.h"
#endif
#include "memory_core.h"
#ifdef SUPPORT_HW_SHA256
#include "sec_sha.h"
#else
#include "sha256/sha256.h"
#endif

#include "codeloader_efuse.h"
#include "flash_porting.h"

#ifndef SHA256_HASH_SIZE
#define SHA256_HASH_SIZE    32
#endif

#define INPUT_BUGGER_OFFSET 4
#define MAX_HEX_BYTES_ON_LINE 16

#define MAX_DATA_FLASH_SIZE 0x800000
#define MAX_NAND_BUFF_LEN 4224

typedef enum {
    CODELOADER_COMMAND_DUMP_ERASE_FLASH_CMD_DUMP,
    CODELOADER_COMMAND_DUMP_ERASE_FLASH_CMD_ERASE,
} codeloader_dump_erase_flash_cmd_e;

typedef enum {
    CODELOADER_COMMAND_DUMP_ERASE_FLASH_INDEX_XIP,
    CODELOADER_COMMAND_DUMP_ERASE_FLASH_INDEX_DATA,
} codeloader_dump_erase_flash_index_e;

/**
 * Start flashing commands handler type
 */
typedef struct {
    core_images_e cimage;
    uint16_t relative_flash_page;
} __attribute__((packed)) codeloader_start_flashing_in_t; //lint !e959

typedef struct {
    uint32_t start_addr;
    uint32_t length_of_data;
} __attribute__((packed)) codeloader_start_download_package_t; //lint !e959

typedef struct {
    cores_t core;
    uint16_t key;
    uint16_t length_of_data;
    uint8_t data[];
} __attribute__((packed)) codeloader_kv_set_key_no_attr_t; //lint !e959

#define CODELOADER_SET_KV_KEY_ATTRIBUTE_PERMANENT 0x01

typedef struct {
    cores_t core;
    uint16_t key;
    uint8_t attributes;
    uint16_t length_of_data;
    uint8_t data[];
} __attribute__((packed)) codeloader_kv_set_key_t; //lint !e959  Lint is wrong here about the enum size.

/**
 * used as pointer to the buffer to write, the actual size is requested
 * by cl_cmd_proc_utils_cons_gen_out_msg
 */
typedef struct {
    uint8_t data_to_read[1];
} __attribute__((packed)) codeloader_tlv_read_data_out_t;

typedef struct {
    uint16_t read_start;
    uint16_t read_length;
} __attribute__((packed)) codeloader_tlv_read_data_in_t; //lint !e959

typedef struct {
    cores_t core;
    uint16_t key_id;
    uint16_t key_data_offset;
    uint16_t key_data_max_length;
} __attribute__((packed)) codeloader_kv_get_key_in_t; //lint !e959
/**
 * used as pointer to the buffer to write, the actual size is requested by
 * cl_cmd_proc_utils_cons_gen_out_msg
 */
typedef struct {
    uint16_t key_length;
    uint8_t data_to_read[1];
} __attribute__((packed)) codeloader_kv_read_no_attr_data_out_t; //lint !e959
#define CODELOADER_GET_KV_KEY_ATTRIBUTE_PERMANENT 0x01

/**
 * used as pointer to the buffer to write, the actual size is requested by
 * cl_cmd_proc_utils_cons_gen_out_msg
 */
typedef struct {
    uint8_t attributes;
    uint16_t key_length;
    uint8_t data_to_read[1];
} __attribute__((packed)) codeloader_kv_read_data_out_t; //lint !e959

typedef struct {
    cores_t core;
    uint16_t start_index;
    uint16_t max_number;
} __attribute__((packed)) codeloader_kv_get_keys_num_in_t; //lint !e959

/**
 * used as pointer to the buffer to write, the actual size is requested by
 * cl_cmd_proc_utils_cons_gen_out_msg
 */
typedef struct {
    uint16_t key_length;
    uint8_t data_to_read[1];
} __attribute__((packed)) cl_kv_get_key_list_data_out_t; //lint !e959

typedef struct {
    codeloader_dump_erase_flash_cmd_e cmd;
    codeloader_dump_erase_flash_index_e flash_index;
    uint32_t start_addr;
    uint32_t end_addr;
} __attribute__((packed)) codeloader_dump_erase_flash_t;

typedef struct {
    uint32_t start_addr;
    uint32_t length;
} __attribute__((packed)) codeloader_set_data_flash_region_t;

typedef enum {
    CODELOADER_COMMAND_SET_BIN_POSITION_AT_FLASH = 0,
    CODELOADER_COMMAND_SET_BIN_POSITION_AT_FS_BIN  = 1,
    CODELOADER_COMMAND_SET_BIN_POSITION_AT_EFUSE = 2,
    CODELOADER_COMMAND_SET_BIN_POSITION_AT_INVALID,
} codeloader_set_bin_position_at_t;

typedef struct {
    codeloader_set_bin_position_at_t type;
    uint32_t load_start_addr;
    uint32_t load_length;
} __attribute__((packed)) codeloader_set_bin_head_info_t; //lint !e959

typedef struct {
    uint32_t load_start_addr;
    uint32_t load_length;
    uint8_t checksum[SHA256_HASH_SIZE];
} __attribute__((packed)) codeloader_set_bin_sha_data_t; //lint !e959

//lint -esym(751, codeloader_flash_cmd_error_code_t)
typedef enum {
    CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_FLASHING = -1,
    CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_NON_MULTIPLE_OF_4 = -2,
    CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_CANNOT_CLAIM_NV_PAGE = -3,
    CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_SET_EFUSE = -4,
    CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_WR_NANDFLASH = -5,
} codeloader_flash_cmd_error_code_t;

/**
 * RF Config commands error codes
 */
//lint -esym(751, codeloader_rf_cmd_error_code_t)
//lint -esym(749, codeloader_rf_cmd_error_code_t::CODELOADER_COMMAND_RF_COMMANDS_ERROR_CODE_ERROR_FLASHING)
typedef enum {
    CODELOADER_COMMAND_RF_COMMANDS_ERROR_CODE_ERROR_FLASHING = -1,
    CODELOADER_COMMAND_RF_COMMANDS_ERROR_CONFIG_TOO_BIG = -2,
} codeloader_rf_cmd_error_code_t;

//lint -esym(751, codeloader_read_rf_error_t)
typedef enum {
    CODELOADER_COMMAND_LAYER_READ_RF_ERROR_RF_NOT_SET = -1,
} codeloader_read_rf_error_t;

typedef enum {
    CODELOADER_COMMAND_LAYER_SSB_DBG_CMD_INVALID = -1,
    CODELOADER_COMMAND_LAYER_SSB_DBG_FLASH_INDEX_INVALID = -2,
    CODELOADER_COMMAND_LAYER_SSB_DBG_WRITE_FLASH_FAIL = -3,
} codeloader_ssb_dbg_error_code_t;

typedef enum {
    CODELOADER_COMMAND_DATA_FLASH_ERR_INVALID_PARAM = -1,
    CODELOADER_COMMAND_DATA_FLASH_ERR_REGION_NOT_SET = -2,
    CODELOADER_COMMAND_DATA_FLASH_ERR_MALLOC_FAIL = -3,
    CODELOADER_COMMAND_DATA_FLASH_ERR_WRITE_FLASH_FAIL = -4,
} codeloader_data_flash_error_code_t;

/** Next offset in the flash page to write to */
static uint32_t g_codeloader_flash_writing_commands_location;
/** Maximum offset which the core is allowed to write to */
static uint32_t g_codeloader_flash_writing_commands_max_location;

static uint32_t g_codeloader_data_flash_write_start_addr = 0xFFFFFFFF;
static uint32_t g_codeloader_data_flash_write_length = 0xFFFFFFFF;

static codeloader_set_bin_head_info_t g_codeloader_data_save_position = {0};

#if PUYA_FLASH_SUPPORT == YES
static uint8_t g_puya_sha256[NV_FLASH_REGION_LENGTH] = {0};
#endif

/**
 * Write to the current position in flash a number of bytes in a given buffer
 * @param input_buffer_to_write source buffer to write
 * @param bytes_to_write number of bytes to write
 * @return true if successful
 */
static bool codeloader_execute_command_flash_writing_helper_flash_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write);

#ifdef CFG_DRIVERS_NANDFLASH
/* write to nandflash and update the internal state */
static bool codeloader_execute_command_nandflash_writing_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write);
#endif
#ifdef CFG_DRIVERS_MMC
/* write to emmc and update the internal state */
static bool codeloader_execute_command_emmc_writing_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write);
#endif

//lint -esym(459, codeloader_command_processor_start_flashing)
/* Start flashing command function */
#ifdef SUPPORT_PARTITION_INFO
int8_t codeloader_command_processor_start_flashing(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    codeloader_start_flashing_in_t *input_parameters = NULL;
    partition_ids_t id;
    partition_information_t info;

    // check parameters
    if (input_buffer->length != sizeof(codeloader_start_flashing_in_t)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    input_parameters = (codeloader_start_flashing_in_t *)input_buffer->buffer;  //lint !e826
    input_parameters->relative_flash_page = bitswap_16bit(input_parameters->relative_flash_page);

    id = cpu_utils_core_iamge_to_partition_id(input_parameters->cimage);
    if (id == PARTITION_MAX_CNT) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    errcode_t ret_val = uapi_partition_get_info(id, &info);
    if (ret_val != ERRCODE_SUCC || info.type != PARTITION_BY_ADDRESS) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    if (input_parameters->relative_flash_page * FLASH_PAGE_SIZE > info.part_info.addr_info.size) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    g_codeloader_flash_writing_commands_location = FLASH_START + info.part_info.addr_info.addr +
                                                   input_parameters->relative_flash_page * FLASH_PAGE_SIZE;
    g_codeloader_flash_writing_commands_max_location = g_codeloader_flash_writing_commands_location +
                                                       info.part_info.addr_info.size - 1;

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#else
int8_t codeloader_command_processor_start_flashing(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    codeloader_start_flashing_in_t *input_parameters = NULL;
    uint16_t code_section_length_in_pages;
    core_images_e cimage_to_flash;

    // check parameters
    if (input_buffer->length != sizeof(codeloader_start_flashing_in_t)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        return return_code;
    }

    input_parameters = (codeloader_start_flashing_in_t *)input_buffer->buffer;  //lint !e826
    input_parameters->relative_flash_page = bitswap_16bit(input_parameters->relative_flash_page);

    cimage_to_flash = input_parameters->cimage;
    if (!codeloader_command_processor_utils_cimage_is_valid(cimage_to_flash)) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        return return_code;
    }

    // check permissions
    if (!codeloader_check_permissions(cpu_utils_core_images_to_cores(cimage_to_flash))) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
        return return_code;
    }

    memory_map *core_mem_map = memory_get_cached_core_map(cimage_to_flash);
    if (core_mem_map == NULL) { return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS; }
    // If it is trying to write out of boundaries return error by parameters
    if (core_mem_map->code_section_length > 0) {
        code_section_length_in_pages = flash_get_page(core_mem_map->code_addr +
                                       core_mem_map->code_section_length - 1) -
                                       flash_get_page(core_mem_map->code_addr) + 1;
    } else {
        code_section_length_in_pages = 0;
    }
    if (input_parameters->relative_flash_page <= code_section_length_in_pages) {
        g_codeloader_flash_writing_commands_location = ((flash_get_page(core_mem_map->code_addr) +
                                                      input_parameters->relative_flash_page) *
                                                      FLASH_PAGE_SIZE) +
                                                      FLASH_START;
        g_codeloader_flash_writing_commands_max_location = ((flash_get_page(core_mem_map->code_addr) +
                                                          code_section_length_in_pages) *
                                                          FLASH_PAGE_SIZE) +
                                                          FLASH_START - 1;
        // If we are updating the CORE_IMAGES_BT image run everything from RAM
        if (cimage_to_flash == CORE_IMAGES_BT) {
            codeloader_set_security_core_updated();
        }
    } else {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    return return_code;
}
#endif

//lint -esym(459, codeloader_command_processor_start_download_package)
int8_t codeloader_command_processor_start_download_package(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    uint32_t flash_size;
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    flash_size = flash_info.flash_size;
    codeloader_start_download_package_t *input_parameters = NULL;
    // check parameters
    if (input_buffer->length == sizeof(codeloader_start_download_package_t)) {
        input_parameters = (codeloader_start_download_package_t *)input_buffer->buffer;  //lint !e826
        input_parameters->start_addr = bswap(input_parameters->start_addr);
        input_parameters->length_of_data = bswap(input_parameters->length_of_data);
        if ((input_parameters->start_addr >= FLASH_START) &&
            (input_parameters->start_addr < flash_config_get_end()) &&
            (input_parameters->length_of_data < flash_size)) {
            g_codeloader_flash_writing_commands_location = input_parameters->start_addr;
            g_codeloader_flash_writing_commands_max_location = input_parameters->start_addr +
                                                             input_parameters->length_of_data;
#ifndef USE_EMBED_FLASH
            if (uapi_flash_block_erase(CHIP_FLASH_ID, input_parameters->start_addr - FLASH_START,
                                       input_parameters->length_of_data, true) != ERRCODE_SUCC) {
#else
            if (uapi_eflash_erase(input_parameters->start_addr - FLASH_START,
                                  input_parameters->length_of_data) != ERRCODE_SUCC) {
#endif
                return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
            }
        } else {
            return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
    }
    return return_code;
}

//lint -esym(459, codeloader_command_processor_send_data)
/* Send data command function */
int8_t codeloader_command_processor_send_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (((input_buffer->length % INPUT_BUGGER_OFFSET) == 0) || (input_buffer->length == 0)) {
        if (g_codeloader_data_save_position.type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_FLASH) {
            if (codeloader_execute_command_flash_writing_helper_flash_buffer(input_buffer->buffer,
                input_buffer->length) != true) {
                return_code = CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_FLASHING;
            }
        } else if (g_codeloader_data_save_position.type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_EFUSE) {
            if (!codeloader_efuse_set_byte(input_buffer->buffer, input_buffer->length)) {
                return_code = CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_SET_EFUSE;
            }
        } else if (g_codeloader_data_save_position.type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_FS_BIN) {
#ifdef CFG_DRIVERS_NANDFLASH
            if (!codeloader_execute_command_nandflash_writing_buffer(input_buffer->buffer, input_buffer->length))
#endif
#ifdef CFG_DRIVERS_MMC
            if (!codeloader_execute_command_emmc_writing_buffer(input_buffer->buffer, input_buffer->length))
#endif
            {
                return_code = CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_ERROR_WR_NANDFLASH;
            }
        } else {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
    } else {
        return_code = CODELOADER_COMMAND_FLASH_COMMANDS_ERROR_CODE_NON_MULTIPLE_OF_4;
    }

    return return_code;
}

/* End flashing command function */
int8_t codeloader_command_processor_end_flashing(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (input_buffer->length != 0) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}

//lint -esym(459, codeloader_command_processor_erase_nv_config)
int8_t codeloader_command_processor_erase_nv_config(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(input_buffer);
    UNUSED(output_buffer);
    if (codeloader_check_permissions(CORES_BT_CORE) &&
        codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_NV_CONFIG)) {
        codeloader_main_set_state(CODELOADER_STATE_ERASE_NV_AND_RESET);  // Allows success code to be passed back to PC
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    }
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
}

//lint -esym(459, codeloader_command_processor_erase_ssb)
int8_t codeloader_command_processor_erase_ssb(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(input_buffer);
    UNUSED(output_buffer);
    if ((codeloader_check_permissions(CORES_BT_CORE)) &&
        codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CHIP) &&
        !bootloader_configuration_ssb_sha256_set()) {
#ifndef USE_EMBED_FLASH
#if SSB_BACKUP == YES
        // the start page of ssb is 0, and the end page is SSB_FLASH_REGION_PAGES, include SSB BACKUP REGION FIRST PAGE
            if ((uapi_flash_block_erase(CHIP_FLASH_ID, 0, SSB_FLASH_REGION_LENGTH, true) == ERRCODE_SUCC) &&
                (uapi_flash_block_erase(CHIP_FLASH_ID, SSB_BACKUP_FLASH_REGION_START - FLASH_START,
                                        SSB_BACKUP_FLASH_REGION_LENGTH, true) == ERRCODE_SUCC)) {
#else
        // the start page of ssb is 0, and the end page is [SSB_FLASH_REGION_PAGES - 1]
            if (uapi_flash_block_erase(CHIP_FLASH_ID, 0, SSB_FLASH_REGION_LENGTH, true) == ERRCODE_SUCC) {
#endif
                codeloader_main_set_state(CODELOADER_STATE_ERASE_SSB_AND_RESET);
                return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            }
#else
#if SSB_BACKUP == YES
        // the start page of ssb is 0, and the end page is SSB_FLASH_REGION_PAGES, include SSB BACKUP REGION FIRST PAGE
            if ((uapi_eflash_erase(0, SSB_FLASH_REGION_LENGTH) == ERRCODE_SUCC) &&
                (uapi_eflash_erase(SSB_BACKUP_FLASH_REGION_START - FLASH_START,
                                   SSB_BACKUP_FLASH_REGION_LENGTH) == ERRCODE_SUCC)) {
#else
        // the start page of ssb is 0, and the end page is [SSB_FLASH_REGION_PAGES - 1]
            if (uapi_eflash_erase(0, SSB_FLASH_REGION_LENGTH) == ERRCODE_SUCC) {
#endif
                codeloader_main_set_state(CODELOADER_STATE_ERASE_SSB_AND_RESET);
                return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
            }
#endif
    }
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
}

#if !defined(TARGET_LOCATION_ROM)
//lint -esym(459, codeloader_command_processor_erase_flash_above_ssb)
int8_t codeloader_command_processor_erase_flash_above_ssb(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(input_buffer);
    UNUSED(output_buffer);
    if ((codeloader_check_permissions(CORES_BT_CORE)) &&
        codeloader_unlocked_check(CODELOADER_LOCK_WRITE_ERASE_CHIP)) {
        // Erase all flash pages
#ifndef USE_EMBED_FLASH
        if (uapi_flash_chip_erase(CHIP_FLASH_ID) != ERRCODE_SUCC) {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR;
        }
#else
        if (uapi_eflash_chip_erase(EMBED_FLASH_0) != ERRCODE_SUCC) {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR;
        }
#endif
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    }
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED;
}

__attribute__((unused)) static bool check_permissions_core_unlocked(cores_t core)
{
    if (codeloader_check_permissions(core) &&
        ((core == CORES_BT_CORE && codeloader_unlocked_check(CODELOADER_LOCK_WRITE_KV_BT)) ||
        (core == CORES_PROTOCOL_CORE && codeloader_unlocked_check(CODELOADER_LOCK_WRITE_KV_PROTOCOL)) ||
        (core == CORES_APPS_CORE && codeloader_unlocked_check(CODELOADER_LOCK_WRITE_KV_APPS)))) {
        return true;
    } else {
        return false;
    }
}

//lint -esym(459, codeloader_command_processor_set_kv_key)
int8_t codeloader_command_processor_set_kv_key(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(input_buffer);
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#endif  // TARGET_LOCATION_ROM

static void extract_flash_write(const uint8_t *input_buffer_to_write, uint32_t bytes_to_write)
{
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) &&
        ((g_codeloader_flash_writing_commands_location >= NV_FLASH_REGION_START) &&
            (g_codeloader_flash_writing_commands_location < (NV_FLASH_REGION_START + NV_FLASH_REGION_LENGTH)))) {
        uapi_flash_write_data_without_align(CHIP_FLASH_ID,
            g_codeloader_flash_writing_commands_location - FLASH_START,
            input_buffer_to_write,
            bytes_to_write);
    } else {
        uapi_flash_write_data(CHIP_FLASH_ID,
            g_codeloader_flash_writing_commands_location - FLASH_START,
            input_buffer_to_write,
            bytes_to_write);
    }
}

/* write to flash and update the internal state */
static bool codeloader_execute_command_flash_writing_helper_flash_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write)
{
    uint32_t end_address;
    bool return_value = true;

    end_address = g_codeloader_flash_writing_commands_location + bytes_to_write - INPUT_BUGGER_OFFSET;
    if (end_address <= g_codeloader_flash_writing_commands_max_location) {
        // The only way this could fail is if other tasks has been added to the task list
        // Which should not be possible given the current state of the system
#ifndef SUPPORT_TWO_FLASH
#ifndef USE_EMBED_FLASH
        extract_flash_write(input_buffer_to_write, bytes_to_write);
#else
        uapi_eflash_write(g_codeloader_flash_writing_commands_location - FLASH_START,
                          (uint32_t *)input_buffer_to_write, bytes_to_write);
#endif
#else
        flash_id_t flash_id = CHIP_FLASH_ID;
        uint32_t flash_start_addr = FLASH_START;
        flash_porting_switch_flash_id(&flash_id, &flash_start_addr, g_codeloader_flash_writing_commands_location);
        uapi_flash_write_data(flash_id, g_codeloader_flash_writing_commands_location - flash_start_addr,
                              input_buffer_to_write, bytes_to_write);
#endif
    g_codeloader_flash_writing_commands_location += bytes_to_write;
    } else {
#ifdef LARGE_BUFFER
    uint32_t extra_addr = end_address - g_codeloader_flash_writing_commands_max_location;
    bytes_to_write = bytes_to_write - extra_addr;
#ifndef USE_EMBED_FLASH
    extract_flash_write(input_buffer_to_write, bytes_to_write);
#else
    uapi_eflash_write(
        g_codeloader_flash_writing_commands_location - FLASH_START, (uint32_t *)input_buffer_to_write, bytes_to_write);
#endif
    g_codeloader_flash_writing_commands_location += bytes_to_write;
    return_value = true;
#else
    return_value = false;
#endif
    }
    return return_value;
}

int8_t codeloader_command_processor_dbg_dump_erase_flash_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    errcode_t ret;
    UNUSED(output_buffer);

    if (input_buffer->length != sizeof(codeloader_dump_erase_flash_t)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_BUFFER_NOT_AVAILABLE;
    }
    codeloader_dump_erase_flash_t *dump_erase_cmd = NULL;
    dump_erase_cmd = (codeloader_dump_erase_flash_t *)input_buffer->buffer;
    dump_erase_cmd->start_addr = bswap(dump_erase_cmd->start_addr);
    dump_erase_cmd->end_addr = bswap(dump_erase_cmd->end_addr);
    if (dump_erase_cmd->cmd > CODELOADER_COMMAND_DUMP_ERASE_FLASH_CMD_ERASE) {
        return CODELOADER_COMMAND_LAYER_SSB_DBG_CMD_INVALID;
    }
    if (dump_erase_cmd->flash_index > CODELOADER_COMMAND_DUMP_ERASE_FLASH_INDEX_DATA) {
        return CODELOADER_COMMAND_LAYER_SSB_DBG_FLASH_INDEX_INVALID;
    }

    UNUSED(dump_erase_cmd->flash_index);

    PRINT("Cmd:0x%x,start=0x%x,end=0x%x\n", dump_erase_cmd->cmd, dump_erase_cmd->start_addr, dump_erase_cmd->end_addr);
    if (dump_erase_cmd->cmd == CODELOADER_COMMAND_DUMP_ERASE_FLASH_CMD_DUMP) {
        for (uint32_t i = dump_erase_cmd->start_addr; i < dump_erase_cmd->end_addr; i++) {
            PRINT("%2x ", *(uint8_t *)(uintptr_t)i);
            if (i % MAX_HEX_BYTES_ON_LINE == 0) {
                PRINT("\n");
            }
        }
    } else {
        ret = uapi_flash_block_erase(CHIP_FLASH_ID, dump_erase_cmd->start_addr,
            dump_erase_cmd->end_addr - dump_erase_cmd->start_addr, true);
        if (ret != ERRCODE_SUCC) {
            PRINT("Erase the flash data fail\n");
        }
    }

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}

#ifdef CFG_DRIVERS_NANDFLASH
/* write to nandflash and update the internal state */
static bool codeloader_execute_command_nandflash_writing_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write)
{
    bool return_value = true;
    const uint8_t *ptr = input_buffer_to_write;
    static uint32_t nand_pos = 0;
    struct nand_driver_struct *nand_drv = (struct nand_driver_struct *)get_nand_flash();
    static uint8_t nand_buf[MAX_NAND_BUFF_LEN] = {0xff};
    uint32_t start = g_codeloader_data_save_position.load_start_addr * nand_drv->pages_per_block;
    uint32_t end = start + g_codeloader_data_save_position.load_length * nand_drv->pages_per_block;

    // rev data len is 4k,divide data len to (page size+obb size)
    while (bytes_to_write != 0) {
        if (nand_pos + bytes_to_write >= (nand_drv->bytes_per_page + nand_drv->bytes_per_oob)) {
            if (memcpy_s(nand_buf + nand_pos, MAX_NAND_BUFF_LEN, ptr,
                         (nand_drv->bytes_per_page + nand_drv->bytes_per_oob - nand_pos)) != EOK) {
                PRINT("nand memcpy fail!\n");
                return_value = false;
                break;
            }
            bytes_to_write = bytes_to_write - (nand_drv->bytes_per_page + nand_drv->bytes_per_oob - nand_pos);
            ptr = ptr + (nand_drv->bytes_per_page + nand_drv->bytes_per_oob - nand_pos);
            nand_pos = 0;

            // If all data in the obb segment is 0xff, the obb data is not written to the NAND flash
            if (nand_oob_check(nand_buf, nand_drv->bytes_per_page, nand_drv->bytes_per_oob)) {
                g_codeloader_flash_writing_commands_location++;
                continue;
            }
        } else {
            if (memcpy_s(nand_buf + nand_pos, MAX_NAND_BUFF_LEN, ptr, bytes_to_write) != EOK) {
                PRINT("nand memcpy fail!\n");
                return_value = false;
                break;
            }
            nand_pos = nand_pos + bytes_to_write;
            bytes_to_write = 0;
            continue;
        }

        if (g_codeloader_flash_writing_commands_location < start ||
            g_codeloader_flash_writing_commands_location >= end) {
            return_value = false;
            break;
        }

        while (!nand_write_and_check(nand_drv, nand_buf, g_codeloader_flash_writing_commands_location)) {
            PRINT("nand write fail, pos=%d next pos!\r\n", g_codeloader_flash_writing_commands_location);
            g_codeloader_flash_writing_commands_location++;
            if (g_codeloader_flash_writing_commands_location == end) {
                break;
            }
        }
        g_codeloader_flash_writing_commands_location++;
    }
    return return_value;
}
#endif

#ifdef CFG_DRIVERS_MMC
static bool codeloader_execute_command_emmc_writing_buffer(
    const uint8_t *input_buffer_to_write, uint32_t bytes_to_write)
{
    const char *ptr = (char*)input_buffer_to_write;
    uint16_t len;
    int ret;
    if (bytes_to_write %  MAX_BLOCK_SIZE == 0) {
        len = bytes_to_write / MAX_BLOCK_SIZE;
    } else {
        len = bytes_to_write / MAX_BLOCK_SIZE + 1;
    }

    ret = ssb_emmc_write(ptr, g_codeloader_flash_writing_commands_location, len);
    PRINT("emmc pos=%d len=%d ret=%d\n", g_codeloader_flash_writing_commands_location, len, ret);
    if (ret == len) {
        g_codeloader_flash_writing_commands_location += len;
        return true;
    } else {
        return false;
    }
}
#endif

int8_t codeloader_command_processor_dbg_show_disk_info(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(input_buffer);
    UNUSED(output_buffer);
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}

//lint -esym(459, codeloader_command_processor_set_dataflash_region)
int8_t codeloader_command_processor_set_dataflash_region(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    UNUSED(g_codeloader_data_flash_write_start_addr);
    UNUSED(g_codeloader_data_flash_write_length);
    codeloader_set_data_flash_region_t *input_parameters = NULL;
    if (input_buffer->length != sizeof(codeloader_set_data_flash_region_t)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    input_parameters = (codeloader_set_data_flash_region_t *)input_buffer->buffer;  //lint !e826
    if (input_parameters->start_addr >= MAX_DATA_FLASH_SIZE || input_parameters->length >= MAX_DATA_FLASH_SIZE) {
        return CODELOADER_COMMAND_DATA_FLASH_ERR_INVALID_PARAM;
    }

    g_codeloader_data_flash_write_start_addr = input_parameters->start_addr;
    g_codeloader_data_flash_write_length = input_parameters->length;
    PRINT("Data flash: start=0x%x, len=0x%x\n", g_codeloader_data_flash_write_start_addr,
        g_codeloader_data_flash_write_length);

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}

//lint -esym(459, codeloader_command_processor_clear_dataflash_region)
int8_t codeloader_command_processor_clear_dataflash_region(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);

    if (input_buffer->length != 0) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    g_codeloader_data_flash_write_start_addr = 0xFFFFFFFF;
    g_codeloader_data_flash_write_length = 0xFFFFFFFF;

    PRINT("Clear the data-flash region info.\n");

    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}

#ifdef CFG_DRIVERS_NANDFLASH
static int8_t codeloader_set_bin_head_info(uint32_t* start_addr, uint32_t* length)
{
    struct nand_driver_struct *nand_drv = (struct nand_driver_struct *)get_nand_flash();
    if (g_nandflash_init != 0) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    // default set
    if (*start_addr == 0xFFFFFFFF) {
        *start_addr = 0;
    }
    if (*length == 0xFFFFFFFF) {
        *length = nand_drv->total_block;
    }

    erase_block(*start_addr, *length);
    g_codeloader_flash_writing_commands_location = *start_addr * nand_drv->pages_per_block;
    PRINT("nand start block=%d pos=%d len=%d\n", *start_addr, g_codeloader_flash_writing_commands_location, *length);
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#endif

#ifdef CFG_DRIVERS_MMC
#define MMC_START_LOCATION 2048
static int8_t codeloader_set_bin_head_info(uint32_t* start_addr, uint32_t* length)
{
    struct rt_mmcsd_host* mmc = get_ssb_emmc();

    (void)ssb_emmc_erase(0, mmc->card->erase_size);
    g_codeloader_flash_writing_commands_location = MMC_START_LOCATION;
    PRINT("mmc erase size=%d pos=%d\n", mmc->card->erase_size, g_codeloader_flash_writing_commands_location);
    return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
}
#endif

int8_t codeloader_command_processor_set_bin_head_info(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
    codeloader_set_bin_head_info_t *input_parameters = NULL;
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    input_parameters = (codeloader_set_bin_head_info_t *)input_buffer->buffer;  //lint !e826
    if (input_parameters->type >= CODELOADER_COMMAND_SET_BIN_POSITION_AT_INVALID) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    uint32_t load_start_addr = bswap(input_parameters->load_start_addr);
    uint32_t load_length = bswap(input_parameters->load_length);

#ifdef USE_EMBED_FLASH
    load_start_addr = load_start_addr - EMBED_FLASH_START + FLASH_START;
#endif

    if (input_parameters->type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_FLASH) {
#ifndef SUPPORT_TWO_FLASH
        if (load_start_addr < FLASH_START || load_start_addr >= FLASH_MAX_END) {
#else
        if (load_start_addr < DATA_FLASH_START ||  load_start_addr >= FLASH_MAX_END) {
#endif
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }

        if (load_start_addr + load_length > FLASH_MAX_END) {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
        g_codeloader_flash_writing_commands_location = load_start_addr;
        // If the last page of the bin file is less than 4 KB, the tool supplements 4 KB.
        if (load_length % FLASH_PAGE_SIZE != 0) {
            load_length = (load_length / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE + FLASH_PAGE_SIZE;
        }
        g_codeloader_flash_writing_commands_max_location = load_start_addr + load_length;
#ifndef SUPPORT_TWO_FLASH
#ifdef USE_EMBED_FLASH
        if (uapi_eflash_erase(load_start_addr - FLASH_START, load_length) != ERRCODE_SUCC) {
#else
        if (uapi_flash_block_erase(CHIP_FLASH_ID, load_start_addr - FLASH_START, load_length, true) != ERRCODE_SUCC) {
#endif
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
#else
        flash_id_t flash_id = CHIP_FLASH_ID;
        uint32_t flash_start_addr = FLASH_START;
        flash_porting_switch_flash_id(&flash_id, &flash_start_addr, load_start_addr);
        if (uapi_flash_block_erase(flash_id, load_start_addr - flash_start_addr, load_length, true) != ERRCODE_SUCC) {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
        }
#endif
    }

    if (input_parameters->type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_FS_BIN) {
#if (defined CFG_DRIVERS_NANDFLASH) || (defined CFG_DRIVERS_MMC)
        return_code = codeloader_set_bin_head_info(&load_start_addr, &load_length);
        if (return_code != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
            return return_code;
        }
#endif
    }

    g_codeloader_data_save_position.type = input_parameters->type;
    g_codeloader_data_save_position.load_start_addr = load_start_addr;
    g_codeloader_data_save_position.load_length = load_length;
    return return_code;
}

int8_t codeloader_command_processor_set_bin_sha_data(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer)
{
    UNUSED(output_buffer);
#ifdef SUPPORT_TWO_FLASH
    flash_id_t flash_id = CHIP_FLASH_ID;
    uint32_t flash_start_addr = FLASH_START;
    if (flash_porting_switch_flash_id(&flash_id, &flash_start_addr, g_codeloader_data_save_position.load_start_addr)) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;
    }
#endif
    codeloader_set_bin_sha_data_t *input_parameters = NULL;
    char sha256[SHA256_HASH_SIZE] = { 0 };
    int8_t return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS;

    if (input_buffer == NULL) {
        return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

    // type EFUSE and NANDFLASH do not check sha256
    if (g_codeloader_data_save_position.type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_EFUSE ||
        g_codeloader_data_save_position.type == CODELOADER_COMMAND_SET_BIN_POSITION_AT_FS_BIN) {
        return return_code;
    }

    input_parameters = (codeloader_set_bin_sha_data_t *)input_buffer->buffer;  //lint !e826
    uint32_t load_start_addr = bswap(input_parameters->load_start_addr);
    uint32_t load_length = bswap(input_parameters->load_length);
    if ((load_start_addr != g_codeloader_data_save_position.load_start_addr)
        || (load_length > g_codeloader_data_save_position.load_length)) {
            return CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }

#if PUYA_FLASH_SUPPORT == YES
    /* puya flash do not verification */
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    if ((flash_info.flash_id == FLASH_MANUFACTURER_PUYA_P25Q64SL) &&
        ((g_codeloader_data_save_position.load_start_addr >= NV_FLASH_REGION_START) &&
         (g_codeloader_data_save_position.load_length <= NV_FLASH_REGION_LENGTH))) {
        (void)uapi_flash_exit_from_xip_mode(0);
        (void)uapi_flash_read_data_without_align(0, NV_FLASH_REGION_START - FLASH_START, g_puya_sha256, load_length);
        (void)uapi_flash_switch_to_xip_mode(0);
        g_codeloader_data_save_position.load_start_addr = g_puya_sha256;
    }
#endif

#if ((TARGET_CHIP_SOCMN1 == 1) || (TARGET_CHIP_BRANDY == 1))
    uapi_flash_switch_to_xip_mode(0);
#endif
#ifdef SUPPORT_HW_SHA256
    sec_sha_init();
    (void)sec_sha256_update((void *)((uintptr_t)(g_codeloader_data_save_position.load_start_addr)), load_length);
    (void)sec_sha256_final((uint8_t *)sha256, load_length);
#else
    sha256_context_t md;

    (void)sha256_init(&md);
    (void)SHA256Update(&md, (unsigned char *)((uintptr_t)(g_codeloader_data_save_position.load_start_addr)),
                       load_length);
    (void)sha256_final(&md, (unsigned char *)sha256, SHA256_HASH_SIZE);
#endif
#if ((TARGET_CHIP_SOCMN1 == 1) || (TARGET_CHIP_BRANDY == 1))
    uapi_flash_exit_from_xip_mode(0);
#endif

    /* 0 == compare succeeded */
    if (memcmp(sha256, input_parameters->checksum, SHA256_HASH_SIZE) != 0) {
        return_code = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS;
    }
    return return_code;
}
