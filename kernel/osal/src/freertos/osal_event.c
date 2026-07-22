/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Create: 2021-07-11
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include "hwi.h"
#include "soc_osal.h"
#include "osal_inner.h"

#if defined(configUSE_16_BIT_TICKS) && configUSE_16_BIT_TICKS == 1
#define EVENT_BITS_INVALID_BITS    0xff00U
#else
#define EVENT_BITS_INVALID_BITS    0xff000000UL
#endif
#define BIT_31 (1 << 31)

int osal_event_init(osal_event *event_obj)
{
    if (event_obj == NULL || event_obj->event != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    event_obj->event = xEventGroupCreate();
    if (event_obj->event == NULL) {
        osal_log("xEventGroupCreate failed!\n");
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_event_write(osal_event *event_obj, unsigned int mask)
{
    if (event_obj == NULL || event_obj->event == NULL ||
	    (mask & EVENT_BITS_INVALID_BITS) != 0 || (mask & BIT_31) == 1) {
        osal_log("mask=0x%x parameter invalid!\n", mask);
        return OSAL_FAILURE;
    }

    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        (void)xEventGroupSetBits(event_obj->event, mask);
        return OSAL_SUCCESS;
    }

    BaseType_t taskwoken = pdFALSE;
    BaseType_t result = xEventGroupSetBitsFromISR(event_obj->event, mask, &taskwoken);
    if (result != pdPASS) {
        osal_log("result=0x%x invalid!\n", result);
        return OSAL_FAILURE;
    }
    portYIELD_FROM_ISR(taskwoken);
    return OSAL_SUCCESS;
}

int osal_event_read(osal_event *event_obj, unsigned int mask, unsigned int timeout_ms, unsigned int mode)
{
    unsigned int ret;
    if (event_obj == NULL || mask == 0 || event_obj->event == NULL ||
	    (mask & EVENT_BITS_INVALID_BITS) != 0 || (mask & BIT_31) == 1) {
        osal_log("mask=0x%x parameter invalid!\n", mask);
        return OSAL_FAILURE;
    }
    if (((mode & OSAL_WAITMODE_OR) && (mode & OSAL_WAITMODE_AND)) || // AND & OR cannnot exist at the same time
        (mode & ~(OSAL_WAITMODE_OR | OSAL_WAITMODE_AND | OSAL_WAITMODE_CLR)) || // mode cannot match other bit
        !(mode & (OSAL_WAITMODE_OR | OSAL_WAITMODE_AND))) { // mode must have one of (AND,OR)
        osal_log("mode=0x%x invalid!\n", mode);
        return OSAL_FAILURE;
    }

    unsigned int readout =  xEventGroupWaitBits(event_obj->event, mask,
        mode & OSAL_WAITMODE_CLR, // need clear after read?
        mode & OSAL_WAITMODE_AND, // if true return when read all, or return when read anyone
        pdMS_TO_TICKS(timeout_ms)); // need ms to tick
    ret = readout & mask;
    if ((ret == 0) || (ret != mask && mode == OSAL_WAITMODE_AND)) {
        osal_log("readout=0x%x mask=0x%x timeout!\n", readout, mask);
        return OSAL_FAILURE;
    }
    return (int)ret;
}

int osal_event_clear(osal_event *event_obj, unsigned int mask)
{
    if (event_obj == NULL || event_obj->event == NULL || (mask & EVENT_BITS_INVALID_BITS) != 0) {
        osal_log("mask=0x%x parameter invalid!\n", mask);
        return OSAL_FAILURE;
    }

    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        (void)xEventGroupClearBits(event_obj->event, mask);
        return OSAL_SUCCESS;
    }

    (void)xEventGroupClearBitsFromISR(event_obj->event, mask);
    return OSAL_SUCCESS;
}

int osal_event_destroy(osal_event *event_obj)
{
    if (event_obj == NULL || event_obj->event == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    vEventGroupDelete(event_obj->event);
    event_obj->event = NULL;
    return OSAL_SUCCESS;
}
