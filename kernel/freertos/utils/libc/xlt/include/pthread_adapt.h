#ifndef __PTHREAD_ADAPT__
#define __PTHREAD_ADAPT__

#ifdef __LITEOS__
#include "los_list.h"
#include "los_event.h"
#include "los_mux.h"

#define LITEOS_PTHREAD_MUX MuxBaseCB stLock
#define LITEOS_PTHREAD_EVENT EVENT_CB_S event
#define LITEOS_PTHREAD_EVENT_INITIALIZER { 0, { NULL, NULL } }
#define LITEOS_PTHREAD_INITIALIZER { { (struct LOS_DL_LIST *)NULL, (struct LOS_DL_LIST *)NULL }, NULL, 0 }
#define PTHREAD_MUTEX_PRIORITY LOS_TASK_PRIORITY_LOWEST

typedef struct __pthread_attr_s {
    unsigned int detachstate;
    unsigned int schedpolicy;
    struct sched_param schedparam;
    unsigned int inheritsched;
    unsigned int scope;
    unsigned int stackaddr_set;
    void* stackaddr;
    unsigned int stacksize_set;
    size_t stacksize;
#ifdef CONFIG_LIBC_KERNEL_SMP
    cpu_set_t cpuset;
#endif
} pthread_attr_t;

/**
 * @ingroup pthread
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_PROTECT.
 */

/**
 * @ingroup pthread
 * Mutex attributes object
 */
typedef struct pthread_mutexattr {
    UINT8 protocol;
    UINT8 prioceiling;
    UINT8 type;
    UINT8 reserved;
} pthread_mutexattr_t;

/**
 * @ingroup pthread
 * Mutex object
 */
typedef struct pthread_mutex {
    pthread_mutexattr_t stAttr;
    LITEOS_PTHREAD_MUX;
} pthread_mutex_t;

typedef struct {
    clockid_t clock;
} pthread_condattr_t;

/**
 * @ingroup pthread
 * Condition variable structure.
 */
typedef struct pthread_cond {
     pthread_condattr_t stCondAttr;
     volatile int count;
     LITEOS_PTHREAD_EVENT;
     pthread_mutex_t* mutex;
     volatile int value;
} pthread_cond_t;

#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_CREATE_DETACHED 1

#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_DEFAULT 0
#define PTHREAD_MUTEX_RECURSIVE 1
#define PTHREAD_MUTEX_ERRORCHECK 2
#define PTHREAD_MUTEX_ERRORCHECK_NP 2
#define PTHREAD_MUTEX_RECURSIVE_NP 1

#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_MUTEX_ROBUST 1

#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_INHERIT 1
#define PTHREAD_PRIO_PROTECT 2

#define PTHREAD_INHERIT_SCHED 0
#define PTHREAD_EXPLICIT_SCHED 1

#define PTHREAD_SCOPE_SYSTEM 0
#define PTHREAD_SCOPE_PROCESS 1

/**
 * @ingroup pthread
 * Define a condition variable to be shared between threads within the same process.
 */
#define PTHREAD_PROCESS_PRIVATE 0

/**
 * @ingroup pthread
 * Define a condition variable to be shared among multiple processes.
 */
#define PTHREAD_PROCESS_SHARED 1

#define PTHREAD_MUTEX_INITIALIZER {{{0}}}
#define PTHREAD_RWLOCK_INITIALIZER {{{0}}}
/**
 * @ingroup pthread
 * Statically initialize a condition variable.
 */
#define PTHREAD_COND_INITIALIZER { { 0 }, -1, LITEOS_PTHREAD_EVENT_INITIALIZER, NULL, -1 }

/* Initializer for pthread_once_t instances */
/**
 * @ingroup pthread
 * Define the initial value of pthread_once_t.
 */
#define PTHREAD_ONCE_INIT 0


#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DISABLE 1
#define PTHREAD_CANCEL_MASKED 2

#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

#define PTHREAD_CANCELED ((void *)-1)

#define PTHREAD_BARRIER_SERIAL_THREAD (-1)

/**
 * @ingroup pthread
 * Define the default mutex protocol as POSIX_MUTEX_DEFAULT_INHERIT.
 */
#ifndef POSIX_MUTEX_DEFAULT_INHERIT
#define POSIX_MUTEX_DEFAULT_INHERIT
#endif

#if defined POSIX_MUTEX_DEFAULT_INHERIT
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies the priority inheritance protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_INHERIT, PTHREAD_MUTEX_PRIORITY, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    LITEOS_PTHREAD_INITIALIZER }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies the priority inheritance protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_INHERIT, PTHREAD_MUTEX_PRIORITY, 0, 0 }, \
    LITEOS_PTHREAD_INITIALIZER }
#elif defined POSIX_MUTEX_DEFAULT_PROTECT
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies the priority protection protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_PROTECT, PTHREAD_MUTEX_PRIORITY, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    LITEOS_PTHREAD_INITIALIZER }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies the priority protection protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_PROTECT, PTHREAD_MUTEX_PRIORITY, 0, 0 },  \
    LITEOS_PTHREAD_INITIALIZER }
#else
/**
 * @ingroup pthread
 * Statically initialize a recursive mutex that applies no priority protocol
 */
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
    { { PTHREAD_PRIO_NONE, PTHREAD_MUTEX_PRIORITY, PTHREAD_MUTEX_RECURSIVE_NP, 0 }, \
    LITEOS_PTHREAD_INITIALIZER }
/**
 * @ingroup pthread
 * Statically initialize a normal mutex that applies no priority protocol
 */
#define PTHREAD_MUTEX_INITIALIZER \
    { { PTHREAD_PRIO_NONE, PTHREAD_MUTEX_PRIORITY, 0, 0 }, \
    LITEOS_PTHREAD_INITIALIZER }
#endif

/*-----------------------------------------------------------*/
#elif defined(__FREERTOS__)
/*-----------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Used to identify a thread attribute object.
 */
typedef struct pthread_attr {
    uint32_t ulpthreadAttrStorage;
} pthread_attr_t;

/**
 * @brief Used to identify a mutex attribute object.
 */
typedef struct pthread_mutexattr {
    uint32_t ulpthreadMutexAttrStorage;
} pthread_mutexattr_t;

/**
 * @brief Mutex attribute object.
 */
 typedef struct pthread_mutexattr_internal {
    int iType; /**< Mutex type. */
 } pthread_mutexattr_internal_t;


/**
 * @brief Used for mutexes.
 */
typedef struct pthread_mutex_internal {
    BaseType_t xIsInitialized;          /**< Set to pdTRUE if this mutex is initialized, pdFALSE otherwise. */
    StaticSemaphore_t xMutex;           /**< FreeRTOS mutex. */
    TaskHandle_t xTaskOwner;            /**< Owner; used for deadlock detection and permission checks. */
    pthread_mutexattr_internal_t xAttr; /**< Mutex attributes. */
} pthread_mutex_t;

/**
 * @brief Used to identify a condition attribute object.
 */
typedef void *pthread_condattr_t;

/**
 * @brief Used for condition variables.
 */
typedef struct pthread_cond_internal {
    BaseType_t xIsInitialized;            /**< Set to pdTRUE if this condition variable is initialized, pdFALSE otherwise. */
    StaticSemaphore_t xCondWaitSemaphore; /**< Threads block on this semaphore in pthread_cond_wait. */
    unsigned iWaitingThreads;             /**< The number of threads currently waiting on this condition variable. */
} pthread_cond_t;

/**
 * @name pthread detach state.
 */
/**@{ */
#define PTHREAD_CREATE_DETACHED    0       /**< Detached. */
#define PTHREAD_CREATE_JOINABLE    1       /**< Joinable (default). */
/**@} */

/**
 * @name Returned to a single thread after a successful pthread_barrier_wait.
 *
 * @brief POSIX specifies that "The constant PTHREAD_BARRIER_SERIAL_THREAD is defined in
 * <pthread.h> and its value shall be distinct from any other value returned by pthread_barrier_wait()."
 * So it's defined as negative to distinguish it from the errnos, which are positive.
 */
#define PTHREAD_BARRIER_SERIAL_THREAD    ( -2 )

/**
 * @name Mutex types.
 */
/**@{ */
#ifndef PTHREAD_MUTEX_NORMAL
 #define PTHREAD_MUTEX_NORMAL        0                        /**< Non-robust, deadlock on relock, does not remember owner. */
#endif
#ifndef PTHREAD_MUTEX_ERRORCHECK
 #define PTHREAD_MUTEX_ERRORCHECK    1                        /**< Non-robust, error on relock,  remembers owner. */
#endif
#ifndef PTHREAD_MUTEX_RECURSIVE
 #define PTHREAD_MUTEX_RECURSIVE     2                        /**< Non-robust, recursive relock, remembers owner. */
#endif
#ifndef PTHREAD_MUTEX_DEFAULT
 #define PTHREAD_MUTEX_DEFAULT       PTHREAD_MUTEX_NORMAL     /**< PTHREAD_MUTEX_NORMAL (default). */
#endif
/**@} */

#define PTHREAD_COND_INITIALIZER           \
    ((( pthread_cond_t )                   \
    {                                      \
        .xIsInitialized = pdFALSE,         \
        .xCondWaitSemaphore = { { 0 } },   \
        .iWaitingThreads = 0               \
    }                                      \
    ))



/**
 * @brief Compile-time initializer of pthread_mutex_t.
 */
#define PTHREAD_MUTEX_INITIALIZER      \
    (((pthread_mutex_t)                \
    {                                  \
        .xIsInitialized = pdFALSE,     \
        .xMutex = { },                 \
        .xTaskOwner = NULL,            \
        .xAttr = { .iType = 0 }        \
    }                                  \
    ))

#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP         \
    (((pthread_mutex_t)                                \
    {                                                  \
        .xIsInitialized = pdFALSE,                     \
        .xMutex = { { 0 } },                           \
        .xTaskOwner = NULL,                            \
        .xAttr = { .iType = PTHREAD_MUTEX_RECURSIVE }  \
    }                                                  \
    ))
#elif defined(__NON_OS__)

typedef void *pthread_attr_t;

typedef void *pthread_condattr_t;

typedef void *pthread_mutexattr_t;

typedef void *pthread_mutex_t;

typedef void *pthread_cond_t;
#endif
#endif /* __PTHREAD_ADAPT__ */
