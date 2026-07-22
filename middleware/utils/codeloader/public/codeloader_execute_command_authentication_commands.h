/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  BT CODELOADER AUTHENTICATION COMMANDS MODULE.
 * Author:
 * Date: 2020-07-13
 */

#ifndef CODELOADER_EXECUTE_COMMAND_AUTHENTICATION_COMMANDS_H
#define CODELOADER_EXECUTE_COMMAND_AUTHENTICATION_COMMANDS_H

#include "codeloader_common.h"
/*
 * Signature Types & Keystore enumeration
 */
typedef enum {
    SIGNATURE_TYPE_SHA256 = 0,
    SIGNATURE_TYPE_DIEID = 1,
    SIGNATURE_TYPE_RSA4096 = 2,
} signature_type_e;

/**
 * @brief  Adds a signature to Security KV Store
 * @param  input_buffer buffer with the command parameters
 * @param  output_buffer output buffer to send
 */
int8_t codeloader_command_processor_add_signature(
    const codeloader_rx_buffer_t *input_buffer, codeloader_common_generic_buffer_t *output_buffer);

#endif