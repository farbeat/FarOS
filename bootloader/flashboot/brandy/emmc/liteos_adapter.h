/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:   LITEOS ADAPTER HEADER FILE
 * Author:
 * Create:  2020-04
 */
#ifndef LITEOS_ADAPTER_H
#define LITEOS_ADAPTER_H

#include "irmalloc.h"
#include "securec.h"
#include "chip_io.h"
#include "rt_sys_header.h"
#include "debug_print.h"

#ifdef TEST_SUIT
#define SDIO_WAIT_RESPONSE_TIME 1000
#else
#define SDIO_WAIT_RESPONSE_TIME 100
#endif
#define RT_NULL                 NULL
#define rt_tick_get()           (uint32_t)uapi_systick_get_count()
#define mmcsd_delay_ms(ms)      uapi_tcxo_delay_ms(ms)
#define RT_TICK_PER_SECOND      1000
#define MMC_RT_LOG_ERR

#ifdef MMC_OPEN_ALL_LOG
#define MMC_RT_LOG_ERR
#define MMC_RT_LOG_INFO
#define MMC_RT_LOG_WARNING
#define MMC_RT_LOG_DEBUG
#endif
#ifdef MMC_RT_LOG_ERR
#define LOG_E(fmt, arg...) do { \
    PRINT("rt-mmc:%s:%d: ", __FUNCTION__, __LINE__); \
    PRINT(fmt, ##arg); \
    PRINT("\r\n"); \
} while (0)
#else
#define LOG_E(fmt, arg...) do { } while (0)
#endif
#ifdef MMC_RT_LOG_INFO
#define LOG_I(fmt, arg...) do { \
    PRINT("rt-mmc:%s:%d: ", __FUNCTION__, __LINE__); \
    PRINT(fmt, ##arg); \
    PRINT("\r\n"); \
} while (0)
#else
#define LOG_I(fmt, arg...) do { } while (0)
#endif
#ifdef MMC_RT_LOG_WARNING
#define LOG_W(fmt, arg...) do { \
    PRINT("rt-mmc:%s:%d: ", __FUNCTION__, __LINE__); \
    PRINT(fmt, ##arg); \
    PRINT("\r\n"); \
} while (0)
#else
#define LOG_W(fmt, arg...) do { } while (0)
#endif
#ifdef MMC_RT_LOG_DEBUG
#define LOG_D(fmt, arg...) do { \
    PRINT("rt-mmc:%s:%d: ", __FUNCTION__, __LINE__); \
    PRINT(fmt, ##arg); \
    PRINT("\r\n"); \
} while (0)
#else
#define LOG_D(fmt, arg...) do { } while (0)
#endif

#define memalign(boundary, size)    irmalloc(size)
#define mmc_assert(cond)            do { \
                                        if (!(cond)) \
                                            panic(1, __LINE__); \
                                    } while (0)
#define RT_ASSERT(cond)             if (!(cond)) \
                                        panic(1, __LINE__)
#define rt_malloc(size)             irmalloc(size)
#define rt_calloc(n, size)          irzalloc((n) * (size))
#define rt_set_errno(error)         panic(1, __LINE__)
#define rt_free(rmem)               irfree(rmem)
// Encapsulation liteos function
typedef uint32_t mmc_sem;
#define MMC_SEM_WAIT_FOREVER 0xFFFFFFFF
#define mmc_sem_create(sem_id)  LOS_SemCreate(1, sem_id)
#define mmc_sem_post(sem_id)  LOS_SemPost(sem_id)
#define mmc_sem_pend(sem_id, timeout)  LOS_SemPend(sem_id, timeout)
#define mmc_sem_delete(sem_id)   LOS_SemDelete(sem_id)
/* define mutex lock */
typedef uint32_t mmc_mutex;
#define MMC_MUTEX_WAIT_FOREVER 0xFFFFFFFF
#define mmc_mutex_init(mutex)   LOS_MuxCreate(mutex)
#define mmc_mutex_lock(mutex, timeout)   LOS_MuxPend(mutex, timeout)
#define mmc_mutex_unlock(mutex) LOS_MuxPost(mutex)
#define mmc_mutex_delete(mutex) LOS_MuxDelete(mutex)

/* define create thread */
typedef unsigned int mmc_thread;
#define mmc_thread_delete(id) { LOS_TaskDelete(id); }
#endif