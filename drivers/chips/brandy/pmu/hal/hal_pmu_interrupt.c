/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: PMU INTERRUPT FUNCTIONS
 */

#include "hal_pmu_interrupt.h"
#include "non_os.h"
#include "hal_reg_config.h"

/* ALLOW TO SLEEP REGS of GLB_CLT_M_RB Module */
#define PMU_LPM_M_ALW_TO_SLP_REG                (PMU1_CTL_RB_BASE + 0x670)

#define PMU_LPM_AUDIO_INT_MASK_REG              (PMU1_CTL_RB_BASE + 0x6A0)
#define PMU_LPM_VPU_INT_EN_BIT                  0
#define PMU_LPM_MAD_INT_EN_BIT                  1
#define PMU_LPM_AUDIO_WDT_INT_EN_BIT            2

/* MCPU LPM_XXXX_WKUP_INT_MASK Register of PMU_CLT_RB Module */
#define PMU_LPM_MCPU_MRTC_WKUP_INT_MASK_REG     (PMU1_CTL_RB_BASE + 0x6A4)
#define PMU_LPM_MCPU_DRTC_WKUP_INT_MASK_REG     (PMU1_CTL_RB_BASE + 0x6A8)
#define PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG      (PMU1_CTL_RB_BASE + 0x6AC)
#define PMU_LPM_MCPU_GPIO_WKUP_INT_MASK_REG     (PMU1_CTL_RB_BASE + 0x6B0)

/* MCPU LPM xxx WKUP MASK start bit postion */
#define PMU_LPM_MCPU_MRTC_WKUP_MASK_POSITION 0
#define PMU_LPM_MCPU_MRTC_WKUP_MASK_BITS     4

#define PMU_LPM_MCPU_DRTC_WKUP_MASK_POSITION 0
#define PMU_LPM_MCPU_DRTC_WKUP_MASK_BITS     4

#define PMU_LPM_MCPU_D2M_1_IPC_WKUP_MASK_POSITION 0
#define PMU_LPM_MCPU_D2M_1_IPC_WKUP_MASK_BITS     2

#define PMU_LPM_MCPU_D2M_0_IPC_WKUP_MASK_POSITION 2
#define PMU_LPM_MCPU_D2M_0_IPC_WKUP_MASK_BITS     2

#define PMU_LPM_MCPU_B2D_1_IPC_WKUP_MASK_POSITION 4
#define PMU_LPM_MCPU_B2D_1_IPC_WKUP_MASK_BITS     2

#define PMU_LPM_MCPU_B2D_0_IPC_WKUP_MASK_POSITION 6
#define PMU_LPM_MCPU_B2D_0_IPC_WKUP_MASK_BITS     2

#define PMU_LPM_MCPU_B2M_IPC_WKUP_MASK_POSITION   8
#define PMU_LPM_MCPU_B2M_IPC_WKUP_MASK_BITS       2

#define PMU_LPM_MCPU_GPIO_WKUP_MASK_POSITION 0
#define PMU_LPM_MCPU_GPIO_WKUP_MASK_BITS     4

/* MCPU LPM xxx WKUP MASK */
#define PMU_LPM_MCPU_MRTC_WKUP_MASK  0xF
#define PMU_LPM_MCPU_DRTC_WKUP_MASK  0xF
#define PMU_LPM_MCPU_IPC_WKUP_MASK   0x3
#define PMU_LPM_MCPU_GPIO_WKUP_MASK  0xF

/* MCPU LPM_WKUP_EVT_XXX Register of PMU_CLT_RB Module */
#define PMU_LPM_MCPU_WKUP_EVT_CLR_REG           (PMU1_CTL_RB_BASE + 0x680)
#define PMU_LPM_MCPU_WKUP_EVT_EN_REG            (PMU1_CTL_RB_BASE + 0x684)
#define PMU_LPM_MCPU_WKUP_EVT_STS_REG           (PMU1_CTL_RB_BASE + 0x688)

/* MCPU LPM_WKUP_INT_XXX Register of PMU_CLT_RB Module */
#define PMU_LPM_MCPU_WKUP_INT_CLR_REG           (PMU1_CTL_RB_BASE + 0x690)
#define PMU_LPM_MCPU_WKUP_INT_EN_REG            (PMU1_CTL_RB_BASE + 0x694)
#define PMU_LPM_MCPU_WKUP_INT_STS_REG           (PMU1_CTL_RB_BASE + 0x698)

/* MCPU LPM_SLP_EVT_XXX Register of PMU_CLT_RB Module */
#define PMU_LPM_MCPU_SLP_EVT_CLR_REG            (PMU1_CTL_RB_BASE + 0x700)
#define PMU_LPM_MCPU_SLP_EVT_EN_REG             (PMU1_CTL_RB_BASE + 0x704)
#define PMU_LPM_MCPU_SLP_EVT_STS_REG            (PMU1_CTL_RB_BASE + 0x708)

/* MCPU LPM_SLP_INT_XXX Register of PMU_CLT_RB Module */
#define PMU_LPM_MCPU_SLP_INT_CLR_REG            (PMU1_CTL_RB_BASE + 0x710)
#define PMU_LPM_MCPU_SLP_INT_EN_REG             (PMU1_CTL_RB_BASE + 0x714)
#define PMU_LPM_MCPU_SLP_INT_STS_REG            (PMU1_CTL_RB_BASE + 0x718)

/* MCPU LPM_XXX_SLP BIT position */
#define PMU_LPM_MCPU_SLEEPING  1
#define PMU_LPM_MCPU_SLEEPDEEP 0
#define PMU_LPM_MCPU_SLP_MASK  0x3

#define PMU_SLEEP_INTERRUPT_ID_MASK  0x3

#define PMU_LPM_SYS_BYPASS_BT_SLP_STS           (PMU1_CTL_RB_BASE + 0x380)

#define PMU_LPM_AUDIO_ALW_TO_SLP_REG            (PMU1_CTL_RB_BASE + 0x720)

#define PMU_LPM_AUDIO_WKUP_EVT_CLR_REG          (PMU1_CTL_RB_BASE + 0x724)
#define PMU_LPM_AUDIO_WKUP_EN_REG               (PMU1_CTL_RB_BASE + 0x728)
#define PMU_LPM_AUDIO_WKUP_EVT_STS_REG          (PMU1_CTL_RB_BASE + 0x72C)

#define PMU_LPM_AUDIO_WKUP_INT_CLR_REG          (PMU1_CTL_RB_BASE + 0x730)
#define PMU_LPM_AUDIO_WKUP_INT_EN_REG           (PMU1_CTL_RB_BASE + 0x734)
#define PMU_LPM_AUDIO_WKUP_INT_STS_REG          (PMU1_CTL_RB_BASE + 0x738)
#define PMU_LPM_AUDIO_WKUP_BIT                  0

#define PMU_LPM_MSYS_SLP_HISTORY_REG            (PMU1_CTL_RB_BASE + 0x08C)
/* MCPU SYS_HISTORY BIT position */
#define PMU_MSYS_HISTORY_CLR 1
#define PMU_MSYS_HISTORY     0

#define PMU_LPM_REG_WIDTH    16

void hal_pmu_lpm_audio_vpu_interrupt_mask(switch_type_t on)
{
    hal_reg_config_bit(PMU_LPM_AUDIO_INT_MASK_REG, on, (REG16_POS)PMU_LPM_VPU_INT_EN_BIT);
}

void hal_pmu_lpm_audio_mad_interrupt_mask(switch_type_t on)
{
    hal_reg_config_bit(PMU_LPM_AUDIO_INT_MASK_REG, on, (REG16_POS)PMU_LPM_MAD_INT_EN_BIT);
}

static void hal_pmu_lpm_wakeup_interrupt_mask(uint32_t reg_addr, switch_type_t on, uint16_t position,
                                              uint8_t bits, uint16_t mask)
{
    if (on != 0) {
        reg16_setbits(reg_addr, position, bits, mask);
    } else {
        reg16_clrbits(reg_addr, position, bits);
    }
}

void hal_pmu_lpm_mrtc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_MRTC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_MRTC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_MRTC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_MRTC_WKUP_MASK);
}

void hal_pmu_lpm_drtc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_DRTC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_DRTC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_DRTC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_DRTC_WKUP_MASK);
}

void hal_pmu_lpm_d2m_0_ipc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_D2M_0_IPC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_D2M_0_IPC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_IPC_WKUP_MASK);
}

void hal_pmu_lpm_d2m_1_ipc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_D2M_1_IPC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_D2M_1_IPC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_IPC_WKUP_MASK);
}

void hal_pmu_lpm_b2d_0_ipc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_B2D_0_IPC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_B2D_0_IPC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_IPC_WKUP_MASK);
}

void hal_pmu_lpm_b2d_1_ipc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_B2D_1_IPC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_B2D_1_IPC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_IPC_WKUP_MASK);
}

void hal_pmu_lpm_b2m_ipc_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_IPC_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_B2M_IPC_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_B2M_IPC_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_IPC_WKUP_MASK);
}

void hal_pmu_lpm_gpio_wakeup_interrupt_mask(switch_type_t on)
{
    hal_pmu_lpm_wakeup_interrupt_mask(PMU_LPM_MCPU_GPIO_WKUP_INT_MASK_REG,
                                      on,
                                      PMU_LPM_MCPU_GPIO_WKUP_MASK_POSITION,
                                      PMU_LPM_MCPU_GPIO_WKUP_MASK_BITS,
                                      PMU_LPM_MCPU_GPIO_WKUP_MASK);
}

void hal_pmu_lpm_mcpu_allow_to_sleep(switch_type_t on)
{
    do {
        reg16_write(PMU_LPM_M_ALW_TO_SLP_REG, (uint16_t)on);
    } while (on != readw(PMU_LPM_M_ALW_TO_SLP_REG));
}

switch_type_t hal_pmu_lpm_get_mcpu_allow_to_sleep(void)
{
    return readw(PMU_LPM_M_ALW_TO_SLP_REG);
}

void hal_pmu_lpm_wakeup_status_clear(lpm_action_type_t type, uint16_t position)
{
    uint32_t clr_regaddr;

    if (type == LPM_INT_ACTION) {
        clr_regaddr = PMU_LPM_MCPU_WKUP_INT_CLR_REG;
    } else {
        clr_regaddr = PMU_LPM_MCPU_WKUP_EVT_CLR_REG;
    }

    reg16_setbit(clr_regaddr, POS(position));
}

static void hal_pmu_lpm_config_bitmask(uint32_t register_addr, switch_type_t on, uint16_t bitmask)
{
    if (on == TURN_ON) {
        reg16_setbitmsk(register_addr, bitmask);
    } else {
        reg16_clrbitmsk(register_addr, bitmask);
    }
}

void hal_pmu_lpm_wakeup_all_status_clear(lpm_action_type_t type)
{
    uint32_t clr_regaddr = (type == LPM_INT_ACTION) ? PMU_LPM_MCPU_WKUP_INT_CLR_REG : PMU_LPM_MCPU_WKUP_EVT_CLR_REG;
    reg16_setbitmsk(clr_regaddr, PMU_LPM_MCPU_WKUP_ALL_MASK);
}

void hal_pmu_lpm_wakeup_action_enable(lpm_action_type_t type, switch_type_t on, uint16_t position)
{
    uint32_t register_addr = (type == LPM_INT_ACTION) ? PMU_LPM_MCPU_WKUP_INT_EN_REG : PMU_LPM_MCPU_WKUP_EVT_EN_REG;

    if (on == TURN_ON) {
        reg16_setbit(register_addr, POS(position));
    } else {
        reg16_clrbit(register_addr, POS(position));
    }
}

void hal_pmu_lpm_wakeup_action_all_enable(lpm_action_type_t type, switch_type_t on)
{
    uint32_t register_addr = (type == LPM_INT_ACTION) ? PMU_LPM_MCPU_WKUP_INT_EN_REG : PMU_LPM_MCPU_WKUP_EVT_EN_REG;
    hal_pmu_lpm_config_bitmask(register_addr, on, PMU_LPM_MCPU_WKUP_MASK);
}

void hal_pmu_lpm_wakeup_action_all_init(lpm_action_type_t type)
{
    uint32_t register_addr = (type == LPM_INT_ACTION) ? PMU_LPM_MCPU_WKUP_INT_EN_REG : PMU_LPM_MCPU_WKUP_EVT_EN_REG;
    reg16_clrbitmsk(register_addr, PMU_LPM_MCPU_WKUP_MASK);
}

uint16_t hal_pmu_lpm_wakeup_get_status(lpm_action_type_t type, uint16_t position)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_WKUP_INT_STS_REG;
    } else {
        register_addr = PMU_LPM_MCPU_WKUP_EVT_STS_REG;
    }

    return reg16_getbit(register_addr, POS(position));
}

uint16_t hal_pmu_lpm_wakeup_get_all_status(lpm_action_type_t type)
{
    uint16_t interrupt_id;
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_WKUP_INT_STS_REG;
    } else {
        register_addr = PMU_LPM_MCPU_WKUP_EVT_STS_REG;
    }
    interrupt_id = reg16_getbits(register_addr, POS(PMU_LPM_MCPU_AON_CAPSENS_WAKEUP), PMU_LPM_MCPU_WKAEUP_MASK_LEN);

    return (interrupt_id & PMU_LPM_MCPU_WKUP_ALL_MASK);
}

void hal_pmu_lpm_make_sure_all_status_clear(lpm_type_t lpm_type, lpm_action_type_t action_type)
{
    uint32_t sts_regaddr;
    uint16_t mask_bits;

    if (lpm_type == LPM_TYPE_WAKEUP) {
        mask_bits = PMU_LPM_MCPU_WKUP_MASK;
        if (action_type == LPM_INT_ACTION) {
            sts_regaddr = PMU_LPM_MCPU_WKUP_INT_STS_REG;
        } else {
            sts_regaddr = PMU_LPM_MCPU_WKUP_EVT_STS_REG;
        }
    } else {
        mask_bits = PMU_LPM_MCPU_SLP_MASK;
        if (action_type == LPM_INT_ACTION) {
            sts_regaddr = PMU_LPM_MCPU_SLP_INT_STS_REG;
        } else {
            sts_regaddr = PMU_LPM_MCPU_SLP_EVT_STS_REG;
        }
    }

    while ((readw(sts_regaddr) & mask_bits) != 0) {
        if (lpm_type == LPM_TYPE_WAKEUP) {
            hal_pmu_lpm_wakeup_all_status_clear(action_type);
        } else {
            hal_pmu_lpm_sleep_all_status_clear(action_type);
        }
    }
}

void hal_pmu_lpm_sleep_status_clear(lpm_action_type_t type, uint16_t position)
{
    uint32_t clr_regaddr;
    uint32_t sts_regaddr;

    if (type == LPM_INT_ACTION) {
        clr_regaddr = PMU_LPM_MCPU_SLP_INT_CLR_REG;
        sts_regaddr = PMU_LPM_MCPU_SLP_INT_STS_REG;
    } else {
        clr_regaddr = PMU_LPM_MCPU_SLP_EVT_CLR_REG;
        sts_regaddr = PMU_LPM_MCPU_SLP_EVT_STS_REG;
    }

    do {
        reg16_setbit(clr_regaddr, POS(position));
    } while (reg16_getbit(sts_regaddr, POS(position)) != 0);
}

void hal_pmu_lpm_sleep_all_status_clear(lpm_action_type_t type)
{
    uint32_t clr_regaddr;

    if (type == LPM_INT_ACTION) {
        clr_regaddr = PMU_LPM_MCPU_SLP_INT_CLR_REG;
    } else {
        clr_regaddr = PMU_LPM_MCPU_SLP_EVT_CLR_REG;
    }

    reg16_setbitmsk(clr_regaddr, PMU_LPM_MCPU_SLP_MASK);
}

uint16_t hal_pmu_lpm_sleep_get_status(lpm_action_type_t type, uint16_t position)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_SLP_INT_STS_REG;
    } else {
        register_addr = PMU_LPM_MCPU_SLP_EVT_STS_REG;
    }

    return reg16_getbit(register_addr, POS(position));
}

uint16_t hal_pmu_lpm_sleep_get_all_status(lpm_action_type_t type)
{
    uint16_t interrupt_id;
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_SLP_INT_STS_REG;
    } else {
        register_addr = PMU_LPM_MCPU_SLP_EVT_STS_REG;
    }

    interrupt_id = reg16_getbits(register_addr,
                                 POS(PMU_LPM_MCPU_SLEEPDEEP),
                                 (POS(PMU_LPM_MCPU_SLEEPING) - POS(PMU_LPM_MCPU_SLEEPDEEP) + 1));

    return (interrupt_id & PMU_SLEEP_INTERRUPT_ID_MASK);
}

void hal_pmu_lpm_sleep_action_enable(lpm_action_type_t type, switch_type_t on, uint16_t position)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_SLP_INT_EN_REG;
    } else {
        register_addr = PMU_LPM_MCPU_SLP_EVT_EN_REG;
    }

    if (on == TURN_ON) {
        reg16_setbit(register_addr, POS(position));
    } else {
        reg16_clrbit(register_addr, POS(position));
    }
}

void hal_pmu_lpm_sleep_action_all_enable(lpm_action_type_t type, switch_type_t on)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_SLP_INT_EN_REG;
    } else {
        register_addr = PMU_LPM_MCPU_SLP_EVT_EN_REG;
    }

    hal_pmu_lpm_config_bitmask(register_addr, on, PMU_LPM_MCPU_SLP_MASK);
}

void hal_pmu_lpm_sleep_action_all_init(lpm_action_type_t type)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_MCPU_SLP_INT_EN_REG;
    } else {
        register_addr = PMU_LPM_MCPU_SLP_EVT_EN_REG;
    }

    reg16_clrbitmsk(register_addr, PMU_LPM_MCPU_SLP_MASK);
}

bool hal_pmu_lpm_msystem_sleep_history(void)
{
    return (bool)reg16_getbit(PMU_LPM_MSYS_SLP_HISTORY_REG, POS(PMU_MSYS_HISTORY));
}

void hal_pmu_lpm_msystem_sleep_history_clear(void)
{
    reg16_setbit(PMU_LPM_MSYS_SLP_HISTORY_REG, POS(PMU_MSYS_HISTORY_CLR));
}

void hal_pmu_lpm_system_bypass_bt_status(switch_type_t on)
{
    if (on != 0) {
        // Bypass BT system status
        reg16_clrbit(PMU_LPM_SYS_BYPASS_BT_SLP_STS, 0);
    } else {
        // Do not bypass BT system status
        reg16_setbit(PMU_LPM_SYS_BYPASS_BT_SLP_STS, 0);
    }
}

void hal_pmu_lpm_audio_allow_to_sleep(switch_type_t on)
{
    hal_reg_config_bit(PMU_LPM_AUDIO_ALW_TO_SLP_REG, on, (REG16_POS)PMU_LPM_AUDIO_WKUP_BIT);
}

void hal_pmu_lpm_audio_wakeup_status_clear(lpm_action_type_t type)
{
    uint32_t clr_regaddr;
    uint32_t sts_regaddr;

    if (type == LPM_INT_ACTION) {
        clr_regaddr = PMU_LPM_AUDIO_WKUP_INT_CLR_REG;
        sts_regaddr = PMU_LPM_AUDIO_WKUP_INT_STS_REG;
    } else {
        clr_regaddr = PMU_LPM_AUDIO_WKUP_EVT_CLR_REG;
        sts_regaddr = PMU_LPM_AUDIO_WKUP_EVT_STS_REG;
    }

    do {
        reg16_setbit(clr_regaddr, PMU_LPM_AUDIO_WKUP_BIT);
    } while (reg16_getbit(sts_regaddr, PMU_LPM_AUDIO_WKUP_BIT) != 0);
}

void hal_pmu_lpm_audio_wakeup_enable(lpm_action_type_t type, switch_type_t on)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_AUDIO_WKUP_INT_EN_REG;
    } else {
        register_addr = PMU_LPM_AUDIO_WKUP_EN_REG;
    }

    if (on == TURN_ON) {
        reg16_setbit(register_addr, PMU_LPM_AUDIO_WKUP_BIT);
    } else {
        reg16_clrbit(register_addr, PMU_LPM_AUDIO_WKUP_BIT);
    }
}

uint16_t hal_pmu_lpm_audio_wakeup_get_status(lpm_action_type_t type)
{
    uint32_t register_addr;

    if (type == LPM_INT_ACTION) {
        register_addr = PMU_LPM_AUDIO_WKUP_INT_STS_REG;
    } else {
        register_addr = PMU_LPM_AUDIO_WKUP_EVT_STS_REG;
    }

    return reg16_getbit(register_addr, PMU_LPM_AUDIO_WKUP_BIT);
}

