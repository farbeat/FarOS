/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module Task
 */
#include "FreeRTOS.h"
#include "print.h"
#include "tasks.c"
#include "task_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void vTaskInfoPrint(void)
{
    char cStatus;
    UBaseType_t uxCurrentNumberOfTasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *pxTaskStatusArray = pvPortMalloc(uxCurrentNumberOfTasks * sizeof(TaskStatus_t));
    if (pxTaskStatusArray == NULL) {
        return;
    }
    UBaseType_t uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxCurrentNumberOfTasks, NULL);

    printf("\nName           TID\tCurPriority\tBasePriority\tStatus\tWaterLine\tStackBase");

#if defined(configRECORD_STACK_HIGH_ADDRESS) && (configRECORD_STACK_HIGH_ADDRESS == 1)
    printf("\tTotalSize\tUsedMaxSize");
#endif
    printf("\r\n");

    printf("----------------------------------------------------------------------------------------------------\r\n");
    for (UBaseType_t x = 0; x < uxArraySize; x++) {
        switch (pxTaskStatusArray[x].eCurrentState) {
            case eRunning:
                cStatus = tskRUNNING_CHAR;
                break;
            case eReady:
                cStatus = tskREADY_CHAR;
                break;
            case eBlocked:
                cStatus = tskBLOCKED_CHAR;
                break;
            case eSuspended:
                cStatus = tskSUSPENDED_CHAR;
                break;
            case eDeleted:
                cStatus = tskDELETED_CHAR;
                break;
            case eInvalid:
            default:
                cStatus = (char)0x00;
                break;
        }

        printf("%-13s\t%-04u\t%-13u\t%-09u\t%-04c\t0x%-09x\t0x%-09x", pxTaskStatusArray[x].pcTaskName,
            (unsigned int) pxTaskStatusArray[x].xTaskNumber, (unsigned int) pxTaskStatusArray[x].uxCurrentPriority,
            (unsigned int) pxTaskStatusArray[x].uxBasePriority, cStatus,
            (unsigned int) pxTaskStatusArray[x].usStackHighWaterMark, (unsigned int) pxTaskStatusArray[x].pxStackBase);

#if defined(configRECORD_STACK_HIGH_ADDRESS) && (configRECORD_STACK_HIGH_ADDRESS == 1)
            UBaseType_t uxTotalSize = (pxTaskStatusArray[x].xHandle->pxEndOfStack -
                pxTaskStatusArray[x].xHandle->pxStack + 1) * sizeof(StackType_t);
            UBaseType_t uxUsedMaxSize = uxTotalSize - pxTaskStatusArray[x].usStackHighWaterMark;
            printf("\t0x%-09x\t0x%-09x", (unsigned int) uxTotalSize, (unsigned int) uxUsedMaxSize);
#endif

        printf("\r\n");
    }
    vPortFree(pxTaskStatusArray);
}

UBaseType_t xTaskCntGet(void)
{
    return uxTaskGetNumberOfTasks();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
