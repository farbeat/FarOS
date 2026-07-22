/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: This module include test function of FMP module.
 */
#include "errcode.h"
#include "ble_fmp.h"
#include "sample_common.h"
#include "ble_fmp_target.h"
#include "ble_fmp_locator.h"
#include "sample_ble_fmp.h"

static void sample_fmp_target_evt_handler(uint8_t value)
{
    switch (value) {
        case FMP_ALERT_LEVEL_NO_ALERT:
            bts_sample_info_log("Find Me Stop Alerting!\r\n");
            break;
        case FMP_ALERT_LEVEL_MILD_ALERT:
            bts_sample_info_log("Find Me Alert Mild!\r\n");
            break;
        case FMP_ALERT_LEVEL_HIGH_ALERT:
            bts_sample_info_log("Find Me Alert High!\r\n");
            break;
        default:
            bts_sample_info_log("Find Me BAD Level:%u!\r\n", value);
            break;
    }
}

static void ble_at_fmp_cmd_target_init(int argc, char *argv[])
{
    bts_unused(argc);
    bts_unused(argv);
    fmp_target_evt_handler_t evt_handler = sample_fmp_target_evt_handler;
    errcode_t ret = fmp_target_init(evt_handler);
    bts_sample_info_log("Init fmp target retcode:%u\r\n", ret);
}

static void ble_at_fmp_cmd_locator_connect(int argc, char *argv[])
{
    if (argc != BT_NUM2) {
        bts_sample_error_log("Invalid param Num:%d\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    errcode_t result = fmp_locator_connect(client_id, conn_id);
    bts_sample_info_log("fmp client connect target retcode:%u\r\n", result);
}

static void ble_at_fmp_cmd_send_alert(int argc, char *argv[])
{
    uint8_t alert_level;

    if (argc != BT_NUM3) {
        bts_sample_error_log("Invalid param Num:%d\r\n", argc);
        return;
    }
    uint8_t client_id = 0;
    uint16_t conn_id = 0;
    client_id = (uint8_t)atoi(argv[0]);
    conn_id = (uint16_t)atoi(argv[1]);
    alert_level = (uint8_t)atoi(argv[BT_NUM2]);
    errcode_t result = fmp_locator_send_alert(client_id, conn_id, alert_level);
    bts_sample_info_log("fmp client send alert retcode:%u\r\n", result);
}

typedef void (*ble_at_fmp_cmd_handle_func)(int argc, char **argv);

typedef enum {
    BLE_AT_FMP_TG_INIT = 0, /* 0 fmp target init */
    BLE_AT_FMP_LC_CONN,    /* 1 fmp locator connect */
    BLE_AT_FMP_LC_ALERT,   /* 2 fmp send alert */
} ble_at_fmp_cmd_id;

typedef struct {
    int id;
    ble_at_fmp_cmd_handle_func func;
} ble_at_fmp_cmd_info_t;

static ble_at_fmp_cmd_info_t g_ble_at_fmp_cmd_hdls[] = {
    { BLE_AT_FMP_TG_INIT, ble_at_fmp_cmd_target_init },
    { BLE_AT_FMP_LC_CONN, ble_at_fmp_cmd_locator_connect },
    { BLE_AT_FMP_LC_ALERT, ble_at_fmp_cmd_send_alert },
};

void ble_at_fmp_srv_cmd(int cmd_id, char **params, int params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_ble_at_fmp_cmd_hdls) / sizeof(ble_at_fmp_cmd_info_t);
    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_ble_at_fmp_cmd_hdls[i].id == cmd_id && g_ble_at_fmp_cmd_hdls[i].func != NULL) {
            g_ble_at_fmp_cmd_hdls[i].func(params_cnt, params);
            return;
        }
    }
}
