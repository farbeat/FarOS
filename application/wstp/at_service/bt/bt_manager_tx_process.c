/**
 * Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides at cmd manager source\n
 * Author: CompanyName \n
 * History: \n
 * 2022-11-17， Create file. \n
 */
#include "debug_print.h"
#include "soc_osal.h"
#include "at_cmd_api.h"
#include "app_at_process.h"
#include "bt_manager_msg_manager.h"
#include "bt_manager_tx_process.h"

void bt_manager_msg_send(uint8_t *data, uint32_t data_len)
{
    if (data == NULL) {
        wstp_print("app_at_msg_send data is null!"NEWLINE);
        return;
    }

    at_cmd_msg_info_t msg;
    msg.payload_len = data_len;
    msg.payload = osal_kmalloc(data_len, OSAL_GFP_KERNEL);
    if (msg.payload == NULL) {
        wstp_print("app_at_msg_send payload memory alloc fail, length is "NEWLINE, data_len);
        return;
    }

    if (memcpy_s(msg.payload, msg.payload_len, (const void *)data, data_len) != EOK) {
        wstp_print("app_at_msg_send receive data memcpy error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
        return;
    }

    if (osal_msg_queue_write_copy(get_bt_manager_queue_id(), (void *)&msg, sizeof(at_cmd_msg_info_t), 0) != OSAL_SUCCESS) {
        wstp_print("app_at_msg_send send msg error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
    }
}
