/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_SPP_IF_H
#define STATCK_SPP_IF_H
#include "ipc.h"
#include "mpc_dispatch.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************** Macro Definition ******************/
#define BTSDK_SPP_SVRCHNL_MIN 0
#define BTSDK_SPP_SVRCHNL_MAX 30

typedef struct {
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t  data4[8];
} bt_sdk_uuid_stru, *p_bt_sdk_uuid_stru;


/*************** Structure Definition ******************/
typedef struct _bt_sdk_app_ext_spp_attr_stru {
    uint32_t      size;            /* Size of this structure */
    uint32_t      sdp_record_handle; /* 32bit interger specifies the SDP service record handle */
    bt_sdk_uuid_stru service_class128; /* 128bit UUID specifies the service class of this service record */
    uint8_t       svc_name[BTSDK_SERVICENAME_MAXLENGTH]; /* Service name, in UTF-8 */
    uint8_t       rf_svr_chnl;       /* RFCOMM server channel assigned to this service record */
} bt_sdk_app_ext_spp_attr_stru, *p_bt_sdk_app_ext_spp_attr_stru;

/* Used by ipc message b to a */
typedef struct {
    uint32_t conn_hdl;
    uint32_t event_type;
    uint16_t data_len;
    uint8_t data[0];
} bt_spp_ipc_app_param_stru, *pbt_spp_ipc_app_param_stru;

typedef enum {
    BTSDK_SPP_EVT_CONNECTED,
    BTSDK_SPP_EVT_DISCONNECTED,
    BTSDK_SPP_EVT_BYTES_RECEIVED, /* Bytes received from peer. parameter: NULL */
    BTSDK_SPP_EVT_FC_ON           /* Flow control ON, that is the APP is allowed to write stream. parameter: NULL */
} bt_sdk_spp_event_type;

typedef void (btsdk_spp_event_cb)(uint32_t hdl, bt_sdk_spp_event_type event, uint8_t *data, uint16_t data_len);

uint32_t btsdk_spp_read_stream(uint32_t conn_hdl, uint8_t *data, uint32_t data_size, uint32_t *bytes_read);
uint32_t btsdk_spp_write_stream(uint32_t conn_hdl, uint8_t *data, uint32_t data_size, uint32_t *bytes_written);
uint32_t btsdk_register_spp_service(uint16_t svr_chnl, const char *svc_name, uint32_t len);
uint32_t btsdk_unregister_spp_service(uint32_t svc_hdl);
void btsdk_spp_register_callback(btsdk_spp_event_cb *app_event_cbk);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_SPP_IF_H */
