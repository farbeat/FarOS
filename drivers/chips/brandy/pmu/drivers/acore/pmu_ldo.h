/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description:  PMU DRIVER HEADER FILE
 */

#ifndef NON_OS_APPLICATION_CORE_PMU_LDO_H
#define NON_OS_APPLICATION_CORE_PMU_LDO_H

#include "chip_io.h"

/** @defgroup connectivity_drivers_non_os_pmu_ldo PMU Ldo
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */

typedef enum {
    /* SIMO region. */
    PMU_SIMO_VDD0P8,
    PMU_SIMO_VDD1P9,
    PMU_SIMO_VDD1P0,
    PMU_SIMO_VDD1P3,
    PMU_SIMO_VDD0P8_BCORE,
    PMU_SIMO_MAX,
} pmu_simo_t;

typedef enum {
    /* ldo region. */
    PMU_LDO_CLDO1,
    PMU_LDO_CLDO2,
    PMU_LDO_CLDO3,
    PMU_LDO_XLDO,
    PMU_LDO_CMU_LDO,
    PMU_LDO_INTLDO1,
    PMU_LDO_INTLDO2,
    PMU_LDO_AUXLDO0,
    PMU_LDO_AUXLDO1,
    PMU_LDO_AUXLDO2,
    PMU_LDO_AUXLDO3,
    PMU_LDO_IOLDO,
    PMU_LDO_MAX,
} pmu_ldo_t;

/**
 * @brief  PMU manual config init.
 */
void pmu_wakeup_manual_config(void);

/**
 * @brief  Hardware wait time init for TCXO clock statble,after wakeup.
 */
void pmu_wakeup_wait_time_init(void);

/**
 * @brief  LDO power on/off.
 * @param  sub      Which ldo select.
 * @param  power_on Turn on/Turn off.
 */
void pmu_ldo_power_on(pmu_ldo_t sub, switch_type_t power_on);

/**
 * @brief  LDO power adjust.
 * @param  sub   Which ldo select.
 * @param  level Voltage level.
 */
void pmu_ldo_power_vset(pmu_ldo_t sub, uint8_t level);

/**
 * @brief  LDO power on/off.
 * @param  sub      Which ldo select.
 * @param  bypass_en Turn on/Turn off.
 */
void pmu_ldo_bypass_en(pmu_ldo_t sub, switch_type_t bypass_en);

/**
 * @brief  VDD0p8 sleep power adjust.
 * @param  level Voltage level.
 */
void pmu_vdd0p8_sleep_power_vset(uint8_t level);

/**
 * @brief  bcore VDD0p8 power adjust.
 * @param  level Voltage level.
 */
void pmu_bcore_vdd0p8_power_vset(uint8_t level);

/**
 * @brief  VDD0p8 power adjust.
 * @param  level Voltage level.
 */
void pmu_vdd0p8_power_vset(uint8_t level);

/**
 * @brief  VDD0p8 power adjust get.
 * @param  level Voltage level.
 */
void pmu_vdd0p8_power_vget(uint8_t *level);

/**
 * @brief  LDO power on/off.
 * @param  sub      Which ldo select.
 * @param  level Voltage level.
 */
void pmu_buck_vset(pmu_simo_t sub, uint8_t level);

/**
 * @brief  CLD1 sleep power on/off.
 * @param  bypass_en Turn on/Turn off.
 */
void pmu_control_cldo1_sleep_bypass(switch_type_t power_on);

/**
  * @}
  */
#endif
