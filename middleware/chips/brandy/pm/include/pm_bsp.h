/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power manage BSP layer header file
 */

#ifndef LIB_PM_BSP_H
#define LIB_PM_BSP_H

#include "pm_definition.h"

/**
 * @defgroup middleware_utils_pm_api Pm
 * @ingroup  middleware_utils
 * @{
 */

/**
 * @if Eng
 * @brief  Initializing the PMU Driver.
 * @else
 * @brief  初始化 PMU 驱动
 * @endif
 */
void uapi_pmu_init(void);

/**
 * @if Eng
 * @brief  PMU switch configuration.
 * @param  pwr_id Configurable PMU module ID，see pmu_id_t.
 * @param  config Switch Status
 * @else
 * @brief  PMU 开关配置
 * @param  pwr_id 模块 id.
 * @param  config 电源开关状态
 * @endif
 */
void uapi_pmu_switch(pmu_id_t pwr_id, switch_type_t config);

/**
 * @if Eng
 * @brief  Config the voltage level of a specified power domain.
 * @param  domain Indicates the power domain ID.
 * @param  level Voltage level.
 * @else
 * @brief  配置指定电源域的电压
 * @param  domain 电源域ID
 * @param  level 电压值
 * @endif
 */
void uapi_pmu_vset(pmu_id_t domain, uint8_t level);

/**
 * @if Eng
 * @brief  Obtains the voltage level of a specified power domain.
 * @param  domain Indicates the power domain ID.
 * @param  level Voltage level.
 * @else
 * @brief  获取指定电源域的电压
 * @param  domain 电源域ID
 * @param  level 电压值
 * @endif
 */
void uapi_pmu_vget(pmu_id_t domain, uint8_t *level);

/**
 * @if Eng
 * @brief  Initializing the clocks Driver.
 * @else
 * @brief  初始化时钟驱动
 * @endif
 */
void uapi_clocks_init(void);

/**
 * @if Eng
 * @brief  CLOCKS switch configuration.
 * @param  id Configurable clocks module ID, see clocks_id_t.
 * @param  config Switch Status.
 * @else
 * @brief  配置时钟开关
 * @param  id 模块 id.
 * @param  config 时钟开关状态
 * @endif
 */
void uapi_clocks_switch(clocks_id_t id, switch_type_t config);

/**
 * @if Eng
 * @brief  Configuring the Clock Source and Frequency Divider of a Specified Module.
 * @param  id Configurable PMU module ID.
 * @param  clk_src Switch Status.
 * @param  clk_div Clock divider configuration.
 * @else
 * @brief  配置指定模块的时钟源和分频系数
 * @param  id 模块 id.
 * @param  clk_src 时钟源
 * @param  clk_div 时钟分频系数


 * @endif
 */
void uapi_clocks_config(clocks_id_t id, clocks_src_t clk_src, uint8_t clk_div);

/**
 * @if Eng
 * @brief  Obtains the clock frequency of a specified module.
 * @param  module Clock module ID.
 * @param  freq clock frequency of the module.
 * @else
 * @brief  获取指定模块的时钟频率
 * @param  module 时钟模块 ID
 * @param  freq 模块的时钟频率
 * @endif
 */
void uapi_clocks_get_module_freq(clocks_id_t module, uint32_t *freq);

/**
 * @}
 */

#endif