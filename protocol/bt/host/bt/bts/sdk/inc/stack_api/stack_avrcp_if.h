/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_AVRCP_IF_H
#define STATCK_AVRCP_IF_H

#include "ipc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************** Structure Definition ******************/
/* AVRCP specific structure for member 'ext_attributes' in _BtSdkRemoteServiceAttrStru */
typedef struct {
    uint32_t size;                      /* Size of bt_sdk_rmt_avrcp_svc_ext_attr_stru */
    uint16_t mask;                      /* Specifies whether an optional attribute value is available */
    uint16_t profile_ver;               /* Profile version number, e.g. BTSDK_AVRCP_VER_1_0 */
    uint16_t supported_features;        /* Supported features, bit masks of e.g. BTSDK_AVRCP_FEATURES_CATEGORY_1 */
} bt_sdk_rmt_avrcp_svc_ext_attr_stru, *p_bt_sdk_rmt_avrcp_svc_ext_attr_stru;

/* DEPRECATED, reserve for the old version BlueSoleil. */
typedef struct {
    uint32_t       dev_hdl;            /* Handle to the peer device */
    uint8_t        state_flag;      /* Button state(0: pressed 1: released) */
    uint8_t        op_id;           /* Pass through command ID */
    uint8_t        length;          /* Length of op_data, Always 0 */
    uint8_t        op_data[1];      /* Additional parameter data, ignored */
} bt_sdk_pass_thr_req_stru, *p_bt_sdk_pass_thr_req_stru;

/* DEPRECATED, reserve for the old version BlueSoleil. */
typedef struct {
    uint32_t    dev_hdl;             /* Handle to the peer device */
    uint8_t     tl;
    uint8_t     state_flag;          /* Button state(0: pressed 1: released) */
    uint8_t     op_id;               /* Pass through command ID */
    uint8_t     length;              /* Length of op_data, Always 0 */
    uint8_t     op_data[1];          /* Additional parameter data, ignored */
} bt_sdk_pass_thr_rsp_stru, *p_bt_sdk_pass_thr_rsp_stru;

/* DEPRECATED, reserve for the old version BlueSoleil. */
typedef struct {
    uint32_t    dev_hdl;             /* Handle to the peer device */
    uint8_t     state_flag;          /* Button state(0: pressed 1: released) */
    uint16_t    vendor_unique_id;    /* Vendor Unique Operation IDs */
} bt_sdk_group_navi_req_stru, *p_bt_sdk_group_navi_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GENERAL_REJECT_RSP */
typedef struct {
    uint32_t    size;            /* Size of this structure */
    /* type of the command being rejected, e.g. BTSDK_APP_EV_AVRCP_GET_CAPABILITIES_IND */
    uint16_t    cmd_type;
    uint8_t     error_code;       /* Error code, e.g. BTSDK_AVRCP_ERROR_INVALID_COMMAND */
} bt_sdk_general_reject_rsp_stru, *p_bt_sdk_general_reject_rsp_stru;

typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     state_flag;          /* Button state(BTSDK_AVRCP_BUTTON_STATE_PRESSED: pressed
                                                    BTSDK_AVRCP_BUTTON_STATE_RELEASED: released) */
    uint8_t     op_id;               /* Pass through command ID */
    uint16_t    vendor_unique_id;     /* Vendor Unique Operation IDs
                                            When the op_id == BTSDK_AVRCP_OPID_VENDORUNIQUE, should be
                                                BTSDK_AVRCP_BGN_NEXTGROUP:        Next Group
                                                BTSDK_AVRCP_BGN_PREVIOUSGROUP:    Previous Group
                                            Other always 0 */
} bt_sdk_pass_through_stru, *p_bt_sdk_pass_through_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_CAPABILITIES_IND,
   BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_VALUES_IND,
   BTSDK_APP_EV_AVRCP_INFORM_BATTERYSTATUS_OF_CT_IND, BTSDK_APP_EV_AVRCP_SET_ABSOLUTE_VOLUME_IND,
   BTSDK_APP_EV_AVRCP_PLAY_ITEM_RSP, BTSDK_APP_EV_AVRCP_ADDTO_NOWPLAYING_RSP,
   BTSDK_APP_EV_AVRCP_SET_ABSOLUTE_VOLUME_RSP,
   BTSDK_APP_EV_AVRCP_SET_ADDRESSED_PLAYER_RSP, BTSDK_APP_EV_AVRCP_GENERAL_REJECT_RSP */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     id;                 /* ID of the item requested, e.g. CapabilityID of the GetCapabilites request */
} bt_sdk_id_stru, *p_bt_sdk_id_stru,
  bt_sdk_get_capabilities_req_stru, *p_bt_sdk_get_capabilities_req_stru,
  bt_sdk_list_player_app_set_val_req_stru, *p_bt_sdk_list_player_app_set_val_req_stru,
  bt_sdk_inform_batt_status_req_stru, *p_bt_sdk_inform_batt_status_req_stru,
  bt_sdk_set_absolute_vol_req_stru, *p_bt_sdk_set_absolute_vol_req_stru,
  bt_sdk_play_item_rsp_stru, *p_bt_sdk_play_item_rsp_stru,
  bt_sdk_add_to_now_playing_rsp_stru, *p_bt_sdk_add_to_now_playing_rsp_stru,
  bt_sdk_set_absolute_vol_rsp_stru, *p_bt_sdk_set_absolute_vol_rsp_stru,
  bt_sdk_set_addresed_player_rsp_stru, *p_bt_sdk_set_addresed_player_rsp_stru,
  bt_sdk_get_total_num_of_items_req_stru, *p_bt_sdk_get_total_num_of_items_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_SET_ADDRESSED_PLAYER_IND, BTSDK_APP_EV_AVRCP_SET_BROWSED_PLAYER_IND */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint16_t    id;                 /* Player ID */
} bt_sdk_size2id_stru, *p_bt_sdk_size2id_stru,
  bt_sdk_set_addresed_player_req_stru, *p_bt_sdk_set_addresed_player_req_stru,
  bt_sdk_set_browsed_player_req_stru, *p_bt_sdk_set_browsed_player_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_CAPABILITIES_RSP */
typedef struct {
    uint32_t    size;                   /* Size of this structure, including the full content of capability */
    uint8_t     capability_id;            /* ID of capability requested */
    uint8_t     count;                    /* Number of CompanyID or EventID */
    uint8_t     capability[1];            /* List of CompanyID or EventID
                                           3...3*n for COMPANY_ID,
                                           2...n for EVENTS_SUPPORTED
                                        */
} bt_sdk_get_capabilities_rsp_stru, *p_bt_sdk_get_capabilities_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_ATTR_RSP,
   BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_VALUES_RSP,
   BTSDK_APP_EV_AVRCP_GET_CURRENTPLAYER_SETTING_VALUE_RSP, BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_ATTR_TEXT_RSP */
typedef struct {
    uint32_t    size;                   /* Size of this structure, including the full content of id */
    uint8_t     num;                    /* Number of ID provided */
    uint8_t     id[1];                  /* List of ID, 2...n for ID */
} bt_sdk_num_id_stru, *p_bt_sdk_num_id_stru,
  bt_sdk_list_player_app_set_attr_rsp_stru, *p_bt_sdk_list_player_app_set_attr_rsp_stru,
  bt_sdk_list_player_app_set_val_rsp_stru, *p_bt_sdk_list_player_app_set_val_rsp_stru,
  bt_sdk_get_cur_player_app_set_val_req_stru, *p_bt_sdk_get_cur_player_app_set_val_req_stru,
  bt_sdk_get_player_app_set_attr_txt_req_stru, *p_bt_sdk_get_player_app_set_attr_txt_req_stru;

/* Player Application Setting Parameter */
typedef struct {
    uint8_t     attr_id;                 /* Player Application Setting Arrtribute ID */
    uint8_t     value_id;                /* Player Application Setting Value ID */
} bt_sdk_id_pair_stru, *p_bt_sdk_id_pair_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_SET_CURRENTPLAYER_SETTING_VALUE_IND,
   BTSDK_APP_EV_AVRCP_GET_CURRENTPLAYER_SETTING_VALUE_RSP */
typedef struct {
    uint32_t    size;                   /* Size of this structure, including the full content of id */
    uint8_t     num;                    /* Number of attributes */
    bt_sdk_id_pair_stru id[1];              /* List of the requested attribues ID and value ID */
} bt_sdk_num_id_pair_stru, *p_bt_sdk_num_id_pair_stru,
  bt_sdk_set_cur_player_app_set_val_req_stru, *p_bt_sdk_set_cur_player_app_set_val_req_stru,
  bt_sdk_get_cur_player_app_set_val_rsp_stru, *p_bt_sdk_get_cur_player_app_set_val_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_VALUE_TEXT_IND */
typedef struct {
    uint32_t    size;                   /* Size of this structure, including the full content of value_id */
    uint8_t     attr_id;                 /* Attribute ID */
    uint8_t     num;                    /* Number of Value IDs */
    uint8_t     value_id[1];             /* List of Value IDs */
} bt_sdk_id_num_id_stru, *p_bt_sdk_id_num_id_stru,
  bt_sdk_get_player_app_set_val_txt_req_stru, *p_bt_sdk_get_player_app_set_val_txt_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_INFORM_CHARACTERSET_IND */
typedef struct {
    uint32_t    size;
    uint8_t     num;                     /* number of CharacterSet IDs */
    uint16_t    characterset_id[1];       /* List of CharacterSet IDs */
} bt_sdk_num_csid_stru, *p_bt_sdk_num_csid_stru,
  bt_sdk_inform_char_set_req_stru, *p_bt_sdk_inform_char_set_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_ELEMENT_ATTRIBUTES_IND */
typedef struct {
    uint32_t    size;
    uint8_t     identifier[8];            /* Unique identifier to identify an element on TG */
    uint8_t     num;                      /* Number of attr_id */
    uint32_t    attr_id[1];                /* Attributes ID */
} bt_sdk_id_num4id_stru, *p_bt_sdk_id_num4id_stru,
  bt_sdk_get_element_attr_req_stru, *p_bt_sdk_get_element_attr_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_PLAY_STATUS_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    song_length;               /* The total length of the playing song in milliseconds */
    uint32_t    song_position;             /* The current position of the playing in milliseconds elapsed */
    uint8_t     play_status;               /* Current status of Playing */
} bt_sdk_play_status_rsp_stru, *p_bt_sdk_play_status_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_CHANGE_PATH_IND */
typedef struct {
    uint32_t    size;
    uint16_t    uid_counter;
    uint8_t     direction;
    uint8_t     folder_uid[8];
} bt_sdk_change_path_req_stru, *p_bt_sdk_change_path_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_CHANGE_PATH_RSP */
typedef struct {
    uint32_t    size;
    uint8_t     status;          /* The Result of the SetBrowsedPlayer operation.
                                    if an error has occurred then this is the only field present in the response. */
    uint32_t    items_num;        /* The number of items in the folder which has been changed to,
                                    ie the new current folder */
} bt_sdk_change_path_rsp_stru, *p_bt_sdk_change_path_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_FOLDER_ITEMS_IND */
typedef struct {
    uint32_t    size;
    uint8_t     scope;         /* e.g AVRCP_SCOPE_MEDIAPLAYER_LIST */
    uint32_t    start_item;     /* The offset within the listing of the item which should be the first returned item.
                                  The first media element in the listing is at offset 0. */
    uint32_t    end_item;       /* The offset within the listing of the item which should be the final returned. */
    uint8_t     attr_count;     /* The number of the requested attributes.
                                  0x00            All    attributes are requested. There is no following Attribute
                                  List.
                                  0x01-0xFE    The following Attribute List contains this number of attributes.
                                  0xFF            No attributes are requested. There is no following Attribute List.
                                */
    uint32_t    attr_id[1];     /* Attributes which are requested to be returned for each item returned. */
} bt_sdk_get_folder_item_req_stru, *p_bt_sdk_get_folder_item_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_ITEM_ATTRIBUTES_IND */
typedef struct {
    uint32_t    size;
    uint8_t     scope;          /* The scope in which the UID of the media element item or folder item is valid. */
    uint8_t     uid[8];         /* The UID of the media element item or folder item to returned. */
    uint16_t    uid_counter;
    uint8_t     attr_num;        /* The number of attributes IDs in the following Attribute ID list.
                                   if this calue is 0 then all attributes are requested. */
    uint32_t    attr_id[1];      /* e.g list AVRCP_MA_TITLEOF_MEDIA */
} bt_sdk_get_item_attr_req_stru, *p_bt_sdk_get_item_attr_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_SEARCH_IND */
typedef struct {
    uint32_t    size;
    uint16_t    characterset_id;  /* CharacterSet ID.
                                    0x006A                The calue of UTF-8 as defined in IANA character set document.
                                    All other values        reserved.
                                  */
    uint16_t    len;             /* The length of the search string in octets. */
    uint8_t     string[1];       /* The stirng to search on in the specified character set. */
} bt_sdk_search_req_stru, *p_bt_sdk_search_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_SEARCH_RSP,
   BTSDK_APP_EV_AVRCP_GET_TOTAL_NUMBER_OF_ITMES_RSP */
typedef struct {
    uint32_t    size;
    uint8_t     status;          /* The Result of the GetItemAttributes operation.
                                    if an error has occurred then this is the only field present in the response. */
    uint16_t    uid_counter;
    uint32_t    items_num;        /* The number of media element items found in the search */
} bt_sdk_search_rsp_stru, *p_bt_sdk_search_rsp_stru,
  bt_sdk_get_total_num_of_items_rsp_stru, *p_bt_sdk_get_total_num_of_items_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_PLAY_ITEM_IND */
typedef struct {
    uint32_t    size;
    uint8_t     scope;           /* The scope in which the UID of the media elemnt item or folder item,
                                    if supported, is valid. */
    uint8_t     uid[8];          /* The UID of the media element item or folder item. */
    uint16_t    uid_counter;
} bt_sdk_play_item_req_stru, *p_bt_sdk_play_item_req_stru,
  bt_sdk_add_to_now_playing_req_stru, *p_bt_sdk_add_to_now_playing_req_stru;

typedef struct {
    uint8_t     id;               /* AttributeID or ValueID */
    uint16_t    characterset_id;   /* Character set ID */
    uint8_t     len;              /* Length of the player application setting attribute string */
    uint8_t     string[1];        /* Player application setting attribute string in specified character set */
} bt_sdk_id_string_stru, *p_bt_sdk_id_string_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_PLAYER_APPLICATION_SETTING_ATTRIBUTES_TEXT_RSP,
BTSDK_APP_EV_AVRCP_GET_PLAYER_APPLICATION_SETTING_VALUE_TEXT_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    subpacket_type;
    union {
        uint8_t id_num;                 /* The number of Items, BTSDK_AVRCP_PACKET_HEAD */
        bt_sdk_id_string_stru id_string;    /* BTSDK_AVRCP_SUBPACKET */
    };
} bt_sdk_id_string_rsp_stru, *p_bt_sdk_id_string_rsp_stru,
  bt_sdk_get_player_app_setting_attr_txt_rsp_stru, *p_bt_sdk_get_player_app_setting_attr_txt_rsp_stru,
  bt_sdk_get_player_app_setting_val_txt_rsp_stru, *p_bt_sdk_get_player_app_setting_val_txt_rsp_stru;

typedef struct { /* Attribute Value Entry list */
    uint32_t    attr_id;                 /* Attributes ID */
    uint16_t    characterset_id;         /* Character set ID */
    uint16_t    len;                    /* Length of value */
    uint8_t     value[0];               /* Value of the specified attribute */
} bt_sdk4id_string_stru, *p_bt_sdk4id_string_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_ELEMENT_ATTRIBUTES_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    subpacket_type;
    union {
        uint8_t    id_num;                /* The number of Items, BTSDK_AVRCP_PACKET_HEAD */
        bt_sdk4id_string_stru id_value;      /* BTSDK_AVRCP_SUBPACKET */
    };
} bt_sdk_get_element_attr_rsp_stru, *p_bt_sdk_get_element_attr_rsp_stru;

typedef struct {
    uint8_t        status;
    uint8_t        attr_num;        /* The number of Item */
} bt_sdk_get_item_attr_rsp_head_stru, *p_bt_sdk_get_item_attr_rsp_head_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_ITEM_ATTRIBUTES_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    subpacket_type;
    union {
        bt_sdk_get_item_attr_rsp_head_stru packet_head;    /* BTSDK_AVRCP_PACKET_HEAD */
        bt_sdk4id_string_stru entry;                  /* BTSDK_AVRCP_SUBPACKET */
    };
} bt_sdk_get_item_attr_rsp_stru, *p_bt_sdk_get_item_attr_rsp_stru;

typedef struct {
    uint8_t     status;            /* The Result of the SetBrowsedPlayer operation.
                                      if an error has occurred then this is the only field present in the response. */
    uint16_t    uid_counter;
    uint32_t    items_num;
    uint16_t    characterset_id;    /* The character set ID to be displayed on CT. */
    uint8_t     folder_depth;       /* The number of Folder Name Length/Folder Name pairs which follow. */
} btsdk_set_browsed_player_rsp_head_stru, *p_btsdk_set_browsed_player_rsp_head_stru;

typedef struct {
    uint16_t    folder_name_len;        /* The length in octets of the folder name which follows. */
    uint8_t     folder_name[1];
} btsdk_set_browsed_player_rsp_item_stru, *p_btsdk_set_browsed_player_rsp_item_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_SET_BROWSED_PLAYER_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    subpacket_type;
    union {
        btsdk_set_browsed_player_rsp_head_stru packet_head;    /* BTSDK_AVRCP_PACKET_HEAD */
        btsdk_set_browsed_player_rsp_item_stru folder_item;    /* BTSDK_AVRCP_SUBPACKET */
    };
} bt_sdk_set_browsed_player_rsp_stru, *p_bt_sdk_set_browsed_player_rsp_stru;

#define AVRCP_MP_ITEM_FEATUREBIT_LEN 16
#define AVRCP_FOLDER_ITEM_UID_LEN 8
#define AVRCP_ELEMENT_ITEM_UID_LEN 8

typedef struct {
    uint16_t    player_id;
    uint8_t     play_status;
    uint8_t     major_player_type;
    uint32_t    player_subtype;
    uint8_t     feature_bitmask[AVRCP_MP_ITEM_FEATUREBIT_LEN];
    uint16_t    characterset_id;        /* e.g AVRCP_CHARACTERSETID_UTF8 */
    uint16_t    name_len;
    uint8_t     name[1];
} bt_sdk_media_player_item_stru, *p_bt_sdk_media_player_item_stru;

typedef struct {
    uint8_t     folder_uid[AVRCP_FOLDER_ITEM_UID_LEN];
    uint8_t     folder_type;
    uint8_t     is_playable;
    uint16_t    characterset_id;
    uint16_t    name_len;
    uint8_t     name[1];
} bt_sdk_folder_item_stru, *p_bt_sdk_folder_item_stru;

typedef struct {
    uint8_t     element_uid[AVRCP_ELEMENT_ITEM_UID_LEN]; /* UID as defined in 6.10.3 */
    uint8_t     media_type;          /* e.g AVRCP_MEDIATYPE_AUDIO */
    uint8_t     attr_num;
    uint16_t    characterset_id;     /* e.g AVRCP_CHARACTERSETID_UTF8 */
    uint16_t    name_len;
    uint8_t     name[1];
} bt_sdk_media_element_item_stru, *p_bt_sdk_media_element_item_stru;

typedef struct {
    uint16_t    items_num;           /* The total number of items returned in this listing */
    uint16_t    uid_counter;
    uint16_t    item_len;            /* The size of the member in union. */
    uint8_t     item_type;
    uint8_t     status;             /* The Result of the GetFolderItem operation.
                                       if an error has occurred then this is the only field present in the response. */
    union {
        bt_sdk_media_player_item_stru    player_item;    /* itemType == AVRCP_ITEMTYPE_MEDIAPLAYER_ITEM */
        bt_sdk_folder_item_stru         folder_item;    /* itemType == AVRCP_ITEMTYPE_FOLDER_ITEM */
        bt_sdk_media_element_item_stru   element_item;   /* itemType == AVRCP_ITEMTYPE_MEDIAELEMENT_ITEM */
    };
} bt_sdk_browsable_item_stru, *p_bt_sdk_browsable_item_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_GET_FOLDER_ITEMS_RSP */
typedef struct {
    uint32_t    size;
    uint32_t    subpacket_type;
    union {
        bt_sdk_browsable_item_stru    item;            /* subpacket_type = BTSDK_AVRCP_PACKET_HEAD */
        bt_sdk4id_string_stru        element_attr;     /* subpacket_type = BTSDK_AVRCP_SUBPACKET,
                                                      when item.itemType == AVRCP_ITEMTYPE_MEDIAELEMENT_ITEM
                                                    */
    };
} bt_sdk_get_folder_item_rsp_stru, *p_bt_sdk_get_folder_item_rsp_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_REGISTER_NOTIFICATION_IND */
typedef struct {
    uint32_t    size;              /* Size of this structure */
    uint8_t     event_id;           /* ID of the event requires notification,
                                      e.g. BTSDK_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED */
    uint32_t    playback_interval;  /* Playback interval in seconds */
} bt_sdk_register_notif_req_stru, *p_bt_sdk_register_notif_req_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_TRACK_REACHED_END_NOTIF, BTSDK_APP_EV_AVRCP_TRACK_REACHED_START_NOTIF
BTSDK_APP_EV_AVRCP_NOW_PLAYING_CONTENT_CHANGED_NOTIF, BTSDK_APP_EV_AVRCP_AVAILABLE_PLAYERS_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;            /* Response code, e.g. BTSDK_AVRCP_RSP_NOT_IMPLEMENTED */
} bt_sdk_notif_null_stru, *p_bt_sdk_notif_null_stru,
  bt_sdk_track_reach_end_stru, *p_bt_sdk_track_reach_end_stru,
  bt_sdk_track_reach_start_stru, *p_bt_sdk_track_reach_start_stru,
  bt_sdk_now_playing_content_changed_stru, *p_bt_sdk_now_playing_content_changed_stru,
  bt_sdk_available_player_changed_stru, *p_bt_sdk_available_player_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_VOLUME_CHANGED_NOTIF, BTSDK_APP_EV_AVRCP_PLAYBACK_STATUS_CHANGED_NOTIF
BTSDK_APP_EV_AVRCP_BATT_STATUS_CHANGED_NOTIF, BTSDK_APP_EV_AVRCP_SYSTEM_STATUS_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint8_t     id;
} bt_sdk_notif_id_stru, *p_bt_sdk_notif_id_stru,
  bt_sdk_vol_changed_stru, *p_bt_sdk_vol_changed_stru,
  bt_sdk_play_status_changed_stru, *p_bt_sdk_play_status_changed_stru,
  bt_sdk_batt_status_changed_stru, *p_bt_sdk_batt_status_changed_stru,
  bt_sdk_sys_status_changed_stru, *p_bt_sdk_sys_status_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_UIDS_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint16_t    uid_counter;
} bt_sdk_uids_changed_stru, *p_bt_sdk_uids_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_PLAYBACK_POS_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint32_t    pos;                /* Current playback position in millisecond. */
} bt_sdk_play_pos_changed_stru, *p_bt_sdk_play_pos_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_AVAILABLE_PLAYERS_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint16_t    player_id;           /* Unique Media Player ID */
    uint16_t    uid_counter;
} bt_sdk_addr_player_changed_stru, *p_bt_sdk_addr_player_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_TRACK_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint8_t     identifier[8];      /* Unique Identifier to identify an element on TG,
                                       as is used for the GetElementAttributes command. */
} bt_sdk_track_changed_stru, *p_bt_sdk_track_changed_stru;

/* type of the Parameter of BTSDK_APP_EV_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED_NOTIF */
typedef struct {
    uint32_t    size;               /* Size of this structure */
    uint8_t     rsp_code;
    uint8_t     num;                /* Number of attributes */
    bt_sdk_id_pair_stru id[1];          /* List of the requested attribues ID and value ID */
} bt_sdk_player_app_set_changed_stru, *p_bt_sdk_player_app_set_changed_stru;

/*************** Macro Definition ******************/
/* AVRCP version number */
#define BTSDK_AVRCP_VER_1_0                             0x100
#define BTSDK_AVRCP_VER_1_3                             0x103
#define BTSDK_AVRCP_VER_1_4                             0x104

/* AVRCP Supported features */
#define BTSDK_AVRCP_FEATURES_CATEGORY_1                 0x0001
#define BTSDK_AVRCP_FEATURES_CATEGORY_2                 0x0002
#define BTSDK_AVRCP_FEATURES_CATEGORY_3                 0x0004
#define BTSDK_AVRCP_FEATURES_CATEGORY_4                 0x0008
#define BTSDK_AVRCP_FEATURES_PLAYER_APP_SETTINGS        0x0010
#define BTSDK_AVRCP_FEATURES_GROUP_NAVIGATION           0x0020
#define BTSDK_AVRCP_FEATURES_BROWSING                   0x0040
#define BTSDK_AVRCP_FEATURES_MULTI_MEDIA_PLAYER_APP     0x0080

/* AV/C Response Code, 4 Bits */
#define BTSDK_AVRCP_RSP_NOT_IMPLEMENTED                 0x08
#define BTSDK_AVRCP_RSP_ACCEPTED                        0x09
#define BTSDK_AVRCP_RSP_REJECTED                        0x0A
#define BTSDK_AVRCP_RSP_STABLE                          0x0C /* Implemented */
#define BTSDK_AVRCP_RSP_CHANGED                         0x0D /* for notification */
#define BTSDK_AVRCP_RSP_INTERIM                         0x0F
/* AV/C Panel Commands operation_id */
/* Used by the op_id of bt_sdk_pass_thr_req_stru */
#define BTSDK_AVRCP_OPID_AVC_PANEL_POWER                0x40
#define BTSDK_AVRCP_OPID_AVC_PANEL_VOLUME_UP            0x41
#define BTSDK_AVRCP_OPID_AVC_PANEL_VOLUME_DOWN          0x42
#define BTSDK_AVRCP_OPID_AVC_PANEL_MUTE                 0x43
#define BTSDK_AVRCP_OPID_AVC_PANEL_PLAY                 0x44
#define BTSDK_AVRCP_OPID_AVC_PANEL_STOP                 0x45
#define BTSDK_AVRCP_OPID_AVC_PANEL_PAUSE                0x46
#define BTSDK_AVRCP_OPID_AVC_PANEL_RECORD               0x47
#define BTSDK_AVRCP_OPID_AVC_PANEL_REWIND               0x48
#define BTSDK_AVRCP_OPID_AVC_PANEL_FAST_FORWARD         0x49
#define BTSDK_AVRCP_OPID_AVC_PANEL_EJECT                0x4a
#define BTSDK_AVRCP_OPID_AVC_PANEL_FORWARD              0x4b
#define BTSDK_AVRCP_OPID_AVC_PANEL_BACKWARD             0x4c

#define BTSDK_AVRCP_OPID_VENDORUNIQUE                   0x7e

/* used by the state_flag of bt_sdk_pass_thr_req_stru */
#define BTSDK_AVRCP_BUTTON_STATE_PRESSED                0
#define BTSDK_AVRCP_BUTTON_STATE_RELEASED               1

/* Vendor Unique Operation IDs, used by the vendor_unique_id of bt_sdk_group_navi_req_stru */
#define BTSDK_AVRCP_BGN_NEXTGROUP                       0x0000
#define BTSDK_AVRCP_BGN_PREVGROUP                       0x0001

#define BTSDK_AVRCP_CHARACTERSETID_UTF8                 0x006A /* CharacterSetID, UTF-8 */

/* Capabilities ID */
/* Used by BTSDK_AVRCP_GetCapabilities Command to specific capability reuqested */
#define BTSDK_AVRCP_CAPABILITYID_COMPANY_ID                     0x2
#define BTSDK_AVRCP_CAPABILITYID_EVENTS_SUPPORTED               0x3

/* Notification event IDs, possible values of BtSdkRegisterNotifiReqStru::eventId. */
#define BTSDK_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED               0x01
#define BTSDK_AVRCP_EVENT_TRACK_CHANGED                         0x02
/* if any action (e.g. GetElementAttributes) is undertaken on the CT as reaction to the EVENT_TRACK_REACHED_END,
   the CT should register the EVENT_TRACK_REACHED_END again before initiating this action in order to get informed
   about intermediate changes regarding the track status. */
#define BTSDK_AVRCP_EVENT_TRACK_REACHED_END                     0x03
/* if any action (e.g. GetElementAttributes) is undertaken on the CT as reaction to the EVENT_TRACK_REACHED_START,
   the CT should register the EVENT_TRACK_REACHED_START again before initiating this action in order to get informed
   about intermediate changes regarding the track status. */
#define BTSDK_AVRCP_EVENT_TRACK_REACHED_START                   0x04
#define BTSDK_AVRCP_EVENT_PLAYBACK_POS_CHANGED                  0x05
#define BTSDK_AVRCP_EVENT_BATT_STATUS_CHANGED                   0x06
#define BTSDK_AVRCP_EVENT_SYSTEM_STATUS_CHANGED                 0x07
#define BTSDK_AVRCP_EVENT_PLAYER_APPLICATION_SETTING_CHANGED    0x08
/* if the NowPlaying folder is browsed as reaction to the EVENT_NOW_PLAYING_CONTENT_CHANGED,
   the CT should register the EVENT_NOW_PLAYING_CONTENT_CHANGED again before browsing the NowPlaying folder
   in order to get informed about intermediate changes in that folder. */
#define BTSDK_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED           0x09
/* if the Media Player List is browsed as reaction to the EVENT_AVAILABLE_PLAYERS_CHANGED,
   the CT should register the EVENT_AVAILABLE_PLAYERS_CHANGED again before browsing the Media Player list
   in order to get informed about intermediate changes of the available players. */
#define BTSDK_AVRCP_EVENT_AVAILABLE_PLAYERS_CHANGED             0x0A
#define BTSDK_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED              0x0B
/* if the Media Player Virtual Filesystem is browsed as reaction to the EVENT_UIDS_CHANGED,
   the CT should register the EVENT_UIDS_CHANGED again before browsing the Media Player Virtual Filesystem
   in order to get informed about intermediate changes within the fileystem. */
#define BTSDK_AVRCP_EVENT_UIDS_CHANGED                          0x0C
#define BTSDK_AVRCP_EVENT_VOLUME_CHANGED                        0x0D /* 0x0e-0xFF reserved for future use */

/* Player application settings PDUs */
/* List of defined Player Application Settings and Values. */
#define BTSDK_AVRCP_PASA_ILLEGAL                                0x00
#define BTSDK_AVRCP_PASA_EQUALIZER_ONOFF_STATUS                 0x01
#define BTSDK_AVRCP_PASA_REPEAT_MODE_STATUS                     0x02
#define BTSDK_AVRCP_PASA_SHUFFLE_ONOFF_STATUS                   0x03
#define BTSDK_AVRCP_PASA_SCAN_ONOFF_STATUS                      0x04
/* PASA 0x05 - 0x7F Reserved for future use.
   0x80 - 0xFF Provided for TG driven static media player menu extension by CT
*/

/* 0x01 Equalizer ON/OFF status */
#define BTSDK_AVRCP_EQUALIZER_OFF                               0x01
#define BTSDK_AVRCP_EQUALIZER_ON                                0x02

/* 0x02 Repeat Mode status */
#define BTSDK_AVRCP_REPEAT_MODE_OFF                             0x01
#define BTSDK_AVRCP_REPEAT_MODE_SINGLE_TRACK_REPEAT             0x02
#define BTSDK_AVRCP_REPEAT_MODE_ALL_TRACK_REPEAT                0x03
#define BTSDK_AVRCP_REPEAT_MODE_GROUP_REPEAT                    0x04

/* 0x03 Shuffle ON/OFF status */
#define BTSDK_AVRCP_SHUFFLE_OFF                                 0x01
#define BTSDK_AVRCP_SHUFFLE_ALL_TRACKS_SHUFFLE                  0x02
#define BTSDK_AVRCP_SHUFFLE_GROUP_SHUFFLE                       0x03

/* 0x04 Scan ON/OFF status */
#define BTSDK_AVRCP_SCAN_OFF                                    0x01
#define BTSDK_AVRCP_SCAN_ALL_TRACKS_SCAN                        0x02
#define BTSDK_AVRCP_SCAN_GROUP_SCAN                             0x03

/* Battery Status, InformBatteryStatusOfCT command */
#define BTSDK_AVRCP_BATTERYSTATUS_NORMAL                        0x0
#define BTSDK_AVRCP_BATTERYSTATUS_WARNING                       0x1
#define BTSDK_AVRCP_BATTERYSTATUS_CRITICAL                      0x2
#define BTSDK_AVRCP_BATTERYSTATUS_EXTERNAL                      0x3
#define BTSDK_AVRCP_BATTERYSTATUS_FULL_CHARGE                   0x4

/* GetElementAttributes */
/* List of Media Attributes */
#define BTSDK_AVRCP_MA_ILLEGAL                                  0x00 /* should not be used */
#define BTSDK_AVRCP_MA_TITLEOF_MEDIA                            0x01 /* Any text encoded in specified character set */
#define BTSDK_AVRCP_MA_NAMEOF_ARTIST                            0x02 /* Any text encoded in specified character set */
#define BTSDK_AVRCP_MA_NAMEOF_ALBUM                             0x03 /* Any text encoded in specified character set */
/* Numeric ASCII text with zero suppresses, ex. Track number of the CD */
#define BTSDK_AVRCP_MA_NUMBEROF_MEDIA                           0x04
#define BTSDK_AVRCP_MA_TOTALNUMBEROF_MEDIA                      0x05 /* Numeric ASCII text with zero suppresses */
#define BTSDK_AVRCP_MA_GENRE                                    0x06 /* Any text encoded in specified character set */
/* Playing time in millisecond, 2min30sec->150000, 08-0xFFFFFFFF reserved for future use */
#define BTSDK_AVRCP_MA_PLAYING_TIME                             0x07

/* Notification */
/* Current status of playing */
#define BTSDK_AVRCP_PLAYSTATUS_STOPPED                          0x00
#define BTSDK_AVRCP_PLAYSTATUS_PLAYING                          0x01
#define BTSDK_AVRCP_PLAYSTATUS_PAUSED                           0x02
#define BTSDK_AVRCP_PLAYSTATUS_FWD_SEEK                         0x03
#define BTSDK_AVRCP_PLAYSTATUS_REV_SEEK                         0x04 /* 0x05-0xfe are reserved */
#define BTSDK_AVRCP_PLAYSTATUS_ERROR                            0xFF

/* if TG does not support SongLength and Song Position on TG, shall return 0xFFFFFFFF */
#define BTSDK_AVRCP_NOTSUPPORT_VALUE                            0xFFFFFFFF

/* System status for AVRCP_EVENT_SYSTEM_STATUS_CHANGED */
#define BTSDK_AVRCP_SYSTEM_POWER_ON                             0x00
#define BTSDK_AVRCP_SYSTEM_POWER_OFF                            0x01
#define BTSDK_AVRCP_SYSTEM_UNPLUGGED                            0x02

/* Media Content Navigation */
/* There are four scopes in which media content navigation may take place.
   Scopes summarizes them and they are described in more detail in the following sections.
*/
/* Media Player Item, Contains all available media players */
#define BTSDK_AVRCP_SCOPE_MEDIAPLAYER_LIST                      0x00
/* Folder Item and Media Element Item, The virtual filesystem containing the media content of the browsed player */
#define BTSDK_AVRCP_SCOPE_MEDIAPLAYER_VIRTUAL_FILESYSTEM        0x01
/* Media Element Item, The results of a search operation on the browsed player */
#define BTSDK_AVRCP_SCOPE_MEDIAPLAYER_SEARCH                    0x02
/* Media Element Item, The Now Playing list (or queue) of the addressed player */
#define BTSDK_AVRCP_SCOPE_MEDIAPLAYER_NOWPLAYING                0x03

/* Item type - 1 Octet */
#define BTSDK_AVRCP_ITEMTYPE_MEDIAPLAYER_ITEM                   0x01
#define BTSDK_AVRCP_ITEMTYPE_FOLDER_ITEM                        0x02
#define BTSDK_AVRCP_ITEMTYPE_MEDIAELEMENT_ITEM                  0x03

/* Major Player type - 1 Octet */
#define BTSDK_AVRCP_MAJORPLAYERTYPE_AUDIO                       0x01
#define BTSDK_AVRCP_MAJORPLAYERTYPE_VIDEO                       0x02
#define BTSDK_AVRCP_MAJORPLAYERTYPE_BROADCASTING_AUDIO          0x04
#define BTSDK_AVRCP_MAJORPLAYERTYPE_BROADCASTING_VIDEO          0x08

/* Player Sub type - 4 Octets */
#define BTSDK_AVRCP_PLAYERSUBTYPE_NONE                          0x00000000
#define BTSDK_AVRCP_PLAYERSUBTYPE_AUDIOBOOK                     0x00000001
#define BTSDK_AVRCP_PLAYERSUBTYPE_PODCAST                       0x00000002

/* Feature Bit Mask - 16 Octets */
#define BTSDK_AVRCP_FBM_OCTET_ALL                               0xFF

/* Octet 0 */
#define BTSDK_AVRCP_FBM_SELECT                                  0x01
#define BTSDK_AVRCP_FBM_UP                                      0x02
#define BTSDK_AVRCP_FBM_DOWN                                    0x04
#define BTSDK_AVRCP_FBM_LEFT                                    0x08
#define BTSDK_AVRCP_FBM_RIGHT                                   0x10
#define BTSDK_AVRCP_FBM_RIGHTUP                                 0x20
#define BTSDK_AVRCP_FBM_RIGHTDOWN                               0x40
#define BTSDK_AVRCP_FBM_LEFTUP                                  0x80

/* Octet 1 */
#define BTSDK_AVRCP_FBM_LEFTDOWN                                0x01
#define BTSDK_AVRCP_FBM_ROOTMENU                                0x02
#define BTSDK_AVRCP_FBM_SETUPMENU                               0x04
#define BTSDK_AVRCP_FBM_CONTENTSMENU                            0x08
#define BTSDK_AVRCP_FBM_FAVORITEMENU                            0x10
#define BTSDK_AVRCP_FBM_EXIT                                    0x20
#define BTSDK_AVRCP_FBM_0                                       0x40
#define BTSDK_AVRCP_FBM_1                                       0x80

/* Octet 2 */
#define BTSDK_AVRCP_FBM_2                                       0x01
#define BTSDK_AVRCP_FBM_3                                       0x02
#define BTSDK_AVRCP_FBM_4                                       0x04
#define BTSDK_AVRCP_FBM_5                                       0x08
#define BTSDK_AVRCP_FBM_6                                       0x10
#define BTSDK_AVRCP_FBM_7                                       0x20
#define BTSDK_AVRCP_FBM_8                                       0x40
#define BTSDK_AVRCP_FBM_9                                       0x80

/* Octet 3 */
#define BTSDK_AVRCP_FBM_DOT                                     0x01
#define BTSDK_AVRCP_FBM_ENTER                                   0x02
#define BTSDK_AVRCP_FBM_CLEAR                                   0x04
#define BTSDK_AVRCP_FBM_CHANNELUP                               0x08
#define BTSDK_AVRCP_FBM_CHANNELDOWN                             0x10
#define BTSDK_AVRCP_FBM_PREVIOUSCHANNEL                         0x20
#define BTSDK_AVRCP_FBM_SOUNDSELECT                             0x40
#define BTSDK_AVRCP_FBM_INPUTSELECT                             0x80

/* Octet 4 */
#define BTSDK_AVRCP_FBM_DISPLAY_INFORMATION                     0x01
#define BTSDK_AVRCP_FBM_HELP                                    0x02
#define BTSDK_AVRCP_FBM_PAGEUP                                  0x04
#define BTSDK_AVRCP_FBM_PAGEDOWN                                0x08
#define BTSDK_AVRCP_FBM_POWER                                   0x10
#define BTSDK_AVRCP_FBM_VOLUMEUP                                0x20
#define BTSDK_AVRCP_FBM_VOLUMEDOWN                              0x40
#define BTSDK_AVRCP_FBM_MUTE                                    0x80

/* Octet 5 */
#define BTSDK_AVRCP_FBM_PLAY                                    0x01
#define BTSDK_AVRCP_FBM_STOP                                    0x02
#define BTSDK_AVRCP_FBM_PAUSE                                   0x04
#define BTSDK_AVRCP_FBM_RECORD                                  0x08
#define BTSDK_AVRCP_FBM_REWIND                                  0x10
#define BTSDK_AVRCP_FBM_FASTFORWARD                             0x20
#define BTSDK_AVRCP_FBM_EJECT                                   0x40
#define BTSDK_AVRCP_FBM_FORWARD                                 0x80

/* Octet 6 */
#define BTSDK_AVRCP_FBM_BACKWARD                                0x01
#define BTSDK_AVRCP_FBM_ANGLE                                   0x02
#define BTSDK_AVRCP_FBM_SUBPICTURE                              0x04
#define BTSDK_AVRCP_FBM_F1                                      0x08
#define BTSDK_AVRCP_FBM_F2                                      0x10
#define BTSDK_AVRCP_FBM_F3                                      0x20
#define BTSDK_AVRCP_FBM_F4                                      0x40
#define BTSDK_AVRCP_FBM_F5                                      0x80

/* Octet 7 */
#define BTSDK_AVRCP_FBM_VENDOR_UNIQUE                           0x01
#define BTSDK_AVRCP_FBM_BASIC_GROUP_NAVIGATION                  0x02
#define BTSDK_AVRCP_FBM_ADVANCED_CONTROL_PLAYER                 0x04
#define BTSDK_AVRCP_FBM_BROWSING                                0x08
#define BTSDK_AVRCP_FBM_SEARCHING                               0x10
#define BTSDK_AVRCP_FBM_ADDTO_NOWPLAYING                        0x20
#define BTSDK_AVRCP_FBM_UIDS_UNIQUE_INPLAYERBROWSE_TREE         0x40
#define BTSDK_AVRCP_FBM_ONLY_BROWSABLE_WHEN_ADDRESSED           0x80

/* Octet 8 */
#define BTSDK_AVRCP_FBM_ONLY_SEARCHABLE_WHEN_ADDRESSED          0x01
#define BTSDK_AVRCP_FBM_NOWPLAYING                              0x02
#define BTSDK_AVRCP_FBM_UIDPERSISTENCY                          0x04

/* Folder Item */
/* Folder type - 1 Octet */
#define BTSDK_AVRCP_FOLDERTYPE_MIXED                            0x00
#define BTSDK_AVRCP_FOLDERTYPE_TITLES                           0x01
#define BTSDK_AVRCP_FOLDERTYPE_ALBUMS                           0x02
#define BTSDK_AVRCP_FOLDERTYPE_ARTISTS                          0x03
#define BTSDK_AVRCP_FOLDERTYPE_GENRES                           0x04
#define BTSDK_AVRCP_FOLDERTYPE_PLAYLISTS                        0x05
#define BTSDK_AVRCP_FOLDERTYPE_YEARS                            0x06

/* Is Playable - 1 Octet */
#define BTSDK_AVRCP_ISPLAYABLE_CANNOT                           0x00
#define BTSDK_AVRCP_ISPLAYABLE_CAN                              0x01

/* Media type - 1 Octet */
#define BTSDK_AVRCP_MEDIATYPE_AUDIO                             0x00
#define BTSDK_AVRCP_MEDIATYPE_VIDEO                             0x01

/* Browsing Commands */
#define BTSDK_AVRCP_DIRECTION_FOLDER_UP                         0x00
#define BTSDK_AVRCP_DIRECTION_FOLDER_DOWN                       0x01

/* Volume Handling */
/*-----------------------------------------------------------------------------
Description:
    An Absolute Volume is represented in one octet. The top bit (bit 7) is reserved
    for future definition. The volume is specified as a percentage of the maximum.
    The value 0x0 corresponds to 0%. The value 0x7F corresponds to 100%. Scaling
    should be applied to achieve values between these two.
-----------------------------------------------------------------------------*/
#define BTSDK_AVRCP_ABSOLUTE_VOLUME_MIN                     0x00
#define BTSDK_AVRCP_ABSOLUTE_VOLUME_MAX                     0x7F

/* Basic Group Navigation    */
#define BTSDK_AVRCP_BGN_PREVIOUSGROUP                       0x0001

/* 6.15 Error handling */
/* List of Error Status Code */
#define BTSDK_AVRCP_ERROR_INVALID_COMMAND              0x00 /* All */
#define BTSDK_AVRCP_ERROR_INVALID_PARAMETER            0x01 /* All */
#define BTSDK_AVRCP_ERROR_SPECIFIED_PARAMETER_NOTFOUND 0x02 /* All */
#define BTSDK_AVRCP_ERROR_INTERNAL_ERROR               0x03 /* All */
#define BTSDK_AVRCP_ERROR_SUCCESSFUL                   0x04 /* All except where the response CType is AV/C REJECTED */
#define BTSDK_AVRCP_ERROR_UID_CHANGED                  0x05 /* All */
#define BTSDK_AVRCP_ERROR_RESERVED                     0x06 /* All */
#define BTSDK_AVRCP_ERROR_INVALID_DIRECTION            0x07 /* The Direction parameter is invalid, Change Path */
#define BTSDK_AVRCP_ERROR_NOTA_DIRECTORY               0x08 /* The UID provided does not refer to a folder item, \
                                                               Change Path */
#define BTSDK_AVRCP_ERROR_UID_DOESNOT_EXIST        0x09 /* The UID provided does not refer to any currently  \
                                                           valid item. Change Path, PlayItem, AddToNowPlaying, \
                                                           GetItemAttributes */
#define BTSDK_AVRCP_ERROR_INVALID_SCOPE            0x0A /* The scope parameter is invalid. GetFolderItems, PlayItem, \
                                                           AddToNowPlayer, GetItemAttributes. */
#define BTSDK_AVRCP_ERROR_RANGE_OUTOF_BOUNDS       0x0B /* The start of range provided is not valid, GetFolderItems */
#define BTSDK_AVRCP_ERROR_UID_ISA_DIRECTORY        0x0C /* The UID provided refers to a directory, which cannot be \
                                                           handled by this media player, PlayItem, AddToNowPlaying */
#define BTSDK_AVRCP_ERROR_MEDIA_INUSE              0x0D /* The media is not able to be used for this operation \
                                                           at this time, PlayItem, AddToNowPlaying */
#define BTSDK_AVRCP_ERROR_NOWPLAYING_LISTFULL      0x0E /* No more items can be added to the Now Playing List, \
                                                           AddToNowPlaying */
#define BTSDK_AVRCP_ERROR_SEARCH_NOTSUPPORTED      0x0F /* The Browsed Media Player does not support search, Search */
#define BTSDK_AVRCP_ERROR_SEARCH_INPROGRESS        0x10 /* A search operation is already in progress, Search */
#define BTSDK_AVRCP_ERROR_INVALID_PLAYERID         0x11 /* The specified Player Id does not refer to a valid player, \
                                                           SetAddressedPlayer, SetBrowsedPlayer */
#define BTSDK_AVRCP_ERROR_PLAYER_NOT_BROWSABLE     0x12 /* The Player Id supplied refers to a Media Player \
                                                           which does not support browsing. SetBrowsedPlayer */
#define BTSDK_AVRCP_ERROR_PLAYER_NOT_ADDRESSED     0x13 /* The Player Id supplied refers to a player which is not   \
                                                           currently addressed, and the command is not able to be   \
                                                           performed if the player is not set as addressed. Search, \
                                                           SetBrowsedPlayer */
/* The Search result list does not contain valid entries, e.g. \
   after being invalidated due to change of browsed player.    \
   GetFolderItems */
#define BTSDK_AVRCP_ERROR_NO_VALID_SEARCH_RESULTS  0x14
#define BTSDK_AVRCP_ERROR_NO_AVAILABLE_PLAYERS     0x15 /* All */
#define BTSDK_AVRCP_ERROR_ADDRESSED_PLAYER_CHANGED 0x16 /* Register Notification. 0x17-0xff Reserved all */
#define BTSDK_AVRCP_ERROR_TIMEOUT                  0x88 /* Monitor timer expired. Private error code. */
#define BTSDK_AVRCP_ERROR_NOT_IMPLEMENTED          0x89 /* Not Implemented response is recived. Private error code. */
#define BTSDK_AVRCP_ERROR_ALREADY_NOTIFY_THE_SAME_EVT 0x8A

/* The type of subpacket in complex struct */
#define BTSDK_AVRCP_PACKET_HEAD                             0x01
#define BTSDK_AVRCP_SUBPACKET                               0x02

/* The type of subpacket in BtSdkGetFolderItemRsp struct */
#define BTSDK_AVRCP_PACKET_BROWSABLE_ITEM                   0x01
#define BTSDK_AVRCP_PACKET_MEDIA_ATTR                       0x02

typedef bool (btsdk_avrcp_ct_rsp_cbk_func)(uint32_t dev_hdl, uint16_t rsp_type, uint8_t *param, uint16_t len);
typedef bool (btsdk_avrcp_tg_cmd_cbk_func)(uint32_t dev_hdl, uint8_t tl, uint16_t cmd_type, uint8_t *param);

typedef struct {
    btsdk_avrcp_ct_rsp_cbk_func   *ct_rsp_cbk;
    btsdk_avrcp_tg_cmd_cbk_func   *tg_cmd_cbk;
} sdk_avrcp_cbk_stru;

int32_t avrcp_ct_reg_srv_req(bool enable);
int32_t avrcp_ct_reg_notify_req(uint32_t dev_hdl, uint8_t event_id, uint32_t playback_interval);
int32_t avrcp_ct_get_cap_req(uint32_t dev_hdl, uint8_t id);
int32_t avrcp_ct_pass_through_req(uint32_t dev_hdl, uint8_t state_flag, uint8_t op_id, uint8_t vendor_unique_id);
int32_t avrcp_ct_get_element_attr_req(uint32_t dev_hdl, uint32_t *attr_id_list, uint8_t attr_num);
int32_t avrcp_ct_get_play_status_req(uint32_t dev_hdl);
int32_t avrcp_ct_set_absolute_vol_req(uint32_t dev_hdl, uint8_t id);
int32_t avrcp_tg_set_absolute_vol_rsp(uint32_t dev_hdl, uint8_t id, uint8_t tl);
int32_t avrcp_tg_event_play_status_changed(uint8_t rsp_code, uint8_t id);
int32_t avrcp_tg_event_track_changed(uint8_t rsp_code, uint64_t identifier);
int32_t avrcp_tg_event_track_reach_end(uint8_t rsp_code);
int32_t avrcp_tg_event_track_reach_start(uint8_t rsp_code);
int32_t avrcp_tg_event_play_pos_changed(uint8_t rsp_code, uint32_t pos);
int32_t avrcp_tg_event_play_content_changed(uint8_t rsp_code);
int32_t avrcp_tg_event_available_player_changed(uint8_t rsp_code);
int32_t avrcp_tg_event_addr_player_changed(uint8_t rsp_code, uint16_t player_id, uint16_t uid_counter);
int32_t avrcp_tg_event_uids_changed(uint8_t rsp_code, uint16_t uid_counter);
int32_t avrcp_tg_event_vol_changed(uint8_t rsp_code, uint8_t id);
int32_t avrcp_ct_get_capabilities_rsp(uint32_t dev_hdl, uint8_t tl, uint8_t capability_id, uint8_t count,
    uint8_t *capability);
int32_t avrcp_tg_reg_command_cbk(bool enable);
int32_t avrcp_ct_reg_rsp_cbk(bool enable);
void btsdk_avrcp_tg_reg_command_cbk(btsdk_avrcp_tg_cmd_cbk_func avrcp_tg_cmd_cbk);
void btsdk_avrcp_ct_reg_response_cbk(btsdk_avrcp_ct_rsp_cbk_func avrcp_ct_rsp_cbk);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_AV_IF_H */
