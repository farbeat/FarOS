/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides efuse driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-20, Create file. \n
 */
#include <stdint.h>
#include <stdio.h>
#include "securec.h"
#include "common_def.h"
#include "hal_efuse.h"
#include "hal_efuse_v100_reg_op.h"
#include "efuse_porting.h"
#include "tcxo.h"

#define HAL_EFUSE_WRITE_MODE        0xa5a5
#define HAL_EFUSE_READ_MODE         0x5a5a
#define HAL_EFUSE_CLEAR_RESULT      0x0
#define HAL_EFUSE_REG_WIDTH         1U
#define HAL_EFUSE_REG_SHIFT         2U
#define HAL_EFUSE_REG_LENGTH        4U
#define HAL_EFUSE_REG_DATA_SHIFT    8U
#define HAL_EFUSE_REG_DATA_LENGTH   16
#define HAL_EFUSE_REG_VALID_DATA    0x1
#define HAL_EFUSE_REG_BYTE_OFFSET   1
#define HAL_EFUSE_BYTES_PRE_REG     2
#define HAL_EFUSE_BYTE_MASK         0xFF
#define HAL_EFUSE_REG_H_MASK        0xFF00
#define HAL_EFUSE_REG_L_MASK        0x00FF
#define HAL_EFUSE_POWER_ON_VAL      0xea69
#define HAL_EFUSE_POWER_OFF_VAL     0x0

#define HAL_EFUSE_POWER_ON_DELAY_US 120ULL
#define HAL_EFUSE_RECORD_NUMS     3

#ifdef CONFIG_EFUSE_READ_TO_RAM
static uint8_t g_store_efuse_value[HAL_EFUSE_RECORD_NUMS][EFUSE_REGION_MAX_BYTES] = {0};

static errcode_t hal_efuse_v100_read_byte(uint32_t byte_address, uint8_t *value);

static void hal_efuse_v100_read_val_to_ram(void)
{
    // Record efuse.
    uintptr_t byte_addr = 0;
    for (uint32_t i = 0; i < EFUSE_REGION_NUM; i++) {
        for (uint32_t j = 0; j < g_efuse_region_size[i]; j++) {
            hal_efuse_v100_read_byte(byte_addr, &g_store_efuse_value[i][j]);
            byte_addr++;
        }
    }
}
#endif

static errcode_t hal_efuse_v100_refresh_read(hal_efuse_region_t region)
{
    if (region >= HAL_EFUSE_REGION_MAX) {
        return ERRCODE_INVALID_PARAM;
    }
#ifdef CONFIG_EFUSE_CLK_EN
    efuse_port_set_clk_en(region, true);
#endif
    hal_efuse_ctl_set_wr_rd(region, HAL_EFUSE_READ_MODE);
    while (hal_efuse_ctl_get(region) != HAL_EFUSE_CLEAR_RESULT) {}
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_init(void)
{
    if (hal_efuse_v100_regs_init() != 0) {
        return ERRCODE_EFUSE_REG_ADDR_INVALID;
    }

    if (hal_efuse_v100_refresh_read(HAL_EFUSE_REGION_2) != 0) {
        return ERRCODE_FAIL;
    }

    if (hal_efuse_v100_refresh_read(HAL_EFUSE_REGION_1) != 0) {
        return ERRCODE_FAIL;
    }
#ifdef CONFIG_EFUSE_READ_TO_RAM
    hal_efuse_v100_read_val_to_ram();
#endif
    return ERRCODE_SUCC;
}

static void hal_efuse_v100_clear_all_write_regs(hal_efuse_region_t region)
{
    uint32_t write_addr = g_efuse_region_write_address[region];
    for (uint32_t i = 0; i < EFUSE_REGION_MAX_BYTES; i += HAL_EFUSE_REG_SHIFT) {
        *((volatile uint16_t *)((uintptr_t)(write_addr))) = 0;
        write_addr += HAL_EFUSE_REG_LENGTH;
    }
}

static void hal_efuse_v100_deinit(void)
{
    hal_efuse_v100_clear_all_write_regs(HAL_EFUSE_REGION_1);
    hal_efuse_v100_regs_deinit();
}

static errcode_t hal_efuse_v100_flush_write(hal_efuse_region_t region)
{
    if (region >= HAL_EFUSE_REGION_MAX) {
        return ERRCODE_INVALID_PARAM;
    }
    efuse_port_set_en_switch(HAL_EFUSE_POWER_ON_VAL);
    uapi_tcxo_delay_us(HAL_EFUSE_POWER_ON_DELAY_US);
    hal_efuse_ctl_set_wr_rd(region, HAL_EFUSE_WRITE_MODE);
    while (hal_efuse_ctl_get_wr_rd(region) != HAL_EFUSE_CLEAR_RESULT) {}
    efuse_port_set_en_switch(HAL_EFUSE_POWER_OFF_VAL);
    hal_efuse_v100_clear_all_write_regs(region);
    return ERRCODE_SUCC;
}

static uint32_t hal_efuse_v100_get_writeread_addr(uint32_t byte_addr)
{
    hal_efuse_region_t region = hal_efuse_get_region(byte_addr);
    uint16_t offset = hal_efuse_get_byte_offset(byte_addr);
    return g_efuse_region_read_address[region] + ((offset >> 1U) << HAL_EFUSE_REG_SHIFT);
}

static errcode_t hal_efuse_v100_read_byte(uint32_t byte_address, uint8_t *value)
{
    if ((byte_address >= EFUSE_MAX_BYTES) || (value == NULL)) {
        return ERRCODE_INVALID_PARAM;
    }

#ifdef CONFIG_EFUSE_CLK_EN
    hal_efuse_region_t region = hal_efuse_get_region(byte_address);
    efuse_port_set_clk_en(region, true);
#endif

    volatile uint16_t *efuse_byte = (volatile uint16_t *)((uintptr_t)hal_efuse_v100_get_writeread_addr(byte_address));

    if ((byte_address % HAL_EFUSE_BYTES_PRE_REG) != 0) {
        *value = (uint8_t)((*efuse_byte >> HAL_EFUSE_REG_DATA_SHIFT) & HAL_EFUSE_BYTE_MASK);
    } else {
        *value = (uint8_t)(*efuse_byte & HAL_EFUSE_BYTE_MASK);
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_read_byte_common(uint32_t byte_address, uint8_t *value)
{
#ifndef CONFIG_EFUSE_READ_TO_RAM
    hal_efuse_v100_read_byte(byte_address, value);
#else
    hal_efuse_region_t region = hal_efuse_get_region(byte_address);
    uint16_t offset = hal_efuse_get_byte_offset(byte_address);
    *value = g_store_efuse_value[region][offset];
#endif
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_write_byte(uint32_t byte_address, uint8_t value)
{
    if (byte_address >= EFUSE_MAX_BYTES) {
        return ERRCODE_INVALID_PARAM;
    }

    uint16_t write_value;
    hal_efuse_region_t region = hal_efuse_get_region(byte_address);
    uint16_t offset = hal_efuse_get_byte_offset(byte_address);

#ifdef CONFIG_EFUSE_CLK_EN
    efuse_port_set_clk_en(region, true);
#endif

    volatile uint16_t *efuse_byte = (volatile uint16_t *)((uintptr_t)g_efuse_region_write_address[region] +
                                    ((offset >> 1U) << HAL_EFUSE_REG_SHIFT));

    write_value = (byte_address % HAL_EFUSE_BYTES_PRE_REG != 0) ?
                   (uint16_t)(((uint16_t)value << HAL_EFUSE_REG_DATA_SHIFT) & HAL_EFUSE_REG_H_MASK) :
                   ((uint16_t)value & HAL_EFUSE_REG_L_MASK);

    *efuse_byte = *efuse_byte | write_value;

#ifdef CONFIG_EFUSE_READ_TO_RAM
    if (byte_address % HAL_EFUSE_BYTES_PRE_REG != 0) {
        g_store_efuse_value[region][offset] = g_store_efuse_value[region][offset] | value;
    } else {
        g_store_efuse_value[region][offset] = g_store_efuse_value[region][offset] | write_value;
    }
#endif
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_write_operation(uint32_t address, uint8_t value, hal_efuse_region_t region)
{
    if (hal_efuse_v100_write_byte(address, value) == 0) {
        if (hal_efuse_v100_flush_write(region) == 0) {
            if (hal_efuse_v100_refresh_read(region) == 0) {
                return ERRCODE_SUCC;
            }
        }
    }
    hal_efuse_v100_clear_all_write_regs(region);
    return ERRCODE_FAIL;
}

static errcode_t hal_efuse_v100_write_buffer_operation(uint32_t address, const uint8_t *buffer, uint16_t length)
{
    uint8_t read_byte = 0;
    for (uint32_t i = 0; i < length; i++) {
        if (hal_efuse_v100_read_byte_common(address + i, &read_byte) == 0) {
            if (read_byte != 0) {
                return ERRCODE_FAIL;
            }
        } else {
            return ERRCODE_FAIL;
        }

        if (hal_efuse_v100_write_byte(address + i, buffer[i]) != 0) {
            hal_efuse_region_t region_tmp = hal_efuse_get_region(address + i);
            hal_efuse_v100_clear_all_write_regs(region_tmp);
            return ERRCODE_FAIL;
        }

        if ((((address + i + 1) % EFUSE_REGION_MAX_BYTES) == 0) || ((i + 1) == length)) {
            hal_efuse_region_t region = hal_efuse_get_region(address + i);
            if ((hal_efuse_v100_flush_write(region) != 0) || (hal_efuse_v100_refresh_read(region) != 0)) {
                hal_efuse_v100_clear_all_write_regs(region);
                return ERRCODE_FAIL;
            }
        }
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_get_die_id(const uint8_t *buffer, uint16_t length)
{
    uint32_t byte_number = EFUSE_DIE_ID_BASE_BYTE_ADDR;
    for (uint32_t i = 0; i < length; i++) {
        if (hal_efuse_v100_read_byte_common(byte_number + i, &buffer[i]) != 0) {
            return ERRCODE_FAIL;
        }
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_efuse_v100_get_chip_id(const uint8_t *buffer, uint16_t length)
{
    unused(buffer);
    unused(length);
    return ERRCODE_SUCC;
}

hal_efuse_funcs_t g_hal_efuse_v100_funcs = {
    .init = hal_efuse_v100_init,
    .deinit = hal_efuse_v100_deinit,
    .flush_write = hal_efuse_v100_flush_write,
    .refresh_read = hal_efuse_v100_refresh_read,
    .read_byte = hal_efuse_v100_read_byte_common,
    .write_byte = hal_efuse_v100_write_byte,
    .clear = hal_efuse_v100_clear_all_write_regs,
    .write_op = hal_efuse_v100_write_operation,
    .write_buffer_op = hal_efuse_v100_write_buffer_operation,
    .get_die_id = hal_efuse_v100_get_die_id,
    .get_chip_id = hal_efuse_v100_get_chip_id
};

hal_efuse_funcs_t *hal_efuse_v100_funcs_get(void)
{
    return &g_hal_efuse_v100_funcs;
}
