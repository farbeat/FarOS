/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of lcd common interface.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __LCD_COMMON_H__
#define __LCD_COMMON_H__

#include "soc_errno.h"
#include "soc_lcd_api.h"
#include "soc_mipi_tx.h"
#include "lcd_config.h"

#define MAX_LCD_ID_LEN 4
#define MAX_BRIGHT_LEN 2

typedef struct {
    ext_errno (*display_on)(uint16_t brightness);
    ext_errno (*display_off)(void);
    ext_errno (*enter_sleep_mode)(void);
    ext_errno (*exit_sleep_mode)(void);
    ext_errno (*enter_idle_mode)(void);
    ext_errno (*exit_idle_mode)(void);
    ext_errno (*te_on)(void);
    ext_errno (*te_off)(void);
    ext_errno (*set_brightness)(uint16_t value);
    ext_errno (*get_brightness)(uint16_t *value);
    ext_errno (*write)(lcd_cmd_sequ *cmds, uint32_t cmd_count);
    ext_errno (*read)(uint8_t cmd, uint8_t *buff, uint8_t buff_len);
    ext_errno (*get_chip_id)(uint32_t *id);
    lcd_drv_cfg *(*get_lcd_config)(void);
    ext_errno (*set_lcd_config)(lcd_drv_cfg *config);
    ext_errno (*chip_id_check)(void);
} lcd_drv_common;

lcd_drv_common *lcd_drv_get_common_api(void);

#endif