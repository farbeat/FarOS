/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: adapt sdt
 * This file should be changed only infrequently and with great care.
 */

#ifndef SAMPLE_DATA_ADAPT_H
#define SAMPLE_DATA_ADAPT_H
#include <stdint.h>
#include "errcode.h"
#include "diag.h"
errcode_t diag_cmd_sample_data(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
void zdiag_adapt_sample_data_report(uint8_t *sdt_buf, uint16_t sdt_buf_size);
#endif