/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for active noise cancellation
 * Author: audio
 */

#ifndef __ANC_STRUCT_H__
#define __ANC_STRUCT_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ANC_REG_SIZE 1024

/* AHA mode */
typedef enum {
    AHA_MODE_ANC_OFF = 0,
    AHA_MODE_ANC_ON,
    AHA_MODE_AH,
    AHA_MODE_HT,

    AHA_MODE_MAX,
} aha_mode;

typedef struct {
    td_u32 offset;
    td_u32 value;
} anc_reg;

typedef struct {
    aha_mode aha_mode;
    anc_reg anc_reg[ANC_REG_SIZE];
} anc_config;

typedef struct {
    aha_mode aha_mode;
    td_s8 *config;
    td_u32 config_len;
} anc_cfg;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __ANC_STRUCT_H__ */
