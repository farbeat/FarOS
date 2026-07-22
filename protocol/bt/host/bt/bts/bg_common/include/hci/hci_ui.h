/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* ---------------------------------------------------------------------------
Module Name:
    hci_ui.h
Abstract:
    This module defines HCI interface structure, macro & functions.
--------------------------------------------------------------------------- */

#ifndef G_HCI_UI_H
#define G_HCI_UI_H

#include "hci_stru.h"
#include "sysdep.h"

#define HCI_HANDLE_BIT_NUM 16
#define OCTET_BIT_LEN 8

#ifdef HUAWEI_ADAPT_VENDOR
#define HCI_TLPKT_COMMAND        0x01
#define HCI_TLPKT_ACLDATA        0x02
#define HCI_TLPKT_SCODATA        0x03
#define HCI_TLPKT_EVENT          0x04

#endif /* HUAWEI_ADAPT_VENDOR */

enum { /* data type use in tl adapter */
    HCI_DATATYPE_COMMAND = 1,
    HCI_DATATYPE_ACL = 2,
    HCI_DATATYPE_SCO = 3,
    HCI_DATATYPE_EVENT = 4,
    /* added for iso event */
    HCI_DATATYPE_ISO = 5,
    HCI_DATATYPE_GLE_CMD = 0xA1,
    HCI_DATATYPE_GLE_EVENT = 0xA2,
    HCI_DATATYPE_GLE_ACB = 0xA3,
    HCI_DATATYPE_GLE_ICB = 0xA4,
};

enum { /* data type use for distinguish BLE or GLE */
    HCI_DATATYPE_BLE = 0,
    HCI_DATATYPE_GLE = 1,
};
/* --------------------------------------------------------------------------- */
/*                         HCI Transport Layer Interface                              */
/* --------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
In the function Func_Startup & Func_StartSco :
    if return values is 0, means successfully;
    if return values is not 0, means not successfully;
In the function Func_Send :
    return values is the number of bytes you send successfully;
In the function Func_GetType :
    return values is the AMP type MACROs;
----------------------------------------------------------------------------- */
typedef int (func_receive)                 (HANDLE host_hdl, uint8_t *p_buf, uint32_t len);
typedef int (func_receive_buff)             (HANDLE host_hdl, struct buff_stru *buff);

typedef struct _tl_callback_stru {
    func_receive *receive;
    func_receive_buff *receive_buff;
} tl_callback_stru;

typedef int (func_startup)                 (HANDLE tl_hdl, tl_callback_stru *p_callback, HANDLE host_hdl);
typedef void (func_shutdown)             (HANDLE tl_hdl);
typedef int (func_send)                 (HANDLE tl_hdl, uint8_t *p_buf, uint32_t len);
typedef int (func_start_sco)             (HANDLE tl_hdl, uint16_t sco_hdl);
typedef void (func_stop_sco)             (HANDLE tl_hdl, uint16_t sco_hdl);
typedef unsigned char (func_get_type)     (HANDLE tl_hdl);

typedef struct _transport_layer_stru {
    func_get_type *get_type;
    func_startup *startup;
    func_shutdown *shutdown;
    func_send *send;
    func_start_sco *start_sco;
    func_stop_sco *stop_sco;
} transport_layer_stru;

/* Range: 0x000-0xEFF (0xF00 - 0xFFF Reserved for future use) */
#define HCI_INVAILD_HANDLE                        0xFFFF
#define HCI_AMPID_BREDR                          0 /* type/id in struct hci_local_stru */
#define hci_opcode(g, c)                        (((g) << 10) | (c))
#define hci_ogf(o)                                ((uint8_t)((o) >> 10)) /* 6bits */
#define hci_ocf(o)                                ((o) & ((1 << 10) - 1)) /* 10bits */
#define HCI_AUTH_FAIL_RETRY_TIMES 3

enum { /* OGF for Command Code, 6 bits */
    HCI_OGF_LINKCONTROL =                         0x01,
    HCI_OGF_LINKPOLICY =                         0x02,
    HCI_OGF_CONTROLLER_BASEBAND =                0x03,
    HCI_OGF_INFORMATIONAL_PARAM =                 0x04,
    HCI_OGF_STATUS_PARAM =                         0x05,
    HCI_OGF_TESTING =                             0x06,
    HCI_OGF_LE =                                 0x08,
    HCI_OGF_VENDOR =                             0x3F
};

/* --------------------------------------------------------------------------- */
/*                             HCI UI Interface structure                              */
/* --------------------------------------------------------------------------- */
enum { /* cap in hci_available_addr_cap */
    HCI_ADDRESS_CAP_LE =                         0x01, /* and LELink shall exist when this bit set */
    HCI_ADDRESS_CAP_BREDR =                     0x02,
    HCI_ADDRESS_CAP_IDENTITY =                     0x04, /* as identity address exist(BREDR or SMPD or LELink) */
    HCI_ADDRESS_CAP_MAIN =                         (HCI_ADDRESS_CAP_BREDR | HCI_ADDRESS_CAP_LE), /* AMPID = 0 */
    HCI_ADDRESS_CAP_80211 =                     0x08,
    HCI_ADDRESS_CAP_UWB =                         0x10,
    HCI_ADDRESS_CAP_AMP =                         (HCI_ADDRESS_CAP_80211 | HCI_ADDRESS_CAP_UWB), /* AMPID != 0 */

    /* HCI Internal used bitmask to deliver information */
    /* Internal use when remote is found by identity, need to update it into lelink, not in any object */
    HCI_ADDRESS_CAP_LELINK_UPDATE =             0x80,
    /* Do not generate remote change notify since there is another behind this call */
    HCI_ADDRESS_CAP_DONOT_CHANGE_NOTIFY =         0x40,
    HCI_ADDRESS_CAP_INFO =
                                (HCI_ADDRESS_CAP_LELINK_UPDATE | HCI_ADDRESS_CAP_DONOT_CHANGE_NOTIFY)
};

struct hci_address_stru {
    uint8_t bd[BD_ADDR_LEN];                        /* 6octets, 48bits */
    /* etc, HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS = HCI_PEER_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS */
    uint8_t atype;
    uint8_t cap;                                    /* etc, HCI_ADDRESS_CAP_LE, reserved for compatibility only. */
};

struct hci_controller_addr_node_stru {
    struct hci_address_stru addr;
    uint8_t mask; /* etc, HCI_CANODE_MASK_WHITELIST */
};

#define HCI_ROLE_DEFAULT                        (HCI_ROLE_SLAVE+1)    /* Default role setting */
struct hci_acl_connection_response_ui_stru {        /* UI Input in hci_acl_connection_response */
    struct hci_connection_request_ev_stru *ev;
    uint8_t  link_role;                            /* HCI_ROLE_SLAVE, HCI_ROLE_MASTER or HCI_ROLE_DEFAULT */
    /* etc: HCI_OPS_REJECT_CONNECTION_REQUEST, HCI_OPS_ACCEPT_CONNECTION_REQUEST */
    uint32_t opscode;
};

struct hci_synchronous_connection_stru {            /* Common part for hci_set_synchronous_param */
    uint32_t transmit_bandwidth;
    uint32_t receive_bandwidth;
    uint16_t max_latency;
    uint16_t voice_setting;
    uint8_t retransmission_effort;
    uint16_t packet_type;
};

struct hci_synchronous_connection_response_ui_stru { /* UI Input in hci_synchronous_connection_response */
    struct hci_connection_request_ev_stru *ev;    /* event received */
    struct hci_synchronous_connection_stru par;    /* eSCO Parameters */
    /* etc: HCI_OPS_REJECT_SYNCHRONOUS_CONNECTION_REQUEST, HCI_OPS_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST */
    uint32_t opscode;
};

struct hci_synchronous_enhanced_connection_response_ui_stru {
    struct hci_enhanced_accept_synchronous_connection_request_stru par;
    uint32_t opscode;
};

struct hci_create_scoui_stru {                    /* UI Input in hci_synchronous_connection_response */
    struct hci_address_stru addr;                /* Remote addr */
    struct hci_synchronous_connection_stru par;    /* eSCO Parameters */
};

/* -----------------------------------------------------------------------------
Description:
    Upper protocol callback function for HCI.
    hdl:         (arg length(2octets) << 16) | ampid(1octet).
    context:     upper defined address.
    arg:         parameter for this op_ev.
    op_ev:        (opcode(passive is 0x0000) << 16) | evindex(include HCI_EVO_ or HCI_EVI)
----------------------------------------------------------------------------- */
typedef void (HCI_CbkFunc)(HANDLE hdl, void *context, void *arg, uint32_t op_ev);

struct hci_cbk_line_stru { /* UI cbk func jump martix */
    uint16_t id;                /* HCI_OPC_ or HCI_EVI_(EVO) */
    void *func;                /* HCI_CbkFunc for external, HCI_CbkTableFunc for internal */
};

enum {
    HCI_CBK_TYPE_INTERNAL = 1,
    HCI_CBK_TYPE_EXTERNAL = 2,
};

struct hci_cbk_node_stru {
    const struct hci_cbk_line_stru *table;
    uint32_t size;
    uint8_t type;
};

enum { /* action in struct hci_qo_s_cbk_node_stru *f */
    HCI_QOS_ACTION_GO = 0,
    HCI_QOS_ACTION_STOP = 1
};

struct hci_qo_s_cbk_node_stru { /* for hci_qos_reg */
    void (*f)(uint8_t action); /* etc, HCI_QOS_ACTION_GO */
    uint8_t idx; /* to match Unreg with Reg */
};

/* cbk arg for HCI_EVO_COMMAND_ERROR, also for L2 command finish/cancel */
struct hci_command_error_stru {
    uint16_t status;                                /* Extended by HCI_STATUS_RTX, include COMMAND_STATUS failed */
    uint16_t opcode;                                /* HCI_OPC_ */
    uint8_t req[1];                                /* Varable length, visit as void *, check platform CPU */
};

/* -----------------------------------------------------------------------------
Description:
    Level 2 Command Node Queue finished callback.
----------------------------------------------------------------------------- */
typedef void (hci_l2queue_cbk_func)(HANDLE hdl, void *context, struct hci_command_error_stru *arg, uint16_t len);

#define HCI_COMMAND_MASK_TRANSPARENT        0x01    /* The input command parameter is in the PDU format */

struct hci_command_node_stru { /* Level 1 command node */
    struct fsm_timer *timer; /* RTX/ERTX Timer */
    void *par; /* raw structure for context. */
    void *context; /* application context pointer */
    hci_l2queue_cbk_func *qcbk; /* Level 2 queue, when this command finished for any reason, cbk */
    uint16_t opcode; /* etc: HCI_OPC_INQUIRY */
#ifndef CONFIG_HCI_NEW_STREAM_STRUCT_CONVERSION
    uint16_t struinfo; /* Command & Return structure convert info */
#endif
    /* Waiting for this event for complete, if command op may wait more than 1 event, check table for patch */
    uint8_t evindex;
    uint8_t mask; /* HCI_COMMAND_MASK_TRANSPARENT */
#ifdef CONFIG_HCI_NEW_STREAM_STRUCT_CONVERSION
    uint8_t par_stru_len; /* length of par, which is a struct */
    uint8_t recv_cmd_status_evt; /* FALSE: Hasn't received cmd_status_evt, TRUE: Has received cmd_status_evt */
#endif
    uint8_t ampid;
    void *_rom;
};

struct hci_command_queue_stru {                    /* Level 2 command queue node */
    struct bth_list_stru list;                        /* node is struct hci_command_node_stru */
    hci_l2queue_cbk_func *qcbk;                    /* Use to cbk when queue finished */
    /* cbk arg by caller, this shall be a unique memory address used as a marker of this l2 queue */
    void *context;
    uint8_t ampid;                                /* target controller id */
};

union hci_set_event_filter_condition_in_union {
    struct hci_set_event_filter_condition_in_stru {
        uint8_t class_of_device[3];
        uint8_t class_of_device_mask[3];
    } c;
    uint8_t bd[6];
};

struct hci_set_event_filter_in_stru {                /* UI Interface for hci_set_event_filter */
    uint8_t filter_type;
    uint8_t filter_condition_type;                /* optional */
    union hci_set_event_filter_condition_in_union condition;
    uint8_t auto_accept_flag;                        /* optional */
};

enum { /* flag in struct hci_inquiry_in_stru */
    HCI_INQUIRY_FLAG_ENTER_PERIOIDIC_MODE =     0,
    HCI_INQUIRY_FLAG_EXIT_PERIOIDIC_MODE =         1,
    HCI_INQUIRY_FLAG_INQUIRY =                     2,
    HCI_INQUIRY_FLAG_INQUIRY_CANCEL =             3,

    HCI_INQUIRY_FLAG_CLASS_OF_DEVICE =             0x80, /* Set_Event_Filter */
    HCI_INQUIRY_FLAG_BD =                         0x40, /* Set_Event_Filter */
    HCI_INQUIRY_FLAG_SET_EVENT_FILTER =         (HCI_INQUIRY_FLAG_CLASS_OF_DEVICE | HCI_INQUIRY_FLAG_BD)
};

struct hci_inquiry_in_stru {                        /* input arg for hci_inquiry */
    hci_l2queue_cbk_func *cbk;
    void *context;
    uint16_t max_period_len;
    uint16_t min_period_len;
    uint8_t lap[3];
    uint8_t inquiry_len;
    uint8_t num_responses;
    uint8_t flag;                                    /* HCI_INQUIRY_FLAG_xxx */
    union hci_set_event_filter_condition_in_union condition;
};

struct hci_eir_data_node_stru { /* Node in hci_extended_inquiry_response_decode */
#ifdef CONFIG_SUB_STRUCTURE_ALIGNED_FIXED
    uint8_t padding[2];        /* For ARM Linux GCC, substru is algined by 4 octets */
#endif
    uint8_t len;                /* 1 type + x length */
    uint8_t eir_data_type;    /* etc:HCI_EIR_DATATYPE_FLAGS */
    union {
        /* General data pointer */
        uint8_t p[1];

        /* HCI_EIR_DATATYPE_SHORTENED_LOCAL_NAME,
        HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME */
        struct hci_eir_data_name_stru {
            uint8_t data[1]; /* len - 1 octets */
        } name;

        /* HCI_EIR_DATATYPE_FLAGS */
        uint8_t flags; /* etc, HCI_EIR_FLAGS_BREDR_NOT_SUPPORTED */

        /* HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA */
        struct hci_eir_data_manufacturer_data_stru {
            uint8_t company_identifier_code[2];
            uint8_t data[1]; /* len - 1 - 2 octets */
        } manufacturer_data;
#ifdef ENABLE_CHANGE_DEVICE_NAME
        /* HCI_EIR_DATATYPE_MANUFACTURER_SPECIFIC_DATA */
        struct hci_eir_data_device_id_stru {
            uint8_t vendor_id_src[2]; // two bytes in device id profile
            uint8_t vendor_id[2]; // two bytes in device id profile
            uint8_t product_id[2]; // two bytes in device id profile
            uint8_t version[2]; // two bytes in device id profile
        } device_id_data;
#endif
        /* HCI_EIR_DATATYPE_TX_POWER_LEVEL */
        uint8_t tx_power_level; /* -127 to + 127dBm, pathloss = Tx Power Level - RSSI */

        /* HCI_EIR_DATATYPE_SLAVE_CONNECTION_INTERVAL_RANGE, only for LE AD */
        struct hci_eir_data_slave_connection_interval_range_stru {
            uint8_t conn_interval_min[2];
            uint8_t conn_interval_max[2];
        /* The first 2 octets defines the minimum value for the connection interval, the second 2oct for maximum */
        } slave_connection_interval_range;

        /* HCI_EIR_DATATYPE_INCOMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_COMPLETE_LIST_OF_16BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_LIST_OF_16BIT_SERVICE_SOLICITATION_UUIDS,
        HCI_EIR_DATATYPE_INCOMPLETE_LIST_OF_32BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_COMPLETE_LIST_OF_32BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_LIST_OF_32BIT_SERVICE_SOLICITATION_UUIDS,
        HCI_EIR_DATATYPE_INCOMPLETE_LIST_OF_128BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_COMPLETE_LIST_OF_128BIT_SERVICE_CLASS_UUIDS,
        HCI_EIR_DATATYPE_LIST_OF_128BIT_SERVICE_SOLICITATION_UUIDS */
        uint8_t uuid[1]; /* 2,4,or 16 octets */

        /* HCI_EIR_DATATYPE_SERVICE_DATA_16BIT_UUID */
        struct hci_eir_data_service_data16stru {
            uint8_t uuid16[2];
            uint8_t data[1]; /* len - 1 - 2 octets */
        } service_data16;

        /* HCI_EIR_DATATYPE_SERVICE_DATA_32BIT_UUID */
        struct hci_eir_data_service_data32stru {
            uint8_t uuid32[4];
            uint8_t data[1]; /* len - 1 - 4 octets */
        } service_data32;

        /* HCI_EIR_DATATYPE_SERVICE_DATA_128BIT_UUID */
        struct hci_eir_data_service_data128stru {
            uint8_t uuid128[16];
            uint8_t data[1]; /* len - 1 - 16 octets */
        } service_data128;

        /* HCI_EIR_DATATYPE_APPEARANCE, only for LE AD */
        uint8_t appearance[2];

        /* HCI_EIR_DATATYPE_PUBLIC_TARGET_ADDRESS,
        HCI_EIR_DATATYPE_RANDOM_TARGET_ADDRESS, only for LE AD */
        uint8_t addr[6]; /* Multiples of 6 octets */

        /* HCI_EIR_DATATYPE_LE_BLUETOOTH_DEVICE_ADDRESS, only for LE AD */
        /* The least significant bit of the most significant octet defines if the Device Address is a
        Public Address or a Random Address. LSB = 1 Then Random Device Address.
        LSB = 0 Then Public Device Address. Bits 1 to 7 in the most significant octet are reserved for future use */
        uint8_t leaddr[7]; /* 7 octets */

        /* HCI_EIR_DATATYPE_LE_ROLE */
        uint8_t le_role; /* etc, HCI_LE_ROLE_ONLY_PERIPHERAL_ROLE_SUPPORTED */

        /* HCI_EIR_DATATYPE_ADVERTISING_INTERVAL, only for LE AD */
        uint8_t advertising_interval[2]; /* Units: 0.625 msec */

        /* Following only for Vol3, Part C, 5.2.2.7 OOB data Block format */
        /* HCI_EIR_DATATYPE_CLASS_OF_DEVICE */
        uint8_t class_of_device[3];

        /* HCI_EIR_DATATYPE_SIMPLE_PAIRING_HASH_C192 */
        /* HCI_EIR_DATATYPE_SIMPLE_PAIRING_HASH_C256 */
        uint8_t simple_pairing_hash_c[16]; /* 16 octets */

        /* HCI_EIR_DATATYPE_SIMPLE_PAIRING_RANDOMIZER_R192 */
        /* HCI_EIR_DATATYPE_SIMPLE_PAIRING_RANDOMIZER_R256 */
        uint8_t simple_pairing_randomizer_r[16]; /* 16 octets */

        /* HCI_EIR_DATATYPE_LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE */
        uint8_t lesc_confirmation[16];

        /* HCI_EIR_DATATYPE_LE_SECURE_CONNECTIONS_RANDOM_VALUE */
        uint8_t lesc_random[16];
    } u;
}; /* all member uint8_t, so no pendding at all, visit from &node->len */

/* ACL link auto-sniff, by YDX, 2019.3.20 */
struct hci_auto_sniff_par_stru {
    uint8_t idle_time; /* Idle time in seconds before entering sniff mode */
    uint16_t sniff_max_interval; /* sniff maximum interval */
    uint16_t sniff_min_interval; /* sniff minimum interval */
    uint16_t sniff_attempt; /* sniff attempt */
    uint16_t sniff_timeout; /* sniff timeout */
};

struct hci_auto_sniff_req_stru {
    uint8_t bd[6];
    struct hci_auto_sniff_par_stru *par;
};

/* --------------------------------------------------------------------------- */
/*                             callback for HCI_EVO_Events                              */
/* --------------------------------------------------------------------------- */
enum {
    /* Local born without buffer size, without BD, just used to active waiting, for TL-REG caller */
    HCI_LOCALCONTROLLER_TLREG_FINISH,
    HCI_LOCALCONTROLLER_REMOVE, /* Local controller removed */
    HCI_LOCALCONTROLLER_NOTIFY_BUFFER_SIZE, /* ACL/LE buffer size, or data block size */
    HCI_LOCALCONTROLLER_NOTIFY_AMPINFO, /* AMP status notify, temply saved in le_status */
    HCI_LOCALCONTROLLER_NOTIFY_BREDR_LE_CAP /* BREDR or LE or DuleMode */
};

struct hci_local_controller_changed_stru {    /* par of HCI_EVO_LOCALCONTROLLER_CHANGED */
    HANDLE tl_hdl;
    struct hci_address_stru addr;            /* Local Read_BDADDR for BD and feature to get cap */
    uint16_t max_pdusize;                        /* 1500 */
    uint8_t ampid;                            /* HCI_AMPID_BREDR */
    uint8_t action;                            /* etc: HCI_LOCALCONTROLLER_TLREG_FINISH */
    uint8_t amp_status;                        /* HCI_AMP_STATUS_NO_CAPACITY */
    uint8_t cap;                                /* etc: HCI_ADDRESS_CAP_LE */
};

#define HCI_MPN        2 /* 2*uint,64bits,enough? */

typedef struct {
    uint32_t num[HCI_MPN];
} hci_mp_int;

enum { /* dcounter[x] in struct hci_local_stru */
    HCI_DCOUNTER_SEND,
    HCI_DCOUNTER_RECV
};

struct hci_info_read_local_stru {
    struct hci_address_stru br_addr;            /* BREDR address - Shall be all 0s in LE-only mode */
    /* Identity address - May be public or static if LE supported. Otherwise, shall be all 0s. */
    struct hci_address_stru id_addr;
    /* Random address - May be all 0s, or RPA, or None-RPA if LE supported. Otherwise, shall be all 0s. */
    struct hci_address_stru rd_addr;
    hci_mp_int dcounter[2];                    /* etc: HCI_DCOUNTER_SEND */
    uint8_t hci_version;
    uint8_t ampid;
};

#define HCI_REMOTE_INFO_SRC_INQUIRY                1
#define HCI_REMOTE_INFO_SRC_CONNECT_REQ            2
/* The upper two provide device_of_class */
#define HCI_REMOTE_INFO_SRC_ADV_REPORT            3
#define HCI_REMOTE_INFO_SRC_SCAN_RESPONSE       4
#define HCI_REMOTE_INFO_SRC_REMOTE_FEATURES        5
#define HCI_REMOTE_INFO_SRC_OTHER                0xFF

struct hci_remote_info_stru {
    /* Could be Public from BREDR, from LE, or Random from LE, or converted by SMP into public, just a reference */
    struct hci_address_stru addr;
    uint8_t src;    /* HCI_REMOTE_INFO_SRC_INQUIRY_RESULT etc. */
    /* etc, HCI_CLASS_OF_DEVICE_LIMITED_DISCOVERABLE_MODE, 23~13 for bitmask, 12~8 for enum major, 7~2 for minor,
       1~0 are 0 */
    uint8_t class_of_device[3];
    uint8_t page_scan_repetition_mode;        /* etc: HCI_PAGE_SCAN_REPETITION_MODE_R2 */

    /* BR/EDR Range: -128  <= N <= 127 (signed integer) Units: dB */
    /* AMP: Range: AMP type specific (signed integer)  Units: dBm */
    /* LE: Range: -127 to 20, 127 (signed integer) Units: dBm */
    uint8_t rssi;
    uint8_t eir_len;
    uint8_t eir[1];                            /* Variable length */
};

/* SDU segment flag */
enum {
    QOS_SDU_START =     0x01,                /* 0x01    start of SDU, a bitmask */
    QOS_SDU_END =         0x80,                /* 0x80    end of SDU, a bitmask */
    QOS_SDU_CONTINUE =     0x02                /* 0x02, not start, not end */
};

struct hci_l2cap_data_cfm_stru {                /* QoS DataCfm */
    uint16_t cid;
};

struct hci_info_read_link_security_stru {        /* = struct HCI_ACLLinkSecurityStru part */
    uint16_t mask;                            /* etc: HCI_SECURITY_AUTHENTICATE, HCI_SECURITY_LINKKEY */
};

struct hci_info_read_le_size_number_stru {            /* All LE Controller size or number limitations */
    uint8_t white_list_size;                    /* White list size */
    uint8_t resolving_list_size;                /* Resolving list size */
    uint16_t max_adv_data_length;                /* Maximum advertising data length */
    uint8_t number_of_adv_sets;                /* Number of advertising sets supported */
    uint8_t prd_dev_list_size;                /* Periodic advertiser list size */

    void *_rom;
};
enum { /* result of struct hci_security_author_req_stru */
    HCI_SECURITY_AUTHOR_PASS,
    HCI_SECURITY_AUTHOR_NOTALLOW
};

struct hci_security_author_req_stru {            /* par of HCI_EVO_AUTHORIZATION_REQUEST */
    struct hci_address_stru addr[2];            /* x in HCI_LOCAL */
    uint16_t hcihdl;                            /* Based on a link object */
    uint16_t channel;                         /* PSM for L2CAP Server channel for RFCOMM */
    uint8_t proto_id;                            /* etc: PROT_L2CAP, PROT_RFCOMM */
    uint8_t result;                            /* etc: HCI_SECURITY_AUTHOR_PASS */
};

enum { /* parameter values of hci_le_set_op_report_stru::op_id  */
    HCI_LE_SETOP_SET_ADV_ENABLE,
    HCI_LE_SETOP_CURRENT_ADV_STATE,        /* Report current advertising state */
    HCI_LE_SETOP_ADV_NOT_ALLOWED,        /* Advertising can't be enabled/disabled */
    HCI_LE_SETOP_SET_SCAN_ENABLE,
    HCI_LE_SETOP_CURRENT_SCAN_STATE,    /* Report current scan state */
    HCI_LE_SETOP_SCAN_NOT_ALLOWED,        /* LE scan can't be enabled/disabled */

    HCI_LE_SETOP_RANDOM_ADDR_UPDATE,    /* Local random address is updated */
};

/* Parameter values of HCI_EVO_LE_SET_SCAN_ADV_STEP  */
struct hci_le_set_op_report_stru {
    uint8_t op_id;    /* Operation being executed */
    uint8_t op_value;    /* 0 - Disable 1 - Enable. */
};

/* Parameter values of HCI_EVO_PAIRCFM_ASK4  */
struct hci_security_pair_cfm_ask4stru {
    uint8_t remote_bd[6];             /* remote BD_ADDR */
    uint8_t initiator;                /* 0 - Remote initiates pairing 1 - Local initiates pairing */
    uint8_t bond;                     /* HCI_BOND_NOBOND/HCI_BOND_DEDICATED/HCI_BOND_GENERAL */
};

/* Parameter values of HCI_EVO_ENCRYPTION_CHANGED  */
struct hci_security_encryption_changed_stru {
    uint8_t remote_bd[6];             /* remote BD_ADDR */
    uint8_t encryption_result;        /* encryption result */
};

enum { /* HCI Command Error Internal reason */
    HCI_STATUS_NO_FLOWCONTROL = 0x0100,        /* for HCI_OPC_HOST_NUMBER_OF_COMPLETED_PACKETS */
    HCI_STATUS_NOT_SUPPORTED,
    HCI_STATUS_RTX,
    HCI_STATUS_CANCELED,
    HCI_STATUS_LOCAL_CONTROLLER_REMOVED,
    HCI_STATUS_MITM_NOT_SATISIFIED, /* Were requested for MITM, but for IO or other reason, result is non_MITM */
    HCI_STATUS_LINK_DISCONNECTED, /* Some link-related cmd should be removed when link disconnected */
};

enum { /* For hci_cbk Additional event section in struct HCI_EvCBKStru, added enum for HCI_EVI_xxx */
    HCI_EVO_ACLLINK = HCI_EVI_ENDLINE,        /* Create/Delete/1st Ready, struct hci_link_cbk_stru */
    HCI_EVO_LELINK,                            /* LE, struct hci_link_cbk_stru */
    /* eSCO, HCI_EVI_CONNECTION_REQUEST, waiting for Connection_Response */
    HCI_EVO_ESCOLINK,
    HCI_EVO_LOCALCONTROLLER_CHANGED,        /* struct hci_local_controller_changed_stru */
    HCI_EVO_REMOTECONTROLLER_CHANGED,        /* struct hci_remote_info_stru */
    HCI_EVO_ACL_DATA_IND,                    /* struct buff_stru */
    HCI_EVO_PHY_DATA_IND,                    /* struct buff_stru */
    HCI_EVO_LE_DATA_IND,                    /* struct buff_stru */
    HCI_EVO_SCO_DATA_IND,                    /* struct buff_stru */

    /* HCI Internal filter event from EVI_ */
    HCI_EVO_COMMAND_ERROR,                    /* Command Terminate reason for OpCbk, struct hci_command_error_stru */
    /* Shall Register OPC(Cover Error) before EVI(Cover Passive) with one function */

    HCI_EVO_AUTHORIZATION_REQUEST,            /* struct hci_security_author_req_stru */
    HCI_EVO_AUTHORIZATION_ABORT,            /* struct hci_security_author_req_stru */
    /* struct hci_user_confirmation_request_ev_stru, Popup a window and display something */
    HCI_EVO_SECURITY_DISPLAY,
    HCI_EVO_LINK_KEY_SAVE,                    /* struct hci_security_link_key_save_stru, app to save bonded linkkey */
    HCI_EVO_LINK_KEY_ASK4,                    /* struct hci_security_link_key_ask4stru, ask for linkkey */
    HCI_EVO_PIN_CODE,                        /* struct hci_pin_code_request_ev_stru, ask for PINCode */
    HCI_EVO_COMMAND_COMPLETE,                /* return parameter in command_complete */
    HCI_EVO_LOOPBACK,                        /* struct hci_loopback_command_ev_stru */
    HCI_EVO_L2CAP_DATACFM,                    /* struct hci_l2cap_data_cfm_stru */
    HCI_EVO_REMOTE_OOB_DATA_REQUEST,        /* struct hci_remote_oob_data_request_stru */
    HCI_EVO_NO_FLOW_CONTROL,                /* NULL */
    HCI_EVO_LE_SET_OP_REPORT,               /* struct hci_le_set_op_report_stru */
    HCI_EVO_PAIRCFM_ASK4,                   /* struct hci_security_pair_cfm_ask4stru */
    HCI_EVO_ENCRYPTION_CHANGED,             /* struct  hci_security_encryption_changed_stru */

    HCI_EVO_GLELINK,
    HCI_EVO_GLE_DATA_IND,
};

enum { /* Bitmask in hci_acl_link_ref */
    /* input r is the acllink object, no need to find again */
    HCI_ACLLINK_ACLOBJECT =                             0x8000,
    HCI_ACLLINK_LE =                                     0x4000,

    /* UI interface */
    HCI_ACLLINK_REF_BREDR =                             (0x0001),
    HCI_ACLLINK_REF_LE =                                 (0x0002 | HCI_ACLLINK_LE)
};

enum { /* Job in struct hci_link_cbk_stru, for HCI_EVO_LELINK & HCI_EVO_ACLLINK */
    HCI_LINKCBK_CREATE,                    /* to create a shadow obj */
    HCI_LINKCBK_DELETE,                    /* Disconnected, kill shadow obj */
    HCI_LINKCBK_CONNECTED,                /* Connected, Notify hcihdl, address, role, etc */
    HCI_LINKCBK_READY                    /* Ready to use the link for upper */
};

enum { /* mask in HCI_Set_Channel_Classification */
    HCI_SETAFH_OCCUPY =     0x01, /* otherwise release */
    HCI_SETAFH_SENDCMD =     0x02 /* or later */
};

enum { /* mask in struct hci_link_stru / struct hci_link_cbk_stru */
    /* Local initiated connection, local manage disconnect of the link */
    HCI_ACLMASK_INITIATOR =                 0x01,
    /* Before connection complete is judged, two bits may exist simultaneously */
    HCI_ACLMASK_ACCEPTOR =                     0x08,
    /* Connection complete OK/ACL already exist received, only disconnect can kill link */
    HCI_ACLMASK_CONNECTION_COMPLETED =         0x10,

    /* Master or Slave(Unknown yet), only for LE since Role_Change is not watched */
    HCI_ACLMASK_MASTER =                     0x02,
    HCI_ACLMASK_RECVDATA_TAIL_UNCOMPLETE =     0x04 /* Recedata list tail node is a uncomplete L2CAP segments head
    or is complete but waiting for link status change to ready(GetRemoteInfo) */
};

enum { /* addr[x] in struct hci_link_cbk_stru */
    HCI_LOCAL =         0,
    HCI_REMOTE =         1
};

enum {
    BTH_LINK_BR = 0,
    BTH_LINK_BLE = 1,
    BTH_LINK_GLE = 2,
};
struct hci_link_cbk_stru {                /* Cbk arg of hci_acl_link_cbk */
    void *hci;                            /* Host link handle */
    uint16_t hdl;                            /* Controller link handle */
    /* HCI_LOCAL(Current used) & HCI_REMOTE(Public/Static Prefered, or CurLink) */
    struct hci_address_stru addr[2];
    uint8_t mask;                            /* etc: HCI_ACLMASK_INITIATOR, This is only valid when ready */
    uint8_t job;                            /* etc: HCI_LINKCBK_CREATE */
    uint8_t reason;                        /* etc: HCI_STATUS_CONNECTION_FAILED_TO_BE_ESTABLISHED */

    void *_rom;
};

struct hci_physical_link_stru {            /* in of HCI_PhysicalLink */
    void *acllink;                        /* As handle */
    uint8_t *assoc;                        /* Remote assoc, sub mem */
    uint32_t opscode;                        /* HCI_OPS_ACCEPT_PHYSICAL_LINK or HCI_OPS_CREATE_PHYSICAL_LINK */
    uint16_t assoc_len;
    uint8_t phyhdl;
    uint8_t ampid;

    void *_rom;
};

enum { /* fea in struct smp_key_stru */
    SMP_KEYFEA_ECDH =             0x01, /* By ECDH */
    SMP_KEYFEA_MITM =             0x02, /* Authenticated */
    SMP_KEYFEA_LTK_SEND =         0x04, /* Ability to been encrypt as a slave */
    SMP_KEYFEA_LTK_RECV =         0x08, /* Ability to encrypt as a master */
    SMP_KEYFEA_CSRK_ENCODE =     0x10, /* Ability to encode the signing data */
    SMP_KEYFEA_CSRK_DECODE =     0x20, /* Ability to decode/verify the signing data */
    SMP_KEYFEA_IRK_SEND =         0x40, /* Ability to use RPA, remote could recognize */
    SMP_KEYFEA_IRK_RECV =         0x80 /* Ability to decode remote`s RPA */
};

struct smp_key_stru { /* Master role security database */
    /* as Security Level, both side share this length, by pairing_request and response min */
    uint8_t ltk_len;
    uint8_t fea;                        /* etc, SMP_KEYFEA_ECDH */

    /* Local sent to remote information */
    uint8_t div[2];                    /* 16bits, 2octets, Peripherial(slave) distributed DIV for remote(Master) */

    /* Remote sent to local, or LESC derived on both sides */
    uint8_t ltk[16];                    /* corresponding to Public/Static address or IRK(RPA) */
    uint8_t ediv[2];                    /* eDIV and Rand are only used in legacy LTK encryption(no LESC) */
    uint8_t rand[8];                    /* Encryption keys */
    uint8_t irk[16];                    /* Resolveable address */
    /* Signature, CSRK_DECODE. CSRK_ENCODE is generated by d1(ER, DIV, 1) locally */
    uint8_t csrk[16];
};

struct smp_key_store_stru {            /* Keys that indexes by addr */
    struct hci_address_stru addr;    /* peer address, is dule-mode? */
    struct smp_key_stru key;            /* Bonding information */
};
#define HCI_SEC_LINKKEY_MAX_LEN 16
struct hci_security_remote_stru {
    /* from hci_ev_io_capability_response */
    uint8_t io_capability;                       /* [033],Enum */
    uint8_t authentication_requirements;      /* [035],Enum, Remote */

    uint8_t pin_code_len;                        /* Default PINCode Length, if 0, PINCode not valid */
    uint8_t pin_code[HCI_PIN_CODE_MAX_LEN];      /* Default PINCode */

    /* Remote OOB info, local have peer`s */
    /* Local recieves Remote`s OOB, [034],Enum etc, HCI_OOB_DATA_PRESENT_OOB_AUTHENTICATION_DATA_NOT_PRESENT */
    uint8_t oob_data_present;

    /* LE Keys by SMP */
    struct smp_key_stru *lekey;
    /* Local send OOB to Remote, OOB need to generate PK[LOCAL] and R[Local] before pairing,
       sk[32] + pk[64] + tk(r)[16] */
    uint8_t *oob_pkr;

    /* align */
    struct bth_list_stru reqlist;                /* struct hci_security_node_stru */
    uint16_t mask;                            /* etc: HCI_SECURITY_AUTHENTICATE, HCI_SECURITY_LINKKEY */
    uint8_t link_key[HCI_SEC_LINKKEY_MAX_LEN];
    uint8_t key_type;                            /* etc: HCI_KEY_TYPE_COMBINATION_KEY */
    /* Pairing Failes 3 consecutive times, ACLLink disconnect with error code 0x05 - Authentication Failure */
    uint8_t fail_counter;
};

struct hci_remote_oob_data_request_stru {
    uint8_t bd[BD_ADDR_LEN];
    /* 0 = HCI_OPS_REMOTE_OOB_DATA_REQUEST_REPLY / 1 = HCI_OPS_REMOTE_OOB_EXTENDED_DATA_REQUEST_REPLY */
    uint8_t extended;
};

enum { /* mask in struct hci_remote_stru */
    /* hci_ev_read_remote_extended_features or hci_ev_remote_host_supported_features_notification */
    HCI_REMOTEMASK_CONTROLLER_FEATURES =         0x01, /* Page 0 */
    HCI_REMOTEMASK_HOST_FEATURES =                 0x02, /* Page 1, When got remote host features, don`t ask again */
    /* Got remote IO Capability Response, use to detect Request before or after Response */
    HCI_REMOTEMASK_IO_CAPABILITY_RESPONSE =     0x04,
    HCI_REMOTEMASK_IO_CAPABILITY_REQUEST =         0x08,
    HCI_REMOTEMASK_IO_CAPABILITY_ROUND =
                (HCI_REMOTEMASK_IO_CAPABILITY_REQUEST | HCI_REMOTEMASK_IO_CAPABILITY_RESPONSE),
    HCI_REMOTEMASK_BREDR_PAIRING_ONPROGRESS =     0x10,
    /* When start inquiry or scan, set this cap to die if not seen */
    HCI_REMOTEMASK_TO_DIE =                     0x20,
    HCI_REMOTEMASK_ASK4_LINK_KEY =                 0x40, /* When asking for Link Key from the APP */
    HCI_REMOTEMASK_ASK4_PAIRING_CONFIRMATION =     0x80
};

enum {
    HCI_REMOTE_MIN = 0,
    HCI_REMOTE_MAX = 1
};

#define HCI_LMP_FEATURE_MAX_PAGE  3                /* 0, 1, 2, for local & remote */

struct hci_remote_stru {                        /* Only Remote Primary BREDR/LE controller, AMP is not a remote obj */
    struct hci_security_remote_stru sec;
    /* etc: HCI_ADDR_IDENTITY, HCI use current address, whatever type it is, the address for controller */
    struct hci_address_stru addr[2];
    /* etc: HCI_REMOTE_MIN, Min, Max, [210],Time, From AD Perpherial Perfered Parameter */
    uint16_t conn_interval[2];
    uint16_t clock_offset;                    /* etc: HCI_CLOCK_OFFSET_VALID | x */
    uint8_t cap;                                /* etc: HCI_ADDRESS_CAP_LE */
    uint8_t page_scan_repetition_mode;        /* etc: HCI_PAGE_SCAN_REPETITION_MODE_R0 */
    uint16_t mask;                                /* etc: HCI_REMOTEMASK_HOST_FEATURES */
    /* extended features 8 pages? current allocate 3 page? need bitmask if we have already asked */
    uint8_t lmp_features[HCI_LMP_FEATURE_MAX_PAGE][8];
    void *_gle;
};

enum { /* mode in struct HCI_SecuritySetModeStru */
    HCI_SECURITY_MODE1,                    /* No Security, Forbidden in 2.1, expection is SDP */
    HCI_SECURITY_MODE2,                    /* Service Level Security, 3 & 1 is part of 2, so treat as 2 */
    HCI_SECURITY_MODE3,                    /* Link Level Security, HCI_OPS_WRITE_AUTHENTICATION_ENABLE */
    /* Mode2, but Forced to Auth/Encrypt, expection is SDP, HCI_OPC_WRITE_SIMPLE_PAIRING_MODE */
    HCI_SECURITY_MODE4
};

enum { /* Additional enum bit mask identifies the types of Security OOB data available */
    HCI_OOB_DATA_NOT_PRESENT =  0x00,
    HCI_OOB_DATA_P192_CR_VALUE_PRESENT = 0x01,
    HCI_OOB_DATA_P256_CR_VALUE_PRESENT = 0x02,
    HCI_OOB_DATA_P192_AND_P256_CR_VALUE_PRESENT = 0x03,
    HCI_OOB_DATA_SMP_TK_VALUE_PRESENT = 0x10,
    HCI_OOB_DATA_SECURE_SMP_CR_VALUE_PRESENT = 0x20,
    HCI_OOB_DATA_SMP_TK_AND_SECURE_SMP_CR_VALUE_PRESENT = 0x30
};

struct hci_security_remote_oob_stru {            /* Remote Controller Security OOB data */
    struct hci_address_stru addr;            /* Remote Addr */

    /* Direction for: HCI_OPS_IO_CAPABILITY_REQUEST_REPLY and SMP Pairing_Request / Security_Request */
    /* [034],Enum etc, HCI_OOB_DATA_PRESENT_OOB_AUTHENTICATION_DATA_NOT_PRESENT */
    uint8_t oob_data_present;
};

struct hci_security_link_key_recover_stru { /* in of hci_security_link_key_recover */
    uint8_t bd[BD_ADDR_LEN];                /* [123],Val */
    uint8_t link_key[HCI_SEC_LINKKEY_MAX_LEN];                 /* [173],Val */
    uint8_t key_type;                        /* etc: HCI_KEY_TYPE_COMBINATION_KEY */
    uint8_t accept;                        /* TRUE for Reply or recover key, FALSE for Negative Reply */
};

enum { /* mask in struct hci_security_node_stru */
    /* authentication, ask this device is allowed or not? we are initiator of HCI_Authentication_Requested */
    HCI_SECURITY_AUTHENTICATE =             0x0001,
    HCI_SECURITY_AUTHORIZE =                 0x0002,    /* authorization, popup to ask if this PSM is allowed? */
    HCI_SECURITY_ENCRYPT =                     0x0004,    /* encryption is required on node, encrypted on remote */
    HCI_SECURITY_MITM =                     0x0008,    /* authentication with keytype check for MITM, Level 3 */
    HCI_SECURITY_P256 =                     0x0010,    /* AES-CCM, P256 ECDH */
    /* Incoming channel, upper layer concept, this is not security direction! */
    HCI_SECURITY_INCOMING =                 0x0020,

    /* Other internal masks for remote->sec.mask, also encrypt, mitm, p256, authenticate */
    /* Authorization callback ongoing, for abort, only for require node */
    HCI_SECURITY_AUTHORIZE_ONGOING =         0x8000,
    HCI_SECURITY_LINKKEY =                     0x1000,    /* LinkKey validation, go for authencation for it */
    HCI_SECURITY_BAD_LINKKEY =                 0x0800,    /* LinkKey Level not enough, a temp procedure mask */
    HCI_SECURITY_BONDED    =                     0x0400,    /* Bonded, linkkey is bonded or unbonded(temp) */
    HCI_SECURITY_BONDABLE =                 0x0200    /* Bondable */
};

enum { /* Result in Security callback */
    HCI_SECURITY_CBK_COMPLETE =             0,
    HCI_SECURITY_CBK_PENDDING =             (1 << 16)
};

struct hci_security_info_stru {    /* node in HCI_SECURITYLIST */
    uint16_t channel;                /* PSM or server channel */
    uint16_t mask;                /* etc: HCI_SECURITY_AUTHENTICATE */
    uint8_t proto_id;                /* etc: PROT_L2CAP */
};

#ifdef BTH_AUDIO_OVER_DSP
#pragma pack(2)
struct hci_share_mem_for_voice_data_stru {
    uint16_t sync_hdl;         /* connect handle */
    uint32_t stream_out_mem;    /* memory for voice data transmission from dsp to BTC */
    uint32_t stream_in_mem;     /* memory for voice data transmission from BTC to dsp */
    uint16_t enable;          /* enable/disable the memory */
};
#pragma pack()
#endif

enum { /* Bond in struct hci_security_node_stru */
    HCI_BOND_NOBOND =         0,    /* No Bond */
    HCI_BOND_DEDICATED =     1,    /* Without a L2CAP/RFCOMM channel */
    HCI_BOND_GENERAL =         2    /* With a L2CAP/RFCOMM channel */
};

struct hci_security_node_stru {            /* node in link->sec.reqlist */
    struct hci_security_info_stru info;    /* Must in 1st item for searching */
    /* Security Caller cbk, HCI_SMCbkFunc *, but dead-lock called, so void *, for KEIL compile reason */
    void *cbk;
    uint16_t hcihdl;                        /* Based on a link object */
    uint8_t bond;                            /* etc: HCI_BOND_DEDICATED */
};

/* -----------------------------------------------------------------------------
Description:
    Security Request ListNode callback.
----------------------------------------------------------------------------- */
typedef void (HCI_SMCbkFunc)(struct hci_security_node_stru *node, uint32_t result);

#define HCI_SECURITY_LINK_KEY_REQUEST_REPLY_STRU hci_security_link_key_recover_stru    /* For compatibility */

struct hci_security_link_key_ask4stru { /* HCI_EVO_LINK_KEY_ASK4 */
    uint8_t local_bd[BD_ADDR_LEN];        /* [123],Val */
    uint8_t remote_bd[BD_ADDR_LEN];        /* [123],Val */
};

struct hci_security_link_key_save_stru { /* HCI_EVO_LINK_KEY_SAVE */
    uint8_t local_bd[BD_ADDR_LEN];        /* [123],Val */
    uint8_t remote_bd[BD_ADDR_LEN];        /* [123],Val */
    uint8_t link_key[16];                 /* [173],Val */
    uint8_t key_type;                        /* etc: HCI_KEY_TYPE_COMBINATION_KEY */
};

#ifdef CONFIG_JITWS
/* Macro components of HCI_OPS_ */
/* Ret struinfo offset or EVI selector bitmask */
#define hci_ops2mask(ops_)                        (((uint16_t)ops_ >= 0xFC00)? 0:((ops_) & 0x8000))
/* OPCode, remove 0x8000 bitmask */
#define hci_ops2opc(ops_)                        (((uint16_t)ops_ >= \
    0xFC00)?(uint16_t)(ops_):((uint16_t)((ops_) & 0x7FFF)))
#ifdef CONFIG_HCI_NEW_STREAM_STRUCT_CONVERSION
#define hci_ops2strulen(ops_)                   ((uint8_t)((ops_) >> 16)) /* struct lenght of opcode */
#else
#define hci_ops2reqoff(ops_)                    ((uint8_t)((ops_) >> 16)) /* Req struinfo offset */
#endif
#define hci_ops2retevi(ops_)                    ((uint8_t)((ops_) >> 24)) /* Ret struinfo offset or EVI */

#define hci_set_off(p, evindex_)                ((p)[(evindex_) >> 3] &= ~((1 << ((evindex_) & 7))))
#define hci_set_bit(p, evindex_)                ((p)[(evindex_) >> 3] |= (1 << ((evindex_) & 7)))
#define hci_check_bit(p, evindex_)                ((p)[(evindex_) >> 3] & (1 << ((evindex_) & 7)))
#else

/* Macro components of HCI_OPS_ */
#define hci_ops2mask(ops_)                        ((ops_) & 0x8000) /* Ret struinfo offset or EVI selector bitmask */
#define hci_ops2opc(ops_)                        ((uint16_t)((ops_) & 0x7FFF)) /* OPCode, remove 0x8000 bitmask */
#ifdef CONFIG_HCI_NEW_STREAM_STRUCT_CONVERSION
#define hci_ops2strulen(ops_)                   ((uint8_t)((ops_) >> 16)) /* struct lenght of opcode */
#else
#define hci_ops2reqoff(ops_)                    ((uint8_t)((ops_) >> 16)) /* Req struinfo offset */
#endif
#define hci_ops2retevi(ops_)                    ((uint8_t)((ops_) >> 24)) /* Ret struinfo offset or EVI */

#define hci_set_off(p, evindex_)                ((p)[(evindex_) >> 3] &= ~((1 << ((evindex_) & 7))))
#define hci_set_bit(p, evindex_)                ((p)[(evindex_) >> 3] |= (1 << ((evindex_) & 7)))
#define hci_check_bit(p, evindex_)                ((p)[(evindex_) >> 3] & (1 << ((evindex_) & 7)))
#endif
/* -----------------------------------------------------------------------------
Description:
    HCI LE UI structures.
----------------------------------------------------------------------------- */
/* Parameters of Extended Scan */
struct hci_le_ext_scan_parameters_stru {
    /* Extended Scann Parameters */
    /* Policy to filter advertising packets, one of t_hci_scanning_filter_policy, e.g.
       HCI_SCANNING_FILTER_POLICY_ACCEPT_ALL */
    uint8_t scan_filter_policy;
    uint8_t scan_phys;             /* Scanning PHYs, combination of t_hci_scanning_ph_ys, e.g. HCI_SCANNING_PHYS_1M */
    /* type of the scan of 1M PHY. one of t_hci_le_scan_type, e.g. HCI_LE_SCAN_TYPE_PASSIVE_SCANNING */
    uint8_t scan_type_1m;
    /* type of the scan of Coded PHY. one of t_hci_le_scan_type, e.g. HCI_LE_SCAN_TYPE_PASSIVE_SCANNING */
    uint8_t scan_type_coded;
    /* Parameters for 1M PHY */
    /* Time interval from when the Controller started its last scan until it begins the subsequent scan on the
       primary advertising channel, 0x0004 - 0xFFFF. N*0.625ms */
    uint16_t scan_interval_1m;
    /* Duration of the scan on the primary advertising channel, 0x0004 - 0xFFFF. N*0.625ms */
    uint16_t scan_window_1m;
    /* Parameters for 1M Coded PHY */
    /* Time interval from when the Controller started its last scan until it begins the subsequent scan on the
       primary advertising channel, 0x0004 - 0xFFFF. N*0.625ms */
    uint16_t scan_interval_coded;
    /* Duration of the scan on the primary advertising channel, 0x0004 - 0xFFFF. N*0.625ms */
    uint16_t scan_window_coded;
};

/* Parameters of Extended Scan Enable */
struct hci_le_ext_scan_enable_stru {
    /* One of t_hci_filter_duplicates, e.g. HCI_FILTER_DUPLICATES_DUPLICATE_FILTERING_DISABLED */
    uint8_t filter_duplicate;
    uint8_t limited;              /* e.g. LE_DISCOVERYMODE_GENERAL */
    uint16_t duration;             /* Scan duration, 0x0001 - 0xFFFF. N*0.625ms */
    uint16_t period;                /* Scan period, 0x0001 - 0xFFFF. N*1.28s */
};

struct hci_white_addr_list {
    uint8_t count;
    struct hci_controller_addr_node_stru *addr_list;
};

/* --------------------------------------------------------------------------- */
/*                                     UI Functions                             */
/* --------------------------------------------------------------------------- */
void hcih_init(void);
#ifdef CONFIG_BT_STACK_DONE
void hci_done(void);
#endif
void hci_cleanup(void);

void hci_cbk_reg(const struct hci_cbk_line_stru *table, uint32_t size);
void hci_cbk_prime_reg(const struct hci_cbk_line_stru *table, uint32_t size);
void hci_cbk_un_reg(const struct hci_cbk_line_stru *table);
void hci_cbk_un_reg_sync(const struct hci_cbk_line_stru *table);

void hci_tl_reg(HANDLE tl_hdl, transport_layer_stru *func);
void hci_tl_un_reg(HANDLE tl_hdl);

void *hci_acl_link_ref(struct hci_address_stru *r, uint32_t flag);
void hci_acl_link_un_ref(void *hci);
void hci_acl_link_kill(void *hci);

BOOL hci_get_remote_addr_by_acl_hdl(uint16_t conn_hdl, struct hci_address_stru *rmt_addr);
BOOL hci_get_remote_link_key_by_acl_hdl(uint16_t conn_hdl, uint8_t *link_key);

void hci_create_sco_connection(struct hci_create_scoui_stru *in);
void hci_acl_connection_response(struct hci_acl_connection_response_ui_stru *in);
void hci_synchronous_connection_response(struct hci_synchronous_connection_response_ui_stru *in);
void hci_enhanced_synchronous_connection_response(
    struct hci_synchronous_enhanced_connection_response_ui_stru *in);
void hci_set_default_link_role(uint8_t is_master);

void hci_command_send_amp(void *context, void *par, uint32_t opscode, uint8_t ampid);
void hci_command_send_vendor(void *context, void *par, uint16_t opcode, uint8_t evi, uint8_t paramlen, uint8_t ampid);
void hci_commandl2_send_queue(struct hci_command_queue_stru *queue);
/* context when create queue */
#ifdef CONFIG_BREDR_HCI
void hci_commandl2_cancel_queue(void *context, uint8_t ampid);
#endif
void hci_cmd_send_func(void *context, void *par, uint32_t opscode);
#define hci_command_send(context_, par_, opscode_)        hci_command_send_amp((context_), (par_), (opscode_), \
    HCI_AMPID_BREDR)

struct hci_command_queue_stru *hci_commandl2_new_queue(hci_l2queue_cbk_func *cbk, void *context, uint8_t ampid);
struct hci_command_node_stru *hci_commandl2_new_node(struct bth_list_stru *list, uint32_t opscode);
struct hci_set_event_filter_stru *hci_set_event_filter(struct hci_set_event_filter_in_stru *in);
void hci_controller_init(hci_l2queue_cbk_func *cbk, void *context, uint8_t ampid);
void hci_controller_reconfig(hci_l2queue_cbk_func *cbk, void *context, uint8_t ampid);
void *hci_inquiry(struct hci_inquiry_in_stru *in);
void hci_disconnect(uint16_t connection_handle, hci_active_dis_reason reason);
void hci_acl_data_send(struct buff_stru *data, uint32_t cid_hcihdl, uint8_t ampid);
void hci_sco_data_send(struct buff_stru *data, uint16_t scohdl);
void hci_set_auto_sniff(uint8_t *bd, struct hci_auto_sniff_par_stru *par);

uint8_t hci_compare_addr(struct hci_address_stru *s, struct hci_address_stru *r);
uint8_t hci_compare_bd(struct hci_address_stru *addr, const uint8_t *bd);
uint8_t hci_compare_addr_non_0(struct hci_address_stru *s, struct hci_address_stru *r);
uint8_t hci_compare_all_num(uint8_t *p, uint8_t num, uint32_t len);
#ifdef NOT_USED_YET
uint8_t hci_available_addr_cap(struct hci_address_stru *r);
#endif
uint8_t hci_available_addr_cap2(const struct hci_remote_stru *remote);
uint8_t hci_secure_connection_supported(struct hci_remote_stru *remote);
void hci_remote_new_cap_addr(struct hci_remote_stru *remote, struct hci_address_stru *r, uint8_t cap);
struct hci_remote_stru *hci_remote_find_create(uint8_t *bd, uint8_t atype, uint8_t cap);
void hci_remote_change_notify(uint8_t src, struct hci_remote_stru *remote, uint8_t *class_of_device, uint8_t rssi,
                              uint8_t *eir, uint8_t eir_len);

uint8_t hci_extended_inquiry_response_encode(struct bth_list_stru *in, uint8_t *out, uint8_t *end);
uint8_t hci_extended_inquiry_response_decode(struct bth_list_stru *list, uint8_t *eir, uint8_t *end);

struct hci_info_read_local_stru *hci_info_read_local(uint8_t ampid);
void hci_info_read_link_security(struct hci_info_read_link_security_stru *out, void *acl);
void hci_info_read_le_size_number(struct hci_info_read_le_size_number_stru *out, uint8_t ampid);

void hci_security_set_bondable(uint8_t bondable);
void hci_security_set_oob(struct hci_security_remote_oob_stru *in);
void hci_security_read_local_oob_data(void);
void hci_security_set_local_io(uint8_t io_capability);
void hci_security_request(struct hci_security_node_stru *in);
void hci_security_abort(struct hci_security_node_stru *in);
void hci_security_authorized(struct hci_security_author_req_stru *in);
void hci_security_require_reg(struct hci_security_info_stru *in);
void hci_security_require_un_reg(struct hci_security_info_stru *in);
void hci_security_pin_code_reply(struct hci_pin_code_request_reply_stru *in);
void hci_security_pairing_confirmation(uint8_t *bd, uint8_t response);

void hci_security_link_key_recover(struct hci_security_link_key_recover_stru *in);
#define HCI_SECURITY_LINKKEY_REPLY hci_security_link_key_recover    /* For compatibility */
void hci_security_link_key_remove(uint8_t *bd);
void hci_security_le_keys_recover(struct bth_list_stru *list);
void hci_security_le_keys_remove(struct hci_address_stru *addr);

uint8_t *hci_security_get_sc_link_key(void *hci);
/* 6 octets */
uint32_t hci_security_passkey_entry_input(uint8_t *in);
uint8_t hci_security_get_local_io(void);
uint8_t hci_security_get_bondable(void);
#ifdef CONFIG_SMP_LESC_LINKKEY
uint8_t hci_security_get_bredr_pairing_status(const struct hci_remote_stru *remote);
#endif

uint32_t hci_command_get_len(const void *stru, uint32_t opscode);
/* Central Frequency, etc,2412 */
/* Bandwidth, 22,40 */
void hci_set_channel_classification(uint32_t c, uint32_t w,
    /* etc, HCI_SETAFH_OCCUPY */
    uint8_t mask);
#ifdef NOT_USED_YET
void hci_qos_reg(void (*f)(uint8_t action), uint8_t idx);
void hci_qos_un_reg(void (*f)(uint8_t action), uint8_t idx);
const uint8_t *bt_get_le_mac(void);
#endif
void hci_iso_data_send(struct buff_stru *data);

enum { /* Returning of l2cap_header_check */
    L2CAP_HEADERCHECK_OK =             0,
    L2CAP_HEADERCHECK_ERROR =         1,
    L2CAP_HEADERCHECK_KEEPWAIT =     2
};

uint16_t hci_get_mobile_connection_handle(uint8_t *bd);
#if (BTH_WITH_SMART_WEAR == YES)
uint8_t hci_page0feature_find_by_remote_addr(uint8_t *addr);
#endif
void hmac_sha256(uint8_t *key, uint32_t key_len, uint8_t *data, uint32_t data_len);

void hci_le_set_event_mask(uint8_t isset, uint8_t *mask);

#endif /* G_HCI_UI_H */

