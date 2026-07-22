/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os timer header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _ASM_REGISTER_CONFIG_H
#define _ASM_REGISTER_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TIMER0_REG_BASE                 0x01310000
#define DBG_UART_REG_BASE               0x01321000
#define CLIC_REG_BASE                   0xb0000000


#define TIMER_TICK_REG_BASE             TIMER0_REG_BASE /* timer for tick */
#define TIMER_TIME_REG_BASE             TIMER1_REG_BASE /* timer for time */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ASM_REGISTER_CONFIG_H */
