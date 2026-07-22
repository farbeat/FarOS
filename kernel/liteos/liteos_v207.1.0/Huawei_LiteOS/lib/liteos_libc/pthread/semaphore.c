/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Semaphore file
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "semaphore.h"
#include "limits.h"
#include "sys/types.h"
#include "errno.h"
#include "los_sem_pri.h"
#include "time_pri.h"
#include "los_sys.h"

STATIC INT32 MapErrno(UINT32 err)
{
    if (err == LOS_OK) {
        return ENOERR;
    }
    switch (err) {
        case LOS_ERRNO_SEM_ALL_BUSY:
            errno = ENOSPC;
            break;
        case LOS_ERRNO_SEM_INVALID:
        case LOS_ERRNO_SEM_UNAVAILABLE:
            errno = EINVAL;
            break;
        case LOS_ERRNO_SEM_PENDED:
            errno = EBUSY;
            break;
        case LOS_ERRNO_SEM_PEND_IN_LOCK:
            errno = EPERM;
            break;
        case LOS_ERRNO_SEM_PEND_INTERR:
            errno = EINTR;
            break;
        case LOS_ERRNO_SEM_TIMEOUT:
            errno = ETIMEDOUT;
            break;
        case LOS_ERRNO_SEM_OVERFLOW:
            errno = ENOSPC;
            break;
        default:
            errno = EINVAL;
            break;
    }
    return errno;
}

static inline unsigned int SemaphoreGetId(const sem_t *sem)
{
    return ((LosSemCB *)(sem->sem))->semId;
}
static inline unsigned short SemaphoreGetCount(const sem_t *sem)
{
    return ((LosSemCB *)(sem->sem))->semCount;
}
/* Initialize semaphore to value, shared is not supported in Huawei LiteOS. */
int sem_init(sem_t *sem, int shared, unsigned int value)
{
    UINT32 semHandle = 0;
    UINT32 ret;

    (VOID)shared;
    if ((sem == NULL) || (value > SEM_VALUE_MAX)) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SemCreate(value, &semHandle);
    if (MapErrno(ret) != ENOERR) {
        return -1;
    }

    sem->sem = GET_SEM(semHandle);

    return 0;
}

int sem_destroy(sem_t *sem)
{
    UINT32 ret;

    if ((sem == NULL) || (sem->sem == NULL)) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SemDelete(SemaphoreGetId(sem));
    if (MapErrno(ret) != ENOERR) {
        return -1;
    }
    return 0;
}

int sem_wait(sem_t *sem)
{
    UINT32 ret;

    if ((sem == NULL) || (sem->sem == NULL)) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SemPend(SemaphoreGetId(sem), LOS_WAIT_FOREVER);
    if (MapErrno(ret) == ENOERR) {
        return 0;
    } else {
        return -1;
    }
}

int sem_trywait(sem_t *sem)
{
    UINT32 ret;

    if ((sem == NULL) || (sem->sem == NULL)) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SemPend(SemaphoreGetId(sem), LOS_NO_WAIT);
    if (MapErrno(ret) == ENOERR) {
        return 0;
    } else {
        if ((errno != EINVAL) || (ret == LOS_ERRNO_SEM_UNAVAILABLE)) {
            errno = EAGAIN;
        }
        return -1;
    }
}

int sem_timedwait(sem_t *sem, const struct timespec *timeout)
{
    UINT32 ret;
    UINT32 tickCnt;

    if ((sem == NULL) || (sem->sem == NULL)) {
        errno = EINVAL;
        return -1;
    }

    if (!ValidTimespec(timeout)) {
        errno = EINVAL;
        return -1;
    }

    tickCnt = OsTimespec2Tick(timeout);
    ret = LOS_SemPend(SemaphoreGetId(sem), tickCnt);
    if (MapErrno(ret) == ENOERR) {
        return 0;
    } else {
        return -1;
    }
}

int sem_post(sem_t *sem)
{
    UINT32 ret;

    if ((sem == NULL) || (sem->sem == NULL)) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SemPost(SemaphoreGetId(sem));
    if (MapErrno(ret) != ENOERR) {
        return -1;
    }

    return 0;
}

int sem_getvalue(sem_t *sem, int *currVal)
{
    INT32 val;

    if ((sem == NULL) || (sem->sem == NULL) || (currVal == NULL)) {
        errno = EINVAL;
        return -1;
    }
    val = SemaphoreGetCount(sem);

    *currVal = val;
    return 0;
}

sem_t *sem_open(const char *name, int openFlag, ...)
{
    (VOID)name;
    (VOID)openFlag;
    errno = ENOSYS;
    return NULL;
}

int sem_close(sem_t *sem)
{
    (VOID)sem;
    errno = ENOSYS;
    return -1;
}

int sem_unlink(const char *name)
{
    (VOID)name;
    errno = ENOSYS;
    return -1;
}
