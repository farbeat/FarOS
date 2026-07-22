/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT A2DP Service module.
 */

#include "sample_a2dp_service.h"
#include <soc_osal.h>
#include <stdlib.h>
#include <td_base.h>
#include <securec.h>
#include "bts_a2dp_source.h"
#include "sample_common.h"
#include "bts_br_gap.h"
#include "bts_avrcp_controller.h"
#include "bts_avrcp_target.h"
#ifndef MEMORY_MINI
#include "bt_audio_manager_wrapper.h"
#endif

#define SAMPLE_A2DP_TASK_STACK_SIZE 0x1800
#define LONG_MIN    (-LONG_MAX - 1)
#define LONG_MAX    2147483647L
/* a2dp service test cmd id */
typedef enum {
    SAMPLE_A2DP_SRC_ID_REGISTER_CALLBACK,     /* 0 注册回调 */
    SAMPLE_A2DP_SRC_ID_DEREGISTER_CALLBACK,   /* 1 取消回调注册 */
    SAMPLE_A2DP_SRC_ID_GET_CONN_STATE,        /* 2 获取连接状态 */
    SAMPLE_A2DP_SRC_ID_GET_PLAYING_STATE,     /* 3 获取播放状态 */
    SAMPLE_A2DP_SRC_ID_CONNECT,               /* 4 与设备建立A2DP连接 */
    SAMPLE_A2DP_SRC_ID_DISCONNECT,            /* 5 与设备断开A2DP连接 */
    SAMPLE_A2DP_SRC_ID_GET_ACTIVE_DEVICE,     /* 6 获取连接活跃设备 */
} sample_adapter_service_cmd_id;

static void sample_a2dp_src_connect_state_changed_callback(bd_addr_t *bd_addr, int conn_state)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x conn_state: %x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], conn_state); /* 3 2 1 0 idx */

    if (conn_state == PROFILE_STATE_CONNECTED) {
#ifndef MEMORY_MINI
        audio_manager_set_device_connection_state(OUT_BLUETOOTH_A2DP, AUDIO_DEVICE_CONNECTED);
#endif
    }

    if (conn_state == PROFILE_STATE_DISCONNECTED) {
#ifndef MEMORY_MINI
        audio_manager_set_device_connection_state(OUT_BLUETOOTH_A2DP, AUDIO_DEVICE_DISCONNECTED);
#endif
    }
}

static void sample_a2dp_src_playing_state_changed_callback(bd_addr_t *bd_addr, int playing_state)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x playing_state: %x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], playing_state); /* 3 2 1 0 idx */
    /* 播放状态变化通知CT */
    avrcp_tg_notify_playback_status_changed((unsigned char)playing_state);
}

static void sample_a2dp_src_configuration_changed_callback(bd_addr_t *bd_addr, a2dp_codec_info_t *info)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x ",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* 3 2 1 0 idx */
    bts_sample_log(
        "codec_priority: %04x codec_type: %04x sample_rate: %02x bits_per_sample: %02x channel_mode: %02x",
        info->codec_priority, info->codec_type, info->sample_rate, info->bits_per_sample, info->channel_mode);
    bts_sample_log(
        "codec_specific1: %04x codec_specific2: %04x codec_specific3: %04x codec_specific4: %04x\n",
        (unsigned int)info->codec_specific1,
        (unsigned int)info->codec_specific2,
        (unsigned int)info->codec_specific3,
        (unsigned int)info->codec_specific4);
}

a2dp_src_callbacks_t g_sample_a2dp_src_callbacks = {
    sample_a2dp_src_connect_state_changed_callback,
    sample_a2dp_src_playing_state_changed_callback,
    sample_a2dp_src_configuration_changed_callback
};
a2dp_src_callbacks_t g_sample_a2dp_src_callbacks_null = { 0, 0, 0 };

/* 注册回调 */
static void sample_a2dp_src_cmd_register_callbacks(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {    // sscanf_s  接口待平台适配
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    char *end;
    int fill_callbacks = (int)strtol(params[1], &end, 10); // 10:base
    if (end == params[1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if ((fill_callbacks == LONG_MIN || fill_callbacks == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_register_callbacks(NULL);
    } else if (fill_callbacks != 0) {
        ret = a2dp_src_register_callbacks(&g_sample_a2dp_src_callbacks);
    } else {
        ret = a2dp_src_register_callbacks(&g_sample_a2dp_src_callbacks_null);
    }

    bts_sample_log("ret(int): %x\n", ret);
}

/* wstp注册回调 */
int wstp_a2dp_src_register_callbacks(void)
{
    int ret = a2dp_src_register_callbacks(&g_sample_a2dp_src_callbacks);
#ifndef MEMORY_MINI
    /* 媒体初始化要在A2DP连接之前调用 */
    audio_manager_init(AUDIO_STREAM_MUSIC);
#endif
    bts_sample_log("a2dp reg cbk ret %x\n", ret);
    return ret;
}

/* 取消回调注册 */
static void sample_a2dp_src_cmd_deregister_callbacks(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    char *end;
    int fill_callbacks = (int)strtol(params[1], &end, 10); // 10:base
    if (end == params[1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if ((fill_callbacks == LONG_MIN || fill_callbacks == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_deregister_callbacks(NULL);
    } else if (fill_callbacks != 0) {
        ret = a2dp_src_deregister_callbacks(&g_sample_a2dp_src_callbacks);
    } else {
        ret = a2dp_src_deregister_callbacks(&g_sample_a2dp_src_callbacks_null);
    }

    bts_sample_log("ret(int): %x\n", ret);
}

/* 取消回调注册 */
int wstp_a2dp_src_deregister_callbacks(void)
{
    int ret = a2dp_src_deregister_callbacks(&g_sample_a2dp_src_callbacks);
    bts_sample_log("ret(int): %x\n", ret);
    return ret;
}

/* 获取设备连接状态 */
static void sample_a2dp_src_cmd_get_device_connect_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_get_device_connect_state(NULL);
    } else {
        ret = a2dp_src_get_device_connect_state(&bd_addr);
    }

    bts_sample_log("state(int): %d\n", ret);
}

/* 获取设备连接状态 */
int wstp_a2dp_src_get_device_connect_state(const bd_addr_t *bd_addr)
{
    int ret = a2dp_src_get_device_connect_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

/* 获取设备播放状态 */
static void sample_a2dp_src_cmd_get_playing_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_get_playing_state(NULL);
    } else {
        ret = a2dp_src_get_playing_state(&bd_addr);
    }

    bts_sample_log("state(int): %d\n", ret);
}

/* 获取设备播放状态 */
int wstp_a2dp_src_get_playing_state(const bd_addr_t *bd_addr)
{
    int ret = a2dp_src_get_playing_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

/* 与设备建立A2DP连接 */
static void sample_a2dp_src_cmd_connect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return;
    }
    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_connect(NULL);
    } else {
        ret = a2dp_src_connect(&bd_addr);
    }

    bts_sample_log("a2dp_src_connect ret(int): %x\n", ret);
}

/* 与设备建立A2DP连接 */
int wstp_a2dp_src_connect(const bd_addr_t *bd_addr)
{
    int ret = a2dp_src_connect(bd_addr);
    bts_sample_log("a2dp_src_connect ret(int): %x\n", ret);
    return ret;
}

/* 与设备断开A2DP连接 */
static void sample_a2dp_src_cmd_disconnect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: sample_mode addr\n");
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = a2dp_src_disconnect(NULL);
    } else {
        ret = a2dp_src_disconnect(&bd_addr);
    }

    bts_sample_log("a2dp_src_disconnect ret(int): %x\n", ret);
}

/* 与设备断开A2DP连接 */
int wstp_a2dp_src_disconnect(const bd_addr_t *bd_addr)
{
    int ret = a2dp_src_disconnect(bd_addr);
    bts_sample_log("a2dp_src_disconnect ret(int): %x\n", ret);
    return ret;
}

/* 获取活跃设备 */
static void sample_a2dp_src_cmd_get_active_device(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    bd_addr_t bd_addr = a2dp_src_get_active_device();

    bts_sample_log("a2dp_src_get_active_device addr: ****%02x%02x%02x%02x \n",
        bd_addr.addr[3], bd_addr.addr[2], bd_addr.addr[1], bd_addr.addr[0]); /* 3 2 1 0 idx */
}

/* 获取活跃设备 */
void wstp_a2dp_src_get_active_device(void)
{
    bd_addr_t bd_addr = a2dp_src_get_active_device();

    bts_sample_log("a2dp_src_get_active_device addr: ****%02x%02x%02x%02x \n",
        bd_addr.addr[3], bd_addr.addr[2], bd_addr.addr[1], bd_addr.addr[0]); /* 3 2 1 0 idx */
}

btsrv_sample_cmd_hdl_info g_a2dp_src_sample_hdls[] = {
    /* 0 */
    { SAMPLE_A2DP_SRC_ID_REGISTER_CALLBACK, sample_a2dp_src_cmd_register_callbacks },
    { SAMPLE_A2DP_SRC_ID_DEREGISTER_CALLBACK, sample_a2dp_src_cmd_deregister_callbacks },
    { SAMPLE_A2DP_SRC_ID_GET_CONN_STATE, sample_a2dp_src_cmd_get_device_connect_state },
    { SAMPLE_A2DP_SRC_ID_GET_PLAYING_STATE, sample_a2dp_src_cmd_get_playing_state },
    { SAMPLE_A2DP_SRC_ID_CONNECT, sample_a2dp_src_cmd_connect },

    /* 5 */
    { SAMPLE_A2DP_SRC_ID_DISCONNECT, sample_a2dp_src_cmd_disconnect },
    { SAMPLE_A2DP_SRC_ID_GET_ACTIVE_DEVICE, sample_a2dp_src_cmd_get_active_device },
};

void bt_a2dp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_a2dp_src_sample_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_a2dp_src_sample_hdls[i].id == cmd_id && g_a2dp_src_sample_hdls[i].func != NULL) {
            g_a2dp_src_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("bt_a2dp_service_sample_cmd cmd_id %d not support now!\n", cmd_id);
}
