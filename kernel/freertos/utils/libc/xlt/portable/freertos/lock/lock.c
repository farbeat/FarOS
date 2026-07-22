#include "FreeRTOS.h"
#include "semphr.h"
#include "libc_lock_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

int libc_lock(uintptr_t *lock)
{
    if (lock == NULL) {
        return -1;
    }

    SemaphoreHandle_t semaphore = NULL;
    uint32_t handle = *(uint32_t *)lock;
    if (handle == LIBC_LOCK_INITIALIZER) {
        semaphore = xSemaphoreCreateRecursiveMutex();
        if (semaphore == NULL) {
            return -1;
        }
        *lock = (uintptr_t)semaphore;
    } else {
        semaphore = (SemaphoreHandle_t)(*lock);
    }

    if(xSemaphoreTakeRecursive(semaphore, portMAX_DELAY) != pdTRUE) {
        return -1;
    }

    return 0;
}

int libc_unlock(uintptr_t *lock)
{
    
    if (lock == NULL) {
        return -1;
    }

    xSemaphoreGiveRecursive((SemaphoreHandle_t)(*lock));

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
