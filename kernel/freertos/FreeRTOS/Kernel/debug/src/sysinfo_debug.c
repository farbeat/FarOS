/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module software system infomation
 */
#include "task_debug.h"
#include "queue_debug.h"
#include "swtmr_debug.h"
#include "print.h"
#include "sysinfo_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void vSystemInfoPrint(void)
{
    PRINTK(ePrintkDebug, "\nModule    Used      Total\n");
    PRINTK(ePrintkDebug, "--------------------------------\n");
    PRINTK(ePrintkDebug, "Task      %-10u%-10s\n",
           xTaskCntGet(),
           "unlimited");
    PRINTK(ePrintkDebug, "Sem       %-10u%-10d\n",
           xSemCntGet(),
           configQUEUE_REGISTRY_SIZE);
    PRINTK(ePrintkDebug, "Mutex     %-10u%-10d\n",
           xMutexCntGet(),
           configQUEUE_REGISTRY_SIZE);
    PRINTK(ePrintkDebug, "Queue     %-10u%-10d\n",
           xQueueCntGet(),
           configQUEUE_REGISTRY_SIZE);
    PRINTK(ePrintkDebug, "SwTmr     %-10u%-10s\n",
           xSwTmrCntGet(),
           "unlimited");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
