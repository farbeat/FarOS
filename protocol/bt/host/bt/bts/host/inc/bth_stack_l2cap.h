/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: bth mpc stack struct for l2cap
 * Author: @CompanyNameTag
 * Create: 2022-03-01
 */
#ifndef BTH_STACK_L2CAP_H
#define BTH_STACK_L2CAP_H

#include <stdint.h>
#include "bth_stack.h"

/* General Interface for Stack L2CAP */
enum {
    BTH_CH_LOCAL = 0,
    BTH_CH_REMOTE = 1,
    BTH_CH_NUM,
};
enum {
    BTH_L2CAP_FIXED_PSM = 0x00,
    BTH_L2CAP_FIXED_LEPSM = 0x01,
    BTH_L2CAP_DYNAMIC_PSM = 0x02,
    BTH_L2CAP_DYNAMIC_LEPSM = 0x03,

    BTH_L2CAP_FIXED_LECID = 0x04,
    BTH_L2CAP_FIXED_CID = 0x05,

    BTH_L2CAP_ECHO = 0x09 /* Only for EchoReq, internal use only */
};
enum {
    BTH_L2CAP_CONN_CFM = 0x00,
    BTH_L2CAP_CONN_COMPLETE,
    BTH_L2CAP_CONN_INDICATION,
};

#pragma pack (1)
typedef struct {
    uint16_t psm; /* Single PSM, the connection target PSM */
    uint16_t cid;  /* Local CID */
    uint8_t channel_type; /* enum:l2cap_channel_type */
    uint16_t mps;
    uint16_t mtu;
    uint16_t init_credits;
    void *cbk;
} bth_l2cap_psm_node;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint16_t psm;
    uint16_t cid[BTH_CH_NUM];
    uint16_t mps[BTH_CH_NUM];
    uint16_t mtu[BTH_CH_NUM];
    uint16_t init_credits;
    uint16_t conn_hdl;
    uint8_t channel_type; /* enum:l2cap_channel_type */
    uint8_t role;
    uint8_t status;
} bth_l2cap_credit_conn;

typedef struct {
    bth_l2cap_credit_conn conn;
    uint16_t sub_ev;
    uint16_t result;
} bth_l2cap_credit_conn_ev;

typedef struct {
    bth_address addr; /* remote addr */
    uint16_t psm;
    uint16_t cid;
    /*
     * if the command is sent by the application to the peer device,
     * credits must be set to a valid value. if credits is set to 0,
     * the peer device cannot send data streams to the local device.
     */
    uint16_t credits;
    /*
     * if the command is sent by the peer device to the local application,
     * r_credits is set to a valid value. if r_credits is set to 0,
     * the application cannot send data streams to the local and peer devices.
     */
    uint16_t r_credits;
} bth_l2cap_credit_flow_cntl;

typedef struct {
    bth_address addr;
    uint16_t cid;
    uint16_t conn_handle;
    bth_data_t data;
} bth_l2cap_credit_conn_rx;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint16_t psm;
    uint16_t cid; /* local cid */
    uint16_t result;
} bth_l2cap_disconnect;

typedef struct {
    bth_address addr;
    uint16_t psm;
    uint16_t cid;
    bth_data_t data;
} bth_l2cap_ch_tx;

typedef struct {
    uint8_t l2cap_key_size;
    uint8_t l2cap_item_need_auth_check;
} bth_l2cap_config_param;
#pragma pack ()
/* General Interface for Stack L2CAP */


#endif /* BTH_STACK_L2CAP_H */
