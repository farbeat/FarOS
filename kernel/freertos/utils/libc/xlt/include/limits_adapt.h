#ifndef __LIMITS_ADAPT__
#define __LIMITS_ADAPT__

#ifdef __LITEOS__
#include "los_sem.h"

#define PORT_PTHREAD_STACK_MIN LOS_TASK_MIN_STACK_SIZE
#define PORT_SEM_VALUE_MAX LOS_SEM_COUNT_MAX
#elif defined(__FREERTOS__)
#include "FreeRTOS.h"

#define PORT_PTHREAD_STACK_MIN configMINIMAL_STACK_SIZE * sizeof(StackType_t)
#define PORT_SEM_VALUE_MAX 0x7FFFU
#endif

#endif /* __LIMITS_ADAPT__ */
