#ifndef _POLL_ADAPT_H
#define _POLL_ADAPT_H

#ifdef __FREERTOS__
#include "FreeRTOS.h"
#include "spinlock_adapt.h"
#include "event_groups.h"
typedef struct LOS_DL_LIST {
    struct LOS_DL_LIST *pstPrev;
    struct LOS_DL_LIST *pstNext;
} LOS_DL_LIST;

static inline void RTOS_ListInit(LOS_DL_LIST *list)
{
    list->pstNext = list;
    list->pstPrev = list;
}

static inline void RTOS_ListDelInit(LOS_DL_LIST *list)
{
    list->pstNext->pstPrev = list->pstPrev;
    list->pstPrev->pstNext = list->pstNext;
    RTOS_ListInit(list);
}

typedef struct {
    EventGroupHandle_t handler;
} EVENT_CB_S;

typedef unsigned long nfds_t;

typedef struct wait_queue_head {
    EVENT_CB_S     stEvent;
    spinlock_t     lock;
    LOS_DL_LIST    poll_queue;
} wait_queue_head_t;
#elif defined(__LITEOS__)
#include "semaphore.h"
#include "linux/wait.h"
#else
struct pollfd {
    int fd;
    short events;
    short revents;
};
#endif

#if defined(__LITEOS__) || defined(__FREERTOS__)

typedef unsigned int pollevent_t;

struct pollfd {
    int fd;               /* The descriptor being polled */
    pollevent_t events;   /* The input event flags */
    pollevent_t revents;  /* The output event flags */
    void *priv;           /* For use by drivers */
};

struct tag_poll_wait_entry;

typedef struct tag_poll_wait_entry* poll_wait_head;

typedef struct tag_poll_table {
    poll_wait_head wait;
    pollevent_t key;
} poll_table;

extern void notify_poll(wait_queue_head_t *);
extern void notify_poll_with_key(wait_queue_head_t *, pollevent_t);
#endif
#endif /* _POLL_ADAPT_H */
