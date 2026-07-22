/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER PROCESSOR UTILS
 * Author:
 * Create: 2018-10-15
 */

#include <stdbool.h>
#include "codeloader_common.h"
#include "codeloader_command_processor_utils.h"
#include "codeloader_message_layer.h"
#include "chip_io.h"

/**
 * Generic command ack message
 */
typedef struct {
    int8_t return_code;
    uint8_t parameters[1];
} __attribute__((packed)) codeloader_out_ack_generic_msg_t;

/*
 *  Private function declarations
 */
/**
 * returns an output buffer for the command layer
 * @param output_buffer buffer pointing to the beginning of a generic command reply message
 * @param length length of the parameters in the reply
 */
static void codeloader_command_processor_get_output_buffer(
    codeloader_common_generic_buffer_t *output_buffer, uint16_t length);

uint8_t *cl_cmd_proc_utils_cons_gen_out_msg(
    codeloader_common_generic_buffer_t *output_buffer, int8_t code, uint16_t params_length)
{
    codeloader_out_ack_generic_msg_t *output_message = NULL;
    codeloader_command_processor_get_output_buffer(output_buffer,
                                                   (uint16_t)(sizeof(codeloader_out_ack_generic_msg_t)
                                                                   - 1) + params_length);
    output_message = (codeloader_out_ack_generic_msg_t *)output_buffer->buffer;  //lint !e826
    output_message->return_code = code;
    return output_message->parameters;
}

bool codeloader_command_processor_utils_cimage_is_valid(core_images_e cimage)
{
    return cimage < CORE_IMAGES_MAX_NUMBER;
}

static void codeloader_command_processor_get_output_buffer(
    codeloader_common_generic_buffer_t *output_buffer, uint16_t length)
{
    output_buffer->buffer = NULL;
    output_buffer->length = 0;
    codeloader_message_layer_get_output_buffer(output_buffer, length);
}
