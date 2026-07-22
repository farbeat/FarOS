/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  PMU DRIVER Source
 */

#include "pmu_pg.h"
#include <stdint.h>
#include "soc_osal.h"
#include "std_def.h"
#include "tcxo.h"

#define PMU1_WAIT_POWER_EN_TIME_US          30ULL

static void pmu1_dig_pg_l2m_gmmu_power_on(switch_type_t power_on)
{
    if (power_on == TURN_ON) {
        hal_pmu1_dig_pg_l2m_gmmu_pwr_en(true);
        (void)hal_pmu1_dig_pg_l2m_gmmu_pwr_ack_get();
        hal_pmu1_dig_pg_l2m_gmmu_ret1_en(false);
    } else {
        hal_pmu1_dig_pg_l2m_gmmu_ret1_en(true);
        hal_pmu1_dig_pg_l2m_gmmu_pwr_en(false);
    }
}

static void pmu1_dig_pg_l2m_dsp_power_on(switch_type_t power_on)
{
    if (power_on == TURN_ON) {
        hal_pmu1_dig_pg_l2m_dsp_pwr_en(true);
        (void)hal_pmu1_dig_pg_l2m_dsp_pwr_ack_get();
        hal_pmu1_dig_pg_l2m_dsp_ret1_en(false);
    } else {
        hal_pmu1_dig_pg_l2m_dsp_ret1_en(true);
        hal_pmu1_dig_pg_l2m_dsp_pwr_en(false);
    }
}

static void pmu1_dig_pg_xip_mem_power_on(switch_type_t power_on)
{
    if (power_on == TURN_ON) {
        hal_pmu1_dig_pg_xip_mem_pwr_en(true);
        (void)hal_pmu1_dig_pg_xip_mem_pwr_ack_get();
        hal_pmu1_dig_pg_xip_mem_retention_en(false);
    } else {
        hal_pmu1_dig_pg_xip_mem_retention_en(true);
        hal_pmu1_dig_pg_xip_mem_pwr_en(false);
    }
}

void pmu1_dig_pg_sub_power_on(pmu_pg_sub_t sub, switch_type_t power_on)
{
    if (sub <= PMU_PG_SUB_COM) {
        if (power_on == TURN_ON) {
            hal_pmu1_dig_pg_common_iso_en((pmu1_dig_pg_sub_sys_t)sub, true);
            hal_pmu1_dig_pg_common_pwr_en((pmu1_dig_pg_sub_sys_t)sub, true);
            (void)hal_pmu1_dig_pg_common_pwr_ack_get((pmu1_dig_pg_sub_sys_t)sub);
            hal_pmu1_dig_pg_common_iso_en((pmu1_dig_pg_sub_sys_t)sub, false);
        } else {
            hal_pmu1_dig_pg_common_iso_en((pmu1_dig_pg_sub_sys_t)sub, true);
            hal_pmu1_dig_pg_common_pwr_en((pmu1_dig_pg_sub_sys_t)sub, false);
        }
    }

    if (sub == PMU_PG_SUB_L2M_GMMU) {
        pmu1_dig_pg_l2m_gmmu_power_on(power_on);
    } else if (sub == PMU_PG_SUB_L2M_DSP) {
        pmu1_dig_pg_l2m_dsp_power_on(power_on);
    } else if (sub == PMU_PG_SUB_XIP_MEM) {
        pmu1_dig_pg_xip_mem_power_on(power_on);
    } else if (sub == PMU_PG_SUB_BMEM_RAM) {
        hal_pmu1_dig_pg_bram_pwr_en(power_on);
    }
}

void pmu1_dsp_l2ram_config(uint8_t size)
{
    l2m_dsp_memory_share_mode_t mode = (l2m_dsp_memory_share_mode_t)size;
    if (mode >= L2MEM_SHARE_MAX_FOR_DSP) {
        return;
    }
    uint32_t irq = osal_irq_lock();
    hal_pmu1_dsp_l2ram_config(mode);
    osal_irq_restore(irq);
}

void pmu_bt_ram_config(uint8_t size)
{
    bt_memory_share_mode_t mode = (bt_memory_share_mode_t)size;
    if (mode >= BRAM_SHARE_MAX_FOR_BT_DIAG) {
        return;
    }
    uint32_t irq = osal_irq_lock();
    hal_pmu_bt_config(mode);
    osal_irq_restore(irq);
}