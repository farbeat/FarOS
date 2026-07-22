/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm clock driver adapter code
 */

#include "pm_clocks.h"
#include "pm_bsp.h"
#include "chip_io.h"
#include "core.h"
#include "clocks.h"
#include "clocks_types.h"
#include "hal_pmu_peripheral.h"
#include "hal_clocks_app_priv.h"
#include "clocks_config_priv.h"
#include "clocks_switch.h"
#include "errcode.h"
#include "log_common.h"
#include "log_def_pf.h"
#include "debug_print.h"

#define  TCXO_2X_VALUE    64000000
#define  PLL_VALUE        884736000
#define  PM_MODE_BITMAP_TO_MODE   32
#define  WORK_PM_MODE(value)  (pm_mode_t)(PM_MODE_BITMAP_TO_MODE - __builtin_clz(value) - 1)
#define  NEW_PM_MODE_VOTE(uid, mode)    ({ \
                g_pm_mode_vote[mode] = BIT(uid); })
#define  JOIN_PM_MODE_VOTE(uid, old_vote, mode)    ({ \
                old_vote = old_vote | BIT(uid); \
                g_pm_mode_vote[mode] = old_vote; })
#define  REMOVE_PM_MODE_VOTE(uid, old_vote, mode)    ({ \
                old_vote = old_vote & (~BIT(uid)); \
                g_pm_mode_vote[mode] = old_vote; })
#define  FREE_PM_MODE_VOTE(mode)    ({ \
                g_pm_mode_vote[mode] = 0; })

static uint32_t g_pm_mode_vote[PM_MODE_MAX] = {0};
static uint32_t g_pm_mode_bitmap = BIT(PM_MODE_NONE);
static uint64_t g_pm_tick_begin = 0;
static pm_hifi_freq_t g_hifi_freq[HIFI_LPM_CLK_MAX] = {
    { CLOCKS_SRC_TCXO_2X, CLOCK_DIV_1, TCXO_2X_VALUE, CLOCKS_CLK_SRC_MAX, CLOCK_DIV_0, 0 }, // dsp:64MHz, codec:0
    // dsp:147Mhz, codec:24.576Mhz
    { CLOCKS_SRC_PLL, CLOCK_DIV_6, PLL_VALUE / CLOCK_DIV_6, CLOCKS_SRC_PLL, CLOCK_DIV_36, PLL_VALUE / CLOCK_DIV_36 },
    // dsp:176Mhz, codec:24.576Mhz
    { CLOCKS_SRC_PLL, CLOCK_DIV_5, PLL_VALUE / CLOCK_DIV_5, CLOCKS_SRC_PLL, CLOCK_DIV_36, PLL_VALUE / CLOCK_DIV_36 },
    // dsp:294Mhz, codec:24.576Mhz
    { CLOCKS_SRC_PLL, CLOCK_DIV_3, PLL_VALUE / CLOCK_DIV_3, CLOCKS_SRC_PLL, CLOCK_DIV_36, PLL_VALUE / CLOCK_DIV_36 },
};
static pm_hifi_mode_t g_hifi_mode = HIFI_LPM_CLK_LEVEL1;
static hifi_inform_handler g_hifi_inform_callback = NULL;

static void clocks_core_perips_config(clocks_id_t id, switch_type_t config)
{
    hal_pmu_perip_request_type_t type = HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER;
    if (config != 0) {
        type = HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER;
    }

    if (id <= CLOCKS_ID_M_PWM) {
        hal_pmu_mcpu_perip_config((hal_pmu_mcpu_perips_type_t)(id - CLOCKS_ID_M_SMDMA), type);
    } else if (id <= CLOCKS_ID_B_TSENSOR) {
        return;
    } else if (id <= CLOCKS_ID_AON_CWDT) {
        hal_pmu_maon_perip_config((hal_pmu_maon_perips_type_t)(id - CLOCKS_ID_AON_CWDT), type);
    } else if (id <= CLOCKS_ID_MEM_OPI) {
        hal_pmu_mem_perip_config((hal_pmu_mem_perips_type_t)(id - CLOCKS_ID_MEM_OPI), type);
    } else if (id <= CLOCKS_ID_DAON_GPIO) {
        hal_pmu_daon_perip_config((hal_pmu_daon_perips_type_t)(id - CLOCKS_ID_DAON_GPIO), type);
    }
}

static void clocks_glb_clken_config(clocks_id_t id, switch_type_t config)
{
    hal_clocks_ccrg_module_t module = (hal_clocks_ccrg_module_t)(id - CLOCKS_ID_SOC_MCU_CORE);
    hal_clocks_glb_clken_config(module, config);
}

void uapi_clocks_init(void)
{
    system_clocks_config_t clk_cfg = CLOCKS_ALL_PLL_09V;
    clock_switch_pre_handler_register(opi_pre_handler, CLOCKS_CCRG_MODULE_XIP_OPI);
#if CORE == MASTER_BY_ALL
    if (clk_cfg == CLOCKS_ALL_PLL_09V) {
        uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P925V); // in fact vdd is 0.9v
    } else {
        uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P850V); // in fact vdd is 0.825v
    }
    clocks_app_clk_init(clk_cfg);

#ifndef BUILD_APPLICATION_SSB
    clocks_hardware_sub_init();
    clocks_auto_cg_config();
    clocks_bridging_low_powerp_config(true);
#endif
    system_mcu_clocks_config();
#else
    bt_adjust_freq_init();
#endif
}

void uapi_clocks_switch(clocks_id_t id, switch_type_t config)
{
    if (id < CLOCKS_ID_SOC_MCU_CORE) {
        // 外设时钟门控
        clocks_core_perips_config(id, config);
    } else if (id < CLOCKS_ID_MAX) {
        if (id == CLOCKS_ID_SOC_MCU_CORE) {
            return;
        }

        // 模块时钟门控
        clocks_glb_clken_config(id, config);
    }
}

void uapi_clocks_hifi_config(clocks_src_t clk_src, uint8_t clk_div)
{
    clocks_set_dsp_freq(clk_src, clk_div);
}

void uapi_clocks_codec_config(clocks_src_t clk_src, uint8_t clk_div)
{
    clocks_set_codec_freq(clk_src, clk_div);
}

void uapi_clocks_config(clocks_id_t id, clocks_src_t clk_src, uint8_t clk_div)
{
#if CORE == MASTER_BY_ALL && CHIP_ASIC
    clocks_ccrg_module_t module = (clocks_ccrg_module_t)(id - CLOCKS_ID_SOC_MCU_CORE);
    system_ccrg_clock_config(module, (clocks_clk_src_t)clk_src, clk_div);
#else
    UNUSED(id);
    UNUSED(clk_src);
    UNUSED(clk_div);
    // ipc?
#endif
}

void uapi_clocks_get_module_freq(clocks_id_t module, uint32_t *freq)
{
    if (freq == NULL || module > CLOCKS_ID_SOC_MCU_CORE || module > CLOCKS_ID_SOC_CODEC) {
        return;
    }

    *freq = clocks_get_module_frequency((clocks_ccrg_module_t)module);
}

void uapi_get_current_pm_mode_info(void)
{
    pm_mode_t work_mode = WORK_PM_MODE(g_pm_mode_bitmap);
    uint32_t vote = g_pm_mode_vote[work_mode];
    for (uint8_t i = 0; i < PM_ID_MAX; i++) {
        if ((vote & (1 << i)) != 0) {
            oml_pf_log_print2(LOG_ACORE_PLT_BSP_DRIVER_CLOCK, LOG_NUM_DRIVER_CLOCK, LOG_LEVEL_INFO,
                "pm working mode is %d at uid %d vote.", work_mode, i);
            PRINT("pm working mode is %d at uid %d vote.\r\n", work_mode, i);
        }
    }
    clocks_clk_cfg_t *cur_clks = clocks_get_current_clock_cfg();
    if (cur_clks == NULL) {
        return;
    }
    for (uint8_t i = 0; i < CLOCKS_CCRG_MODULE_MAX; i++) {
        PRINT("pm working ccrg clks[0x%x] src 0x%x, div 0x%x.\r\n", i, cur_clks[i].clk_src, cur_clks[i].clk_div);
        oml_pf_log_print3(LOG_ACORE_PLT_BSP_DRIVER_CLOCK, LOG_NUM_DRIVER_CLOCK, LOG_LEVEL_INFO,
            "pm working ccrg clks[0x%x] src 0x%x, div 0x%x.", i, cur_clks[i].clk_src, cur_clks[i].clk_div);
    }
}

static system_mcu_freq_t uapi_pm_mode_type_translation(pm_mode_t mode)
{
    switch (mode) {
        case PM_MODE_LP:
            return CLOCKS_M64;
        case PM_MODE_NORMAL:
            return CLOCKS_M_H0;
        case PM_MODE_PERFORMACE:
            return CLOCKS_M_H1;
        default:
            return CLOCKS_MCU_FREQ_MAX;
    }
}

errcode_t uapi_pm_request_mode(pm_id_t uid, pm_mode_t mode)
{
    if (mode == PM_MODE_NONE) {
        return ERRCODE_FAIL;
    }

    uint32_t irq = osal_irq_lock();
    pm_mode_t work_mode = WORK_PM_MODE(g_pm_mode_bitmap);
    uint32_t old_vote = g_pm_mode_vote[work_mode];
    if (mode > work_mode || work_mode == PM_MODE_NONE) {
        system_mcu_freq_t mcu_freq = uapi_pm_mode_type_translation(mode);
        clocks_set_mcu_freq(mcu_freq);
        if ((old_vote & BIT(uid)) != 0) {
            if (old_vote == BIT(uid)) {
                g_pm_mode_bitmap =  g_pm_mode_bitmap & (~BIT(work_mode));
            }
            REMOVE_PM_MODE_VOTE(uid, old_vote, work_mode);
        }
        NEW_PM_MODE_VOTE(uid, mode);
        g_pm_mode_bitmap |= BIT(mode);
        osal_irq_restore(irq);
    } else if (mode == work_mode) {
        JOIN_PM_MODE_VOTE(uid, old_vote, work_mode);
        osal_irq_restore(irq);
        return ERRCODE_PM_MODE_ONLY_JOIN_VOTE;
    } else if (mode < work_mode) {
        if ((old_vote & BIT(uid)) == BIT(uid)) {
            if (old_vote == BIT(uid)) {
                uint32_t new_pm_mode_bitmap = g_pm_mode_bitmap & (~BIT(work_mode));
                if (new_pm_mode_bitmap == BIT(PM_MODE_NONE)) {
                    new_pm_mode_bitmap |= BIT(mode);
                    NEW_PM_MODE_VOTE(uid, mode);
                }
                pm_mode_t new_highest_mode = WORK_PM_MODE(new_pm_mode_bitmap);
                system_mcu_freq_t mcu_freq = uapi_pm_mode_type_translation(new_highest_mode);
                clocks_set_mcu_freq(mcu_freq);
                FREE_PM_MODE_VOTE(work_mode);
                g_pm_mode_bitmap = new_pm_mode_bitmap;
                osal_irq_restore(irq);
                // the current uid has exited, and the mode is downgraded, need return detail reason.
                return ERRCODE_PM_MODE_DOWNGRADED;
            } else {
                REMOVE_PM_MODE_VOTE(uid, old_vote, work_mode);
                osal_irq_restore(irq);
                // the current uid has exited, but the mode busy, need return detail reason.
                return ERRCODE_PM_MODE_ONLY_REMOVE_VOTE;
            }
        }
        osal_irq_restore(irq);
        // forbid config lower mode than current mode
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

void pm_set_hifi_mode(pm_hifi_mode_t mode)
{
    PRINT("SET DSP freq=%d, div=%d, codec freq=%d, div=%d\r\n", g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div,
          g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
    if (mode < HIFI_LPM_CLK_MAX) {
        g_hifi_mode = mode;
    }
    switch (mode) {
        case HIFI_LPM_CLK_LOW_LEVEL:
            clocks_set_dsp_freq(g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div);
            clocks_set_codec_freq(g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
            break;
        case HIFI_LPM_CLK_LEVEL1:
            clocks_set_dsp_freq(g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div);
            clocks_set_codec_freq(g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
            break;
        case HIFI_LPM_CLK_LEVEL2:
            clocks_set_dsp_freq(g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div);
            clocks_set_codec_freq(g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
            break;
        case HIFI_LPM_CLK_MAX:
            clocks_set_dsp_freq(g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div);
            clocks_set_codec_freq(g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
            break;
        default:
            PRINT("mode = %d not support, set default config\r\n", mode);
            clocks_set_dsp_freq(g_hifi_freq[mode].dsp_src, g_hifi_freq[mode].dsp_div);
            clocks_set_codec_freq(g_hifi_freq[mode].codec_src, g_hifi_freq[mode].codec_div);
            break;
    }
}

pm_hifi_freq_t uapi_pm_get_hifi_freq(pm_hifi_mode_t mode)
{
    return g_hifi_freq[mode];
}

pm_hifi_mode_t uapi_pm_get_hifi_mode(void)
{
    return g_hifi_mode;
}

void uapi_register_hifi_inform_callback(hifi_inform_handler callback)
{
    g_hifi_inform_callback = callback;
}

void uapi_set_hifi_mode(pm_hifi_mode_t mode)
{
    pm_set_hifi_mode(mode);
    g_hifi_mode = mode;
    PRINT("set hifi mode=%d.\r\n", mode);
    if (get_dsp_power_up_status(SLAVE_CPU_DSP0) == true) {
        if (g_hifi_inform_callback != NULL) {
            g_hifi_inform_callback();
        }
    }
}
