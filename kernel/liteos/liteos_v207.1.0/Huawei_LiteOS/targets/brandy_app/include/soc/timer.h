/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os timer header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _HISOC_TIMER_H
#define _HISOC_TIMER_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#define MACHINE_TIMER_LOAD_COUNT        0x00
#define MACHINE_TIMER_LOAD_COUNT_L      0x00
#define MACHINE_TIMER_LOAD_COUNT_H      0x02
#define MACHINE_TIMER_CURRENT_VALUE     0x04
#define MACHINE_TIMER_CONTROL_REG       0x08
#define MACHINE_TIMER_EOI               0x0C
#define MACHINE_TIMER_INT_STATUS        0x10
#define MACHINE_TIMER_ENABLE_USRMODE    0x3

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
