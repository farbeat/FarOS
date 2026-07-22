/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Log uart port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-14， Create file. \n
 */

#ifndef LOG_UART_PORTING_H
#define LOG_UART_PORTING_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef HSO_SUPPORT
#define LOG_UART_RX_MAX_BUFFER_SIZE 64
#else
#define LOG_UART_RX_MAX_BUFFER_SIZE 1
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif