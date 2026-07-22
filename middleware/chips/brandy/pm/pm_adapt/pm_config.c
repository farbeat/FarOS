/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: PM config src file
 */

#include "pm_config.h"
#include "pm_definition.h"
#include "clocks_types.h"
#include "pm_control.h"

#define PM_CONFIG_LDO_NUM    12
#define PM_CONFIG_SIMO_NUM   4
#define PM_CONFIG_PWR_NUM    (PM_CONFIG_LDO_NUM + PM_CONFIG_SIMO_NUM)

pm_pwr_cfg_t g_pm_pwr_default[] = {
    {true, false, PMU_VDD0P8_1P0_0P900V},    // VDD0P8
    {true, false, PMU_VDD1P9_1P3_1P900V},    // VDD1P9
    {true, false, PMU_VDD0P8_1P0_1P050V},    // VDD1P0
    {true, false, PMU_VDD1P9_1P3_1P200V},    // VDD1P3
    {true, false, PMU_CLDO_LEVEL_0P800V},    // CLDO1
    {true, true,  PMU_CLDO_LEVEL_0P800V},    // CLDO2
    {true, false, PMU_CLDO_LEVEL_0P800V},    // CLDO3
    {true, false, PMU_XLDO_LEVEL_1P800V},    // XLDO
    {true, false, PMU_CMU_LDO_LEVEL_0P900V}, // CMU_LDO
    {true, false, PMU_INTLDO1_LEVEL_1P80V},  // INTLDO1
    {true, false, PMU_INTLDO2_LEVEL_1P80V},  // INTLDO2
    {true, false, PMU_AUXLDO_LEVEL_1P800V},  // AUXLDO0
    {true, false, PMU_AUXLDO_LEVEL_1P800V},  // AUXLDO1
    {true, false, PMU_AUXLDO_LEVEL_1P800V},  // AUXLDO2
    {true, false, PMU_AUXLDO_LEVEL_1P800V},  // AUXLDO3
    {true, false, PMU_IOLDO_LEVEL_1P800V},   // IOLDO
};

pm_sub_en_t g_pm_sub_en_config[] = {
    /* all enable */
    {.bcore_en = true,  .audio_en = true,  .display_en  = true,  .sdio_en = true},

    /* disable display&audio */
    {.bcore_en = true,  .audio_en = false, .display_en  = false, .sdio_en = true},

    /* mcu only */
    {.bcore_en = false, .audio_en = false, .display_en  = false, .sdio_en = false},
};

pm_clk_en_t g_pm_clk_en_config[] = {
    /* all enable */
    {.xo_en = 1, .xo_2x_en = 1, .clk_rc_en = 1, .pll_en = 1, .b32m_en = 1, .b32m_2x_en = 1},
    /* pll disable */
    {.xo_en = 1, .xo_2x_en = 1, .clk_rc_en = 1, .pll_en = 0, .b32m_en = 1, .b32m_2x_en = 1},
    /* 32m only */
    {.xo_en = 1, .xo_2x_en = 0, .clk_rc_en = 0, .pll_en = 0, .b32m_en = 1, .b32m_2x_en = 0},
};

pm_clk_cfg_t g_pm_clk_default[] = {
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // mcu
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // perp_ls
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // perp_uart
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // perp_spi
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // com
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // sdio_m
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // mem_bus
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // xip_opi
    {.clk_en = true, .clk_sel = CLOCKS_SRC_TCXO, .div_num = CLOCK_DIV_NONE}, // xip_qspi
    {.clk_en = false, .clk_sel = CLOCKS_SRC_MAX, .div_num = CLOCK_DIV_NONE}, // gpu
    {.clk_en = false, .clk_sel = CLOCKS_SRC_MAX, .div_num = CLOCK_DIV_NONE}, // hifi
    {.clk_en = false, .clk_sel = CLOCKS_SRC_MAX, .div_num = CLOCK_DIV_NONE}, // codec
    {.clk_en = true,  .clk_sel = CLOCKS_SRC_MAX, .div_num = CLOCK_DIV_NONE}, // xip_opi_cache
};

static pm_pwr_cfg_t *pm_config_get_pwr_cfg(pm_mode_t mode)
{
    UNUSED(mode);
    return &g_pm_pwr_default;
}

static pm_clk_cfg_t *pm_config_get_clk_cfg(pm_mode_t mode)
{
    UNUSED(mode);
    return &g_pm_clk_default;
}

static pm_sub_en_t *pm_config_get_sub_pwr_en(pm_mode_t mode)
{
    return &g_pm_sub_en_config[mode];
}

static pm_clk_en_t *pm_config_get_clk_src_en(pm_mode_t mode)
{
    return &g_pm_clk_en_config[mode];
}

bool pm_config_vset_first(pm_mode_t mode)
{
    /* 关注0p8/cldo1,如果是升压场景，则先调压，否则先调频 */
    clocks_clk_cfg_t *cur_cfg = clocks_get_current_clock_cfg();
    pm_clk_cfg_t *clk_cfg = pm_config_get_clk_cfg(mode);
    pm_clk_en_t *clk_en = pm_config_get_clk_src_en(mode);

    for (uint8_t i = 0; i <= CLOCKS_CCRG_MODULE_CODEC; i++) {
        if (clk_cfg[i].clk_sel > cur_cfg[i].clk_src) {
            return true;
        }

        if (clk_cfg[i].clk_sel == cur_cfg[i].clk_src &&
            clk_cfg[i].div_num < cur_cfg[i].clk_div) {
            return true;
        }

        if (hal_clocks_bcpu_is_switch_tcxo_2x() == false &&
            clk_en->b32m_2x_en == true) {
            return true;
        }
    }
    return false;
}

void pm_config_pwr_cfg(pm_mode_t mode)
{
    uint8_t level = 0;
    pmu_id_t id;
    pm_pwr_cfg_t *pwr_cfg = pm_config_get_pwr_cfg(mode);
    for (uint8_t i = 0; i < PM_CONFIG_PWR_NUM; i++) {
        if (i >= PM_CONFIG_SIMO_NUM) {
            id = (pmu_id_t)(i - PM_CONFIG_SIMO_NUM + PMU_ID_LDO_CLDO1);
        } else {
            id = (pmu_id_t)(i + PMU_ID_SIMO_VDD0P8);
        }

        uapi_pmu_domain_vget(id, &level);
        if (pwr_cfg[i].pwr_vset != level && pwr_cfg[i].pwr_sw_en == false) {
            uapi_pmu_domain_vset(id);
        }

        if (id < PMU_ID_SIMO_VDD0P8) {
            pmu_ldo_bypass_en(id - PMU_ID_LDO_CLDO1, pwr_cfg[i].pwr_sw_en);
        }
    }
}

void pm_config_clk_cfg(pm_mode_t mode)
{
    uint8_t div;
    uint8_t clk_en;
    clocks_src_t src;
    pm_clk_cfg_t *clk_cfg = pm_config_get_clk_cfg(mode);
    for (clocks_id_t id = CLOCKS_ID_SOC_MCU_CORE; id <= CLOCKS_ID_SOC_CODEC; id++) {
        src = clk_cfg[id].clk_sel;
        div = clk_cfg[id].div_num;
        clk_en = clk_cfg[id].clk_en;
        uapi_clocks_config(id + CLOCKS_ID_SOC_MCU_CORE, src, div);
        uapi_clocks_switch(id + CLOCKS_ID_SOC_MCU_CORE, clk_en);
    }
}

void pm_config_clk_en(pm_mode_t mode)
{
    uint8_t div;
    clocks_src_t src;
    pm_clk_en_t *src_clk = pm_config_get_clk_src_en(mode);

    hal_cmu_manual_select_control(CMU_MAN_TCXO_EN, src_clk->xo_en);
    hal_cmu_manual_config_control(CMU_MAN_TCXO_EN, src_clk->xo_en);

    hal_cmu_manual_select_control(CMU_MAN_RC2_CLKEN, src_clk->clk_rc_en);
    hal_cmu_manual_config_control(CMU_MAN_RC2_CLKEN, src_clk->clk_rc_en);

    hal_clocks_ccrg_xo_dll2_clk_en(src_clk->xo_2x_en);

    hal_cmu_manual_select_control(CMU_MAN_B32M_CLKEN, src_clk->b32m_en);
    hal_cmu_manual_config_control(CMU_MAN_B32M_CLKEN, src_clk->b32m_en);

    hal_clocks_bcpu_dll_enable(src_clk->b32m_2x_en);

    hal_clocks_pll_module_clken(src_clk->pll_en);
}

void pm_config_pwr_en(pm_mode_t mode)
{
    pmu_id_t id;
    pm_pwr_cfg_t *pwr_cfg = pm_config_get_pwr_cfg(mode);
    for (uint8_t i = 0; i < PM_CONFIG_PWR_NUM; i++) {
        if (i >= PM_CONFIG_SIMO_NUM) {
            id = (pmu_id_t)(i - PM_CONFIG_SIMO_NUM + PMU_ID_LDO_CLDO1);
        } else {
            id = (pmu_id_t)(i + PMU_ID_SIMO_VDD0P8);
        }

        uapi_pmu_switch(id, pwr_cfg->pwr_en);
    }

    pm_sub_en_t *sub_en = pm_config_get_sub_pwr_en(mode);
    if (sub_en->bcore_en != 0) {
        pm_power_control(PM_BT_POWER_UP_AND_RUN);
    } else {
        pm_power_control(PM_BT_POWER_OFF);
    }

    if (sub_en->audio_en != 0) {
        pm_power_control(PM_DSP_POWER_UP_AND_RUN);
    } else {
        pm_power_control(PM_DSP_POWER_OFF);
    }

    if (sub_en->display_en != 0) {
        pm_power_control(PM_DISPLAY_POWER_UP_AND_RUN);
    } else {
        pm_power_control(PM_DISPLAY_POWER_OFF);
    }

    if (sub_en->sdio_en != 0) {
        pm_power_control(PM_SDIO_POWER_UP_AND_RUN);
    } else {
        pm_power_control(PM_SDIO_POWER_OFF);
    }
}