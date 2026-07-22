/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include "securec.h"
#include "soc_osal.h"
#if defined(__FREERTOS__)
#include "osal_adapt.h"
#endif
#include "common_def.h"
#include "debug_print.h"
#include "at_cmd_api.h"
#include "app_common_api.h"
#include "app_msg_manager.h"
#include "app_at_process.h"
#include "app_rx_process.h"

void app_at_task_init(void)
{
#ifdef SUPPORT_DIAG_V2_PROTOCOL
    uapi_diag_service_register(DIAG_SER_APP_TEST, mcu_diag_cmd_process);
#endif
#if defined(__FREERTOS__)
    osal_register_print(wstp_print);
#endif
}

void app_at_task_process_handler(void *param)
{
    unused(param);
    int32_t ret = OSAL_FAILURE;
    at_cmd_msg_info_t msg_data;
    uint32_t msg_size = sizeof(at_cmd_msg_info_t);

    app_at_task_init();
    while (1) {
        (void)memset_s(&msg_data, sizeof(msg_data), 0, sizeof(msg_data));
        ret = osal_msg_queue_read_copy(get_app_at_queue_id(), &msg_data, &msg_size, OSAL_MSGQ_WAIT_FOREVER);
        if (ret != OSAL_SUCCESS) {
            continue;
        }

        app_at_process(msg_data.payload, msg_data.payload_len);
        osal_kfree(msg_data.payload);
    }
}

