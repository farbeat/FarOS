/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: workqueue
 * Create: 2021-12-16
 */

#include <FreeRTOS.h>
#include <timers.h>
#include "hwi.h"
#include "soc_osal.h"
#include "osal_inner.h"

int osal_workqueue_init(osal_workqueue *work, osal_workqueue_handler handler)
{
    if (work == NULL || work->handler != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    work->handler = handler;

    return OSAL_SUCCESS;
}

int osal_workqueue_schedule(osal_workqueue *work)
{
    if (work == NULL || work->handler == NULL) {
        osal_log("parameter invalid!\n");
        return FALSE;
    }
    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        BaseType_t ret = xTimerPendFunctionCall((PendedFunction_t)work->handler, work, 0, 0);
        if (ret != pdPASS) {
            return FALSE;
        }
        return TRUE;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t result = xTimerPendFunctionCallFromISR((PendedFunction_t)work->handler, work, 0,
        &xHigherPriorityTaskWoken);
    if (result != pdPASS) {
        return FALSE;
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return TRUE;
}

void osal_workqueue_destroy(osal_workqueue *work)
{
    if (work == NULL || work->handler == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    work->handler = NULL;
}