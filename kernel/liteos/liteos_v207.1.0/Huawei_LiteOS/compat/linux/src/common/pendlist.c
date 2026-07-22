/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: pendlist for linux adapter
 * Author: Huawei LiteOS Team
 * Create: 2021-07-14
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
#include "pendlist.h"

/* wake pendlist task */
VOID WakeupFromPendList(LOS_DL_LIST *list, UINT32 nodeCnt)
{
    LosTaskCB *tcb = NULL;

    if (list == NULL) {
        return;
    }

    UINT32 wakeCnt = 0;
    while (!LOS_ListEmpty(list)) {
        tcb = OS_TCB_FROM_PENDLIST(LOS_DL_LIST_FIRST(list));
        OsSchedWake(tcb);

        /* count wake-up nodes */
        if ((nodeCnt != ALL_NODES_OF_LIST) && ((++wakeCnt) == nodeCnt)) {
            break;
        }
    }
}


/*
 * add running task to specified pendlist and pend timeout ticks and return remain ticks
 * This interface already holds the g_taskSpin lock, and using print will cause a deadlock.
 */
UINT32 BlockOnPendList(LOS_DL_LIST *list, UINT32 timeout)
{
    LosTaskCB *task = OsCurrTaskGet();

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    if (timeout == LOS_NO_WAIT) {
        return 0;
    }

    if (list == NULL) {
        return 0;
    }

    UINT64 startTicks = LOS_TickCountGet();

    OsSchedWait(task, list, timeout);

    /* timeout */
    if (task->taskStatus & OS_TASK_STATUS_TIMEOUT) {
        task->taskStatus &= ~OS_TASK_STATUS_TIMEOUT;
        return 0;
    }

    UINT64 usedTicks = LOS_TickCountGet() - startTicks;

    /* less than timeout ticks, return remain ticks, else return 1 tick */
    return usedTicks < timeout ? (timeout - (UINT32)usedTicks) : 1;
}
