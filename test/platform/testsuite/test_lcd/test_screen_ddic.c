/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include "soc_mipi_tx.h"
#include "mipi_tx.h"
#include "test_screen_lcd.h"
#include "debug_print.h"
#include "test_screen_ddic.h"

combo_dev_cfg_t g_test_1280x800_60_rgb24_video_ddic = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 885,     /* phy_data_rate 885MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_test_120x240_60_rgb24_video = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 120,  /* hact 120 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 12 lines */
        .vact_lines   = 240, /* vact 240 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 60,     /* phy_data_rate 60MHz */
    .pixel_clk = 3312,       /* pixel_clk 3312KHz */
};

combo_dev_cfg_t g_test_1280x800_60_rgb24_events = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  NON_BURST_MODE_SYNC_EVENTS,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 885,     /* phy_data_rate 885MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_test_1280x800_60_rgb24_pulses = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  NON_BURST_MODE_SYNC_PULSES,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 885,     /* phy_data_rate 885MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_test_120x240_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 120,  /* hact 120 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 12 lines */
        .vact_lines   = 240, /* vact 240 lines */
        .vfp_lines    = 20,   /* vfp 20 lines */
    },
    .phy_data_rate = 84,     /* phy_data_rate 84MHz */
    .pixel_clk = 3312,       /* pixel_clk 3312KHz */
};

combo_dev_cfg_t g_test_1280x800_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 885,     /* phy_data_rate 885MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_test_280x456_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 280, /* hact 280 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 456,  /* vact 456 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 264,     /* phy_data_rate 264 MHz */
    .pixel_clk = 10628,       /* pixel_clk 10628 KHz */
};

combo_dev_cfg_t g_test_240x432_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 240, /* hact 240 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 432,  /* vact 432 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 216,     /* phy_data_rate 885 MHz */
    .pixel_clk = 8986,       /* pixel_clk 8986 KHz */
};

combo_dev_cfg_t g_test_390x390_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 390, /* hact 390 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 390,  /* vact 390 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 150,     /* phy_data_rate 150 MHz */
    .pixel_clk = 12014,       /* pixel_clk 8986 KHz */
};

combo_dev_cfg_t g_test_600x600_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 600, /* hact 600 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 600,  /* vact 600 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 624,     /* phy_data_rate 624 MHz */
    .pixel_clk = 25949,       /* pixel_clk 25949 KHz */
};

combo_dev_cfg_t g_test_500x500_60_rgb24 = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 500, /* hact 500 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 500,  /* vact 500 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 456,     /* phy_data_rate 456 MHz */
    .pixel_clk = 18653,       /* pixel_clk 18653 KHz */
};

combo_dev_cfg_t g_test_800x1280_60_rgb24_1p3g = {
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
    .phy_data_rate = 1300,     /* phy_data_rate 1300MHz */
    .pixel_clk = 89773,       /* pixel_clk 89773KHz */
};

combo_dev_cfg_t g_test_454x454_60_rgb24_ddic = {
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

combo_dev_cfg_t g_test_480x800_60_rgb24_ddic = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 10,   // hsa 10 pixels
        .hbp_pixels   = 20,   // hbp 20 pixels
        .hact_pixels  = 480,  // hact 480 pixels
        .hfp_pixels   = 20,   // hfp 20 pixels
        .vsa_lines    = 2,    // vsa 2 lines
        .vbp_lines    = 14,   // vbp 14 lines
        .vact_lines   = 800,  // vact 800 lines
        .vfp_lines    = 16,   // vfp 16 lines
    },
    .phy_data_rate = 318,     // 318Mbps
    .pixel_clk = 26458,       // 26458KHz
};

combo_dev_cfg_t g_test_390x390_60_rgb24_318m_ddic = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 390, /* hact 390 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 390,  /* vact 390 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 318,     // 318Mbps
    .pixel_clk = 26458,       // 26458KHz
};

combo_dev_cfg_t g_test_600x600_60_rgb24_cmd_2lane = {
    .devno = 0,
    .lane_id = {0, -1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 20,   /* hsa 20 pixels */
        .hbp_pixels   = 40,   /* hbp 40 pixels */
        .hact_pixels  = 600, /* hact 600 pixels */
        .hfp_pixels   = 20,   /* hfp 20 pixels */
        .vsa_lines    = 4,    /* vsa 4 lines */
        .vbp_lines    = 12,   /* vbp 14 lines */
        .vact_lines   = 600,  /* vact 600 lines */
        .vfp_lines    = 20,    /* vfp 20 lines */
    },
    .phy_data_rate = 624,     /* phy_data_rate 624 MHz */
    .pixel_clk = 25949,       /* pixel_clk 25949 KHz */
};

combo_dev_cfg_t g_test_1280x800_60_rgb24_1g = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_CMD,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 1000,    /* phy_data_rate 1000MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_test_1280x800_60_rgb24_video_1g = {
    .devno = 0,
    .lane_id = {0, 1},
    .out_mode = OUT_MODE_DSI_VIDEO,
    .video_mode =  BURST_MODE,
    .out_format = OUT_FORMAT_RGB_24BIT,
    .sync_info = {
        .hsa_pixels   = 32,   /* hsa 32 pixels */
        .hbp_pixels   = 80,   /* hbp 80 pixels */
        .hact_pixels  = 1280, /* hact 1280 pixels */
        .hfp_pixels   = 48,   /* hfp 18 pixels */
        .vsa_lines    = 6,    /* vsa 6 lines */
        .vbp_lines    = 14,   /* vbp 14 lines */
        .vact_lines   = 800,  /* vact 800 lines */
        .vfp_lines    = 3,    /* vfp 3 lines */
    },
    .phy_data_rate = 1000,    /* phy_data_rate 1000MHz */
    .pixel_clk = 71108,       /* pixel_clk 71108KHz */
};

combo_dev_cfg_t g_panel_454x454_60_config_1g = {
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
    .phy_data_rate = 1000,    // 1000Mbps
    .pixel_clk = 20000,       // 20000KHz
};

combo_dev_cfg_t *g_ddic_mipi_sequ[] = {
    &g_test_1280x800_60_rgb24_video_ddic,
    &g_test_120x240_60_rgb24_video,
    &g_test_1280x800_60_rgb24_events,
    &g_test_1280x800_60_rgb24_pulses,
    &g_test_120x240_60_rgb24,
    &g_test_1280x800_60_rgb24,
    &g_test_280x456_60_rgb24,
    &g_test_240x432_60_rgb24,
    &g_test_390x390_60_rgb24,
    &g_test_600x600_60_rgb24,
    &g_test_500x500_60_rgb24,
    &g_test_800x1280_60_rgb24_1p3g,
    &g_test_480x800_60_rgb24_ddic,
    &g_test_390x390_60_rgb24_318m_ddic,
    &g_test_1280x800_60_rgb24_1g,
    &g_test_1280x800_60_rgb24_video_1g,
    &g_panel_454x454_60_config_1g,
};

test_drv_api g_ddic_api = {0};

combo_dev_cfg_t *ddic_get_dev_cfg(int index)
{
    return g_ddic_mipi_sequ[index];
}

test_drv_api *ddic_get_drv_api(void)
{
    return &g_ddic_api;
}

void ddic_change_120x240_datarate(unsigned int datarate)
{
    unsigned int rgb16_default = g_test_120x240_60_rgb24_video.phy_data_rate;
    unsigned int rgb24_default = g_test_120x240_60_rgb24.phy_data_rate;
    unsigned int re_390x390_cmd = g_test_390x390_60_rgb24.phy_data_rate;
    unsigned int re_1280x800_video = g_test_800x1280_60_rgb24_1p3g.phy_data_rate;
    unsigned int re_1280x800_cmd = g_test_1280x800_60_rgb24.phy_data_rate;
    g_test_120x240_60_rgb24_video.phy_data_rate = datarate;
    g_test_120x240_60_rgb24.phy_data_rate = datarate;
    g_test_390x390_60_rgb24.phy_data_rate = datarate;
    g_test_800x1280_60_rgb24_1p3g.phy_data_rate = datarate;
    g_test_1280x800_60_rgb24.phy_data_rate = datarate;
    PRINT("[120x240 video rgb16]datarate: %d >> %d\n", rgb16_default,
        g_test_120x240_60_rgb24_video.phy_data_rate);
    PRINT("[120x240 cmd rgb24]datarate: %d >> %d\n", rgb24_default,
        g_test_120x240_60_rgb24.phy_data_rate);
    PRINT("[390x390 cmd rgb24]datarate: %d >> %d\n", re_390x390_cmd,
        g_test_390x390_60_rgb24.phy_data_rate);
    PRINT("[1280x800 video rgb24]datarate: %d >> %d\n", re_1280x800_video,
        g_test_800x1280_60_rgb24_1p3g.phy_data_rate);
    PRINT("[1280x800 cmd rgb24]datarate: %d >> %d\n", re_1280x800_video,
        g_test_1280x800_60_rgb24.phy_data_rate);
}