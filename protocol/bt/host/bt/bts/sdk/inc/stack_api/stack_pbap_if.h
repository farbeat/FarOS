/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_HFP_IF_H
#define STATCK_HFP_IF_H

#include "ipc.h"
#include "stack_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************** Macro Definition ******************/
#define BTSDK_PBAP_PULL_ASYNC

#define BTSDK_PBAP_MAX_DELIMITER          0x02

/* Possible values for member 'order' in _BtSdkPBAPParamStru */
#define BTSDK_PBAP_ORDER_INDEXED          0x00
#define BTSDK_PBAP_ORDER_NAME             0x01
#define BTSDK_PBAP_ORDER_PHONETIC         0x02

/* Possible values for member 'search_attrib' in _BtSdkPBAPParamStru */
#define BTSDK_PBAP_SCHATTR_NAME           0x00
#define BTSDK_PBAP_SCHATTR_NUMBER         0x01
#define BTSDK_PBAP_SCHATTR_SOUND          0x02

/* Possible values for member 'format' in _BtSdkPBAPParamStru */
#define BTSDK_PBAP_FMT_VCARD21            0x00
#define BTSDK_PBAP_FMT_VCARD30            0x01

/* Filter bit mask supported by PBAP1.0. Possible values for parameter
'flag' of BTSDK_PBAPFilterComposer. */
#define BTSDK_PBAP_FILTER_VERSION               0x00
#define BTSDK_PBAP_FILTER_FN                    0x01
#define BTSDK_PBAP_FILTER_N                     0x02
#define BTSDK_PBAP_FILTER_PHOTO                 0x03
#define BTSDK_PBAP_FILTER_BDAY                  0x04
#define BTSDK_PBAP_FILTER_ADR                   0x05
#define BTSDK_PBAP_FILTER_LABEL                 0x06
#define BTSDK_PBAP_FILTER_TEL                   0x07
#define BTSDK_PBAP_FILTER_EMAIL                 0x08
#define BTSDK_PBAP_FILTER_MAILER                0x09
#define BTSDK_PBAP_FILTER_TZ                    0x0A
#define BTSDK_PBAP_FILTER_GEO                   0x0B
#define BTSDK_PBAP_FILTER_TITLE                 0x0C
#define BTSDK_PBAP_FILTER_ROLE                  0x0D
#define BTSDK_PBAP_FILTER_LOGO                  0x0E
#define BTSDK_PBAP_FILTER_AGENT                 0x0F
#define BTSDK_PBAP_FILTER_ORG                   0x10
#define BTSDK_PBAP_FILTER_NOTE                  0x11
#define BTSDK_PBAP_FILTER_REV                   0x12
#define BTSDK_PBAP_FILTER_SOUND                 0x13
#define BTSDK_PBAP_FILTER_URL                   0x14
#define BTSDK_PBAP_FILTER_UID                   0x15
#define BTSDK_PBAP_FILTER_KEY                   0x16
#define BTSDK_PBAP_FILTER_NICKNAME              0x17
#define BTSDK_PBAP_FILTER_CATEGORIES            0x18
#define BTSDK_PBAP_FILTER_PROID                 0x19
#define BTSDK_PBAP_FILTER_CLASS                 0x1A
#define BTSDK_PBAP_FILTER_SORT_STRING           0x1B
#define BTSDK_PBAP_FILTER_X_IRMC_CALL_DATETIME  0x1C
#define BTSDK_PBAP_FILTER_PROPRIETARY_FILTER    0x27
#define BTSDK_PBAP_FILTER_INVALID               0xFF

/* Possible flags for member 'repositories' in _BtSdkLocalPSEServerAttrStru,
_BtSdkRmtPSESvcAttrStru */
#define BTSDK_PBAP_REPO_LOCAL   0x01
#define BTSDK_PBAP_REPO_SIM     0x02

/* Possible flags for member 'mask' in _BtSdkPBAPParamStru */
#define BTSDK_PBAP_PM_ORDER       0x0001
#define BTSDK_PBAP_PM_SCHVALUE    0x0002
#define BTSDK_PBAP_PM_SCHATTR     0x0004
#define BTSDK_PBAP_PM_MAXCOUNT    0x0008
#define BTSDK_PBAP_PM_LISTOFFSET  0x0010
#define BTSDK_PBAP_PM_FILTER      0x0020
#define BTSDK_PBAP_PM_FORMAT      0x0040
#define BTSDK_PBAP_PM_PBSIZE      0x0080
#define BTSDK_PBAP_PM_MISSEDCALLS 0x0100

#define BTSDK_PBAP_FILTER_LEN 8

/*************** Structure Definition ******************/
typedef struct {
    uint32_t size;
    uint16_t mask;
    uint8_t  root_dir[BTSDK_PATH_MAXLENGTH + 1];
    uint8_t  path_delimiter[BTSDK_PBAP_MAX_DELIMITER + 1];
    uint8_t  repositories;
} bt_sdk_local_pse_server_attr_stru, *p_bt_sdk_local_pse_server_attr_stru;

typedef struct {
    uint32_t size;
    uint16_t mask;
    uint8_t  repositories;
} bt_sdk_rmt_pse_svc_attr_stru, *p_bt_sdk_rmt_pse_svc_attr_stru;

typedef struct {
    uint16_t mask;
    uint8_t  filter[BTSDK_PBAP_FILTER_LEN];
    uint16_t max_count;
    uint16_t list_offset;
    uint8_t  order;
    uint8_t  format;
    uint8_t  *search_val;
    uint8_t  search_attrib;
    uint8_t  missed_calls;
    uint16_t pb_size;
} bt_sdk_pbap_param_stru, *p_bt_sdk_pbap_param_stru;

typedef struct {
    uint8_t first;
    uint8_t last;
    uint8_t *filename;
    uint32_t filesize;
    uint32_t cursize;
} bt_sdk_pbap_status_ev_arg; /* for event btsdk_pbap_ev_status use */

typedef struct {
    uint8_t first;
    uint8_t last;
    uint32_t filesize;
    uint32_t cursize;
    uint8_t filename[1];
} bt_ipc_pbap_status_ev_arg; /* for event btsdk_pbap_ev_status use to send ipc msg */

typedef enum {
    BTSDK_PBAP_EV_STATUS, /* notify app the progress of downloading */
    BTSDK_PBAP_EV_PULLPHONEBOOK, /* the order of pull parts match the PBAP_EventType parts */
    BTSDK_PBAP_EV_PULLVCARDLIST,
    BTSDK_PBAP_EV_PULLVCAR
} bt_sdk_pbap_event_type;

/* PBAP event */
typedef enum {
    PBAP_EV_APP_EVENT,
    PBAP_EV_WRITE_FILE,
} pbap_ev;

/* Used by ipc message b to a */
typedef struct {
    uintptr_t hdl;   /* Handle to the remote device. */
    uint32_t len;
    uint8_t  buffer[0];
} bt_pbap_ipc_msg_param_stru, *pbt_pbap_ipc_msg_param_stru;

/* Used by ipc message b to a */
typedef struct {
    uint32_t event;
    uint32_t result;
    uint8_t arg[0];
} bt_pbap_ipc_app_param_stru, *pbt_pbap_ipc_app_param_stru;

typedef void (btsdk_event_callback_func)(bt_sdk_pbap_event_type event, uint32_t result, void *arg);
typedef void (btsdk_write_file_func)(uint32_t file_hdl, uint8_t *buf, uint32_t len);

typedef struct {
    btsdk_event_callback_func   *event_cbk;
    btsdk_write_file_func       *write_func;
} sdk_pce_inst_stru;

int32_t btsdk_pbap_pull_phone_book(
    uint32_t conn_hdl, const char *path, uint32_t path_len, p_bt_sdk_pbap_param_stru param, uintptr_t file_hdl);
int32_t btsdk_pbap_pull_card_list(
    uint32_t conn_hdl, const char *path, uint32_t path_len, p_bt_sdk_pbap_param_stru param, uintptr_t file_hdl);
int32_t btsdk_pbap_pull_card_entry(
    uint32_t conn_hdl, const char *path, uint32_t path_len, p_bt_sdk_pbap_param_stru param, uintptr_t file_hdl);
int32_t btsdk_pbap_set_path(uint32_t conn_hdl, const char* path, uint32_t path_len);
int32_t btsdk_pbap_cancel_transfer(uint32_t conn_hdl);
void btsdk_pbap_reg_cbk(sdk_pce_inst_stru *pbap_cbk);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_HFP_IF_H */
