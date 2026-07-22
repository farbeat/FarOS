/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:HAL CPU CLOCK DRIVER HEADER.
 * Author: @CompanyNameTag
 * Create: 2020-01-13
 */

#ifndef HAL_CPU_CLOCK_H
#define HAL_CPU_CLOCK_H
#include "clocks.h"
#include "hal_clocks_app.h"
#include "hal_clocks_glb.h"
/** @defgroup connectivity_drivers_hal_clock CLOCK
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/**
 * @brief  Config OPI_XIP group clock source and clock div.
 * @param  clk_source OPI_XIP normal clock source.
 * @param  div Config clk div.
 * @return None
 */
void hal_xip_opi_clock_switch(hal_clocks_ccrg_mcu_group_clk_src_t clk_source, uint8_t div);

/**
 * @brief  Config aon sub clock source and clock div.
 * @param  clksel aon sub clock source.
 * @param  aon_bus_div Config aon bus clk div.
 * @param  aon_com_div Config aon com clk div.
 * @param  hf_div Config aon hf clk div.
 * @return None
 */
void hal_aon_sub_rc_tcxo_clock_switch(hal_clocks_aon_clk_t clksel, uint8_t aon_bus_div,
                                      uint8_t aon_com_div, uint8_t hf_div);

/**
 * @brief  Config aon sub 32K clock source.
 * @param  clksel aon sub 32K clock source.
 * @return None
 */
void hal_aon_sub_32k_clock_switch(switch_type_t clksel);

/**
 * @brief  bt clock switch to 1x TCXO clk.
 * @return None
 */
void hal_cpu_bt_switch_to_tcxo_1x(void);

/**
 * @brief  bt clock switch to 2x TCXO clk.
 * @return None
 */
void hal_cpu_bt_switch_to_tcxo_2x(void);

/**
  * @}
  */
#endif
