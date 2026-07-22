/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Header file of display bus.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#ifndef __LCD_BUS_H__
#define __LCD_BUS_H__

#include "soc_mipi_tx.h"
#include "qspi_display.h"
#include "soc_lcd_api.h"
#include "hal_gpio.h"
#include "td_base.h"

typedef enum {
    LP_CTRL_ENTER_SDLP,
    LP_CTRL_EXIT_SDLP,
    LP_CTRL_EXIT_ULPS,
    LP_CTRL_CLEAR_EXIT_ULPS,
    LP_CTRL_MAX
} lcd_bus_lp_ctrl_t;

typedef struct {
    bool is_cmd_mode;
    uint32_t width;
    uint32_t height;
    sync_info_t sync_info;
    uint32_t refresh_rate;
    uint32_t pixel_clk;
} bus_panel_info_t;

typedef struct {
    uint32_t cmd_set_col_h;
    uint32_t set_col_p0;
    uint32_t set_col_p1;
    uint32_t cmd_set_raw_h;
    uint32_t set_raw_p0;
    uint32_t set_raw_p1;
} partial_display_cmd_t;

typedef enum {
    PANEL_UPDATE_TIMING,
    PANEL_UPDATE_SCREEN,
    PANEL_DISPLAY_REFRESH, /* update timing and screen */
    PANEL_OPT_MAX,
} panel_option_t;

typedef struct {
    ext_errno (*bus_init)(combo_dev_cfg_t *bus_cfg);
    ext_errno (*bus_deinit)(void);
    ext_errno (*bus_enable)(void);
    ext_errno (*bus_disable)(void);
    ext_errno (*bus_write_cmd)(lcd_cmd_sequ *cmd);
    ext_errno (*bus_read_cmd)(uint32_t cmd, uint8_t *buff, uint8_t len);
    ext_errno (*bus_send_frame)(uint32_t addr, qspi_send_frame_cb_t callback);
    ext_errno (*bus_register_te_isr)(gpio_callback_t callback);
    ext_errno (*bus_lp_ctrl)(lcd_bus_lp_ctrl_t mode);
    void (*bus_register_panel_info)(uint16_t x_start, uint16_t y_start, uint32_t fps);
    ext_errno (*bus_get_panel_timing)(const ext_rect *rect, bus_panel_info_t *info);
    ext_errno (*bus_get_partial_display_cmd)(const ext_rect *rect, partial_display_cmd_t *cmd);
    ext_errno (*bus_update_panel)(panel_option_t opt);
    ext_errno (*bus_refresh_area)(const ext_rect *rect);
    ext_errno (*bus_suspend)(void);
    ext_errno (*bus_resume)(void);
} lcd_bus_api;

void lcd_bus_register_api(const lcd_bus_api *bus_api, lcd_bus_type_t type);
lcd_bus_api *lcd_get_bus_api(void);
lcd_bus_type_t lcd_get_bus_type(void);
#endif
