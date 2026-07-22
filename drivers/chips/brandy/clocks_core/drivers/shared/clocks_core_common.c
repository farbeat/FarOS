/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  CLOCKS CORE COMMON SOURCE.
 * Author: @CompanyNameTag
 * Create: 2021-12-25
 */

#include "clocks_core_common.h"
#include "hal_clocks.h"
#include "clocks_switch_priv.h"
#include "systick.h"
#include "non_os.h"
#include "tcxo.h"

#define TEST_MCU_CLK_MS               0x10
// Average count per ms is 24000, 24000 * 16 = 384000
#define TEST_MCU_CLK_CNT_MS           384000

uint32_t g_mcu_core_clk = TCXO_1X_CLK;

/* Check the current CPU clock by counting for fpga. */
void init_mcu_core_clk(void)
{
    uint64_t origin_ticks = uapi_systick_get_ms();
    uint64_t current_ticks = uapi_systick_get_ms();
    uint64_t origin_count = uapi_tcxo_get_count();
    while (current_ticks - origin_ticks < TEST_MCU_CLK_MS) {
        current_ticks = uapi_systick_get_ms();
    }
    uint64_t current_count = uapi_tcxo_get_count();
    if (current_count - origin_count > TEST_MCU_CLK_CNT_MS) {
        g_mcu_core_clk = TCXO_1X_CLK;
    } else {
        g_mcu_core_clk = TCXO_16M_CLK;
    }
}

// It can only be used on the FPGA.
uint32_t get_mcu_core_clk(void)
{
    return g_mcu_core_clk;
}

uint32_t uart_get_clock_value(uart_bus_t uart)
{
#if (CORE != MASTER_BY_ALL) || defined(BUILD_APPLICATION_ROM) || CHIP_FPGA
    UNUSED(uart);
    return g_mcu_core_clk;
#else
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }

    switch (uart) {
        case UART_BUS_0:
            return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_UART);
        case UART_BUS_1:
            return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_UART);
        default:
            return 0; //lint !e527
    }
#endif
}
