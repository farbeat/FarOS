/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides uart port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-06， Create file. \n
 */
#ifndef UART_PORT_H
#define UART_PORT_H

#include <stdint.h>
#include "platform_core.h"
#include "std_def.h"
#include "debug_print.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_uart Uart
 * @ingroup  drivers_port
 * @{
 */

#define SW_UART_BAUDRATE 115200UL
#define UART_L_BAUDRATE  115200UL
#define UART_BUS_MAX_NUM UART_BUS_MAX_NUMBER

/*  Uart FIFO Level corresponding number of passes  */
#define UART_FIFO_INT_LEVEL_1_8_PASSNUM 4
#define UART_FIFO_INT_LEVEL_1_4_PASSNUM 8
#define UART_FIFO_INT_LEVEL_1_2_PASSNUM 16
#define UART_FIFO_INT_LEVEL_3_4_PASSNUM 24
#define UART_FIFO_INT_LEVEL_7_8_PASSNUM 28

/* rx fifo level */
#ifdef HSO_SUPPORT
#define HAL_UART_PL011_FIFO_INT_LEVEL_PORTING UART_FIFO_INT_LEVEL_3_4
#else
#define HAL_UART_PL011_FIFO_INT_LEVEL_PORTING UART_FIFO_INT_LEVEL_7_8
#endif

typedef struct {
    uint32_t uartdr;
    uint32_t uartrsr;
    uint32_t res0;
    uint32_t res1;
    uint32_t res2;
    uint32_t res3;
    uint32_t uartfr;
    uint32_t res4;
    uint32_t uartilpr;
    uint32_t uartibrd;  // Integer baud divider
    uint32_t uartfbrd;  // Fractional Baud divider
    uint32_t uartlcr_h;
    uint32_t uartcr;
    uint32_t uartifls;
    uint32_t uartimsc;
    uint32_t uartris;
    uint32_t uartmis;
    uint32_t uarticr;
    uint32_t uartdmacr;
} uart_ctrl_t;

/** Uart FIFO Level */
typedef enum {
    UART_FIFO_INT_LEVEL_1_8,
    UART_FIFO_INT_LEVEL_1_4,
    UART_FIFO_INT_LEVEL_1_2,
    UART_FIFO_INT_LEVEL_3_4,
    UART_FIFO_INT_LEVEL_7_8,
    UART_FIFO_INT_LEVEL_MAX
} uart_fifo_int_lvl_t;

/* UART recovery register after the UART wakes up from deep sleep */
typedef struct {
    volatile uint32_t uartilpr;         /*!< IrDA low-power counter register.  <i>Offset: 20h</i>. */
    volatile uint32_t uartibrd;         /*!< Integer baud rate divisor register.  <i>Offset: 24h</i>. */
    volatile uint32_t uartfbrd;         /*!< Fractional baud rate divisor register.  <i>Offset: 28h</i>. */
    volatile uint32_t uartlcr_h;        /*!< Line control register, HIGH byte  <i>Offset: 2Ch</i>. */
    volatile uint32_t uartcr;           /*!< Control register.  <i>Offset: 30h</i>. */
    volatile uint32_t uartifls;         /*!< Interrupt FIFO level select register.  <i>Offset: 34h</i>. */
    volatile uint32_t uartimsc;         /*!< Interrupt mask set/clear.  <i>Offset: 38h</i>. */
    volatile uint32_t uartdmacr;        /*!< DMA control register.  <i>Offset: 48h</i>. */
}uart_recovery_regs_t;

/**
 * @brief  Base address list for all of the IPs.
 */
extern const uintptr_t g_uart_base_addrs[UART_BUS_MAX_NUM];

/**
 * @brief  Register hal funcs objects into hal_uart module.
 */
void uart_port_register_hal_funcs(uart_bus_t bus);

/**
 * @brief  Unregister hal funcs objects from hal_uart module.
 */
void uart_port_unregister_hal_funcs(uart_bus_t bus);

/**
 * @brief  Get the bus clock of specified uart.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The bus clock of specified uart.
 */
uint32_t uart_port_get_clock_value(uart_bus_t bus);

/**
 * @brief  Config the pinmux of the uarts above.
 */
void uart_port_config_pinmux(uart_bus_t bus);

/**
 * @brief  unRegister the interrupt of uarts.
 */
void uart_port_unregister_irq(uart_bus_t bus);

/**
 * @brief  Register the interrupt of uarts.
 */
void uart_port_register_irq(uart_bus_t bus);

/**
 * @brief  Handler of UART0 IRQ.
 */
void irq_uart0_handler(void);

/**
 * @brief  Handler of UART1 IRQ.
 */
void irq_uart1_handler(void);

/**
 * @brief  Handler of UART2 IRQ.
 */
void irq_uart2_handler(void);

/**
 * @brief Init uart bus config.
 */
void uart_config_init(void);

/**
 * @brief  Clear uart pending signal.
 *
 * @param uart uart bus.
 */
void hal_uart_clear_pending(uart_bus_t uart);

/**
 * @brief  uart lock.
 *
 * @param uart bus.
 */
uint32_t uart_porting_lock(uart_bus_t bus);

/**
 * @brief  uart unlock.
 *
 * @param uart bus.
 */
void uart_porting_unlock(uart_bus_t bus, uint32_t irq_sts);


#if defined(CONFIG_UART_SUPPORT_DMA)
/**
 * @brief  Get the DMA destination handshaking select of uart transfer.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The DMA destination handshaking select of uart transfer.
 */
uint8_t uart_port_get_dma_trans_dest_handshaking(uart_bus_t bus);

/**
 * @brief  Get the DMA source handshaking select of uart transfer.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 * @return The DMA source handshaking select of uart transfer.
 */
uint8_t uart_port_get_dma_trans_src_handshaking(uart_bus_t bus);
#endif  /* CONFIG_UART_SUPPORT_DMA */

uintptr_t uart_porting_base_addr_get(uart_bus_t bus);

/**
 * @brief  The UART saves the register value during deep sleep.
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 */
void uart_save_reg(uart_bus_t bus);

/**
 * @brief  The UART recovery register after deep sleep wakeup
 * @param  [in]  bus The uart bus. see @ref uart_bus_t
 */
void uart_recovery_reg(uart_bus_t bus);

/**
 * @brief  The UART h0 saves the register value during deep sleep.
 */
void uart_h0_suspend(void);

/**
 * @brief  The UART h0 recovery register after wake up in deep sleep.
 */
void uart_h0_resume(void);

/**
 * @brief  The UART h1 saves the register value during deep sleep.
 */
void uart_h1_suspend(void);

/**
 * @brief  The UART h1 recovery register after wake up in deep sleep.
 */
void uart_h1_resume(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif