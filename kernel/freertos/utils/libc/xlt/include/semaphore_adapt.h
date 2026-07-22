#ifndef _SEMAPHORE_ADAPT_H
#define _SEMAPHORE_ADAPT_H

#ifdef __LITEOS__

/**
 * @ingroup semaphore
 * API parameter sructure
 */
typedef struct posix_sem {
    void *sem;
} sem_t;

#elif defined(__FREERTOS__)
#include "FreeRTOS.h"
/**
 * @brief Semaphore type.
 */
typedef struct
{
    StaticSemaphore_t xSemaphore; /**< FreeRTOS semaphore. */
    int value;                    /**< POSIX semaphore count. */
} sem_t;

#endif
#endif /* _SEMAPHORE_ADAPT_H */
