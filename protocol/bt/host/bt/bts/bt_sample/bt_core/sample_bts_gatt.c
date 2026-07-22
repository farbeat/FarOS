/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: This module include test function of BT Adapter Service module.
 */

#include "sample_bts_gatt.h"
#include <osal_list.h>
#include <stdlib.h>
#include <td_base.h>
#include "securec.h"
#include "soc_osal.h"
#include "bts_br_gap.h"
#include "bts_le_gap.h"
#include "bts_gatt_server.h"
#include "bts_gatt_client.h"
#include "sample_common.h"
#include "ble_gatts_callbacks.h"
#include "ble_pro_dis_client.h"
#include "ble_bas_client.h"
#include "ble_fmp_locator.h"
#include "ble_ancs_client.h"
#include "ble_ams_client.h"
#include "osal_task.h"

#define BTSRV_LOG_DEBUG
#define GATTS_SAMPLE_MAX_ATTR_NUM 1000
#define GATT_UUID_LEN_2 2
#define BLE_SPEED_TEST_MAX_DATA_LENGTH 514
#define BLE_SPEED_TEST_MIN_DATA_LENGTH 5
#define BLE_SPEED_TEST_CLIENT_ID 1
#define BLE_SPEED_TEST_CONNECT_ID 64
#define BLE_SPEED_TEST_SERVER_ID 1
#define BLE_SPEED_TEST_PKT_NUM_MAX 20001
#define BLE_SERVER_CBK_FILL_MODE_ALL 1
#define BLE_SERVER_CBK_FILL_MODE_NO_WRITE 2
#define BLE_CLIENT_CBK_FILL_MODE_ALL 1
#define BLE_CLIENT_CBK_FILL_MODE_PROFILE 2
#define BLE_CLIENT_CBK_FILL_MODE_NO_NOTIFY 3
#define BLE_CLIENT_CBK_FILL_MODE_AUTO 4
#define BLE_TEST_SERVICE_UUID_AUTO_OFFSET 6
#define BLE_TEST_SERVICE_START_WAIT_MS 10
#define BLE_TEST_SERVICE_CCC_LEN 2

/* 用于标记正在添加的attr */
static struct osal_list_head g_btsrv_sample_attr_list = { 0 };
static gap_ble_adv_params_t g_btsrv_sample_adv_param;
static uint32_t g_btsrv_sample_attr_num = 0;
static uint16_t g_test_service_handle = 0;
static bool g_auto_client_discovery = false;

/* gatt test cmd id */
typedef enum {
    BTSRV_SAMPLE_GATTS_REG_ID = 0,        /* 0 注册GATT server */
    BTSRV_SAMPLE_GATTS_UNREG_ID,          /* 1 取消注册GATT server */
    BTSRV_SAMPLE_GATTS_DISCONN,           /* 2 断开与远端client连接 */
    BTSRV_SAMPLE_GATTS_ADD_SRV,           /* 3 添加service */
    BTSRV_SAMPLE_GATTS_ADD_INCL_SRV,      /* 4 添加included service */

    BTSRV_SAMPLE_GATTS_ADD_CHAR,          /* 5 添加characteristic */
    BTSRV_SAMPLE_GATTS_ADD_DESC,          /* 6 添加descriptor */
    BTSRV_SAMPLE_GATTS_START_SRV,         /* 7 启动service */
    BTSRV_SAMPLE_GATTS_STOP_SRV,          /* 8 停止service */
    BTSRV_SAMPLE_GATTS_DEL_SRV,           /* 9 删除service */

    BTSRV_SAMPLE_GATTS_CLR_ALL_SRV,       /* 10 清除所有services */
    BTSRV_SAMPLE_GATTS_SEND_RSP,          /* 11 收到远端client的读写请求时发送响应 */
    BTSRV_SAMPLE_GATTS_SEND_IND,          /* 12 给远端client发送indication/notification */
    BTSRV_SAMPLE_GATTS_SET_ENC,           /* 13 设置安全连接加密类型 */
    BTSRV_SAMPLE_GATTS_REG_CBK,           /* 14 注册Gatt server回调函数 */

    BTSRV_SAMPLE_GATTS_ADD_SVC_SYNC,      /* 15 初始化service，用于系统应用的service启动 */
    BTSRV_SAMPLE_GATTS_ADD_CHAR_SYNC,     /* 16 去初始化(BTSRV_SAMPLE_GATTS_START_SRV_EX_INIT的)service */
    BTSRV_SAMPLE_GATTS_ADD_DESC_SYNC,     /* 17 打印目前已初始化的用于扩展接口的service */
    BTSRV_SAMPLE_GATTS_NOTIFY_BY_UUID,    /* 18 启动service，仅提供给系统应用使用，不对三方应用开放 */
    BTSRV_SAMPLE_GATTS_SET_MTU,           /* 19 停止service，仅提供给系统应用使用，不对三方应用开放 */

    BTSRV_SAMPLE_GATTC_REG_ID = 20,       /* 20 注册GATT client */
    BTSRV_SAMPLE_GATTC_UNREG_ID,          /* 21 取消注册GATT client */
    BTSRV_SAMPLE_GATTC_DISC_SVC,          /* 22 发现 Service */
    BTSRV_SAMPLE_GATTC_DISC_CHAR,         /* 23 发现 Characterisitic */
    BTSRV_SAMPLE_GATTC_DISC_DESC,         /* 24 发现 Descriptor */

    BTSRV_SAMPLE_GATTC_READ_BY_HANDLE,    /* 25 通过 handle 读 */
    BTSRV_SAMPLE_GATTC_READ_BY_UUID,      /* 26 通过 uuid 读 */
    BTSRV_SAMPLE_GATTC_WRITE_REQ,         /* 27 从server读characteristics value */
    BTSRV_SAMPLE_GATTC_WRITE_CMD,         /* 28 向server写characteristics value */
    BTSRV_SAMPLE_GATTC_CONFIG_MTU,        /* 29 设置MTU size */

    BTSRV_SAMPLE_GATTC_REG_CBK = 30,           /* 30 注册GATT client 回调 */
    BTSRV_SAMPLE_GATT_ENABLE_BLE_STACK,        /* 31 打开BLE协议栈 */
    BTSRV_SAMPLE_GATT_DISABLE_BLE_STACK,       /* 32 关闭BLE协议栈 */
    BTSRV_SAMPLE_GATT_BLE_GET_LOCAL_ADDR,      /* 33 获取本地设备地址 */
    BTSRV_SAMPLE_GATT_BLE_SET_LOCAL_ADDR,      /* 34 设置本地设备地址 */

    BTSRV_SAMPLE_GATT_BLE_SET_APPEARANCE,      /* 35 设置BLE外观 */
    BTSRV_SAMPLE_GATT_BLE_GET_DEV_NAME,        /* 36 获取本地设备名称 */
    BTSRV_SAMPLE_GATT_BLE_SET_DEV_NAME,        /* 37 设置本地设备名称 */
    BTSRV_SAMPLE_GATT_BLE_PAIR_RMT_DEV,        /* 38 配对对端设备 */
    BTSRV_SAMPLE_GATT_BLE_GET_PAIRED_DEV_NUM,  /* 39 获取配对设备数量 */

    BTSRV_SAMPLE_GATT_BLE_SET_ADV_DATA = 40,   /* 40 设置BLE广播数据 */
    BTSRV_SAMPLE_GATT_BLE_SET_ADV_PARAM,       /* 41 设置广播参数 */
    BTSRV_SAMPLE_GATT_BLE_START_ADV,           /* 41 开始广播 */
    BTSRV_SAMPLE_GATT_BLE_STOP_ADV,            /* 42 停止广播 */
    BTSRV_SAMPLE_GATT_BLE_SET_SCAN_PARA,       /* 44 设置扫描参数 */

    BTSRV_SAMPLE_GATT_BLE_START_SCAN,          /* 45 开始扫描 */
    BTSRV_SAMPLE_GATT_BLE_STOP_SCAN,           /* 46 停止扫描 */
    BTSRV_SAMPLE_GATT_BLE_GATT_REG_CBK,        /* 47 注册GATT回调函数 */
    BTSRV_SAMPLE_GATT_BLE_GET_PAIR_DEV,        /* 48 获取配对设备 */
    BTSRV_SAMPLE_GATT_BLE_GET_PAIR_STATUS,     /* 49 获取配对状态 */

    BTSRV_SAMPLE_GATT_BLE_RMV_PAIR = 50,       /* 50 删除配对信息 */
    BTSRV_SAMPLE_GATT_BLE_READ_RSSI,           /* 51 读取RSSI信息 */
    BTSRV_SAMPLE_GATT_BLE_SET_PHY,             /* 52 设置PHTY参数 */
    BTSRV_SAMPLE_GATT_BLE_SET_DATA_LEN,        /* 53 设置数据长度 */
    BTSRV_SAMPLE_GATT_BLE_GET_CONN_PARAM,      /* 54 获取连接参数信息 */

    /* 55 */
    BTSRV_SAMPLE_GATT_READ_BT_MAC = 55,             /* 55 获取本地设备的MAC地址 */
    BTSRV_SAMPLE_GATTS_SHOW_LIST,
    BTSRV_SAMPLE_GATTC_CONN_LIST,
    BTSRV_SAMPLE_GATT_BLE_SET_SEC_PARAM,                /* 58 设置安全参数信息 */
    BTSRV_SAMPLE_GATTC_SPEED = 59,
    BTSRV_SAMPLE_GATTS_SPEED = 60,

    /* 61 */
    BTSRV_SAMPLE_GATT_BLE_SET_CTKD_ENABLE = 61,            /* 61 设置一键双连开关 */
    BTSRV_SAMPLE_GATTS_SERVICE,                   /* 62 验证GATT服务添加上限 */
} btsrv_gatt_sample_cmd_id;

static int32_t g_btsrv_sample_server_id = 0;

__attribute__((unused)) static void sample_gatts_reg_server_cbk(int status, uint32_t server_id, bt_uuid_t *app_uuid)
{
    bts_unused(app_uuid);
    g_btsrv_sample_server_id = (int32_t)server_id;
    bts_sample_log("[GATTServer]RegisterServerCallback server: %u status: %d\n", server_id, status);
}

__attribute__((unused)) static void sample_gatts_conn_server_cbk(uint32_t conn_id, uint32_t server_id,
    const bd_addr_t *addr)
{
    if (addr == NULL) {
        bts_sample_log("addr is null \n");
        return;
    }
    bts_sample_log("[GATTServer]ConnectServerCallback server: %u conn_id: %u ", server_id, conn_id);
    bts_sample_log("addr: ****%02x%02x%02x%02x\n",
        addr->addr[3], addr->addr[2], /* addr下标 2 3 */
        addr->addr[1], addr->addr[0]); /* addr下标 0 1 */
}

__attribute__((unused)) static void sample_gatts_disconn_server_cbk(uint32_t conn_id, uint32_t server_id,
    const bd_addr_t *addr)
{
    if (addr == NULL) {
        bts_sample_log("addr is null \n");
        return;
    }
    bts_sample_log("[GATTServer]DisconnectServerCallback server: %u conn_id: %u ", server_id, conn_id);
    bts_sample_log("addr: ****%02x%02x%02x%02x\n",
        addr->addr[3], addr->addr[2], /* addr下标 2 3 */
        addr->addr[1], addr->addr[0]); /* addr下标 0 1 */
}

static void sample_gatts_service_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t handle, errcode_t status)
{
    if (uuid == NULL) {
        bts_sample_log("uuid is null \n");
        return;
    }
    bts_sample_log("[GATTServer]ServiceAddCallback server: %u status: %u srv_handle: %u uuid: ",
        server_id, status, handle);
    for (int32_t i = uuid->uuid_len - 1; i >= 0 ; i--) {
        bts_sample_log("%02x", (uint8_t)uuid->uuid[i]);
    }
    bts_sample_log("\n");
}

static void sample_gatts_char_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t service_handle,
    gatts_add_character_result_t *result, errcode_t status)
{
    if (uuid == NULL) {
        bts_sample_log("uuid is null \n");
        return;
    }
    bts_sample_log("[GATTServer]CharacteristicAddCallback server: %u status: %u srv_hdl: %u char_hdl: %x"
        " value_handle:%x uuid: ", server_id, status, service_handle, result->handle, result->value_handle);
    for (int32_t i = uuid->uuid_len - 1; i >= 0 ; i--) {
        bts_sample_log("%02x", (uint8_t)uuid->uuid[i]);
    }
    bts_sample_log("\n");
}

static void sample_gatts_desc_add_cbk(uint8_t server_id, bt_uuid_t *uuid, uint16_t service_handle,
    uint16_t handle, errcode_t status)
{
    if (uuid == NULL) {
        bts_sample_log("uuid is null \n");
        return;
    }
    bts_sample_log("[GATTServer]DescriptorAddCallback server: %u status: %u srv_hdl: %u desc_hdl: %x uuid: ",
        server_id, status, service_handle, handle);
    for (int32_t i = uuid->uuid_len - 1; i >= 0 ; i--) {
        bts_sample_log("%02x", (uint8_t)uuid->uuid[i]);
    }
    bts_sample_log("\n");
}

static void sample_gatts_service_start_cbk(uint8_t server_id, uint16_t handle, errcode_t status)
{
    bts_sample_log("[GATTServer]ServiceStartCallback server: %u status: %u srv_hdl: %u\n", server_id, status, handle);
    g_test_service_handle = handle;
}

static void sample_gatts_service_stop_cbk(uint8_t server_id, uint16_t handle, errcode_t status)
{
    bts_sample_log("[GATTServer]ServiceStopCallback server: %u status: %u srv_hdl: %u\n", server_id, status, handle);
}

static void sample_gatts_service_delete_cbk(uint8_t server_id, errcode_t status)
{
    bts_sample_log("[GATTServer]ServiceDeleteCallback server: %u status: %u \n", server_id, status);
}

static void sample_gatts_req_read_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_read_cb_t *cb_para,
    errcode_t status)
{
    bts_unused(status);
    if (cb_para == NULL) {
        bts_sample_log("cb_para is null \n");
        return;
    }
    bts_sample_log("server_id:%u, conn id: %u, request_id: %u, handle:%u, need_authorize:%d need_rsp:%d\n",
        server_id, conn_id, cb_para->request_id, cb_para->handle, cb_para->need_authorize, cb_para->need_rsp);
    bts_sample_log("\n");
}

static void sample_gatts_req_write_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_write_cb_t *cb_para,
    errcode_t status)
{
    bts_unused(status);
    if (cb_para == NULL) {
        bts_sample_log("cb_para is null \n");
        return;
    }

    bts_sample_log("server_id:%u, conn id: %u, request_id: %u, handle:%u, need_authorize:%d, need_rsp:%d\n",
        server_id, conn_id, cb_para->request_id, cb_para->handle, cb_para->need_authorize, cb_para->need_rsp);
    bts_sample_log("\n");
}

__attribute__((unused)) static void sample_gatts_rsp_confirm_cbk(int status, uint32_t handle)
{
    bts_sample_log("[GATTServer]ResponseConfirmationCallback: status: %d handle: %u\n", status, handle);
}

__attribute__((unused)) static void sample_gatts_indication_sent_cbk(uint32_t conn_id, int status)
{
    bts_sample_log("[GATTServer]IndicationSentCallback: status: %d conn_id: %u\n", status, conn_id);
}

static void sample_gatts_mtu_change_cbk(uint8_t server_id, uint16_t conn_id, uint16_t mtu_size, errcode_t status)
{
    bts_unused(server_id);
    bts_unused(status);
    bts_sample_log("[GATTServer]MtuChangeCallback: mtu: %u conn_id: %u\n", mtu_size, conn_id);
}

__attribute__((unused)) static uint32_t sample_gatts_req_read_ex_cbk(const unsigned char *buff, const unsigned int *len)
{
    bts_sample_log("[GATTServer]ble_gatts_service_tRead len: %u buff: ", (*len));
    for (uint32_t i = 0; i < (*len); i++) {
        bts_sample_log("%02x ", buff[i]);
    }
    bts_sample_log("\n");
    return ERRCODE_BT_SUCCESS;
}

__attribute__((unused)) static uint32_t sample_gatts_req_write_ex_cbk(const unsigned char *buff, unsigned int len)
{
    if (buff == NULL) {
        bts_sample_log("buff is null \n");
        return ERRCODE_BT_FAIL;
    }
    bts_sample_log("[GATTServer]ble_gatts_service_tWrite len: %u buff: ", len);
    for (uint32_t i = 0; i < len; i++) {
        bts_sample_log("%02x ", buff[i]);
    }
    bts_sample_log("\n");
    return ERRCODE_BT_SUCCESS;
}

__attribute__((unused)) static uint32_t sample_gatts_req_indicate_ex_cbk(const unsigned char *buff, unsigned int len)
{
    if (buff == NULL) {
        bts_sample_log("buff is null \n");
        return ERRCODE_BT_FAIL;
    }
    bts_sample_log("[GATTServer]ble_gatts_service_tIndicate len: %u buff: ", len);
    for (uint32_t i = 0; i < len; i++) {
        bts_sample_log("%02x ", buff[i]);
    }
    bts_sample_log("\n");
    return ERRCODE_BT_SUCCESS;
}

/* 注册GATT server回调 params: sample_mode fill_callbacks */
static void sample_gatts_cmd_reg_cbk(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint32_t fill_callbacks = (uint32_t)atoi(params[BT_NUM1]);

    errcode_t ret;
    gatts_callbacks_t cb = {
        sample_gatts_service_add_cbk,
        sample_gatts_char_add_cbk,
        sample_gatts_desc_add_cbk,
        sample_gatts_service_start_cbk,
        sample_gatts_service_stop_cbk,
        sample_gatts_service_delete_cbk,
        sample_gatts_req_read_cbk,
        sample_gatts_req_write_cbk,
        sample_gatts_mtu_change_cbk,
    };
    if ((sample_mode & 0x1) != 0) {
        ret = gatts_register_callbacks(NULL);
    } else if (fill_callbacks == BLE_SERVER_CBK_FILL_MODE_ALL) {
        ret = gatts_register_callbacks(&cb);
        bts_sample_log("ble_gatts_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else if (fill_callbacks == BLE_SERVER_CBK_FILL_MODE_NO_WRITE) {
        cb.write_request_cb = NULL;
        ret = gatts_register_callbacks(&cb);
        bts_sample_log("ble_gatts_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else {
        gatts_callbacks_t cb1 = { 0, };
        ret = gatts_register_callbacks(&cb1);
        bts_sample_log("ble_gatts_register_callbacks test ret(errcode_t): %x\n", ret);
    }
}

static void sample_gattc_discovery_svc_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_service_result_t *service, errcode_t status)
{
    bts_sample_log("[GATTClient]Discovery service callback: status: %u client_id: %u conn_id: %u\n",
        status, client_id, conn_id);
    if (service != NULL) {
        bts_sample_log("service: start_hdl: 0x%04x end_hdl: 0x%04x uuid_len: %u uuid:",
            service->start_hdl, service->end_hdl, service->uuid.uuid_len);
        for (uint16_t i = service->uuid.uuid_len; i > 0; i--) {
            bts_sample_log("%02x", service->uuid.uuid[i - 1]);
        }
        bts_sample_log("\n");
        if (g_auto_client_discovery && status == ERRCODE_BT_SUCCESS) {
            gattc_discovery_character_param_t param = {0};
            param.service_handle = service->start_hdl;
            gattc_discovery_character(client_id, conn_id, &param);
        }
    }
}

static void sample_gattc_discovery_svc_response(uint8_t client_id, uint16_t conn_id,
    bt_uuid_t *uuid, errcode_t status)
{
    if (uuid == NULL) {
        bts_sample_log("uuid is null \n");
        return;
    }

    bts_sample_log("[GATTClient]Discovery service rsp: status: %u client_id: %u conn_id: %u uuid_len:%u uuid:\r\n",
        status, client_id, conn_id, uuid->uuid_len);
    for (uint16_t i = uuid->uuid_len; i > 0 ; i--) {
        bts_sample_log("%02x", uuid->uuid[i - 1]);
    }
}

static void sample_gattc_discovery_char_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_result_t *character, errcode_t status)
{
    bts_sample_log("[GATTClient]Discovery characteristic callback: status: %u client_id: %u conn_id: %u\n",
        status, client_id, conn_id);
    if (character != NULL) {
        bts_sample_log("character: declare_handle: 0x%04x value_handle: 0x%04x properties: 0x%02x uuid_len: %u uuid:",
            character->declare_handle, character->value_handle, character->properties, character->uuid.uuid_len);
        for (uint16_t i = character->uuid.uuid_len; i > 0; i--) {
            bts_sample_log("%02x", character->uuid.uuid[i - 1]);
        }
        bts_sample_log("\n");
        if (g_auto_client_discovery && status == ERRCODE_BT_SUCCESS) {
            gattc_discovery_descriptor(client_id, conn_id, character->declare_handle);
        }
    }
}

static void sample_gattc_discovery_char_response(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_character_param_t *param, errcode_t status)
{
    bts_sample_log("[GATTClient]Discovery characteristic rsp: status: %u client_id: %u conn_id: %u\r\n",
        status, client_id, conn_id);
    if (param != NULL) {
        bts_sample_log("service_handle: 0x%04x uuid_len: %u uuid:", param->service_handle, param->uuid.uuid_len);
        for (uint16_t i = param->uuid.uuid_len; i > 0; i--) {
            bts_sample_log("%02x", param->uuid.uuid[i - 1]);
        }
        bts_sample_log("\n");
    }
}

static void sample_gattc_discovery_desc_cbk(uint8_t client_id, uint16_t conn_id,
    gattc_discovery_descriptor_result_t *descriptor, errcode_t status)
{
    bts_sample_log("[GATTClient]Discovery descriptor callback: status: %u client_id: %u conn_id: %u\n",
        status, client_id, conn_id);
    if (descriptor != NULL) {
        bts_sample_log("descriptor_hdl: 0x%04x uuid_len: %u uuid:",
            descriptor->descriptor_hdl, descriptor->uuid.uuid_len);
        for (uint16_t i = descriptor->uuid.uuid_len; i > 0; i--) {
            bts_sample_log("%02x", descriptor->uuid.uuid[i - 1]);
        }
        bts_sample_log("\n");
    }
}

static void sample_gattc_discovery_desc_response(uint8_t client_id, uint16_t conn_id,
    uint16_t character_handle, errcode_t status)
{
    bts_sample_log("[GATTClient]Discovery descriptor rsp: status: %u client_id: %u conn_id: %ud char_hdl:0x%04x\r\n",
        status, client_id, conn_id, character_handle);
}

static void sample_gattc_read_cfm_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *read_result,
    gatt_status_t status)
{
    bts_sample_log("[GATTClient]Read Cfm cbk: status: %u client_id: %u conn_id: %u",
        status, client_id, conn_id);
    if (read_result == NULL) {
        bts_sample_log("read_result is null \n");
        return;
    }

    bts_sample_log(" handle:0x%04x data_len:%u data: ", read_result->handle, read_result->data_len);
    for (uint16_t i = 0; i < read_result->data_len; i++) {
        bts_sample_log("%02x ", read_result->data[i]);
    }
    bts_sample_log("\n");
}

static void sample_gattc_read_by_uuid_response(uint8_t client_id, uint16_t conn_id,
    gattc_read_req_by_uuid_param_t *param, errcode_t status)
{
    bts_sample_log("[GATTClient]Read by uuid rsp: status: %u client_id: %u conn_id: %u",
        status, client_id, conn_id);
    if (param == NULL) {
        bts_sample_log("param is null \n");
        return;
    }

    bts_sample_log(" start_hdl:0x%04x end_hdl:%u uuid_len:%u uuid: ", param->start_hdl, param->end_hdl,
        param->uuid.uuid_len);
    for (uint16_t i = param->uuid.uuid_len; i > 0 ; i--) {
        bts_sample_log("%02x", param->uuid.uuid[i - 1]);
    }
    bts_sample_log("\n");
}

static void sample_gattc_write_cfm_cbk(uint8_t client_id, uint16_t conn_id, uint16_t handle, gatt_status_t status)
{
    bts_sample_log("[GATTClient]Write cfm cbk: status: %u client_id: %u conn_id: %u handle:0x%04x.\r\n",
        status, client_id, conn_id, handle);
}

static void sample_gattc_mtu_changed_cbk(uint8_t client_id, uint16_t conn_id, uint16_t mtu_size, errcode_t status)
{
    bts_sample_log("[GATTClient]MTU changed cbk: status: %u client_id: %u conn_id: %u mtu:%u.\r\n",
        status, client_id, conn_id, mtu_size);
}

static void sample_gattc_notification_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data,
    errcode_t status)
{
    bts_sample_log("[GATTClient]Notification cbk: status: %u client_id: %u conn_id: %u",
        status, client_id, conn_id);
    if (data == NULL) {
        bts_sample_log("data is null \n");
        return;
    }

    bts_sample_log(" handle:0x%04x data_len:%u data: ", data->handle, data->data_len);
    bts_sample_log("%02x ", data->data[0]);
    bts_sample_log("\n");
}

static void sample_gattc_indication_cbk(uint8_t client_id, uint16_t conn_id, gattc_handle_value_t *data,
    errcode_t status)
{
    bts_sample_log("[GATTClient]Indication cbk: status: %u client_id: %u conn_id: %u",
        status, client_id, conn_id);
    if (data == NULL) {
        bts_sample_log("data is null \n");
        return;
    }

    bts_sample_log(" handle:0x%04x data_len:%u data: ", data->handle, data->data_len);
    for (uint16_t i = 0; i < data->data_len; i++) {
        bts_sample_log("%02x ", data->data[i]);
    }
    bts_sample_log("\n");
}

/* 注册gatt 服务器端 params: sample_mode app_uuid */
static void sample_gatts_cmd_reg_id(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("gatts_register params_cnt error! need: sample_mode app_uuid\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    bt_uuid_t uuid;
    if ((sample_mode & 0x1) != 0) {
        uuid.uuid_len = 0;
    } else {
        uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM1])) / 2; /* 2个字符为1字节 */
        if (decode_uuid_from_str(params[BT_NUM1], uuid.uuid) != EXT_ERR_SUCCESS) {
            bts_sample_log("decode app_uuid error!\n");
            return;
        }
    }
    uint8_t server_id = 0;
    errcode_t ret = gatts_register_server(&uuid, &server_id);
    bts_sample_log("[GATTServer]gatts_register ret(errcode_t): %u server_id:%u\n", ret, server_id);
}


/* 取消注册gatt 服务器端 params: server_id */
static void sample_gatts_cmd_unreg_id(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("params_cnt error! need: server_id\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    errcode_t ret = gatts_unregister_server(server_id);

    bts_sample_log("[GATTServer]BleGattsUnregister ret(errcode_t): %x\n", ret);
}


/* 断开与client的连接 params: sample_mode bd_addr server_id conn_id  */
__attribute__((unused)) static void sample_gatts_cmd_disconn(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM4) {
        bts_sample_log("params_cnt error! need: sample_mode bd_addr server_id conn_id\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[BT_NUM1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("[GATTServer]sample_gatts_cmd_disconn decode addr error!\n");
        return;
    }

    int server_id = atoi(params[BT_NUM2]);
    int conn_id = atoi(params[BT_NUM3]);
}

/* 添加service params: sample_mode server_id is_primary attr_num uuid  */
static void sample_gatts_cmd_add_srv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM4) {
        bts_sample_log("params_cnt error! need: sample_mode server_id is_primary uuid\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM1]);
    bool is_primary = atoi(params[BT_NUM2]);
    bt_uuid_t srv_uuid;
    errcode_t ret;

    srv_uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM3])) / 2; /* 2个字符为1字节; */
    if (decode_uuid_from_str(params[BT_NUM3], srv_uuid.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode service_uuid error!\n");
        return;
    }

    if (sample_mode == 0) {
        ret = gatts_add_service(server_id, &srv_uuid, is_primary);
        bts_sample_log("gatts add service ret(errcode_t): %x\n", ret);
    } else {
        uint16_t handle;
        ret = gatts_add_service_sync(server_id, &srv_uuid, is_primary, &handle);
        bts_sample_log("gatts add service sync ret(errcode_t): %x handle:0x%04x.\r\n", ret, handle);
    }
}

/* 添加characteristic params: sample_mode server_id srv_handle properties permissions char_uuid */
static void sample_gatts_cmd_add_char(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM8) {
        bts_sample_log("params_cnt error! need: sample_mode server_id char_uuid properties permissions\n");
        return;
    }
    gatts_add_chara_info_t character = {0};
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM1]);
    uint16_t srv_handle = (uint16_t)atoi(params[BT_NUM2]);
    character.properties = (uint8_t)atoi(params[BT_NUM3]);
    character.permissions = (uint8_t)atoi(params[BT_NUM4]);
    character.chara_uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM5])) / 2; /* 2个字符为1字节; */
    if (decode_uuid_from_str(params[BT_NUM5], character.chara_uuid.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("gatts cmd add char decode char_uuid error!\n");
        return;
    }
    character.value_len = (uint16_t)atoi(params[BT_NUM6]);
    character.value = (uint8_t *)bts_sample_mem_new(sizeof(uint8_t) * character.value_len);
    if (character.value == NULL) {
        bts_sample_log("gatts cmd add char malloc value error!\n");
        return;
    }
    if (decode_hex_from_str(params[BT_NUM7], character.value) != EXT_ERR_SUCCESS) {
        bts_sample_mem_free(character.value);
        bts_sample_log("failed to decode input value.\n");
        return;
    }

    errcode_t ret;
    if (sample_mode == 0) {
        ret = gatts_add_characteristic(server_id, srv_handle, &character);
        bts_sample_log("gatts add characteristic ret(errcode_t): %x\n", ret);
    } else {
        gatts_add_character_result_t result = {0};
        ret = gatts_add_characteristic_sync(server_id, srv_handle, &character, &result);
        bts_sample_log("gatts add characteristic sync ret(errcode_t): %x handle:0x%04x val_hdl:0x%04x\n",
            ret, result.handle, result.value_handle);
    }
    bts_sample_mem_free(character.value);
}

/* 添加descriptor params: sample_mode server_id srv_handle permissions desc_uuid */
static void sample_gatts_cmd_add_desc(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM6) {
        bts_sample_log("params_cnt error! need: sample_mode server_id srv_handle permissions desc_uuid\n");
        return;
    }
    errcode_t ret;
    uint32_t sample_mode = (uint32_t)atoi(params[0]);
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM1]);
    uint16_t srv_handle = (uint16_t)atoi(params[BT_NUM2]);

    gatts_add_desc_info_t descriptor = {0};
    descriptor.permissions = (uint8_t)atoi(params[BT_NUM3]);
    descriptor.desc_uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM4])) / 2; /* 2个字符为1字节; */
    if (decode_uuid_from_str(params[BT_NUM4], descriptor.desc_uuid.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample gatts cmd add char decode desc_uuid error!\n");
        return;
    }

    descriptor.value_len = (uint16_t)(strlen(params[BT_NUM5])) / 2; /* 2个字符为1字节; */
    descriptor.value = (uint8_t *)bts_sample_mem_new(sizeof(uint8_t) * descriptor.value_len);
    if (descriptor.value == NULL) {
        bts_sample_log("sample gatts cmd add char malloc value error!\n");
        return;
    }
    if (decode_hex_from_str(params[BT_NUM5], descriptor.value) != EXT_ERR_SUCCESS) {
        bts_sample_mem_free(descriptor.value);
        bts_sample_log("failed to decode input value.\n");
        return;
    }

    if (sample_mode == 0) {
        ret = gatts_add_descriptor(server_id, srv_handle, &descriptor);
        bts_sample_log("gatts add descriptor ret(errcode_t): %x\n", ret);
    } else {
        uint16_t handle;
        ret = gatts_add_descriptor_sync(server_id, srv_handle, &descriptor, &handle);
        bts_sample_log("gatts add descriptor sync ret(errcode_t): %x handle:0x%04x.\n", ret, handle);
    }
    bts_sample_mem_free(descriptor.value);
}

/* 启动service params: server_id srv_handle */
static void sample_gatts_cmd_start_srv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("params_cnt error! need: server_id srv_handle\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t srv_handle = (uint16_t)atoi(params[BT_NUM1]);
    errcode_t ret = gatts_start_service(server_id, srv_handle);
    bts_sample_log("gatts_start_service ret(errcode_t): %x\n", ret);
}

/* 停止service params: server_id srv_handle */
static void sample_gatts_cmd_stop_srv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("params_cnt error! need: server_id srv_handle\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t srv_handle = (uint16_t)atoi(params[BT_NUM1]);
    errcode_t ret = gatts_stop_service(server_id, srv_handle);
    bts_sample_log("gatts_stop_service ret(errcode_t): %x\n", ret);
}

/* 删除service params: server_id srv_handle */
static void sample_gatts_cmd_del_srv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("params_cnt error! need: server_id srv_handle\n");
        return;
    }

    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t srv_handle = (uint16_t)atoi(params[BT_NUM1]);
    errcode_t ret = gatts_delete_service(server_id, srv_handle);
    bts_sample_log("gatts_delete_service ret(errcode_t): %x\n", ret);
}

/* 清除所有service params: server_id */
static void sample_gatts_cmd_clr_all_srv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("params_cnt error! need: server_id\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    errcode_t ret = gatts_delete_all_services(server_id);
    bts_sample_log("gatts delete all services ret(errcode_t): %x\n", ret);
}

/* 发送响应 params:  sample_mode server_id conn_id status attr_handle value_len value  */
__attribute__((unused)) static void sample_gatts_cmd_send_rsp(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM7) {
        bts_sample_log("params_cnt error! need: sample_mode server_id conn_id status attr_handle value_len "
            "value\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM1]);
    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gatts_send_response(server_id, 0, NULL);
    } else {
        ret = ERRCODE_NOT_SUPPORT;
    }
    bts_sample_log("ble_gatts_send_response ret(errcode_t): %x\n", ret);
}

/* 发送通知 params: sample_mode server_id conn_id attr_handle confirm value_len value */
static void sample_gatts_cmd_send_ind(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM7) {
        bts_sample_log("params_cnt error! need: sample_mode server_id conn_id attr_handle confirm value_len "
            "value\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM1]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM2]);
    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gatts_notify_indicate(server_id, conn_id, NULL);
    } else {
        gatts_ntf_ind_t ind_para;
        ind_para.attr_handle = (uint16_t)atoi(params[BT_NUM3]);
        ind_para.value_len = (uint16_t)atoi(params[BT_NUM5]);
        ind_para.value = bts_sample_mem_new(ind_para.value_len);
        if (decode_hex_from_str(params[BT_NUM6], ind_para.value) != EXT_ERR_SUCCESS) {
            bts_sample_mem_free(ind_para.value);
            bts_sample_log("[GATTServer] failed to decode input value.\n");
            return;
        }
        ret = gatts_notify_indicate(server_id, conn_id, &ind_para);
        bts_sample_mem_free(ind_para.value);
    }
    bts_sample_log("ble_gatts_send_indication ret(errcode_t): %x\n", ret);
}

/* 发送通知 params: server_id conn_id uuid start_hdl end_hdl value_len value */
static void sample_gatts_cmd_send_ind_by_uuid(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM7) {
        bts_sample_log("params_cnt error! need: server_id conn_id uuid start_hdl end_hdl value_len value\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);

    gatts_ntf_ind_by_uuid_t ind_para = {0};
    ind_para.start_handle = (uint16_t)atoi(params[BT_NUM3]);
    ind_para.end_handle = (uint16_t)atoi(params[BT_NUM4]);
    ind_para.value_len = (uint16_t)atoi(params[BT_NUM5]);
    ind_para.chara_uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM2])) / 2; /* 2个字符为1字节; */
    if (decode_uuid_from_str(params[BT_NUM2], ind_para.chara_uuid.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample gatts cmd send notify by uuid decode chara_uuid error!\n");
        return;
    }
    ind_para.value = bts_sample_mem_new(ind_para.value_len);
    if (decode_hex_from_str(params[BT_NUM6], ind_para.value) != EXT_ERR_SUCCESS) {
        bts_sample_mem_free(ind_para.value);
        bts_sample_log("[GATTServer] failed to decode input value.\n");
        return;
    }
    errcode_t ret = gatts_notify_indicate_by_uuid(server_id, conn_id, &ind_para);
    bts_sample_mem_free(ind_para.value);
    bts_sample_log("gatts notify indicate by uuid ret(errcode_t): %x\n", ret);
}

/* 发送通知 params: server_id mtu_size */
static void sample_gatts_cmd_set_mtu_size(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("params_cnt error! need: server_id mtu_size.\n");
        return;
    }
    uint8_t server_id = (uint8_t)atoi(params[BT_NUM0]);
    uint32_t mtu_size = (uint32_t)atoi(params[BT_NUM1]);

    errcode_t ret = gatts_set_mtu_size(server_id, mtu_size);
    bts_sample_log("gatts set mtu size ret(errcode_t): %x\n", ret);
}

/* 注册GATT client回调 params: sample_mode fill_callbacks */
static void sample_gattc_cmd_reg_cbk(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint32_t fill_callbacks = (uint32_t)atoi(params[BT_NUM1]);

    errcode_t ret;
    gattc_callbacks_t cb = {
        sample_gattc_discovery_svc_cbk,
        sample_gattc_discovery_svc_response,
        sample_gattc_discovery_char_cbk,
        sample_gattc_discovery_char_response,
        sample_gattc_discovery_desc_cbk,
        sample_gattc_discovery_desc_response,
        sample_gattc_read_cfm_cbk,
        sample_gattc_read_by_uuid_response,
        sample_gattc_write_cfm_cbk,
        sample_gattc_mtu_changed_cbk,
        sample_gattc_notification_cbk,
        sample_gattc_indication_cbk,
    };
    g_auto_client_discovery = false;
    if ((sample_mode & 0x1) != 0) {
        ret = gattc_register_callbacks(NULL);
    } else if (fill_callbacks == BLE_CLIENT_CBK_FILL_MODE_ALL) {
        ret = gattc_register_callbacks(&cb);
        bts_sample_log("[GATTClient]ble_gattc_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else if (fill_callbacks == BLE_CLIENT_CBK_FILL_MODE_PROFILE) {
        ret = ble_client_register_gatt_callbacks();
        bts_sample_log("[GATTClient] register gatt callback with funcs ret(int): %x\n", ret);
    } else if (fill_callbacks == BLE_CLIENT_CBK_FILL_MODE_NO_NOTIFY) {
        cb.notification_cb = NULL;
        ret = gattc_register_callbacks(&cb);
        bts_sample_log("[GATTClient]ble_gattc_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else if (fill_callbacks == BLE_CLIENT_CBK_FILL_MODE_AUTO) {
        g_auto_client_discovery = true;
        ret = gattc_register_callbacks(&cb);
        bts_sample_log("[GATTClient]ble_gattc_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else {
        gattc_callbacks_t cb1 = { 0, };
        ret = gattc_register_callbacks(&cb1);
        bts_sample_log("[GATTClient]ble_gattc_register_callbacks test ret(errcode_t): %x\n", ret);
    }
}

static void sample_gattc_cmd_reg_id(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("gattc_register params_cnt error! need: sample_mode app_uuid\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    bt_uuid_t uuid;
    uint8_t client_id;
    if ((sample_mode & 0x1) != 0) {
        uuid.uuid_len = 0;
    } else {
        uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM1])) / 2; /* 2个字符为1字节 */
        if (decode_uuid_from_str(params[BT_NUM1], uuid.uuid) != EXT_ERR_SUCCESS) {
            bts_sample_log("decode app_uuid error!\n");
            return;
        }
    }
    errcode_t ret = gattc_register_client(&uuid, &client_id);
    bts_sample_log("[GATTClient]gattc_register ret(errcode_t): %x client_id:%u\n", ret, client_id);
}


/* 取消注册 gatt 客户端 params: client_id  */
static void sample_gattc_cmd_unreg_id(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("params_cnt error! need: client_id\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    errcode_t ret = gattc_unregister_client(client_id);
    bts_sample_log("[GATTClient]gattc_unregister ret(errcode_t): %x\n", ret);
}

/* 搜索 service params: client_id conn_id uuid */
static void sample_gattc_cmd_discovery_service(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2 && params_cnt != BT_NUM3) {
        bts_sample_log("params_cnt error! need: client_id\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    bt_uuid_t uuid = {0};
    errcode_t ret;
    if (params_cnt == BT_NUM2) {
        uuid.uuid_len = 0;
        ret = gattc_discovery_service(client_id, conn_id, &uuid);
    } else {
        uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM2])) / 2; /* 2个字符为1字节 */
        if (decode_uuid_from_str(params[BT_NUM2], uuid.uuid) != EXT_ERR_SUCCESS) {
            bts_sample_log("decode service uuid error!\n");
            return;
        }
        ret = gattc_discovery_service(client_id, conn_id, &uuid);
    }
    bts_sample_log("[GATTClient]Discovery service for client(id:%u) conn(id:%u) ret(errcode_t): %x\n",
        client_id, conn_id, ret);
}

/* 搜索 characteristic params: client_id conn_id service_handle uuid */
static void sample_gattc_cmd_discovery_character(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM3 && params_cnt != BT_NUM4) {
        bts_sample_log("params_cnt error! need: client_id\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    uint16_t svc_handle = (uint16_t)atoi(params[BT_NUM2]);
    gattc_discovery_character_param_t characteristic = {0};
    characteristic.service_handle = svc_handle;
    errcode_t ret;
    if (params_cnt == BT_NUM3) {
        characteristic.uuid.uuid_len = 0;
        ret = gattc_discovery_character(client_id, conn_id, &characteristic);
    } else {
        characteristic.uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM3])) / 2; /* 2个字符为1字节 */
        if (decode_uuid_from_str(params[BT_NUM3], characteristic.uuid.uuid) != EXT_ERR_SUCCESS) {
            bts_sample_log("decode service uuid error!\n");
            return;
        }
        ret = gattc_discovery_character(client_id, conn_id, &characteristic);
    }
    bts_sample_log("[GATTClient]Discovery charactersitic for client(id:%u) conn(id:%u) svc_hdl(id:%u)"
        " ret(errcode_t): %x\n", client_id, conn_id, svc_handle, ret);
}

/* 搜索 descriptor params: client_id conn_id character_handle */
static void sample_gattc_cmd_discovery_descriptor(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM3) {
        bts_sample_log("params_cnt error! need: client_id\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    uint16_t character_handle = (uint16_t)atoi(params[BT_NUM2]);
    errcode_t ret = gattc_discovery_descriptor(client_id, conn_id, character_handle);

    bts_sample_log("[GATTClient]Discovery descriptor for client(id:%u) conn(id:%u) chara_hdl(id:%u)"
        " ret(errcode_t): %x\n", client_id, conn_id, character_handle, ret);
}

__attribute__((unused)) static bool btsrv_sample_init_uuid_from_str(bt_uuid_t *uuid, const char *param)
{
    uuid->uuid_len = (uint8_t)(strlen(param)) / 2; /* 2个字符为1字节 */
    if (decode_uuid_from_str(param, uuid->uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode app_uuid error!\n");
        return false;
    }
    return true;
}

/* 通过 att handle 读: client_id conn_id att_handle */
static void sample_gattc_cmd_read_by_handle(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM3) {
        bts_sample_log("params_cnt error! need: client_id conn_id att_handle \r\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    uint16_t att_handle = (uint16_t)atoi(params[BT_NUM2]);

    errcode_t ret = gattc_read_req_by_handle(client_id, conn_id, att_handle);
    bts_sample_log("[GATTClient]Read by handle ret(errcode_t): %x\n", ret);
}

/* 通过 uuid 读: client_id conn_id start_hdl end_hdl uuid */
static void sample_gattc_cmd_read_by_uuid(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM5) {
        bts_sample_log("params_cnt error! need: client_id conn_id att_handle \r\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    gattc_read_req_by_uuid_param_t read_req = { 0 };
    read_req.start_hdl = (uint16_t)atoi(params[BT_NUM2]);
    read_req.end_hdl = (uint16_t)atoi(params[BT_NUM3]);
    read_req.uuid.uuid_len = (uint8_t)(strlen(params[BT_NUM4])) / 2; /* 2个字符为1字节 param 4 */
    if (decode_uuid_from_str(params[BT_NUM4], read_req.uuid.uuid) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode service uuid error!\n");
        return;
    }

    errcode_t ret = gattc_read_req_by_uuid(client_id, conn_id, &read_req);
    bts_sample_log("[GATTClient]Read by uuid ret(errcode_t): %x\n", ret);
}

/* 写请求 params: client_id conn_id att_handle len value */
static void sample_gattc_cmd_write_req(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM5) {
        bts_sample_log("params_cnt error! need: client_id conn_id att_handle len value.\r\n");
        return;
    }
    gattc_handle_value_t att_value = { 0 };
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    att_value.handle = (uint16_t)atoi(params[BT_NUM2]);
    att_value.data_len = (uint16_t)atoi(params[BT_NUM3]);
    att_value.data = (uint8_t *)bts_sample_mem_new(sizeof(char) * att_value.data_len);
    if (att_value.data == NULL) {
        bts_sample_log("malloc att_value.data error!\n");
        return;
    }
    if (decode_hex_from_str(params[BT_NUM4], att_value.data) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode att_value.data error!\n");
        bts_sample_mem_free(att_value.data);
        return;
    }
    errcode_t ret = gattc_write_req(client_id, conn_id, &att_value);

    bts_sample_log("[GATTClient]Write requet ret(errcode_t): %x\n", ret);
    bts_sample_mem_free(att_value.data);
}

/* 写命令 params: client_id conn_id att_handle len value */
static void sample_gattc_cmd_write_cmd(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM5) {
        bts_sample_log("params_cnt error! need: client_id conn_id att_handle len value.\r\n");
        return;
    }
    gattc_handle_value_t att_value = { 0 };
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    att_value.handle = (uint16_t)atoi(params[BT_NUM2]);
    att_value.data_len = (uint16_t)atoi(params[BT_NUM3]);
    att_value.data = (uint8_t *)bts_sample_mem_new(sizeof(char) * att_value.data_len);
    if (att_value.data == NULL) {
        bts_sample_log("malloc att_value.data error!\n");
        return;
    }
    if (decode_hex_from_str(params[BT_NUM4], att_value.data) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode att_value.data error!\n");
        bts_sample_mem_free(att_value.data);
        return;
    }
    errcode_t ret = gattc_write_cmd(client_id, conn_id, &att_value);

    bts_sample_log("[GATTClient]Write cmd ret(errcode_t): %x\n", ret);
    bts_sample_mem_free(att_value.data);
}

/* 配置 mtu : client_id conn_id mtu_size */
static void sample_gattc_cmd_exchange_mtu(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM3) {
        bts_sample_log("params_cnt error! need: client_id conn_id mtu_size \r\n");
        return;
    }
    uint8_t client_id = (uint8_t)atoi(params[BT_NUM0]);
    uint16_t conn_id = (uint16_t)atoi(params[BT_NUM1]);
    uint32_t mtu_size = (uint32_t)atoi(params[BT_NUM2]);

    errcode_t ret = gattc_exchange_mtu_req(client_id, conn_id, mtu_size);
    bts_sample_log("[GATTClient]Exchange mtu ret(errcode_t): %x\n", ret);
}

/* 获取远端设备RSSI params: sample_mode client_id addr */
__attribute__((unused)) static void sample_gattc_cmd_read_remote_rssi(char **params, int32_t params_cnt)
{
    bts_unused(params);
    if (params_cnt != BT_NUM3) {
        bts_sample_log("BleGattcReadRemoteRssi params_cnt error! need: sample_mode client_id addr\n");
        return;
    }
}

/* 设置BLE广播数据 params: sample_mode adv_id adv_data scan_rsp_data */
static void sample_gatt_cmd_ble_set_adv_data(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM4) {
        bts_sample_log("params_cnt error! need: sample_mode adv_id adv_data scan_rsp_data\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    uint8_t adv_id = (uint8_t)atoi(params[BT_NUM1]);
    char *adv_data = (char *)params[BT_NUM2];
    char *scan_rsp_data = (char *)params[BT_NUM3];
    gap_ble_config_adv_data_t cfg_adv_data;
    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_ble_set_adv_data(adv_id, NULL);
    } else {
        cfg_adv_data.adv_length = (uint16_t)(strlen(adv_data)) / 2; /* 2个字符为1字节 */
        cfg_adv_data.scan_rsp_length = (uint16_t)(strlen(scan_rsp_data)) / 2; /* 2个字符为1字节 */
        cfg_adv_data.adv_data = bts_sample_mem_new(cfg_adv_data.adv_length);
        if (cfg_adv_data.adv_data == NULL) {
            bts_sample_log("malloc adv data error!\n");
            return;
        }
        cfg_adv_data.scan_rsp_data = bts_sample_mem_new(cfg_adv_data.scan_rsp_length);
        if (cfg_adv_data.scan_rsp_data == NULL) {
            bts_sample_mem_free(cfg_adv_data.adv_data);
            bts_sample_log("malloc scan data error!\n");
            return;
        }
        if (decode_hex_from_str(adv_data, cfg_adv_data.adv_data) != EXT_ERR_SUCCESS ||
            decode_hex_from_str(scan_rsp_data, cfg_adv_data.scan_rsp_data) != EXT_ERR_SUCCESS) {
            bts_sample_log("decode adv data error!\n");
            bts_sample_mem_free(cfg_adv_data.adv_data);
            bts_sample_mem_free(cfg_adv_data.scan_rsp_data);
            return;
        }
        ret = gap_ble_set_adv_data(adv_id, &cfg_adv_data);
        bts_sample_mem_free(cfg_adv_data.adv_data);
        bts_sample_mem_free(cfg_adv_data.scan_rsp_data);
    }
    bts_sample_log("gap_ble_set_adv_data ret(errcode_t): %x\n", ret);
}

/* 设置广播参数 params: adv_id min_int max_int adv_type channel_map adv_filter_policy tx_pwr duration [peer_addr] */
static void sample_gatt_cmd_ble_set_adv_param(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM8 && params_cnt != BT_NUM9) {
        bts_sample_log("params_cnt error! need: adv_id min_int max_int adv_type channel_map"
         "adv_filter_policy tx_pwr duration [peer_addr]\n");
        return;
    }
    gap_ble_adv_params_t adv_param = { 0 };
    uint8_t adv_id = (uint8_t)atoi(params[BT_NUM0]);
    adv_param.min_interval = (uint32_t)atoi(params[BT_NUM1]);
    adv_param.max_interval = (uint32_t)atoi(params[BT_NUM2]);
    adv_param.adv_type = (uint8_t)atoi(params[BT_NUM3]);
    adv_param.channel_map = (uint8_t)atoi(params[BT_NUM4]);
    adv_param.adv_filter_policy = (uint8_t)atoi(params[BT_NUM5]);
    adv_param.tx_power = (uint32_t)atoi(params[BT_NUM6]);
    adv_param.duration = (uint32_t)atoi(params[BT_NUM7]);
    if (params_cnt == BT_NUM9 &&
        decode_bd_addr_from_str(params[BT_NUM8], adv_param.peer_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("set ble adv param decode peer addr error!\n");
        return;
    }
    errcode_t ret = gap_ble_set_adv_param(adv_id, &adv_param);
    bts_sample_log("gap ble set adv param ret(errcode_t): %x\n", ret);
}

/* 开始广播 */
static void sample_gatt_cmd_ble_start_adv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("ggap ble start adv params_cnt error! need: adv_id.\r\n");
        return;
    }
    uint8_t adv_id = (uint8_t)atoi(params[BT_NUM0]);
    errcode_t ret = gap_ble_start_adv(adv_id);
    bts_sample_log("gap ble start adv ret(errcode_t): %x\n", ret);
}

/* 停止广播 params: adv_id */
static void sample_gatt_cmd_ble_stop_adv(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("gap ble stop adv params_cnt error! need: adv_id\n");
        return;
    }
    uint8_t adv_id = (uint8_t)atoi(params[BT_NUM0]);
    errcode_t ret = gap_ble_stop_adv(adv_id);
    bts_sample_log("gap ble stop adv ret(errcode_t): %x\n", ret);
}

/* 设置扫描参数 params: scan_interval scan_window scan_type scan_phy scan_filter_policy */
static void sample_gatt_cmd_ble_set_scan_para(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM5) {
        bts_sample_log("gap ble set scan parameters params_cnt error! need: scan_interval scan_window"
            " scan_type scan_phy scan_filter_policy.\r\n");
        return;
    }

    gap_ble_scan_params_t scan_param = { 0 };
    scan_param.scan_interval = (uint16_t)atoi(params[BT_NUM0]);
    scan_param.scan_window = (uint16_t)atoi(params[BT_NUM1]);
    scan_param.scan_type = (uint8_t)atoi(params[BT_NUM2]);
    scan_param.scan_phy = (uint8_t)atoi(params[BT_NUM3]);
    scan_param.scan_filter_policy = (uint8_t)atoi(params[BT_NUM4]);
    errcode_t ret = gap_ble_set_scan_parameters(&scan_param);
    bts_sample_log("gap ble set scan parameters ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_start_scan(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    errcode_t ret = gap_ble_start_scan();
    bts_sample_log("gap ble start scan ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_stop_scan(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    errcode_t ret = gap_ble_stop_scan();
    bts_sample_log("gap ble stop scan ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_get_local_addr(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    bd_addr_t rmt_addr = { 0 };
    errcode_t ret = gap_ble_get_local_addr(&rmt_addr);
    bts_sample_log("gap ble get local addr ret(errcode_t): %x addr_type: %u addr:"
        "0x%02x:**:**:**:%02x:%02x\r\n",
        ret, rmt_addr.type, rmt_addr.addr[BT_NUM0], rmt_addr.addr[BT_NUM4], rmt_addr.addr[BT_NUM5]);
}

static void sample_gatt_cmd_ble_set_local_addr(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1 && params_cnt != BT_NUM2) {
        bts_sample_log("gap ble set local addr params_cnt error! need: addr [addr_type].\r\n");
        return;
    }

    bd_addr_t rmt_addr = { 0 };
    if (decode_bd_addr_from_str(params[0], rmt_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
            bts_sample_log("gap ble set local addr decode addr error!\n");
            return;
        }
    if (params_cnt == BT_NUM2) {
        rmt_addr.type = (uint8_t)atoi(params[BT_NUM1]);
    }
    errcode_t ret = gap_ble_set_local_addr(&rmt_addr);
    bts_sample_log("gap ble set local addr ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_set_local_appearance(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("gap ble set local appearance params_cnt error! need: appearance.\r\n");
        return;
    }

    uint16_t appearance = (uint16_t)atoi(params[BT_NUM0]);
    errcode_t ret = gap_ble_set_local_appearance(appearance);
    bts_sample_log("gap ble set local appearance ret(errcode_t): %x\n", ret);
}

#define SAMPLE_BLE_NAME_LEN_MAX 255
static void sample_gatt_cmd_ble_get_local_name(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    uint8_t name[SAMPLE_BLE_NAME_LEN_MAX];
    uint8_t len = SAMPLE_BLE_NAME_LEN_MAX;
    errcode_t ret = gap_ble_get_local_name(name, &len);
    if (len >= SAMPLE_BLE_NAME_LEN_MAX) {
        len = SAMPLE_BLE_NAME_LEN_MAX;
    }
    name[len - 1] = '\0';
    bts_sample_log("gap ble get local name ret(errcode_t): %x name:%s len:%u.\n", ret, name, len);
}

static void sample_gatt_cmd_ble_set_local_name(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("gap ble set local name params_cnt error! need: name length.\r\n");
        return;
    }

    const uint8_t *name = (const uint8_t *)params[0];
    uint8_t len = (uint8_t)atoi(params[1]);
    errcode_t ret = gap_ble_set_local_name(name, len);
    bts_sample_log("gap ble set local name ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_pair_remote_device(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1 && params_cnt != BT_NUM2) {
        bts_sample_log("gap ble pair remote device params_cnt error! need: addr [addr_type].\r\n");
        return;
    }

    bd_addr_t rmt_addr = { 0 };
    if (decode_bd_addr_from_str(params[0], rmt_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
            bts_sample_log("gap ble pair remote device decode addr error!\n");
            return;
        }
    if (params_cnt == BT_NUM2) {
        rmt_addr.type = (uint8_t)atoi(params[BT_NUM1]);
    }
    errcode_t ret = gap_ble_pair_remote_device(&rmt_addr);
    bts_sample_log("gap ble pair remote device ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_get_paired_devices_num(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    uint16_t number;
    errcode_t ret = gap_ble_get_paired_devices_num(&number);
    bts_sample_log("gap ble pair remote device ret(errcode_t): %x number: %u.\r\n", ret, number);
}

static void sample_gatt_cmd_ble_get_paired_devices(char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    bd_addr_t *paired_list = NULL;
    uint16_t number = 0;
    errcode_t ret = gap_ble_get_paired_devices_num(&number);
    if (ret != ERRCODE_BT_SUCCESS || number <= 0) {
        bts_sample_log("gap ble pair remote device ret(errcode_t): %x number: %u.\r\n", ret, number);
        return;
    }
    paired_list = (bd_addr_t *)(void *)bts_sample_mem_new((uint32_t)number * sizeof(bd_addr_t));
    if (paired_list == NULL) {
        bts_sample_log("gap ble pair remote device malloc error!\n");
        return;
    }
    if (memset_s(paired_list, (size_t)number * sizeof(bd_addr_t), 0, (size_t)number * sizeof(bd_addr_t)) != EOK) {
        bts_sample_log("gap ble pair remote device memset error!\n");
        return;
    }
    ret = gap_ble_get_paired_devices(paired_list, &number);
    bts_sample_log("gap ble pair remote device ret(errcode_t): %x number: %u.\r\n", ret, number);
    if (ret == ERRCODE_BT_SUCCESS) {
        bts_sample_log("show ble paired list:\n");
        bts_sample_log("| %-4s | %-17s |\n", "idx", "address");
        for (int i = 0; i < number; i++) {
            bts_sample_log("| %-4d | %02x:%02x:**:**:**:%02x |\n", i,
                (paired_list + i)->addr[5], (paired_list + i)->addr[4], /* idx 5 4 */
                (paired_list + i)->addr[0]); /* idx 0 */
        }
        bts_sample_log("show ble paired list end!\n");
    }
    if (paired_list) {
        bts_sample_mem_free(paired_list);
    }
}

static void sample_gatt_cmd_ble_get_pair_state(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1 && params_cnt != BT_NUM2) {
        bts_sample_log("gap ble get pair state params_cnt error! need: addr [addr_type].\r\n");
        return;
    }

    bd_addr_t rmt_addr = { 0 };
    if (decode_bd_addr_from_str(params[0], rmt_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
            bts_sample_log("gap ble get pair state decode addr error!\n");
            return;
        }
    if (params_cnt == BT_NUM2) {
        rmt_addr.type = (uint8_t)atoi(params[BT_NUM1]);
    }
    gap_ble_pair_state_t state;
    errcode_t ret = gap_ble_get_pair_state(&rmt_addr, &state);
    bts_sample_log("gap ble get pair state ret(errcode_t): %x state(gap_ble_pair_state_t): %d\n", ret, state);
}

static void sample_gatt_cmd_ble_remove_pair(char **params, int32_t params_cnt)
{
    errcode_t ret;
    if (params_cnt > BT_NUM2) {
        bts_sample_log("gap ble remove pair params_cnt error!\r\n");
        return;
    }

    if (params_cnt == BT_NUM0) {
        ret = gap_ble_remove_all_pairs();
    } else {
        bd_addr_t rmt_addr = { 0 };
        if (decode_bd_addr_from_str(params[0], rmt_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
            bts_sample_log("gap ble remove pair decode addr error!\n");
            return;
        }
        if (params_cnt == BT_NUM2) {
            rmt_addr.type = (uint8_t)atoi(params[BT_NUM1]);
        }
        ret = gap_ble_remove_pair(&rmt_addr);
    }
    bts_sample_log("gap ble remove pair ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_read_rssi(char **params, int32_t params_cnt)
{
    errcode_t ret;
    if (params_cnt != BT_NUM1) {
        bts_sample_log("gap ble read rssi params_cnt error!\r\n");
        return;
    }
    uint16_t conn_id = (uint16_t)atoi(params[0]);
    ret = gap_ble_read_remote_device_rssi(conn_id);
    bts_sample_log("gap ble read rssi ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_set_phy(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM5) {
        bts_sample_log("gap ble set phy params_cnt error!\r\n");
        return;
    }
    gap_le_set_phy_t param = { 0 };
    param.conn_handle = (uint16_t)atoi(params[BT_NUM0]);
    param.all_phys = (uint8_t)atoi(params[BT_NUM1]);
    param.rx_phys = (uint8_t)atoi(params[BT_NUM2]);
    param.tx_phys = (uint8_t)atoi(params[BT_NUM3]);
    param.phy_options = (uint16_t)atoi(params[BT_NUM4]);
    errcode_t ret = gap_ble_set_phy(&param);
    bts_sample_log("gap ble set phy ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_set_data_len(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM3) {
        bts_sample_log("gap ble set data len params_cnt error!\r\n");
        return;
    }
    gap_le_set_data_length_t param = { 0 };
    param.conn_handle = (uint16_t)atoi(params[BT_NUM0]);
    param.maxtxoctets = (uint16_t)atoi(params[BT_NUM1]);
    param.maxtxtime = (uint16_t)atoi(params[BT_NUM2]);
    errcode_t ret = gap_ble_set_data_length(&param);
    bts_sample_log("gap ble set data length ret(errcode_t): %x\n", ret);
}

static void sample_gap_ble_enable_stack_cbk(errcode_t status)
{
    bts_sample_log("EnableBLECallback status: %u\n", status);
}

static void sample_gap_ble_disable_stack_cbk(errcode_t status)
{
    bts_sample_log("DisableBLECallback status: %u\n", status);
}

static void sample_gap_ble_set_adv_data_cbk(uint8_t adv_id, errcode_t status)
{
    bts_sample_log("SetAdvDataCallback adv_id: %u status: %u\n", adv_id, status);
}

static void sample_gap_ble_set_adv_param_cbk(uint8_t adv_id, errcode_t status)
{
    bts_sample_log("SetAdvParamCallback adv_id: %u status: %u\n", adv_id, status);
}

static void sample_gap_ble_set_scan_param_cbk(errcode_t status)
{
    bts_sample_log("SetScanParameterCallback status: %u\n", status);
}

static void sample_gap_ble_start_adv_cbk(uint8_t adv_id, adv_status_t status)
{
    bts_sample_log("StartAdvCallback adv_id: %u status: %d\n", adv_id, status);
}

static void sample_gap_ble_stop_adv_cbk(uint8_t adv_id, adv_status_t status)
{
    bts_sample_log("StopAdvCallback adv_id: %u status: %d\n", adv_id, status);
}

static void sample_gap_ble_scan_result_cbk(gap_scan_result_data_t *scan_result_data)
{
    if (scan_result_data == NULL) {
        bts_sample_log("scan_result_data is null \n");
        return;
    }
    bts_sample_log("event_type:%u addr_type: %u addr:0x%02x:**:**:**:%02x:%02x adv_len:%u.\r\n",
        scan_result_data->event_type, scan_result_data->addr.type, scan_result_data->addr.addr[BT_NUM0],
        scan_result_data->addr.addr[BT_NUM4], scan_result_data->addr.addr[BT_NUM5], scan_result_data->adv_len);
}

static void sample_gap_ble_conn_state_change_cbk(uint16_t conn_id, bd_addr_t *addr, gap_ble_conn_state_t conn_state,
    gap_ble_pair_state_t pair_state, gap_ble_disc_reason_t disc_reason)
{
    bts_sample_log("ConnectStateChanged conn_id:%u addr:0x%02x:**:**:**:%02x:%02x addr_type:%u"
        "conn_state:%d pair_state:%d\r\n", conn_id, addr->addr[BT_NUM0], addr->addr[BT_NUM4],
        addr->addr[BT_NUM5], addr->type, conn_state, pair_state);
    if (conn_state == 0) { /* 断连时，清理 client profile的远端列表 */
        bts_sample_log("ConnectStateChanged disc_reason:%x\r\n", disc_reason);
        ble_dis_client_clear_remote_by_conn(1, conn_id);
        ble_bas_client_clear_remote_by_conn(1, conn_id);
        ble_fmp_locator_clear_remote_by_conn(1, conn_id);
        ble_ancs_client_clear_remote_by_conn(1, conn_id);
        ble_ams_client_clear_remote_by_conn(1, conn_id);
    }
}

static void sample_gap_ble_paried_complete_cbk(uint16_t conn_id, const bd_addr_t *addr, errcode_t status)
{
    bts_sample_log("PairedCompleteCall conn_id:%u status:%u\r\n", conn_id, status);
    if (addr != NULL) {
        bts_sample_log("addr:0x%02x:**:**:**:%02x:%02x addr_type:%u\r\n",
            addr->addr[BT_NUM0], addr->addr[BT_NUM4], addr->addr[BT_NUM5], addr->type);
    }
}

static void sample_gap_ble_terminate_adv_cbk(uint8_t adv_id, adv_status_t status)
{
    bts_sample_log("TerminateAdvCall adv_id:%u status:%d\r\n", adv_id, status);
}

static void sample_gap_ble_auth_complete_cbk(uint16_t conn_id, const bd_addr_t *addr, errcode_t status,
    const ble_auth_info_evt_t* evt)
{
    bts_sample_log("AuthCompleteCall conn_id:%u status:0x%x\r\n", conn_id, status);
    if (addr != NULL) {
        bts_sample_log("addr:0x%02x:**:**:**:%02x:%02x addr_type:%u\r\n",
            addr->addr[BT_NUM0], addr->addr[BT_NUM4], addr->addr[BT_NUM5], addr->type);
    }
    if (evt != NULL) {
        bts_sample_log("ltk_len:%u ltk:", evt->ltk_len);
        for (uint8_t i = 0; i < evt->ltk_len; i++) {
            bts_sample_log("%02x", evt->ltk[i]);
        }
        bts_sample_log("\n");
    }
}

static void sample_gap_ble_read_remote_rssi_cbk(uint16_t conn_id, int8_t rssi, errcode_t status)
{
    bts_sample_log("read remote rssi conn_id:%u rssi:%d status:%u\r\n", conn_id, rssi, status);
}

static void sample_gap_ble_connect_param_update_cbk(uint16_t conn_id, errcode_t status,
    const gap_ble_conn_param_update_t *param)
{
    bts_sample_log("connect param changed conn_id:%u status:%u", conn_id, status);
    if (param != NULL) {
        bts_sample_log(" interval:%u latency:%u timeout:%u\r\n", param->interval, param->latency, param->timeout);
    }
}

/* 注册GATT回调 params: sample_mode fill_callbacks */
static void sample_gatt_cmd_ble_gatt_reg_cbk(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) {
        bts_sample_log("gap_ble_register_callbacks params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = (uint32_t)atoi(params[BT_NUM0]);
    int fill_callbacks = atoi(params[BT_NUM1]);

    errcode_t ret;
    if ((sample_mode & 0x1) != 0) {
        ret = gap_ble_register_callbacks(NULL);
    } else if (fill_callbacks != 0) {
        gap_ble_callbacks_t cb = {
            sample_gap_ble_enable_stack_cbk,
            sample_gap_ble_disable_stack_cbk,
            sample_gap_ble_set_adv_data_cbk,
            sample_gap_ble_set_adv_param_cbk,
            sample_gap_ble_set_scan_param_cbk,
            sample_gap_ble_start_adv_cbk,
            sample_gap_ble_stop_adv_cbk,
            sample_gap_ble_scan_result_cbk,
            sample_gap_ble_conn_state_change_cbk,
            sample_gap_ble_paried_complete_cbk,
            sample_gap_ble_read_remote_rssi_cbk,
            sample_gap_ble_terminate_adv_cbk,
            sample_gap_ble_auth_complete_cbk,
            sample_gap_ble_connect_param_update_cbk,
        };
        ret = gap_ble_register_callbacks(&cb);
        bts_sample_log("gap_ble_register_callbacks with funcs ret(errcode_t): %x\n", ret);
    } else {
        gap_ble_callbacks_t cb = { 0, };
        ret =  gap_ble_register_callbacks(&cb);
        bts_sample_log("gap_ble_register_callbacks test ret(errcode_t): %x\n", ret);
    }
}

/* 设置安全参数 params: bondable io_capability sc_enable sc_mode */
static void sample_gatt_cmd_ble_set_sec_param(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM4) {
        bts_sample_log("params_cnt error! need: bondable io_capability sc_enable sc_mode \n");
        return;
    }
    gap_ble_sec_params_t sec_params = { 0 };
    sec_params.bondable = (uint8_t)atoi(params[BT_NUM0]);
    sec_params.io_capability = (uint8_t)atoi(params[BT_NUM1]);
    sec_params.sc_enable = (uint8_t)atoi(params[BT_NUM2]);
    sec_params.sc_mode = (uint8_t)atoi(params[BT_NUM3]);
    errcode_t ret = gap_ble_set_sec_param(&sec_params);
    bts_sample_log("gap ble set sec param ret(errcode_t): %x\n", ret);
}

static void sample_gatt_cmd_ble_get_conn_param(char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM1) {
        bts_sample_log("params_cnt error! need: connect_handle \n");
        return;
    }
    uint16_t conn_hdl = (uint16_t)atoi(params[BT_NUM0]);
    gap_ble_conn_param_update_t conn_param = {0};
    errcode_t ret = gap_ble_get_connection_param(conn_hdl, &conn_param);
    bts_sample_log("ble connect param ret(errcode_t): %x interval:%u latency:%u timeout:%u\n",
        ret, conn_param.interval, conn_param.latency, conn_param.timeout);
}

static void sample_gattc_speed(char **params, int32_t params_cnt)
{
    uint32_t interval = (uint32_t)atoi(params[BT_NUM0]);
    uint16_t handle = (uint16_t)atoi(params[BT_NUM1]);
    uint16_t len = (uint16_t)atoi(params[BT_NUM2]);
    uint16_t num = (uint16_t)atoi(params[BT_NUM3]);
    bts_unused(params_cnt);
    if (len >= BLE_SPEED_TEST_MAX_DATA_LENGTH) {
        len = BLE_SPEED_TEST_MAX_DATA_LENGTH;
    } else if (len < BLE_SPEED_TEST_MIN_DATA_LENGTH) {
        len = BLE_SPEED_TEST_MIN_DATA_LENGTH;
    }
    bts_sample_log("server interval:%u!\n", interval);
    char param_data[10] = {'0', '1', '0', '2', '0', '3', '0', '4', '0', '5'}; /* 十六进制位数:10 */
    uint8_t client_id = BLE_SPEED_TEST_CLIENT_ID;
    uint16_t conn_id = BLE_SPEED_TEST_CONNECT_ID;
    gattc_handle_value_t att_value = { 0 };
    att_value.handle = handle;
    att_value.data_len = len;
    att_value.data = (uint8_t *)bts_sample_mem_new(sizeof(char) * att_value.data_len);
    if (att_value.data == NULL) {
        bts_sample_log("malloc att_value.data error!\n");
        return;
    }
    if (decode_hex_from_str(param_data, att_value.data) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode att_value.data error!\n");
        bts_sample_mem_free(att_value.data);
        return;
    }
    bts_sample_log("client write start!\n");
    uint32_t count = 1;
    for (;;) {
        if (count <= num) {
            gattc_write_cmd(client_id, conn_id, &att_value);
            count = count + 1;
        } else {
            count = 1;
            osal_msleep(interval);
        }
    }
    bts_sample_log("client write end!\n");
    bts_sample_mem_free(att_value.data);
}
 
static void sample_gatts_speed(char **params, int32_t params_cnt)
{
    uint32_t interval = (uint32_t)atoi(params[BT_NUM0]);
    uint16_t handle = (uint16_t)atoi(params[BT_NUM1]);
    uint16_t len = (uint16_t)atoi(params[BT_NUM2]);
    uint16_t num = (uint16_t)atoi(params[BT_NUM3]);
    bts_unused(params_cnt);
    if (len >= BLE_SPEED_TEST_MAX_DATA_LENGTH) {
        len = BLE_SPEED_TEST_MAX_DATA_LENGTH;
    } else if (len < BLE_SPEED_TEST_MIN_DATA_LENGTH) {
        len = BLE_SPEED_TEST_MIN_DATA_LENGTH;
    }
    bts_sample_log("server interval:%u!\n", interval);
    uint8_t server_id = BLE_SPEED_TEST_SERVER_ID;
    uint16_t conn_id = BLE_SPEED_TEST_CONNECT_ID;
    char param_data[10] = {'1', '1', '1', '2', '1', '3', '1', '4', '1', '5'}; /* 十六进制位数:10 */
    gatts_ntf_ind_t ind_para;
    ind_para.attr_handle = handle;
    ind_para.value_len = len;
    ind_para.value = bts_sample_mem_new(ind_para.value_len);
    if (ind_para.value == NULL) {
        bts_sample_log("malloc ind_para.data error!\n");
        return;
    }
    if (decode_hex_from_str(param_data, ind_para.value) != EXT_ERR_SUCCESS) {
        bts_sample_mem_free(ind_para.value);
        bts_sample_log("[GATTServer] failed to decode input value.\n");
        return;
    }
    bts_sample_log("server notify start!\n");
    uint32_t count = 1;
    for (;;) {
        if (count <= num) {
            gatts_notify_indicate(server_id, conn_id, &ind_para);
            count = count + 1;
        } else {
            count = 1;
            osal_msleep(interval);
        }
    }
    bts_sample_log("server notify end!\n");
    bts_sample_mem_free(ind_para.value);
}

static void sample_gatt_cmd_ble_ctkd_param(char **params, int32_t params_cnt)
{
    errcode_t ret;
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("ble secure ctkd param error! need: bd_addr.\n");
        return;
    }
 
    bool ctkd_enable = (bool)atoi(params[0]);
    ret = gap_ble_set_ctkd_enable(ctkd_enable);
    bts_sample_log("set ble ctkd ret:0x%x.\n", ret);
}

static void sample_gatts_service_character_descriptor(gatts_add_desc_info_t *descriptor, uint16_t service_handle,
    uint16_t *desc_handle, uint8_t *ccc_value)
{
    descriptor->desc_uuid.uuid_len = GATT_UUID_LEN_2;
    descriptor->desc_uuid.uuid[0] = 0x29;
    descriptor->desc_uuid.uuid[1] = 0x02;
    descriptor->permissions = (GATT_ATTRIBUTE_PERMISSION_READ | GATT_ATTRIBUTE_PERMISSION_WRITE);
    descriptor->value_len = BLE_TEST_SERVICE_CCC_LEN;
    descriptor->value = ccc_value;
    gatts_add_descriptor_sync(1, service_handle, descriptor, desc_handle);
    bts_sample_log("[GATTServer] AddCCC handle=%d.\r\n", *desc_handle);
}

/* 验证BLE添加服务上限 */
static void sample_gatts_service(char **params, int32_t params_cnt)
{
    bts_unused(params_cnt);
    uint32_t service_num = (uint32_t)atoi(params[0]);
    uint32_t character_num = (uint32_t)atoi(params[1]);
    uint16_t value_len = (uint16_t)atoi(params[2]);    // 2 第三个参数
    uint16_t service_handle = 0;
    uint16_t desc_handle = 0;
    bt_uuid_t srv_uuid = {0};
    bt_uuid_t desc_uuid = {0};
    gatts_add_chara_info_t character = {0};
    gatts_add_character_result_t result = {0};
    uint8_t *value_data = (uint8_t *)bts_sample_mem_new(value_len);
    value_data[0] = 0xab;
    gatts_add_desc_info_t descriptor = {0};
    uint8_t ccc_value[BLE_TEST_SERVICE_CCC_LEN] = {0x01, 0x00};
    bts_sample_log("[GATTServer] service limit test start!\n");
    for (uint32_t i = 0; i < service_num; i++) {
        service_handle = 0;
        srv_uuid.uuid_len = GATT_UUID_LEN_2;
        srv_uuid.uuid[0] = 0x18;
        srv_uuid.uuid[1] = (BLE_TEST_SERVICE_UUID_AUTO_OFFSET + i);
        gatts_add_service_sync(1, &srv_uuid, true, &service_handle);
        bts_sample_log("[GATTServer] AddService handle=%d.\r\n", service_handle);
        if (service_handle != 0) {
            for (uint32_t k = 0; k < character_num; k++) {
                character.chara_uuid.uuid_len = GATT_UUID_LEN_2;
                character.chara_uuid.uuid[0] = 0x2C;
                character.chara_uuid.uuid[1] = (1 + k);
                character.permissions = (GATT_ATTRIBUTE_PERMISSION_READ | GATT_ATTRIBUTE_PERMISSION_WRITE);
                character.properties = (GATT_CHARACTER_PROPERTY_BIT_READ |
                    GATT_CHARACTER_PROPERTY_BIT_WRITE_NO_RSP | GATT_CHARACTER_PROPERTY_BIT_NOTIFY);
                character.value_len = value_len;
                character.value = value_data;
                gatts_add_characteristic_sync(1, service_handle, &character, &result);
                bts_sample_log("[GATTServer] AddCharacter handle=%d value_handle=%d.\r\n",
                    result.handle, result.value_handle);
                desc_handle = 0;
                sample_gatts_service_character_descriptor(&descriptor, service_handle,
                    &desc_handle, ccc_value);
            }
            gatts_start_service(1, service_handle);
            while (g_test_service_handle != service_handle) {
                osal_msleep(BLE_TEST_SERVICE_START_WAIT_MS);
            }
        }
    }
    bts_sample_log("[GATTServer] service limit test end!\n");
}

typedef void (*ble_at_gatt_cmd_handle_func)(char **params, int32_t params_cnt);
typedef struct {
    uint32_t id;
    ble_at_gatt_cmd_handle_func func;
} ble_at_gatt_cmd_hdl_info;

ble_at_gatt_cmd_hdl_info g_btsrv_gatt_sample_hdls[] = {
    /* 0 -- GATT Server */
    { BTSRV_SAMPLE_GATTS_REG_ID,  sample_gatts_cmd_reg_id },
    { BTSRV_SAMPLE_GATTS_UNREG_ID,  sample_gatts_cmd_unreg_id },
    { BTSRV_SAMPLE_GATTS_DISCONN,  NULL },
    { BTSRV_SAMPLE_GATTS_ADD_SRV,  sample_gatts_cmd_add_srv },
    { BTSRV_SAMPLE_GATTS_ADD_INCL_SRV, NULL },

    /* 5 */
    { BTSRV_SAMPLE_GATTS_ADD_CHAR, sample_gatts_cmd_add_char },
    { BTSRV_SAMPLE_GATTS_ADD_DESC, sample_gatts_cmd_add_desc },
    { BTSRV_SAMPLE_GATTS_START_SRV, sample_gatts_cmd_start_srv },
    { BTSRV_SAMPLE_GATTS_STOP_SRV, sample_gatts_cmd_stop_srv },
    { BTSRV_SAMPLE_GATTS_DEL_SRV, sample_gatts_cmd_del_srv },

    /* 10 */
    { BTSRV_SAMPLE_GATTS_CLR_ALL_SRV, sample_gatts_cmd_clr_all_srv },
    { BTSRV_SAMPLE_GATTS_SEND_RSP, NULL },
    { BTSRV_SAMPLE_GATTS_SEND_IND, sample_gatts_cmd_send_ind },
    { BTSRV_SAMPLE_GATTS_SET_ENC, NULL },
    { BTSRV_SAMPLE_GATTS_REG_CBK, sample_gatts_cmd_reg_cbk },

    /* 15 */
    { BTSRV_SAMPLE_GATTS_ADD_SVC_SYNC, NULL },
    { BTSRV_SAMPLE_GATTS_ADD_CHAR_SYNC, NULL },
    { BTSRV_SAMPLE_GATTS_ADD_DESC_SYNC, NULL },
    { BTSRV_SAMPLE_GATTS_NOTIFY_BY_UUID, sample_gatts_cmd_send_ind_by_uuid },
    { BTSRV_SAMPLE_GATTS_SET_MTU, sample_gatts_cmd_set_mtu_size },

    /* 20 -- GATT Client */
    { BTSRV_SAMPLE_GATTC_REG_ID, sample_gattc_cmd_reg_id },
    { BTSRV_SAMPLE_GATTC_UNREG_ID, sample_gattc_cmd_unreg_id },
    { BTSRV_SAMPLE_GATTC_DISC_SVC, sample_gattc_cmd_discovery_service },
    { BTSRV_SAMPLE_GATTC_DISC_CHAR, sample_gattc_cmd_discovery_character },
    { BTSRV_SAMPLE_GATTC_DISC_DESC, sample_gattc_cmd_discovery_descriptor },

    /* 25 */
    { BTSRV_SAMPLE_GATTC_READ_BY_HANDLE, sample_gattc_cmd_read_by_handle },
    { BTSRV_SAMPLE_GATTC_READ_BY_UUID, sample_gattc_cmd_read_by_uuid},
    { BTSRV_SAMPLE_GATTC_WRITE_REQ, sample_gattc_cmd_write_req },
    { BTSRV_SAMPLE_GATTC_WRITE_CMD, sample_gattc_cmd_write_cmd },
    { BTSRV_SAMPLE_GATTC_CONFIG_MTU, sample_gattc_cmd_exchange_mtu },

    /* 30 */
    { BTSRV_SAMPLE_GATTC_REG_CBK, sample_gattc_cmd_reg_cbk },
    { BTSRV_SAMPLE_GATT_BLE_GET_LOCAL_ADDR, sample_gatt_cmd_ble_get_local_addr },
    { BTSRV_SAMPLE_GATT_BLE_SET_LOCAL_ADDR, sample_gatt_cmd_ble_set_local_addr },

    /* 35 */
    { BTSRV_SAMPLE_GATT_BLE_SET_APPEARANCE, sample_gatt_cmd_ble_set_local_appearance },
    { BTSRV_SAMPLE_GATT_BLE_GET_DEV_NAME, sample_gatt_cmd_ble_get_local_name },
    { BTSRV_SAMPLE_GATT_BLE_SET_DEV_NAME, sample_gatt_cmd_ble_set_local_name },
    { BTSRV_SAMPLE_GATT_BLE_PAIR_RMT_DEV, sample_gatt_cmd_ble_pair_remote_device },
    { BTSRV_SAMPLE_GATT_BLE_GET_PAIRED_DEV_NUM, sample_gatt_cmd_ble_get_paired_devices_num },

    /* 40 -- GATT ADV & SCAN */
    { BTSRV_SAMPLE_GATT_BLE_SET_ADV_DATA, sample_gatt_cmd_ble_set_adv_data },
    { BTSRV_SAMPLE_GATT_BLE_SET_ADV_PARAM, sample_gatt_cmd_ble_set_adv_param },
    { BTSRV_SAMPLE_GATT_BLE_START_ADV, sample_gatt_cmd_ble_start_adv },
    { BTSRV_SAMPLE_GATT_BLE_STOP_ADV, sample_gatt_cmd_ble_stop_adv },
    { BTSRV_SAMPLE_GATT_BLE_SET_SCAN_PARA, sample_gatt_cmd_ble_set_scan_para },

    /* 45 */
    { BTSRV_SAMPLE_GATT_BLE_START_SCAN, sample_gatt_cmd_ble_start_scan },
    { BTSRV_SAMPLE_GATT_BLE_STOP_SCAN, sample_gatt_cmd_ble_stop_scan },
    { BTSRV_SAMPLE_GATT_BLE_GATT_REG_CBK, sample_gatt_cmd_ble_gatt_reg_cbk },
    { BTSRV_SAMPLE_GATT_BLE_GET_PAIR_DEV, sample_gatt_cmd_ble_get_paired_devices },
    { BTSRV_SAMPLE_GATT_BLE_GET_PAIR_STATUS, sample_gatt_cmd_ble_get_pair_state },

    /* 50 */
    { BTSRV_SAMPLE_GATT_BLE_RMV_PAIR, sample_gatt_cmd_ble_remove_pair },
    { BTSRV_SAMPLE_GATT_BLE_READ_RSSI, sample_gatt_cmd_ble_read_rssi },
    { BTSRV_SAMPLE_GATT_BLE_SET_PHY, sample_gatt_cmd_ble_set_phy },
    { BTSRV_SAMPLE_GATT_BLE_SET_DATA_LEN, sample_gatt_cmd_ble_set_data_len },
    { BTSRV_SAMPLE_GATT_BLE_GET_CONN_PARAM, sample_gatt_cmd_ble_get_conn_param },

    /* 56 */
    { BTSRV_SAMPLE_GATTS_SHOW_LIST, NULL },
    { BTSRV_SAMPLE_GATTC_CONN_LIST, NULL },
    { BTSRV_SAMPLE_GATT_BLE_SET_SEC_PARAM, sample_gatt_cmd_ble_set_sec_param },
    { BTSRV_SAMPLE_GATTC_SPEED, sample_gattc_speed},
    { BTSRV_SAMPLE_GATTS_SPEED, sample_gatts_speed},

    /* 61 */
    { BTSRV_SAMPLE_GATT_BLE_SET_CTKD_ENABLE, sample_gatt_cmd_ble_ctkd_param},
    { BTSRV_SAMPLE_GATTS_SERVICE, sample_gatts_service},
};

void btsrv_gatt_sample_cmd(uint32_t cmd_id, char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_btsrv_gatt_sample_hdls) / sizeof(ble_at_gatt_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_btsrv_gatt_sample_hdls[i].id == cmd_id && g_btsrv_gatt_sample_hdls[i].func != NULL) {
            g_btsrv_gatt_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("btsrv_gatt_sample_cmd cmd_id %d not support now!\n", cmd_id);
}
