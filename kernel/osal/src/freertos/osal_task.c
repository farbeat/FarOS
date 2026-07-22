/*
* Copyright (c) CompanyNameMagicTag 2019-2022. All rights reserved.
* Description:osal_task
* Create: 2019-10-11
*/
#include <FreeRTOS.h>
#include <task.h>

#include "soc_osal.h"
#include "osal_inner.h"

#ifndef BYTE_PER_WORD
#define BYTE_PER_WORD 4
#endif

#define OS_SYS_US_PER_MS        1000
#define OS_SYS_US_PER_SECOND   1000000
#define OSAL_UINT32_MAX 0xffffffffU
#define MINIMAL_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define PRO_MAX 31

static int osal_map_priority(unsigned int priority)
{
    int ret;
    switch (priority) {
        case 0:
            ret = configMAX_PRIORITIES - 1;
            break;
        case 1:
            ret = 53;
            break;
        case 2:
            ret = 51;
            break;
        case 3:
            ret = 50;
            break;
        case 4:
            ret = 48;
            break;
        case 5:
            ret = 46;
            break;
        case 6:
            ret = 44;
            break;
        case 7:
            ret = 42;
            break;
        case 8:
            ret = 41;
            break;
        case 9:
            ret = 40;
            break;
        case 10:
            ret = 38;
            break;
        case 11:
            ret = 36;
            break;
        case 12:
            ret = 34;
            break;
        case 13:
            ret = 32;
            break;
        case 14:
            ret = 30;
            break;
        case 15:
            ret = 28;
            break;
        case 16:
            ret = 26;
            break;
        case 17:
            ret = 24;
            break;
        case 18:
            ret = 20;
            break;
        case 19:
            ret = 19;
            break;
        case 20:
            ret = 18;
            break;
        case 21:
            ret = 16;
            break;
        case 22:
            ret = 15;
            break;
        case 23:
            ret = 14;
            break;
        case 24:
            ret = 12;
            break;
        case 25:
            ret = 10;
            break;
        case 26:
            ret = 8;
            break;
        case 27:
            ret = 6;
            break;
        case 28:
            ret = 5;
            break;
        case 29:
            ret = 4;
            break;
        case 30:
            ret = 2;
            break;
        case 31:
            ret = 0;
            break;
        default:
            ret = 26;
            break;
    }
    return ret;
}

osal_task *osal_kthread_create_ext(osal_kthread_init *init_handle)
{
    if (init_handle == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }
    TaskHandle_t taskhandle = NULL;
    osal_task *pthread = (osal_task *)pvPortMalloc(sizeof(osal_task));
    if (pthread == NULL) {
        osal_log("pvPortMalloc error!\n");
        return NULL;
    }
    unsigned int stacksize = (init_handle->stacksize <= MINIMAL_STACK_SIZE) ? MINIMAL_STACK_SIZE : init_handle->stacksize;
    unsigned short stackwordsize = (unsigned short)(stacksize / BYTE_PER_WORD);
    unsigned long ret = xTaskCreate((TaskFunction_t)init_handle->handler, init_handle->taskname, stackwordsize, init_handle->data,
        osal_map_priority(init_handle->taskprio), &taskhandle);
    if (ret != pdPASS) {
        vPortFree(pthread);
        osal_log("xTaskCreate error!\n");
        return NULL;
    }
    pthread->task = taskhandle;
    return pthread;
}

osal_task *osal_kthread_create(osal_kthread_handler handler, void *data, const char *name, unsigned int stack_size)
{
    if (handler == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }
    TaskHandle_t taskhandle = NULL;
    osal_task *pthread = (osal_task *)pvPortMalloc(sizeof(osal_task));
    if (pthread == NULL) {
        osal_log("pvPortMalloc error!\n");
        return NULL;
    }
    unsigned int stacksize = (stack_size <= MINIMAL_STACK_SIZE) ? MINIMAL_STACK_SIZE : stack_size;
    unsigned short stackwordsize = (unsigned short)(stacksize / BYTE_PER_WORD);
    unsigned long ret = xTaskCreate((TaskFunction_t)handler, name, stackwordsize, data,
        (tskIDLE_PRIORITY + 1), &taskhandle);
    if (ret != pdPASS) {
        vPortFree(pthread);
        osal_log("xTaskCreate error!\n");
        return NULL;
    }
    pthread->task = taskhandle;
    return pthread;
}

int osal_kthread_set_priority(osal_task *task, unsigned int priority)
{
    if (task == NULL || task->task == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    if (priority > PRO_MAX) {
        osal_log("priority invalid!\n");
        return OSAL_FAILURE;
    }
    vTaskPrioritySet((TaskHandle_t)task->task, (unsigned int)osal_map_priority(priority));
    return OSAL_SUCCESS;
}

int osal_kthread_should_stop(void)
{
    return 0;
}

void osal_kthread_destroy(osal_task *task, unsigned int stop_flag)
{
    if (task == NULL || task->task == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    osal_unused(stop_flag);
    TaskHandle_t task_del = (TaskHandle_t)task->task;
    eTaskState state = eTaskGetState(task_del);
    vPortFree((void*)task);
    if (state == eDeleted) {
        return;
    }

    vTaskDelete(task_del);
}

void osal_yield(void)
{
    taskYIELD();
}

void osal_schedule(void)
{
    taskYIELD();
}

void osal_kthread_lock(void)
{
    vTaskSuspendAll();
}

void osal_kthread_unlock(void)
{
    xTaskResumeAll();
}

long osal_get_current_tid(void)
{
    return (long)xTaskGetCurrentTaskHandle();
}

unsigned long osal_msleep(unsigned int msecs)
{
    vTaskDelay(pdMS_TO_TICKS(msecs));
    return 0;
}

void osal_udelay(unsigned int usecs)
{
    uint64_t cycles = (uint64_t)usecs * configTICK_CLOCK_HZ / OS_SYS_US_PER_SECOND;
    uint64_t deadline = ullTickGetCycles() + cycles;
    while (ullTickGetCycles() < deadline) {
        portNOP();
    }
}

void osal_mdelay(unsigned int msecs)
{
    uint32_t r_msecs = msecs;
    uint32_t delayus = (OSAL_UINT32_MAX / OS_SYS_US_PER_MS) * OS_SYS_US_PER_MS;

    while (r_msecs > OSAL_UINT32_MAX / OS_SYS_US_PER_MS) {
        osal_udelay(delayus);
        r_msecs -= (OSAL_UINT32_MAX / OS_SYS_US_PER_MS);
    }
    osal_udelay(r_msecs * OS_SYS_US_PER_MS);
}

/* Description: to do in free rtos */
long osal_get_current_pid(void)
{
    return 0;
}