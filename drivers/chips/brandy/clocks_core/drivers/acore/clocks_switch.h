/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SYSTEM CLOCKS SWITCH DRIVER
 * Author: @CompanyNameTag
 * Create: 2020-7-28
 */

#ifndef CLOCKS_SWITCH_H
#define CLOCKS_SWITCH_H

#include <stdint.h>
#include "clocks_switch_priv.h"
#include "pmu_cmu.h"
#include "hal_clocks_app_priv.h"

/** @addtogroup connectivity_drivers_non_os_clocks_core
  * @{
  */
typedef void (* clock_switch_proc_func)(clocks_clk_src_t clk_src, uint8_t clk_div);

/**
 * @brief  Get the current clock config.
 * @return Return the current clock config.
 */
clocks_clk_cfg_t *clocks_get_current_clock_cfg(void);

/**
 * @brief Init the pll vote information.
 */
void clocks_pll_init_vote(void);

/**
 * @brief Add PLL Vote.
 * @param pll_src Pll type.
 * @param module  Clock module to add vote.
 */
void clocks_pll_add_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module);

/**
 * @brief Remove PLl vote.
 * @param pll_src Pll type.
 * @param module Clock module to remove vote.
 */
void clocks_pll_remove_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module);

/**
 * @brief Register clock switch pre handler function for each clock module.
 * @param func The per handler function to register.
 * @param module The clock module selected.
 */
void clock_switch_pre_handler_register(clock_switch_proc_func func, clocks_ccrg_module_t module);

/**
 * @brief config common crg module clock.
 * @param module  The clock module to config clock.
 * @param clk_src The clock source selected.
 * @param clk_div The clock frenquency div number.
 */
void system_ccrg_clock_config(clocks_ccrg_module_t module, clocks_clk_src_t clk_src, uint8_t clk_div);

/**
 * @brief  Translate clocks_clk_src_t type to clocks_pll_src_t.
 * @param clk_src The clock source selected.
 * @return Return clocks_pll_src_t type.
 */
clocks_pll_src_t clocks_pll_veto_type_translation(clocks_clk_src_t clk_src);

/**
 * @brief Translate clocks_clk_src_t type to clocks_pll_src_t.
 * @param clk_src The clock source selected.
 * @return Return pmu_cmu_core_t type.
 */
pmu_cmu_core_t clocks_pll_used_init_type_translation(clocks_clk_src_t clk_src);

/**
 * @brief PLL power on.
 * @param pll_src The pll source selected.
 */
void clocks_pll_power_on(clocks_pll_src_t pll_src);

/**
 * @brief PLL power off.
 * @param pll_src The pll source selected.
 */
void clocks_pll_power_off(clocks_pll_src_t pll_src);

/**
 * @brief Translate type of hal_clocks_ccrg_mcu_group_clk_src_t based on clock module and clock source.
 * @param module  The clock module selected.
 * @param clk_src The clock source selected.
 * @return hal_clocks_ccrg_mcu_group_clk_src_t
 */
hal_clocks_ccrg_mcu_group_clk_src_t clocks_src_sel_type_translation(clocks_ccrg_module_t module,
                                                                    clocks_clk_src_t clk_src);

/**
 * @brief Get clock module frequency.
 * @param module The clock module selected.
 * @return Return the clock frequency of clock module selected.
 */
uint32_t clocks_get_module_frequency(clocks_ccrg_module_t module);

/**
 * @brief Get the clock frequency of pll.
 * @param clk_src The clock source selected.
 * @return Return the pll clock frequency.
 */
uint32_t clocks_get_pll_frequency(clocks_clk_src_t clk_src);

/**
 * @brief Config opi clock frequency.
 * @param clk_src The clock source selected.
 * @param clk_div The clock frenquency div number.
 */
void opi_pre_handler(clocks_clk_src_t clk_src, uint8_t clk_div);

/**
 * @brief Config PerPLs clock Reload.
 * @param on The clock is reload.
 */
void system_ccrg_clock_perpls_set_reload(bool on);

/**
 * @brief all clocks config to the clock source of RC.
 */
void clk_switch_to_rc(void);

/**
  * @}
  */
#endif
