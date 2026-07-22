/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module software Timer
 */
#include "FreeRTOS.h"
#include "print.h"
#include "timers.c"
#include "swtmr_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static void vSwtmrDataPrint(TimerHandle_t pxNextTimer)
{
    if (pxNextTimer != NULL) {
        vPrintk(ePrintkInfo,
            "%-07d  \t%-07s  \t%-10d\t%-06u  \t0x%-11x\t\n",
            (uint32_t)pvTimerGetTimerID(pxNextTimer),
            (char *)pcTimerGetName(pxNextTimer),
            uxTimerGetReloadMode(pxNextTimer),
            (unsigned int)xTimerGetPeriod(pxNextTimer),
            pxNextTimer);
    } else {
        mtCOVERAGE_TEST_MARKER();
    }
}

static void vSwtmrSingleTraverse(List_t *TimerList)
{
    List_t *pxList = TimerList;
    if (listLIST_IS_EMPTY(pxList) == pdFALSE) {
        List_t *pxFirstTimer = NULL;
        List_t *pxNextTimer = NULL;
        listGET_OWNER_OF_NEXT_ENTRY(pxFirstTimer, pxList);
        do {
            listGET_OWNER_OF_NEXT_ENTRY(pxNextTimer, pxList);
            vSwtmrDataPrint((TimerHandle_t)pxNextTimer);
        } while (pxNextTimer != pxFirstTimer);
    } else {
        mtCOVERAGE_TEST_MARKER();
    }
}

static void vSwtmrTitlePrint(void)
{
    vPrintk(ePrintkInfo, "\nSwTmrID  \tTmrName  \tReloadMode\tPeriod  \thandlerAddr\n");
    vPrintk(ePrintkInfo, "-------  \t-------  \t----------\t------  \t-----------\n");
}

void vSwtmrInfoPrint(void)
{
    vSwtmrTitlePrint();
    vSwtmrSingleTraverse((List_t *)pxCurrentTimerList);
    vSwtmrSingleTraverse((List_t *)pxOverflowTimerList);
}

UBaseType_t xSwTmrCntGet(void)
{
    UBaseType_t swtmr_cnt = 0;
    List_t *pxList = (List_t *)pxCurrentTimerList;
    List_t *pxFirstTimer, *pxNextTimer;
    if (listLIST_IS_EMPTY(pxList) == pdFALSE) {
        listGET_OWNER_OF_NEXT_ENTRY(pxFirstTimer, pxList);
        do {
            listGET_OWNER_OF_NEXT_ENTRY(pxNextTimer, pxList);
            swtmr_cnt++;
        } while (pxNextTimer != pxFirstTimer);
    } else {
        mtCOVERAGE_TEST_MARKER();
    }
    pxList = (List_t *)pxOverflowTimerList;
    if (listLIST_IS_EMPTY(pxList) == pdFALSE) {
        listGET_OWNER_OF_NEXT_ENTRY(pxFirstTimer, pxList);
        do {
            listGET_OWNER_OF_NEXT_ENTRY(pxNextTimer, pxList);
            swtmr_cnt++;
        } while (pxNextTimer != pxFirstTimer);
    } else {
        mtCOVERAGE_TEST_MARKER();
    }
    return swtmr_cnt;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
