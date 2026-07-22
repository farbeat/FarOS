/*
 * Copyright (c) CompanyNameMagicTag 2024. All rights reserved.
 * Description: This module include test function of ANCS Client module.
 */
#include <stdint.h>
#include <stdlib.h>
#include <td_base.h>
#include "osal_addr.h"
#include "sample_common.h"
#include "ble_ancs_client.h"
#include "sample_ble_ancs.h"

#define TEN 10
#define ANCS_ATT_BYTE_LENGTH 6
#define ANCS_DEC_CHAR_COUNT 2
#define BYTE_BIT_COUNT 8

/* ANCS客户端接口命令id */
typedef enum {
    BLE_SAMPLE_ANCS_CLIENT_REGISTER_CBK = 1, /* 1 注册ANCS客户端回调函数 */
    BLE_SAMPLE_ANCS_CLIENT_SETUP,            /* 2  ANCS客户端连接服务端 */
    BLE_SAMPLE_ANCS_CLIENT_SUBSCRIBE_NOTITY, /* 3  订阅通知 */
    BLE_SAMPLE_ANCS_CLIENT_SUBSCRIBE_DATA,   /* 4  订阅数据 */
    BLE_SAMPLE_ANCS_CLIENT_PERFORM_ACTION,   /* 5  执行操作 */
    BLE_SAMPLE_ANCS_CLIENT_NOTIFY_ATT_GET,   /* 6  执行操作 */
    BLE_SAMPLE_ANCS_CLIENT_APP_ATT_GET       /* 7  执行操作 */
} ble_ancs_client_at_cmd_id_t;

/* 命令处理函数接口 */
typedef void (*ble_ancs_client_at_func_t)(uint8_t argc, char **argv);

/* ANCS客户端AT命令id与接口对应关系 */
typedef struct {
    ble_ancs_client_at_cmd_id_t id;
    ble_ancs_client_at_func_t func;
} ble_ancs_client_at_hdl_info_t;

/* ANCS服务建立结果回调 */
static void ble_ancs_client_setup_result_cbk(uint8_t client_id, uint16_t conn_id, errcode_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[ANCSClientSetupCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
}

/* ANCS服务订阅结果回调 */
static void ble_ancs_client_subscribe_result_cbk(uint8_t client_id, uint16_t conn_id,
    ancs_client_subscribe_type_t type, gatt_status_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[ANCSClientSubscribeResultCBK]conn_id:%u type:%d status:0x%x\r\n",
        conn_id, type, status);
}

/* ANCS服务命令执行结果回调 */
static void ble_ancs_client_write_result_cbk(uint8_t client_id, uint16_t conn_id, gatt_status_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[ANCSClientCMDResultCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
}

/* ANCS服务消息通知回调 */
static void ble_ancs_client_notify_source_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_src_t *notify_src, errcode_t status)
{
    bts_unused(client_id);
    if (status == ERRCODE_BT_SUCCESS) {
        bts_sample_info_log(
            "[ANCSClientNotifySRCCBK]conn_id:%u status:0x%x notify_id:%lu "
            "eventId:%u eventFlags:0x%x category_id:%d\r\n",
            conn_id, status,
            notify_src->notification_uid,
            notify_src->event_id,
            notify_src->event_flags,
            notify_src->category_id);
    } else {
        bts_sample_info_log("[ANCSClientNotifySRCCBK]conn_id:%u status:0x%x.\r\n",
            conn_id, status);
    }
}

/* ANCS服务通知属性响应结果回调 */
static void ble_ancs_client_notification_attr_rsp_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ancs_notification_attr_rsp_t *notify_attr_rsp, errcode_t status)
{
    bts_unused(client_id);
    if (status == ERRCODE_BT_SUCCESS) {
        bts_sample_info_log("[ANCSClientNotifyATTRSPCBK]conn_id:%u status:0x%x notify_uid:%lu\r\n",
            conn_id, status, notify_attr_rsp->notification_uid);
        uint16_t att_num = 1;
        struct osal_list_head *list_head = &(notify_attr_rsp->attr_rsp_lst);
        struct osal_list_head *node = list_head->next;
        while (node != NULL && node != list_head) {
            ble_ancs_attr_rsp_t *attr_node = (ble_ancs_attr_rsp_t *)node;
            bts_sample_log("[ANCSClientNotifyATTRSPCBK]att id:%u length:%lu att_value:%s\r\n",
                attr_node->att_id, attr_node->att_length, attr_node->att);
            if (att_num == notify_attr_rsp->att_num) {
                break;
            }
            att_num = att_num + 1;
            node = node->next;
        }
    } else {
        bts_sample_error_log("[ANCSClientNotifyATTRSPCBK]conn_id:%u status:0x%x.\r\n",
            conn_id, status);
    }
}

/* ANCS服务应用属性响应结果回调 */
static void ble_ancs_client_app_attr_rsp_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ancs_app_attr_rsp_t *app_attr_rsp, errcode_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[ANCSClientAPPATTRSPCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
    if (status == ERRCODE_BT_SUCCESS) {
        uint16_t att_num = 1;
        struct osal_list_head *list_head = &(app_attr_rsp->attr_rsp_lst);
        struct osal_list_head *node = list_head->next;
        while (node != NULL && node != list_head) {
            ble_ancs_attr_rsp_t *attr_node = (ble_ancs_attr_rsp_t *)node;
            if (attr_node->att_id == ANCS_APP_ATT_DISPLAY_NAME) {
                bts_sample_log("[ANCSClientAPPATTRSPCBK]app id:%s length:%lu app name:%s\r\n",
                    app_attr_rsp->app_id.id, attr_node->att_length, attr_node->att);
            }
            if (att_num == app_attr_rsp->att_num) {
                break;
            }
            att_num = att_num + 1;
            node = node->next;
        }
    } else {
        bts_sample_error_log("[ANCSClientAPPATTRSPCBK]conn_id:%u status:0x%x.\r\n",
            conn_id, status);
    }
}

/* 注册ancs服务回调 */
static void ble_ancs_client_at_cmd_register_cbk(uint8_t argc, char **argv)
{
    bts_unused(argc);
    bts_unused(argv);
    ble_ancs_client_callbacks_t cbk = {0};
    cbk.setup_cb = ble_ancs_client_setup_result_cbk;
    cbk.subscribe_cb = ble_ancs_client_subscribe_result_cbk;
    cbk.write_result_cb = ble_ancs_client_write_result_cbk;
    cbk.notify_source_cb = ble_ancs_client_notify_source_cbk;
    cbk.notify_attr_cb = ble_ancs_client_notification_attr_rsp_cbk;
    cbk.app_attr_cb = ble_ancs_client_app_attr_rsp_cbk;
    errcode_t ret = ble_ancs_client_register_callbacks(&cbk);
    bts_sample_info_log("ble ancs client register cbk ret:0x%x\r\n", ret);
    return;
}

/* ANCS服务建立 */
static void ble_ancs_client_at_cmd_setup(uint8_t argc, char **argv)
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    errcode_t ret = ble_ancs_client_setup_service(client_id, conn_id);
    bts_sample_info_log("ble ancs client setup ret:0x%x\r\n", ret);
    return;
}

/* 订阅通知 */
static void ble_ancs_client_at_cmd_subscribe_notify(uint8_t argc, char **argv)
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t sub_flag = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    sub_flag = (uint8_t)atoi(argv[BT_NUM2]);
    if (sub_flag > ANCS_CLIENT_NOTIFY_ENABLE) {
        bts_sample_error_log("ble ancs client argc invalid :%u\r\n", argc);
        return;
    }
    errcode_t ret = ble_ancs_client_subscribe_notification_source(client_id, conn_id, sub_flag);
    bts_sample_info_log("ble ancs client sub notify ret:0x%x\r\n", ret);
    return;
}

/* 订阅数据 */
static void ble_ancs_client_at_cmd_subscribe_data(uint8_t argc, char **argv)
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t sub_flag = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    sub_flag = (uint8_t)atoi(argv[BT_NUM2]);
    if (sub_flag > ANCS_CLIENT_NOTIFY_ENABLE) {
        bts_sample_error_log("ble ancs client argc invalid :%u\r\n", argc);
        return;
    }

    errcode_t ret = ble_ancs_client_subscribe_data_source(client_id, conn_id, sub_flag);
    bts_sample_info_log("ble ancs client sub data ret:0x%x\r\n", ret);
    return;
}

/* 执行消息行为（积极或消极行为） */
static void ble_ancs_client_at_cmd_perform_action(uint8_t argc, char **argv)
{
    if (argc != BT_NUM4) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint32_t notify_uid = 0;
    uint8_t action_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    notify_uid = (uint32_t)atoi(argv[BT_NUM2]);
    action_id = (uint8_t)atoi(argv[BT_NUM3]);
    if (action_id > ANCS_ACTION_RESERVED) {
        bts_sample_error_log("ble ancs client argc invalid :%u\r\n", argc);
        return;
    }
    ble_ancs_notification_action_req_t action_cmd = {0};
    action_cmd.notification_uid = notify_uid;
    action_cmd.action_id = action_id;
    errcode_t ret = ble_ancs_client_send_perform_notify_action_cmd(client_id, conn_id, &action_cmd);
    bts_sample_info_log("ble ancs client perform action ret:0x%x\r\n", ret);
    return;
}

/* 十六进制字符转数值 */
static uint8_t char_2_num(int8_t c)
{
    if (c >= '0' && c <= '9') {
        return (uint8_t)(c - '0');
    } else if (c >= 'A' && c <= 'F') {
        return (uint8_t)(c - 'A' + TEN);
    } else if (c >= 'a' && c <= 'f') {
        return (uint8_t)(c - 'a' + TEN);
    } else {
        return TD_U8_MAX;
    }
}

/* 十六进制字符转数值 */
static uint8_t str_2_byte(const char *str)
{
    return ((char_2_num(str[0]) << TD_U4_BITS) + char_2_num(str[1]));
}

/* 十六进制字符转属性请求参数结构体 */
static void decode_att_req_from_str(char *str,  ble_ancs_attr_req_t *nums)
{
    uint32_t len = (uint32_t)strlen(str);
    uint32_t cnt = 0;
    char *ptr_str = NULL;
    uint16_t length_big_byte = 0;
    uint16_t length_little_byte = 0;
    ptr_str = str;
    for (uint32_t i = 0; i < len - 1; i += ANCS_ATT_BYTE_LENGTH) { /* 6个字符为1组，2个字符为一个字节 */
        nums[cnt].att_id = str_2_byte(ptr_str);
        ptr_str += ANCS_DEC_CHAR_COUNT;
        length_big_byte = (((uint16_t)str_2_byte(ptr_str)) << BYTE_BIT_COUNT);
        ptr_str += ANCS_DEC_CHAR_COUNT;
        length_little_byte = ((uint16_t)str_2_byte(ptr_str));
        nums[cnt].att_max_length = (uint16_t)(length_big_byte | length_little_byte);
        ptr_str += ANCS_DEC_CHAR_COUNT;
        cnt++;
    }
}

/* 获取通知属性 */
static void ble_ancs_client_at_cmd_get_notify_att(uint8_t argc, char **argv)
{
    if (argc != BT_NUM5) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint32_t notify_uid = 0;
    uint8_t att_num = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    notify_uid = (uint32_t)atoi(argv[BT_NUM2]);
    att_num = (uint8_t)atoi(argv[BT_NUM3]);
    if (att_num == 0) {
        bts_sample_error_log("ble ancs client notify att req att num is zero!\r\n");
        return;
    }
    uint32_t len = (uint32_t)strlen(argv[BT_NUM4]);
    if ((att_num * ANCS_ATT_BYTE_LENGTH) != len) {
        bts_sample_error_log("ble ancs client notify att req att num is wrong!\r\n");
        return;
    }

    /* 获取来电通知属性 */
    ble_ancs_notification_attr_req_t *notify_attr_req = bts_sample_mem_new(
        sizeof(ble_ancs_notification_attr_req_t) + (att_num * sizeof(ble_ancs_attr_req_t)));
    notify_attr_req->att_num = att_num;
    notify_attr_req->notification_uid = notify_uid;
    decode_att_req_from_str(argv[BT_NUM4], notify_attr_req->attr);
    errcode_t ret = ble_ancs_client_send_get_notify_att_cmd(client_id, conn_id, notify_attr_req);
    bts_sample_mem_free(notify_attr_req);
    bts_sample_info_log("ble ancs client notify att get ret:0x%x\r\n", ret);
    return;
}

/* 解析应用属性参数 */
static errcode_t parse_app_att_req_param(ble_ancs_app_attr_req_t *app_attr_req,
    uint16_t app_id_len, uint16_t att_num, char **argv)
{
    app_attr_req->att_num = att_num;
    app_attr_req->app_id.len = app_id_len + 1;
    uint8_t *id = bts_sample_mem_new(app_attr_req->app_id.len);
    if (id == NULL) {
        bts_sample_error_log("[ANCSClientSample]app id memnew error!\r\n");
        app_attr_req->app_id.id = NULL;
        return ERRCODE_BT_MALLOC_FAIL;
    }
    if (memcpy_s(id, app_attr_req->app_id.len, (uint8_t *)argv[BT_NUM2], app_id_len) != EOK) {
        bts_sample_error_log("[ANCSClientSample]app id memcpy error!\r\n");
        bts_sample_mem_free(id);
        app_attr_req->app_id.id = NULL;
        return ERRCODE_BT_MEMCPY_FAIL;
    }
    *(id + app_id_len) = 0; /* This string is NULL-terminated */
    app_attr_req->app_id.id = id;

    /* 解析应用属性参数 */
    decode_att_req_from_str(argv[BT_NUM4], (ble_ancs_attr_req_t *)(app_attr_req->attr_id));
    return ERRCODE_BT_SUCCESS;
}

/* 获取应用属性 */
static void ble_ancs_client_at_cmd_get_app_att(uint8_t argc, char **argv)
{
    if (argc != BT_NUM5) {
        bts_sample_error_log("ble ancs client argc num invalid :%u\r\n", argc);
        return;
    }

    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t att_num = 0;
    uint16_t app_id_len = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    app_id_len = (uint16_t)strlen(argv[BT_NUM2]);
    att_num = (uint8_t)atoi(argv[BT_NUM3]);
    if (att_num == 0) {
        bts_sample_error_log("ble ancs client notify att req att num is zero!\r\n");
        return;
    }
    uint32_t len = strlen(argv[BT_NUM4]);
    if ((att_num * ANCS_ATT_BYTE_LENGTH) != len) {
        bts_sample_error_log("ble ancs client notify att req att num is wrong!\r\n");
        return;
    }

    /* 获取应用属性 */
    ble_ancs_app_attr_req_t *app_attr_req =
        bts_sample_mem_new(sizeof(ble_ancs_app_attr_req_t) + (att_num * sizeof(uint8_t)));
    errcode_t result = parse_app_att_req_param(app_attr_req, app_id_len, att_num, argv);
    if (result != ERRCODE_BT_SUCCESS) {
        if (app_attr_req->app_id.id != NULL) {
            bts_sample_mem_free(app_attr_req->app_id.id);
        }
        bts_sample_mem_free(app_attr_req);
        return;
    }
    errcode_t ret = ble_ancs_client_send_get_app_att_cmd(client_id, conn_id, app_attr_req);
    bts_sample_info_log("ble ancs client app att get ret:0x%x\r\n", ret);
    bts_sample_mem_free(app_attr_req->app_id.id);
    bts_sample_mem_free(app_attr_req);
    return;
}

/* ANCS客户端AT命令与接口对应关系 */
static ble_ancs_client_at_hdl_info_t g_ble_ancs_client_at_hdl[] = {
    { BLE_SAMPLE_ANCS_CLIENT_REGISTER_CBK, ble_ancs_client_at_cmd_register_cbk },
    { BLE_SAMPLE_ANCS_CLIENT_SETUP, ble_ancs_client_at_cmd_setup },
    { BLE_SAMPLE_ANCS_CLIENT_SUBSCRIBE_NOTITY, ble_ancs_client_at_cmd_subscribe_notify },
    { BLE_SAMPLE_ANCS_CLIENT_SUBSCRIBE_DATA, ble_ancs_client_at_cmd_subscribe_data },
    { BLE_SAMPLE_ANCS_CLIENT_NOTIFY_ATT_GET, ble_ancs_client_at_cmd_get_notify_att },
    { BLE_SAMPLE_ANCS_CLIENT_APP_ATT_GET, ble_ancs_client_at_cmd_get_app_att },
    { BLE_SAMPLE_ANCS_CLIENT_PERFORM_ACTION, ble_ancs_client_at_cmd_perform_action },
};

void ble_ancs_client_at_cmd(int32_t cmd_id, char **params, uint8_t params_cnt)
{
    size_t hdl_cnt = sizeof(g_ble_ancs_client_at_hdl) / sizeof(ble_ancs_client_at_hdl_info_t);
    for (uint8_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_ancs_client_at_hdl[i].id == cmd_id && g_ble_ancs_client_at_hdl[i].func != NULL) {
            g_ble_ancs_client_at_hdl[i].func(params_cnt, params);
            return;
        }
    }
}
