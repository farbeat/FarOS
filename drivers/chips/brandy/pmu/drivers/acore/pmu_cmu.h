/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  BT PMU DRIVER HEADER FILE
 */

#ifndef NON_OS_PMU_APPLICATION_PMU_CMU_H
#define NON_OS_PMU_APPLICATION_PMU_CMU_H

#include "chip_io.h"
#include "chip_definitions.h"
#include "platform_core.h"
#include "hal_pmu_cmu.h"

/** @defgroup connectivity_drivers_non_os_pmu_cmu PMU CMU
  * @ingroup  connectivity_drivers_non_os_pmu
  * @{
  */
typedef enum {
    BUCK1_OCP_GRM_INT           = 0,
    BUCK2_OCP_GRM_INT           = 1,
    OCP_PALDO_GRM_INT           = 2,
    BUCK1_SCP_GRM_INT           = 3,
    BUCK2_SCP_GRM_INT           = 4,
    UNLOCK_GRM_INT              = 5,
    OVP_OUT_GRM_INT             = 6,
    UVLO_OUT_GRM_INT            = 7,
    IOLDO_OCP_GRM_INT           = 8,
} pmu_ana_sts_raw_int_t;

typedef enum {
    PMU_ANA_BUCK1_OCP_GRM       = 0,
    PMU_ANA_BUCK2_OCP_GRM       = 1,
    PMU_ANA_OCP_PALDO_GRM       = 2,
    PMU_ANA_BUCK1_SCP_GRM       = 3,
    PMU_ANA_BUCK2_SCP_GRM       = 4,
    PMU_ANA_UNLOCK_GRM          = 5,
    PMU_ANA_OVP_OUT_GRM         = 6,
    PMU_ANA_UVLO_OUT_GRM        = 7,
    PMU_ANA_IOLDO_OCP_GRM       = 8,
} pmu_ana_sts_grm_int_t;

typedef enum {
    PMU_DLL2_MCU,
    PMU_DLL2_DSP,
    PMU_DLL2_OPI,
    PMU_DLL2_SDIO,
    PMU_DLL2_MCU_LS,
    PMU_DLL2_MAX,
} pmu_dll2_module_t;

typedef struct {
    uint16_t pll_fbdiv;
    uint16_t pll_frac_h;
    uint16_t pll_frac_l;
    uint8_t pll_postdiv1;
    uint8_t pll_postdiv2;
} cmu_pll_config_t;

/**
 * @brief Get pll used or not.
 * @return Return true if pll is used, false if not used.
 */
bool pmu_cmu_pll_used_get(void);

/**
 * @brief  Enable/Disable raw interrupt.
 * @param  ana_sts_raw_int Config which interrupt.
 * @param  int_en Enable/Disable interrupt.
 */
void pmu_cmu_analog_status_raw_interrupt_enable(pmu_ana_sts_raw_int_t ana_sts_raw_int,
                                                switch_type_t int_en);

/**
 * @brief  Enable/Disable grm interrupt.
 * @param  ana_sts_grm_int Config which interrupt.
 * @param  int_en Enable/Disable interrupt.
 */
void pmu_cmu_analog_status_grm_interrupt_enable(pmu_ana_sts_grm_int_t ana_sts_grm_int,
                                                switch_type_t int_en);

/**
 * @brief  Cmu pll init and enable.
 */
void pmu_cmu_init(void);

/**
 * @brief  Cmu pll reinit.
 * @param  pll_cfg  PLL config.
 */
void pmu_cmu_pll_reinit(pmu_cmu_core_t pll, cmu_pll_config_t pll_cfg);

/**
 * @brief  Cmu pll power off.
 */
void pmu_cmu_pll_deinit(pmu_cmu_core_t pll);

/**
 * @brief  Get pmu cmu pll status.
 * @return bool
 */
bool pmu_cmu_get_pll_status(void);

/**
 * @brief  Init set pmu cmu pll status.
 * @param  used      PLL used or not..
 */
void pmu_cmu_init_pll_status(bool used);

/**
 * @brief  Cmu txco init.
 */
void pmu_cmu_tcxo_init(void);

/**
 * @brief  Adjust PLL clocks.
 * @param  step     PLL adjust step num.
 */
void pmu_cmu_pll_step(int32_t step);

/**
 * @brief  Register pll adjust ipc callback.
 */
void hal_lpc_init(void);

/**
 * @brief  Get pll config value.
 * @param  pll_core  Which PLL.
 * @return PLL clock value.
 */
uint32_t pmu_cmu_get_pll_clock_value(void);

/**
 * @brief Enable pll unlock interrupt.
 */
void pmu_pll_unlock_interrupt_enable(void);

/**
 * @brief Disable pll unlock interrupt.
 */
void pmu_pll_unlock_interrupt_disable(void);

/**
  * @}
  */
#endif
