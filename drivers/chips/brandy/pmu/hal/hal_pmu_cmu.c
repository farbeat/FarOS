/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Pmu and cmu hal driver source file.
 */

#include "hal_pmu_cmu.h"
#include "platform_core.h"
#include "hal_clocks_glb.h"
#include "hal_reg_config.h"
#include "tcxo.h"

#define PMU_REG_HIGH_16                         16

#define LPM_SLP_CFG_VLD_BYPASS_REG              (PMU1_CTL_RB_BASE + 0x790)
#define SLP_CFG_VLD_DOWN_BY_SLEEP               1

#define PMU_CMU_NOR_CFG_REG                     (PMU1_CTL_RB_BASE + 0x800)
#define PMU_CMU_SLP_CFG_REG                     (PMU1_CTL_RB_BASE + 0x804)
 /* CMU DBG CONTROL */
#define PMU_CMU_DBG_SEL_REG                     (PMU1_CTL_RB_BASE + 0x808)

 /* CMU RESET MANUAL & SLE Module */
#define PMU_CMU_MAN_REG                         (PMU1_CTL_RB_BASE + 0xE4)
#define PMU_CMU_SEL_REG                         (PMU1_CTL_RB_BASE + 0xE8)
#define PMU_CMU_STS_REG                         (PMU1_CTL_RB_BASE + 0xEC)
#define PMU_CMU_CTRL_CONFIG                     0x1FF

#define RC2_CFG_SEL_REG                         (PMU1_CTL_RB_BASE + 0xF0)
#define RC_CFG_CONTROL_BY_WKSLP_STATE_MACHINE   1

#define PMU_CMU_FAST_XO_ISO_BIT                 6
#define PMU_CMU_APERP_32K_SEL_BIT               7
#define PMU_CMU_TCXO_EN_BIT                     8
#define PMU_CMU_A32M_CLKEN_BIT                  9
#define PMU_CMU_B32M_CLKEN_BIT                  10
#define PMU_CMU_RC_PD_BIT                       11
#define PMU_CMU_RC_RSTN_BIT                     12
#define PMU_CMU_RC2DBB_PD_BIT                   13
#define PMU_CMU_RC2DBB_GT_BIT                   14
#define PMU_CMU_RC2_CLKEN_BIT                   15

#define SIMO1_CFG_3_REG                         (PMU1_CTL_RB_BASE + 0x10C)
#define SIMO1_ECO_EN_SEL                        8

#define PMU1_ANA_STATUS_REG                     (PMU1_CTL_RB_BASE + 0x30c)
#define PMU1_ANA_STATUS_STICK_REG               (PMU1_CTL_RB_BASE + 0x310)
#define PMU1_ANA_STATUS_STICK_CLR_REG           (PMU1_CTL_RB_BASE + 0x314)
#define PMU1_ANA_STATUS_GRM_REG                 (PMU1_CTL_RB_BASE + 0x318)
#define PMU1_ANA_STATUS_GRM_STICK_REG           (PMU1_CTL_RB_BASE + 0x31C)
#define PMU1_ANA_STATUS_GRM_STICK_CLR_REG       (PMU1_CTL_RB_BASE + 0x320)
#define PMU1_ANA_STATUS_CLR_MASK                0x1FF

#define PMU1_ANA_STATUS_RAW_INT_EN_REG          (PMU1_CTL_RB_BASE + 0x32c)
#define PMU1_ANA_STATUS_GRM_INT_EN_REG          (PMU1_CTL_RB_BASE + 0x330)

#define PMU1_CMU_DBG_REG                        (PMU1_CTL_RB_BASE + 0x80c)

#define PMU2_PLL_CLK_EN_CFG                     (PMU2_CMU_CTL_RB_BASE + 0x020)
#define PMU2_PLL_CFG_REG                        (PMU2_CMU_CTL_RB_BASE + 0x2c8)
// PLL CLOCK STEP REG
#define PMU2_PLL_FBDIV_TARGET_REG               (PMU2_CMU_CTL_RB_BASE + 0x600)
#define PMU2_FBDIV_TARGET_BIT                   0
#define PMU2_FBDIV_TARGET_LEN                   12

#define PMU2_PLL_FRAC_TARGET_H_REG              (PMU2_CMU_CTL_RB_BASE + 0x604)
#define PMU2_FRAC_TARGET_H_BIT                  0
#define PMU2_FRAC_TARGET_H_LEN                  8

#define PMU2_PLL_FRAC_TARGET_L_REG              (PMU2_CMU_CTL_RB_BASE + 0x608)
#define PMU2_FRAC_TARGET_L_BIT                  0
#define PMU2_FRAC_TARGET_L_LEN                  16

#define PMU2_PLL_FBDIV_MAN_REG                  (PMU2_CMU_CTL_RB_BASE + 0x610)
#define PMU2_FBDIV_MAN_BIT                      0
#define PMU2_FBDIV_MAN_LEN                      12

#define PMU2_PLL_FRAC_MAN_H_REG                 (PMU2_CMU_CTL_RB_BASE + 0x614)
#define PMU2_FRAC_MAN_H_BIT                     0
#define PMU2_FRAC_MAN_H_LEN                     8

#define PMU2_PLL_FRAC_MAN_L_REG                 (PMU2_CMU_CTL_RB_BASE + 0x618)
#define PMU2_FRAC_MAN_L_BIT                     0
#define PMU2_FRAC_MAN_L_LEN                     16

#define PMU2_PLL_FBDIV_SEL_REG                  (PMU2_CMU_CTL_RB_BASE + 0x620)
#define PMU2_FBDIV_SEL_BIT                      0

#define PMU2_PLL_FRAC_SEL_REG                   (PMU2_CMU_CTL_RB_BASE + 0x624)
#define PMU2_FRAC_SEL_BIT                       0

#define PMU2_PLL_CFG_VLD_REG                    (PMU2_CMU_CTL_RB_BASE + 0x628)
#define PMU2_PLL_CFG_VLD_BIT                    0

#define PMU2_PLL_FRAC_STEP_H_REG                (PMU2_CMU_CTL_RB_BASE + 0x248)
#define PMU2_PLL_FRAC_STEP_L_REG                (PMU2_CMU_CTL_RB_BASE + 0x24C)

#define PMU2_PLL_FBDIV_STS_REG                  (PMU2_CMU_CTL_RB_BASE + 0x250)
#define PMU2_PLL_FRAC_STS_H_REG                 (PMU2_CMU_CTL_RB_BASE + 0x254)
#define PMU2_PLL_FRAC_STS_L_REG                 (PMU2_CMU_CTL_RB_BASE + 0x258)

#define PMU2_PLL_POSTDIV_REG                    (PMU2_CMU_CTL_RB_BASE + 0x2c4)
#define PMU2_POSTDIV1_CORE_BIT                  0
#define PMU2_POSTDIV1_CORE_LEN                  3
#define PMU2_POSTDIV2_CORE_BIT                  3
#define PMU2_POSTDIV2_CORE_LEN                  3

#define PMU2_PLL_LOCK_STS_REG                   (PMU2_CMU_CTL_RB_BASE + 0x2d8)
#define PMU2_PLL_LOCK_STS_BIT                   0

#define PMU_CMU_TIMEOUT             1000
#define PMU_CMU_POWER_ON_DELAY_US   150ULL
#define PMU_CMU_PLL_LOCK_DELAY_US   150ULL

/*********************** CMU nor config ****************************/
void hal_cmu_nor_control(cmu_cfg_t cmu_nor_control, switch_type_t select_en)
{
    // Enable or disable cmu select.
    hal_reg_config_bit(PMU_CMU_NOR_CFG_REG, select_en, (REG16_POS)cmu_nor_control);
}

/*********************** CMU slp config ****************************/
void hal_cmu_slp_control(cmu_cfg_t cmu_slp_control, switch_type_t select_en)
{
    // Enable or disable cmu select.
    hal_reg_config_bit(PMU_CMU_SLP_CFG_REG, select_en, (REG16_POS)cmu_slp_control);
}

/*********************** CMU dbg config ****************************/
void hal_cmu_dbg_select(switch_type_t dbg_switch)
{
    if (dbg_switch == TURN_ON) {
        writew(PMU_CMU_DBG_SEL_REG, 0xffff);
    } else {
        writew(PMU_CMU_DBG_SEL_REG, 0);
    }
}

void hal_cmu_dbg_select_set(uint16_t cmu_sel_set)
{
    writew(PMU_CMU_DBG_SEL_REG, cmu_sel_set);
}

void hal_cmu_dbg_select_control(cmu_cfg_t cmu_sel_control, switch_type_t select_en)
{
    // Enable or disable cmu select.
    hal_reg_config_bit(PMU_CMU_DBG_SEL_REG, select_en, (REG16_POS)cmu_sel_control);
}

void hal_cmu_dbg_config_control(cmu_cfg_t cmu_signal, switch_type_t en)
{
    // 1: power off, 0: power on
    if (en == TURN_ON) {
        reg16_setbit(PMU1_CMU_DBG_REG, POS(cmu_signal));
    } else {
        reg16_clrbit(PMU1_CMU_DBG_REG, POS(cmu_signal));
    }
}

/*********************** CMU manual config ****************************/
void hal_cmu_manual_config(switch_type_t power_switch)
{
    if (power_switch == TURN_ON) {
        writew(PMU_CMU_MAN_REG, PMU_CMU_CTRL_CONFIG);
    } else {
        writew(PMU_CMU_MAN_REG, 0);
    }
}

void hal_cmu_manual_single_config(cmu_cfg_t cmu_signal, switch_type_t manual)
{
    if (manual == TURN_ON) {
        reg16_setbit(PMU_CMU_DBG_SEL_REG, POS(cmu_signal));
    } else {
        reg16_clrbit(PMU_CMU_DBG_SEL_REG, POS(cmu_signal));
    }
}

void hal_cmu_manual_config_set(uint16_t cmu_man_set)
{
    writew(PMU_CMU_MAN_REG, cmu_man_set);
}

void hal_cmu_manual_config_control(hal_cmu_man_sel_t cmu_man_control, switch_type_t manual_en)
{
    // Enable or disable cmu manual.
    hal_reg_config_bit(PMU_CMU_MAN_REG, manual_en, (REG16_POS)cmu_man_control);
}

void hal_cmu_manual_select(switch_type_t manual_switch)
{
    if (manual_switch == TURN_ON) {
        writew(PMU_CMU_SEL_REG, PMU_CMU_CTRL_CONFIG);
    } else {
        writew(PMU_CMU_SEL_REG, 0);
    }
}

void hal_cmu_manual_select_set(uint16_t cmu_sel_set)
{
    writew(PMU_CMU_SEL_REG, cmu_sel_set);
}

void hal_cmu_manual_select_control(hal_cmu_man_sel_t cmu_sel_control, switch_type_t select_en)
{
    // Enable or disable cmu select.
    hal_reg_config_bit(PMU_CMU_SEL_REG, select_en, (REG16_POS)cmu_sel_control);
}

uint16_t hal_pmu_cmu_get_ana_status(void)
{
    return readw(PMU1_ANA_STATUS_REG);
}

void hal_pmu_cmu_clear_ana_int_status(void)
{
    writew(PMU1_ANA_STATUS_STICK_CLR_REG, PMU1_ANA_STATUS_CLR_MASK);
    writew(PMU1_ANA_STATUS_GRM_STICK_CLR_REG, PMU1_ANA_STATUS_CLR_MASK);
}

void hal_pmu_cmu_analog_status_raw_interrupt_enable(uint16_t pos)
{
    reg16_setbit(PMU1_ANA_STATUS_RAW_INT_EN_REG, POS(pos));
}

void hal_pmu_cmu_analog_status_raw_interrupt_disable(uint16_t pos)
{
    reg16_clrbit(PMU1_ANA_STATUS_RAW_INT_EN_REG, POS(pos));
}

void hal_pmu_analog_status_grm_interrupt_enable(uint16_t position, switch_type_t on)
{
    if (on != 0) {
        reg16_setbit(PMU1_ANA_STATUS_GRM_INT_EN_REG, POS(position));
    } else {
        reg16_clrbit(PMU1_ANA_STATUS_GRM_INT_EN_REG, POS(position));
    }
}

void hal_pmu_cmu_core_pll_cfg_control(pmu_cmu_core_pll_cfg_t pll_cfg, switch_type_t pll_switch)
{
    if (pll_switch == TURN_ON) {
        reg16_setbit(PMU2_PLL_CFG_REG, POS(pll_cfg));
    } else {
        reg16_clrbit(PMU2_PLL_CFG_REG, POS(pll_cfg));
    }
}

void hal_pmu2_cmu_pll_fbdiv_target_set(uint16_t div)
{
    reg16_write(PMU2_PLL_FBDIV_TARGET_REG, div);
}

void hal_pmu2_cmu_pll_frac_target_set(uint32_t frac)
{
    uint32_t low = frac & 0xffff;
    uint32_t high = frac >> PMU_REG_HIGH_16;

    reg16_write((void *)(uintptr_t)PMU2_PLL_FRAC_TARGET_H_REG, (uint16_t)high);
    reg16_write((void *)((uintptr_t)PMU2_PLL_FRAC_TARGET_L_REG), (uint16_t)low);
}


void hal_pmu2_cmu_pll_fbdiv_manual_set(uint16_t div)
{
    reg16_setbits(PMU2_PLL_FBDIV_MAN_REG, PMU2_FBDIV_MAN_BIT, PMU2_FBDIV_MAN_LEN, div);
}

uint16_t hal_pmu2_cmu_pll_fbdiv_manual_get(void)
{
    return reg16_getbits(PMU2_PLL_FBDIV_MAN_REG, PMU2_FBDIV_MAN_BIT, PMU2_FBDIV_MAN_LEN);
}

void hal_pmu2_cmu_pll_frac_manual_set(uint16_t num_h, uint16_t num_l)
{
    reg16_write(PMU2_PLL_FRAC_MAN_H_REG, num_h);
    reg16_write(PMU2_PLL_FRAC_MAN_L_REG, num_l);
}

uint32_t hal_pmu2_cmu_pll_frac_manual_get(void)
{
    uint32_t h_value = readw(PMU2_PLL_FRAC_MAN_H_REG);
    uint32_t l_value = readw(PMU2_PLL_FRAC_MAN_L_REG);

    return ((h_value << PMU_REG_HIGH_16) | l_value);
}

void hal_pmu2_cmu_pll_fbdiv_control_select(switch_type_t manual_control)
{
    hal_reg_config_bit(PMU2_PLL_FBDIV_SEL_REG, manual_control, (REG16_POS)PMU2_FBDIV_SEL_BIT);
}

void hal_pmu2_cmu_pll_frac_control_select(switch_type_t manual_control)
{
    hal_reg_config_bit(PMU2_PLL_FRAC_SEL_REG, manual_control, (REG16_POS)PMU2_FRAC_SEL_BIT);
}

void hal_pmu2_cmu_pll_lock_check(void)
{
    hal_reg_status_check_timeout(PMU2_PLL_LOCK_STS_REG, PMU2_PLL_LOCK_STS_BIT, TURN_ON, PMU_CMU_PLL_LOCK_DELAY_US);
}

void hal_pmu2_cmu_pll_cfg_valid(switch_type_t valid)
{
    hal_reg_config_bit(PMU2_PLL_CFG_VLD_REG, valid, (REG16_POS)PMU2_PLL_CFG_VLD_BIT);
}

void hal_pmu2_cmu_pll_set_frac_step(uint16_t step_h, uint16_t step_l)
{
    writew(PMU2_PLL_FRAC_STEP_H_REG, step_h);
    writew(PMU2_PLL_FRAC_STEP_L_REG, step_l);
}

uint16_t hal_pmu2_cmu_pll_fbdiv_status(void)
{
    return reg16(PMU2_PLL_FBDIV_STS_REG);
}

uint32_t hal_pmu2_cmu_pll_frac_status(void)
{
    uint32_t sts_h;
    uint32_t sts_l;
    sts_h = (uint32_t)reg16(PMU2_PLL_FRAC_STS_H_REG);
    sts_l = (uint32_t)reg16(PMU2_PLL_FRAC_STS_L_REG);
    sts_h = (sts_h << PMU_REG_HIGH_16) + sts_l;

    return sts_h;
}

void hal_pmu2_cmu_pll_post_div_set(uint8_t div2, uint8_t div1)
{
    reg16_setbits(PMU2_PLL_POSTDIV_REG, PMU2_POSTDIV2_CORE_BIT, PMU2_POSTDIV2_CORE_LEN, div2);
    reg16_setbits(PMU2_PLL_POSTDIV_REG, PMU2_POSTDIV1_CORE_BIT, PMU2_POSTDIV1_CORE_LEN, div1);
}

void hal_pmu2_cmu_pll_post_div_get(uint8_t *div2, uint8_t *div1)
{
    *div1 = reg16_getbits(PMU2_PLL_POSTDIV_REG, PMU2_POSTDIV2_CORE_BIT, PMU2_POSTDIV2_CORE_LEN);
    *div2 = reg16_getbits(PMU2_PLL_POSTDIV_REG, PMU2_POSTDIV1_CORE_BIT, PMU2_POSTDIV1_CORE_LEN);
}

void hal_pmu_rc2_control_sel(void)
{
    writew(RC2_CFG_SEL_REG, RC_CFG_CONTROL_BY_WKSLP_STATE_MACHINE);
}

void hal_pmu_slp_cfg_vld_sel(void)
{
    writew(LPM_SLP_CFG_VLD_BYPASS_REG, SLP_CFG_VLD_DOWN_BY_SLEEP);
}

void hal_pmu_simo1_set(void)
{
    reg16_setbit(SIMO1_CFG_3_REG, SIMO1_ECO_EN_SEL);
}