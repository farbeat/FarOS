/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include "common_def.h"
#include "soc_osal.h"
#include "at_cmd_api.h"
#include "bt_manager_msg_manager.h"
#include "bt_manager_at_process.h"
#include "bt_manager_rx_process.h"

void bt_manager_task_process_handler(void *param)
{
    unused(param);
    int32_t ret = OSAL_FAILURE;
    at_cmd_msg_info_t msg_data;
    uint32_t msg_size = sizeof(at_cmd_msg_info_t);

    bt_manager_at_init();
    while (1) {
        (void)memset_s(&msg_data, sizeof(msg_data), 0, sizeof(msg_data));
        ret = osal_msg_queue_read_copy(get_bt_manager_queue_id(), &msg_data, &msg_size, OSAL_MSGQ_WAIT_FOREVER);
        if (ret != OSAL_SUCCESS) {
            continue;
        }

        bt_manager_at_process(msg_data.payload, msg_data.payload_len);
        osal_kfree(msg_data.payload);
    }
}
