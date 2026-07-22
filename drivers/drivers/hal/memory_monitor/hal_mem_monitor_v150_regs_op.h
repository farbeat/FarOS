/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides v150 MEM_Monitor register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#ifndef HAL_MEM_MONITOR_V150_REGS_OP_H
#define HAL_MEM_MONITOR_V150_REGS_OP_H

#include <stdint.h>
#include "errcode.h"
#include "hal_mem_monitor_v150_regs_def.h"
#include "mem_monitor_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_mem_monitor_v150_regs_op MEM_Monitor V150 Regs Operation
 * @ingroup  drivers_hal_mem_monitor
 * @{
 */
#define MEM_MONITOR_REGS ((mem_monitor_regs_t *)g_base_addr)
extern uintptr_t g_base_addr;

/**
 * @brief  Set the value of @ref mem_monitor_en_data_t.en.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  val The value of @ref mem_monitor_en_data_t.en.
 */
static inline void hal_mem_monitor_reg_set_en(int32_t monitor, uint32_t val)
{
    mem_monitor_en_data_t en_data;
    en_data.d32 = MEM_MONITOR_REGS[monitor].en;
    en_data.b.en = val;
    MEM_MONITOR_REGS[monitor].en = en_data.d32;
}

/**
 * @brief  Set the value of @ref mem_monitor_en_data_t.clock_en.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  val The value of @ref mem_monitor_en_data_t.clock_en.
 */
static inline void hal_mem_monitor_reg_set_clock_en(int32_t monitor, uint32_t val)
{
    mem_monitor_en_data_t en_data;
    en_data.d32 = MEM_MONITOR_REGS[monitor].en;
    en_data.b.clock_en = val;
    MEM_MONITOR_REGS[monitor].en = en_data.d32;
}

/**
 * @brief  Clear the MEM_Monitor work mode.
 * @param  [in]  monitor The value of monitor num.
 */
static inline void hal_mem_monitor_reg_addr_en_clr(uint32_t monitor)
{
    MEM_MONITOR_REGS[monitor].addr_en = 0;
}

/**
 * @brief  Set the MEM_Monitor Lower 16 bits of the lower address.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of section.
 * @param  [in]  addr_low_low_16 The value of Lower 16 bits of the low address.
 */
static inline void hal_mem_monitor_reg_mem_addr_set_l_l(uint32_t monitor,
                                                        uint32_t section,
                                                        uint16_t addr_low_low_16)
{
    MEM_MONITOR_REGS[monitor].mem_addr[section].mem_addr_l_l = addr_low_low_16;
}

/**
 * @brief  Set the MEM_Monitor Higher 16 bits of the low address.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of section.
 * @param  [in]  addr_low_high_16 The value of Higher 16 bits of the low address.
 */
static inline void hal_mem_monitor_reg_mem_addr_set_l_h(uint32_t monitor,
                                                        uint32_t section,
                                                        uint16_t addr_low_high_16)
{
    MEM_MONITOR_REGS[monitor].mem_addr[section].mem_addr_l_h = addr_low_high_16;
}

/**
 * @brief  Set the MEM_Monitor Lower 16 bits of the high address.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of section.
 * @param  [in]  addr_high_low_16 The value of Lower 16 bits of the high address.
 */
static inline void hal_mem_monitor_reg_mem_addr_set_h_l(uint32_t monitor,
                                                        uint32_t section,
                                                        uint16_t addr_high_low_16)
{
    MEM_MONITOR_REGS[monitor].mem_addr[section].mem_addr_h_l = addr_high_low_16;
}

/**
 * @brief  Set the MEM_Monitor Higher 16 bits of the high address.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of section.
 * @param  [in]  addr_high_high_16 The value of Higher 16 bits of the high address.
 */
static inline void hal_mem_monitor_reg_mem_addr_set_h_h(uint32_t monitor,
                                                        uint32_t section,
                                                        uint16_t addr_high_high_16)
{
    MEM_MONITOR_REGS[monitor].mem_addr[section].mem_addr_h_h = addr_high_high_16;
}

/**
 * @brief  Clear the MEM_Monitor interrupt.
 * @param  [in]  monitor The value of monitor num.
 */
static inline void hal_mem_monitor_reg_clr_interrupt(uint32_t monitor)
{
    MEM_MONITOR_REGS[monitor].int_clr = 1;
}

/**
 * @brief  Set the MEM_Monitor interrupt delay time.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  delay The value of interrupt delay time.
 */
static inline void hal_mem_monitor_reg_delay_interrupt(uint32_t monitor, uint16_t delay)
{
    MEM_MONITOR_REGS[monitor].int_dly = delay;
}

/**
 * @brief  Get the MEM_Monitor capture id.
 * @param  [in]  monitor The value of monitor num.
 */
static inline uint16_t hal_mem_monitor_reg_id_capture(uint32_t monitor)
{
    return *(uint16_t *)(uintptr_t)(&MEM_MONITOR_REGS[monitor].id);
}

/**
 * @brief  Get the MEM_Monitor capture low address.
 * @param  [in]  monitor The value of monitor num.
 */
static inline uint16_t hal_mem_monitor_reg_addr_l_capture(uint32_t monitor)
{
    return *(uint16_t *)(uintptr_t)(&MEM_MONITOR_REGS[monitor].addr_l);
}

/**
 * @brief  Get the MEM_Monitor capture high address.
 * @param  [in]  monitor The value of monitor num.
 */
static inline uint16_t hal_mem_monitor_reg_addr_h_capture(uint32_t monitor)
{
    return *(uint16_t *)(uintptr_t)(&MEM_MONITOR_REGS[monitor].addr_h);
}

/**
 * @brief  Set the MEM_Monitor section function read.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of memory segment ID.
 */
void hal_mem_monitor_reg_addr_en_r(uint32_t monitor, uint32_t section);

/**
 * @brief  Set the MEM_Monitor section function write.
 * @param  [in]  monitor The value of monitor num.
 * @param  [in]  section The value of  memory segment ID.
 */
void hal_mem_monitor_reg_addr_en_w(uint32_t monitor, uint32_t section);

/**
 * @brief  Get the MEM_Monitor interrupt status.
 * @param  [in]  monitor The value of monitor num.
 */
uint16_t hal_mem_monitor_reg_int_sts_get(uint32_t monitor);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif