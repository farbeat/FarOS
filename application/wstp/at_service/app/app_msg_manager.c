/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include "debug_print.h"
#include "securec.h"
#include "common_def.h"
#include "soc_osal.h"
#include "cmsis_os2.h"
#include "app_msg_manager.h"

#define APP_AT_MSG_COUNT       16

static unsigned long g_app_at_queue = 0xFFFFFFFF;

void app_at_task_queue_init(void)
{
    osal_msg_queue_create("app_at_service_queue", APP_AT_MSG_COUNT, &g_app_at_queue, 0, sizeof(at_cmd_msg_info_t));
}

unsigned long get_app_at_queue_id(void)
{
    return g_app_at_queue;
}