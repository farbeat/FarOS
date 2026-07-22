/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_GAP_IF_H
#define STATCK_GAP_IF_H

#include <stdint.h>
#include "ipc.h"
#include "stack_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Possible link type */
#define BTSDK_SCO_LINK            0x00
#define BTSDK_ACL_LINK            0x01
#define BTSDK_ESCO_LINK           0x02

/* Default role of local device when creating a new ACL connection. */
#define BTSDK_MASTER_ROLE                        0x00
#define BTSDK_SLAVE_ROLE                        0x01
#define BTSDK_MASTER_SLAVE                        0x02

/* Device type for upper layer */
#define BTSDK_DEV_TYPE_LE_ONLY       0x01
#define BTSDK_DEV_TYPE_BREDR_ONLY    0x10
#define BTSDK_DEV_TYPE_BREDR_LE      0x11

/* Link type for btsdk_create_acl_link */
#define BTSDK_LINK_TYPE_LE              0x01
#define BTSDK_LINK_TYPE_BREDR           0x02

/* Device Address type for upper layer */
#define BTSDK_DEV_ADDR_TYPE_PUBLIC    0x00
#define BTSDK_DEV_ADDR_TYPE_RANDOM    0x01

#define BD_ADDR_LEN    6    /* bluetooth address length */

/* Discovery Mode for BTSDK_SetDiscoveryMode() and BTSDK_GetDiscoveryMode() */
#define BTSDK_VM_DISABLE                          0x0000
#define BTSDK_GENERAL_DISCOVERABLE                0x01
#define BTSDK_LIMITED_DISCOVERABLE                0x02
#define BTSDK_DISCOVERABLE                        BTSDK_GENERAL_DISCOVERABLE
#define BTSDK_CONNECTABLE                        0x04
#define BTSDK_PAIRABLE                            0x08
#define BTSDK_DISCOVERY_DEFAULT_MODE            (BTSDK_DISCOVERABLE | BTSDK_CONNECTABLE | BTSDK_PAIRABLE)

/* type of Callback Indication */
enum {
    BTSDK_PIN_CODE_REQ_IND,         // 0x00
    BTSDK_LINK_KEY_REQ_IND,         // 0x01
    BTSDK_LINK_KEY_NOTIF_IND,       // 0x02
    BTSDK_AUTHENTICATION_FAIL_IND,  // 0x03
    BTSDK_INQUIRY_RESULT_IND,       // 0x04
    BTSDK_INQUIRY_COMPLETE_IND,     // 0x05
    BTSDK_AUTHORIZATION_IND,        // 0x06
    BTSDK_CONNECTION_REQUEST_IND,   // 0x07
    BTSDK_CONNECTION_COMPLETE_IND,  // 0x08
    BTSDK_CONNECTION_EVENT_IND,     // 0x09
    BTSDK_AUTHORIZATION_ABORT_IND,  // 0x0A
    BTSDK_VENDOR_EVENT_IND,         // 0x0B

    /* if: BT2.1 Supported */
    BTSDK_IOCAP_REQ_IND,            // 0x0C
    BTSDK_USR_CFM_REQ_IND,          // 0x0D
    BTSDK_PASSKEY_REQ_IND,          // 0x0E
    BTSDK_REM_OOBDATA_REQ_IND,      // 0x0F
    BTSDK_PASSKEY_NOTIF_IND,        // 0x10
    BTSDK_SIMPLE_PAIR_COMPLETE_IND, /* 0x11, use this event to kill window only */

    BTSDK_OBEX_AUTHEN_REQ_IND,        // 0x12
    BTSDK_DISCONNECTION_COMPLETE_IND, // 0x13
    BTSDK_SHORTCUT_EVENT_IND,         // 0x14
    BTSDK_DEVICE_FOUND_IND,           // 0x15

    BTSDK_READ_WHITE_LIST_SIZE,              // 0x16
    BTSDK_READ_ADVERTISING_CHANNEL_TX_POWER, // 0x17
    BTSDK_READ_TRANSMIT_POWER_LEVEL,  // 0x18
    BTSDK_ADD_DEVICE_TO_WHITE_LIST,
    BTSDK_REMOVE_DEVICE_FROM_WHITE_LIST,
    BTSDK_CLEAR_WHITE_LIST,

    BTSDK_ENUM_MAX     // let it at the bottom of the array
};

#define BTSDK_MAX_EVENT_NUM            BTSDK_ENUM_MAX
#define MAX_DEV_NAME_SIZE 128

/* Distinguishing whether the broadcast data is BR or BLE in callback */
typedef enum {
    BTSDK_FOUND_BR_DEV_DATA =                                        0x00,
    BTSDK_FOUND_BLE_DEV_DATA =                                       0x01
} bt_sdk_dev_found_data_type;

/* OBEX authentication information MAX length */
#define BTSDK_MAX_USERID_LEN            20  /* Shall be the same as MAX_USER_ID_LEN. */
#define BTSDK_MAX_PWD_LEN               64  /* Shall be the same as MAX_PWD_LEN. */

typedef struct {
    char s_dev_name[MAX_DEV_NAME_SIZE];
    uint32_t class_of_device;
    uint16_t appearance;
    uint8_t bd_addr[BD_ADDR_LEN];
    /* etc, HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS = HCI_PEER_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS */
    uint8_t a_type;
    uint8_t u_cap;     /* etc, HCI_ADDRESS_CAP_LE, reserved for compatibility only. */
    int8_t rssi;    /* RSSI, Range -127 <= rssi <= 20; 127 means RSSI is not available */
    bt_sdk_dev_found_data_type data_type;
    uint16_t eir_size; /* Size of the Extended Inquiry Response Data or LE Advertising Report Data */
    uint8_t *eir_data; /* Extended Inquiry Response Data or LE Advertising Report Data. The content may be different
                                from the one got from the device discovery procedure. The Host Stack may modify it,
                                    e.g.
                       add the complete name got from name discovery procedure, add a service list got from service
                                discovery procedure. The Host Stack will construct an EIR packet for a device not
                       supporting EIR. */
} bt_sdk_dev_found_info;

typedef struct bt_sdk_conn_dev_info {
    uint8_t link_type; /* 1:LE ACL Link  2:BR/EDR ACL Link */
    uint8_t bd[BD_ADDR_LEN]; /* 6octets, 48bits */
    uint8_t atype; /* etc, HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS = HCI_PEER_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS */
    uint8_t cap; /* etc, HCI_ADDRESS_CAP_LE, reserved for compatibility only. */
} bt_sdk_conn_dev_info_t;

typedef void (btsdk_obex_authen_rsp_func)(void *conn_handle, const uint8_t *user_id, const uint8_t *password);
typedef struct _bt_sdk_obex_auth_info_stru {
    uint32_t dev_hdl;     /* [in] Device handle of the OBEX server */
    uint16_t svc_type;    /* [in] type of the OBEX server */
    uint8_t   pwd_only;    /* [in] whether the user id is needed */
    btsdk_obex_authen_rsp_func *rsp_func;           /* [in] Function used to send authentication response */
    uint8_t  user_id[BTSDK_MAX_USERID_LEN + 1];        /* [out] the user id string    */
    uint8_t     pwd[BTSDK_MAX_PWD_LEN + 1];                /* [out] the password string */
} bt_sdk_obex_auth_info_stru, *p_bt_sdk_obex_auth_info_stru;

typedef struct _bt_sdk_vendor_cmd_stru {
    uint16_t ocf;            /* OCF Range (10 bits): 0x0000-0x03FF */
    uint8_t     param_len;        /* length of param in bytes */
    uint8_t     param[1];        /* Parameters to be packed in the vendor command. Little endian is adopted. */
} bt_sdk_vendor_cmd_stru, *p_bt_sdk_vendor_cmd_stru;

typedef struct {
    uint16_t mask;                               /* Decide which parameter to be modified or retrieved */
    uint16_t service_class;                       /* Service class, 16bit UUID */
    int8_t svc_name[BTSDK_SERVICENAME_MAXLENGTH]; /* must in UTF-8 */
    uint16_t security_level;                      /* Authorization, Authentication, Encryption, None */
    /* Accept, Prompt, Reject (untrusted device), combined with security level "Authorization" */
    void *ext_attributes; /* Profile specific attributes */
    uint16_t author_method;
    uint8_t multiple; /* Allow register the same class Service, 0:only one, 1~x:rest parts */
} bt_sdk_local_server_attr_stru, *p_bt_sdk_local_server_attr_stru;

typedef struct {
    uint16_t mask;                                  /* Decide which parameter to be retrieved */
    union {
        uint16_t svc_class;                         /* For Compatibility */
        uint16_t service_class;
    };                                              /* type of this service record */
    uint32_t dev_hdl;                               /* Handle to the remote device which provides this service. */
    int8_t svc_name[BTSDK_SERVICENAME_MAXLENGTH];  /* Service name in UTF-8 */
    void *ext_attributes;                        /* Free by the APP */
    uint16_t status;
} bt_sdk_remote_service_attr_stru, *p_bt_sdk_remote_service_attr_stru;

typedef struct {
    uint8_t link_key[BTSDK_LINKKEY_LEN];    /* link key value */
    uint8_t key_type;                       /* etc: HCI_KEY_TYPE_COMBINATION_KEY */
    uint8_t accept;                        /* TRUE for Reply or recover key, FALSE for Negative Reply.
                                          valid in key request, and the notification is ignored. */
} br_security_link_key_stru;

/* Parameters of Sniff_Mode command */
typedef struct _bt_sdk_sniff_mode_stru {
    uint16_t conn_hdl;                    /* reserved, set it to 0. */
    uint16_t max;                        /* Sniff mode max interval. */
    uint16_t min;                        /* Sniff mode min interval. */
    uint16_t attempt;                    /* Sniff mode attempt value. */
    uint16_t timeout;                    /* Sniff mode timeout value. */
} bt_sdk_sniff_mode_stru;
typedef bt_sdk_sniff_mode_stru* p_bt_sdk_sniff_mode_stru;

typedef struct _bt_sdk_connection_property_stru {
    uint32_t role : 2;
    uint32_t result : 30;
    uint32_t device_handle;
    uint32_t service_handle;
    uint16_t service_class;
    uint32_t duration;
    uint8_t bd_addr[BTSDK_BDADDR_LEN];
#ifdef CONFIG_BTSDK_FLOW_STATISTIC
    uint32_t received_bytes;
    uint32_t sent_bytes;
#endif
} bt_sdk_connection_property_stru, *p_bt_sdk_connection_property_stru;

/* value range for typedef struct ble_adv_para.adv_filter_policy */
typedef enum ble_adv_filter_policy {
    BLE_ADV_FILTER_POLICY_SCAN_ANY_CONNECT_ANY =                     0x00,
    BLE_ADV_FILTER_POLICY_SCAN_WHITE_LIST_CONNECT_ANY =              0x01,
    BLE_ADV_FILTER_POLICY_SCAN_ANY_CONNECT_WHITE_LIST =              0x02,
    BLE_ADV_FILTER_POLICY_SCAN_WHITE_LIST_CONNECT_WHITE_LIST =       0x03
} ble_adv_filter_policy_t;

/* value range for typedef struct ble_adv_para.adv_type */
typedef enum ble_adverting_type {
    BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED =                            0x00,
    BLE_ADV_TYPE_CONNECTABLE_HIGH_DUTY_CYCLE_DIRECTED =              0x01,
    BLE_ADV_TYPE_SCANNABLE_UNDIRECTED =                              0x02,
    BLE_ADV_TYPE_NON_CONNECTABLE_UNDIRECTED =                        0x03,
    BLE_ADV_TYPE_CONNECTABLE_LOW_DUTY_CYCLE_DIRECTED =               0x04
} ble_adverting_type_t;

/* value range for typedef struct ble_adv_para.adv_channel_map */
typedef enum ble_adv_channel_map {
    BLE_ADV_CHANNEL_MAP_CH_37 =                      0x01,
    BLE_ADV_CHANNEL_MAP_CH_38 =                      0x02,
    BLE_ADV_CHANNEL_MAP_CH_39 =                      0x04,
    BLE_ADV_CHANNEL_MAP_CH_37_CH_38 =                0x03,
    BLE_ADV_CHANNEL_MAP_CH_37_CH_39 =                0x05,
    BLE_ADV_CHANNEL_MAP_CH_38_CH_39 =                0x06,
    BLE_ADV_CHANNEL_MAP_CH_DEFAULT =                 0x07
} ble_adv_channel_map_t;


/* advertising parameters stru
   1.when adv_type == 0x01, advIntervalMin & advIntervalMax won't be used.
   2.when own_addr_type == 0x02 or 0x03,peer_addr contains identity Address & type;These parameters are
used to locate the corresponding local IRK in the resolving list; this IRK is used
to generate the own address used in the advertisement.
   3.when adv_type == 0x01 or 0x04, peerAddrType and peer_addr shall be valid.
   4.when directed advertising(adv_type == 0x01 or 0x04) is enabled, adv_filter_policy won't be used.
*/
typedef struct ble_adv_para {
    uint8_t peer_addr[BTSDK_BDADDR_LEN];
    uint32_t duration;         /* unit: ms */
    uint16_t adv_interval_min;  /* unit: slot, 1 slot = 0.625ms. e.g 0x0800 (1.28 s)
                             * Range: 0x20 - 0x4000, which equals 20 ~ 10240 ms */
    uint16_t adv_interval_max;  /* unit: slot, 1 slot = 0.625ms. e.g 0x0800 (1.28 s)
                             * Range: 0x20 - 0x4000, which equals 20 ~ 10240 ms */
    ble_adverting_type_t adv_type;
    ble_address_type peer_addr_type; /* BLE peerAddrType only support 0x00 & 0x01 */
    ble_adv_channel_map_t adv_channel_map;
    ble_adv_filter_policy_t adv_filter_policy;
} ble_adv_para_t;

typedef enum ble_ad_type {
    AD_DATATYPE_FLAGS =                                                 0x01,
    AD_DATATYPE_INCOMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS =          0x02,
    AD_DATATYPE_COMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS =            0x03,
    AD_DATATYPE_INCOMPLETE_LIST_OF_32BIT_SERVICE_CLASS_UUIDS =          0x04,
    AD_DATATYPE_COMPLETE_LIST_OF_32BIT_SERVICE_CLASS_UUIDS =            0x05,
    AD_DATATYPE_INCOMPLETE_LIST_OF_128BIT_SERVICE_CLASS_UUIDS =         0x06,
    AD_DATATYPE_COMPLETE_LIST_OF_128BIT_SERVICE_CLASS_UUIDS =           0x07,
    AD_DATATYPE_SHORTENED_LOCAL_NAME =                                  0x08,
    AD_DATATYPE_COMPLETE_LOCAL_NAME =                                   0x09,
    AD_DATATYPE_TX_POWER_LEVEL =                                        0x0A,
    AD_DATATYPE_CLASS_OF_DEVICE =                                       0x0D,
    AD_DATATYPE_SIMPLE_PAIRING_HASH_C192 =                              0x0E,
    AD_DATATYPE_SIMPLE_PAIRING_RANDOMIZER_R192 =                        0x0F,
    AD_DATATYPE_DEVICE_ID =                                             0x10,
    AD_DATATYPE_SECURITY_MANAGER_TK_VALUE =                             0x10,
    AD_DATATYPE_SECURITY_MANAGER_OUT_OF_BAND_FLAGS =                    0x11,
    AD_DATATYPE_SLAVE_CONNECTION_INTERVAL_RANGE =                       0x12,
    AD_DATATYPE_LIST_OF_16BIT_SERVICE_SOLICITATION_UUIDS =              0x14,
    AD_DATATYPE_LIST_OF_128BIT_SERVICE_SOLICITATION_UUIDS =             0x15,
    AD_DATATYPE_SERVICE_DATA =                                          0x16,
    AD_DATATYPE_SERVICE_DATA_16BIT_UUID =                               0x16,
    AD_DATATYPE_PUBLIC_TARGET_ADDRESS =                                 0x17,
    AD_DATATYPE_RANDOM_TARGET_ADDRESS =                                 0x18,
    AD_DATATYPE_APPEARANCE =                                            0x19,
    AD_DATATYPE_ADVERTISING_INTERVAL =                                  0x1A,
    AD_DATATYPE_LE_BLUETOOTH_DEVICE_ADDRESS =                           0x1B,
    AD_DATATYPE_LE_ROLE =                                               0x1C,
    AD_DATATYPE_SIMPLE_PAIRING_HASH_C256 =                              0x1D,
    AD_DATATYPE_SIMPLE_PAIRING_RANDOMIZER_R256 =                        0x1E,
    AD_DATATYPE_LIST_OF_32BIT_SERVICE_SOLICITATION_UUIDS =              0x1F,
    AD_DATATYPE_SERVICE_DATA_32BIT_UUID =                               0x20,
    AD_DATATYPE_SERVICE_DATA_128BIT_UUID =                              0x21,
    AD_DATATYPE_LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE =              0x22,
    AD_DATATYPE_LE_SECURE_CONNECTIONS_RANDOM_VALUE =                    0x23,
    AD_DATATYPE_URI =                                                   0x24,
    AD_DATATYPE_INDOOR_POSITIONING =                                    0x25,
    AD_DATATYPE_TRANSPORT_DISCOVERY_DATA =                              0x26,
    AD_DATATYPE_LE_SUPPORTED_FEATURES =                                 0x27,
    AD_DATATYPE_CHANNEL_MAP_UPDATE_INDICATION =                         0x28,
    AD_DATATYPE_3D_INFORMATION_DATA =                                   0x3D,
    AD_DATATYPE_MANUFACTURER_SPECIFIC_DATA =                            0xFF
} ble_ad_type_t;

/* value range for typedef struct ble_scan_para.scan_type */
typedef enum ble_scaning_type {
    BLE_SCAN_TYPE_PASSIVE_SCANNING = 0x00,   /* Local won't send scan_req */
    BLE_SCAN_TYPE_ACTIVE_SCANNING = 0x01     /* Local will send scan_req */
} ble_scaning_type_t;

/* value range for typedef struct ble_scan_para.scan_filter_policy:
   0, Accept all advertisement packets except directed advertising packets not
   addressed to this device (default);
   1, Accept only advertisement packets from devices where the advertiser's
   address is in the White list. Directed advertising packets which are not
   addressed for this device shall be ignored;
   2, Accept all undirected advertisement packets, and all directed advertising
   packets where the initiator address is a resolvable private address, and all
   directed advertising packets addressed to this device;
   3, Accept all undirected advertisement packets from devices where the
   advertiser's address is in the White list, and all directed advertising
   packets where the initiator address is a resolvable private address, and all
   directed advertising packets addressed to this device. */
typedef enum ble_scaning_filter_policy {
    BLE_SCANNING_FILTER_POLICY_ACCEPT_ALL                = 0x00,
    BLE_SCANNING_FILTER_POLICY_ONLY_WHITE_LIST           = 0x01,
    BLE_SCANNING_FILTER_POLICY_ACCEPT_ALL_AND_RPA        = 0x02,
    BLE_SCANNING_FILTER_POLICY_ONLY_WHITE_LIST_AND_RPA   = 0x03
} ble_scaning_filter_policy_t;

/* ble gap */
typedef struct ipc_add_le_dev_para {
    uint32_t addr_type;
    uint8_t bd_addr[BD_ADDR_LEN];
} ipc_add_le_dev_para_t;

#define MAX_DATA_LENGTH 30
typedef struct ipc_adv_data {
    uint8_t ad_type;
    uint8_t length;
    uint8_t data[MAX_DATA_LENGTH];
} ipc_adv_data_t;

typedef struct ipc_set_le_adv_data {
    uint8_t adv_cnt;
    uint8_t scan_rsp_cnt;
    ipc_adv_data_t data[0]; /* adv_cnt + scan_rsp_cnt */
} ipc_set_le_adv_data_t;

/* scan parameters stru */
typedef struct ble_scan_para {
    uint32_t duration;
    uint16_t scan_interval;    /* unit: slot, 1 slot = 0.625ms. e.g 0x0800 (1.28 s)
                             * Range: 0x04 - 0x4000, which equals 2.5 ~ 10240 ms */
    uint16_t scan_window;      /* unit: slot, 1 slot = 0.625ms. e.g 0x0800 (1.28 s)
                             * Range: 0x04 - 0x4000, which equals 2.5 ~ 10240 ms */
    ble_scaning_type_t scan_type;
    ble_address_type own_addr_type;
    ble_scaning_filter_policy_t scan_filter_policy;
} ble_scan_para_t;

typedef struct adv_scan_rsp_data {
    ble_ad_type_t ad_type;
    uint8_t length;
    uint8_t *payload;
} adv_data_t, scan_rsp_data_t;

struct hci_address_stru {
    uint8_t bd[BD_ADDR_LEN];                        /* 6octets, 48bits */
    /* etc, HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS = HCI_PEER_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS */
    uint8_t atype;
    uint8_t cap;                                    /* etc, HCI_ADDRESS_CAP_LE, reserved for compatibility only. */
};

struct gap_create_connection_cfm_stru {
    struct hci_address_stru  addr;  /* Remote Address */
    uint16_t acl_hdl;               /* The connection handle of the ACL link */
    uint8_t link_type;              /* type of the ACL link created. One of t_gap_acl_link_type */
};

struct gap_execute_command_cfm_stru {
    /* Specifies the command confirmed, e.g. HCI_OPS_INQUIRY. It also specifies the content of the event_parameter. */
    uint32_t opindex;
    uint32_t size;            /* Specifies the size, in bytes, of the event_parameter buffer. */
    void   *event_parameter; /* Pointer to a command specific data structure contains the
                                parameters of the complete event, e.g. hci_inquiry_complete_ev_stru. */
};

#define CONFIG_HCI_REMOTE_NAME_MAX_LEN 248
struct hci_remote_name_request_complete_ev_stru {
    uint8_t status;                          /* [001],Enum */
    uint8_t bd[6];                           /* [125],Val */
    uint8_t remote_name[CONFIG_HCI_REMOTE_NAME_MAX_LEN];                /* [162],String */
};

typedef struct {
    unsigned char addr[BD_ADDR_LEN]; /*!< @if Eng bluetooth device address @else 蓝牙地址 @endif */
} bth_addr_t;

typedef struct {
    uint32_t dev_hdl;
    uint32_t dev_class;
    uint8_t link_type;
    bth_addr_t bd_addr;
} connect_req_ind_struct;

typedef struct {
    uint8_t op;
    void *func;
} bt_sdk_callback_stru;

enum {
    ACTIVE_SCENE_IDLE = 0x00,
    ACTIVE_SCENE_A2DP_SBC = 0x01,
    ACTIVE_SCENE_A2DP_AAC = 0x02,
    ACTIVE_SCENE_A2DP_HWA = 0x03,
    ACTIVE_SCENE_SCO = 0x10,
};

enum {
    ACTIVE_SCENE_LOW_BITRATE = 0x00,
    ACTIVE_SCENE_MID_BITRATE = 0x01,
    ACTIVE_SCENE_HIGH_BITRATE = 0x02,
};

typedef struct {
    uint8_t scene;
    uint8_t bit_rate;
    uint8_t is_low_delay;
    uint8_t addr[BD_ADDR_LEN];
} btsdk_gap_active_scene_t;

typedef struct {
    uint16_t interval;
    uint16_t window;
} btsdk_gap_inquiry_scan_param_t;

/* callback Function */
typedef void (gap_create_connection_cfm_cbk)(void *context, uint16_t result,
    struct gap_create_connection_cfm_stru *cfm_par);
typedef void (gap_execute_command_cfm_cbk)(void *context, uint16_t result,
    struct gap_execute_command_cfm_stru *cfm_par);
typedef void  (btsdk_pin_req_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_link_key_req_ind_func)(const uint8_t *remote_addr, uint8_t addr_len,
    br_security_link_key_stru *link_key);
typedef void  (btsdk_link_key_notif_ind_func)(const uint8_t *remote_addr, uint8_t addr_len,
    const br_security_link_key_stru *link_key);
typedef void  (btsdk_authentication_faill_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_inquiry_result_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_inquiry_complete_ind_func)(void);
typedef void  (btsdk_authorization_req_ind_func)(uint32_t svc_hdl, uint32_t dev_hdl);
typedef uint8_t  (btsdk_connection_request_ind_func)(uint32_t dev_hdl, uint32_t dev_class, uint8_t link_type,
    const uint8_t *remote_addr, uint8_t addr_len);
typedef void  (btsdk_connection_complete_ind_func)(uint32_t dev_hdl, bt_sdk_conn_dev_info_t *conn_dev_info);
typedef void  (btsdk_disconnection_complete_ind_func)(uint32_t dev_hdl, uint32_t reason);
typedef void  (btsdk_connection_event_ind_func)(uint32_t conn_hdl, uint16_t event, uint8_t *arg);
typedef void  (btsdk_shortcut_event_ind_func)(uint32_t shc_hdl, uint16_t event);
typedef void  (btsdk_author_abort_ind_func)(uint32_t svc_hdl, uint32_t dev_hdl);
typedef void  (btsdk_vendor_event_ind_func)(uint8_t ev_code, uint8_t ev_param_size, uint8_t *ev_param);
/* if: BT2.1 Supported */
typedef void  (btsdk_io_cap_req_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_usr_cfm_req_ind_func)(uint32_t dev_hdl, uint32_t num_value);
typedef void  (btsdk_passkey_req_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_rem_oob_data_req_ind_func)(uint32_t dev_hdl);
typedef void  (btsdk_passkey_notif_ind_func)(uint32_t dev_hdl, uint32_t num_value);
typedef void  (btsdk_simple_pair_complete_ind_func)(uint32_t dev_hdl, uint8_t result);
/* endif BT2.1 Supported */
typedef bool (btsdk_obex_authen_ind_func)(uint32_t conn_hdl, p_bt_sdk_obex_auth_info_stru auth_info);

typedef void (btsdk_device_found_func)(uint32_t dev_hdl, bt_sdk_dev_found_info *p_dev_found_info);

typedef void (btsdk_read_white_list_size_ind_func)(uint8_t white_list_size);
typedef void (btsdk_add_dev_to_white_list_ind_func)(uint32_t result);
typedef void (btsdk_remove_dev_from_white_list_ind_func)(uint32_t result);
typedef void (btsdk_clear_white_list_ind_func)(uint32_t result);
typedef void (btsdk_read_adv_chan_tx_pwr_ind_func)(uint8_t transmit_power_level);
typedef void (btsdk_read_tx_power_level_ind_func)(uint32_t conn_hdl, uint8_t transmit_power_level);

int32_t btsdk_get_local_bd_addr(uint8_t *addr, uint8_t length);
uint32_t btsdk_set_local_bd_addr(uint8_t* addr, uint8_t addr_len);
uint32_t btsdk_get_local_name(uint8_t *name, uint16_t *length);
uint32_t btsdk_set_local_name(uint8_t* name, uint16_t len);
uint32_t btsdk_read_rssi_br(uintptr_t dev_hdl, int8_t *rssi);
int32_t btsdk_init(void);
int32_t btsdk_start_default_bt(void);
int32_t btsdk_set_discovery_mode(uint16_t mode);
int32_t btsdk_get_discovery_mode(uint16_t *mode);
int32_t btsdk_stop_device_discovery(void);
uintptr_t btsdk_get_remote_device_handle(uint8_t *bd_addr, uint8_t addr_len);
bool btsdk_is_device_connected(uintptr_t dev_hdl);
int32_t btsdk_disconnect_acl_by_handle(uintptr_t dev_hdl);
int32_t btsdk_un_pair_device(uintptr_t dev_hdl);
uintptr_t btsdk_add_remote_device(uint8_t *bd_addr, uint8_t addr_len);
int32_t btsdk_update_remote_device_name(uintptr_t dev_hdl, uint8_t *name, uint16_t *plen);
int32_t btsdk_update_remote_device_name_async(uintptr_t dev_hdl, gap_execute_command_cfm_cbk *cfm_cbk);
int32_t btsdk_start_device_discovery(uint32_t dev_class, uint8_t max_num,  uint8_t max_seconds);
int32_t btsdk_get_remote_device_address(uintptr_t dev_hdl, uint8_t *bd_addr, uint8_t addr_len);
int32_t btsdk_create_acl_link(uintptr_t dev_hdl, uint8_t link_type);
int32_t btsdk_create_acl_link_async(uintptr_t dev_hdl, uint8_t link_type, gap_create_connection_cfm_cbk *cfm_cbk);
int32_t btsdk_pair_device(uintptr_t dev_hdl);
uint32_t btsdk_browse_remote_services(uintptr_t dev_hdl, uintptr_t *svc_hdl, uint32_t *svc_count);
int32_t btsdk_disconnect(uintptr_t handle);
int32_t btsdk_register_callback(bt_sdk_callback_stru *cbk);
int32_t btsdk_connect_ex(uintptr_t dev_hdl, uint16_t service_class, uintptr_t *conn_hdl);
int32_t btsdk_reconnect_to_phone(uintptr_t dev_hdl);
int32_t btsdk_le_set_adv_data(const adv_data_t *data, uint8_t adv_cnt,
                              const scan_rsp_data_t *scan_rsp, uint8_t scan_rsp_cnt);
int32_t btsdk_le_set_adv_enable(ble_adv_para_t *para);
int32_t btsdk_le_set_scan_enable(ble_scan_para_t *para);
int32_t btsdk_le_set_scan_disable(void);
uint32_t btsdk_done(void);
uint32_t btsdk_set_tx_power(uint32_t dev_hdl, uint8_t pwr_level);
uint32_t btsdk_set_active_scene(
    const uint8_t *bd_addr, uint8_t addr_len, uint8_t scene, uint8_t bit_rate, uint8_t is_low_delay);
uint32_t btsdk_set_inq_scan_param(uint16_t interval, uint16_t window);
uintptr_t btsdk_restore_remote_device(uint8_t *bd_addr, uint8_t* link_key, uint32_t dev_class);
int32_t btsdk_get_link_mode(uintptr_t dev_hdl, uint8_t* link_mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_GAP_IF_H */
