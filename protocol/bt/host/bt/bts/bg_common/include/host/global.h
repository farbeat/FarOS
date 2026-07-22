/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    global.h
Abstract:
    This file includes the definition of global data and definitions.
--------------------------------------------------------------------------- */
#ifndef GLOBAL_H
#define GLOBAL_H

#include "sysdep.h"
#include "debug.h"
#include "btlist.h"
#include "fsm.h"
#include "hdlmgnt.h"
#include "buff.h"
#include "bttask.h"
#include "bt_log.h"
#include "bt_logn.h"
#ifdef DEBUG_ON_PC
#include "securec_win32.h"
#else
#include "securec.h"
#endif
#if CHIP_SUSONG
#include "avdtp_ui.h"
#endif

#define PROT_HCI    1
#define PROT_L2CAP    2
#define PROT_SDP    3
#define PROT_RFCOMM    4
#define PROT_ATT    5
#define PROT_SPP    6
#define PROT_OBEX   7
#define PROT_BNEP   8
#define PROT_HCRP    9
#define PROT_PAN   10
#define PROT_HID     11
#define PROT_SDK     12
#define PROT_AVCTP     13
#define PROT_AVDTP     14
#define PROT_BIP     15
#define PROT_AVRCP   16
#define PROT_A2DP      17
#define PROT_SMP      18
#define PROT_PBAP     19
#define PROT_FTP     20
#define PROT_OPP     21
#define PROT_SYNC     22
#define PROT_GAP     23
#define PROT_SDAP     24
#define PROT_HFP     25
#define PROT_MAP     26

#define MOD_FSM        0
#define MOD_SYSDEP    0
#define MOD_HANDLE    0
#define MOD_LIST    0
#define MOD_BUFF    0
#define MOD_GLOBAL  0


#define MAX_DEFINE_PER_PROTOCOL        100


#define PROT_HCI_BASE        ((PROT_HCI-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_L2CAP_BASE        ((PROT_L2CAP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_SDP_BASE        ((PROT_SDP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_RFCOMM_BASE    ((PROT_RFCOMM-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_SPP_BASE        ((PROT_SPP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_OBEX_BASE        ((PROT_OBEX-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_BNEP_BASE        ((PROT_BNEP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_PAN_BASE        ((PROT_PAN-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_HCRP_BASE        ((PROT_HCRP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_HID_BASE        ((PROT_HID-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_AVCTP_BASE        ((PROT_AVCTP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_AVDTP_BASE        ((PROT_AVDTP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_BIP_BASE        ((PROT_BIP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_A2DP_BASE        ((PROT_A2DP-1)*MAX_DEFINE_PER_PROTOCOL)
#define PROT_HFP_BASE        ((PROT_HFP-1)*MAX_DEFINE_PER_PROTOCOL)

#define L2CAP_CO_RESERVE    4

#define BTH_WITH_SMART_WEAR_YES 1
#define BTH_WITH_SMART_WEAR_NO  0

/*
 * Global variables
*/
#ifndef CONFIG_OS_NONE
extern HANDLE g_quit_ev;
extern SYSTEM_LOCK g_localinfo_lock;

#define ENTER_LOCALINFO_LOCK        enter_critical(g_localinfo_lock)
#define LEAVE_LOCALINFO_LOCK        leave_critical(g_localinfo_lock)
#else
#define ENTER_LOCALINFO_LOCK
#define LEAVE_LOCALINFO_LOCK
#endif

/* free_acl_data( may be mapped to the recycle function of lower HCI layer in case of SOC */
#define free_acl_data(d) BUFF_FREE(d)

typedef struct ram_cb_fun_stru_ {
    uint8_t id;
    void *func; /* call back function */
} ram_cb_fun_stru;

enum {
    MEM_DELETE,
    BUFF_DELETE,
    BUFF_ALLOC,
    PRI_BUFF_NEW,
    MEM_ALLOC,
    MEM_NEW_AND_SET_NEW0_,
    MEM_NEW_AND_SET_NEW0,
    LIST_NODE_NEW,
    LIST_S_NODE_NEW,
    L2CAP_UI_DISC_REQ,
#ifdef CONFIG_AVDTP
    AVDTP_STREAM_START_ACCEPT,
#endif
    L2CAP_DATA_REQ,
    L2CAP_CONN_REQ,
    L2CAP_CFG_RSP,
    L2CAP_DISC_REQ,
    HCI_CBK_EVT,
    HCI_CMD_SEND_INTER,
    IS_FACTORY_MODE,
#ifdef CONFIG_BREDR_HCI
    HCI_SECU_RESULT_EVT,
    HCI_CMD_NEW_PIN_REPLY,
    HCI_RMT_FD_BY_BD,
    HCI_SECU_COMPL,
    HCI_CMD_NEW_LKKEY_REPLY,
    HCI_CMD_NEW_SECU_LKKEY_ASK4,
    HCI_CMD_NEW_PIN_NEG_REPLY,
    HCI_CMD_NEW_LKKEY_NEG_REPLY,
    HCI_RMT_GET_ADDR,
    HCI_SECU_CHECK_SUPP_SSP,
    HCI_SECU_CHECK_SUPP_SC,
    HCI_SECU_PENDING_CB,
    HCI_SECU_AUTH_CB,
    HCI_SECU_LKKEY_GOOD,
    HCI_SECU_GO,
    HCI_SECU_HDL_AUTH_FAIL,
    HCI_SECU_END,
#endif
    HCI_LOCAL_FD_BY_ID,
    HCI_CHECK_BIT,
    HCI_RMT_FD_CREATE,
#ifdef CONFIG_BREDR_HCI
    HCI_CMD_NEW_AUTH_REQ,
    HCI_CMD_NEW_SET_CONN_ENCRYPT,
#endif
    FSM_DEL_TIMER,
    HCI_LINK_RMV,
    ACL_DATA_SEND_PENDING,
    HCI_CMD_RMV_ERR,
#ifdef CONFIG_BREDR_HCI
    HCI_CMD_RMV,
    HCI_AUTO_SNIFF_STOP_TIMER,
    HCI_EV_RMT_SUPP_FEAT_CAP,
#endif
    HCI_CMD_CHECK_SUPP,
#ifdef CONFIG_BREDR_HCI
    HCI_OP_READ_LOCAL_FEAT_PAGE0,
#endif
    HCI_LOCAL_CHG_NOTIFY,
#ifdef CONFIG_RFCOMM
    RFCOM_CONN_EVT_CBK,
    RFCOM_SECU_REQ,
    RFCOM_SEND_SIG,
    RFCOM_DLCI0_DONE,
    RFCOM_AUTO_CREDIT,
    RFCOM_EVT_CBK,
    RFCOM_DLCIX_SEND_PN,
    RFCOM_DLCIX_DONE,
    RFCOM_DLCI0_DISP_DLCIX_DONE,
    RFCOM_TIMER_END,
    RFCOM_TIMER_FD,
    RFCOM_DLCI0_DISP_SEND_PN,
    RFCOM_DLCIX_FD_BY_DDLCI,
    RFCOM_DLCIX_RD_MSC,
    RFCOM_SEND_UIH_FC,
    RFCOM_DLCIX_DISC_REQ,
    RFCOM_SEND_UIH_RLS,
    RFCOM_DLCIX_CREATE_BY_SRV,
    RFCOM_SEND_UIH_PN,
    RFCOM_SEND_UIH_MSC,
    RFCOM_DLCIX_UIH_MSC_DONE,
    RFCOM_SEND_UIH_RPN,
    RFCOM_UIH_SIG_DFT_HDL,
    RFCOM_UIH_SIG_GNERAL,
    RFCOM_SEND_UIH_TEST,
    RFCOM_UIH_TEST_CFM,
    RFCOM_UIH_FC_IND,
    RFCOM_UIH_NSC_CFM,
    RFCOM_UIH_UNKNOWN,
#endif
    L2CAP_UNREG_CBK,
#ifdef CONFIG_RFCOMM
    RFCOM_SRVDLC_FD_BY_DLCI,
#endif
    L2CAP_GET_HCI_HDL,
#ifdef CONFIG_RFCOMM
    RFCOM_L2CAP_CB,
    RFCOM_DLCI0_FD_BY_BD,
    RFCOM_L2CAP_SET_CFG_REQ,
    RFCOM_DLCI0_INIT,
    RFCOM_DLCIX_INIT,
    RFCOM_FAIL_CBK,
    RFCOM_DLCI0_FD_BY_CID,
    RFCOM_CALC_FCS,
    RFCOM_SABM_IND,
    RFCOM_UA_IND,
    RFCOM_DM_IND,
    RFCOM_DISC_IND,
    RFCOM_UIH_SIG,
    RFCOM_DLCIX_UIH_DATA_IND,
    RFCOM_DLCIX_UIH_DATA_REQ,
    RFCOM_TIMER_START,
#endif
    L2CAP_CONN_RSP,
    FSM_INST_FREE,
    L2CAP_DATA_SEND,
    L2CAP_CFG_OPT_MODE,
    L2CAP_SIG_NEW,
    L2CAP_SIG_NEW_LE,
    L2CAP_SECU_REQ,
    L2CAP_DISC_REQ_HLP,
    L2CAP_CONN_RSP_HLP,
    L2CAP_CID_DONE,
    L2CAP_ACL_FD_BY_ADDR,
    L2CAP_CID_FD_BY_LOCAL_CID,
    L2CAP_EVT_CBK,
    L2CAP_PSM_FD_MASK,
    L2CAP_CID_DISC_DONE,
    L2CAP_ACL_DONE,
    L2CAP_DATA_OPEN,
#ifdef CONFIG_BREDR_L2CAP
    L2CAP_CFG_OPT_MODE_CHK,
    L2CAP_CFG_OPT_DECODE_RFC_MD,
    L2CAP_CFG_OPT_DECODE,
#endif
    L2CAP_CFG_CHK_COMPLT,
#ifdef CONFIG_BREDR_L2CAP
    L2CAP_CFG_REQ,
    L2CAP_CFG_OPT_LEN,
    L2CAP_CFG_OPT_FILL,
#endif
    L2CAP_ACL_SEND,
    L2CAP_DATA_STOP,
#ifdef CONFIG_BREDR_L2CAP
    L2CAP_CFG_RSP_CORE,
#endif
    L2CAP_CMD_REJ_RSP,
    L2CAP_SIG_TRANS_FD,
    L2CAP_LE_CONN_PAR_CHK,
    HCI_CMD_SEND_FUNC,
    L2CAP_SECU_CBK2CID_GEN_RESULT,
    L2CAP_INFO_BOX,
    L2CAP_SECU_HCI_CBK,
    L2CAP_LE_FLOW_CTRL_CREDIT_REQ,
    SMP_CMP_ADDR,
    BTH_STACK_CHR_ERR,
    FSM_ADD_TIMER_CX,
    FSM_EVT_FUN_INTER,
#ifdef CONFIG_BREDR_HCI
    HCI_SCO_DATA_SEND,
#endif
    HCI_CBK_REG,
    HCI_CBK_UNREG,
    HCI_CMP_ADDR,
    HCI_RMT_FD_BY_ADDR,
    FSM_RESTART_TIMER_1,
    FSM_SET_RANDOM,
    FSM_RANDOM_CB,
#ifdef CONFIG_A2DP
    A2DP_STREAM_HDL_DONE,
#endif
#ifdef CONFIG_AVDTP
    AVCTP_SESSION_DISC,
    AVCTP_DATA_SEND,
    AVCTPC_INIT,
    FSM_EVT_FUNC_KX,
    FSM_EVT_FUNC_CX,
    AVDTP_SEP_SELEC,
#endif
    SET_EVENT_CB,
    WAIT_FOR_SINGLE_OBJ,
    FSM_INST_NEW,
    L2CAP_REG_CBK,
    ENTER_LOCAL_INFO_LOCK,
    LEAVE_LOCAL_INFO_LOCK,
    BUF_RESERVE,
    ENTER_CRITICAL_FUNC,
    LEAVE_CRITICAL_FUNC,
    CREATE_EVT_OBJ,
    CLOSE_EVT_OBJ,
    CREATE_CRITICAL_OBJ,
    UTF8_TO_WIDE,
    WIDE_TO_UTF8,
    IS_BTH_WITH_SMART_WEAR,
    FSM_ADD_TIMER_LITE,
    FSM_DEL_TIMER2,
    FSM_EVT_FUNC_EX,
    FREE_CRITICAL_CB,
#if CHIP_SUSONG
    AVDTP_SEP_GROUP_MASK_CLR,
    AVDTP_SEP_GROUP_REMOVE,
#endif
    RAM_CB_FUNC_MAX /* no more item behind this */
};

extern ram_cb_fun_stru g_ram_cb_fun[RAM_CB_FUNC_MAX];
void mem_frees(void* p);
void buff_free_func(void* p);
void *buff_new_func(unsigned short size, uint32_t off);
void *buff_new_s_func(unsigned short size, uint32_t off);
struct buff_stru* pri_buf_new_func(uint16_t size, uint16_t off, uint8_t pri);
void *mem_news(int size);
void* new0fun_(int size);
void* new0fun(int size);
void *list_node_new_func(int size);
void *list_s_node_new_func(int size);
void fsm_del_timer(struct fsm_timer **ft);
void fsm_event_func_internal_func(struct fsm_inst *fi, void *func, void *arg);
struct fsm_timer *fsm_add_timer_func(struct fsm_inst *fi, uint32_t millisec, uint8_t event, void *func,
    void *arg, const char *name, uint32_t u_id);
struct fsm_timer *fsm_add_timer_lite(struct fsm_inst *fi, uint32_t millisec, uint8_t event,
    void *arg, const char *name, uint32_t id);
void bth_stack_chr_err(uint8_t info2, uint8_t info3);
struct fsm_timer *fsm_restart_timer(struct fsm_timer *ft, struct fsm_inst *fi, uint32_t millisec, uint8_t event,
    void *func, void *arg, void *name, uint32_t u_id);
uint32_t fsm_random_fun(uint32_t s);
struct fsm_inst *fsm_inst_new_fun(void *user_data, const char *name);
uint8_t *fsm_del_timer2func(struct fsm_timer *in_ft);

void enter_local_info_lock(void);
void leave_local_info_lock(void);
bool is_bth_with_smart_wear(void);
void fsm_evt_func_ex(struct fsm_inst *fi, void *func, void *arg);
void bth_patch_init(void);

typedef void (MemFreeCbFunc)(void* p);
typedef void (BuffFreeCb)(void* p);
typedef void* (BuffNewCb)(int size, uint32_t off);
typedef struct buff_stru* (PriBufNewCb)(uint16_t size, uint16_t off, uint8_t pri);
typedef void* (MemNewCbFunc)(int size);
typedef void* (New0FunCb_)(int size);
typedef void* (New0FunCb)(int size);
typedef void* (ListNodeNewCb)(int size);
typedef void* (ListSNodeNewCb)(int size);
typedef void (FsmDelTimerCb)(struct fsm_timer **ft);
typedef void (FsmEventFuncInternalCb)(struct fsm_inst *fi, void *func, void *arg);
typedef struct fsm_timer* (FsmAddTimerCb)(struct fsm_inst *fi, uint32_t millisec, uint8_t event, void *func,
    void *arg, const char *name, uint32_t u_id);
typedef void (BthStackChrErrCb)(uint8_t info2, uint8_t info3);
typedef void (FsmInstFreeCb)(struct fsm_inst *fi);
typedef struct fsm_timer* (FsmRestartTimerCb)(struct fsm_timer *ft, struct fsm_inst *fi, uint32_t millisec,
    uint8_t event, void *func, void *arg, void *name, uint32_t u_id);
typedef struct fsm_inst* (FsmInstNewCb)(void *user_data, const char *name);
typedef void (EnterLocalInfoLockCb)(void);
typedef void (LeaveLocalInfoLockCb)(void);
typedef uint8_t (BuffResCb)(struct buff_stru *buf, int16_t delta);
typedef void (EnterCriticalCb)(SYSTEM_LOCK mylock);
typedef void (LeaveCriticalCb)(SYSTEM_LOCK mylock);
typedef HANDLE (CreateEvtCb)(LPCSTR name);
typedef void (CloseEvtCb)(HANDLE myev);
typedef SYSTEM_LOCK (CreateCriticalCb)(LPCSTR name);
typedef bool (IsBthWithSmartWearCb)(void);
typedef struct fsm_timer* (FsmAddTimerLiteCb)(struct fsm_inst *fi, uint32_t millisec, uint8_t event,
    void *arg, const char *name, uint32_t id);
typedef uint8_t *(FsmDelTimer2Cb)(struct fsm_timer *in_ft);
typedef void (FsmEvtFuncExCb)(struct fsm_inst *fi, void *func, void *arg);
#if CHIP_SUSONG
typedef void (AvdtpSepGroupMaskClear)(struct avdtp_sep_stru *in, uint8_t mask);
typedef void (AvdtpSepGroupRemove)(struct avdtp_sep_stru *in);
#endif
#endif

