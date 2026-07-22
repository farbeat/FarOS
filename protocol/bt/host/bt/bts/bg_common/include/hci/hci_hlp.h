
/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

/* ---------------------------------------------------------------------------
Module Name:
    hci_hlp.h
Abstract:
    This module defines HCI internal structure, macro & functions.
--------------------------------------------------------------------------- */

#ifndef G_HCI_HLP_H
#define G_HCI_HLP_H

#include "hci_ui.h"

#pragma pack(1)

#define ATT_UUID128_LEN 16
#define ATT_UUID32_LEN 4
#define ATT_UUID16_LEN 2

#define HCI_BTC_INIT_EVENT_MASK_SIZE 0X8 /* WRITE EVENT_MASK_SIZE OF BTC */

#define HCI_OPCODE_BITS_NUM 16

#define DEC_STEP 10

typedef struct {
    uint8_t length;
    uint8_t sub_ev_code; /* a1 */
    /* HCI_OPS_SET_EVENT_MASK */
    uint8_t event_mask[HCI_BTC_INIT_EVENT_MASK_SIZE];
    /* HCI_OPS_SET_EVENT_MASK_PAGE_2 */
    uint8_t event_mask_page2[HCI_BTC_INIT_EVENT_MASK_SIZE];
    /* HCI_OPS_LE_SET_EVENT_MASK */
    uint8_t le_event_mask[HCI_BTC_INIT_EVENT_MASK_SIZE];
    /* HCI_OPS_WRITE_PAGE_SCAN_ACTIVITY */
    uint16_t page_scan_interval;             /* Interval Time */
    uint16_t page_scan_window;               /* Scan Window Time */
    /* HCI_OPS_WRITE_LE_HOST_SUPPORT */
    uint8_t le_supported_host;
    uint8_t simultaneous_le_host;
    /* HCI_OPS_LE_WRITE_SUGGESTED_DEFAULT_DATA_LENGTH */
    uint16_t suggested_maxtx_octets;
    uint16_t suggested_maxtx_time;
    /* HCI_OPS_LE_SET_DEFAULT_PHY */
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
    /* HCI_OPS_WRITE_PAGE_SCAN_TYPE */
    uint8_t page_scan_type;
    /* HCI_OPS_WRITE_AFH_CHANNEL_ASSESSMENT_MODE */
    uint8_t afh_channel_assessment_mode;
    /* HCI_OPS_WRITE_INQUIRY_MODE */
    uint8_t inquiry_mode;
    /* HCI_OPS_SET_CONTROLLER_TO_HOST_FLOW_CONTROL */
    uint8_t flow_control_enable;
    /* HCI_OPS_WRITE_SIMPLE_PAIRING_MODE */
    uint8_t simple_pairing_mode;
    /* HCI_OPS_WRITE_CONNECTION_ACCEPT_TIMEOUT */
    uint16_t conn_accept_timeout;
    /* HCI_OPS_WRITE_DEFAULT_LINK_POLICY_SETTINGS */
    uint16_t link_policy_settings;

    void *_rom;
} fast_reset_write_btc_init_cmd;
#pragma pack()

typedef struct {
    uint16_t subrate;
    uint16_t latency;
} hci_subrate;

fast_reset_write_btc_init_cmd fast_reset_init_write_cmd(void);

#define HCI_OPC_RESET_FAST_SET_BTC_INIT 0xFC0B

#define HCI_OPS_RESET_FAST_SET_BTC_INIT 0x0d30FC0B /* ((0x0F << 24) 'R'  | (0x30 << 16)  | 0xFC0B | 0x0000) */

#define HCI_INTERNAL_FUNCTION    1 /* Only used by hci_le.h to exclude internal functions for other modules */

extern struct fsm_inst *hci_fsminst;
extern uint8_t g_is_bth_with_smart_wear;
/* --------------------------------------------------------------------------- */
/*                                 HCI Interface Macros                               */
/* --------------------------------------------------------------------------- */
enum {
    /* Ret in hci_command_check_block */
    HCI_CMD_ALLOW,
    HCI_CMD_BLOCK_BYNUM, /* Number of hci command packet limited */
    HCI_CMD_BLOCK_BYOPCODE, /* Same operation is outstanding */
    HCI_CMD_BLOCK_BYRESET, /* Reset is outstanding, pendding other commands */

    /* Debug Only */
    HCI_CMD_ADD,
    HCI_CMD_REMOVE
};

struct hci_bit_pointer_stru { /* For hci_coding_get_bits */
    uint8_t *p; /* Current Octet pointer */
    uint8_t off; /* Keep current octet remaining bits usage offset */
};

struct hci_x_param_stru {
    uint8_t len;
    uint8_t data[1];
};

struct hci_rtx_timer_stru {
    uint8_t ampid; /* to find local controller node */
    uint16_t opcode; /* to find command node in local, only in sent section */
};

struct hci_local_timer_stru {
    void *local; /* to find local controller node */
    uint8_t id; /* etc, HCI_LOCALRTX_ID_ADVSCAN_TIME_LIMITED */
};

struct hci_acl_link_timer_stru {
    void *acllink;
};

/* --------------------------------------------------------------------------- */
/*                             Local/Remote Object structure                          */
/* --------------------------------------------------------------------------- */
enum { /* timeout[x] in struct hci_local_stru */
    HCI_LINFO_TIMEOUT_PAGE, /* *0.625, include extended page timeout */
    HCI_LINFO_TIMEOUT_CONN_ACCEPT,
    HCI_LINFO_TIMEOUT_EXTENDED_INQUIRY_LENGTH, /* *0.625 */

    HCI_LINFO_TIMEOUT_SLOTS /* x counter */
};

enum { /* packet_length[x] & packet_num[x] in struct hci_local_stru */
    HCI_LINFO_PACKET_ACL,
    HCI_LINFO_PACKET_LE, /* Maybe different with ACL, check HCI_LINFO_MASK_LE_SHARE_ACL_BUFFER */
    HCI_LINFO_PACKET_DATABLOCK,
    HCI_LINFO_PACKET_CONTROLLER_TO_HOST,

    HCI_LINFO_PACKET_SLOTS /* x counter */
};

enum { /* mask in struct hci_local_stru */
    HCI_LINFO_MASK_IN_PERIODIC_INQUIRY_MODE =             0x00000001,

    /* Two methods of data flow control are defined: 'packet-based' flow control and
    'data-block-based' flow control, known as buffer management */
    HCI_LINFO_MASK_BLOCK_BASED_DATA_FLOW_CONTROL =         0x00000002,
    HCI_LINFO_MASK_LOOPBACK_MODE =                         0x00000004, /* Loopback mode, not used now */
    HCI_LINFO_MASK_SET_EVENT_FILTER_INQUIRY =             0x00000008,

    /* Limited scan/advertise for LE, scan + adv toghter? */
    HCI_LINFO_MASK_LIMITED =                             0x00000010,
    /* Static address only generate once when power on, use to control generate only once */
    HCI_LINFO_MASK_STATIC_ADDRESS =                     0x00000020,
    HCI_LINFO_MASK_LE_ADV_PARAMETER =                     0x00000040, /* New LE_ADV_Parameter */
    HCI_LINFO_MASK_BONDABLE =                             0x00000080, /* Local bondable */
    /* LE Advertise/Scan Fast to Slow automaticly, when it comes slow, bit removed */
    HCI_LINFO_MASK_FAST_AUTO_SLOW =                     0x00000200,
    /* State QoS already called stoped, otherwise is data processing */
    HCI_LINFO_MASK_QOS_STOP =                             0x00000400,
    HCI_LINFO_MASK_CONTROLLER_TO_HOST_FLOW_CONTROL =     0x00000800,
    /* LE_Read_Buffer_Size return 0, shared between BR/EDR and LE transports */
    HCI_LINFO_MASK_LE_SHARE_ACL_BUFFER =                 0x00001000,
    HCI_LINFO_MASK_SECURE_SIMPLE_PAIRING_ENABLE =         0x00002000,
    HCI_LINFO_MASK_SECURE_CONNECTION_ENABLE =             0x00004000,

    HCI_LINFO_MASK_ADV_ENABLED =                         0x00008000, /* Mesh test */
    HCI_LINFO_MASK_SCAN_ENABLED =                         0x00010000,
    HCI_LINFO_MASK_LE_SCAN_PARAMETER =                    0x00020000,
    HCI_LINFO_MASK_LE_SCAN_ENABLE_PARAMETER =            0x00040000,
    HCI_LINFO_MASK_RESOLVABLE_ADDRESS =                 0x00080000, /* Local random address is set to RPA */
    HCI_LINFO_MASK_NONE_RESOLVABLE_ADDRESS =            0x00100000, /* Local random address is set to None-RPA */
    /* Local shall be the link master when accept the connection request */
    HCI_LINFO_MASK_BR_LINK_MASTER =                        0x00200000,
    HCI_LINFO_MASK_RESET_KEEP_BITS =                     (HCI_LINFO_MASK_BONDABLE | HCI_LINFO_MASK_QOS_STOP)
};

enum { /* Timer event of struct hci_local_stru */
    /* Timer[x] */
    HCI_LOCALRTX_ID_ADVSCAN =                         0,
    HCI_LOCALRTX_ID_LE_PRIVATE_ADDRESS_CHANGE =        1,
    HCI_LOCALRTX_ID_UNKNOWN_ACLDATA_FREE =             2,
    HCI_LOCALRTX_ID_AUTO_SNIFF =                       3, /* ACL link auto-sniff timer, by YDX, 2019.3.20 */
    HCI_LOCALRTX_ID_TIMER_COUNT, /* Num of Local Timer, 3 */

    /* Events */
    HCI_LOCALRTX_LE_ADVERTISE_TIME_LIMITED =         1, /* Time Limited, DONOT change sequence, 1 */
    HCI_LOCALRTX_LE_SCAN_TIME_LIMITED =             2, /* Time Limited, DONOT change sequence, 2 */
    HCI_LOCALRTX_LE_ADVERTISE_FAST_AUTO_SLOW =         3, /* Fast to slow, DONOT change sequence, 3 */
    HCI_LOCALRTX_LE_SCAN_FAST_AUTO_SLOW =             4, /* Fast to slow, DONOT change sequence, 4 */
    HCI_LOCALRTX_LE_RESOLVABLE_ADDRESS_CHANGE =        5, /* Change address */
    HCI_LOCALRTX_LE_NON_RESOLVABLE_ADDRESS_CHANGE =    6, /* Change address */
    HCI_LOCALRTX_UNKNOWN_ACLDATA =                    7, /* Cache ACL data before connection handle awares */
    HCI_LOCALRTX_AUTO_SNIFF =                       8 /* ACL link auto-sniff timer, by YDX, 2019.3.20 */
};

enum { /* le_status in local controller object struct hci_local_stru */
    /* Bitmask */
    HCI_LEISA_IDLE =         0,
    HCI_LEISA_INITIATE =     1,
    HCI_LEISA_SCAN =         2,
    HCI_LEISA_ADV =         3,
    HCI_LEISA_SA =             4,

    HCI_LEISA_ENABLE =         0x80,
    HCI_LEISA_DISABLE =     0x40,
    HCI_LEISA_TRANSIST =     (0x80 | 0x40),

    HCI_LEISA_IDLE2SCAN =     HCI_LEISA_ENABLE | (HCI_LEISA_IDLE << 3) | (HCI_LEISA_SCAN),
    HCI_LEISA_SCAN2IDLE =     HCI_LEISA_DISABLE | (HCI_LEISA_SCAN << 3) | (HCI_LEISA_IDLE),
    HCI_LEISA_IDLE2ADV =     HCI_LEISA_ENABLE | (HCI_LEISA_IDLE << 3) | (HCI_LEISA_ADV),
    HCI_LEISA_ADV2IDLE =     HCI_LEISA_DISABLE | (HCI_LEISA_ADV << 3) | (HCI_LEISA_IDLE),
    HCI_LEISA_SCAN2SA =     HCI_LEISA_ENABLE | (HCI_LEISA_SCAN << 3) | (HCI_LEISA_SA),
    HCI_LEISA_SA2SCAN =     HCI_LEISA_DISABLE | (HCI_LEISA_SA << 3) | (HCI_LEISA_SCAN),
    HCI_LEISA_ADV2SA =         HCI_LEISA_ENABLE | (HCI_LEISA_ADV << 3) | (HCI_LEISA_SA),
    HCI_LEISA_SA2ADV =         HCI_LEISA_DISABLE | (HCI_LEISA_SA << 3) | (HCI_LEISA_ADV),
    HCI_LEISA_SA2IDLE =     HCI_LEISA_DISABLE | (HCI_LEISA_SA << 3) | (HCI_LEISA_IDLE),
};

enum {
    HCI_LEISA_SCAN_ENABLE =     0x80,
    HCI_LEISA_ADV_ENABLE =      0x40,
    HCI_LEISA_SCAN_DISABLE =     0x20,
    HCI_LEISA_ADV_DISABLE =     0x10
};

#define HCI_ADVERTISING_TYPE_DISABLED    0xFF
#define HCI_LE_SCAN_TYPE_DISABLED        0xFF

#define hci_leisa_go(t, s, d)    ((t) | ((s) << 3) | (d))
#define hci_leisa_from(a)        (((a) >> 3) & 0x07)
#define hci_leisa_to(a)            ((a) & 0x07)

#define hci_linfo_packet_slotamp(local_) \
        ((((local_)->mask & HCI_LINFO_MASK_BLOCK_BASED_DATA_FLOW_CONTROL) != 0) \
        ? HCI_LINFO_PACKET_DATABLOCK \
        : HCI_LINFO_PACKET_ACL)

#define hci_linfo_packet_slot(local_, link_)        (((link_)->link.type == HCI_LINK_TYPE_LE) \
        ? (((local_)->mask & HCI_LINFO_MASK_LE_SHARE_ACL_BUFFER) != 0) \
            ? HCI_LINFO_PACKET_ACL \
            : HCI_LINFO_PACKET_LE \
        : (((local_)->mask & HCI_LINFO_MASK_BLOCK_BASED_DATA_FLOW_CONTROL) != 0) \
            ? HCI_LINFO_PACKET_DATABLOCK \
            : HCI_LINFO_PACKET_ACL)

enum { /* type of the LE_ControllerAddrNodeList_xxx */
    HCI_CANODE_MASK_WHITELIST =         0x01,
    HCI_CANODE_MASK_RESOLVINGLIST =     0x02
};

#if (defined(CONFIG_LE) && defined(CONFIG_BREDR_HCI))
/* Dual Mode */
#define LOCAL_ADDR_NUM        3    /* HCI_ADDR_IDENTITY, HCI_ADDR_PRIVATE, HCI_ADDR_BREDR */
#define HCI_ADDR_BREDR        2    /* After HCI_ADDR_PRIVATE */
#else
#define HCI_ADDR_BREDR        HCI_ADDR_IDENTITY /* LE Only or BREDR Only, Public is IDENTITY */
#ifdef CONFIG_LE
/* LE Only */
#define LOCAL_ADDR_NUM        2    /* HCI_ADDR_IDENTITY/HCI_ADDR_BREDR, HCI_ADDR_PRIVATE */
#else
/* BREDR Only */
#define LOCAL_ADDR_NUM        1    /* HCI_ADDR_IDENTITY/HCI_ADDR_BREDR */
#endif
#endif

struct le_set_scan_adv_stru {
    uint8_t enable;
    uint8_t type;
    uint8_t limited;
    uint8_t speed;
    uint8_t white;
};

struct hci_local_stru {                                /* Local controller include BREDR/AMP */
    HANDLE tl_hdl;                                    /* Transport Layer Handle */
    transport_layer_stru *func;                        /* Transport Layer Functions */
    /* Limited advertising or scanning for LE, NOCP delay control */
    struct fsm_timer *timer[HCI_LOCALRTX_ID_TIMER_COUNT];
    /* QoS send buffer list, etc, HCI_LINFO_PACKET_DATABLOCK */
    struct bth_list_stru acldata_list[CONFIG_HCI_LOCAL_ACLDATA_LIST_LEN];
    /* QoS callback list, data source instance, to control go/stop */
    struct bth_list_stru qos_cbk;
    struct bth_list_stru cmd_list;                        /* Level 1, struct hci_command_node_stru */
    /* Node as struct hci_controller_addr_node_stru, WhiteList and ResolvingList */
    struct bth_list_stru controller_addr_list;
    /* Unknown ACLdata before connection handle awares, a patch for driver */
    struct bth_list_stru unknown_data;
    /* Pointer to the 1st pendding command in cmd_list, pendcmd is not sent yet */
    struct hci_command_node_stru *pendcmd;
    /* etc: HCI_ADDR_IDENTITY, AMP controller does not have BD, cap only vailed in [0 = Public] */
    struct hci_address_stru addr[LOCAL_ADDR_NUM];
    /* Saves the public address of the device and assigns a value to addr[HCI_ADDR_IDENTITY] if necessary. */
    uint8_t  public_addr[BD_ADDR_LEN];
    /* Saves the static address of the device and assigns a value to addr[HCI_ADDR_IDENTITY] if necessary. */
    uint8_t  static_addr[BD_ADDR_LEN];
    hci_mp_int dcounter[2];                            /* etc: HCI_DCOUNTER_SEND */

    uint32_t mask;                                    /* Bitmask etc:HCI_LINFO_MASK_IN_PERIODIC_INQUIRY_MODE */
    uint32_t timeout[HCI_LINFO_TIMEOUT_SLOTS];        /* [x] as HCI_LINFO_TIMEOUT_PAGE */
    uint16_t packet_length[HCI_LINFO_PACKET_SLOTS];    /* [x] as HCI_LINFO_PACKET_ACL */
    uint16_t packet_num[HCI_LINFO_PACKET_SLOTS];        /* [x] as HCI_LINFO_PACKET_ACL */
    uint8_t hci_version;                                /* HCI_HCI_VERSION_4_0 */
    uint8_t supported_commands[64];                    /* Supported Command Bit mask table */
    uint8_t lmp_features[HCI_LMP_FEATURE_MAX_PAGE][8]; /* extended features 8 pages? current allocate 2 page? */
#ifdef CONFIG_LE
    uint8_t le_features[8];                            /* LE features */
    uint8_t le_states[8];                                /* LE supported states */
    uint8_t white_list_size;                            /* From LE_Read_White_List_Size returns */
    uint8_t resolving_list_size;                        /* From LE_Read_Resolving_List_Size returns */
    uint16_t max_adv_data_length;                        /* From LE_Read_Maximum_Advertising_Data_length returns */
    uint8_t num_adv_sets;                                /* From LE_Read_Number_Of_Supported_Advertising_Sets returns */
    uint8_t prd_dev_list_size;                        /* From LE_Read_Periodic_Avertiser_List_Size returns */

    uint8_t leisa_tar;                                /* HCI_LEISA_ADV_ENABLE */
    uint8_t leisa_cur;                                /* HCI_LEISA_ADV */
    uint8_t is_le_ext_adving;
    struct le_set_scan_adv_stru adv_set;                /* ADV setting - For compatibility */
    struct le_set_scan_adv_stru scan_set;                /* SCAN setting - For compatibility */
    struct hci_le_set_advertising_parameters_stru *adv_par; /* Legacy Advertising parameters */
    struct hci_le_ext_scan_parameters_stru *scan_par; /* Extended Scan parameters */
    struct hci_le_ext_scan_enable_stru scan_enable_par; /* Extended Scan Enable parameters */
#endif

    uint8_t cap;                                        /* etc: HCI_ADDRESS_CAP_LE */
#ifdef MULTIPLE_TRANSPORT_LAYER_SUPPORT
    uint8_t ampid;                                    /* HCI allocated local Controller ID */
#endif
    /* From Command_Status and Command_Complete num_hci_command_packets */
    uint8_t cmd_num;

    void *_rom;
};

/* --------------------------------------------------------------------------- */
/*                             Root Object structure                             */
/* --------------------------------------------------------------------------- */
#define HCI_AFH_MAP_MAX_LEN 11
struct hci_root_stru {                        /* hci_fsminst->user_data, HCI Root */
    struct bth_list_stru local;                    /* struct hci_local_stru, Local controllers */
    struct bth_list_stru remote;                    /* struct hci_remote_stru, Remote devices */
    struct bth_list_stru link;                    /* struct hci_link_stru, Links, include unestablished */
    struct bth_list_stru evcbk;                    /* struct HCI_EvCBKStru, HCI user callbacks */
    struct bth_list_stru evcbk_prime;        /* struct HCI_EvCBKStru, adv report HCI user callbacks */
    /* struct hci_security_info_stru, security requirement registered by GAP */
    struct bth_list_stru security;
    tl_callback_stru *tlbase;                /* Poor transport layer interface.... */
    uint8_t io_capability;                    /* etc: HCI_IO_CAPABILITY_DISPLAYONLY */
    uint8_t afhmap[HCI_AFH_MAP_MAX_LEN];                        /* 81bits, AFH RF Channels map for BREDR, LE, WIFI */

    void *_rom;
};

/* --------------------------------------------------------------------------- */
/*                             Link Object structures                               */
/* --------------------------------------------------------------------------- */
struct hci_link_stru {                        /* Common header of a Link */
    struct hci_local_stru *local;
    struct hci_remote_stru *remote;
    uint16_t connection_handle;                /* ACL/LE/Log Handle */
    uint8_t type;                                /* HCI_LINK_TYPE_ACL, HCI_LINK_TYPE_LE, HCI_LINK_TYPE_LOG */
};

enum { /* Timer event */
    HCI_ACLLINK_TIMER_LAZYRTX = 1,
    HCI_ACLLINK_TIMER_TRANSRTX /* for L2CAP LE Update parameter */
};

enum { /* ACL status, status in HCI_ACLHDL level, in L2CAP_ACLLinkStru/hci_le_link_stru */
    HCI_ACL_DATARECV =                         0x80, /* status Bitmask, Valid status to receive ACL data */
    /* Connecting or Cancel, LE command has no BD, so only one outstanding command may exist */
    HCI_ACL_LECONN =                         0x40,
    /* Disconnecting or Cancel, only local initiated disconnection may lead to reconnect */
    HCI_ACL_LOCAL_DISCONNECT =                 0x20,

    HCI_ACL_IDLE =                             (0),
    /* Before connection complete, active or passive or raced */
    HCI_ACL_CONNECTING =                     (1 | HCI_ACL_LECONN),
    HCI_ACL_READY =                         (2),
    HCI_ACL_READY_WITH_REMOTE_INFO =         (3 | HCI_ACL_DATARECV),
    HCI_ACL_DISCONNECTING =                 (4 | HCI_ACL_DATARECV | HCI_ACL_LOCAL_DISCONNECT),
    HCI_ACL_CANCEL =                         (5 | HCI_ACL_LECONN | HCI_ACL_LOCAL_DISCONNECT),
    /* Idle, but waiting for local status changes, etc, local is scaning, disable before initiate connection */
    HCI_ACL_LECONNECT_WAITLOCAL =             (6)
};

struct hci_recv_context_stru {
    struct bth_list_stru recvdata_list;            /* Uncompleted l2cap acl data packets, struct buff_stru ** */
    uint16_t receive_num;                        /* Controller to host outstanding packet before NoCP */
};

struct hci_send_context_stru {
    uint16_t sending_num;                        /* Host to controller outstanding packet before NoCP */
};

enum {
    HCI_CURRENT_MODE_EXITING =                 0xFF
};

struct hci_acl_link_stru { /* Each remote shall have only one ACL-U */
    struct hci_link_stru link;
    struct hci_send_context_stru send;
    struct hci_recv_context_stru recv;

    /* Common part of ACL/LE */
    struct fsm_timer *timer;             /* ACL lazy idle timer, delay ACL disconnection */
    /* Delay timer before killing LE Link due to LE Connection Cancel.
       to prevent from executing another LE Create Connection Command before LE Connection Complete event returned. */
    struct fsm_timer *cancel_timer;
    uint8_t ref_count;                    /* + -, How many cid session and echo/info ref this hci_hdl */
    uint8_t status;                         /* etc: HCI_ACL_CONNECTING */
    uint8_t mask;                            /* etc: HCI_ACLMASK_INITIATOR */
    uint8_t response_num;                    /* Waiting for accept/reject the incoming connection upper cbk counter */

    /* Different part with LE */
    uint8_t link_mode;                    /* etc: HCI_CURRENT_MODE_SNIFF, + HCI_CURRENT_MODE_EXITING */
    uint8_t link_role;                    /* link role when accept incoming connection request */
    /* ACL link auto-sniff, by YDX, 2019.3.20 */
    uint8_t idle_seconds;                     /* current idle duration (*2s) */
    uint8_t tr_occured;                       /* 0 - No data transfer. 1 - data transfer. */
    /* Sniff parameters. NULL means auto-sniff is not required for this link */
    struct hci_auto_sniff_par_stru *sniff_par;
    uint8_t active_disc_reason; /* For Print Disconnect Reason */
    hci_subrate subrate_par; /* add for same size with le link */

    void *_rom;
};

/* Note:  struct hci_acl_link_stru And struct hci_le_link_stru Must be Same Size */

/* Header part shall be equal to struct hci_acl_link_stru, Each remote shall have only one LE-U */
struct hci_le_link_stru {
    struct hci_link_stru link;
    struct hci_send_context_stru send;
    struct hci_recv_context_stru recv;

    /* Common part of ACL/LE */
    struct fsm_timer *timer;             /* ACL lazy idle timer, delay ACL disconnection */
    /* Delay timer before killing LE Link due to LE Connection Cancel.
       to prevent from executing another LE Create Connection Command before LE Connection Complete event returned. */
    struct fsm_timer *cancel_timer;
    uint8_t ref_count;                    /* + -, so signed, How many cid session and echo/info ref this hci_hdl */
    uint8_t status;                         /* etc: HCI_ACL_CONNECTING */
    uint8_t mask;                            /* etc: HCI_ACLMASK_INITIATOR */
    uint8_t response_num;                    /* Waiting for accept/reject the incoming connection upper cbk counter */
    /* Different part with ACL */
    uint8_t le_features[8];                /* LE Read Remote used features, ACL is in remote object */
    uint8_t active_disc_reason; /* For Print Disconnect Reason */
    hci_subrate subrate_par; /* subrate parameters. NULL means no subrate event for this le link */

    void *_rom;
};

struct hci_log_link_stru { /* Physical link is removed since handle is allocated by host */
    struct hci_link_stru link;
    struct hci_send_context_stru send;
    uint8_t tx_flow_spec_id;
};

struct hci_io_capability_req {
    uint8_t bond;
    uint8_t init;
    uint8_t mitm;
};

struct hci_set_channel_classification_stru {
    uint32_t c; /* Central Frequency, etc,2412 */
    uint32_t w; /* Bandwidth, 22,40 */
    uint8_t mask; /* etc, HCI_SETAFH_OCCUPY */
};

#define HCI_CMDSTRU         void
#define hci_cmdpar(arg_)    ((HCI_CMDSTRU *)(arg_))
#define HCI_CMDNEW             MEM_NEW(sizeof(HCI_CMDSTRU))

/* --------------------------------------------------------------------------- */
/*                             HCI data/Command Related                             */
/* --------------------------------------------------------------------------- */

struct hci_tl_recv_stru { /* Arg in hcic_tl_recv_event */
    HANDLE host_hdl;
    uint8_t *data; /* Newed by HCI */
    uint8_t *end; /* end pointer of data, the 0xFD after last octet of valid data */
};

enum { /* Command RTX timeout */
    /* Since the maximum response timeout is strongly dependent on the HCI-Transport Layer used,
    it is recommended to use a default value of one second for this timer */
    HCI_TIME_CMDRTX =                         CONFIG_HCI_TIME_CMDRTX,    /* Before Command status */
    HCI_TIME_CMDERTX =                         CONFIG_HCI_TIME_CMDERTX, /* After Command status */
    HCI_TIME_CMDPINCODE =                     CONFIG_HCI_TIME_PINCODE, /* Both side user PIN Code input time */
    HCI_TIME_CMDDELTA =                     CONFIG_HCI_TIME_DELTA,    /* Addtional time for calcuated time */
    /* ACL connection cast time, so when no body use ACLLink, still waiting some seconds */
    HCI_TIME_ACLLAZY =                         CONFIG_HCI_TIME_ACLLAZY,
    HCI_TIME_ACCEPT_CONNECTION_TIMEOUT =     CONFIG_HCI_TIME_ACCEPT_CONNECTION, /* 0x0001~0xB540, 0.625~29s */
    HCI_TIME_LE_CREATE_CONNECTION =         CONFIG_HCI_TIME_LE_CREATE_CONNECTION
};

enum { /* Flag in hci_local_remove_one */
    HCI_LOCAL_REMOVE,
    HCI_LOCAL_RESET
};

enum { /* addhead in HCI_CommandL2_AddNode */
    HCI_CMDL2_ADDHEAD,
    HCI_CMDL2_ADDTAIL
};

enum { /* return in hci_set_event_filter_inquiry_check */
    HCI_SET_EVENT_FILTER_ACTION_ADD_INQUIRY,
    HCI_SET_EVENT_FILTER_ACTION_CLEAR_INQUIRY,
    HCI_SET_EVENT_FILTER_ACTION_CLEAR_ALL,
    HCI_SET_EVENT_FILTER_ACTION_OTHER
};

enum { /* Returning of hci_security_link_key_good */
    HCI_SECURITY_LINKKEY_GOOD =     0,
    HCI_SECURITY_LINKKEY_NOTEXIST = 1,
    HCI_SECURITY_LINKKEY_BAD =         2
};

/* HCI internal callback function dispatcher */
typedef void (HCI_CbkTableFunc) (struct hci_local_stru *local, void *par);

#define HCI_ROOT                                ((struct hci_root_stru *)(hci_fsminst->user_data))
#define HCI_LOCALLIST                            (&(HCI_ROOT->local))
#define HCI_REMOTELIST                            (&(HCI_ROOT->remote))
#define HCI_LINKLIST                            (&(HCI_ROOT->link)) /* ACL,LE,Logical Links */
#define HCI_EVCBKLIST                            (&(HCI_ROOT->evcbk)) /* Callback Tables */
#define HCI_EVCBKLIST_PRIME                     (&(HCI_ROOT->evcbk_prime)) /* Prime Callback Tables */
#define HCI_SECURITYLIST                        (&(HCI_ROOT->security))
#define HCI_SECURITY_LOCALIO                    (HCI_ROOT->io_capability)
#define HCI_SECURITY_LOCALIO_GET                ((HCI_SECURITY_LOCALIO != \
    HCI_IO_CAPABILITY_KEYBOARDDISPLAY) ? HCI_SECURITY_LOCALIO : HCI_IO_CAPABILITY_DISPLAYYESNO)
#define HCI_AFHMAP                                (HCI_ROOT->afhmap)
#define HCI_SUPCMDGAP_LAST \
    (&hci_table_supportcmdgap[(sizeof(hci_table_supportcmdgap) / sizeof(struct hci_op_code_gap_stru)) - 1])
#define HCI_EVGAP_LAST \
    (&hci_table_evgap[(sizeof(hci_table_evgap) / sizeof(struct hci_index_gap_stru)) - 1])
#define HCI_EVOP_LAST                             (&g_hci_table_evop[g_hci_table_evop_size - 1])
#define hci_codingp_event(evi_)                    (&hci_table_coding_event[hci_table_codingoff_event[(evi_)]])
/* ACL or LE */
#define hci_linkevo(link_)                        (((link_)->link.type == \
    HCI_LINK_TYPE_ACL) ? HCI_EVO_ACL_DATA_IND : HCI_EVO_LE_DATA_IND)
#define hci_cmderr(op_)                            (((op_) << 16) | HCI_EVO_COMMAND_ERROR) /* HCI_OPC_ for hci_cbk */
#define hci_command_send_internal(local_, par_, link_, opscode_)     hci_command_send_core((local_), \
    hci_command_create_node((par_), (link_), (opscode_)))
#define hci_key_unauthenticated(type_)            (((type_) != \
    HCI_KEY_TYPE_AUTHENTICATED_COMBINATION_KEY_GENERATED_FROM_P192) && \
    ((type_) != HCI_KEY_TYPE_AUTHENTICATED_COMBINATION_KEY_GENERATED_FROM_P256))

/* Macro components of struinfo */
#define hci_stru2reqoff(struinfo_)                ((uint8_t)(struinfo_)) /* Req struinfo offset */
/* Ret struinfo offset, 0xFF for not exist */
#define hci_stru2retoff(struinfo_)                ((uint8_t)((struinfo_) >> 8))

/* uint32_t op_ev */
#define hci_opev(o, e)                            (((o) << 16) | (e))
#define hci_op(s)                                ((uint16_t)((s) >> 16))
#define hci_ev(s)                                ((uint16_t)(s))

/* HCI is little endian */
#define HCI_DECODE2BYTE                            decode2byte_little
#define HCI_DECODE4BYTE                            decode4byte_little
#define HCI_ENCODE2BYTE                            encode2byte_little
#define HCI_ENCODE4BYTE                            encode4byte_little

/* --------------------------------------------------------------------------- */
/*                         HCI Structure < -> BitStream Macros                          */
/* --------------------------------------------------------------------------- */
#ifdef CONFIG_ALIGN_FOURBYTES
#define CONFIG_ALIGN_BTYES                        4
#elif defined(CONFIG_ALIGN_TWOBYTES)
#define CONFIG_ALIGN_BTYES                        2
#else /* defined(CONFIG_ALIGN_ONEBYTE) */
#define CONFIG_ALIGN_BTYES                        1
#endif
#define hci_padstep(s)                            (min(CONFIG_ALIGN_BTYES, (s)))
#define hci_padding(p, \
    l)                        ((p) += ((hci_padstep(l) - ((uint32_t)(uintptr_t)(p) % hci_padstep(l))) % hci_padstep(l)))

enum {
    HCI_CODE_0BITS =                             0x00,
    HCI_CODE_2BITS =                             0x01,
    HCI_CODE_4BITS =                             0x02,
    HCI_CODE_8BITS =                             0x03,

    /* 2Bits Basic type Items */
    HCI_CODE_N1 =                                 0x00, /* Num */
    HCI_CODE_U1 =                                 0x01, /* 1Oct - uint8_t */
    HCI_CODE_U2 =                                 0x02, /* 2Oct - uint16_t */
    HCI_CODE_U4 =                                 0x03 /* 4Oct - uint32_t */
};

enum { /* flag in hci_coding_len */
    HCI_CODING_INSTRU =                         0x01, /* Src input is structure, not air */
    HCI_CODING_OUTSRC =                         0x02, /* output src length, not dst */

    /* Input stru, Output stru length */
    HCI_CODING_GETSTRULEN =                     (HCI_CODING_INSTRU | HCI_CODING_OUTSRC),
    HCI_CODING_GETAIR =                         (HCI_CODING_INSTRU), /* Get PDU or length of PDU to the controller */
    HCI_CODING_GETSTRU =                         0 /* GET structure or its length to upper layer */
};

enum { /* event in hcic_security_request */
    HCI_SECURITY_EVENT_REQUEST =                 1,
    HCI_SECURITY_EVENT_ABORT =                     2
};

enum { /* Bitmask in authentication_requirements enum */
    HCI_AUTHENTICATION_REQUIREMENTS_NOMITM =     0x00,
    HCI_AUTHENTICATION_REQUIREMENTS_MITM =         0x01
};

enum {
    HCI_ACLDATA_MASK_SENTOUT =                     0x01, /* send out succefully or not */
    HCI_ACLDATA_MASK_ADDTAIL =                     0x02 /* Add head or tail */
};

enum { /* mask in hci_acl_link_disconnect_deal */
    HCI_ACLLINK_DISCONNECT_NOW =                 0x01,
    HCI_ACLLINK_DISCONNECT_EMERGENCY =             0x02
};

/* Move here since SMP are using this to store SMPD address */
enum { /* index of addr[x] in struct hci_local_stru / struct hci_remote_stru */
    /* Public or Static, distinguish by the 49th bit in addr, for BREDR / LE */
    HCI_ADDR_IDENTITY =                             0,
    /* Used in Local object, RPA or nRPA, Only for LE, by LESetRandomAddress */
    HCI_ADDR_PRIVATE =                                 1,
    /* Used in Remote object, current LE link(or advertising remote using) address */
    HCI_ADDR_LELINK =                                 1
};

/* --------------------------------------------------------------------------- */
/*                             Function Definations                               */
/* --------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------
Description:
    This function is called to send a packet into Transport Layer.
--------------------------------------------------------------------------- */
void hci_tl_send_data(struct hci_local_stru *local, uint8_t *data, uint32_t len);

uint8_t hci_dt_get_ev_index(uint8_t evcode);
uint32_t hci_dt_get_sup_cmd_bit_index(uint16_t opcode);

void hci_cbk(struct hci_local_stru *local, void *context, void *arg, uint32_t len, uint32_t op_ev);
#ifdef CONFIG_BREDR_HCI
uint8_t hci_cbk_get_num(uint16_t index);
#endif
void le_set_scan_parmeters(struct hci_local_stru *local, uint8_t type, uint8_t limited,
    uint8_t speed);
void le_set_advertising_parameters(struct hci_local_stru *local, uint8_t type,
    uint8_t limited, uint8_t speed, uint8_t white);
void le_set_direct_advertising_parameters(struct hci_local_stru *local,
    struct hci_address_stru *peer, uint8_t type, uint8_t limited, uint8_t speed, uint8_t white);

void le_isa_check(struct hci_local_stru *local);
void le_isa_connect_complete(struct hci_local_stru *local);
void le_isa_target(struct hci_local_stru *local, uint8_t tar);
void le_isa_enable_result(struct hci_local_stru *local, uint8_t success);
void le_isa_go_idle(struct hci_local_stru *local);
void le_isa_set_op_report(const struct hci_local_stru *local, uint8_t op_id, uint8_t op_state);
struct hci_le_link_stru *le_link_find_on_local(const struct hci_local_stru *local);
void hci_local_start_timer(struct hci_local_stru *local, uint8_t *prompt,
    uint32_t timeout, uint8_t restart, uint8_t event, uint8_t id);
void hci_local_init(struct hci_local_stru *local);
void hci_local_remove_unknown_acl_data(struct hci_local_stru *local);
void hci_local_remove(struct hci_local_stru *local);
void hci_local_remove_one(struct hci_local_stru *local, uint8_t flag);
void hci_local_changed_notify(struct hci_local_stru *local, HANDLE tl_hdl, uint8_t action);
struct hci_local_stru *hci_local_find_by_hdl(const HANDLE host_hdl);
#ifdef NOT_USED_YET
struct hci_local_stru *hci_local_find_by_addr(struct hci_address_stru *l);
#endif
struct hci_local_stru *hci_local_find_by_id(uint8_t ampid);
struct hci_local_stru *hci_local_find_by_tlhdl(HANDLE tl_hdl);

/* ACL link auto-sniff, by YDX, 2019.3.20 */
void hci_auto_sniff_stop_timer(struct hci_local_stru *local);
void hci_auto_sniff_rtx(struct hci_local_stru *local);
void hcic_auto_sniff_set(const struct fsm_inst *fi, uint8_t event,
    struct hci_auto_sniff_req_stru *req);
#ifdef CHIP_BRANDY
struct hci_remote_stru *hci_remote_find_by_bd(uint8_t *bd);
#endif
void hci_remote_remove(struct hci_remote_stru *remote);
void hci_remote_remove_unuse(struct hci_remote_stru *remote);
void hci_remote_set_to_die(uint8_t cap);
struct hci_remote_stru *hci_remote_find_by_addr(struct hci_address_stru *r);

void *hci_link_create(struct hci_local_stru *local, struct hci_remote_stru *remote, uint8_t type);
void hci_link_remove_by_lr(const void *lr);
void hci_link_remove_all(void);
void hci_link_remove(struct hci_acl_link_stru *link, uint8_t reason);
void *hci_link_find_by_hdl(const struct hci_local_stru *local, uint16_t handle);
void bth_hci_acl_data_recv(struct hci_acl_link_stru *link, struct buff_stru *in, uint8_t pb);
uint8_t bth_hci_acl_data_recombination_check(struct buff_stru *fst_pkt, struct buff_stru *next_pkt);
void hci_acl_link_disconnect_send(struct hci_acl_link_stru *link, uint8_t reason, uint8_t mask);
uint8_t hci_acl_link_disconnect_deal(struct hci_acl_link_stru *link, uint8_t mask);
void hci_acl_link_controller2host_fc(struct hci_acl_link_stru *link, uint8_t force);

struct hci_acl_link_stru *hci_acl_link_find_by_bd_type(struct hci_local_stru *local, uint8_t *bd,
    uint8_t type, uint8_t linktype);
#define hci_acl_link_find_by_bd(local_, bd_)        hci_acl_link_find_by_bd_type((local_), (bd_), \
    HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS, HCI_LINK_TYPE_ACL)

void hci_acl_link_cbk(struct hci_acl_link_stru *link, uint16_t reason_job);
void hci_acl_link_ready(struct hci_acl_link_stru *link);
void hci_acl_link_create_connection_cancel(struct hci_acl_link_stru *link, uint8_t mask);
void hcic_acl_link_un_ref(const struct fsm_inst *fi, uint8_t event, struct hci_acl_link_stru *link);
void hcic_acl_link_kill(const struct fsm_inst *fi, uint8_t event, struct hci_acl_link_stru *link);
void hci_acl_link_create_connection(struct hci_acl_link_stru *link);
void hci_acl_link_connection_cancel_delay_start(struct hci_acl_link_stru *link, uint8_t status);
struct hci_acl_link_stru *hci_acl_link_find_by_lr(const struct hci_local_stru *local,
    const struct hci_remote_stru *remote, uint8_t linktype);
struct hci_acl_link_stru *hci_acl_link_find_by_ptr(const void *acl);

void bth_hci_recv_acl_data(const struct fsm_inst *fi, uint8_t type, struct buff_stru *in);
void hcic_tl_recv_sco_data(const struct fsm_inst *fi, uint8_t event, struct buff_stru *in);

void list_qo_s_remove_all(struct hci_local_stru *local);
void list_qos_check_cbk(struct hci_local_stru *local);

uint16_t hci_set_packet_type(struct hci_local_stru *local, uint8_t link_type);
void hci_set_synchronous_param(struct hci_local_stru *local, struct hci_synchronous_connection_stru *in,
    struct hci_synchronous_connection_stru *esco);
uint8_t hci_set_event_filter_inquiry_check(struct hci_set_event_filter_stru *par);

uint8_t hci_command_check_support(struct hci_local_stru *local, uint16_t opcode);
void hci_command_send_core(struct hci_local_stru *local, struct hci_command_node_stru *cmd);
struct hci_command_node_stru *hci_command_create_node(void *par, void *context, uint32_t opscode);
uint8_t hci_command_check_block(struct hci_local_stru *local, struct hci_command_node_stru *cmd);
void hci_struct_to_stream_data(struct hci_command_node_stru *cmd, uint8_t *dst, uint16_t stream_len);
void hci_command_remove(struct hci_local_stru *local, struct hci_command_node_stru *cmd, void *arg, uint16_t len,
                        uint16_t status, uint16_t evindex);
void hci_command_remove_by_local(struct hci_command_node_stru *in);
#define hci_command_remove_error(local_, cmd_, status_)    hci_command_remove((local_), (cmd_), NULL, 0, (status_), \
                                                                             HCI_EVO_COMMAND_ERROR)

void hci_command_check_send(struct hci_local_stru *local);
struct hci_command_error_stru *hci_command_error_new(struct hci_command_node_stru *cmd, uint16_t *lenout,
    uint16_t status);
/* Could be NULL */
void hci_command_remove_by_context(struct hci_local_stru *local, void *context, uint16_t status);
struct hci_command_node_stru *hci_command_find_by_op_code(struct hci_local_stru *local, uint16_t opcode);
struct hci_command_node_stru *hci_command_find_by_ev_index(struct hci_local_stru *local,
    const void *link, uint8_t evindex, uint16_t opcode);
#ifdef CONFIG_BREDR_HCI
void hci_command_remove_by_op_code(struct hci_local_stru *local, uint16_t opcode, uint16_t status);
#endif
void hci_command_send_emergency(struct hci_local_stru *local, void *par, uint32_t opscode);
void hci_cmd_send_internal_func(struct hci_local_stru *local, void *par, void *context, uint32_t opscode);
void hci_cmd_rmv_err_func(struct hci_local_stru *local, struct hci_command_node_stru *cmd, uint16_t status);
void hci_acl_data_send_pendding(struct hci_local_stru *local, uint8_t slot);
#ifdef NOT_USED_YET
int hci_tl_recv_data(HANDLE host_hdl, uint8_t *data, uint32_t len);
#endif
int hci_tl_recv_data_buff(HANDLE host_hdl, struct buff_stru *buff);
void hcic_tl_recv_event(const struct fsm_inst *fi, uint8_t type, struct hci_tl_recv_stru *in);

uint8_t hci_coding_get_bits(struct hci_bit_pointer_stru *bp, uint8_t len);
uint16_t hci_coding_len(uint8_t *src, uint8_t *end, const uint8_t *p, uint8_t flag);
void hci_coding(uint8_t *src, uint8_t *end, uint8_t *dst, const uint8_t *p, uint8_t flag);
void hci_internal_cbk(HANDLE host_hdl, void *context, void *par, uint32_t evindex);
void hci_internal_cbk_reg(void);

#ifdef CONFIG_HCI_NEW_STREAM_STRUCT_CONVERSION
uint16_t hci_stream_to_struct(uint8_t event_index, uint8_t *in_stream, void **out_stru);
uint16_t hci_struct_to_stream(struct hci_command_node_stru *cmd, uint8_t **out_stream);
uint16_t hci_cmd_cmplete_stream_to_struct(struct hci_command_complete_ev_stru *in, void **out_stru);
uint16_t hci_le_meta_stream_to_struct(struct hci_le_meta_ev_stru *ev, void **out_stru);
#endif
void hci_ev_command_status_restart_ertx(struct hci_local_stru *local, struct hci_command_node_stru *cmd);
void hci_ev_command_status(struct hci_local_stru *local, struct hci_command_status_ev_stru *in);
void hci_ev_command_complete(struct hci_local_stru *local, struct hci_command_complete_ev_stru *in);
void hci_ev_loopback_command(struct hci_local_stru *local, struct hci_loopback_command_ev_stru *ev);

void hci_ev_link_key_request(struct hci_local_stru *local, struct hci_link_key_request_ev_stru *ev);
void hci_ev_pin_code_request(struct hci_local_stru *local, struct hci_pin_code_request_ev_stru *ev);
void hci_ev_authentication_complete(struct hci_local_stru *local, struct hci_authentication_complete_ev_stru *ev);
void hci_ev_encryption_change(struct hci_local_stru *local, struct hci_encryption_change_ev_stru *ev);
void hci_ev_io_capability_request(struct hci_local_stru *local, struct hci_io_capability_request_ev_stru *ev);
void hci_ev_io_capability_response(struct hci_local_stru *local, struct hci_io_capability_response_ev_stru *ev);
void hci_ev_user_confirmation_request(struct hci_local_stru *local, struct hci_user_confirmation_request_ev_stru *ev);
void hci_ev_remote_oob_data_request(struct hci_local_stru *local, struct hci_remote_oob_data_request_ev_stru *ev);
void hci_ev_simple_pairing_complete(struct hci_local_stru *local, struct hci_simple_pairing_complete_ev_stru *ev);

void hci_ev_connection_complete(struct hci_local_stru *local, struct hci_connection_complete_ev_stru *ev);
void hci_ev_connection_request(struct hci_local_stru *local, struct hci_connection_request_ev_stru *ev);
void hci_ev_synchronous_connection_complete(struct hci_local_stru *local,
                                            struct hci_synchronous_connection_complete_ev_stru *ev);
void hci_ev_disconnection_complete(struct hci_local_stru *local, struct hci_disconnection_complete_ev_stru *ev);
void hci_op_create_connection_cancel(struct hci_local_stru *local, struct hci_create_connection_cancel_comp_stru *ev);
void hci_ev_number_of_cmp_pkt_rom(struct hci_local_stru *local, struct hci_number_of_completed_packets_ev_stru *ev);
uint32_t hci_ev_cmd_status_restart_ertx_rom(struct hci_local_stru *local, struct hci_command_node_stru *cmd);
struct hci_command_node_stru *hci_cmd_find_by_cmd_status_evt_rom(struct hci_local_stru *local, uint16_t opcode);
void hci_ev_hardware_error_rom(struct hci_local_stru *local,
    const struct hci_hardware_error_ev_stru *ev);
#ifdef CONFIG_BREDR_HCI
void hci_ev_remote_supported_features_cap(const struct hci_local_stru *local,
    struct hci_remote_stru *remote, uint8_t page);
#endif
void hci_op_read_local_supported_cmd_rom(struct hci_local_stru *local,
                                         struct hci_read_local_supported_commands_comp_stru *ev);
void hci_op_read_local_feature_page0rom(struct hci_local_stru *local, uint8_t *fea);
void hci_op_read_local_feature_page0(struct hci_local_stru *local, const uint8_t *fea);
void hci_op_read_buffer_size_rom(struct hci_local_stru *local, struct hci_read_buffer_size_comp_stru *ev);

void hcic_init(void);
#ifdef CONFIG_BT_STACK_DONE
void hcic_done(struct fsm_inst *fi, uint8_t event, const void *arg);
#endif
void hcic_cleanup(const struct fsm_inst *fi, uint8_t event, const void *arg);
void hcic_cbk_reg(const struct fsm_inst *fi, uint8_t event, struct hci_cbk_node_stru *arg);
void hcic_cbk_prime_reg(const struct fsm_inst *fi, uint8_t event, struct hci_cbk_node_stru *arg);
void hcic_cbk_un_reg(const struct fsm_inst *fi, uint8_t event, const struct hci_cbk_line_stru *table);
void hcic_commandl1_input(const struct fsm_inst *fi, uint8_t event, struct hci_command_node_stru *in);
void hcic_commandl2_input(const struct fsm_inst *fi, uint8_t event, struct hci_command_queue_stru *queue);
void hcic_command_rtx(const struct fsm_inst *fi, uint8_t event, struct hci_rtx_timer_stru *rts);
void hcic_tl_reg(const struct fsm_inst *fi, uint8_t event, struct hci_local_stru *local);
void hcic_tl_un_reg(const struct fsm_inst *fi, uint8_t event, void *tl_hdl);
void hcic_acl_data_send(const struct fsm_inst *fi, uint8_t event, struct buff_stru *arg);
#ifdef CONFIG_BREDR_HCI
void hcic_sco_data_send(const struct fsm_inst *fi, uint8_t event, struct buff_stru *arg);
#endif
void hcic_create_sco_connection(const struct fsm_inst *fi, uint8_t event, struct hci_create_scoui_stru *in);
void hcic_acl_link_disconnect(const struct fsm_inst *fi, uint8_t event, struct hci_disconnect_stru *par);
void hcic_acl_link_connection_response(struct fsm_inst *fi, uint8_t event,
    struct hci_acl_connection_response_ui_stru *in);
void hcic_synchronous_connection_response(const struct fsm_inst *fi, uint8_t event,
                                          struct hci_synchronous_connection_response_ui_stru *in);
void hcic_synchronous_enhanced_connection_response(const struct fsm_inst *fi, uint8_t event,
                                                   struct hci_synchronous_enhanced_connection_response_ui_stru *in);
#ifdef CONFIG_BREDR_HCI
void hcic_set_channel_classification(const struct fsm_inst *fi, uint8_t event,
    struct hci_set_channel_classification_stru *par);
#endif
void hcic_set_default_link_role(const struct fsm_inst *fi, uint8_t event, void *arg);

void hci_security_go(struct hci_acl_link_stru *link);
void hci_security_result_ev(struct hci_local_stru *local, uint16_t hdl, uint16_t mask, uint16_t status);
void hci_security_clear_remote_by_acl_link(struct hci_acl_link_stru *link);
void hcic_security_authorized(const struct fsm_inst *fi, uint8_t event, struct hci_security_author_req_stru *in);
void hcic_security_request(const struct fsm_inst *fi, uint8_t event, struct hci_security_node_stru *in);
void hcic_security_require_reg(const struct fsm_inst *fi, uint8_t event, struct hci_security_info_stru *info);
void hcic_security_require_un_reg(const struct fsm_inst *fi, uint8_t event,
    struct hci_security_info_stru *info);
void hcic_security_set_local_io(const struct fsm_inst *fi, uint8_t io_capability, void *arg);
void hcic_security_set_bondable(const struct fsm_inst *fi, uint8_t bondable, void *arg);
void hcic_security_set_oob(const struct fsm_inst *fi, uint8_t event, struct hci_security_remote_oob_stru *in);
void hcic_security_read_local_oob_data(const struct fsm_inst *fi, uint8_t event, void *arg);
void hcic_security_link_key_recover(const struct fsm_inst *fi, uint8_t event,
    struct hci_security_link_key_recover_stru *in);
void hcic_security_link_key_remove(const struct fsm_inst *fi, uint8_t event, uint8_t *bd);
void hcic_security_pin_code_reply(const struct fsm_inst *fi, uint8_t event, struct hci_pin_code_request_reply_stru *in);
void hcic_security_pairing_confirmation(const struct fsm_inst *fi, uint8_t event, uint8_t *bd);

/* Used in DBG */
struct hci_security_info_stru *hci_security_object_find(struct hci_security_info_stru *node,
                                                        struct hci_security_info_stru *in);
/* Used in DBG */
uint8_t hci_security_link_key_good(struct hci_local_stru *local, struct hci_remote_stru *remote,
    struct hci_security_node_stru *node);
void hcic_security_le_keys_recover(const struct fsm_inst *fi, uint8_t event, struct bth_list_stru *list);
void hcic_security_le_keys_remove(const struct fsm_inst *fi, uint8_t event, struct hci_address_stru *addr);
void hci_security_authorization_cbk_rom(struct hci_acl_link_stru *link, struct hci_security_node_stru *node,
    uint16_t evi);
uint8_t hci_security_check_support_ssp(struct hci_local_stru *local, struct hci_remote_stru *remote);
uint8_t hci_security_check_support_sc(struct hci_local_stru *local, struct hci_remote_stru *remote);
void hci_security_go_rom(struct hci_acl_link_stru *link);
void hci_security_pendding_cbk(struct hci_security_node_stru *node, uint8_t type);
void hci_security_authorization_cbk(struct hci_acl_link_stru *link, struct hci_security_node_stru *node, uint16_t evi);
void hci_security_complete(struct hci_acl_link_stru *link, uint16_t result);
void hci_secu_result_ev_rom(struct hci_local_stru *local, uint16_t hdl, uint16_t mask, uint16_t status);
void hci_security_handle_auth_failed(struct hci_acl_link_stru *link, struct hci_remote_stru *remote, uint16_t status);
void hci_secu_clr_rmt_by_acl_link_rom(struct hci_acl_link_stru *link);
void hci_security_end(struct hci_security_node_stru *node, uint16_t result);
void hcic_secu_req_rom(const struct fsm_inst *fi, uint8_t event,
    struct hci_security_node_stru *in);
void hcic_secu_le_keys_recover_rom(const struct fsm_inst *fi, uint8_t event, struct bth_list_stru *list);
void hcic_secu_pin_code_reply_rom(const struct fsm_inst *fi, uint8_t event, struct hci_pin_code_request_reply_stru *in);
void hcic_secu_pairing_confirm_rom(const struct fsm_inst *fi, uint8_t response, uint8_t *bd);
void hcic_secu_link_key_recover_rom(const struct fsm_inst *fi, uint8_t event,
    struct hci_security_link_key_recover_stru *in);
void hci_ev_link_key_req_rom(struct hci_local_stru *local, struct hci_link_key_request_ev_stru *ev);
void hci_ev_pin_code_req_rom(struct hci_local_stru *local, struct hci_pin_code_request_ev_stru *ev);
void hci_ev_encryption_change_rom(struct hci_local_stru *local, struct hci_encryption_change_ev_stru *ev);
bool hci_ev_io_secu_pair_hdl_rom(struct hci_local_stru *local, struct hci_remote_stru *remote,
                                 void *reply, struct hci_io_capability_request_ev_stru *ev,
                                 struct hci_io_capability_req *capaility_req);
uint8_t hci_security_link_key_good_rom(struct hci_local_stru *local, struct hci_remote_stru *remote,
    struct hci_security_node_stru *node);

uint8_t le_controller_addr_node_list_count(struct hci_local_stru *local, uint8_t type); /* Used in DBG */

void hcic_iso_data_send(const struct fsm_inst *fi, uint8_t event, struct buff_stru *arg);
#ifdef CONFIG_BREDR_HCI
uint8_t hci_get_stream_len_by_filter_type(uint8_t filter_type, uint8_t filter_condition_type);
#endif
uint8_t *hci_stream_to_struct_cpy_stream(const uint8_t *stream, uint8_t len);
void hci_coding_endian(uint8_t **src, uint8_t **dest, uint8_t len);
uint8_t hci_coding_get_max_ele(struct hci_bit_pointer_stru *bp);
struct hci_command_node_stru *hci_command_find_by_context(struct hci_local_stru *local, const void *context);
void list_qos_cbk(struct hci_local_stru *local, uint8_t action);
struct buff_stru *list_qo_s_get(struct bth_list_stru *list);
void list_qo_s_free_buf(struct hci_local_stru *local, struct buff_stru *buf);
uint8_t list_qo_s_add(struct hci_local_stru *local, struct buff_stru *buf, uint8_t mask, uint8_t slot);
void list_qo_s_remove_hdl(struct hci_local_stru *local, uint16_t hdl);
void hci_acl_data_mp_add(hci_mp_int *r, uint32_t len);
bool hci_check_bit(const uint8_t *buf, uint8_t evindex);
void *hci_cmd_new_auth_req(void);
void *hci_cmd_new_set_conn_encrypt(void);
void *hci_cmd_new_pin_code_rply(void);
void *hci_cmd_new_pin_code_neg_rply(void);
void *hci_cmd_new_link_key_neg_rply(void);
void *hci_cmd_new_secu_link_key_ask4(void);
void *hci_cmd_new_link_key_rply(void);
uint8_t bth_hci_recv_acl_data_header_proc(const struct hci_local_stru *local,
    struct buff_stru *in, uint8_t *pb, uint16_t *handle, uint8_t *header_len);

typedef void (HciAclDataSendPenddingCb)(struct hci_local_stru *local, uint8_t slot);
typedef void (HciCmdSendInternalCb)(struct hci_local_stru *local, void *par, void *context, uint32_t opscode);
typedef void (HciCmdRmvErrCb)(struct hci_local_stru *local, struct hci_command_node_stru *cmd, uint16_t status);
typedef void (HciCmdRmvCb)(struct hci_local_stru *local, struct hci_command_node_stru *cmd, void *arg,
                           uint16_t len, uint16_t status, uint16_t evindex);
typedef void (HciCbkCb)(struct hci_local_stru *local, void *context, void *arg, uint32_t len, uint32_t op_ev);
typedef void (HciAutoSniffStopTimerCb)(struct hci_local_stru *local);
typedef void (HciEvRmtSupportedFeatCapCb)(struct hci_local_stru *local, struct hci_remote_stru *remote, uint8_t page);
typedef uint8_t (HciCmdCheckSupportCb)(struct hci_local_stru *local, uint16_t opcode);
typedef void (HciOpReadLocalFeatPage0Cb)(struct hci_local_stru *local, uint8_t *fea);
typedef void (HciLocalChangedNotifyCb)(struct hci_local_stru *local, HANDLE tl_hdl, uint8_t action);
typedef void (HciRemoteGetAddrCb)(struct hci_remote_stru *remote, struct hci_address_stru *addr);
typedef uint8_t (HciSecuChkSupportSSPCb)(struct hci_local_stru *local, struct hci_remote_stru *remote);
typedef uint8_t (HciSecuChkSupportSCCb)(struct hci_local_stru *local, struct hci_remote_stru *remote);
typedef void (HciSecurityPenddingCbkCb)(struct hci_security_node_stru *node, uint8_t type);
typedef void (HciSecuAuthCbkCb)(struct hci_acl_link_stru *link,
    struct hci_security_node_stru *node, uint16_t evi);
typedef void (HciSecuCmpCb)(struct hci_acl_link_stru *link, uint16_t result);
typedef uint8_t (HciSecuLkKeyGoodCb)(struct hci_local_stru *local, struct hci_remote_stru *remote,
    struct hci_security_node_stru *node);
typedef void (HciSecuGoCb)(struct hci_acl_link_stru *link);
typedef void (HciSecuHdlAuthFailedCb)(struct hci_acl_link_stru *link, struct hci_remote_stru *remote, uint16_t status);
typedef void (HciSecuEndCb)(struct hci_security_node_stru *node, uint16_t result);
typedef struct hci_local_stru* (HciLocalFindByIDCb)(uint8_t ampid);
typedef bool (HciCheckBitCb)(uint8_t *buf, uint8_t evindex);
typedef struct hci_remote_stru* (HciRmtFindCreateCb)(uint8_t *bd, uint8_t atype, uint8_t cap);
typedef struct hci_remote_stru* (HciRmtFindByBDCb)(uint8_t *bd);
typedef void (HciSecuResultEvCb)(struct hci_local_stru *local, uint16_t hdl, uint16_t mask, uint16_t status);
typedef void* (HciCmdNewAuthReqCb)(void);
typedef void* (HciCmdNewSetConnEncryptCb)(void);
typedef void* (HciCmdNewPinCodeRplyCb)(void);
typedef void* (HciCmdNewPinCodeNegRplyCb)(void);
typedef void* (HciCmdNewLinkKeyNegRplyCb)(void);
typedef void* (HciCmdNewSecuLinkKeyAsk4Cb)(void);
typedef void* (HciCmdNewLinkKeyRplyCb)(void);
typedef void (HciSecuAuthCb)(struct hci_security_author_req_stru *in);
/* Expend to visit more vars */
typedef void (HciLinkRmvFun)(struct hci_acl_link_stru *link, uint8_t reason);
typedef uint8_t (HciCmpAddrCb)(struct hci_address_stru *s, struct hci_address_stru *r);

#endif /* G_HCI_HLP_H */

