/*
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description:   HAL CLOCKS MODULE
 */

#include "hal_clocks.h"
#include "soc_osal.h"
#include "debug_print.h"

#define CMU_CLOCKS_BSUB_SLEEP_REG   (B_CTL_RB_BASE + 0x254)
#define CMU_CLOCKS_BSUB_NORMAL_REG  (B_CTL_RB_BASE + 0x254)
#define CMU_BSUB_SLEEP_CH_SEL_BIT   3
#define CMU_BSUB_NORMAL_CH_SEL_BIT  7
#define BSUB_CFG_REFRESH_REG        (B_CTL_RB_BASE + 0x260)

#define CMU_CLOCKS_MSUB_SLEEP_REG   (M_CTL_RB_BASE + 0x3e4)
#define CMU_CLOCKS_MSUB_NORMAL_REG  (M_CTL_RB_BASE + 0x3e0)
#define CMU_CLOCKS_MSUB_CH_SEL_BIT  6
#define MSUB_CFG_REFRESH_REG        (M_CTL_RB_BASE + 0x3F0)

#define CMU_CLOCK_SUB_SLEEP_CONFIG  CMU_CLOCKS_MSUB_SLEEP_REG
#define DMA_CLKEN_SLP_BIT           3
#define BUS_CLKEN_SLP_BIT           2

#define CMU_CLOCK_SUB_NORMAL_CONFIG CMU_CLOCKS_MSUB_NORMAL_REG
#define DMA_CLKEN_NOR_BIT           3
#define BUS_CLKEN_NOR_BIT           2
#define SUB_CFG_REFRESH_REG         MSUB_CFG_REFRESH_REG
#define SUB_CFG_REFRESH_BIT         0

#define CHECK_FREQUENCY 1000
uint32_t g_clocks_check_reg = 0;
uint32_t g_clocks_check_reg_value = 0;
uint32_t g_clocks_check_reg_bit = 0;

void hal_clocks_config_bit(uint32_t reg_addr, switch_type_t on, REG16_POS position)
{
    uint32_t irq_sts = osal_irq_lock();
    if (on != 0) {
        reg16_setbit((void *)((uintptr_t)reg_addr), position);
    } else {
        reg16_clrbit((void *)((uintptr_t)reg_addr), position);
    }
    osal_irq_restore(irq_sts);
}

void hal_clocks_select_config(uint32_t reg_addr, switch_type_t on, REG16_POS position)
{
    if (on != 0) {
        reg16_setbit((void *)(uintptr_t)reg_addr, POS(position));
    } else {
        reg16_clrbit((void *)((uintptr_t)reg_addr), POS(position));
    }
}

bool hal_clocks_select_get_status(uint32_t reg_addr, uint16_t position)
{
    return reg16_getbit((void *)((uintptr_t)reg_addr), POS(position));
}

void hal_clocks_status_check(uint32_t addr, uint16_t offset, switch_type_t on)
{
    g_clocks_check_reg = addr;
    g_clocks_check_reg_bit = offset;
    while ((switch_type_t)hal_clocks_select_get_status(addr, offset) != on) {
        g_clocks_check_reg_value = readl((void *)((uintptr_t)g_clocks_check_reg));
    }

    g_clocks_check_reg = 0;
    g_clocks_check_reg_value = 0;
    g_clocks_check_reg_bit = 0;
}

#if !defined (BUILD_APPLICATION_ROM)
#include "panic.h"
#include "systick.h"

void hal_clocks_status_check_timeout(uint32_t addr, uint16_t offset, switch_type_t on, uint32_t timeout)
{
    uint64_t time = uapi_systick_get_ms();
    uint64_t check_frq = CHECK_FREQUENCY;

    g_clocks_check_reg = addr;
    g_clocks_check_reg_bit = offset;
    while ((switch_type_t)hal_clocks_select_get_status(addr, offset) != on) {
        check_frq--;
        if (check_frq == 0) {
            if ((time + timeout) < uapi_systick_get_ms()) {
                g_clocks_check_reg_value = readl((void *)((uintptr_t)g_clocks_check_reg));
#if (defined BUILD_APPLICATION_SSB)
                PRINT("CMU check timeout! addr is %x, with value %x\r\n", g_clocks_check_reg, g_clocks_check_reg_value);
#else
                panic(PANIC_CLOCKS_GLB_ERR, g_clocks_check_reg);
#endif
            } else {
                check_frq = CHECK_FREQUENCY;
            }
        }
    }

    g_clocks_check_reg = 0;
    g_clocks_check_reg_value = 0;
    g_clocks_check_reg_bit = 0;
}
#else
void hal_clocks_status_check_timeout(uint32_t addr, uint16_t offset, switch_type_t on, uint32_t timeout)
{
    UNUSED(timeout);
    while (hal_clocks_select_get_status(addr, offset) != on) { }
}
#endif

void hal_clocks_cpu_gating_mode(hal_clocks_cpu_gating_mode_type_t mode)
{
    UNUSED(mode);
#if CORE == MASTER_BY_ALL && CHIP_ASIC
    hal_clocks_bcpu_hclken_mode(mode);
#endif
}

void hal_clocks_dma_power_mode(hal_clocks_power_mode_type_t mode, switch_type_t on)
{
    if (mode == HAL_CLOCKS_POWER_NORMAL) {
        reg16_setbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
        hal_clocks_config_bit(CMU_CLOCK_SUB_NORMAL_CONFIG, on, (REG16_POS)DMA_CLKEN_NOR_BIT);
        reg16_clrbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
    } else {
        hal_clocks_config_bit(CMU_CLOCK_SUB_SLEEP_CONFIG, on, (REG16_POS)DMA_CLKEN_SLP_BIT);
    }
}

void hal_clocks_bus_power_mode(hal_clocks_power_mode_type_t mode, switch_type_t on)
{
    if (mode == HAL_CLOCKS_POWER_NORMAL) {
        reg16_setbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
        hal_clocks_config_bit(CMU_CLOCK_SUB_NORMAL_CONFIG, on, (REG16_POS)BUS_CLKEN_NOR_BIT);
        reg16_clrbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
    } else {
        hal_clocks_config_bit(CMU_CLOCK_SUB_SLEEP_CONFIG, on, (REG16_POS)BUS_CLKEN_SLP_BIT);
    }
}

void hal_clocks_sub_normal_cfg(hal_clocks_sub_nor_cfg_t sub_normal_cfg, switch_type_t on)
{
    reg16_setbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
    hal_clocks_config_bit(CMU_CLOCK_SUB_NORMAL_CONFIG, on, (REG16_POS)sub_normal_cfg);
    reg16_clrbit(SUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
}

void hal_clocks_sub_sleep_cfg(hal_clocks_sub_slp_cfg_t sub_sleep_cfg, switch_type_t on)
{
    hal_clocks_config_bit(CMU_CLOCK_SUB_SLEEP_CONFIG, on, (REG16_POS)sub_sleep_cfg);
}

void hal_clocks_bsub_ch_config(hal_clocks_power_mode_type_t mode, switch_type_t on)
{
    if (mode == HAL_CLOCKS_POWER_NORMAL) {
        reg16_setbit(BSUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
        hal_clocks_config_bit(CMU_CLOCKS_BSUB_NORMAL_REG, on, (REG16_POS)CMU_BSUB_NORMAL_CH_SEL_BIT);
        reg16_clrbit(BSUB_CFG_REFRESH_REG, SUB_CFG_REFRESH_BIT);
    } else {
        hal_clocks_config_bit(CMU_CLOCKS_BSUB_SLEEP_REG, on, (REG16_POS)CMU_BSUB_SLEEP_CH_SEL_BIT);
    }
}
