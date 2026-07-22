/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT AUDIO HAL module.
 */

#include "sample_bt_audio_hal.h"
#include "soc_osal.h"
#include "soc_errno.h"
#include "td_type.h"
#include "securec.h"
#include "bt_audio_hal_interface.h"
#include "sample_common.h"

/* a2dp service test cmd id */
typedef enum {
    SAMPLE_BT_AUDIO_HAL_REGISTER_LISTENER,    /* 0 注册回调 */
    SAMPLE_BT_AUDIO_HAL_DEREGISTER_LISTENER,  /* 1 取消注册回调 */
    SAMPLE_BT_AUDIO_HAL_START_AUDIO_STREAM,   /* 2 启动音频通道数据传输 */
    SAMPLE_BT_AUDIO_HAL_PAUSE_AUDIO_STREAM,   /* 3 暂停音频通道数据传输 */
    SAMPLE_BT_AUDIO_HAL_STOP_AUDIO_STREAM,    /* 4 停止音频通道数据传输 */
    SAMPLE_BT_AUDIO_HAL_SET_CODEC,            /* 5 设置音频通道参数 */
    SAMPLE_BT_AUDIO_HAL_GET_CODEC,            /* 6 获取音频通道参数 */
    SAMPLE_BT_AUDIO_HAL_AVRCP_VOLUME,         /* 7 设置AVRCP音量参数 */
    SAMPLE_BT_AUDIO_HAL_ATTACH_SHMID,         /* 8 为音频通道绑定音频端口 */
    SAMPLE_BT_AUDIO_HAL_DETACH_SHMID,         /* 9 取消音频通道端口绑定 */
} sample_bt_audio_hal_cmd_id;

/* adts头信息 */
typedef struct {
    uint16_t sync_word : 12;
    uint16_t id : 1;
    uint16_t layer : 2;
    uint16_t protection_absent : 1;

    uint16_t profile : 2;
    uint16_t sampling_freq_idx : 4;
    uint16_t private_bit : 1;
    uint16_t channel_config : 3;
    uint16_t original_copy : 1;
    uint16_t home : 1;
    uint16_t copyright_bit : 1;
    uint16_t copyright_start : 1;
    uint16_t not_exist1 : 2;

    uint32_t length : 13;
    uint32_t adts_buffer_fullness : 11;
    uint32_t num_of_raw_data : 2;
    uint32_t not_exist2 : 6;

    uint8_t *payload;
    int32_t payload_len;
} sample_adts_frame_info;

/* 事件回调函数 */
static void bt_audio_hal_sample_listener(bt_audio_event_type type, const void *data, int32_t size, void *context)
{
    (void)context;
    bts_sample_log("type:%d data:%08x size:%d\n", type, *((uint32_t *)data), size);

    switch (type) {
        case BT_AUDIO_A2DP_STREAM_OPENED: {
            bt_audio_a2dp_stream_open_data *open_data = (bt_audio_a2dp_stream_open_data *)data;
            uint32_t stream_hdl = (uint32_t)(uintptr_t)open_data->stream_hdl;
            bts_sample_log("stream open stream_hdl:%08x mtu:%u f_size:%u n_frame:%u\n",
                stream_hdl, open_data->stream_mtu, open_data->frame_size,
                open_data->num_frame);
            break;
        }
        default:
            break;
    }
}

static uint32_t sample_a2dp_fill_sbc_codec(const char **params, int32_t params_cnt, bt_a2dp_sbc_codec_caps *sbc_caps)
{
    if (params_cnt < 7) { /* 7 params */
        bts_sample_log("params cnt err, need 7, not %d\n", params_cnt);
        return EXT_ERR_INVALID_PARAMETER;
    }

    int tmp_data;
    int offset = 0;
    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->sample_frequency = (uint32_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->chnl_mode = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->block_length = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->subband = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->alloc_method = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->min_bitpool = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    sbc_caps->max_bitpool = (uint8_t)tmp_data;

    bts_sample_log(
        "sbc codec sampleFreq: %x chnlMode: %x blockLen: %x subBand: %x allocMtd: %x bitpool: %x %x\n",
        sbc_caps->sample_frequency, sbc_caps->chnl_mode, sbc_caps->block_length, sbc_caps->subband,
        sbc_caps->alloc_method, sbc_caps->min_bitpool, sbc_caps->max_bitpool);

    return EXT_ERR_SUCCESS;
}

static uint32_t sample_a2dp_fill_aac_codec(const char **params, int32_t params_cnt,
    bt_a2dp_mpeg24_codec_caps *aac_caps)
{
    if (params_cnt < 5) { /* 5 params */
        bts_sample_log("params cnt err, need 5, not %d\n", params_cnt);
        return EXT_ERR_INVALID_PARAMETER;
    }

    int tmp_data;
    int offset = 0;
    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    aac_caps->sample_frequency = (uint32_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    aac_caps->bit_rate = (uint32_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    aac_caps->channels = (uint8_t)tmp_data;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    aac_caps->object_type = (uint8_t)tmp_data;

    aac_caps->rfa = 0;

    if (sscanf_s(params[offset++], "%x", &tmp_data) <= 0) {
        bts_sample_log("sscanf fail\n");
        return EXT_ERR_FAILURE;
    }
    aac_caps->vbr = (uint8_t)tmp_data;

    bts_sample_log("aac codec sampleFreq: %x bit_rate: %x channels: %x objectType: %x vbr: %x\n",
        aac_caps->sample_frequency, aac_caps->bit_rate, aac_caps->channels, aac_caps->object_type, aac_caps->vbr);
    return EXT_ERR_SUCCESS;
}

static uint32_t sample_a2dp_fill_codec(const char **params, int32_t params_cnt, bt_a2dp_codec_param *codec)
{
    if (params_cnt <= 1) {
        bts_sample_log("sample_a2dp_fill_codec params cnt err\n");
        return EXT_ERR_INVALID_PARAMETER;
    }

    codec->codec_type = (uint8_t)atoi(params[0]);
    uint32_t ret;
    switch (codec->codec_type) {
        case BT_AUDIO_CODEC_SBC:
            ret = sample_a2dp_fill_sbc_codec(&params[1], params_cnt - 1, (bt_a2dp_sbc_codec_caps *)codec->codec_caps);
            break;

        case BT_AUDIO_CODEC_MPEG24:
            ret =
                sample_a2dp_fill_aac_codec(&params[1], params_cnt - 1, (bt_a2dp_mpeg24_codec_caps *)codec->codec_caps);
            break;

        default: {
            ret = EXT_ERR_INVALID_PARAMETER;
            bts_sample_log("not support codec type %u\n", codec->codec_type);
            break;
        }
    }

    return ret;
}

#define MAX_SEND_LEN 720
#define MAX_BUF_LEN  1024
#define MAX_FILE_PATH_LEN 128
#define DEAULT_SBC_LEN 10

static void sample_bt_audio_hal_cmd_register_callback(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    uint32_t ret = bt_register_audio_listener(bt_audio_hal_sample_listener, NULL);
    bts_sample_log("ret: %x\n", ret);
}

static void sample_bt_audio_hal_cmd_deregister_callback(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    bts_unused(params_cnt);

    uint32_t ret = bt_deregister_audio_listener(bt_audio_hal_sample_listener);
    bts_sample_log("ret: %x\n", ret);
}

/* 启动打流 */
static void sample_bt_audio_hal_cmd_start_stream(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf fail\n");
        return;
    }

    uint32_t ret = bt_start_audio_stream((td_pvoid)(uintptr_t)stream_hdl);
    bts_sample_log("stream_hdl: %08x ret: %08x\n", stream_hdl, ret);
}

/* 暂停打流 */
static void sample_bt_audio_hal_cmd_pause_stream(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf fail\n");
        return;
    }

    uint32_t ret = bt_pause_audio_stream((td_pvoid)(uintptr_t)stream_hdl);
    bts_sample_log("stream_hdl: %08x ret: %08x\n", stream_hdl, ret);
}

/* 停止打流 */
static void sample_bt_audio_hal_cmd_stop_stream(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 1：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf fail\n");
        return;
    }

    uint32_t ret = bt_stop_audio_stream((td_pvoid)(uintptr_t)stream_hdl);
    bts_sample_log("stream_hdl: %08x ret: %08x\n", stream_hdl, ret);
}

/* 设置音频通道参数 */
static void sample_bt_audio_hal_cmd_set_codec(const char **params, int32_t params_cnt)
{
    if (params_cnt < 3) { /* 3：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl param_type params...\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf strame_hdl fail\n");
        return;
    }

    uint32_t param_type = 0;
    if (sscanf_s(params[1], "%x", &param_type) <= 0) {
        bts_sample_log("sscanf param_type fail\n");
        return;
    }

    bt_a2dp_codec_param codec;
    uint32_t ret = sample_a2dp_fill_codec(&params[2], params_cnt - 2, &codec); /* 2： 前面已解析2个参数，需要跳过 */
    if (ret != EXT_ERR_SUCCESS) {
        bts_sample_log("fill codec fail ret: %x\n", ret);
        return;
    }

    ret = bt_set_audio_parameter(
        (td_pvoid)(uintptr_t)stream_hdl, param_type, (void *)&codec, sizeof(bt_a2dp_codec_param));
    bts_sample_log("bt_set_audio_parameter ret: %x\n", ret);
}

/* 获取音频通道参数 */
static void sample_bt_audio_hal_cmd_get_codec(const char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM2) { /* 3：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl param_type\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf strame_hdl fail\n");
        return;
    }

    uint32_t param_type = 0;
    if (sscanf_s(params[1], "%x", &param_type) <= 0) {
        bts_sample_log("sscanf param_type fail\n");
        return;
    }

    bt_a2dp_codec_param codec;
    uint32_t ret = bt_get_audio_parameter((td_pvoid)(uintptr_t)stream_hdl, param_type, (void *)&codec,
        sizeof(bt_a2dp_codec_param));
    bts_sample_log("ret: %x codec_type: %u ", ret, codec.codec_type);
    if (codec.codec_type == BT_AUDIO_CODEC_SBC) {
        bt_a2dp_sbc_codec_caps *caps = (bt_a2dp_sbc_codec_caps *)codec.codec_caps;
        bts_sample_log(
            "sbc sampleFreq: %02x chnlMode: %02x blockLen: %02x subBand: %02x allocMtd: %02x Bitpool: %02x %02x\n",
            caps->sample_frequency, caps->chnl_mode, caps->block_length,
            caps->subband, caps->alloc_method, caps->min_bitpool, caps->max_bitpool);
    } else if (codec.codec_type == BT_AUDIO_CODEC_MPEG24) {
        bt_a2dp_mpeg24_codec_caps *caps = (bt_a2dp_mpeg24_codec_caps *)codec.codec_caps;
        bts_sample_log("aac sampleFreq: %02x bit_rate: %02x channels: %02x objectType: %02x rfa: %02x vbr: %02x\n",
            caps->sample_frequency, caps->bit_rate, caps->channels, caps->object_type, caps->rfa, caps->vbr);
    } else {
        bts_sample_log(" invalid codec type!\n");
    }
}

/* 设置AVRCP音量参数 */
static void sample_bt_audio_hal_cmd_avrcp_volume(const char **params, int32_t params_cnt)
{
    if (params_cnt < 3) { /* 3：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl param_type params...\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf strame_hdl fail\n");
        return;
    }

    uint32_t param_type = 0;
    if (sscanf_s(params[1], "%x", &param_type) <= 0) {
        bts_sample_log("sscanf param_type fail\n");
        return;
    }

    uint32_t volume = 0;
    if (sscanf_s(params[2], "%x", &param_type) <= 0) {    /* 2：第三个参数 */
        bts_sample_log("sscanf volume fail\n");
        return;
    }

    bts_sample_log("stream_hdl 0x%x param_type %u volume %u\r\n", stream_hdl, param_type, volume);
    uint32_t ret =
        bt_set_audio_parameter((td_pvoid)(uintptr_t)stream_hdl, param_type, (void *)&volume, sizeof(uint8_t));
    bts_sample_log("ret: %x\n", ret);
}

/* 设置shmid */
static void sample_bt_audio_hal_cmd_attach_audio_port(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl param_type\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf strame_hdl fail\n");
        return;
    }

    uint32_t share_mem_id = 0;
    if (sscanf_s(params[1], "%x", &share_mem_id) <= 0) {
        bts_sample_log("sscanf share_mem_id fail\n");
        return;
    }

    bt_audio_port_params param = {};
    param.port_type = A2DP;
    param.share_mem_id = share_mem_id;
    bts_sample_log("stream_hdl 0x%x share_mem_id 0x%x\r\n", stream_hdl, share_mem_id);
    uint32_t ret = bt_attach_audio_port((td_pvoid)(uintptr_t)stream_hdl, &param);
    bts_sample_log("ret: %x\n", ret);
}

/* 取消shmid */
static void sample_bt_audio_hal_cmd_detach_audio_port(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("params_cnt error! need: stream_hdl param_type\n");
        return;
    }

    uint32_t stream_hdl = 0;
    if (sscanf_s(params[0], "%x", &stream_hdl) <= 0) {
        bts_sample_log("sscanf strame_hdl fail\n");
        return;
    }

    uint32_t share_mem_id = 0;
    if (sscanf_s(params[1], "%x", &share_mem_id) <= 0) {
        bts_sample_log("sscanf share_mem_id fail\n");
        return;
    }

    bt_audio_port_params param = {};
    param.port_type = A2DP;
    param.share_mem_id = share_mem_id;
    bts_sample_log("stream_hdl 0x%x share_mem_id 0x%x\r\n", stream_hdl, share_mem_id);
    uint32_t ret = bt_detach_audio_port((td_pvoid)(uintptr_t)stream_hdl, &param);
    bts_sample_log("ret: %x\n", ret);
}

btsrv_sample_cmd_hdl_info g_bt_audio_hal_sample_hdls[] = {
    /* 0 */
    { SAMPLE_BT_AUDIO_HAL_REGISTER_LISTENER, sample_bt_audio_hal_cmd_register_callback },
    { SAMPLE_BT_AUDIO_HAL_DEREGISTER_LISTENER, sample_bt_audio_hal_cmd_deregister_callback },
    { SAMPLE_BT_AUDIO_HAL_START_AUDIO_STREAM, sample_bt_audio_hal_cmd_start_stream },
    { SAMPLE_BT_AUDIO_HAL_PAUSE_AUDIO_STREAM, sample_bt_audio_hal_cmd_pause_stream },
    { SAMPLE_BT_AUDIO_HAL_STOP_AUDIO_STREAM, sample_bt_audio_hal_cmd_stop_stream },

    /* 5 */
    { SAMPLE_BT_AUDIO_HAL_SET_CODEC, sample_bt_audio_hal_cmd_set_codec },
    { SAMPLE_BT_AUDIO_HAL_GET_CODEC, sample_bt_audio_hal_cmd_get_codec },
    { SAMPLE_BT_AUDIO_HAL_AVRCP_VOLUME, sample_bt_audio_hal_cmd_avrcp_volume },
    { SAMPLE_BT_AUDIO_HAL_ATTACH_SHMID, sample_bt_audio_hal_cmd_attach_audio_port },
    { SAMPLE_BT_AUDIO_HAL_DETACH_SHMID, sample_bt_audio_hal_cmd_detach_audio_port },
};

void bt_audio_hal_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_bt_audio_hal_sample_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_bt_audio_hal_sample_hdls[i].id == cmd_id && g_bt_audio_hal_sample_hdls[i].func != NULL) {
            g_bt_audio_hal_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }
    bts_sample_log("cmd_id %d not support now!\n", cmd_id);
}
