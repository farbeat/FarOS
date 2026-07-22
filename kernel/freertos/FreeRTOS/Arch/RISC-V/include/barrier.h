/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef _ARCH_BARRIER_H
#define _ARCH_BARRIER_H

/* This API is used to suspend execution until interrupt or a debug request occurs. */
static inline void wfi(void)
{
    __asm__ __volatile__("wfi");
}

/* This API is used to fence for memory. */
static inline void mb(void)
{
    __asm__ __volatile__("fence":::"memory");
}

#define ISB() mb()
#define dsb() mb()
#define dmb() mb()

#endif /* _ARCH_BARRIER_H */
