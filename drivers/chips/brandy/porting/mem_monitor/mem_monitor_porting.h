/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides MEM_Monitor port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-15, Create file. \n
 */
#ifndef MEM_MONITOR_PORTING_H
#define MEM_MONITOR_PORTING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_mem_monitor MEM_Monitor
 * @ingroup  drivers_port
 * @{
 */
#define MEM_MONITOR_BEGINE_ADDR                 0x87000000
#define MEM_MONITOR_END_ADDR                    0x8700FFFF

/**
 * @if Eng
 * @brief  MEM_Monitor v150 register num.
 * @else
 * @brief  MEM_Monitor v150寄存器编号。
 * @endif
 */
typedef enum mem_monitor {
    MEM_MONITOR_NUM_0,
    MEM_MONITOR_NUM_1,
    MEM_MONITOR_NUM_MAX
} mem_monitor_t;

/**
 * @if Eng
 * @brief  MEM_Monitor v150 section ID.
 * @else
 * @brief  MEM_Monitor v150段ID。
 * @endif
 */
typedef enum mem_monitor_section {
    MEM_MONITOR_SECTION_0,
    MEM_MONITOR_SECTION_1,
    MEM_MONITOR_SECTION_2,
    MEM_MONITOR_SECTION_3,
    MEM_MONITOR_SECTION_MAX
} mem_monitor_section_t;

/**
 * @if Eng
 * @brief  Get the base address of a specified MEM_Monitor.
 * @return The base address of specified MEM_Monitor.
 * @else
 * @brief  获取指定MEM_Monitor的基地址。
 * @return 指定MEM_Monitor的基地址。
 * @endif
 */
uintptr_t mem_monitor_port_base_addr_get(void);

/**
 * @if Eng
 * @brief  Register hal funcs objects into hal_mem_monitor module.
 * @else
 * @brief  将hal funcs对象注册到hal_mem_monitor模块中。
 * @endif
 */
void mem_monitor_port_register_hal_funcs(void);

/**
 * @if Eng
 * @brief  Unregister hal funcs objects from hal_mem_monitor module.
 * @else
 * @brief  从hal_mem_monitor模块注销hal funcs对象。
 * @endif
 */
void mem_monitor_port_unregister_hal_funcs(void);

/**
 * @if Eng
 * @brief  Register irq for MEM_Monitor.
 * @else
 * @brief  MEM_Monitor注册中断。
 * @endif
 */
void mem_monitor_port_register_irq(void);

/**
 * @if Eng
 * @brief  Unregister irq for MEM_Monitor.
 * @else
 * @brief  MEM_Monitor去注册中断。
 * @endif
 */
void mem_monitor_port_unregister_irq(void);

/**
 * @if Eng
 * @brief  Lock of MEM_Monitor interrupt.
 * @else
 * @brief  MEM_Monitor中断锁定。
 * @endif
 */
void mem_monitor_port_irq_lock(void);

/**
 * @if Eng
 * @brief  Unlock of mem_monitor interrupt.
 * @else
 * @brief  MEM_Monitor中断解锁。
 * @endif
 */
void mem_monitor_port_irq_unlock(void);

/**
 * @if Eng
 * @brief  Check whether the addr configured is valid.
 * @param  [in]  addr_start Memory monitoring start address.
 * @param  [in]  addr_end Memory monitoring end address.
 * @return The value 'true' indicates that the addr is valid and the value 'false' indicates that the addr is invalid.
 * @else
 * @brief  检查配置的地址是否合法。
 * @param  [in]  addr_start 内存监控起始地址。
 * @param  [in]  addr_end 内存监控结束地址。
 * @return 值“true”表示addr有效，值“false”表示addr无效。
 * @endif
 */
bool mem_monitor_port_check_addr(uint32_t addr_start, uint32_t addr_end);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif