/*
 * Copyright (c) @CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description:  L2ram Memory Map configuration
 * Author: @CompanyNameTag
 * Create: 2020-06-25
 */

#include "hal_l2ram_private.h"
#include "soc_osal.h"
#include "chip_io.h"
#include "panic.h"
#include "systick.h"
#include "hal_reg_config.h"
#include "debug_print.h"

#define MEM_SHARE_MODE_CTRL         (M_CTL_RB_BASE + 0x0A50)
#define DSP_SHARE_MODE_BIT          0
#define DSP_SHARE_MODE_LEN          5
#define B_SHARE_MODE_BIT            9
#define B_SHARE_MODE_LEN            2

#define MEM_SHARE_CTRL_GT           (M_CTL_RB_BASE + 0x0A68)
#define DSP_SHARE_MODE_GT           0
#define BMEM_SHARE_MODE_GT          2

#define L2M_DSP_PWR_EN_CFG0     (PMU1_CTL_RB_BASE + 0x6C)
#define L2M_DSP_PWR_ACK_CFG0    (PMU1_CTL_RB_BASE + 0x70)
#define L2M_DSP_ISO_EN_CFG0     (PMU1_CTL_RB_BASE + 0x74)
#define L2M_DSP_ALL_ENABLE 0xFFFF
#define L2M_BT_PWR_EN_CFG0      (PMU1_CTL_RB_BASE + 0xA0C)
#define L2M_BT_PWR_ACK_CFG0     (PMU1_CTL_RB_BASE + 0xA14)
#define L2M_BT_ISO_EN_CFG0      (PMU1_CTL_RB_BASE + 0xA1C)
#define L2M_BT_RF_DIG_TOP_BIT        3
#define L2M_BT_PWR_MEM_BIT 0
#define L2M_BT_PWR_MEM_LEN 3
#define L2M_BT_PWR_MEM_ALL_ENABLE 0x7

#define L2RAM_MEM_POWER_EN_CFG0     (PMU1_CTL_RB_BASE + 0x60)
#define L2RAM_MEM_POWER_EN_CFG1     (PMU1_CTL_RB_BASE + 0x64)
#define L2RAM_MEM_RF_DIG_BIT        2
#define L2RAM_MEM_ACK_CFG0          (PMU1_CTL_RB_BASE + 0x68)
#define L2RAM_MEM_ACK_CFG1          (PMU1_CTL_RB_BASE + 0x6c)
#define L2RAM_ISO_EN_CFG0           (PMU1_CTL_RB_BASE + 0x70)
#define L2RAM_ISO_EN_CFG1           (PMU1_CTL_RB_BASE + 0x74)

#define HAL_MEM_APP_TIMEOUT         1000
#define CHECK_FREQUENCY             1000

void hal_dsp_memory_l2ram_config(hal_dsp_memory_share_config_t hal_memory_share_config)
{
    if (hal_memory_share_config >= HAL_MEM_SHARE_MAX) {
        return;
    }

    unsigned int irq_sts = osal_irq_lock();
    reg16_clrbit(MEM_SHARE_CTRL_GT, DSP_SHARE_MODE_GT);
    reg16_setbits(MEM_SHARE_MODE_CTRL, DSP_SHARE_MODE_BIT, DSP_SHARE_MODE_LEN, hal_memory_share_config);
    reg16_setbit(MEM_SHARE_CTRL_GT, DSP_SHARE_MODE_GT);
    osal_irq_restore(irq_sts);
}

void hal_b_memory_l2ram_config(hal_b_memory_share_config_t hal_memory_share_config)
{
    if (hal_memory_share_config > HAL_MEM_SHARE_256K_FOR_BT) {
        return;
    }

    unsigned int irq_sts = osal_irq_lock();
    reg16_clrbit(MEM_SHARE_CTRL_GT, BMEM_SHARE_MODE_GT);
    reg16_setbits(MEM_SHARE_MODE_CTRL, B_SHARE_MODE_BIT, B_SHARE_MODE_LEN, hal_memory_share_config);
    reg16_setbit(MEM_SHARE_CTRL_GT, BMEM_SHARE_MODE_GT);
    osal_irq_restore(irq_sts);
}

static void hal_pmu_l2ram_mem_power_ack(hal_pmu_l2ram_mem_type_t hal_l2ram_type)
{
    hal_reg_status_check_timeout(L2RAM_MEM_ACK_CFG0, (uint16_t)hal_l2ram_type, TURN_ON, HAL_MEM_APP_TIMEOUT);
}

static void hal_pmu_l2ram_mem_iso_enable(hal_pmu_l2ram_mem_type_t hal_l2ram_type, switch_type_t iso_switch)
{
    hal_reg_config_bit(L2RAM_ISO_EN_CFG0, iso_switch, (REG16_POS)hal_l2ram_type);
}

void hal_pmu_l2ram_mem_ram_power_enable(hal_pmu_l2ram_mem_type_t hal_l2ram_type, switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    if (power_switch != 0) {
        hal_reg_config_bit(L2RAM_MEM_POWER_EN_CFG0, power_switch, (REG16_POS)hal_l2ram_type);
        hal_pmu_l2ram_mem_power_ack(hal_l2ram_type);
        hal_pmu_l2ram_mem_iso_enable(hal_l2ram_type, TURN_OFF);
    } else {
        hal_pmu_l2ram_mem_iso_enable(hal_l2ram_type, TURN_ON);
        hal_reg_config_bit(L2RAM_MEM_POWER_EN_CFG0, power_switch, (REG16_POS)hal_l2ram_type);
    }
    osal_irq_restore(irq_sts);
}

switch_type_t hal_memory_l2ram_get_power_mode(hal_memory_l2ram_region_type_t region)
{
    hal_memory_l2ram_region_type_t bt_region;
    if (region >= HAL_MEMORY_L2RAM_REGION_MAX) {
        return TURN_OFF;
    }
    if (region <= HAL_MEMORY_L2RAM_DSP_REGION_30_31) {
        return (switch_type_t)reg16_getbit(L2M_DSP_PWR_EN_CFG0, (REG16_POS)region);
    }
    bt_region = region - HAL_MEMORY_L2RAM_BT_REGION_0_1;
    return (switch_type_t)reg16_getbit(L2M_BT_PWR_EN_CFG0, (REG16_POS)bt_region);
}

static void hal_pmu_l2ram_dsp_power_enable(hal_memory_l2ram_region_type_t region, switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    if (power_switch != 0) {
        hal_reg_config_bit(L2M_DSP_PWR_EN_CFG0, power_switch, (REG16_POS)region);
        hal_reg_status_check_timeout(L2M_DSP_PWR_ACK_CFG0, (uint16_t)region, TURN_ON, HAL_MEM_APP_TIMEOUT);
        hal_reg_config_bit(L2M_DSP_ISO_EN_CFG0, TURN_OFF, (REG16_POS)region);
    } else {
        hal_reg_config_bit(L2M_DSP_ISO_EN_CFG0, TURN_ON, (REG16_POS)region);
        hal_reg_config_bit(L2M_DSP_PWR_EN_CFG0, power_switch, (REG16_POS)region);
    }
    osal_irq_restore(irq_sts);
}

static void hal_pmu_l2ram_bt_power_enable(hal_memory_l2ram_region_type_t region, switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    hal_memory_l2ram_region_type_t bt_region = region - HAL_MEMORY_L2RAM_BT_REGION_0_1;
    if (power_switch != 0) {
        hal_reg_config_bit(L2M_BT_PWR_EN_CFG0, power_switch, (REG16_POS)bt_region);
        hal_reg_status_check_timeout(L2M_BT_PWR_ACK_CFG0, (uint16_t)bt_region, TURN_ON, HAL_MEM_APP_TIMEOUT);
        hal_reg_config_bit(L2M_BT_ISO_EN_CFG0, TURN_OFF, (REG16_POS)bt_region);
    } else {
        hal_reg_config_bit(L2M_BT_ISO_EN_CFG0, TURN_ON, (REG16_POS)bt_region);
        hal_reg_config_bit(L2M_BT_PWR_EN_CFG0, power_switch, (REG16_POS)bt_region);
    }
    osal_irq_restore(irq_sts);
}

void hal_memory_l2ram_set_power_mode(hal_memory_l2ram_region_type_t region, switch_type_t power_switch)
{
    if (region >= HAL_MEMORY_L2RAM_REGION_MAX) {
        return;
    }
    if (region <= HAL_MEMORY_L2RAM_DSP_REGION_30_31) {
        hal_pmu_l2ram_dsp_power_enable(region, power_switch);
    } else {
        hal_pmu_l2ram_bt_power_enable(region, power_switch);
    }
}

void hal_pmu_rf_dig_power_enable(switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    if (power_switch != 0) {
        hal_reg_config_bit(L2M_BT_PWR_EN_CFG0, power_switch, L2M_BT_RF_DIG_TOP_BIT);
        hal_reg_status_check_timeout(L2M_BT_PWR_ACK_CFG0, L2M_BT_RF_DIG_TOP_BIT, TURN_ON, HAL_MEM_APP_TIMEOUT);
        hal_reg_config_bit(L2M_BT_ISO_EN_CFG0, TURN_OFF, L2M_BT_RF_DIG_TOP_BIT);
    } else {
        hal_reg_config_bit(L2M_BT_ISO_EN_CFG0, TURN_ON, L2M_BT_RF_DIG_TOP_BIT);
        hal_reg_config_bit(L2M_BT_PWR_EN_CFG0, power_switch, L2M_BT_RF_DIG_TOP_BIT);
    }
    osal_irq_restore(irq_sts);
}

static void hal_reg_status_mask_check_timeout(uint32_t addr, uint16_t value, uint16_t mask, uint32_t timeout)
{
    uint64_t time = uapi_systick_get_ms();
    uint64_t check_frq = CHECK_FREQUENCY;
    uint16_t reg_val;
    reg16_read((void *)((uintptr_t)addr), reg_val);
    while ((reg_val & mask) != value) {
        check_frq--;
        if (check_frq == 0) {
            if ((time + (uint64_t)timeout) < uapi_systick_get_ms()) {
                reg16_read((void *)((uintptr_t)addr), reg_val);
                PRINT("pmu status timeout!! pmu addr is %x, with value %x\r\n", addr, reg_val);
                panic(PANIC_PMU_LDO, addr);
            }
            check_frq = CHECK_FREQUENCY;
        }
        reg16_read((void *)((uintptr_t)addr), reg_val);
    }
}

static void hal_pmu_l2ram_dsp_power_all_enable(switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    if (power_switch != 0) {
        reg16_write(L2M_DSP_PWR_EN_CFG0, L2M_DSP_ALL_ENABLE);
        hal_reg_status_mask_check_timeout(L2M_DSP_PWR_ACK_CFG0, L2M_DSP_ALL_ENABLE,
            L2M_DSP_ALL_ENABLE, HAL_MEM_APP_TIMEOUT);
        reg16_write(L2M_DSP_ISO_EN_CFG0, 0x0);
    } else {
        reg16_write(L2M_DSP_ISO_EN_CFG0, L2M_DSP_ALL_ENABLE);
        reg16_write(L2M_DSP_PWR_EN_CFG0, 0x0);
    }
    osal_irq_restore(irq_sts);
}
static void hal_pmu_l2ram_bt_power_all_enable(switch_type_t power_switch)
{
    unsigned int irq_sts = osal_irq_lock();
    if (power_switch != 0) {
        reg16_setbits(L2M_BT_PWR_EN_CFG0, L2M_BT_PWR_MEM_BIT, L2M_BT_PWR_MEM_LEN, L2M_BT_PWR_MEM_ALL_ENABLE);
        hal_reg_status_mask_check_timeout(L2M_BT_PWR_ACK_CFG0, L2M_BT_PWR_MEM_ALL_ENABLE,
            L2M_BT_PWR_MEM_ALL_ENABLE, HAL_MEM_APP_TIMEOUT);
        reg16_setbits(L2M_BT_ISO_EN_CFG0, L2M_BT_PWR_MEM_BIT, L2M_BT_PWR_MEM_LEN, 0x0);
    } else {
        reg16_setbits(L2M_BT_ISO_EN_CFG0, L2M_BT_PWR_MEM_BIT, L2M_BT_PWR_MEM_LEN, L2M_BT_PWR_MEM_ALL_ENABLE);
        reg16_setbits(L2M_BT_PWR_EN_CFG0, L2M_BT_PWR_MEM_BIT, L2M_BT_PWR_MEM_LEN, 0x0);
    }
    osal_irq_restore(irq_sts);
}

/**
 * @brief  l2ram dsp and bt all power control interface.
 * @param  power_switch  power on/off.
 */
void hal_pmu_l2ram_mem_power_enable(switch_type_t power_switch)
{
    hal_pmu_l2ram_dsp_power_all_enable(power_switch);
    hal_pmu_l2ram_bt_power_all_enable(power_switch);
}