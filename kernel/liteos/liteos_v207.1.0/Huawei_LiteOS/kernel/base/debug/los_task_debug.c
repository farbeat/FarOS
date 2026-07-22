/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Debug
 * Author: Huawei LiteOS Team
 * Create: 2021-11-08
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
#include "stdio.h"
#include "stdlib.h"
#include "los_config.h"
#include "los_exc.h"
#include "los_memstat_pri.h"
#include "los_sem_pri.h"
#include "los_task_pri.h"
#include "los_exc_pri.h"
#ifdef LOSCFG_KERNEL_CPUP
#include "los_cpup_pri.h"
#endif

#define OS_INVALID_SEM_ID  0xFFFFFFFF

#ifdef LOSCFG_KERNEL_CPUP
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpupAll[LOSCFG_BASE_CORE_TSK_LIMIT];
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpupMultiRecord[LOSCFG_BASE_CORE_TSK_LIMIT];
LITE_OS_SEC_BSS STATIC CPUP_INFO_S g_taskCpupOneRecord[LOSCFG_BASE_CORE_TSK_LIMIT];
#endif

LITE_OS_SEC_BSS STATIC UINT32      g_taskWaterLine[LOSCFG_BASE_CORE_TSK_LIMIT];
LITE_OS_SEC_BSS STATIC LosTaskCB   g_osTaskCBArrayBackup[LOSCFG_BASE_CORE_TSK_LIMIT + 1];
LITE_OS_SEC_DATA LosTaskCB         * const g_taskCBArrayBackup = &g_osTaskCBArrayBackup[0];

typedef struct {
    UINT16 status;
    const CHAR *statusStr;
} TaskStatus;

#ifdef LOSCFG_TASK_JOINABLE
#define TASK_STATUS_TBL_LEN 8
#else
#define TASK_STATUS_TBL_LEN 7
#endif

STATIC TaskStatus g_taskStatusTbl[TASK_STATUS_TBL_LEN] = {
    {OS_TASK_STATUS_RUNNING, "Running"},
    {OS_TASK_STATUS_READY, "Ready"},
    {OS_TASK_STATUS_DELAY, "Delay"},
    {OS_TASK_STATUS_PEND_TIME | OS_TASK_STATUS_SUSPEND, "SuspendTime"},
    {OS_TASK_STATUS_PEND_TIME | OS_TASK_STATUS_PEND, "PendTime"},
    {OS_TASK_STATUS_PEND, "Pend"},
    {OS_TASK_STATUS_SUSPEND, "Suspend"},
#ifdef LOSCFG_TASK_JOINABLE
    {OS_TASK_STATUS_ZOMBIE, "Zombie"},
#endif
};

const CHAR *OsTskStatusConvertStr(UINT16 taskStatus)
{
    UINT32 i;
    for (i = 0; i < TASK_STATUS_TBL_LEN; ++i) {
        if ((taskStatus & g_taskStatusTbl[i].status) == g_taskStatusTbl[i].status) {
            return g_taskStatusTbl[i].statusStr;
        }
    }
    return "Invalid";
}

STATIC VOID OsTaskWaterLineGet(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        (VOID)OsStackWaterLineGet((const UINTPTR *)((UINTPTR)taskCB->topOfStack + taskCB->stackSize),
                                  (const UINTPTR *)taskCB->topOfStack, &g_taskWaterLine[taskCB->taskId]);
    }
}

STATIC VOID OsDebugTskInfoTitle(VOID)
{
    EXCINFO_PRINTK("\r\nName                   TaskEntryAddr       TID    ");
#ifdef LOSCFG_KERNEL_SMP
    EXCINFO_PRINTK("Affi    CPU    ");
#endif
    EXCINFO_PRINTK("Priority   Status       "
                   "StackSize    WaterLine      StackPoint             TopOfStack             SemID        EventMask");

#ifdef LOSCFG_KERNEL_CPUP
    EXCINFO_PRINTK("  CPUP  CPUP%3d.%ds  CPUP%3d.%ds  ", OS_CPUP_MULTI_PERIOD_S, OS_CPUP_MULTI_PERIOD_MS, \
        OS_CPUP_PERIOD_S, OS_CPUP_PERIOD_MS);
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
    EXCINFO_PRINTK("   MEMUSE");
#endif
    EXCINFO_PRINTK("\n");
    EXCINFO_PRINTK("----                   -------------       ---    ");
#ifdef LOSCFG_KERNEL_SMP
    EXCINFO_PRINTK("-----   ----   ");
#endif
    EXCINFO_PRINTK("--------   --------     "
           "---------    ----------     ----------             ----------             ----------   ---------");
#ifdef LOSCFG_KERNEL_CPUP
    EXCINFO_PRINTK("  ----  ----------  ----------  ");
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
    EXCINFO_PRINTK("   ------");
#endif
    EXCINFO_PRINTK("\n");
}

STATIC INLINE UINT32 OsGetSemID(const LosTaskCB *taskCB)
{
    UINT32 semId = OS_INVALID_SEM_ID;

    if (taskCB->taskSem != NULL) {
        semId = ((LosSemCB *)taskCB->taskSem)->semId;
    }

    return semId;
}

/*
 * Some targets use vendor's print function, which doesn't support special format characters like '-' or '.'.
 * And then, we need to redefine it in targets codes.
 * If you modify these format strings, please repair copyies in targets codes at the same time, otherwise,
 * maybe there exists error printf info.
 */
#if !defined(TASK_NAME_FMT)
#define TASK_NAME_FMT ("%-23s0x%-18.*lx0x%-5x")
#endif
#if !defined(TASK_CPU_FMT) && defined(LOSCFG_KERNEL_SMP)
#define TASK_CPU_FMT ("0x%04x  %4d   ")
#endif
#if !defined(TASK_STACK_FMT)
#define TASK_STACK_FMT ("%-11u%-13s0x%-11x0x%-11x  0x%-18.*lx   0x%-18.*lx   0x%-11x")
#endif
#if !defined(TASK_EVENT_FMT) && defined(LOSCFG_BASE_IPC_EVENT)
#define TASK_EVENT_FMT ("0x%-6x")
#endif
#if !defined(TASK_CPUP_FMT) && defined(LOSCFG_KERNEL_CPUP)
#define TASK_CPUP_FMT (" %4u.%1u%7u.%1u%9u.%1u   ")
#endif
#if !defined(TASK_MEM_FMT) && defined(LOSCFG_MEM_TASK_STAT)
#define TASK_MEM_FMT ("       %-11u")
#endif

STATIC VOID OsDebugTskInfoData(const LosTaskCB *allTaskArray)
{
    const LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 semId;

    for (loop = 0; loop < g_taskMaxNum; ++loop) {
        taskCB = allTaskArray + loop;
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        semId = OsGetSemID(taskCB);

        EXCINFO_PRINTK(TASK_NAME_FMT, taskCB->taskName, OS_HEX_ADDR_WIDTH, (UINTPTR)taskCB->taskEntry,
                       taskCB->taskId);

#ifdef LOSCFG_KERNEL_SMP
        EXCINFO_PRINTK(TASK_CPU_FMT, taskCB->cpuAffiMask, (INT16)(taskCB->currCpu));
#endif
        EXCINFO_PRINTK(TASK_STACK_FMT, taskCB->priority,
                       OsTskStatusConvertStr(taskCB->taskStatus), taskCB->stackSize,
                       g_taskWaterLine[taskCB->taskId],
                       OS_HEX_ADDR_WIDTH, (UINTPTR)taskCB->stackPointer, OS_HEX_ADDR_WIDTH, taskCB->topOfStack, semId);

#ifdef LOSCFG_BASE_IPC_EVENT
        EXCINFO_PRINTK(TASK_EVENT_FMT, taskCB->eventMask);
#endif

#ifdef LOSCFG_KERNEL_CPUP
        EXCINFO_PRINTK(TASK_CPUP_FMT,
                       g_taskCpupAll[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
                       g_taskCpupAll[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
                       g_taskCpupMultiRecord[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
                       g_taskCpupMultiRecord[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT,
                       g_taskCpupOneRecord[taskCB->taskId].uwUsage / LOS_CPUP_PRECISION_MULT,
                       g_taskCpupOneRecord[taskCB->taskId].uwUsage % LOS_CPUP_PRECISION_MULT);
#endif /* LOSCFG_KERNEL_CPUP */
#ifdef LOSCFG_MEM_TASK_STAT
        EXCINFO_PRINTK(TASK_MEM_FMT, OsMemTaskUsage(taskCB->taskId));
#endif
        EXCINFO_PRINTK("\n");
    }
}

UINT32 OsDbgTskInfoGet(UINT32 taskId)
{
    BOOL lockFlag = FALSE;
    UINT32 intSave;
    LosTaskCB *tcbArray = g_taskCBArrayBackup;
    size_t size = (LOSCFG_BASE_CORE_TSK_LIMIT + 1) * sizeof(LosTaskCB);

    if (taskId == OS_ALL_TASK_MASK) {
#ifdef LOSCFG_KERNEL_CPUP
        (VOID)memset(g_taskCpupAll, 0, sizeof(g_taskCpupAll));
        (VOID)memset(g_taskCpupMultiRecord, 0, sizeof(g_taskCpupMultiRecord));
        (VOID)memset(g_taskCpupOneRecord, 0, sizeof(g_taskCpupOneRecord));
#endif
        (VOID)memset((VOID *)g_taskWaterLine, 0, sizeof(g_taskWaterLine));

        if (LOS_SpinHeld(&g_taskSpin) == FALSE) {
            SCHEDULER_LOCK(intSave);
            lockFlag = TRUE;
        }
        (VOID)memcpy(tcbArray, g_osTaskCBArray, size);

#ifdef LOSCFG_KERNEL_CPUP
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpupAll, CPUP_ALL_TIME, 1);
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpupMultiRecord, CPUP_LAST_MULIT_RECORD, 1);
        (VOID)LOS_AllCpuUsage(LOSCFG_BASE_CORE_TSK_LIMIT, g_taskCpupOneRecord, CPUP_LAST_ONE_RECORD, 1);
#endif
        OsTaskWaterLineGet(tcbArray);

        if (lockFlag == TRUE) {
            SCHEDULER_UNLOCK(intSave);
        }

        OsDebugTskInfoTitle();
        OsDebugTskInfoData(tcbArray);
    } else {
        OsTaskBackTrace(taskId);
    }

    return LOS_OK;
}
