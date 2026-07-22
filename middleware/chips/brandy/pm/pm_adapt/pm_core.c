/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm core adapter code
 */

#include "pm_core.h"
#include "platform.h"
#include "chip_io.h"
#include "systick.h"
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#include "touch_screen_def.h"
#endif
#include "arch_encoding.h"
#include "tcxo_porting.h"
#include "hal_pmu_ldo.h"
#include "arch_barrier.h"
#include "pmu_ldo.h"

#if (ARCH == RISCV70)
#define CORE_RESET_PC_L            (GLB_CTL_M_RB_BASE + 0x4)
#define CORE_RESET_PC_H            (GLB_CTL_M_RB_BASE + 0x8)
#define CORE_SUSPEND_REGS_CNT      152
#else
#define CORE_RESET_PC_L            (GLB_CTL_B_RB_BASE + 0x4)
#define CORE_RESET_PC_H            (GLB_CTL_B_RB_BASE + 0x8)
#define CORE_SUSPEND_REGS_CNT      128
#endif
#define CLIC_SUSPEND_REGS_CNT      96
#define PM_UART_FR                 0x18
#define PM_UARTFR_TXFE_MASK        0x80

typedef struct {
    uint32_t reg_cfg;
    uint8_t reg_int_ie[CLIC_SUSPEND_REGS_CNT];
    uint8_t reg_int_ip[CLIC_SUSPEND_REGS_CNT];
    uint8_t reg_int_attr[CLIC_SUSPEND_REGS_CNT];
    uint8_t reg_int_ctl[CLIC_SUSPEND_REGS_CNT];
} clic_regs_group;

static clic_regs_group g_clic_regs;
static uint32_t g_core_reset_pc;
uint32_t g_core_suspend_regs[CORE_SUSPEND_REGS_CNT] = {0};

static void clic_suspend(void)
{
    reg8_read(CLIC_BASE_REG + SYS_CLIC_CFG, g_clic_regs.reg_cfg);
    for (uint8_t index = 0; index < CLIC_SUSPEND_REGS_CNT; index++) {
        reg8_read(CLIC_BASE_REG + sys_clic_intie(index), g_clic_regs.reg_int_ie[index]);
        reg8_read(CLIC_BASE_REG + sys_clic_intip(index), g_clic_regs.reg_int_ip[index]);
        reg8_read(CLIC_BASE_REG + sys_clic_intattr(index), g_clic_regs.reg_int_attr[index]);
        reg8_read(CLIC_BASE_REG + sys_clic_intctl(index), g_clic_regs.reg_int_ctl[index]);
    }
}

void pm_core_close_all_irq(void)
{
    for (uint8_t index = 0; index < CLIC_SUSPEND_REGS_CNT; index++) {
        reg8_write(CLIC_BASE_REG + sys_clic_intie(index), 0x0);
    }
}

static void clic_resume(void)
{
    reg8_write(CLIC_BASE_REG + SYS_CLIC_CFG, g_clic_regs.reg_cfg);
    for (uint8_t index = 0; index < CLIC_SUSPEND_REGS_CNT; index++) {
        reg8_write(CLIC_BASE_REG + sys_clic_intie(index), g_clic_regs.reg_int_ie[index]);
        reg8_write(CLIC_BASE_REG + sys_clic_intip(index), g_clic_regs.reg_int_ip[index]);
        reg8_write(CLIC_BASE_REG + sys_clic_intattr(index), g_clic_regs.reg_int_attr[index]);
        reg8_write(CLIC_BASE_REG + sys_clic_intctl(index), g_clic_regs.reg_int_ctl[index]);
    }
}

void pm_core_set_reset_pc(uint32_t cb)
{
    uint32_t wakeup_pc = cb;
    writel(CORE_RESET_PC_L, (uint16_t)wakeup_pc);
    writel(CORE_RESET_PC_H, (uint16_t)(wakeup_pc >> 16)); // shift 16bit
}

void pm_core_save_reset_pc(void)
{
    g_core_reset_pc = (uint32_t)(readl(CORE_RESET_PC_H) << 16) | (readl(CORE_RESET_PC_L)); // shift 16bit
}

void pm_core_restore_reset_pc(void)
{
    pm_core_set_reset_pc(g_core_reset_pc);
}
#ifdef SW_UART_DEBUG
bool pm_core_test_tx_empty_status(uint32_t base)
{
    while ((readl(base + PM_UART_FR) & PM_UARTFR_TXFE_MASK) != PM_UARTFR_TXFE_MASK) {
    }
    if ((readl(base + PM_UART_FR) & PM_UARTFR_TXFE_MASK) != PM_UARTFR_TXFE_MASK) {
        return false;
    } else {
        return true;
    }
}

void pm_core_wait_uart_tx_idle(void)
{
    while (pm_core_test_tx_empty_status(UART0_BASE) != true) {
    }
}
#endif

void pm_core_suspend(void)
{
#if (ARCH == RISCV70)
    clic_suspend();
    ArchDCacheFlush();
#endif
    uart_h0_suspend();
    uart_h1_suspend();
    uapi_dma_close();
    uapi_dma_deinit();
    display_suspend();
    storage_device_suspend();
    opi_psram_suspend();
    qspi0_suspend();
    pin_of_poweroff_suspend();
#ifdef CONFIG_CLOSED_ALL_IE
    pin_of_poweron_suspend();
#endif
    clocks_system_suspend_clock();
#ifdef SW_UART_DEBUG
    pm_core_wait_uart_tx_idle();
#endif
    hal_pmu_set_cmu_xo_trim(HAL_PMU_XO_CORE_DS_3_075X);
    pm_core_save_reset_pc();
    pm_core_set_reset_pc((uint32_t)lowpower_cpu_resume);
    lowpower_cpu_suspend(); // Must be placed at the end of this function
}

void pm_core_suspend_min(void)
{
#if (ARCH == RISCV70)
    clic_suspend();
    ArchDCacheFlush();
#endif
    uart_h0_suspend();
    uart_h1_suspend();
    uapi_dma_close();
    uapi_dma_deinit();

    storage_device_suspend();
    opi_psram_suspend();
    qspi0_suspend();
    pin_of_poweroff_suspend();
    pin_of_poweron_suspend();
    clocks_system_suspend_clock();
#ifdef SW_UART_DEBUG
    pm_core_wait_uart_tx_idle();
#endif
    hal_pmu_set_cmu_xo_trim(HAL_PMU_XO_CORE_DS_3_075X);
    pmu_ldo_power_on(PMU_LDO_AUXLDO1, TURN_ON); // poweron for flash component
    hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO1, TURN_ON);
    pmu_ldo_power_on(PMU_LDO_AUXLDO2, TURN_ON); // poweron for ioldo
    hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO2, TURN_ON);
    pm_core_save_reset_pc();
    pm_core_set_reset_pc((uint32_t)lowpower_cpu_resume);
    lowpower_cpu_suspend(); // Must be placed at the end of this function
}

void pm_core_resume_min(void)
{
#if (ARCH == RISCV70)
    clic_resume();
#endif
    clk_switch_to_rc();
    tcxo_port_enable(TCXO_ON);
    qspi0_resume();

    dsb();
    ArchICacheInvAll();
    ArchDCacheInvAll();
    dsb();

    pin_of_poweroff_resume();
    pin_port_recovery_function();
    uart_h0_resume();
    uart_h1_resume();
    hal_ipc_interrupt_en();

    writel(0x52000180, 0x23fc); // M_axi
    writel(0x52000210, 0xb);    // enable efuse

    opi_psram_resume();
    clocks_system_resume_clock();
    uapi_dma_init();
    uapi_dma_open();
    storage_device_resume();

    reg16_setbit(0x520003e4, 0x0); // set mcpu_clken_slp in MSUB_SLP_CFG
    reg16_setbit(0x520003e4, 0x1); // set mcpu_hclken_slp in MSUB_SLP_CFG
    clocks_auto_cg_config();
    clocks_bridging_low_powerp_config(true);
    system_mcu_clocks_config();
    hal_pmu_set_cmu_xo_trim(HAL_PMU_XO_CORE_DS_1_025X);
    pm_misc_regs_resume();
    pm_core_restore_reset_pc();
}


void pm_clocks_config_recovery(void)
{
    clocks_system_resume_clock();
    clocks_auto_cg_config();
    clocks_bridging_low_powerp_config(true);
    system_mcu_clocks_config();
}

void pm_core_resume(void)
{
#if (ARCH == RISCV70)
    clic_resume();
#endif
    clk_switch_to_rc();
    tcxo_port_enable(TCXO_ON);
    qspi0_resume();

    dsb();
    ArchICacheInvAll();
    ArchDCacheInvAll();
    dsb();
    pin_of_poweroff_resume();
    pin_port_recovery_function();
    pm_clocks_config_recovery();
    uart_h0_resume();
    uart_h1_resume();
    opi_psram_resume();
    uapi_dma_init();
    uapi_dma_open();
    cpu_trace_enable_mcpu_repeat();
    storage_device_resume();
    display_resume();
    if (non_os_get_nmi_raw_status() == 0x2) {
        uapi_watchdog_kick();
    }
    pm_misc_regs_resume();
    pm_core_restore_reset_pc();
}

uint64_t pm_core_get_current_ms(void)
{
    return uapi_systick_get_ms();
}
