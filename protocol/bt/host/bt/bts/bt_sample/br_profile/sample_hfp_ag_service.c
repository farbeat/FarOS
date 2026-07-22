/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT HFP Service module.
 */

#include "sample_hfp_ag_service.h"
#include <soc_osal.h>
#include <td_base.h>
#include <stdlib.h>
#include "securec.h"
#include "errcode.h"
#include "bts_br_gap.h"
#include "bts_hfp_ag.h"
#include "sample_common.h"
#ifdef MEMORY_MINI
#include "audio_manager.h"
#else
#include "bt_audio_manager_wrapper.h"
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#define SAMPLE_HFP_TASK_STACK_SIZE 0x1600
#define SAMPLE_HFP_TASK_PRI 20
#define HFP_MEDIA_STATUS_DEINIT 0
#define HFP_MEDIA_STATUS_INIT 1
#define HFP_MEDIA_STATUS_START 2
#define HFP_MEDIA_FLAG_IDLE 0
#define HFP_MEDIA_FLAG_BUSY 1

#define HFP_AG_IND_SIGNAL_DEFAULT 3
#define HFP_AG_IND_BATTERY_DEFAULT 5

typedef enum {
    SAMPLE_HFP_AG_CMD_INIT = 0,                          /* AG初始化 */
    SAMPLE_HFP_AG_CMD_DEINIT,                            /* AG去初始化 */
    SAMPLE_HFP_AG_CMD_REG_CALLBACK,                      /* 注册回调 */
    SAMPLE_HFP_AG_CMD_CONN_HFP,                          /* HFP连接 */
    SAMPLE_HFP_AG_CMD_DISCONN_HFP,                       /* HFP断开连接 */
    SAMPLE_HFP_AG_CMD_CONN_SCO = 5,                      /* 建立SCO连接 */
    SAMPLE_HFP_AG_CMD_DISCONN_SCO,                       /* 断开SCO连接 */
    SAMPLE_HFP_AG_CMD_GET_DEV_CONN_STATE,                /* 获取设备Profile连接状态 */
    SAMPLE_HFP_AG_CMD_GET_SCO_CONN_STATE,                /* 获取SCO连接状态 */
    SAMPLE_HFP_AG_CMD_ANSWER_CALL,                       /* 接听电话 */
    SAMPLE_HFP_AG_CMD_FINISH_CALL = 10,                  /* 挂断电话 */
    SAMPLE_HFP_AG_CMD_SET_VOLUME,                        /* 设置扬声器/麦克风音量 */
    SAMPLE_HFP_AG_CMD_SEND_INCOMING_EVENT,               /* 发送来电事件 */
    SAMPLE_HFP_AG_CMD_SEND_NETWORK_EVENT,                /* 发送网络事件 */
    SAMPLE_HFP_AG_CMD_SEND_CALL_LIST,                    /* 发送通话列表 */
    SAMPLE_HFP_AG_CMD_SEND_SUBSCRIBER_NUMBER = 15,       /* 发送订阅号码信息 */
    SAMPLE_HFP_AG_CMD_SEND_BATTERY_CHARGE,               /* 发送电量信息 */
    SAMPLE_HFP_AG_CMD_SEND_CMEE_ERROR,                   /* 发送扩展错误码信息 */
    SAMPLE_HFP_AG_CMD_SEND_NETWORK_OPERATOR,              /* 发送运营商信息 */
    SAMPLE_HFP_AG_CMD_SEND_INDICATOR_VAL,                 /* 发送指示器值 */
    SAMPLE_HFP_AG_CMD_ORIGINATE_CALL,                     /* 呼出电话 */
} sample_hfp_ag_cmd_id;

typedef void (*hfp_ag_sample_cmd_handle_func)(const char **params, int32_t params_cnt);

typedef struct {
    sample_hfp_ag_cmd_id id;
    hfp_ag_sample_cmd_handle_func func;
} sample_hfp_ag_hdl_info;

static hfp_cind_info_t g_sample_hfp_cind_info = {0};

#ifdef MEMORY_MINI
static struct AudioManager *g_audio_manager = NULL;
static struct AudioAdapter *g_primary_adapter = NULL;
static struct AudioAdapter *g_bluetooth_adapter = NULL;
#endif

static void sample_hfp_ag_print_addr(const bd_addr_t *bd_addr)
{
    if (bd_addr != NULL) {
        bts_sample_log("addr: %02x:%02x:**:**:**:%02x ",
            bd_addr->addr[5], bd_addr->addr[4], bd_addr->addr[0]); /* addr 只打印0,4,5位 */
    }
}

static int32_t g_hfp_media_status = 0;
static int32_t g_hfp_media_flag = 0;

#ifdef MEMORY_MINI
#define MAX_AUDIO_ADAPTER_NUM 2
#define MAX_KEY_VALUE_LEN 25

static enum AudioFormat g_hfp_codec = AUDIO_FORMAT_mSBC;

static const int32_t g_audio_channel_1 = 1;
static const int32_t g_audio_sample_rate_8k = 8000;
static const int32_t g_audio_sample_rate_16k = 16000;

static struct AudioCapture *g_audio_capture_up_link = NULL; // mic
static struct AudioCapture *g_audio_capture_down_link = NULL; // sco in
static struct AudioRender *g_audio_render_up_link = NULL; // sco out
static struct AudioRender *g_audio_render_down_link = NULL; // speaker
#else
static AudioCodecFormat g_hfp_codec = mSBC;
static int32_t g_hdl_up_mic = -1;
static int32_t g_hdl_up_sco = -1;
static int32_t g_hdl_dn_spk = -1;
static int32_t g_hdl_dn_sco = -1;
static int32_t g_hdl_up_modem = -1;
static int32_t g_hdl_dn_modem = -1;
#endif

#ifdef MEMORY_MINI
static void audio_sample_attr_init(enum AudioFormat audio_format, struct AudioSampleAttributes *param)
{
    param->type = AUDIO_IN_COMMUNICATION;
    if (audio_format == AUDIO_FORMAT_PCM_16_BIT) {
        param->sampleRate = (uint32_t)g_audio_sample_rate_8k;
    } else if (audio_format == AUDIO_FORMAT_mSBC) {
        param->sampleRate = (uint32_t)g_audio_sample_rate_16k;
    }
    param->format = audio_format;
    param->channelCount = (uint32_t)g_audio_channel_1;
    param->interleaved = false;
}

static int32_t audio_capture_init(bool up_link, enum AudioFormat audio_format)
{
    struct AudioSampleAttributes param = {};
    audio_sample_attr_init(audio_format, &param);

    struct AudioDeviceDescriptor device_desc = {};
    device_desc.portId = 0;
    device_desc.desc = NULL;

    int32_t ret;
    if (up_link) {
        device_desc.pins = PIN_IN_MIC;
        ret = g_primary_adapter->CreateCapture(g_primary_adapter, &device_desc, &param, &g_audio_capture_up_link);
        if (ret != 0 || g_audio_capture_up_link == NULL) {
            bts_sample_log("{%s():%d} create uplink audio capture failed\n", __FUNCTION__, __LINE__);
            return ret;
        }
    } else {
        device_desc.pins = PIN_IN_BT_SCO;
        ret = g_bluetooth_adapter->CreateCapture(g_bluetooth_adapter, &device_desc, &param, &g_audio_capture_down_link);
        if (ret != 0 || g_audio_capture_down_link == NULL) {
            bts_sample_log("{%s():%d} create downlink audio capture failed\n", __FUNCTION__, __LINE__);
            return ret;
        }
    }

    return 0;
}

static int32_t audio_render_init(bool up_link, enum AudioFormat audio_format)
{
    struct AudioSampleAttributes param = {};
    audio_sample_attr_init(audio_format, &param);

    struct AudioDeviceDescriptor device_desc = {};
    device_desc.portId = 0;
    device_desc.desc = NULL;

    int32_t ret;
    if (up_link) {
        device_desc.pins = PIN_OUT_BT_SCO;
        ret = g_bluetooth_adapter->CreateRender(g_bluetooth_adapter, &device_desc, &param, &g_audio_render_up_link);
        if (ret != 0 || g_audio_render_up_link == NULL) {
            bts_sample_log("{%s():%d} create uplink audio render failed\n", __FUNCTION__, __LINE__);
            return ret;
        }
    } else {
        device_desc.pins = PIN_OUT_SPEAKER;
        ret = g_primary_adapter->CreateRender(g_primary_adapter, &device_desc, &param, &g_audio_render_down_link);
        if (ret != 0 || g_audio_render_down_link == NULL) {
            bts_sample_log("{%s():%d} create downlink audio render failed\n", __FUNCTION__, __LINE__);
            return ret;
        }
    }

    return 0;
}

static int32_t audio_capture_render_init(void)
{
    if (g_primary_adapter == NULL || g_bluetooth_adapter == NULL) {
        bts_sample_log("{%s():%d} audio adapter is null\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int32_t ret = audio_capture_init(true, g_hfp_codec);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink capture init failed\n", __FUNCTION__, __LINE__);
        return ret;
    }
    ret = audio_render_init(true, g_hfp_codec);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink render init failed\n", __FUNCTION__, __LINE__);
        (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
        return ret;
    }
    ret = audio_capture_init(false, g_hfp_codec);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink capture init failed\n", __FUNCTION__, __LINE__);
        (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
        (void)g_bluetooth_adapter->DestroyRender(g_bluetooth_adapter, g_audio_render_up_link);
        return ret;
    }
    ret = audio_render_init(false, g_hfp_codec);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink render init failed\n", __FUNCTION__, __LINE__);
        (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
        (void)g_bluetooth_adapter->DestroyRender(g_bluetooth_adapter, g_audio_render_up_link);
        (void)g_bluetooth_adapter->DestroyCapture(g_bluetooth_adapter, g_audio_capture_down_link);
        return ret;
    }

    return 0;
}

static int32_t audio_capture_render_start(void)
{
    int32_t ret = g_audio_capture_up_link->control.Start((AudioHandle)g_audio_capture_up_link);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink capture start failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = g_audio_render_up_link->control.Start((AudioHandle)g_audio_render_up_link);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink render start failed\n", __FUNCTION__, __LINE__);
        (void)g_audio_capture_up_link->control.Stop((AudioHandle)g_audio_capture_up_link);
        return ret;
    }

    ret = g_audio_capture_down_link->control.Start((AudioHandle)g_audio_capture_down_link);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink capture start failed\n", __FUNCTION__, __LINE__);
        (void)g_audio_capture_up_link->control.Stop((AudioHandle)g_audio_capture_up_link);
        (void)g_audio_render_up_link->control.Stop((AudioHandle)g_audio_render_up_link);
        return ret;
    }

    ret = g_audio_render_down_link->control.Start((AudioHandle)g_audio_render_down_link);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink render start failed\n", __FUNCTION__, __LINE__);
        (void)g_audio_capture_up_link->control.Stop((AudioHandle)g_audio_capture_up_link);
        (void)g_audio_render_up_link->control.Stop((AudioHandle)g_audio_render_up_link);
        (void)g_audio_capture_down_link->control.Stop((AudioHandle)g_audio_capture_down_link);
        return ret;
    }

    return 0;
}

static int32_t establish_audio_capture_render_pipeline(void)
{
    uint32_t mic_shm_id = 0;
    uint32_t spk_shm_id = 0;

    int32_t ret = g_audio_capture_up_link->attr.GetCurrentChannelId((AudioHandle)g_audio_capture_up_link, &mic_shm_id);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink capture get channel id failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    char key_value_pair_mic[MAX_KEY_VALUE_LEN] = { 0 };
    ret = sprintf_s(key_value_pair_mic, sizeof(key_value_pair_mic), "%s = %u", "attachId", mic_shm_id);
    if (ret == -1) {
        bts_sample_log("{%s():%d} sprintf_s failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = g_audio_render_up_link->attr.SetExtraParams((AudioHandle)g_audio_render_up_link, key_value_pair_mic);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio uplink render set param failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = g_audio_render_down_link->attr.GetCurrentChannelId((AudioHandle)g_audio_render_down_link, &spk_shm_id);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink render get channel id failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    char key_value_pair_spk[MAX_KEY_VALUE_LEN] = { 0 };
    ret = sprintf_s(key_value_pair_spk, sizeof(key_value_pair_spk), "%s = %u", "attachId", spk_shm_id);
    if (ret == -1) {
        bts_sample_log("{%s():%d} sprintf_s failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = g_audio_capture_down_link->attr.SetExtraParams((AudioHandle)g_audio_capture_down_link, key_value_pair_spk);
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio downlink capture set param failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    bts_sample_log("{%s():%d} mic_shm_id: 0x%x, spk_shm_id: 0x%x\n", __FUNCTION__, __LINE__, mic_shm_id, spk_shm_id);

    ret = audio_capture_render_start();
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio capture render start failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    return 0;
}

static int32_t abrogation_audio_capture_render_pipeline(void)
{
    if (g_audio_capture_up_link != NULL) {
        (void)g_audio_capture_up_link->control.Stop((AudioHandle)g_audio_capture_up_link);
        if (g_primary_adapter != NULL) {
            (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
            g_audio_capture_up_link = NULL;
        }
    }

    if (g_audio_render_up_link != NULL) {
        (void)g_audio_render_up_link->control.Stop((AudioHandle)g_audio_render_up_link);
        if (g_bluetooth_adapter != NULL) {
            (void)g_bluetooth_adapter->DestroyRender(g_bluetooth_adapter, g_audio_render_up_link);
            g_audio_render_up_link = NULL;
        }
    }

    if (g_audio_capture_down_link != NULL) {
        (void)g_audio_capture_down_link->control.Stop((AudioHandle)g_audio_capture_down_link);
        if (g_bluetooth_adapter != NULL) {
            (void)g_bluetooth_adapter->DestroyCapture(g_bluetooth_adapter, g_audio_capture_down_link);
            g_audio_capture_down_link = NULL;
        }
    }

    if (g_audio_render_down_link != NULL) {
        (void)g_audio_render_down_link->control.Stop((AudioHandle)g_audio_render_down_link);
        if (g_primary_adapter != NULL) {
            (void)g_primary_adapter->DestroyRender(g_primary_adapter, g_audio_render_down_link);
        }
    }

    return 0;
}
#endif

#ifndef MEMORY_MINI
static int32_t audio_stream_init(void)
{
    /* 请求音频焦点 */
    /* deactive响铃时申请的焦点后，重新申请通话焦点 */
    audio_manager_activate_interrupt(AUDIO_STREAM_VOICE_CALL_VOLTE);
    audio_manager_set_device_connection_state(IN_BLUETOOTH_SCO, AUDIO_DEVICE_CONNECTED);
    audio_manager_set_device_connection_state(OUT_BLUETOOTH_SCO, AUDIO_DEVICE_CONNECTED);
    audio_manager_set_device_connection_state(IN_MODEM_HEADSET, AUDIO_DEVICE_CONNECTED);
    audio_manager_set_device_connection_state(OUT_MODEM_HEADSET, AUDIO_DEVICE_CONNECTED);
    g_hdl_up_sco = audio_stream_in_init(AUDIO_UP_LINK, g_hfp_codec);
    g_hdl_up_modem = audio_stream_out_init(AUDIO_UP_LINK, g_hfp_codec);
    g_hdl_dn_sco = audio_stream_out_init(AUDIO_DOWN_LINK, g_hfp_codec);
    g_hdl_dn_modem = audio_stream_in_init(AUDIO_DOWN_LINK, g_hfp_codec);

    return 0;
}

static int32_t establish_audio_stream_pipeline(void)
{
    uint32_t modem_out_shm_id = 0;
    uint32_t modem_in_shm_id = 0;

    audio_stream_in_start(g_hdl_dn_modem);
    audio_stream_out_start(g_hdl_dn_sco);
    audio_stream_in_get_channel_id(g_hdl_dn_modem, &modem_in_shm_id);
    audio_stream_out_attach_frontend(g_hdl_dn_sco, modem_in_shm_id);
    audio_stream_in_start(g_hdl_up_sco);
    audio_stream_out_start(g_hdl_up_modem);
    audio_stream_out_get_channel_id(g_hdl_up_modem, &modem_out_shm_id);
    audio_stream_in_attach_backend(g_hdl_up_sco, modem_out_shm_id);

    return 0;
}

static int32_t abrogation_audio_stream_pipeline(void)
{
    /* 销毁上下行通路 */
    audio_stream_in_stop(g_hdl_up_sco);
    audio_stream_in_deinit(g_hdl_up_sco);
    audio_stream_out_stop(g_hdl_up_modem);
    audio_stream_out_deinit(g_hdl_up_modem);
    audio_stream_out_stop(g_hdl_dn_sco);
    audio_stream_out_deinit(g_hdl_dn_sco);
    audio_stream_in_stop(g_hdl_dn_modem);
    audio_stream_in_deinit(g_hdl_dn_modem);
    audio_manager_set_device_connection_state(IN_BLUETOOTH_SCO, AUDIO_DEVICE_DISCONNECTED);
    audio_manager_set_device_connection_state(OUT_BLUETOOTH_SCO, AUDIO_DEVICE_DISCONNECTED);
    audio_manager_set_device_connection_state(IN_MODEM_HEADSET, AUDIO_DEVICE_DISCONNECTED);
    audio_manager_set_device_connection_state(OUT_MODEM_HEADSET, AUDIO_DEVICE_DISCONNECTED);
    /* 释放音频焦点 */
    audio_manager_deactivate_interrupt();
    audio_manager_deinit();

    return 0;
}
#endif

static int32_t sample_audio_init_start_task_body(void *data)
{
    bts_unused(data);
    bts_sample_log("sample_audio_init_start enter\n");
    while ((g_hfp_media_flag != HFP_MEDIA_FLAG_IDLE) || (g_hfp_media_status != HFP_MEDIA_STATUS_DEINIT)) {
        osal_msleep(1);
    }
    /* 设置媒体状态繁忙 */
    g_hfp_media_flag = HFP_MEDIA_FLAG_BUSY;

    int ret;
#ifdef MEMORY_MINI
    ret = audio_capture_render_init();
    if (ret != 0) {
        bts_sample_log("{%s():%d} audio capture render init failed\n", __FUNCTION__, __LINE__);
        return ret;
    }
#else
    audio_stream_init();
#endif

    /* Init success */
    g_hfp_media_status = HFP_MEDIA_STATUS_INIT;

    /* 建立电话上下行通路 */
#ifdef MEMORY_MINI
    ret = establish_audio_capture_render_pipeline();
    if (ret != 0) {
        bts_sample_log("{%s():%d} establish audio capture render pipeline failed\n", __FUNCTION__, __LINE__);
        (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
        (void)g_bluetooth_adapter->DestroyRender(g_bluetooth_adapter, g_audio_render_up_link);
        (void)g_bluetooth_adapter->DestroyCapture(g_bluetooth_adapter, g_audio_capture_down_link);
        (void)g_primary_adapter->DestroyRender(g_primary_adapter, g_audio_render_down_link);
        return ret;
    }
#else
    establish_audio_stream_pipeline();
#endif

    bts_sample_log("sample_audio_init_start end\n");
    /* Start success */
    g_hfp_media_status = HFP_MEDIA_STATUS_START;
    g_hfp_media_flag = HFP_MEDIA_FLAG_IDLE;
    return 0;
}

static int32_t sample_audio_deinit_task_body(void *data)
{
    bts_unused(data);
    bts_sample_log("sample_audio_deinit enter\n");
    while ((g_hfp_media_flag != HFP_MEDIA_FLAG_IDLE) || (g_hfp_media_status != HFP_MEDIA_STATUS_START)) {
        osal_msleep(1);
    }
    bts_sample_log("sample_audio_deinit start\n");
    /* 设置媒体状态繁忙 */
    g_hfp_media_flag = HFP_MEDIA_FLAG_BUSY;

#ifdef MEMORY_MINI
    abrogation_audio_capture_render_pipeline();
#else
    abrogation_audio_stream_pipeline();
#endif

    bts_sample_log("sample_audio_deinit end\n");
    /* Deinit success */
    g_hfp_media_status = HFP_MEDIA_STATUS_DEINIT;
    g_hfp_media_flag = HFP_MEDIA_FLAG_IDLE;
    return 0;
}

void sample_hfp_ag_set_spk_volume(uint8_t volume)
{
#ifdef MEMORY_MINI
    /* vol range [0, 1.0] */
    float vol = (float)volume / SAMPLE_HFP_PROF_VOL_LEVEL;
    if (g_audio_render_down_link != NULL) {
        g_audio_render_down_link->volume.SetVolume((AudioHandle)g_audio_render_down_link, vol);
    } else {
        bts_sample_log("set volume: %u failed!\n", volume);
    }
#else
    /* vol range [0, 100] */
    int32_t vol = volume * SAMPLE_HFP_MEDIA_VOL_LEVEL / SAMPLE_HFP_PROF_VOL_LEVEL;
    audio_manager_set_volume(AUDIO_STREAM_VOICE_CALL_VOLTE, vol);
#endif
}

static void sample_hfp_ag_conn_state_changed_callback(const bd_addr_t *bd_addr, profile_connect_state_t state)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("state: %d\n", state);
}

static void sample_hfp_ag_sco_conn_state_changed_callback(const bd_addr_t *bd_addr, hfp_sco_connect_state_t state)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("state: %d\n", state);
    osal_kthread_handler task_body = NULL;
    switch (state) {
        case HFP_SCO_STATE_CONNECTING:
            break;

        case HFP_SCO_STATE_CONNECTED:
            task_body = sample_audio_init_start_task_body;
            break;

        case HFP_SCO_STATE_DISCONNECTING:
            break;

        case HFP_SCO_STATE_DISCONNECTED:
            task_body = sample_audio_deinit_task_body;
            break;

        default: {
            bts_sample_log("invalid state!\n");
            break;
        }
    }
    if (task_body == NULL) {
        bts_sample_log("no task body!\n");
        return;
    }

    osal_task *task = osal_kthread_create(task_body, NULL, "sample_hfp_audio_task", SAMPLE_HFP_TASK_STACK_SIZE);
    if (task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    int ret = osal_kthread_set_priority(task, SAMPLE_HFP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }
}

static void sample_hfp_ag_codec_changed_callback(const bd_addr_t *bd_addr, hfp_ag_codec_id_t codec)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("codec : %d\n", codec);
    if (codec == HFP_AG_CODEC_ID_CVSD) {
#ifdef MEMORY_MINI
        g_hfp_codec = AUDIO_FORMAT_PCM_16_BIT;
#else
        g_hfp_codec = PCM;
#endif
    } else if (codec == HFP_AG_CODEC_ID_MSBC) {
#ifdef MEMORY_MINI
        g_hfp_codec = AUDIO_FORMAT_mSBC;
#else
        g_hfp_codec = mSBC;
#endif
    }
}

static void sample_hfp_ag_volume_changed_callback(const bd_addr_t *bd_addr, hfp_volume_type_t type, uint8_t volume)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("type: %d, volume: %u\n", type, volume);
    if (type == HFP_VOLUME_SPEAKER) {
        sample_hfp_ag_set_spk_volume(volume);
    }
}

static void sample_hfp_ag_dial_req_callback(const bd_addr_t *bd_addr, unsigned char *number, unsigned int len)
{
    sample_hfp_ag_print_addr(bd_addr);
    for (unsigned int i = 0; i < len; i++) {
        bts_sample_log("number: %d ", *((uint8_t *)number + i));
    }
    bts_sample_log("len: %d.\n", len);
}

static void sample_hfp_ag_mem_dial_req_callback(const bd_addr_t *bd_addr, unsigned int index)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("index: %d.\n", index);
}

static void sample_hfp_ag_lastnum_dial_req_callback(const bd_addr_t *bd_addr)
{
    sample_hfp_ag_print_addr(bd_addr);
}

static void sample_hfp_ag_answer_call_req_callback(const bd_addr_t *bd_addr, hfp_ag_type_t type)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("type: %d.\n", type);
}

static void sample_hfp_ag_cancel_call_req_callback(const bd_addr_t *bd_addr, hfp_ag_type_t type)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("type: %d.\n", type);
}

static void sample_hfp_ag_dtmf_req_callback(const bd_addr_t *bd_addr, unsigned int code)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("code: 0x%x.\n", code);
}

static void sample_hfp_ag_subscriber_number_req_callback(const bd_addr_t *bd_addr)
{
    sample_hfp_ag_print_addr(bd_addr);
    /* TODO 造数据响应 */
}

static void sample_hfp_ag_current_calls_req_callback(const bd_addr_t *bd_addr)
{
    sample_hfp_ag_print_addr(bd_addr);
    /* TODO 造数据响应 */
}

static void sample_hfp_ag_current_indicator_req_callback(const bd_addr_t *bd_addr)
{
    g_sample_hfp_cind_info.service = 1;
    g_sample_hfp_cind_info.call = 0;
    g_sample_hfp_cind_info.callsetup = 0;
    g_sample_hfp_cind_info.callheld = 0;
    g_sample_hfp_cind_info.signal = HFP_AG_IND_SIGNAL_DEFAULT;
    g_sample_hfp_cind_info.roam = 0;
    g_sample_hfp_cind_info.battchg = HFP_AG_IND_BATTERY_DEFAULT;
    errcode_t ret = (errcode_t)hfp_ag_send_indicator_val(bd_addr, &g_sample_hfp_cind_info);
    bts_sample_log("sample hfp ag send cur indicator cbk ret=0x%x\n", ret);
}

static void sample_hfp_ag_network_operator_format_req_callback(const bd_addr_t *bd_addr)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("sample hfp ag AT+COPS= req cbk.\n");
}

static void sample_hfp_ag_network_operator_req_callback(const bd_addr_t *bd_addr)
{
    sample_hfp_ag_print_addr(bd_addr);
    bts_sample_log("sample hfp ag AT+COPS? req cbk.\n");
}

static void sample_hfp_ag_reg_callback(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    errcode_t ret = ERRCODE_BT_SUCCESS;
    hfp_ag_callbacks_t callbacks = {
        sample_hfp_ag_conn_state_changed_callback,
        sample_hfp_ag_sco_conn_state_changed_callback,
        sample_hfp_ag_codec_changed_callback,
        sample_hfp_ag_volume_changed_callback,
        sample_hfp_ag_dial_req_callback,
        sample_hfp_ag_mem_dial_req_callback,
        sample_hfp_ag_lastnum_dial_req_callback,
        sample_hfp_ag_answer_call_req_callback,
        sample_hfp_ag_cancel_call_req_callback,
        sample_hfp_ag_dtmf_req_callback,
        sample_hfp_ag_subscriber_number_req_callback,
        sample_hfp_ag_current_calls_req_callback,
        sample_hfp_ag_current_indicator_req_callback,
        sample_hfp_ag_network_operator_format_req_callback,
        sample_hfp_ag_network_operator_req_callback,
    };
    ret = (errcode_t)hfp_ag_register_callbacks(&callbacks);
    bts_sample_log("sample hfp ag reg cbk ret=0x%x\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_conn_hfp(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag conn hfp param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag conn hfp decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_connect(&bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_disconn_hfp(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag disconn hfp param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag disconn hfp decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_disconnect(&bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_conn_sco(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag conn sco param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag conn sco decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_audio_connect(&bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_disconn_sco(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag disconn sco param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag disconn sco decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_audio_disconnect(&bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_get_dev_conn_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag dev conn state param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag dev conn state decode addr error!\n");
        return;
    }
    int ret = hfp_ag_get_device_connect_state(&bd_addr);
    bts_sample_log("ret: %d\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_get_sco_conn_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag sco state param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag sco state decode addr error!\n");
        return;
    }
    int ret = hfp_ag_get_sco_connect_state(&bd_addr);
    bts_sample_log("ret: %d\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_answer_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag answer call param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag answer call decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_send_answer_call_event(&bd_addr);
    bts_sample_log("ret: %d\n", ret);
}

/* <addr> <type> */
static void sample_hfp_ag_cmd_finish_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2: param count */
        bts_sample_log("sample hfp ag finish call param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag finish call decode addr error!\n");
        return;
    }
    hfp_ag_action_t type = (hfp_ag_action_t)atoi(params[1]);
    errcode_t ret = (errcode_t)hfp_ag_send_finish_call_event(&bd_addr, type);
    bts_sample_log("type: %d ret: %d\n", type, ret);
}

/* <addr> <volume_type> <volume> */
static void sample_hfp_ag_cmd_set_volume(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag set volume decode addr error!\n");
        return;
    }
    hfp_volume_type_t type = (hfp_volume_type_t)atoi(params[1]); /* 第1个参数 */
    uint8_t volume = (uint8_t)atoi(params[2]); /* 第2个参数 */
    errcode_t ret = (errcode_t)hfp_ag_set_volume(&bd_addr, type, volume);
    bts_sample_log("type: %d volume: %u ret: %u\n", type, volume, ret);
}

/* <addr> <event> <param> */
static void sample_hfp_ag_cmd_send_network_event(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag set volume decode addr error!\n");
        return;
    }
    hfp_ag_network_event_t event = (hfp_ag_network_event_t)atoi(params[1]); /* 第1个参数 */
    uint8_t param = (uint8_t)atoi(params[2]); /* 第2个参数 */
    errcode_t ret = (errcode_t)hfp_ag_send_network_event(&bd_addr, event, param);
    bts_sample_log("event: %d param: %u ret: %u\n", event, param, ret);
}

/* <addr> <type> <service> <number> <alpha_str> */
static void sample_hfp_ag_cmd_send_incoming_event(const char **params, int32_t params_cnt)
{
    if (params_cnt != 5) { /* 5个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag set volume decode addr error!\n");
        return;
    }

    uint8_t type = (uint8_t)atoi(params[1]); /* index:1 */
    uint8_t service = (uint8_t)atoi(params[2]); /* index:2 */
    const char *number_str = params[3]; /* index:3 */
    size_t str_len = strlen(number_str);
    if (str_len > HFP_AG_PHONENUM_MAX_DIGITS) { /* number最大长度为32 */
        bts_sample_log("number len exceeds max limit! str_len = %u \n", str_len);
        return;
    }
    uint8_t number_len = (uint8_t)str_len;

    const char *name_str = params[4]; /* index:4 *//* index:4 */
    size_t name_str_len = strlen(name_str);
    if (name_str_len > 256) { /* name最大长度为256 */
        bts_sample_log("name len exceeds max limit! str_len = %u \n", str_len);
        return;
    }
    uint8_t name_len = (uint8_t)name_str_len;

    hfp_phone_info_t *info = bts_sample_mem_new(sizeof(hfp_phone_info_t) + name_len);
    if (memset_s(info, sizeof(hfp_phone_info_t) + name_len, 0,
        sizeof(hfp_phone_info_t) + name_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    info->type = type;
    info->service = service;
    info->num_len = number_len;
    if (memcpy_s(info->number, HFP_AG_PHONENUM_MAX_DIGITS, (uint8_t *)number_str, str_len) != EOK) {
        bts_sample_log("number memcpy error.\n");
        bts_sample_mem_free(info);
        return;
    }
    info->name_len = name_len;
    if (memcpy_s(info->alpha_str, name_len, (int8_t *)name_str, name_len) != EOK) {
        bts_sample_log("name memcpy error.\n");
        bts_sample_mem_free(info);
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_send_network_incoming_event(&bd_addr, info);
    bts_sample_mem_free(info);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> <idx> <dir> <status> <mode> <mpty> <type> <number> <is_last> */
static void sample_hfp_ag_cmd_send_call_list(const char **params, int32_t params_cnt)
{
    if (params_cnt != 9) { /* 9个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag set volume decode addr error!\n");
        return;
    }
    uint8_t number_len = 0;
    const char *number_str = params[7]; /* 7:idx */
    size_t str_len = strlen(number_str);
    if (str_len > HFP_AG_PHONENUM_MAX_DIGITS) { /* number最大长度为32 */
        bts_sample_log("number len exceeds max limit! str_len = %u \n", str_len);
        return;
    }
    number_len = (uint8_t)str_len;
    hfp_clcc_info_t *info = bts_sample_mem_new(sizeof(hfp_clcc_info_t) + number_len);
    if (memset_s(info, sizeof(hfp_clcc_info_t) + number_len, 0,
        sizeof(hfp_clcc_info_t) + number_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    info->idx = (uint8_t)atoi(params[1]);
    info->dir = (uint8_t)atoi(params[2]); /* 2:idx */
    info->status = (uint8_t)atoi(params[3]); /* 3:idx */
    info->mode = (uint8_t)atoi(params[4]); /* 4:idx */
    info->mpty = (uint8_t)atoi(params[5]); /* 5:idx */
    info->type = (uint8_t)atoi(params[6]); /* 6:idx */
    info->num_len = number_len;
    if (memcpy_s(info->number, number_len, (uint8_t *)number_str, str_len) != EOK) {
        bts_sample_log("number memcpy error.\n");
        bts_sample_mem_free(info);
        return;
    }

    hfp_ag_last_t is_last = (hfp_ag_last_t)atoi(params[8]); /* 8:idx */
    errcode_t ret = (errcode_t)hfp_ag_send_current_calls(&bd_addr, info, is_last);
    bts_sample_mem_free(info);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> <type> <service> <number> <alpha_str> <is_last> */
static void sample_hfp_ag_cmd_send_subscriber_number(const char **params, int32_t params_cnt)
{
    if (params_cnt != 6) { /* 6个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag set volume decode addr error!\n");
        return;
    }

    uint8_t type = (uint8_t)atoi(params[1]); /* index:1 */
    uint8_t service = (uint8_t)atoi(params[2]); /* index:2 */
    const char *number_str = params[3]; /* index:3 */
    size_t str_len = strlen(number_str);
    if (str_len > 32) { /* number最大长度为32 */
        bts_sample_log("number len exceeds max limit! str_len = %u \n", str_len);
        return;
    }
    uint8_t number_len = (uint8_t)str_len;

    const char *name_str = params[4]; /* index:4 *//* index:4 */
    size_t name_str_len = strlen(name_str);
    if (name_str_len > 256) { /* name最大长度为256 */
        bts_sample_log("name len exceeds max limit! str_len = %u \n", str_len);
        return;
    }
    uint8_t name_len = (uint8_t)name_str_len;

    hfp_phone_info_t *info = bts_sample_mem_new(sizeof(hfp_phone_info_t) + name_len);
    if (memset_s(info, sizeof(hfp_phone_info_t) + name_len, 0,
        sizeof(hfp_phone_info_t) + name_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    info->type = type;
    info->service = service;
    info->num_len = number_len;
    if (memcpy_s(info->number, HFP_AG_PHONENUM_MAX_DIGITS, (uint8_t *)number_str, str_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    info->name_len = name_len;
    if (memcpy_s(info->alpha_str, name_len, (int8_t *)name_str, name_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    hfp_ag_last_t is_last = (hfp_ag_last_t)atoi(params[5]);
    errcode_t ret = (errcode_t)hfp_ag_send_subscriber_number(&bd_addr, info, is_last);
    bts_sample_mem_free(info);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> <value> */
static void sample_hfp_ag_cmd_send_battery_charge(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag send battery charge decode addr error!\n");
        return;
    }
    uint8_t value = (uint8_t)atoi(params[1]); /* 第1个参数 */
    errcode_t ret = (errcode_t)hfp_ag_send_battery_charge(&bd_addr, value);
    bts_sample_log("value: %d ret: %u\n", value, ret);
}

/* <addr> <error_code> */
static void sample_hfp_ag_cmd_send_cmee_error(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag send cmee error decode addr error!\n");
        return;
    }
    hfp_ag_cmee_error_t error_code = (hfp_ag_cmee_error_t)atoi(params[1]); /* 第1个参数 */
    errcode_t ret = (errcode_t)hfp_ag_send_cmee_error(&bd_addr, error_code);
    bts_sample_log("error_code: %d ret: %u\n", error_code, ret);
}

/* <addr> <mode> <format> <operator_name> */
static void sample_hfp_ag_cmd_send_network_operator(const char **params, int32_t params_cnt)
{
    if (params_cnt != 4) { /* 4个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag send network operator decode addr error!\n");
        return;
    }

    const char *operator_name = params[3]; /* index3 */
    uint8_t operator_len = 0;
    size_t name_str_len = strlen(operator_name);
    if (name_str_len > HFP_AG_COPS_OPERATOR_MAXLEN) { /* name最大长度为16 */
        bts_sample_log("name len exceeds max limit! str_len = %u \n", name_str_len);
        return;
    }
    operator_len = (uint8_t)name_str_len;
    
    hfp_cops_info_t *info = bts_sample_mem_new(sizeof(hfp_cops_info_t) + operator_len);
    if (memset_s(info, sizeof(hfp_cops_info_t) + operator_len, 0,
        sizeof(hfp_cops_info_t) + operator_len) != EOK) {
        bts_sample_mem_free(info);
        return;
    }
    info->mode = (uint8_t)atoi(params[1]);
    info->format = (uint8_t)atoi(params[2]); /* index2 */
    info->operator_len = operator_len;
    if (memcpy_s(info->operator_name, operator_len, (int8_t *)operator_name, operator_len) != EOK) {
        bts_sample_log("name memcpy error.\n");
        bts_sample_mem_free(info);
        return;
    }

    errcode_t ret = (errcode_t)hfp_ag_send_network_operator(&bd_addr, info);
    bts_sample_mem_free(info);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> <service> <call> <callsetup> <callheld> <signal> <roam> <battchg> */
static void sample_hfp_ag_cmd_send_indicator_val(const char **params, int32_t params_cnt)
{
    if (params_cnt != 8) { /* 8个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag send indicator val decode addr error!\n");
        return;
    }

    g_sample_hfp_cind_info.service = (uint8_t)atoi(params[1]);
    g_sample_hfp_cind_info.call = (uint8_t)atoi(params[2]); /* index:2 */
    g_sample_hfp_cind_info.callsetup = (uint8_t)atoi(params[3]); /* index:3 */
    g_sample_hfp_cind_info.callheld = (uint8_t)atoi(params[4]); /* index:4 */
    g_sample_hfp_cind_info.signal = (uint8_t)atoi(params[5]); /* index:5 */
    g_sample_hfp_cind_info.roam = (uint8_t)atoi(params[6]); /* index:6 */
    g_sample_hfp_cind_info.battchg = (uint8_t)atoi(params[7]); /* index:7 */

    errcode_t ret = (errcode_t)hfp_ag_send_indicator_val(&bd_addr, &g_sample_hfp_cind_info);
    bts_sample_log("ret: %u\n", ret);
}

/* <addr> */
static void sample_hfp_ag_cmd_originate_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) {
        bts_sample_log("sample hfp ag originate call param is invalid.\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[0], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample hfp ag originate call decode addr error!\n");
        return;
    }
    errcode_t ret = (errcode_t)hfp_ag_send_originate_call_event(&bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

sample_hfp_ag_hdl_info g_sample_hfp_ag_hdls[] = {
    { SAMPLE_HFP_AG_CMD_INIT, NULL },
    { SAMPLE_HFP_AG_CMD_DEINIT, NULL},
    { SAMPLE_HFP_AG_CMD_REG_CALLBACK, sample_hfp_ag_reg_callback},
    { SAMPLE_HFP_AG_CMD_CONN_HFP, sample_hfp_ag_cmd_conn_hfp},
    { SAMPLE_HFP_AG_CMD_DISCONN_HFP, sample_hfp_ag_cmd_disconn_hfp},
    { SAMPLE_HFP_AG_CMD_CONN_SCO, sample_hfp_ag_cmd_conn_sco},
    { SAMPLE_HFP_AG_CMD_DISCONN_SCO, sample_hfp_ag_cmd_disconn_sco},
    { SAMPLE_HFP_AG_CMD_GET_DEV_CONN_STATE, sample_hfp_ag_cmd_get_dev_conn_state},
    { SAMPLE_HFP_AG_CMD_GET_SCO_CONN_STATE, sample_hfp_ag_cmd_get_sco_conn_state},
    { SAMPLE_HFP_AG_CMD_ANSWER_CALL, sample_hfp_ag_cmd_answer_call},
    { SAMPLE_HFP_AG_CMD_FINISH_CALL, sample_hfp_ag_cmd_finish_call},
    { SAMPLE_HFP_AG_CMD_SET_VOLUME, sample_hfp_ag_cmd_set_volume},
    { SAMPLE_HFP_AG_CMD_SEND_INCOMING_EVENT, sample_hfp_ag_cmd_send_incoming_event},
    { SAMPLE_HFP_AG_CMD_SEND_NETWORK_EVENT, sample_hfp_ag_cmd_send_network_event},
    { SAMPLE_HFP_AG_CMD_SEND_CALL_LIST, sample_hfp_ag_cmd_send_call_list},
    { SAMPLE_HFP_AG_CMD_SEND_SUBSCRIBER_NUMBER, sample_hfp_ag_cmd_send_subscriber_number},
    { SAMPLE_HFP_AG_CMD_SEND_BATTERY_CHARGE, sample_hfp_ag_cmd_send_battery_charge},
    { SAMPLE_HFP_AG_CMD_SEND_CMEE_ERROR, sample_hfp_ag_cmd_send_cmee_error},
    { SAMPLE_HFP_AG_CMD_SEND_NETWORK_OPERATOR, sample_hfp_ag_cmd_send_network_operator},
    { SAMPLE_HFP_AG_CMD_SEND_INDICATOR_VAL, sample_hfp_ag_cmd_send_indicator_val},
    { SAMPLE_HFP_AG_CMD_ORIGINATE_CALL, sample_hfp_ag_cmd_originate_call},
};

void bt_hfp_ag_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    bts_sample_log("id: %u\n", cmd_id);

    size_t hdl_cnt = sizeof(g_sample_hfp_ag_hdls) / sizeof(sample_hfp_ag_hdl_info);
    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_sample_hfp_ag_hdls[i].id == cmd_id) {
            g_sample_hfp_ag_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("invalid cmd_id = %d \n", cmd_id);
}

#ifdef  __cplusplus
}
#endif
