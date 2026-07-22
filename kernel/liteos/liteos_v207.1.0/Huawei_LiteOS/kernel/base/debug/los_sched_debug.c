/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: Schedule Statistics
 * Author: Huawei LiteOS Team
 * Create: 2018-11-16
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

#include "los_task_pri.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#else
#include "asm/interrupt_config.h"
#endif

#define HIGHTASKPRI           16
#define NS_PER_MS             1000000
#define DECIMAL_TO_PERCENTAGE 100

typedef struct {
    UINT64      idleRuntime;
    UINT64      idleStarttime;
    UINT64      highTaskRuntime;
    UINT64      highTaskStarttime;
    UINT64      sumPriority;
    UINT32      prioritySwitch;
    UINT32      highTaskSwitch;
    UINT32      contextSwitch;
    UINT32      hwiNum;
#ifdef LOSCFG_KERNEL_SMP
    UINT32      ipiIrqNum;
    UINT32      migrationIn;
#endif
} StatPercpu;

STATIC BOOL g_statisticsStartFlag = FALSE;
STATIC UINT64 g_statisticsStartTime;
STATIC UINT64 g_statisticsPastTime = 0;
STATIC StatPercpu g_statPercpu[LOSCFG_KERNEL_CORE_NUM];

STATIC VOID OsSchedStatPerCpu(const LosTaskCB *runTask, const LosTaskCB *newTask,
                              UINT32 cpuId, UINT64 currTime)
{
    UINT32 idleTaskId;
    UINT64 runtime;

    idleTaskId = OsGetIdleTaskId();

    g_statPercpu[cpuId].contextSwitch++;

    if ((runTask->taskId != idleTaskId) && (newTask->taskId == idleTaskId)) {
        g_statPercpu[cpuId].idleStarttime = currTime;
    }

    if ((runTask->taskId == idleTaskId) && (newTask->taskId != idleTaskId)) {
        runtime = currTime - g_statPercpu[cpuId].idleStarttime;
        g_statPercpu[cpuId].idleRuntime += runtime;
        g_statPercpu[cpuId].idleStarttime = 0;
    }

    if ((runTask->priority >= HIGHTASKPRI) && (newTask->priority < HIGHTASKPRI)) {
        g_statPercpu[cpuId].highTaskStarttime = currTime;
    }

    if ((runTask->priority < HIGHTASKPRI) && (newTask->priority >= HIGHTASKPRI)) {
        runtime = currTime - g_statPercpu[cpuId].highTaskStarttime;
        g_statPercpu[cpuId].highTaskRuntime += runtime;
        g_statPercpu[cpuId].highTaskStarttime = 0;
    }

    if (newTask->priority < HIGHTASKPRI) {
        g_statPercpu[cpuId].highTaskSwitch++;
    }

    if (newTask->taskId != idleTaskId) {
        g_statPercpu[cpuId].sumPriority += newTask->priority;
        g_statPercpu[cpuId].prioritySwitch++;
    }

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStat(LosTaskCB *runTask, LosTaskCB *newTask)
{
    UINT64 now, runtime;
    UINT32 cpuId;

    if (g_statisticsStartFlag != TRUE) {
        return;
    }

    cpuId = ArchCurrCpuid();
    now = LOS_CurrNanosec();

    SchedStat *schedRun = &runTask->schedStat;
    SchedStat *schedNew = &newTask->schedStat;
    SchedPercpu *cpuRun = &schedRun->schedPercpu[cpuId];
    SchedPercpu *cpuNew = &schedNew->schedPercpu[cpuId];

    /* calculate one chance of running time */
    runtime = now - schedRun->startRuntime;

    /* add runtime to running task statistics */
    cpuRun->runtime += runtime;
    schedRun->allRuntime += runtime;

    /* add context switch counters and schedule start time */
    cpuNew->contextSwitch++;
    schedNew->allContextSwitch++;
    schedNew->startRuntime = now;
    OsSchedStatPerCpu(runTask, newTask, cpuId, now);
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatHwi(size_t intNum)
{
    UINT32 cpuId = ArchCurrCpuid();

    if ((g_statisticsStartFlag != TRUE) || (intNum == OS_TICK_INT_NUM)) {
        return;
    }

    g_statPercpu[cpuId].hwiNum++;
#ifdef LOSCFG_KERNEL_SMP
    /* 16: 0~15 is ipi interrupts */
    if (intNum < 16) {
        g_statPercpu[cpuId].ipiIrqNum++;
    }
#endif
    return;
}

#ifdef LOSCFG_KERNEL_SMP
LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatMgr(LosTaskCB *task)
{
    UINT32 currCpuId = task->currCpu;
    SchedStat *schedNew = &task->schedStat;
    SchedPercpu *cpuNew = &schedNew->schedPercpu[currCpuId];

    if (g_statisticsStartFlag != TRUE) {
        return;
    }

    g_statPercpu[currCpuId].migrationIn++;
    schedNew->allMigration++;
    cpuNew->migrationIn++;
}
#endif

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatPause(VOID)
{
    UINT64 runtime;
    LosTaskCB *taskNow = NULL;
    UINT32 cpuId = ArchCurrCpuid();
    UINT64 now = LOS_CurrNanosec();

    if (g_statisticsStartFlag != TRUE) {
        return;
    }

    taskNow = OsCurrTaskGet();

    SchedStat *schedRun = &taskNow->schedStat;
    SchedPercpu *cpuRun = &schedRun->schedPercpu[cpuId];

    /* calculate one chance of running time */
    runtime = now - schedRun->startRuntime;

    /* add runtime to running task statistics */
    cpuRun->runtime += runtime;
    schedRun->allRuntime += runtime;

    if (taskNow->taskId == OsGetIdleTaskId()) {
        runtime = now - g_statPercpu[cpuId].idleStarttime;
        g_statPercpu[cpuId].idleRuntime += runtime;
    }

    if (taskNow->priority < HIGHTASKPRI) {
        runtime = now - g_statPercpu[cpuId].highTaskStarttime;
        g_statPercpu[cpuId].highTaskRuntime += runtime;
    }
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatResume(VOID)
{
    LosTaskCB *taskNow = NULL;
    UINT64 now = LOS_CurrNanosec();
    UINT32 cpuId = ArchCurrCpuid();

    if (g_statisticsStartFlag != TRUE) {
        return;
    }

    taskNow = OsCurrTaskGet();
    SchedStat *schedRun = &taskNow->schedStat;

    /* restart counting */
    schedRun->startRuntime = now;
    if (taskNow->taskId == OsGetIdleTaskId()) {
        g_statPercpu[cpuId].idleStarttime = now;
    }

    if (taskNow->priority < HIGHTASKPRI) {
        g_statPercpu[cpuId].highTaskStarttime = now;
    }
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsSchedStatCpuInfoGet(SchedStatCpuInfo *info, UINT32 cpuId)
{
    if ((info == NULL) || (cpuId >= LOSCFG_KERNEL_CORE_NUM)) {
        return LOS_NOK;
    }

    info->passTime = (DOUBLE)g_statisticsPastTime / NS_PER_MS;
    info->idleTaskTimePercent = ((DOUBLE)(g_statPercpu[cpuId].idleRuntime) / g_statisticsPastTime) *
                                DECIMAL_TO_PERCENTAGE;
    info->taskSwiNum = g_statPercpu[cpuId].contextSwitch;
    info->hwiNum = g_statPercpu[cpuId].hwiNum;
    info->noIdleTaskSwiAvgPri = (g_statPercpu[cpuId].prioritySwitch == 0) ? LOS_TASK_PRIORITY_LOWEST :
                                ((DOUBLE)(g_statPercpu[cpuId].sumPriority) / (g_statPercpu[cpuId].prioritySwitch));
    info->hiTaskTimePercent = ((DOUBLE)(g_statPercpu[cpuId].highTaskRuntime) /g_statisticsPastTime) *
                              DECIMAL_TO_PERCENTAGE;
    info->hiTaskSwiNum = g_statPercpu[cpuId].highTaskSwitch;
    info->hiTaskAvgRunTime = (g_statPercpu[cpuId].highTaskSwitch == 0) ? 0 :
                             ((DOUBLE)(g_statPercpu[cpuId].highTaskRuntime) / (g_statPercpu[cpuId].highTaskSwitch)) /
                             NS_PER_MS;
#ifdef LOSCFG_KERNEL_SMP
    info->smpHwiNum = g_statPercpu[cpuId].ipiIrqNum;
    info->smpMigrationNum = g_statPercpu[cpuId].migrationIn;
#endif
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR UINT32 OsSchedStatTaskInfoGet(SchedStatTaskInfo *info, UINT32 taskId)
{
    LosTaskCB *taskCB = NULL;
    UINT32 cpuId;

    if ((info == NULL) || (taskId >= g_taskMaxNum)) {
        return LOS_NOK;
    }

    taskCB = (((LosTaskCB *)g_osTaskCBArray) + taskId);
    if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
        return LOS_NOK;
    }

    info->taskName = taskCB->taskName;
    info->passTime = (DOUBLE)g_statisticsPastTime / NS_PER_MS;
    info->allRuntime = (DOUBLE)(taskCB->schedStat.allRuntime) / NS_PER_MS;
    info->allTaskSwiNum = taskCB->schedStat.allContextSwitch;
#ifdef LOSCFG_KERNEL_SMP
    info->allSmpMigrationNum = taskCB->schedStat.allMigration;
#endif
    info->taskUseCpuNum = 0;
    for (cpuId = 0; cpuId < LOSCFG_KERNEL_CORE_NUM; cpuId++) {
#ifdef LOSCFG_KERNEL_SMP
        if (!((1U << cpuId) & (UINT32)taskCB->cpuAffiMask)) {
            continue;
        }
#endif
        info->cpuInfo[info->taskUseCpuNum].cpuId = cpuId;
        info->cpuInfo[info->taskUseCpuNum].cpuRunTime = (DOUBLE)(taskCB->schedStat.schedPercpu[cpuId].runtime) /
                                                        NS_PER_MS;
        info->cpuInfo[info->taskUseCpuNum].cpuTaskSwiNum = taskCB->schedStat.schedPercpu[cpuId].contextSwitch;
#ifdef LOSCFG_KERNEL_SMP
        info->cpuInfo[info->taskUseCpuNum].cpuSmpMigrationNum = taskCB->schedStat.schedPercpu[cpuId].migrationIn;
#endif
        info->taskUseCpuNum++;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatShowNamePrint(VOID)
{
    PRINTK("CPU       Idle(%%)    ContextSwitch      HwiNum      "
           "Avg Pri      HiTask(%%)        HiTask SwiNum     HiTask P(ms)"
#ifdef LOSCFG_KERNEL_SMP
           "        MP Hwi        MGR"
#endif
           "\n");
    PRINTK("----    ---------      -----------    --------    ---------     "
           "----------         ------------       ----------"
#ifdef LOSCFG_KERNEL_SMP
           "        ------     ------"
#endif
           "\n");
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatInfoNamePrint(VOID)
{
    PRINTK("Task                          TID              Total Time     Total CST"
#ifdef LOSCFG_KERNEL_SMP
           "     Total MGR"
#endif
           "     CPU                   Time           CST"
#ifdef LOSCFG_KERNEL_SMP
           "           MGR"
#endif
           "\n");
    PRINTK("----                          ---      ------------------    ----------"
#ifdef LOSCFG_KERNEL_SMP
           "    ----------"
#endif
           "    ----     ------------------    ----------"
#ifdef LOSCFG_KERNEL_SMP
           "    ----------"
#endif
           "\n");
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatInfo(VOID)
{
    UINT32 loop, cpuId, ret;
    SchedStatTaskInfo taskInfo;

    PRINTK("\nPassed Time: %+16lf ms\n", ((DOUBLE)g_statisticsPastTime / NS_PER_MS));
    PRINTK("--------------------------------\n");
    OsSchedStatInfoNamePrint();

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        (VOID)memset(&taskInfo, 0, sizeof(taskInfo));
        ret = OsSchedStatTaskInfoGet(&taskInfo, loop);
        if (ret != LOS_OK) {
            continue;
        }

        PRINTK("%-30s0x%-6x%+16lf ms    %10u"
#ifdef LOSCFG_KERNEL_SMP
               "    %10u"
#endif
               "\n", taskInfo.taskName, loop, taskInfo.allRuntime, taskInfo.allTaskSwiNum
#ifdef LOSCFG_KERNEL_SMP
               , taskInfo.allSmpMigrationNum
#endif
               );
        for (cpuId = 0; cpuId < taskInfo.taskUseCpuNum; cpuId++) {
            PRINTK("                                                                           "
#ifdef LOSCFG_KERNEL_SMP
                   "              "
#endif
                   "CPU%u    %+16lf ms  %12u"
#ifdef LOSCFG_KERNEL_SMP
                   "  %12u"
#endif
                   "\n", taskInfo.cpuInfo[cpuId].cpuId, taskInfo.cpuInfo[cpuId].cpuRunTime,
                   taskInfo.cpuInfo[cpuId].cpuTaskSwiNum

#ifdef LOSCFG_KERNEL_SMP
                   , taskInfo.cpuInfo[cpuId].cpuSmpMigrationNum
#endif
                   );
        }
    }
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatShow(VOID)
{
    UINT32 cpuId, ret;
    SchedStatCpuInfo info;

    PRINTK("\nPassed Time: %+16lf ms\n", ((DOUBLE)g_statisticsPastTime / NS_PER_MS));
    PRINTK("--------------------------------\n");
    OsSchedStatShowNamePrint();

    for (cpuId = 0; cpuId < LOSCFG_KERNEL_CORE_NUM; cpuId++) {
        (VOID)memset(&info, 0, sizeof(info));
        ret = OsSchedStatCpuInfoGet(&info, cpuId);
        if (ret != LOS_OK) {
            continue;
        }
        PRINTK("CPU%u   %+10lf%17u%12u   %+10lf    %+11lf%21u    %+13lf"
#ifdef LOSCFG_KERNEL_SMP
               "    %10u    %7u"
#endif
               "\n", cpuId, info.idleTaskTimePercent, info.taskSwiNum, info.hwiNum,
               info.noIdleTaskSwiAvgPri, info.hiTaskTimePercent, info.hiTaskSwiNum, info.hiTaskAvgRunTime
#ifdef LOSCFG_KERNEL_SMP
               , info.smpHwiNum, info.smpMigrationNum
#endif
               );
    }
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatStart(VOID)
{
    LosTaskCB *taskCB = NULL;
    SchedStat *schedRun = NULL;
    UINT32 loop;
    UINT32 cpuId = 0;
    UINT32 intSave;

    SCHEDULER_LOCK(intSave);

    if (g_statisticsStartFlag) {
        SCHEDULER_UNLOCK(intSave);
        PRINTK("schedule statistic has started\n");
        return;
    }

    (VOID)memset(g_statPercpu, 0, sizeof(g_statPercpu));
    g_statisticsStartTime = LOS_CurrNanosec();

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (((LosTaskCB *)g_osTaskCBArray) + loop);
        if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
#ifdef LOSCFG_KERNEL_SMP
            cpuId = taskCB->currCpu;
#endif
            if (IsIdleTask(taskCB->taskId) == TRUE) {
                g_statPercpu[cpuId].idleStarttime = g_statisticsStartTime;
            } else {
                g_statPercpu[cpuId].sumPriority += taskCB->priority;
                g_statPercpu[cpuId].prioritySwitch++;
            }

            if (taskCB->priority < HIGHTASKPRI) {
                g_statPercpu[cpuId].highTaskStarttime = g_statisticsStartTime;
                g_statPercpu[cpuId].highTaskSwitch++;
            }
        }

        /* reset task sched stats. */
        (VOID)memset(&taskCB->schedStat, 0, sizeof(SchedStat));

        if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
            schedRun = &taskCB->schedStat;
            schedRun->startRuntime = g_statisticsStartTime;
        }
    }
    g_statisticsStartFlag = TRUE;

    SCHEDULER_UNLOCK(intSave);
    PRINTK("schedule statistic start\n");

    return;
}

LITE_OS_SEC_TEXT_MINOR VOID OsSchedStatStop(VOID)
{
    LosTaskCB *taskCB = NULL;
    UINT32 loop;
    UINT32 cpuId = 0;
    UINT64 statisticsStopTime;
    UINT64 runtime;
    UINT32 intSave;

    SCHEDULER_LOCK(intSave);

    if (g_statisticsStartFlag != TRUE) {
        SCHEDULER_UNLOCK(intSave);
        PRINTK("Please set schedule statistic start\n");
        return;
    }

    g_statisticsStartFlag = FALSE;
    statisticsStopTime = LOS_CurrNanosec();
    g_statisticsPastTime = statisticsStopTime - g_statisticsStartTime;
    g_statisticsStartTime = 0;

    for (loop = 0; loop < g_taskMaxNum; loop++) {
        taskCB = (((LosTaskCB *)g_osTaskCBArray) + loop);
        if (taskCB->taskStatus & OS_TASK_STATUS_RUNNING) {
#ifdef LOSCFG_KERNEL_SMP
            cpuId = taskCB->currCpu;
#endif
            if (IsIdleTask(taskCB->taskId) == TRUE) {
                runtime = statisticsStopTime - g_statPercpu[cpuId].idleStarttime;
                g_statPercpu[cpuId].idleRuntime += runtime;
                g_statPercpu[cpuId].idleStarttime = 0;
            }
            if (taskCB->priority < HIGHTASKPRI) {
                runtime = statisticsStopTime - g_statPercpu[cpuId].highTaskStarttime;
                g_statPercpu[cpuId].highTaskRuntime += runtime;
                g_statPercpu[cpuId].highTaskStarttime = 0;
            }
        }
    }

    SCHEDULER_UNLOCK(intSave);
    OsSchedStatShow();

    return;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(start_shellcmd, CMD_TYPE_EX, "schedstatstart", 0, (CmdCallBackFunc)OsSchedStatStart);
SHELLCMD_ENTRY(stop_shellcmd, CMD_TYPE_EX, "schedstatstop", 0, (CmdCallBackFunc)OsSchedStatStop);
SHELLCMD_ENTRY(info_shellcmd, CMD_TYPE_EX, "schedstatinfo", 0, (CmdCallBackFunc)OsSchedStatInfo);
#endif /* LOSCFG_SHELL */
