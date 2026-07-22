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
#include "test_screen_weitai.h"

combo_dev_cfg_t g_test_400x1600_60_rgb24_burst = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 30,   /* hsa 18 pixels */
        .hbp_pixels   = 50,   /* hbp 18 pixels */
        .hact_pixels  = 400,  /* hact 800 pixels */
        .hfp_pixels   = 50,   /* hfp 18 pixels */
        .vsa_lines    = 20,   /* vsa 4 lines */
        .vbp_lines    = 30,   /* vbp 10 lines */
        .vact_lines   = 1600, /* vact 1280 lines */
        .vfp_lines    = 30,   /* vfp 20 lines */
    },
    .phy_data_rate = 642,     /* phy_data_rate 642MHz */
    .pixel_clk = 53424,       /* pixel_clk 53424KHz */
};

combo_dev_cfg_t g_test_400x1600_60_rgb24_non_burst_sync_pulses = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  NON_BURST_MODE_SYNC_PULSES,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 30,   /* hsa 18 pixels */
        .hbp_pixels   = 50,   /* hbp 18 pixels */
        .hact_pixels  = 400,  /* hact 800 pixels */
        .hfp_pixels   = 50,   /* hfp 18 pixels */
        .vsa_lines    = 20,   /* vsa 4 lines */
        .vbp_lines    = 30,   /* vbp 10 lines */
        .vact_lines   = 1600, /* vact 1280 lines */
        .vfp_lines    = 30,   /* vfp 20 lines */
    },
    .phy_data_rate = 642,     /* phy_data_rate 642MHz */
    .pixel_clk = 53424,       /* pixel_clk 53424KHz */
};

combo_dev_cfg_t g_test_400x1600_60_rgb24_non_burst_sync_events = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  NON_BURST_MODE_SYNC_EVENTS,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 30,   /* hsa 18 pixels */
        .hbp_pixels   = 50,   /* hbp 18 pixels */
        .hact_pixels  = 400,  /* hact 800 pixels */
        .hfp_pixels   = 50,   /* hfp 18 pixels */
        .vsa_lines    = 20,   /* vsa 4 lines */
        .vbp_lines    = 30,   /* vbp 10 lines */
        .vact_lines   = 1600, /* vact 1280 lines */
        .vfp_lines    = 30,   /* vfp 20 lines */
    },
    .phy_data_rate = 642,     /* phy_data_rate 642MHz */
    .pixel_clk = 53424,       /* pixel_clk 53424KHz */
};

combo_dev_cfg_t *g_400x1600_mipi_sequ[] = {
    &g_test_400x1600_60_rgb24_burst,
    &g_test_400x1600_60_rgb24_non_burst_sync_pulses,
    &g_test_400x1600_60_rgb24_non_burst_sync_events,
};

test_cmd_sequ g_weitai_screen_init_on[] = {
    {1, 2, {0xB0, 0x5A}, 0},
    {1, 2, {0xB1, 0x00}, 0},
    {1, 2, {0x89, 0x01}, 0},
    {1, 2, {0x91, 0x17}, 0},
    {1, 2, {0xB1, 0x03}, 0},
    {1, 2, {0x2C, 0x28}, 0},
    {1, 2, {0x14, 0xE5}, 0},
    {1, 2, {0x0B, 0x04}, 0},
    {1, 2, {0xB1, 0x03}, 0},
    {1, 2, {0x2C, 0x2C}, 0},
    {1, 2, {0xB1, 0x00}, 0},
    {1, 2, {0x89, 0x03}, 0},
    {1, 1, {0x11}, 100},
    {1, 1, {0x29}, 0},
};

static int32_t weitai_lcd_display_init(combo_dev_cfg_t *attr)
{
    unused(attr);
    int32_t ret;
    uint32_t i;
    test_cmd_sequ *cmd_desc = g_weitai_screen_init_on;
    uint32_t cmd_cnt = lcd_array_size(g_weitai_screen_init_on);

    for (i = 0; i < cmd_cnt; i++) {
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

test_drv_api g_weitai_api = {
    .lcd_init = weitai_lcd_display_init,
};

combo_dev_cfg_t *weitai_get_dev_cfg(int index)
{
    return g_400x1600_mipi_sequ[index];
}

test_drv_api *weitai_get_drv_api(void)
{
    return &g_weitai_api;
}