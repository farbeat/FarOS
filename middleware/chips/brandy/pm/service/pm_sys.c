/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power manage sys code
 */

#include "pm_sys.h"
#include "pm.h"
#include "pm_os.h"
#include "pm_core.h"

void pm_mode_switch(pm_mode_t mode)
{
    /*
     * ldo电压level是否一致.遍历ldo
     * sub门控是否一致, 门控 + 复位/解复位
     * 时钟源：pll是否开启，xo\xo dll2是否开启
     * 时钟配置：时钟源选择，分频比
     * 全局时钟门控打开
     * pm暂不考虑对外设门控操作
     * 调节dsp/bt需ipc通知
     */
    bool vset_first = pm_config_vset_first(mode);
    if (vset_first) {
        // vset = sw_en > clk_cfg > clk_en > pwr_en
        pm_config_pwr_cfg(mode);
        pm_config_clk_cfg(mode);
        pm_config_clk_en(mode);
        pm_config_pwr_en(mode);
    } else {
        // clk_cfg > vset = sw_en > clk_en > pwr_en
        pm_config_clk_cfg(mode);
        pm_config_pwr_cfg(mode);
        pm_config_clk_en(mode);
        pm_config_pwr_en(mode);
    }
}

static void pm_reset_process(void)
{
    // todo
}

void pm_enter(pm_mode_t mode)
{
    if (mode == PM_MODE_RESET) {
        pm_reset_process();
    } else {
        pm_mode_switch(mode);
    }
}
