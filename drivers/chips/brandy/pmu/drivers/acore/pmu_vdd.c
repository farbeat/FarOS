/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:   APPLICATION PMU DRIVER
 */

#include "pmu_vdd.h"
#include "hal_pmu_vdd.h"
#include "std_def.h"

uint8_t g_pmu_0p8_vget = 0;

typedef struct {
    volatile uint32_t lock;
    volatile uint32_t req_closed;
} pmu_vdd1p0_lock_core_t;

typedef struct {
    volatile uint32_t system_lock_turn;
    volatile pmu_vdd1p0_lock_core_t m_req;
    volatile pmu_vdd1p0_lock_core_t s_req;
} pmu_vdd1p0_lock_t;

/*
 *  Private variable declarations
 */
static volatile pmu_vdd1p0_lock_t g_pmu_vdd1p0_st __attribute__((section(".sys_status")));

void pmu_vdd1p0_status_init(void)
{
    g_pmu_vdd1p0_st.m_req.lock = 0;
    g_pmu_vdd1p0_st.m_req.req_closed = 0;
    g_pmu_vdd1p0_st.s_req.lock = 0;
    g_pmu_vdd1p0_st.s_req.req_closed = 0;
}

static void pmu_vdd1p0_status_enter_critical(void)
{
    g_pmu_vdd1p0_st.m_req.lock = 1;
    g_pmu_vdd1p0_st.system_lock_turn = 1;
    while (g_pmu_vdd1p0_st.s_req.lock == 1 && g_pmu_vdd1p0_st.system_lock_turn == 1) {}
}

static void pmu_vdd1p0_status_exit_critical(void)
{
    g_pmu_vdd1p0_st.m_req.lock = 0;
}

void pmu_vdd1p0_request_switch(switch_type_t en)
{
    uint32_t irq = osal_irq_lock();
    pmu_vdd1p0_status_enter_critical();
    if (en == TURN_ON) {
        if (g_pmu_vdd1p0_st.s_req.req_closed != 0) {
            hal_pmu_vdd_enable(HAL_PMU_VDD1P0, TURN_ON);
        }
        g_pmu_vdd1p0_st.m_req.req_closed = 0;
    } else {
        if (g_pmu_vdd1p0_st.s_req.req_closed != 0) {
            hal_pmu_vdd_enable(HAL_PMU_VDD1P0, TURN_OFF);
        }
        g_pmu_vdd1p0_st.m_req.req_closed = 1;
    }
    pmu_vdd1p0_status_exit_critical();
    osal_irq_restore(irq);
}

void pmu_vdd0p8_power_vget(uint8_t *level)
{
    *level = g_pmu_0p8_vget;
}

void pmu_vdd0p8_power_vset(uint8_t level)
{
    /* 1. Set vset step, default: 0.25mv
     * 2. Config cldo vset nor value
     * 3. requset vset config
     * 4. wait ack, clr ack
     */
    g_pmu_0p8_vget = level;
    hal_pmu_vdd0p8_set_nor_voltage(level);
    hal_pmu_vdd0p8_vset_requset(PMU_VSET_MCORE);
    (void)hal_pmu_vdd0p8_vset_ack_get(PMU_VSET_MCORE);
    hal_pmu_vdd0p8_vset_ack_clr(PMU_VSET_MCORE);
}

void pmu_vdd0p8_sleep_power_vset(uint8_t level)
{
    hal_pmu_vdd0p8_set_slp_voltage(level);
}

void pmu_bcore_vdd0p8_power_vset(uint8_t level)
{
    hal_pmu_bcore_vdd0p8_set_nor_voltage(level);
    hal_pmu_vdd0p8_vset_requset(PMU_VSET_BCORE);
    (void)hal_pmu_vdd0p8_vset_ack_get(PMU_VSET_BCORE);
    hal_pmu_vdd0p8_vset_ack_clr(PMU_VSET_BCORE);
}