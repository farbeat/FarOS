/*
* Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
* Description:osal_mutex
* Author: AuthorNameMagicTag
* Create: 2019-10-11
*/
#include <FreeRTOS.h>
#include <semphr.h>
#include "soc_osal.h"
#include "osal_inner.h"

int osal_mutex_init(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    #if defined(configUSE_RECURSIVE_MUTEXES) && (configUSE_RECURSIVE_MUTEXES == 1)
    SemaphoreHandle_t handle = xSemaphoreCreateRecursiveMutex();
    #else
    SemaphoreHandle_t handle = xSemaphoreCreateMutex();
    #endif
    if (handle == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    mutex->mutex = handle;
    vQueueAddToRegistry(handle, "Mutex");
    return OSAL_SUCCESS;
}

void osal_mutex_destroy(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    vQueueUnregisterQueue((SemaphoreHandle_t)mutex->mutex);
    vSemaphoreDelete((SemaphoreHandle_t)mutex->mutex);
    mutex->mutex = NULL;
}

int osal_mutex_lock(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    #if defined(configUSE_RECURSIVE_MUTEXES) && (configUSE_RECURSIVE_MUTEXES == 1)
    BaseType_t ret = xSemaphoreTakeRecursive((SemaphoreHandle_t)mutex->mutex, portMAX_DELAY);
    #else
    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)mutex->mutex, portMAX_DELAY);
    #endif
    return ret == pdTRUE ? OSAL_SUCCESS : OSAL_FAILURE;
}

int osal_mutex_lock_timeout(osal_mutex *mutex, unsigned int timeout)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    #if defined(configUSE_RECURSIVE_MUTEXES) && (configUSE_RECURSIVE_MUTEXES == 1)
    BaseType_t ret = xSemaphoreTakeRecursive((SemaphoreHandle_t)mutex->mutex, pdMS_TO_TICKS(timeout));
    #else
    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)mutex->mutex, pdMS_TO_TICKS(timeout));
    #endif
    return ret == pdTRUE ? OSAL_SUCCESS : OSAL_FAILURE;
}

int osal_mutex_trylock(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return pdFALSE;
    }
    #if defined(configUSE_RECURSIVE_MUTEXES) && (configUSE_RECURSIVE_MUTEXES == 1)
    BaseType_t ret = xSemaphoreTakeRecursive((SemaphoreHandle_t)mutex->mutex, 0);
    #else
    BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)mutex->mutex, 0);
    #endif
    return ret == pdTRUE ? TRUE : FALSE;
}

void osal_mutex_unlock(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    #if defined(configUSE_RECURSIVE_MUTEXES) && (configUSE_RECURSIVE_MUTEXES == 1)
    (void)xSemaphoreGiveRecursive((SemaphoreHandle_t)mutex->mutex);
    #else
    (void)xSemaphoreGive((SemaphoreHandle_t)mutex->mutex);
    #endif
}

int osal_mutex_is_locked(osal_mutex *mutex)
{
    if (mutex == NULL || mutex->mutex == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    TaskHandle_t taskhandle = xSemaphoreGetMutexHolder(mutex->mutex);
    return taskhandle != NULL ? TRUE : FALSE;
}
