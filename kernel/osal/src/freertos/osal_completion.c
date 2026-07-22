/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */
#include <FreeRTOS.h>
#include <semphr.h>
#include "task.h"
#include "hwi.h"
#include "soc_osal.h"
#include "osal_list.h"
#include "osal_inner.h"

#define UINT_MAX 0xffffffffU // same as liteos

enum CompletionState {
    COMPLETION_ONE,
    COMPLETION_ALL
};

typedef struct completion {
    struct osal_list_head node_list;
    unsigned int comcount;
    enum CompletionState state;
    SemaphoreHandle_t lock;
} completion_t;

typedef struct comtask {
    struct osal_list_head node;
    TaskHandle_t task;
} osal_com;

static void init_completion(struct completion *x)
{
    OSAL_INIT_LIST_HEAD(&x->node_list);
    x->lock = xSemaphoreCreateMutex();
    x->comcount = 0;
    x->state = COMPLETION_ONE;
}

static void complete(struct completion *x)
{
    TaskHandle_t resumedtask = NULL;
    osal_com *tmp = NULL;

    uxHwiLock();
    if (!osal_list_empty(&x->node_list)) {
        tmp = osal_list_first_entry(&x->node_list, osal_com, node);
        resumedtask = tmp->task;
        vTaskResume(resumedtask);
        osal_list_del(&tmp->node);
        vPortFree((void*)tmp);
        uxHwiUnlock();
        return;
    } else if (x->comcount != UINT_MAX) {
        x->comcount++;
    }
    uxHwiUnlock();
}

static void destroy_complete(struct completion *x)
{
    struct osal_list_head *cur = NULL;
    struct osal_list_head *next = NULL;
    osal_com *tmp = NULL;

    xSemaphoreTake(x->lock, portMAX_DELAY);
    osal_list_for_each_safe(cur, next, &x->node_list) {
        osal_list_del(cur);
        tmp = osal_list_entry(cur, osal_com, node);
        vPortFree((void*)tmp);
    }
    xSemaphoreGive(x->lock);
    vSemaphoreDelete(x->lock);
}

static int no_need_wait(struct completion *x)
{
    if (x->state == COMPLETION_ALL) {
        return TRUE;
    }
    if (x->comcount > 0) {
        x->comcount--;
        return TRUE;
    }
    return FALSE;
}

static void wait_for_completion(struct completion *x)
{
    if (xHwiGetInterruptId() != configHWI_INVALID_IRQ_ID) {
        osal_log("Calling %s in interrupt callback is not allowed.\n", __FUNCTION__);
        return;
    }
    if (no_need_wait(x) == TRUE) {
        return;
    }
    osal_com *wait = NULL;
    wait = (osal_com *)pvPortMalloc(sizeof(osal_com));
    if (wait == NULL) {
        osal_log("pvPortMalloc error!\n");
        return;
    }
    OSAL_INIT_LIST_HEAD(&wait->node);
    wait->task = xTaskGetCurrentTaskHandle();
    xSemaphoreTake(x->lock, portMAX_DELAY);
    osal_list_add_tail(&wait->node, &x->node_list);
    xSemaphoreGive(x->lock);
    vTaskSuspend(wait->task);
}

int osal_completion_init(osal_completion *com)
{
    if (com == NULL || com->completion != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    com->completion = (osal_completion *)pvPortMalloc(sizeof(completion_t));
    if (com->completion == NULL) {
        osal_log("pvPortMalloc error!\n");
        return OSAL_FAILURE;
    }
    init_completion(com->completion);
    return OSAL_SUCCESS;
}

void osal_complete(osal_completion *com)
{
    if (com == NULL || com->completion == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    complete(com->completion);
}

void osal_wait_for_completion(osal_completion *com)
{
    if (com == NULL || com->completion == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    wait_for_completion(com->completion);
}

void osal_complete_destory(osal_completion *com)
{
    if (com == NULL || com->completion == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    destroy_complete(com->completion);
    vPortFree((void*)com->completion);
    com->completion = NULL;
}