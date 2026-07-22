/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  PMU DRIVER HEADER FILE
 */
#ifndef NON_OS_SHARED_PMU_H
#define NON_OS_SHARED_PMU_H

#include "std_def.h"
#include "core.h"

/** @defgroup connectivity_drivers_non_os_pmu_pmu PMU
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */
/**
 * @brief  Initialises the PMU Module.
 * @note   it should give a valid configuration to the PMU that allows going safely into deep sleep.
 */
void pmu_init(void);

/**
 * @brief  Init all interrupts and events,that is disable it.
 */
void pmu_lpm_interrupts_and_events_init(void);

/**
 * @brief  Show some system interrupts inforomation.
 */
void pmu_lpm_interrupts_info_show(void);

/**
 * @brief  Get The whole system sleep history.
 * @return bool       ture/false
 */
bool pmu_system_sleep_history(void);

/**
 * @brief  Clear The whole system sleep history.
 */
void pmu_system_sleep_history_clear(void);

/**
 * @brief  Select pmu rst manual
 */
void pmu_reset_manual_select(void);

#if (CORE == MASTER_BY_ALL)
/**
 * @brief  display power on/off and reset
 */
void pmu_display_power_and_reset_cfg(bool turn_on);

/**
 * @brief  sdio and emmc reset
 */
void pmu_sdio_glb_common_reset(bool turn_on);
#endif
/**
 * @brief  auxldo power on/off
 * @param  turn_on     false: off, true: on
 */
void pmu_auxldo_power_config(bool turn_on);

/**
  * @}
  */
#endif
