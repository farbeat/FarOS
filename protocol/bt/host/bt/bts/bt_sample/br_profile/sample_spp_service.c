/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test function of BT SPP Service module.
 */

#include "sample_spp_service.h"

#include <stdlib.h>
#include <td_base.h>
#include <soc_osal.h>
#include <securec.h>
#include "errcode.h"
#include "sample_common.h"
#include "bts_br_gap.h"
#include "bts_spp.h"
#include "debug_print.h"

#define SAMPLE_SPP_TASK_STACK_SIZE 2048
#define SAMPLE_SPP_TASK_PRI 20

#define SAMPLE_SPP_FILE_PATH_MAX_LEN 1024
#define SAMPLE_SPP_FILE_IO_BUF_LEN 128
#define SAMPLE_SPP_FILE_IO_TIMEOUT 120

/* BT_NUM */
#define BT_NUM2 2
#define BT_NUM6 6

/* 用于测试框架发送码流 */
#define SAMPLE_SPP_STREAM_CYCLE 255

/* a核文件系统暂未适配，暂时关闭相关处理函数 */
#define BTS_SPP_FILE_SYS NO

/* a2dp service test cmd id */
typedef enum {
    SAMPLE_SPP_CMD_ID_SERVER_CREATE,      /* 0 server创建 */
    SAMPLE_SPP_CMD_ID_SERVER_ACCEPT,      /* 1 server等待连接 */
    SAMPLE_SPP_CMD_ID_SERVER_CLOSE,       /* 2 server关闭 */
    SAMPLE_SPP_CMD_ID_CONNECT,            /* 3 client连接server */
    SAMPLE_SPP_CMD_ID_DISCONNECT,         /* 4 断开连接 */
    SAMPLE_SPP_CMD_ID_IS_SPP_CONNECT,     /* 5 获取连接状态 */
    SAMPLE_SPP_CMD_ID_GET_REMOTE_ADDR,    /* 6 获取远端设备地址 */
    SAMPLE_SPP_CMD_ID_READ,               /* 7 读数据 */
    SAMPLE_SPP_CMD_ID_WRITE,              /* 8 写数据 */
    SAMPLE_SPP_CMD_ID_FILE_RCV,           /* 9 接收文件数据 */
    SAMPLE_SPP_CMD_ID_FILE_SEND,          /* 10 发送文件数据 */
    SAMPLE_SPP_CMD_ID_DELAY_SAMPLE_RCV,     /* 11 时延测试接收端配置 */
    SAMPLE_SPP_CMD_ID_DELAY_SAMPLE_SEND,    /* 12 时延测试发送端配置 */
} sample_spp_service_cmd_id;

/* client连接server任务参数 */
typedef struct {
    uint32_t sample_mode;
    spp_create_socket_para_t socket_params;
    bd_addr_t addr;
} sample_spp_connect_task_params;

/* 写数据任务参数 */
typedef struct {
    uint32_t sample_mode;
    int client_id;
    int buf_len;
    int duration;
    int is_round_print;
    uint8_t read_buf[0];
} sample_spp_read_task_params;

/* 读数据任务参数 */
typedef struct {
    uint32_t sample_mode;
    int client_id;
    int data_len;
    int cycle_num;
    int delay_time;
    int is_round_print;
    uint8_t data[0];
} sample_spp_write_task_params;

/* 收发文件任务参数 */
typedef struct {
    int client_id;
    int timeout;
    char *save_file_path;
    char *src_file_path;
    char *buf;
} sample_spp_file_io_params;

/* 时延测试任务参数 */
typedef struct {
    int client_id;
    int data_len;
    int cycle_num;
    int is_print_time;
    char buf[0];
} sample_spp_delay_sample_params;

void wstp_spp_cmd_server_create(const char *spp_uuid, const char *name, uint8_t name_len)
{
    spp_create_socket_para_t param;
    if (generate_uuid_from_string((const char *)spp_uuid, (uint8_t)strlen(spp_uuid), &param.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("get uuid fail\n");
        return;
    }

    param.socket_type = SPP_SOCKET_RFCOMM;
    param.is_encrypt = true;

    bts_sample_log("uuid: %s uuid_len: %u\n", spp_uuid, param.uuid.uuid_len);
    bts_sample_log("socket_type: %x is_encrypt: %d name: %s nameLen: %u\n", param.socket_type,
        param.is_encrypt, name, name_len);

    int ret = spp_server_create(&param, name, name_len);
    bts_sample_log("spp_server_create ret: %x\n", ret);
}

/* 获取活跃设备 */
static void sample_spp_cmd_server_create(const char **params, int32_t params_cnt)
{
    spp_create_socket_para_t param;
    if (params_cnt != BT_NUM6) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    uint32_t sample_mode;
    if (sscanf_s((const char *)params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }

    if (generate_uuid_from_string((const char *)params[1], (uint8_t)strlen((const char *)params[1]),
        &param.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("get uuid fail\n");
        return;
    }

    param.socket_type = (spp_socket_type_t)atoi((const char *)params[2]); /* params idx 2 */
    param.is_encrypt = (atoi((const char *)params[3]) > 0 ? true : false); /* params idx 3 */

    const char *name = params[4]; /* params idx 4 */
    uint32_t name_len = (uint32_t)atoi((const char *)params[5]); /* params idx 5 */

    bts_sample_log("sample_mode: %x uuid: %s uuid_len: %u\n", sample_mode, params[1], param.uuid.uuid_len);
    bts_sample_log("socket_type: %x is_encrypt: %d name: %s nameLen: %u\n", param.socket_type,
        param.is_encrypt, name, name_len);

    int ret;
    if ((sample_mode & 0x3) == 0x3) { /* 0x3: 表示前两个参数均填NULL */
        ret = spp_server_create((spp_create_socket_para_t *)NULL, (char *)NULL, name_len);
    } else if ((sample_mode & 0x3) == 0x1) {
        ret = spp_server_create((spp_create_socket_para_t *)NULL, name, name_len);
    } else if ((sample_mode & 0x3) == 0x2) {
        ret = spp_server_create(&param, (char *)NULL, name_len);
    } else {
        ret = spp_server_create(&param, name, name_len);
    }
    bts_sample_log("spp_server_create ret: %x\n", ret);
}

static osal_task *g_spp_server_accept_task = NULL;
static bool g_spp_server_wait_accept = false;
static int sample_spp_server_accept_task_body(void *data)
{
    int server_id = (int)(intptr_t)data;
    bts_sample_log("server_id: %x\n", server_id);

    int ret = spp_server_accept(server_id);

    bts_sample_log("spp_server_accept ret: %x\n", ret);
    g_spp_server_wait_accept = false;
    bts_sample_log("spp_server_accept task %x task.task %x\n",
        g_spp_server_accept_task, g_spp_server_accept_task->task);
    if (g_spp_server_accept_task != NULL && g_spp_server_accept_task->task != NULL) {
        osal_kthread_destroy(g_spp_server_accept_task, 0);
        g_spp_server_accept_task = NULL;
    }

    return OSAL_SUCCESS;
}

/* 等待client连接 */
void wstp_spp_cmd_server_accept(uint32_t server_id)
{
    if (g_spp_server_wait_accept == true) {
        bts_sample_log("create task fail. exsit running task.\n");
        return;
    }
    g_spp_server_wait_accept = true;
    g_spp_server_accept_task = osal_kthread_create(sample_spp_server_accept_task_body,
        (void *)(uintptr_t)server_id,
        "spp_server_accept_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (g_spp_server_accept_task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(g_spp_server_accept_task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

/* 等待client连接 */
static void sample_spp_cmd_server_accept(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int server_id;
    if (sscanf_s((const char *)params[0], "%x", &server_id) <= 0) {
        bts_sample_log("get server_id fail\n");
        return;
    }

    osal_task *task = osal_kthread_create(sample_spp_server_accept_task_body,
        (void *)(uintptr_t)server_id,
        "spp_server_accept_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

/* 关闭server */
void wstp_spp_cmd_server_close(int32_t server_id)
{
    int ret = spp_server_close(server_id);
    bts_sample_log("spp_server_close ret: %x\n", ret);
}


/* 关闭server */
static void sample_spp_cmd_server_close(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int server_id;
    if (sscanf_s(params[0], "%x", &server_id) <= 0) {
        bts_sample_log("get server_id fail\n");
        return;
    }

    int ret = spp_server_close(server_id);

    bts_sample_log("spp_server_close ret: %x\n", ret);
}

static int sample_spp_client_connect_task_body(void *data)
{
    sample_spp_connect_task_params *task_params = (sample_spp_connect_task_params *)data;
    spp_create_socket_para_t *socket_param = &task_params->socket_params;

    bts_sample_log("testMode: %x uuid: %s", task_params->sample_mode, socket_param->uuid.uuid);
    for (int32_t i = 0; i < socket_param->uuid.uuid_len; i++) {
        bts_sample_log("%02x ", (uint8_t)socket_param->uuid.uuid[i]);
    }
    bts_sample_log("socket_type: %x is_encrypt: %d addr: ",
        socket_param->socket_type, socket_param->is_encrypt);
    for (int32_t i = BD_ADDR_LEN - 1; i >= 0; i--) {
        bts_sample_log("%02x", task_params->addr.addr[i]);
    }
    bts_sample_log("\n");

    int ret;
    if ((task_params->sample_mode & 0x3) == 0x3) { /* 0x3: 表示前两个参数均填NULL */
        ret = spp_connect((spp_create_socket_para_t *)NULL, (bd_addr_t *)NULL);
    } else if ((task_params->sample_mode & 0x3) == 0x1) {
        ret = spp_connect((spp_create_socket_para_t *)NULL, &task_params->addr);
    } else if ((task_params->sample_mode & 0x3) == 0x2) {
        ret = spp_connect(socket_param, (bd_addr_t *)NULL);
    } else {
        ret = spp_connect(socket_param, &task_params->addr);
    }
    bts_sample_log("spp_connect ret: %x\n", ret);

    bts_sample_mem_free(data);
    return OSAL_SUCCESS;
}

/* client连接server */
void wstp_spp_cmd_connect(const char *spp_uuid, const char *addr)
{
    uint8_t uuid_len = (uint8_t)strlen(spp_uuid);
    sample_spp_connect_task_params *task_params =
        (sample_spp_connect_task_params *)bts_sample_mem_new(sizeof(sample_spp_connect_task_params));

    task_params->sample_mode = 0;
    if (generate_uuid_from_string(spp_uuid, uuid_len + 1, &task_params->socket_params.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("generate uuid fail\n");
        return;
    }

    task_params->socket_params.socket_type = SPP_SOCKET_RFCOMM; /* params idx 2 */
    task_params->socket_params.is_encrypt = true; /* params idx 3 */

    if (decode_bd_addr_from_str(addr, task_params->addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) { /* idx 4 */
        bts_sample_log("decode addr error! input_addr: %s\n", addr); /* params idx 4 */
        return;
    }

    osal_task *task = osal_kthread_create(sample_spp_client_connect_task_body, task_params, "spp_client_connect_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

/* client连接server */
static void sample_spp_cmd_connect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 5) { /* 参数个数 5 */
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }
    uint8_t uuid_len = (uint8_t)strlen(params[1]);
    sample_spp_connect_task_params *task_params =
        (sample_spp_connect_task_params *)bts_sample_mem_new(sizeof(sample_spp_connect_task_params));

    if (sscanf_s(params[0], "%x", &task_params->sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }

    if (generate_uuid_from_string(params[1], uuid_len + 1, &task_params->socket_params.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("generate uuid fail\n");
        return;
    }

    task_params->socket_params.socket_type = (spp_socket_type_t)atoi(params[2]); /* params idx 2 */
    task_params->socket_params.is_encrypt = (atoi(params[3]) > 0 ? true : false); /* params idx 3 */

    if (decode_bd_addr_from_str(params[4], task_params->addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) { /* idx 4 */
        bts_sample_log("decode addr error! input_addr: %s\n", params[4]); /* params idx 4 */
        return;
    }

    osal_task *task = osal_kthread_create(sample_spp_client_connect_task_body, task_params, "spp_client_connect_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}


static int sample_spp_disconnect_task_body(void *data)
{
    int client_id = (int)(intptr_t)data;
    bts_sample_log("client_id: %x\n", client_id);

    int ret = spp_disconnect(client_id);

    bts_sample_log("spp_disconnect ret: %x\n", ret);

    return OSAL_SUCCESS;
}

/* spp断开连接 */
void wstp_spp_cmd_disconnect(uint32_t client_id)
{
    osal_task *task = osal_kthread_create(sample_spp_disconnect_task_body,
        (void *)(uintptr_t)client_id,
        "spp_disconnect_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success. client_id: 0x%x\n", client_id);
}

/* 断开连接 */
static void sample_spp_cmd_disconnect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int client_id;
    if (sscanf_s(params[0], "%x", &client_id) <= 0) {
        bts_sample_log("get client_id fail\n");
        return;
    }

    osal_task *task = osal_kthread_create(sample_spp_disconnect_task_body,
        (void *)(uintptr_t)client_id,
        "spp_disconnect_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

/* 检查SPP是否连接 */
void wstp_spp_cmd_is_spp_connect(int32_t client_id)
{
    bool ret = is_spp_connected(client_id);

    bts_sample_log("is_spp_connected ret(bool): %d\n", ret);
}

/* 检查SPP是否连接 */
static void sample_spp_cmd_is_spp_connect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int client_id;
    if (sscanf_s(params[0], "%x", &client_id) <= 0) {
        bts_sample_log("get client_id fail\n");
        return;
    }

    bool ret = is_spp_connected(client_id);

    bts_sample_log("is_spp_connected ret(bool): %d\n", ret);
}

/* 获取对端设备地址 */
void wstp_spp_cmd_get_remote_addr(int32_t client_id)
{
    bd_addr_t addr;
    int ret = spp_get_remote_addr(client_id, &addr);

    bts_sample_log("spp_get_remote_addr ret(int): %d addr: ", ret);
    if (ret == ERRCODE_BT_SUCCESS) {
        for (int32_t i = BD_ADDR_LEN - 1; i >= 0; i--) {
            bts_sample_log("%02x", addr.addr[i]);
        }
    }
    bts_sample_log("\n");
}

/* 获取对端设备地址 */
static void sample_spp_cmd_get_remote_addr(const char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }

    int client_id;
    if (sscanf_s(params[1], "%x", &client_id) <= 0) {
        bts_sample_log("get client_id fail\n");
        return;
    }

    bd_addr_t addr = {0};
    int ret;
    if ((sample_mode & 0x2) == 0x2) {
        ret = spp_get_remote_addr(client_id, NULL);
    } else {
        ret = spp_get_remote_addr(client_id, &addr);
    }

    bts_sample_log("spp_get_remote_addr ret(int): %d addr: ", ret);
    if (ret == ERRCODE_BT_SUCCESS) {
        for (int32_t i = BD_ADDR_LEN - 1; i >= 0; i--) {
            bts_sample_log("%02x", addr.addr[i]);
        }
    }
    bts_sample_log("\n");
}

static void sample_spp_conn_state_changed_callback(uint8_t client_id, profile_connect_state_t state)
{
    if (state == PROFILE_STATE_CONNECTED) {
        bts_sample_log("client:%u connected\n", client_id);
    } else {
        bts_sample_log("client:%u disconnected\n", client_id);
    }
}

static void sample_spp_proc_apk_data(uint8_t client_id, uint8_t *data, uint32_t data_len)
{
    if (data == NULL) {
        bts_sample_log("read_buf is empty\n");
        return;
    }
    if (data_len > 0) {
        bts_sample_log("rcv spp msg from client:%u, data_len=%u\n", client_id, data_len);
    }
}

int sample_spp_register_callbacks(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    spp_callbacks_t callbacks = {
        sample_spp_proc_apk_data,
        sample_spp_conn_state_changed_callback,
    };
    ret = (errcode_t)spp_register_callbacks(&callbacks);
    return (int)ret;
}

static int32_t sample_spp_read_input_data(const char **params, int32_t params_cnt, uint8_t *data, int32_t data_len)
{
    int32_t i;
    uint32_t tmp_data;
    for (i = 0; i < params_cnt; i++) {
        if (sscanf_s(params[i], "%x", &tmp_data) <= 0) { /* 7 固定参数个数 */
            break;
        }
        data[i] = (uint8_t)tmp_data;
    }
    if (i != params_cnt) {
        return i;
    }

    for (; i < data_len; i++) {
        data[i] = data[i % params_cnt];
    }

    return data_len;
}

static int sample_spp_write_task_body(void *data)
{
    sample_spp_write_task_params *task_params = (sample_spp_write_task_params *)data;
    long tid = osal_get_current_tid();

    bts_sample_log("testMode: %x client_id: %x dataLen: %d cycleNum: %d delayTime: %d isPrint: %d tid: %x\n",
        task_params->sample_mode, task_params->client_id, task_params->data_len, task_params->cycle_num,
        task_params->delay_time, task_params->is_round_print, (uint32_t)tid);

    uint32_t start_sec = sample_get_sec();
    uint32_t end_sec = 0;
    int round = 0;
    uint32_t send_bytes = 0;
    int ret = 1;
    while (ret > 0 && round < task_params->cycle_num) {
        if (round > 0 && task_params->delay_time > 0) {
            osal_msleep((unsigned int)task_params->delay_time);
        }

        if ((task_params->sample_mode & 0x2) == 0x2) {
            ret = spp_write(task_params->client_id, NULL, (uint32_t)task_params->data_len);
        } else {
            ret = spp_write(task_params->client_id, (const char *)task_params->data, (uint32_t)task_params->data_len);
        }
        if (task_params->is_round_print > 0) {
            bts_sample_log("round: %d ret: %d\n", round, ret);
        }

        end_sec = sample_get_sec();
        round++;
        if (ret > 0) {
            send_bytes += (uint32_t)ret;
        }
    };

    uint32_t duration = time_sub_duration(end_sec, start_sec);
    bts_sample_log("write task end. tid: %x startSec: %u endSec: %u sendBytes: %u Bps: %u\n",
        (uint32_t)tid, start_sec, end_sec, send_bytes, (duration > 0 ? (send_bytes / duration) : 0));

    bts_sample_mem_free(data);
    return OSAL_SUCCESS;
}

/* 写数据 */
void wstp_spp_cmd_write(int32_t client_id, int32_t repeat_times, int32_t cycle_num, int32_t delay_time)
{
    int input_data_len = 1; /* 2: param idx */
    int data_len = (int)(input_data_len * repeat_times);

    sample_spp_write_task_params *task_params =
        (sample_spp_write_task_params *)(void *)bts_sample_mem_new(sizeof(sample_spp_write_task_params) + data_len);
    if (task_params == NULL) {
        bts_sample_log("malloc task_params fail\n");
        return;
    }

    task_params->sample_mode = 0;
    task_params->client_id = client_id;
    task_params->data_len = data_len;
    task_params->cycle_num = cycle_num; /* 4: params idx */
    task_params->delay_time = delay_time; /* 5: params idx */
    task_params->is_round_print = 0; /* 6: params idx */
    for (int i = 0; i < data_len; i++) {
        task_params->data[i] = i % SAMPLE_SPP_STREAM_CYCLE;
    }

    osal_task *task = osal_kthread_create(sample_spp_write_task_body, task_params, "spp_write_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        bts_sample_mem_free(task_params);
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

static int32_t sample_spp_cmd_write_set_task_param(sample_spp_write_task_params *task_params, const char **params)
{
    int input_data_len = atoi(params[2]); /* 2: param idx */
    int repeat_times = atoi(params[3]); /* 3: param idx */
    int data_len = (int)(input_data_len * repeat_times);
    if (sscanf_s(params[0], "%x", &task_params->sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return EXT_FAILURE;
    }
    if (sscanf_s(params[1], "%x", &task_params->client_id) <= 0) {
        bts_sample_log("get client_id fail\n");
        return EXT_FAILURE;
    }

    task_params->data_len = data_len;
    task_params->cycle_num = atoi(params[4]); /* 4: params idx */
    task_params->delay_time = atoi(params[5]); /* 5: params idx */
    task_params->is_round_print = atoi(params[6]); /* 6: params idx */

    return EXT_SUCCESS;
}

/* 写数据 */
static void sample_spp_cmd_write(const char **params, int32_t params_cnt)
{
    const int fixed_params_num = 7; /* 7 本命令固定参数个数 */
    if (params_cnt < fixed_params_num) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int input_data_len = atoi(params[2]); /* 2: param idx */
    int repeat_times = atoi(params[3]); /* 3: param idx */
    int data_len = (int)(input_data_len * repeat_times);
    if (params_cnt != fixed_params_num + input_data_len) {
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    sample_spp_write_task_params *task_params =
        (sample_spp_write_task_params *)(void *)bts_sample_mem_new(sizeof(sample_spp_write_task_params) + data_len);
    if (task_params == NULL) {
        bts_sample_log("malloc task_params fail\n");
        return;
    }

    int32_t ret = sample_spp_cmd_write_set_task_param(task_params, params);
    if (ret != EXT_SUCCESS) {
        bts_sample_mem_free(task_params);
        return;
    }

    int32_t read_num = sample_spp_read_input_data(params + fixed_params_num, input_data_len, task_params->data,
        data_len);
    if (read_num != data_len) {
        bts_sample_mem_free(task_params);
        bts_sample_log("get data fail. get succ cnt %d\n", read_num);
        return;
    }

    osal_task *task = osal_kthread_create(sample_spp_write_task_body, task_params, "spp_write_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        bts_sample_mem_free(task_params);
        return;
    }
    ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success.\n");
}

#if (BTS_SPP_FILE_SYS == YES)
/* 发送文件测试任务body */
static int sample_spp_file_send_task_body(void *data)
{
    sample_spp_file_io_params *task_params = (sample_spp_file_io_params *)data;
    long tid = osal_get_current_tid();

    bts_sample_log("tid: %x client_id: %x srcFilePath: %s\n", tid, task_params->client_id,
        task_params->src_file_path);

    /* 计算源文件大小 */
    int src_file_size = get_file_size(task_params->src_file_path);
    if (src_file_size == -1) {
        bts_sample_log("cal src file size error\n");
        bts_sample_mem_free(task_params);
        return OSAL_SUCCESS;
    }

    /* 打开发送文件 */
    FILE *fp = fopen(task_params->src_file_path, "rb");
    if (fp == NULL) {
        bts_sample_log("open src file error fp: %x\n", fp);
        bts_sample_mem_free(task_params);
        return OSAL_SUCCESS;
    }

    int send_bytes = 0;
    while (send_bytes < src_file_size) {
        int read_len = fread(task_params->buf, 1, SAMPLE_SPP_FILE_IO_BUF_LEN, fp);
        if (read_len <= 0) {
            bts_sample_log("read file error. fp: %x read_len: %d\n", fp, read_len);
            break;
        }

        int write_len = spp_write(task_params->client_id, task_params->buf, read_len);
        if (write_len != read_len) {
            bts_sample_log("SPP write error! writeLen: %d realWriteLen: %d\n", read_len, write_len);
            break;
        }

        send_bytes += write_len;
        if (send_bytes % 2048 == 0) { /* 每2048字节打印一下进度 */
            bts_sample_log("send_bytes: %d\n", send_bytes);
        }
    };

    if (fclose(fp) != 0) {
        bts_sample_log("close src file error fp: %x\n", fp);
        bts_sample_mem_free(task_params);
        return OSAL_SUCCESS;
    }

    bts_sample_log("file send task end. tid: %x src_file_size: %d send_bytes: %d\n",
        tid, src_file_size, send_bytes);

    bts_sample_mem_free(task_params);
    return OSAL_SUCCESS;
}

/* 发送文件数据 */
static void sample_spp_cmd_file_send(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2 固定参数个数 */
        bts_sample_log("invalid params cnt %x\n", params_cnt);
        return;
    }

    int src_path_len = strnlen(params[1], SAMPLE_SPP_FILE_PATH_MAX_LEN) + 1;

    int tesk_params_len = sizeof(sample_spp_file_io_params) + src_path_len + SAMPLE_SPP_FILE_IO_BUF_LEN;
    sample_spp_file_io_params *task_params = (sample_spp_file_io_params *)bts_sample_mem_new(tesk_params_len);
    if (task_params == NULL) {
        bts_sample_log("malloc task_params fail\n");
        return;
    }
    (void)memset_s(task_params, tesk_params_len, 0, tesk_params_len);
    task_params->src_file_path = (char *)task_params + sizeof(sample_spp_file_io_params);
    task_params->buf = (char *)task_params + sizeof(sample_spp_file_io_params) + src_path_len;

    if (sscanf_s(params[0], "%x", &task_params->client_id) <= 0) {
        bts_sample_log("get client_id fail\n");
        return;
    }

    (void)strncpy_s(task_params->src_file_path, src_path_len, params[1], src_path_len);

    osal_task *task = osal_kthread_create(sample_spp_file_send_task_body, task_params, "spp_file_send_task",
        SAMPLE_SPP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        bts_sample_mem_free(task_params);
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_SPP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }

    bts_sample_log("create task success. task: %x\n", task);
}
#endif

static btsrv_sample_cmd_hdl_info g_spp_sample_cmd_hdls[] = {
    /* 0 */
    { SAMPLE_SPP_CMD_ID_SERVER_CREATE, sample_spp_cmd_server_create },
    { SAMPLE_SPP_CMD_ID_SERVER_ACCEPT, sample_spp_cmd_server_accept },
    { SAMPLE_SPP_CMD_ID_SERVER_CLOSE, sample_spp_cmd_server_close },
    { SAMPLE_SPP_CMD_ID_CONNECT, sample_spp_cmd_connect },
    { SAMPLE_SPP_CMD_ID_DISCONNECT, sample_spp_cmd_disconnect },

    /* 5 */
    { SAMPLE_SPP_CMD_ID_IS_SPP_CONNECT, sample_spp_cmd_is_spp_connect },
    { SAMPLE_SPP_CMD_ID_GET_REMOTE_ADDR, sample_spp_cmd_get_remote_addr },
    { SAMPLE_SPP_CMD_ID_READ, NULL },
    { SAMPLE_SPP_CMD_ID_WRITE, sample_spp_cmd_write },
#if (BTS_SPP_FILE_SYS == YES)
    { SAMPLE_SPP_CMD_ID_FILE_RCV, NULL },
    { SAMPLE_SPP_CMD_ID_FILE_SEND, sample_spp_cmd_file_send },
#endif
    { SAMPLE_SPP_CMD_ID_DELAY_SAMPLE_RCV, NULL },
    { SAMPLE_SPP_CMD_ID_DELAY_SAMPLE_SEND, NULL }
};

void bt_spp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_spp_sample_cmd_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_spp_sample_cmd_hdls[i].id == cmd_id && g_spp_sample_cmd_hdls[i].func != NULL) {
            g_spp_sample_cmd_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("cmd_id %d not support now!\n", cmd_id);
}
