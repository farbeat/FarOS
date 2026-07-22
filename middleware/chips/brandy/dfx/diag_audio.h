/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * Description: adapt sdt
 * This file should be changed only infrequently and with great care.
 * Author: audio
 * Create: 2024-06-06
 */

#ifndef DIAG_AUDIO_H
#define DIAG_AUDIO_H
#include <stdint.h>
#include "errcode.h"
#include "diag.h"

#define NV_AUDIO_VQE_PARAM_SIZE 48

typedef struct {
    uint32_t size;
    uint32_t data[NV_AUDIO_VQE_PARAM_SIZE];
} audio_vqe_param_t;

errcode_t diag_see_param_save(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
errcode_t diag_see_param_trans(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option);
#endif