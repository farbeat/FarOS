/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Pthread Mutex Deadlock Check
 * Author: Huawei LiteOS Team
 * Create: 2022-07-23
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

#include "pthread_mux_debug_pri.h"

LOS_DL_LIST g_pthreadMuxList;

STATIC LosPthreadMuxCheck *OsLockCheckInit(VOID)
{
    LosPthreadMuxCheck *lockCheck = NULL;

    lockCheck = (LosPthreadMuxCheck *)LOS_MemAlloc(m_aucSysMem1, sizeof(LosPthreadMuxCheck));
    if (lockCheck == NULL) {
        goto ERROR_OUT;
    }

    lockCheck->deadLockDet.name = (CHAR *)LOS_MemAlloc(m_aucSysMem1, NAME_BUFF_SIZE);
    if (lockCheck->deadLockDet.name == NULL) {
        goto ERROR_FREE;
    }
    lockCheck->muxCB = NULL;
    lockCheck->deadLockDet.cpuId = 0xFFFFFFFF;
    lockCheck->deadLockDet.owner = NULL;
    lockCheck->deadLockDet.type = LOCK_PTHREAD;

    return lockCheck;

ERROR_FREE:
    (VOID)LOS_MemFree(m_aucSysMem1, lockCheck);

ERROR_OUT:
    PRINT_ERR("%s: malloc failed!\n", __FUNCTION__);
    return NULL;
}

VOID OsLockDepListInit(VOID)
{
    LosPthreadMuxCheck *lockCheck = NULL;

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    LOS_SpinUnlockNoSched(&g_taskSpin);
    lockCheck = OsLockCheckInit();
    LOS_SpinLock(&g_taskSpin);
    if (lockCheck == NULL) {
        return;
    }

    if ((g_pthreadMuxList.pstPrev == NULL) || (g_pthreadMuxList.pstNext == NULL)) {
        LOS_ListInit(&g_pthreadMuxList);
    }

    LOS_ListInit(&lockCheck->muxList);
    LOS_ListTailInsert(&g_pthreadMuxList, &lockCheck->muxList);
}

LosLockCheck *OsLockDepPthreadMuxGet(MuxBaseCB *muxCB)
{
    LosPthreadMuxCheck *lockCheck = NULL;

    if (muxCB == NULL) {
        return NULL;
    }

    if (LOS_ListEmpty(&g_pthreadMuxList) == TRUE) {
        return NULL;
    } else {
        LOS_DL_LIST_FOR_EACH_ENTRY(lockCheck, &g_pthreadMuxList, LosPthreadMuxCheck, muxList) {
            if (lockCheck->muxCB == muxCB) {
                return &lockCheck->deadLockDet;
            }
        }
        return NULL;
    }
}

VOID OsLockDepPthreadMuxInsert(MuxBaseCB *muxCB)
{
    LosPthreadMuxCheck *lockCheck = NULL;
    int ret;
    if (muxCB == NULL) {
        return;
    }

    LOS_DL_LIST_FOR_EACH_ENTRY(lockCheck, &g_pthreadMuxList, LosPthreadMuxCheck, muxList) {
        if (lockCheck->muxCB == NULL) {
            lockCheck->muxCB = muxCB;
            ret = sprintf_s(lockCheck->deadLockDet.name, NAME_BUFF_SIZE, "%#08x", muxCB);
            if (ret < 0) {
                return;
            }
        }
    }
}

VOID OsLockDepPthreadMuxDelete(MuxBaseCB *muxCB)
{
    LosPthreadMuxCheck *lockCheck = NULL;

    LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));

    if (muxCB == NULL) {
        return;
    }

    if (LOS_ListEmpty(&g_pthreadMuxList) == TRUE) {
        return;
    } else {
        LOS_DL_LIST_FOR_EACH_ENTRY(lockCheck, &g_pthreadMuxList, LosPthreadMuxCheck, muxList) {
            if (lockCheck->muxCB == muxCB) {
                LOS_ListDelete(&lockCheck->muxList);
                LOS_SpinUnlockNoSched(&g_taskSpin);
                (VOID)LOS_MemFree(m_aucSysMem1, lockCheck->deadLockDet.name);
                (VOID)LOS_MemFree(m_aucSysMem1, lockCheck);
                LOS_SpinLock(&g_taskSpin);
                return;
            }
        }
    }
}
