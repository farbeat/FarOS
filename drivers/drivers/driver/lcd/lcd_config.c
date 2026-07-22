/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Lcd driver configuration.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#include <stdint.h>
#include "common_def.h"
#include "lcd_config.h"

#define LCD_EDO_466X466_SCREEN_ID  0x66 /* 根据批次调整 */
#define LCD_EDO_454X454_SCREEN_ID  0xf00
#define LCD_BOE_454X454_SCREEN_ID  0x500
#define LCD_SH_240X296_SCREEN_ID   0x101
#define LCD_BOE_194X368_SCREEN_ID  0x4a00

const combo_dev_cfg_t g_panel_466x466_60_config = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 10,   // hsa 10 pixels
        .hbp_pixels   = 10,   // hbp 10 pixels
        .hact_pixels  = 466,  // hact 454 pixels
        .hfp_pixels   = 12,   // hfp 10 pixels
        .vsa_lines    = 2,    // vsa 4 lines
        .vbp_lines    = 10,   // vbp 12 lines
        .vact_lines   = 466,  // vact 454 lines
        .vfp_lines    = 10,   // vfp 10 lines
    },
    .phy_data_rate = 480,     // 480Mbps
    .pixel_clk = 20000,       // 20000KHz
};

const combo_dev_cfg_t g_panel_454x454_60_config = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 10,   // hsa 10 pixels
        .hbp_pixels   = 10,   // hbp 10 pixels
        .hact_pixels  = 454,  // hact 454 pixels
        .hfp_pixels   = 10,   // hfp 10 pixels
        .vsa_lines    = 4,    // vsa 4 lines
        .vbp_lines    = 12,   // vbp 12 lines
        .vact_lines   = 454,  // vact 454 lines
        .vfp_lines    = 10,   // vfp 10 lines
    },
    .phy_data_rate = 480,     // 480Mbps
    .pixel_clk = 20000,       // 20000KHz
};

const combo_dev_cfg_t g_panel_240x296_60_config = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 4,   // hsa 14 pixels hpw
        .hbp_pixels   = 11,   // hbp 11 pixels HBP
        .hact_pixels  = 368,  // hact 240 pixels HPX
        .hfp_pixels   = 14,   // hfp 14 pixels hfp
        .vsa_lines    = 2,    // vsa 2 lines vpw
        .vbp_lines    = 6,   // vbp 6 lines vbp
        .vact_lines   = 448,  // vact 296 lines vpx
        .vfp_lines    = 8,   // vfp 8 lines vfp
    },
    .phy_data_rate = 480,     // 480Mbps
    .pixel_clk = 20000,       // 20000KHz
};

const combo_dev_cfg_t g_panel_194x368_qspi_config = {
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hact_pixels  = 194,  // hact 368 pixels
        .vact_lines   = 368,  // vact 368 lines
    },
};

/* display off sequence for common lcd */
const lcd_cmd_sequ g_common_cmd_display_off[] = {
    {1, 2, {0xfe, 0x00}, 0},   /* select default cmd page */
    {1, 1, {0x28}, 0},         /* display off */
    {1, 1, {0x10}, 120},       /* sleep in */
};

/* sleep in sequence for common lcd */
const lcd_cmd_sequ g_common_cmd_sleep_in[] = {
    {1, 2, {0xfe, 0x00}, 0},   /* select default cmd page */
    {1, 1, {0x28}, 0},         /* display off */
    {1, 1, {0x10}, 120},       /* sleep in */
};

/* sleep in sequence for common lcd */
const lcd_cmd_sequ g_common_cmd_sleep_out[] = {
    {1, 2, {0xfe, 0x00}, 0},   /* select default cmd page */
    {1, 1, {0x11}, 120},       /* sleep out */
    {1, 1, {0x29}, 0},         /* display on */
};

/* idle in sequence for EDO and BOE 454*454 lcd */
lcd_cmd_sequ g_common_cmd_idle_in[] = {
    {1, 2, {0xfe, 0x00}, 0},
    {1, 1, {0x39}, 0},
};

/* idle out sequence for EDO and BOE 454*454 lcd */
const lcd_cmd_sequ g_common_cmd_idle_out[] = {
    {1, 2, {0xfe, 0x00}, 0},
    {1, 1, {0x38}, 0},
};

/* te on sequence for EDO and BOE 454*454 lcd */
const lcd_cmd_sequ g_common_cmd_te_on[] = {
    {1, 2, {0xfe, 0x00}, 0},
    {1, 1, {0x35}, 0},
};

/* te off sequence for EDO and BOE 454*454 lcd */
const lcd_cmd_sequ g_common_cmd_te_off[] = {
    {1, 2, {0xfe, 0x00}, 0},
    {1, 1, {0x34}, 0},
};

/* set brightness sequence for EDO and BOE 454*454 lcd */
lcd_cmd_sequ g_common_cmd_brightness[] = {
    {1, 2, {0xfe, 0x00}, 0},
    {1, 2, {0x51, 0xff}, 0},
};

/* init sequence for EDO 454*454 lcd */
const lcd_cmd_sequ g_edo_454x454_screen_init_on[] = {
    {1, 2, {0xfe, 0x01}, 0},
    {1, 2, {0x30, 0x43}, 0}, /* set LCD frequency to 15HZ */
    {1, 2, {0xfe, 0x00}, 0},
    {1, 2, {0x3a, 0x77}, 0},
    {1, 2, {0x53, 0x20}, 0},
    {1, 2, {0x51, 0xff}, 0},
    {0, 5, {0x2a, 0x00, 0x0e, 0x01, 0xd3}, 0},
    {0, 5, {0x2b, 0x00, 0x00, 0x01, 0xc5}, 0},
    {1, 3, {0x44, 0x01, 0xc5}, 0},
    {1, 2, {0x35, 0x00}, 0},
    {1, 1, {0x11}, 120},
    {1, 1, {0x29}, 0},
};

/* init sequence for BOE 454*454 lcd */
const lcd_cmd_sequ g_boe_454x454_screen_init_on[] = {
    {1, 2, {0xfe, 0x0d}, 0},
    {1, 2, {0x01, 0x00}, 0},
    {1, 2, {0x03, 0x00}, 0},
    {1, 2, {0x1b, 0x00}, 0},
    {1, 2, {0x1d, 0x00}, 0},
    {1, 2, {0x37, 0x00}, 0},
    {1, 2, {0x39, 0x00}, 0},
    {1, 2, {0x52, 0x58}, 0},
    {1, 2, {0x53, 0x00}, 0},

    {1, 2, {0xfe, 0x01}, 0},
    {1, 2, {0x72, 0x05}, 0},
    {1, 2, {0x73, 0x16}, 0},
    {1, 2, {0x74, 0x0c}, 0},

    {1, 2, {0xfe, 0x01}, 0},
    {1, 2, {0x18, 0x55}, 0},
    {1, 2, {0xc5, 0x10}, 0},
    {1, 2, {0x72, 0x0e}, 0},
    {1, 2, {0x73, 0x1c}, 0},

    {1, 2, {0xfe, 0x07}, 0},
    {1, 2, {0x15, 0x04}, 0},
    {1, 2, {0xfe, 0x01}, 0},
    {1, 2, {0x0a, 0xf8}, 0},

    {1, 2, {0xfe, 0x00}, 0},
    {1, 2, {0x51, 0xff}, 0},
    {1, 2, {0x35, 0x00}, 0},
    {0, 5, {0x2a, 0x00, 0x0c, 0x01, 0xd1}, 0},
    {0, 5, {0x2b, 0x00, 0x00, 0x01, 0xc5}, 0},
    {1, 2, {0x53, 0x20}, 0},
    {1, 1, {0x11}, 120},
    {1, 1, {0x29}, 0},
};

const lcd_cmd_sequ g_sh_240x296_screen_init_on[] = {
    {1, 1, {0x11}, 5},
    {1, 5, {0x2a, 0x00, 0x00, 0x01, 0x6f}, 1},
    {1, 5, {0x2b, 0x00, 0x00, 0x01, 0xbf}, 1},
    {1, 3, {0x44, 0x01, 0xbf}, 1},
    {1, 2, {0x35, 0x00}, 1},
    {1, 2, {0x3a, 0x77}, 1},
    {1, 2, {0x53, 0x20}, 80},
    {1, 1, {0x29}, 0},
};

const lcd_cmd_sequ g_boe_194x368_screen_init_on[] = {
    {0, 2, {0xfe, 0x00}, 0},
    {0, 3, {0x44, 0x00, 0xB7}, 0},
    {0, 2, {0x3a, 0x77}, 0},
    {0, 2, {0x51, 0x80}, 0},
    {0, 2, {0x35, 0x0}, 0},
    {0, 5, {0x2a, 0x00, 0x00, 0x00, 0xc1}, 0},
    {0, 1, {0x11}, 120},
    {0, 1, {0x29}, 100},
    {0, 1, {0x39}, 0},
};

/* init sequence for EDO 466*466 lcd */
const lcd_cmd_sequ g_edo_466x466_screen_init_on[] = {
    { 1, 2, { 0xFE, 0x82 }, 0 },
    { 1, 2, { 0x52, 0x01 }, 0 },
    { 1, 2, { 0xFE, 0x00 }, 0 },
    { 1, 2, { 0x35, 0x00 }, 0 },
    { 1, 2, { 0x53, 0x20 }, 0 },
    { 1, 2, { 0x51, 0xFF }, 0 },
    { 1, 5, { 0x44, 0x01, 0x4D, 0x00, 0x00 }, 0 },
    { 0, 5, { 0x2A, 0x00, 0x08, 0x01, 0xD9 }, 0 },
    { 0, 5, { 0x2B, 0x00, 0x00, 0x01, 0xD1 }, 0 },
    { 1, 1, { 0x11 }, 60 },
    { 1, 1, { 0x29 }, 0 },

};

const lcd_cmds_sequ g_display_off_cmds = {
    .cmd = g_common_cmd_display_off,
    .cmd_cnt = array_size(g_common_cmd_display_off),
};

const lcd_cmds_sequ g_idle_in_cmds = {
    .cmd = g_common_cmd_idle_in,
    .cmd_cnt = array_size(g_common_cmd_idle_in),
};

const lcd_cmds_sequ g_idle_out_cmds = {
    .cmd = g_common_cmd_idle_out,
    .cmd_cnt = array_size(g_common_cmd_idle_out),
};

const lcd_cmds_sequ g_te_on_cmds = {
    .cmd = g_common_cmd_te_on,
    .cmd_cnt = array_size(g_common_cmd_te_on),
};

const lcd_cmds_sequ g_te_off_cmds = {
    .cmd = g_common_cmd_te_off,
    .cmd_cnt = array_size(g_common_cmd_te_off),
};

const lcd_cmds_sequ g_brightness_cmds = {
    .cmd = g_common_cmd_brightness,
    .cmd_cnt = array_size(g_common_cmd_brightness),
};

const lcd_cmds_sequ g_edo_454x454_screen_init_on_cmds = {
    .cmd = g_edo_454x454_screen_init_on,
    .cmd_cnt = array_size(g_edo_454x454_screen_init_on),
};

const lcd_cmds_sequ g_boe_454x454_screen_init_on_cmds = {
    .cmd = g_boe_454x454_screen_init_on,
    .cmd_cnt = array_size(g_boe_454x454_screen_init_on),
};

const lcd_cmds_sequ g_sh_240x296_screen_init_on_cmds = {
    .cmd = g_sh_240x296_screen_init_on,
    .cmd_cnt = array_size(g_sh_240x296_screen_init_on),
};

const lcd_cmds_sequ g_boe_194x368_screen_init_on_cmds = {
    .cmd = g_boe_194x368_screen_init_on,
    .cmd_cnt = array_size(g_boe_194x368_screen_init_on),
};

const lcd_cmds_sequ g_sleep_in_cmds = {
    .cmd = g_common_cmd_sleep_in,
    .cmd_cnt = array_size(g_common_cmd_sleep_in),
};

const lcd_cmds_sequ g_sleep_out_cmds = {
    .cmd = g_common_cmd_sleep_out,
    .cmd_cnt = array_size(g_common_cmd_sleep_out),
};

const lcd_cmds_sequ g_edo_466x466_screen_init_on_cmds = {
    .cmd = g_edo_466x466_screen_init_on,
    .cmd_cnt = array_size(g_edo_466x466_screen_init_on),
};

const lcd_drv_cfg g_lcd_driver_ops[] = {
    {
        .bus_type             = BUS_DISPLAY_MIPI,
        .bus_cfg              = &g_panel_466x466_60_config,
        .x_start              = 0x8,
        .y_start              = 0,
        .fps                  = 60,
        .lcd_id               = LCD_EDO_466X466_SCREEN_ID,
        .display_on_cmds      = &g_edo_466x466_screen_init_on_cmds,
        .display_off_cmds     = &g_display_off_cmds,
        .enter_idle_cmds      = &g_idle_in_cmds,
        .exit_idle_cmds       = &g_idle_out_cmds,
        .te_on_cmds           = &g_te_on_cmds,
        .te_off_cmds          = &g_te_off_cmds,
        .set_brightnes_cmds   = &g_brightness_cmds,
        .sleep_in_cmds        = &g_sleep_in_cmds,
        .sleep_out_cmds       = &g_sleep_out_cmds,
        .lcd_id_info.reg_addr = 0xdb,
        .lcd_id_info.read_len = 3,
    },
    {
        .bus_type             = BUS_DISPLAY_MIPI,
        .bus_cfg              = &g_panel_454x454_60_config,
        .x_start              = 0xe,
        .y_start              = 0,
        .fps                  = 60,
        .lcd_id               = LCD_EDO_454X454_SCREEN_ID,
        .display_on_cmds      = &g_edo_454x454_screen_init_on_cmds,
        .display_off_cmds     = &g_display_off_cmds,
        .enter_idle_cmds      = &g_idle_in_cmds,
        .exit_idle_cmds       = &g_idle_out_cmds,
        .te_on_cmds           = &g_te_on_cmds,
        .te_off_cmds          = &g_te_off_cmds,
        .set_brightnes_cmds   = &g_brightness_cmds,
        .sleep_in_cmds        = &g_sleep_in_cmds,
        .sleep_out_cmds       = &g_sleep_out_cmds,
        .lcd_id_info.reg_addr = 0x04,
        .lcd_id_info.read_len = 3,
    },
    {
        .bus_type             = BUS_DISPLAY_MIPI,
        .bus_cfg              = &g_panel_454x454_60_config,
        .x_start              = 0xc,
        .y_start              = 0,
        .fps                  = 60,
        .lcd_id               = LCD_BOE_454X454_SCREEN_ID,
        .display_on_cmds      = &g_boe_454x454_screen_init_on_cmds,
        .display_off_cmds     = &g_display_off_cmds,
        .enter_idle_cmds      = &g_idle_in_cmds,
        .exit_idle_cmds       = &g_idle_out_cmds,
        .te_on_cmds           = &g_te_on_cmds,
        .te_off_cmds          = &g_te_off_cmds,
        .set_brightnes_cmds   = &g_brightness_cmds,
        .sleep_in_cmds        = &g_sleep_in_cmds,
        .sleep_out_cmds       = &g_sleep_out_cmds,
        .lcd_id_info.reg_addr = 0x04,
        .lcd_id_info.read_len = 3,
    },
    {
        .bus_type             = BUS_DISPLAY_MIPI,
        .bus_cfg              = &g_panel_240x296_60_config,
        .x_start              = 0,
        .y_start              = 0,
        .fps                  = 60,
        .lcd_id               = LCD_SH_240X296_SCREEN_ID,
        .display_on_cmds      = &g_sh_240x296_screen_init_on_cmds,
        .display_off_cmds     = &g_display_off_cmds,
        .enter_idle_cmds      = &g_idle_in_cmds,
        .exit_idle_cmds       = &g_idle_out_cmds,
        .te_on_cmds           = &g_te_on_cmds,
        .te_off_cmds          = &g_te_off_cmds,
        .set_brightnes_cmds   = &g_brightness_cmds,
        .sleep_in_cmds        = &g_sleep_in_cmds,
        .sleep_out_cmds       = &g_sleep_out_cmds,
        .lcd_id_info.reg_addr = 0xda,
        .lcd_id_info.read_len = 3,
    },
    {
        .bus_type             = BUS_DISPLAY_QSPI,
        .bus_cfg              = &g_panel_194x368_qspi_config,
        .x_start              = 0,
        .y_start              = 0,
        .fps                  = 60,
        .lcd_id               = LCD_BOE_194X368_SCREEN_ID,
        .display_on_cmds      = &g_boe_194x368_screen_init_on_cmds,
        .display_off_cmds     = &g_display_off_cmds,
        .enter_idle_cmds      = &g_idle_in_cmds,
        .exit_idle_cmds       = &g_idle_out_cmds,
        .te_on_cmds           = &g_te_on_cmds,
        .te_off_cmds          = &g_te_off_cmds,
        .set_brightnes_cmds   = &g_brightness_cmds,
        .sleep_in_cmds        = &g_sleep_in_cmds,
        .sleep_out_cmds       = &g_sleep_out_cmds,
        .lcd_id_info.reg_addr = 0x04,
        .lcd_id_info.read_len = 3,
    }
};

lcd_drv_cfg *lcd_get_driver_ops(void)
{
    return g_lcd_driver_ops;
}

uint32_t lcd_get_ops_num(void)
{
    return array_size(g_lcd_driver_ops);
}

combo_dev_cfg_t *lcd_get_bus_escape_config(void)
{
    return g_lcd_driver_ops[0].bus_cfg;
}

lcd_drv_cfg *lcd_get_driver_config(uint32_t id)
{
    uint32_t i;
    if (id == 0) {
        return NULL;
    }

    for (i = 0; i < array_size(g_lcd_driver_ops); i++) {
        if (g_lcd_driver_ops[i].lcd_id == id) {
            return &(g_lcd_driver_ops[i]);
        }
    }

    return NULL;
}
