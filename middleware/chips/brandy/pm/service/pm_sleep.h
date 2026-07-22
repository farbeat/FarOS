/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power sleep header file
 */

#ifndef LIB_PM_SLEEP_H
#define LIB_PM_SLEEP_H

#include "pm.h"
#include "chip_io.h"

/**
 * @defgroup connectivity_libs_pm PM_SERVICE
 * @ingroup  connectivity_libs
 * @{
 */

enum {
    PM_SLEEP_LS,
    PM_SLEEP_DS,
    PM_SLEEP_MAX,
};

typedef struct {
    uint16_t last_veto_count;
    uint16_t last_veto_id;
} sleep_veto_t;

typedef struct {
    uint16_t slp_event;
    uint16_t wkup_event;
} sleep_event_t;

typedef struct {
    uint64_t total_slp_time  : 48;
    uint64_t total_slp_count : 16;
} sleep_history_t;

typedef struct {
    uint64_t        sleep_base_time;
    sleep_veto_t    sleep_veto;
    sleep_event_t   event;
    sleep_history_t sleep_history[PM_SLEEP_MAX];
} sleep_info_t;

sleep_info_t *pm_get_sleep_info(void);

void pm_set_sleep_threshold(uint16_t ms);
bool hal_ipc_get_core_clr_sts(cores_t core);
void pm_enter_sleep(uint32_t sleep_ms_val);
void pm_set_mcu_dynamic_power_flag(uint8_t flag);
#endif