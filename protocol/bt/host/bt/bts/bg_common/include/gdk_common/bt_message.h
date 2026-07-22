/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef __BT_MESSAGE__H_
#define __BT_MESSAGE__H_


#ifdef __cplusplus
#if __cplusplus

extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define BT_CONN_MESSAGE_BASE                (0x01)  /* BT message id start */
#define BT_HFP_CLNT_MESSAGE_BASE            (0x20)
#define BT_DEV_SM_MESSAGE_BASE              (0x30)
#define BT_A2DP_CLNT_MESSAGE_BASE           (0x40)
#define BT_MPC_MESSAGE_BASE                 (0x50)
#define BT_BLE_MESSAGE_BASE                 (0x60)

#define d_milisec(s) ((unsigned int)((s) * 1))

#define d_sec(s)    ((unsigned int)((s) * 1000))
/* !
Number of minutes to delay for.
*/
#define d_min(m)    ((unsigned int)((m) * (unsigned int) 60 * 1000))
/* !
Number of hours to delay for.
*/
#define d_hour(h)   ((unsigned int)((h) * (unsigned int) 60) * (unsigned int) 60 * 1000)

typedef gdk_task_t  task;
typedef gdk_task_data_t task_data_t;
typedef gdk_msg_data_t message;

#define bt_message_send_later(task, id, param, unsigned int) \
    gdk_msg_send_later(task, id, param, unsigned int)
#define bt_message_send(task, id, param) \
    gdk_msg_send(task, id, param)
#define bt_message_cancel_all(task, id) \
    gdk_msg_cancel(task, id)

#define bt_message_cancel_first(task, id) \
    gdk_msg_cancel(task, id)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* end of __BT_MESSAGE__H_ */

