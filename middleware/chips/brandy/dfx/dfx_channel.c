/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: dfx channel
 * This file should be changed only infrequently and with great care.
 */

#include <stdint.h>
#include "errcode.h"
#include "dfx_adapt_layer.h"
#include "diag.h"
#include "diag_ind_src.h"
#include "diag_channel.h"
#include "zdiag_adapt_layer.h"
#include "log_file.h"
#include "log_uart.h"
#include "uart.h"
#include "soc_diag_cmd_id.h"
#include "diag_msg.h"
#include "debug_print.h"
#include "bts_br_gap.h"
#include "bts_spp.h"
#include "dfx_channel.h"
#if ENABLE_LOW_POWER == YES
#include "pm_definition.h"
#endif
#include "bts_gatt_server.h"

#define DIAG_SER_DATA_MFS        990
#define DIAG_SER_GATT_DATA_MFS   500
#ifdef SUPPORT_ALIPAY_SEC
#define DIAG_SER_DATA_MAX_LEN    0x3000
#else
#define DIAG_SER_DATA_MAX_LEN    0x800
#endif

diag_channel_id_t g_diag_channel_id = DIAG_CHANNEL_ID_INVALID;
static uart_bus_t g_diag_uart_id = UART_BUS_1;
static osal_mutex g_diag_uart_out_mutex = {NULL};

static uint16_t g_diag_gatt_hdl = 0;
static uint8_t g_diag_gatt_server_id = 0;
static uint16_t g_diag_gatt_conn_id = 0;

static void diag_channel_rx_bt_data(uint8_t *data, uint16_t size)
{
    (void)uapi_diag_channel_rx_mux_char_data(DIAG_CHANNEL_ID_1, data, size);
}

#ifdef SUPPORT_BREDR
uint8_t g_spp_client_id;
static void spp_recv_data_cb(uint8_t client_id, uint8_t *data, uint32_t data_len)
{
    g_spp_client_id = client_id;
    diag_channel_rx_bt_data(data, (uint16_t)data_len);
}
static void conn_state_changed_cb(uint8_t client_id, profile_connect_state_t state)
{
    unused(state);
    g_spp_client_id = client_id;
}
#endif

static errcode_t diag_channel_bt_output(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[],
                                        uint8_t cnt)
{
    unused(fd);
    unused(data_type);
    unused(cnt);

    if (data[0] == NULL || len[0] == 0) {
        return ERRCODE_INVALID_PARAM;
    }
#ifdef SUPPORT_BREDR
    if (spp_write((int)g_spp_client_id, (const char*)data[0], (const uint32_t)len[0]) < 0) {
        return ERRCODE_FAIL;
    } else {
        return ERRCODE_SUCC;
    }
#else
    return ERRCODE_SUCC;
#endif
}

static void diag_channel_register_bt_callback(void)
{
#ifdef SUPPORT_BREDR
    spp_callbacks_t callbacks = {
        spp_recv_data_cb,
        conn_state_changed_cb,
    };

    (void)spp_register_callbacks(&callbacks);
#endif
}

static errcode_t diag_channel_bt_gatt_output(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[],
    uint8_t cnt)
{
    errcode_t ret = ERRCODE_FAIL;
    if (data[0] == NULL || len[0] == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    gatts_ntf_ind_t param = {0};
    param.attr_handle = g_diag_gatt_hdl;
    param.value_len = len[0];
    param.value = data[0];

    if (g_diag_gatt_hdl != 0) {
        ret = gatts_notify_indicate(g_diag_gatt_server_id, g_diag_gatt_conn_id, &param);
    }
    return ret;
}

void diag_gatt_set_handle(uint8_t server_id, uint16_t conn_id, uint16_t handle)
{
    g_diag_gatt_server_id = server_id;
    g_diag_gatt_conn_id = conn_id;
    g_diag_gatt_hdl = handle;
}

void diag_gatt_write_req_cbk(uint8_t server_id, uint16_t conn_id, gatts_req_write_cb_t *write_cb_para,
    errcode_t status)
{
    diag_gatt_set_handle(server_id, conn_id, write_cb_para->handle);
    (void)uapi_diag_channel_rx_frame_data(DIAG_CHANNEL_ID_2, write_cb_para->value, write_cb_para->length);
}

static void diag_channel_register_bt_gatt_callback(void)
{
#ifndef SUPPORT_OHOSFWK
    gatts_callbacks_t cb = {0};
    cb.write_request_cb = diag_gatt_write_req_cbk;
    (void)gatts_register_callbacks(&cb);
#endif
}

errcode_t diag_cmd_uart_config(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);
    errcode_t ret = ERRCODE_FAIL;

    if (g_diag_uart_id >= UART_BUS_MAX_NUM) {
        return ERRCODE_INVALID_PARAM;
    }
    if (cmd_id == DIAG_CMD_CHANGE_UART_CONFIG) {
        uart_attr_t *attr = cmd_param;
        ret = uapi_uart_set_attr(g_diag_uart_id, attr);
        if (ret == ERRCODE_SUCC) {
            PRINT("uart set cfg OK! baud_rate = %d data_bits = %d stop_bits = %d parity = %d\n",
                  attr->baud_rate, attr->data_bits, attr->stop_bits, attr->parity);
            uapi_diag_report_packet(cmd_id, option, (uint8_t *)("success\r\n"),
                (uint16_t)strlen("success\r\n") + 1, true);
        } else {
            PRINT("uart set cfg failed!\r\n");
            uapi_diag_report_packet(cmd_id, option, (uint8_t *)("fail\r\n"), (uint16_t)strlen("fail\r\n") + 1, false);
        }
    } else if (cmd_id == DIAG_CMD_GET_UART_CONFIG) {
        uart_attr_t get_attr = {0};
        ret = uapi_uart_get_attr(g_diag_uart_id, &get_attr);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)&get_attr, sizeof(get_attr), true);
    }

    return ret;
}

void diag_uart_rx_proc(uint8_t *buffer, uint16_t length)
{
    uapi_diag_channel_rx_mux_char_data(DIAG_CHANNEL_ID_0, buffer, length);
}

static int32_t diag_channel_uart_output(void *fd, dfx_data_type_t data_type, uint8_t *data[], uint16_t len[],
                                        uint8_t cnt)
{
    unused(fd);
    unused(data_type);
#if ENABLE_LOW_POWER == YES
    uapi_pm_add_sleep_veto(PM_ID_SYS);
#endif
    int int_flag = osal_in_interrupt();
    if (int_flag == 0) {
        (void)osal_mutex_lock_timeout(&g_diag_uart_out_mutex, OSAL_MUTEX_WAIT_FOREVER);
    }
    for (uint8_t i = 0; i < cnt; i++) {
        if (len[i] != 0) {
            log_uart_send_buffer(data[i], len[i]);
        }
    }
    if (int_flag == 0) {
        osal_mutex_unlock(&g_diag_uart_out_mutex);
    }
#if ENABLE_LOW_POWER == YES
    uapi_pm_remove_sleep_veto(PM_ID_SYS);
#endif
    return ERRCODE_SUCC;
}

errcode_t diag_cmd_set_offline_log(uint16_t cmd_id, void *cmd_param, uint16_t cmd_param_size, diag_option_t *option)
{
    unused(cmd_param_size);

    bool offline_enabled = false;
    uint32_t enable = *(uint32_t *)cmd_param;

    if (enable == (uint32_t)-1) {
        offline_enabled = uapi_zdiag_offline_log_is_enable();
    } else if (enable == 0 || enable == 1) {
        offline_enabled = (enable == 0) ? false : true;
        uapi_zdiag_set_offline_log_enable(offline_enabled);
        if (offline_enabled != uapi_zdiag_offline_log_is_enable()) {
            uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Set Offline log failed\r\n"),
                                    (uint16_t)strlen("Set Offline log failed\r\n") + 1, true);
            return ERRCODE_SUCC;
        }
    } else if (enable == 2) { /* 2 means logfile suspend */
        (void)uapi_logfile_suspend(STORE_DIAG);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log suspend\r\n"),
                                strlen("Offline log suspend\r\n") + 1, true);
        return ERRCODE_SUCC;
    } else if (enable == 3) { /* 3 means logfile resume */
        (void)uapi_logfile_resume(STORE_DIAG);
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log resume\r\n"),
                                strlen("Offline log resume\r\n") + 1, true);
        return ERRCODE_SUCC;
    } else {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Invaild Parameter\r\n"),
                                (uint16_t)strlen("Invaild Parameter\r\n") + 1, true);
        return ERRCODE_SUCC;
    }

    if (offline_enabled) {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log Enabled\r\n"),
                                (uint16_t)strlen("Offline log Enabled\r\n") + 1, true);
    } else {
        uapi_diag_report_packet(cmd_id, option, (uint8_t *)("Offline log Disabled\r\n"),
                                (uint16_t)strlen("Offline log Disabled\r\n") + 1, true);
    }

    return ERRCODE_SUCC;
}

errcode_t diag_register_channel(void)
{
    g_diag_channel_id = DIAG_CHANNEL_ID_0;
    if (uapi_diag_channel_init(g_diag_channel_id, DIAG_CHANNEL_ATTR_NEED_RX_BUF) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    if (uapi_diag_channel_set_connect_hso_addr(g_diag_channel_id, (uint8_t)diag_adapt_get_default_dst()) !=
        ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    if (uapi_diag_channel_set_tx_hook(g_diag_channel_id, diag_channel_uart_output) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    osal_mutex_init(&g_diag_uart_out_mutex);

#if (CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE == DFX_YES)
    errcode_t ret;
    /* SPP通道 */
    ret = uapi_diag_channel_init(DIAG_CHANNEL_ID_1, DIAG_CHANNEL_ATTR_NEED_RX_BUF);
    if (ret != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (uapi_diag_channel_set_tx_hook(DIAG_CHANNEL_ID_1, diag_channel_bt_output) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (uapi_diag_channel_set_mfs(DIAG_CHANNEL_ID_1, DIAG_SER_DATA_MFS, DIAG_SER_DATA_MAX_LEN) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    diag_channel_register_bt_callback();

    /* BLE GATT通道 */
    if (uapi_diag_channel_init(DIAG_CHANNEL_ID_2, DIAG_CHANNEL_ATTR_NONE) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (uapi_diag_channel_set_tx_hook(DIAG_CHANNEL_ID_2, diag_channel_bt_gatt_output) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    if (uapi_diag_channel_set_mfs(DIAG_CHANNEL_ID_2, DIAG_SER_GATT_DATA_MFS, DIAG_SER_DATA_MAX_LEN) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }

    diag_channel_register_bt_gatt_callback();

#endif /* CONFIG_DFX_SUPPORT_DIAG_CONNECT_BLE */
    return ERRCODE_SUCC;
}