/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef __GDK_MSG_H__
#define __GDK_MSG_H__
#include "custdefs.h"
#include "stdint.h"
#include "gdk_list.h"
#include "gdk_api.h"

#ifndef GDK_MILISEC
#define gdk_milisec(s) ((uint32_t)((s) * 1))
#endif /* GDK_MILISEC */

#ifndef GDK_SEC
#define gdk_sec(s)    ((uint32_t)((s) * 1000))
#endif /* GDK_SEC */
/* !
Number of minutes to delay for.
*/
#ifndef GDK_MIN
#define gdk_min(m)    ((uint32_t)((m) * (uint32_t) 60 * 1000))
#endif /* GDK_MIN */
/* !
Number of hours to delay for.
*/
#ifndef GDK_HOUR
#define gdk_hour(h)   ((uint32_t)((h) * (uint32_t)60) * (uint32_t)60 * 1000)
#endif /* GDK_HOUR */

#define GDK_MSG_SIG 0x00000001
#define GDK_RETRY_TIMEOUT 10
#define GDK_MSG_TIME_GAP  10

typedef const void* gdk_msg_data_t;

typedef struct gdk_task_data_s *gdk_task_t;

typedef struct gdk_task_data_s {
    void (*handler)(uint16_t, gdk_msg_data_t);
} gdk_task_data_t;

typedef struct {
    gdk_list_link_t  link;
    uint16_t         msg_id;
    gdk_msg_data_t   msg_data;
    gdk_task_t       task;
    uint64_t         timestamp;
} gdk_message_t;

/**
 * @brief init message machine.
 *        create message timer for sched later.
 *        create mutex for critical resource access.
 */
void gdk_msg_init(void);

/**
 * @brief send message to virtual task.
 *        would be handled at next schedure.
 * @param task virtual task handler.
 * @param msg_id request message identifier.
 * @param msg_data hold the message parameters.
 */
void gdk_msg_send(gdk_task_t task, uint16_t id, GDK_MSG_DATA_T msg_data);
/**
 * @brief enqueue message, schedure when delay expired.
 * @param task virtual task handler.
 * @param msg_id request message identifier.
 * @param msg_data hold the message parameters.
 * @param delay   delay time.
 */
void gdk_msg_send_later(
    gdk_task_t task,
    uint16_t msg_id,
    GDK_MSG_DATA_T msg_data,
    uint32_t delay);
/**
 * @brief flush message queue with specific task and message..
 * @param task virtual task handler.
 * @param msg_id request message identifier.
 */
uint8_t gdk_msg_cancel(const gdk_task_t task, uint16_t id);

/**
 * @brief loop message queue,
 * and deal the queue message in order.
 */
void gdk_msg_loop(void);
/**
 * @brief Flush all specific task's message.
 * @param task  point to task that need to flush message
 */
void gdk_msg_flush(const gdk_task_t task);
/**
 * @brief get message data with specific task and message and index..
 * @param task virtual task handler.
 * @param msg_id request message identifier.
 * @param num  request message index.
 */
void* gdk_msg_get_data(const gdk_task_t task, uint16_t id, int num);
uint16_t gdk_msg_get_msg_num_by_msgid(const gdk_task_t task, uint16_t id);
/*
 * Get message number by specific task and message id.
 */
uint32_t gdk_msg_get_message_number(const gdk_task_t task, uint16_t id);
/**
 * @brief flush message queue with specific task and message..
 * @param task virtual task handler.
 * @param msg_id request message identifier.
 * @index  index  request message index.
 */
uint8_t gdk_msg_cancel_by_index(const gdk_task_t task, uint16_t id, int index);
#endif /* __GDK_MSG_H__ */
