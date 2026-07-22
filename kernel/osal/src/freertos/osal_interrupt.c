/*
* Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
* Description:osal_interrupt
* Author: AuthorNameMagicTag
* Create: 2021-09-11
*/

#include <FreeRTOS.h>
#include <hwi.h>
#include "soc_osal.h"
#include "osal_inner.h"
#include "barrier.h"

void *osal_irq_get_private_dev(void *param_dev)
{
    return param_dev;
}

int osal_irq_request(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev)
{
    if (handler == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    BaseType_t ret = xHwiCreate(irq, configHWI_PRIO_IGNORE, (HwiFunction_t)handler, dev);
    if (ret != pdPASS) {
        osal_log("xHwiCreate failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }

    return OSAL_SUCCESS;
}

void osal_irq_free(unsigned int irq, void *dev)
{
    BaseType_t ret = xHwiDelete(irq);
    if (ret != pdPASS) {
        osal_log("xHwiDelete failed! ret = %#x.\n", ret);
    }
}

void osal_irq_enable(unsigned int irq)
{
    BaseType_t ret = xHwiEnable(irq);
    if (ret != pdPASS) {
        osal_log("xHwiEnable failed! ret = %#x.\n", ret);
    }
    mb();
}

void osal_irq_disable(unsigned int irq)
{
    BaseType_t ret = xHwiDisable(irq);
    if (ret != pdPASS) {
        osal_log("xHwiDisable failed! ret = %#x.\n", ret);
    }
    mb();
}

unsigned int osal_irq_clear(unsigned int vector)
{
    xHwiClear(vector);
    mb();
    return OSAL_SUCCESS;
}

unsigned int osal_irq_unlock(void)
{
    unsigned int ret = uxHwiUnlock();
    mb();
    return ret;
}

unsigned int osal_irq_lock(void)
{
    unsigned int ret = uxHwiLock();
    mb();
    return ret;
}

void osal_irq_restore(unsigned int irq_status)
{
    vHwiRestore(irq_status);
    mb();
}

int osal_irq_set_priority(unsigned int irq, unsigned short priority)
{
    UBaseType_t ret = xHwiSetPriority(irq, priority);
    if (ret != pdPASS) {
        osal_log("xHwiSetPriority failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_in_interrupt(void)
{
    int id = xHwiGetInterruptId();
    if (id == configHWI_INVALID_IRQ_ID) {
        return FALSE;
    }
    return TRUE;
}
