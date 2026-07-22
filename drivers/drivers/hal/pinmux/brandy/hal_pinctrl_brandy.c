/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides HAL pinctrl \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-29, Create file. \n
 */

#include <stdint.h>
#include "common_def.h"
#include "errcode.h"
#include "pinctrl_porting.h"
#include "hal_pinctrl_brandy.h"

/* pin base address */
#define HAL_PIN_PINMUX_C_PINMUX_CTL_ADDR     0x52008000
#define HAL_PIN_PINMUX_PINMUX_CTL_ADDR       0x5700C000
#define HAL_PIN_MEM_PINMUX_CTL_ADDR          0xA3005000
#define HAL_PIN_HS_PINMUX_CTL_ADDR           0x52065800
#define HAL_PIN_ULPON_CTL_ADDR               0x5702C000

/* PINMUX_CTL */
#define HAL_S_AGPIO_L_MODE_BASE_ADDR              HAL_PIN_PINMUX_PINMUX_CTL_ADDR
#define HAL_PAD_S_AGPIO_L_CFG_BASE_ADDR          (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0x400)
#define HAL_PAD_S_MGPIO_CFG_BASE_ADDR            (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0x800)
#define HAL_PAD_S_EGPIO_CFG_BASE_ADDR            (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0xC00)
#define HAL_PAD_S_HGPIO_CFG_BASE_ADDR            (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0x1000)
#define HAL_S_AGPIO_R_MODE_BASE_ADDR             (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0x1400)
#define HAL_PAD_S_AGPIO_R_BASE_ADDR              (HAL_PIN_PINMUX_PINMUX_CTL_ADDR + 0x1800)

/* MEM_PINMUX */
#define HAL_S_EGPIO_MODE_BASE_ADDR               HAL_PIN_MEM_PINMUX_CTL_ADDR

/* S_HGPIO */
#define HAL_S_HGPIO_MODE_BASE_ADDR               HAL_PIN_HS_PINMUX_CTL_ADDR

/* S_MGPIO */
#define HAL_S_MGPIO_MODE_BASE_ADDR               HAL_PIN_PINMUX_C_PINMUX_CTL_ADDR

/* AON_CTL */
#define HAL_ULP_GPIO_MODE_BASE_ADDR              (HAL_PIN_ULPON_CTL_ADDR + 0x300)
#define HAL_PAD_S_ULP_GPIO_BASE_ADDR             (HAL_PIN_ULPON_CTL_ADDR + 0x200)

/* Because the pin pad starts from 1, the base address offset of the pad starts from 200. */
#define HAL_PAD_S_AGPIO_L_CFG_ADDR          (HAL_PAD_S_AGPIO_L_CFG_BASE_ADDR + 0x200)
#define HAL_PAD_S_MGPIO_CFG_ADDR            (HAL_PAD_S_MGPIO_CFG_BASE_ADDR + 0x200)
#define HAL_PAD_S_EGPIO_CFG_ADDR            (HAL_PAD_S_EGPIO_CFG_BASE_ADDR + 0x200)
#define HAL_PAD_S_HGPIO_CFG_ADDR            (HAL_PAD_S_HGPIO_CFG_BASE_ADDR + 0x200)
#define HAL_PAD_S_AGPIO_R_CFG_ADDR          (HAL_PAD_S_AGPIO_R_BASE_ADDR + 0x200)

#define HAL_PIN_PINMUX_ULP_GPIO_4_POS   0
#define HAL_PIN_PINMUX_ULP_GPIO_5_POS   1
#define HAL_PIN_PINMUX_ULP_GPIO_6_POS   2
#define HAL_PIN_BRANDY_MODE_CONTROL_CONFIG_BITS 4U
#define HAL_PIN_BRANDY_DS_CFG_BITS              2
#define HAL_PIN_BRANDY_IE_CFG_BIT               1

/**
 * @brief  PIN config dirver strength addr map.
 */
typedef struct {
    pin_t begin;             /*!< The first pin of each group. */
    pin_t end;               /*!< The last pin of each group. */
    uint32_t addr;           /*!< Address of the group. */
    uint32_t reg_num;        /*!< Configure the number of registers of the pin. */
    uint32_t reg_bit;        /*!< The first bit to config. */
} hal_pin_ds_map_t;

/**
 * @brief  PIN config mode addr map.
 */
typedef struct {
    pin_t begin;                /*!< The first pin of each group. */
    pin_t end;                  /*!< The last pin of each group. */
    uint32_t reg_addr;          /*!< Address of the group. */
    uint32_t per_num;           /*!< Number of pins that can be configured by one register. */
} hal_pin_mode_map_t;

/**
 * @brief  PIN config pull addr map.
 */
typedef struct {
    pin_t begin;            /*!< The first pin of each group. */
    pin_t end;              /*!< The last pin of each group. */
    uint32_t addr;          /*!< Address of group. */
    uint32_t ps_bit;        /*!< The first bit to config. */
    uint32_t pe_bit;        /*!< The pe bit to config. */
} hal_pin_pull_map_t;

static hal_pin_mode_map_t const g_pin_mode_map[] = {
    { S_EGPIO0,  S_EGPIO25,     HAL_S_EGPIO_MODE_BASE_ADDR,         1 },
    { S_HGPIO0,  S_HGPIO15,     HAL_S_HGPIO_MODE_BASE_ADDR,         1 },
    { S_MGPIO0,  S_MGPIO38,     HAL_S_MGPIO_MODE_BASE_ADDR,         1 },
    { S_AGPIO_L0,  S_AGPIO_L32, HAL_S_AGPIO_L_MODE_BASE_ADDR,       1 },
    { S_AGPIO_R0,  S_AGPIO_R10, HAL_S_AGPIO_R_MODE_BASE_ADDR,       1 },
    { ULP_GPIO0, ULP_GPIO6,     HAL_ULP_GPIO_MODE_BASE_ADDR,        1 },
};

static hal_pin_pull_map_t const g_pin_pull_map[] = {
    { S_EGPIO0,  S_EGPIO25, HAL_PAD_S_EGPIO_CFG_ADDR, 2, 0},
    { S_HGPIO0,  S_HGPIO15, HAL_PAD_S_HGPIO_CFG_ADDR, 2, 0},
    { S_MGPIO0,  S_MGPIO38, HAL_PAD_S_MGPIO_CFG_ADDR, 2, 0},
    { S_AGPIO_L0,  S_AGPIO_L32, HAL_PAD_S_AGPIO_L_CFG_ADDR, 2, 0},
    { S_AGPIO_R0,  S_AGPIO_R10, HAL_PAD_S_AGPIO_R_CFG_ADDR, 2, 0},
    { ULP_GPIO0, ULP_GPIO6, HAL_PAD_S_ULP_GPIO_BASE_ADDR, 2, 0},
};

static hal_pin_ds_map_t const g_pin_ds_map[] = {
    { S_EGPIO0,  S_EGPIO25, HAL_PAD_S_EGPIO_CFG_ADDR,  1, 6 },
    { S_HGPIO0,  S_HGPIO15, HAL_PAD_S_HGPIO_CFG_ADDR,  1, 6 },
    { S_MGPIO0,  S_MGPIO38, HAL_PAD_S_MGPIO_CFG_ADDR,  1, 6 },
    { S_AGPIO_L0,  S_AGPIO_L32, HAL_PAD_S_AGPIO_L_CFG_ADDR,  1, 6 },
    { S_AGPIO_R0,  S_AGPIO_R10, HAL_PAD_S_AGPIO_R_CFG_ADDR,  1, 6 },
    { ULP_GPIO0, ULP_GPIO6, HAL_PAD_S_ULP_GPIO_BASE_ADDR, 1, 6 },
};

static void hal_pin_get_mode_addr(hal_pin_mode_map_t **pin_map, uint32_t *map_size)
{
    *map_size = (uint32_t)array_size(g_pin_mode_map);
    *pin_map = g_pin_mode_map;
}

static void hal_pin_get_pull_addr(hal_pin_pull_map_t **pin_map, uint32_t *map_size)
{
    *map_size = (uint32_t)array_size(g_pin_pull_map);
    *pin_map = g_pin_pull_map;
}

static void hal_pin_get_ds_addr(hal_pin_ds_map_t **pin_map, uint32_t *map_size)
{
    *map_size = (uint32_t)array_size(g_pin_ds_map);
    *pin_map = g_pin_ds_map;
}

static void hal_pin_get_ulp_mode_addr_and_bits(pin_t pin, uint32_t *reg_addr, uint32_t *config_pos)
{
    switch (pin) {
        case ULP_GPIO4:
            *config_pos = HAL_PIN_PINMUX_ULP_GPIO_4_POS;
            break;
        case HRESET:
            *config_pos = HAL_PIN_PINMUX_ULP_GPIO_5_POS;
            break;
        case ULP_GPIO6:
            *config_pos = HAL_PIN_PINMUX_ULP_GPIO_6_POS;
            break;
        default:
            return;
    }
    *reg_addr = (uint32_t)HAL_ULP_GPIO_MODE_BASE_ADDR;
}

static hal_pin_mode_map_t *hal_pin_brandy_get_aon_mode_addr(pin_t pin)
{
    hal_pin_mode_map_t *pin_map = NULL;
    uint32_t map_size = 0;

    hal_pin_get_mode_addr(&pin_map, &map_size);
    for (uint32_t i = 0; i < map_size; i++) {
        if ((pin >= pin_map[i].begin) && (pin <= pin_map[i].end)) {
            return (pin_map + i);
        }
    }
    return NULL;
}

static errcode_t hal_pin_brandy_set_aon_mode(pin_t pin, pin_mode_t mode)
{
    uint32_t reg_addr, config_pos;
    hal_pin_mode_map_t *pin_map = NULL;

    pin_map = hal_pin_brandy_get_aon_mode_addr(pin);
    if (pin_map == NULL) {
        return ERRCODE_FAIL;
    }
    reg_addr = (uintptr_t)pin_map->reg_addr + ((uint32_t)(pin - pin_map->begin) / pin_map->per_num) *
               (sizeof(uint32_t));
    config_pos = (((uint32_t)pin % pin_map->per_num) * HAL_PIN_BRANDY_MODE_CONTROL_CONFIG_BITS);
    uapi_reg_setbits16(reg_addr, config_pos, HAL_PIN_BRANDY_MODE_CONTROL_CONFIG_BITS, mode);
    return ERRCODE_SUCC;
}

static errcode_t hal_pin_brandy_set_ulp_mode(pin_t pin, pin_mode_t mode)
{
    uint32_t reg_addr = 0;
    uint32_t config_pos;
    hal_pin_get_ulp_mode_addr_and_bits(pin, &reg_addr, &config_pos);
    if (reg_addr == 0) {
        return ERRCODE_FAIL;
    }
    if (mode == PIN_MODE_0) {
        uapi_reg_clrbit16((uintptr_t)reg_addr, config_pos);
    } else {
        uapi_reg_setbit16((uintptr_t)reg_addr, config_pos);
    }
    return ERRCODE_SUCC;
}

static pin_mode_t hal_pin_brandy_get_aon_mode(pin_t pin)
{
    uint32_t reg_addr, config_pos;
    hal_pin_mode_map_t *pin_map = NULL;

    pin_map = hal_pin_brandy_get_aon_mode_addr(pin);
    if (pin_map == NULL) {
        return PIN_MODE_MAX;
    }
    reg_addr = (uintptr_t)pin_map->reg_addr + ((uint32_t)(pin - pin_map->begin) / pin_map->per_num) *
               (sizeof(uint32_t));
    config_pos = (((uint32_t)pin % pin_map->per_num) * HAL_PIN_BRANDY_MODE_CONTROL_CONFIG_BITS);
    return (pin_mode_t)uapi_reg_getbits16(reg_addr, config_pos, HAL_PIN_BRANDY_MODE_CONTROL_CONFIG_BITS);
}

static pin_mode_t hal_pin_brandy_get_ulp_mode(pin_t pin)
{
    uint32_t reg_addr = 0;
    uint32_t config_pos;
    hal_pin_get_ulp_mode_addr_and_bits(pin, &reg_addr, &config_pos);
    if (reg_addr == 0) {
        return PIN_MODE_MAX;
    }
    return (pin_mode_t)uapi_reg_getbits16((uintptr_t)reg_addr, config_pos, 1);
}

static hal_pin_ds_map_t *hal_pin_brandy_get_aon_ds_addr(pin_t pin)
{
    hal_pin_ds_map_t *pin_map = NULL;
    uint32_t map_size = 0;

    hal_pin_get_ds_addr(&pin_map, &map_size);
    for (uint32_t i = 0; i < map_size; i++) {
        if ((pin >= pin_map[i].begin) && (pin <= pin_map[i].end)) {
            return (pin_map + i);
        }
    }
    return NULL;
}

static errcode_t hal_pin_brandy_set_aon_ds(pin_t pin, pin_drive_strength_t ds)
{
    uint32_t config_pos = 0;
    uint32_t reg_addr;
    hal_pin_ds_map_t *pin_map = NULL;

    pin_map = hal_pin_brandy_get_aon_ds_addr(pin);
    if (pin_map == NULL) {
        return ERRCODE_FAIL;
    }
    reg_addr = (uintptr_t)pin_map->addr + ((uint32_t)pin - (uint32_t)(pin_map->begin)) * (sizeof(uint32_t));
    config_pos = pin_map->reg_bit;
    uapi_reg_setbits16(reg_addr, config_pos, HAL_PIN_BRANDY_DS_CFG_BITS, ds);
    return ERRCODE_SUCC;
}

static pin_drive_strength_t hal_pin_brandy_get_aon_ds(pin_t pin)
{
    uint32_t config_pos = 0;
    uint32_t reg_addr;
    hal_pin_ds_map_t *pin_map = NULL;

    pin_map = hal_pin_brandy_get_aon_ds_addr(pin);
    if (pin_map == NULL) {
        return PIN_DS_MAX;
    }
    reg_addr = (uintptr_t)pin_map->addr + ((uint32_t)pin - (uint32_t)(pin_map->begin)) * (sizeof(uint32_t));
    config_pos = pin_map->reg_bit;

    return (pin_drive_strength_t)uapi_reg_getbits16(reg_addr, config_pos, HAL_PIN_BRANDY_DS_CFG_BITS);
}

static hal_pin_pull_map_t *hal_pin_brandy_get_pull_addr(pin_t pin)
{
    hal_pin_pull_map_t *pin_map = NULL;
    uint32_t map_size = 0;

    hal_pin_get_pull_addr(&pin_map, &map_size);
    for (uint32_t i = 0; i < map_size; i++) {
        if ((pin >= pin_map[i].begin) && (pin <= pin_map[i].end)) {
            return (pin_map + i);
        }
    }
    return NULL;
}

static errcode_t hal_pin_brandy_set_aon_pull(pin_t pin, pin_pull_t pull_type)
{
    hal_pin_pull_map_t *pin_map = NULL;
    uint32_t ps_bit = 0;
    uint32_t reg_addr = 0;
    uint32_t pe_bit = 0;

    pin_map = hal_pin_brandy_get_pull_addr(pin);
    if (pin_map == NULL) {
        return ERRCODE_FAIL;
    }
    reg_addr = (uintptr_t)pin_map->addr + (uint32_t)(pin - pin_map->begin) * (sizeof(uint32_t));
    ps_bit = pin_map->ps_bit;
    pe_bit = pin_map->pe_bit;

    switch (pull_type) {
        case PIN_PULL_NONE:
            uapi_reg_clrbit16(reg_addr, pe_bit);
            break;
        case PIN_PULL_DOWN:
            uapi_reg_setbit16(reg_addr, pe_bit);
            uapi_reg_clrbit16(reg_addr, ps_bit);
            break;
        case PIN_PULL_UP:
            uapi_reg_setbit16(reg_addr, pe_bit);
            uapi_reg_setbit16(reg_addr, ps_bit);
            break;
        default:
            return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

static pin_pull_t hal_pin_brandy_get_aon_pull(pin_t pin)
{
    hal_pin_pull_map_t *pin_map = NULL;
    uint32_t ps_bit = 0;
    uint32_t reg_addr = 0;
    uint32_t pe_bit = 0;

    pin_map = hal_pin_brandy_get_pull_addr(pin);
    if (pin_map == NULL) {
        return PIN_PULL_MAX;
    }
    reg_addr = (uintptr_t)pin_map->addr + (uint32_t)(pin - pin_map->begin) * (sizeof(uint32_t));
    ps_bit = pin_map->ps_bit;
    pe_bit = pin_map->pe_bit;

    if (uapi_reg_getbits16(reg_addr, pe_bit, 1) == 0) {
        return PIN_PULL_NONE;
    } else if (uapi_reg_getbits16(reg_addr, ps_bit, 1) == 0) {
        return PIN_PULL_DOWN;
    } else {
        return PIN_PULL_UP;
    }
}

static errcode_t hal_pin_brandy_set_mode(pin_t pin, pin_mode_t mode)
{
    if (pin >= ULP_PIN) {
        return hal_pin_brandy_set_ulp_mode(pin, mode);
    } else {
        return hal_pin_brandy_set_aon_mode(pin, mode);
    }
}

static pin_mode_t hal_pin_brandy_get_mode(pin_t pin)
{
    pin_mode_t mode;

    if (pin >= ULP_PIN) {
        mode = hal_pin_brandy_get_ulp_mode(pin);
    } else {
        mode = hal_pin_brandy_get_aon_mode(pin);
    }
    return mode;
}

static errcode_t hal_pin_brandy_set_ds(pin_t pin, pin_drive_strength_t ds)
{
    return hal_pin_brandy_set_aon_ds(pin, ds);
}

static pin_drive_strength_t hal_pin_brandy_get_ds(pin_t pin)
{
    return hal_pin_brandy_get_aon_ds(pin);
}

static errcode_t hal_pin_brandy_set_pull(pin_t pin, pin_pull_t pull_type)
{
    return hal_pin_brandy_set_aon_pull(pin, pull_type);
}

static pin_pull_t hal_pin_brandy_get_pull(pin_t pin)
{
    return hal_pin_brandy_get_aon_pull(pin);
}

#if defined(CONFIG_PINCTRL_SUPPORT_IE)
STATIC errcode_t hal_pin_brandy_set_ie(pin_t pin, pin_input_enable_t ie)
{
    hal_pin_pull_map_t *pin_map = NULL;
    uint32_t reg_addr = 0;

    pin_map = hal_pin_brandy_get_pull_addr(pin);
    if (pin_map == NULL) {
        return ERRCODE_FAIL;
    }
    reg_addr = (uintptr_t)pin_map->addr + (uint32_t)(pin - pin_map->begin) * (sizeof(uint32_t));
    if (ie == PIN_IE_DISABLE) {
        reg16_clrbit(reg_addr, HAL_PIN_BRANDY_IE_CFG_BIT);
    } else if (ie == PIN_IE_ENABLE) {
        reg16_setbit(reg_addr, HAL_PIN_BRANDY_IE_CFG_BIT);
    }
    return ERRCODE_SUCC;
}

STATIC pin_input_enable_t hal_pin_brandy_get_ie(pin_t pin)
{
    hal_pin_pull_map_t *pin_map = NULL;
    uint32_t reg_addr = 0;
    uint16_t ie_enable_bit = 0;
    pin_input_enable_t ie;

    pin_map = hal_pin_brandy_get_pull_addr(pin);
    if (pin_map == NULL) {
        return PIN_IE_MAX;
    }
    reg_addr = (uintptr_t)pin_map->addr + (uint32_t)(pin - pin_map->begin) * (sizeof(uint32_t));
    ie_enable_bit = reg32_getbits(reg_addr, HAL_PIN_BRANDY_IE_CFG_BIT, 1); // ie--bit1
    if (ie_enable_bit == 0) {
        return PIN_IE_DISABLE;
    } else if (ie_enable_bit == 1) {
        return PIN_IE_ENABLE;
    } else {
        return PIN_IE_MAX;
    }
}
#endif /* CONFIG_PINCTRL_SUPPORT_IE */

static hal_pin_funcs_t g_hal_pin_brandy_funcs = {
    .set_mode = hal_pin_brandy_set_mode,
    .get_mode = hal_pin_brandy_get_mode,
    .set_ds = hal_pin_brandy_set_ds,
    .get_ds = hal_pin_brandy_get_ds,
    .set_pull = hal_pin_brandy_set_pull,
    .get_pull = hal_pin_brandy_get_pull,
#if defined(CONFIG_PINCTRL_SUPPORT_IE)
    .set_ie = hal_pin_brandy_set_ie,
    .get_ie = hal_pin_brandy_get_ie,
#endif /* CONFIG_PINCTRL_SUPPORT_IE */
};

hal_pin_funcs_t *hal_pin_brandy_funcs_get(void)
{
    return &g_hal_pin_brandy_funcs;
}
