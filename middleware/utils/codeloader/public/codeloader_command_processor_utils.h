/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:CODELOADER COMMAND PROCESSOR UTILS HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_COMMAND_PROCESSOR_UTILS_H
#define SRC_APPLICATION_CODELOADER_COMMAND_PROCESSOR_UTILS_H

#include "codeloader_common.h"
#include "platform_types.h"

/**
 * @brief  Codeloader General return codes
 */
typedef enum {
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_SUCCESS = 1,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PARAMETERS,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_PERMISSION_DENIED,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_BUFFER_NOT_AVAILABLE,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_UNKNOWN_COMMAND,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_INIT_HANDSHAKE,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_PROTOCOL,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_WRONG_COMMAND_LAYER_STRUCTURE,
    CODELOADER_COMMAND_GENERAL_CHECK_RECEIVED_SHA_FAILED,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_ERROR_IN_MEMORY_PROCESS,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_NULL_BUFFER,
    CODELOADER_COMMAND_GENERAL_RETURN_CODE_SEC_DEBUG_FAIL,
} codeloader_command_general_return_code_e;

/**
 * @brief  Codeloader return codes for commands that interact with KV storage
 */
typedef enum {
    CODELOADER_COMMAND_KV_STORAGE_RET_ERROR = -1,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_IN_PARAMETERS = -2,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_NOT_ENOUGH_SPACE = -3,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_TRYING_TO_MODIFY_A_PERMANENT_KEY = -4,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_DEFRAGMENTATION_NEEDED = -5,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_CLAIMING_PAGES = -6,
    CODELOADER_COMMAND_KV_STORAGE_ERROR_UNKNOWN = -7,
    CODELOADER_COMMAND_KV_STORAGE_KEY_UNKNOWN = -8,
    CODELOADER_COMMAND_KV_STORAGE_REQUESTED_TO_MUCH_DATA = -9,
} codeloader_command_kv_storage_error_e;

/**
 * @brief  CodeLoader CPU emnumeration
 */
typedef enum {
    CL_CORES_A_BT_CORE = 0, /* !< Security Core Image A. */
    CL_CORES_A_PROTOCOL_CORE = 1, /* !< Protocol Core Image A. */
    CL_CORES_A_APPS_CORE = 2,     /* !< Application Core Image A. */
    CL_CORES_B_BT_CORE = 5, /* !< Security Core Image B. */
    CL_CORES_B_APPS_CORE = 6,     /* !< Application Core Image B. */
    CL_CORES_MAX_NUMBER = 7,      /* !< max core number */
} cl_cores_e;

/**
 * @brief  constructs a general output message with a code and a space for parameters if needed
 * @param  output_buffer buffer pointing to the beginning of the command layer message
 * @param  code code to write in the ack return field. can be a codeloader_command_general_return_code_e
 * or a particular return code < 0
 * @param  params_length length of the parameters
 * @return buffer pointing to the buffer for the parameters field
 */
uint8_t *cl_cmd_proc_utils_cons_gen_out_msg(codeloader_common_generic_buffer_t *output_buffer,
                                            int8_t code, uint16_t params_length);

/**
 * @brief  Returns true if the supplied core image id is valid.
 * @return true if the supplied core image id is a valid core cimage identifier.
 */
bool codeloader_command_processor_utils_cimage_is_valid(core_images_e cimage);

#endif
