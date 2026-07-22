
/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os external port header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _CMSIS_EXTENSION_H
#define _CMSIS_EXTENSION_H

#include <stdbool.h>

/**
 * Define a callback function for getting the UART working status
 * @note The UART working status is get via callback function in OS layer
 */
typedef bool (*cb_uart_is_need_awake_fn)(void);


/**
 * Define a callback register fucntion for getting the UART working status
 * @note The UART working status is get via callback function in OS layer
 */
void osUartVetoCallbackRegister(cb_uart_is_need_awake_fn cb);

#endif
