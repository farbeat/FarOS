/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Lock Dependency Check.
 * Author: Huawei LiteOS Team
 * Create: 2022-08-01
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

#include "los_lockdep.h"
#include "los_task_pri.h"
#include "los_printf_pri.h"
#include "los_exc.h"
#include "los_atomic.h"
#include "los_mux_pri.h"
#include "los_sem_pri.h"
#include "pthread_mux_debug_pri.h"

#define SHIFT_32_BIT    32
#define ATOMIC_USED     1
#define ATOMIC_UNUSED   0
Atomic g_lockdepAvailable[4] = {ATOMIC_USED, ATOMIC_USED, ATOMIC_USED, ATOMIC_USED};

const CHAR *DumpLockDepErrorString[LOCKDEP_ERR_MAX] = {
    "success",
    "double lock",
    "dead lock",
    "unlock without lock",
    "lockdep overflow"
};

const CHAR *DumpLockTypeName[LOCK_TYPE_MAX] = {
#ifdef LOSCFG_KERNEL_SPINDEP
    "Spinlock name:",
#endif
#ifdef LOSCFG_KERNEL_MUXDEP
    "Mutex ID:",
#endif
#ifdef LOSCFG_KERNEL_SEMDEP
    "Sem ID:",
#endif
#ifdef LOSCFG_PTHREAD_MUXDEP
    "MutexBaseCB addr:",
#endif
};

#ifdef LOSCFG_KERNEL_MUXDEP
LosLockCheck *g_muxDeadlockDetArray[LOSCFG_BASE_IPC_MUX_LIMIT];
LosLockCheck *OsMuxLockDepGet(UINT32 id)
{
    return g_muxDeadlockDetArray[GET_MUX_INDEX(id)];
}
#endif

#ifdef LOSCFG_KERNEL_SEMDEP
LosLockCheck *g_semDeadlockDetArray[LOSCFG_BASE_IPC_SEM_LIMIT];
LosLockCheck *OsSemLockDepGet(UINT32 id)
{
    return g_semDeadlockDetArray[GET_MUX_INDEX(id)];
}
#endif

VOID LockDepMalloc(enum LockType type, UINT32 id)
{
    LosLockCheck *lock = NULL;
    int ret;

    lock = (LosLockCheck *)LOS_MemAlloc(m_aucSysMem1, (UINT32)sizeof(LosLockCheck));
    if (lock == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        return;
    }

    lock->name = (CHAR *)LOS_MemAlloc(m_aucSysMem1, NAME_BUFF_SIZE);
    if (lock->name == NULL) {
        PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
        (VOID)LOS_MemFree(m_aucSysMem1, lock);
        return;
    }

    ret = sprintf_s(lock->name, NAME_BUFF_SIZE, "%04d", id);
    if (ret < 0) {
        PRINT_ERR("%s: sprintf_s failed!\n", __FUNCTION__);
        (VOID)LOS_MemFree(m_aucSysMem1, lock->name);
        (VOID)LOS_MemFree(m_aucSysMem1, lock);
        return;
    }

#ifdef LOSCFG_KERNEL_MUXDEP
    if (type == LOCK_MUTEX) {
        g_muxDeadlockDetArray[GET_MUX_INDEX(id)] = lock;
    }
#endif
#ifdef LOSCFG_KERNEL_SEMDEP
    if (type == LOCK_SEM) {
        g_semDeadlockDetArray[GET_MUX_INDEX(id)] = lock;
    }
#endif
}

/* atomic insurance for lockdep check */
STATIC INLINE VOID OsLockDepRequire(enum LockType type, UINT32 *intSave)
{
    *intSave = LOS_IntLock();
    while (LOS_AtomicCmpXchg32bits(&g_lockdepAvailable[type], ATOMIC_UNUSED, ATOMIC_USED)) { /* busy waiting */
    }
}

STATIC INLINE VOID OsLockDepRelease(enum LockType type, UINT32 intSave)
{
    LOS_AtomicSet(&g_lockdepAvailable[type], ATOMIC_USED);
    LOS_IntRestore(intSave);
}
STATIC BOOL OSCheckDoubleLock(const LosTaskCB *current, const LosLockCheck *lock)
{
#ifdef LOSCFG_KERNEL_SPINDEP
    LosTaskCB *lockOwner = lock->owner;
    if ((lock->type == LOCK_SPIN) && (current == lockOwner)) {
        return TRUE;
    }
#else
    (VOID)current;
    (VOID)lock;
#endif
    return FALSE;
}

STATIC BOOL OsCheckNoncircular(const LosTaskCB *current, const LosLockCheck *lock)
{
    const LosLockCheck *lockTemp = NULL;
    LosTaskCB *lockOwner = lock->owner;
    if (current == lockOwner) {
        return FALSE;
    }
    do {
        if (current == lockOwner) {
            return TRUE;
        }
        if (lockOwner->lockDep[lock->type].waitLock != NULL) {
            lockTemp = lockOwner->lockDep[lock->type].waitLock;
            lockOwner = lockTemp->owner;
        } else {
            break;
        }
    } while (lockOwner != NULL);
    return FALSE;
}

WEAK VOID OsLockDepPanic(enum LockDepErrType errType)
{
    /* halt here */
    (VOID)errType;
    (VOID)LOS_IntLock();
    OsBackTrace();
    while (1) {}
}

STATIC CHAR *OsLockDepNameGet(enum LockType type, const LockDep *lockDep, UINT32 index)
{
#ifdef LOSCFG_PTHREAD_MUXDEP
    if (type == LOCK_PTHREAD) {
        return OS_PTHREAD_MUTEX_CHECK_FROM_DEADLOCK(lockDep->heldLocks[index].lockPtr)->deadLockDet.name;
    }
#else
    (VOID)type;
#endif
    LosLockCheck *lockCheck = lockDep->heldLocks[index].lockPtr;
    return lockCheck->name;
}

STATIC LosLockCheck *OsLockDepGetLock(enum LockType type, const void* lock)
{
#ifdef LOSCFG_PTHREAD_MUXDEP
    if (type == LOCK_PTHREAD) {
        LosPthreadMuxCheck *lockCheck = OS_PTHREAD_MUTEX_CHECK_FROM_DEADLOCK(lock);
        return &lockCheck->deadLockDet;
    }
#else
    (VOID)type;
#endif
    return (LosLockCheck *)lock;
}

STATIC VOID DumpLockMsg(const LosLockCheck *lock, const LockDep *lockDep)
{
    LosLockCheck *lockCheck = NULL;
    INT32 index;
    for (index = 0; index < lockDep->lockDepth; index++) {
        if (lockDep->heldLocks[index].lockPtr == lock) {
            PRINT_EXC("[%d] %s %s <-- addr:0x%x\n", index, DumpLockTypeName[lock->type],
                OsLockDepNameGet(lock->type, lockDep, index), (UINTPTR)LOCKDEP_GET_ADDR(lockDep, index));
        } else {
            PRINT_EXC("[%d] %s %s \n", index, DumpLockTypeName[lock->type],
                OsLockDepNameGet(lock->type, lockDep, index));
        }
    }

    if (lockDep->waitLock == NULL) {
        lockCheck = OsLockDepGetLock(lock->type, lock);
    } else {
        lockCheck = OsLockDepGetLock(lock->type, lockDep->waitLock);
    }
    PRINT_EXC("[%d] %s %s <-- now\n", index, DumpLockTypeName[lockCheck->type], lockCheck->name);
}

STATIC VOID OsLockDepDumpLock(const LosTaskCB *task, const LosLockCheck *lock,
                              const VOID *requestAddr, enum LockDepErrType errType)
{
    const enum LockType type = lock->type;
    const LockDep *lockDep = &task->lockDep[type];
    const LosTaskCB *temp = task;

    PRINT_EXC("lockdep check failed\n");
    PRINT_EXC("error type   : %s\n", DumpLockDepErrorString[errType]);
    PRINT_EXC("request addr : 0x%x\n", (UINTPTR)requestAddr);
    do {
        PRINT_EXC("task name    : %s\n", temp->taskName);
        PRINT_EXC("task id      : %u\n", temp->taskId);
        PRINT_EXC("cpu num      : %u\n", lock->cpuId);
        PRINT_EXC("start dumping lockdep information\n");

        DumpLockMsg(lock, lockDep);

        if (errType == LOCKDEP_ERR_DEAD_LOCK) {
            temp = lock->owner;
            lockDep = &temp->lockDep[type];
            lock = temp->lockDep[type].waitLock;
        }
    } while ((temp != task) && (lock != NULL));
    OsLockDepPanic(errType);
}

VOID OsLockDepCheckInit(enum LockType type, LosLockCheck *lock, UINT32 id)
{
    if ((type >= LOCK_TYPE_MAX) || (lock == NULL)) {
        return;
    }

    lock->id = id;
    lock->type = type;
    lock->cpuId = 0xFFFFFFFF;
    lock->owner = NULL;
}

VOID OsLockDepCheckDeInit(enum LockType type, LosLockCheck *lock, UINT32 id)
{
    if ((type < LOCK_TYPE_MAX) && (lock != NULL) && (lock->name != NULL)) {
        (VOID)LOS_MemFree(m_aucSysMem1, lock->name);
        (VOID)LOS_MemFree(m_aucSysMem1, lock);
#ifdef LOSCFG_KERNEL_MUXDEP
        if (type == LOCK_MUTEX) {
            g_muxDeadlockDetArray[GET_MUX_INDEX(id)] = NULL;
        }
#endif
#ifdef LOSCFG_KERNEL_SEMDEP
        if (type == LOCK_SEM) {
            g_semDeadlockDetArray[GET_MUX_INDEX(id)] = NULL;
        }
#endif
    }
}

VOID OsLockDepCheckIn(enum LockType type, LosLockCheck *lock, UINT32 id)
{
    if ((type >= LOCK_TYPE_MAX) || (lock == NULL)) {
        return;
    }

    UINT32 intSave;
    enum LockDepErrType checkResult = LOCKDEP_SUCCESS;
    VOID *requestAddr = (VOID *)__builtin_return_address(0);
    LosTaskCB *current = OsCurrTaskGet();
    LockDep *lockDep = &current->lockDep[lock->type];
    LosTaskCB *lockOwner = NULL;
    OsLockDepRequire(lock->type, &intSave);

    if (lockDep->lockDepth >= (INT32)MAX_LOCK_DEPTH) {
        checkResult = LOCKDEP_ERR_OVERFLOW;
        goto OUT;
    }

    lockOwner = lock->owner;
    /* not owned by any tasks yet, not doing following checks */
    if (lockOwner == NULL) {
        goto OUT;
    }

    if (OSCheckDoubleLock(current, lock)) {
        checkResult = LOCKDEP_ERR_DOUBLE_LOCK;
        goto OUT;
    }

    if (OsCheckNoncircular(current, lock)) {
        checkResult = LOCKDEP_ERR_DEAD_LOCK;
        goto OUT;
    }

OUT:
    if (checkResult == LOCKDEP_SUCCESS) {
        /*
         * though the check may succeed, the waitLock still need to be set.
         * because the OsLockDepCheckIn and OsLockDepRecord is not strictly multi-core
         * sequential, there would be more than two tasks can pass the checking, but
         * only one task can successfully obtain the lock.
         */
        lockDep->waitLock = lock;
        lockDep->heldLocks[lockDep->lockDepth].lockAddr = requestAddr;
        lockDep->heldLocks[lockDep->lockDepth].waitTime = HalClockGetCycles(); /* start time */
    } else {
        OsLockDepDumpLock(current, lock, requestAddr, checkResult);
    }
    OsLockDepRelease(lock->type, intSave);
}

VOID OsLockDepRecord(enum LockType type, LosLockCheck *lock, UINT32 id)
{
    if ((type >= LOCK_TYPE_MAX) || (lock == NULL)) {
        return;
    }

    UINT32 intSave;
    UINT64 cycles;
    LosTaskCB *current = OsCurrTaskGet();
    LockDep *lockDep = &current->lockDep[lock->type];
    HeldLocks *heldlock = &lockDep->heldLocks[lockDep->lockDepth];

    OsLockDepRequire(lock->type, &intSave);
    /*
     * OsLockDepCheckIn records start time t1, after the lock is obtained, we
     * get the time t2, (t2 - t1) is the time of waiting for the lock
     */
    cycles = HalClockGetCycles();
    heldlock->waitTime = cycles - heldlock->waitTime;
    heldlock->holdTime = cycles;

    /* record lock info */
    lock->owner = current;
    lock->cpuId = ArchCurrCpuid();

    /* record lockdep info */
    heldlock->lockPtr = lock;
    lockDep->lockDepth++;
    lockDep->waitLock = NULL;

    OsLockDepRelease(lock->type, intSave);
}

VOID OsLockDepCheckOut(enum LockType type, LosLockCheck *lock, UINT32 id)
{
    if ((type >= LOCK_TYPE_MAX) || (lock == NULL)) {
        return;
    }

    UINT32 intSave;
    INT32 depth;
    VOID *requestAddr = (VOID *)__builtin_return_address(0);
    LosTaskCB *current = OsCurrTaskGet();
    LosTaskCB *owner = NULL;
    LockDep *lockDep = NULL;
    HeldLocks *heldlocks = NULL;

    OsLockDepRequire(lock->type, &intSave);

    owner = lock->owner;
    if (owner == NULL) {
        OsLockDepDumpLock(current, lock, requestAddr, LOCKDEP_ERR_UNLOCK_WITHOUT_LOCK);
        goto OUT;
    }

    lockDep = &owner->lockDep[lock->type];
    heldlocks = &lockDep->heldLocks[0];
    depth = lockDep->lockDepth;

    /* find the lock position */
    while (--depth >= 0) {
        if (heldlocks[depth].lockPtr == lock) {
            break;
        }
    }

    LOS_ASSERT(depth >= 0);

    /* record lock holding time */
    heldlocks[depth].holdTime = HalClockGetCycles() - heldlocks[depth].holdTime;

    /* if unlock an older lock, needs move heldLock records */
    while (depth < (lockDep->lockDepth - 1)) {
        lockDep->heldLocks[depth] = lockDep->heldLocks[depth + 1];
        depth++;
    }

    lockDep->lockDepth--;
    lock->cpuId = 0xFFFFFFFF;
    lock->owner = NULL;

OUT:
    OsLockDepRelease(lock->type, intSave);
}
