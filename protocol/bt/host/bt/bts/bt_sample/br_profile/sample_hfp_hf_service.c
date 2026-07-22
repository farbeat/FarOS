/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT HFP Service module.
 */

#include "sample_hfp_hf_service.h"
#include <soc_osal.h>
#include <td_base.h>
#include <stdlib.h>
#include "securec.h"
#include "errcode.h"
#include "bts_br_gap.h"
#include "bts_hfp_hf.h"
#include "sample_common.h"
#ifdef MEMORY_MINI_HFP_SUPPORT
#include "audio_manager.h"
#else
#include "bt_audio_manager_wrapper.h"
#endif
#include "bts_pbap.h"

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
static bd_addr_t g_hfp_hf_curr_addr;

/* HFP service test cmd id */
typedef enum {
    SAMPLE_HFP_HF_CMD_REG_CALLBACK,       /* 注册回调 */
    SAMPLE_HFP_HF_CMD_DIAL_PHONE_NUM,     /* 拨号 */
    SAMPLE_HFP_HF_CMD_ANSWER_CALL,        /* 接听电话 */
    SAMPLE_HFP_HF_CMD_REJECT_CALL,        /* 拒接电话 */
    SAMPLE_HFP_HF_CMD_FINISH_CALL,        /* 挂断电话 */
    SAMPLE_HFP_HF_CMD_SET_SPK,            /* 设置喇叭音量 */
    SAMPLE_HFP_HF_CMD_SET_MIC,            /* 设置麦克风音量 */
    SAMPLE_HFP_HF_CMD_AUDIO_CONN,         /* 通话音频切换 */
    SAMPLE_HFP_HF_CMD_HOLD_CALL,          /* 挂起通话 */
    SAMPLE_HFP_HF_CMD_SEND_DTMF_TONE,     /* 发送DTMF码 */
    SAMPLE_HFP_HF_CMD_OPEN_VOICE_RECOGN,  /* 开启语音识别 */
    SAMPLE_HFP_HF_CMD_CLOSE_VOICE_RECOGN, /* 关闭语音识别 */
    SAMPLE_HFP_HF_CMD_GET_DEV_CONN_STATE, /* 获取设备Profile连接状态 */
    SAMPLE_HFP_HF_CMD_GET_SCO_CONN_STATE, /* 获取SCO连接状态 */
    SAMPLE_HFP_HF_CMD_CONN_SCO,           /* 建立SCO连接 */
    SAMPLE_HFP_HF_CMD_DISCONN_SCO,        /* 断开SCO连接 */
} sample_hfp_hf_cmd_id;

typedef void (*hfp_hf_sample_cmd_handle_func)(const char **params, int32_t params_cnt);

typedef struct {
    sample_hfp_hf_cmd_id id;
    hfp_hf_sample_cmd_handle_func func;
} sample_hfp_hf_hdl_info;

#ifdef MEMORY_MINI_HFP_SUPPORT
struct AudioManager *g_audio_manager = NULL;
struct AudioAdapter *g_primary_adapter = NULL;
struct AudioAdapter *g_bluetooth_adapter = NULL;

static int32_t audio_hal_init(void)
{
    g_audio_manager = GetAudioManagerFuncs();
    if (g_audio_manager == NULL) {
        bts_sample_log("{%s():%d} get audio manager failed\n", __FUNCTION__, __LINE__);
        return -1;
    }

    int ret;
    int32_t size = -1;
    struct AudioAdapterDescriptor *descs = NULL;
    ret = g_audio_manager->GetAllAdapters(g_audio_manager, &descs, &size);
    if (ret != 0) {
        bts_sample_log("{%s():%d} get all adapters failed\n", __FUNCTION__, __LINE__);
        return ret;
    }

    struct AudioAdapter *adapter = NULL;
    for (int32_t index = 0; index < size; index++) {
        const struct AudioAdapterDescriptor *desc = &descs[index];
        if (g_audio_manager->LoadAdapter(g_audio_manager, desc, &adapter) == 0) {
            ret = adapter->InitAllPorts(adapter);
            if (ret != 0) {
                bts_sample_log("{%s():%d} adapter %s init ports failed\n", __FUNCTION__, __LINE__, desc->adapterName);
                g_audio_manager->UnloadAdapter(g_audio_manager, adapter);
                adapter = NULL;
                continue;
            }
            if (strcmp(desc->adapterName, "Primary") == 0) {
                g_primary_adapter = adapter;
            } else if (strcmp(desc->adapterName, "Bluetooth") == 0) {
                g_bluetooth_adapter = adapter;
            }
        }
    }

    return 0;
}
#endif

static void sample_hfp_hf_print_addr(const bd_addr_t *bd_addr)
{
    if (bd_addr != NULL) {
        bts_sample_log("addr: ****%02x%02x%02x%02x ",
            bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* addr 下标 3 2 1 0 */
    }
}

static void sample_handle_hfp_connection_state_changed(const bd_addr_t *bd_addr, profile_connect_state_t state)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("state: %d\n", state);
}

static int32_t g_hfp_media_status = 0;
static int32_t g_hfp_media_flag = 0;
static uint8_t g_spk_volume = SAMPLE_HFP_PROF_VOL_LEVEL;
static osal_task *task_audio_init = NULL;
static osal_task *task_audio_deinit = NULL;

#ifdef MEMORY_MINI_HFP_SUPPORT
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
#endif

#ifdef MEMORY_MINI_HFP_SUPPORT
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
            g_audio_render_down_link = NULL;
        }
    }

    return 0;
}
#endif

#ifndef MEMORY_MINI_HFP_SUPPORT
static int32_t audio_stream_init(void)
{
    /* 请求音频焦点 */
    /* deactive响铃时申请的焦点后，重新申请通话焦点 */
    audio_manager_activate_interrupt(AUDIO_STREAM_VOICE_CALL_BT_SCO);
    audio_manager_set_device_connection_state(IN_BLUETOOTH_SCO, AUDIO_DEVICE_CONNECTED);
    audio_manager_set_device_connection_state(OUT_BLUETOOTH_SCO, AUDIO_DEVICE_CONNECTED);
    g_hdl_up_mic = audio_stream_in_init(AUDIO_UP_LINK, g_hfp_codec);
    g_hdl_up_sco = audio_stream_out_init(AUDIO_UP_LINK, g_hfp_codec);
    g_hdl_dn_spk = audio_stream_out_init(AUDIO_DOWN_LINK, g_hfp_codec);
    g_hdl_dn_sco = audio_stream_in_init(AUDIO_DOWN_LINK, g_hfp_codec);

    return 0;
}

static int32_t establish_audio_stream_pipeline(void)
{
    uint32_t mic_shm_id = 0;
    uint32_t spk_shm_id = 0;

    audio_stream_in_start(g_hdl_dn_sco);
    audio_stream_out_start(g_hdl_dn_spk);
    audio_stream_out_get_channel_id(g_hdl_dn_spk, &spk_shm_id);
    audio_stream_in_attach_backend(g_hdl_dn_sco, spk_shm_id);
    audio_stream_in_start(g_hdl_up_mic);
    audio_stream_out_start(g_hdl_up_sco);
    audio_stream_in_get_channel_id(g_hdl_up_mic, &mic_shm_id);
    audio_stream_out_attach_frontend(g_hdl_up_sco, mic_shm_id);

    return 0;
}

static int32_t abrogation_audio_stream_pipeline(void)
{
    /* 销毁上下行通路 */
    audio_stream_out_stop(g_hdl_up_sco);
    audio_stream_out_deinit(g_hdl_up_sco);
    audio_stream_in_stop(g_hdl_dn_sco);
    audio_stream_in_deinit(g_hdl_dn_sco);
    audio_stream_in_stop(g_hdl_up_mic);
    audio_stream_in_deinit(g_hdl_up_mic);
    audio_stream_out_stop(g_hdl_dn_spk);
    audio_stream_out_deinit(g_hdl_dn_spk);
    audio_manager_set_device_connection_state(IN_BLUETOOTH_SCO, AUDIO_DEVICE_DISCONNECTED);
    audio_manager_set_device_connection_state(OUT_BLUETOOTH_SCO, AUDIO_DEVICE_DISCONNECTED);
    /* 释放音频焦点 */
    audio_manager_deactivate_interrupt();
    audio_manager_deinit();

    return 0;
}
#endif

static void sample_hfp_audio_task_destroy(osal_task **task)
{
    bts_sample_log("destroy task id: %p\n", (*task)->task);
    if (*task != NULL && (*task)->task != NULL) {
        osal_kthread_destroy(*task, 0);
    }
}

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
#ifdef MEMORY_MINI_HFP_SUPPORT
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
#ifdef MEMORY_MINI_HFP_SUPPORT
    ret = establish_audio_capture_render_pipeline();
    if (ret != 0) {
        bts_sample_log("{%s():%d} establish audio capture render pipeline failed\n", __FUNCTION__, __LINE__);
        (void)g_primary_adapter->DestroyCapture(g_primary_adapter, g_audio_capture_up_link);
        (void)g_bluetooth_adapter->DestroyRender(g_bluetooth_adapter, g_audio_render_up_link);
        (void)g_bluetooth_adapter->DestroyCapture(g_bluetooth_adapter, g_audio_capture_down_link);
        (void)g_primary_adapter->DestroyRender(g_primary_adapter, g_audio_render_down_link);
        return ret;
    }
    sample_hfp_hf_set_spk_volume(g_spk_volume);
#else
    establish_audio_stream_pipeline();
#endif

    bts_sample_log("sample_audio_init_start end\n");
    /* Start success */
    g_hfp_media_status = HFP_MEDIA_STATUS_START;
    g_hfp_media_flag = HFP_MEDIA_FLAG_IDLE;

    sample_hfp_audio_task_destroy(&task_audio_init);
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

#ifdef MEMORY_MINI_HFP_SUPPORT
    abrogation_audio_capture_render_pipeline();
#else
    abrogation_audio_stream_pipeline();
#endif

    bts_sample_log("sample_audio_deinit end\n");
    /* Deinit success */
    g_hfp_media_status = HFP_MEDIA_STATUS_DEINIT;
    g_hfp_media_flag = HFP_MEDIA_FLAG_IDLE;

    sample_hfp_audio_task_destroy(&task_audio_deinit);
    return 0;
}

static void sample_hfp_audio_task_create(osal_task **task, osal_kthread_handler handler)
{
    *task = osal_kthread_create(handler, NULL, "sample_hfp_audio_task", SAMPLE_HFP_TASK_STACK_SIZE);
    if (*task == NULL) {
        bts_sample_log("create task fail\n");
        return;
    }
    bts_sample_log("create task id: %p\n", (*task)->task);
    int ret = osal_kthread_set_priority(*task, SAMPLE_HFP_TASK_PRI);
    if (ret != OSAL_SUCCESS) {
        bts_sample_log("set pri error: %08x\n", ret);
    }
}

static void sample_handle_sco_state_changed(const bd_addr_t *bd_addr, hfp_sco_connect_state_t state)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("state: %d\n", state);
    osal_kthread_handler task_body = NULL;
    switch (state) {
        case HFP_SCO_STATE_CONNECTING:
            sample_hfp_audio_task_create(&task_audio_init, sample_audio_init_start_task_body);
            break;

        case HFP_SCO_STATE_CONNECTED:
            break;

        case HFP_SCO_STATE_DISCONNECTING:
            break;

        case HFP_SCO_STATE_DISCONNECTED:
        case HFP_SCO_STATE_CONNECT_FAILURE:
            sample_hfp_audio_task_create(&task_audio_deinit, sample_audio_deinit_task_body);
            break;

        default: {
            bts_sample_log("invalid state!\n");
            break;
        }
    }
}

static void sample_handle_hfp_battery_changed(const bd_addr_t *bd_addr, int32_t battery_level)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("battery_level: %d\n", battery_level);
}

static void sample_handle_hfp_signal_strength_changed(const bd_addr_t *bd_addr, int32_t signal_strength)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("signal_strength: %d\n", signal_strength);
}

static void sample_handle_hfp_regist_status_changed(const bd_addr_t *bd_addr, int32_t status)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("status: %d\n", status);
}

static void sample_handle_hfp_roaming_status_changed(const bd_addr_t *bd_addr, int32_t status)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("status: %d\n", status);
}

static void sample_handle_hfp_operator_selection(const bd_addr_t *bd_addr, uint8_t *name, uint32_t len)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("name: %s, len: %u\n", name, len);
}

static void sample_handle_hfp_subscriber_number(const bd_addr_t *bd_addr, uint8_t *number, uint32_t len)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("number: %s, len: %u\n", number, len);
}

static void sample_handle_voice_recog_status_changed(const bd_addr_t *bd_addr, int32_t status)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("status: %d\n", status);
}

static void sample_handle_inband_ringtone_changed(const bd_addr_t *bd_addr, int32_t status)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("status: %d\n", status);
    uint32_t stat = (uint32_t)status;
    if ((stat & HFP_HF_IN_BAND_TONG) == 0) { /* 带外铃声 */
        if ((stat & HFP_HF_INCOMING_CALL) != 0) { /* 带外铃声来电振铃 */
            /* 本地媒体响铃 */
        } else if ((stat & HFP_HF_TERMINATE_LOCAL_RINGTONE) != 0) { /* 终止本地生成的振铃音 */
            /* 本地媒体结束响铃 */
        }
    }
}

static td_char g_call_state_string[][20] = {
    "Active",
    "Held",
    "Dialing",
    "Alerting",
    "Incoming",
    "Waiting",
    "Response and Hold",
    "Finished"
};

#define HFP_PHONENUM_MAX_DIGITS 32
static void sample_handle_hfp_call_state_changed(const bd_addr_t *bd_addr, const hfp_hf_calls_info_t *call)
{
    sample_hfp_hf_print_addr(bd_addr);
    if (call != NULL) {
        bts_sample_log("id: %d, number: ", call->id);
        for (int i = 0; i < call->number_len; i++) {
            bts_sample_log("%c", call->number[i]);
        }
        bts_sample_log(", outgoing: %d, state: %s\n", call->outgoing, g_call_state_string[call->state]);
    }
    /* 响铃时查询电话号码对应的list返回vcf和name */
    if (call->state == HFP_HF_CALL_STATE_INCOMING && call->number_len != 0) {
        char *tel_name = "telecom";
        uint32_t ret = (uint32_t)pbap_pce_set_phone_book(bd_addr, (unsigned char*)tel_name);
        if (ret != ERRCODE_BT_SUCCESS) {
            bts_sample_log("set phone book ret 0x%x\n", ret);
            return;
        }
        char *name = "pb";
        char number[HFP_PHONENUM_MAX_DIGITS + 1] = {0};
        if (memcpy_s(number, HFP_PHONENUM_MAX_DIGITS, call->number, call->number_len) != EOK) {
            bts_sample_log("memcpy_s fail.");
            return;
        }
        pbap_pce_pull_vcard_listing_param_t thr_par = {0};
        thr_par.name = (unsigned char*)name;
        thr_par.max_list_count = 1; /* 1: max list */
        thr_par.list_offset = 0;
        thr_par.order = PBAP_ORDER_INDEXED;
        thr_par.search_value = (unsigned char*)number; /* 查询条件要有字符串结束符 */
        thr_par.search_property = PBAP_SEARCH_PROPERTY_NUMBER;
        bts_sample_log("pull vcard list number %s search_value %s\n", call->number, thr_par.search_value);
        ret = pbap_pce_pull_vcard_listing(bd_addr, &thr_par);
        bts_sample_log("pull vcard list ret 0x%x\n", ret);
    }
}

void sample_hfp_hf_set_spk_volume(uint8_t volume)
{
    bts_sample_log("sample hfp set spk volume: %u\n", volume);
#ifdef MEMORY_MINI_HFP_SUPPORT
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
    audio_manager_set_volume(AUDIO_STREAM_VOICE_CALL_BT_SCO, vol);
#endif
}

static void sample_handle_hfp_volume_changed(const bd_addr_t *bd_addr, hfp_volume_type_t type, uint8_t volume)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("type: %d, volume: %u\n", type, volume);
    if (type == HFP_VOLUME_SPEAKER) {
        g_spk_volume = volume;
        sample_hfp_hf_set_spk_volume(volume);
    }
}

static void sample_handle_hfp_dial_error_code(const bd_addr_t *bd_addr, int32_t code)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("code : %d\n", code);
}

static void sample_handle_hfp_codec_changed(const bd_addr_t *bd_addr, int codec)
{
    sample_hfp_hf_print_addr(bd_addr);
    bts_sample_log("codec : %d\n", codec);
    if (codec == HFP_HF_CODEC_ID_CVSD) {
#ifdef MEMORY_MINI_HFP_SUPPORT
        g_hfp_codec = AUDIO_FORMAT_PCM_16_BIT;
#else
        g_hfp_codec = PCM;
#endif
    } else if (codec == HFP_HF_CODEC_ID_MSBC) {
#ifdef MEMORY_MINI_HFP_SUPPORT
        g_hfp_codec = AUDIO_FORMAT_mSBC;
#else
        g_hfp_codec = mSBC;
#endif
    }
}

static bool sample_hfp_hf_is_null_pointer(unsigned int sample_mode, unsigned int index)
{
    return ((sample_mode & (1 << index)) != 0);
}

/* 注册回调 */
static void sample_hfp_hf_cmd_00_reg_callback(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bool fill_callbacks = atoi(params[1]);
    if (sample_mode >= (1 << ((uint32_t)params_cnt - 1))) {
        bts_sample_log("sample_mode err: %u!\n", sample_mode);
        return;
    }
    errcode_t ret = ERRCODE_BT_SUCCESS;
    if (sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        ret = (errcode_t)hfp_hf_register_callbacks(NULL);
        return;
    }
    if (fill_callbacks != 0) {
        hfp_hf_callbacks_t callbacks = {
            sample_handle_hfp_connection_state_changed,
            sample_handle_sco_state_changed,
            sample_handle_hfp_battery_changed,
            sample_handle_hfp_signal_strength_changed,
            sample_handle_hfp_regist_status_changed,
            sample_handle_hfp_roaming_status_changed,
            sample_handle_hfp_operator_selection,
            sample_handle_hfp_subscriber_number,
            sample_handle_voice_recog_status_changed,
            sample_handle_inband_ringtone_changed,
            sample_handle_hfp_volume_changed,
            sample_handle_hfp_call_state_changed,
            sample_handle_hfp_dial_error_code,
            sample_handle_hfp_codec_changed,
        };
        ret = (errcode_t)hfp_hf_register_callbacks(&callbacks);
    } else {
        hfp_hf_callbacks_t callbacks = { 0 };
        ret = (errcode_t)hfp_hf_register_callbacks(&callbacks);
    }
    bts_sample_log("ret: %u\n", ret);
}

// 蓝牙通话注册回调
void wstp_hfp_hf_reg_callback(void)
{
#ifdef MEMORY_MINI_HFP_SUPPORT
    audio_hal_init();
#endif
    errcode_t ret = ERRCODE_BT_SUCCESS;
    hfp_hf_callbacks_t callbacks = {
        sample_handle_hfp_connection_state_changed,
        sample_handle_sco_state_changed,
        sample_handle_hfp_battery_changed,
        sample_handle_hfp_signal_strength_changed,
        sample_handle_hfp_regist_status_changed,
        sample_handle_hfp_roaming_status_changed,
        sample_handle_hfp_operator_selection,
        sample_handle_hfp_subscriber_number,
        sample_handle_voice_recog_status_changed,
        sample_handle_inband_ringtone_changed,
        sample_handle_hfp_volume_changed,
        sample_handle_hfp_call_state_changed,
        sample_handle_hfp_dial_error_code,
        sample_handle_hfp_codec_changed,
    };
    ret = (errcode_t)hfp_hf_register_callbacks(&callbacks);
    bts_sample_log("wstp_hfp_hf_reg_callback ret=0x%x\n", ret);
}

static bd_addr_t* sample_hfp_hf_parse_addr(const char* addr_str)
{
    if (decode_bd_addr_from_str(addr_str, g_hfp_hf_curr_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("decode addr error!\n");
        return NULL;
    }
    return &g_hfp_hf_curr_addr;
}

/* 拨号 */
static void sample_hfp_hf_cmd_01_dial_num(const char **params, int32_t params_cnt)
{
    if (params_cnt != 4) { /* 4个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    uint8_t *number = NULL;
    int32_t str_len = 0;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 1)) {
        number = (uint8_t*)params[2]; /* 第2个参数 */
        str_len = (int32_t)strlen(params[2]); /* 第2个参数 */
    }

    int32_t num_cnt = atoi(params[3]); /* 第3个参数 */
    if (str_len != num_cnt) {
        bts_sample_log("num cnt err: %d!\n", num_cnt);
        return;
    }

    errcode_t call_ret = (errcode_t)hfp_hf_start_dial(bd_addr, number, (uint8_t)num_cnt);
    bts_sample_log("%s len: %d ret: %u\n", number, num_cnt, call_ret);
}

static void sample_hfp_hf_cmd_answer_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    int32_t type = atoi(params[2]); /* 第2个参数 */
    errcode_t answer_ret = (errcode_t)hfp_hf_accept_incoming_call(bd_addr, type);
    bts_sample_log("type: %d ret: %u\n", type, answer_ret);
}

static void sample_hfp_hf_cmd_reject_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t reject_ret = (errcode_t)hfp_hf_reject_incoming_call(bd_addr);
    bts_sample_log("ret: %u\n", reject_ret);
}

static void sample_hfp_hf_cmd_finish_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t finish_ret = (errcode_t)hfp_hf_finish_call(bd_addr);
    bts_sample_log("ret: %u\n", finish_ret);
}

static void sample_hfp_hf_cmd_set_spk(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    uint8_t volume = (uint8_t)atoi(params[2]); /* 第2个参数 */

    errcode_t ret = (errcode_t)hfp_hf_set_volume(bd_addr, HFP_VOLUME_SPEAKER, volume);
    bts_sample_log("volume: %u ret: %u\n", volume, ret);
}

static void sample_hfp_hf_cmd_set_mic(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    uint8_t volume = (uint8_t)atoi(params[2]); /* 第2个参数 */
    errcode_t ret = (errcode_t)hfp_hf_set_volume(bd_addr, HFP_VOLUME_MIC, volume);
    bts_sample_log("volume: %u ret: %u\n", volume, ret);
}

static void sample_hfp_hf_cmd_hold_call(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t ret = (errcode_t)hfp_hf_hold_active_call(bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

static void sample_hfp_hf_cmd_send_dtmf_tone(const char **params, int32_t params_cnt)
{
    if (params_cnt != 3) { /* 3个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    uint8_t code = *(uint8_t*)params[2]; /* 第2个参数 */
    errcode_t ret = (errcode_t)hfp_hf_send_dtmf_tone(bd_addr, code);
    bts_sample_log("code: %c ret: %u\n", code, ret);
}

static void sample_hfp_hf_cmd_open_voice_recogn(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t ret = (errcode_t)hfp_hf_open_voice_recognition(bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

static void sample_hfp_hf_cmd_close_voice_recogn(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t ret = (errcode_t)hfp_hf_close_voice_recognition(bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

static void sample_hfp_hf_cmd_get_dev_conn_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    int ret = hfp_hf_get_device_connect_state(bd_addr);
    bts_sample_log("ret: %d\n", ret);
}

static void sample_hfp_hf_cmd_get_sco_conn_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    int ret = hfp_hf_get_sco_connect_state(bd_addr);
    bts_sample_log("ret: %d\n", ret);
}

static void sample_hfp_hf_cmd_conn_sco(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t ret = (errcode_t)hfp_hf_connect_sco(bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

static void sample_hfp_hf_cmd_disconn_sco(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2个参数 */
        bts_sample_log("params cnt err: %d!\n", params_cnt);
        return;
    }

    uint32_t sample_mode = 0;
    if (sscanf_s(params[0], "%x", &sample_mode) <= 0) {
        bts_sample_log("get sample_mode fail\n");
        return;
    }
    bd_addr_t* bd_addr = NULL;
    if (!sample_hfp_hf_is_null_pointer(sample_mode, 0)) {
        bd_addr = sample_hfp_hf_parse_addr(params[1]);
    }

    errcode_t ret = (errcode_t)hfp_hf_disconnect_sco(bd_addr);
    bts_sample_log("ret: %u\n", ret);
}

sample_hfp_hf_hdl_info g_sample_hfp_hf_hdls[] = {
    { SAMPLE_HFP_HF_CMD_REG_CALLBACK, sample_hfp_hf_cmd_00_reg_callback },
    { SAMPLE_HFP_HF_CMD_DIAL_PHONE_NUM, sample_hfp_hf_cmd_01_dial_num  },
    { SAMPLE_HFP_HF_CMD_ANSWER_CALL, sample_hfp_hf_cmd_answer_call  },
    { SAMPLE_HFP_HF_CMD_REJECT_CALL, sample_hfp_hf_cmd_reject_call },
    { SAMPLE_HFP_HF_CMD_FINISH_CALL, sample_hfp_hf_cmd_finish_call },
    { SAMPLE_HFP_HF_CMD_SET_SPK, sample_hfp_hf_cmd_set_spk },
    { SAMPLE_HFP_HF_CMD_SET_MIC, sample_hfp_hf_cmd_set_mic },
    { SAMPLE_HFP_HF_CMD_HOLD_CALL, sample_hfp_hf_cmd_hold_call },
    { SAMPLE_HFP_HF_CMD_SEND_DTMF_TONE, sample_hfp_hf_cmd_send_dtmf_tone },
    { SAMPLE_HFP_HF_CMD_OPEN_VOICE_RECOGN, sample_hfp_hf_cmd_open_voice_recogn },
    { SAMPLE_HFP_HF_CMD_CLOSE_VOICE_RECOGN, sample_hfp_hf_cmd_close_voice_recogn },
    { SAMPLE_HFP_HF_CMD_GET_DEV_CONN_STATE, sample_hfp_hf_cmd_get_dev_conn_state },
    { SAMPLE_HFP_HF_CMD_GET_SCO_CONN_STATE, sample_hfp_hf_cmd_get_sco_conn_state },
    { SAMPLE_HFP_HF_CMD_CONN_SCO, sample_hfp_hf_cmd_conn_sco },
    { SAMPLE_HFP_HF_CMD_DISCONN_SCO, sample_hfp_hf_cmd_disconn_sco },
};

void bt_hfp_hf_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    bts_sample_log("id: %u\n", cmd_id);

    size_t hdl_cnt = sizeof(g_sample_hfp_hf_hdls) / sizeof(sample_hfp_hf_hdl_info);
    for (size_t i = 0; i < hdl_cnt; i++) {
        if (g_sample_hfp_hf_hdls[i].id == cmd_id) {
            g_sample_hfp_hf_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("invalid cmd_id = %d \n", cmd_id);
}

#ifdef  __cplusplus
}
#endif
