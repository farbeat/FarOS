/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: osal atomic source file.
 */

#include <FreeRTOS.h>
#include <atomic.h>
#include "soc_osal.h"
#include "osal_inner.h"

int osal_atomic_read(osal_atomic *atomic)
{
    int ret;
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    ATOMIC_ENTER_CRITICAL();
    {
        ret = atomic->counter;
    }
    ATOMIC_EXIT_CRITICAL();
    return ret;
}

void osal_atomic_set(osal_atomic *atomic, int i)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    ATOMIC_ENTER_CRITICAL();
    {
        atomic->counter = i;
    }
    ATOMIC_EXIT_CRITICAL();
}

int osal_atomic_inc_return(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    (void)Atomic_Increment_u32((volatile uint32_t*)(&atomic->counter));
    return atomic->counter;
}

int osal_atomic_dec_return(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    (void)Atomic_Decrement_u32((volatile uint32_t*)(&atomic->counter));
    return atomic->counter;
}

void osal_atomic_inc(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    (void)Atomic_Increment_u32((volatile uint32_t*)(&atomic->counter));
}

void osal_atomic_dec(osal_atomic *atomic)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    (void)Atomic_Decrement_u32((volatile uint32_t*)(&atomic->counter));
}

void osal_atomic_add(osal_atomic *atomic, int count)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    (void)Atomic_Add_u32((volatile uint32_t*)(&atomic->counter), (uint32_t)count);
}

int osal_atomic_add_return(osal_atomic *atomic, int count)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    (void)Atomic_Add_u32((volatile uint32_t*)(&atomic->counter), (unsigned int)count);
    return atomic->counter;
}

void osal_atomic_sub(osal_atomic *atomic, unsigned int count)
{
    if (atomic == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    (void)Atomic_Subtract_u32((volatile uint32_t*)(&atomic->counter), count);
}

int osal_atomic_dec_and_test(osal_atomic *atomic)
{
    return osal_atomic_dec_return(atomic) == 0;
}
