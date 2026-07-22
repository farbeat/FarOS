/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  PMU DRIVER Header
 */

#ifndef PMU1_PG_H
#define PMU1_PG_H

#include "hal_pmu_pg.h"
#include "chip_io.h"

/** @defgroup connectivity_drivers_non_os_pmu_pg PMU Page Gating
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */

typedef enum {
    PMU_PG_SUB_B_SOC,          /*!< B_SOC. */
    PMU_PG_SUB_BT_CBB,         /*!< BT_CBB. */
    PMU_PG_SUB_B_MEM,          /*!< B_MEM. */
    PMU_PG_SUB_BT_EM,          /*!< BT_EM. */
    PMU_PG_SUB_MCU_HS,         /*!< MCU_HS. */
    PMU_PG_SUB_DSUB,           /*!< DSUB. */
    PMU_PG_SUB_ASUB,           /*!< ASUB. */
    PMU_PG_SUB_S_EGPIO,        /*!< S_EGPIO. */
    PMU_PG_SUB_BT_RF,          /*!< BT_RF. */
    PMU_PG_SUB_MCU,            /*!< MCU. */
    PMU_PG_SUB_MEM,            /*!< MEM. */
    PMU_PG_SUB_COM,            /*!< COM. */
    PMU_PG_SUB_L2M_GMMU,       /*!< L2M_GMMU. */
    PMU_PG_SUB_L2M_DSP,        /*!< L2M_DSP. */
    PMU_PG_SUB_XIP_MEM,        /*!< XIP_MEM. */
    PMU_PG_SUB_BMEM_RAM,       /*!< BMEM RAM. */
    PMU_PG_SUB_MAX,
} pmu_pg_sub_t;

/**
 * @brief Power on the sub module.
 * @param sub      sub system to be configured.
 * @param power_on Turn on/off.
 */
void pmu1_dig_pg_sub_power_on(pmu_pg_sub_t sub, switch_type_t power_on);

/**
 * @brief DSP l2m sharemode config.
 * @param size     Share Mode value.
 */
void pmu1_dsp_l2ram_config(uint8_t size);

/**
 * @brief BRAM sharemode config.
 * @param size     Share Mode value.
 */
void pmu_bt_ram_config(uint8_t size);

/**
  * @}
  */
#endif