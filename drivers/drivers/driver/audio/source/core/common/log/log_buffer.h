/*
 * Copyright (c) CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: header of log buffer
 * Author: audio
 * Create: 2022-10-15
 */

#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include <stdint.h>
#include "log_memory_section.h"

typedef enum {
    LOG_RET_OK,
    LOG_RET_ERROR_IN_PARAMETERS,
    LOG_RET_ERROR_NOT_ENOUGH_SPACE,
    LOG_RET_ERROR_CORRUPT_SHARED_MEMORY,
    LOG_RET_ERROR_OVERFLOW
} log_ret_e;

typedef struct {
    td_u32 time_us; /* time when the logging was generated */
    td_u16 length;  /* log message length */
} __attribute__((packed)) log_buffer_header;

void log_buffer_write(const log_buffer_header *lb_header, const td_u8 *buffer, td_bool *was_empty);

log_ret_e log_buffer_get_available_for_next_message(td_u32 *av);

td_void log_buffer_init(void);

#endif
