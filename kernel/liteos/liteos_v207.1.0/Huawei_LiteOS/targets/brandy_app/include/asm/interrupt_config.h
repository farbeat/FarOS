/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os interrupt config header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _ASM_INTERRUPT_CONFIG_H
#define _ASM_INTERRUPT_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define NUM_HAL_INTERRUPT_TIMER0        20
#define NUM_HAL_INTERRUPT_TIMER         7
#define OS_TICK_INT_NUM                 NUM_HAL_INTERRUPT_TIMER0


#define NUM_HAL_INTERRUPT_MSIP          3
#define MACHINE_INT_NUM                 7
#define NUM_HAL_INTERRUPT_MTIP          7
#define NUM_HAL_INTERRUPT_MEIP          11
#define NUM_HAL_INTERRUPT_CSIP          12
#define NUM_HAL_INTERRUPT_INV           10
#define OS_RISCV_SYS_VECTOR_CNT         26
#define NUM_HAL_INTERRUPT_UART          32

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ASM_INTERRUPT_CONFIG_H */
