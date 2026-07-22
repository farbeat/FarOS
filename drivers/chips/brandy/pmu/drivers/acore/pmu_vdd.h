/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  PMU DRIVER HEADER FILE.
 */

#ifndef NON_OS_APPLICATION_CORE_PMU_VDD_H
#define NON_OS_APPLICATION_CORE_PMU_VDD_H

#include "chip_io.h"

/** @defgroup connectivity_drivers_non_os_pmu_vdd PMU Vdd
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */
/**
 * @brief  Defines the different VDD_IO voltage regulator levels
 */
typedef enum {
    PMU_LDO_VDD_LEVEL_0P6V = 0,   //!< Set internal LDO regulator to 0.6V
    PMU_LDO_VDD_LEVEL_0P7V = 1,   //!< Set internal LDO regulator to 0.7V
    PMU_LDO_VDD_LEVEL_0P8V = 2,   //!< Set internal LDO regulator to 0.8V
    PMU_LDO_VDD_LEVEL_0P85V = 3,  //!< Set internal LDO regulator to 0.8V
    PMU_LDO_VDD_LEVEL_0P9V = 4,   //!< Set internal LDO regulator to 0.9V
} pmu_ldo_vdd_level_t;

/**
 * @brief  Defines the different VDD_IO banks available
 */
typedef enum {
    PMU_VDD_CLDO1_BANK = 0,   //!< VDD_CLDO1 (BCPU/MCPU)
    PMU_VDD_CLDO2_BANK = 1,   //!< VDD_CLDO2 (DSP)
    PMU_VDD_SYSLDO_BANK = 2,  //!< VDD_SYSLDO (COMMON/AON)
    PMU_VDD_BANK_MAX,
} pmu_vdd_ldo_bank_t;

/**
 * @brief  Set ldo(sysldo/cldo1) voltage when on sleep mode.
 * @return None
 */
void pmu_ldo_set_sleep_voltage(void);

/**
 * @brief  Set ldo(sysldo/cldo1) voltage when on normal mode.
 * @param  bank      cldo1/cldo2/sysldo the power domain
 * @param  level     VDD level
 * @return None
 */
void pmu_ldo_set_normal_voltage(pmu_vdd_ldo_bank_t bank, pmu_ldo_vdd_level_t level);

/**
 * @brief  Get ldo(sysldo/cldo1) voltage when on normal mode.
 * @param  bank      cldo1/cldo2/sysldo the power domain
 * @return level     VDD level
 */
pmu_ldo_vdd_level_t pmu_ldo_get_vdd(pmu_vdd_ldo_bank_t bank);

/**
 * @brief  Init vdd1p0 status.
 * @return None
 */
void pmu_vdd1p0_status_init(void);

/**
 * @brief  Switch vdd1p0 status.
 * @param  en   turn on/turn off.
 * @return None
 */
void pmu_vdd1p0_request_switch(switch_type_t en);

/**
  * @}
  */
#endif
