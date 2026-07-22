/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides hal MEM_Monitor \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#ifndef HAL_MEM_MONITOR_H
#define HAL_MEM_MONITOR_H

#include <stdint.h>
#include "errcode.h"
#include "mem_monitor_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_mem_monitor_api MEM_Monitor
 * @ingroup  drivers_hal_mem_monitor
 * @{
 */

/**
 * @if Eng
 * @brief  Definition of the MEM_Monitor work mode.
 * @else
 * @brief  MEM_Monitor监控模式定义。
 * @endif
 */
typedef enum hal_mem_monitor_mode {
    HAL_MEM_MONITOR_MODE_READ = 1,         /*!< @if Eng MEM_Monitor mode: read mode.
                                                @else    MEM_Monitor模式：监控读模式。  @endif */
    HAL_MEM_MONITOR_MODE_WRITE             /*!< @if Eng MEM_Monitor mode: write mode.
                                                @else    MEM_Monitor模式：监控写模式。  @endif */
} hal_mem_monitor_mode_t;

/**
 * @if Eng
 * @brief  MEM_Monitor callback functions.
 * @param  [in]  addr MEM_Monitor Monitoring Address.
 * @param  [in]  mode MEM_Monitor Monitor the read/write status.
 * @else
 * @brief  MEM_Monitor 回调类型定义。
 * @param  [in]  addr MEM_Monitor监控地址。
 * @param  [in]  mode MEM_Monitor监控读写状态。
 * @endif
 */
typedef void (*hal_mem_monitor_callback_t)(uint32_t addr, hal_mem_monitor_mode_t mode);

/**
 * @if Eng
 * @brief  Initialize device for HAL MEM_Monitor.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t .
 * @else
 * @brief  HAL层MEM_Monitor的初始化接口。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
typedef errcode_t (*hal_mem_monitor_init_t)(void);

/**
 * @if Eng
 * @brief  Deinitialize device for HAL MEM_Monitor.
 * @else
 * @brief  HAL层MEM_Monitor的去初始化接口。
 * @endif
 */
typedef void (*hal_mem_monitor_deinit_t)(void);

/**
 * @if Eng
 * @brief  Enable Memory monitoring.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @else
 * @brief  使能MEM_Monitor内存监控。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @endif
 */
typedef void (*hal_mem_monitor_enable_t)(mem_monitor_t monitor);

/**
 * @if Eng
 * @brief  Disable Memory monitoring.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @else
 * @brief  去使能MEM_Monitor内存监控。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @endif
 */
typedef void (*hal_mem_monitor_disable_t)(mem_monitor_t monitor);

/**
 * @if Eng
 * @brief  Enable MEM_Monitor clock.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @else
 * @brief  HAL层MEM_Monitor的时钟使能。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @endif
 */
typedef void (*hal_mem_monitor_clock_enable_t)(mem_monitor_t monitor);

/**
 * @if Eng
 * @brief  Disable MEM_Monitor clock.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @else
 * @brief  HAL层MEM_Monitor的时钟使能。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @endif
 */
typedef void (*hal_mem_monitor_clock_disable_t)(mem_monitor_t monitor);

/**
 * @if Eng
 * @brief  Setting the Monitoring Address Segment.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @param  [in]  section Address section for memory monitoring.
 * @param  [in]  addr_start Memory monitoring start address.
 * @param  [in]  addr_end Memory monitoring end address.
 * @retval ture: The address section set up successfully.
 * @retval false: The address section set up failed.
 * @else
 * @brief  HAL层MEM_Monitor设置监控地址段。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @param  [in]  section 内存监控地址段，参考。
 * @param  [in]  addr_start 内存监控起始地址。
 * @param  [in]  addr_end 内存监控终结地址。
 * @retval ture  对: 设置地址段成功。
 * @retval false 错：设置地址段失败。
 * @endif
 */
typedef bool (*hal_mem_monitor_set_section_t)(mem_monitor_t monitor,
                                              mem_monitor_section_t section,
                                              uint32_t addr_start,
                                              uint32_t addr_end);

/**
 * @if Eng
 * @brief  Set memory monitoring mode for HAL MEM_Monitor.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @param  [in]  section Address section for memory monitoring.
 * @param  [in]  mode Memory monitoring mode.
 * @else
 * @brief  HAL层MEM_Monitor的内存监控模式设置接口。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @param  [in]  section 内存监控地址段，参考。
 * @param  [in]  mode 内存监控模式，参考。
 * @endif
 */
typedef bool (*hal_mem_monitor_set_section_mode_t)(mem_monitor_t monitor,
                                                   mem_monitor_section_t section,
                                                   hal_mem_monitor_mode_t mode);

/**
 * @if Eng
 * @brief  Enable Interrupt for hal MEM_Monitor.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @param  [in]  delay MEM_Monitor interrupt report delay time.
 * @else
 * @brief  HAL层MEM_Monitor的配置中断迟上报。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @param  [in]  delay MEM_Monitor中断延迟上报时间。
 * @endif
 */
typedef void (*hal_mem_monitor_set_interrupt_report_delay_t)(mem_monitor_t monitor, uint16_t delay);

 /**
 * @if Eng
 * @brief  Register a callback asociated with a MEM_Monitor interrupt cause.
 * @param  [in]  monitor Indicates the index of the memory monitoring register.
 * @param  [in]  callback  The interrupt callback to register.
 * @else
 * @brief  注册与MEM_Monitor中断原因关联的回调。
 * @param  [in]  monitor 内存监控寄存器索引。
 * @param  [in]  callback  寄存器的中断回调。
 * @endif
 */
typedef void (*hal_mem_monitor_set_callback_t)(mem_monitor_t monitor,
                                               hal_mem_monitor_callback_t callback);

/**
 * @if Eng
 * @brief  Interface between MEM_Monitor driver and MEM_Monitor hal.
 * @else
 * @brief  Driver层MEM_Monitor和HAL层MEM_Monitor的接口。
 * @endif
 */
typedef struct {
    hal_mem_monitor_init_t init;                          /*!< @if Eng Init device interface.
                                                               @else   HAL层MEM_Monitor的初始化接口 @endif */
    hal_mem_monitor_deinit_t deinit;                      /*!< @if Eng Deinit device interface.
                                                               @else   HAL层MEM_Monitor去初始化接口 @endif */
    hal_mem_monitor_enable_t enable;                      /*!< @if Eng Control MEM_Monitor enable interface.
                                                               @else   HAL层MEM_Monitor开启接口 @endif */
    hal_mem_monitor_disable_t disable;                    /*!< @if Eng Control MEM_Monitor disable interface.
                                                               @else   HAL层MEM_Monitor关闭接口 @endif */
    hal_mem_monitor_clock_enable_t clock_enable;          /*!< @if Eng Control MEM_Monitor enable clock interface.
                                                               @else   HAL层MEM_Monitor中断时钟开启接口 @endif */
    hal_mem_monitor_clock_disable_t clock_disable;        /*!< @if Eng Control MEM_Monitor disenable clock interface.
                                                               @else   HAL层MEM_Monitor设置回调函数接口 @endif */
    hal_mem_monitor_set_section_t set_section;            /*!< @if Eng Control MEM_Monitor set section interface.
                                                               @else   HAL层MEM_Monitor设置监控代码段接口 @endif */
    hal_mem_monitor_set_section_mode_t set_section_mode;  /*!< @if Eng Control MEM_Monitor set section mode interface.
                                                               @else   HAL层MEM_Monitor设置工作模式接口 @endif */
    hal_mem_monitor_set_interrupt_report_delay_t set_interrupt_report_delay;
                                           /*!< @if Eng Control MEM_Monitor set interrupt report delay time interface.
                                                @else   HAL层MEM_Monitor设置中断延迟上报时间接口 @endif */
    hal_mem_monitor_set_callback_t set_callback;          /*!< @if Eng Control MEM_Monitor set callback interface.
                                                               @else   HAL层MEM_Monitor设置回调函数接口 @endif */
} hal_mem_monitor_funcs_t;

/**
 * @if Eng
 * @brief  Register @ref hal_mem_monitor_funcs_t into the g_hal_mem_monitors_funcs.
 * @param  [out]  funcs Interface between MEM_Monitor driver and MEM_Monitor hal.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t.
 * @else
 * @brief  注册 @ref hal_mem_monitor_funcs_t 到 g_hal_mem_monitors_funcs 。
 * @param  [out]  funcs Driver层MEM_Monitor和HAL层MEM_Monitor的接口实例。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_mem_monitor_register_funcs(hal_mem_monitor_funcs_t *funcs);

/**
 * @if Eng
 * @brief  Unregister @ref hal_mem_monitor_funcs_t from the g_hal_mem_monitors_funcs.
 * @return ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t.
 * @else
 * @brief  从g_hal_mem_monitors_funcs注销 @ref hal_mem_monitor_funcs_t 。
 * @return ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_mem_monitor_unregister_funcs(void);

/**
 * @if Eng
 * @brief  Get interface between MEM_Monitor driver and MEM_Monitor hal, see @ref hal_mem_monitor_funcs_t.
 * @return Interface between MEM_Monitor driver and MEM_Monitor hal, see @ref hal_mem_monitor_funcs_t.
 * @else
 * @brief  获取Driver层MEM_Monitor和HAL层MEM_Monitor的接口实例，参考 @ref hal_mem_monitor_funcs_t 。
 * @return Driver层MEM_Monitor和HAL层MEM_Monitor的接口实例，参考 @ref hal_mem_monitor_funcs_t 。
 * @endif
 */
hal_mem_monitor_funcs_t *hal_mem_monitor_get_funcs(void);

/**
 * @if Eng
 * @brief  Init the MEM_Monitor which will set the base address of registers.
 * @retval ERRCODE_SUCC   Success.
 * @retval Other          Failure. For details, see @ref errcode_t.
 * @else
 * @brief  初始化MEM_Monitor，设置寄存器的基地址。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t hal_mem_monitor_regs_init(void);

/**
 * @if Eng
 * @brief  Deinit the hal_drv_mem_monitor which will clear the base address of registers has been
 *         set by @ref hal_mem_monitor_regs_init.
 * @else
 * @brief  去初始化，然后清除在 @ref hal_mem_monitor_regs_init 中设置的寄存器地址。
 * @endif
 */
void hal_mem_monitor_regs_deinit(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif