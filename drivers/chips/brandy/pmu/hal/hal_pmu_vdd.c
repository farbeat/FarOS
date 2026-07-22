/*
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description:  HAL PMU VDD IO DRIVER IMPLEMENTATION
 */

#include "hal_pmu_vdd.h"
#include "soc_osal.h"
#include "hal_reg_config.h"
#include "platform_core.h"
#ifdef USE_CMSIS_OS
#include "cmsis_os2.h"
#ifdef __LITEOS__
#include "los_hwi.h"
#endif
#endif
// SIMOBUCK config reg
#define PMU_SIMO1_CFG2_REG                              (PMU1_CTL_RB_BASE + 0x108)
#define PMU_SIMO1_CFG4_REG                              (PMU1_CTL_RB_BASE + 0x110) // 0p8/1p9 en
#define PMU_SIMO1_CFG12_REG                             (PMU1_CTL_RB_BASE + 0x130) // 1p9 vset
#define PMU_SIMO1_CFG14_REG                             (PMU1_CTL_RB_BASE + 0x138)
#define PMU_SIMO2_CFG1_REG                              (PMU1_CTL_RB_BASE + 0x140)
#define PMU_SIMO2_CFG3_REG                              (PMU1_CTL_RB_BASE + 0x148) // 1p0/1p3 en
#define PMU_SIMO2_CFG10_REG                             (PMU1_CTL_RB_BASE + 0x164) // 1p3 vset
#define PMU_SIMO2_CFG12_REG                             (PMU1_CTL_RB_BASE + 0x16c)
#define PMU_SIMO2_CFG13_REG                             (PMU1_CTL_RB_BASE + 0x170) // 1p0 vset
#define PMU_SIMO1_CFG_LPM_REG                           (PMU1_CTL_RB_BASE + 0x180) // 0p8 vset
#define PMU_SIMO1_VO2_VSET_B                            (GLB_CTL_B_RB_BASE + 0x310) // bcore 0p8
#define PMU_SIMO_VO2_EN_BIT                              1
#define PMU_SIMO_VO1_EN_BIT                              2

/* VDD0p8 vset request */
#define PMU_SIMO1_VO2_VSET_CFG_M                (GLB_CTL_M_RB_BASE + 0x3d8)
#define PMU_SIMO1_VO2_VSET_REQ_BIT              0
#define PMU_SIMO1_VO2_VSET_ACK_BIT              4
#define PMU_SIMO1_VO2_VSET_ACK_CLR_BIT          8

/* VDD0p8 vset request in bcore */
#define PMU_SIMO1_VO2_VSET_CFG_B                (GLB_CTL_B_RB_BASE + 0x314)
#define PMU_SIMO1_VO2_VSET_REQ_B_BIT            0
#define PMU_SIMO1_VO2_VSET_ACK_B_BIT            4
#define PMU_SIMO1_VO2_VSET_ACK_CLR_B_BIT        8

// VDD0p8 vset config
#define PMU_SIMO1_VO1_VSET_BIT                  7
#define PMU_SIMO1_VO1_VSET_LEN                  6
#define PMU_SIMO2_VO1_VSET_BIT                  0
#define PMU_SIMO2_VO1_VSET_LEN                  6
#define PMU_SIMO2_VO2_VSET_BIT                  0
#define PMU_SIMO2_VO2_VSET_LEN                  6

#define PMU_SIMO1_VO2_VSET_NOR_BIT              0
#define PMU_SIMO1_VO2_VSET_NOR_LEN              6
#define PMU_SIMO1_VO2_VSET_SLP_BIT              6
#define PMU_SIMO1_VO2_VSET_SLP_LEN              6
#define PMU_BCORE_SIMO1_VO2_VSET_NOR_BIT        0
#define PMU_BCORE_SIMO1_VO2_VSET_NOR_LEN        6

#define PMU_DBG_SEL_REG                         (PMU1_CTL_RB_BASE + 0x810)

// Power on time config reg
#define ULP_AON_VDD1P9_SW_OFF_BOOT_TIME_REG             (ULP_AON_CTL_RB_ADDR + 0x180)
#define ULP_AON_SLEEP_N_BOOT_TIME_REG                   (ULP_AON_CTL_RB_ADDR + 0x184)
#define ULP_AON_EN_REF_BG1_BOOT_TIME_REG                (ULP_AON_CTL_RB_ADDR + 0x188)
#define ULP_AON_VDD0P7_TO_SYS_ISO_EN_BOOT_TIME_REG      (ULP_AON_CTL_RB_ADDR + 0x18C)
#define ULP_AON_RST_BOOT_32K_N_BOOT_TIME_REG            (ULP_AON_CTL_RB_ADDR + 0x190)
#define ULP_AON_ULP_WKUP_AON_BOOT_TIME_REG              (ULP_AON_CTL_RB_ADDR + 0x194)
// Wakeup time config reg
#define ULP_AON_RST_BOOT_32K_N_WKUP_TIME_REG            (ULP_AON_CTL_RB_ADDR + 0x198)
#define ULP_AON_ULP_WKUP_AON_WKUP_TIME_REG              (ULP_AON_CTL_RB_ADDR + 0x19C)
#define ULP_AON_ULP_SLP_HLD_TIME_REG                    (ULP_AON_CTL_RB_ADDR + 0x1A0)
#define ULP_AON_VDD1P9_SW_OFF_SLP_TIME_SOC_REG          (ULP_AON_CTL_RB_ADDR + 0x1A4)
#define ULP_AON_SLEEP_N_SLP_TIME_SOC_REG                (ULP_AON_CTL_RB_ADDR + 0x1A8)
#define ULP_AON_EN_REF_BG1_SLP_TIME_SOC_REG             (ULP_AON_CTL_RB_ADDR + 0x1AC)
#define ULP_AON_VDD0P7_TO_SYS_ISO_EN_SLP_TIME_SOC_REG   (ULP_AON_CTL_RB_ADDR + 0x1B0)
#define ULP_AON_RST_BOOT_32K_N_SLP_TIME_SOC_REG         (ULP_AON_CTL_RB_ADDR + 0x1B4)
#define ULP_AON_ULP_WKUP_AON_SLP_TIME_SOC_REG           (ULP_AON_CTL_RB_ADDR + 0x1B8)
#define ULP_AON_TIME_REG_ADDR_OFFSET                    4

#define HAL_PMU_CPU_TIMEOUT                             1000
#define PMU_VSET_ACK_TIMEOUT                            100ULL

static void hal_pmu_simo1_vdd_en(hal_pmu_vdd_t vdd, switch_type_t type)
{
    uint8_t en_bit = (vdd == HAL_PMU_VDD0P8 ? PMU_SIMO_VO2_EN_BIT : PMU_SIMO_VO1_EN_BIT);
    if (type == TURN_ON) {
        reg16_setbit(PMU_SIMO1_CFG4_REG, en_bit);
    } else {
        reg16_clrbit(PMU_SIMO1_CFG4_REG, en_bit);
    }
}

static void hal_pmu_simo2_vdd_en(hal_pmu_vdd_t vdd, switch_type_t type)
{
    uint8_t en_bit = (vdd == HAL_PMU_VDD1P0 ? PMU_SIMO_VO2_EN_BIT : PMU_SIMO_VO1_EN_BIT);
    if (type == TURN_ON) {
        reg16_setbit(PMU_SIMO2_CFG3_REG, en_bit);
    } else {
        reg16_clrbit(PMU_SIMO2_CFG3_REG, en_bit);
    }
}

void hal_pmu_simo_buck_init(void)
{
    // 配置pmu优化寄存器
    reg16_write(PMU_SIMO1_CFG2_REG, 0x14c0);
    reg16_write(PMU_SIMO2_CFG1_REG, 0xee5);
    reg16_write(PMU_SIMO2_CFG13_REG, 0x52);
    reg16_write(PMU_SIMO1_CFG14_REG, 0x99);
    reg16_write(PMU_SIMO2_CFG12_REG, 0x1053);
    reg16_write(PMU_SIMO2_CFG10_REG, 0x644); /* vdd1p3: 0.9V */
}

void hal_pmu_vdd_enable(hal_pmu_vdd_t vdd, switch_type_t type)
{
    if (vdd == HAL_PMU_VDD0P8 || vdd == HAL_PMU_VDD1P9) {
        hal_pmu_simo1_vdd_en(vdd, type);
    } else {
        hal_pmu_simo2_vdd_en(vdd, type);
    }
}

void hal_pmu_vdd0p8_set_nor_voltage(hal_pmu_vdd0p8_1p0_level_t level)
{
    reg16_setbits(PMU_SIMO1_CFG_LPM_REG, PMU_SIMO1_VO2_VSET_NOR_BIT, PMU_SIMO1_VO2_VSET_NOR_LEN, level);
}

void hal_pmu_vdd0p8_set_slp_voltage(hal_pmu_vdd0p8_1p0_level_t level)
{
    reg16_setbits(PMU_SIMO1_CFG_LPM_REG, PMU_SIMO1_VO2_VSET_SLP_BIT, PMU_SIMO1_VO2_VSET_SLP_LEN, level);
}

void hal_pmu_bcore_vdd0p8_set_nor_voltage(hal_pmu_vdd0p8_1p0_level_t level)
{
    reg16_setbits(PMU_SIMO1_VO2_VSET_B, PMU_BCORE_SIMO1_VO2_VSET_NOR_BIT, PMU_BCORE_SIMO1_VO2_VSET_NOR_LEN, level);
}

void hal_pmu_vdd0p8_vset_requset(pmu_vset_core_t type)
{
    int32_t reg = PMU_SIMO1_VO2_VSET_CFG_M;
    int32_t req_bit = PMU_SIMO1_VO2_VSET_REQ_BIT;
    if (type == PMU_VSET_MCORE) {
        reg = PMU_SIMO1_VO2_VSET_CFG_B;
        req_bit = PMU_SIMO1_VO2_VSET_REQ_B_BIT;
    }
    reg16_setbit(reg, req_bit);
}

uint8_t hal_pmu_vdd0p8_vset_ack_get(pmu_vset_core_t type)
{
    uint32_t reg = PMU_SIMO1_VO2_VSET_CFG_M;
    uint16_t ack_bit = PMU_SIMO1_VO2_VSET_ACK_BIT;
    if (type == PMU_VSET_MCORE) {
        reg = PMU_SIMO1_VO2_VSET_CFG_B;
        ack_bit = PMU_SIMO1_VO2_VSET_ACK_B_BIT;
    }
    hal_reg_status_check_timeout(reg, ack_bit, TURN_ON, PMU_VSET_ACK_TIMEOUT);
    return reg16_getbit(reg, ack_bit);
}

void hal_pmu_vdd0p8_vset_ack_clr(pmu_vset_core_t type)
{
    int32_t reg = PMU_SIMO1_VO2_VSET_CFG_M;
    int32_t clr_bit = PMU_SIMO1_VO2_VSET_ACK_CLR_BIT;
    if (type == PMU_VSET_MCORE) {
        reg = PMU_SIMO1_VO2_VSET_CFG_B;
        clr_bit = PMU_SIMO1_VO2_VSET_ACK_CLR_B_BIT;
    }
    reg16_setbit(reg, clr_bit);
}

void hal_pmu_vdd1p9_set_voltage(hal_pmu_vdd1p9_1p3_level_t level)
{
    reg16_setbits(PMU_SIMO1_CFG12_REG, PMU_SIMO1_VO1_VSET_BIT, PMU_SIMO1_VO1_VSET_LEN, level);
}

void hal_pmu_vdd1p0_set_voltage(hal_pmu_vdd0p8_1p0_level_t level)
{
    reg16_setbits(PMU_SIMO2_CFG13_REG, PMU_SIMO2_VO2_VSET_BIT, PMU_SIMO2_VO2_VSET_LEN, level);
}

void hal_pmu_vdd1p3_set_voltage(hal_pmu_vdd1p9_1p3_level_t level)
{
    reg16_setbits(PMU_SIMO2_CFG10_REG, PMU_SIMO2_VO1_VSET_BIT, PMU_SIMO2_VO1_VSET_LEN, level);
}

void hal_pmu_dbg_select(switch_type_t dbg_switch)
{
    if (dbg_switch == TURN_ON) {
        writew(PMU_DBG_SEL_REG, 0xffff);
    } else {
        writew(PMU_DBG_SEL_REG, 0);
    }
}

void hal_pmu_dbg_select_control(hal_pmu_dbg_sel_t pmu_sel_control, switch_type_t select_en)
{
    // Enable or disable pmu dbg select.
    hal_reg_config_bit(PMU_DBG_SEL_REG, select_en, (REG16_POS)pmu_sel_control);
}

void hal_ulp_power_on_time_config(ulp_aon_power_on_t ulp_power, uint16_t time_value)
{
    uint32_t pwron_id ;
    uint32_t reg_addr;
    uint16_t val;
    uint16_t power_on_time;
    int16_t offset_value;

    switch (ulp_power) {
        case ULP_AON_VDD1P9_SW_OFF_BOOT_TIME:
            reg_addr = ULP_AON_VDD1P9_SW_OFF_BOOT_TIME_REG;
            break;
        case ULP_AON_SLEEP_N_BOOT_TIME:
            reg_addr = ULP_AON_SLEEP_N_BOOT_TIME_REG;
            break;
        case ULP_AON_EN_REF_BG1_BOOT_TIME:
            reg_addr = ULP_AON_EN_REF_BG1_BOOT_TIME_REG;
            break;
        case ULP_AON_VDD0P7_TO_SYS_ISO_EN_BOOT_TIME:
            reg_addr = ULP_AON_VDD0P7_TO_SYS_ISO_EN_BOOT_TIME_REG;
            break;
        case ULP_AON_RST_BOOT_32K_N_BOOT_TIME:
            reg_addr = ULP_AON_RST_BOOT_32K_N_BOOT_TIME_REG;
            break;
        case ULP_AON_ULP_WKUP_AON_BOOT_TIME:
            reg_addr = ULP_AON_ULP_WKUP_AON_BOOT_TIME_REG;
            break;
        default:
            return;
    }

    uint32_t irq_sts = osal_irq_lock();
    reg16_read(reg_addr, power_on_time);
    reg16_write(reg_addr, time_value);
    offset_value = (int16_t)(power_on_time - time_value);
    val = time_value;
    for (pwron_id = ulp_power + 1; pwron_id < ULP_AON_POWER_ON_MAX; pwron_id++) {
        reg_addr += ULP_AON_TIME_REG_ADDR_OFFSET;
        reg16_read(reg_addr, val);
        val = (uint16_t)(val + offset_value);
        reg16_write(reg_addr, val);
    }
    osal_irq_restore(irq_sts);
}

void hal_ulp_wakeup_time_config(ulp_aon_wakeup_t ulp_wakeup, uint16_t time_value)
{
    uint32_t wkup_id;
    uint32_t reg_addr;
    uint16_t wakeup_time;
    int16_t offset_value;

    switch (ulp_wakeup) {
        case ULP_AON_RST_BOOT_32K_N_WKUP_TIME:
            reg_addr = ULP_AON_RST_BOOT_32K_N_WKUP_TIME_REG;
            break;
        case ULP_AON_ULP_WKUP_AON_WKUP_TIME:
            reg_addr = ULP_AON_ULP_WKUP_AON_WKUP_TIME_REG;
            break;
        case ULP_AON_ULP_SLP_HLD_TIME:
            reg_addr = ULP_AON_ULP_SLP_HLD_TIME_REG;
            break;
        case ULP_AON_VDD1P9_SW_OFF_SLP_TIME_SOC:
            reg_addr = ULP_AON_VDD1P9_SW_OFF_SLP_TIME_SOC_REG;
            break;
        case ULP_AON_SLEEP_N_SLP_TIME_SOC:
            reg_addr = ULP_AON_SLEEP_N_SLP_TIME_SOC_REG;
            break;
        case ULP_AON_EN_REF_BG1_SLP_TIME_SOC:
            reg_addr = ULP_AON_EN_REF_BG1_SLP_TIME_SOC_REG;
            break;
        case ULP_AON_VDD0P7_TO_SYS_ISO_EN_SLP_TIME_SOC:
            reg_addr = ULP_AON_VDD0P7_TO_SYS_ISO_EN_SLP_TIME_SOC_REG;
            break;
        case ULP_AON_RST_BOOT_32K_N_SLP_TIME_SOC:
            reg_addr = ULP_AON_RST_BOOT_32K_N_SLP_TIME_SOC_REG;
            break;
        case ULP_AON_ULP_WKUP_AON_SLP_TIME_SOC:
            reg_addr = ULP_AON_ULP_WKUP_AON_SLP_TIME_SOC_REG;
            break;
        default:
            return;
    }

    uint32_t irq_sts = osal_irq_lock();
    reg16_read(reg_addr, wakeup_time);
    reg16_write(reg_addr, time_value);
    offset_value = (int16_t)(wakeup_time - time_value);

    uint16_t val = time_value;
    for (wkup_id = ulp_wakeup + 1; wkup_id < ULP_AON_WAKEUP_MAX; wkup_id++) {
        reg_addr += ULP_AON_TIME_REG_ADDR_OFFSET;
        reg16_read(reg_addr, val);
        val = (uint16_t)(val + offset_value);
        reg16_write(reg_addr, val);
    }
    osal_irq_restore(irq_sts);
}

