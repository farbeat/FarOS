/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test function of BT AVRCP Service Test module.
 */

#include "sample_avrcp_service.h"
#include <stdlib.h>
#include <td_base.h>
#include <soc_osal.h>
#include "errcode.h"
#include "chip_io.h"
#include "soc_errno.h"
#include "securec.h"
#include "bts_br_gap.h"
#include "bts_avrcp_target.h"
#include "bts_avrcp_controller.h"
#include "bts_a2dp_source.h"
#include "bt_audio.h"
#include "sample_common.h"

#define AVRCP_CT_AUTO_REG_TYPE    1
#define AVRCP_CT_AUTO_REG_NON     0
#define BTS_ATTR_ID_LEN           4
#define BTS_ATTR_LIST_LEN         12
#define BTS_UID_LEN               8
#define MAX_SAMPLE_AVRCP_CT_REG_CNT 5  // 支持一次shell命令最多能注册5个事件
#define AVRCP_SHIFT_BITS_32    32
#define LONG_MIN    (-LONG_MAX - 1)
#define LONG_MAX    2147483647L

enum {
    SAMPLE_AVRCP_TG_ID_REGISTER_CALLBACK,
    SAMPLE_AVRCP_TG_ID_DEREGISTER_CALLBACK,
    SAMPLE_AVRCP_TG_ID_CONNECT,
    SAMPLE_AVRCP_TG_ID_DISCONNECT,
    SAMPLE_AVRCP_TG_ID_GET_CONN_STATE,
    SAMPLE_AVRCP_TG_ID_SET_LOC_STATE,     // 5
    SAMPLE_AVRCP_TG_ID_NOTIFY,            // 6
    SAMPLE_AVRCP_TG_ID_REG_CAP,
    SAMPLE_AVRCP_CT_ID_CONNECT = 10,      // 10
    SAMPLE_AVRCP_CT_ID_DISCONNECT,
    SAMPLE_AVRCP_CT_ID_PRESS_BUTTON,
    SAMPLE_AVRCP_CT_ID_SET_VOLUME,
    SAMPLE_AVRCP_CT_ID_GET_TG_PLAYER_STAT,
    SAMPLE_AVRCP_CT_ID_GET_ELEMENT_ATTR,  // 15
    SAMPLE_AVRCP_CT_ID_GET_EVENT_CAP,
    SAMPLE_AVRCP_CT_ID_REG_EVENT_CAP,
    SAMPLE_AVRCP_CT_ID_GET_CONN_STAT,
};

typedef struct {
    td_pvoid stream_hdl;
    uint8_t loc_volume;
    uint8_t play_stat;
    uint8_t dir_forward;
    uint8_t avrcp_button_flag;
    bd_addr_t bd_addr;
    uint8_t reg_cap_auto;
    uint8_t resrv;
    profile_connect_state_t avrcp_srv_conn_stat;
} sample_avrcp_tg_sample_inf;
sample_avrcp_tg_sample_inf g_avrcp_srv_sample_inf;

enum {
    AVRCP_SRV_PRESS_VALUE,
    AVRCP_SRV_RELEASED_VALUE,
    AVRCP_SRV_INIT_VALUE
};

enum {
    SAMPLE_A2DP_STREAM_UNKNOWN, /* no a2dp */
    SAMPLE_A2DP_STREAM_CREATE,
    SAMPLE_A2DP_STREAM_OPENED,
    SAMPLE_A2DP_STREAM_START,
    SAMPLE_A2DP_STREAM_SUSPENDED,
    SAMPLE_A2DP_STREAM_CLOSED
};

static bd_addr_t *sample_avrcp_srv_get_active_addr(void)
{
    return &g_avrcp_srv_sample_inf.bd_addr;
}

static void sample_avrcp_ct_connect_state_changed_callback(const bd_addr_t *bd_addr, profile_connect_state_t state)
{
    if (bd_addr == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("avrcp ct update conn stat:%u\n", state);
    (void)memcpy_s(g_avrcp_srv_sample_inf.bd_addr.addr, BD_ADDR_LEN, bd_addr->addr, BD_ADDR_LEN);
    bts_sample_log("addr: ****%02x%02x%02x%02x \n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* 3 2 1 0 idx */
    g_avrcp_srv_sample_inf.avrcp_srv_conn_stat = state;
}

static void sample_avrcp_ct_press_button_rsp_cbk(const bd_addr_t *bd_addr, int32_t key_operation)
{
    if (bd_addr == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    if (g_avrcp_srv_sample_inf.avrcp_button_flag == AVRCP_SRV_RELEASED_VALUE) {
        bts_sample_log("avrcp ct press button button_flag change:%u->%u\n",
            g_avrcp_srv_sample_inf.avrcp_button_flag,
            AVRCP_SRV_PRESS_VALUE);
        g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_PRESS_VALUE;
    }
    bts_sample_log("avrcp ct press button cbk key:%d button_flag:%u\n",
        key_operation,
        g_avrcp_srv_sample_inf.avrcp_button_flag);
}

static void sample_avrcp_press_release_button_log(uint32_t key_operation)
{
    switch (key_operation) {
        case AVRCP_KEY_VOLUME_UP:
            bts_sample_log("AVRCP_KEY_VOLUME_UP\n");
            break;
        case AVRCP_KEY_VOLUME_DOWN:
            bts_sample_log("AVRCP_KEY_VOLUME_DOWN\n");
            break;
        case AVRCP_KEY_MUTE:
            bts_sample_log("AVRCP_KEY_VOLUME_MUTE\n");
            break;
        case AVRCP_KEY_PLAY:
            bts_sample_log("AVRCP_KEY_PLAY\n");
            break;
        case AVRCP_KEY_STOP:
            bts_sample_log("AVRCP_KEY_STOP\n");
            break;
        case AVRCP_KEY_PAUSE:
            bts_sample_log("AVRCP_KEY_PAUSE\n");
            break;
        case AVRCP_KEY_FORWARD:
            bts_sample_log("AVRCP_KEY_FORWARD\n");
            break;
        case AVRCP_KEY_BACKWARD:
            bts_sample_log("AVRCP_KEY_BACKWARD\n");
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            bts_sample_log("AVRCP_EVENT_VOLUME_CHANGED\n");
            break;
        default:
            bts_sample_log("invalid\n");
            break;
    }
}

static void sample_avrcp_ct_release_button_rsp_cbk(const bd_addr_t *bd_addr, int32_t key_operation)
{
    bts_unused(bd_addr);
    bts_sample_log("avrcp ct release button cbk key:%d\n", key_operation);
    if (g_avrcp_srv_sample_inf.avrcp_button_flag == AVRCP_SRV_PRESS_VALUE) {
        bts_sample_log("avrcp ct released button button_flag change:%u->%u\n",
            g_avrcp_srv_sample_inf.avrcp_button_flag,
            AVRCP_SRV_RELEASED_VALUE);
        g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
    }
    sample_avrcp_press_release_button_log((uint32_t)key_operation);
}

static void sample_avrcp_ct_support_company_callback(const bd_addr_t *bd_addr,
                                                     avrcp_ct_support_company_para_t *company)
{
    if (bd_addr == NULL || company == NULL || company->company_id == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("sample_avrcp_ct_get_support_company company list:");
    for (uint8_t i = 0; i < company->company_num; i++) {
        bts_sample_log(" 0x%x ", *company->company_id);
    }
    bts_sample_log("\n");
}

static void sample_avrcp_ct_support_event_callback(const bd_addr_t *bd_addr, avrcp_ct_support_event_para_t *event)
{
    if (bd_addr == NULL || event == NULL || event->event_id == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("sample_avrcp_ct_support_event_callback event list:");
    for (uint8_t i = 0; i < event->event_num; i++) {
        bts_sample_log(" 0x%x ", event->event_id[i]);
    }
    bts_sample_log("\n");
    if (g_avrcp_srv_sample_inf.reg_cap_auto != AVRCP_CT_AUTO_REG_TYPE) {
        return;
    }
    uint8_t avrcp_ct_cap[] = {
        AVRCP_NOTIFY_EVENT_RESERVED,  // 0
        AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED,  // 1
        AVRCP_NOTIFY_EVENT_TRACK_CHANGED,
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,  // 5
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,  // 9
        AVRCP_NOTIFY_EVENT_RESERVED,  // A
        AVRCP_NOTIFY_EVENT_RESERVED,
        AVRCP_NOTIFY_EVENT_RESERVED,  // 0xC
        AVRCP_NOTIFY_EVENT_VOLUME_CHANGED,  // 0xD
    };
    for (uint8_t i = 0; i < event->event_num; i++) {
        if (event->event_id[i] < AVRCP_NOTIFY_EVENT_RESERVED &&
            avrcp_ct_cap[event->event_id[i]] != AVRCP_NOTIFY_EVENT_RESERVED) {
            avrcp_ct_register_notification(bd_addr, event->event_id[i], 0);
            bts_sample_log("avrcp_ct_register_notification auto reg event_id:0x%x\n", event->event_id[i]);
        }
    }
}

static void sample_avrcp_ct_get_element_attri_callback(
    const bd_addr_t *bd_addr, avrcp_ct_element_attr_para_cb_t *attr_cb_para)
{
    if (bd_addr == NULL || attr_cb_para == NULL || attr_cb_para->attr_value == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    for (uint8_t i = 0; i < attr_cb_para->attr_num; i++) {
        bts_sample_log("attrId-charaId-valLen: 0x%x-0x%x-%u \n",
            attr_cb_para->attr_value[i].attr_id,
            attr_cb_para->attr_value[i].character_set_id,
            attr_cb_para->attr_value[i].value_len);
        switch (attr_cb_para->attr_value[i].attr_id) {
            case AVRCP_CT_ELEMENT_ATTR_TITLE:
                bts_sample_log("Attr Title:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_ARTIST_NAME:
                bts_sample_log("Attr Name:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_ALBUM_NAME:
                bts_sample_log("Attr Album:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_TRACK_NUMBER:
                bts_sample_log("Attr Track Number:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_TOTAL_NUMBER_OF_TRACKS:
                bts_sample_log("Attr Total Number Of Track:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_TRACK_GENRE:
                bts_sample_log("Attr Track GENRE:");
                break;
            case AVRCP_CT_ELEMENT_ATTR_TRACK_PLAYING_TIME:
                bts_sample_log("Attr Track Playing Time_ms:");
                break;
            default:
                bts_sample_log("Other:");
                break;
        }
        uint8_t *value = (uint8_t *)(void *)bts_sample_mem_new(attr_cb_para->attr_value[i].value_len + 1);
        if (attr_cb_para->attr_value[i].value_len > 0 &&  attr_cb_para->attr_value[i].value != NULL) {
            if (memcpy_s(value, attr_cb_para->attr_value[i].value_len,
                attr_cb_para->attr_value[i].value, attr_cb_para->attr_value[i].value_len) != EOK) {
                bts_sample_mem_free(value);
                bts_sample_log("memcpy_s fail.");
                return;
            }
        }
        value[attr_cb_para->attr_value[i].value_len] = '\0';
        bts_sample_log(" %s \n", value);
        bts_sample_mem_free(value);
    }
}

static void sample_avrcp_ct_get_play_status_callback(const bd_addr_t *bd_addr, avrcp_ct_play_status_cb_t *status)
{
    if (bd_addr == NULL || status == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("test get play status songlen:%u, songpos:%u, playstatus:%u:",
        status->song_length, status->song_position, status->play_status);
}

static uint8_t sample_avrcp_get_local_play_stat(void)
{
    static uint8_t start_flag = false;
    uint8_t a2dp_play_stat = (uint8_t)a2dp_src_get_playing_state(sample_avrcp_srv_get_active_addr());
    uint8_t play_stat;
    if (a2dp_play_stat == A2DP_IS_PLAYING) {
        play_stat = AVRCP_PLAY_STATUS_PLAYING;
        start_flag = true;
    } else if (start_flag != 0) {
        play_stat = AVRCP_PLAY_STATUS_PAUSED;
    } else {
        play_stat = AVRCP_PLAY_STATUS_STOPPED;
        start_flag = false;
    }
    return play_stat;
}

static void sample_avrcp_set_local_play_stat(uint8_t play_stat)
{
    switch (play_stat) {
        case AVRCP_PLAY_STATUS_PLAYING:
            bts_sample_log("START PLAY\n");
            a2dp_src_start_playing(sample_avrcp_srv_get_active_addr());
            break;
        case AVRCP_PLAY_STATUS_PAUSED:
            bts_sample_log("PAUSED PLAY\n");
            a2dp_src_suspend_playing(sample_avrcp_srv_get_active_addr());
            break;
        case AVRCP_PLAY_STATUS_STOPPED:
            bts_sample_log("STOP PLAY\n");
            a2dp_src_stop_playing(sample_avrcp_srv_get_active_addr());
            break;
        default:
            bts_sample_log("UNKNOWN PLAY\n");
            break;
    }
}

static void __attribute__((unused)) sample_avrcp_ct_hdl_notify_play_status_rsp_cbk(
    avrcp_ct_notification_value_cb_t *notification)
{
    uint8_t play_stat = sample_avrcp_get_local_play_stat();

    switch (notification->play_status) {
        case AVRCP_PLAY_STATUS_STOPPED:
        case AVRCP_PLAY_STATUS_PAUSED:
        case AVRCP_PLAY_STATUS_PLAYING:
            if (notification->play_status != play_stat) {
                sample_avrcp_set_local_play_stat(notification->play_status);
            }
            break;
        case AVRCP_PLAY_STATUS_FWD_SEEK:
            bts_sample_log("CT FWD_SEEK\n");
            break;
        case AVRCP_PLAY_STATUS_REV_SEEK:
            bts_sample_log("CT REV_SEEK\n");
            break;
        default:
            bts_sample_log("invalid operation\n");
            break;
    }
}

static void sample_avrcp_ct_hdl_notify_volume_rsp_cbk(avrcp_ct_notification_value_cb_t *notification)
{
    bts_sample_log("Set local volume:0x%x\n", notification->volume); /* 注册事件后通知的音量 */
}

static void sample_avrcp_ct_notification_callback(
    const bd_addr_t *bd_addr, avrcp_notify_event_t event_id, avrcp_ct_notification_value_cb_t *notification)
{
    if (bd_addr == NULL || notification == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("ct notification cbk event_id:0x%x\n", event_id);
    bool notify_flag = true;
    switch (event_id) {
        case AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED:
            bts_sample_log("tg notify ct playback status changed :%u\n", notification->play_status);
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            sample_avrcp_ct_hdl_notify_volume_rsp_cbk(notification);
            break;
        case AVRCP_NOTIFY_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            bts_sample_log("play conent changed event_id:0x%x\n", event_id);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_CHANGED:
            bts_sample_log("track changed event_id:0x%x\n", event_id);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_END:
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_START:
        case AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED:
        case AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED:
        case AVRCP_NOTIFY_EVENT_AVAILABLE_PLAYERS_CHANGED:
        case AVRCP_NOTIFY_EVENT_ADDRESSED_PLAYER_CHANGED:
        case AVRCP_NOTIFY_EVENT_UIDS_CHANGED:
        case AVRCP_NOTIFY_EVENT_RESERVED:
        default:
            notify_flag = false;
            bts_sample_log("invalid event id event_id:0x%x\n", event_id);
            break;
    }
    if (notify_flag) {
        bts_sample_log("notify reg event_id:%u\n", event_id);
        avrcp_ct_register_notification(bd_addr, event_id, 0); /* TG通知变化后CT重新注册事件 */
    }
}

static void sample_avrcp_tg_connect_state_changed_callback(const bd_addr_t *bd_addr, profile_connect_state_t state)
{
    if (bd_addr == NULL) {
        bts_sample_log("null pointer\n");
        return;
    }
    bts_sample_log("avrcp tg update conn stat:%u\n", state);
    if (memcpy_s(&g_avrcp_srv_sample_inf.bd_addr, BD_ADDR_LEN, bd_addr->addr, BD_ADDR_LEN) != EOK) {
        bts_sample_log("memcpy_s fail.");
        return;
    }
    g_avrcp_srv_sample_inf.avrcp_srv_conn_stat = state;
    bts_sample_log("addr: ****%02x%02x%02x%02x \n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0]); /* 3 2 1 0 idx */
}

uint8_t g_avrcp_tg_local_media_volume = AVRCP_ABSOLUTE_VOLUME_INVALID;
#define SAMPLE_A2DP_MEDIA_VOL_LEVEL 100
#define MEDIA_A2DP_SNK_VOL_INTERFACE 1
#if MEDIA_A2DP_SNK_VOL_INTERFACE
#include "audio_manager_c_wrapper.h"
#endif
static uint8_t avrcp_tg_get_loc_media_volume(void)
{
#if MEDIA_A2DP_SNK_VOL_INTERFACE
    if (g_avrcp_tg_local_media_volume == AVRCP_ABSOLUTE_VOLUME_INVALID) {
        int32_t audio_volume = AudioManagerGetVolume(AUDIO_STREAM_A2DP_MUSIC);
        uint8_t a2dp_volume =
            (uint8_t)(audio_volume * AVRCP_ABSOLUTE_VOLUME_PERCENTAGE_100 / SAMPLE_A2DP_MEDIA_VOL_LEVEL);
        bts_sample_log("audio_volume 0x%xa2dp_volume 0x%x\n", audio_volume, a2dp_volume);
        g_avrcp_tg_local_media_volume = a2dp_volume;
    }
#endif
    return g_avrcp_tg_local_media_volume;
}

static void sample_avrcp_tg_notify_volume_changed_proc(uint8_t a2dp_volume)
{
#if MEDIA_A2DP_SNK_VOL_INTERFACE
    int32_t audio_volume = a2dp_volume * SAMPLE_A2DP_MEDIA_VOL_LEVEL / AVRCP_ABSOLUTE_VOLUME_PERCENTAGE_100;
    AudioManagerSetVolume(AUDIO_STREAM_A2DP_MUSIC, audio_volume); /* 设置耳机增益 */
#endif
    avrcp_tg_notify_volume_changed(a2dp_volume); /* 通知音量变化 */
    g_avrcp_tg_local_media_volume = a2dp_volume; /* 控制端发起事件注册，回复interm时的音量 */
}

static void sample_avrcp_tg_notify_meida_status(const bt_avrcp_tg_evt_status_param *notify_event_status_param)
{
    if (notify_event_status_param == NULL || notify_event_status_param->event_id >= AVRCP_NOTIFY_EVENT_RESERVED) {
        bts_sample_log("param is error");
        return;
    }
    switch (notify_event_status_param->event_id) {
        case AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED:
            avrcp_tg_notify_playback_status_changed(*(uint8_t *)notify_event_status_param->event_status);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_CHANGED:
            avrcp_tg_notify_track_changed(*(uint64_t *)notify_event_status_param->event_status);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_END:
            avrcp_tg_notify_track_reached_end();
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_START:
            avrcp_tg_notify_track_reached_start();
            break;
        case AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED:
            avrcp_tg_notify_playback_pos_changed(*(uint32_t *)notify_event_status_param->event_status);
            break;
        case AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED:
            bts_sample_log("not support\n");
            break;
        case AVRCP_NOTIFY_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            avrcp_tg_notify_now_playing_content_changed();
            break;
        case AVRCP_NOTIFY_EVENT_AVAILABLE_PLAYERS_CHANGED:
            avrcp_tg_notify_available_players_changed();
            break;
        case AVRCP_NOTIFY_EVENT_ADDRESSED_PLAYER_CHANGED:
            avrcp_tg_notify_addressed_players_changed(*(uint16_t *)notify_event_status_param->event_status,
                *(uint16_t *)(notify_event_status_param->event_status + sizeof(uint16_t)));
            break;
        case AVRCP_NOTIFY_EVENT_UIDS_CHANGED:
            avrcp_tg_notify_uid_changed(*(uint16_t *)notify_event_status_param->event_status);
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            sample_avrcp_tg_notify_volume_changed_proc(*(uint8_t *)notify_event_status_param->event_status);
            break;
        case AVRCP_NOTIFY_EVENT_RESERVED:
        default:
            break;
    }
}

static void sample_avrcp_set_notify(bt_avrcp_tg_evt_status_param *notify_event_status_param,
    avrcp_notify_event_t event_id, uint8_t event_status)
{
    notify_event_status_param->event_id = event_id;
    notify_event_status_param->event_status[0] = event_status;
}

static void sample_avrcp_tg_notify_press_button_callback(uint32_t key_operation, uint32_t key_value)
{
    bt_avrcp_tg_evt_status_param param;
    bts_sample_log("sample_avcrp_tg_notify:key_type:%u\n", key_operation);
    sample_avrcp_press_release_button_log(key_operation);
    switch (key_operation) {
        case AVRCP_KEY_VOLUME_UP:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_VOLUME_CHANGED, g_avrcp_srv_sample_inf.loc_volume);
            break;
        case AVRCP_KEY_VOLUME_DOWN:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_VOLUME_CHANGED, g_avrcp_srv_sample_inf.loc_volume);
            break;
        case AVRCP_KEY_MUTE:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_VOLUME_CHANGED, 0x0);
            break;
        case AVRCP_KEY_PLAY:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED, AVRCP_PLAY_STATUS_PLAYING);
            break;
        case AVRCP_KEY_STOP:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED, AVRCP_PLAY_STATUS_STOPPED);
            break;
        case AVRCP_KEY_PAUSE:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED, AVRCP_PLAY_STATUS_PAUSED);
            break;
        case AVRCP_KEY_FORWARD:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED, AVRCP_PLAY_STATUS_FWD_SEEK);
            break;
        case AVRCP_KEY_BACKWARD:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED, AVRCP_PLAY_STATUS_REV_SEEK);
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            bts_sample_log("set absolute volume:%u\n", key_value);
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_VOLUME_CHANGED, (uint8_t)key_value);
            break;
        default:
            sample_avrcp_set_notify(&param, AVRCP_NOTIFY_EVENT_RESERVED, AVRCP_PLAY_STATUS_ERROR);
            break;
    }
    sample_avrcp_tg_notify_meida_status(&param);
}

static void sample_avrcp_tg_get_loc_media_status_callback(bt_avrcp_tg_evt_status_param *event_status_param)
{
    if (event_status_param == NULL) {
        return;
    }
    event_status_param->event_status[0] = 0xff;
    uint8_t stream_stat;
    switch (event_status_param->event_id) {
        case AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_CHANGED:
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_END:
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_START:
            break;
        case AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED:
        case AVRCP_NOTIFY_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_AVAILABLE_PLAYERS_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_ADDRESSED_PLAYER_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_UIDS_CHANGED:
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            event_status_param->event_status[0] = avrcp_tg_get_loc_media_volume();
            break;
        case AVRCP_NOTIFY_EVENT_RESERVED:
        default:
            break;
    }
    bts_sample_log("sample_avrcp_tg_get_loc_media_status_callback event_id:0x%x event_status:%u\n",
        event_status_param->event_id,
        event_status_param->event_status[0]);
}
bt_avrcp_tg_bts_cbk g_sample_avrcp_tg_callbacks = {
    sample_avrcp_tg_notify_press_button_callback,
    sample_avrcp_tg_get_loc_media_status_callback,
};

avrcp_tg_callbacks_t g_sample_avrcp_tg_bts_cbk = {
    sample_avrcp_tg_connect_state_changed_callback,

};

avrcp_ct_callbacks_t g_sample_avrcp_ct_bts_cbk = {
    sample_avrcp_ct_connect_state_changed_callback,
    sample_avrcp_ct_press_button_rsp_cbk,
    sample_avrcp_ct_release_button_rsp_cbk,
    sample_avrcp_ct_support_company_callback,
    sample_avrcp_ct_support_event_callback,
    sample_avrcp_ct_get_element_attri_callback,
    sample_avrcp_ct_get_play_status_callback,
    sample_avrcp_ct_notification_callback,
};

/* 注册回调 */
static void sample_bts_avrcp_srv_register_callbacks(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 2：参数个数 */
        bts_sample_log("avrcpRegisterCallbacks params_cnt error! need: sample_mode fill_callbacks\n");
        return;
    }
    uint32_t sample_mode = (uint32_t)atoi(params[0]);

    uint32_t ret;
    if ((sample_mode & 0x1) != 0) {
        bt_avrcp_tg_bts_cbk sample_avrcp_tg_callbacks = { NULL, NULL };
        bt_avrcp_tg_register_audio_cbk(&sample_avrcp_tg_callbacks);
        ret = (uint32_t)avrcp_tg_register_callbacks(NULL);
        ret |= (uint32_t)avrcp_ct_register_callbacks(NULL);
    } else {
        bt_avrcp_tg_register_audio_cbk(&g_sample_avrcp_tg_callbacks);
        ret = (uint32_t)avrcp_tg_register_callbacks(&g_sample_avrcp_tg_bts_cbk);
        ret |= (uint32_t)avrcp_ct_register_callbacks(&g_sample_avrcp_ct_bts_cbk);
    }

    bts_sample_log("sample_bts_avrcp_srv_register_callbacks ret(int): %x\n", ret);
}

/* 注册回调 */
void wstp_avrcp_register_callbacks(void)
{
    errcode_t ret = ERRCODE_BT_SUCCESS;
    bt_avrcp_tg_register_audio_cbk(&g_sample_avrcp_tg_callbacks);
    ret = (errcode_t)avrcp_tg_register_callbacks(&g_sample_avrcp_tg_bts_cbk);
    ret |= (errcode_t)avrcp_ct_register_callbacks(&g_sample_avrcp_ct_bts_cbk);

    bts_sample_log("avrcp reg cbk ret %x\n", ret);
}

/* 取消回调注册 */
static void sample_avrcp_cmd_disregister_callbacks(const char **params, int32_t params_cnt)
{
    bts_unused(params);
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("sample_avrcp_cmd_disregister_callbacks\n");
        return;
    }
    bt_avrcp_tg_bts_cbk tg_bts_cbk = { NULL, NULL };
    bt_avrcp_tg_register_audio_cbk(&tg_bts_cbk);
    (void) avrcp_tg_register_callbacks(NULL);
    bts_sample_log("sample_avrcp_cmd_disregister_callback\n");
}

/* 取消回调注册 */
void wstp_avrcp_disregister_callbacks(void)
{
    bt_avrcp_tg_bts_cbk tg_bts_cbk = { NULL, NULL };
    bt_avrcp_tg_register_audio_cbk(&tg_bts_cbk);
    (void) avrcp_ct_register_callbacks(NULL);
    bts_sample_log("sample_avrcp_cmd_disregister_callback\n");
}

/* 获取avrcp 设备连接状态 */
static void sample_avrcp_tg_cmd_get_device_connect_state(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("sample_avrcp_tg_cmd_get_device_connect_state params_cnt error! need: sample_mode addr\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("sample_avrcp_tg_cmd_get_device_connect_state decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = avrcp_tg_get_device_connect_state(NULL);
    } else {
        ret = avrcp_tg_get_device_connect_state(&bd_addr);
    }

    bts_sample_log("avrcp_tg_get_device_connect_state state(int): %d\n", ret);
}

/* 获取avrcp 设备连接状态 */
int wstp_avrcp_tg_get_dev_conn_state(const bd_addr_t *bd_addr)
{
    int ret = avrcp_tg_get_device_connect_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

/* 获取设备播放状态 */
static void sample_avrcp_tg_cmd_set_loc_inf(const char **params, int32_t params_cnt)
{
    if (params_cnt != BT_NUM4) { /* 2：参数个数 */
        bts_sample_log("please inprt stream_hdl, loc_volue, dir_forward\n");
        return;
    }
    g_avrcp_srv_sample_inf.stream_hdl = 0;
    if (sscanf_s(params[1], "%x", &g_avrcp_srv_sample_inf.stream_hdl) <= 0) {
        bts_sample_log("sample_bt_audio_hal_cmd_start_stream sscanf fail\n");
        return;
    }
    g_avrcp_srv_sample_inf.loc_volume = (uint8_t)atoi(params[POS_2]);
    g_avrcp_srv_sample_inf.dir_forward = (uint8_t)atoi(params[POS_3]);
}

/* 设置设备播放状态 */
void wstp_avrcp_tg_set_loc_inf(const uint8_t *params, int32_t params_cnt)
{
    if (params_cnt != 4) { /* 4：参数个数 */
        bts_sample_log("please inprt stream_hdl, loc_volue, dir_forward\n");
        return;
    }
    g_avrcp_srv_sample_inf.stream_hdl = (td_pvoid)(uintptr_t)params[POS_0];
    g_avrcp_srv_sample_inf.loc_volume = (uint8_t)params[POS_1];
    g_avrcp_srv_sample_inf.dir_forward = (uint8_t)params[POS_2];
}

static void sample_avrcp_tg_notify_meida_status_shell(const char **params)
{
    uint8_t event_id = (uint8_t)(uint32_t)atoi(params[1]);
    uint64_t identifier;
    switch (event_id) {
        case AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED:
            avrcp_tg_notify_playback_status_changed ((uint8_t)(uint32_t)atoi(params[POS_2]));
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_CHANGED:
            identifier = ((uint64_t)((uint32_t)atoi(params[POS_2])) << AVRCP_SHIFT_BITS_32) +
                         ((uint64_t)(uint32_t)atoi(params[POS_3]));
            avrcp_tg_notify_track_changed(identifier);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_END:
            avrcp_tg_notify_track_reached_end();
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_START:
            avrcp_tg_notify_track_reached_start();
            break;
        case AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED:
            avrcp_tg_notify_playback_pos_changed((uint32_t)atoi(params[POS_2]));
            break;
        case AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED:
            bts_sample_log("not support\n");
            break;
        case AVRCP_NOTIFY_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            avrcp_tg_notify_now_playing_content_changed();
            break;
        case AVRCP_NOTIFY_EVENT_AVAILABLE_PLAYERS_CHANGED:
            avrcp_tg_notify_available_players_changed();
            break;
        case AVRCP_NOTIFY_EVENT_ADDRESSED_PLAYER_CHANGED:
            avrcp_tg_notify_addressed_players_changed((uint16_t)(uint32_t)atoi(params[POS_2]),
                (uint16_t)(uint32_t)atoi(params[POS_3]));
            break;
        case AVRCP_NOTIFY_EVENT_UIDS_CHANGED:
            avrcp_tg_notify_uid_changed ((uint16_t)(uint32_t)atoi(params[POS_2]));
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            avrcp_tg_notify_volume_changed ((uint8_t)(uint32_t)atoi(params[POS_2]));
            break;
        default:
            bts_sample_log("unknown event\n");
            break;
    }
}

static void sample_avrcp_tg_cmd_notify_state(const char **params, int32_t params_cnt)
{
    if (params_cnt < 4 || params == NULL) { /* 4：参数个数 */
        bts_sample_log("error param\n");
        return;
    }
    char *end;
    uint8_t event_id = (uint8_t)(uint32_t)strtol(params[1], &end, 10); // 10:base
    if (end == params[1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)event_id == LONG_MIN || (long)event_id == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    if (event_id >= AVRCP_NOTIFY_EVENT_RESERVED) {
        bts_sample_log("event id is error");
        return;
    }
    sample_avrcp_tg_notify_meida_status_shell(params);
}

static void wstp_avrcp_tg_notify_meida_status(const uint8_t *params)
{
    uint8_t event_id = params[POS_0];
    if (event_id >= AVRCP_NOTIFY_EVENT_RESERVED) {
        bts_sample_log("event id is error\n");
        return;
    }
    uint64_t identifier;
    switch (event_id) {
        case AVRCP_NOTIFY_EVENT_PLAYBACK_STATUS_CHANGED:
            avrcp_tg_notify_playback_status_changed(params[POS_1]);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_CHANGED:
            identifier = ((uint64_t)params[POS_1] << AVRCP_SHIFT_BITS_32) + (uint64_t)params[POS_2];
            avrcp_tg_notify_track_changed(identifier);
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_END:
            avrcp_tg_notify_track_reached_end();
            break;
        case AVRCP_NOTIFY_EVENT_TRACK_REACHED_START:
            avrcp_tg_notify_track_reached_start();
            break;
        case AVRCP_NOTIFY_EVENT_PLAYBACK_POS_CHANGED:
            avrcp_tg_notify_playback_pos_changed(params[POS_1]);
            break;
        case AVRCP_NOTIFY_EVENT_BATT_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_SYSTEM_STATUS_CHANGED:
        case AVRCP_NOTIFY_EVENT_PLAYER_APPLICATION_SETTING_CHANGED:
            bts_sample_log("unsupport event 0x%x\n", event_id);
            break;
        case AVRCP_NOTIFY_EVENT_NOW_PLAYING_CONTENT_CHANGED:
            avrcp_tg_notify_now_playing_content_changed();
            break;
        case AVRCP_NOTIFY_EVENT_AVAILABLE_PLAYERS_CHANGED:
            avrcp_tg_notify_available_players_changed();
            break;
        case AVRCP_NOTIFY_EVENT_ADDRESSED_PLAYER_CHANGED:
            avrcp_tg_notify_addressed_players_changed(params[POS_1], params[POS_2]);
            break;
        case AVRCP_NOTIFY_EVENT_UIDS_CHANGED:
            avrcp_tg_notify_uid_changed(params[POS_1]);
            break;
        case AVRCP_NOTIFY_EVENT_VOLUME_CHANGED:
            sample_avrcp_tg_notify_volume_changed_proc(params[POS_1]);
            break;
        case AVRCP_NOTIFY_EVENT_RESERVED:
        default:
            bts_sample_log("unknown event 0x%x\n", event_id);
            break;
    }
}

void wstp_avrcp_tg_notify_state(const uint8_t *params, int32_t params_cnt)
{
    if (params_cnt < 3 || params == NULL) { /* 3：参数个数 */
        bts_sample_log("error param\n");
        return;
    }
    bts_sample_log("event_id:0x%x param1:0x%x param2:0x%x\n", params[POS_0], params[POS_1], params[POS_2]);

    wstp_avrcp_tg_notify_meida_status(params);
}

/* 与设备建立AVRCP连接 */
static void sample_avrcp_tg_cmd_connect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("a2dp_src_connect params_cnt error! need: sample_mode addr\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("a2dp_src_connect decode addr error!\n");
        return;
    }

    /* 初始化要在AVRCP连接之前调用 */

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = avrcp_tg_connect(NULL);
    } else {
        g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
        ret = avrcp_tg_connect(&bd_addr);
    }

    bts_sample_log("avrcp_tg_connect ret(int): %x\n", ret);
}

/* 与设备建立AVRCP连接 */
int wstp_avrcp_tg_connect(const bd_addr_t *bd_addr)
{
    int ret = avrcp_tg_connect(bd_addr);
    bts_sample_log("avrcp_tg_connect ret(int): %x\n", ret);
    return ret;
}

/* 与设备断开AVRCP连接 */
static void sample_avrcp_tg_cmd_disconnect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("a2dp_src_disconnect params_cnt error! need: sample_mode addr\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("a2dp_src_disconnect decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = avrcp_tg_disconnect(NULL);
    } else {
        ret = avrcp_tg_disconnect(&bd_addr);
    }
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
    bts_sample_log("avrcp_tg_disconnect ret(int): %x\n", ret);
}

/* 与设备断开AVRCP连接 */
int wstp_avrcp_tg_disconnect(const bd_addr_t *bd_addr)
{
    int ret = avrcp_tg_disconnect(bd_addr);
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
    bts_sample_log("avrcp_tg_disconnect ret(int): %x\n", ret);
    return ret;
}

static void sample_avrcp_ct_conn_and_reg(const bd_addr_t *bd_addr)
{
    bts_sample_log("test mode auto reg\n");
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
    g_avrcp_srv_sample_inf.reg_cap_auto = AVRCP_CT_AUTO_REG_TYPE;
    int32_t ret = avrcp_ct_connect(bd_addr);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_log("connect ret(int): %x\n", ret);
        return;
    }

    (void) avrcp_ct_get_supported_events(sample_avrcp_srv_get_active_addr());
}

void wstp_avrcp_ct_conn_and_reg(const bd_addr_t *bd_addr)
{
    bts_sample_log("test mode auto reg\n");
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
    g_avrcp_srv_sample_inf.reg_cap_auto = AVRCP_CT_AUTO_REG_TYPE;
    int32_t ret = avrcp_ct_connect(bd_addr);
    if (ret != ERRCODE_BT_SUCCESS) {
        bts_sample_log("connect ret(int): %x\n", ret);
        return;
    }

    (void) avrcp_ct_get_supported_events(bd_addr);
}

/* 与设备建立AVRCP连接 */
static void sample_avrcp_ct_cmd_connect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("a2dp_src_connect params_cnt error! need: sample_mode addr\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("avrcp decode addr error!\n");
        return;
    }
    bts_sample_log("test mode:%u\n", sample_mode & 0xff);
    /* 初始化要在AVRCP连接之前调用 */
    int ret = 0;
    if ((sample_mode & 0xff) == 1) {
        ret = avrcp_ct_connect(NULL);
    } else if ((sample_mode & 0xff) == 2) { /* mode 2 */
        g_avrcp_srv_sample_inf.reg_cap_auto = AVRCP_CT_AUTO_REG_NON;
        g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;
        ret = avrcp_ct_connect(&bd_addr);
    } else {
        sample_avrcp_ct_conn_and_reg(&bd_addr);
        return;
    }

    bts_sample_log("Ct Connect ret(int): %x\n", ret);
}

/* 与设备断开AVRCP连接 */
static void sample_avrcp_ct_cmd_disconnect(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("a2dp_src_disconnect params_cnt error! need: sample_mode addr\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    bd_addr_t bd_addr;
    if (decode_bd_addr_from_str(params[1], bd_addr.addr, BD_ADDR_LEN) != EXT_ERR_SUCCESS) {
        bts_sample_log("a2dp_src_disconnect decode addr error!\n");
        return;
    }

    int ret;
    if ((sample_mode & 0x1) != 0) {
        ret = avrcp_ct_disconnect(NULL);
    } else {
        ret = avrcp_ct_disconnect(&bd_addr);
    }
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;

    bts_sample_log("Ct Disconnect ret(int): %x\n", ret);
}

/* 与设备断开AVRCP连接 */
int wstp_avrcp_ct_disconnect(const bd_addr_t *bd_addr)
{
    int ret = avrcp_ct_disconnect(bd_addr);
    g_avrcp_srv_sample_inf.avrcp_button_flag = AVRCP_SRV_RELEASED_VALUE;

    bts_sample_log("Ct Disconnect ret(int): %x\n", ret);
    return ret;
}

static void sample_avrcp_ct_press_button(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("please input test mode, key operation:0x41~0x7f\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    int32_t key_operation = (int32_t)strtol(params[1], &end, 10); // 10:base
    if (end == params[1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)key_operation == LONG_MIN || (long)key_operation == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    if ((sample_mode & 0x1) != 0) {
        (void) avrcp_ct_press_button(NULL, key_operation);
        (void) avrcp_ct_release_button(NULL, key_operation);
        return;
    }
    (void) avrcp_ct_press_button(&g_avrcp_srv_sample_inf.bd_addr, key_operation);
    uint8_t sleep_cnt = 0;
    while (1) {
        osal_msleep(100);  /* sleep 1000 ms */
        if (g_avrcp_srv_sample_inf.avrcp_button_flag == AVRCP_SRV_PRESS_VALUE) {
            (void) avrcp_ct_release_button(&g_avrcp_srv_sample_inf.bd_addr, key_operation);
            break;
        }
        sleep_cnt++;
        if (sleep_cnt > 20) { /* sleep 20 times */
            bts_sample_log("not receive rsp press button msg\n");
            break;
        }
    }
}

void wstp_avrcp_ct_press_button(const bd_addr_t *bd_addr, int32_t key_operation)
{
    (void)avrcp_ct_press_button(bd_addr, key_operation);
    uint8_t sleep_cnt = 0;
    while (1) {
        osal_msleep(100);  /* sleep 1000 ms */
        if (g_avrcp_srv_sample_inf.avrcp_button_flag == AVRCP_SRV_PRESS_VALUE) {
            (void)avrcp_ct_release_button(bd_addr, key_operation);
            break;
        }
        sleep_cnt++;
        if (sleep_cnt > 20) { /* sleep 20 times */
            bts_sample_log("not receive rsp press button msg\n");
            break;
        }
    }
}

static void sample_avrcp_ct_set_abs_volume(const char **params, int32_t params_cnt)
{
    if (params_cnt != 2) { /* 2：参数个数 */
        bts_sample_log("please inprt sample_mode, volume:0~0x7F\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    uint8_t volume = (uint8_t)(uint32_t)strtol(params[1], &end, 10); // 10:base
    if (end == params[1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)volume == LONG_MIN || (long)volume == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    if ((sample_mode & 0x1) != 0) {
        (void) avrcp_ct_set_absolute_volume(NULL, volume);
        return;
    }
    (void) avrcp_ct_set_absolute_volume(&g_avrcp_srv_sample_inf.bd_addr, volume);
}

int wstp_bts_avrcp_ct_set_abs_volume(const bd_addr_t *bd_addr, uint8_t volume)
{
    return avrcp_ct_set_absolute_volume(bd_addr, volume);
}

static void sample_avrcp_ct_get_tg_player_stat(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 2：参数个数 */
        bts_sample_log("please inprt test mode\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    if ((sample_mode & 0x1) != 0) {
        (void) avrcp_ct_get_play_status(NULL);
        return;
    }
    (void) avrcp_ct_get_play_status(&g_avrcp_srv_sample_inf.bd_addr);
}

int wstp_avrcp_ct_get_tg_player_stat(const bd_addr_t *bd_addr)
{
    return avrcp_ct_get_play_status(bd_addr);
}

static void sample_avrcp_ct_get_element_attri(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 2：参数个数 */
        bts_sample_log("please inprt test mode\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    if ((sample_mode & 0x3) == 0) {
        (void) avrcp_ct_get_element_attributes(NULL, NULL);
        return;
    }
    if ((sample_mode & 0x3) == 1) {
        uint8_t list[BTS_ATTR_LIST_LEN] = { 0x1, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x00 };
        avrcp_ct_get_element_attr_para_t element_attr_param;

        element_attr_param.attr_num = (unsigned char)sizeof(list) / BTS_ATTR_ID_LEN;
        element_attr_param.attr_id_list =
            (unsigned int *)(void *)bts_sample_mem_new(sizeof(unsigned int) * element_attr_param.attr_num);
        if (memcpy_s(element_attr_param.attr_id_list, BTS_ATTR_LIST_LEN, list, BTS_ATTR_LIST_LEN) != EOK) {
            bts_sample_log("memcpy_s fail.");
            return;
        }
        avrcp_ct_get_element_attributes(&g_avrcp_srv_sample_inf.bd_addr, &element_attr_param);
        if (element_attr_param.attr_id_list != NULL) {
            bts_sample_mem_free(element_attr_param.attr_id_list);
            element_attr_param.attr_id_list = NULL;
        }
        return;
    }
}

void wstp_avrcp_ct_get_element_attributes(const bd_addr_t *bd_addr)
{
    avrcp_ct_get_element_attr_para_t element_attr_param;
    element_attr_param.attr_num = AVRCP_CT_ELEMENT_ATTR_TRACK_PLAYING_TIME;
    element_attr_param.attr_id_list =
        (unsigned int *)(void *)bts_sample_mem_new(sizeof(unsigned int) * element_attr_param.attr_num);
    for (int i = 0; i < element_attr_param.attr_num; i++) {
        element_attr_param.attr_id_list[i] = (unsigned int)i + 1;
    }
    avrcp_ct_get_element_attributes(bd_addr, &element_attr_param);
    if (element_attr_param.attr_id_list != NULL) {
        bts_sample_mem_free(element_attr_param.attr_id_list);
        element_attr_param.attr_id_list = NULL;
    }
}

static void sample_avrcp_ct_get_support_event(const char **params, int32_t params_cnt)
{
    if (params_cnt != 1) { /* 2：参数个数 */
        bts_sample_log("please input sample_mode\n");
        return;
    }
    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    if ((sample_mode & 0x1) != 0) {
        (void) avrcp_ct_get_supported_events(NULL);
        return;
    }
    (void) avrcp_ct_get_supported_events(sample_avrcp_srv_get_active_addr());
}

int wstp_avrcp_ct_get_support_event(const bd_addr_t *bd_addr)
{
    return avrcp_ct_get_supported_events(bd_addr);
}

static void sample_avrcp_ct_reg_support_event(const char **params, int32_t params_cnt)
{
    if (params_cnt < BT_NUM3 || params_cnt > MAX_SAMPLE_AVRCP_CT_REG_CNT + BT_NUM2) { /* 7：参数个数 */
        bts_sample_log("please input sample_mode\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    uint32_t interval = (uint32_t)strtol(params[params_cnt - 1], &end, 10); // 10:base
    if (end == params[params_cnt - 1] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)interval == LONG_MIN || (long)interval == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }

    uint8_t len = (uint8_t)params_cnt - 2;
    bts_sample_log("avrcp ct reg event cnt:%u, interval:%u event list:\n", len, interval);
    for (uint8_t i = 1; i <= len; i++) {
        uint8_t event_id = (uint8_t)(uint32_t)strtol(params[i], &end, 10); // 10:base
        if (end == params[i] || *end != '\0') {
            bts_sample_log("string to long int error!\r\n");
            return;
        } else if (((long)event_id == LONG_MIN || (long)event_id == LONG_MAX) && (errno == ERANGE)) {
            bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
            return;
        }
        if ((sample_mode & 0x1) != 0) {
            (void) avrcp_ct_register_notification(NULL, 0, 0);
            return;
        }
        (void) avrcp_ct_register_notification(sample_avrcp_srv_get_active_addr(), event_id, interval);
        bts_sample_log("event_id:0x%x\n", event_id);
    }
}

void wstp_avrcp_ct_reg_support_event(const bd_addr_t *bd_addr, uint8_t event_id, uint32_t interval)
{
    bts_sample_log("avrcp ct reg event addr: ****%02x%02x%02x%02x event_id:0x%x interval 0x%x\n",
        bd_addr->addr[3], bd_addr->addr[2], bd_addr->addr[1], bd_addr->addr[0], event_id, interval); /* 3 2 1 0 idx */
    (void)avrcp_ct_register_notification(sample_avrcp_srv_get_active_addr(), event_id, interval);
}

static void sample_avrcp_get_conn_stat(const char **params, int32_t params_cnt)
{
    if (params_cnt < 1) { /* 7：参数个数 */
        bts_sample_log("please input sample_mode\n");
        return;
    }

    char *end;
    uint32_t sample_mode = (uint32_t)strtol(params[0], &end, 10); // 10:base
    if (end == params[0] || *end != '\0') {
        bts_sample_log("string to long int error!\r\n");
        return;
    } else if (((long)sample_mode == LONG_MIN || (long)sample_mode == LONG_MAX) && (errno == ERANGE)) {
        bts_sample_log("string to long int error, maybe out of range or ERANGE\r\n");
        return;
    }
    if ((sample_mode & 0x1) != 0) {
        (void) avrcp_ct_get_device_connect_state(NULL);
        return;
    }
    int32_t conn_stat = avrcp_ct_get_device_connect_state(&g_avrcp_srv_sample_inf.bd_addr);
    bts_sample_log("avrcp conn stat:%d\n", conn_stat);
}

int wstp_avrcp_ct_get_conn_stat(const bd_addr_t *bd_addr)
{
    int ret = avrcp_ct_get_device_connect_state(bd_addr);
    bts_sample_log("state(int): %d\n", ret);
    return ret;
}

int wstp_avrcp_ct_get_support_companies(const bd_addr_t *bd_addr)
{
    return avrcp_ct_get_supported_companies(bd_addr);
}

btsrv_sample_cmd_hdl_info g_avrcp_sample_hdls[] = {
    /* 0 */
    { SAMPLE_AVRCP_TG_ID_REGISTER_CALLBACK,   sample_bts_avrcp_srv_register_callbacks },
    { SAMPLE_AVRCP_TG_ID_DEREGISTER_CALLBACK, sample_avrcp_cmd_disregister_callbacks },
    { SAMPLE_AVRCP_TG_ID_CONNECT,             sample_avrcp_tg_cmd_connect },
    { SAMPLE_AVRCP_TG_ID_DISCONNECT,          sample_avrcp_tg_cmd_disconnect },
    { SAMPLE_AVRCP_TG_ID_GET_CONN_STATE,      sample_avrcp_tg_cmd_get_device_connect_state },
    { SAMPLE_AVRCP_TG_ID_NOTIFY,              sample_avrcp_tg_cmd_notify_state },

    /* 5 */
    { SAMPLE_AVRCP_TG_ID_SET_LOC_STATE,      sample_avrcp_tg_cmd_set_loc_inf },
    { SAMPLE_AVRCP_CT_ID_CONNECT,            sample_avrcp_ct_cmd_connect },
    { SAMPLE_AVRCP_CT_ID_DISCONNECT,         sample_avrcp_ct_cmd_disconnect },
    { SAMPLE_AVRCP_CT_ID_PRESS_BUTTON,       sample_avrcp_ct_press_button },
    { SAMPLE_AVRCP_CT_ID_SET_VOLUME,         sample_avrcp_ct_set_abs_volume },
    { SAMPLE_AVRCP_CT_ID_GET_TG_PLAYER_STAT, sample_avrcp_ct_get_tg_player_stat },
    { SAMPLE_AVRCP_CT_ID_GET_ELEMENT_ATTR,   sample_avrcp_ct_get_element_attri },
    { SAMPLE_AVRCP_CT_ID_GET_EVENT_CAP,      sample_avrcp_ct_get_support_event },
    { SAMPLE_AVRCP_CT_ID_REG_EVENT_CAP,      sample_avrcp_ct_reg_support_event },
    { SAMPLE_AVRCP_CT_ID_GET_CONN_STAT,      sample_avrcp_get_conn_stat },

};

void bt_avrcp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt)
{
    uint32_t hdl_cnt = sizeof(g_avrcp_sample_hdls) / sizeof(btsrv_sample_cmd_hdl_info);

    for (uint32_t i = 0; i < hdl_cnt; i++) {
        if (g_avrcp_sample_hdls[i].id == cmd_id && g_avrcp_sample_hdls[i].func != NULL) {
            g_avrcp_sample_hdls[i].func(params, params_cnt);
            return;
        }
    }

    bts_sample_log("bt_avrcp_service_sample_cmd cmd_id %u not support now!\n", cmd_id);
}
