/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "hal_hwi.h"
#include "encoding.h"
#include "print.h"

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

static volatile BaseType_t xInterruptHwiId = configHWI_INVALID_IRQ_ID;

BaseType_t xHwiGetInterruptId( void )
{
    return xInterruptHwiId;
}

void vHwiYield( void )
{
    if ( xInterruptHwiId != configHWI_INVALID_IRQ_ID)
    {
        PRINTK( ePrintkErr, "Unsupport yield in interrupt [%d]!\n", xInterruptHwiId);
        return;
    }

    __asm__ volatile( "ecall" );
}

void vHwiEntry( BaseType_t xCause )
{
    BaseType_t xId = xCause & UINT8_CUT_MASK;
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );

    HwiHandle_t *pxHandle = pxPortHwiGetHandle( xId );
    if ( pxHandle == NULL )
    {
        PRINTK( ePrintkErr, "Unspport irq[%d]\n", xId);
        return;
    }

    pxHandle->xCounts++;
    xInterruptHwiId = xId;
    pxHandle->pxCb( xId, pxHandle->pvParameters );
    xInterruptHwiId = configHWI_INVALID_IRQ_ID;
}

BaseType_t xHwiCreate( BaseType_t xId,
                       BaseType_t xPriority,
                       HwiFunction_t pxCallback,
                       void * const pvParameters )
{
    BaseType_t xReturn = pdPASS;

    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );
    configASSERT( pxCallback != NULL );

    HwiHandle_t *pxHandle = pxPortHwiGetHandle( xId );
    if ( pxHandle == NULL )
    {
        pxHandle = pvPortMalloc( sizeof( HwiHandle_t ) );
    }

    if ( pxHandle == NULL)
    {
        return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
    }

    pxHandle->pxCb = pxCallback;
    pxHandle->pvParameters = pvParameters;
    pxHandle->xHwiId = xId;
    pxHandle->xHwiPriority = -1;
    vPortHwiSetHandle( xId, pxHandle );

    if ( xPriority != configHWI_PRIO_IGNORE )
    {
        xReturn = xHwiSetPriority(xId, xPriority);
        if (xReturn != pdPASS) {
            (void)xHwiDelete(xId);
        }
    }

    return xReturn;
}

BaseType_t xHwiDelete( BaseType_t xId )
{
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );

    HwiHandle_t *pxHandle = pxPortHwiGetHandle( xId );
    if (pxHandle != NULL)
    {
        vPortFree(pxHandle);
        vPortHwiSetHandle( xId, NULL );
    }
    else
    {
        return pdFAIL;
    }

    xPortHwiDisable( xId );
    return pdPASS;
}

BaseType_t xHwiSetPriority( BaseType_t xId, BaseType_t xPriority )
{
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );
    HwiHandle_t *pxHandle = pxPortHwiGetHandle(xId);
    if (pxHandle != NULL) {
        pxHandle->xHwiPriority = xPriority;
        return xPortHwiSetPriority(xId, xPriority);
    } else {
        return pdFAIL;
    }
}

BaseType_t xHwiDisable( BaseType_t xId )
{
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );
    return xPortHwiDisable( xId );
}

BaseType_t xHwiEnable( BaseType_t xId )
{
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );
    return xPortHwiEnable( xId );
}

void xHwiClear( BaseType_t xId )
{
    configASSERT( xId < configHWI_MAX_VECTOR_CNT && xId >= 0 );
    return;
}

UBaseType_t uxHwiLock( void )
{
    return uxPortHwiLock();
}

UBaseType_t uxHwiUnlock( void )
{
    return uxPortHwiUnlock();
}

void vHwiRestore( UBaseType_t uxSave )
{
    vPortHwiRestore(uxSave);
}

#ifdef FREERTOS_DEBUG
void vHwiTitlePrint(void);
void xHwiDataPrint(BaseType_t xId);
 
void vHwiInfoPrint(void)
{
    vHwiTitlePrint();
    for (BaseType_t xId = 0; xId < configHWI_MAX_VECTOR_CNT; xId++) {
        xHwiDataPrint(xId);
    }
}
 
void vHwiTitlePrint(void)
{
    PRINTK(ePrintkDebug, "\nInterruptNo     IntPriority     CallBackAddr  \n");
    PRINTK(ePrintkDebug, "-----------     -----------     -------------\n");
}
 
void xHwiDataPrint(BaseType_t xId)
{
    configASSERT(xId < configHWI_MAX_VECTOR_CNT && xId >= 0);
    HwiHandle_t *pxHandle = pxPortHwiGetHandle(xId);
    if (pxHandle == NULL) {
        return;
    }
    PRINTK(ePrintkDebug,
            "%-13d\t%-09d\t%-13X\n",
            pxHandle->xHwiId,
            pxHandle->xHwiPriority,
            pxHandle->pxCb);
}
#endif /* FREERTOS_DEBUG */
