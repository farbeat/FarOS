/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm sleep manage code
 */

#include "pm_sleep.h"
#include "pm.h"
#include "pm_definition.h"
#include "errcode.h"
#include "pm_os.h"
#include "pm_veto.h"
#include "pm_dev.h"
#include "pm_fsm.h"
#include "pm_lpc.h"
#include "pm_core.h"
#include "pm_clocks.h"
#include "hal_rtc.h"
#include "arch_encoding.h"
#include "arch_barrier.h"
#include "debug_print.h"
#include "pmu_interrupt.h"
#if CONFIG_PM_TICKLESS_EN == YES
#include "pm_tickless.h"
#endif
#include "non_os_reboot.h"

#define PM_SLEEP_THRESHOLD_WFI_ONLY_MS  10
#define PM_SLEEP_THRESHOLD_DEEPSLEEP_MS 20
#define PM_SLEEP_SEL_TRACE            0x5700041c
#define PM_SLEEP_TRACE_SEL_TAG(val)   (writew(PM_SLEEP_SEL_TRACE, val))
#define PM_SLEEP_SEL_EVENT            0x777
#define PM_SLEEP_TRACE_DEF_VAL        0x5a5a
#define PM_OS_US_TO_MS                  1000
#define PM_RTC_LOAD_COUNT_REG       0x57024000
#define PM_RTC_CUR_ALERT_US         64000000
#define PM_RTC_LOAD_THRESHOLD_MS   (CLOCK_32K_CALI_FREQ / 50)

enum {
    PM_SLEEP_STATE_IDLE,
    PM_SLEEP_STATE_GOTO_DS,
    PM_SLEEP_STATE_WKUP_PRE,
};

typedef enum {
    SLP_INFO_TIME_BASE = 0,
    SLP_INFO_LIGHTSLEEP,
    SLP_INFO_DEEPSLEEP,
    SLP_INFO_VETO,
    SLP_INFO_MAX,
} slp_record_info_t;

static sleep_info_t g_sleep_info = {0};

#if CONFIG_PM_POWER_GATING_EN == YES
static uint8_t g_pm_stop_state = 0;
#endif

static uint16_t g_pm_sleep_threshold = PM_SLEEP_THRESHOLD_DEEPSLEEP_MS;
static uint8_t g_pm_mcu_dynamic_power_flag = 0;

static void pm_enter_deepsleep_set_flag(void)
{
    writew(GLB_CTL_B_RB_BASE + 0x18, 0x5a5a);
}

static void pm_enter_deepsleep_cancel_flag(void)
{
    writew(GLB_CTL_B_RB_BASE + 0x18, 0xa5a5);
}

static void pm_sleep_info_record(slp_record_info_t info)
{
#if CONFIG_PM_SLEEP_RECORD_EN == YES
    if (info == SLP_INFO_TIME_BASE) {
        g_sleep_info.sleep_base_time = pm_core_get_current_ms();
    } else if (info == SLP_INFO_LIGHTSLEEP) {
        g_sleep_info.sleep_history[PM_SLEEP_LS].total_slp_count++;
        g_sleep_info.sleep_history[PM_SLEEP_LS].total_slp_time +=
            (pm_core_get_current_ms() - g_sleep_info.sleep_base_time);
    } else if (info == SLP_INFO_DEEPSLEEP) {
        g_sleep_info.sleep_history[PM_SLEEP_DS].total_slp_count++;
        g_sleep_info.sleep_history[PM_SLEEP_DS].total_slp_time +=
            (pm_core_get_current_ms() - g_sleep_info.sleep_base_time);
    } else if (info == SLP_INFO_VETO) {
        g_sleep_info.sleep_veto.last_veto_count = pm_veto_get_info()->veto_counts.total_counts;
        g_sleep_info.sleep_veto.last_veto_id = pm_veto_get_info()->last_veto_id;
    }
    g_sleep_info.event.slp_event = lpc_sleep_get_all_status(LPC_EVT_ACTION);
    g_sleep_info.event.wkup_event = lpc_wakeup_get_all_status(LPC_EVT_ACTION);
#else
    UNUSED(info);
#endif
}

static void pm_sleep_prepare(uint8_t sleep_type)
{
    pm_states_t state;
    if (sleep_type == PM_SLEEP_LS) {
        state = PM_STATE_LIGHT_SLEEP;
        lpc_lightsleep_config();
    } else if (sleep_type == PM_SLEEP_DS) {
        state = PM_STATE_DEEP_SLEEP;
        lpc_deepsleep_config();
    }

    for (pm_id_t id = 0; id < PM_ID_MAX; id++) {
        pm_process_fsm_handler(id, state);
    }
}

static void pm_wkup_prepare(uint8_t sleep_type)
{
    pm_states_t state;
    lpc_sleep_all_status_clear(LPC_EVT_ACTION);
    lpc_wakeup_all_status_clear(LPC_EVT_ACTION);

    if (sleep_type == PM_SLEEP_LS) {
        state = PM_STATE_WKUP_FROM_LIGHT_SLEEP;
    } else if (sleep_type == PM_SLEEP_DS) {
        state = PM_STATE_WKUP_FROM_DEEP_SLEEP;
    }

    for (pm_id_t id = 0; id < PM_ID_MAX; id++) {
        pm_process_fsm_handler(id, state);
    }
}

static inline void pm_enter_wfi(void)
{
    reg16_setbits(0x57004704, 0x1, 0x1, 0x0);
    while (reg16_getbits(0x57004708, 0x1, 0x1) != 0) {
        reg16_setbits(0x57004700, 0x1, 0x1, 0x1);
    }
    lpc_wfi();
}

static void pm_enter_light_sleep(void)
{
    pm_sleep_prepare(PM_SLEEP_LS);
    // 进入wfi
    pm_enter_wfi();
    pm_sleep_info_record(SLP_INFO_LIGHTSLEEP);
    pm_wkup_prepare(PM_SLEEP_LS);
}

static void pm_enter_lock_irq(void)
{
    clear_custom_csr(0x300, 0x3);
}

static void pm_enter_unlock_irq(void)
{
    set_custom_csr(0x300, 0x3);
}

static void pm_enter_enable_irq(uint32_t irq)
{
    reg8_write(CLIC_BASE_REG + sys_clic_intie(irq), 0x1);
}

static void pm_enter_disable_irq(uint32_t irq)
{
    reg8_write(CLIC_BASE_REG + sys_clic_intie(irq), 0x0);
}

void pm_enter_deep_sleep_wfi(void)
{
    wfi();
    pm_enter_lock_irq();
    // wait soc aon low power ready
    while (reg16_getbits(0x52000D00, 0x0, 0x4) != 0) {
    }
    lowpower_cpu_resume();
}

void pm_wkup_event_by_ulp_callback(void)
{
    set_cpu_utils_reset_cause(REBOOT_CAUSE_APPLICATION_SYSRESETREQ);
    hal_reboot_chip();
}

void pm_enter_deep_sleep_only_ulp_wakeup(void)
{
    reg16_clrbit(0x520003e4, 0x3); // close mdam clock in deepsleep
    if (pm_dev_suspend() != ERRCODE_SUCC) {
        pm_enter_wfi();
        pm_sleep_info_record(SLP_INFO_LIGHTSLEEP);
        return;
    }
    g_pm_stop_state = PM_SLEEP_STATE_GOTO_DS;
    pm_sleep_prepare(PM_SLEEP_DS);
    pm_core_suspend_min();

    PM_SLEEP_TRACE_SEL_TAG(PM_SLEEP_TRACE_DEF_VAL);
    if (g_pm_stop_state == PM_SLEEP_STATE_GOTO_DS) {
        g_pm_stop_state = PM_SLEEP_STATE_WKUP_PRE;
        pm_core_close_all_irq();
        pm_enter_enable_irq(M_WAKEUP_IRQN);
        lpc_allow_deepsleep();
        pm_enter_unlock_irq();
        if (pmu_mcpu_allow_to_sleep_status() == TURN_ON) {
            dsb();
            pm_enter_deepsleep_cancel_flag();
            writew(0x57004684, 0x2000); // only open ulp wakeup event
            writew(0x57004728, 0x0);
            writew(0x57004694, 0x2000); // only open ulp wakeup interrupt
            writew(0x5702c1f0, 0x8); // only open ulp wakeup powerkey interrupt
            pm_enter_deep_sleep_wfi();
        }
        pm_enter_lock_irq();
    } else if (g_pm_stop_state == PM_SLEEP_STATE_WKUP_PRE) {
        PM_SLEEP_TRACE_SEL_TAG(PM_SLEEP_SEL_EVENT);
        pm_enter_disable_irq(M_WAKEUP_IRQN);
        lpc_forbid_deepsleep();
        pm_core_resume_min();
        pm_dev_resume();
    }
    uint16_t status = pmu_lpm_wakeup_get_status(LPM_EVT_ACTION, PMU_LPM_MCPU_ULP_WAKEUP);
    pm_wkup_prepare(PM_SLEEP_DS);
    pm_sleep_info_record(SLP_INFO_DEEPSLEEP);
    reg16_setbit(0x520003e4, 0x3);  // open mdam clock after deepsleep
    if (status != 0) {
        pm_wkup_event_by_ulp_callback();
    }
}

void pm_wkup_event_by_uart_callback(void)
{
    uapi_pm_add_sleep_veto(PM_ID_DEBUG);
}

static void pm_enter_deep_sleep(void)
{
    reg16_clrbit(0x520003e4, 0x3); // close mdam clock in deepsleep
#if CONFIG_PM_POWER_GATING_EN == YES
    if (pm_dev_suspend() != ERRCODE_SUCC) {
        pm_enter_wfi();
        pm_sleep_info_record(SLP_INFO_LIGHTSLEEP);
        return;
    }
    g_pm_stop_state = PM_SLEEP_STATE_GOTO_DS;
    pm_sleep_prepare(PM_SLEEP_DS);
    pm_core_suspend();
    PM_SLEEP_TRACE_SEL_TAG(PM_SLEEP_TRACE_DEF_VAL);
    if (g_pm_stop_state == PM_SLEEP_STATE_GOTO_DS) {
        g_pm_stop_state = PM_SLEEP_STATE_WKUP_PRE;
        pm_core_close_all_irq();
        pm_enter_enable_irq(M_WAKEUP_IRQN);
        lpc_allow_deepsleep();
        pm_enter_unlock_irq();
        if (pmu_mcpu_allow_to_sleep_status() == TURN_ON) {
            dsb();
            pm_enter_deepsleep_cancel_flag();
            pm_enter_deep_sleep_wfi();
        }
        pm_enter_lock_irq();
    } else if (g_pm_stop_state == PM_SLEEP_STATE_WKUP_PRE) {
        pm_sleep_info_record(SLP_INFO_DEEPSLEEP);
        PM_SLEEP_TRACE_SEL_TAG(PM_SLEEP_SEL_EVENT);
        pm_enter_disable_irq(M_WAKEUP_IRQN);
        lpc_forbid_deepsleep();
        pm_core_resume();
        pm_dev_resume();
    }
#else
    pm_sleep_prepare(PM_SLEEP_DS);
    lpc_allow_deepsleep();
    pm_enter_wfi();
    lpc_forbid_deepsleep();
#endif
#ifndef FT_SINGLE_UART
    uint16_t status = pmu_lpm_wakeup_get_status(LPM_EVT_ACTION, PMU_LPM_MCPU_UART_H1_RX_WAKEUP);
#else
    uint16_t status = pmu_lpm_wakeup_get_status(LPM_EVT_ACTION, PMU_LPM_MCPU_UART_H0_RX_WAKEUP);
#endif
    if (status != 0) {
        pm_wkup_event_by_uart_callback();
    }
    pm_wkup_prepare(PM_SLEEP_DS);
    reg16_setbit(0x520003e4, 0x3);  // open mdam clock after deepsleep
}

sleep_info_t *pm_get_sleep_info(void)
{
    return &g_sleep_info;
}

void pm_enter_sleep(uint32_t sleep_ms_val)
{
    uint32_t status;
    uint32_t sleep_ms = sleep_ms_val;
    uint32_t rtc_times = 0;
    uint32_t rtc_times_alert = 0;
    if ((g_pm_mcu_dynamic_power_flag == 1) || hal_ipc_get_core_clr_sts(CORES_BT_CORE)) {
        return;
    }
    if (sleep_ms == 0) {
        return;
    }
    uapi_rtc_get_current_time_us(0, &rtc_times);
    if (rtc_times > PM_RTC_CUR_ALERT_US) {
        rtc_times_alert = rtc_times;
        rtc_times = rtc_porting_cycle_2_us(readl(PM_RTC_LOAD_COUNT_REG));
    }
    if (rtc_times < PM_OS_US_TO_MS && rtc_times > 0) {
        return;
    }
    rtc_times /= PM_OS_US_TO_MS;
    if (sleep_ms > rtc_times && rtc_times != 0) {
        sleep_ms = rtc_times;
    }
    if (sleep_ms <= PM_SLEEP_THRESHOLD_WFI_ONLY_MS || pm_get_sleep_veto()) {
        status = pm_os_irq_lock();
        pm_sleep_info_record(SLP_INFO_TIME_BASE);
        pm_enter_wfi();
        pm_sleep_info_record(SLP_INFO_LIGHTSLEEP);
        pm_os_irq_restore(status);
        return;
    }

    pm_sleep_info_record(SLP_INFO_TIME_BASE);
    pm_sleep_info_record(SLP_INFO_VETO);

    pm_enter_deepsleep_set_flag();
    status = pm_os_irq_lock();
    if ((sleep_ms <= g_pm_sleep_threshold) || pm_get_sleep_veto() || hal_rtc_get_int_status(0) == 1 ||
        ((readl(PM_RTC_LOAD_COUNT_REG) < PM_RTC_LOAD_THRESHOLD_MS) && (rtc_times_alert > PM_RTC_CUR_ALERT_US))) {
        pm_enter_deepsleep_cancel_flag();
        pm_enter_light_sleep();
    } else {
#if CONFIG_PM_TICKLESS_EN == YES
        pm_tickless_start(sleep_ms);
#endif
        pm_enter_deep_sleep();
#if CONFIG_PM_TICKLESS_EN == YES
        pm_tickless_stop(sleep_ms);
#endif
    }
    pm_os_irq_restore(status);
    pm_enter_deepsleep_cancel_flag();
}

void pm_set_sleep_threshold(uint16_t ms)
{
    if (ms < PM_SLEEP_THRESHOLD_WFI_ONLY_MS) {
        return ;
    }
    g_pm_sleep_threshold = ms;
}

void pm_set_mcu_dynamic_power_flag(uint8_t flag)
{
    g_pm_mcu_dynamic_power_flag = flag;
}

void uapi_pm_enter_sleep(void)
{
    uint32_t sleep_ms = pm_os_get_sleep_ms();
    pm_enter_sleep(sleep_ms);
}
