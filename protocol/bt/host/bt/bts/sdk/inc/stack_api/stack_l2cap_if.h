/*
 * Copyright (c) @CompanyNameMagicTag 2023. All rights reserved.
 * Description: stack l2cap interface
 */
#ifndef STATCK_L2CAP_IF_H
#define STATCK_L2CAP_IF_H

#include "ipc.h"
#include "datatype.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

struct buff_stru {
    uint16_t len; /* length of whole buffer, including data before off */
    uint16_t off;
    uint8_t data[0];
};

enum { /* L2CAP Configuartion option type, used internal */
    L2CAP_COT_MTU =             0x01,
    L2CAP_COT_FLUSHTIMEOUT =     0x02,
    L2CAP_COT_QOS =             0x03,
    L2CAP_COT_RTFC =             0x04,
    L2CAP_COT_FCS =             0x05,
    /* ExtFlowSpec is used instead of FlushTO and QoS when both devices support ExtFLowSpec. */
    L2CAP_COT_EXTFLOWSPEC =     0x06,
    L2CAP_COT_EXTWINSIZE =         0x07 /* Notice: this is the last option we knows... */
};

/* L2CAP Configuration option type mask for upper protocol */
#define L2CAP_COTMASK_MTU                            (1 << (L2CAP_COT_MTU - 1))

typedef void (l2cap_cbk_func)(uint16_t msg, void *arg); /* callback to upper protocol */

struct hci_address_stru {
    uint8_t bd[BD_ADDR_LEN];                        /* 6octets, 48bits */
    /* etc, HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS = HCI_PEER_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS */
    uint8_t atype;
    uint8_t cap;                                    /* etc, HCI_ADDRESS_CAP_LE, reserved for compatibility only. */
};

struct l2cap_echo_stru { /* Req/Cfm */
    /* Req/Cfm/Ind/Rsp common */
    struct hci_address_stru addr; /* Remote Addr */
    uint8_t *data;                 /* [optional] optional 'data' field, need free in L2CAP */
    uint16_t data_len;             /* [optional] length of the optional 'data' field */

    /* Req/Cfm common */
    void *user_data;             /* [optional] user specific data */

    /* Req specific */
    l2cap_cbk_func *cbk;          /* echo requester must assign a 'cbk', to deal with the cfm msg */

    /* Cfm specific */
    /* cfm result. eg: ER_L2CAP_SUCCESS/ER_L2CAP_RTXTIMEOUT/ER_L2CAP_ACLLINK_UNAVAILABLE */
    uint16_t result;

    /* Rsp specific */
    uint8_t id;                    /* l2cap 'Identifier' field, used by external echo rsp */
};

struct l2cap_rt_fc_stru {
    uint16_t     txwindow_size; /* modified to 2bytes as to fit extended_window_size 14 bits */
    uint16_t     retrans_timeout;
    uint16_t     monitor_timeout;
    uint16_t     max_pdusize;
    /* if this is TRUE, input mode shall be used, otherwise disconnect if this is FALSE,
       input mode could be changed during config */
    uint8_t    force_flag;
    uint8_t     mode; /* etc, L2CAP_MODE_BASIC */
    uint8_t     max_transmit;
};

struct l2cap_ext_fc_stru {
    uint32_t     sdu_interarrival_time; /* BE:May increase only, GUAR:None */
    uint32_t     access_latency; /* BE:None changes permitted by responder, GUAR:May decrease only */
    /* BE:None changes permitted by responder, GUAR:May decrease only (unless set to 0xFFFFFFFF,
       in which case no change is permitted) */
    uint32_t     flush_timeout;
    uint16_t     max_sdu; /* BE:May decrease only, GUAR:None */
    uint8_t     identifier; /* The Identifier shall be unique within the scope of a physical link */
    uint8_t     service_type; /* etc, L2CAP_EFS_SERVICETYPE_NOTRAFFIC */
};

struct l2cap_config_option_stru {
    /* type = 0x04 RETRANSMISSION AND FLOW CONTROL OPTION, type = 0x06 EXTENDED WINDOW SIZE OPTION */
    struct l2cap_rt_fc_stru     rt_fc;
    struct l2cap_ext_fc_stru     ext_flow;     /* type = 0x05 EXTENDED FLOW SPECIFICATION OPTION */
    /* type = 0x01 MAXIMUM TRANSMISSION UNIT (MTU), exclude any l2cap headers */
    uint16_t     in_mtu;
    uint16_t     mask;                        /* etc: L2CAP_COTMASK_MTU, which option is used or not */
    uint16_t     hint;                        /* etc: L2CAP_COTMASK_MTU, if this option is a hint or not */
    uint16_t     initial_credits;            /* for LE */
    uint16_t     le_mps;                        /* for LE, MPS */
    /* etc: L2CAP_FCS_16BITFCS, type = 0x07 FRAME CHECK SEQUENCE (FCS) OPTION */
    uint8_t     fcs_type;
};

struct l2cap_connect_stru {
    uint16_t cid;                        /* Local CID */
    uint16_t psm;                        /* Single PSM, the connection target PSM */
    uint16_t result;                        /* Only for rsp or cfm */
    uint16_t status;                        /* Only for rsp, result is pendding should fill this status */
    /* e.g L2CAP_CONNECTMASK_LE_DESIRED, hci_available_addr_cap shall be used to check */
    uint8_t amp_mask;
    uint8_t *cbk;                        /* Upper callback, the owner of this CID object */
    uint8_t psm_mask;                 /* etc, L2CAP_CBK_FIXED_PSM, 5 types */
    uint8_t resv;
    struct hci_address_stru addr;        /* Remote Addr */
    /* Only for connect req, rsp, input config_req param before config procedure */
    struct l2cap_config_option_stru config;
    void *_rom;
};

struct l2cap_disconnect_req_stru { /* When local CID could not find, nothing happened in new l2cap */
    uint16_t     cid; /* Local CID */
    uint16_t     psm;
};

void l2cap_reg_cbk(uint16_t psm, uint8_t *cb, uint8_t mask);
void l2cap_connect_req(struct l2cap_connect_stru *in);
void l2cap_data_req(struct buff_stru *in, uint32_t hdlcid);
void l2cap_echo_req(struct l2cap_echo_stru *in, BOOL ext_call);
void l2cap_disconnect_req(struct l2cap_disconnect_req_stru *in);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_L2CAP_IF_H */
