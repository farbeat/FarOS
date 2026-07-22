/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides efuse port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-20， Create file. \n
 */
#include "hal_efuse_v100.h"
#include "chip_io.h"
#include "efuse_porting.h"

#define FUSE_CTL_RB_BASE            0x57028000
#define FUSE_CTL_MCU_RB_BASE        0x52082000
#define EFUSE0_BASE_ADDR             (FUSE_CTL_RB_BASE + 0xD00)
#define EFUSE1_BASE_ADDR             (FUSE_CTL_MCU_RB_BASE + 0xD10)
#define EFUSE2_BASE_ADDR             (FUSE_CTL_MCU_RB_BASE + 0xD20)
#define HAL_EFUSE0_WRITE_BASE_ADDR   (FUSE_CTL_RB_BASE + 0x700)
#define HAL_EFUSE1_WRITE_BASE_ADDR   (FUSE_CTL_MCU_RB_BASE + 0x700)
#define HAL_EFUSE2_WRITE_BASE_ADDR   (FUSE_CTL_MCU_RB_BASE + 0x700)
#define HAL_EFUSE0_READ_BASE_ADDR    (FUSE_CTL_RB_BASE + 0x800)
#define HAL_EFUSE1_READ_BASE_ADDR    (FUSE_CTL_MCU_RB_BASE + 0xA00)
#define HAL_EFUSE2_READ_BASE_ADDR    (FUSE_CTL_MCU_RB_BASE + 0xC00)
#define HAL_EFUSE_POWER_ON_SWITCH     0x57004258

const uintptr_t g_efuse_base_addr[EFUSE_REGION_NUM] = {
    (uintptr_t)EFUSE0_BASE_ADDR,
    (uintptr_t)EFUSE1_BASE_ADDR,
    (uintptr_t)EFUSE2_BASE_ADDR
};
const uintptr_t g_efuse_region_read_address[EFUSE_REGION_NUM] = {
    (uintptr_t)HAL_EFUSE0_READ_BASE_ADDR,
    (uintptr_t)HAL_EFUSE1_READ_BASE_ADDR,
    (uintptr_t)HAL_EFUSE2_READ_BASE_ADDR
};
const uintptr_t g_efuse_region_write_address[EFUSE_REGION_NUM] = {
    (uintptr_t)HAL_EFUSE0_WRITE_BASE_ADDR,
    (uintptr_t)HAL_EFUSE1_WRITE_BASE_ADDR,
    (uintptr_t)HAL_EFUSE2_WRITE_BASE_ADDR
};

const uintptr_t g_efuse_region_size[EFUSE_REGION_NUM] = {
    (uintptr_t)HAL_EFUSE0_REGION_SIZE,
    (uintptr_t)HAL_EFUSE1_REGION_SIZE,
    (uintptr_t)HAL_EFUSE2_REGION_SIZE,
};

typedef union efuse_en_switch_data {
    uint32_t    d32;
    struct {
        uint32_t    en_efuse_switch_val    : 16;
    } b;
}en_switch_data_t;

typedef struct efuse_en_switch_reg {
    volatile en_switch_data_t en_switch_data;
} efuse_en_switch_reg_t;

static efuse_en_switch_reg_t *g_efuse_en_switch_reg = (efuse_en_switch_reg_t*)(uintptr_t)HAL_EFUSE_POWER_ON_SWITCH;

static inline uint8_t get_efuse_en_switch_val(void)
{
    en_switch_data_t en_switch_val;
    en_switch_val.d32 = g_efuse_en_switch_reg->en_switch_data.d32;
    return (uint8_t)en_switch_val.b.en_efuse_switch_val;
}

static inline void set_efuse_en_switch_val(uint32_t val)
{
    en_switch_data_t en_switch_val;
    en_switch_val.d32 = g_efuse_en_switch_reg->en_switch_data.d32;
    en_switch_val.b.en_efuse_switch_val = val;
    g_efuse_en_switch_reg->en_switch_data.d32 = en_switch_val.d32;
}

void efuse_port_register_hal_funcs(void)
{
    hal_efuse_register_funcs(hal_efuse_v100_funcs_get());
}

void efuse_port_unregister_hal_funcs(void)
{
    hal_efuse_unregister_funcs();
}

hal_efuse_region_t hal_efuse_get_region(uint32_t byte_addr)
{
    return (hal_efuse_region_t)(byte_addr / EFUSE_REGION_MAX_BYTES);
}

uint16_t hal_efuse_get_byte_offset(uint32_t byte_addr)
{
    return byte_addr % EFUSE_REGION_MAX_BYTES;
}

void efuse_port_set_en_switch(uint32_t val)
{
    set_efuse_en_switch_val(val);
}

uint8_t efuse_port_get_en_switch(void)
{
    return get_efuse_en_switch_val();
}

void efuse_port_set_clk_en(hal_efuse_region_t efuse_region, bool en)
{
    unused(en);
    switch (efuse_region) {
        case HAL_EFUSE_REGION_1:
            writew(0x52000210, 0xB);
            break;
        case HAL_EFUSE_REGION_2:
            writew(0x52000210, 0xD);
            break;
        default:
            break;
    }
}

errcode_t efuse_port_read_buffer_by_reload(uint8_t *buffer, uint32_t byte_number, uint16_t length)
{
    if ((length == 0) || (byte_number >= EFUSE_MAX_BYTES) ||
    ((byte_number + length) > EFUSE_MAX_BYTES) || (buffer == NULL)) {
        return ERRCODE_FAIL;
    }
    hal_efuse_funcs_t *hal_funcs = hal_efuse_get_funcs();
    if (hal_funcs->refresh_read(HAL_EFUSE_REGION_1) != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    for (uint32_t i = 0; i < length; i++) {
        if (hal_funcs->read_byte(byte_number + i, &buffer[i]) != ERRCODE_SUCC) {
            return ERRCODE_FAIL;
        }
    }
    return ERRCODE_SUCC;
}