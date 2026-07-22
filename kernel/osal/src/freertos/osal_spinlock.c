/*
* Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
* Description:osal_spinlock
* Create: 2019-10-11
*/
#include <FreeRTOS.h>
#include <task.h>
#include <hwi.h>
#include "spinlock_adapt.h"
#include "soc_osal.h"
#include "osal_errno.h"
#include "osal_inner.h"

int osal_spin_lock_init(osal_spinlock *lock)
{
    RTOS_SpinInit((spinlock_t *)lock);
    return OSAL_SUCCESS;
}

void osal_spin_lock(osal_spinlock *lock)
{
    osal_unused(lock);
}

int osal_spin_trylock(osal_spinlock *lock)
{
    osal_unused(lock);
    return TRUE;
}

void osal_spin_unlock(osal_spinlock *lock)
{
    osal_unused(lock);
}

void osal_spin_lock_bh(osal_spinlock *lock)
{
    vTaskSuspendAll();
}

void osal_spin_unlock_bh(osal_spinlock *lock)
{
    xTaskResumeAll();
}


void osal_spin_lock_irqsave(osal_spinlock *lock, unsigned long *flags)
{
    if (flags == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    osal_unused(lock);
    *flags = (unsigned long)uxHwiLock();
}

void osal_spin_unlock_irqrestore(osal_spinlock *lock, unsigned long *flags)
{
    if (flags == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    osal_unused(lock);
    vHwiRestore(*flags);
}

void osal_spin_lock_destroy(osal_spinlock *lock)
{
    osal_unused(lock);
}
