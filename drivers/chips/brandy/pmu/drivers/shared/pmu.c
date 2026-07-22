/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:  PMU DRIVER
 */

#include "core.h"
#include "non_os.h"
#include "pmu.h"
#if CORE == MASTER_BY_ALL
#include "hal_pmu_peripheral_app.h"
#ifndef BUILD_APPLICATION_SSB
#include "otp.h"
#include "otp_map.h"
#include "pm_definition.h"
#include "pm_bsp.h"
#include "pm_control.h"
#endif
#include "pmu_pg.h"
#include "pmu_cmu.h"
#include "pmu_ldo.h"
#include "ulp_aon_control.h"
#if defined(BUILD_APPLICATION_STANDARD)
#include "hal_clocks_app.h"
#include "ulp_aon_control.h"
#include "hal_pmu_vdd.h"
#endif
#include "hal_clocks_sdio.h"
#include "hal_clocks_glb.h"
#include "clocks_core.h"
#include "hal_pmu_pg.h"
#endif
#include "hal_pmu_ldo.h"
#include "hal_cpu_clocks.h"
#include "pmu_interrupt.h"
#include "debug_print.h"
#include "tcxo.h"
#include "pinctrl_porting.h"
#include "hal_reg_config.h"
#include "panic.h"
#include "clock_calibration.h"
#if defined(BUILD_APPLICATION_STANDARD)
#include "log_common.h"
#include "log_def_pf.h"
#endif
#include "hal_pmu_ldo.h"

#define PMU_LPM_SYS_SLP_HISTORY_REG             (PMU1_CTL_RB_BASE + 0x88)
#define LPM_SYS_WAKING_CNT_CFG                  (PMU1_CTL_RB_BASE + 0x770)
#define LPM_SYS_WAKING_1D_CNT_CFG               (PMU1_CTL_RB_BASE + 0x774)
#define LPM_SYS_WAKING_2D_CNT_CFG               (PMU1_CTL_RB_BASE + 0x778)
#define LPM_SYS_WAKING_3D_CNT_CFG               (PMU1_CTL_RB_BASE + 0x77C)
#define LPM_SYS_WAKE_CNT_CFG                    (PMU1_CTL_RB_BASE + 0x780)
#define LPM_SYS_WAKING_CNT_VAL                  0x9  // LPM_SYS_WAKING_CNT_CFG  270us
#define LPM_SYS_WAKING_1D_CNT_VAL               0x12 // LPM_SYS_WAKING_1D_CNT_CFG  270us
#define LPM_SYS_WAKING_2D_CNT_VAL               0x3F // LPM_SYS_WAKING_2D_CNT_CFG  1350us
#define LPM_SYS_WAKING_3D_CNT_VAL               0x48 // LPM_SYS_WAKING_3D_CNT_CFG  270us
#define LPM_SYS_WAKE_CNT_VAL                    0x51 // LPM_SYS_WAKE_CNT_CFG  270us
/* SYSTEM_HISTORY BIT position */
#define PMU_SYS_HISTORY_CLR  1
#define PMU_SYS_HISTORY      0

#define TCXO_DELAY_150_US   150ULL
#define TCXO_DELAY_30_US    30ULL
#define CLK_DIV_2           2U
#define POS_TWELVE          12
#define POS_EIGHT           8

#if CORE == MASTER_BY_ALL
void pmu_power_on_init(void)
{
    PRINT("pmu_power_on_init begin\r\n");
    pmu_ldo_power_on(PMU_LDO_AUXLDO0, TURN_OFF);
    hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO0, TURN_ON);
    pmu_ldo_power_on(PMU_LDO_AUXLDO1, TURN_ON); // poweron for flash component
    pmu_ldo_power_on(PMU_LDO_AUXLDO2, TURN_ON); // poweron for ioldo
#ifdef QSPI_DISPLAY // qspi will close auxldo3
    pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_OFF);
    hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO3, TURN_ON);
#else // auxldo3 supply for mipi_phy
    pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_ON);
#endif
#ifndef BUILD_APPLICATION_SSB
    uapi_pm_control(PM_SDIO_POWER_UP_AND_RUN);
#endif
    PRINT("pmu_power_on_init end\r\n");
}

void pmu_display_power_and_reset_cfg(bool turn_on)
{
    /* Reset the display module. */
    hal_clocks_glb_display_soft_reset_control(HAL_SOFT_RST4_DISPLAY_CRG, HAL_SOFT_RST_RESET);
    hal_clocks_glb_display_soft_reset_control(HAL_SOFT_RST4_DISPLAY_LGC, HAL_SOFT_RST_RESET);
    if (turn_on) {
        // Auxldo3 poweron(mipi)
        pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_ON);
        /* Power on the display module. */
        pmu1_dig_pg_sub_power_on((pmu_pg_sub_t)PMU1_DIG_PG_DSUB, TURN_ON);
        /* Dereset the display module. */
        hal_clocks_glb_display_soft_reset_control(HAL_SOFT_RST4_DISPLAY_CRG, HAL_SOFT_RST_DERESET);
        uapi_tcxo_delay_us(TCXO_DELAY_30_US);
        hal_clocks_glb_display_soft_reset_control(HAL_SOFT_RST4_DISPLAY_LGC, HAL_SOFT_RST_DERESET);
    } else {
        pmu1_dig_pg_sub_power_on((pmu_pg_sub_t)PMU1_DIG_PG_DSUB, TURN_OFF);

        // Auxldo3 poweroff(mipi)
        pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_ON);
    }
}
#endif

#define PMU_REG_ACK_TIME 1000ULL
void pmu_reg_ack_timeout(uint32_t addr, uint16_t offset, bool on)
{
    uint64_t time = uapi_tcxo_get_ms();
    bool tmp = (bool)reg16_getbit((void *)((uintptr_t)addr), POS(offset));

    while (tmp != on) {
        if ((time + PMU_REG_ACK_TIME) < uapi_tcxo_get_ms()) {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_ack is checking, addr:%x, offset:%x", addr, offset);
#else
            PRINT("Reg_ack is checking, addr:%x, offset:%x\r\n", addr, offset);
#endif
        } else {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_ack has temed out, addr:%x, offset:%x", addr, offset);
#else
            PRINT("Reg_ack has temed out, addr:%x, offset:%x\r\n", addr, offset);
#endif
            break;
        }
        tmp = (bool)reg16_getbit((void *)((uintptr_t)addr), POS(offset));
    }
}

void pmu_reg_sts_timeout(uint32_t addr, uint16_t value)
{
    uint64_t time = uapi_tcxo_get_ms();
    uint16_t tmp = readw(addr);

    while (tmp != value) {
        if ((time + PMU_REG_ACK_TIME) < uapi_tcxo_get_ms()) {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_status is checking, addr:%x, cur_value:%x", addr, tmp);
#else
            PRINT("Reg_status is checking, addr:%x, cur_value:%x\r\n", addr, tmp);
#endif
        } else {
#if defined(BUILD_APPLICATION_STANDARD)
            oml_pf_log_print2(LOG_BCORE_PLT_DRIVER_PMU, LOG_NUM_DRIVER_PMU, LOG_LEVEL_INFO, \
                              "Reg_status has temed out, addr:%x, cur_value:%x", addr, tmp);
#else
            PRINT("Reg_status has temed out, addr:%x, cur_value:%x\r\n", addr, tmp);
#endif
            break;
        }
        tmp = readw(addr);
    }
}

#ifdef BUILD_APPLICATION_SSB
void pmu_init(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_PMU) == true) {
        return;
    }

#if CHIP_ASIC
    hal_pmu_simo_buck_init();
    pmu1_dig_pg_sub_power_on(PMU_PG_SUB_XIP_MEM, TURN_ON);
    pmu1_dig_pg_sub_power_on(PMU_PG_SUB_L2M_GMMU, TURN_ON);
    pmu1_dig_pg_sub_power_on(PMU_PG_SUB_L2M_DSP, TURN_ON);
    pmu_cmu_init();
#endif
    non_os_set_driver_initalised(DRIVER_INIT_PMU, true);
}
#else /* IF BUILD_APPLICATION_STANDARD */
static void pmu_init_misc(void)
{
    /* Init pmu interrupts and events source */
    pmu_lpm_interrupts_and_events_init();
    /* Init pmu interrupts, sleep and wakeup interrupts are included. */
    pmu_init_interrupts();
    // mem retetion en, l2m reretion配成了硬控
    writel(0x57004010, 0x700);
    writew(0x57000810, 0x3ff);
    writew(0x57000814, 0x3ff);
    writew(0x5700403c, 0x0);
    writew(0x5700404c, 0x0);
    for (uint8_t i = 0; i < DSP_L2RAM_AREA_SIZE; i++) { // 关闭dsp所使用l2ram部分的硬控，retention和电源
        reg16_clrbit(0x57000810, i);
        reg16_clrbit(0x57000814, i);
        reg16_setbit(0x5700404c, i);
    }
}

void pmu_init(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_PMU) == true) {
        return;
    }
    writew(0x5702c1d0, 0x0); // 恢复ulp寄存器ULP_SLP_EN
    writew(0x5702C038, 0x0); // 恢复ulp寄存器VBUS_SENSE_DIS
#if CORE == MASTER_BY_ALL
#if CHIP_ASIC
    pmu_power_on_init();
    hal_pmu_mgpio_low_power_config(TURN_ON);
#endif
#endif
    pmu_init_misc();
    // perp32k
    hal_clocks_aperp_32k_sel_control(TURN_OFF);

    pmu_ldo_power_on(PMU_LDO_XLDO, TURN_ON);
    pmu_ldo_power_on(PMU_LDO_INTLDO1, TURN_ON);

    // clk_32k
    ulp_aon_clk_32k_sel(ULP_CLK_32_FROM_PMU_RC_32K);

    // xo_32k_sel
    hal_aon_sub_rc_tcxo_clock_switch(HAL_CLOCK_AON_TCXO, 1, 32, 32); // use the chip default freq div 32
    hal_clocks_rcxo_32k_clk_sel(TURN_ON); // use xo 32k clock
    // vdd0p8 - sleep - 0.6v  nor --- 0825v
    uapi_pmu_vset(PMU_ID_SIMO_VDD0P8, PMU_VDD0P8_1P0_0P950V); // in fact vdd is 0.9v
    pmu_vdd0p8_sleep_power_vset(PMU_VDD0P8_1P0_0P600V);
    uapi_pmu_vset(PMU_ID_SIMO_VDD0P8_BCORE, PMU_VDD0P8_1P0_0P850V); // in fact vdd is 0.825v
    // disable pmu dbg
    hal_pmu_dbg_select(TURN_OFF);

    // cmu glb normal
    hal_cmu_nor_control(CMU_GLB_PD, TURN_OFF);

    // disable cmu dbg
    hal_cmu_dbg_select(TURN_OFF);

    // rc2 cfg sel
    hal_pmu_rc2_control_sel();

    // wait 2.5ms before wakeup
    writew(LPM_SYS_WAKING_CNT_CFG, LPM_SYS_WAKING_CNT_VAL);       // LPM_SYS_WAKING_CNT_CFG  270us
    writew(LPM_SYS_WAKING_1D_CNT_CFG, LPM_SYS_WAKING_1D_CNT_VAL); // LPM_SYS_WAKING_1D_CNT_CFG  270us
    writew(LPM_SYS_WAKING_2D_CNT_CFG, LPM_SYS_WAKING_2D_CNT_VAL); // LPM_SYS_WAKING_2D_CNT_CFG  1350us
    writew(LPM_SYS_WAKING_3D_CNT_CFG, LPM_SYS_WAKING_3D_CNT_VAL); // LPM_SYS_WAKING_3D_CNT_CFG  270us
    writew(LPM_SYS_WAKE_CNT_CFG, LPM_SYS_WAKE_CNT_VAL);           // LPM_SYS_WAKE_CNT_CFG  270us

    // slp bypass
    hal_pmu_slp_cfg_vld_sel();

    // simo eco sel hw
    hal_pmu_simo1_set();

    // cldo1 cldo3 关闭，配hiz防止漏电
    pmu_ldo_bypass_en(PMU_LDO_CLDO1, TURN_ON);
    pmu_ldo_bypass_en(PMU_LDO_CLDO3, TURN_ON);
    pmu_ldo_power_on(PMU_LDO_CLDO1, TURN_OFF);
    pmu_ldo_power_on(PMU_LDO_CLDO3, TURN_OFF);

    pmu_vdd1p0_status_init();

    non_os_set_driver_initalised(DRIVER_INIT_PMU, true);
}
#endif /* BUILD_APPLICATION_STANDARD END */

void pmu_lpm_interrupts_and_events_init(void)
{
#if CORE == MASTER_BY_ALL
    hal_pmu_reset_manual_config(TURN_OFF);
    pmu_lpm_sdio_evt_enable_init();
    pmu_lpm_ulp_gpio_evt_enable_init();
#else
    pmu_lpm_osc_evt_enable_init();
#endif

    /* Disable sleep and wakeup events and interrupts. */
    pmu_lpm_wakeup_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_wakeup_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_OFF);
    pmu_lpm_sleep_action_all_enable(LPM_EVT_ACTION, TURN_OFF);
    pmu_lpm_reg_config_delay();

    /* Clear sleep and wakeup events and interrupts status. */
    pmu_lpm_wakeup_all_status_clear(LPM_EVT_ACTION);
    pmu_lpm_wakeup_all_status_clear(LPM_INT_ACTION);
    pmu_lpm_sleep_all_status_clear(LPM_EVT_ACTION);
    pmu_lpm_sleep_all_status_clear(LPM_INT_ACTION);

    pmu_lpm_reg_config_delay();
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_WAKEUP, LPM_EVT_ACTION);
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_WAKEUP, LPM_INT_ACTION);
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_SLEEP, LPM_EVT_ACTION);
    pmu_lpm_make_sure_all_status_clear(LPM_TYPE_SLEEP, LPM_INT_ACTION);
#ifndef CONFIG_LOW_POWER_TEST
    pmu_cmu_interrupt_config();
#endif
#if CORE == MASTER_BY_ALL
    hal_pmu_reset_manual_select(TURN_OFF);
#else
    /* When BT is powered on for the first time, it will trigger osc_en wakeup interrupt if osc_en_wakeup is enabled.
       However, bt_thread is not running, so osc_en_wakeup should be ignored. */
    pmu_lpm_osc_status_clear();
    pmu_lpm_osc_interrupt_enable_init();
#endif
}

bool pmu_system_sleep_history(void)
{
    return reg16_getbit(PMU_LPM_SYS_SLP_HISTORY_REG, POS(PMU_SYS_HISTORY));
}

void pmu_system_sleep_history_clear(void)
{
    reg16_setbit(PMU_LPM_SYS_SLP_HISTORY_REG, POS(PMU_SYS_HISTORY_CLR));
}

void pmu_reset_manual_select(void)
{
#if (CORE == MASTER_BY_ALL)
    hal_pmu_reset_manual_config(TURN_OFF);
    hal_pmu_reset_manual_select(TURN_OFF);
#endif
}

#if (CORE == MASTER_BY_ALL)
void pmu_sdio_glb_common_reset(bool turn_on)
{
    if (turn_on) {
        hal_clocks_glb_sdio_soft_reset_control(HAL_SOFT_RST6_SDIO_CRG, HAL_SOFT_RST_DERESET);
        hal_clocks_glb_sdio_soft_reset_control(HAL_SOFT_RST6_SDIO_LGC, HAL_SOFT_RST_DERESET);
        hal_clocks_sdio_crg_rst_control((switch_type_t)HAL_SOFT_RST_DERESET);
    } else {
        hal_clocks_glb_sdio_soft_reset_control(HAL_SOFT_RST6_SDIO_CRG, HAL_SOFT_RST_RESET);
        hal_clocks_glb_sdio_soft_reset_control(HAL_SOFT_RST6_SDIO_LGC, HAL_SOFT_RST_RESET);
        hal_clocks_sdio_crg_rst_control((switch_type_t)HAL_SOFT_RST_RESET);
    }
}
#endif
