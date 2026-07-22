/**
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: dump
 * Author:
 * Create:
 */
#ifndef EXCEPTION_DUMP_H
#define EXCEPTION_DUMP_H
#include "vectors.h"

/**
 * @brief  support nandflash storage dump sysmem to file.
 * @return void.
 */
void dump_all_to_file(void);

void printcpu_tracemem(void);

void write_trace(exc_context_t *exc_buff_addr);

void dump_norflash_trace_to_file(void);

#endif