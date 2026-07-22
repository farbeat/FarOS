/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef SOC_UART_H
#define SOC_UART_H

#include "arch_base.h"
#include "asm/interrupt_config.h"
#include "asm/register_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UART_ID                 0
#define CONSOLE_UART            UART_ID
#define NUM_HAL_INTERRUPT_UART  NUM_HAL_INTERRUPT_UART3

#define CONFIG_UART_REG_BASE    UART3_REG_BASE

#define TTY_DEVICE              "/dev/uartdev-0"

#if (defined configBoardType) && (configBoardType == 1)
#define CONFIG_UART_CLK_INPUT   (80 * 1000 * 1000)
#else
#define CONFIG_UART_CLK_INPUT   (24 * 1000 * 1000)
#endif
#define CONSOLE_UART_BAUDRATE   921600
#define CONFIG_UART_BAUD_DIVINT 16
#define CONFIG_UART_BAUD_BITS 8

static inline void uart_clk_cfg(unsigned int uartNum, int flag)
{
    unsigned int tmp;
    tmp = GET_UINT32(CRG_REG_BASE + 0x0E4);
    if (flag != 0) {
        tmp |= (1U << ((uartNum) + 0xf));
    } else {
        tmp &= ~(1U << ((uartNum) + 0xf));
    }
    WRITE_UINT32(tmp, CRG_REG_BASE + 0x0E4);
}

#define UART_IO_MUX_BASE  0x50005000
#define UART_IO_MUX_UART2_TXD 0x718
#define UART_IO_MUX_UART2_RXD 0x71C

#define UART_IO_MUX_UART3_H0_TXD 0x780
#define UART_IO_MUX_UART3_H0_RXD 0x784
#define UART_IO_MUX_UART3_H0_RTS 0x788
#define UART_IO_MUX_UART3_H0_CTS 0X78C

#define UART_IO_CONFIG 1

#ifdef UART_IO_CONFIG
static inline void uart_io_config(void)
{
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART2_TXD);
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART2_RXD);
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART3_H0_TXD);
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART3_H0_RXD);
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART3_H0_RTS);
    WRITE_UINT32(1, UART_IO_MUX_BASE + UART_IO_MUX_UART3_H0_CTS);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
