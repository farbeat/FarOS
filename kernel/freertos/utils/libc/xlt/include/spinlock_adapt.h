#ifndef _SPINLOCK_ADAPT_H
#define _SPINLOCK_ADAPT_H

#ifdef __FREERTOS__

typedef struct {
    void *lock;
} spinlock_t;

static inline void RTOS_SpinInit(spinlock_t *lock)
{
    return;
}
#endif
#endif