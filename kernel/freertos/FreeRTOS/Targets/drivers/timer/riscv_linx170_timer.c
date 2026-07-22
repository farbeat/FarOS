/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#include "FreeRTOS.h"

#include "arch_base.h"
#include "hal_freertos_timer.h"
#include "asm/register_config.h"
#include "asm/interrupt_config.h"
#include "task.h"
#include "hal_hwi.h"
#include "print.h"
#include "encoding.h"
#include "systick.h"
#include "clocks_config.h"
#include "riscv_linx170_timer.h"

#define MACHINE_TIMER_ENABLE_USRMODE    0x3

static void vTickIRQHandler(BaseType_t xId, void *vParam)
{
    BaseType_t value = 0;
    WRITE_UINT8(0, CLIC_BASE_REG + sys_clic_intip(xId));
    READ_UINT32(value, TIMER_TICK_REG_BASE + TIMER_INT_CLR);

    if (xTaskIncrementTick() != 0) {
        vTaskSwitchContext();
    }

    (void)vParam;
}

static void vTimerTickEnable(void)
{
    uint32_t val = GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* timer enabled [bit 7 set as 1] */
    val |= 1U << TIMER_ENABLE_BIT;
    WRITE_UINT32(val, TIMER_TICK_REG_BASE + TIMER_CONTROL);
}

void vTimerTickStart(void)
{
    /* clock start */
    xHwiEnable(OS_TICK_INT_NUM);
    vTimerTickEnable();
}

void vTimerTickInit(void)
{
    /* disable first */
    WRITE_UINT32(0x0, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* set init value as period */
    WRITE_UINT32(configTICK_CLOCK_HZ / configTICK_RATE_HZ, TIMER_TICK_REG_BASE + TIMER_LOAD);

    /*
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * interrupt enabled [bit 5 set as 1]
     * timer mode periodic [bit 6 set as 1]
     */
    WRITE_UINT32(MACHINE_TIMER_ENABLE_USRMODE, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    BaseType_t xRet = xHwiCreate(OS_TICK_INT_NUM, configHWI_PRIO_LOWEST, vTickIRQHandler, NULL);
    if (xRet != pdPASS) {
        PRINTK(ePrintkErr, "TimerTickInit failed\n");
        return;
    }
}

#ifdef SUPPORT_RC_CALIBRATION
volatile uint64_t g_pre_count = 0;
volatile double g_pre_msec = 0;
#define MILLISECONDS_PER_SECOND 1000
#endif

uint64_t ullTickGetCycles(void)
{
#ifdef SUPPORT_RC_CALIBRATION
    volatile double tick;
#else
    uint64_t tick;
#endif
    uint64_t cycle;
    uint32_t ulTickClockPeriod = configTICK_CLOCK_HZ / configTICK_RATE_HZ;
#ifdef SUPPORT_RC_CALIBRATION
    volatile double msec;
    volatile uint64_t now_count;
#endif

    UBaseType_t saveIrq = portSET_INTERRUPT_MASK_FROM_ISR();

#ifdef SUPPORT_RC_CALIBRATION
    if (g_pre_count == 0) {
        tick = (uapi_systick_get_count() * MILLISECONDS_PER_SECOND) / CLOCK_32K_CALI_FREQ;
        g_pre_count = uapi_systick_get_count();
        g_pre_msec = tick;
    } else {
        now_count = uapi_systick_get_count();
        msec = (double)((now_count - g_pre_count) * MILLISECONDS_PER_SECOND) / CLOCK_32K_CALI_FREQ;
        tick = g_pre_msec + msec;

        g_pre_count = now_count;
        g_pre_msec = tick;
    }
#else
    tick = uapi_systick_get_ms();
#endif
    READ_UINT32(ulTickClockPeriod, TIMER_TICK_REG_BASE + TIMER_LOAD);
    cycle = (uint64_t)(tick * ulTickClockPeriod);

    portCLEAR_INTERRUPT_MASK_FROM_ISR(saveIrq);
    return cycle;
}

void vTimerEnable(void)
{
    /* set init value as period */
    WRITE_UINT32(configTICK_CLOCK_HZ / configTICK_RATE_HZ, TIMER_TICK_REG_BASE + TIMER_LOAD);
    WRITE_UINT32(MACHINE_TIMER_ENABLE_USRMODE, TIMER_TICK_REG_BASE + TIMER_CONTROL);
}

void vTimerDisable(void)
{
    WRITE_UINT32(0x0, TIMER_TICK_REG_BASE + TIMER_CONTROL);
}