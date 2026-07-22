/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides v150 HAL MEM_Monitor \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#ifndef HAL_MEM_MONITOR_V150_H
#define HAL_MEM_MONITOR_V150_H

#include "hal_mem_monitor.h"
#include "hal_mem_monitor_v150_regs_op.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_mem_monitor_v150 MEM_Monitor V150
 * @ingroup  drivers_hal_mem_monitor
 * @{
 */

/**
 * @if Eng
 * @brief  Get the instance of v150.
 * @return The instance of v150.
 * @else
 * @brief  获取v150实例。
 * @return v150实例。
 * @endif
 */
hal_mem_monitor_funcs_t *hal_mem_monitor_v150_funcs_get(void);

/**
 * @if Eng
 * @brief  Handler of the MEM_Monitor interrupt request.
 * @else
 * @brief  MEM_Monitor中断处理函数。
 * @endif
 */
void hal_mem_monitor_irq_handler(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif