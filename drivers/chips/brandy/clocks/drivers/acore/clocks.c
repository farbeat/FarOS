/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:   CLOCKS DRIVER.
 */

#include "clocks.h"
#include "hal_clocks.h"
#include "non_os.h"
#include "clocks_core.h"
#include "clocks_switch.h"
#include "hal_clocks_app.h"
#include "pmu.h"
#if !defined(BUILD_APPLICATION_ATE) && MASTER_ONLY
#include "hal_xip.h"
#endif

#if defined(BUILD_APPLICATION_STANDARD) || defined(TEST_SUITE)
static system_clocks_config_t g_current_system_clock_config = CLOCKS_CONFIG_MAX;
void clocks_system_init(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) == true) {
        return;
    } else {
        non_os_set_driver_initalised(DRIVER_INIT_CLOCKS_CORE, true);
    }

    clocks_app_clk_init(CLOCKS_ALL_TCXO_64M);
    g_current_system_clock_config = CLOCKS_ALL_TCXO_64M;
    UNUSED(g_current_system_clock_config);
}

void system_mcu_clocks_config(void)
{
    uint32_t length;
    system_mcu_perip_clk_t *mcu_perip_clks = clocks_system_mcu_perip_clock_config_get(&length);
    uint32_t i = 0;
    for (i = 0; i < length; i++) {
        hal_clocks_app_perips_config(mcu_perip_clks[i].perip, mcu_perip_clks[i].on);
    }
}

#else
void clocks_system_init(void)
{
#if defined(BUILD_APPLICATION_SSB)
    clock_switch_pre_handler_register(opi_pre_handler, CLOCKS_CCRG_MODULE_XIP_OPI);
    clocks_ssb_clk_init();
    non_os_set_driver_initalised(DRIVER_INIT_CLOCKS_CORE, true);
#endif
}
#endif

void clocks_hardware_sub_init(void)
{
    // Xip dereset
    hal_clocks_glb_xip_soft_reset_control(HAL_SOFT_RST7_XIP_CRG, HAL_SOFT_RST_DERESET);
    hal_clocks_glb_xip_soft_reset_control(HAL_SOFT_RST7_XIP_LGC, HAL_SOFT_RST_DERESET);

    // core enter sleep, disable DMA/mbus/mcpu_h/mcpu clk
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCU_CORE_CH_SEL_SLP, TURN_ON);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_COM_BUS_CLKEN_SLP, TURN_ON);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_SDMA_CLKEN_SLP, TURN_ON);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MDMA_CLKEN_SLP, TURN_OFF);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MBUS_CLKEN_SLP, TURN_ON);

    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCPU_HCLKEN_SLP, TURN_ON);
    hal_clocks_sub_sleep_cfg(HAL_CLOCKS_MCPU_CLKEN_SLP, TURN_ON);
}

void clocks_init(void)
{
#if CHIP_ASIC
    clocks_system_init();
#ifndef BUILD_APPLICATION_SSB
    clocks_hardware_sub_init();

    clocks_auto_cg_config();
    clocks_bridging_low_powerp_config(true);
#endif
#endif
}

uint32_t clocks_get_core_frequency(void)
{
    return clocks_get_ccrg_clk_freq(CLOCKS_CCRG_MODULE_MCU_CORE);
}

uint32_t clocks_get_tcxo_clock(void)
{
    return TCXO_1X_CLK;
}

void clocks_auto_cg_config(void)
{
#if CLK_AUTO_CG_ENABLE == YES
    uint32_t length;
    system_clk_cg_t const *cg_cfg = clock_system_cg_config_get(&length);
    for (uint16_t i = 0; i < length; i++) {
        hal_clocks_module_auto_cg_control(cg_cfg[i].moudle, cg_cfg[i].on);
    }
#endif
}

void clocks_bridging_low_powerp_config(bool on)
{
#if CLK_AUTO_CG_ENABLE == YES
    if (on) {
        hal_clocks_bridging_exit_lp_config(TURN_OFF);
    } else {
        hal_clocks_bridging_exit_lp_config(TURN_ON);
    }
#endif
    UNUSED(on);
}