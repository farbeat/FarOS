/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Codeloader efuse header file \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-04, Create file. \n
 */

#ifndef SRC_APPLICATION_CODELOADER_EFUSE_H
#define SRC_APPLICATION_CODELOADER_EFUSE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief  Codeloader function processor for write efuse
 * @param  input_buffer_to_write --the buffer to write
 * @param  bytes_to_write buffer length
 */
bool codeloader_efuse_set_byte(const uint8_t *input_buffer_to_write, uint32_t bytes_to_write);
#endif