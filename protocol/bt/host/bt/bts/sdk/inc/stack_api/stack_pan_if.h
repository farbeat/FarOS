/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: stack pan interface
 */
#ifndef STATCK_PAN_IF_H
#define STATCK_PAN_IF_H

#include "ipc.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef void (*btsdk_pan_rpt_net_state)(uint8_t net_state);
typedef void (*btsdk_pan_rpt_filter_result)(uint8_t ipv4_only);
typedef void (*btsdk_pan_rpt_ethernet_packet)(uint8_t *payload, uint16_t payload_length);
typedef struct {
    btsdk_pan_rpt_net_state net_state_cbk;
    btsdk_pan_rpt_filter_result filter_result_cbk;
    btsdk_pan_rpt_ethernet_packet ethernet_cbk;
} btsdkpan_cbk;

int32_t btsdk_pan_write_filter_ipv4only(uint8_t *addr);
int32_t btsdk_pan_write_ethernet(uint8_t *addr, uint8_t *ip_packet, uint16_t packet_length);
void btsdk_pan_register_callback(const btsdkpan_cbk *callback);
int32_t pan_register_cbk(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_AV_IF_H */