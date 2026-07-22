/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE HEADER.
 * Author: @CompanyNameTag
 * Create: 202-04-13
 */
#ifndef CLOCKS_CORE_H
#define CLOCKS_CORE_H

#include "hal_clocks_app_priv.h"
#include "clocks_types.h"
#include "clocks_config.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks_core
 * @{
 */
typedef void(* sdiom_clk_handler_func)(uint16_t clk_level);

/**
 * @brief  register to hanler the mmc internal freq.
 * @param  func the callback function.
 */
void mmc_internal_freq_handler_register(sdiom_clk_handler_func func);

/**
 * @brief  Get the value of property is_ss_chip.
 * @return Value of the property is_ss_chip.
 */
bool is_ss_chip_get(void);

/**
 * @brief  Set the value of property is_ss_chip.
 * @param  value Value of the property is_ss_chip.
 */
void is_ss_chip_set(bool value);

/**
 * @brief  Set the 0p8 voltage according to the pvsensor.
 */
void clocks_set_0p8_voltage_according_pvsensor(void);

/**
 * @brief  Set 0p8_0v7_level voltage according bt_ipc msg.
 * @param  below_zero_degree    true:<=0°， false:>0°
 */
void set_0p8_voltage_according_tempetature(bool below_zero_degree);

/**
 * @brief  Set 0p8_0v7_level voltage before bt power up.
 */
void set_0p8_voltage_before_bt_power_up(void);

/**
 * @brief  Set 0p8_0v7_level voltage after bt power off.
 */
void set_0p8_voltage_after_bt_power_off(void);

/**
 * @brief  Get the bt clock configuration.
 * @return BT clock source.
 */
clocks_clk_src_t get_bt_clock_config(void);

/**
 * @brief  Mcu clock freq config.
 * @param  clk_level Mcu clock freq level.
 * @return true/false.
 */
bool clocks_set_mcu_freq(system_mcu_freq_t clk_level);

/**
 * @brief  System clocks freq config.
 * @param  clocks_freq clocks freq config level.
 * @param  dsp0_div    dsp0 div config.
 * @param  dsp1_div    dsp1 div config.
 * @return true/false.
 */
bool system_clocks_set_freq(system_clocks_freq_t clocks_freq, uint8_t dsp0_div, uint8_t dsp1_div);

/**
 * @brief  Set dsp status power up/down.
 * @param  slave_cpu Choose dsp or dsp1.
 * @param  status dsp power on or power down.
 */
void set_dsp_power_up_status(slave_cpu_t slave_cpu, bool status);

/**
 * @brief  Get dsp status power up/down.
 * @param  slave_cpu Choose dsp or dsp1.
 * @return true or false.
 */
bool get_dsp_power_up_status(slave_cpu_t slave_cpu);

/**
 * @brief  Dsp freq config.
 * @param  cfg         List that store the configuration of frequency infomation.
 * @param  cfg_size    Configuration list size.
 */
bool clocks_set_dsp_freq(clocks_clk_src_t clk_type, uint8_t clk_div);

/**
 * @brief  Get the dsp frequency.
 * @return Dsp frequency.
 */
dsp_clock_freq_t clocks_get_dsp_freq(void);

/**
 * @brief  Get dsp core freq.
 * @return dsp core freq.
 */
dsp_clock_freq_t get_dsp_core_freq(void);

/**
 * @brief  Get system freq.
 * @return system freq.
 */
system_freq_t get_system_freq(void);

/**
 * @brief  Display module clock config.
 * @param  clk_level Display clock freq level.
 * @return true/false.
 */
bool system_display_set_freq(system_display_freq_t clk_level);

/**
 * @brief  Sdio module clock config.
 * @param  clk_level Sdio clock freq level.
 * @return true/false.
 */
bool clocks_set_sdio_freq(system_sdio_freq_t clk_level);

/**
 * @brief Set bt clocks.
 * @param bt_core_clk Bcpu clock.
 */
void bt_clocks_set_freq(clocks_clk_src_t bt_core_clk);

/**
 * @brief Clock init function used in ssb.
 */
void clocks_ssb_clk_init(void);

/**
 * @brief System Clock init function.
 * @param clock_config system clock config.
 * @return Return clock init success or failed.
 */
bool clocks_app_clk_init(system_clocks_config_t clock_config);

/**
 * @brief  Get spi clock frequency value.
 * @return Return clock of spi frequency.
 */
uint32_t clocks_get_spi_clock_value(void);

/**
 * @brief  Get xip clock frequency value.
 * @return Return clock of xip frequency.
 */
uint32_t clocks_get_xip_clock_value(void);

/**
 * @brief  Clock resume after system exit sleep.
 */
void clocks_system_resume_clock(void);

/**
 * @brief  Switch clock to CLOCKS_CLK_SRC_TCXO before system enter sleep.
 *
 */
void clocks_system_suspend_clock(void);

/**
 * @}
 */
#endif