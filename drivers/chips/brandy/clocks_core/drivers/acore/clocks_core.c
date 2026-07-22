/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  CLOCKS CORE PRIVATE SOURCE.
 */
#include "clocks_core.h"
#include <stdint.h>
#include "securec.h"
#include "clocks_switch.h"
#include "soc_osal.h"
#ifdef USE_CMSIS_OS
#include "pmu_cmu.h"
#endif
#include "pmu_ldo.h"
#include "hal_pmu_vdd.h"
#include "hal_cpu_clocks.h"
#include "hal_clocks_app.h"
#include "product.h"
#include "hal_cpu_core.h"
#include "panic.h"
#include "debug_print.h"
#include "application_version.h"
#include "hal_xip.h"
#if !defined(BUILD_APPLICATION_SSB)
#include "log_uart.h"
#include "pm_definition.h"
#endif

#ifndef BUILD_APPLICATION_SSB
typedef union {
    struct {
        uint16_t dsp_clk_level;
        uint8_t  dsp0_div;
        uint8_t  dsp1_div;
    } freq;
    uint32_t data;
} dsp_freq_level_t;

typedef enum {
    IPC_DSP_FREQ_TCXO_32M,  // Dsp clk: 32M.
    IPC_DSP_FREQ_TCXO_64M,  // Dsp clk: 64M.
    IPC_DSP_FREQ_PLL_DIV6,  // Dsp clk: 580M / 6 = 96.667M. Need update
    IPC_DSP_FREQ_PLL_DIV4,  // Dsp clk: 580M / 4 = 145M. Need update
    IPC_DSP_FREQ_PLL_DIV3,  // Dsp clk: 580M / 3 = 193.333M. Need update
    IPC_DSP_FREQ_PLL_DIV2,  // Dsp clk: 580M / 2 = 290M. Need update
    IPC_DSP_FREQ_MAX,
} ipc_dsp_freq_t;
#endif

#if !defined(BUILD_APPLICATION_SSB)
static clocks_i2s_src_t g_i2s_clk_config = I2S_CLOCK_PLL_OFF;
static bool g_dsp_power_up = false;
static clocks_control_voltage_t g_current_0p8_voltage = CLDO_0P8_0V925;
//lint -esym(551, g_core_0p8_voltage)
static dsp_clock_freq_t g_dsp_clock_freq = {0};
static system_freq_t g_system_freq = {0};
static hal_pmu_vdd0p8_1p0_level_t g_0p8_bt_work_min_vset = HAL_PMU_VDD0P8_1P0_0P750V; // bt work need min voltage
static cmu_pll_config_t g_mcu_pll_200mhz_cfg = {0x12, 0xc0, 0x0, 0x3, 0x1};
static clocks_clk_cfg_t g_system_resume_clocks[CLOCKS_CCRG_MODULE_MAX];
static sdiom_clk_handler_func g_sdiom_handler = NULL;

void clocks_set_0p8_voltage_according_pvsensor(void)
{
}

void set_0p8_voltage_according_tempetature(bool below_zero_degree)
{
    uint32_t irq_sts = osal_irq_lock();
    if (below_zero_degree) {
        g_0p8_bt_work_min_vset = HAL_PMU_VDD0P8_1P0_0P750V;
    }

    if (g_current_0p8_voltage == CLDO_0P8_0V7) {
        pmu_vdd0p8_power_vset(g_0p8_bt_work_min_vset);
    }
    osal_irq_restore(irq_sts);
}
#endif

#if defined(BUILD_APPLICATION_SSB)
#define PMU_VDD0P8_1P0_0P950V 14
void clocks_ssb_clk_init(void)
{
    pmu_vdd0p8_power_vset(PMU_VDD0P8_1P0_0P950V);
    clocks_clk_cfg_t const *clk_cfg = clocks_ssb_clk_cfg_get();
    for (uint32_t i = 0; i < CLOCKS_CCRG_MODULE_MAX; i++) {
        system_ccrg_clock_config((clocks_ccrg_module_t)i, clk_cfg[i].clk_src, clk_cfg[i].clk_div);
    }
    PRINT("SSB used %s clock."NEWLINE, clk_cfg[0].clk_src == CLOCKS_CLK_SRC_TCXO_2X ? "tcxo 2x" : "pll");
    uint32_t sec_ret = (uint32_t)memcpy_s((void *)SHARED_MEM_START, sizeof(clocks_clk_cfg_t) * CLOCKS_CCRG_MODULE_MAX,
                                          (uint8_t *)clk_cfg, sizeof(clocks_clk_cfg_t) * CLOCKS_CCRG_MODULE_MAX);
    if (sec_ret != 0) {
        PRINT("SSB memory copy failed."NEWLINE);
    }

    hal_clocks_ccrg_mcu_bus_div_set(CLOCK_DIV_1);
}
#else
bool clocks_app_clk_init(system_clocks_config_t clock_config)
{
    if (clock_config >= CLOCKS_CONFIG_MAX) {
        return false;
    }

    clocks_clk_cfg_t const *system_clocks = clocks_system_all_clocks_get(clock_config);
    for (uint8_t module = CLOCKS_CCRG_MODULE_MCU_CORE; module < CLOCKS_CCRG_MODULE_MAX; module++) {
        system_ccrg_clock_config(module, system_clocks[module].clk_src, system_clocks[module].clk_div);
        g_system_resume_clocks[module].clk_src = CLOCKS_CLK_SRC_MAX;
    }

    if (clock_config == CLOCKS_ALL_TCXO_32M) {
        bt_clocks_set_freq(CLOCKS_CLK_SRC_TCXO);
    } else {
        bt_clocks_set_freq(CLOCKS_CLK_SRC_TCXO_2X);
    }

    return true;
}
#endif

#ifndef BUILD_APPLICATION_SSB
static void master_inform_slave_adjust_freq(cores_t core, uint32_t freq)
{
    ipc_payload_adjust_slave_freq_t ipc_freq;
    ipc_freq.slave_freq = freq;
    (void)ipc_send_message(core, IPC_ACTION_SLAVE_ADJUST_FREQ, (ipc_payload *)&ipc_freq,
                           sizeof(ipc_payload_adjust_slave_freq_t), IPC_PRIORITY_HIGHEST, false);
}

void bt_clocks_set_freq(clocks_clk_src_t bt_core_clk)
{
    if (!hal_cpu_is_bt_enabled()) {
        return;
    }
    bool bclk_is_tcxo_2x = hal_clocks_bcpu_get_normal_config();
    if (bt_core_clk == CLOCKS_CLK_SRC_TCXO) {
        if (bclk_is_tcxo_2x) {
            master_inform_slave_adjust_freq(CORES_BT_CORE, (uint32_t)CLOCKS_CLK_SRC_TCXO);
        }
    } else if (bt_core_clk == CLOCKS_CLK_SRC_TCXO_2X) {
        if (!bclk_is_tcxo_2x) {
            master_inform_slave_adjust_freq(CORES_BT_CORE, (uint32_t)CLOCKS_CLK_SRC_TCXO_2X);
        }
    } else {
        panic(PANIC_CLOCKS_SWITCH, __LINE__);
    }
}

static void master_inform_dsp_current_freq(cores_t core, uint32_t freq)
{
    ipc_payload_dsp_current_freq_t ipc_freq;
    ipc_freq.freq = freq;
    (void)ipc_send_message(core, IPC_ACTION_DSP_CURRENT_FREQ, (ipc_payload *)&ipc_freq,
                           sizeof(ipc_payload_dsp_current_freq_t), IPC_PRIORITY_HIGHEST, false);
}

static void send_current_clk_to_dsp(system_clocks_freq_t clocks_freq, uint8_t dsp0_div, uint8_t dsp1_div)
{
    dsp_freq_level_t dsp_freq_level;
    // The DSP frequency is cycled every IPC_DSP_FREQ_MAX levels.
    uint8_t level = (uint8_t)clocks_freq % IPC_DSP_FREQ_MAX;
    if (clocks_freq == CLOCKS_M_H_B64_D_H) {
        level = IPC_DSP_FREQ_PLL_DIV2;
    }
    dsp_freq_level.freq.dsp_clk_level = level;
    dsp_freq_level.freq.dsp0_div = dsp0_div;
    dsp_freq_level.freq.dsp1_div = dsp1_div;
    master_inform_dsp_current_freq(CORES_PROTOCOL_CORE, dsp_freq_level.data);
}

clocks_clk_src_t get_bt_clock_config(void)
{
    bool bclk_is_tcxo_2x = hal_clocks_bcpu_get_normal_config();
    if (bclk_is_tcxo_2x) {
        return CLOCKS_CLK_SRC_TCXO_2X;
    } else {
        return CLOCKS_CLK_SRC_TCXO;
    }
}
#endif

#ifndef BUILD_APPLICATION_SSB
void sdiom_internal_freq_handler_register(sdiom_clk_handler_func func)
{
    g_sdiom_handler = func;
}

bool clocks_set_sdio_freq(system_sdio_freq_t clk_level)
{
    if (clk_level == CLOCKS_SDIO_FREQ_MAX) {
        return false;
    }
    clocks_clk_cfg_t sdio_freq = clocks_system_sdio_freq_get(clk_level);
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_SDIOM, sdio_freq.clk_src, sdio_freq.clk_div);
    return true;
}

void set_dsp_power_up_status(slave_cpu_t slave_cpu, bool status)
{
    if (slave_cpu >= SLAVE_CPU_BT) {
        return;
    }
    uint32_t irq_status = osal_irq_lock();
    if (slave_cpu == SLAVE_CPU_DSP0) {
        g_dsp_power_up = status;
    }
    osal_irq_restore(irq_status);
}

bool get_dsp_power_up_status(slave_cpu_t slave_cpu)
{
    if (slave_cpu == SLAVE_CPU_DSP0) {
        return g_dsp_power_up;
    }

    return false;
}

static void system_membus_and_qspi_set_freq(clocks_clk_src_t clk_type, uint8_t clk_div)
{
    if (clk_type == CLOCKS_CLK_SRC_MAX) {
        clk_type = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_src;
        clk_div = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_div;
        clk_div *= 2;  // mcu_clk is membus_clk * 2.
    }
    // mem bus div cfg, must less than 2*xip_qspi clk
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MEM_BUS, clk_type, clk_div);
    // qspi clk cfg
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_XIP_QSPI, clk_type, clk_div);
}

static void mcu_clock_set_freq(clocks_clk_src_t mcu_src, uint8_t mcu_div)
{
    uint32_t irq_status = osal_irq_lock();
    if (mcu_src == CLOCKS_CLK_SRC_PLL && mcu_div <= CLOCK_DIV_4) {
        hal_clocks_ccrg_mcu_bus_div_set(CLOCK_DIV_2);
    } else {
        hal_clocks_ccrg_mcu_bus_div_set(CLOCK_DIV_0);
    }
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MCU_CORE, mcu_src, mcu_div);
    osal_irq_restore(irq_status);
}

uint16_t get_sdiom_freq_level(clocks_clk_src_t src)
{
    uint16_t level = CLOCKS_SDIO_FREQ_32M;
    if (src == CLOCKS_CLK_SRC_RC) {
        level = CLOCKS_SDIO_FREQ_50M;
    } else if (src == CLOCKS_CLK_SRC_TCXO) {
        level = CLOCKS_SDIO_FREQ_32M;
    } else if (src == CLOCKS_CLK_SRC_TCXO_2X) {
        level = CLOCKS_SDIO_FREQ_64M;
    } else if (src == CLOCKS_CLK_SRC_PLL) {
        level = CLOCKS_SDIO_FREQ_H;
    }
    return level;
}

static void mcu_clocks_set_freq(system_mcu_core_clk_cfg_t sys_mcu_clks)
{
    uint16_t level = CLOCKS_SDIO_FREQ_32M;
    clocks_clk_src_t mcu_core_clk = sys_mcu_clks.mcu_clk.clk_src;
    uint8_t mcu_clk_div = sys_mcu_clks.mcu_clk.clk_div;
    clocks_clk_src_t psram_clk = sys_mcu_clks.psram_clk.clk_src;
    uint8_t psram_clk_div = sys_mcu_clks.psram_clk.clk_div;
    clocks_clk_src_t mem_clk = sys_mcu_clks.mem_clk.clk_src;
    uint8_t mem_clk_div = sys_mcu_clks.mem_clk.clk_div;
    clocks_clk_src_t flash_clk = sys_mcu_clks.flash_clk.clk_src;
    uint8_t flash_clk_div = sys_mcu_clks.flash_clk.clk_div;
    clocks_clk_src_t sdiom_clk = sys_mcu_clks.sdiom_clk.clk_src;
    uint8_t sdiom_clk_div = sys_mcu_clks.sdiom_clk.clk_div;
    clocks_clk_src_t gpu_clk = sys_mcu_clks.gpu_clk.clk_src;
    uint8_t gpu_clk_div = sys_mcu_clks.gpu_clk.clk_div;
    clocks_clk_src_t spi_clk = sys_mcu_clks.spi_clk.clk_src;
    uint8_t spi_clk_div = sys_mcu_clks.spi_clk.clk_div;

    mcu_clock_set_freq(mcu_core_clk, mcu_clk_div);
    uint32_t irq_status = osal_irq_lock();
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_XIP_OPI, psram_clk, psram_clk_div);
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MEM_BUS, mem_clk, mem_clk_div);
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_XIP_QSPI, flash_clk, flash_clk_div);
#ifdef CFG_DRIVERS_MMC
    level = get_sdiom_freq_level(sdiom_clk);
    if (g_sdiom_handler != NULL) { g_sdiom_handler(level); }
#else
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_SDIOM, sdiom_clk, sdiom_clk_div);
#endif
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_GPU, gpu_clk, gpu_clk_div);
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MCU_PERP_SPI, spi_clk, spi_clk_div);
    if (mcu_core_clk == CLOCKS_CLK_SRC_TCXO && mcu_clk_div == CLOCK_DIV_2) {
        system_ccrg_clock_config(CLOCKS_CCRG_MODULE_COM_BUS, CLOCKS_CLK_SRC_TCXO, CLOCK_DIV_2);
    }
    osal_irq_restore(irq_status);
}

static clocks_control_voltage_t core_get_0p8_vol_according_mcu(clocks_control_voltage_t need_voltage,
                                                               clocks_clk_src_t mcu_core_clk,
                                                               uint8_t mcu_clk_div)
{
    if (mcu_core_clk == CLOCKS_CLK_SRC_PLL) {
        if (mcu_clk_div <= CLOCK_DIV_4) {
            return CLDO_0P8_0V95;
        }
    } else if (mcu_core_clk == CLOCKS_CLK_SRC_TCXO) {
        if (mcu_clk_div == CLOCK_DIV_2) {
            return CLDO_0P8_0V625;
        }
    }
    return need_voltage;
}

static clocks_control_voltage_t core_get_0p8_vol_according_dsp(clocks_control_voltage_t need_voltage,
                                                               clocks_clk_src_t dsp_core_clk,
                                                               uint8_t dsp_clk_div)
{
    if (dsp_core_clk == CLOCKS_CLK_SRC_PLL) {
        if (dsp_clk_div < CLOCK_DIV_6) {
            return CLDO_0P8_0V95;
        }
    }
    return need_voltage;
}

static clocks_control_voltage_t core_get_0p8_required_voltage(clocks_clk_src_t mcu_core_clk, uint8_t mcu_clk_div,
                                                              clocks_clk_src_t dsp_core_clk, uint8_t dsp_clk_div)
{
    clocks_control_voltage_t need_voltage = CLDO_0P8_0V85;
    need_voltage = core_get_0p8_vol_according_mcu(need_voltage, mcu_core_clk, mcu_clk_div);
    need_voltage = core_get_0p8_vol_according_dsp(need_voltage, dsp_core_clk, dsp_clk_div);
    return need_voltage;
}

static bool judge_and_raise_0p8_voltage(clocks_control_voltage_t need_voltage)
{
    if (need_voltage == CLDO_0P8_0V95) {
        if (g_current_0p8_voltage == CLDO_0P8_0V85) {
            uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P950V);
        } else if (g_current_0p8_voltage == CLDO_0P8_0V625) {
            // Firstly, need modify cldo1 bypass, secondly modify buck0p8.
            hal_pmu_cldo_enable_nor_bypass(PMU_LDO_CLDO1, TURN_ON);
            pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_OFF);
            uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P950V);
        }
    } else if (need_voltage == CLDO_0P8_0V85) {
        if (g_current_0p8_voltage == CLDO_0P8_0V625) {
            // Firstly, need modify cldo1 bypass, secondly modify buck0p8.
            hal_pmu_cldo_enable_nor_bypass(PMU_LDO_CLDO1, TURN_ON);
            pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_OFF);
            uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P950V);
        }
    }

    return true;
}

static bool judge_and_reduce_0p8_voltage(clocks_control_voltage_t need_voltage)
{
    if (need_voltage == CLDO_0P8_0V85) {
        if (g_current_0p8_voltage > CLDO_0P8_0V85) {
            uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P850V);
        }
    } else if (need_voltage == CLDO_0P8_0V625) {
        if (g_current_0p8_voltage > CLDO_0P8_0V625) {
            // Firstly, need modify cldo1 man, secondly modify cldo1.
            pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_ON);
            hal_pmu_cldo_enable_nor_bypass(PMU_LDO_CLDO1, TURN_OFF);
            uapi_pmu_vset(PMU_ID_LDO_CLDO1, PMU_CLDO_LEVEL_0P625V);
        }
    }
    return true;
}

bool clocks_set_mcu_freq(system_mcu_freq_t clk_level)
{
    if (clk_level == CLOCKS_MCU_FREQ_MAX) {
        return false;
    }
    system_mcu_core_clk_cfg_t mcu_freq = clocks_system_mcu_freq_get(clk_level);
    clocks_clk_src_t mcu_core_clk = mcu_freq.mcu_clk.clk_src;
    uint8_t mcu_clk_div = mcu_freq.mcu_clk.clk_div;

    clocks_control_voltage_t need_voltage;
    uint32_t irq_status = osal_irq_lock();
    need_voltage = core_get_0p8_required_voltage(mcu_core_clk, mcu_clk_div,
                                                 clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_HIFI].clk_src,
                                                 clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_HIFI].clk_div);
    // Raise the voltage first.
    if (!judge_and_raise_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_status);
        return false;
    }

    // Config the frequency again.
    mcu_clocks_set_freq(mcu_freq);
    // Reduce the voltage finally.
    if (!judge_and_reduce_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_status);
        return false;
    }
    g_current_0p8_voltage = need_voltage;
    g_system_freq.mcu_only_flag = true;
    g_system_freq.system_mcu_freq = clk_level;
    osal_irq_restore(irq_status);
    return true;
}

bool system_clocks_set_freq(system_clocks_freq_t clocks_freq, uint8_t dsp0_div, uint8_t dsp1_div)
{
    system_clocks_freq_config_t sys_clocks = clocks_system_freq_cfg_get(clocks_freq);
    clocks_clk_src_t mcu_core_clk = sys_clocks.mcu_core_clk.clk_src;
    uint8_t mcu_clk_div = sys_clocks.mcu_core_clk.clk_div;
    clocks_clk_src_t bt_core_clk = sys_clocks.bt_core_clk;
    clocks_clk_src_t dsp_core_clk = sys_clocks.dsp_core_clk.clk_src;
    uint8_t dsp_clk_div = sys_clocks.dsp_core_clk.clk_div;

    uint32_t irq_sts = osal_irq_lock();

    clocks_control_voltage_t need_voltage = core_get_0p8_required_voltage(mcu_core_clk, mcu_clk_div,
                                                                          dsp_core_clk, dsp_clk_div);
    // Raise the voltage first.
    if (!judge_and_raise_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_sts);
        return false;
    }

    // Config the frequency again.
    bt_clocks_set_freq(bt_core_clk);
    mcu_clock_set_freq(mcu_core_clk, mcu_clk_div);
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_HIFI, dsp_core_clk, dsp_clk_div);

    // Reduce the voltage finally.
    if (!judge_and_reduce_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_sts);
        return false;
    }
    g_current_0p8_voltage = need_voltage;
    if (get_dsp_power_up_status(SLAVE_CPU_DSP0)) {
        clocks_clk_cfg_t dsp_freq_cfg[HAL_CLOCKS_DSP_MAX] = { 0 };
        dsp_freq_cfg[HAL_CLOCKS_DSP0].clk_src = dsp_core_clk;
        dsp_freq_cfg[HAL_CLOCKS_DSP0].clk_div = dsp0_div;
        dsp_freq_cfg[HAL_CLOCKS_DSP1].clk_src = dsp_core_clk;
        dsp_freq_cfg[HAL_CLOCKS_DSP1].clk_div = dsp1_div;
        clocks_set_dsp_freq(dsp_freq_cfg, sizeof(dsp_freq_cfg) / sizeof(clocks_clk_cfg_t));
        memset_s(&g_dsp_clock_freq, sizeof(g_dsp_clock_freq), 0, sizeof(g_dsp_clock_freq));
        send_current_clk_to_dsp(clocks_freq, dsp0_div, dsp1_div);
    } else {
        g_dsp_clock_freq.dsp_clk = dsp_core_clk;
        g_dsp_clock_freq.dsp0_div = dsp0_div;
        g_dsp_clock_freq.dsp1_div = dsp1_div;
    }
    g_system_freq.system_all_freq = clocks_freq;
    g_system_freq.dsp0_div = dsp0_div;
    g_system_freq.dsp1_div = dsp1_div;
    g_system_freq.mcu_only_flag = false;

    osal_irq_restore(irq_sts);

    return true;
}

system_freq_t get_system_freq(void)
{
    return g_system_freq;
}

bool system_display_set_freq(system_display_freq_t clk_level)
{
    UNUSED(clk_level);
    return true;
}

bool clocks_set_dsp_freq(clocks_clk_src_t clk_type, uint8_t clk_div)
{
    if (clk_type > CLOCKS_CLK_SRC_MAX) {
        return false;
    }
    clocks_clk_src_t hifi_clk = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_HIFI].clk_src;
    uint8_t hifi_clk_div =  clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_HIFI].clk_div;
    if ((clk_type == hifi_clk) && (clk_div == hifi_clk_div)) {
        return true;
    }
    clocks_control_voltage_t need_voltage;
    uint32_t irq_status = osal_irq_lock();
    need_voltage = core_get_0p8_required_voltage(clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_src,
                                                 clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_div,
                                                 clk_type, clk_div);
    if (!judge_and_raise_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_status);
        return false;
    }
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_HIFI, clk_type, clk_div);
    // Reduce the voltage finally.
    if (!judge_and_reduce_0p8_voltage(need_voltage)) {
        osal_irq_restore(irq_status);
        return false;
    }
    g_current_0p8_voltage = need_voltage;
    osal_irq_restore(irq_status);
    return true;
}

void clocks_set_codec_freq(clocks_clk_src_t clk_type, uint8_t clk_div)
{
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_CODEC, clk_type, clk_div);
}
#endif

uint32_t clocks_get_spi_clock_value(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }
    return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_MCU_PERP_SPI);
}

uint32_t clocks_get_xip_clock_value(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_CLOCKS_CORE) != true) {
        return TCXO_1X_CLK;
    }

    return clocks_get_module_frequency(CLOCKS_CCRG_MODULE_XIP_QSPI);
}

#ifndef BUILD_APPLICATION_SSB
void clocks_system_resume_clock(void)
{
    for (uint32_t i = 0; i < CLOCKS_CCRG_MODULE_MAX; i++) {
        clocks_clk_src_t clk_src = g_system_resume_clocks[i].clk_src;
        if (clk_src != CLOCKS_CLK_SRC_MAX) {
            system_ccrg_clock_config((clocks_ccrg_module_t)i, clk_src, g_system_resume_clocks[i].clk_div);
            g_system_resume_clocks[i].clk_src = CLOCKS_CLK_SRC_MAX;
            g_system_resume_clocks[i].clk_div = CLOCK_DIV_1;
        }
    }
    system_ccrg_clock_perpls_set_reload(true);
    clocks_clk_src_t mcu_clk_src = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_src;
    uint8_t mcu_clk_div = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_div;
    if (mcu_clk_src == CLOCKS_CLK_SRC_TCXO && mcu_clk_div == CLOCK_DIV_2) {
        // if the clock is 16 MHz, need modify cldo1 man at wakup.
        pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_ON);
        hal_pmu_cldo_enable_nor_bypass(PMU_LDO_CLDO1, TURN_OFF);
        uapi_pmu_vset(PMU_ID_LDO_CLDO1, PMU_CLDO_LEVEL_0P625V);
    }
}

void clocks_system_suspend_clock(void)
{
    clocks_clk_src_t clk_src;
    uint8_t clk_div;
    system_ccrg_clock_perpls_set_reload(false);
    for (uint32_t i = CLOCKS_CCRG_MODULE_MCU_PERP_LS; i < CLOCKS_CCRG_MODULE_MAX; i++) {
        clk_src = clocks_get_current_clock_cfg()[(clocks_ccrg_module_t)i].clk_src;
        if (clk_src != CLOCKS_CLK_SRC_MAX) {
            g_system_resume_clocks[i].clk_src = clk_src;
            g_system_resume_clocks[i].clk_div = clocks_get_current_clock_cfg()[(clocks_ccrg_module_t)i].clk_div;
            if (i != CLOCKS_CCRG_MODULE_MCU_PERP_UART && i != CLOCKS_CCRG_MODULE_MCU_PERP_LS) {
                system_ccrg_clock_config((clocks_ccrg_module_t)i, CLOCKS_CLK_SRC_RC, CLOCK_DIV_1);
            }
        }
    }

    clk_src = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_src;
    clk_div = clocks_get_current_clock_cfg()[CLOCKS_CCRG_MODULE_MCU_CORE].clk_div;
    g_system_resume_clocks[0].clk_src = clk_src;
    g_system_resume_clocks[0].clk_div = clk_div;
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_MCU_CORE, CLOCKS_CLK_SRC_RC, CLOCK_DIV_1);
    if (clk_src == CLOCKS_CLK_SRC_TCXO && clk_div == CLOCK_DIV_2) {
        // if the clock is 16 MHz, need modify cldo1 bypass at sleep.
        hal_pmu_cldo_enable_nor_bypass(PMU_LDO_CLDO1, TURN_ON);
        pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_OFF);
    }
}
#endif