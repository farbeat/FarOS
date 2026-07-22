/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#include <stdio.h>
#include "dma_porting.h"
#include "debug_print.h"
#include "hal_uart_v120.h"
#include "uart_porting.h"
#ifdef SUPPORT_HAL_PINCTRL
#include "pinctrl_porting.h"
#include "pinctrl.h"
#endif
#include "uart.h"
#include "oal_interface.h"
#include "platform_core.h"
#include "securec.h"
#include "soc_osal.h"
#ifdef SUPPORT_CLOCKS_CORE
#include "clocks_core_common.h"
#endif
#if defined(SUPPORT_TEST_COMMON)
#include "test_suite_uart.h"
#endif
#if defined(SUPPORT_AT_CMD)
#include "at_cmd_api.h"
#endif
#ifndef BUILD_APPLICATION_SSB
#include "log_uart.h"
#include "cmsis_os.h"
#endif
#if defined(SW_RTT_DEBUG)
#include "SEGGER_RTT.h"
#endif

#ifdef BUILD_APPLICATION_SSB
#define UART_RX_BUFFER_SIZE 1
#else
#define UART_RX_BUFFER_SIZE 56
#endif

#define AT_BLE_HEADER_LENGTH 49
#define AT_CMD_BUFSIZE_MAX   256
#define UARTILPR        0x20
#define UARTIBRD        0x24
#define UARTFBRD        0x28
#define UARTLCRH        0x2c
#define UARTCR          0x30
#define UARTIFLS        0x34
#define UARTIMSC        0x38
#define UARTDMACR       0x48
#define UARTRESET       (M_CTL_RB_BASE + 0x0168)
#define UARTBUS0        0x1
#define UARTBUS1        0x0
typedef struct {
    pin_mode_t tx_pin_mode;
    pin_mode_t rx_pin_mode;
    pin_mode_t cts_pin_mode;
    pin_mode_t rts_pin_mode;
} uart_pin_mode_t;

typedef struct uart_config {
    uart_pin_config_t pin;
    uart_pin_mode_t pin_mode;
    core_irq_t irq_num;
    osal_irq_handler irq_func;
    uart_rx_callback_t rx_callback;
    uart_attr_t line_config;
    uart_buffer_config_t buffer_config;
#ifndef BUILD_APPLICATION_SSB
    osal_mutex uart_mutex;
    bool support_mutex;
#endif
} uart_config_t;

const uintptr_t g_uart_base_addrs[UART_BUS_MAX_NUM] = {
    (uintptr_t)UART0_BASE,
    (uintptr_t)UART1_BASE,
};

uintptr_t uart_porting_base_addr_get(uart_bus_t bus)
{
    return g_uart_base_addrs[bus];
}

static bool g_sw_debug_uart_enabled = false;
uint8_t g_uart0_rx_buffer[UART_RX_BUFFER_SIZE];
uint8_t g_uart1_rx_buffer[UART_RX_BUFFER_SIZE];
#if defined(SUPPORT_AT_CMD)
static void at_cmd_uart_rx_callback(const void *buf, uint16_t buf_len, bool remaining);
#endif

void log_factory_at_callback(const void *buffer, uint16_t length, bool remaining);

static uart_config_t g_uart_config[UART_BUS_MAX_NUMBER] = {
#ifdef BUILD_APPLICATION_SSB
    /* UART_BUS_0 for debug print */
    {
        {
            .tx_pin = S_MGPIO19,
            .rx_pin = S_MGPIO20,
            .cts_pin = PIN_NONE,
            .rts_pin = PIN_NONE
        },
        {
            .tx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART1,
            .rx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART1,
            .cts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX,
            .rts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX
        },
        UART_0_IRQN,
        (osal_irq_handler)irq_uart0_handler,
        NULL,
        {
            .baud_rate = 115200,
            .data_bits = UART_DATA_BIT_8,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_BIT_1,
        },
        {
            .rx_buffer_size = UART_RX_BUFFER_SIZE,
            .rx_buffer = g_uart0_rx_buffer,
        }
    },
    /* UART_BUS_1 for codeloader */
    {
        {
            .tx_pin     = S_MGPIO33,
            .rx_pin     = S_MGPIO34,
            .cts_pin    = PIN_NONE,
            .rts_pin    = PIN_NONE
        },
        {
            .tx_pin_mode     = (pin_mode_t)HAL_PIO_FUNC_UART_HS_UART,
            .rx_pin_mode     = (pin_mode_t)HAL_PIO_FUNC_UART_HS_UART,
            .cts_pin_mode    = (pin_mode_t)HAL_PIO_FUNC_MAX,
            .rts_pin_mode    = (pin_mode_t)HAL_PIO_FUNC_MAX,
        },
        UART_1_IRQN,
        (osal_irq_handler)irq_uart1_handler,
        NULL,
        {
            .baud_rate = 115200,
            .data_bits = UART_DATA_BIT_8,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_BIT_1,
        },
        {
            .rx_buffer_size = UART_RX_BUFFER_SIZE,
            .rx_buffer = g_uart1_rx_buffer,
        }
    }
#else
    /** UART_BUS_0, which is used to at cmd and debug print in wstp version,
     * and which is used to testustie and debug print in testsuite version
     */
    {
        {
            .tx_pin = S_MGPIO19,
            .rx_pin = S_MGPIO20,
            .cts_pin = PIN_NONE,
            .rts_pin = PIN_NONE
        },
        {
            .tx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART1,
            .rx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART1,
            .cts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX,
            .rts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX
        },
        UART_0_IRQN,
        (osal_irq_handler)irq_uart0_handler,
#if defined(SUPPORT_TEST_COMMON)
        test_suite_uart_rx_callback,
#elif defined(SUPPORT_AT_CMD)
        at_cmd_uart_rx_callback,
#else
        NULL,
#endif
        {
            .baud_rate = 115200,
            .data_bits = UART_DATA_BIT_8,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_BIT_1,
        },
        {
            .rx_buffer_size = UART_RX_BUFFER_SIZE,
            .rx_buffer = g_uart0_rx_buffer,
        },
        NULL,
#if defined(SUPPORT_TEST_COMMON)
        false,
#else
        true,
#endif
    },
    /* UART_BUS_1 is used to hso uart */
    {
        {
            .tx_pin = S_MGPIO33,
            .rx_pin = S_MGPIO34,
            .cts_pin = PIN_NONE,
            .rts_pin = PIN_NONE
        },
        {
            .tx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART_HS_UART,
            .rx_pin_mode = (pin_mode_t)HAL_PIO_FUNC_UART_HS_UART,
            .cts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX,
            .rts_pin_mode = (pin_mode_t)HAL_PIO_FUNC_MAX
        },
        UART_1_IRQN,
        (osal_irq_handler)irq_uart1_handler,
#ifndef FT_SINGLE_UART
#if SYS_DEBUG_MODE_ENABLE == YES
        log_uart_rx_callback,
#else
        NULL,
#endif
#else
        log_factory_at_callback,
#endif
        {
            .baud_rate = 500000,
            .data_bits = UART_DATA_BIT_8,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_BIT_1,
        },
        {
            .rx_buffer_size = UART_RX_BUFFER_SIZE,
            .rx_buffer = g_uart1_rx_buffer,
        },
        NULL,
        false
    }
#endif
};
static uart_recovery_regs_t g_uart_recovery_reg[UART_BUS_MAX_NUMBER] = { 0 };

#ifdef FT_SINGLE_UART
void log_factory_at_callback(const void *buffer, uint16_t length, bool remaining)
{
    log_uart_rx_callback(buffer, length, remaining);
    if (zdiag_is_enable() == false) {
#ifdef SUPPORT_AT_CMD
        at_cmd_msg_receive_handler((uint8_t *)buffer, length);
#endif
        hal_uart_clear_pending(UART_BUS_1);
    }
}
#endif

void uart_port_register_hal_funcs(uart_bus_t bus)
{
    hal_uart_register_funcs(bus, hal_uart_v120_funcs_get());
}

void uart_port_unregister_hal_funcs(uart_bus_t bus)
{
    hal_uart_unregister_funcs(bus);
}

uint32_t uart_port_get_clock_value(uart_bus_t bus)
{
    return uart_get_clock_value(bus);
}

void uart_port_config_pinmux(uart_bus_t bus)
{
    UNUSED(bus);
    uapi_pin_set_mode(g_uart_config[UART_BUS_1].pin.tx_pin, g_uart_config[UART_BUS_1].pin_mode.tx_pin_mode);
    uapi_pin_set_mode(g_uart_config[UART_BUS_1].pin.rx_pin, g_uart_config[UART_BUS_1].pin_mode.rx_pin_mode);
    uapi_pin_set_mode(g_uart_config[UART_BUS_0].pin.tx_pin, g_uart_config[UART_BUS_0].pin_mode.tx_pin_mode);
    uapi_pin_set_mode(g_uart_config[UART_BUS_0].pin.rx_pin, g_uart_config[UART_BUS_0].pin_mode.rx_pin_mode);
}

void uart_port_register_irq(uart_bus_t bus)
{
    osal_irq_request(g_uart_config[bus].irq_num, g_uart_config[bus].irq_func, NULL, NULL, NULL);
    osal_irq_set_priority(g_uart_config[bus].irq_num, irq_prio(g_uart_config[bus].irq_num));
    osal_irq_enable(g_uart_config[bus].irq_num);
}

void uart_port_unregister_irq(uart_bus_t bus)
{
    osal_irq_disable(g_uart_config[bus].irq_num);
}

void irq_uart0_handler(void)
{
    hal_uart_irq_handler(UART_BUS_0);
}

void irq_uart1_handler(void)
{
    hal_uart_irq_handler(UART_BUS_1);
}

#ifdef SUPPORT_AT_CMD
static void at_cmd_uart_rx_callback(const void *buf, uint16_t buf_len, bool remaining)
{
    unused(remaining);
    at_cmd_msg_receive_handler((uint8_t *)buf, buf_len);
    hal_uart_clear_pending(SW_DEBUG_UART_BUS);
}
#endif

void uart_config_init(void)
{
    for (uint8_t i = 0; i < UART_BUS_MAX_NUM; i++) {
        if ((g_uart_config[i].pin.tx_pin != PIN_NONE) || (g_uart_config[i].pin.rx_pin != PIN_NONE)) {
            uapi_uart_init(i, &(g_uart_config[i].pin), &(g_uart_config[i].line_config), NULL,
                           &(g_uart_config[i].buffer_config));
        }
        if (g_uart_config[i].rx_callback != NULL) {
            uapi_uart_register_rx_callback(i, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                           (uint32_t)(g_uart_config[i].buffer_config.rx_buffer_size),
                                           g_uart_config[i].rx_callback);
        }

#ifndef BUILD_APPLICATION_SSB
        if (g_uart_config[i].support_mutex == true) {
            osal_mutex_init(&(g_uart_config[i].uart_mutex));
        }
#endif
    }
}

#if (defined SW_UART_DEBUG)||(defined FT_SINGLE_UART)
void sw_debug_uart_deinit(void)
{
    uapi_uart_deinit(SW_DEBUG_UART_BUS);
}

void sw_debug_uart_init(uint32_t baud_rate)
{
    g_uart_config[SW_DEBUG_UART_BUS].line_config.baud_rate = baud_rate;
    uapi_uart_init(SW_DEBUG_UART_BUS, &(g_uart_config[SW_DEBUG_UART_BUS].pin),
                   &(g_uart_config[SW_DEBUG_UART_BUS].line_config), NULL,
                   &(g_uart_config[SW_DEBUG_UART_BUS].buffer_config));
}

void sw_debug_uart_reset_baund(void)
{
    while (uapi_uart_has_pending_transmissions(SW_DEBUG_UART_BUS)) { }
    uapi_uart_set_attr(SW_DEBUG_UART_BUS, &g_uart_config[SW_DEBUG_UART_BUS].line_config);
}
#endif

void print_str(const char *str, ...)
{
    static uint8_t s[1024];  // 1024 means string max length.
    va_list args;
    int32_t str_len;

    if ((str == NULL) || (strlen(str) == 0)) {
        return;
    }

    va_start(args, str);
    str_len = vsprintf_s((char *)s, sizeof(s), str, args);
    va_end(args);

    if (str_len < 0) {
        return;
    }
#if defined(SW_RTT_DEBUG)
    SEGGER_RTT_Write(0, (const char *)s, str_len);
#elif defined(SW_UART_DEBUG)
    if (hal_uart_get_funcs(SW_DEBUG_UART_BUS) == NULL) {
        return;
    }
    uapi_uart_write(SW_DEBUG_UART_BUS, (const void *)s, (uint32_t)str_len, 0);
#else
#endif
}

void hal_uart_clear_pending(uart_bus_t uart)
{
    switch (uart) {
        case UART_BUS_0:
            int_clear_pending_irq(UART_0_IRQN);
            break;
        case UART_BUS_1:
            int_clear_pending_irq(UART_1_IRQN);
            break;
        default:
            break;
    }
}

#if defined(CONFIG_UART_SUPPORT_DMA)
uint8_t uart_port_get_dma_trans_dest_handshaking(uart_bus_t bus)
{
    switch (bus) {
        case UART_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L_TX;
        case UART_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_H0_TX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}

uint8_t uart_port_get_dma_trans_src_handshaking(uart_bus_t bus)
{
    switch (bus) {
        case UART_BUS_0:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_L_RX;
        case UART_BUS_1:
            return (uint8_t)HAL_DMA_HANDSHAKING_UART_H0_RX;
        default:
            return (uint8_t)HAL_DMA_HANDSHAKING_MAX_NUM;
    }
}
#endif  /* CONFIG_UART_SUPPORT_DMA */

uint32_t uart_porting_lock(uart_bus_t bus)
{
#ifndef BUILD_APPLICATION_SSB
    if ((g_uart_config[bus].support_mutex == true) && (osKernelGetState() == osKernelRunning) &&
        (osal_in_interrupt() == 0)) {
        return (uint32_t)osal_mutex_lock_timeout(&(g_uart_config[bus].uart_mutex), OSAL_MUTEX_WAIT_FOREVER);
    }
#else
    UNUSED(bus);
#endif
    return ERRCODE_SUCC;
}

void uart_porting_unlock(uart_bus_t bus, uint32_t irq_sts)
{
#ifndef BUILD_APPLICATION_SSB
    unused(irq_sts);
    if ((g_uart_config[bus].support_mutex == true) && (osKernelGetState() == osKernelRunning) &&
        (osal_in_interrupt() == 0)) {
        osal_mutex_unlock(&(g_uart_config[bus].uart_mutex));
    }
#else
    UNUSED(bus);
    UNUSED(irq_sts);
#endif
}

void uart_save_reg(uart_bus_t bus)
{
    uint32_t base = uart_porting_base_addr_get(bus);
    g_uart_recovery_reg[bus].uartilpr = readw(base + UARTILPR);
    g_uart_recovery_reg[bus].uartibrd = readw(base + UARTIBRD);
    g_uart_recovery_reg[bus].uartfbrd = readw(base + UARTFBRD);
    g_uart_recovery_reg[bus].uartlcr_h = readw(base + UARTLCRH);
    g_uart_recovery_reg[bus].uartcr = readw(base + UARTCR);
    g_uart_recovery_reg[bus].uartifls = readw(base + UARTIFLS);
    g_uart_recovery_reg[bus].uartimsc = readw(base + UARTIMSC);
    g_uart_recovery_reg[bus].uartdmacr = readw(base + UARTDMACR);
}

void uart_recovery_reg(uart_bus_t bus)
{
    uint16_t uart_bit = UARTBUS0;
    if (bus == UART_BUS_0) {
        uart_bit = UARTBUS0;
    } else if (bus == UART_BUS_1) {
        uart_bit = UARTBUS1;
    }
    reg16_clrbit(UARTRESET, uart_bit);
    reg16_setbit(UARTRESET, uart_bit);
    uint32_t base = uart_porting_base_addr_get(bus);
    writew(base + UARTILPR, g_uart_recovery_reg[bus].uartilpr);
    writew(base + UARTIBRD, g_uart_recovery_reg[bus].uartibrd);
    writew(base + UARTFBRD, g_uart_recovery_reg[bus].uartfbrd);
    writew(base + UARTLCRH, g_uart_recovery_reg[bus].uartlcr_h);
    writew(base + UARTCR, g_uart_recovery_reg[bus].uartcr);
    writew(base + UARTIFLS, g_uart_recovery_reg[bus].uartifls);
    writew(base + UARTIMSC, g_uart_recovery_reg[bus].uartimsc);
    writew(base + UARTDMACR, g_uart_recovery_reg[bus].uartdmacr);
}

void uart_h0_suspend(void)
{
    uart_save_reg(UART_BUS_0);
}

void uart_h0_resume(void)
{
    uart_recovery_reg(UART_BUS_0);
}

void uart_h1_suspend(void)
{
    uart_save_reg(UART_BUS_1);
}

void uart_h1_resume(void)
{
    uart_recovery_reg(UART_BUS_1);
}