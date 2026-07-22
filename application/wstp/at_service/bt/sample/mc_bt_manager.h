/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: This file is the code for bt manager header
 * Author:
 * Create:
 */
#ifndef MC_BT_MANAGER_H
#define MC_BT_MANAGER_H

#include "stdbool.h"
#include <stdio.h>
#include "errcode.h"
#include "soc_errno.h"

#define BT_MAC_LEN 6
#define BT_ONE_ADDR_LEN 2
#define BT_MAC_LEN_AND_IS_SNIFF 7
#define BT_MAX_CMD_STR_LEN 32
#define BTSVC_STRUCT_LEN 4

typedef struct {
    uint16_t cmdID;
    uint16_t length;
    uint8_t data[0];
} bt_manager_svc_inter_info_t;

typedef enum {
    BT_MANAGER_SVC_UI_INFO_CHANGE,
    BT_MANAGER_UI_CMD_MAX_ID
} bt_manager_svc_ui_notify_cmd_t;

typedef struct {
    uint16_t devType;
    uint8_t tranID;                    /* The inter commu header transaction id   */
    uint8_t control;                   /* The inter commu header control          */
    uint8_t serviceID;                 /* The inter commu header Service ID       */
    uint8_t commandID;                 /* The inter commu header Command ID       */
    uint8_t appID[BTSVC_STRUCT_LEN];
} bt_manager_commu_header_t;

typedef struct {
    bt_manager_commu_header_t interHeader;  /* The inter commu header        */
    uint16_t payloadLength;           /* The Length of Payload         */
    uint8_t tranIDSrc;                /* The Length of Payload         */
    uint8_t reserved;                 /* For the 4 Byte alignment      */
    uint8_t payload[0];               /* The pointer of Payload buf    */
} bt_manager_commu_inter_info_t;

/* CID */
enum {
    MC_BT_MANAGER_SYNC_REQ = 0x01,
    MC_BT_MANAGER_SYNC_RSP,
    MC_BT_MANAGER_CHANGE_REQ,
    MC_BT_MANAGER_CHANGE_RSP,
    MC_BT_MANAGER_LOCK_STATE_NOTIFY,
    MC_BT_MANAGER_FAC_RESET,
    MC_BT_MANAGER_DEV_SHUTDOWN,
    MC_BT_MANAGER_DEV_HIGHT_POWER,
    MC_BT_MANAGER_SELFIND_INFO_NOTIFY, /* Only used to Selfind adv func */
    MC_BT_MANAGER_PDL_OPERATION = 0x0B, /* Transmit the storage info of PDL to BT Core */
    MC_BT_MANAGER_RESET_TYPE = 0x0C, /* Transmit the storage info of PDL to BT Core */
    MC_BT_MANAGER_POWER_CONTROL = 0x0F,
};

enum {
    BT_MANAGER_POWER_ON_BT,
    BT_MANAGER_POWER_OFF_BT,
};

typedef struct {
    uint16_t offset;
    uint8_t data[1]; /* mem start */
} bt_manager_default_t;

//BTH WSTP
enum {
    BTSRV_WSTP_AT_TEST = 0x0900,
    BTSRV_WSTP_AT_ENABLE_BT = 0x0901,
    BTSRV_WSTP_AT_DISABLE_BT = 0x0902,
    BTSRV_WSTP_AT_BT_SERVICE_TEST_CMD = 0x0903,
    BTSRV_WSTP_AT_SET_LOCAL_ADDR = 0x0904,
    BTSRV_WSTP_AT_GET_LOCAL_ADDR = 0x0905,
    BTSRV_WSTP_AT_SET_LOCAL_NAME = 0x0906,
    BTSRV_WSTP_AT_GET_LOCAL_NAME = 0x0907,
    BTSRV_WSTP_AT_BT_OPEN_INQSCAN = 0x0908,
    BTSRV_WSTP_AT_BT_CLOSE_INQSCAN = 0x0909,
    BTSRV_WSTP_AT_FACTORY_RESET = 0x090A,
    BTSRV_WSTP_AT_BT_START_DISCOVERY = 0x090B,
    BTSRV_WSTP_AT_BT_CANCEL_DISCOVERY = 0x090C,
    BTSRV_WSTP_AT_BT_READ_DISCOVERY_INFO = 0x090D,
    BTSRV_WSTP_AT_BT_DFX_SHOW_CONN_LIST = 0x090E,
    BTSRV_WSTP_AT_BT_DFX_SHOW_REMOTE_DEV_LIST = 0x090F,
    BTSRV_WSTP_AT_BT_GET_PAIRED_DEVICES_NUM = 0x0910,
    BTSRV_WSTP_AT_BT_GET_PAIRED_DEVICES_LIST = 0x0911,
    BTSRV_WSTP_AT_BT_GET_PAIR_STATE = 0x0912,
    BTSRV_WSTP_AT_BLE_GATT_CREATE_SERVER = 0x0913,
    BTSRV_WSTP_AT_BLE_GATT_CREATE_SERVICE = 0x0914,
    BTSRV_WSTP_AT_BLE_GATT_ADD_INCLUDE_SERVICE = 0x0915,
    BTSRV_WSTP_AT_BLE_GATT_ADD_CHARACTER = 0x0916,
    BTSRV_WSTP_AT_BLE_GATT_ADD_DESCRIPTOR = 0x0917,
    BTSRV_WSTP_AT_BLE_GATT_START_SERVICE = 0x0918,
    BTSRV_WSTP_AT_BLE_GATT_SEND_RESPONSE = 0x0919,
    BTSRV_WSTP_AT_BLE_GATT_UPDATE_CHAR_VALUE = 0x091A,
    BTSRV_WSTP_AT_BLE_GATT_UPDATE_DESC_VALUE = 0x091B,
    BTSRV_WSTP_AT_BLE_GATT_GET_GAP_ATTRIBUTE = 0x091C,
    BTSRV_WSTP_AT_BLE_GATT_SET_GAP_ATTRIBUTE = 0x091D,
    BTSRV_WSTP_AT_BLE_GATT_MULTIPLE_VALUE_NTF = 0x091E,
    BTSRV_WSTP_AT_BLE_GATT_MTU_SET_PARAM = 0x091F,
    BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_SERVICE = 0x0920,
    BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_INCLUDE = 0x0921,
    BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_CHAR = 0x0922,
    BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_CHAR_BY_UUID = 0x0923,
    BTSRV_WSTP_AT_BLE_GATT_DISCOVERY_DESC = 0x0924,
    BTSRV_WSTP_AT_BLE_GATT_DISC_DESC_BY_HANDLE = 0x0925,
    BTSRV_WSTP_AT_BLE_GATT_READ_CHARACTER = 0x0926,
    BTSRV_WSTP_AT_BLE_GATT_READ_CHAR_BY_UUID = 0x0927,
    BTSRV_WSTP_AT_BLE_GATT_READ_MULTIPLE_CHAR = 0x0928,
    BTSRV_WSTP_AT_BLE_GATT_MULT_VALUE_READ_CHAR = 0x0929,
    BTSRV_WSTP_AT_BLE_GATT_WRITE_CHARACTER = 0x092A,
    BTSRV_WSTP_AT_BLE_GATT_RELIABLE_WRITE_CHAR = 0x092B,
    BTSRV_WSTP_AT_BLE_GATT_READ_DESCRIPTOR = 0x092C,
    BTSRV_WSTP_AT_BLE_GATT_WRITE_DESCRIPTOR = 0x092D,
    BTSRV_WSTP_AT_BLE_GATT_MTU_CONFIG = 0x092E,
    BTSRV_WSTP_AT_BLE_GATT_VALUE_NOTIFICATION = 0x092F,
    BTSRV_WSTP_AT_BLE_GATT_GET_HID_DEVICE_RPT = 0x0930,
    BTSRV_WSTP_AT_BLE_GATT_SET_ATT_MIN_KEY_SIZE = 0x0931,
    BTSRV_WSTP_AT_BLE_GATT_SET_ATT_CONFIG_PARAM = 0x0932,
    BTSRV_WSTP_AT_BLE_ENABLE_ADV = 0x0933,
    BTSRV_WSTP_AT_BLE_DISABLE_ADV = 0x0934,
    BTSRV_WSTP_AT_BLE_ENTER_SCAN = 0x0935,
    BTSRV_WSTP_AT_BT_CONN_BY_ADDR = 0x0936,
    BTSRV_WSTP_AT_BT_CANCEL_PAIR = 0x0937,
    BTSRV_WSTP_AT_BT_IS_CONNECTED = 0x0938,
    BTSRV_WSTP_AT_BT_DISCONNECT = 0x0939,
    BTSRV_WSTP_AT_PBAP_REG= 0x093A,
    BTSRV_WSTP_AT_PBAP_CONN = 0x093B,
    BTSRV_WSTP_AT_PBAP_DISCONN = 0x093C,
    BTSRV_WSTP_AT_PBAP_PULL_PHONEBOOK = 0x093D,
    BTSRV_WSTP_AT_PBAP_PULL_VCARD_LISTING = 0x093E,
    BTSRV_WSTP_AT_PBAP_PULL_VCARD_ENTRY = 0x093F,
    BTSRV_WSTP_AT_PBAP_STOP_TRANS = 0x0940,
    BTSRV_WSTP_AT_PBAP_GET_CONN_STATE = 0x0941,
    BTSRV_WSTP_AT_AVRCP_REG = 0x0942,
    BTSRV_WSTP_AT_AVRCP_DEREG = 0x0943,
    BTSRV_WSTP_AT_AVRCP_TG_CONN = 0x0944,
    BTSRV_WSTP_AT_AVRCP_TG_DISCONN = 0x0945,
    BTSRV_WSTP_AT_AVRCP_TG_GET_DEV_CONN_STATE = 0x0946,
    BTSRV_WSTP_AT_AVRCP_TG_NOTIFY_STATE = 0x0947,
    BTSRV_WSTP_AT_AVRCP_TG_SET_LOC_INF = 0x0948,
    BTSRV_WSTP_AT_AVRCP_CT_CONN = 0x0949,
    BTSRV_WSTP_AT_AVRCP_CT_DISCONN = 0x094A,
    BTSRV_WSTP_AT_AVRCP_CT_PRESS_BUTTON = 0x094B,
    BTSRV_WSTP_AT_AVRCP_CT_SET_ABS_VOLUME = 0x094C,
    BTSRV_WSTP_AT_AVRCP_CT_GET_TG_PLAY_STAT = 0x094D,
    BTSRV_WSTP_AT_AVRCP_CT_GET_ELEMENT_ATTRI = 0x094E,
    BTSRV_WSTP_AT_AVRCP_CT_GET_SUPPORT_EVENT = 0x094F,
    BTSRV_WSTP_AT_AVRCP_CT_REG_SUPPORT_EVENT = 0x0950,
    BTSRV_WSTP_AT_AVRCP_CT_GET_CONN_STAT = 0x0951,
    BTSRV_WSTP_AT_AVRCP_CT_GET_SUPPORT_COMPANIES = 0x0952,
    BTSRV_WSTP_AT_A2DP_SRC_REG = 0x0953,
    BTSRV_WSTP_AT_A2DP_SRC_DEREG = 0x0954,
    BTSRV_WSTP_AT_A2DP_SRC_GET_DEVICE_CONN_STATE = 0x0955,
    BTSRV_WSTP_AT_A2DP_SRC_GET_PLAYING_STATE = 0x0956,
    BTSRV_WSTP_AT_A2DP_SRC_CONN = 0x0957,
    BTSRV_WSTP_AT_A2DP_SRC_DISCONN = 0x0958,
    BTSRV_WSTP_AT_A2DP_SRC_GET_ACTIVE_DEVICE = 0x0959,
    BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_START = 0x095A,
    BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_SUSPEND = 0x095B,
    BTSRV_WSTP_AT_A2DP_SRC_SET_PLAY_STOP = 0x095C,
    BTSRV_WSTP_AT_BLE_GATT_CALLBACK_REG = 0x095D,
    BTSRV_WSTP_AT_HFP_REG = 0x095E,
    BTSRV_WSTP_AT_HFP_HF_CONN = 0x095F,
    BTSRV_WSTP_AT_HFP_HF_DISCONN = 0x0960,
    BTSRV_WSTP_AT_HFP_HF_CONN_SCO = 0x0961,
    BTSRV_WSTP_AT_HFP_HF_DISCONN_SCO = 0x0962,
    BTSRV_WSTP_AT_HFP_GET_DEV_CONN_STATE = 0x0963,
    BTSRV_WSTP_AT_HFP_GET_SCO_CONN_STATE = 0x0964,
    BTSRV_WSTP_AT_HFP_SET_SPK_VOL = 0x0965,
    BTSRV_WSTP_AT_HFP_SET_MIC_VOL = 0x0966,
    BTSRV_WSTP_AT_HFP_START_DAIL = 0x0967,
    BTSRV_WSTP_AT_HFP_PICKUP = 0x0968,
    BTSRV_WSTP_AT_HFP_REJECT = 0x0969,
    BTSRV_WSTP_AT_HFP_HANGUP = 0x096A,
    BTSRV_WSTP_AT_HFP_HOLD = 0x096B,
    BTSRV_WSTP_AT_SPP_SERVER_CREATE = 0x096C,
    BTSRV_WSTP_AT_SPP_SERVER_ACCEPT = 0x096D,
    BTSRV_WSTP_AT_SPP_SERVER_CLOSE = 0x096E,
    BTSRV_WSTP_AT_SPP_CONN = 0x096F,
    BTSRV_WSTP_AT_SPP_DISCONN = 0x0970,
    BTSRV_WSTP_AT_SPP_ISCONN = 0x0971,
    BTSRV_WSTP_AT_SPP_GET_RMT_ADDR = 0x0972,
    BTSRV_WSTP_AT_SPP_READ_DATA = 0x0973,
    BTSRV_WSTP_AT_SPP_WRITE_DATA = 0x0974,
    BTSRV_WSTP_AT_SPP_READ_FILE = 0x0975,
    BTSRV_WSTP_AT_SPP_WRITE_FILE = 0x0976,
    BTSRV_WSTP_AT_SPP_RCV_DELAY = 0x0977,
    BTSRV_WSTP_AT_SPP_SEND_DELAY = 0x0978,
    BTSRV_WSTP_AT_DFT_REG = 0x0979,
    BTSRV_WSTP_AT_BT_SET_TX_POWER = 0x097A,
    BTSRV_WSTP_AT_BT_SET_DUT_MODE = 0x097B,
    BTSRV_WSTP_AT_BT_SET_SIG = 0x097C,
    BTSRV_WSTP_AT_BT_DFT_ENTER_DUT_MODE = 0x097D,
    BTSRV_WSTP_AT_BT_DFT_BREDR_TX_MODE = 0x097E,
    BTSRV_WSTP_AT_BT_DFT_BREDR_RX_MODE = 0x097F,
    BTSRV_WSTP_AT_BT_DFT_BLE_TX_MODE = 0x0980,
    BTSRV_WSTP_AT_BT_DFT_BLE_RX_MODE = 0x0981,
    BTSRV_WSTP_AT_BT_DFT_RESET_RX_COUNT = 0x0982,
    BTSRV_WSTP_AT_BT_DFT_READ_RX_COUNT = 0x0983,
    BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_TX = 0x0984,
    BTSRV_WSTP_AT_BT_COUPLING_TEST_BT_RX = 0x0985,
    BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_TX = 0x0986,
    BTSRV_WSTP_AT_BT_COUPLING_TEST_BLE_RX = 0x0987,
    BTSRV_WSTP_AT_BT_SET_CT_DELAY_MS = 0x0988,
    BTSRV_WSTP_AT_BT_SET_DONGLE_ADDR = 0x0989,
    BTSRV_WSTP_AT_BT_DFT_SET_POWRE_ALL = 0x098A,
    BTSRV_WSTP_AT_BT_DFX_SHOW_LINK_MODE = 0x098B,
    BTSRV_WSTP_AT_BT_LOG_TYPE_LEVEL = 0x098C,
    BTSRV_WSTP_AT_BLE_SHOW_CONN_LIST = 0x098D,
    BTSRV_WSTP_AT_A2DP_SNK_REG = 0x098E,
    BTSRV_WSTP_AT_A2DP_SNK_DEREG = 0x098F,
    BTSRV_WSTP_AT_A2DP_SNK_GET_DEVICE_CONN_STATE = 0x0990,
    BTSRV_WSTP_AT_A2DP_SNK_GET_PLAYING_STATE = 0x0991,
    BTSRV_WSTP_AT_A2DP_SNK_CONN = 0x0992,
    BTSRV_WSTP_AT_A2DP_SNK_DISCONN = 0x0993,
    BTSRV_WSTP_AT_A2DP_SNK_GET_ACTIVE_DEVICE = 0x0994,
    BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_START = 0x0995,
    BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_SUSPEND = 0x0996,
    BTSRV_WSTP_AT_A2DP_SNK_SET_PLAY_STOP = 0x0997,
    BTSRV_WSTP_AT_BT_CT_RX_COUNT = 0x09FF,
};

enum {
    BTSRV_WSTP_HFP_PICKUP = 0x00,
    BTSRV_WSTP_HFP_HANGUP = 0x01,
    BTSRV_WSTP_HFP_REJECT = 0x02,
};

typedef enum {
    OHOS_BT_STATUS_SUCCESS = 0x00,  /**< error code of success CNcomment:执行成功错误码 CNend */
    OHOS_BT_STATUS_FAIL,            /**< error code of failure CNcomment:执行失败错误码 CNend */
    OHOS_BT_STATUS_NOT_READY,       /**< error code of not ready CNcomment:执行状态未就绪错误码 CNend */
    OHOS_BT_STATUS_NOMEM,           /**< error code of memmery not enough CNcomment:内存不足错误码 CNend */
    OHOS_BT_STATUS_BUSY,            /**< error code of system is busy CNcomment:繁忙无法响应错误码 CNend */
    OHOS_BT_STATUS_DONE,            /**< error code of complete CNcomment:执行完成错误码 CNend */
    OHOS_BT_STATUS_UNSUPPORTED,     /**< error code of not support CNcomment:不支持错误码 CNend */
    OHOS_BT_STATUS_PARM_INVALID,    /**< error code of invalid param CNcomment:无效参数错误码 CNend */
    OHOS_BT_STATUS_UNHANDLED,       /**< error code of unhandle CNcomment:未处理错误码 CNend */
    OHOS_BT_STATUS_AUTH_FAILURE,    /**< error code of auth failure CNcomment:鉴权失败错误码 CNend */
    OHOS_BT_STATUS_RMT_DEV_DOWN,    /**< error code of remote device down CNcomment:远端设备关闭错误码 CNend */
    OHOS_BT_STATUS_AUTH_REJECTED    /**< error code of auth rejected CNcomment:鉴权被拒错误码 CNend */
} BtStatus;

typedef enum {
    OHOS_HFP_HF_ACCEPT_CALL_ACTION_NONE = 0x00,     /**< HFP HF phone calling action of none
                                                         CNcomment:接受呼叫后无操作CNend */
    OHOS_HFP_HF_ACCEPT_CALL_ACTION_HOLD = 0x01,     /**< HFP HF phone calling action of hold
                                                         CNcomment:接受呼叫保持当前通话CNend */
    OHOS_HFP_HF_ACCEPT_CALL_ACTION_FINISH = 0x02    /**< HFP HF phone calling action of finish
                                                         CNcomment:接受呼叫后结束当前通话CNend */
} hfp_hf_accept_call_action_type;

/**
 * @ingroup ohos_bt_gap
 *
 * Enum of Bluetooth scan mode. CNcomment:蓝牙扫描模式。CNend
 */
typedef enum {
    OHOS_GAP_SCAN_MODE_NONE = 0x00,                         /**< scan type of none CNcomment:未知扫描类型 CNend */
    OHOS_GAP_SCAN_MODE_CONNECTABLE,                         /**< scan type of connection CNcomment:可连接扫描类型 CNend */
    OHOS_GAP_SCAN_MODE_GENERAL_DISCOVERABLE,                /**< scan type of general discovery
                                                                 CNcomment:通用广播扫描类型 CNend */
    OHOS_GAP_SCAN_MODE_LIMITED_DISCOVERABLE,                /**< scan type of limited discovery
                                                                 CNcomment:受限广播扫描类型 CNend */
    OHOS_GAP_SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE,    /**< scan type of connection and general discovery
                                                                 CNcomment:可连接的通用广播扫描类型 CNend */
    OHOS_GAP_SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE     /**< scan type of connection and limited discovery
                                                                CNcomment:可连接的受限广播扫描类型 CNend */
} GapBtScanMode;

typedef struct {
    /** 地址 */
    unsigned char addr[BT_MAC_LEN];   /**< bluetooth device address CNcomment:蓝牙地址 CNend */
    uint8_t type;    /**< bluetooth device address type CNcomment:蓝牙地址类型 CNend */
} bth_bd_addr;

/**
 * @ingroup ohos_bt_def
 *
 * Enum of hfp volume type. CNcomment:定义蓝牙hfp的音量调整类型。CNend
 */
typedef enum {
    OHOS_HFP_VOLUME_MIC = 0x00,     /**< volume type of microphone CNcomment:调整microphone增益 CNend */
    OHOS_HFP_VOLUME_SPEAKER         /**< volume type of speaker CNcomment:调整speaker增益 CNend */
} bt_hfp_volume_type;

typedef void (*spp_apk_rcv_callback)(uint8_t *data, uint32_t data_len);

typedef struct {
    spp_apk_rcv_callback spp_apk_rcv_cb;
} spp_apk_callbacks_t;

#ifndef BTOU16
#define BTOU16(x) (((*(x)) << 8) | (*((x) + 1)))
#endif

void BtManagerApSendDataToBt(uint8_t serviceID, uint8_t commandID, int32_t sendLength, const uint8_t *data);
bool McBtManagerFactoryReset(void);
bool McBtManagerCleanKeyReset(void);
void McBtManagerInit(void);

// For bth compile warning
extern int8_t mc_ble_gatt_create_server_req_atb(void);
extern int bt_at_cmd_ble_enter_scan(int argc, char *argv[]);
extern int bt_at_cmd_multiple_value_nfy(int argc, char *argv[]);
extern int bt_at_cmd_set_le_att_mtu_param(int argc, char *argv[]);
extern int bt_at_cmd_read_character(int argc, char *argv[]);
extern int bt_at_cmd_read_multiple_character(int argc, char *argv[]);
extern int bt_at_cmd_multiple_value_read_character(int argc, char *argv[]);
extern int bts_at_cmd_set_att_key_size(int argc, char *argv[]);
extern int bts_at_cmd_gatt_create_service(int argc, char *argv[]);
extern int bts_at_cmd_gatt_add_include_service(int argc, char *argv[]);
extern int bts_at_cmd_gatt_add_characteristic(int argc, char *argv[]);
extern int bts_at_cmd_gatt_add_descriptor(int argc, char *argv[]);
extern int bts_at_cmd_gatt_start_service(int argc, char *argv[]);
extern int bts_at_cmd_gatt_send_response(int argc, char *argv[]);
extern int bts_at_cmd_gatt_notify_characteristic_changed(int argc, char *argv[]);
extern int bts_at_cmd_gatt_update_descriptor_value_changed(int argc, char *argv[]);
extern int bts_at_cmd_gatt_get_gap_ui_attributes(int argc, char *argv[]);
extern int bts_at_cmd_gatt_set_gap_ui_attributes(int argc, char *argv[]);
extern bool gap_br_set_bt_scan_mode(int mode, int duration);
extern bool wstp_connect_remote_device(const bth_bd_addr *bd_addr);
extern int bts_at_cmd_gatt_discovery_service(int argc, char *argv[]);
extern int bts_at_cmd_gatt_find_include_service(int argc, char *argv[]);
extern int bts_at_cmd_gatt_discovery_characteristics(int argc, char *argv[]);
extern int bts_at_cmd_gatt_discovery_characteristics_by_uuid(int argc, char *argv[]);
extern int bts_at_cmd_gatt_read_characteristics_by_uuid(int argc, char *argv[]);
extern int bts_at_cmd_gatt_discovery_descriptor_by_handle(int argc, char *argv[]);
extern int bts_at_cmd_gatt_write_characteristic_value(int argc, char *argv[]);
extern int bts_at_cmd_gatt_write_descriptor_value(int argc, char *argv[]);
extern int bts_at_cmd_gatt_mtu_config(int argc, char *argv[]);
extern int bts_at_cmd_gatt_get_report_map(int argc, char *argv[]);
extern int pbap_pce_disconnect(const bth_bd_addr *bd_addr);
extern int bts_at_cmd_gatt_reliable_write_characteristic_value(int argc, char *argv[]);
extern int pbap_pce_connect(const bth_bd_addr *bd_addr);
extern uint8_t ble_conn_gap_start_adv(void);
extern uint8_t ble_conn_gap_stop_adv(void);

// For bth wstp
extern errcode_t enable_bt_stack(void);
extern errcode_t disable_bt_stack(void);
extern errcode_t enable_ble(void);
extern errcode_t bluetooth_set_local_addr(unsigned char *mac, unsigned int len);
extern errcode_t bluetooth_get_local_addr(unsigned char *mac, unsigned int len);
extern errcode_t bluetooth_set_local_name(const unsigned char *localName, unsigned char length);
extern errcode_t bluetooth_get_local_name(unsigned char *localName, unsigned char *length);
extern bool bluetooth_factory_reset(void);
extern bool gap_br_start_discovery(void);
extern bool gap_br_cancel_discovery(void);
extern int hfp_hf_accept_incoming_call(const bth_bd_addr *bd_addr, hfp_hf_accept_call_action_type action_type);
extern int hfp_hf_finish_call(const bth_bd_addr *bd_addr);
extern int hfp_hf_reject_incoming_call(const bth_bd_addr *bd_addr);
extern void btsrv_at_cmd_gap_register_callbacks(void);
extern void wstp_hfp_hf_reg_callback(void);
extern void wstp_pbap_pce_reg_callback(void);
extern void reset_found_dev_list(void);
extern void update_found_dev_list(void);
extern void show_found_dev_list(void);
extern int hfp_hf_set_volume(const bth_bd_addr *bd_addr, bt_hfp_volume_type type, uint8_t volume);
extern int hfp_hf_start_dial(const bth_bd_addr *bd_addr, const uint8_t* number, uint8_t len);
extern void wstp_avrcp_register_callbacks(void);
extern void wstp_avrcp_disregister_callbacks(void);
extern int wstp_avrcp_tg_connect(const bth_bd_addr *bd_addr);
extern int wstp_avrcp_tg_disconnect(const bth_bd_addr *bd_addr);
extern int wstp_avrcp_tg_get_dev_conn_state(const bth_bd_addr *bd_addr);
extern void wstp_avrcp_tg_notify_state(const uint8_t *params, int32_t params_cnt);
extern void wstp_avrcp_tg_set_loc_inf(const uint8_t *params, int32_t params_cnt);
extern void wstp_avrcp_ct_conn_and_reg(const bth_bd_addr *bd_addr);
extern int wstp_avrcp_ct_disconnect(const bth_bd_addr *bd_addr);
extern void wstp_avrcp_ct_press_button(const bth_bd_addr *bd_addr, int32_t key_operation);
extern int wstp_bts_avrcp_ct_set_abs_volume(const bth_bd_addr *bd_addr, uint8_t volume);
extern int wstp_avrcp_ct_get_tg_player_stat(const bth_bd_addr *bd_addr);
extern void wstp_avrcp_ct_get_element_attributes(const bth_bd_addr *bd_addr);
extern int wstp_avrcp_ct_get_support_event(const bth_bd_addr *bd_addr);
extern void wstp_avrcp_ct_reg_support_event(const bth_bd_addr *bd_addr, uint8_t event_id, uint32_t interval);
extern int wstp_avrcp_ct_get_conn_stat(const bth_bd_addr *bd_addr);
extern int wstp_a2dp_src_register_callbacks(void);
extern int wstp_a2dp_snk_register_callbacks(void);
extern int wstp_a2dp_src_deregister_callbacks(void);
extern int wstp_a2dp_src_get_device_connect_state(const bth_bd_addr *bdAddr);
extern int wstp_a2dp_src_get_playing_state(const bth_bd_addr *bdAddr);
extern int wstp_a2dp_src_connect(const bth_bd_addr *bdAddr);
extern int wstp_a2dp_src_disconnect(const bth_bd_addr *bdAddr);
extern void wstp_a2dp_src_get_active_device(void);
extern void wstp_avrcp_ct_hdl_notify_play_status(uint8_t play_status);
extern void wstp_pbap_cmd_pull_phonebook(
    uint8_t folder, uint8_t vcf, uint16_t max_list_count, uint16_t list_offset, uint8_t format);
extern void wstp_pbap_cmd_pull_vcard_entry(uint8_t folder, uint8_t vcf, char *vcf_name, uint8_t format);
extern int pbap_pce_stop_trans(const bth_bd_addr *bd_addr);
extern int pbap_pce_get_device_connect_state(const bth_bd_addr *bd_addr);
extern int hfp_hf_connect(const bth_bd_addr *bd_addr);
extern int hfp_hf_disconnect(const bth_bd_addr *bd_addr);
extern int hfp_hf_connect_sco(const bth_bd_addr *bd_addr);
extern int hfp_hf_disconnect_sco(const bth_bd_addr *bd_addr);
extern int hfp_hf_get_device_connect_state(const bth_bd_addr *bd_addr);
extern int hfp_hf_get_sco_connect_state(const bth_bd_addr *bd_addr);
extern int hfp_hf_hold_active_call(const bth_bd_addr *bd_addr);
extern int pbap_print_heap_info_req(void);
extern int a2dp_src_start_playing(const bth_bd_addr *bd_addr);
extern int a2dp_src_suspend_playing(const bth_bd_addr *bd_addr);
extern int a2dp_src_stop_playing(const bth_bd_addr *bd_addr);
extern int bt_service_sample_cmd(int argc, char *argv[]);
extern void btsrv_dfx_show_conn_list(void);
extern void btsrv_dfx_show_remote_dev_list(void);
extern void wstp_get_paired_devices_num(void);
extern void wstp_get_paired_devices_list(void);
extern void wstp_get_pair_state(const bth_bd_addr *bd_addr);
extern int wstp_avrcp_ct_get_support_companies(const bth_bd_addr *bd_addr);
extern void wstp_spp_cmd_server_create(char *spp_uuid, char *name, uint8_t name_len);
extern void wstp_spp_cmd_server_accept(uint32_t server_id);
extern void wstp_spp_cmd_connect(char *spp_uuid, char *addr);
extern void wstp_spp_cmd_disconnect(uint32_t client_id);
extern void wstp_spp_cmd_is_spp_connect(int32_t client_id);
extern void wstp_spp_cmd_get_remote_addr(int32_t client_id);
extern void wstp_spp_cmd_read(int32_t client_id, int32_t buf_len, int32_t duration);
extern void wstp_spp_cmd_write(int32_t client_id, int32_t repeat_times, int32_t cycle_num, int32_t delay_time);
extern void wstp_spp_cmd_server_close(int32_t server_id);
extern int spp_apk_register_callbacks(spp_apk_callbacks_t *func);
extern ext_errno bt_dft_reg_callbacks(void);
extern uint32_t bt_set_tx_power(const bth_bd_addr *addr, uint8_t pwr_level);
extern void btsrv_dfx_show_link_mode(void);
int sample_spp_register_callbacks(void);
void bt_wstp_dft_set_dut_mode(uint8_t mode);
void bt_wstp_dft_set_sig_cmd(uint8_t freq, uint8_t mode, uint8_t switch_trx);
void btsrv_set_log_type_level(uint32_t log_type_level);

#endif
