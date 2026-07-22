/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Low power management interface.
 * Author:
 * Create:  2021-5-25
 */
#ifndef LOW_POWER_MANAGEMENT_H
#define LOW_POWER_MANAGEMENT_H
#include "lpm_dev_ops.h"

/** @defgroup connectivity_libs_connectivity_power_management CONNECTIVITY Power Management
  * @ingroup  connectivity_libs_connectivity
  * @{
  */
/**
 * @brief  Power on/Power off capsensor LDO.
 * @param  on True:power on, false:power off.
 * @return Whether the configuration is OK, for details, refer to lpm_ret_t.
 */
int lpm_capsensor_power_config(bool on);

/**
 * @brief  Get capsensor LDO power status.
 * @return Power status, LPM_RET_ON: power on, LPM_RET_OFF: power off.
 */
int lpm_capsensor_power_status(void);

/**
 * @brief  Power on/Power off adc LDO.
 * @param  on True:power on, false:power off.
 * @return Whether the configuration is OK, for details, refer to lpm_ret_t.
 */
int lpm_adc_power_config(bool on);

/**
 * @brief  Get adc LDO power status.
 * @return Power status, LPM_RET_ON: power on, LPM_RET_OFF: power off.
 */
int lpm_adc_power_status(void);

/**
 * @brief  Enable or disable adc clock.
 * @param  on True: enable, false: disable.
 * @return Whether the configuration is OK, for details, refer to lpm_ret_t.
 */
int lpm_adc_clock_config(bool on);

/**
 * @brief  Get adc clock enable status.
 * @return Clock enable status, LPM_RET_ON: enable, LPM_RET_OFF: disable.
 */
int lpm_adc_clock_status(void);

/**
 * @brief  Get pwm clock enable status.
 * @return Clock enable status, LPM_RET_ON: enable, LPM_RET_OFF: disable.
 */
int lpm_pwm_clock_status(void);

/**
 * @brief  Set adc clock frequency divider number.
 * @param  clk_div Divider number.
 * @return Whether the configuration is OK, for details, refer to lpm_ret_t.
 */
int lpm_adc_set_div_num(int clk_div);

/**
 * @brief  Get adc clock frequency divider number.
 * @return Divider number.
 */
int lpm_adc_get_div_num(void);

/**
 * @brief  Initialize low-power code, register low-power interfaces.
 */
void low_power_management_init(void);

/**
  * @}
  */
#endif