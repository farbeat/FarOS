/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER LINE LAYER MODULE HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_LINE_LAYER_H
#define SRC_APPLICATION_CODELOADER_LINE_LAYER_H
#include "codeloader_common.h"

/**
 * @brief  codeloader_line_layer_process_input_message return types
 */
typedef enum {
    CODELOADER_LINE_LAYER_RETURN_SUCCESS,            // !< CODELOADER_LINE_LAYER_RETURN_SUCCESS
    CODELOADER_LINE_LAYER_RETURN_BAD_CRC,            // !< CODELOADER_LINE_LAYER_RETURN_BAD_CRC
    CODELOADER_LINE_LAYER_RETURN_BAD_FORMED_MESSAGE  // !< CODELOADER_LINE_LAYER_RETURN_BAD_FORMED_MESSAGE
} codeloader_line_layer_return_e;

/**
 * @brief  Process the message in the line layer.  It checks the consistency of the CRC.
 * @param  processing_buffer pointer to the buffer to process. The buffer and length change after the execution of this
 * function. The new buffer and length points to the payload to be passed to the layer above
 * @return CODELOADER_LINE_LAYER_RETURN_SUCCESS if the CRC checking was successful, an error code otherwise.
 */
codeloader_line_layer_return_e codeloader_line_layer_process_input_message(
    codeloader_rx_buffer_t *processing_buffer);

/**
 * @brief  Returns a pointer to the output buffer requested for the layer above.
 * @param  output_buffer buffer ready for the layer above with length available
 * @param  length length requested by the layer above
 */
void codeloader_line_layer_get_output_buffer(codeloader_common_generic_buffer_t *output_buffer, uint16_t length);

/**
 * @brief  Send the data in the processing buffer message buffer.
 * @param  processing_message it calculates the CRC and form the line layer message ready to be sent to the
 * codeloader_uart.
 */
void codeloader_line_layer_send(codeloader_common_generic_buffer_t *processing_message);

#endif