/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  PMU DRIVER Source
 */

#include "hal_pmu_pg.h"
#include <stdint.h>
#include "panic.h"
#include "platform_core.h"
#include "chip_io.h"
#include "hal_reg_config.h"

#define PMU1_B_SOC_PWR_EN_REG               (PMU1_CTL_RB_BASE + 0x20)
#define PMU1_BT_CBB_PWR_EN_REG              (PMU1_CTL_RB_BASE + 0x24)
#define PMU1_BMEM_PWR_EN_REG                (PMU1_CTL_RB_BASE + 0x28)
#define PMU1_BT_EM_PWR_EN_REG               (PMU1_CTL_RB_BASE + 0x2C)

#define PMU1_MCU_HS_PWR_EN_REG              (PMU1_CTL_RB_BASE + 0x30)
#define PMU1_DSUB_PWR_EN_REG                (PMU1_CTL_RB_BASE + 0x34)
#define PMU1_ASUB_PWR_EN_REG                (PMU1_CTL_RB_BASE + 0x38)

#define PMU1_S_EGPIO_PWR_EN_REG             (PMU1_CTL_RB_BASE + 0x5C)
#define PMU1_BT_RF_PWR_EN_REG               (PMU1_CTL_RB_BASE + 0x60)
#define PMU1_MCU_PWR_EN_REG                 (PMU1_CTL_RB_BASE + 0x64)
#define PMU1_MEM_PWR_EN_REG                 (PMU1_CTL_RB_BASE + 0x68)
#define PMU1_COM_PWR_EN_REG                 (PMU1_CTL_RB_BASE + 0x78)
#define PMU1_ISO_EN_BIT                     8
#define PMU1_PWR_ACK_BIT                    4
#define PMU1_PWR_EN_BIT                     0

#define PMU1_MCU_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x880)
#define PMU1_BSOC_AUTO_PG_REG               (PMU1_CTL_RB_BASE + 0X884)
#define PMU1_COM_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x888)
#define PMU1_MEM_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x88C)
#define PMU1_BMEM_AUTO_PG_REG               (PMU1_CTL_RB_BASE + 0x890)
#define PMU1_AUTO_PG_EN_BIT                 0

#define PMU1_L2M_GMMU_PWR_EN_REG            (PMU1_CTL_RB_BASE + 0x3C)
#define PMU1_L2M_GMMU_RET1_EN_REG           (PMU1_CTL_RB_BASE + 0x40)
#define PMU1_L2M_GMMU_RET2_EN_REG           (PMU1_CTL_RB_BASE + 0x44)
#define PMU1_L2M_GMMU_PWR_ACK_REG           (PMU1_CTL_RB_BASE + 0x48)
#define PMU1_L2M_DSP_PWR_EN_REG             (PMU1_CTL_RB_BASE + 0x4C)
#define PMU1_L2M_DSP_RET1_EN_REG            (PMU1_CTL_RB_BASE + 0x50)
#define PMU1_L2M_DSP_RET2_EN_REG            (PMU1_CTL_RB_BASE + 0x54)
#define PMU1_L2M_DSP_PWR_ACK_REG            (PMU1_CTL_RB_BASE + 0x58)
#define PMU1_L2M_PWR_EN_MASK                0xFFF

#define PMU1_BMEM_PWR_MODE_REG              (PMU1_CTL_RB_BASE + 0x6C)
#define PMU1_BMEM_PWR_MODE_EN_BIT           0
#define PMU1_BMEM_PWR_MODE_RDY_BIT          8
#define PMU1_BMEM_PWR_MODE_EN_MASK          0x1F

#define PMU1_XIP_MEM_PWR_REG                (PMU1_CTL_RB_BASE + 0x900)
#define PMU1_XIP_MEM_RET1_EN_BIT            0
#define PMU1_XIP_MEM_RET2_EN_BIT            4
#define PMU1_XIP_MEM_PWR_EN_BIT             8
#define PMU1_XIP_MEM_PWR_RDY_BIT            12

#define PMU1_OPERATION_TIMEOUT_MS           30
#define PMU1_XIP_PWR_ON_DELAY_US            1000

#define MEM_SHARE_CTRL_REG                  (XIP_CACHE_CTL + 0xd6c)
#define MEM_SHARE_CTRL_GT_REG               (XIP_CACHE_CTL + 0xd70)
#define DSP_SHARE_MODE_BIT                  0
#define DSP_SHARE_MODE_LEN                  4
#define DSP_SHARE_MODE_GT                   0

#define BRAM_SHARE_MODE_REG                 (COM_CTL_RB_BASE + 0xae4)
#define BRAM_SHARE_MODE_GT                  4
#define BRAM_SHARE_MODE_BIT                 0
#define BRAM_SHARE_MODE_LEN                 2

static uint32_t g_pmu1_dig_pg_regs[PMU1_DIG_PG_MAX] = {
    /* Common region. */
    PMU1_B_SOC_PWR_EN_REG,
    PMU1_BT_CBB_PWR_EN_REG,
    PMU1_BMEM_PWR_EN_REG,
    PMU1_BT_EM_PWR_EN_REG,
    PMU1_MCU_HS_PWR_EN_REG,
    PMU1_DSUB_PWR_EN_REG,
    PMU1_ASUB_PWR_EN_REG,
    PMU1_S_EGPIO_PWR_EN_REG,
    PMU1_BT_RF_PWR_EN_REG,
    PMU1_MCU_PWR_EN_REG,
    PMU1_MEM_PWR_EN_REG,
    PMU1_COM_PWR_EN_REG,
    PMU1_MCU_AUTO_PG_REG,
    PMU1_BSOC_AUTO_PG_REG,
    PMU1_COM_AUTO_PG_REG,
    PMU1_MEM_AUTO_PG_REG,
    PMU1_BMEM_AUTO_PG_REG,
};

static uint32_t hal_pmu1_dig_pg_common_reg_get(pmu1_dig_pg_sub_sys_t sys)
{
    if (sys == PMU1_DIG_PG_MAX) {
        panic(PANIC_PMU, __LINE__);
        return 0; //lint !e527  unreachable code
    }
    return g_pmu1_dig_pg_regs[sys];
}

void hal_pmu1_dig_pg_common_iso_en(pmu1_dig_pg_sub_sys_t sys, bool en)
{
    uint32_t pg_cfg_reg = hal_pmu1_dig_pg_common_reg_get(sys);
    if (en) {
        reg16_setbit(pg_cfg_reg, PMU1_ISO_EN_BIT);
    } else {
        reg16_clrbit(pg_cfg_reg, PMU1_ISO_EN_BIT);
    }
}

void hal_pmu1_dig_pg_common_pwr_en(pmu1_dig_pg_sub_sys_t sys, bool en)
{
    uint32_t pg_cfg_reg = hal_pmu1_dig_pg_common_reg_get(sys);
    if (en) {
        reg16_setbit(pg_cfg_reg, PMU1_PWR_EN_BIT);
    } else {
        reg16_clrbit(pg_cfg_reg, PMU1_PWR_EN_BIT);
    }
}

bool hal_pmu1_dig_pg_common_pwr_ack_get(pmu1_dig_pg_sub_sys_t sys)
{
    uint32_t pg_cfg_reg = hal_pmu1_dig_pg_common_reg_get(sys);
    hal_reg_status_check_timeout(pg_cfg_reg, PMU1_PWR_ACK_BIT, TURN_ON, PMU1_OPERATION_TIMEOUT_MS);
    return reg16_getbit(pg_cfg_reg, PMU1_PWR_ACK_BIT);
}

void hal_pmu1_dig_pg_auto_pwr_en(pmu1_dig_pg_sub_sys_t sys, bool en)
{
    uint32_t auto_pg_reg = hal_pmu1_dig_pg_common_reg_get(sys);
    if (en) {
        reg16_setbit(auto_pg_reg, PMU1_AUTO_PG_EN_BIT);
    } else {
        reg16_clrbit(auto_pg_reg, PMU1_AUTO_PG_EN_BIT);
    }
}

void hal_pmu1_dig_pg_l2m_gmmu_pwr_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_GMMU_PWR_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_GMMU_PWR_EN_REG, PMU1_L2M_PWR_EN_MASK);
    }
}

void hal_pmu1_dig_pg_l2m_gmmu_ret1_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_GMMU_RET1_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_GMMU_RET1_EN_REG, 0xFFF);
    }
}

void hal_pmu1_dig_pg_l2m_gmmu_ret2_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_GMMU_RET2_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_GMMU_RET2_EN_REG, 0xFFF);
    }
}

bool hal_pmu1_dig_pg_l2m_gmmu_pwr_ack_get(void)
{
    hal_reg_status_check_timeout(PMU1_L2M_GMMU_PWR_ACK_REG, 0, 0, PMU1_OPERATION_TIMEOUT_MS);
    return reg16_getbit(PMU1_L2M_GMMU_PWR_ACK_REG, 0);
}

void hal_pmu1_dig_pg_l2m_dsp_pwr_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_DSP_PWR_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_DSP_PWR_EN_REG, PMU1_L2M_PWR_EN_MASK);
    }
}

void hal_pmu1_dig_pg_sub_l2m_dsp_pwr_en(hal_l2m_dsp_power_on_t pos, bool en)
{
    if (en) {
        reg16_setbit(PMU1_L2M_DSP_PWR_EN_REG, pos);
    } else {
        reg16_clrbit(PMU1_L2M_DSP_PWR_EN_REG, pos);
    }
}

void hal_pmu1_dig_pg_l2m_dsp_ret1_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_DSP_RET1_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_DSP_RET1_EN_REG, 0x3FF);
    }
}

void hal_pmu1_dig_pg_sub_l2m_dsp_ret1_en(hal_l2m_dsp_power_on_t pos, bool en)
{
    if (en) {
        reg16_setbit(PMU1_L2M_DSP_RET1_EN_REG, pos);
    } else {
        reg16_clrbit(PMU1_L2M_DSP_RET1_EN_REG, pos);
    }
}

void hal_pmu1_dig_pg_l2m_dsp_ret2_en(bool en)
{
    if (en) {
        reg16_write(PMU1_L2M_DSP_RET2_EN_REG, 0);
    } else {
        reg16_write(PMU1_L2M_DSP_RET2_EN_REG, 0x3FF);
    }
}

bool hal_pmu1_dig_pg_l2m_dsp_pwr_ack_get(void)
{
    hal_reg_status_check_timeout(PMU1_L2M_DSP_PWR_ACK_REG, 0, 0, PMU1_OPERATION_TIMEOUT_MS);
    return reg16_getbit(PMU1_L2M_DSP_PWR_ACK_REG, 0);
}

bool hal_pmu1_dig_pg_sub_l2m_dsp_pwr_ack_get(hal_l2m_dsp_power_on_t pos)
{
    return reg16_getbit(PMU1_L2M_DSP_PWR_ACK_REG, pos);
}

void hal_pmu1_dig_pg_xip_mem_pwr_en(bool en)
{
    if (en) {
        reg16_clrbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_PWR_EN_BIT);
    } else {
        reg16_setbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_PWR_EN_BIT);
    }
}

void hal_pmu1_dig_pg_xip_mem_retention_en(bool en)
{
    if (en) {
        reg16_clrbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_RET1_EN_BIT);
        reg16_clrbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_RET2_EN_BIT);
    } else {
        reg16_setbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_RET1_EN_BIT);
        reg16_setbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_RET2_EN_BIT);
    }
}

bool hal_pmu1_dig_pg_xip_mem_pwr_ack_get(void)
{
    hal_reg_status_check_timeout(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_PWR_RDY_BIT, 0, PMU1_XIP_PWR_ON_DELAY_US);
    return reg16_getbit(PMU1_XIP_MEM_PWR_REG, PMU1_XIP_MEM_PWR_RDY_BIT);
}

void hal_pmu1_dig_pg_bram_pwr_en(bool en)
{
    if (en) {
        reg16_write(PMU1_BMEM_PWR_MODE_REG, 0);
    } else {
        reg16_write(PMU1_BMEM_PWR_MODE_REG, PMU1_BMEM_PWR_MODE_EN_MASK);
    }
}

void hal_pmu1_dsp_l2ram_config(l2m_dsp_memory_share_mode_t mode)
{
    reg16_clrbit(MEM_SHARE_CTRL_GT_REG, DSP_SHARE_MODE_GT);
    reg16_setbits(MEM_SHARE_CTRL_REG, DSP_SHARE_MODE_BIT, DSP_SHARE_MODE_LEN, mode);
    reg16_setbit(MEM_SHARE_CTRL_GT_REG, DSP_SHARE_MODE_GT);
}

void hal_pmu_bt_config(bt_memory_share_mode_t mode)
{
    reg16_clrbit(BRAM_SHARE_MODE_REG, BRAM_SHARE_MODE_GT);
    reg16_setbits(BRAM_SHARE_MODE_REG, BRAM_SHARE_MODE_BIT, BRAM_SHARE_MODE_LEN, mode);
    reg16_setbit(BRAM_SHARE_MODE_REG, BRAM_SHARE_MODE_GT);
}