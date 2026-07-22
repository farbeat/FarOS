/*
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description:  BT PMU DRIVER HEADER FILE
 * Author: @CompanyNameTag
 * Create: 2019-12-19
 */

#ifndef HAL_PMU_PERIPHERAL_BT_H
#define HAL_PMU_PERIPHERAL_BT_H
#include "hal_pmu_peripheral.h"
#include "chip_io.h"

/** @addtogroup  connectivity_drivers_hal_pmu_peripheral
  * @{
  */
/**
  * @brief  Get bcpu peripheral work status.
  * @param  perip        The peripheral to config
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_bcpu_perip_get_status(hal_pmu_bcpu_perips_type_t perip);

/**
  * @brief  Config bcpu peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_bcpu_perip_config(hal_pmu_bcpu_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Get bcpu alwasy on peripheral work status.
  * @param  perip        The peripheral to config
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_baon_perip_get_status(hal_pmu_baon_perips_type_t perip);

/**
  * @brief  Config bcpu alwasy on peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_baon_perip_config(hal_pmu_baon_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Config bt watchdog reset/dereset.
  * @param  on        bt watchdog reset/dereset.
  */
void hal_pmu_bwdt_rst_control(switch_type_t on);

/**
  * @brief  Config b gpio clock.
  * @param  on      open/close
  */
void hal_pmu_bgpio_low_power_config(switch_type_t on);

/**
  * @}
  */
#endif
