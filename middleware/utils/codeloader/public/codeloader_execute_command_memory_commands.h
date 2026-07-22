/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER MEMORY COMMANDS MODULE.
 * Author:
 * Date: 2020-07-13
 */

#ifndef CODELOADER_EXECUTE_COMMAND_MEMORY_COMMANDS_H
#define CODELOADER_EXECUTE_COMMAND_MEMORY_COMMANDS_H

#include "codeloader_common.h"

typedef enum {
    FLASH_LIMIT_BT,
    FLASH_LIMIT_DSP,
    FLASH_LIMIT_APP,
    FLASH_LIMIT_RECV,
    FLASH_LIMIT_EXTERN0,
    FLASH_LIMIT_EXTERN1,
    FLASH_LIMIT_SEC_SSB,
} flash_limit_e;

/**
 * @brief  Codeloader function processor for the validate checksum command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_validate_checksum(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the set memory config command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_memory_config(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the erase core command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_erase_core(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the erase nandflash command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_erase_nand(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the flash limits command for all cores, including core images B.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_processor_set_flash_limits_v2(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the temporal lock command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_temporal_lock(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the permanent lock command.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 * @note   This function writes to the OTP!
 */
int8_t codeloader_command_processor_set_permanent_lock(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

int8_t codeloader_command_processor_read_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);
#endif