/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER LINE LAYER MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "codeloader_line_layer.h"
#include "codeloader_uart.h"
#include "chip_io.h"
typedef struct {
    uint8_t crc[1];
} __attribute__((packed)) codeloader_line_tail_t;

typedef struct {
    codeloader_line_tail_t *tail;
    uint8_t *payload;
    uint32_t payload_length;
} codeloader_line_msg_handler_t;

/**
 * Get the line layer handler from a message from the layer above.
 * @param layer_above_message pointer to the layer above message
 * @param message_handler line layer message handler
 * @return
 */
static void codeloader_line_layer_handler_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_line_msg_handler_t *message_handler);

/**
 * Handler to the input message from this layer
 * @param line_layer_message buffer received in this layer.
 * @param message_handler mesage handler to the line layer message
 * @return bool if the message was well formed to be handled
 */
static bool codeloader_line_layer_handler_from_this_layer(
    const codeloader_rx_buffer_t *line_layer_message, codeloader_line_msg_handler_t *message_handler);

/**
 * Check the line layer CRC of a given message handler
 * @param message_handler pointer to the line layer message handler
 * @return true if the CRC check was successful
 */
static bool codeloader_line_layer_check_crc(const codeloader_line_msg_handler_t *message_handler);

/**
 * Calculates the crc of a given data and return it in a detination specified
 * @param data_to_check pointer to the beginning of the data to check
 * @param length data length
 * @param crc_destination destination to write the CRC to
 */
static void codeloader_line_layer_helper_calculate_crc(uint8_t *data_to_check,
                                                       uint32_t length, uint8_t *crc_destination);

codeloader_line_layer_return_e codeloader_line_layer_process_input_message(codeloader_rx_buffer_t *processing_buffer)
{
    codeloader_line_layer_return_e return_code = CODELOADER_LINE_LAYER_RETURN_SUCCESS;
    codeloader_line_msg_handler_t message_handler = { 0 };

    if (codeloader_line_layer_handler_from_this_layer(processing_buffer, &message_handler) == true) {
        // check header
        if (codeloader_line_layer_check_crc(&message_handler) == true) {
            processing_buffer->buffer = message_handler.payload;
            processing_buffer->length = message_handler.payload_length;
        } else {
            return_code = CODELOADER_LINE_LAYER_RETURN_BAD_CRC;
        }
    } else {
        return_code = CODELOADER_LINE_LAYER_RETURN_BAD_FORMED_MESSAGE;
    }

    if (return_code != CODELOADER_LINE_LAYER_RETURN_SUCCESS) {
        processing_buffer->buffer = NULL;
        processing_buffer->length = 0;
    }
    return return_code;
}

/* Get the output buffer to put the message from the layer above */
void codeloader_line_layer_get_output_buffer(codeloader_common_generic_buffer_t *output_buffer, uint16_t length)
{
    uint16_t tail_length = sizeof(codeloader_line_tail_t);
    codeloader_uart_get_tx_buffer(output_buffer, length + tail_length);
    // the pointer keeps unchanged
    output_buffer->length = length;
}

/* Send the message from the layer above */
void codeloader_line_layer_send(codeloader_common_generic_buffer_t *processing_message)
{
    codeloader_line_msg_handler_t out_message_handler;
    codeloader_line_layer_handler_from_layer_above(processing_message, &out_message_handler);

    // CRC
    codeloader_line_layer_helper_calculate_crc(out_message_handler.payload, out_message_handler.payload_length,
                                               out_message_handler.tail->crc);

    processing_message->buffer = (uint8_t *)out_message_handler.payload;
    processing_message->length = (uint16_t)out_message_handler.payload_length + sizeof(codeloader_line_tail_t);

    codeloader_uart_write(processing_message);
}

/* from the layer above to the line */
static void codeloader_line_layer_handler_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_line_msg_handler_t *message_handler)
{
    message_handler->tail = (codeloader_line_tail_t *)(layer_above_message->buffer + layer_above_message->length);
    message_handler->payload = (uint8_t *)(layer_above_message->buffer);
    message_handler->payload_length = layer_above_message->length;
}

/* from the line to the layer above */
static bool codeloader_line_layer_handler_from_this_layer(
    const codeloader_rx_buffer_t *line_layer_message,
    codeloader_line_msg_handler_t *message_handler)
{
    uint16_t tail_length = sizeof(codeloader_line_tail_t);
    bool return_value = true;

    if (line_layer_message != NULL && message_handler != NULL) {
        if (line_layer_message->buffer != NULL
            && line_layer_message->length >= tail_length) {
            message_handler->tail = (codeloader_line_tail_t *)(line_layer_message->buffer +
                line_layer_message->length - tail_length);
            message_handler->payload = (uint8_t *)(line_layer_message->buffer);
            message_handler->payload_length = line_layer_message->length - tail_length;
        } else {
            message_handler->tail = NULL;
            message_handler->payload = NULL;
            message_handler->payload_length = 0;
            return_value = false;
        }
    } else {
        return_value = false;  // should not happen
    }
    return return_value;
}

/* Checks the line layer message CRC */
static bool codeloader_line_layer_check_crc(const codeloader_line_msg_handler_t *message_handler)
{
    uint8_t calculated_crc;
    codeloader_line_layer_helper_calculate_crc(message_handler->payload, message_handler->payload_length,
                                               &calculated_crc);
    return (message_handler->tail->crc[0] == calculated_crc);
}

/* Helper function to calculate the crc */
static void codeloader_line_layer_helper_calculate_crc(uint8_t *data_to_check,
                                                       uint32_t length, uint8_t *crc_destination)
{
    uint8_t *data;
    uint8_t crc;
    data = (uint8_t *)data_to_check;
    crc = 0;
    while (length > 0) {
        crc = crc ^ *(data++);
        length--;
    }
    *crc_destination = crc;
}

