/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Display bus code encapsulation.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#include "lcd_bus.h"

static lcd_bus_api g_lcd_bus_api = {0};
static lcd_bus_type_t g_lcd_bus_type = BUS_DISPLAY_INVALID;

void lcd_bus_register_api(const lcd_bus_api *bus_api, lcd_bus_type_t type)
{
    if (bus_api == NULL) {
        return;
    }

    g_lcd_bus_api.bus_init = bus_api->bus_init;
    g_lcd_bus_api.bus_deinit = bus_api->bus_deinit;
    g_lcd_bus_api.bus_enable = bus_api->bus_enable;
    g_lcd_bus_api.bus_disable = bus_api->bus_disable;
    g_lcd_bus_api.bus_write_cmd = bus_api->bus_write_cmd;
    g_lcd_bus_api.bus_read_cmd = bus_api->bus_read_cmd;
    g_lcd_bus_api.bus_send_frame = bus_api->bus_send_frame;
    g_lcd_bus_api.bus_register_te_isr = bus_api->bus_register_te_isr;
    g_lcd_bus_api.bus_lp_ctrl = bus_api->bus_lp_ctrl;
    g_lcd_bus_api.bus_register_panel_info = bus_api->bus_register_panel_info;
    g_lcd_bus_api.bus_get_panel_timing = bus_api->bus_get_panel_timing;
    g_lcd_bus_api.bus_get_partial_display_cmd = bus_api->bus_get_partial_display_cmd;
    g_lcd_bus_api.bus_update_panel = bus_api->bus_update_panel;
    g_lcd_bus_api.bus_refresh_area = bus_api->bus_refresh_area,
    g_lcd_bus_api.bus_resume = bus_api->bus_resume,
    g_lcd_bus_api.bus_suspend = bus_api->bus_suspend,

    g_lcd_bus_type = type;
}

lcd_bus_api *lcd_get_bus_api(void)
{
    return &g_lcd_bus_api;
}

lcd_bus_type_t lcd_get_bus_type(void)
{
    return g_lcd_bus_type;
}
