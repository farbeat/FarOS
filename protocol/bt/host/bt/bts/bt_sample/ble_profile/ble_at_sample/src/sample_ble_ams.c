/*
 * Copyright (c) CompanyNameMagicTag 2024. All rights reserved.
 * Description: This module include test function of AMS Client module.
 */
#include <stdint.h>
#include <stdlib.h>
#include <td_base.h>
#include "osal_addr.h"
#include "sample_common.h"
#include "ble_ams_client.h"
#include "sample_ble_ams.h"

#define HEX_BYTE_CHAR_COUNT 2

/* AMS客户端接口命令id */
typedef enum {
    BLE_SAMPLE_AMS_CLIENT_REGISTER_CBK = 1,                 /* 1 注册AMS客户端回调函数 */
    BLE_SAMPLE_AMS_CLIENT_SETUP,                            /* 2  AMS客户端连接服务端 */
    BLE_SAMPLE_AMS_CLIENT_SUBSCRIBE_REMOTE_COMMNAD,         /* 3  订阅远端命令 */
    BLE_SAMPLE_AMS_CLIENT_SUBSCRIBE_ENTITY_UPDATE,          /* 4  订阅实体更新 */
    BLE_SAMPLE_AMS_CLIENT_SEND_REMOTE_CMD,                  /* 5  发送远端命令 */
    BLE_SAMPLE_AMS_CLIENT_REGISTER_INTERESTED_ENTITY_ATT,   /* 6  注册感兴趣的实体属性 */
    BLE_SAMPLE_AMS_CLIENT_RETRIEVE_FULL_ENTITY_ATT,         /* 7  请求加载完整实体属性 */
    BLE_SAMPLE_AMS_CLIENT_READ_FULL_ENTITY_ATT              /* 8  读取完整实体属性 */
} ble_ams_client_at_cmd_id_t;

/* 命令处理函数接口 */
typedef void (*ble_ams_client_at_func_t)(uint8_t argc, char **argv);

/* AMS客户端AT命令id与接口对应关系 */
typedef struct {
    ble_ams_client_at_cmd_id_t id;
    ble_ams_client_at_func_t func;
} ble_ams_client_at_hdl_info_t;

/* 当前活跃媒体设备支持的远端命令 */
ble_ams_remote_cmd_rpt_t *g_remote_cmd_supported = NULL;

/* AMS服务建立结果回调 */
static void ble_ams_client_setup_result_cbk(uint8_t client_id, uint16_t conn_id, errcode_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[AMSClientSetupCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
}

/* AMS服务订阅结果回调 */
static void ble_ams_client_subscribe_result_cbk(uint8_t client_id, uint16_t conn_id,
    ams_client_subscribe_type_t type, gatt_status_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[AMSClientSubscribeResultCBK]conn_id:%u type:%d status:0x%x\r\n",
        conn_id, type, status);
}

/* AMS服务远端支持的命令上报 */
static void ble_ams_client_remote_cmd_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ams_remote_cmd_rpt_t *remote_cmd, errcode_t status)
{
    bts_unused(client_id);
    if (status == ERRCODE_BT_SUCCESS) {
        bts_sample_log("[AMSClientRemoteCMDCBK]conn_id:%u status:0x%x cmd_num=%u ",
            conn_id, status, remote_cmd->cmd_num);
        if (g_remote_cmd_supported != NULL) {
            bts_sample_mem_free(g_remote_cmd_supported);
        }
        uint16_t length = (uint16_t)sizeof(ble_ams_remote_cmd_rpt_t) + remote_cmd->cmd_num;
        g_remote_cmd_supported = bts_sample_mem_new(length);
        g_remote_cmd_supported->cmd_num = remote_cmd->cmd_num;
        if (memcpy_s(g_remote_cmd_supported->remote_cmd, remote_cmd->cmd_num,
            remote_cmd->remote_cmd, remote_cmd->cmd_num) != EOK) {
            bts_sample_error_log("[AMSClientSample] memcpy error!\r\n");
            bts_sample_mem_free(g_remote_cmd_supported);
            return;
        }
        bts_sample_log("supported_cmd=");
        for (int i = 0; i < remote_cmd->cmd_num; i++) {
            bts_sample_log("0x%02x", remote_cmd->remote_cmd[i]);
        }
        bts_sample_log("\r\n");
    } else {
        bts_sample_info_log("[AMSClientRemoteCMDCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
    }
}

/* AMS服务实体更新上报 */
static void ble_ams_client_entity_update_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ams_entity_update_rpt_t *entity_update, errcode_t status)
{
    bts_unused(client_id);
    if (status == ERRCODE_BT_SUCCESS) {
        bts_sample_info_log(
            "[AMSClientEntityUpdateCBK]conn_id:%u status:0x%x entityId=%u att_id=%u update_flags=0x%x value=%s\r\n",
            conn_id, status,
            entity_update->entity_id,
            entity_update->att_id,
            entity_update->entity_update_flags,
            entity_update->value);
    } else {
        bts_sample_info_log("[AMSClientEntityUpdateCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
    }
}

/* AMS客户端写请求响应结果上报 */
static void ble_ams_client_write_character_rsp_cbk(uint8_t client_id, uint16_t conn_id,
    ams_client_write_character_type_t type,
    gatt_status_t status)
{
    bts_unused(client_id);
    bts_sample_info_log("[AMSClientWriteRSPCBK]conn_id:%u type:%d status:0x%x\r\n", conn_id, type, status);
}

/* AMS客户端读请求响应结果上报 */
static void ble_ams_client_read_full_attr_result_cbk(uint8_t client_id, uint16_t conn_id,
    ble_ams_full_attr_value_t *read_result,
    gatt_status_t status)
{
    bts_unused(client_id);
    if (status == ERRCODE_BT_SUCCESS) {
        bts_sample_info_log("[AMSClientReadFullAttrCBK]conn_id:%u status:0x%x value_len=%d value=%s\r\n",
            conn_id, status,
            read_result->value_len,
            read_result->value);
    } else {
        bts_sample_info_log("[AMSClientReadFullAttrCBK]conn_id:%u status:0x%x\r\n", conn_id, status);
    }
}

/* 注册AMS服务回调 */
static void ble_ams_client_at_cmd_register_cbk(uint8_t argc, char **argv)
{
    bts_unused(argc);
    bts_unused(argv);
    ble_ams_client_callbacks_t cbk = {0};
    cbk.setup_cb = ble_ams_client_setup_result_cbk;
    cbk.subscribe_cb = ble_ams_client_subscribe_result_cbk;
    cbk.remote_cmd_cb = ble_ams_client_remote_cmd_cbk;
    cbk.entity_update_cb = ble_ams_client_entity_update_cbk;
    cbk.write_rsp_cb = ble_ams_client_write_character_rsp_cbk;
    cbk.read_full_attr_result_cb = ble_ams_client_read_full_attr_result_cbk;
    errcode_t ret = ble_ams_client_register_callbacks(&cbk);
    bts_sample_info_log("ble ams client register cbk ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端发起服务建立 */
static void ble_ams_client_at_cmd_setup(uint8_t argc, char **argv)
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    errcode_t ret = ble_ams_client_setup_service(client_id, conn_id);
    bts_sample_info_log("ble ams client setup ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端订阅远端命令特征通知 */
static void ble_ams_client_at_cmd_remote_cmd(uint8_t argc, char **argv)
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t sub_flag = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    sub_flag = (uint8_t)atoi(argv[BT_NUM2]);
    if (sub_flag > AMS_CLIENT_NOTIFY_ENABLE) {
        bts_sample_error_log("ble ams client argc invalid :%u\r\n", argc);
        return;
    }
    errcode_t ret = ble_ams_client_subscribe_remote_cmd(client_id, conn_id, sub_flag);
    bts_sample_info_log("ble ams client sub remote cmd ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端订阅实体更新特征通知 */
static void ble_ams_client_at_cmd_entity_update(uint8_t argc, char **argv)
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t sub_flag = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    sub_flag = (uint8_t)atoi(argv[BT_NUM2]);
    if (sub_flag > AMS_CLIENT_NOTIFY_ENABLE) {
        bts_sample_error_log("ble ams client argc invalid :%u\r\n", argc);
        return;
    }
    errcode_t ret = ble_ams_client_subscribe_entity_update(client_id, conn_id, sub_flag);
    bts_sample_info_log("ble ams client sub entity update ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端发送远端命令 */
static void ble_ams_client_at_cmd_send_remote_cmd(uint8_t argc, char **argv)
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t remote_cmd = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    remote_cmd = (uint8_t)atoi(argv[BT_NUM2]);
    if (remote_cmd > AMS_RMT_CMD_ID_BOOKMARK_TRACK) {
        bts_sample_error_log("ble ams client argc invalid :%u\r\n", argc);
        return;
    }
    errcode_t ret = ble_ams_client_send_remote_cmd(client_id, conn_id, remote_cmd);
    bts_sample_info_log("ble ams client send remote cmd ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端注册感兴趣的实体属性 */
static void ble_ams_client_at_cmd_register_interested_entity_att(uint8_t argc, char **argv)
{
    if (argc != BT_NUM5) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint16_t att_num = 0;
    uint8_t entity_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    entity_id = (uint8_t)atoi(argv[BT_NUM2]);
    att_num = (uint16_t)atoi(argv[BT_NUM3]);
    uint32_t len = (uint32_t)strlen(argv[BT_NUM4]);
    if (len != (att_num * HEX_BYTE_CHAR_COUNT)) {
        bts_sample_error_log("ble ams client argc att_num invalid :%u\r\n", att_num);
        return;
    }

    ble_ams_interested_entity_attr_req_t *attr_req =
        bts_sample_mem_new(sizeof(ble_ams_interested_entity_attr_req_t) + att_num);
    attr_req->entity_id = entity_id;
    attr_req->att_num = att_num;
    if (decode_hex_from_str(argv[BT_NUM4], attr_req->att_id) != EXT_ERR_SUCCESS) {
        bts_sample_mem_free(attr_req);
        bts_sample_error_log("ble ams client decode attr fail!\r\n");
        return;
    }
    errcode_t ret = ble_ams_client_register_interested_entity_attribute(client_id, conn_id, attr_req);
    bts_sample_info_log("ble ams client register interested attr ret:0x%x\r\n", ret);
    bts_sample_mem_free(attr_req);
    return;
}

/* AMS客户端请求加载完整的实体属性 */
static void ble_ams_client_at_cmd_retrieve_full_attr(uint8_t argc, char **argv)
{
    if (argc != BT_NUM4) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    uint8_t entity_id = 0;
    uint8_t attr_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);
    entity_id = (uint8_t)atoi(argv[BT_NUM2]);
    attr_id = (uint8_t)atoi(argv[BT_NUM3]);

    ble_ams_full_entity_attr_cmd_t *full_attr = bts_sample_mem_new(sizeof(ble_ams_full_entity_attr_cmd_t));
    full_attr->entity_id = entity_id;
    full_attr->att_id = attr_id;
    errcode_t ret = ble_ams_client_retrieve_full_entity_attribute(client_id, conn_id, full_attr);
    bts_sample_info_log("ble ams client retrieve full attr ret:0x%x\r\n", ret);
    bts_sample_mem_free(full_attr);
    return;
}

/* AMS客户端读取完整的实体属性 */
static void ble_ams_client_at_cmd_read_full_attr(uint8_t argc, char **argv)
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("ble ams client argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[BT_NUM0]);
    conn_id = (uint16_t)atoi(argv[BT_NUM1]);

    errcode_t ret = ble_ams_client_read_full_entity_attribute(client_id, conn_id);
    bts_sample_info_log("ble ams client read full attr ret:0x%x\r\n", ret);
    return;
}

/* AMS客户端AT命令与接口对应关系 */
static ble_ams_client_at_hdl_info_t g_ble_ams_client_at_hdl[] = {
    { BLE_SAMPLE_AMS_CLIENT_REGISTER_CBK, ble_ams_client_at_cmd_register_cbk },
    { BLE_SAMPLE_AMS_CLIENT_SETUP, ble_ams_client_at_cmd_setup },
    { BLE_SAMPLE_AMS_CLIENT_SUBSCRIBE_REMOTE_COMMNAD, ble_ams_client_at_cmd_remote_cmd },
    { BLE_SAMPLE_AMS_CLIENT_SUBSCRIBE_ENTITY_UPDATE, ble_ams_client_at_cmd_entity_update },
    { BLE_SAMPLE_AMS_CLIENT_SEND_REMOTE_CMD, ble_ams_client_at_cmd_send_remote_cmd },
    { BLE_SAMPLE_AMS_CLIENT_REGISTER_INTERESTED_ENTITY_ATT, ble_ams_client_at_cmd_register_interested_entity_att },
    { BLE_SAMPLE_AMS_CLIENT_RETRIEVE_FULL_ENTITY_ATT, ble_ams_client_at_cmd_retrieve_full_attr },
    { BLE_SAMPLE_AMS_CLIENT_READ_FULL_ENTITY_ATT, ble_ams_client_at_cmd_read_full_attr },
};

/* AMS客户端AT命令处理入口函数 */
void ble_ams_client_at_cmd(int32_t cmd_id, char **params, uint8_t params_cnt)
{
    size_t hdl_cnt = sizeof(g_ble_ams_client_at_hdl) / sizeof(ble_ams_client_at_hdl_info_t);
    for (uint8_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_ams_client_at_hdl[i].id == cmd_id && g_ble_ams_client_at_hdl[i].func != NULL) {
            g_ble_ams_client_at_hdl[i].func(params_cnt, params);
            return;
        }
    }
}