#ifndef _LIBC_LOCK_ADAPT_H
#define _LIBC_LOCK_ADAPT_H

#include <stdint.h>
#include <limits.h>
#include "pthread_adapt.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef __LITEOS__
/* init pthread_mutex for __fdopen and stdin stdout stderr in LiteOS & seLiteOS */
#define __INIT_LOCK_ATTR(_lock) \
        _lock.stAttr.protocol = PTHREAD_PRIO_INHERIT, \
        _lock.stAttr.prioceiling = LOS_TASK_PRIORITY_LOWEST, \
        _lock.stAttr.type = PTHREAD_MUTEX_RECURSIVE_NP, \
        _lock.stAttr.reserved = 0, \
        _lock.stLock.muxList.pstPrev = (struct LOS_DL_LIST *)NULL, \
        _lock.stLock.muxList.pstNext = (struct LOS_DL_LIST *)NULL, \
        _lock.stLock.owner = NULL, \
        _lock.stLock.muxCount = 0

#elif defined(__FREERTOS__)
#define __INIT_LOCK_ATTR(_lock)         \
     _lock.xIsInitialized = pdFALSE,     \
     _lock.xTaskOwner = NULL,            \
     _lock.xAttr.iType = PTHREAD_MUTEX_RECURSIVE

#endif

extern int libc_lock(uintptr_t *lock);
extern int libc_unlock(uintptr_t *lock);
#define LIBC_LOCK_INITIALIZER UINT_MAX
#define LIBC_LOCK_INIT(x) unsigned int x = LIBC_LOCK_INITIALIZER
#define LIBC_LOCK(x) libc_lock(&(x))
#define LIBC_UNLOCK(x) libc_unlock(&(x))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LIBC_LOCK_ADAPT_H */
