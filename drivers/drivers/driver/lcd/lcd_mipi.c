/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: MIPI display  code encapsulation.
 * Author: @CompanyNameTag
 * Create: 2022-09-22
 */

#include "lcd_mipi.h"
#include "stdio.h"
#include "securec.h"
#include "soc_errno.h"
#include "tcxo.h"
#include "soc_mipi_tx.h"
#include "soc_lcd_api.h"
#include "mipi_tx_def.h"
#include "mipi_tx.h"
#include "lcd_bus.h"
#include "lcd_common.h"

#define PARTIAL_DISPLAY_DATA_LEN     5
#define BYTE_MASK                    0xff
#define SET_COL_ADDR                 0x2a
#define SET_RAW_ADDR                 0x2b
#define PHY_LPTX_EN                  0x560005B8
#define PHY_BAND_EN                  0x56000598
#define PHY_LPTX_DP                  0x560005cc

typedef struct {
    bus_panel_info_t panel_info;
    ext_rect area;
    uint16_t x_start;   /* screen releated, be registered by LCD */
    uint16_t y_start;   /* screen releated, be registered by LCD */
    bool panel_is_init; /* only panel operation related */
    bool panel_is_reg;  /* only panel operation related */
} lcd_mipi_info_t;

static lcd_mipi_info_t g_lcd_mipi_info = {0};

ext_errno mipi_bus_init(combo_dev_cfg_t *bus_cfg)
{
    ext_errno ret;
    ret = uapi_mipi_tx_init();
    if (ret != EXT_ERR_SUCCESS && ret != EXT_ERR_MIPI_ALREADY_INIT) {
        lcd_err("uapi_mipi_tx_init fail, ret = %X\r\n", ret);
        return ret;
    }

    ret = uapi_mipi_tx_set_dev_cfg(bus_cfg);
    if (ret != EXT_ERR_SUCCESS && ret != EXT_ERR_MIPI_DEV_ALREADY_ENABLE) {
        (void)uapi_mipi_tx_deinit();
        lcd_err("uapi_mipi_tx_set_dev_cfg fail, ret = %X\r\n", ret);
        return ret;
    }

    g_lcd_mipi_info.panel_info.is_cmd_mode           = (bus_cfg->out_mode == OUT_MODE_DSI_CMD) ? true : false;
    g_lcd_mipi_info.panel_info.width                 = bus_cfg->sync_info.hact_pixels;
    g_lcd_mipi_info.panel_info.height                = bus_cfg->sync_info.vact_lines;

    g_lcd_mipi_info.panel_info.sync_info.hsa_pixels  = bus_cfg->sync_info.hsa_pixels;
    g_lcd_mipi_info.panel_info.sync_info.hbp_pixels  = bus_cfg->sync_info.hbp_pixels;
    g_lcd_mipi_info.panel_info.sync_info.hact_pixels = bus_cfg->sync_info.hact_pixels;
    g_lcd_mipi_info.panel_info.sync_info.hfp_pixels  = bus_cfg->sync_info.hfp_pixels;
    g_lcd_mipi_info.panel_info.sync_info.vsa_lines   = bus_cfg->sync_info.vsa_lines;
    g_lcd_mipi_info.panel_info.sync_info.vbp_lines   = bus_cfg->sync_info.vbp_lines;
    g_lcd_mipi_info.panel_info.sync_info.vact_lines  = bus_cfg->sync_info.vact_lines;
    g_lcd_mipi_info.panel_info.sync_info.vfp_lines   = bus_cfg->sync_info.vfp_lines;

    g_lcd_mipi_info.panel_info.pixel_clk             = bus_cfg->pixel_clk;

    g_lcd_mipi_info.area.x = 0;
    g_lcd_mipi_info.area.y = 0;
    g_lcd_mipi_info.area.width = g_lcd_mipi_info.panel_info.width;
    g_lcd_mipi_info.area.height = g_lcd_mipi_info.panel_info.height;

    g_lcd_mipi_info.panel_is_init = true;

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_deinit(void)
{
    ext_errno ret;

    ret = uapi_mipi_tx_deinit();

    g_lcd_mipi_info.panel_is_init = false;
    return ret;
}

ext_errno mipi_bus_enable(void)
{
    return uapi_mipi_tx_enable();
}

ext_errno mipi_bus_disable(void)
{
    return uapi_mipi_tx_disable();
}

static ext_errno mipi_dcs_cmd_send(lcd_cmd_sequ *dcs_cmd)
{
    ext_errno ret;
    cmd_info_t cmd_info = {0};

    if (dcs_cmd == NULL) {
        lcd_err("line[%d] dcs_cmd is illegal null\r\n", __LINE__);
        return EXT_ERR_LCD_INVALID_NULL_POINTER;
    }

    cmd_info.work_mode = MIPI_TX_WORK_MODE_LP;
    if (dcs_cmd->data_len > 2) { /* len greater than 2: dsi long packet */
        cmd_info.cmd_size = dcs_cmd->data_len;
        cmd_info.data_type = DCS_LONG_WRITE;
        cmd_info.cmd = dcs_cmd->data;
    } else if (dcs_cmd->data_len == 2) { /* len equals 2: dsi dcs command with a parameter */
        cmd_info.cmd_size = bus_short_packet(dcs_cmd->data[1], dcs_cmd->data[0]);
        cmd_info.data_type = DCS_SHORT_WRITE_1P;
    } else if (dcs_cmd->data_len == 1) { /* len equals 1: dsi dcs command with 0 parameter */
        cmd_info.cmd_size = dcs_cmd->data[0] & 0xff;
        cmd_info.data_type = DCS_SHORT_WRITE_0P;
    } else {
        lcd_err("line[%d] invalid cmd size\r\n", __LINE__);
        return EXT_ERR_LCD_INVALID_DATA_LENGTH;
    }

    ret = uapi_mipi_tx_set_cmd(&cmd_info);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("line[%d] dcs_cmd send failed, ret=0x%x\r\n", __LINE__, ret);
        return ret;
    }

    if (dcs_cmd->delay_ms > 0) {
        uapi_tcxo_delay_ms(dcs_cmd->delay_ms);
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno mipi_generic_cmd_send(lcd_cmd_sequ *generic_cmd)
{
    ext_errno ret;
    cmd_info_t cmd_info = {0};

    if (generic_cmd == NULL) {
        lcd_err("line[%d] generic_cmd is illegal null\r\n", __LINE__);
        return EXT_ERR_LCD_INVALID_NULL_POINTER;
    }

    cmd_info.work_mode = MIPI_TX_WORK_MODE_LP;
    if (generic_cmd->data_len > 2) { /* len greater than 2: dsi long packet */
        cmd_info.cmd_size = generic_cmd->data_len;
        cmd_info.data_type = GEN_LONG_WRITE;
        cmd_info.cmd = generic_cmd->data;
    } else if (generic_cmd->data_len == 2) { /* len equals 2: dsi generic_cmd command with 2 parameters */
        cmd_info.cmd_size = bus_short_packet(generic_cmd->data[1], generic_cmd->data[0]);
        cmd_info.data_type = GEN_SHORT_WRITE_2P;
    } else if (generic_cmd->data_len == 1) {
        cmd_info.cmd_size = generic_cmd->data[0] & 0xff;
        cmd_info.data_type = GEN_SHORT_WRITE_1P;
    } else {
        cmd_info.data_type = GEN_SHORT_WRITE_0P;
    }

    ret = uapi_mipi_tx_set_cmd(&cmd_info);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("line[%d] generic_cmd send failed, ret=0x%x\r\n", __LINE__, ret);
        return ret;
    }

    if (generic_cmd->delay_ms > 0) {
        uapi_tcxo_delay_ms(generic_cmd->delay_ms);
    }

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_write(lcd_cmd_sequ *cmd)
{
    ext_errno ret;

    if (cmd->dcs_flag != 0) {
        ret = mipi_dcs_cmd_send(cmd);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    } else {
        ret = mipi_generic_cmd_send(cmd);
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_read(uint32_t cmd, uint8_t *buff, uint8_t buff_len)
{
    ext_errno ret;
    get_cmd_info_t get_info = {0};
    cmd_info_t set_info = {0};

    set_info.data_type = 0x37; /* mipi set read packet max len command */
    set_info.cmd_size = buff_len; /* set read packet max len */
    ret = uapi_mipi_tx_set_cmd(&set_info);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("[LCD_MIPI-%d]set read packet cmd failed, ret = %X\r\n", __LINE__, ret);
        return ret;
    }

    get_info.work_mode = MIPI_TX_WORK_MODE_LP;
    get_info.data_type = 0x14;
    get_info.data_param = (uint16_t)cmd;
    get_info.get_data_size = buff_len;
    get_info.get_data = buff;
    ret = uapi_mipi_tx_get_cmd(&get_info);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("[LCD_MIPI-%d]mipi read fail, ret = 0x%x \r\n", __LINE__, ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_lp_ctrl(lcd_bus_lp_ctrl_t mode)
{
    switch (mode) {
        case LP_CTRL_ENTER_SDLP:
            return uapi_mipi_tx_auto_enter_sdlp();
        case LP_CTRL_EXIT_SDLP:
            return uapi_mipi_tx_exit_sdlp();
        case LP_CTRL_EXIT_ULPS:
#ifdef MIPI_ULPS_SUPPORT
            return uapi_mipi_tx_exit_ulps();
#else
            return EXT_ERR_SUCCESS;
#endif
        case LP_CTRL_CLEAR_EXIT_ULPS:
#ifdef MIPI_ULPS_SUPPORT
            return uapi_mipi_tx_clear_exit_ulps();
#else
            return EXT_ERR_SUCCESS;
#endif
        default:
            return EXT_ERR_LCD_INVALID_MODE;
    }
}

ext_errno mipi_bus_get_panel_timing(const ext_rect *rect, bus_panel_info_t *info)
{
    sync_info_t *sync_info = &g_lcd_mipi_info.panel_info.sync_info;

    if (!g_lcd_mipi_info.panel_is_init || !g_lcd_mipi_info.panel_is_reg) {
        lcd_err("lcd mipi init error\r\n");
        return EXT_ERR_LCD_MIPI_NOT_READY;
    }

    if (rect == NULL) {
        g_lcd_mipi_info.area.x      = 0;
        g_lcd_mipi_info.area.y      = 0;
        g_lcd_mipi_info.area.width  = g_lcd_mipi_info.panel_info.width;
        g_lcd_mipi_info.area.height = g_lcd_mipi_info.panel_info.height;
    } else {
        if ((rect->x + rect->width > g_lcd_mipi_info.panel_info.width) ||
            (rect->y + rect->height > g_lcd_mipi_info.panel_info.height)) {
            lcd_err("refresh region over the screen, region[%d,%d,%d,%d]\n",
                rect->x, rect->y, rect->width, rect->height);
            return EXT_ERR_LCD_MIPI_OVER_REGION;
        }
        if (sync_info->hact_pixels > rect->width) {
            sync_info->hfp_pixels += sync_info->hact_pixels - (uint16_t)rect->width;
        } else {
            sync_info->hfp_pixels -= (uint16_t)rect->width - sync_info->hact_pixels;
        }
        sync_info->hact_pixels = (uint16_t)rect->width;
        sync_info->vact_lines = (uint16_t)rect->height;

        g_lcd_mipi_info.area.x      = rect->x;
        g_lcd_mipi_info.area.y      = rect->y;
        g_lcd_mipi_info.area.width  = rect->width;
        g_lcd_mipi_info.area.height = rect->height;
    }

    if (memcpy_s(info, sizeof(bus_panel_info_t), &g_lcd_mipi_info.panel_info, sizeof(bus_panel_info_t)) != EOK) {
        return EXT_ERR_MIPI_MEMCPY_FAIL;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno mipi_bus_update_timing(void)
{
    if (!g_lcd_mipi_info.panel_info.is_cmd_mode) {
        lcd_err("Partial display is not supported in video mode!\n");
        return EXT_ERR_LCD_MIPI_NOT_SUPPORT_SCREEN_TYPE;
    }

    return mipi_tx_partial_refresh(&g_lcd_mipi_info.panel_info.sync_info);
}

static ext_errno set_screen_display_area(uint16_t start, uint16_t end, uint8_t addr)
{
    cmd_info_t cmd_info = {0};
    uint8_t data[PARTIAL_DISPLAY_DATA_LEN] = {0};
    ext_errno ret;

    data[0] = addr;
    data[1] = (start >> 8) & BYTE_MASK; /* byte1 start high byte, 8: byte_shift */
    data[2] = start & BYTE_MASK;        /* byte2 start low byte */
    data[3] = (end >> 8) & BYTE_MASK;   /* byte3 end high byte, 8: byte_shift */
    data[4] = end & BYTE_MASK;          /* byte4 end low byte */

    cmd_info.cmd_size = PARTIAL_DISPLAY_DATA_LEN;
    cmd_info.data_type = GEN_LONG_WRITE;
    cmd_info.cmd = data;

    return uapi_mipi_tx_set_cmd(&cmd_info);
}

static ext_errno mipi_bus_set_area(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    ext_errno ret = set_screen_display_area(x_start, x_end, SET_COL_ADDR);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("line[%d] generic_cmd[0x%x] send failed, ret=0x%x\r\n", __LINE__, SET_COL_ADDR, ret);
        return ret;
    }

    ret = set_screen_display_area(y_start, y_end, SET_RAW_ADDR);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("line[%d] generic_cmd[0x%x] send failed, ret=0x%x\r\n", __LINE__, SET_RAW_ADDR, ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno mipi_bus_update_screen(void)
{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;

    x_start = (uint16_t)g_lcd_mipi_info.area.x + g_lcd_mipi_info.x_start;
    y_start = (uint16_t)g_lcd_mipi_info.area.y + g_lcd_mipi_info.y_start;
    x_end   = x_start + (uint16_t)g_lcd_mipi_info.area.width - 1;
    y_end   = y_start + (uint16_t)g_lcd_mipi_info.area.height - 1;

    return mipi_bus_set_area(x_start, y_start, x_end, y_end);
}

ext_errno mipi_bus_refresh_area(const ext_rect *rect)
{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;

    x_start = (uint16_t)rect->x + g_lcd_mipi_info.x_start;
    y_start = (uint16_t)rect->y + g_lcd_mipi_info.y_start;
    x_end   = x_start + (uint16_t)rect->width - 1;
    y_end   = y_start + (uint16_t)rect->height - 1;

    return mipi_bus_set_area(x_start, y_start, x_end, y_end);
}

ext_errno mipi_bus_get_partial_display_cmd(const ext_rect *rect, partial_display_cmd_t *cmd)
{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;

    if (!g_lcd_mipi_info.panel_is_init || !g_lcd_mipi_info.panel_is_reg) {
        lcd_err("lcd mipi init error\r\n");
        return EXT_ERR_LCD_MIPI_NOT_READY;
    }

    if (rect == NULL || cmd == NULL) {
        lcd_err("Invalid rect param!\n");
        return EXT_ERR_LCD_INVALID_NULL_POINTER;
    }

    if (memcmp(rect, &g_lcd_mipi_info.area, sizeof(ext_rect)) != 0) {
        lcd_err("screen region & mipi timing not match!, screen[%d,%d,%u,%u], timing[%d,%d,%u,%u]\n",
            rect->x, rect->y, rect->width, rect->height, g_lcd_mipi_info.area.x, g_lcd_mipi_info.area.y,
            g_lcd_mipi_info.area.width, g_lcd_mipi_info.area.height);
            return EXT_ERR_LCD_MIPI_REGION_NOT_MATCH;
    }

    x_start = (uint16_t)g_lcd_mipi_info.area.x + g_lcd_mipi_info.x_start;
    y_start = (uint16_t)g_lcd_mipi_info.area.y + g_lcd_mipi_info.y_start;
    x_end   = x_start + (uint16_t)g_lcd_mipi_info.area.width - 1;
    y_end   = y_start + (uint16_t)g_lcd_mipi_info.area.height - 1;

    cmd->cmd_set_col_h = 0x529; /* 0x529: payload_len = 5, generate long write data type--0x29 */
    cmd->set_col_p0 = uapi_joinu32(SET_COL_ADDR, uapi_hiu8(x_start), uapi_lou8(x_start), uapi_hiu8(x_end));
    cmd->set_col_p1 = uapi_joinu32(uapi_lou8(x_end), 0, 0, 0);

    cmd->cmd_set_raw_h = 0x529; /* 0x529: payload_len = 5, generate long write data type--0x29 */
    cmd->set_raw_p0 = uapi_joinu32(SET_RAW_ADDR, uapi_hiu8(y_start), uapi_lou8(y_start), uapi_hiu8(y_end));
    cmd->set_raw_p1 = uapi_joinu32(uapi_lou8(y_end), 0, 0, 0);

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_update_panel(panel_option_t opt)
{
    ext_errno ret;

    if (!g_lcd_mipi_info.panel_is_init || !g_lcd_mipi_info.panel_is_reg) {
        lcd_err("lcd mipi init error\r\n");
        return EXT_ERR_LCD_MIPI_NOT_READY;
    }

    switch (opt) {
        case PANEL_UPDATE_TIMING:
            return mipi_bus_update_timing();
        case PANEL_UPDATE_SCREEN:
            return mipi_bus_update_screen();
        case PANEL_DISPLAY_REFRESH:
            ret = mipi_bus_update_timing();
            if (ret != EXT_ERR_SUCCESS) {
                return ret;
            }
            return mipi_bus_update_screen();
        default:
            return EXT_ERR_LCD_MIPI_INVALID_OPTION;
    }
}

void mipi_bus_register_panel_info(uint16_t x_start, uint16_t y_start, uint32_t fps)
{
    g_lcd_mipi_info.x_start = x_start;
    g_lcd_mipi_info.y_start = y_start;
    g_lcd_mipi_info.panel_info.refresh_rate = fps;

    g_lcd_mipi_info.panel_is_reg = true;
}

ext_errno mipi_bus_suspend(void)
{
    ext_errno ret;

    /* mipi bypass */
    uapi_reg_write(PHY_LPTX_EN, 0xe0);
    uapi_reg_write(PHY_BAND_EN, 0x0e);
    uapi_reg_write(PHY_LPTX_DP, 0xc0);

    ret = mipi_bus_disable();
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("bus_disable fail, ret = %X\r\n", ret);
        return ret;
    }

    ret = mipi_bus_deinit();
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("bus_deinit fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

ext_errno mipi_bus_resume(void)
{
    ext_errno ret;
    lcd_drv_common *lcd_drv_comm_api = NULL;
    lcd_drv_cfg *lcd_cfg = NULL;

    /* close mipi bypass */
    uapi_reg_write(PHY_LPTX_EN, 0x0);
    uapi_reg_write(PHY_BAND_EN, 0x0);
    uapi_reg_write(PHY_LPTX_DP, 0xfc);

    lcd_drv_comm_api = lcd_drv_get_common_api();
    if (lcd_drv_comm_api == NULL) {
        lcd_err("lcd_drv_comm_api is null.\r\n");
        return EXT_ERR_FAILURE;
    }

    lcd_cfg = lcd_drv_comm_api->get_lcd_config();
    if (lcd_cfg == NULL) {
        lcd_err("lcd_cfg is null.\r\n");
        return EXT_ERR_FAILURE;
    }

    ret = mipi_bus_init(lcd_cfg->bus_cfg);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("bus_init fail, ret = %X\r\n", ret);
        return ret;
    }

    ret = mipi_bus_enable();
    if (ret != EXT_ERR_SUCCESS && ret != EXT_ERR_MIPI_DEV_ALREADY_ENABLE) {
        lcd_err("bus_enable fail, ret = %X\r\n", ret);
        return ret;
    }
    return EXT_ERR_SUCCESS;
}