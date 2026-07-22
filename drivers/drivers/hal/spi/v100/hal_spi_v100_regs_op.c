/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 SPI register operation api \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-06-05, Create file. \n
 */
#include <stdint.h>
#include "common_def.h"
#include "hal_spi_v100_regs_op.h"

volatile uint32_t *hal_spi_v100_int_set_reg(spi_bus_t bus, ssi_int_reg_t reg)
{
    volatile uint32_t *reg_addr = NULL;
    switch (reg) {
        case INT_MASK_REG:
            reg_addr = &spis_regs(bus)->imr;
            break;
        default:
            break;
    }
    return reg_addr;
}

volatile uint32_t *hal_spi_v100_int_get_reg(spi_bus_t bus, ssi_int_reg_t reg)
{
    volatile uint32_t *reg_addr = NULL;
    switch (reg) {
        case INT_MASK_REG:
            reg_addr = &spis_regs(bus)->imr;
            break;
        case INT_STATUS_REG:
            reg_addr = &spis_regs(bus)->isr;
            break;
        case RAW_INT_STATUS_REG:
            reg_addr = &spis_regs(bus)->risr;
            break;
    }
    return reg_addr;
}
