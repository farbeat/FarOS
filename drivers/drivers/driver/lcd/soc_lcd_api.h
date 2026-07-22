/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of lcd driver.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __SOC_LCD_API_H__
#define __SOC_LCD_API_H__

#include "stdint.h"
#include "stdbool.h"
#include "product.h"
#include "soc_errno.h"
#include "soc_mipi_tx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DATA_LENGTH 5

typedef enum {
    LCD_DISPLAY_OFF,     /* LCD关闭显示 */
    LCD_NORMAL_DISPLAY,  /* LCD正常显示模式 */
    LCD_IDLE_MODE,       /* LCD空闲模式，显示效果变差，只显示8种颜色 */
    LCD_SLEEP_MODE,      /* LCD睡眠模式 */
} lcd_display_status;

typedef enum {
    LCD_TE_ON = 1,  /* LCD开启TE */
    LCD_TE_OFF,     /* LCD关闭TE */
} lcd_te_status;

typedef struct {
    lcd_display_status disp_status;
    lcd_te_status te_status;
} ext_lcd_status;

typedef struct lcd_cmd_sequ {
    uint8_t  dcs_flag; /* 当前数据是否通过DCS协议命令下发，为0表示不是DCS命令 */
    uint8_t  data_len; /* payload length, 小于等于64 */
    uint8_t  data[MAX_DATA_LENGTH]; /* data内包含cmd和参数，有效数据最短为1（例如0x11），最长为5 */
    uint16_t delay_ms; /* 发送命令后等待时间，单位ms */
} lcd_cmd_sequ;

typedef struct lcd_cmds_sequ {
    lcd_cmd_sequ *cmd; /* dsi命令数组 */
    uint16_t cmd_cnt; /* dsi命令个数 */
} lcd_cmds_sequ;

typedef struct {
    uint8_t reg_addr;  /* lcd寄存器地址 */
    uint16_t read_len; /* 读取寄存器数据长度 */
} ext_lcd_read_info;

typedef enum {
    BUS_DISPLAY_MIPI = 1,
    BUS_DISPLAY_QSPI,
    BUS_DISPLAY_INVALID
} lcd_bus_type_t;

typedef struct {
    lcd_bus_type_t bus_type;
    combo_dev_cfg_t *bus_cfg;
    uint16_t x_start;
    uint16_t y_start;
    uint16_t fps;
    uint32_t lcd_id;
    uint16_t ddic_id;
    lcd_cmds_sequ *display_on_cmds;
    lcd_cmds_sequ *display_off_cmds;
    lcd_cmds_sequ *enter_idle_cmds;
    lcd_cmds_sequ *exit_idle_cmds;
    lcd_cmds_sequ *te_on_cmds;
    lcd_cmds_sequ *te_off_cmds;
    lcd_cmds_sequ *set_brightnes_cmds;
    lcd_cmds_sequ *sleep_in_cmds;
    lcd_cmds_sequ *sleep_out_cmds;
    ext_lcd_read_info lcd_id_info;
} lcd_drv_cfg;

ext_errno uapi_lcd_info_register(lcd_drv_cfg *info);

ext_errno uapi_lcd_init(void);

ext_errno uapi_lcd_deinit(void);

ext_lcd_status uapi_lcd_get_status(void);

bool uapi_is_lcd_connect(void);

ext_errno uapi_lcd_display_on(void);

ext_errno uapi_lcd_display_off(void);

ext_errno uapi_lcd_enter_sleep_mode(void);

ext_errno uapi_lcd_exit_sleep_mode(void);

ext_errno uapi_lcd_enter_idle_mode(void);

ext_errno uapi_lcd_exit_idle_mode(void);

ext_errno uapi_lcd_te_on(void);

ext_errno uapi_lcd_te_off(void);

ext_errno uapi_lcd_set_brightness(uint16_t value);

ext_errno uapi_lcd_get_brightness(uint16_t *value);

ext_errno uapi_lcd_write_reg(lcd_cmd_sequ *cmds, uint32_t cmd_count);

ext_errno uapi_lcd_read_reg(uint8_t cmd, uint8_t *buff, uint8_t buff_len);

ext_errno uapi_lcd_bus_enter_sdlp(void);

ext_errno uapi_lcd_bus_exit_sdlp(void);

ext_errno uapi_lcd_bus_exit_ulps(void);

ext_errno uapi_lcd_bus_clear_exit_ulps(void);

bool lcd_get_status(void);

void lcd_set_status(bool lcd_on);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif