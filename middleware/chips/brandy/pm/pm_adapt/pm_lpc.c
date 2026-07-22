/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm lpc adapter code
 */

#include "pm_lpc.h"
#include "pm_bsp.h"
#include "pm_core.h"
#include "chip_io.h"
#include "core.h"
#include "lpc.h"
#include "pmu_interrupt.h"
#include "ulp_aon_control.h"
#include "hal_lpc.h"
#if CORE == MASTER_BY_ALL
#include "hal_pmu_pg.h"
#include "hal_pmu_ldo.h"
#endif

#define LPC_ULP_WKUP_EN                 (ULP_AON_CTL_RB_ADDR + 0x1f0)
#define LPC_ULP_PWRON_WKUP_EN_BIT       3
#define LPC_ULP_CAPSENS_WKUP_EN_BIT     2
#define LPC_ULP_GPIO_WKUP_EN_BIT        1
#define LPC_ULP_AON_WKUP_WKUP_EN_BIT    0
#define LPC_ULP_SLP_EN                  (ULP_AON_CTL_RB_ADDR + 0x1d0)
#define LPC_ULP_SLP_INT_EN              (ULP_AON_CTL_RB_ADDR + 0x1d4)
#define LPC_VBUS_SENSE_DIS              (ULP_AON_CTL_RB_ADDR + 0x38)

static void lpc_power_auto_config(void)
{
#if CORE == MASTER_BY_ALL
    hal_pmu1_dig_pg_auto_pwr_en(PMU1_DIG_AUTO_PG_MCU, TURN_ON);
    hal_pmu1_dig_pg_auto_pwr_en(PMU1_DIG_AUTO_PG_MEM, TURN_ON);
    hal_pmu1_dig_pg_auto_pwr_en(PMU1_DIG_AUTO_PG_COM, TURN_ON);

#else
    hal_pmu1_dig_pg_auto_pwr_en(PMU1_DIG_AUTO_PG_BSOC, TURN_OFF);
    hal_pmu1_dig_pg_auto_pwr_en(PMU1_DIG_AUTO_PG_BMEM, TURN_OFF);

    hal_pmu_cldo_vset_auto_cfg(HAL_PMU_CLDO2, TURN_OFF);
#endif
}

static void lpc_aon_wakeup_int_and_event_enable(switch_type_t wakeup_en)
{
    uint16_t ulp_aon_wkup_en_config = BIT(LPC_ULP_CAPSENS_WKUP_EN_BIT) | \
                                    BIT(LPC_ULP_GPIO_WKUP_EN_BIT) | \
                                    BIT(LPC_ULP_PWRON_WKUP_EN_BIT) | \
                                    BIT(LPC_ULP_AON_WKUP_WKUP_EN_BIT);
    if (wakeup_en != 0) {
        // ULP AON wakeup enable
        writew(LPC_ULP_WKUP_EN, ulp_aon_wkup_en_config);
    } else {
        // ULP AON wakeup mask
        writew(LPC_ULP_WKUP_EN, 0);
    }
}

static void lpc_before_enter_deep_sleep_handle(void)
{
    pmu_lpm_before_enter_wfi_handle();

    /* enable all sleep events,for can be enter sleep after WFI. */
    pmu_lpm_sleep_action_all_enable(LPM_EVT_ACTION, TURN_ON);
    /* enable all wakeup events,for can be wakeup. */
    pmu_lpm_wakeup_action_all_enable(LPM_EVT_ACTION, TURN_ON);
#if CORE == MASTER_BY_ALL
    hal_pmu_lpm_audio_wakeup_enable(LPM_EVT_ACTION, TURN_ON);
#endif

#if CONFIG_PM_SLEEP_INT_EN == YES
    /* enable all sleep int, for can be enter sleep after WFI. */
    pmu_lpm_sleep_action_all_enable(LPM_INT_ACTION, TURN_ON);
#endif
    /* enable all wakeup int, for can be wakeup. */
    pmu_lpm_wakeup_action_all_enable(LPM_INT_ACTION, TURN_ON);
    /* enable ulp aon wk int and event */
    lpc_aon_wakeup_int_and_event_enable(TURN_ON);
    /* clear core sleep records. */
    lpc_system_sleep_history_clear();
}

void lpc_lightsleep_config(void)
{
    pmu_lpm_before_enter_wfi_handle();
}

void lpc_deepsleep_config(void)
{
    lpc_before_enter_deep_sleep_handle();
    ulp_aon_before_enter_wfi_handle();
}

void lpc_wfi(void)
{
    hal_lpc_enter_wfi();
}

void lpc_allow_deepsleep(void)
{
#if CORE == MASTER_BY_ALL
    pmu_mcpu_allow_to_sleep(TURN_ON);
#else
    pmu_bcpu_allow_to_sleep(TURN_ON);
#endif
}

void lpc_forbid_deepsleep(void)
{
#if CORE == MASTER_BY_ALL
    pmu_mcpu_allow_to_sleep(TURN_OFF);
#else
    pmu_bcpu_allow_to_sleep(TURN_OFF);
#endif
}

bool lpc_system_sleep_history(void)
{
#if CORE == MASTER_BY_ALL
    return hal_pmu_lpm_msystem_sleep_history();
#else
    return hal_pmu_bsystem_sleep_history();
#endif
}

void lpc_system_sleep_history_clear(void)
{
#if CORE == MASTER_BY_ALL
    hal_pmu_lpm_msystem_sleep_history_clear();
#else
    hal_pmu_bsystem_sleep_history_clear();
#endif
}

void lpc_wakeup_all_status_clear(lpc_action_type_t type)
{
    hal_pmu_lpm_wakeup_all_status_clear(type);
}

uint16_t lpc_wakeup_get_all_status(lpc_action_type_t type)
{
    return hal_pmu_lpm_wakeup_get_all_status(type);
}

void lpc_sleep_all_status_clear(lpc_action_type_t type)
{
    hal_pmu_lpm_sleep_all_status_clear(type);
}

uint16_t lpc_sleep_get_all_status(lpc_action_type_t type)
{
    return hal_pmu_lpm_sleep_get_all_status(type);
}

void lpc_aon_sleep_prepare(void)
{
    reg16_setbit(LPC_ULP_SLP_EN, 0x0);
    reg16_setbit(LPC_ULP_SLP_INT_EN, 0x0);
    reg16_clrbit(LPC_VBUS_SENSE_DIS, 0x0);
}

void uapi_lpc_init(void)
{
    lpc_init();
    lpc_power_auto_config();
}