/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of lcd driver configuration.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__

#include "soc_lcd_api.h"
#include "soc_mipi_tx.h"
#include "debug_print.h"

#define lcd_err(x, ...) \
    do { \
        PRINT("lcd:"x, ##__VA_ARGS__); \
    } while (0)

#define lcd_print(fmt, ...)            \
    do {                                \
        PRINT(fmt, ##__VA_ARGS__);     \
    } while (0)

#define DEFAULT_BRIGHTNESS 255

typedef enum {
    VIDEO_LCD = 1,
    COMMAND_LCD = 2,
} lcd_type;

lcd_drv_cfg *lcd_get_driver_ops(void);
uint32_t lcd_get_ops_num(void);
combo_dev_cfg_t *lcd_get_bus_escape_config(void);
lcd_drv_cfg *lcd_get_driver_config(uint32_t id);

#endif
