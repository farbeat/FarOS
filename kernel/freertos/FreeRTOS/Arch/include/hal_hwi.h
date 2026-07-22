/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef HAL_HWI_H
#define HAL_HWI_H

#include "hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    HwiFunction_t pxCb;
    void *pvParameters;
    BaseType_t xCounts;
    BaseType_t xHwiId;
    BaseType_t xHwiPriority;
} HwiHandle_t;

void vPortHwiSetHandle( BaseType_t xId, HwiHandle_t *pxHandle );
HwiHandle_t *pxPortHwiGetHandle( BaseType_t xId );
BaseType_t xPortHwiSetPriority( BaseType_t xId, BaseType_t xPriority );
BaseType_t xPortHwiDisable( BaseType_t xId );
BaseType_t xPortHwiEnable( BaseType_t xId );
void vPortHwiClear( BaseType_t xId );

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
