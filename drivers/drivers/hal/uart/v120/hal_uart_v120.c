/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V120 HAL uart \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-07, Create file. \n
 */
#include <stdint.h>
#include "soc_osal.h"
#include "securec.h"
#include "common_def.h"
#include "uart_porting.h"
#include "hal_uart_v120.h"

#if defined(CONFIG_UART_BAUD_RATE_DIV_8)
#define UARTLCR_H_INIT_VALUE 0x4700
#define UARTCR_H_INIT_VALUE 0x30

#define IBRD_NEED_BAUD_OFFSET_NUM      3
#define REMAINDER_NEED_BAUD_OFFSET_NUM 3
#define FBRD_NEED_REMAINDER_OFFSET_NUM 4
#else
#define UARTLCR_H_INIT_VALUE 0x8F00
#define UARTCR_H_INIT_VALUE 0x0

#define IBRD_NEED_BAUD_OFFSET_NUM      4
#define REMAINDER_NEED_BAUD_OFFSET_NUM 4
#define FBRD_NEED_REMAINDER_OFFSET_NUM 3
#endif

typedef enum {
    HAL_UART_V120_FIFO_INT_LEVEL_1_8,
    HAL_UART_V120_FIFO_INT_LEVEL_1_4,
    HAL_UART_V120_FIFO_INT_LEVEL_1_2,
    HAL_UART_V120_FIFO_INT_LEVEL_3_4,
    HAL_UART_V120_FIFO_INT_LEVEL_7_8
} hal_uart_fifo_int_lvl_t;

static uint32_t g_hal_uart_rx_fifo_level[UART_BUS_MAX_NUM] = { 0 };

static hal_uart_callback_t g_hal_uart_callback[UART_BUS_MAX_NUM] = { 0 };
static bool g_hal_uart_v120_inited_array[UART_BUS_MAX_NUM] = { 0 };
static hal_uart_attr_t g_hal_uart_attr_array[UART_BUS_MAX_NUM] = { 0 };

static void hal_uart_v120_disable_uart(uart_bus_t bus)
{
    hal_uart_uartcr_set_uarten(bus, 0);
    hal_uart_uartlcr_h_set(bus, UARTLCR_H_INIT_VALUE);
    hal_uart_uartcr_set(bus, UARTCR_H_INIT_VALUE);   /* Disable everything */
}

static void hal_uart_v120_enable_uart(uart_bus_t bus, const hal_uart_pin_config_t *pins)
{
    if (pins->rx_pin != PIN_NONE) {
        hal_uart_uartcr_set_rxe(bus, 1);
    }

    if (pins->tx_pin != PIN_NONE) {
        hal_uart_uartcr_set_txe(bus, 1);
    }

    if (pins->cts_pin != PIN_NONE) {
        hal_uart_uartcr_set_cts_en(bus, 1);
    }

    if (pins->rts_pin != PIN_NONE) {
        hal_uart_uartcr_set_rts_en(bus, 1);
    }

    hal_uart_uartcr_set_uarten(bus, 1);
}

static void hal_uart_v120_set_baud_rate(uart_bus_t bus, uint32_t baud)
{
    /* Baud rate divider calculation:
     * It needs an integer part an a fractional part.
     * The integer part is the result of clock/(16*baud)
     * According to the documentation the fractional part is the result of
     * round (decimal_part * 64), where decimal part is for example 0.085
     * This decimal part could be expressed as
     * remainder/(16*baud) , being remainder = uart_clk % (baud * 16)
     * Then
     * round(decimal_part * 64) : round(remainder * 64 / (16 * baud)) = round(remainder * 4 / (baud))
     */
    uint32_t uart_clk = uart_port_get_clock_value(bus);
    if ((baud << IBRD_NEED_BAUD_OFFSET_NUM) != 0) {
        uint32_t brd_i = uart_clk / (baud << IBRD_NEED_BAUD_OFFSET_NUM);
        uint32_t remainder = uart_clk % (baud << REMAINDER_NEED_BAUD_OFFSET_NUM);
        uint32_t brd_f = ((remainder << FBRD_NEED_REMAINDER_OFFSET_NUM) / baud) >> 1;
        brd_f += ((remainder << FBRD_NEED_REMAINDER_OFFSET_NUM) / baud) & 1;

        hal_uart_uartibrd_set_baud_divint(bus, brd_i);
        hal_uart_uartfbrd_set_baud_divfrac(bus, brd_f);

        /* in order to make IBRD and FBRD update (ARM DDI 0183F,3-13) */
        hal_uart_uartlcr_h_set(bus, hal_uart_uartlcr_h_get(bus));
    }
}

static void hal_uart_v120_set_data_bits(uart_bus_t bus, hal_uart_data_bit_t bits)
{
    hal_uart_uartlcr_h_set_wlen(bus, bits);
}

static void hal_uart_v120_set_parity(uart_bus_t bus, hal_uart_parity_t parity)
{
    switch (parity) {
        case UART_PARITY_EVEN:
            hal_uart_uartlcr_h_set_parity_en(bus, 1);
            hal_uart_uartlcr_h_set_eps(bus, 1);
            break;

        case UART_PARITY_ODD:
            hal_uart_uartlcr_h_set_parity_en(bus, 1);
            hal_uart_uartlcr_h_set_eps(bus, 0);
            break;

        default:
            hal_uart_uartlcr_h_set_parity_en(bus, 0);
            break;
    }
}

static void hal_uart_v120_set_stop_bits(uart_bus_t bus, hal_uart_stop_bit_t stop_bits)
{
    hal_uart_uartlcr_h_set_stp2(bus, (uint32_t)stop_bits);
}

static void hal_uart_v120_set_rxfifo_thresh_inner(uart_bus_t bus, uint32_t val)
{
    g_hal_uart_rx_fifo_level[bus] = val;
    hal_uart_uartifls_set_rxiflsel(bus, val);
}

static errcode_t hal_uart_v120_init(uart_bus_t bus, hal_uart_callback_t callback, const hal_uart_pin_config_t *pins,
                                    const hal_uart_attr_t *attr, hal_uart_flow_ctrl_t flow_ctrl)
{
    unused(flow_ctrl);
    if (g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_SUCC;
    }

    if ((hal_uart_regs_init(bus)) != ERRCODE_SUCC) {
        return ERRCODE_UART_REG_ADDR_INVALID;
    }

    /* Be sure the UART is disabled before configuring it. */
    hal_uart_v120_disable_uart(bus);
    hal_uart_uartlcr_h_set_en_fifos(bus, 1);
    hal_uart_v120_set_rxfifo_thresh_inner(bus, (uint32_t)HAL_UART_V120_FIFO_INT_LEVEL_7_8);
    hal_uart_uartifls_set_txiflsel(bus, (uint32_t)HAL_UART_V120_FIFO_INT_LEVEL_1_8);

    /* Configures the line settings for a UART. */
    hal_uart_v120_set_baud_rate(bus, attr->baud_rate);
    hal_uart_v120_set_data_bits(bus, (hal_uart_data_bit_t)attr->data_bits);
    hal_uart_v120_set_parity(bus, (hal_uart_parity_t)attr->parity);
    hal_uart_v120_set_stop_bits(bus, (hal_uart_stop_bit_t)attr->stop_bits);

    g_hal_uart_attr_array[bus].baud_rate = attr->baud_rate;
    g_hal_uart_attr_array[bus].data_bits = attr->data_bits;
    g_hal_uart_attr_array[bus].parity = attr->parity;
    g_hal_uart_attr_array[bus].stop_bits = attr->stop_bits;

    g_hal_uart_callback[bus] = callback;

    hal_uart_v120_enable_uart(bus, pins);

    g_hal_uart_v120_inited_array[bus] = true;

    return ERRCODE_SUCC;
}

static uint32_t hal_uart_v120_txfifo_level_change(uint32_t level)
{
    uint32_t new_level = level;
    if (level > HAL_UART_V120_FIFO_INT_LEVEL_1_8) {
        new_level = level - 1;
    }
    return new_level;
}

static errcode_t hal_uart_v120_ctrl_set_rxfifo_int_level(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    uint32_t level = (uint32_t)param;
    if (level > (uint32_t)HAL_UART_V120_FIFO_INT_LEVEL_7_8) {
        return ERRCODE_INVALID_PARAM;
    }

    hal_uart_v120_set_rxfifo_thresh_inner(bus, (uint32_t)level);
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_set_txfifo_int_level(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    uint32_t level = hal_uart_v120_txfifo_level_change((uint32_t)param);
    if (level > (uint32_t)HAL_UART_V120_FIFO_INT_LEVEL_7_8) {
        return ERRCODE_INVALID_PARAM;
    }

    hal_uart_uartifls_set_txiflsel(bus, (uint32_t)level);
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_set_attr(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    hal_uart_attr_t *attr = (hal_uart_attr_t *)param;
    hal_uart_v120_set_baud_rate(bus, attr->baud_rate);
    hal_uart_v120_set_data_bits(bus, (hal_uart_data_bit_t)attr->data_bits);
    hal_uart_v120_set_parity(bus, (hal_uart_parity_t)attr->parity);
    hal_uart_v120_set_stop_bits(bus, (hal_uart_stop_bit_t)attr->stop_bits);

    (void)memcpy_s(&g_hal_uart_attr_array[bus], sizeof(hal_uart_attr_t), attr, sizeof(hal_uart_attr_t));

    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_get_attr(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    hal_uart_attr_t *attr = (hal_uart_attr_t *)param;
    if (memcpy_s(attr, sizeof(hal_uart_attr_t), &g_hal_uart_attr_array[bus], sizeof(hal_uart_attr_t)) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_en_tx_int(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    bool enable = (bool)param;
    if (enable) {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_TXI, 1);
    } else {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_TXI, 0);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_en_rx_int(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    bool enable = (bool)param;
    if (enable) {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RXI, 1);
    } else {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RXI, 0);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_en_idle_int(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    bool enable = (bool)param;
    if (enable) {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RTI, 1);
    } else {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RTI, 0);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_en_parity_err_int(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    bool enable = (bool)param;
    if (enable) {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_PARITY_ERR_I, 1);
    } else {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_PARITY_ERR_I, 0);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_en_frame_err_int(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_UART_NOT_INIT;
    }

    bool enable = (bool)param;
    if (enable) {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_FRAME_ERR_I, 1);
    } else {
        hal_uart_int_set(bus, UARTIMSC, UART_INT_FRAME_ERR_I, 0);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_check_tx_fifo_full(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    bool *tx_fifo_full = (bool *)param;
    if (hal_uart_uartfr_get_txff(bus) != 0) {
        *tx_fifo_full = true;
    } else {
        *tx_fifo_full = false;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_check_tx_fifo_empty(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    bool *tx_fifo_full = (bool *)param;
    if (hal_uart_uartfr_get_txfe(bus) != 0) {
        *tx_fifo_full = true;
    } else {
        *tx_fifo_full = false;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_check_rx_fifo_emp(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (param == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    bool *rx_fifo_emp = (bool *)param;
    if (hal_uart_uartfr_get_rxfe(bus) != 0) {
        *rx_fifo_emp = true;
    } else {
        *rx_fifo_emp = false;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_check_uart_busy(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    if (param == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    bool *uart_busy = (bool *)param;
    if (hal_uart_uartfr_get_busy(bus) != 0) {
        *uart_busy = true;
    } else {
        *uart_busy = false;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_get_rxfifo_passnum(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    uint32_t rx_level = 0;
    const uint32_t rx_level_bytes[UART_FIFO_INT_LEVEL_MAX] = {
        UART_FIFO_INT_LEVEL_1_8_PASSNUM,
        UART_FIFO_INT_LEVEL_1_4_PASSNUM,
        UART_FIFO_INT_LEVEL_1_2_PASSNUM,
        UART_FIFO_INT_LEVEL_3_4_PASSNUM,
        UART_FIFO_INT_LEVEL_7_8_PASSNUM
    };

    if (param == 0) {
        return ERRCODE_INVALID_PARAM;
    }

    uint32_t *state = (uint32_t *)param;
    rx_level = hal_uart_uartifls_get_rxiflsel(bus);
    if (rx_level < UART_FIFO_INT_LEVEL_MAX) {
        *state = rx_level_bytes[rx_level];
    } else {
        *state = 0xFFFFFFFF;
    }

    return ERRCODE_SUCC;
}

#if defined(CONFIG_UART_SUPPORT_LPM)
static uart_regs_t g_uart_regs_backup[UART_BUS_MAX_NUM] = { 0 };

static errcode_t hal_uart_v120_ctrl_suspend(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    unused(param);

    errno_t err = memcpy_s(&g_uart_regs_backup[bus], sizeof(uart_regs_t),
                           (void *)hal_uart_base_addrs_get(bus), sizeof(uart_regs_t));
    if (err != EOK) {
        return ERRCODE_MEMCPY;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_uart_v120_ctrl_resume(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    unused(param);

    uart_regs_t *uart_regs = &g_uart_regs_backup[bus];
    hal_uart_uartcr_set_uarten(bus, 0);
    hal_uart_ilpr_set_ilpdvsr(bus, uart_regs->uartilpr);
    hal_uart_uartibrd_set_baud_divint(bus, uart_regs->uartibrd);
    hal_uart_uartfbrd_set_baud_divfrac(bus, uart_regs->uartfbrd);
    hal_uart_uartlcr_h_set(bus, uart_regs->uartlcr_h);
    uartcr_data_t uartcr_data;
    uartcr_data.d32 = uart_regs->uartcr;
    uartcr_data.b.uarten = 0;
    hal_uart_uartcr_set(bus, uartcr_data.d32);
    hal_uart_uartifls_set_txiflsel(bus, uart_regs->uartifls);
    hal_uart_int_set(bus, UARTIMSC, UART_INT_ALL, uart_regs->uartimsc);
    hal_uart_uartcr_set_uarten(bus, 1);

    return ERRCODE_SUCC;
}
#endif /* CONFIG_UART_SUPPORT_LPM */

#if defined(CONFIG_UART_SUPPORT_DMA)
static errcode_t hal_uart_v120_get_dma_data_addr(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    uint32_t *addr = (uint32_t *)param;
    *addr = (uint32_t)(uintptr_t)(&(((uart_regs_t *)g_hal_uarts_regs[bus])->uartdr));
    return ERRCODE_SUCC;
}
#endif

#define HAL_UART_FIFO_DEPTH_DIV_8 8
#define HAL_UART_FIFO_DEPTH_DIV_7 7
#define HAL_UART_FIFO_DEPTH_DIV_4 4
#define HAL_UART_FIFO_DEPTH_DIV_3 3
#define HAL_UART_FIFO_DEPTH_DIV_2 2
static errcode_t hal_uart_v120_get_uart_rxfifo_threshold(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    unused(id);
    uint32_t rx_fifo_thresh = 0;
    uint32_t *addr = (uint32_t *)param;
    switch (g_hal_uart_rx_fifo_level[bus]) {
        case HAL_UART_V120_FIFO_INT_LEVEL_1_8:
            rx_fifo_thresh = CONFIG_UART_FIFO_DEPTH / HAL_UART_FIFO_DEPTH_DIV_8;
            break;
        case HAL_UART_V120_FIFO_INT_LEVEL_1_4:
            rx_fifo_thresh = CONFIG_UART_FIFO_DEPTH / HAL_UART_FIFO_DEPTH_DIV_4;
            break;
        case HAL_UART_V120_FIFO_INT_LEVEL_1_2:
            rx_fifo_thresh = CONFIG_UART_FIFO_DEPTH / HAL_UART_FIFO_DEPTH_DIV_2;
            break;
        case HAL_UART_V120_FIFO_INT_LEVEL_3_4:
            rx_fifo_thresh = CONFIG_UART_FIFO_DEPTH * HAL_UART_FIFO_DEPTH_DIV_3 / HAL_UART_FIFO_DEPTH_DIV_4;
            break;
        default:
            rx_fifo_thresh = CONFIG_UART_FIFO_DEPTH * HAL_UART_FIFO_DEPTH_DIV_7 / HAL_UART_FIFO_DEPTH_DIV_8;
            break;
    }
    *addr = rx_fifo_thresh;
    return ERRCODE_SUCC;
}

static hal_uart_ctrl_t g_hal_uart_ctrl_func_array[UART_CTRL_MAX + 1] = {
    hal_uart_v120_ctrl_set_attr,             /* UART_CTRL_SET_ATTR */
    hal_uart_v120_ctrl_get_attr,             /* UART_CTRL_GET_ATTR */
    hal_uart_v120_ctrl_en_tx_int,            /* UART_CTRL_EN_TX_INT */
    hal_uart_v120_ctrl_en_rx_int,            /* UART_CTRL_EN_RX_INT */
    hal_uart_v120_ctrl_en_idle_int,          /* UART_CTRL_EN_IDLE_INT */
    hal_uart_v120_ctrl_en_parity_err_int,    /* UART_CTRL_EN_PARITY_ERR_INT */
    hal_uart_v120_ctrl_en_frame_err_int,     /* UART_CTRL_EN_FRAME_ERR_INT */
    hal_uart_v120_ctrl_check_tx_fifo_full,   /* UART_CTRL_CHECK_TX_FIFO_FULL */
    hal_uart_v120_ctrl_check_tx_fifo_empty,  /* UART_CTRL_CHECK_TX_BUSY */
    hal_uart_v120_ctrl_check_rx_fifo_emp,    /* UART_CTRL_CHECK_RX_FIFO_EMPTY */
    NULL,                                    /* UART_CTRL_FIFO_ENABLE */
    hal_uart_v120_ctrl_set_rxfifo_int_level, /* UART_CTRL_SET_RX_FIFO_LEVEL */
    hal_uart_v120_ctrl_set_txfifo_int_level, /* UART_CTRL_SET_TX_FIFO_LEVEL */
    NULL,                                    /* UART_CTRL_GET_REG_ADDRS */
    NULL,                                    /* UART_CTRL_TX_DMA_PROCESS */
    NULL,                                    /* UART_CTRL_FLOW_CTRL */
    NULL,                                    /* UART_CTRL_RESTORE */
    hal_uart_v120_ctrl_check_uart_busy,      /* UART_CTRL_CHECK_UART_BUSY */
    hal_uart_v120_ctrl_get_rxfifo_passnum,   /* UART_CTRL_GET_RX_FIFO_PASSNUM */
#if defined(CONFIG_UART_SUPPORT_LPM)
    hal_uart_v120_ctrl_suspend, /* UART_CTRL_SUSPEND */
    hal_uart_v120_ctrl_resume,  /* UART_CTRL_RESUME */
#endif                          /* CONFIG_UART_SUPPORT_LPM */
#if defined(CONFIG_UART_SUPPORT_DMA)
    hal_uart_v120_get_dma_data_addr,
#endif
    hal_uart_v120_get_uart_rxfifo_threshold,  /* get uart rx fifo threshold */
    NULL,                                   /* UART_CTRL_MAX */
};

static errcode_t hal_uart_v120_deinit(uart_bus_t bus)
{
    if (!g_hal_uart_v120_inited_array[bus]) {
        return ERRCODE_SUCC;
    }

    hal_uart_v120_disable_uart(bus);
    hal_uart_regs_deinit(bus);

    g_hal_uart_callback[bus] = NULL;
    g_hal_uart_v120_inited_array[bus] = false;

    return ERRCODE_SUCC;
}
static errcode_t hal_uart_v120_write(uart_bus_t bus, const uint8_t *data, uint16_t len)
{
    uint8_t *data_buffer = (uint8_t *)data;
    uint16_t length = len;

    while (length > 0) {
        if (hal_uart_uartfr_get_txff(bus) == 0) {
            hal_uart_uartdr_set_data(bus, *data_buffer++);
            length--;
        }
    }
    return ERRCODE_SUCC;
}

static int32_t hal_uart_v120_read(uart_bus_t bus, const uint8_t *data, uint16_t len)
{
    int32_t i = 0;
    uint8_t *data_buff = (uint8_t *)data;

    if (data == NULL || len == 0) {
        return 0;
    }
    for (i = 0; i < len; i++) {
        if (hal_uart_uartfr_get_rxfe(bus) != 0) {
            break;
        }
        data_buff[i] = (uint8_t)hal_uart_uartdr_get_data(bus);
    }
    return i;
}

static errcode_t hal_uart_v120_ctrl(uart_bus_t bus, hal_uart_ctrl_id_t id, uintptr_t param)
{
    if (g_hal_uart_ctrl_func_array[id] != NULL) {
        return g_hal_uart_ctrl_func_array[id](bus, id, param);
    }
    return ERRCODE_UART_NOT_IMPLEMENT;
}

void hal_uart_irq_handler(uart_bus_t bus)
{
    if (!g_hal_uart_callback[bus]) {
        return;
    }

    if (hal_uart_int_get(bus, UARTMIS, UART_INT_FRAME_ERR_I) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_FRAME_ERR_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_FRAME_ERR_I, 1);
        hal_uart_uartrsr_set_wdata(bus, 0);
    }
    if (hal_uart_int_get(bus, UARTMIS, UART_INT_PARITY_ERR_I) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_PARITY_ERR_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_PARITY_ERR_I, 1);
        hal_uart_uartrsr_set_wdata(bus, 0);
    }
    if (hal_uart_int_get(bus, UARTMIS, UART_INT_BREAK_ERR_I) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_BREAK_ERR_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_BREAK_ERR_I, 1);
        hal_uart_uartrsr_set_wdata(bus, 0);
    }
    if (hal_uart_int_get(bus, UARTMIS, UART_INT_OVERRUN_ERR_I) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_OVERRUN_ERR_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_OVERRUN_ERR_I, 1);
        hal_uart_uartrsr_set_wdata(bus, 0);
    }

    if (hal_uart_int_get(bus, UARTMIS, UART_INT_RXI) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_RX_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_RXI, 1);
    }
    if (hal_uart_int_get(bus, UARTMIS, UART_INT_RTI) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_IDLE_ISR, 0);
        hal_uart_int_set(bus, UARTICR, UART_INT_RTI, 1);
    }
    if (hal_uart_int_get(bus, UARTMIS, UART_INT_TXI) != 0) {
        g_hal_uart_callback[bus](bus, UART_EVT_TX_ISR, 0);
    }
}

static void hal_uart_v120_dma_config(uart_bus_t bus, const hal_uart_extra_attr_t * extra_attr)
{
    if (extra_attr->rx_dma_enable) {
        hal_uart_uartdmacr_set_rxdmaen(bus, 1);
        hal_uart_v120_set_rxfifo_thresh_inner(bus, (uint32_t)extra_attr->rx_int_threshold);
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RTI, 0);
        hal_uart_int_set(bus, UARTIMSC, UART_INT_RXI, 0);
    }
    if (extra_attr->tx_dma_enable) {
        hal_uart_uartdmacr_set_txdmaen(bus, 1);
    }
}

static hal_uart_funcs_t g_hal_uart_v120_funcs = {
    .init = hal_uart_v120_init,
    .deinit = hal_uart_v120_deinit,
    .write = hal_uart_v120_write,
    .read = hal_uart_v120_read,
    .ctrl = hal_uart_v120_ctrl,
    .dma_cfg = hal_uart_v120_dma_config
};

hal_uart_funcs_t *hal_uart_v120_funcs_get(void)
{
    return &g_hal_uart_v120_funcs;
}
