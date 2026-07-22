/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER MESSAGE LAYER MODULE HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_MESSAGE_LAYER_H
#define SRC_APPLICATION_CODELOADER_MESSAGE_LAYER_H

#include "codeloader.h"
#include "chip_definitions.h"

/** Human readable message to send in the first message payload */
#if (CHIP_LIBRA == 1)
#define CHIP_NAME "LIBRA"
#elif (CHIP_SOCMN1 == 1)
#define CHIP_NAME "SOCMN1"
#elif (CHIP_BRANDY == 1)
#define CHIP_NAME "BRANDY"
#elif (CHIP_BS21 == 1)
#define CHIP_NAME "BS21"
#elif (CHIP_BS25 == 1)
#define CHIP_NAME "BS25"
#elif (CHIP_WS53 == 1)
#define CHIP_NAME "WS53"
#endif

#if defined BUILD_APPLICATION_ROM
#define CODELOADER_INIT_MESSAGE CHIP_NAME "-romloader"
#else
#define CODELOADER_INIT_MESSAGE CHIP_NAME "-ssb-codeloader"
#endif

/**
 * @brief  Message layer processor return type
 */
typedef enum {
    CODELOADER_MESSAGE_LAYER_RETURN_SUCCESS,             // !< CODELOADER_MESSAGE_LAYER_RETURN_SUCCESS
    CODELOADER_MESSAGE_LAYER_RETURN_BAD_FORMED_MESSAGE,  // !< CODELOADER_MESSAGE_LAYER_RETURN_BAD_FORMED_MESSAGE
    CODELOADER_MESSAGE_LAYER_RETURN_NULL_POINTER,
} codeloader_message_layer_return_e;

/**
 * @brief  NACK Message Error Codes
 */
typedef enum {
    CODELOADER_MESSAGE_LAYER_NACK_GENERIC_ERROR,
    CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_LINE_BAD_CRC,
    CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_LINE_BAD_FORMED,
    CODELOADER_MESSAGE_LAYER_NACK_MESSAGE_CODE_MESSAGE_BAD_FORMED,
} codeloader_message_layer_nack_message_code_e;

/**
 * @brief  Process the message in the line layer.  It checks the consistency of the CRC.
 * @param  processing_buffer pointer to the buffer to process. The buffer and length change after the execution of this
 * function. The new buffer and length points to the payload to be passed to the layer above
 * @return CODELOADER_MESSAGE_LAYER_RETURN_SUCCESS if the message processing was successful, an error code otherwise.
 */
codeloader_message_layer_return_e codeloader_message_layer_process_input_message(
    codeloader_rx_buffer_t *processing_buffer);

/**
 * @brief  Returns a pointer to the output buffer requested for the layer above.
 * @param  output_buffer buffer ready for the layer above with length available
 * @param  length length requested by the layer above
 */
void codeloader_message_layer_get_output_buffer(codeloader_common_generic_buffer_t *output_buffer, uint16_t length);

/**
 * @brief  Form a reply message for the message in the message_out_buffer.
 *         It uses the message number of the message_in to form the message.
 * @param  message_out message from the layer above to be sent
 * @param  message_in input message this reply is for
 */
void codeloader_message_layer_form_reply(codeloader_common_generic_buffer_t *message_out,
    const codeloader_common_generic_buffer_t *message_in);

/**
 * @brief  Send the init message
 */
void codeloader_message_layer_send_init_message(const codeloader_options_t *options);

/**
 * @brief  Send the error message
 */
void codeloader_message_layer_send_error_message(void);

/**
 * @brief  Form a nack message with a given nack code
 * @param  nack_code code to send in the nack message.
 */
void codeloader_message_layer_form_nack_message(codeloader_common_generic_buffer_t *output_buffer,
    codeloader_message_layer_nack_message_code_e nack_code);

#endif