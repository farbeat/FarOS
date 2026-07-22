/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE COMMON HEADER.
 * Author: @CompanyNameTag
 * Create: 2021-12-25
 */
#ifndef CLOCKS_CORE_COMMON_H
#define CLOCKS_CORE_COMMON_H

#include <stdint.h>
#include "platform_core.h"

/**
 * @addtogroup connectivity_drivers_non_os_clocks_core
 * @{
 */
/**
 * @brief
 * @param uart Uart bus selected.
 * @return uart clock frequency.
 */
uint32_t uart_get_clock_value(uart_bus_t uart);

/**
 * @brief Set the CPU clock on the FPGA.
 */
void init_mcu_core_clk(void);

/**
 * @brief Get the CPU clock on the FPGA.
 * @return mcu core clock.
 */
uint32_t get_mcu_core_clk(void);

/**
 * @}
 */
#endif