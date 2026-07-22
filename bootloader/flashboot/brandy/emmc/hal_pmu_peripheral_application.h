/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  APP PMU DRIVER HEADER FILE
 * Author:
 * Create: 2020-01-13
 */
#ifndef HAL_PMU_PERIPHERAL_APPLICATION_H
#define HAL_PMU_PERIPHERAL_APPLICATION_H
#include "hal_pmu_peripheral.h"
#include "chip_io.h"
/** @defgroup connectivity_drivers_hal_pmu_peripheral_app PMU Peripheral
  * @ingroup  connectivity_drivers_hal
  * @{
  */
typedef struct {
    uint16_t m_top0_l;
    uint16_t m_top0_h;
    uint16_t m_top1_l;
    uint16_t m_top1_h;
    uint16_t m_top2_l;
    uint16_t m_top2_h;
    uint16_t m_top3_l;
    uint16_t m_top3_h;
    uint16_t m_top4_l;
    uint16_t m_top4_h;
    uint16_t m_top5_l;
    uint16_t m_top5_h;
    uint16_t m_clocken0;
    uint16_t m_clocken1;
    uint16_t m_clocken2;
    uint16_t m_glb_clocken;
    uint16_t m_perp_ls_cr;
} hal_pmu_m_clocks_state_t;

/**
  * @brief  Get mcpu peripheral work status.
  * @param  perip        The peripheral to config
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_mcpu_perip_get_status(hal_pmu_mcpu_perips_type_t perip);

/**
  * @brief  Reset mcpu peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_mcpu_perip_reset(hal_pmu_mcpu_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Config mcpu peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_mcpu_perip_config(hal_pmu_mcpu_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Get mcpu alwasy on peripheral work status.
  * @param  perip        The peripheral to config
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_maon_perip_get_status(hal_pmu_maon_perips_type_t perip);

/**
  * @brief  Config mcpu alwasy on peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_maon_perip_config(hal_pmu_maon_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Get mem sub peripheral work status.
  * @param  perip        The peripheral to config
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_mem_perip_get_status(hal_pmu_mem_perips_type_t perip);

/**
  * @brief  Config mem sub peripheral.
  * @param  perip        The peripheral to config
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_mem_perip_config(hal_pmu_mem_perips_type_t perip, hal_pmu_perip_request_type_t request);

/**
  * @brief  Get dsp cpu always on gpio peripheral work status.
  * @return HAL_PMU_PERIP_STATUS        The peripheral work status.
  */
hal_pmu_perip_status_type_t hal_pmu_daon_perip_get_status(void);

/**
  * @brief  Config dsp cpu alwyas on gpio peripheral.
  * @param  request      The request to operate pheripheral
  */
void hal_pmu_daon_perip_config(hal_pmu_perip_request_type_t request);

/**
  * @brief  Config m gpio clock.
  * @param  on      open/close
  */
void hal_pmu_mgpio_low_power_config(switch_type_t on);

/**
  * @brief  Get peripheral clcok state in abnormal.
  */
void hal_pmu_get_perip_state(void);

/**
  * @}
  */
#endif
