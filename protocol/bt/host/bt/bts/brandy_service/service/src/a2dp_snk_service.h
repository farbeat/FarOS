/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: BT A2DP Service module.
 */
#ifndef __BT_A2DP_SNK_SERVICE_H__
#define __BT_A2DP_SNK_SERVICE_H__

#include <td_type.h>
#include "bt_audio_hal_interface.h"
#include "btsrv_task.h"
#include "bts_def.h"

/* A2DP service 模块消息ID */
enum {
    A2DP_SNK_MSG_ID_HANDLE_CONN_EVENT,          /* 处理A2DP服务连接事件 */
    A2DP_SNK_MSG_ID_HANDLE_REMOTE_SEP_CAP,      /* 处理收到对端设备能力集 */
    A2DP_SNK_MSG_ID_HANDLE_STREAM_STATE_CHG,    /* 处理A2DP流状态变化 */
    A2DP_SNK_MSG_ID_HDNDLE_STREAM_CONFIG_CHG,   /* 处理A2DP流参数变化 */
    A2DP_SNK_MSG_ID_HANDLE_STREAM_REJECT,       /* 处理A2DP流拒绝事件 */
    A2DP_SNK_MSG_ID_NOTIFY_CONN_STATE_CHANGED,  /* 通知A2DP连接状态变化 */
    A2DP_SNK_MSG_ID_NOTIFY_STREAM_START_REQ,    /* 通知A2DP流开始请求 */
    A2DP_SNK_MSG_ID_NOTIFY_PLAY_STATE_CHANGED,  /* 通知A2DP播放状态变化 */
    A2DP_SNK_MSG_ID_NOTIFY_CONFIG_CHANGED,      /* 通知A2DP参数变化 */
    A2DP_SNK_MSG_ID_NOTIFY_SEND_AUDIO_STREAM,   /* 通知A2DP发送音频流 */
};

/* 发送A2DP Service模块消息 */
#define a2dp_snk_send_msg(id, p0, p1, p2) \
    btsrv_send_simple_msg(BTSRV_MSG_ID_A2DP_SNK_SERVICE, \
    (uintptr_t)(id), (uintptr_t)(p0), (uintptr_t)(p1), (uintptr_t)(p2))

#define A2DP_MOVE_BIT2 2
#define A2DP_MOVE_BIT4 4
#define A2DP_MOVE_BIT7 7
#define A2DP_MOVE_BIT8 8
#define A2DP_MOVE_BIT16 16

void a2dp_snk_service_init(void);
void a2dp_snk_service_deinit(void);
void a2dp_snk_handle_conn_event(uint32_t conn_hdl, uint16_t event, uint8_t *arg);
void a2dp_snk_handle_acl_disconnect(uint32_t dev_hdl);

/* 获取活跃的stream hdl */
uint32_t a2dp_snk_get_active_stream_hdl(void);

/* 获取活跃连接设备handle */
uint32_t a2dp_snk_get_active_dev_hdl(uint32_t stream_hdl);

bd_addr_t a2dp_snk_get_active_device(void);

/*
 * 功能：设置A2DP的CODEC
 * 注意：
 *  1、在A2DP未连接情况下，可直接设置；
 *  2、在A2DP已连接情况下，分两种场景：
 *      1）若为不同大类codec切换：需要在stream CLOSE的状态下设置，然后在OPEN时进行重新协商，并不保证重协商结果与设置完全一致。
 *      2）若为相同大类codec切换：需要在stream OPEN的状态下设置，即若处于START状态需要先SUSPEND再调本接口。
 */
uint32_t a2dp_snk_set_stream_codec(bt_a2dp_codec_param *codec);

/* 获取A2DP的CODEC */
uint32_t a2dp_snk_get_stream_codec(bt_a2dp_codec_param *codec);

uint8_t a2dp_snk_get_stream_codec_type(void);

uint8_t a2dp_snk_get_stream_status(void);

void a2dp_snk_register_callback(void);

#endif /* __BT_A2DP_SNK_SERVICE_H__ */
