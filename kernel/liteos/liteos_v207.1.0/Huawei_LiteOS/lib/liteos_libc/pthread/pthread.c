/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Pthread file
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

#include "pthread.h"
#include "pprivate.h"
#include "errno.h"
#include "stdio.h"
#include "limits.h"
#include "pthread_impl.h"

/*
 * Array of pthread control structures. A pthread_t object is
 * "just" an index into this array.
 */

STATIC _pthread_data g_pthreadData[LOSCFG_BASE_CORE_TSK_LIMIT + 1];
/* Count of number of threads that have exited and not been reaped. */

STATIC UINT16 g_pthreadNameNumber = 1;
SPIN_LOCK_INIT(g_pthreadNameSpin);

/* this is to protect the pthread data */
STATIC pthread_mutex_t g_pthreadsDataMutex = PTHREAD_MUTEX_INITIALIZER;

STATIC INT32 MapErrno(UINT32 err)
{
    if (err == LOS_OK) {
        return ENOERR;
    }
    switch (err) {
        case LOS_ERRNO_TSK_ID_INVALID:
        case LOS_ERRNO_TSK_PTR_NULL:
        case LOS_ERRNO_TSK_NAME_EMPTY:
        case LOS_ERRNO_TSK_ENTRY_NULL:
        case LOS_ERRNO_TSK_PRIOR_ERROR:
        case LOS_ERRNO_TSK_STKSZ_TOO_LARGE:
        case LOS_ERRNO_TSK_STKSZ_TOO_SMALL:
        case LOS_ERRNO_TSK_NOT_CREATED:
        case LOS_ERRNO_TSK_CPU_AFFINITY_MASK_ERR:
            errno = EINVAL;
            break;
        case LOS_ERRNO_TSK_TCB_UNAVAILABLE:
        case LOS_ERRNO_TSK_MP_SYNC_RESOURCE:
        case LOS_ERRNO_SEM_OVERFLOW:
        case LOS_ERRNO_SEM_ALL_BUSY:
            errno = ENOSPC;
            break;
        case LOS_ERRNO_TSK_NO_MEMORY:
            errno = ENOMEM;
            break;
        default:
            errno = EINVAL;
            break;
    }
    return errno;
}

/*
 * Private version of pthread_self() that returns a pointer to our internal
 * control structure.
 */
_pthread_data *pthread_get_self_data(void)
{
    UINT32 runningTaskPID = LOS_CurTaskIDGet();
    _pthread_data *data = &g_pthreadData[runningTaskPID];

    return data;
}

_pthread_data *pthread_get_data(pthread_t id)
{
    _pthread_data *data = NULL;

    if ((id >= (pthread_t)(sizeof(g_pthreadData) / sizeof(_pthread_data))) || (id < 0)) {
        return NULL;
    }

    data = &g_pthreadData[id];
    /* Check that this is a valid entry */
    if (data->state == PTHREAD_DATA_UNUSED) {
        return NULL;
    }

    /* Check that the entry matches the id */
    if (data->id != id) {
        return NULL;
    }

    /* Return the pointer */
    return data;
}

/*
 * Check whether there is a cancel pending and if so, whether
 * cancellations are enabled. We do it in this order to reduce the
 * number of tests in the common case - when no cancellations are
 * pending. We make this inline so it can be called directly below for speed
 */
STATIC INT32 CheckForCancel(VOID)
{
    _pthread_data *self = pthread_get_self_data();
    if ((self->canceled == PTHREAD_CANCEL_STATE_PEND) &&
        (self->cancelstate == PTHREAD_CANCEL_ENABLE)) {
        return 1;
    }
    return 0;
}

STATIC VOID SetPthreadAttr(const _pthread_data *self, const pthread_attr_t *attr, pthread_attr_t *outAttr)
{
    /*
     * Set use_attr to the set of attributes we are going to
     * actually use. Either those passed in, or the default set.
     */
    if (attr == NULL) {
        (VOID)pthread_attr_init(outAttr);
    } else {
        (VOID)memcpy(outAttr, attr, sizeof(pthread_attr_t));
    }

    /*
     * If the stack size is not valid, we can assume that it is at
     * least PTHREAD_STACK_MIN bytes.
     */
    if (!outAttr->stacksize_set) {
        outAttr->stacksize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    }
    if (outAttr->inheritsched == PTHREAD_INHERIT_SCHED) {
        if (self->task == NULL) {
            outAttr->schedparam.sched_priority = ((LosTaskCB *)(OsCurrTaskGet()))->priority;
        } else {
            outAttr->schedpolicy = self->attr.schedpolicy;
            outAttr->schedparam  = self->attr.schedparam;
            outAttr->scope       = self->attr.scope;
        }
    }
}

STATIC VOID InitPthreadData(pthread_t threadId, pthread_attr_t *userAttr,
                            const CHAR name[], size_t len)
{
    errno_t err;

    LosTaskCB *taskCB = OS_TCB_FROM_TID(threadId);
    _pthread_data *created = &g_pthreadData[threadId];

    err = strncpy_s(created->name, sizeof(created->name), name, len - 1);
    if (err != EOK) {
        PRINT_ERR("%s: %d, err: %d\n", __FUNCTION__, __LINE__, err);
        return;
    }

    userAttr->stacksize   = taskCB->stackSize;
    taskCB->taskName      = created->name;
#ifdef LOSCFG_KERNEL_SMP
    if (userAttr->cpuset.__bits[0] > 0) {
        taskCB->cpuAffiMask = (UINT16)userAttr->cpuset.__bits[0];
    }
#endif
    created->attr         = *userAttr;
    created->id           = threadId;
    created->task         = taskCB;
    created->state        = PTHREAD_DATA_USED;
    /* need to confirmation */
    created->cancelstate  = PTHREAD_CANCEL_ENABLE;
    created->canceltype   = PTHREAD_CANCEL_DEFERRED;
    created->cancelbuf    = NULL;
    created->canceled     = PTHREAD_CANCEL_STATE_INIT;
    created->freestack    = 0; /* no use default : 0 */
    created->stackmem     = (UINT32)taskCB->topOfStack;
    created->thread_data  = NULL;
}

STATIC INT32 SetPthreadName(CHAR *name, UINT32 len)
{
    INT32 ret;

    LOS_SpinLock(&g_pthreadNameSpin);
    ret = snprintf_s(name, len, len - 1, "pth%02u", g_pthreadNameNumber);
    if (ret == -1) {
        goto OUT;
    }
    ++g_pthreadNameNumber;

OUT:
    LOS_SpinUnlock(&g_pthreadNameSpin);
    return ret;
}

STATIC VOID OsTaskSetExitState(pthread_t thread)
{
    _pthread_data *joined = NULL;
    joined = pthread_get_data(thread);
    if (joined != NULL) {
        joined->state = PTHREAD_DATA_UNUSED;
    }
}

STATIC VOID OsTaskDeleteCancelCheck(pthread_t thread)
{
    _pthread_data *data = NULL;
    data = pthread_get_data(thread);
    if (data == NULL) {
        return;
    }
    if (data->canceled == PTHREAD_CANCEL_STATE_EXEC) {
        data->task->threadJoinRetval = (VOID *)PTHREAD_CANCELED;
        data->canceled = PTHREAD_CANCEL_STATE_INIT;
    }
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*startRoutine)(void *), void *arg)
{
    pthread_attr_t userAttr;
    UINT32 ret;
    CHAR name[PTHREAD_DATA_NAME_MAX] = {0};
    TSK_INIT_PARAM_S taskInitParam = {0};
    UINT32 taskHandle;
    _pthread_data *self = pthread_get_self_data();

    if ((thread == NULL) || (startRoutine == NULL)) {
        return EINVAL;
    }

    if (g_taskRecycleHook == NULL) {
        g_taskRecycleHook = (TASK_RECYCLE_HOOK)OsTaskSetExitState;
    }

    SetPthreadAttr(self, attr, &userAttr);
    if (SetPthreadName(name, sizeof(name)) == -1) {
        *thread = (pthread_t)-1;
        return MapErrno((UINT32)-1);
    }

    taskInitParam.pcName       = name;
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)(UINTPTR)startRoutine;
    taskInitParam.usTaskPrio   = (UINT16)userAttr.schedparam.sched_priority;
    taskInitParam.uwStackSize  = (UINT32)userAttr.stacksize;
    /* Set the pthread default joinable */
    taskInitParam.uwResved     = (userAttr.detachstate == PTHREAD_CREATE_JOINABLE) ? \
                                    LOS_TASK_STATUS_JOINABLE : LOS_TASK_STATUS_DETACHED;

    LOS_TASK_PARAM_INIT_ARG(taskInitParam, arg);
    ret = LOS_TaskCreateOnly(&taskHandle, &taskInitParam);
    if (ret != LOS_OK) {
        *thread = (pthread_t)-1;

        return MapErrno(ret);
    }
    *thread = (pthread_t)taskHandle;
    InitPthreadData(*thread, &userAttr, name, PTHREAD_DATA_NAME_MAX);
    (VOID)LOS_TaskResume(taskHandle);

    return ENOERR;
}

void pthread_exit(void *retVal)
{
    _pthread_data *self = pthread_get_self_data();
    int *oldState = NULL;

    if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, oldState) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    __pthread_tsd_run_dtors();

    if (pthread_mutex_lock(&g_pthreadsDataMutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    self->task->threadJoinRetval = retVal;
    /* Call the cleanup handlers. */
    while (self->cancelbuf != NULL) {
        if (self->cancelbuf->routine != NULL) {
            self->cancelbuf->routine(self->cancelbuf->arg);
        }
        self->cancelbuf = self->cancelbuf->next;
    }

    if (pthread_mutex_unlock(&g_pthreadsDataMutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    (VOID)LOS_TaskDelete(self->task->taskId);
}

int pthread_join(pthread_t thread, void **retVal)
{
    UINT32 ret;
    _pthread_data *joined = NULL;

    /* Check for cancellation first. */
    pthread_testcancel();
    joined = pthread_get_data(thread);
    if (joined == NULL) {
        return ESRCH;
    }

    ret = LOS_TaskJoin((UINT32)thread, (UINTPTR *)retVal);
    if (ret == LOS_ERRNO_TSK_NOT_JOIN_SELF) {
        return EDEADLK;
    }

    pthread_testcancel();
    return MapErrno(ret);
}

/*
 * Set the detachstate of the thread to "detached". The thread then does not
 * need to be joined and its resources will be freed when it exits.
 */
int pthread_detach(pthread_t thread)
{
    UINT32 ret;

    ret = LOS_TaskDetach((UINT32)thread);
    if ((ret == LOS_ERRNO_TSK_IS_DETACHED) || (ret == LOS_ERRNO_TSK_ALREADY_JOIN)) {
        return EINVAL;
    }  else if (ret != LOS_OK) {
        return ESRCH;
    }

    return ENOERR;
}

int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
    _pthread_data *data = NULL;
    int ret;

    if ((param == NULL) || (param->sched_priority < LOS_TASK_PRIORITY_HIGHEST) ||
        (param->sched_priority > LOS_TASK_PRIORITY_LOWEST)) {
        return EINVAL;
    }

    if ((policy == SCHED_FIFO) || (policy == SCHED_OTHER)) {
        return ENOTSUP;
    }

    if (policy != SCHED_RR) {
        return EINVAL;
    }

    /* The parameters seem OK, change the thread. */
    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    data = pthread_get_data(thread);
    if (data == NULL) {
        ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
        if (ret != ENOERR) {
            return ret;
        }
        return ESRCH;
    }

    /* Only support one policy now */
    data->attr.schedpolicy = SCHED_RR;
    data->attr.schedparam  = *param;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }
    (VOID)LOS_TaskPriSet((UINT32)thread, (UINT16)param->sched_priority);

    return ENOERR;
}

int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param)
{
    _pthread_data *data = NULL;
    int ret;

    if ((policy == NULL) || (param == NULL)) {
        return EINVAL;
    }

    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    data = pthread_get_data(thread);
    if (data == NULL) {
        goto ERR_OUT;
    }

    *policy = data->attr.schedpolicy;
    *param = data->attr.schedparam;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    return ret;
ERR_OUT:
    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }
    return ESRCH;
}

int pthread_setschedprio(pthread_t thread, int prio)
{
    _pthread_data *data = NULL;
    int ret;

    if ((prio < LOS_TASK_PRIORITY_HIGHEST) || (prio > LOS_TASK_PRIORITY_LOWEST)) {
        return EINVAL;
    }

    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    data = pthread_get_data(thread);
    if (data == NULL) {
        ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
        if (ret != ENOERR) {
            return ret;
        }
        return ESRCH;
    }

    data->attr.schedparam.sched_priority = prio;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    return (int)LOS_TaskPriSet((UINT32)thread, (UINT16)prio);
}

/* Call initRoutine just the once per control variable. */
int pthread_once(pthread_once_t *onceControl, void (*initRoutine)(void))
{
    pthread_once_t old;
    int ret;

    if ((onceControl == NULL) || (initRoutine == NULL)) {
        return EINVAL;
    }

    /* Do a test and set on the onceControl object. */
    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    old = *onceControl;
    *onceControl = 1;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }
    /* If the onceControl was zero, call the initRoutine(). */
    if (!old) {
        initRoutine();
    }

    return ENOERR;
}

/*
 * Set cancel state of current thread to ENABLE or DISABLE.
 * Returns old state in *oldState.
 */
int pthread_setcancelstate(int state, int *oldState)
{
    _pthread_data *self = NULL;
    int ret;

    if ((state != PTHREAD_CANCEL_ENABLE) && (state != PTHREAD_CANCEL_DISABLE)) {
        return EINVAL;
    }

    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    self = pthread_get_self_data();

    if (oldState != NULL) {
        *oldState = self->cancelstate;
    }

    self->cancelstate = (UINT8)state;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    return ENOERR;
}

/*
 * Set cancel type of current thread to ASYNCHRONOUS or DEFERRED.
 * Returns old type in *oldType.
 */
int pthread_setcanceltype(int type, int *oldType)
{
    _pthread_data *self = NULL;
    int ret;

    if ((type != PTHREAD_CANCEL_ASYNCHRONOUS) && (type != PTHREAD_CANCEL_DEFERRED)) {
        return EINVAL;
    }

    ret = pthread_mutex_lock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    self = pthread_get_self_data();
    if (oldType != NULL) {
        *oldType = self->canceltype;
    }

    self->canceltype = (UINT8)type;

    ret = pthread_mutex_unlock(&g_pthreadsDataMutex);
    if (ret != ENOERR) {
        return ret;
    }

    return ENOERR;
}

STATIC UINT32 DoPthreadCancel(_pthread_data *data)
{
    UINT32 ret = LOS_OK;
    LOS_TaskLock();
    data->canceled = PTHREAD_CANCEL_STATE_INIT;
    if ((data->task->taskStatus == 0) || (LOS_TaskSuspend(data->task->taskId) != ENOERR)) {
        ret = LOS_NOK;
        goto OUT;
    }
    data->canceled = PTHREAD_CANCEL_STATE_EXEC;
    (VOID)LOS_TaskDelete(data->task->taskId);
OUT:
    LOS_TaskUnlock();
    return ret;
}

int pthread_cancel(pthread_t thread)
{
    _pthread_data *data = NULL;

    if (pthread_mutex_lock(&g_pthreadsDataMutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    data = pthread_get_data(thread);
    if (data == NULL) {
        if (pthread_mutex_unlock(&g_pthreadsDataMutex) != ENOERR) {
            PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
        }
        return ESRCH;
    }

    if (g_taskDeleteHook == NULL) {
        g_taskDeleteHook = (TASK_DELETE_HOOK)OsTaskDeleteCancelCheck;
    }

    data->canceled = PTHREAD_CANCEL_STATE_PEND;

    if ((data->cancelstate == PTHREAD_CANCEL_ENABLE) &&
        (data->canceltype == PTHREAD_CANCEL_ASYNCHRONOUS)) {
        /*
         * If the thread has cancellation enabled, and it is in
         * asynchronous mode, suspend it and set corresponding thread's status.
         * We also release the thread out of any current wait to make it wake up.
         */
        if (DoPthreadCancel(data) == LOS_NOK) {
            goto ERROR_OUT;
        }
    }

    /*
     * Otherwise the thread has cancellation disabled, in which case
     * it is up to the thread to enable cancellation
     */
    if (pthread_mutex_unlock(&g_pthreadsDataMutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }

    return ENOERR;
ERROR_OUT:
    if (pthread_mutex_unlock(&g_pthreadsDataMutex) != ENOERR) {
        PRINT_ERR("%s: %d failed\n", __FUNCTION__, __LINE__);
    }
    return ESRCH;
}

/* Compare two thread identifiers. */
int pthread_equal(pthread_t thread1, pthread_t thread2)
{
    return thread1 == thread2;
}

/*
 * Test for a pending cancellation for the current thread and terminate
 * the thread if there is one.
 */
void pthread_testcancel(void)
{
    if (CheckForCancel()) {
        /*
         * If we have cancellation enabled, and there is a cancellation
         * pending, then go ahead and do the deed.
         * Exit now with special retVal. pthread_exit() calls the
         * cancellation handlers implicitly.
         */
        pthread_exit((void *)PTHREAD_CANCELED);
    }
}

/* Get current thread id. */
pthread_t pthread_self(void)
{
    _pthread_data *data = pthread_get_self_data();

    return data->id;
}

/*
 * Set the cpu affinity mask for the thread
 */
int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t* cpuset)
{
    INT32 ret = (INT32)sched_setaffinity(thread, cpusetsize, cpuset);
    if (ret == -1) {
        return errno;
    } else {
        return ENOERR;
    }
}

/*
 * Get the cpu affinity mask from the thread
 */
int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpu_set_t* cpuset)
{
    INT32 ret = (INT32)sched_getaffinity(thread, cpusetsize, cpuset);
    if (ret == -1) {
        return errno;
    } else {
        return ENOERR;
    }
}

void pthread_cleanup_push_inner(struct pthread_cleanup_buffer *buffer,
                                void (*routine)(void *), void *arg)
{
    _pthread_data *self = __pthread_self();

    buffer->routine = routine;
    buffer->arg = arg;
    buffer->next = self->cancelbuf;
    self->cancelbuf = buffer;
}

void pthread_cleanup_pop_inner(struct pthread_cleanup_buffer *buffer, int execute)
{
    __pthread_self()->cancelbuf = buffer->next;
    if (execute) {
        if (buffer->routine != NULL) {
            buffer->routine(buffer->arg);
        }
    }
}
