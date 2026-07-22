/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  CLOCKS DRIVER HEADER
 */

#ifndef SRC_DRIVERS_NON_OS_SHARED_CLOCKS_H
#define SRC_DRIVERS_NON_OS_SHARED_CLOCKS_H

#include "std_def.h"
#include "hal_clocks_glb.h"

/** @defgroup connectivity_drivers_non_os_clocks CLOCKS
  * @ingroup  connectivity_drivers_non_os
  * @{
  */

#define RC_1X_DIV_CLK   1  // System RC 1X clock.
#define TCXO_1X_DIV_CLK 1  // System TCXO/1 clock.
#define TCXO_2X_DIV_CLK 2  // System TCXO/2 clock.
#define PLL_2X_DIV_CLK  2  // clock is PLL CLK 1/2 clkdiv
#define PLL_3X_DIV_CLK  3  // clock is PLL CLK 1/3 clkdiv
#define PLL_4X_DIV_CLK  4  // clock is PLL CLK 1/4 clkdiv
#define PLL_5X_DIV_CLK  5  // clock is PLL CLK 1/5 clkdiv
#define PLL_6X_DIV_CLK  6  // clock is PLL CLK 1/6 clkdiv
#define PLL_9X_DIV_CLK  9  // clock is PLL CLK 1/9 clkdiv
#define PLL_10X_DIV_CLK 10 // clock is PLL CLK 1/10 clkdiv
#define PLL_12X_DIV_CLK 12 // clock is PLL CLK 1/12 clkdiv
#define PLL_15X_DIV_CLK 15 // clock is PLL CLK 1/15 clkdiv
#define PLL_18X_DIV_CLK 18 // clock is PLL CLK 1/18 clkdiv

/**
 * @brief  Initialize the clocks with the given configuration.
 */
void clocks_init(void);

/**
 * @brief  Init some hardware confing about low power.
 */
void clocks_hardware_sub_init(void);

/**
 * @brief  Return the value of the tcxo clock.
 * @return value of the tcxo clock in Hz.
 */
uint32_t clocks_get_tcxo_clock(void);

/**
 * @brief  Auto cg config.
 */
void clocks_auto_cg_config(void);

/**
 * @brief  Clocks bridging enter low power config.
 * @param  on  true:enter lp, false:exit lp.
 */
void clocks_bridging_low_powerp_config(bool on);

/**
 * @brief  Return the value of the core clock.
 * @return value of the core clock in Hz.
 * @note   the value returned is the mid one between the maximum limit and lower not the instant one.
 */
uint32_t clocks_get_core_frequency(void);

/**
 * @brief  system mcu all perip clocks config.
 */
void system_mcu_clocks_config(void);

/**
  * @}
  */
#endif
