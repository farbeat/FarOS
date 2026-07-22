/*
 * Copyright (c) @CompanyNameMagicTag Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:  At cmd source file.
 * Author: @CompanyNameTag
 * Create:
 */
#include "stdint.h"
#include <stdlib.h>
#include "securec.h"
#include "common_def.h"
#include "debug_print.h"
#include "soc_osal.h"
#include "cmsis_os2.h"
#include "at_cmd_api.h"
#include "ipc.h"
#include "ipc_protocol.h"
#include "common.h"
#include "key_id.h"

#define AT_CMD_BUFSIZE          256
#define AT_CMD_NAME_MAX_LEN     32
#define AT_CMD_END_MARK_LEN     2
#define AT_CMD_MODULE_MAX_NUM   5
#define AT_CMD_MSG_COUNT        16
#define OFF_TWO                 2
#define UART2_IPC_SEND_TIMEOUT  500
/**
 * @brief At cmd table set structure.
 */
typedef struct at_cmd_set_t {
    at_cmd_table_t *cmd_set;
    uint32_t count;
} at_cmd_set_t;

typedef struct {
    uint16_t data_len;
    uint8_t  data[AT_CMD_BUFSIZE];
} ipc_payload_atcmd_t;

static unsigned long g_at_cmd_msg_queue = 0xFFFFFFFF;

static at_cmd_set_t g_at_cmd_module_list[AT_CMD_MODULE_MAX_NUM];
static uint8_t g_at_cmd_module_index = 0;

errcode_t uapi_nv_read(uint16_t key, uint16_t kvalue_max_length, uint16_t *kvalue_length, uint8_t *kvalue);

/* 根据AT命令cmd_name，遍历table，确认对应AT命令的处理handle，并传入AT命令参数 */
static uint32_t at_cmd_search_and_process(uint32_t cmd_end_index, const char *cmd_name, uint8_t *data, uint32_t data_len)
{
    uint32_t para_len = 0;
    uint16_t cmd_count;
    at_cmd_table_t *moudule_cmd = NULL;

    for (uint32_t i = 0; i < g_at_cmd_module_index; i++) {
        moudule_cmd = g_at_cmd_module_list[i].cmd_set;
        cmd_count = g_at_cmd_module_list[i].count;
        for (uint32_t index = 0; index < cmd_count; ++index) {
            if (strcasecmp(cmd_name, moudule_cmd[index].str_cmd) == 0) {
                if (data_len == cmd_end_index) {
                    para_len = 0;
                } else {
                    para_len = data_len - cmd_end_index - 1; /* 下传参数长度，-1为等号长度 */
                }
                return moudule_cmd[index].cmd_handle(&data[cmd_end_index + 1], para_len, moudule_cmd[index].str_common);
            }
        }
    }
    return ERRCODE_INVALID_PARAM;
}

/* 解析AT命令中命令部分，传参给cmd_name */
static uint32_t at_cmd_parse_cmd_name(const uint8_t *data, uint32_t data_len, char *cmd_name, uint32_t *cmd_end_index)
{
    if ((data == NULL) || (cmd_name == NULL) || (cmd_end_index == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

    uint32_t i = 0;
    for (; i < data_len; ++i) {
        if ((data[i]  == '=') || (data[i] == '\0')) {
            break;
        }
    }

    if (memcpy_s(cmd_name, AT_CMD_NAME_MAX_LEN, data, i) != EOK) {
        PRINT("[at cmd] memcpy_s fail"NEWLINE);
        return ERRCODE_MEMCPY;
    }

    cmd_name[i] = '\0';
    *cmd_end_index = i;

    return ERRCODE_SUCC;
}

static void at_cmd_process(uint8_t *data, uint32_t data_len)
{
    if (data == NULL) { return; }
    char cmd_name[AT_CMD_NAME_MAX_LEN];
    uint32_t cmd_end_index;
    uint16_t pos = 0;
    uint32_t i;
    static uint8_t at_buf[AT_CMD_BUFSIZE];
    static uint16_t buf_len = 0;
    (void)memset_s(cmd_name, AT_CMD_NAME_MAX_LEN, 0, AT_CMD_NAME_MAX_LEN);

    /* AT cmd最大长度为256 */
    if (buf_len + data_len > AT_CMD_BUFSIZE) {
        PRINT("[at cmd]ERR!at cmd len over %d!\r\n", AT_CMD_BUFSIZE);
        buf_len = 0;
        return;
    }

    /* 由于uart buf为56，AT cmd超长需要组包 */
    for (i = 0; i < data_len; ++i) {
        if (data[i] == (uint8_t)'\r') {
            memcpy_s(at_buf + buf_len, AT_CMD_BUFSIZE - buf_len, data, i);
            buf_len += i;
            at_buf[buf_len] = '\0';
            // AT cmd添加结束符，长度+1
            buf_len += 1;
            break;
        } else if (data[i] == (uint8_t)'\n') {
            // 解决因为\r\n因为长度被截断，导致的异常
            buf_len = 0;
            return;
        }
    }

    if (i == data_len) {
        memcpy_s(at_buf + buf_len, AT_CMD_BUFSIZE - buf_len, data, data_len);
        buf_len += data_len;
        return;
    }

    /* 确认AT命令头 */
    for (i = 0; i < buf_len; i++) {
        if (at_buf[i] == (uint8_t)'A' && at_buf[i + 1] == (uint8_t)'T' &&
            at_buf[i + OFF_TWO] == (uint8_t)'^') {
            pos = i;
            break;
        }
    }

    if (at_cmd_parse_cmd_name(&at_buf[pos], (buf_len - pos), cmd_name, &cmd_end_index) != ERRCODE_SUCC) {
        PRINT("[at cmd]parse cmd name err!");
        buf_len = 0;
        return;
    }
    if (at_cmd_search_and_process(cmd_end_index, cmd_name, &at_buf[pos], (buf_len - pos)) != ERRCODE_SUCC) {
        PRINT("[at cmd]cannot find cmd handle, data=%s len=%d"NEWLINE, (char *)&at_buf[pos], (buf_len - pos));
    }
    buf_len = 0;
    return;
}

#ifdef FT_BT_UART
static bool at_cmd_get_abt_from_bt(ipc_action_t message, const volatile ipc_payload *payload_p,
                                   cores_t src, uint32_t id)
{
    unused(message);
    unused(src);
    unused(id);
    if (payload_p == NULL) {
        return true;
    }

    ipc_payload_atcmd_t *ipc_msg = (ipc_payload_atcmd_t*)(payload_p);
    at_cmd_msg_receive_handler(ipc_msg->data, ipc_msg->data_len);
    return true;
}
 
void at_cmd_send_abt_result_to_bt(uint8_t *result, uint32_t result_len)
{
    ipc_payload_atcmd_t ipc_msg = {0};
    if (result == NULL || result_len == 0 || result_len >= AT_CMD_BUFSIZE) {
        PRINT("abt result err"NEWLINE);
        return;
    }
 
    ipc_msg.data_len = result_len;
    memcpy_s(ipc_msg.data, AT_CMD_BUFSIZE, result, result_len);
    if (memcpy_s(ipc_msg.data, AT_CMD_BUFSIZE, result, result_len) != EOK) {
        PRINT("abt result memcpy err"NEWLINE);
        return;
    }
    ipc_spin_send_message_timeout(CORES_BT_CORE, IPC_ACTION_BT_ATCMD, &ipc_msg, sizeof(ipc_msg),
                                  IPC_PRIORITY_HIGHEST, true, UART2_IPC_SEND_TIMEOUT);
}
#endif

void at_cmd_set_calibration_to_bt(btsrv_calibration_config_t *config, uint16_t value_len)
{
    ipc_send_message(CORES_BT_CORE, IPC_ACTION_BT_CALI, config, value_len, IPC_PRIORITY_LOWEST, false);
}

void set_calibration_from_nv(void)
{
    uint32_t kvalue_max_length = sizeof(btsrv_calibration_config_t);
    btsrv_calibration_config_t *kvalue = (btsrv_calibration_config_t*)malloc(kvalue_max_length);
    if (kvalue == NULL) {
        PRINT("calibration memory malloc fail"NEWLINE);
        return;
    }
    uint16_t real_len = 0;
    uapi_nv_read(NV_ID_BTSRV_CALIBRATION_CONFIG, kvalue_max_length, &real_len, kvalue);
    if (real_len == 0) {
        PRINT("calibration nv is not active\r\n"NEWLINE);
        free(kvalue);
        return;
    }
    ipc_calibration_config_t *ipc_msg = (ipc_calibration_config_t*)malloc(sizeof(ipc_calibration_config_t));
    if (ipc_msg == NULL) {
        PRINT("ipc_calibration memory malloc fail"NEWLINE);
        return;
    }
    ipc_msg->length = real_len;
    if (memcpy_s(ipc_msg->params, sizeof(ipc_calibration_config_t), kvalue, real_len) == EOK) {
        at_cmd_set_calibration_to_bt(ipc_msg, sizeof(ipc_calibration_config_t));
    } else {
        PRINT("calibration_config memcpy err"NEWLINE);
    }
    free(ipc_msg);
    free(kvalue);
}

void at_cmd_task_process_handler(void *param)
{
    unused(param);
    int32_t ret = OSAL_FAILURE;
    at_cmd_msg_info_t msg_data;
    uint32_t msg_size = sizeof(at_cmd_msg_info_t);

#ifdef FT_BT_UART
    /* 注册从其他核过来的ATCMD消息处理 */
    ipc_register_handler(IPC_ACTION_BT_ATCMD, at_cmd_get_abt_from_bt);
#endif

    while (1) {
        (void)memset_s(&msg_data, sizeof(msg_data), 0, sizeof(msg_data));
        ret = osal_msg_queue_read_copy(g_at_cmd_msg_queue, &msg_data, &msg_size, OSAL_MSGQ_WAIT_FOREVER);
        if (ret != OSAL_SUCCESS) {
            continue;
        }

        at_cmd_process(msg_data.payload, msg_data.payload_len);
        osal_kfree(msg_data.payload);
    }
}

void at_cmd_msg_receive_handler(uint8_t *data, uint32_t data_len)
{
    if (data == NULL) {
        PRINT("at cmd receive data is null!"NEWLINE);
        return;
    }

    at_cmd_msg_info_t msg;
    msg.payload_len = data_len;
    msg.payload = osal_kmalloc(data_len, OSAL_GFP_KERNEL);
    if (msg.payload == NULL) {
        PRINT("msg payload memory alloc fail, length is "NEWLINE, data_len);
        return;
    }

    if (memcpy_s(msg.payload, msg.payload_len, (const void *)data, data_len) != EOK) {
        PRINT("at cmd receive data memcpy error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
        return;
    }

    if (osal_msg_queue_write_copy(g_at_cmd_msg_queue, (void *)&msg, sizeof(at_cmd_msg_info_t), 0) != OSAL_SUCCESS) {
        PRINT("at cmd receive send msg error!"NEWLINE);
        osal_kfree(msg.payload);
        msg.payload = NULL;
    }
}

errcode_t at_cmd_add_moudle(at_cmd_table_t *module_cmd_list, uint32_t cmd_count)
{
    if (g_at_cmd_module_index >= AT_CMD_MODULE_MAX_NUM) {
        return ERRCODE_FAIL;
    }
    g_at_cmd_module_list[g_at_cmd_module_index].cmd_set = module_cmd_list;
    g_at_cmd_module_list[g_at_cmd_module_index].count = cmd_count;
    g_at_cmd_module_index++;
    return ERRCODE_SUCC;
}

void at_cmd_task_queue_init(void)
{
    osal_msg_queue_create("at_queue", AT_CMD_MSG_COUNT, &g_at_cmd_msg_queue, 0, sizeof(at_cmd_msg_info_t));
}