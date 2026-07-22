/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef TEST_SCREEN_LCD_H
#define TEST_SCREEN_LCD_H

#include "soc_mipi_tx.h"
#include "test_screen_config.h"

#ifndef lcd_array_size
#define lcd_array_size(x) ((sizeof(x)) / (sizeof((x)[0])))
#endif /* lcd_array_size */

typedef struct test_cmd_sequ {
    uint8_t dcs_flag; /* 当前数据是否通过DCS协议命令下发，为0表示不是DCS命令 */
    uint8_t data_len; /* payload length, 小于等于64 */
    uint8_t data[64]; /* data内包含cmd和参数，有效数据最短为1（例如11），最长为64 */
    uint16_t delay_ms; /* wait ms after cmd sent */
} test_cmd_sequ;

typedef struct {
    int32_t (*lcd_init)(combo_dev_cfg_t *attr);
} test_drv_api;

typedef struct {
    combo_dev_cfg_t *mipi_attr;
    test_drv_api *drv_api;
} test_lcd_cfg;

int32_t test_dcs_cmd_send(test_cmd_sequ *dcs_cmd);

int32_t test_generic_cmd_send(test_cmd_sequ *generic_cmd);

int32_t test_lcd_assigned_init(lcd_sel lcd_num, int output_mode);

int32_t test_lcd_assigned_deinit(void);

#endif