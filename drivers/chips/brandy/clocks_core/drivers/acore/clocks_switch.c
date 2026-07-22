/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SYSTEM CLOCKS SWITCH DRIVER
 * Author: @CompanyNameTag
 * Create: 2020-7-28
 */
#include "clocks_switch.h"
#include "hal_cpu_clocks.h"
#include "hal_clocks_app.h"
#include "hal_clocks_bt.h"
#include "clocks_core.h"
#include "debug_print.h"
#include "clocks_config_priv.h"
#include "panic.h"
#include "hal_xip.h"
#include "non_os.h"
#include "soc_osal.h"
#include "securec.h"
#include "platform.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "pm_bsp.h"
#ifdef __LITEOS__
#include "los_hwi.h"
#include "tick_adapt.h"
#endif
#endif

#define GLB_CLKEN           0x55000608
#define MCU_CORE_CR_CH0     0x5500060C
#define MCU_CORE_CR_CH1     0x55000610
#define MCU_BUS_DIV         0x55000604
#define MCU_PERP_LS_CR      0x55000614
#define MCU_PERP_UART_CR    0x55000618
#define MCU_PERP_SPI_CR     0x5500061C
#define COM_BUS_CR_CH0      0x55000620
#define COM_BUS_CR_CH1      0x55000624
#define SDIOM_CR            0x55000628
#define MEM_BUS_CR          0x5500062C
#define XIP_OPI_CR          0x55000630
#define XIP_QSPI_CR         0x55000634
#define GPU_CR              0x55000638
#define HIFI_CR_CH0         0x5500063c
#define HIFI_CR_CH1         0x55000640
#define CODEC_CR_CH0        0x55000644
#define CODEC_CR_CH1        0x55000648

#define CH_SEL              0x55000af0
#define MSUB_NOR_CFG        0x520003e0

bool g_pll_used_vote[CLOCKS_PLL_SRC_MAX][CLOCKS_CCRG_MODULE_MAX] = { {false} };
uint8_t g_pll_used_vote_count[CLOCKS_PLL_SRC_MAX] = { 0 };
#define CLK_NUM_2 2

clocks_clk_cfg_t g_current_clocks[CLOCKS_CCRG_MODULE_MAX] = { 0 };

static bool g_perpls_clock_reload = true;

clock_switch_proc_func g_clock_sw_pre_handler[CLOCKS_CCRG_MODULE_MAX];

clocks_clk_cfg_t *clocks_get_current_clock_cfg(void)
{
    return g_current_clocks;
}

void clocks_pll_init_vote(void)
{
    (void)memcpy_s((uint8_t *)g_current_clocks, sizeof(g_current_clocks),
                   (void *)SHARED_MEM_START, sizeof(g_current_clocks));
    uint32_t irq_status = osal_irq_lock();

    for (uint32_t i = 0;i < CLOCKS_CCRG_MODULE_MAX; i++) {
        clocks_pll_src_t pll_src = clocks_pll_veto_type_translation(g_current_clocks[i].clk_src);
        if (pll_src != CLOCKS_PLL_SRC_MAX) {
            g_pll_used_vote[pll_src][i] = true;
            g_pll_used_vote_count[pll_src]++;
        }

        pmu_cmu_core_t pll_used = clocks_pll_used_init_type_translation(g_current_clocks[i].clk_src);
        if (pll_used != PMU_CMU_MAX) {
            pmu_cmu_init_pll_status(true);
        }
    }
    osal_irq_restore(irq_status);
}

void clocks_pll_add_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module)
{
    if (pll_src >= CLOCKS_PLL_SRC_MAX) {
        return;
    }
    if (!g_pll_used_vote[pll_src][module]) {
        g_pll_used_vote[pll_src][module] = true;
        if (g_pll_used_vote_count[pll_src] == 0) {
            clocks_pll_power_on(pll_src);
        }
        g_pll_used_vote_count[pll_src]++;
    }
}

void clocks_pll_remove_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module)
{
    if (pll_src >= CLOCKS_PLL_SRC_MAX) {
        return;
    }
    if (g_pll_used_vote[pll_src][module]) {
        g_pll_used_vote[pll_src][module] = false;
        g_pll_used_vote_count[pll_src]--;
        if (g_pll_used_vote_count[pll_src] == 0) {
            clocks_pll_power_off(pll_src);
        }
    }
}

void clock_switch_pre_handler_register(clock_switch_proc_func func, clocks_ccrg_module_t module)
{
    g_clock_sw_pre_handler[module] = func;
}

void opi_pre_handler(clocks_clk_src_t clk_src, uint8_t clk_div)
{
    UNUSED(clk_div);
    if (clk_src == CLOCKS_CLK_SRC_PLL) {
        writew(CMU_CLOCK_XIP_CACHE_SYNC_MODE_REG, 0x1);  // sync mode = 1, high rate freq
        hal_clocks_xip_cache_div_config(XIP_CACHE_DIV_2_FREQ_VAL);
    } else {
        writew(CMU_CLOCK_XIP_CACHE_SYNC_MODE_REG, 0x0);  // sync mode = 0, low rate
        hal_clocks_xip_cache_div_config(1);
    }
}

static void system_ccrg_clock_config_pre_handler(clocks_ccrg_module_t module, clocks_clk_src_t clk_src, uint8_t clk_div)
{
    if (g_clock_sw_pre_handler[module] != NULL) {
        g_clock_sw_pre_handler[module](clk_src, clk_div);
    }
}

void system_ccrg_clock_perpls_set_reload(bool on)
{
    if (on) {
        g_perpls_clock_reload = true;
    } else {
        g_perpls_clock_reload = false;
    }
}

static bool system_ccrg_clock_perpls_is_reload(void)
{
    return g_perpls_clock_reload;
}

void system_ccrg_clock_config(clocks_ccrg_module_t module, clocks_clk_src_t clk_src, uint8_t clk_div)
{
    if ((g_current_clocks[module].clk_src == clk_src && g_current_clocks[module].clk_div == clk_div)) {
        return;
    }
    system_ccrg_clock_config_pre_handler(module, clk_src, clk_div);
    hal_clocks_ccrg_mcu_group_clk_src_t src_sel;
    // When clk_src equals CLOCKS_CLK_SRC_MAX, the clock will be disable the clock.
    if (clk_src == CLOCKS_CLK_SRC_MAX) {
        src_sel = HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
    } else {
        src_sel = clocks_src_sel_type_translation(module, clk_src);
        // Invalid clock source.
        if (src_sel == HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX) {
            panic(PANIC_CLOCKS_SWITCH, __LINE__);
        }
    }
    uint32_t irq_status = osal_irq_lock();
    if (g_current_clocks[module].clk_src != clk_src) {
        // Add vote.
        clocks_pll_src_t pll_src = clocks_pll_veto_type_translation(clk_src);
        clocks_pll_add_vote(pll_src, module);
        // Config the clock.
        hal_mcu_group_clock_switch((hal_clocks_ccrg_module_t)module, src_sel, clk_div);
        // Remove vote.
        pll_src = clocks_pll_veto_type_translation(g_current_clocks[module].clk_src);
        clocks_pll_remove_vote(pll_src, module);
        g_current_clocks[module].clk_src = clk_src;
    } else {
        // Config the clock.
        hal_mcu_group_clock_switch((hal_clocks_ccrg_module_t)module, src_sel, clk_div);
    }
    g_current_clocks[module].clk_div = clk_div;
#if defined(BUILD_APPLICATION_STANDARD) && (ARCH != RISCV31)
    if ((module == CLOCKS_CCRG_MODULE_MCU_PERP_LS) && (system_ccrg_clock_perpls_is_reload())) {
        uint32_t cycles_per_tick = clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_LS);
#ifdef __LITEOS__
        cycles_per_tick = (cycles_per_tick + LOSCFG_BASE_CORE_TICK_PER_SECOND / CLK_NUM_2) /
                           LOSCFG_BASE_CORE_TICK_PER_SECOND;
        LOS_SysTickReload(cycles_per_tick);
#endif
    }
#endif
    osal_irq_restore(irq_status);
}

uint32_t clocks_get_ccrg_clk_freq(clocks_ccrg_module_t module)
{
    clocks_clk_src_t clk_src = (clocks_clk_src_t)hal_clocks_ccrg_module_get_src((hal_clocks_ccrg_module_t)module);
    uint8_t clk_div = hal_clocks_ccrg_module_get_div((hal_clocks_ccrg_module_t)module);
    uint32_t clk_freq = 0;
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            clk_freq = RC_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO_2X:
            clk_freq = TCXO_2X_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO:
            clk_freq = TCXO_1X_CLK;
            break;
        case CLOCKS_CLK_SRC_MAX:
            break;
        default:
            clk_freq = clocks_get_pll_frequency(clk_src);
            break;
    }
    if (clk_div > CLOCK_DIV_1) {
        clk_freq = clk_freq / clk_div;
    }
    return clk_freq;
}

uint32_t clocks_get_module_frequency(clocks_ccrg_module_t module)
{
    clocks_clk_src_t clk_src = g_current_clocks[module].clk_src;
    uint8_t clk_div = g_current_clocks[module].clk_div;
    uint32_t clk_freq = 0;
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            clk_freq = RC_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO_2X:
            clk_freq = TCXO_2X_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO:
            clk_freq = TCXO_1X_CLK;
            break;
        case CLOCKS_CLK_SRC_MAX:
            break;
        default:
            clk_freq = clocks_get_pll_frequency(clk_src);
            break;
    }
    if (clk_div > CLOCK_DIV_1) {
        clk_freq = clk_freq / clk_div;
    }
    return clk_freq;
}

void clk_switch_to_rc(void)
{
    // disable GLB_CLKEN
    writew(GLB_CLKEN, 0x0);

    if (reg16_getbit(MSUB_NOR_CFG, 0x6) == 0x0) {
        // mcu ch1 to xo
        writew(MCU_CORE_CR_CH1, 0x1);
        // mcu core switch to ch1
        reg16_setbit(MSUB_NOR_CFG, 0x6);
        // mcu ch0 to xo
        writew(MCU_CORE_CR_CH0, 0x1);
    } else {
        // mcu ch0 to xo
        writew(MCU_CORE_CR_CH0, 0x1);
        // mcu core switch to ch0
        reg16_clrbit(MSUB_NOR_CFG, 0x6);
        // mcu ch1 to xo
        writew(MCU_CORE_CR_CH1, 0x1);
    }

    // set mcu_bus_div is no div
    writew(MCU_BUS_DIV, 0x0);
    writew(MCU_BUS_DIV, 0x3);

    // clk_mcu_perp_ls=clk_rc
    writew(MCU_PERP_LS_CR, 0x3);
    // clk_mcu_perp_uart=clk_rc
    writew(MCU_PERP_UART_CR, 0x3);
    // clk_mcu_perp_spi=clk_rc
    writew(MCU_PERP_SPI_CR, 0x1);

    if (reg16_getbit(CH_SEL, 0x0) == 0x1) {
        // set com bus ch0 to rc
        writew(COM_BUS_CR_CH0, 0x1);
        // com bus switch to ch0
        reg16_clrbit(CH_SEL, 0x0);
        // set com bus ch1 to rc
        writew(COM_BUS_CR_CH1, 0x1);
    } else {
        // set com bus ch1 to rc
        writew(COM_BUS_CR_CH1, 0x1);
        // com bus switch to ch1
        reg16_setbit(CH_SEL, 0x0);
        // set com bus ch0 to rc
        writew(COM_BUS_CR_CH0, 0x1);
    }

    // set clk_sdiom=clk_rc
    writew(SDIOM_CR, 0x1);
    // clk_mem_bus=clk_rc
    writew(MEM_BUS_CR, 0x1);
    // clk_xip_opi=clk_rc
    writew(XIP_OPI_CR, 0x1);
    // clk_xip_qspi=clk_rc
    writew(XIP_QSPI_CR, 0x1);
    // clk_gpu=clk_rc
    writew(GPU_CR, 0x1);

    if (reg16_getbit(CH_SEL, 0x1) == 0x1) {
        // set hifi ch0 to rc
        writew(HIFI_CR_CH0, 0x1);
        // hifi switch to ch0
        reg16_clrbit(CH_SEL, 0x1);
        // set com bus ch1 to rc
        writew(HIFI_CR_CH1, 0x1);
    } else {
        // set hifi bus ch1 to rc
        writew(HIFI_CR_CH1, 0x1);
        // hifi switch to ch1
        reg16_setbit(CH_SEL, 0x1);
        // set hifi bus ch0 to rc
        writew(HIFI_CR_CH0, 0x1);
    }

    if (reg16_getbit(CH_SEL, 0x2) == 0x1) {
        // set codec ch0 to rc
        writew(CODEC_CR_CH0, 0x1);
        // codec switch to ch0
        reg16_clrbit(CH_SEL, 0x2);
        // set codec ch1 to rc
        writew(CODEC_CR_CH1, 0x1);
    } else {
        // set codec ch1 to rc
        writew(CODEC_CR_CH1, 0x1);
        // codec switch to ch1
        reg16_setbit(CH_SEL, 0x2);
        // set codec ch0 to rc
        writew(CODEC_CR_CH0, 0x1);
    }

    // enable GLB_CLKEN
    writew(GLB_CLKEN, 0x3ffe);
}