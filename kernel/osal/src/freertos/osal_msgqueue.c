/*
* Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
* Description:osal_msgqueue
* Create: 2021-09-11
*/

#include <FreeRTOS.h>
#include <queue.h>
#include "hwi.h"
#include "soc_osal.h"
#include "osal_inner.h"

#define OSAL_INVALID_MSG_NUM 0xFFFFFFFF
int osal_msg_queue_create(const char *name, unsigned short queue_len, unsigned long *queue_id,
    unsigned int flags, unsigned short max_msgsize)
{
    if (name == NULL || queue_id == NULL || queue_len == 0) {
        osal_log("invalid param!\n");
        return OSAL_FAILURE;
    }
    osal_unused(name, flags);
    QueueHandle_t handle = xQueueCreate(queue_len, max_msgsize);
    if (handle == NULL) {
        osal_log("xQueueCreate failed!\n");
        return OSAL_FAILURE;
    }
    *queue_id = (unsigned long)handle;
    vQueueAddToRegistry(handle, name);
    osal_log("qName:%s  qID=0x%x\r\n", name, *queue_id);
    return OSAL_SUCCESS;
}

// timeout:ticks
int osal_msg_queue_write_copy(unsigned long queue_id, void *buffer_addr, unsigned int buffer_size, unsigned int timeout)
{
    if (buffer_addr == NULL || buffer_size == 0 || (QueueHandle_t)queue_id == NULL) {
        osal_log("buffer_size:%d buffer_addr:0x%x.\n", buffer_size, buffer_addr);
        return OSAL_FAILURE;
    }

    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        BaseType_t ret = xQueueSendToBack((QueueHandle_t)queue_id, buffer_addr, timeout);
        if (ret != pdPASS) {
            osal_log("xQueueSendToBack failed! ret = 0x%x. qID=0x%x\n", ret, queue_id);
            return OSAL_FAILURE;
        }
        return OSAL_SUCCESS;
    }

    BaseType_t taskwoken = pdFALSE;
    BaseType_t result = xQueueSendToBackFromISR((QueueHandle_t)queue_id, buffer_addr, &taskwoken);
    if (result != pdPASS) {
        osal_log("xQueueSendToBackFromISR failed! result = 0x%x. qID=0x%x\n", result, queue_id);
        return OSAL_FAILURE;
    }
    portYIELD_FROM_ISR(taskwoken);
    return OSAL_SUCCESS;
}

// timeout:ticks
int osal_msg_queue_write_head_copy(unsigned long queue_id, void *buffer_addr, unsigned int buffer_size,
    unsigned int timeout)
{
    if (buffer_addr == NULL || buffer_size == 0 || (QueueHandle_t)queue_id == NULL) {
        osal_log("buffer_size:%d buffer_addr:0x%x.qID=0x%x\n", buffer_size, buffer_addr, queue_id);
        return OSAL_FAILURE;
    }
    BaseType_t ret = xQueueSendToFront((QueueHandle_t)queue_id, buffer_addr, timeout);
    if (ret != pdPASS) {
        osal_log("xQueueSendToFront failed! ret = 0x%x.qID=0x%x\n", ret, queue_id);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

// timeout:ticks
int osal_msg_queue_read_copy(unsigned long queue_id, void *buffer_addr, unsigned int *buffer_size, unsigned int timeout)
{
    if (buffer_addr == NULL || buffer_size == 0 || (QueueHandle_t)queue_id == NULL) {
        osal_log("buffer_size:%d buffer_addr:0x%x.qID=0x%x\n", buffer_size, buffer_addr, queue_id);
        return OSAL_FAILURE;
    }

    if (xHwiGetInterruptId() == configHWI_INVALID_IRQ_ID) {
        BaseType_t ret = xQueueReceive((QueueHandle_t)queue_id, buffer_addr, timeout);
        if (ret != pdPASS) {
            osal_log("xQueueReceive failed! ret = 0x%x.qID=0x%x\n", ret, queue_id);
            return OSAL_FAILURE;
        }
        return OSAL_SUCCESS;
    }

    BaseType_t taskwoken = pdFALSE;
    BaseType_t result = xQueueReceiveFromISR((QueueHandle_t)queue_id, buffer_addr, &taskwoken);
    if (result != pdPASS) {
        osal_log("xQueueReceiveFromISR failed! result = 0x%x.qID=0x%x\n", result, queue_id);
        return OSAL_FAILURE;
    }
    portYIELD_FROM_ISR(taskwoken);
    return OSAL_SUCCESS;
}

void osal_msg_queue_delete(unsigned long queue_id)
{
    if ((QueueHandle_t)queue_id == NULL) {
        osal_log("queueid is invalid\n");
        return;
    }
    vQueueUnregisterQueue((QueueHandle_t)queue_id);
    vQueueDelete((QueueHandle_t)queue_id);
}

int osal_msg_queue_is_full(unsigned long queue_id)
{
    if ((QueueHandle_t)queue_id == NULL) {
        osal_log("queueid is invalid\n");
        return OSAL_FAILURE;
    }
    return uxQueueSpacesAvailable((QueueHandle_t)queue_id) <= 0;
}

unsigned int osal_msg_queue_get_msg_num(unsigned long queue_id)
{
    if ((QueueHandle_t)queue_id == NULL) {
        osal_log("queueid is invalid\n");
        return OSAL_INVALID_MSG_NUM;
    }
    return uxQueueMessagesWaiting((QueueHandle_t)queue_id);
}

