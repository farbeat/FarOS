/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT PAN Service module.
 */

#include "sample_pan_service.h"
#include <soc_osal.h>
#include <stdlib.h>
#include <td_base.h>
#include <securec.h>
#include "sample_common.h"
#include "bts_br_gap.h"
#include "bts_pan.h"

typedef enum {
    SAMPLE_PAN_CMD_OPEN,              /* 0 pan acl 连接 */
    SAMPLE_PAN_CMD_CLOSE,             /* 1 关闭网络 */
    SAMPLE_PAN_CMD_NET_STATE_GET,     /* 2 获取网络状态 */
    SAMPLE_PAN_CMD_WRITE_DATA,     /* 3 写数据 */
    SAMPLE_PAN_CMD_REGISTER_CALLBACK,     /* 4 注册回调 */
    SAMPLE_PAN_CMD_DEREGISTER_CALLBACK,   /* 5 取消回调注册 */
} sample_adapter_service_cmd_id;


static void sample_pan_open(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample pan acl connect:param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample pan acl connect:decode addr error!\n");
        return;
    }
    int result = pan_service_open(&bd_addr);
    bts_sample_log("sample pan acl connect result=%d\n", result);
}

static void sample_pan_close(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample pan acl connect:param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample pan acl connect:decode addr error!\n");
        return;
    }
    int result = pan_service_close(&bd_addr);
    bts_sample_log("sample pan close result=%d\n", result);
}

static void sample_pan_net_state_get(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    uint8_t net_state = pan_service_net_state_get();
    bts_sample_log("sample pan acl connect:net_state is %u.\n", net_state);
}

static void sample_pan_net_state_callback(uint8_t net_state)
{
    bts_sample_log("sample pan net_state=%u\n", net_state);
}

static void sample_pan_rpt_ethernet(uint8_t *packet, uint16_t packet_length)
{
    bts_sample_log("sample pan rpt ethernet length=%lu, first_byte=0x%x\n.", packet_length, *packet);
}

static pan_callbacks_t g_sample_pan_callbacks_t = {
    sample_pan_net_state_callback,
    sample_pan_rpt_ethernet
};

static void sample_pan_register_callback(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    int result = pan_register_callbacks(&g_sample_pan_callbacks_t);
    bts_sample_log("sample pan register callback result=%d\n", result);
}

static void sample_pan_deregister_callback(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    int result = pan_deregister_callbacks();
    bts_sample_log("sample pan deregister callback result=%d\n", result);
}

static void sample_pan_write_data(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);
    uint8_t ip_packet[60] = {
        0x45, 0x00, 0x00, 0x3c, 0x4c, 0x8d, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00,
        0xc0, 0xa8, 0x2c, 0x01,
        0x0e, 0x77, 0x68, 0xfe,
        0x08, 0x00, 0x4c, 0x26, 0x00, 0x01, 0x01, 0x35, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
        0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
        0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69
    };
    int result = pan_service_write_data(ip_packet, 60);
    bts_sample_log("sample pan write data result=%d.\n", result);
}

btsrv_sample_cmd_hdl_info g_pan_sample_hdls[] = {
    { SAMPLE_PAN_CMD_OPEN, sample_pan_open },
    { SAMPLE_PAN_CMD_CLOSE, sample_pan_close },
    { SAMPLE_PAN_CMD_NET_STATE_GET, sample_pan_net_state_get },
    { SAMPLE_PAN_CMD_WRITE_DATA, sample_pan_write_data },
    { SAMPLE_PAN_CMD_REGISTER_CALLBACK, sample_pan_register_callback },
    { SAMPLE_PAN_CMD_DEREGISTER_CALLBACK, sample_pan_deregister_callback }
};

void bt_pan_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_pan_sample_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_pan_sample_hdls[i].id == cmd_id && g_pan_sample_hdls[i].func != NULL) {
            g_pan_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("bt pan cmd_id [%d] not support now!\n", cmd_id);
}