/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power veto header file
 */

#ifndef LIB_PM_VETO_H
#define LIB_PM_VETO_H

#include "chip_io.h"
#include "pm.h"

/**
 * @defgroup connectivity_libs_pm PM_SERVICE
 * @ingroup  connectivity_libs
 * @{
 */

typedef struct {
    uint8_t total_counts;
    uint8_t sub_counts[PM_ID_MAX];
} pm_veto_counts_t;

typedef struct {
    pm_veto_counts_t veto_counts;
    uint16_t last_veto_id;
    uint32_t last_veto_lr;
    uint64_t veto_timeout_timestamp; // 超时票的超时时刻
} pm_veto_t;

bool pm_get_sleep_veto(void);

pm_veto_t *pm_veto_get_info(void);

void pm_veto_start_track(uint8_t veto_mask, uint8_t num);

void pm_veto_stop_track(void);

void pm_veto_init(void);

uint8_t pm_veto_get_id_cnt(pm_id_t veto_id);

errcode_t uapi_pm_add_sleep_veto_with_timeout(pm_id_t veto_id, uint32_t timeout_ms);
#endif