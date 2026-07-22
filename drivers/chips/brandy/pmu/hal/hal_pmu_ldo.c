/*
 * Copyright (c) @CompanyNameMagicTag 2019-2022. All rights reserved.
 * Description:  HAL PMU LDO DRIVER
 */

#include "hal_pmu_ldo.h"
#include "soc_osal.h"
#include "hal_reg_config.h"
#include "tcxo.h"
#include "platform_core.h"

#define PMU_SYS_BOOT_CNT_MASK 0x3FFF

/* PMU SYS_HISTORY register bit describe */
#define PMU_SYS_HISTORY_CLR 1
#define PMU_SYS_HISTORY     0

/* SYS_STATUS registers */
#define PMU_SYS_STATUS_REG                      (PMU1_CTL_RB_BASE + 0x80)
#define PMU_SYS_BOOT_CNT_REG                    (PMU1_CTL_RB_BASE + 0x84)
#define PMU_SYS_CUR_STATUS_BIT                  0
#define PMU_SYS_CUR_STATUS_LEN                  3
#define PMU_SYS_CUR_POWER_SYNC_STS              4

/* PMU1 RESET MANUAL & SEL Module */
#define PMU_RST_MAN_REG                         (PMU1_CTL_RB_BASE + 0xB0)
#define PMU_RST_SEL_REG                         (PMU1_CTL_RB_BASE + 0xB4)
#define PMU_RST_MAN_RST_PWR_C1_CPU_N_BIT        5
#define PMU_RST_MAN_RST_PWR_C1_LOGIC_N_BIT      6
#define PMU_RST_MAN_RST_PWR_C1_CRG_N_BIT        7
#define PMU_RST_MAN_GLB_CLK_FORCE_ON_BIT        9
#define PMU_RST_MAN_C1_CONFIG                   (BIT(PMU_RST_MAN_RST_PWR_C1_CPU_N_BIT) | \
                                                BIT(PMU_RST_MAN_RST_PWR_C1_LOGIC_N_BIT) | \
                                                BIT(PMU_RST_MAN_RST_PWR_C1_CRG_N_BIT))

#define PMU_RST_STS_REG                         (PMU1_CTL_RB_BASE + 0xB8)
#define PMU_RST_PWR_C1_CPU_N_BIT                5
#define PMU_RST_PWR_C1_LOGIC_N_BIT              6
#define PMU_RST_PWR_C1_CRG_N_BIT                7
#define PMU_RST_AON_LOGIC_N_STS_BIT             8
#define PMU_RST_GLB_CLK_FORCE_ON_STS_BIT        9
#define PMU_RST_RST_AON_CRG_N_STS_BIT           10

/* LDO MANUAL & SEL Module */
#define PMU_LDO_MAN_REG                         (PMU1_CTL_RB_BASE + 0xBC)
#define PMU_LDO_SEL_REG                         (PMU1_CTL_RB_BASE + 0xC0)
#define PMU_LDO_STS_REG                         (PMU1_CTL_RB_BASE + 0xC4)
#define PMU_LDO_C1_TO_SYS_ISO_EN_BIT            2
#define PMU_LDO_EN_CLDO1_SW_BIT                 0

/* PMU0 MANUAL & SEL Module */
#define PMU_MAN_0_REG                           (PMU1_CTL_RB_BASE + 0xD0)
#define PMU_SEL_0_REG                           (PMU1_CTL_RB_BASE + 0xE0)
#define PMU_STS_0_REG                           (PMU1_CTL_RB_BASE + 0xDC)
#define PMU_AON_SLP_RDY_BIT                     0

/* PMU1 MANUAL & SEL Module */
#define PMU_MAN_1_REG                           (PMU1_CTL_RB_BASE + 0xD4)
#define PMU_SEL_1_REG                           (PMU1_CTL_RB_BASE + 0xD8)
#define PMU_STS_1_REG                           (PMU1_CTL_RB_BASE + 0x844)
#define PMU_POWER_CTRL_CONFIG                   0xFFFF   // Need check

#define PMU_CLDO1_CFG_REG                       (PMU1_CTL_RB_BASE + 0x208)
#define PMU_CLDO2_CFG_REG                       (PMU1_CTL_RB_BASE + 0x20c)
#define PMU_CLDO3_CFG_REG                       (PMU1_CTL_RB_BASE + 0x220)
#define PMU_EN_CLDO_SW_NOR_BIT                  0
#define PMU_EN_CLDO_SW_SLP_BIT                  1
#define PMU_EN_CLDO_HIZ_BIT                     2
#define PMU_CLDO_ICTR_BIT                       3
#define PMU_CLDO_ICTR_LEN                       3
#define PMU_CLDO_VSET_NOR_BIT                   6
#define PMU_CLDO_VSET_NOR_LEN                   4
#define PMU_CLDO_VSET_SLP_BIT                   10
#define PMU_CLDO_VSET_SLP_LEN                   4

#define PMU_IOLDO_CFG_REG                       (PMU1_CTL_RB_BASE + 0x210)
#define PMU_EN_IOLDO_DELAY_BIT                  13
#define PMU_IOLDO_EN_BIT                        12
#define PMU_IOLDO_VSET_BIT                      6
#define PMU_IOLDO_VSET_LEN                      4
#define PMU_IOLDO_ICTR_BIT                      2
#define PMU_IOLDO_ICTR_LEN                      4
#define PMU_EN_IOLDO_HIZ_BIT                    1
#define PMU_EN_IOLDO_SW_BIT                     0

#define PMU_AUXLDO0_CFG_REG                     (PMU1_CTL_RB_BASE + 0x214)
#define PMU_AUXLDO1_CFG_REG                     (PMU1_CTL_RB_BASE + 0x218)
#define PMU_AUXLDO2_CFG_REG                     (PMU1_CTL_RB_BASE + 0x21c)
#define PMU_AUXLDO3_CFG_REG                     (PMU1_CTL_RB_BASE + 0x224)
#define PMU_EN_AUXLDO_SW_BIT                    1
#define PMU_EN_AUXLDO_HIZ_BIT                   0
#define PMU_AUXLDO_ICTR_BIT                     2
#define PMU_AUXLDO_ICTR_LEN                     3
#define PMU_AUXLDO_VSET_BIT                     5
#define PMU_AUXLDO_VSET_LEN                     4
#define PMU_AUXLDO_EN_BIT                       11
#define PMU_EN_AUXLDO_DELAY_BIT                 12

#define PMU_INTLDO1_CFG_REG                     (PMU1_CTL_RB_BASE + 0x248)
#define PMU_INTLDO1_HIZ_BIT                     0
#define PMU_EN_INTLDO1_SW_BIT                   1
#define PMU_INTLDO1_ICTR_BIT                    2
#define PMU_INTLDO1_ICTR_LEN                    3
#define PMU_INTLDO1_VSET_BIT                    5
#define PMU_INTLDO1_VSET_LEN                    3

#define PMU_CLDO1_VSET_CFG_REG                  (GLB_CTL_M_RB_BASE + 0x3c4)
#define PMU_CLDO2_VSET_CFG_REG                  (GLB_CTL_B_RB_BASE + 0x304)
#define PMU_CLDO3_VSET_CFG_REG                  (GLB_CTL_M_RB_BASE + 0x3d0)
#define PMU_CLDO_VSET_REQ_BIT                   0
#define PMU_CLDO_VSET_ACK_BIT                   4
#define PMU_CLDO_VSET_ACK_CLR_BIT               8

/* System wakeup time config register */
#define PMU_EN_UVLO_WKUP_TIME_SOC_REG           (PMU1_CTL_RB_BASE + 0x408)
#define PMU_EN_OVP_WKUP_TIME_SOC_REG            (PMU1_CTL_RB_BASE + 0x40C)
#define PMU_RST_REG_ADDR_OFFSET                 4

/* System delay time config register */
#define PMU_EN_INTLDO2_DELAY_TIME_SOC_REG       (PMU1_CTL_RB_BASE + 0x460)
#define PMU_EN_XLDO_DELAY_TIME_SOC_REG          (PMU1_CTL_RB_BASE + 0x464)
#define PMU_EN_CLDO1_DELAY_TIME_SOC_REG         (PMU1_CTL_RB_BASE + 0x468)
#define PMU_CMU_LDO_EN_DELAY_TIME_SOC_REG       (PMU1_CTL_RB_BASE + 0x46C)
#define PMU_EN_CLDO3_DELAY_TIME_SOC_REG         (PMU1_CTL_RB_BASE + 0x470)

#define PMU2_INTLDO2_CFG_REG                    (PMU2_CMU_CTL_RB_BASE + 0x28C)
#define PMU2_INTLDO2_HIZ_BIT                    1
#define PMU2_INTLDO2_VSET_BIT                   2
#define PMU2_INTLDO2_VSET_LEN                   3
#define PMU2_INTLDO2_ICTR_BIT                   5
#define PMU2_INTLDO2_ICTR_LEN                   3
#define PMU2_EN_INTLDO2_SW_BIT                  8

#define PMU2_XLDO_CFG_REG                       (PMU2_CMU_CTL_RB_BASE + 0x290)
#define PMU2_XLDO_SEL_BIT                       0
#define PMU2_XLDO_VSET_BIT                      2
#define PMU2_XLDO_VSET_LEN                      4
#define PMU2_XLDO_ICTR_BIT                      6
#define PMU2_XLDO_ICTR_LEN                      3
#define PMU2_EN_XLDO_SW_BIT                     9
#define PMU2_EN_XLDO_HIZ_BIT                    10

#define PMU2_CMU_TRIM_REG                       (PMU2_CMU_CTL_RB_BASE + 0x2B0)
#define PMU2_CMU_XO_CORE_TRIM_BIT               0
#define PMU2_CMU_XO_CORE_TRIM_LEN               2

#define PMU2_CMU_LDO_VSET_REG                   (PMU2_CMU_CTL_RB_BASE + 0x2E0)
#define PMU2_CMU_LDO_VSET_BIT                   0
#define PMU2_CMU_LDO_VSET_LEN                   4

#define PMU_LDO_POWER_ON_DELAY_US               120ULL
#define PMU_PMU2_MANUAL_POWER_ON_DELAY_US       30ULL
#define PMU_VSET_ACK_TIMEOUT                    100ULL

// PMU ldo vset auto cfg
#define PMU_CLDO1_VSET_AUTO_CFG_REG                 (PMU1_CTL_RB_BASE + 0x894)
#define PMU_CLDO2_VSET_AUTO_CFG_REG                 (PMU1_CTL_RB_BASE + 0x898)
#define PMU_CLDO3_VSET_AUTO_CFG_REG                 (PMU1_CTL_RB_BASE + 0x89c)

uint16_t hal_pmu_system_boot_count(void)
{
    uint16_t sys_reboot_cnt = 0;

    reg16_read(PMU_SYS_BOOT_CNT_REG, sys_reboot_cnt);
    return (PMU_SYS_BOOT_CNT_MASK & sys_reboot_cnt);
}

hal_pmu_system_current_status_type_t hal_pmu_system_current_status(void)
{
    return (hal_pmu_system_current_status_type_t)reg16_getbits(PMU_SYS_STATUS_REG,
                                                               PMU_SYS_CUR_STATUS_BIT, PMU_SYS_CUR_STATUS_LEN);
}

/*********************** PMU0 config ****************************/
void hal_aon_sleep_ready_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_MAN_0_REG, PMU_AON_SLP_RDY_BIT);
    } else {
        writew(PMU_MAN_0_REG, 0);
    }
}

void hal_aon_sleep_ready_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_SEL_0_REG, PMU_AON_SLP_RDY_BIT);
    } else {
        writew(PMU_SEL_0_REG, 0);
    }
}

/*********************** RESET config ****************************/
void hal_pmu_reset_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_RST_MAN_REG, PMU_RST_MAN_C1_CONFIG);
    } else {
        writew(PMU_RST_MAN_REG, 0);
    }
}

void hal_pmu_reset_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_RST_SEL_REG, PMU_RST_MAN_C1_CONFIG);
    } else {
        writew(PMU_RST_SEL_REG, 0);
    }
}

/*********************** PMU1 config ****************************/
void hal_pmu_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_MAN_1_REG, PMU_POWER_CTRL_CONFIG);
    } else {
        writew(PMU_MAN_1_REG, 0);
    }
}

void hal_pmu_manual_config_set(uint16_t pmu_manual_config)
{
    writew(PMU_MAN_1_REG, pmu_manual_config);
}

void hal_pmu_manual_config_control(hal_pmu_man_sel_t pmu_man_control, switch_type_t manual_en)
{
    // Enable or disable pmu manual.
    hal_reg_config_bit(PMU_MAN_1_REG, manual_en, (REG16_POS)pmu_man_control);
}

void hal_pmu_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_SEL_1_REG, 0xffff);
    } else {
        writew(PMU_SEL_1_REG, 0);
    }
}

void hal_pmu_manual_select_set(uint16_t pmu_manual_select)
{
    writew(PMU_SEL_1_REG, pmu_manual_select);
}

void hal_pmu_manual_select_control(hal_pmu_man_sel_t pmu_sel_control, switch_type_t select_en)
{
    // Enable or disable pmu manual select.
    hal_reg_config_bit(PMU_SEL_1_REG, select_en, (REG16_POS)pmu_sel_control);
}

void hal_pmu_sts_0_get(uint16_t *sts_0)
{
    *sts_0 = readw(PMU_STS_0_REG);
}

void hal_pmu_sts_1_get(uint16_t *sts_1)
{
    *sts_1 = readw(PMU_STS_1_REG);
}

/*********************** LDO config ****************************/
void hal_ldo_manual_config(switch_type_t power_switch)
{
    if (power_switch != 0) {
        writew(PMU_LDO_MAN_REG, 0xffff);
    } else {
        writew(PMU_LDO_MAN_REG, 0);
    }
}

void hal_ldo_manual_config_set(uint8_t ldo_man_set)
{
    writew(PMU_LDO_MAN_REG, ldo_man_set);
}

void hal_ldo_manual_config_control(hal_ldo_man_sel_t ldo_man_control, switch_type_t manual_en)
{
    // Enable or disable ldo manual.
    hal_reg_config_bit(PMU_LDO_MAN_REG, manual_en, (REG16_POS)ldo_man_control);
}

void hal_ldo_manual_select(switch_type_t manual_switch)
{
    if (manual_switch != 0) {
        writew(PMU_LDO_SEL_REG, 0xffff);
    } else {
        writew(PMU_LDO_SEL_REG, 0);
    }
}

void hal_ldo_manual_select_set(uint8_t ldo_manual_set)
{
    writew(PMU_LDO_SEL_REG, ldo_manual_set);
}

void hal_ldo_manual_select_control(hal_ldo_man_sel_t ldo_sel_control, switch_type_t select_en)
{
    // Enable or disable ldo select.
    hal_reg_config_bit(PMU_LDO_SEL_REG, select_en, (REG16_POS)ldo_sel_control);
}

void hal_pmu_cldo1_enable(switch_type_t en)
{
    // select to manual
    hal_ldo_manual_select_control(EN_CLDO1, TURN_ON);
    hal_ldo_manual_select_control(EN_CLDO1_DELAY, TURN_ON);
    if (en == TURN_ON) {
        hal_ldo_manual_config_control(EN_CLDO1, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_ldo_manual_config_control(EN_CLDO1_DELAY, TURN_ON);
    } else {
        // close ldo
        hal_ldo_manual_config_control(EN_CLDO1, TURN_OFF);
        hal_ldo_manual_config_control(EN_CLDO1_DELAY, TURN_OFF);
        hal_pmu_cldo_enable_high_resistance(HAL_PMU_CLDO1, TURN_ON);
    }
}

void hal_pmu_cldo2_enable(switch_type_t en)
{
    // select to manual
    hal_ldo_manual_select_control(EN_CLDO2, TURN_ON);
    hal_ldo_manual_select_control(EN_CLDO2_DELAY, TURN_ON);
    if (en == TURN_ON) {
        hal_ldo_manual_config_control(EN_CLDO2, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_ldo_manual_config_control(EN_CLDO2_DELAY, TURN_ON);
        uapi_tcxo_delay_us(PMU_PMU2_MANUAL_POWER_ON_DELAY_US);
        hal_ldo_manual_config_control(LDO_C2_TO_SYS_ISO_EN_RB, TURN_OFF);
    } else {
        // close ldo
        hal_ldo_manual_config_control(EN_CLDO2, TURN_OFF);
        hal_ldo_manual_config_control(EN_CLDO2_DELAY, TURN_OFF);
    }
}

void hal_pmu_cldo3_enable(switch_type_t en)
{
    // select to manual
    hal_ldo_manual_select_control(EN_CLDO3, TURN_ON);
    hal_ldo_manual_select_control(EN_CLDO3_DELAY, TURN_ON);
    if (en == TURN_ON) {
        hal_ldo_manual_config_control(EN_CLDO3, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_ldo_manual_config_control(EN_CLDO3_DELAY, TURN_ON);
    } else {
        // close ldo
        hal_ldo_manual_config_control(EN_CLDO3, TURN_OFF);
        hal_ldo_manual_config_control(EN_CLDO3_DELAY, TURN_OFF);
        hal_pmu_cldo_enable_high_resistance(HAL_PMU_CLDO3, TURN_ON);
    }
}

void hal_pmu_cldo_vset_auto_cfg(hal_pmu_cldo_t ldo, switch_type_t select_en)
{
    uint32_t ldo_auto_regs[] = {PMU_CLDO1_VSET_AUTO_CFG_REG, PMU_CLDO2_VSET_AUTO_CFG_REG, PMU_CLDO3_VSET_AUTO_CFG_REG};

    writew(ldo_auto_regs[ldo], select_en);
}

void hal_pmu_cldo_enable_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};
    hal_reg_config_bit(ldo_regs[ldo], enable_bypass, (REG16_POS)PMU_EN_CLDO_SW_NOR_BIT);
    hal_reg_config_bit(ldo_regs[ldo], enable_bypass, (REG16_POS)PMU_EN_CLDO_SW_SLP_BIT);
}

void hal_pmu_cldo_enable_nor_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};
    hal_reg_config_bit(ldo_regs[ldo], enable_bypass, (REG16_POS)PMU_EN_CLDO_SW_NOR_BIT);
}

void hal_pmu_cldo_enable_slp_bypass(hal_pmu_cldo_t ldo, switch_type_t enable_bypass)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};
    hal_reg_config_bit(ldo_regs[ldo], enable_bypass, (REG16_POS)PMU_EN_CLDO_SW_SLP_BIT);
}

void hal_pmu_cldo_enable_high_resistance(hal_pmu_cldo_t ldo, switch_type_t enable_high_z)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};
    if (enable_high_z != 0) {
        reg16_setbit(ldo_regs[ldo], PMU_EN_CLDO_HIZ_BIT);
    } else {
        reg16_clrbit(ldo_regs[ldo], PMU_EN_CLDO_HIZ_BIT);
    }
}

void hal_pmu_cldo_set_ictr(hal_pmu_cldo_t ldo, uint8_t ictr)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};
    reg16_setbits(ldo_regs[ldo], PMU_CLDO_ICTR_BIT, PMU_CLDO_ICTR_LEN, ictr);
}

void hal_pmu_cldo_set_voltage(hal_pmu_cldo_t ldo, hal_pmu_cldo_level_t level)
{
    uint32_t ldo_regs[] = {PMU_CLDO1_CFG_REG, PMU_CLDO2_CFG_REG, PMU_CLDO3_CFG_REG};

    reg16_setbits(ldo_regs[ldo], PMU_CLDO_VSET_NOR_BIT, PMU_CLDO_VSET_NOR_LEN, level);
}

void hal_pmu_cldo_vset_requset(hal_pmu_cldo_t ldo)
{
    uint32_t ldo_vset_regs[] = {PMU_CLDO1_VSET_CFG_REG, PMU_CLDO2_VSET_CFG_REG, PMU_CLDO3_VSET_CFG_REG};

    reg16_setbit(ldo_vset_regs[ldo], PMU_CLDO_VSET_REQ_BIT);
}

uint8_t hal_pmu_cldo_vset_ack_get(hal_pmu_cldo_t ldo)
{
    uint32_t ldo_vset_regs[] = {PMU_CLDO1_VSET_CFG_REG, PMU_CLDO2_VSET_CFG_REG, PMU_CLDO3_VSET_CFG_REG};
    hal_reg_status_check_timeout(ldo_vset_regs[ldo], PMU_CLDO_VSET_ACK_BIT, TURN_ON, PMU_VSET_ACK_TIMEOUT);
    return reg16_getbit(ldo_vset_regs[ldo], PMU_CLDO_VSET_ACK_BIT);
}

void hal_pmu_cldo_vset_ack_clr(hal_pmu_cldo_t ldo)
{
    uint32_t ldo_vset_regs[] = {PMU_CLDO1_VSET_CFG_REG, PMU_CLDO2_VSET_CFG_REG, PMU_CLDO3_VSET_CFG_REG};

    reg16_setbit(ldo_vset_regs[ldo], PMU_CLDO_VSET_ACK_CLR_BIT);
}

void hal_pmu_cmu_ldo_enable(switch_type_t en)
{
    if (en == TURN_ON) {
        hal_pmu_manual_select_control(EN_CMU_LDO, TURN_ON);
        hal_pmu_manual_select_control(EN_CMU_LDO_DELAY, TURN_ON);

        hal_pmu_manual_config_control(EN_CMU_LDO, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_pmu_manual_config_control(EN_CMU_LDO_DELAY, TURN_ON);
    } else {
        hal_pmu_manual_config_control(EN_CMU_LDO, TURN_OFF);
        hal_pmu_manual_config_control(EN_CMU_LDO_DELAY, TURN_OFF);
    }
}

void hal_pmu_cmu_ldo_set_voltage(hal_pmu_cmu_ldo_level_t level)
{
    reg16_setbits(PMU2_CMU_LDO_VSET_REG, PMU2_CMU_LDO_VSET_BIT, PMU2_CMU_LDO_VSET_LEN, level);
}

static uint32_t hal_pmu_auxldo_reg_get(hal_auxldo_id_t auxldo)
{
    uint32_t regs[] = {PMU_AUXLDO0_CFG_REG, PMU_AUXLDO1_CFG_REG, PMU_AUXLDO2_CFG_REG, PMU_AUXLDO3_CFG_REG};
    return regs[auxldo];
}

void hal_pmu_auxldo_enable(hal_auxldo_id_t auxldo, bool en)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    if (en) {
        reg16_setbit(reg, PMU_AUXLDO_EN_BIT);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        reg16_setbit(reg, PMU_EN_AUXLDO_DELAY_BIT);
    } else {
        reg16_clrbit(reg, PMU_AUXLDO_EN_BIT);
        reg16_clrbit(reg, PMU_EN_AUXLDO_DELAY_BIT);
    }
}

bool hal_pmu_auxldo_power_status(hal_auxldo_id_t auxldo)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    return reg16_getbit(reg, PMU_AUXLDO_EN_BIT);
}

void hal_pmu_auxldo_set_voltage(hal_auxldo_id_t auxldo, hal_pmu_auxldo_level_t level)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    reg16_setbits(reg, PMU_AUXLDO_VSET_BIT, PMU_AUXLDO_VSET_LEN, level);
}

void hal_pmu_auxldo_enable_bypass(hal_auxldo_id_t auxldo, switch_type_t enable_bypass)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    hal_reg_config_bit(reg, enable_bypass, (REG16_POS)PMU_EN_AUXLDO_SW_BIT);
}

void hal_pmu_auxldo_enable_high_resistance(hal_auxldo_id_t auxldo, switch_type_t enable_high_z)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    if (enable_high_z != 0) {
        reg16_setbit(reg, PMU_EN_AUXLDO_HIZ_BIT);
    } else {
        reg16_clrbit(reg, PMU_EN_AUXLDO_HIZ_BIT);
    }
}

void hal_pmu_auxldo_set_ictr(hal_auxldo_id_t auxldo, uint8_t ictr)
{
    uint32_t reg = hal_pmu_auxldo_reg_get(auxldo);
    reg16_setbits(reg, PMU_AUXLDO_ICTR_BIT, PMU_AUXLDO_ICTR_LEN, ictr);
}

/* IOLDO control */
void hal_pmu_ioldo_enable(switch_type_t en)
{
    if (en == TURN_ON) {
        reg16_setbit(PMU_IOLDO_CFG_REG, PMU_IOLDO_EN_BIT);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        reg16_setbit(PMU_IOLDO_CFG_REG, PMU_EN_IOLDO_DELAY_BIT);
    } else {
        reg16_clrbit(PMU_IOLDO_CFG_REG, PMU_IOLDO_EN_BIT);
        reg16_clrbit(PMU_IOLDO_CFG_REG, PMU_EN_IOLDO_DELAY_BIT);
    }
}

void hal_pmu_ioldo_set_voltage(hal_pmu_ioldo_level_t level)
{
    reg16_setbits(PMU_IOLDO_CFG_REG, PMU_IOLDO_VSET_BIT, PMU_IOLDO_VSET_LEN, level);
}

void hal_pmu_ioldo_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU_IOLDO_CFG_REG, enable_bypass, (REG16_POS)PMU_EN_IOLDO_SW_BIT);
}

void hal_pmu_ioldo_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z != 0) {
        reg16_setbit(PMU_IOLDO_CFG_REG, PMU_EN_IOLDO_HIZ_BIT);
    } else {
        reg16_clrbit(PMU_IOLDO_CFG_REG, PMU_EN_IOLDO_HIZ_BIT);
    }
}

void hal_pmu_ioldo_set_ictr(uint8_t ictr)
{
    reg16_setbits(PMU_IOLDO_CFG_REG, PMU_IOLDO_ICTR_BIT, PMU_IOLDO_ICTR_LEN, ictr);
}

/* XLDO control */
void hal_pmu_xldo_enable(switch_type_t en)
{
    if (en == TURN_ON) {
        hal_pmu_manual_select_control(EN_XLDO, TURN_ON);
        hal_pmu_manual_select_control(EN_XLDO_DELAY, TURN_ON);

        hal_pmu_manual_config_control(EN_XLDO, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_pmu_manual_config_control(EN_XLDO_DELAY, TURN_ON);
    } else {
        hal_pmu_manual_config_control(EN_XLDO, TURN_OFF);
        hal_pmu_manual_config_control(EN_XLDO_DELAY, TURN_OFF);
    }
}

void hal_pmu_xldo_set_voltage(hal_pmu_xldo_level_type_t level)
{
    reg16_setbits(PMU2_XLDO_CFG_REG, PMU2_XLDO_VSET_BIT, PMU2_XLDO_VSET_LEN, level);
}

void hal_pmu_xldo_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU2_XLDO_CFG_REG, enable_bypass, (REG16_POS)PMU2_EN_XLDO_SW_BIT);
}

void hal_pmu_xldo_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z != 0) {
        reg16_setbit(PMU2_XLDO_CFG_REG, PMU2_EN_XLDO_HIZ_BIT);
    } else {
        reg16_clrbit(PMU2_XLDO_CFG_REG, PMU2_EN_XLDO_HIZ_BIT);
    }
}

void hal_pmu_xldo_set_ictr(uint8_t ictr)
{
    reg16_setbits(PMU2_XLDO_CFG_REG, PMU2_XLDO_ICTR_BIT, PMU2_XLDO_ICTR_LEN, ictr);
}

/* INTLDO1 control */
void hal_pmu_intldo1_enable(switch_type_t en)
{
    if (en == TURN_ON) {
        hal_pmu_manual_select_control(EN_INTLDO1, TURN_ON);
        hal_pmu_manual_select_control(EN_INTLDO1_DELAY, TURN_ON);

        hal_pmu_manual_config_control(EN_INTLDO1, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_pmu_manual_config_control(EN_INTLDO1_DELAY, TURN_ON);
    } else {
        hal_pmu_manual_config_control(EN_INTLDO1, TURN_OFF);
        hal_pmu_manual_config_control(EN_INTLDO1_DELAY, TURN_OFF);
    }
}

void hal_pmu_intldo1_set_voltage(hal_pmu_intldo1_level_type_t level)
{
    reg16_setbits(PMU_INTLDO1_CFG_REG, PMU_INTLDO1_VSET_BIT, PMU_INTLDO1_VSET_LEN, level);
}

void hal_pmu_intldo1_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU_INTLDO1_CFG_REG, enable_bypass, (REG16_POS)PMU_EN_INTLDO1_SW_BIT);
}

void hal_pmu_intldo1_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z == TURN_ON) {
        reg16_setbit(PMU_INTLDO1_CFG_REG, PMU_INTLDO1_HIZ_BIT);
    } else {
        reg16_clrbit(PMU_INTLDO1_CFG_REG, PMU_INTLDO1_HIZ_BIT);
    }
}

/* INTLDO2 control */
void hal_pmu_intldo2_enable(switch_type_t en)
{
    if (en == TURN_ON) {
        hal_pmu_manual_select_control(EN_INTLDO2, TURN_ON);
        hal_pmu_manual_select_control(EN_INTLDO2_DELAY, TURN_ON);

        hal_pmu_manual_config_control(EN_INTLDO2, TURN_ON);
        uapi_tcxo_delay_us(PMU_LDO_POWER_ON_DELAY_US);
        hal_pmu_manual_config_control(EN_INTLDO2_DELAY, TURN_ON);
    } else {
        hal_pmu_manual_config_control(EN_INTLDO2, TURN_OFF);
        hal_pmu_manual_config_control(EN_INTLDO2_DELAY, TURN_OFF);
    }
}

void hal_pmu_intldo2_set_voltage(hal_pmu_intldo2_level_type_t level)
{
    reg16_setbits(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_VSET_BIT, PMU2_INTLDO2_VSET_LEN, level);
}

void hal_pmu_intldo2_enable_bypass(switch_type_t enable_bypass)
{
    hal_reg_config_bit(PMU2_INTLDO2_CFG_REG, enable_bypass, (REG16_POS)PMU2_EN_INTLDO2_SW_BIT);
}

void hal_pmu_intldo2_enable_high_resistance(switch_type_t enable_high_z)
{
    if (enable_high_z == TURN_ON) {
        reg16_setbit(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_HIZ_BIT);
    } else {
        reg16_clrbit(PMU2_INTLDO2_CFG_REG, PMU2_INTLDO2_HIZ_BIT);
    }
}

/* System wakeup time config */
void hal_pmu_system_wakeup_time_set(hal_pmu_system_wakeup_time_type_t pmu_module, int16_t delay_time)
{
    uint32_t reg_addr;
    uint16_t base_time;

    if (pmu_module == HAL_PMU_WKUP_EN_UVLO) {
        reg16_write(PMU_EN_UVLO_WKUP_TIME_SOC_REG, (uint16_t)delay_time);
        return;
    } else {
        reg_addr = (uint32_t)(PMU_EN_OVP_WKUP_TIME_SOC_REG + ((pmu_module - 1) * PMU_RST_REG_ADDR_OFFSET));
    }

    if (pmu_module == HAL_PMU_WKUP_EN_OVP) {
        reg16_read(PMU_EN_UVLO_WKUP_TIME_SOC_REG, base_time);
    } else {
        reg16_read((reg_addr - PMU_RST_REG_ADDR_OFFSET), base_time);
    }

    base_time = (uint16_t)(base_time + delay_time);
    uint32_t irq_sts = osal_irq_lock();
    reg16_write(reg_addr, base_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_en_intldo2_delay_time_config(uint16_t delay_time)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PMU_EN_INTLDO2_DELAY_TIME_SOC_REG, delay_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_en_xldo_delay_time_config(uint16_t delay_time)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PMU_EN_XLDO_DELAY_TIME_SOC_REG, delay_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_en_cldo1_delay_time_config(uint16_t delay_time)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PMU_EN_CLDO1_DELAY_TIME_SOC_REG, delay_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_cmu_ldo_en_delay_time_config(uint16_t delay_time)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PMU_CMU_LDO_EN_DELAY_TIME_SOC_REG, delay_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_cldo3_en_delay_time_config(uint16_t delay_time)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(PMU_EN_CLDO3_DELAY_TIME_SOC_REG, delay_time);
    osal_irq_restore(irq_sts);
}

void hal_pmu_set_cmu_xo_trim(hal_pmu_xo_driver_capacity_t ds)
{
    reg16_setbits(PMU2_CMU_TRIM_REG, PMU2_CMU_XO_CORE_TRIM_BIT, PMU2_CMU_XO_CORE_TRIM_LEN, ds);
}