/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include "soc_mipi_tx.h"
#include "soc_errno.h"
#include "mipi_tx.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "debug_print.h"
#include "test_screen_kdk.h"

combo_dev_cfg_t g_test_800x1280_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 816,     /* phy_data_rate 816MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb18_loosely = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_18BIT_LOOSELY,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 816,     /* phy_data_rate 816MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb18 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_18BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 612,     /* phy_data_rate 612MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb16 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_16BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 544,     /* phy_data_rate 544MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb16_1p1 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_16BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 593,     /* phy_data_rate 593MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb24_1p1 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 889,     /* phy_data_rate 889MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb24_1300m = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 18,   /* hsa 18 pixels */
        .hbp_pixels   = 18,   /* hbp 18 pixels */
        .hact_pixels  = 800,  /* hact 800 pixels */
        .hfp_pixels   = 18,   /* hfp 18 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 10,   /* vbp 10 lines */
        .vact_lines   = 1280, /* vact 1280 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 1300,    /* phy_data_rate 1300MHz */
    .pixel_clk = 67330,       /* pixel_clk 67330KHz */
};

combo_dev_cfg_t *g_800x1280_mipi_sequ[] = {
    &g_test_800x1280_60_rgb24,
    &g_test_800x1280_60_rgb18_loosely,
    &g_test_800x1280_60_rgb18,
    &g_test_800x1280_60_rgb16,
    &g_test_800x1280_60_rgb16_1p1,
    &g_test_800x1280_60_rgb24_1p1,
    &g_test_800x1280_60_rgb24_1300m,
};

test_cmd_sequ g_kdk_screen_init_on[] = {
    {0, 2, {0xe0, 0x00}, 0},
    {0, 2, {0xe1, 0x93}, 0},
    {0, 2, {0xe2, 0x65}, 0},
    {0, 2, {0xe3, 0xf8}, 0},
    {0, 2, {0x80, 0x01}, 0},
    {0, 2, {0xe0, 0x01}, 0},
    {0, 2, {0x00, 0x00}, 0},
    {0, 2, {0x01, 0xa9}, 0},
    {0, 2, {0x03, 0x00}, 0},
    {0, 2, {0x04, 0xa8}, 0},
    {0, 2, {0x0e, 0x00}, 0},
    {0, 2, {0x17, 0x00}, 0},
    {0, 2, {0x18, 0xb1}, 0},
    {0, 2, {0x19, 0x01}, 0},
    {0, 2, {0x1a, 0x00}, 0},
    {0, 2, {0x1b, 0xb1}, 0},
    {0, 2, {0x1c, 0x01}, 0},
    {0, 2, {0x1f, 0x3e}, 0},
    {0, 2, {0x20, 0x2d}, 0},
    {0, 2, {0x21, 0x2d}, 0},
    {0, 2, {0x22, 0x7e}, 0},
    {0, 2, {0x37, 0x19}, 0},
    {0, 2, {0x38, 0x05}, 0},
    {0, 2, {0x39, 0x00}, 0},
    {0, 2, {0x3a, 0x01}, 0},
    {0, 2, {0x3c, 0x78}, 0},
    {0, 2, {0x3d, 0xff}, 0},
    {0, 2, {0x3e, 0xff}, 0},
    {0, 2, {0x3f, 0xff}, 0},
    {0, 2, {0x40, 0x06}, 0},
    {0, 2, {0x41, 0xa0}, 0},
    {0, 2, {0x43, 0x08}, 0},
    {0, 2, {0x44, 0x09}, 0},
    {0, 2, {0x45, 0x28}, 0},
    {0, 2, {0x55, 0x0f}, 0},
    {0, 2, {0x57, 0x89}, 0},
    {0, 2, {0x58, 0x0a}, 0},
    {0, 2, {0x59, 0x0a}, 0},
    {0, 2, {0x5a, 0x28}, 0},
    {0, 2, {0x5b, 0x19}, 0},
    {0, 2, {0x5d, 0x7c}, 0},
    {0, 2, {0x5e, 0x66}, 0},
    {0, 2, {0x5f, 0x56}, 0},
    {0, 2, {0x60, 0x49}, 0},
    {0, 2, {0x61, 0x44}, 0},
    {0, 2, {0x62, 0x34}, 0},
    {0, 2, {0x63, 0x37}, 0},
    {0, 2, {0x64, 0x1f}, 0},
    {0, 2, {0x65, 0x36}, 0},
    {0, 2, {0x66, 0x33}, 0},
    {0, 2, {0x67, 0x32}, 0},
    {0, 2, {0x68, 0x4f}, 0},
    {0, 2, {0x69, 0x3d}, 0},
    {0, 2, {0x6a, 0x43}, 0},
    {0, 2, {0x6b, 0x35}, 0},
    {0, 2, {0x6c, 0x30}, 0},
    {0, 2, {0x6d, 0x24}, 0},
    {0, 2, {0x6e, 0x13}, 0},
    {0, 2, {0x6f, 0x00}, 0},
    {0, 2, {0x70, 0x7c}, 0},
    {0, 2, {0x71, 0x66}, 0},
    {0, 2, {0x72, 0x56}, 0},
    {0, 2, {0x73, 0x49}, 0},
    {0, 2, {0x74, 0x44}, 0},
    {0, 2, {0x75, 0x34}, 0},
    {0, 2, {0x76, 0x37}, 0},
    {0, 2, {0x77, 0x1f}, 0},
    {0, 2, {0x78, 0x36}, 0},
    {0, 2, {0x79, 0x33}, 0},
    {0, 2, {0x7a, 0x32}, 0},
    {0, 2, {0x7b, 0x4f}, 0},
    {0, 2, {0x7c, 0x3d}, 0},
    {0, 2, {0x7d, 0x43}, 0},
    {0, 2, {0x7e, 0x35}, 0},
    {0, 2, {0x7f, 0x30}, 0},
    {0, 2, {0x80, 0x24}, 0},
    {0, 2, {0x81, 0x13}, 0},
    {0, 2, {0x82, 0x00}, 0},
    {0, 2, {0xe0, 0x02}, 0},
    {0, 2, {0x00, 0x45}, 0},
    {0, 2, {0x01, 0x45}, 0},
    {0, 2, {0x02, 0x44}, 0},
    {0, 2, {0x03, 0x44}, 0},
    {0, 2, {0x04, 0x47}, 0},
    {0, 2, {0x05, 0x47}, 0},
    {0, 2, {0x06, 0x46}, 0},
    {0, 2, {0x07, 0x46}, 0},
    {0, 2, {0x08, 0x40}, 0},
    {0, 2, {0x09, 0x1f}, 0},
    {0, 2, {0x0a, 0x1f}, 0},
    {0, 2, {0x0b, 0x1f}, 0},
    {0, 2, {0x0c, 0x1f}, 0},
    {0, 2, {0x0d, 0x1f}, 0},
    {0, 2, {0x0e, 0x1f}, 0},
    {0, 2, {0x0f, 0x41}, 0},
    {0, 2, {0x10, 0x1f}, 0},
    {0, 2, {0x11, 0x1f}, 0},
    {0, 2, {0x12, 0x1f}, 0},
    {0, 2, {0x13, 0x1f}, 0},
    {0, 2, {0x14, 0x1f}, 0},
    {0, 2, {0x15, 0x1f}, 0},
    {0, 2, {0x16, 0x45}, 0},
    {0, 2, {0x17, 0x45}, 0},
    {0, 2, {0x18, 0x44}, 0},
    {0, 2, {0x19, 0x44}, 0},
    {0, 2, {0x1a, 0x47}, 0},
    {0, 2, {0x1b, 0x47}, 0},
    {0, 2, {0x1c, 0x46}, 0},
    {0, 2, {0x1d, 0x46}, 0},
    {0, 2, {0x1e, 0x40}, 0},
    {0, 2, {0x1f, 0x1f}, 0},
    {0, 2, {0x20, 0x1f}, 0},
    {0, 2, {0x20, 0x1f}, 0},
    {0, 2, {0x21, 0x1f}, 0},
    {0, 2, {0x22, 0x1f}, 0},
    {0, 2, {0x23, 0x1f}, 0},
    {0, 2, {0x24, 0x1f}, 0},
    {0, 2, {0x25, 0x41}, 0},
    {0, 2, {0x26, 0x1f}, 0},
    {0, 2, {0x27, 0x1f}, 0},
    {0, 2, {0x28, 0x1f}, 0},
    {0, 2, {0x29, 0x1f}, 0},
    {0, 2, {0x2a, 0x1f}, 0},
    {0, 2, {0x2b, 0x1f}, 0},
    {0, 2, {0x58, 0x40}, 0},
    {0, 2, {0x5b, 0x10}, 0},
    {0, 2, {0x5c, 0x06}, 0},
    {0, 2, {0x5d, 0x20}, 0},
    {0, 2, {0x5e, 0x01}, 0},
    {0, 2, {0x5f, 0x02}, 0},
    {0, 2, {0x63, 0x62}, 0},
    {0, 2, {0x64, 0x62}, 0},
    {0, 2, {0x67, 0x32}, 0},
    {0, 2, {0x68, 0x08}, 0},
    {0, 2, {0x69, 0x62}, 0},
    {0, 2, {0x6a, 0x66}, 0},
    {0, 2, {0x6b, 0x08}, 0},
    {0, 2, {0x6c, 0x00}, 0},
    {0, 2, {0x6d, 0x04}, 0},
    {0, 2, {0x6e, 0x04}, 0},
    {0, 2, {0x6f, 0x88}, 0},
    {0, 2, {0xe0, 0x03}, 0},
    {0, 2, {0x98, 0x3f}, 0},
    {0, 2, {0xe0, 0x04}, 0},
    {0, 2, {0x09, 0x10}, 0},
    {0, 2, {0x2b, 0x2b}, 0},
    {0, 2, {0x2d, 0x03}, 0},
    {0, 2, {0x2e, 0x44}, 0},
    {0, 2, {0xe0, 0x00}, 0},
    {0, 2, {0xe6, 0x02}, 0},
    {0, 2, {0xe7, 0x06}, 0},
    {1, 1, {0x11}, 100},
    {1, 1, {0x29}, 0},
};

static int32_t kdk_lcd_display_init(combo_dev_cfg_t *attr)
{
    unused(attr);
    int32_t ret;
    uint32_t i;
    test_cmd_sequ *cmd_desc = g_kdk_screen_init_on;
    uint32_t cmd_cnt = lcd_array_size(g_kdk_screen_init_on);

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

test_drv_api g_kdk_api = {
    .lcd_init = kdk_lcd_display_init,
};

combo_dev_cfg_t *kdk_get_dev_cfg(int index)
{
    return g_800x1280_mipi_sequ[index];
}

test_drv_api *kdk_get_drv_api(void)
{
    return &g_kdk_api;
}