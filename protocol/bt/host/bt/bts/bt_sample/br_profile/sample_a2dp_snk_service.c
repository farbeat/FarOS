/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT A2DP SNK Service module.
 */

#include "sample_a2dp_snk_service.h"
#include <soc_osal.h>
#include <stdlib.h>
#include <stdint.h>
#include <td_base.h>
#include <securec.h>
#include "bts_a2dp_sink.h"
#include "sample_common.h"
#include "bts_br_gap.h"
#include "bts_avrcp_controller.h"
#include "bt_audio_hal_interface.h"

#define MEDIA_A2DP_SNK_INTERFACE 1

osal_timer g_a2dp_stream_accept_timer = {0};
#define A2DP_STREAM_ACCEPT_TIMER_PERIOD 500
static bool g_a2dp_snk_active_disconnect = false;

static void sample_a2dp_snk_accept_timer_stop(void)
{
    bts_sample_log("stop timer\n");
    if (g_a2dp_stream_accept_timer.timer != NULL) {
        (void)osal_timer_stop(&g_a2dp_stream_accept_timer);
        (void)osal_timer_destroy(&g_a2dp_stream_accept_timer);
        g_a2dp_stream_accept_timer.timer = NULL;
    }
    bts_sample_log("stop timer success!\n");
}

static void sample_a2dp_snk_start_accept_handler(unsigned long data)
{
    bts_unused(data);
    bd_addr_t active_bd_addr = a2dp_snk_get_active_device();
    a2dp_snk_start_accept(&active_bd_addr);
    sample_a2dp_snk_accept_timer_stop();
}

static int32_t sample_a2dp_snk_start_accept_timer(void)
{
    bts_sample_log("start timer\n");
    if (g_a2dp_stream_accept_timer.timer != NULL) {
        bts_sample_log("timer is running");
        return EXT_ERR_SUCCESS;
    }

    g_a2dp_stream_accept_timer.handler = sample_a2dp_snk_start_accept_handler;
    g_a2dp_stream_accept_timer.interval = A2DP_STREAM_ACCEPT_TIMER_PERIOD;

    int32_t timer_ret = osal_timer_init(&g_a2dp_stream_accept_timer);
    if (timer_ret != EOK) {
        bts_sample_log("create timer error! ret: %x\n", timer_ret);
        sample_a2dp_snk_accept_timer_stop();
        return EXT_ERR_FAILURE;
    }
    timer_ret = osal_timer_start(&g_a2dp_stream_accept_timer);
    if (timer_ret != EOK) {
        bts_sample_log("start timer error! ret: %x\n", timer_ret);
        sample_a2dp_snk_accept_timer_stop();
        return EXT_ERR_FAILURE;
    }
    return EXT_ERR_SUCCESS;
}
#if MEDIA_A2DP_SNK_INTERFACE
#include "audio_manager_c_wrapper.h"
AudioFormatConfig g_audio_format_config = {0};
#endif
static int32_t sample_a2dp_snk_audio_create(void)
{
#if MEDIA_A2DP_SNK_INTERFACE
    bts_sample_log("audioFormat %u channelCount %u sampleRate %u \n",
        g_audio_format_config.audioFormat, g_audio_format_config.channelCount, g_audio_format_config.sampleRate);

    int32_t ret = AudioManagerA2dpSinkSetParam(g_audio_format_config);
    bts_sample_log("AudioManagerA2dpSinkSetParam ret %x\n", ret);
    ret = AudioManagerA2dpSinkCreate();
    bts_sample_log("AudioManagerA2dpSinkCreate ret %x\n", ret);
#endif
    /* 起定时器接受音乐开始请求 */
    sample_a2dp_snk_start_accept_timer();
    return 0;
}

static int32_t sample_a2dp_snk_audio_destroy(void)
{
#if MEDIA_A2DP_SNK_INTERFACE
    int32_t ret = AudioManagerA2dpSinkDestroy();
    bts_sample_log("AudioManagerA2dpSinkDestroy ret %x\n", ret);
#endif
    return 0;
}

static void sample_a2dp_snk_connect_state_changed_callback(bd_addr_t *bd_addr, int conn_state)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x conn_state: %x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], conn_state); /* 3 2 1 0 idx */
    /* 主动断开service，断开事件上报后才能去注册server否则状态无法上报 */
    if (g_a2dp_snk_active_disconnect == true && conn_state == PROFILE_STATE_DISCONNECTED) {
        a2dp_snk_service_unregister();
        g_a2dp_snk_active_disconnect = false;
    }
}

static void sample_a2dp_snk_start_stream_request_callback(bd_addr_t *bd_addr)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x \n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* 3 2 1 0 idx */
    sample_a2dp_snk_audio_create();
}

static void sample_a2dp_snk_playing_state_changed_callback(bd_addr_t *bd_addr, int playing_state)
{
    bts_sample_log("addr: ****%02x%02x%02x%02x playing_state: %x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], playing_state); /* 3 2 1 0 idx */
    if (playing_state == A2DP_NOT_PLAYING) {
        sample_a2dp_snk_audio_destroy();
    }
}

static void sample_a2dp_snk_configuration_changed_callback(bd_addr_t *bd_addr, a2dp_snk_codec_info_t *info)
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
#if MEDIA_A2DP_SNK_INTERFACE
    if (info->codec_type == A2DP_SNK_CODEC_TYPE_AAC) {
        g_audio_format_config.audioFormat = AAC_LC;
    } else {
        g_audio_format_config.audioFormat = SBC;
    }
    /* channel count 1 or 2 */
    g_audio_format_config.channelCount = (info->channel_mode == A2DP_SNK_CODEC_CHANNEL_MODE_NONE ? 1 : 2);

    switch (info->sample_rate) {
        case A2DP_SNK_CODEC_SAMPLE_RATE_48000:
            g_audio_format_config.sampleRate = BT_AUDIO_A2DP_SMAPLE_RATE48000;
            break;
        case A2DP_SNK_CODEC_SAMPLE_RATE_88200:
            g_audio_format_config.sampleRate = BT_AUDIO_A2DP_SMAPLE_RATE88200;
            break;
        case A2DP_SNK_CODEC_SAMPLE_RATE_96000:
            g_audio_format_config.sampleRate = BT_AUDIO_A2DP_SMAPLE_RATE96000;
            break;
        default:
            g_audio_format_config.sampleRate = BT_AUDIO_A2DP_SMAPLE_RATE44100;
            break;
    }
    bts_sample_log("audioFormat %u channelCount %u sampleRate %u \n",
        g_audio_format_config.audioFormat, g_audio_format_config.channelCount, g_audio_format_config.sampleRate);
#endif
}

a2dp_snk_callbacks_t g_sample_a2dp_snk_callbacks = {
    sample_a2dp_snk_connect_state_changed_callback,
    sample_a2dp_snk_start_stream_request_callback,
    sample_a2dp_snk_playing_state_changed_callback,
    sample_a2dp_snk_configuration_changed_callback
};

/* wstp注册回调 */
int wstp_a2dp_snk_register_callbacks(void)
{
    int ret = a2dp_snk_register_callbacks(&g_sample_a2dp_snk_callbacks);
    bts_sample_log("a2dp reg cbk ret %x\n", ret);
    return ret;
}

/* 取消回调注册 */
int wstp_a2dp_snk_deregister_callbacks(void)
{
    int ret = a2dp_snk_deregister_callbacks(&g_sample_a2dp_snk_callbacks);
    bts_sample_log("ret(int): %x\n", ret);
    return ret;
}

/* 获取设备连接状态 */
int wstp_a2dp_snk_get_device_connect_state(const bd_addr_t *bd_addr)
{
    int ret = a2dp_snk_get_device_connect_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

/* 获取设备播放状态 */
int wstp_a2dp_snk_get_playing_state(const bd_addr_t *bd_addr)
{
    int ret = a2dp_snk_get_playing_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

/* 与设备建立A2DP连接 */
int wstp_a2dp_snk_connect(const bd_addr_t *bd_addr)
{
    /* 连接service前要注册server否则状态无法上报 */
    a2dp_snk_service_register();
    int ret = a2dp_snk_connect(bd_addr);
    bts_sample_log("a2dp_snk_connect ret(int): %x\n", ret);
    return ret;
}

/* 与设备断开A2DP连接 */
int wstp_a2dp_snk_disconnect(const bd_addr_t *bd_addr)
{
    if (a2dp_snk_get_device_connect_state(bd_addr) != PROFILE_STATE_CONNECTED) {
        bts_sample_log("a2dp_snk_disconnect not connected\n");
        return EXT_ERR_FAILURE;
    }
    g_a2dp_snk_active_disconnect = true;
    int ret = a2dp_snk_disconnect(bd_addr);
    bts_sample_log("a2dp_snk_disconnect ret(int): %x\n", ret);
    return ret;
}

/* 获取活跃设备 */
void wstp_a2dp_snk_get_active_device(void)
{
    bd_addr_t bd_addr = a2dp_snk_get_active_device();

    bts_sample_log("a2dp_snk_get_active_device addr: ****%02x%02x%02x%02x \n",
        bd_addr.addr[3], bd_addr.addr[2], bd_addr.addr[1], bd_addr.addr[0]); /* 3 2 1 0 idx */
}
