/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * dfx debug module task headerfile
 */
#ifndef TASK_DEBUG_H
#define TASK_DEBUG_H

#include "FreeRTOS.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Print the Task information with title and data.
 *
 * @param void.
 *
 * @retval none.
 */
void vTaskInfoPrint(void);

/**
 * @ingroup queue_debug
 * @par Description:
 * This API is used to Get the Task number in our OS.
 *
 * @param void.
 *
 * @retval UBaseType_t.
 */
UBaseType_t xTaskCntGet(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TASK_DEBUG_H */
