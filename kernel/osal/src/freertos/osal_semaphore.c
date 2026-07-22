/*
* Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
* Description:osal_math
* Author: AuthorNameMagicTag
* Create: 2019-10-11
*/
#include <FreeRTOS.h>
#include <semphr.h>
#include "soc_osal.h"
#include "osal_inner.h"
#include "hwi.h"

#define MAX_COUNT 0xFFFE // same to liteos
#define MAX_BINARY_VAL 1

int osal_sem_init(osal_semaphore *sem, int val)
{
    if (sem == NULL || sem->sem != NULL || val < 0) {
        osal_log("parameter invalid! val:%d\n", val);
        return OSAL_FAILURE;
    }

    SemaphoreHandle_t handle = xSemaphoreCreateCounting(MAX_COUNT, val); // (max_count, init_count)
    if (handle == NULL) {
        osal_log("xSemaphoreCreateCounting error! val: %d\n", val);
        return OSAL_FAILURE;
    }
    sem->sem = (void *)handle;
    vQueueAddToRegistry(handle, "Semaphore");
    return OSAL_SUCCESS;
}

int osal_sem_binary_sem_init(osal_semaphore *sem, int val)
{
    if (sem == NULL || sem->sem != NULL || val > MAX_BINARY_VAL || val < 0) {
        osal_log("parameter invalid! val:%d\n", val);
        return OSAL_FAILURE;
    }
    SemaphoreHandle_t handle = xSemaphoreCreateBinary();
    if (handle == NULL) {
        osal_log("xSemaphoreCreateBinary error!\n");
        return OSAL_FAILURE;
    }
    if (val == MAX_BINARY_VAL) {
        (void)xSemaphoreGive(handle);
    }
    sem->sem = handle;
    vQueueAddToRegistry(handle, "Binary_Sema");
    return OSAL_SUCCESS;
}

void osal_sem_destroy(osal_semaphore *sem)
{
    if (sem == NULL || sem->sem == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    vQueueUnregisterQueue((SemaphoreHandle_t)sem->sem);
    vSemaphoreDelete((SemaphoreHandle_t)sem->sem);
    sem->sem = NULL;
}

int osal_sem_down(osal_semaphore *sem)
{
    if (sem == NULL || sem->sem == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    BaseType_t result;
    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        result = xSemaphoreTake((SemaphoreHandle_t)sem->sem, portMAX_DELAY);
        return result == pdTRUE ? OSAL_SUCCESS : OSAL_FAILURE;
    }

    result = xSemaphoreTakeFromISR((SemaphoreHandle_t)sem->sem, NULL);
    return result == pdTRUE ? OSAL_SUCCESS : OSAL_FAILURE;
}

int osal_sem_down_timeout(osal_semaphore *sem, unsigned int timeout)
{
    if (sem == NULL || sem->sem == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)sem->sem, pdMS_TO_TICKS(timeout));
    return ret == pdTRUE ? OSAL_SUCCESS : OSAL_FAILURE;
}

int osal_sem_trydown(osal_semaphore *sem)
{
    if (sem == NULL || sem->sem == NULL) {
        osal_log("parameter invalid!\n");
        return 1;
    }
    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)sem->sem, 0);
    return ret == pdTRUE ? 0 : 1;
}

void osal_sem_up(osal_semaphore *sem)
{
    if (sem == NULL || sem->sem == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        (void)xSemaphoreGive((SemaphoreHandle_t)sem->sem);
        return;
    }

    BaseType_t taskwoken = pdFALSE;
    BaseType_t result = xSemaphoreGiveFromISR((SemaphoreHandle_t)sem->sem, &taskwoken);
    if (result != pdPASS) {
        osal_log("xSemaphoreGiveFromISR failed, result=0x%x invalid!\n", result);
        return;
    }
    portYIELD_FROM_ISR(taskwoken);
}

int osal_sem_down_interruptible(osal_semaphore *sem)
{
    return osal_sem_down(sem);
}