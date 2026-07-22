/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk at log header
 * Author: @CompanyNameTag
 * Create: 2022-03-29
 */
#ifndef SDK_LOG_H
#define SDK_LOG_H

#if (CHIP_BRANDY && !defined(BTH_TEST))
#include "btsrv_log.h"
#endif

#ifndef SDK_LOG_API_H
#define SDK_LOG_API_H

#define BTH_UART_PRINT_ON

#ifdef  BTH_UART_PRINT_ON
#define bth_at_log_print_char(c)        test_suite_uart_send_char(c)
#define bth_at_log_print_str(str)       test_suite_uart_send(str)
#define bth_at_log_print_line(line)     test_suite_uart_send_line(line)
#if (CHIP_BRANDY && !defined(BTH_TEST))
#define bth_at_log_print_f(fmt, args...) bts_info(fmt, ##args)
#else
#define bth_at_log_print_f(fmt, args...) test_suite_uart_sendf(fmt, ##args)
#endif
#else
#define bth_at_log_print_char(c)
#define bth_at_log_print_str(str)
#define bth_at_log_print_line(line)
#define bth_at_log_print_f(fmt, args...)
#endif

#endif  /* SDK_LOG_API_H */
#endif
