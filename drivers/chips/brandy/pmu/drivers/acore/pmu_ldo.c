/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:    APPLICATION PMU LDO DRIVER.
 */

#include "pmu_ldo.h"
#include "non_os.h"
#include "hal_pmu_ldo.h"
#include "hal_pmu_vdd.h"
#include "clocks_core.h"
#include "panic.h"
#include "tcxo.h"

#define PMU_EN_XLDO_DELAY_TIME              2
#define PMU_XLDO_ICTR_CONFIG                0x2

// Systime wakeup time config -- need check on asic
static int16_t g_lpc_wake_up_wait_time_config[HAL_PMU_WKUP_TIME_MAX] = {
    0,  // PMU_EN_UVLO_WKUP_TIME
    0,  // PMU_EN_OVP_WKUP_TIME
    0,  // PMU_EN_REF_BG_WKUP_TIME
    0,  // PMU_EN_REFBUFFER_WKUP_TIME
    0,  // PMU_EN_INTLDO2_WKUP_TIME
    0,  // PMU_EN_IBG2_WKUP_TIME
    0,  // PMU_EN_IPOLY_WKUP_TIME
    0,  // PMU_EN_ITUNE_WKUP_TIME
    0,  // PMU_EN_XLDO_WKUP_TIME
    0,  // PMU_XO_CORE_PD_WKUP_TIME
    0,  // PMU_RC_PD_WKUP_TIME
    0,  // PMU_RC2_DBB_PD_WKUP_TIME
    0,  // PMU_EN_CLDO1_WKUP_TIME
    0,  // PMU_EN_CLDO1_ISO_WKUP_TIME
    0,  // PMU_EN_CLDO3_WKUP_TIME
    0,  // PMU_RST_AON_CRG_WKUP_TIME
    0, // PMU_GLB_CLK_FORCE_ON_WKUP_TIME
    0,  // PMU_RST_AON_LGC_WKUP_TIME
    0,  // PMU_RST_PWR_C1_CRG_N_WKUP_TIME
    0,  // PMU_RST_PWR_C1_LGC_N_WKUP_TIME
    0,  // PMU_RST_PWR_C1_CPU_N_WKUP_TIME
    0,  // PMU_TCXO2ANA_PD_WKUP_TIME
};

static void pmu_cldo_vset_man_control(uint8_t id, uint8_t level)
{
    /* 1. Set vset step, default: 0.25mv
     * 2. Config cldo vset nor value
     * 3. requset vset config
     * 4. wait ack, clr ack
     */
    hal_pmu_cldo_t cldo = (hal_pmu_cldo_t)id;
    hal_pmu_cldo_set_voltage(cldo, level);
    hal_pmu_cldo_vset_requset(cldo);
    (void)hal_pmu_cldo_vset_ack_get(cldo);
    hal_pmu_cldo_vset_ack_clr(cldo);
}

// Need update
void pmu_wakeup_manual_config(void)
{
    // PMU_MAN_1
    hal_pmu_manual_config_control(EN_IBG1, TURN_ON);

    hal_pmu_manual_select_control(EN_IBG1, TURN_ON);

    // PMU_MAN_2
    hal_ldo_manual_config_control(LDO_C1_TO_SYS_ISO_EN, TURN_OFF);

    hal_ldo_manual_select_control(LDO_C1_TO_SYS_ISO_EN, TURN_ON);
}

void pmu_wakeup_wait_time_init(void)
{
    pmu_wakeup_manual_config();

    hal_pmu_xldo_set_voltage(HAL_PMU_XLDO_LEVEL_1P7V);
    hal_pmu_xldo_set_ictr(PMU_XLDO_ICTR_CONFIG);

    for (uint8_t count = 0; count < HAL_PMU_WKUP_TIME_MAX; count++) {
        hal_pmu_system_wakeup_time_set((hal_pmu_system_wakeup_time_type_t)count,
                                       g_lpc_wake_up_wait_time_config[count]);
    }

    hal_pmu_en_xldo_delay_time_config(PMU_EN_XLDO_DELAY_TIME);
}

void pmu_ldo_power_on(pmu_ldo_t sub, switch_type_t power_on)
{
    switch (sub) {
        case PMU_LDO_CLDO1:
            hal_pmu_cldo1_enable(power_on);
            break;
        case PMU_LDO_CLDO2:
            hal_pmu_cldo2_enable(power_on);
            break;
        case PMU_LDO_CLDO3:
            hal_pmu_cldo3_enable(power_on);
            break;
        case PMU_LDO_XLDO:
            hal_pmu_xldo_enable(power_on);
            break;
        case PMU_LDO_CMU_LDO:
            hal_pmu_cmu_ldo_enable(power_on);
            break;
        case PMU_LDO_INTLDO1:
            hal_pmu_intldo1_enable(power_on);
            break;
        case PMU_LDO_INTLDO2:
            hal_pmu_intldo2_enable(power_on);
            break;
        case PMU_LDO_AUXLDO0:
        case PMU_LDO_AUXLDO1:
        case PMU_LDO_AUXLDO2:
        case PMU_LDO_AUXLDO3:
            hal_pmu_auxldo_enable(sub - PMU_LDO_AUXLDO0, (bool)power_on);
            break;
        case PMU_LDO_IOLDO:
            hal_pmu_ioldo_enable(power_on);
            break;
        default:
            break;
    }
}

void pmu_ldo_power_vset(pmu_ldo_t sub, uint8_t level)
{
    switch (sub) {
        case PMU_LDO_CLDO1:
        case PMU_LDO_CLDO2:
        case PMU_LDO_CLDO3:
            pmu_cldo_vset_man_control(sub - PMU_LDO_CLDO1, level);
            break;
        case PMU_LDO_XLDO:
            hal_pmu_intldo1_set_voltage(level);
            break;
        case PMU_LDO_CMU_LDO:
            hal_pmu_cmu_ldo_set_voltage(level);
            break;
        case PMU_LDO_INTLDO1:
            hal_pmu_intldo1_set_voltage(level);
            break;
        case PMU_LDO_INTLDO2:
            hal_pmu_intldo2_set_voltage(level);
            break;
        case PMU_LDO_AUXLDO0:
        case PMU_LDO_AUXLDO1:
        case PMU_LDO_AUXLDO2:
        case PMU_LDO_AUXLDO3:
            hal_pmu_auxldo_set_voltage(sub - PMU_LDO_AUXLDO0, level);
            break;
        case PMU_LDO_IOLDO:
            hal_pmu_ioldo_set_voltage(level);
            break;
        default:
            break;
    }
}

void pmu_buck_vset(pmu_simo_t sub, uint8_t level)
{
    switch (sub) {
        case PMU_SIMO_VDD0P8:
            pmu_vdd0p8_power_vset(level);
            break;
        case PMU_SIMO_VDD1P9:
            hal_pmu_vdd1p9_set_voltage(level);
            break;
        case PMU_SIMO_VDD1P0:
            hal_pmu_vdd1p0_set_voltage(level);
            break;
        case PMU_SIMO_VDD1P3:
            hal_pmu_vdd1p3_set_voltage(level);
            break;
        case PMU_SIMO_VDD0P8_BCORE:
            pmu_bcore_vdd0p8_power_vset(level);
            break;
        default:
            break;
    }
}

void pmu_ldo_bypass_en(pmu_ldo_t sub, switch_type_t bypass_en)
{
    switch (sub) {
        case PMU_LDO_CLDO1:
        case PMU_LDO_CLDO2:
        case PMU_LDO_CLDO3:
            hal_pmu_cldo_enable_bypass(sub - PMU_LDO_CLDO1, bypass_en);
            break;
        case PMU_LDO_XLDO:
            hal_pmu_xldo_enable_bypass(bypass_en);
            break;
        case PMU_LDO_CMU_LDO:
            break;
        case PMU_LDO_INTLDO1:
            hal_pmu_intldo1_enable_bypass(bypass_en);
            break;
        case PMU_LDO_INTLDO2:
            hal_pmu_intldo2_enable_bypass(bypass_en);
            break;
        case PMU_LDO_AUXLDO0:
        case PMU_LDO_AUXLDO1:
        case PMU_LDO_AUXLDO2:
        case PMU_LDO_AUXLDO3:
            hal_pmu_auxldo_enable_bypass(sub - PMU_LDO_AUXLDO0, bypass_en);
            break;
        case PMU_LDO_IOLDO:
            hal_pmu_ioldo_enable_bypass(bypass_en);
            break;
        default:
            break;
    }
}

void pmu_control_cldo1_sleep_bypass(switch_type_t power_on)
{
    if (power_on == TURN_ON) {
        hal_pmu_cldo_enable_slp_bypass(HAL_PMU_CLDO1, TURN_ON);
    } else {
        hal_pmu_cldo_enable_slp_bypass(HAL_PMU_CLDO1, TURN_OFF);
    }
}