/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Workqueue Implementation
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

#include "linux/workqueue.h"
#include "linux/delay.h"
#include "los_swtmr_pri.h"
#include "los_task_pri.h"
#include "common/pendlist.h"
#include "los_mp_pri.h"
#include "los_list.h"
#include "los_init.h"

#define DELAY_TIME  10

struct workqueue_struct *g_pstSystemWq = NULL;

/* spinlock for workqueue module only available on SMP mode */
LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_workqueueSpin);

STATIC cpu_workqueue_struct *InitCpuWorkqueue(struct workqueue_struct *wq, INT32 cpu);
STATIC UINT32 CreateWorkqueueThread(cpu_workqueue_struct *cwq, INT32 cpu);
STATIC VOID WorkerThread(VOID *cwq);
STATIC VOID RunWorkqueue(cpu_workqueue_struct *cwq);
STATIC VOID DelayedWorkTimerFunc(struct timer_list *timer);
typedef BOOL (*OsSortLinkCond)(const LosSwtmrCB *swtmr, const struct delayed_work *dwork);

/*
 * @ingroup workqueue
 * Obtain the first work in a workqueue.
 */
#define WORKLIST_ENTRY(ptr, type, member) ((type *)((CHAR *)(ptr) - ((UINTPTR)&(((type *)0)->member))))

/*
 * @ingroup workqueue
 * Traverse a workqueue.
 */
#define LIST_FOR_WORK_DEL(pos, nextNode, listObject, type, field)            \
    for ((pos) = LOS_DL_LIST_ENTRY((listObject)->pstNext, type, field),      \
         (nextNode) = LOS_DL_LIST_ENTRY((pos)->field.pstNext, type, field);  \
         &(pos)->field != (listObject);                                      \
         (pos) = (nextNode), (nextNode) = LOS_DL_LIST_ENTRY((pos)->field.pstNext, type, field))


void init_delayed_work(struct delayed_work *dwork, work_func_t func)
{
    if ((dwork == NULL) || (func == NULL)) {
        return;
    }
    INIT_WORK(&(dwork->work), func);
    timer_setup(&dwork->timer, DelayedWorkTimerFunc, TIMER_UNVALID);
    dwork->work.work_status = 0;
    dwork->wq = NULL;
}

STATIC INLINE BOOL WorkqueueIsEmpty(cpu_workqueue_struct *cwq)
{
    return LOS_ListEmpty(&cwq->worklist);
}

struct workqueue_struct *__create_workqueue_key(char *name, int singleThread, int freezeable,
                                                int rt, struct lock_class_key *key, const char *lockName)
{
    struct workqueue_struct *wq = NULL;
    cpu_workqueue_struct *cwq   = NULL;
    UINT32 ret;
    (VOID)key;
    (VOID)lockName;

    if ((name == NULL) || (singleThread == 0)) { /* Current only support single thread workqueue */
        return NULL;
    }

    wq = (struct workqueue_struct *)LOS_MemAlloc(m_aucSysMem0, sizeof(struct workqueue_struct));
    if (wq == NULL) {
        return NULL;
    }

    wq->cpu_wq = (cpu_workqueue_struct *)LOS_MemAlloc(m_aucSysMem0, sizeof(cpu_workqueue_struct));
    if (wq->cpu_wq == NULL) {
        (VOID)LOS_MemFree(m_aucSysMem0, wq);
        return NULL;
    }

    wq->name = name;
    wq->singlethread = singleThread;
    wq->freezeable = freezeable;
    wq->rt = rt;
    wq->delayed_work_count = 0;
    LOS_ListInit(&wq->list);
    LOS_ListInit(&wq->pendList);

    cwq = InitCpuWorkqueue(wq, singleThread);
    ret = CreateWorkqueueThread(cwq, singleThread);
    if (ret != LOS_OK) {
        destroy_workqueue(wq);
        wq = NULL;
    }

    return wq;
}

struct workqueue_struct *create_singlethread_workqueue(char *name)
{
    return __create_workqueue_key(name, 1, 0, 0, NULL, NULL);
}

UINT32 OsWorkqueueInit(VOID)
{
    g_pstSystemWq = create_workqueue("system_wq");
    if (g_pstSystemWq == NULL) {
        PRINT_ERR("create system workqueue g_pstSystemWq error\n");
        return LOS_NOK;
    }
    return LOS_OK;
}
LOS_SYS_INIT(OsWorkqueueInit, SYS_INIT_LEVEL_COMPONENT, SYS_INIT_SYNC_0);

STATIC cpu_workqueue_struct *InitCpuWorkqueue(struct workqueue_struct *wq, INT32 cpu)
{
    cpu_workqueue_struct *cwq = wq->cpu_wq;
    (VOID)cpu;

    cwq->wq = wq;
    LOS_ListInit(&cwq->worklist);

    return cwq;
}

STATIC UINT32 CreateWorkqueueThread(cpu_workqueue_struct *cwq, INT32 cpu)
{
    struct workqueue_struct *wq = cwq->wq;
    TSK_INIT_PARAM_S taskInitParam = {0};
    UINT32 ret;
    (VOID)cpu;

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)WorkerThread;
    taskInitParam.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.pcName       = wq->name;
    taskInitParam.usTaskPrio   = 1;
    taskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, cwq);

    ret = LOS_TaskCreate(&cwq->wq->wq_id, &taskInitParam);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    cwq->thread = (task_struct*)OS_TCB_FROM_TID(cwq->wq->wq_id);

    return LOS_OK;
}

STATIC VOID WorkerThread(VOID *cwqParam)
{
    UINT32 intSave;
    cpu_workqueue_struct *cwq = (cpu_workqueue_struct *)cwqParam;

    for (;;) {
        SCHEDULER_LOCK(intSave);
        if (WorkqueueIsEmpty(cwq)) {
            (VOID)BlockOnPendList(&(cwq->wq->pendList), LOS_WAIT_FOREVER);
        }
        SCHEDULER_UNLOCK(intSave);
        RunWorkqueue(cwq);
    }
}

STATIC VOID RunWorkqueue(cpu_workqueue_struct *cwq)
{
    struct work_struct *work = NULL;
    UINT32 intSave;
    work_func_t func = NULL;

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);

    if (!WorkqueueIsEmpty(cwq)) {
        work = WORKLIST_ENTRY(cwq->worklist.pstNext, struct work_struct, entry);
        work->work_status = WORK_STRUCT_RUNNING;
        LOS_ListDelInit(cwq->worklist.pstNext);
        func = work->func;

        cwq->current_work = work;
        LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
        func(work);
        LOS_SpinLockSave(&g_workqueueSpin, &intSave);
        cwq->current_work = NULL;

        if (work->work_status & WORK_STRUCT_RUNNING) {
            work->work_status &= ~WORK_STRUCT_RUNNING;
        }
    }

    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
}

STATIC VOID QueueWorkOn(struct workqueue_struct *wq, struct work_struct *work)
{
    UINT32 intSave;

    work->work_status |= WORK_STRUCT_PENDING;
    LOS_ListTailInsert(&(wq->cpu_wq->worklist), &work->entry);

    SCHEDULER_LOCK(intSave);
    WakeupFromPendList(&(wq->cpu_wq->wq->pendList), ALL_NODES_OF_LIST);
    SCHEDULER_UNLOCK(intSave);
    LOS_MpSchedule(OS_MP_CPU_ALL);
    LOS_Schedule();
}

bool queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
    bool ret = TRUE;
    UINT32 intSave;

    if ((wq == NULL) || (wq->name == NULL) || (work == NULL)) {
        return FALSE;
    }

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);
    if ((work->work_status & WORK_STRUCT_PENDING) == WORK_STRUCT_PENDING) {
        ret = FALSE;
        goto UNLOCK_RETURN;
    }

    QueueWorkOn(wq, work);

UNLOCK_RETURN:
    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    return ret;
}

bool cancel_work_sync(struct work_struct *work)
{
    bool ret = FALSE;

    if (work == NULL) {
        return FALSE;
    }

    if (!work->work_status) {
        ret = FALSE;
    } else if (work->work_status & WORK_STRUCT_RUNNING) {
        ret = FALSE;
    } else {
        ret = TRUE;
    }
    while (work->work_status) {
        msleep(DELAY_TIME);
    }
    return ret;
}

bool flush_work(struct work_struct *work)
{
    if (work == NULL) {
        return FALSE;
    }

    if ((work->work_status & WORK_STRUCT_PENDING) || (work->work_status & WORK_STRUCT_RUNNING)) {
        while (work->work_status) {
            msleep(DELAY_TIME);
        }
        return TRUE;
    }
    return FALSE;
}

STATIC VOID DelayedWorkTimerFunc(struct timer_list *timer)
{
    struct delayed_work *dwork = from_timer(dwork, timer, timer);
    UINT32 intSave;

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);

    /* It should have been called from irqsafe timer with irq already off. */
    dwork->wq->delayed_work_count--;
    (VOID)del_timer(&dwork->timer);

    QueueWorkOn(dwork->wq, &dwork->work);

    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
}

STATIC BOOL OsPerCpuSortLinkSearch(SortLinkAttribute *swtmrSortLink, OsSortLinkCond checkFunc, const VOID *arg)
{
    UINT32 i;
    LosSwtmrCB *curSwtmr = NULL;
    SortLinkList *listSorted = NULL;
    LOS_DL_LIST *listObject = NULL;

    for (i = 0; i < OS_TSK_SORTLINK_LEN; i++) {
        listObject = swtmrSortLink->sortLink + i;
        if (LOS_ListEmpty(listObject)) {
            continue;
        }
        listSorted = LOS_DL_LIST_ENTRY((listObject)->pstNext, SortLinkList, sortLinkNode);
        do {
            curSwtmr = LOS_DL_LIST_ENTRY(listSorted, LosSwtmrCB, sortList);
            if (checkFunc(curSwtmr, arg)) {
                return TRUE;
            }
            listSorted = LOS_DL_LIST_ENTRY(listSorted->sortLinkNode.pstNext, SortLinkList, sortLinkNode);
        } while (&listSorted->sortLinkNode != (listObject));
    }
    return FALSE;
}

STATIC BOOL OsSortLinkSearch(OsSortLinkCond checkFunc, const VOID *arg)
{
    UINT32 i;
    SortLinkAttribute *swtmrSortLink = NULL;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        swtmrSortLink = &OsPercpuGetByID(i)->swtmrSortLink;
        if (OsPerCpuSortLinkSearch(swtmrSortLink, checkFunc, arg)) {
            return TRUE;
        }
    }
    return FALSE;
}

STATIC BOOL OsDelayWorkQueueCond(const LosSwtmrCB *swtmr, const struct delayed_work *dwork)
{
    return (((struct delayed_work *)swtmr->arg) == dwork);
}

bool queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork, unsigned long delayTime)
{
    UINT32 intSave, intSave1;
    bool ret;

    if ((wq == NULL) || (wq->name == NULL) || (wq->cpu_wq == NULL) || (dwork == NULL)) {
        return FALSE;
    }

    dwork->wq = wq;
    if (delayTime == 0) {
        return queue_work(dwork->wq, &dwork->work);
    }

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);
    LOS_SpinLockSave(&g_swtmrSpin, &intSave1);
    if (OsSortLinkSearch(OsDelayWorkQueueCond, dwork)) {
        LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);
        ret = FALSE;
        goto UNLOCK_RETURN;
    }
    LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);

    if (dwork->work.work_status & WORK_STRUCT_PENDING) {
        ret = FALSE;
        goto UNLOCK_RETURN;
    }

    dwork->timer.expires = delayTime + jiffies;
    add_timer(&dwork->timer);
    wq->delayed_work_count++;
    dwork->work.work_status |= WORK_STRUCT_PENDING;
    ret = TRUE;

UNLOCK_RETURN:
    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    return ret;
}


STATIC BOOL OsDelayWorkCancelCond(const LosSwtmrCB *swtmr, const struct delayed_work *dwork)
{
    if ((swtmr->timerId == dwork->timer.timerid) &&
        (swtmr->state == OS_SWTMR_STATUS_TICKING)) {
        return TRUE;
    }
    return FALSE;
}

bool cancel_delayed_work(struct delayed_work *dwork)
{
    struct work_struct *work = NULL;
    struct work_struct *workNext = NULL;
    UINT32 intSave, intSave1;
    bool ret = FALSE;

    if ((dwork == NULL) || (dwork->wq == NULL) || (dwork->wq->cpu_wq == NULL)) {
        return FALSE;
    }

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);
    LOS_SpinLockSave(&g_swtmrSpin, &intSave1);
    if (OsSortLinkSearch(OsDelayWorkCancelCond, dwork)) {
        LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);
        (VOID)del_timer(&dwork->timer);
        dwork->work.work_status &= ~WORK_STRUCT_PENDING;
        dwork->wq->delayed_work_count--;
        LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
        return TRUE;
    }

    LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);
    if (dwork->work.work_status & WORK_STRUCT_RUNNING) {
        ret = FALSE;
    } else if (dwork->work.work_status & WORK_STRUCT_PENDING) {
        LIST_FOR_WORK_DEL(work, workNext, &dwork->wq->cpu_wq->worklist, struct work_struct, entry) {
            if (work == &dwork->work) {
                LOS_ListDelInit(&work->entry);
                dwork->work.work_status &= ~WORK_STRUCT_PENDING;
                ret = TRUE;
                break;
            }
        }
    }

    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    return ret;
}

bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
    return cancel_delayed_work(dwork);
}

bool flush_delayed_work(struct delayed_work *dwork)
{
    UINT32 intSave, intSave1;
    bool ret = TRUE;

    if ((dwork == NULL) || (dwork->wq == NULL)) {
        return FALSE;
    }

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);
    LOS_SpinLockSave(&g_swtmrSpin, &intSave1);
    if (OsSortLinkSearch(OsDelayWorkCancelCond, dwork)) {
        LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);
        (VOID)del_timer(&dwork->timer);
        dwork->wq->delayed_work_count--;
        QueueWorkOn(dwork->wq, &dwork->work);
        LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    } else {
        if (!(dwork->work.work_status & (WORK_STRUCT_PENDING | WORK_STRUCT_RUNNING))) {
            ret = FALSE;
        }
        LOS_SpinUnlockRestore(&g_swtmrSpin, intSave1);
        LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    }
    (VOID)flush_work(&dwork->work);
    return ret;
}

unsigned int work_busy(struct work_struct *work)
{
    UINT32 ret = 0;

    if (work == NULL) {
        return FALSE;
    }

    if (work->work_status & WORK_STRUCT_PENDING) {
        ret |= WORK_BUSY_PENDING;
    }
    if (work->work_status & WORK_STRUCT_RUNNING) {
        ret |= WORK_BUSY_RUNNING;
    }
    return ret;
}

static void drain_workqueue(struct workqueue_struct *wq)
{
    UINT32 intSave;
    while (1) {
        msleep(DELAY_TIME);
        LOS_SpinLockSave(&g_workqueueSpin, &intSave);
        if (WorkqueueIsEmpty(wq->cpu_wq) && (wq->delayed_work_count == 0)) {
            break;
        }

        LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
    }
    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
}

void destroy_workqueue(struct workqueue_struct *wq)
{
    UINT32 intSave;
    if ((wq == NULL) || (wq->cpu_wq == NULL)) {
        return;
    }

    /* Drain it before proceeding with destruction */
    drain_workqueue(wq);

    (VOID)LOS_TaskDelete(wq->wq_id);

    LOS_SpinLockSave(&g_workqueueSpin, &intSave);
    wq->name = NULL;
    LOS_ListDelInit(&wq->list);
    LOS_ListDelInit(&wq->pendList);

    (VOID)LOS_MemFree(m_aucSysMem0, wq->cpu_wq);
    (VOID)LOS_MemFree(m_aucSysMem0, wq);
    LOS_SpinUnlockRestore(&g_workqueueSpin, intSave);
}
