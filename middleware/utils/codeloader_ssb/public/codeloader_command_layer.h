/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER COMMAND LAYER MODULE HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_COMMAND_LAYER_H
#define SRC_APPLICATION_CODELOADER_COMMAND_LAYER_H

#include "codeloader_common.h"
/**
 * @brief  Typedef for the return values from codeloader_command_layer_process_input_message
 */
typedef enum {
    CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER,
    CODELOADER_COMMAND_LAYER_RETURN_DO_NOT_SEND_ANY_REPLY,
    CODELOADER_COMMAND_LAYER_RETURN_RESEND_LAST_MESSAGE
} codeloader_command_layer_return_e;

/**
 * @brief  Message processor for a message in the command layer
 * @param  processing_buffer buffer containing the message in the command layer
 * @param  output_buffer pointer to the buffer structure where the reply message will be placed
 * @return Indicates if the reply in output buffer should be sent or if a nack was requested.
 */
codeloader_command_layer_return_e codeloader_command_layer_process_input_message(
    codeloader_rx_buffer_t *processing_buffer, codeloader_common_generic_buffer_t *output_buffer);

#endif