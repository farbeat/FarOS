/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Debug Swtmr
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

#include "los_swtmr_pri.h"
#include "stdlib.h"

#define SWTMR_STRLEN  12

STATIC CHAR g_shellSwtmrMode[][SWTMR_STRLEN] = {
    "Once",
    "Period",
    "NSD",
    "OPP",
};

STATIC CHAR g_shellSwtmrStatus[][SWTMR_STRLEN] = {
    "UnUsed",
    "Deleting",
    "Created",
    "Ticking",
};

STATIC VOID OsPrintSwtmrMsg(const LosSwtmrCB *swtmr)
{
    PRINTK("0x%08x  "
           "%-7s  "
           "%-6s  "
           "%-6u    "
           "0x%-18.*lx"
           "0x%lx\n",
           swtmr->timerId % LOSCFG_BASE_CORE_SWTMR_LIMIT,
           g_shellSwtmrStatus[swtmr->state],
           g_shellSwtmrMode[swtmr->mode],
           swtmr->interval,
           OS_HEX_ADDR_WIDTH, swtmr->arg,
           swtmr->handler);
}

STATIC INLINE VOID OsPrintSwtmrMsgHead(VOID)
{
    PRINTK("\r\nSwTmrID     State    Mode    Interval  Arg                 handlerAddr\n");
    PRINTK("----------  -------  ------- --------- ----------          --------\n");
}

UINT32 OsDbgSwtmrInfoGet(UINT32 timerId)
{
    LosSwtmrCB *swtmr = g_osSwtmrCBArray;
    UINT16 index;
    UINT16 num = 0;

    OsPrintSwtmrMsgHead();
    if (timerId == OS_ALL_SWTMR_MASK) {
        for (index = 0; index < LOSCFG_BASE_CORE_SWTMR_LIMIT; ++index, ++swtmr) {
            if (swtmr->state != 0) {
                OsPrintSwtmrMsg(swtmr);
                num++;
            }
        }

        if (num == 0) {
            PRINTK("\r\nThere is no swtmr was created!\n");
            return OS_ERROR;
        }
    } else {
        for (index = 0; index < LOSCFG_BASE_CORE_SWTMR_LIMIT; ++index, ++swtmr) {
            if ((timerId == (size_t)(swtmr->timerId % LOSCFG_BASE_CORE_SWTMR_LIMIT)) && (swtmr->state != 0)) {
                OsPrintSwtmrMsg(swtmr);
                return LOS_OK;
            }
        }
        PRINTK("\r\nThe SwTimerID is not exist.\n");
    }
    return LOS_OK;
}
