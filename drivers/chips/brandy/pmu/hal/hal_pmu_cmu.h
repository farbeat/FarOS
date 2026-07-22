/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Pmu and cmu hal driver header file.
 */

#ifndef HAL_PMU_CMU_H
#define HAL_PMU_CMU_H

#include <stdint.h>
#include "chip_io.h"
#include "chip_definitions.h"

/** @defgroup connectivity_drivers_hal_pmu_cmu PMU CMU
  * @ingroup  connectivity_drivers_hal_pmu
  * @{
  */
typedef enum {
    CMU_XO_CORE_PD          = 0,
    CMU_GLB_PD              = 1,
    CMU_CLK_XO_GT           = 2,
    CMU_CLK_TCXO2DBB_GT     = 3,
    CMU_CLK_PLL2CMU_GT      = 4,
    CMU_CLK_PLL2CMU_ISO_EN  = 5,
    CMU_LDO_EN              = 6,
    CMU_XLDO_EN             = 7,
    CMU_CLK_TCXO2DBB_PD     = 8,
    CMU_CLK_TCXO2ANA_PD     = 9,
    CMU_CLK_XO_PLL_GT       = 10,
    CMU_CLK_BT_ADC_GT       = 11,
    CMU_RC2_PD              = 12,
    CMU_RC2_DBB_PD          = 13,
    CMU_RC2_DBB_GT          = 14,
} cmu_cfg_t;

/**
 * @brief  Definition of PMU CMU cores.
 */
typedef enum {
    PMU_CMU_PLL,
    PMU_CMU_AUDIO = PMU_CMU_PLL,
    PMU_CMU_MAX,
} pmu_cmu_core_t;

/**
 * @brief  The pll configuration.
 */
typedef enum {
    CMU_FOUTVCO_PD,
    CMU_FOUTPOSTDIV_PD,
    CMU_FOUT4PHASE_PD,
    CMU_DAC_PD,
    CMU_DSM_PD,
} pmu_cmu_core_pll_cfg_t;

typedef enum {
    CMU_MAN_FAST_XO_ISO = 1,
    CMU_MAN_APERP_32K_SEL,
    CMU_MAN_TCXO_EN,
    CMU_MAN_A32M_CLKEN,
    CMU_MAN_B32M_CLKEN,
    CMU_MAN_RC2_PD,
    CMU_MAN_RC2_RSTN,
    CMU_MAN_RC2_DBB_PD,
    CMU_MAN_RC2_DBB_GT,
    CMU_MAN_RC2_CLKEN,
} hal_cmu_man_sel_t;

/**
 * @brief  CMU normal control(work state).
 * @param  cmu_signal Config signal.
 * @param  select_en Enable/Disable(GT\PD).
 */
void hal_cmu_nor_control(cmu_cfg_t cmu_nor_control, switch_type_t select_en);

/**
 * @brief  CMU sleep control.
 * @param  cmu_signal Config signal.
 * @param  select_en Enable/Disable(GT\PD).
 */
void hal_cmu_slp_control(cmu_cfg_t cmu_slp_control, switch_type_t select_en);

/**
 * @brief  CMU debug signal select.
 * @param  cmu_signal Config signal.
 * @param  en Enable/Disable(GT\PD).
 */
void hal_cmu_dbg_select(switch_type_t dbg_switch);

void hal_cmu_dbg_select_set(uint16_t cmu_sel_set);

void hal_cmu_dbg_select_control(cmu_cfg_t cmu_sel_control, switch_type_t select_en);

/**
 * @brief  CMU signal switch config.
 * @param  cmu_signal Config signal.
 * @param  en Enable/Disable(GT\PD).
 */
void hal_cmu_dbg_config_control(cmu_cfg_t cmu_signal, switch_type_t en);

/**
 * @brief  Config CMU manual select value.
 * @param  cmu_sel_control Which CMU moudle.
 * @param  select_en       Manual value.
 */
void hal_cmu_manual_select_control(hal_cmu_man_sel_t cmu_sel_control, switch_type_t select_en);

/**
 * @brief  Config CMU manual select value.
 * @param  cmu_sel_set CMU manual select value.
 */
void hal_cmu_manual_select_set(uint16_t cmu_sel_set);

/**
 * @brief  Config CMU manual select value.
 * @param  manual_switch Set/Clear CMU manual select default value.
 */
void hal_cmu_manual_select(switch_type_t manual_switch);

/**
 * @brief  Config CMU manual or auto.
 * @param  cmu_man_control Which CMU moudle.
 * @param  manual_en       Manual or auto.
 */
void hal_cmu_manual_config_control(hal_cmu_man_sel_t cmu_man_control, switch_type_t manual_en);

/**
 * @brief  Config CMU manual or auto.
 * @param  cmu_man_set Set CMU manual value.
 */
void hal_cmu_manual_config_set(uint16_t cmu_man_set);

/**
 * @brief  Config CMU manual value.
 * @param  power_switch Set/Clear CMU manual default value.
 */
void hal_cmu_manual_config(switch_type_t power_switch);

/**
 * @brief  Config CMU manual value.
 # @param  cmu_signal Which CMU moudle.
 * @param  manual Set/Clear CMU manual default value.
 */
void hal_cmu_manual_single_config(cmu_cfg_t cmu_signal, switch_type_t manual);
/**
 * @brief  Get ANA status.
 * @return ANA status.
 */
uint16_t hal_pmu_cmu_get_ana_status(void);

/**
 * @brief  Clear ANA status.
 */
void hal_pmu_cmu_clear_ana_int_status(void);

/**
 * @brief  Enable analog status raw interrupt.
 * @param  pos Position of the intterupt.
 */
void hal_pmu_cmu_analog_status_raw_interrupt_enable(uint16_t pos);

/**
 * @brief  Disable analog status raw interrupt.
 * @param  pos Position of the intterupt.
 */
void hal_pmu_cmu_analog_status_raw_interrupt_disable(uint16_t pos);

/**
 * @brief  Config audio pll fbdiv target.
 * @param  div divsion number.
 */
void hal_pmu2_cmu_pll_fbdiv_target_set(uint16_t div);

/**
 * @brief  Config audio pll frac target.
 * @param  frac Frac to set.
 */
void hal_pmu2_cmu_pll_frac_target_set(uint32_t frac);

/**
 * @brief  Set manual config pll fb divsion number.
 * @param  div Divsion number.
 */
void hal_pmu2_cmu_pll_fbdiv_manual_set(uint16_t div);

/**
 * @brief  Get manual config pll fb divsion number.
 * @return Manual config pll fb divsion number.
 */
uint16_t hal_pmu2_cmu_pll_fbdiv_manual_get(void);

/**
 * @brief  Set manual config pll frac divsion number.
 * @param  num_h High divsion number.
 * @param  num_l Low divsion number.
 */
void hal_pmu2_cmu_pll_frac_manual_set(uint16_t num_h, uint16_t num_l);

/**
 * @brief  Get manual config pll frac divsion number.
 * @return CMU manual config pll frac divsion number.
 */
uint32_t hal_pmu2_cmu_pll_frac_manual_get(void);

/**
 * @brief  PLL fbdiv config control select.
 * @param  manual_control   manual control or auto control.
 */
void hal_pmu2_cmu_pll_fbdiv_control_select(switch_type_t manual_control);

/**
 * @brief  PLL frac config control select.
 * @param  manual_control   manual control or auto control.
 */
void hal_pmu2_cmu_pll_frac_control_select(switch_type_t manual_control);

/**
 * @brief  Get PLL lock status.
 */
void hal_pmu2_cmu_pll_lock_check(void);

/**
 * @brief  PLL fbdiv/frac config valid or not.
 * @param  valid          The config valid or not.
 */
void hal_pmu2_cmu_pll_cfg_valid(switch_type_t valid);

/**
 * @brief  Config pll frac step.
 * @param  step_h       high step value.
 * @param  step_l       low step value.
 */
void hal_pmu2_cmu_pll_set_frac_step(uint16_t step_h, uint16_t step_l);

/**
 * @brief  Get pll fbdiv status num.
 * @return FBDIV status num.
 */
uint16_t hal_pmu2_cmu_pll_fbdiv_status(void);

/**
 * @brief  Get pll frac status num.
 * @return FRAC status num.
 */
uint32_t hal_pmu2_cmu_pll_frac_status(void);

/**
 * @brief  Config pll postdiv value.
 * @param  div2  post div2 value.
 * @param  div1  post div1 value.
 */
void hal_pmu2_cmu_pll_post_div_set(uint8_t div2, uint8_t div1);

/**
 * @brief  Config rc2 control value.
 */
void hal_pmu_rc2_control_sel(void);

/**
 * @brief  Config sleep and voltage.
 */
void hal_pmu_slp_cfg_vld_sel(void);

/**
 * @brief  Config simo1 eco.
 */
void hal_pmu_simo1_set(void);

/**
 * @brief  Get pll postdiv value.
 * @param  div2  post div2 value.
 * @param  div1  post div1 value.
 */
void hal_pmu2_cmu_pll_post_div_get(uint8_t *div2, uint8_t *div1);

/**
 * @brief  Config ana grim interrupt.
 */
void hal_pmu_analog_status_grm_interrupt_enable(uint16_t position, switch_type_t on);
/**
  * @}
  */
#endif