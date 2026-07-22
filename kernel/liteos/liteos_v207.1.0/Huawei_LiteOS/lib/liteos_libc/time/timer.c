/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Timer file
 * Author: Huawei LiteOS Team
 * Create: 2020-08-28
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

#include "time_pri.h"
#include "time.h"
#include "limits.h"
#include "signal.h"
#include "los_swtmr_pri.h"

int timer_create(clockid_t clockId, struct sigevent *evp, timer_t *timerId)
{
    LosSwtmrCB *swtmr = NULL;
    UINT32 ret;
    UINT16 swtmrId;

    if ((clockId != CLOCK_REALTIME) || (timerId == NULL) || (evp == NULL)) {
        errno = EINVAL;
        return -1;
    }

    if ((evp->sigev_notify == SIGEV_SIGNAL) || (evp->sigev_notify == SIGEV_NONE)) {
        errno = ENOTSUP;
        return -1;
    } else if (evp->sigev_notify != SIGEV_THREAD) {
        errno = EINVAL;
        return -1;
    }

    ret = LOS_SwtmrCreate(1, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)(UINTPTR)evp->sigev_notify_function, &swtmrId,
                          (UINTPTR)evp->sigev_value.sival_int);
    if (ret != LOS_OK) {
        errno = (ret == LOS_ERRNO_SWTMR_MAXSIZE) ? EAGAIN : EINVAL;
        return -1;
    }

    swtmr = OS_SWT_FROM_SWTID(swtmrId);
    *timerId = swtmr;

    return 0;
}

int timer_delete(timer_t timerId)
{
    LosSwtmrCB *swtmr = (LosSwtmrCB *)timerId;
    if (OS_INT_ACTIVE || (timerId == NULL)) {
        goto ERROUT;
    }

    if (LOS_SwtmrDelete(swtmr->timerId)) {
        goto ERROUT;
    }

    return 0;

ERROUT:
    errno = EINVAL;
    return -1;
}

int timer_settime(timer_t timerId, int flags, const struct itimerspec *value, /* new value */
                  struct itimerspec *oldValue)                                /* old value to return, always 0 */
{
    LosSwtmrCB *swtmr = (LosSwtmrCB *)timerId;
    UINT32 interval, expiry, ret;
    UINT32 intSave;

    (VOID) flags;
    if ((value == NULL) || OS_INT_ACTIVE || (timerId == NULL)) {
        errno = EINVAL;
        return -1;
    }

    if (!ValidTimespec(&value->it_value) || !ValidTimespec(&value->it_interval)) {
        errno = EINVAL;
        return -1;
    }

    if (oldValue != NULL) {
        (VOID)timer_gettime(timerId, oldValue);
    }

    ret = LOS_SwtmrStop(swtmr->timerId);
    if (ret == LOS_ERRNO_SWTMR_ID_INVALID) {
        errno = EINVAL;
        return -1;
    }

    expiry = OsTimespec2Tick(&value->it_value);
    interval = OsTimespec2Tick(&value->it_interval);
    if (expiry == 0) {
        /*
         * 1) when expiry is 0, means timer should be stopped.
         * 2) If timer is ticking, stopping timer is already done before.
         * 3) If timer is created but not ticking, return 0 as well.
         */
        return 0;
    } else {
        LOS_SpinLockSave(&g_swtmrSpin, &intSave);
        if (interval == 0) {
            swtmr->mode = LOS_SWTMR_MODE_NO_SELFDELETE;
        } else {
            swtmr->mode = LOS_SWTMR_MODE_OPP;
        }
    }

    swtmr->expiry = expiry;
    swtmr->interval = interval;
    swtmr->overrun = 0;
    LOS_SpinUnlockRestore(&g_swtmrSpin, intSave);
    if (LOS_SwtmrStart(swtmr->timerId)) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

int timer_gettime(timer_t timerId, struct itimerspec *value)
{
    UINT32 tick = 0;
    LosSwtmrCB *swtmr = NULL;
    UINT32 ret;

    swtmr = (LosSwtmrCB *)timerId;

    /* expire time */
    if ((value == NULL) || (swtmr == NULL)) {
        errno = EINVAL;
        return -1;
    }
    /* get expire time */
    ret = LOS_SwtmrTimeGet(swtmr->timerId, &tick);
    if ((ret != LOS_OK) && (ret != LOS_ERRNO_SWTMR_NOT_STARTED)) {
        errno = EINVAL;
        return -1;
    }

    OsTick2TimeSpec(&value->it_value, tick);
    OsTick2TimeSpec(&value->it_interval, swtmr->interval);
    return 0;
}

int timer_getoverrun(timer_t timerId)
{
    LosSwtmrCB *swtmr = (LosSwtmrCB *)timerId;
    if (swtmr == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (swtmr->timerId >= OS_SWTMR_MAX_TIMERID) {
        errno = EINVAL;
        return -1;
    }

    if ((swtmr->overrun) >= (UINT8)DELAYTIMER_MAX) {
        return (INT32)DELAYTIMER_MAX;
    }
    return (INT32)(swtmr->overrun);
}
