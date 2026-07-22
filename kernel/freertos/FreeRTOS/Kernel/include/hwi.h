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

/*
 * This is the list implementation used by the scheduler.  While it is tailored
 * heavily for the schedulers needs, it is also available for use by
 * application code.
 *
 * list_ts can only store pointers to list_item_ts.  Each ListItem_t contains a
 * numeric value (xItemValue).  Most of the time the lists are sorted in
 * ascending item value order.
 *
 * Lists are created already containing one list item.  The value of this
 * item is the maximum possible that can be stored, it is therefore always at
 * the end of the list and acts as a marker.  The list member pxHead always
 * points to this marker - even though it is at the tail of the list.  This
 * is because the tail contains a wrap back pointer to the true head of
 * the list.
 *
 * In addition to it's value, each list item contains a pointer to the next
 * item in the list (pxNext), a pointer to the list it is in (pxContainer)
 * and a pointer to back to the object that contains it.  These later two
 * pointers are included for efficiency of list manipulation.  There is
 * effectively a two way link between the object containing the list item and
 * the list item itself.
 *
 *
 * \page ListIntroduction List Implementation
 * \ingroup FreeRTOSIntro
 */

#ifndef HWI_H
#define HWI_H

#ifndef INC_FREERTOS_H
    #error "FreeRTOS.h must be included before hwi.h"
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#ifndef configHWI_INVALID_IRQ_ID
#define configHWI_INVALID_IRQ_ID  ( 0 )
#endif

#ifndef configHWI_PRIO_IGNORE
#define configHWI_PRIO_IGNORE ( -1 )
#endif

#ifndef configHWI_PRIO_LIMIT
#define configHWI_PRIO_LIMIT    7
#endif

#ifndef configHWI_PRIO_HIGHEST
#define configHWI_PRIO_HIGHEST  0
#endif

#ifndef configHWI_PRIO_LOWEST
#define configHWI_PRIO_LOWEST   (configHWI_PRIO_LIMIT - 1)
#endif

typedef void (* HwiFunction_t)( BaseType_t, void * );

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiCreate( BaseType_t xId,
 *                       BaseType_t xPriority,
 *                       HwiFunction_t pxCallback,
 *                       void * const pvParameters );
 * </pre>
 *
 * Create a hardware interrupt and register a hardware interrupt handling function.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @param xPriority TThe hardware interrupt priority. The value range is
 * [configHWI_PRIO_LOWEST, configHWI_PRIO_HIGHEST].
 *
 * @param pxCallback The interrupt handler used when a hardware interrupt is triggered.
 *
 * @param pvParameters The input parameter of the interrupt handler used when
 * a hardware interrupt is triggered.
 *
 * @return If the interrupt is successfully create then pdPASS is returned.
 * If insufficient memory for hardware interrupt creation then
 * errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY is returned.
 * If the interrupt cannot be created then pdFAIL is returned.
 *
 * \defgroup xHwiCreate xHwiCreate
 * \ingroup HWI
 */
BaseType_t xHwiCreate( BaseType_t xId,
                       BaseType_t xPriority,
                       HwiFunction_t pxCallback,
                       void * const pvParameters ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiDelete( BaseType_t xId );
 * </pre>
 *
 * Delete a hardware interrupt.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @return If the interrupt is successfully deleted then pdPASS is returned.
 * If the interrupt has not been created then pdFAIL is returned.
 *
 * \defgroup xHwiDelete xHwiDelete
 * \ingroup HWI
 */
BaseType_t xHwiDelete( BaseType_t xId ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiSetPriority( BaseType_t xId, BaseType_t xPriority );
 * </pre>
 *
 * Set interrupts priority.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @param xPriority TThe hardware interrupt priority. The value range is
 * [configHWI_PRIO_LOWEST, configHWI_PRIO_HIGHEST].
 *
 * @return If the interrupt is successfully set priority then pdPASS is returned.
 * If the priority cannot be seted to interrupt then pdFAIL is returned.
 *
 * \defgroup xHwiSetPriority xHwiSetPriority
 * \ingroup HWI
 */
BaseType_t xHwiSetPriority( BaseType_t xId, BaseType_t xPriority ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiEnable( BaseType_t xId );
 * </pre>
 *
 * Enable the corresponding interrupt mask of the interrupt controller,
 * so that the interrupt source can be sent to the CPU.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @return If the interrupt is successfully enabled then pdPASS is returned.
 * If the interrupt number is invalid then pdFAIL is returned.
 *
 * \defgroup xHwiEnable xHwiEnable
 * \ingroup HWI
 */
BaseType_t xHwiEnable( BaseType_t xId ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiDisable( BaseType_t xId );
 * </pre>
 *
 * Disable the corresponding interrupt mask of the interrupt controller,
 * so that the interrupt source can be sent to the CPU.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @return If the interrupt is successfully disabled then pdPASS is returned.
 * If the interrupt number is invalid then pdFAIL is returned.
 *
 * \defgroup xHwiDisable xHwiDisable
 * \ingroup HWI
 */
BaseType_t xHwiDisable( BaseType_t xId ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * void xHwiClear( BaseType_t xId )
 * </pre>
 *
 * Clear the pending status of other interrupts.
 *
 * @param xId The hardware interrupt number. The value range is
 * [configHWI_SYS_VECTOR_CNT, configHWI_MAX_VECTOR_CNT].
 *
 * @return none.
 *
 * \defgroup xHwiClear xHwiClear
 * \ingroup HWI
 */
void xHwiClear( BaseType_t xId ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * UBaseType_t uxHwiLock( void );
 * </pre>
 *
 * Disable all interrupts.
 *
 * @param none.
 *
 * @return mstatus value before all interrupts are disabled.
 *
 * \defgroup uxHwiLock uxHwiLock
 * \ingroup HWI
 */
UBaseType_t uxHwiLock( void ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * UBaseType_t uxHwiUnlock( void );
 * </pre>
 *
 * Enable all interrupts.
 *
 * @param none.
 *
 * @return mstatus value after all interrupts are enabled.
 *
 * \defgroup uxHwiUnlock uxHwiUnlock
 * \ingroup HWI
 */
UBaseType_t uxHwiUnlock( void ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * void vHwiRestore( UBaseType_t uxSave );
 * </pre>
 *
 * Restore interrupts.
 *
 * @param uxSave mstatus value before all interrupts are disabled.
 *
 * @return none.
 *
 * \defgroup vHwiRestore vHwiRestore
 * \ingroup HWI
 */
void vHwiRestore( UBaseType_t uxSave ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * BaseType_t xHwiGetInterruptId( void );
 * </pre>
 *
 * Get current interrupt id.
 *
 * @param void.
 *
 * @return current interrupt id.
 * Returns configHWI_INVALID_IRQ_ID if not currently in interrupt state
 *
 * \defgroup xHwiGetInterruptId xHwiGetInterruptId
 * \ingroup HWI
 */
BaseType_t xHwiGetInterruptId( void ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * void vHwiYield( void );
 * </pre>
 *
 * Yield current state.
 *
 * @param void.
 *
 * @return none.
 *
 * \defgroup vHwiYield vHwiYield
 * \ingroup HWI
 */
void vHwiYield( void ) PRIVILEGED_FUNCTION;

/**
 * hwi.h
 * <pre>
 * void vHwiInfoPrint( void );
 * </pre>
 *
 * Print the Hwi information with title and data.
 *
 * @param void.
 *
 * @return none.
 *
 * \defgroup vHwiInfoPrint vHwiInfoPrint
 * \ingroup HWI
 */
#ifdef FREERTOS_DEBUG
void vHwiInfoPrint( void ) PRIVILEGED_FUNCTION;
#endif /* FREERTOS_DEBUG */

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef HWI_H */
