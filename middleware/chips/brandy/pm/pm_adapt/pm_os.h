/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm os adapter header file
 */

#ifndef SRC_LIB_PM_OS_H
#define SRC_LIB_PM_OS_H

#include "chip_io.h"
#include "platform.h"

#if defined(__FREERTOS__)
#include "FreeRTOS.h"
#endif

#define PM_OS_MS_PER_TICKS   1

#if defined(__LITEOS__)
#define PM_OS_TICKS_PER_S    LOSCFG_BASE_CORE_TICK_PER_SECOND
#elif defined(__FREERTOS__)
#define PM_OS_TICKS_PER_S    configTICK_RATE_HZ
#else
#define PM_OS_TICKS_PER_S    1000
#endif

#define PM_US_PER_OS_TICKS   (1000000 / PM_OS_TICKS_PER_S)

typedef void (*lowpower_hook)(void);

uint32_t pm_os_get_sleep_ms(void);

void pm_os_compensate_sleep_ticks(uint32_t ticks);

void pm_os_register_hook(lowpower_hook cb);

uint32_t pm_os_irq_lock(void);

void pm_os_irq_restore(uint32_t irq_status);

void *pm_os_malloc(uint32_t size);

void pm_os_free(void *addr);

void pm_os_timer_enable(void);

void pm_os_timer_disable(void);
#endif
