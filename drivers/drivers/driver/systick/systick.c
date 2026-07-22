/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides systick driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-01, Create file. \n
 */
#include <stdbool.h>
#include "securec.h"
#include "interrupt/osal_interrupt.h"

#include "systick_porting.h"
#include "hal_systick.h"
#include "systick.h"

static bool g_systick_hw_inited = false;

static hal_systick_funcs_t *g_hal_funcs = NULL;

void uapi_systick_init(void)
{
    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(g_systick_hw_inited)) {
        osal_irq_restore(irq_sts);
        return;
    }

    systick_port_register_hal_funcs();
    g_hal_funcs = hal_systick_get_funcs();
    g_hal_funcs->init();
    g_systick_hw_inited = true;
    osal_irq_restore(irq_sts);
}

void uapi_systick_deinit(void)
{
    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(!g_systick_hw_inited)) {
        osal_irq_restore(irq_sts);
        return;
    }

    g_hal_funcs->deinit();
    systick_port_unregister_hal_funcs();
    g_systick_hw_inited = false;
    osal_irq_restore(irq_sts);
}

errcode_t uapi_systick_count_clear(void)
{
    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(!g_systick_hw_inited)) {
        osal_irq_restore(irq_sts);
        return ERRCODE_SYSTICK_NOT_INIT;
    }

    errcode_t ret = ERRCODE_FAIL;

    g_hal_funcs->ctrl(SYSTICK_CTRL_COUNT_CLEAR, 1);
    ret = g_hal_funcs->ctrl(SYSTICK_CTRL_COUNT_CLEARED, 0);
    osal_irq_restore(irq_sts);

    return ret;
}

uint64_t uapi_systick_get_count(void)
{
    uint64_t count = 0;

    uint32_t irq_sts = osal_irq_lock();
    if (unlikely(!g_systick_hw_inited)) {
        osal_irq_restore(irq_sts);
        return count;
    }

    g_hal_funcs->ctrl(SYSTICK_CTRL_COUNT_GET, (uintptr_t)&count);
    osal_irq_restore(irq_sts);

    return count;
}

uint64_t uapi_systick_get_s(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    return uapi_systick_get_time(SYSTICK_TIME_S);
#else
    return convert_count_2_s(uapi_systick_get_count());
#endif
}

uint64_t uapi_systick_get_ms(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    return uapi_systick_get_time(SYSTICK_TIME_MS);
#else
    return convert_count_2_ms(uapi_systick_get_count());
#endif
}

uint64_t uapi_systick_get_us(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    return uapi_systick_get_time(SYSTICK_TIME_US);
#else
    return convert_count_2_us(uapi_systick_get_count());
#endif
}

errcode_t uapi_systick_delay_count(uint64_t c_delay)
{
    if (unlikely(!g_systick_hw_inited)) {
        return ERRCODE_SYSTICK_NOT_INIT;
    }

    uint64_t current_ticks;
    uint64_t origin_ticks;

    origin_ticks = uapi_systick_get_count();
    for (;;) {
        current_ticks = uapi_systick_get_count();
        if ((current_ticks - origin_ticks) >= c_delay) {
            break;
        }
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_systick_delay_s(uint32_t s_delay)
{
    if (unlikely(!g_systick_hw_inited)) {
        return ERRCODE_SYSTICK_NOT_INIT;
    }

    uapi_systick_delay_count((uint64_t)convert_s_2_count((uint64_t)s_delay));

    return ERRCODE_SUCC;
}

errcode_t uapi_systick_delay_ms(uint32_t m_delay)
{
    if (unlikely(!g_systick_hw_inited)) {
        return ERRCODE_SYSTICK_NOT_INIT;
    }

    uapi_systick_delay_count((uint64_t)convert_ms_2_count((uint64_t)m_delay));

    return ERRCODE_SUCC;
}

errcode_t uapi_systick_delay_us(uint32_t u_delay)
{
    if (unlikely(!g_systick_hw_inited)) {
        return ERRCODE_SYSTICK_NOT_INIT;
    }

    uapi_systick_delay_count((uint64_t)convert_us_2_count((uint64_t)u_delay));
    return ERRCODE_SUCC;
}