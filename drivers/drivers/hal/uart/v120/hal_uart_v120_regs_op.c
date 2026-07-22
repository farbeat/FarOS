/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V120 uart register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "uart_porting.h"
#include "hal_uart_v120_regs_op.h"

static volatile uint32_t *hal_uart_int_addr_get(uart_bus_t bus, uart_int_reg_t reg)
{
    volatile uint32_t *reg_addr = NULL;
    switch (reg) {
        case UARTIMSC:
            reg_addr = &((uart_regs_t *)g_hal_uarts_regs[bus])->uartimsc;
            break;
        case UARTRIS:
            reg_addr = &((uart_regs_t *)g_hal_uarts_regs[bus])->uartris;
            break;
        case UARTMIS:
            reg_addr = &((uart_regs_t *)g_hal_uarts_regs[bus])->uartmis;
            break;
        default:
            reg_addr = &((uart_regs_t *)g_hal_uarts_regs[bus])->uarticr;
            break;
    }
    return reg_addr;
}

uint32_t hal_uart_int_get(uart_bus_t bus, uart_int_reg_t reg, uart_int_t int_id)
{
    volatile uint32_t *reg_addr = NULL;
    reg_addr = hal_uart_int_addr_get(bus, reg);

    if (int_id == UART_INT_ALL) {
        return *reg_addr;
    }

    return ((*reg_addr) & (1 << (uint32_t)int_id)) >> ((uint32_t)int_id);
}

void hal_uart_int_set(uart_bus_t bus, uart_int_reg_t reg, uart_int_t int_id,  uint32_t val)
{
    volatile uint32_t *reg_addr = NULL;
    reg_addr = hal_uart_int_addr_get(bus, reg);

    if (int_id == UART_INT_ALL) {
        *reg_addr = val;
        return;
    }

    if (val != 0) {
        *reg_addr |= (1 << (uint32_t)int_id);
    } else {
        *reg_addr &= ~(1 << (uint32_t)int_id);
    }
}
