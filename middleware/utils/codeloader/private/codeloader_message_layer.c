/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER MESSAGE LAYER MODULE
 * Author:
 * Create: 2018-10-15
 */

#include "codeloader_common.h"
#include "codeloader_message_layer.h"
#include "codeloader_line_layer.h"
#include "securec.h"
#include "error_code.h"
#include "codeloader_uart.h"

/*
 *  Private type definitions
 */
#define CODELOADER_ERROR_CODE_MESSAGE "error_code:"
#define INIT_MSG_RESERVED_LEN 5
/**
 * Message layer message types
 */
//lint -esym(751, codeloader_msg_cmd_out_e)
typedef enum {
    CODELOADER_MESSAGE_LAYER_COMMAND_OUT_NACK = 1,  // !< CODELOADER_MESSAGE_LAYER_COMMAND_OUT_NACK
    CODELOADER_MESSAGE_LAYER_COMMAND_OUT_ACK,       // !< CODELOADER_MESSAGE_LAYER_COMMAND_OUT_ACK
    CODELOADER_MESSAGE_LAYER_COMMAND_OUT_INIT       // !< CODELOADER_MESSAGE_LAYER_COMMAND_OUT_INIT
} codeloader_msg_cmd_out_e;

/**
 * Input message header
 */
typedef struct {
    uint8_t message_number;
} __attribute__((packed)) codeloader_input_msg_header_t;

/**
 * Generic input message handler
 */
//lint -esym(754, codeloader_generic_input_msg_t::header)
typedef struct {
    codeloader_input_msg_header_t header;
    uint8_t payload[1];
} __attribute__((packed)) codeloader_generic_input_msg_t;

/**
 * Message layer reply message header
 */
typedef struct {
    uint8_t message_number;
    uint8_t ack;
    uint8_t message_acked;
} __attribute__((packed)) codeloader_reply_msg_header_t;

/**
 * Message layer reply message handler
 */
typedef struct {
    codeloader_reply_msg_header_t *header;
    uint8_t *payload;
    uint16_t payload_length;
} codeloader_output_msg_handler_t;  //lint !e959

/**
 * Generic output message handler
 */
//lint -esym(751, codeloader_generic_output_msg_header_t)
//lint -esym(754, codeloader_generic_output_msg_header_t::message_number)
typedef struct {
    uint8_t message_number;
} __attribute__((packed)) codeloader_generic_output_msg_header_t;

/**
 * Init message header
 */
typedef struct {
    uint8_t message_number;
    uint8_t message_type;
} __attribute__((packed)) codeloader_init_msg_header_t;

/**
 * Init message handler
 */
typedef struct {
    codeloader_init_msg_header_t header;
    uint8_t string_message[1];  // used to have a pointer to the begnning, actual size could be bigger.
} __attribute__((packed)) codeloader_init_message_t;

/**
 * Nack message type handler
 */
typedef struct {
    uint8_t message_number;
    uint8_t nack;
    uint8_t nack_error_code;
} __attribute__((packed)) codeloader_nack_message_t;

/**
 * Input message hander
 */
typedef struct {
    codeloader_input_msg_header_t *header;
    uint8_t *payload;
    uint32_t payload_length;
} codeloader_input_msg_handler_t;

/** Variable to keep the next message number to send */
static uint8_t g_codeloader_message_layer_message_number = 0;

/**
 * Gets an input message handler from the layer above
 * @param layer_above_message message from the layer above
 * @param message_handler handler to the input message returned
 */
static void codeloader_message_layer_handler_input_message_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_input_msg_handler_t *message_handler);

/**
 * Gets an output message handler from the layer above
 * @param layer_above_message message from the layer above
 * @param message_handler handler to the input message returned
 */
static void codeloader_message_layer_handler_output_message_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_output_msg_handler_t *message_handler);

/* Process the message layer buffer and returns a buffer for the layer above */
codeloader_message_layer_return_e codeloader_message_layer_process_input_message(
    codeloader_rx_buffer_t *processing_buffer)
{
    codeloader_message_layer_return_e return_code = CODELOADER_MESSAGE_LAYER_RETURN_SUCCESS;
    codeloader_generic_input_msg_t *input_message = NULL;
    uint16_t header_length = sizeof(codeloader_input_msg_header_t);

    if (processing_buffer != NULL) {
        if (processing_buffer->length >= header_length
            && processing_buffer->buffer != NULL) {
            input_message = (codeloader_generic_input_msg_t *)processing_buffer->buffer;  //lint !e826
            processing_buffer->buffer = input_message->payload;
            processing_buffer->length = processing_buffer->length - header_length;
        } else {
            processing_buffer->buffer = NULL;
            processing_buffer->length = 0;
            return_code = CODELOADER_MESSAGE_LAYER_RETURN_BAD_FORMED_MESSAGE;
        }
    } else {
        return_code = CODELOADER_MESSAGE_LAYER_RETURN_NULL_POINTER;  // should not get here
    }

    return return_code;
}

/* Gets an output buffer for the layer above */
void codeloader_message_layer_get_output_buffer(codeloader_common_generic_buffer_t *output_buffer,
    uint16_t length)
{
    uint16_t header_length = sizeof(codeloader_reply_msg_header_t);
    codeloader_line_layer_get_output_buffer(output_buffer, length + header_length);
    output_buffer->buffer = output_buffer->buffer + header_length;
    output_buffer->length = length;
}

/* Form a message reply */
void codeloader_message_layer_form_reply(codeloader_common_generic_buffer_t *processing_message,
    const codeloader_common_generic_buffer_t *layer_above_in_message)
{
    codeloader_input_msg_handler_t input_message_handler;
    codeloader_output_msg_handler_t output_message_handler;

    codeloader_message_layer_handler_input_message_from_layer_above(layer_above_in_message, &input_message_handler);
    codeloader_message_layer_handler_output_message_from_layer_above(processing_message, &output_message_handler);

    output_message_handler.header->message_number = g_codeloader_message_layer_message_number++;
    output_message_handler.header->ack = CODELOADER_MESSAGE_LAYER_COMMAND_OUT_ACK;
    output_message_handler.header->message_acked = input_message_handler.header->message_number;

    processing_message->buffer = (uint8_t *)output_message_handler.header;
    processing_message->length = output_message_handler.payload_length +
                                 sizeof(codeloader_reply_msg_header_t);
}

static uint16_t get_init_message_length(const codeloader_options_t *options)
{
    /*
    * This define block checks that the ouput buffer is at least big enough for the init message, and errors
    * on compile if it is not big enough
    */
    // Version String too long
    ct_assert((sizeof(CODELOADER_INIT_MESSAGE) + sizeof(codeloader_init_msg_header_t) +
               sizeof(codeloader_init_msg_info_t)) <= CODELOADER_TX_BUFFER_SIZE);  //lint !e514 !e754

    uint16_t header_length = sizeof(codeloader_init_msg_header_t);
    header_length += (uint16_t)sizeof(CODELOADER_INIT_MESSAGE);
    header_length += sizeof(options->info.codeloader_type);
    header_length += sizeof(options->info.rom_code_version);
    header_length += sizeof(options->info.chip);
    header_length += sizeof(options->info.codeloader_version);
    header_length += sizeof(uint8_t) * INIT_MSG_RESERVED_LEN;  // reserved bytes at the end
    return header_length;
}  //lint !e550 Options not accessed

/* Send the init message */
void codeloader_message_layer_send_init_message(const codeloader_options_t *options)
{
    /* This function sends the init message according to NL-002662-SP.
     * Any changes to this message must be documented and the Codeloader Version increased.
     */
    codeloader_common_generic_buffer_t output_buffer;
    uint16_t header_length;
    codeloader_init_message_t *output_message = NULL;
    uint32_t flash_version;
    uint32_t rom_version;

    header_length = get_init_message_length(options);

    codeloader_line_layer_get_output_buffer(&output_buffer, header_length);
    output_message = (codeloader_init_message_t *)output_buffer.buffer;  //lint !e826
    memset_s(output_buffer.buffer, output_buffer.length, 0, output_buffer.length);

    output_message->header.message_number = g_codeloader_message_layer_message_number++;
    output_message->header.message_type = CODELOADER_MESSAGE_LAYER_COMMAND_OUT_INIT;

    uint8_t *output_message_buffer = output_message->string_message;

    if (memcpy_s((void *)output_message_buffer, header_length, CODELOADER_INIT_MESSAGE,
        strlen(CODELOADER_INIT_MESSAGE) + 1) != EOK) {  //lint !e419
        return;
    }

    output_message_buffer += strlen(CODELOADER_INIT_MESSAGE) + 1;  //lint !e416
    header_length -= (uint16_t)(strlen(CODELOADER_INIT_MESSAGE) + 1);
    if (memcpy_s((void *)output_message_buffer, header_length, (const void *)&(options->info.codeloader_type),
        sizeof(options->info.codeloader_type)) != EOK) {  //lint !e419
        return;
    }

    output_message_buffer += sizeof(options->info.codeloader_type);  //lint !e416
    header_length -= sizeof(options->info.codeloader_type);

    rom_version = bswap(options->info.rom_code_version);
    if (memcpy_s((void *)output_message_buffer, header_length, (const void *)&rom_version,
        sizeof(options->info.rom_code_version)) != EOK) {  //lint !e419
        return;
    }

    output_message_buffer += sizeof(options->info.rom_code_version);  //lint !e416
    header_length -= sizeof(options->info.rom_code_version);

    if (memcpy_s((void *)output_message_buffer, header_length, (const void *)&(options->info.chip),
        sizeof(options->info.chip)) != EOK) {  //lint !e419
        return;
    }

    output_message_buffer += sizeof(options->info.chip);  //lint !e416
    header_length -= sizeof(options->info.chip);

    flash_version = bswap(options->info.codeloader_version);
    if (memcpy_s((void *)output_message_buffer, header_length, (const void *)&flash_version,
        sizeof(options->info.codeloader_version)) != EOK) {  //lint !e419
        return;
    }

    codeloader_line_layer_send(&output_buffer);
}

/* Send the error message */
void codeloader_message_layer_send_error_message(void)
{
    codeloader_rx_buffer_t codeloader_main_rx_buffer;
    codeloader_common_generic_buffer_t output_buffer;
    uint16_t header_length;
    codeloader_init_message_t *output_message = NULL;
    uint8_t *output_message_buffer = NULL;
    errno_t sec_ret;
    uint32_t error = error_code_get();
    if (error == ERROR_CODE_NONE_ERROR) {
        return;
    }
    header_length = sizeof(codeloader_init_msg_header_t);
    header_length += (uint16_t)sizeof(CODELOADER_ERROR_CODE_MESSAGE);
    header_length += sizeof(error);

    codeloader_line_layer_get_output_buffer(&output_buffer, header_length);
    output_message = (codeloader_init_message_t *)output_buffer.buffer;  //lint !e826
    memset_s(output_buffer.buffer, header_length, 0, header_length);

    output_message->header.message_number = g_codeloader_message_layer_message_number++;
    output_message->header.message_type = CODELOADER_MESSAGE_LAYER_COMMAND_OUT_INIT;

    output_message_buffer = output_message->string_message;

    sec_ret = memcpy_s((void *)output_message_buffer, header_length, CODELOADER_ERROR_CODE_MESSAGE,
        strlen(CODELOADER_ERROR_CODE_MESSAGE) + 1);  //lint !e419
    if (sec_ret != EOK) {
        return;
    }

    output_message_buffer += strlen(CODELOADER_ERROR_CODE_MESSAGE) + 1;  //lint !e416
    header_length -= (uint16_t)(strlen(CODELOADER_ERROR_CODE_MESSAGE) + 1);

    sec_ret = memcpy_s((void *)output_message_buffer, header_length, (const void *)&error, sizeof(error));  //lint !e419
    if (sec_ret != EOK) {
        return;
    }

    codeloader_uart_default_init(&codeloader_main_rx_buffer, NULL, NULL);
    codeloader_line_layer_send(&output_buffer);
    codeloader_uart_deinit();
}

/* Form a nack message with a given code */
void codeloader_message_layer_form_nack_message(codeloader_common_generic_buffer_t *output_buffer,
    codeloader_message_layer_nack_message_code_e nack_code)
{
    uint16_t header_length = sizeof(codeloader_nack_message_t);
    codeloader_nack_message_t *output_message = NULL;

    codeloader_line_layer_get_output_buffer(output_buffer, header_length);
    output_message = (codeloader_nack_message_t *)output_buffer->buffer;  //lint !e826
    output_message->nack = CODELOADER_MESSAGE_LAYER_COMMAND_OUT_NACK;
    output_message->nack_error_code = (uint8_t)nack_code;
    output_message->message_number = g_codeloader_message_layer_message_number++;
}

static void codeloader_message_layer_handler_input_message_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_input_msg_handler_t *message_handler)
{
    size_t header_length = sizeof(codeloader_input_msg_header_t);
    message_handler->header = (codeloader_input_msg_header_t *)(layer_above_message->buffer -
                                                                                header_length);
    message_handler->payload = (uint8_t *)(layer_above_message->buffer);
    message_handler->payload_length = layer_above_message->length;
}

static void codeloader_message_layer_handler_output_message_from_layer_above(
    const codeloader_common_generic_buffer_t *layer_above_message,
    codeloader_output_msg_handler_t *message_handler)
{
    size_t header_length = sizeof(codeloader_reply_msg_header_t);
    message_handler->header = (codeloader_reply_msg_header_t *)(layer_above_message->buffer -
                                                                                header_length);  //lint !e826
    message_handler->payload = (uint8_t *)layer_above_message->buffer;
    message_handler->payload_length = (uint16_t)layer_above_message->length;
}
