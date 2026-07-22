/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef RISCV_LINX131_TIMER_H
#define RISCV_LINX131_TIMER_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TIMER_LOAD    0x0
#define TIMER_VALUE   0x4
#define TIMER_CONTROL 0x8
#define TIMER_INT_CLR 0xC
#define TIMER_RIS     0x10
#define TIMER_MIS     0x14
#define TIMER_BGLOAD  0x18

#define TIMER_ENABLE_BIT           7
#define TIMER_COUNTING_MODE_BIT    6
#define TIMER_INTERRUPT_ENABLE_BIT 5
#define TIMER_SIZE_SELECT_BIT      1

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
