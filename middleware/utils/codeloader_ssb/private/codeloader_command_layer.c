/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER COMMAND LAYER MODULE
 * Author:
 * Create:
 */
#include "codeloader.h"
#include "codeloader_command_layer.h"
#include "codeloader_message_layer.h"
#include "codeloader_command_processor_utils.h"
#include "codeloader_execute_command_control_commands.h"
#include "codeloader_execute_command_authentication_commands.h"
#include "codeloader_execute_command_flash_writing_commands.h"
#include "codeloader_execute_command_memory_commands.h"
#include "chip_io.h"
#include "codeloader_command_codes.h"

/**
 * Command layer generic input message
 */
typedef struct {
    uint8_t command;
    uint8_t parameters[1];
} __attribute__((packed)) codeloader_command_layer_generic_in_message;

/**
 * Command layer generic output message
 */
// lint -esym(754, codeloader_command_layer_generic_out_message::command)
// lint -esym(754, codeloader_command_layer_generic_out_message::return_value)
// lint -esym(754, codeloader_command_layer_generic_out_message::parameters)
// lint -esym(751, codeloader_command_layer_generic_out_message)
typedef struct {
    uint8_t command;
    int8_t return_value;
    uint8_t parameters[1];
} __attribute__((packed)) codeloader_command_layer_generic_out_message;

/**
 * Command lookup struct
 */
typedef struct {
    uint8_t command_code;
    COMMAND_PROCESSOR_EXECUTION_FUNCTION command;
} codeloader_commands;

/**
 * Command execution functions for the SSB codeloader
 * ANY CHANGE TO THIS TABLE MUST BE DOCUMMENTED IN NL-002662-SP.
 * This list corresponds to the Codeloader Type SSB in NL-002662-SP.
 * If the list of supported commands change the Codeloader Version must be increased.
 */
static const codeloader_commands g_codeloader_command_layer_command_execution_functions[] = {
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_INIT, codeloader_command_processor_get_permissions },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_VALIDATE_CHECKSUM, codeloader_command_processor_validate_checksum },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_START_FLASHING_PAGE, codeloader_command_processor_start_flashing },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_START_DOWNLOAD_PACKAGE, codeloader_command_processor_start_download_package },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SEND_DATA, codeloader_command_processor_send_data },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_END_FLASHING_PAGE, codeloader_command_processor_end_flashing },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_MEM_CONFIG, codeloader_command_processor_set_memory_config },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_CORE, codeloader_command_processor_erase_core },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_EXIT, codeloader_command_processor_exit },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_NACK, codeloader_command_processor_nack },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_PERMANENT_LOCK, codeloader_command_processor_set_permanent_lock },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_TEMPORAL_LOCK, codeloader_command_processor_set_temporal_lock },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_RESET, codeloader_command_processor_reset },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_SPEED, codeloader_command_processor_set_uart_speed },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_NV_CONFIG, codeloader_command_processor_erase_nv_config },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_SSB, codeloader_command_processor_erase_ssb },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_ERASE_CHIP, codeloader_command_processor_erase_flash_above_ssb },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_FLASH_LIMITS, codeloader_processor_set_flash_limits_v2 },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_ADD_SIGNATURE, codeloader_command_processor_add_signature },
    { CODELOADER_COMMAND_LAYER_COMMAND_READ_VERSION_INFO, codeloader_command_processor_read_version_info },
    { CODELOADER_COMMAND_LAYER_COMMAND_IN_SET_KV_KEY, codeloader_command_processor_set_kv_key },
    { CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_HEAD_INFO, codeloader_command_processor_set_bin_head_info },
    { CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_SHA_DATA, codeloader_command_processor_set_bin_sha_data },
    { CODELOADER_COMMAND_LAYER_COMMAND_READ_DATA, codeloader_command_processor_read_data },
};

/**
 * Gets the function pointer for the given command, NULL if that command does not exist
 * @param command_code The code of the command we want
 * @return A pointer to the function for this command_code, NULL if command_code is invalid
 */
static COMMAND_PROCESSOR_EXECUTION_FUNCTION codeloader_get_function_from_command(uint8_t command_code)
{
    for (uint32_t i = 0; i < (sizeof(g_codeloader_command_layer_command_execution_functions) /
        sizeof(g_codeloader_command_layer_command_execution_functions[0])); i++) {
        if (g_codeloader_command_layer_command_execution_functions[i].command_code == command_code) {
            return g_codeloader_command_layer_command_execution_functions[i].command;
        }
    }
    return NULL;
}

static codeloader_command_layer_return_e codeloader_command_layer_waiting_for_init(
    codeloader_rx_buffer_t *processing_buffer, codeloader_common_generic_buffer_t *output_buffer,
    codeloader_command_layer_command_in_e command_code, COMMAND_PROCESSOR_EXECUTION_FUNCTION command,
    int8_t *command_ret_value)
{
    codeloader_command_layer_return_e return_value = CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER;
    if (command_code != CODELOADER_COMMAND_LAYER_COMMAND_IN_INIT) {
        *command_ret_value = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_INIT_HANDSHAKE;
        // burntools not receive ssb-codeloader or send init discard, will send reset chip try again.
        if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_RESET) {
            command(processing_buffer, output_buffer);
        }
        return CODELOADER_COMMAND_LAYER_RETURN_DO_NOT_SEND_ANY_REPLY;
    }
    // if it is the init command_code try to execute it
    *command_ret_value = command(processing_buffer, output_buffer);
    if (*command_ret_value == CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
        codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
    } else {
        return_value = CODELOADER_COMMAND_LAYER_RETURN_DO_NOT_SEND_ANY_REPLY;
    }
    return return_value;
}

static codeloader_command_layer_return_e codeloader_command_layer_waiting_for_command(
    codeloader_rx_buffer_t *processing_buffer, codeloader_common_generic_buffer_t *output_buffer,
    codeloader_command_layer_command_in_e command_code, COMMAND_PROCESSOR_EXECUTION_FUNCTION command,
    int8_t *command_ret_value)
{
    if (command_code != CODELOADER_COMMAND_LAYER_COMMAND_IN_END_FLASHING_PAGE
        && command_code != CODELOADER_COMMAND_LAYER_COMMAND_IN_SEND_DATA) {
        *command_ret_value = command(processing_buffer, output_buffer);
        if (*command_ret_value != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
            (void)cl_cmd_proc_utils_cons_gen_out_msg(output_buffer, *command_ret_value, 0);
            return CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER;
        }

        if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_START_FLASHING_PAGE ||
            command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_START_DOWNLOAD_PACKAGE ||
            command_code == CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_HEAD_INFO) {
            codeloader_main_set_state(CODELOADER_STATE_RECEIVING_DATA_TO_FLASH);
        } else if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_EXIT) {
            codeloader_main_set_state(CODELOADER_STATE_EXIT);
        }
    } else {
        *command_ret_value = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PROTOCOL;
    }
    return CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER;
}

static codeloader_command_layer_return_e codeloader_command_layer_process_input_message_internal(
    codeloader_rx_buffer_t *processing_buffer, codeloader_common_generic_buffer_t *output_buffer,
    codeloader_command_layer_command_in_e command_code, COMMAND_PROCESSOR_EXECUTION_FUNCTION command,
    int8_t *command_ret_value)
{
    codeloader_command_layer_return_e return_value = CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER;
    codeloader_state_e state = codeloader_main_get_state();
    if (state == CODELOADER_STATE_WAITING_FOR_INIT) {
        return_value = codeloader_command_layer_waiting_for_init(processing_buffer, output_buffer,
            command_code, command, command_ret_value);
    } else if (state == CODELOADER_STATE_WAITING_FOR_COMMAND) {
        return_value = codeloader_command_layer_waiting_for_command(processing_buffer, output_buffer,
            command_code, command, command_ret_value);
    } else if (state == CODELOADER_STATE_RECEIVING_DATA_TO_FLASH) {
        if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_END_FLASHING_PAGE ||
            command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_SEND_DATA ||
            command_code == CODELOADER_COMMAND_LAYER_COMMAND_SET_BIN_SHA_DATA) {
            *command_ret_value = command(processing_buffer, output_buffer);
            if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_END_FLASHING_PAGE
                && *command_ret_value == CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
                codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
            } else if (*command_ret_value != CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
                // if data error or something, wait burntools send reset chip command.
                codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
            }
        } else {
            *command_ret_value = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PROTOCOL;
            codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
        }
    } else {
        // it should not get here
        *command_ret_value = CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PROTOCOL;
        codeloader_main_set_state(CODELOADER_STATE_WAITING_FOR_COMMAND);
    }
    return return_value;
}

/* Message processor for a message in the command layer */
codeloader_command_layer_return_e codeloader_command_layer_process_input_message(
    codeloader_rx_buffer_t *processing_buffer, codeloader_common_generic_buffer_t *output_buffer)
{
    codeloader_command_layer_return_e return_value = CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER;
    codeloader_command_layer_generic_in_message *input_message = NULL;
    int8_t command_ret_value;

    output_buffer->buffer = NULL;
    output_buffer->length = 0;

    if (processing_buffer == NULL) {
        (void)cl_cmd_proc_utils_cons_gen_out_msg(output_buffer, CODELOADER_COMMAND_GENERAL_RETURN_CODE_NULL_BUFFER, 0);
        codeloader_error_log_record(CODELOADER_COMMAND_LAYER_COMMAND_INVILIAD,
                                    CODELOADER_COMMAND_GENERAL_RETURN_CODE_NULL_BUFFER);
        return return_value;
    }

    if ((processing_buffer->length < (sizeof(codeloader_command_layer_generic_in_message) - 1)) ||
        processing_buffer->buffer == NULL) {
        (void)cl_cmd_proc_utils_cons_gen_out_msg(output_buffer,
            CODELOADER_COMMAND_GENERAL_RETURN_CODE_WRONG_COMMAND_LAYER_STRUCTURE, 0);
        codeloader_error_log_record(CODELOADER_COMMAND_LAYER_COMMAND_INVILIAD,
                                    CODELOADER_COMMAND_GENERAL_RETURN_CODE_WRONG_COMMAND_LAYER_STRUCTURE);
        return return_value;
    }

    input_message = (codeloader_command_layer_generic_in_message *)processing_buffer->buffer;
    processing_buffer->buffer = input_message->parameters;
    processing_buffer->length = processing_buffer->length -
        (sizeof(codeloader_command_layer_generic_in_message) - 1);
    codeloader_command_layer_command_in_e command_code = (codeloader_command_layer_command_in_e)input_message->command;
    COMMAND_PROCESSOR_EXECUTION_FUNCTION command = codeloader_get_function_from_command(command_code);
    if (!command) {
        (void)cl_cmd_proc_utils_cons_gen_out_msg(output_buffer,
            CODELOADER_COMMAND_GENERAL_RETURN_CODE_UNKNOWN_COMMAND, 0);
        codeloader_error_log_record(command_code, CODELOADER_COMMAND_GENERAL_RETURN_CODE_UNKNOWN_COMMAND);
        return return_value;
    }
    if (command_code == CODELOADER_COMMAND_LAYER_COMMAND_IN_NACK) {
        command_ret_value = command(processing_buffer, output_buffer);
        // Mark it to resend the last message.
        if (command_ret_value == CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS) {
            return_value = CODELOADER_COMMAND_LAYER_RETURN_RESEND_LAST_MESSAGE;
        }
        return return_value;
    }

    return_value = codeloader_command_layer_process_input_message_internal(processing_buffer, output_buffer,
                                                                           command_code, command, &command_ret_value);
    // if no message was created create a generic one
    if (return_value == CODELOADER_COMMAND_LAYER_RETURN_SEND_REPLY_IN_OUTPUT_BUFFER &&
        output_buffer->buffer == NULL) {
        (void)cl_cmd_proc_utils_cons_gen_out_msg(output_buffer, command_ret_value, 0);
    }
    codeloader_error_log_record(command_code, command_ret_value);
    // The reply message has been placed in output_buffer->buffer
    return return_value;
}
