/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm dev operations code
 */

#include "pm.h"
#include "pm_sleep.h"
#include "pm_veto.h"
#include "pm_dev.h"
#include "pm_fsm.h"
#include "clocks_core.h"
#include "chip_io.h"
#include "rtc.h"
#include "watchdog.h"
#include "watchdog_porting.h"
#include "securec.h"
#include "pmu_interrupt.h"

typedef struct {
    uint32_t           slp_ms;
    uint32_t           handle;
    rtc_callback_t cb;
} pm_rtc_timer_t;

typedef struct {
    pm_dev_id_t dev_id;
    pm_dev_ops_t dev_ops;
} pm_dev_t;

#if CORE == MASTER_BY_ALL
static clocks_clk_cfg_t g_cpu_cfg;
#endif

#define PM_SHIPMODE_DELAY_SLEEP_MS   100

// ostimer
static uint32_t pm_dev_ostimer_ioctl(uint8_t type, uint8_t *data, uint32_t len)
{
    UNUSED(data);
    UNUSED(len);
    switch (type) {
        case DEV_OSTIMER_ENABLE:
            pm_os_timer_enable();
            break;
        case DEV_OSTIMER_DISABLE:
            pm_os_timer_disable();
            break;
        default:
            break;
    }
    return 0;
}

// rtc
static uint32_t rtc_start_wkup_timer(uint8_t *data, uint32_t len)
{
    if (len != sizeof(pm_rtc_timer_t)) {
        return (uint32_t)ERRCODE_PM_ERROR;
    }

    pm_rtc_timer_t *timer = (pm_rtc_timer_t *)data;
    uint32_t wakeup_time = timer->slp_ms;
    rtc_handle_t *handle = (rtc_handle_t)(timer->handle);
    if (handle == NULL) {
        return (uint32_t)ERRCODE_PM_ERROR;
    }
    if (*handle == NULL) {
        uapi_rtc_create(RTC_0, handle);
        uapi_rtc_start(*handle, wakeup_time, (rtc_callback_t)timer->cb, 0);
    } else {
        uapi_rtc_start(*handle, wakeup_time, (rtc_callback_t)timer->cb, 0);
    }
    return (uint32_t)ERRCODE_SUCC;
}

static uint32_t pm_dev_rtc_ioctl(uint8_t type, uint8_t *data, uint32_t len)
{
    UNUSED(data);
    UNUSED(len);
    switch (type) {
        case DEV_RTC_START:
            return rtc_start_wkup_timer(data, len);
        default:
            break;
    }
    return 0;
}

static uint32_t pm_dev_wdt_ioctl(uint8_t type, uint8_t *data, uint32_t len)
{
    UNUSED(data);
    UNUSED(len);
    switch (type) {
        case DEV_WDT_ENABLE:
            watchdog_turnon_clk();
            uapi_watchdog_set_time(watchdog_get_conifgtime);
            uapi_watchdog_enable(WDT_MODE_INTERRUPT);
            break;
        case DEV_WDT_DISABLE:
            watchdog_turnon_clk();
            uapi_watchdog_disable();
            watchdog_turnoff_clk();
            break;
        default:
            break;
    }
    return 0;
}

static void pm_dev_register_sleep_ops(void)
{
    pm_dev_t all_devs[] = PM_ALL_DEV;

    for (uint8_t i = 0; i < sizeof(all_devs) / sizeof(pm_dev_t); i++) {
        uapi_pm_register_dev_ops(all_devs[i].dev_id, &(all_devs[i].dev_ops));
    }
}

static uint32_t core_crg_switch(uint8_t *data, uint32_t len)
{
    UNUSED(data);
    UNUSED(len);
#if CORE == MASTER_BY_ALL
    if (data == NULL) {
        return 0;
    }

    if (data[0] == PM_STATE_DEEP_SLEEP) {
        // todo
    } else if (data[0] == PM_STATE_WKUP_FROM_DEEP_SLEEP) {
        // todo
    }
#endif
    return 0;
}

static void pm_fsm_register_sleep_handler(void)
{
    uint8_t state = PM_STATE_DEEP_SLEEP;
    pm_fsm_content_t content = {&state, 1, core_crg_switch};
    uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_DEEP_SLEEP, &content);

    state = PM_STATE_WKUP_FROM_DEEP_SLEEP;
    uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_WKUP_FROM_DEEP_SLEEP, &content);
}

void uapi_pm_init(void)
{
    pm_veto_init();
    pm_dev_register_sleep_ops();
    pm_fsm_register_sleep_handler();
#if ENABLE_LOW_POWER == YES
#ifdef __LITEOS__
    LOS_LowpowerHookReg(uapi_pm_enter_sleep);
#endif
    uapi_pm_add_sleep_veto(PM_ID_DEBUG);
#endif
}

void uapi_sys_force_mcu_only_deep_sleep_mode(uint32_t arg)
{
    UNUSED(arg);
    uint32_t irq = osal_irq_lock();

    reg16_clrbit(0x57000168, 0x2); // close watchdog clock
    pm_enter_deep_sleep_only_ulp_wakeup();
    osal_irq_restore(irq);
}

void uapi_sys_shipmode(uint32_t arg)
{
    UNUSED(arg);
    uint32_t irq = osal_irq_lock();
    uapi_pm_control(PM_BT_POWER_OFF);
    uapi_pm_control(PM_DSP_POWER_OFF);
    uapi_pm_control(PM_DISPLAY_POWER_OFF);

    /* clr ulp reg0 */
    writew(0x5702c010, 0);

    /* clr ulp int and event */
    writel(0x5702C1E4, 0x3F);
    writel(0x5702C1EC, 0x3F);
    uapi_tcxo_delay_us(PM_SHIPMODE_DELAY_SLEEP_MS);

    reg16_clrbit(0x5702c508, POS_14);
    writew(0x5700450c, 0x100);

    // gpio
    writew(0x5702C200, 0x0);
    writew(0x5702C204, 0x0);
    writew(0x5702C208, 0x0);
    writew(0x5702C20c, 0x0);
    writew(0x5702C210, 0x0);

    writew(0x5702c200, 0x7);
    writew(0x57030004, 0x0);
    writew(0x57030038, 0x1);
    writew(0x5703003c, 0x1);
    writew(0x57030034, 0x0);
    writew(0x57030030, 0x1);

    writew(0x570040d8, 0xc000);
    writew(0x5702c028, 0x0);
    writew(0x5702c024, 0x3);
    writew(0x5702c1d0, 0x1);
    writew(0x5702C038, 0x0);
    writew(0x5702C1F0, 0x1C);
    writew(0x5702C110, 0x101);
    writew(0x570040d0, 0x1);
    writew(0x570040e0, 0x1);
    osal_irq_restore(irq);
}
