/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm lpc header file
 */

#ifndef SRC_LIB_PM_LPC_H
#define SRC_LIB_PM_LPC_H

#include "chip_io.h"

/** PMU_LPC definitions */
typedef enum {
    LPC_EVT_ACTION,
    LPC_INT_ACTION,
} lpc_action_type_t;

typedef enum {
    LPC_TYPE_WAKEUP,
    LPC_TYPE_SLEEP,
} lpc_type_t;

void lpc_lightsleep_config(void);

void lpc_deepsleep_config(void);

void lpc_wfi(void);

void lpc_allow_deepsleep(void);

void lpc_forbid_deepsleep(void);

bool lpc_system_sleep_history(void);

void lpc_system_sleep_history_clear(void);

void lpc_wakeup_all_status_clear(lpc_action_type_t type);

uint16_t lpc_wakeup_get_all_status(lpc_action_type_t type);

void lpc_sleep_all_status_clear(lpc_action_type_t type);

uint16_t lpc_sleep_get_all_status(lpc_action_type_t type);

void lpc_aon_sleep_prepare(void);
#endif
