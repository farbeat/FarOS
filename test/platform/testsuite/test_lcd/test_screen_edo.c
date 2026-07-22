/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include "soc_mipi_tx.h"
#include "soc_errno.h"
#include "mipi_tx.h"
#include "test_screen_lcd.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "debug_print.h"
#include "test_screen_edo.h"

combo_dev_cfg_t g_test_454x454_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   // hsa 20 pixels
        .hbp_pixels   = 40,   // hbp 40 pixels
        .hact_pixels  = 454,  // hact 454 pixels
        .hfp_pixels   = 20,   // hfp 20 pixels
        .vsa_lines    = 4,    // vsa 4 lines
        .vbp_lines    = 12,   // vbp 12 lines
        .vact_lines   = 454,  // vact 454 lines
        .vfp_lines    = 20,   // vfp 20 lines
    },
    .phy_data_rate = 377,     // 377Mbps
    .pixel_clk = 15700,       // 15700KHz
};

combo_dev_cfg_t g_test_454x454_60_rgb18_loosely = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_18BIT_LOOSELY,
    .sync_info = {
        .hsa_pixels   = 20,   // hsa 20 pixels
        .hbp_pixels   = 40,   // hbp 40 pixels
        .hact_pixels  = 454,  // hact 454 pixels
        .hfp_pixels   = 20,   // hfp 20 pixels
        .vsa_lines    = 4,    // vsa 4 lines
        .vbp_lines    = 12,   // vbp 12 lines
        .vact_lines   = 454,  // vact 454 lines
        .vfp_lines    = 20,   // vfp 20 lines
    },
    .phy_data_rate = 377,     // 377Mbps
    .pixel_clk = 15700,       // 15700KHz
};

combo_dev_cfg_t g_test_454x454_60_rgb18 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_18BIT,
    .sync_info = {
        .hsa_pixels   = 20,   // hsa 20 pixels
        .hbp_pixels   = 40,   // hbp 40 pixels
        .hact_pixels  = 454,  // hact 454 pixels
        .hfp_pixels   = 20,   // hfp 20 pixels
        .vsa_lines    = 4,    // vsa 4 lines
        .vbp_lines    = 12,   // vbp 12 lines
        .vact_lines   = 454,  // vact 454 lines
        .vfp_lines    = 20,   // vfp 20 lines
    },
    .phy_data_rate = 360,     // 360Mbps
    .pixel_clk = 15700,       // 15700KHz
};

combo_dev_cfg_t g_test_454x454_60_rgb16 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_16BIT,
    .sync_info = {
        .hsa_pixels   = 20,   // hsa 20 pixels
        .hbp_pixels   = 40,   // hbp 40 pixels
        .hact_pixels  = 454,  // hact 454 pixels
        .hfp_pixels   = 20,   // hfp 20 pixels
        .vsa_lines    = 4,    // vsa 4 lines
        .vbp_lines    = 12,   // vbp 12 lines
        .vact_lines   = 454,  // vact 454 lines
        .vfp_lines    = 20,   // vfp 20 lines
    },
    .phy_data_rate = 324,     // 324Mbps
    .pixel_clk = 15700,       // 15700KHz
};

combo_dev_cfg_t *g_454x454_mipi_sequ[] = {
    &g_test_454x454_60_rgb24,
    &g_test_454x454_60_rgb18_loosely,
    &g_test_454x454_60_rgb18,
    &g_test_454x454_60_rgb16,
};

test_cmd_sequ g_boe_edo_screen_init_on[] = {
    {1, 2, {0xfe, 0x07}, 0},
    {1, 2, {0x15, 0x04}, 0},
    {1, 2, {0xfe, 0x00}, 0},
    {1, 2, {0x3a, 0x77}, 0},
    {1, 2, {0x53, 0x20}, 0},
    {1, 2, {0x51, 0xff}, 0},
    {0, 5, {0x2a, 0x00, 0x0e, 0x01, 0xd3}, 0},
    {0, 5, {0x2b, 0x00, 0x00, 0x01, 0xc5}, 0},
    {1, 2, {0x35, 0x00}, 0},
    {1, 1, {0x11}, 120},
    {1, 1, {0x29}, 0},
};

static int32_t edo_lcd_display_init(combo_dev_cfg_t *attr)
{
    int32_t ret;
    uint32_t i;
    test_cmd_sequ *cmd_desc = g_boe_edo_screen_init_on;
    uint32_t cmd_cnt = lcd_array_size(g_boe_edo_screen_init_on);
    if (attr == NULL) {
        PRINT("err:edo lcd attr is empty!\r\n");
        return EXT_ERR_FAILURE;
    }

    for (i = 0; i < cmd_cnt; i++) {
        if (cmd_desc[i].data[0] == 0x3a) {
            if (attr->out_format == OUT_FORMAT_RGB_16BIT) {
                cmd_desc[i].data[1] = 0x75; /* rgb16 */
            } else if (attr->out_format == OUT_FORMAT_RGB_18BIT) {
                cmd_desc[i].data[1] = 0x76; /* rgb18 */
            } else {
                cmd_desc[i].data[1] = 0x77; /* rgb24, rgb18 loosely */
            }
        }

        if (cmd_desc[i].dcs_flag != 0) {
            ret = test_dcs_cmd_send(&(cmd_desc[i]));
            if (ret != EXT_ERR_SUCCESS) {
                return ret;
            }
        } else {
            ret = test_dcs_cmd_send(&(cmd_desc[i]));
            if (ret != EXT_ERR_SUCCESS) {
                return ret;
            }
        }
    }

    return ret;
}

test_drv_api g_edo_api = {
    .lcd_init = edo_lcd_display_init,
};

combo_dev_cfg_t *edo_get_dev_cfg(int index)
{
    return g_454x454_mipi_sequ[index];
}

test_drv_api *edo_get_drv_api(void)
{
    return &g_edo_api;
}
