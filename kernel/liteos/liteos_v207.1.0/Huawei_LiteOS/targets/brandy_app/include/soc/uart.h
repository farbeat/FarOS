/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os uart header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _HISOC_UART_H
#define _HISOC_UART_H

#include "asm/platform.h"
#include "los_typedef.h"
#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UART_WITHOUT_LOCK 0
#define UART_WITH_LOCK    1
#define UART_REG_BASE           DBG_UART_REG_BASE

#define TTY_DEVICE              "/dev/uartdev-0"

#define CONFIG_UART_CLK_INPUT   24000000
#define CONSOLE_UART_BAUDRATE   115200

typedef enum uart_bus {
    UART_BUS_0,             /*!< UART Bus 0 */
    UART_BUS_1,             /*!< UART Bus 1 */
    UART_BUS_2,             /*!< UART Bus 2 */
    UART_BUS_3,             /*!< UART Bus 3 */
    UART_BUS_MAX_NUM
} uart_bus_t;

int32_t uapi_uart_write(uart_bus_t bus, const uint8_t *buffer, uint32_t length, uint32_t timeout);
void UartPuts(const char *s, uint32_t len, bool isLock);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
