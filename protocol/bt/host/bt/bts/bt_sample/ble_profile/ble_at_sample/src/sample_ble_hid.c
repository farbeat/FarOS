/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test function of HID Service module.
 */
#include <stdint.h>
#include "osal_task.h"
#include "osal_addr.h"
#include "sample_common.h"
#include "ble_hid_take_photo_server.h"
#include "sample_ble_hid.h"

static void bt_at_cmd_hid_srv_register_cbks(uint8_t argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    return;
}

static void bt_at_cmd_hid_srv_init(uint8_t argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    ble_hid_server_init();
    return;
}

static void bt_at_cmd_hid_send_input_report(uint8_t argc, char *argv[])
{
    bts_sample_info_log("argc:%u\r\n", argc);
    if (argc < BT_NUM1) {
        bts_sample_error_log("invalid param number:%u\r\n", argc);
        return;
    }
    uint8_t *data = (uint8_t *)bts_sample_mem_new(sizeof(uint8_t) * argc);
    for (uint8_t i = 0; i < argc; i++) {
        bts_sample_error_log("argv[%u] %s\r\n", i, argv[i]);
        data[i] = (uint8_t)atoi(argv[i]);
    }
    (void)ble_hid_server_send_input_report(data, argc); /* 上报按下按键 */
    errno_t ret = memset_s(data, sizeof(uint8_t) * argc, 0, sizeof(uint8_t) * argc);
    if (ret != EOK) {
        bts_sample_error_log("memset fail: %d\r\n", ret);
        bts_sample_mem_free(data);
        return;
    }
    osal_msleep(100); /* wait 100 ms */
    (void)ble_hid_server_send_input_report(data, argc); /* 上报按下按键 */
    bts_sample_mem_free(data);
    return;
}

/* 命令处理函数接口 */
typedef void (*ble_at_hid_cmd_handle_func)(uint8_t argc, char **argv);

/* 接口命令id与接口对应关系数据结构定义 */
typedef struct {
    uint8_t id;
    ble_at_hid_cmd_handle_func func;
} ble_hid_at_cmd_hdl_info_t;

/* 服务端接口命令id */
typedef enum {
    BLE_HID_SRV_AT_CMD_ID_REG = 0,      /* 0 hid server callback reg */
    BLE_HID_SRV_AT_CMD_ID_INIT,         /* 1 hid server init */
    BLE_HID_SRV_AT_CMD_ID_SEND_INPUT,   /* 2 hid server send input report */
} ble_hid_srv_at_cmd_id_t;

/* 服务端接口命令id与接口对应关系 */
static ble_hid_at_cmd_hdl_info_t g_ble_hid_srv_at_cmd_hdls[] = {
    { BLE_HID_SRV_AT_CMD_ID_REG, bt_at_cmd_hid_srv_register_cbks },
    { BLE_HID_SRV_AT_CMD_ID_INIT, bt_at_cmd_hid_srv_init },
    { BLE_HID_SRV_AT_CMD_ID_SEND_INPUT, bt_at_cmd_hid_send_input_report },
};

/* 服务端接口命令分发函数 */
void ble_at_hid_srv_cmd(uint8_t cmd_id, char **params, uint8_t params_cnt)
{
    size_t hdl_cnt = sizeof(g_ble_hid_srv_at_cmd_hdls) / sizeof(ble_hid_at_cmd_hdl_info_t);
    for (uint8_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_hid_srv_at_cmd_hdls[i].id == cmd_id && g_ble_hid_srv_at_cmd_hdls[i].func != NULL) {
            g_ble_hid_srv_at_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}

