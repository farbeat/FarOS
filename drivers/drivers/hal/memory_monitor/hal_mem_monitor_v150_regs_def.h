/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Provides v150 MEM_Monitor register \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-23, Create file. \n
 */
#ifndef HAL_MEM_MONITOR_V150_REGS_DEF_H
#define HAL_MEM_MONITOR_V150_REGS_DEF_H

#include <stdint.h>
#include "mem_monitor_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_hal_mem_monitor_v150_regs_def Mem_monitor V150 Regs Definition
 * @ingroup  drivers_hal_mem_monitor
 * @{
 */

/**
 * @brief  This union represents the bit fields in the en register. \n
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union mem_monitor_en_data {
    uint32_t d32;                                     /*!< Raw register data. */
    struct {
        uint32_t en             :   1;                /*!< If this bit is set to 1, mem_monitor is open. */
        uint32_t clock_en       :   1;                /*!< If this bit is set to 1, mem_monitor clock is open. */
    } b;                                              /*!< Register bits. */
} mem_monitor_en_data_t;

/**
 * @brief  This union represents the bit fields in the addr_en register. \n
 *         Read the register into the <i>d32</i> member then
 *         set/clear the bits using the <i>b</i> elements.
 */
typedef union mem_monitor_addr_en_data {
    uint32_t d32;                                     /*!< Raw register data. */
    struct {
        uint32_t w0             :   1;
        uint32_t w1             :   1;
        uint32_t w2             :   1;
        uint32_t w3             :   1;
        uint32_t r0             :   1;
        uint32_t r1             :   1;
        uint32_t r2             :   1;
        uint32_t r3             :   1;
    } b;                                              /*!< Register bits. */
} mem_monitor_addr_en_data_t;

typedef struct mem_monitor_mem_addr {
    uint32_t mem_addr_l_l;                            /*!< MEM_Monitor ADDR L L. */
    uint32_t mem_addr_l_h;                            /*!< MEM_Monitor ADDR L H. */
    uint32_t mem_addr_h_l;                            /*!< MEM_Monitor ADDR H L. */
    uint32_t mem_addr_h_h;                            /*!< MEM_Monitor ADDR H H. */
} mem_monitor_mem_addr_t;

/**
 * @brief  Registers associated with MEM_Monitor.
 */
typedef struct mem_monitor_regs {
    volatile uint32_t en;                                                 /*!< en.     <i>Offset: 00h</i>. */
    volatile uint32_t mem_int;                                            /*!< int.    <i>Offset: 04h</i>. */
    volatile uint32_t int_mask;                                           /*!< int_mask. <i>Offset: 08h</i>. */
    volatile uint32_t int_sts;                                            /*!< int_sts.  <i>Offset: 0Ch</i>. */
    volatile uint32_t int_clr;                                            /*!< int_clr.  <i>Offset: 10h</i>. */
    volatile uint32_t addr_l;                                             /*!< addr_l.   <i>Offset: 14h</i>. */
    volatile uint32_t addr_h;                                             /*!< addr_h.   <i>Offset: 18h</i>. */
    volatile uint32_t id;                                                 /*!< id.       <i>Offset: 1Ch</i>. */
    volatile uint32_t addr_en;                                            /*!< addr_en.  <i>Offset: 20h</i>. */
    volatile uint32_t int_dly;                                            /*!< int_dly.  <i>Offset: 24h</i>. */
    volatile mem_monitor_mem_addr_t mem_addr[MEM_MONITOR_SECTION_MAX];    /*!< mem_addr. <i>Offset: 44h</i>. */
} mem_monitor_regs_t;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif