/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: dfx freertos task
 * This file should be changed only infrequently and with great care.
 */

#include "dfx_task.h"
#include <FreeRTOS.h>
#include <task.h>
#include "securec.h"
#include "errcode.h"

uint32_t dfx_os_get_task_cnt(void)
{
    return uxTaskGetNumberOfTasks();
}

errcode_t dfx_os_get_all_task_info(task_info_t *inf, uint32_t info_cnt)
{
    uint32_t current_number_of_tasks = (uint32_t)uxTaskGetNumberOfTasks();
    TaskStatus_t *task_status_array;
    uint32_t x;
    uint32_t array_size;

    task_status_array = (TaskStatus_t *)pvPortMalloc(current_number_of_tasks * sizeof(TaskStatus_t));
    if (task_status_array == NULL) {
        return ERRCODE_MALLOC;
    }

    array_size = (uint32_t)uxTaskGetSystemState(task_status_array, array_size, NULL);

    (void)memset_s(inf, info_cnt * sizeof(task_info_t), 0, info_cnt * sizeof(task_info_t));
    for (x = 0; x < array_size && x < info_cnt; x++) {
        task_info_t *info = &inf[x];
        if (memcpy_s(info->name, sizeof(info->name), task_status_array[x].pcTaskName,
                     strlen(task_status_array[x].pcTaskName)) != EOK) {
            vPortFree(task_status_array);
            return ERRCODE_FAIL;
        }
        info->id = (uint32_t)task_status_array[x].xTaskNumber;
        info->valid = (task_status_array[x].eCurrentState != eInvalid) ? true : false;
        info->status = task_status_array[x].eCurrentState;
        info->priority = (uint16_t)task_status_array[x].uxCurrentPriority;
        info->stack_size = 0;
        info->bottom_of_stack = (uint32_t)(intptr_t)task_status_array[x].pxStackBase;
        info->overflow_flag = (task_status_array[x].usStackHighWaterMark > 0) ? false : true;
    }
    vPortFree(task_status_array);
    return ERRCODE_SUCC;
}

errcode_t dfx_os_get_task_info(uint32_t taskid, task_info_t *info)
{
    return ERRCODE_FAIL;
}

const char *dfx_os_task_status_convert_str(uint16_t task_status)
{
    return "Invaild";
}
