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
#include "app_msg_manager.h"
#include "app_at_process.h"
#include "app_tx_process.h"

void app_at_msg_send(uint8_t *data, uint32_t data_len)
{
    if (data == NULL) {
        wstp_print("app_at_msg_send data is null!"NEWLINE);
        return;
    }

    at_cmd_msg_info_t msg;
    msg.payload_len = data_len;
    /* +1是为字符串结束符多申请内存 */
    msg.payload = osal_kmalloc(data_len + 1, OSAL_GFP_KERNEL);
    if (msg.payload == NULL) {
        wstp_print("app_at_msg_send payload memory alloc fail, length is "NEWLINE, data_len);
        return;
    }

    (void)memset_s(msg.payload, data_len + 1, 0, data_len + 1);
    if (memcpy_s(msg.payload, msg.payload_len, (const void *)data, data_len) != EOK) {
        wstp_print("app_at_msg_send receive data memcpy error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
        return;
    }

    if (osal_msg_queue_write_copy(get_app_at_queue_id(), (void *)&msg, sizeof(at_cmd_msg_info_t), 0) != OSAL_SUCCESS) {
        wstp_print("app_at_msg_send send msg error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
    }
}

void app_pkg_msg_send(uint8_t msg_id, uint8_t control, uint8_t *data, uint32_t data_len)
{
    app_msg_info_t* app_msg;
    app_msg = (app_msg_info_t*)osal_kmalloc(sizeof(app_msg_info_t) + data_len, OSAL_GFP_KERNEL);
    if (app_msg == NULL) {
        wstp_print("app_pkg_diag_msg_send malloc fail!!"NEWLINE);
        return;
    }
    memset_s(app_msg, sizeof(app_msg_info_t) + data_len, 0, sizeof(app_msg_info_t) + data_len);
    app_msg->id = msg_id;
    app_msg->control = control;
    app_msg->payload_len = data_len;

    if (data_len > 0 && data != NULL) {
        if (memcpy_s(app_msg->payload, app_msg->payload_len, (const void *)data, data_len) != EOK) {
            wstp_print("app_pkg_diag_msg_send data memcpy error!"NEWLINE);
            osal_kfree(app_msg);
            return;
        }
    }

    app_at_msg_send((uint8_t *)app_msg, sizeof(app_msg_info_t) + data_len);
    osal_kfree(app_msg);
}

void app_pkg_diag_msg_send(uint8_t *data, uint32_t data_len)
{
    app_pkg_msg_send(DIAG_COMMON_MSG, DIAG_CONTROL_TRUE, data, data_len);
}
