/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: OAL OS
 * Author:
 * Create: 2021-05-24
 */
#include "non_os.h"
#include "oal_interface.h"
#if (CORE != WIFI) && !defined(BUILD_APPLICATION_ROM) && !defined(CONTROL_CORE)
#include "irmalloc.h"
#endif
#include "panic.h"
#include "osal_interrupt.h"
#include "arch_barrier.h"
#include "soc_osal.h"
#include "interrupt.h"

typedef void (*oal_int_func)(void);

uint32_t oal_int_create(uint32_t int_num, uint32_t int_prio, oal_int_func func, uint32_t param)
{
    /* software may want to use those unused interrupt,so here we don't check the irq id */
    UNUSED(param);
    if (func == NULL) {
        panic(PANIC_INT, __LINE__);
        return OSAL_FAILURE;
    }
    int_set_priority((int32_t)int_num, int_prio);
    (void)int_set_irq_func((int32_t)int_num, func);
    int_enable_irq((int32_t)int_num);
    return OSAL_SUCCESS;
}

uint32_t oal_int_delete(uint32_t int_num)
{
    int_clear_pending_irq((int32_t)int_num);
    int_disable_irq((int32_t)int_num);
    return OSAL_SUCCESS;
}

void oal_int_enable_all(void)
{
    non_os_exit_critical();
}

void oal_int_disable_all(void)
{
    non_os_enter_critical();
}

void osal_irq_restore(unsigned int irq_status)
{
    UNUSED(irq_status);
    non_os_exit_critical();
}

unsigned int osal_irq_lock(void)
{
    non_os_enter_critical();
    return OSAL_SUCCESS;
}
unsigned int osal_irq_unlock(void)
{
    non_os_exit_critical();
    return OSAL_SUCCESS;
}

void *oal_mem_alloc(uint32_t size)
{
#if (CORE != WIFI) && !defined(BUILD_APPLICATION_ROM) && !defined(CONTROL_CORE)
    return irmalloc(size);
#else
    UNUSED(size);
    return NULL;
#endif
}

int osal_irq_request(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev)
{
    UNUSED(thread_fn);
    UNUSED(name);
    UNUSED(dev);
    if (handler == NULL) {
        panic(PANIC_INT, __LINE__);
        return 1;
    }
    (void)int_set_irq_func((int32_t)irq, (isr_function)handler);

    return OSAL_SUCCESS;
}

int osal_irq_set_priority(unsigned int irq, unsigned short priority)
{
    int_set_priority((int32_t)irq, priority);
    return OSAL_SUCCESS;
}

unsigned int osal_irq_clear(unsigned int vector)
{
    int_clear_pending_irq((int32_t)vector);
    return OSAL_SUCCESS;
}

void osal_irq_enable(unsigned int irq)
{
    int_enable_irq((int32_t)irq);
}

void osal_irq_disable(unsigned int irq)
{
    oal_int_delete(irq);
}

void osal_irq_free(unsigned int irq, void *dev)
{
    UNUSED(dev);
    UNUSED(irq);
}

int osal_in_interrupt(void)
{
    return int_is_interrupt_context() ? 1 : 0;
}

void osal_dsb(void)
{
    dsb();
}

void osal_isb(void)
{
    isb();
}