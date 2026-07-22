/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: mpc trans task
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef __MPC_TRANS_H_
#define __MPC_TRANS_H_

#include <soc_osal.h>
#include "custdefs.h"
#include "ipc_actions.h"
#include "platform_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define ACORE_MAX_PKT_RETRY_CNT  3
#define ACORE_IPC_SEND_PKT_TIMEOUT  5
#define WRITE_PROPERTY_ERR_TIMES 100
#define BT_ACORE_TASK_QUEUE_SIZE 500
typedef enum {
    BT_PROP_ACORE_INITIALIZE = 0xC00,
    BT_PROP_ACORE_RX_PACKET  = 0xC01,
    BT_PROP_ACORE_TX_PACKET  = 0xC02,
    BT_PROP_ACORE_MSG_CHECK  = 0xC03,
    BT_PROPERTY_NUM /* NOTE: Add your own property above this item,total number of properties */
} bt_property_id;

typedef void (*bt_propertycallback)(void *buff);

typedef union {
    uint32_t value;
    void* data;
} bt_property_value;

typedef struct {
    bt_property_id id;
    bt_property_value data;
    bt_propertycallback func;
} bt_property;
bool bt_acore_task_fmk_write_property(bt_property_id id, bt_property_value *data);
void bt_acore_tran_task_rx_process_pkt(void *data, uint16_t data_len);
bool bt_acore_ipc_receive_data(ipc_action_t message,
                               const volatile ipc_payload *payload_p,
                               cores_t src,
                               uint32_t id);
void bt_acore_task_main(void const *params);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* __MPC_TRANS_H_ */

