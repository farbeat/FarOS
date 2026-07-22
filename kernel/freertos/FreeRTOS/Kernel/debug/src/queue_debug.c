/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module queue
 */
#include "stdio.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.c"
#include "queue_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static UBaseType_t g_queue_num = 0;
static UBaseType_t g_sem_num = 0;
static UBaseType_t g_mutex_num = 0;

static void vQueueCountStatistics(void)
{
    UBaseType_t ux;
    for (ux = (UBaseType_t) 0U; ux < (UBaseType_t) configQUEUE_REGISTRY_SIZE; ux++) {
        if (xQueueRegistry[ux].pcQueueName != NULL) {
            uint8_t type = ucQueueGetQueueType(xQueueRegistry[ux].xHandle);
            if (type == queueQUEUE_TYPE_BASE)
                g_queue_num++;
            else if (type == queueQUEUE_TYPE_COUNTING_SEMAPHORE || type == queueQUEUE_TYPE_BINARY_SEMAPHORE)
                g_sem_num++;
            else if (type == queueQUEUE_TYPE_MUTEX || type == queueQUEUE_TYPE_RECURSIVE_MUTEX)
                g_mutex_num++;
        } else {
            mtCOVERAGE_TEST_MARKER();
        }
    }
}

static void vQueueCountClear(void)
{
    g_queue_num = 0;
    g_sem_num = 0;
    g_mutex_num = 0;
}

void vQueueInfoPrint(void)
{
    UBaseType_t ux;
    for (ux = (UBaseType_t)0U; ux < (UBaseType_t)configQUEUE_REGISTRY_SIZE; ux++) {
        if (xQueueRegistry[ux].pcQueueName != NULL) {
            uint8_t type = ucQueueGetQueueType(xQueueRegistry[ux].xHandle);
            if (type == queueQUEUE_TYPE_BASE) {
                printf("\nQueue name <%s>, Queue Handle is<0x%x>,queue len is %u, ",
                    pcQueueGetName(xQueueRegistry[ux].xHandle),
                    xQueueRegistry[ux].xHandle,
                    uxQueueSpacesAvailable(xQueueRegistry[ux].xHandle) +
                    uxQueueMessagesWaiting(xQueueRegistry[ux].xHandle));
                printf("Queue used:0x%u, Queue available: 0x%u, ",
                    uxQueueMessagesWaiting(xQueueRegistry[ux].xHandle),
                    uxQueueSpacesAvailable(xQueueRegistry[ux].xHandle));
                printf("Queue Type is: %u\n",
                    ucQueueGetQueueType(xQueueRegistry[ux].xHandle));
                    traceQUEUE_REGISTRY_ADD(xQueueRegistry[ux], pcQueueName);
            }
        } else {
            mtCOVERAGE_TEST_MARKER();
        }
    }
}

void vSemaphoreInfoPrint(void)
{
    UBaseType_t ux;
    printf("Used Semaphore List: \n");
    printf("\nSemHandle      SemName        MaxCount       Count          \n");
    printf("------------   ------------   -------------  ---------------  \n");
    for (ux = (UBaseType_t)0U; ux < (UBaseType_t)configQUEUE_REGISTRY_SIZE; ux++) {
        if (xQueueRegistry[ux].pcQueueName != NULL) {
            uint8_t type = ucQueueGetQueueType(xQueueRegistry[ux].xHandle);
            if (type == queueQUEUE_TYPE_COUNTING_SEMAPHORE||
                type == queueQUEUE_TYPE_BINARY_SEMAPHORE) {
                printf("0x%-13x\t%-11s\t%-14u\t%-22u \n",
                    xQueueRegistry[ux].xHandle,
                    pcQueueGetName(xQueueRegistry[ux].xHandle),
                    uxQueueSpacesAvailable(xQueueRegistry[ux].xHandle) +
                    uxQueueMessagesWaiting(xQueueRegistry[ux].xHandle),
                    uxQueueMessagesWaiting(xQueueRegistry[ux].xHandle));
                traceQUEUE_REGISTRY_ADD(xQueueRegistry[ux], pcQueueName);
            }
        } else {
            mtCOVERAGE_TEST_MARKER();
        }
    }
}

void vMutexInfoPrint(void)
{
    UBaseType_t ux;
    for (ux = (UBaseType_t) 0U; ux < (UBaseType_t) configQUEUE_REGISTRY_SIZE; ux++) {
        if (xQueueRegistry[ux].pcQueueName != NULL) {
            uint8_t type = ucQueueGetQueueType(xQueueRegistry[ux].xHandle);
            if (type == queueQUEUE_TYPE_MUTEX ||
            type == queueQUEUE_TYPE_RECURSIVE_MUTEX) {
            printf("\nMutex Handle <0x%x>,Mutex Name<%s>,MutexHolder Handle: 0x%x\n",
                xQueueRegistry[ux].xHandle,
                pcQueueGetName(xQueueRegistry[ux].xHandle),
                xQueueGetMutexHolder(xQueueRegistry[ux].xHandle)
            );
            traceQUEUE_REGISTRY_ADD(xQueueRegistry[ux], pcQueueName);
            }
        } else {
            mtCOVERAGE_TEST_MARKER();
        }
    }
}

UBaseType_t xMutexCntGet(void)
{
    UBaseType_t temp_num = 0;
    vQueueCountStatistics();
    temp_num = g_mutex_num;
    vQueueCountClear();
    return temp_num;
}

UBaseType_t xQueueCntGet(void)
{
    UBaseType_t temp_num = 0;
    vQueueCountStatistics();
    temp_num = g_queue_num;
    vQueueCountClear();
    return temp_num;
}

UBaseType_t xSemCntGet(void)
{
    UBaseType_t temp_num = 0;
    vQueueCountStatistics();
    temp_num = g_sem_num;
    vQueueCountClear();
    return temp_num;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
