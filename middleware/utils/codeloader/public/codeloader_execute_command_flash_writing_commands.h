/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER FLASH COMMANDS MODULE.
 * Author:
 * Date: 2020-07-13
 */

#ifndef CODELOADER_EXECUTE_COMMAND_FLASH_WRITING_COMMANDS_H
#define CODELOADER_EXECUTE_COMMAND_FLASH_WRITING_COMMANDS_H

#include "codeloader_common.h"

/**
 * @brief  Codeloader function processor for the start flashing command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_start_flashing(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for update fota package
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_start_download_package(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the send data command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_send_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the end flashing command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_end_flashing(const codeloader_rx_buffer_t *input_buffer,
    codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Erases all the nv config except the security cores config pages
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_erase_nv_config(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Erases all flash of the SSB
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_erase_ssb(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Erases all flash following the SSB
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_erase_flash_above_ssb(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Sets a specific keys value in the KV storage
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_kv_key(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Dump or erase the flash data
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_dbg_dump_erase_flash_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Show the disk infomation
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_dbg_show_disk_info(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Set the region of data-flash.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_dataflash_region(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Set the data wants to write into data-flash.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_dataflash_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Clear the region of data-flash..
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_clear_dataflash_region(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Set bin head info.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_bin_head_info(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Set Bin Sha Data.
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_bin_sha_data(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

#endif