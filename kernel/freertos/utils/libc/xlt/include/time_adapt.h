#ifndef _TIME_ADAPT_H
#define _TIME_ADAPT_H

#ifdef __LITEOS__
#define CLOCKS_PER_SEC 1000000L
#elif defined(__FREERTOS__)
#include "FreeRTOS.h"
#define CLOCKS_PER_SEC ((clock_t )configTICK_RATE_HZ)
#endif

#endif /* _TIME_ADAPT_H */
