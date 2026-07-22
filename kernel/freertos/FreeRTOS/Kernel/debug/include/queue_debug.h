/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module queue headerfile
 */
#ifndef QUEUE_DEBUG_H
#define QUEUE_DEBUG_H

#include "FreeRTOS.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Print the Queue information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vQueueInfoPrint(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Print the semaphore information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vSemaphoreInfoPrint(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Print the mutex information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vMutexInfoPrint(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Get the mutex number in our OS.
 *
 * @param void.
 *
 * @retval UBaseType_t.
 */
UBaseType_t xMutexCntGet(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to get the queue number in our OS.
 *
 * @param void.
 *
 * @retval UBaseType_t.
 */
UBaseType_t xQueueCntGet(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to get the Semaphore number in our OS.
 *
 * @param void.
 *
 * @retval UBaseType_t.
 */
UBaseType_t xSemCntGet(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* QUEUE_DEBUG_H */
