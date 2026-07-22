/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  HAL PMU VDD IO DRIVER INTERFACE
 */

#ifndef HAL_PMU_VDD_H
#define HAL_PMU_VDD_H

#include <stdint.h>
#include "chip_io.h"

/** @defgroup connectivity_drivers_hal_pmu_vdd PMU VDD
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */

typedef enum {
    HAL_PMU_VDD0P8,
    HAL_PMU_VDD1P0,
    HAL_PMU_VDD1P3,
    HAL_PMU_VDD1P9,
} hal_pmu_vdd_t;

/* simo1_vo2&simo2_vo2 */
typedef enum {
    HAL_PMU_VDD0P8_1P0_0P600V    = 0,
    HAL_PMU_VDD0P8_1P0_0P625V    = 1,
    HAL_PMU_VDD0P8_1P0_0P650V    = 2,
    HAL_PMU_VDD0P8_1P0_0P675V    = 3,
    HAL_PMU_VDD0P8_1P0_0P700V    = 4,
    HAL_PMU_VDD0P8_1P0_0P725V    = 5,
    HAL_PMU_VDD0P8_1P0_0P750V    = 6,
    HAL_PMU_VDD0P8_1P0_0P775V    = 7,
    HAL_PMU_VDD0P8_1P0_0P800V    = 8,
    HAL_PMU_VDD0P8_1P0_0P825V    = 9,
    HAL_PMU_VDD0P8_1P0_0P850V    = 10,
    HAL_PMU_VDD0P8_1P0_0P875V    = 11,
    HAL_PMU_VDD0P8_1P0_0P900V    = 12,
    HAL_PMU_VDD0P8_1P0_0P925V    = 13,
    HAL_PMU_VDD0P8_1P0_0P950V    = 14,
    HAL_PMU_VDD0P8_1P0_0P975V    = 15,
    HAL_PMU_VDD0P8_1P0_1P000V    = 16,
    HAL_PMU_VDD0P8_1P0_1P025V    = 17,
    HAL_PMU_VDD0P8_1P0_1P050V    = 18,
    HAL_PMU_VDD0P8_1P0_1P075V    = 19,
    HAL_PMU_VDD0P8_1P0_1P100V    = 20,
} hal_pmu_vdd0p8_1p0_level_t;

/* simo1_vo1&simo2_vo1 */
typedef enum {
    HAL_PMU_VDD1P9_1P3_0P800V    = 0,
    HAL_PMU_VDD1P9_1P3_0P850V    = 2,
    HAL_PMU_VDD1P9_1P3_0P900V    = 4,
    HAL_PMU_VDD1P9_1P3_0P950V    = 6,
    HAL_PMU_VDD1P9_1P3_1P000V    = 8,
    HAL_PMU_VDD1P9_1P3_1P050V    = 10,
    HAL_PMU_VDD1P9_1P3_1P100V    = 12,
    HAL_PMU_VDD1P9_1P3_1P150V    = 14,
    HAL_PMU_VDD1P9_1P3_1P200V    = 16,
    HAL_PMU_VDD1P9_1P3_1P250V    = 18,
    HAL_PMU_VDD1P9_1P3_1P300V    = 20,
    HAL_PMU_VDD1P9_1P3_1P350V    = 22,
    HAL_PMU_VDD1P9_1P3_1P400V    = 24,
    HAL_PMU_VDD1P9_1P3_1P450V    = 26,
    HAL_PMU_VDD1P9_1P3_1P500V    = 28,
    HAL_PMU_VDD1P9_1P3_1P550V    = 30,
    HAL_PMU_VDD1P9_1P3_1P600V    = 32,
    HAL_PMU_VDD1P9_1P3_1P650V    = 34,
    HAL_PMU_VDD1P9_1P3_1P700V    = 36,
    HAL_PMU_VDD1P9_1P3_1P750V    = 38,
    HAL_PMU_VDD1P9_1P3_1P800V    = 40,
    HAL_PMU_VDD1P9_1P3_1P850V    = 42,
    HAL_PMU_VDD1P9_1P3_1P900V    = 44,
    HAL_PMU_VDD1P9_1P3_1P950V    = 46,
    HAL_PMU_VDD1P9_1P3_2P000V    = 48,
} hal_pmu_vdd1p9_1p3_level_t;

typedef enum {
    CLDO1_VSET,
    EN_CLDO1_SW,
    SIMO_VO2_VSET,
    PMU2_SLEEP,
    PMU_READY_M,
    CLDO3_VSET,
    EN_CLDO3_SW,
    PMU_READY_B,
    CLDO2_VSET,
    EN_CLDO2_SW,
} hal_pmu_dbg_sel_t;

typedef enum {
    ULP_AON_VDD1P9_SW_OFF_BOOT_TIME,
    ULP_AON_SLEEP_N_BOOT_TIME,
    ULP_AON_EN_REF_BG1_BOOT_TIME,
    ULP_AON_VDD0P7_TO_SYS_ISO_EN_BOOT_TIME,
    ULP_AON_RST_BOOT_32K_N_BOOT_TIME,
    ULP_AON_ULP_WKUP_AON_BOOT_TIME,
    ULP_AON_POWER_ON_MAX,
} ulp_aon_power_on_t;

typedef enum {
    ULP_AON_RST_BOOT_32K_N_WKUP_TIME,
    ULP_AON_ULP_WKUP_AON_WKUP_TIME,
    ULP_AON_ULP_SLP_HLD_TIME,
    ULP_AON_VDD1P9_SW_OFF_SLP_TIME_SOC,
    ULP_AON_SLEEP_N_SLP_TIME_SOC,
    ULP_AON_EN_REF_BG1_SLP_TIME_SOC,
    ULP_AON_VDD0P7_TO_SYS_ISO_EN_SLP_TIME_SOC,
    ULP_AON_RST_BOOT_32K_N_SLP_TIME_SOC,
    ULP_AON_ULP_WKUP_AON_SLP_TIME_SOC,
    ULP_AON_WAKEUP_MAX,
} ulp_aon_wakeup_t;

typedef enum {
    PMU_VSET_MCORE,
    PMU_VSET_BCORE,
} pmu_vset_core_t;

/**
 * @brief  PMU SIMO BUCK init.
 */
void hal_pmu_simo_buck_init(void);

void hal_pmu_vdd_enable(hal_pmu_vdd_t vdd, switch_type_t type);

/**
 * @brief  Config vdd0p8 normal voltage.
 * @param  level vdd0p8 voltage level.
 */
void hal_pmu_vdd0p8_set_nor_voltage(hal_pmu_vdd0p8_1p0_level_t level);

/**
 * @brief  Config vdd0p8 sleep voltage.
 * @param  level vdd0p8 voltage level.
 */
void hal_pmu_vdd0p8_set_slp_voltage(hal_pmu_vdd0p8_1p0_level_t level);

/**
 * @brief  Requset vdd0p8 voltage adjustment.
 * @param  type which core.
 */
void hal_pmu_vdd0p8_vset_requset(pmu_vset_core_t type);

/**
 * @brief  Get vdd0p8 vset requst ack.
 * @param  type which core.
 */
uint8_t hal_pmu_vdd0p8_vset_ack_get(pmu_vset_core_t type);

/**
 * @brief  Clear vdd0p8 vset requst ack.
 * @param  type which core.
 */
void hal_pmu_vdd0p8_vset_ack_clr(pmu_vset_core_t type);

/**
 * @brief  set the voltage of 1p9
 * @param  level        the voltage
 */
void hal_pmu_vdd1p9_set_voltage(hal_pmu_vdd1p9_1p3_level_t level);

/**
 * @brief  set the voltage of 1p0
 * @param  level        the voltage
 */
void hal_pmu_vdd1p0_set_voltage(hal_pmu_vdd0p8_1p0_level_t level);

/**
 * @brief  set the voltage of 1p3
 * @param  level        the voltage
 */
void hal_pmu_vdd1p3_set_voltage(hal_pmu_vdd1p9_1p3_level_t level);

void hal_pmu_dbg_select(switch_type_t dbg_switch);

void hal_pmu_dbg_select_control(hal_pmu_dbg_sel_t pmu_sel_control, switch_type_t select_en);

/**
 * @brief  set the voltage of 0p8 in bcore
 * @param  level        the voltage
 */
void hal_pmu_bcore_vdd0p8_set_nor_voltage(hal_pmu_vdd0p8_1p0_level_t level);

/**
 * @brief  ULP power on time config
 * @param  ulp_power    Which power module.
 * @param  time_value   Power on time config.
 */
void hal_ulp_power_on_time_config(ulp_aon_power_on_t ulp_power, uint16_t time_value);

/**
 * @brief  ULP wakeup time config.
 * @param  ulp_wakeup    Which module.
 * @param  time_value    Wakeup time config.
 */
void hal_ulp_wakeup_time_config(ulp_aon_wakeup_t ulp_wakeup, uint16_t time_value);

/**
  * @}
  */
#endif
