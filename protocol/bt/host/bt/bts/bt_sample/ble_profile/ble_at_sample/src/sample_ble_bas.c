/*
 * Copyright (c) CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test function of BAS Service module.
 */
#include <stdint.h>
#include <stdlib.h>
#include "sample_common.h"
#include "ble_bas_client.h"
#include "ble_bas_server.h"
#include "sample_ble_bas.h"

#define BLE_BAS_SAMPLE_PARAM_INDEX_2 2
#define LONG_MIN    (-LONG_MAX - 1)
#define LONG_MAX    2147483647L

/* 命令处理函数接口 */
typedef void (*ble_at_bas_cmd_handle_func_t)(uint8_t argc, char **argv);

/* BAS服务端接口命令id与接口对应关系 */
typedef struct {
    uint8_t id;
    ble_at_bas_cmd_handle_func_t func;
} ble_at_bas_srv_cmd_hdl_info_t;

/* BAS服务端接口命令id */
typedef enum {
    BTSRV_SAMPLE_BLE_BAS_SRV_INIT = 1,              /* 1 bas server init */
    BTSRV_SAMPLE_BLE_BAS_SRV_GET_BATTERY_LEVEL, /* 2 bas server get battery level */
    BTSRV_SAMPLE_BLE_BAS_SRV_NOTIFY_BATTERY_LEVEL, /* 3 bas server notify battery level */
} ble_at_bas_srv_cmd_id_t;

/* BAS客户端接口命令id与接口对应关系 */
typedef struct {
    uint8_t id;
    ble_at_bas_cmd_handle_func_t func;
} ble_at_bas_cln_cmd_hdl_info_t;

/* BAS客户端接口命令id */
typedef enum {
    BTSRV_SAMPLE_BLE_BAS_CLN_BAS_SVC = 1,                                /* 1 bas client discovery service */
    BTSRV_SAMPLE_BLE_BAS_CLN_RD_BATTERY_LEVEL,                       /* 2 bas client read remote battery level */
    BTSRV_SAMPLE_BLE_BAS_CLN_RD_CHARACTERISTIC_DESCRIPTOR,           /* 3 bas client read remote
                                                                      characteristic descriptor */
    BTSRV_SAMPLE_BLE_BAS_CLN_WR_CLIENT_CHARACTERISTIC_CONFIGURATION, /* 4 bas client write remote client characteristic
                                                                       configuration */
} ble_at_bas_cln_cmd_id_t;

/* BAS服务端服务初始化结果回调函数 */
static void bas_server_init_execute_result_cbk(uint8_t result)
{
    bts_sample_info_log("bas service init result:%u\r\n", result);
}

/* BAS服务端初始化 */
static void bt_at_cmd_bas_srv_init(uint8_t argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    ble_bas_server_init(bas_server_init_execute_result_cbk);
    return;
}

/* BAS服务端读取电量值缓存 */
static void bt_at_cmd_bas_srv_get_battery_level(uint8_t argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    uint8_t data = ble_bas_server_battery_level_get();
    bts_sample_info_log("get battery level is %u\r\n", data);
    return;
}

/* BAS服务端电量值变化通知对端 */
static void bt_at_cmd_bas_srv_notify_battery_level(uint8_t argc, char *argv[])
{
    char *end;
    if (argc != BT_NUM1) {
        bts_sample_error_log("bas server notify cmd argc num invalid:%u\r\n", argc);
        return;
    }
    uint8_t data = (uint8_t)strtol(argv[0], &end, 10); // 10:base
    if (end == argv[0] || *end != '\0') {
        bts_sample_error_log("string to long int error!\r\n");
        return;
    } else if (((long)data == LONG_MIN || (long)data == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_error_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    (void)ble_bas_server_battery_level_notify(data);
    return;
}

/* BAS服务端命令与接口对应关系 */
static ble_at_bas_srv_cmd_hdl_info_t g_ble_at_bas_srv_cmd_hdls[] = {
    { BTSRV_SAMPLE_BLE_BAS_SRV_INIT, bt_at_cmd_bas_srv_init },
    { BTSRV_SAMPLE_BLE_BAS_SRV_GET_BATTERY_LEVEL, bt_at_cmd_bas_srv_get_battery_level },
    { BTSRV_SAMPLE_BLE_BAS_SRV_NOTIFY_BATTERY_LEVEL, bt_at_cmd_bas_srv_notify_battery_level }
};

void ble_at_bas_srv_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt)
{
    size_t hdl_cnt = sizeof(g_ble_at_bas_srv_cmd_hdls) / sizeof(ble_at_bas_srv_cmd_hdl_info_t);
    for (uint8_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_at_bas_srv_cmd_hdls[i].id == cmd_id && g_ble_at_bas_srv_cmd_hdls[i].func != NULL) {
            g_ble_at_bas_srv_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}

/* BAS客户端接收电量通知回调函数 */
static void bas_cln_notify_cbk(uint8_t value)
{
    bts_sample_info_log("bas receive change notify, battery level:%u\r\n", value);
}

/* BAS客户端读取电量值回调函数 */
static void bas_cln_character_read_cbk(uint8_t value)
{
    bts_sample_info_log("bas battery level:%u\r\n", value);
}

/* 读取特征格式描述符回调函数 */
static void bas_cln_cfd_read_cbk(bas_client_cfd_t cfd_info)
{
    bts_sample_info_log("bas cfd format is 0x%02X\r\n", cfd_info.format);
    bts_sample_info_log("bas cfd exponent is 0x%02X\r\n", cfd_info.exponent);
    bts_sample_info_log("bas cfd unit is 0x%02X%02x\r\n", cfd_info.unit.uuid[1], cfd_info.unit.uuid[0]);
    bts_sample_info_log("bas cfd name_space is 0x%02X\r\n", cfd_info.name_space);
    bts_sample_info_log("bas cfd description is 0x%02X%02x\r\n", cfd_info.description.uuid[1],
        cfd_info.description.uuid[0]);
}

/* BAS客户端读取客户端特征配置描述符回调函数 */
static void bas_cln_cccd_read_cbk(uint16_t value)
{
    bts_sample_info_log("bas cccd value:0x%04X\r\n", value);
}

/* BAS客户端发现服务结果回调函数 */
static void bas_cln_discovery_execute_result_cbk(uint8_t result)
{
    bts_sample_info_log("bas service discovery result:%u\r\n", result);
}

/* BAS客户端发现服务 */
static void bt_at_cmd_bas_cln_discovery_service(uint8_t argc, char *argv[])
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("bas read battery level cmd argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    errcode_t ret = ble_bas_client_discovery_service(client_id, conn_id, bas_cln_discovery_execute_result_cbk);
    bts_sample_info_log("bas client discovery service result:%u\r\n", ret);
    return;
}

/* BAS客户端读取电量值 */
static void bt_at_cmd_bas_cln_rd_rmt_battery_level(uint8_t argc, char *argv[])
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("bas read battery level cmd argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    bts_sample_info_log("bas read character client_id:0x%x conn_id:0x%x\r\n", client_id, conn_id);
    errcode_t ret = ble_bas_client_read_battery_level(client_id, conn_id,
        bas_cln_character_read_cbk, bas_cln_notify_cbk);
    bts_sample_info_log("bas read character result:%u\r\n", ret);
    return;
}

/* BAS客户端读取电量特征的描述符 */
static void bt_at_cmd_bas_cln_rd_rmt_characteristic_descriptor(uint8_t argc, char *argv[])
{
    if (argc != BT_NUM3) {
        bts_sample_error_log("bas read cfd cmd argc num invalid :%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    uint8_t type = (uint8_t)atoi(argv[BLE_BAS_SAMPLE_PARAM_INDEX_2]);
    
    errcode_t ret;
    if (type == 0) {
        ret = ble_bas_client_read_format_descriptor(client_id, conn_id, bas_cln_cfd_read_cbk);
    } else if (type == 1) {
        ret = ble_bas_client_read_ccc_descriptor(client_id, conn_id, bas_cln_cccd_read_cbk);
    } else {
        bts_sample_error_log("bas read cfd cmd argc param3 invalid :%u\r\n", type);
        return;
    }
    bts_sample_info_log("bas read descriptor result:%u\r\n", ret);
    return;
}

/* BAS客户端关闭或者开启电量值通知 */
static void bt_at_cmd_bas_cln_wr_rmt_client_characteristic_configuration(uint8_t argc, char *argv[])
{
    uint8_t enable_flag;
    if (argc != BT_NUM3) {
        bts_sample_error_log("bas write ccd cmd argc invalid num:%u\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    enable_flag = (uint8_t)atoi(argv[BLE_BAS_SAMPLE_PARAM_INDEX_2]);

    errcode_t ret;
    if (enable_flag == 0) {
        ret = ble_bas_client_write_ccc_descriptor(client_id, conn_id, BAS_CLIENT_CCCD_NOTIFY_DISABLE);
    } else {
        ret = ble_bas_client_write_ccc_descriptor(client_id, conn_id, BAS_CLIENT_CCCD_NOTIFY_ENABLE);
    }
    bts_sample_info_log("bas write ccd result:%u\r\n", ret);
    return;
}

/* BAS客户端命令与接口对应关系 */
static ble_at_bas_cln_cmd_hdl_info_t g_ble_at_bas_cln_cmd_hdls[] = {
    { BTSRV_SAMPLE_BLE_BAS_CLN_BAS_SVC, bt_at_cmd_bas_cln_discovery_service },
    { BTSRV_SAMPLE_BLE_BAS_CLN_RD_BATTERY_LEVEL, bt_at_cmd_bas_cln_rd_rmt_battery_level },
    { BTSRV_SAMPLE_BLE_BAS_CLN_RD_CHARACTERISTIC_DESCRIPTOR, bt_at_cmd_bas_cln_rd_rmt_characteristic_descriptor},
    { BTSRV_SAMPLE_BLE_BAS_CLN_WR_CLIENT_CHARACTERISTIC_CONFIGURATION,
      bt_at_cmd_bas_cln_wr_rmt_client_characteristic_configuration}
};

void ble_at_bas_cln_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt)
{
    size_t hdl_cnt = sizeof(g_ble_at_bas_cln_cmd_hdls) / sizeof(ble_at_bas_cln_cmd_hdl_info_t);
    for (uint8_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_at_bas_cln_cmd_hdls[i].id == cmd_id && g_ble_at_bas_cln_cmd_hdls[i].func != NULL) {
            g_ble_at_bas_cln_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}
