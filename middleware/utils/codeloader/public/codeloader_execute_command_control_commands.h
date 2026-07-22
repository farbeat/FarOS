/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  codeloader control commands.
 * Author:
 * Date: 2020-07-13
 */

#ifndef CODELOADER_EXECUTE_COMMAND_CONTROL_COMMANDS_H
#define CODELOADER_EXECUTE_COMMAND_CONTROL_COMMANDS_H

#include "codeloader_common.h"

/**
 * @brief  Set the g_control_command_uart_message_recieved.
 * @param  val The value want to set.
 */
void control_command_uart_message_recieved_set(bool val);

/**
 * @brief  Codeloader function processor for the get permissions command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_get_permissions(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  This is a helper function for the set uart speed command, it is called from the
 * tx callback in the codeloader.c file (since that is where we know that we have nothing else to transmit)
 */
void codeloader_command_processor_set_uart_speed_set(void);

/**
 * @brief  Sets a new uart speed for the codeloader
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_set_uart_speed(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the exit command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_exit(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Codeloader function processor for the nack command
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_nack(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Resets the chip
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_reset(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Read version info from running codeloader
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_read_version_info(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

/**
 * @brief  Returns true if the security core code was moved to the Bootloader Program RAM.
 * @return true if the security core is now running from the Bootloader Program RAM.
 */
bool codeloader_command_processor_is_running_from_bootloader_ram(void);

#endif