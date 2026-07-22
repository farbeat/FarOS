/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: wait
 * Author: AuthorNameMagicTag
 * Create: 2021-03-08
 */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "hwi.h"
#include "poll_adapt.h"
#include "soc_osal.h"
#include "osal_inner.h"

int osal_wait_init(osal_wait *wait)
{
    if (wait == NULL || wait->wait != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    wait_queue_head_t *wq = (wait_queue_head_t *)pvPortMalloc(sizeof(wait_queue_head_t));
    if (wq == NULL) {
        osal_log("pvPortMalloc error!\n");
        return OSAL_FAILURE;
    }
    EventGroupHandle_t handler = xEventGroupCreate();
    if (handler == NULL) {
        osal_log("xEventGroupCreate failed!\n");
        vPortFree(wq);
        return OSAL_FAILURE;
    }
    wq->stEvent.handler = handler;
    RTOS_SpinInit(&wq->lock);
    RTOS_ListInit(&wq->poll_queue);
    wait->wait = wq;
    return OSAL_SUCCESS;
}

int osal_wait_interruptible(osal_wait *wait, osal_wait_condition_func func, const void *param)
{
    // not support interruptible wait in liteos
    return osal_wait_uninterruptible(wait, func, param);
}

int osal_wait_uninterruptible(osal_wait *wait, osal_wait_condition_func func, const void *param)
{
    if (wait == NULL || wait->wait == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    wait_queue_head_t *wq = (wait_queue_head_t *)(wait->wait);
    EventGroupHandle_t handler = wq->stEvent.handler;
    while (func != NULL && !func(param)) {
        (void)xEventGroupWaitBits(handler, 0x1U,
            OSAL_WAITMODE_CLR,  // need clear after read?
            OSAL_WAITMODE_AND,  // if true return when read all, or return when read anyone
            OSAL_WAIT_FOREVER); // need ms to tick
    }
    return OSAL_SUCCESS;
}

static inline int is_timeout(unsigned int ret, unsigned int mask, int mode)
{
    return ((ret == 0) || (ret != mask && mode == OSAL_WAITMODE_AND));
}

/* *
 * @par Description:
 * This API is used to sleep a process until the condition evaluates to true or a timeout elapses.
 * The condition is checked each time when the waitqueue wait is woken up.
 * @attention
 * The value range of parameter timeout is [0, 0xFFFFFFFF], and 0xFFFFFFFF means waiting forever.</li>
 * @param  wait [IN] the waitqueue to wait on.
 * @param  condition [IN] a condition evaluates to true or false.
 * @param  timeout [IN] the max sleep time unit is ms.
 *
 * @retval #0 return 0 if the condition evaluated to false after the timeout elapsed
 * @retval #1 return 1 if the condition evaluated to true after the timeout elapsed
 * @retval #others return the remaining ticks if the condition evaluated to true before the timeout elapsed
 */
int osal_wait_timeout_interruptible(osal_wait *wait, osal_wait_condition_func func, const void *param, unsigned long ms)
{
    if (wait == NULL || wait->wait == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    wait_queue_head_t *wq = (wait_queue_head_t *)(wait->wait);
    EventGroupHandle_t handler = wq->stEvent.handler;
    unsigned int ticksnow = (unsigned int)xTaskGetTickCount();
    unsigned int timetick = pdMS_TO_TICKS(ms);
    if (timetick > INT32_MAX) {
        osal_log("ms parameter invalid!\n");
        return OSAL_FAILURE;
    }
    int timeout = (int)timetick;
    int tmp_timeout = timeout;
    int ret = timeout;
    if (func == NULL) {
        return ret; /* default return timeout if condition func NULL */
    }
    if (ms == 0 && func(param)) {
        return 1;
    }
    while (!func(param)) {
        unsigned int event_ret = xEventGroupWaitBits(handler, 0x1U, OSAL_WAITMODE_CLR, OSAL_WAITMODE_AND, tmp_timeout);
        if (ms == OSAL_WAIT_FOREVER) {
            if (!func(param)) {
                continue;
            }
            break;
        }

        tmp_timeout = (int)(timeout - (unsigned int)(xTaskGetTickCount() - ticksnow));
        if (tmp_timeout <= 0 || is_timeout(event_ret, OSAL_WAITMODE_AND, 0x1U)) {
            ret = (func(param) == FALSE) ? FALSE : TRUE;
            break;
        }
        if (func(param)) {
            ret = tmp_timeout;
            break;
        }
    }
    return ret;
}


int osal_wait_timeout_uninterruptible(osal_wait *wait, osal_wait_condition_func func, const void *param,
    unsigned long ms)
{
    return osal_wait_timeout_interruptible(wait, func, param, ms);
}

void osal_wait_wakeup(osal_wait *wait)
{
    if (wait == NULL || wait->wait == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    wait_queue_head_t *wq = (wait_queue_head_t *)(wait->wait);
    EventGroupHandle_t handler = wq->stEvent.handler;
    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        (void)xEventGroupSetBits(handler, 0x1U);
        return;
    }

    BaseType_t taskwoken = pdFALSE;
    BaseType_t result = xEventGroupSetBitsFromISR(handler, 0x1U, &taskwoken);
    if (result != pdFAIL) {
        portYIELD_FROM_ISR(taskwoken);
    }
}

void osal_wait_wakeup_interruptible(osal_wait *wait)
{
    osal_wait_wakeup(wait);
}

void osal_wait_destroy(osal_wait *wait)
{
    if (wait == NULL || wait->wait == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    wait_queue_head_t *wq = (wait_queue_head_t *)(wait->wait);
    EventGroupHandle_t handler = wq->stEvent.handler;
    vEventGroupDelete(handler);
    RTOS_ListDelInit(&wq->poll_queue);
    vPortFree(wq);
    wait->wait = NULL;
}
