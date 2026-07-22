/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides tcxo port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#ifndef TCXO_PORTING_H
#define TCXO_PORTING_H

#include <stdint.h>
#include "platform_core.h"

#if TCXO_CLK_DYN_ADJUST == YES
#include "clocks_core_common.h"
#define HAL_TCXO_TICKS_PER_SECOND   (get_mcu_core_clk())
#define HAL_TCXO_TICKS_PER_M_SECOND (get_mcu_core_clk() / 1000)
#define HAL_TCXO_TICKS_PER_U_SECOND (get_mcu_core_clk() / 1000000)

#define TCXO_TICKS_PER_M_SECOND (get_mcu_core_clk() / 1000)
#define TCXO_TICKS_PER_U_SECOND (get_mcu_core_clk() / 1000000)
#else
#define HAL_TCXO_TICKS_PER_SECOND 32000000
#define HAL_TCXO_TICKS_PER_M_SECOND 32000
#define HAL_TCXO_TICKS_PER_U_SECOND 32

#define TCXO_TICKS_PER_M_SECOND 32000
#define TCXO_TICKS_PER_U_SECOND 32
#endif
#define TCXO_LOCK_GET_ATTE 0xFFFFFFFF

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    TCXO_OFF,
    TCXO_ON,
} tcxo_switch_t;

/**
 * @brief  Get the base address of a specified TCXO.
 * @return The base address of specified TCXO.
 */
uintptr_t tcxo_porting_base_addr_get(void);

/**
 * @brief  Register hal funcs objects into hal_tcxo module.
 */
void tcxo_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_tcxo module.
 */
void tcxo_port_unregister_hal_funcs(void);

/**
 * @brief  get tcxo ticks per microseconds.
 */
uint32_t tcxo_porting_ticks_per_usec_get(void);

/**
 * @brief  set tcxo ticks per microseconds.
 */
void tcxo_porting_ticks_per_usec_set(uint32_t ticks);

/**
 * @brief  enable tcxo.
 */
void tcxo_port_enable(tcxo_switch_t en);

/**
 * @brief  tcxo delay us.
 */
void tcxo_port_delay_us(uint32_t delay);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
