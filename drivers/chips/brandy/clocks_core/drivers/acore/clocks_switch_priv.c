
#include "clocks_switch_priv.h"
#include "hal_clocks_app.h"
#include "common_def.h"
#include "clocks_switch.h"
#include "pmu_cmu.h"
#include "clocks_config_priv.h"
#include "hal_pmu_vdd.h"

clocks_pll_src_t clocks_pll_veto_type_translation(clocks_clk_src_t clk_src)
{
    switch (clk_src) {
        case CLOCKS_CLK_SRC_TCXO_2X:
            return CLOCKS_PLL_SRC_TCXO_COM_DLL2;
        case CLOCKS_CLK_SRC_PLL:
            return CLOCKS_PLL_SRC_PLL;
        default:
            return CLOCKS_PLL_SRC_MAX;
    }
}

pmu_cmu_core_t clocks_pll_used_init_type_translation(clocks_clk_src_t clk_src)
{
    switch (clk_src) {
        case CLOCKS_CLK_SRC_PLL:
            return PMU_CMU_PLL;
        default:
            return PMU_CMU_MAX;
    }
}

void clocks_pll_power_on(clocks_pll_src_t pll_src)
{
    cmu_pll_config_t pll_cfg;
    switch (pll_src) {
        case CLOCKS_PLL_SRC_TCXO_COM_DLL2:
            hal_clocks_ccrg_xo_dll2_clk_en((switch_type_t)true);
            break;
        case CLOCKS_PLL_SRC_PLL:
            pmu_vdd1p0_request_switch(TURN_ON);
            pll_cfg = clocks_pll_cfg_get();
            pmu_cmu_pll_reinit(PMU_CMU_PLL, pll_cfg);
            break;
        default:
            break;
    }
}

void clocks_pll_power_off(clocks_pll_src_t pll_src)
{
    switch (pll_src) {
        case CLOCKS_PLL_SRC_TCXO_COM_DLL2:
            hal_clocks_ccrg_xo_dll2_clk_en((switch_type_t)false);
            break;
        case CLOCKS_PLL_SRC_PLL:
            pmu_cmu_pll_deinit(PMU_CMU_PLL);
            pmu_vdd1p0_request_switch(TURN_OFF);
            break;
        default:
            break;
    }
}

hal_clocks_ccrg_mcu_group_clk_src_t clocks_src_sel_type_translation(clocks_ccrg_module_t module, \
                                                                    clocks_clk_src_t clk_src)
{
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_RC;
        case CLOCKS_CLK_SRC_TCXO_2X:
            if (module >= CLOCKS_CCRG_MODULE_PAD_OUT0) {
                return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
            } else {
                return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO_DLL2;
            }
        case CLOCKS_CLK_SRC_TCXO:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_XO;
        case CLOCKS_CLK_SRC_PLL:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_PLL;
        default:
            return HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
    }
}

uint32_t clocks_get_pll_frequency(clocks_clk_src_t clk_src)
{
    unused(clk_src);
    return pmu_cmu_get_pll_clock_value();
}