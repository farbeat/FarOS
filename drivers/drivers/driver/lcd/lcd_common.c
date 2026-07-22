/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Lcd common interface.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#include "lcd_common.h"
#include <stdint.h>
#include "soc_errno.h"
#include "lcd_config.h"
#include "lcd_bus.h"

static lcd_drv_cfg *g_lcd_config = NULL;
static lcd_bus_api *g_lcd_disp_bus = NULL;

static ext_errno lcd_drv_write(lcd_cmd_sequ *cmds, uint32_t cmd_count)
{
    ext_errno ret;
    uint32_t i;

    if (cmds == NULL || cmd_count == 0) {
        return EXT_ERR_LCD_INVALID_WRITE_CMD;
    }
    if (g_lcd_disp_bus == NULL) {
        g_lcd_disp_bus = lcd_get_bus_api();
        if (g_lcd_disp_bus == NULL) {
            return EXT_ERR_LCD_INVALID_BUS_OPS;
        }
    }

    if (g_lcd_disp_bus->bus_write_cmd == NULL) {
        return EXT_ERR_LCD_INVALID_BUS_WRITE_FUNC;
    }

    if (g_lcd_disp_bus->bus_lp_ctrl != NULL) {
        g_lcd_disp_bus->bus_lp_ctrl(LP_CTRL_EXIT_SDLP);
    }

    for (i = 0; i < cmd_count; i++) {
        ret = g_lcd_disp_bus->bus_write_cmd(&(cmds[i]));
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }
    if (g_lcd_disp_bus->bus_lp_ctrl != NULL) {
        g_lcd_disp_bus->bus_lp_ctrl(LP_CTRL_ENTER_SDLP);
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_read(uint8_t cmd, uint8_t *buff, uint8_t buff_len)
{
    ext_errno ret;

    if (g_lcd_disp_bus == NULL) {
        g_lcd_disp_bus = lcd_get_bus_api();
        if (g_lcd_disp_bus == NULL) {
            return EXT_ERR_LCD_INVALID_BUS_OPS;
        }
    }
    if (g_lcd_disp_bus->bus_read_cmd == NULL) {
        return EXT_ERR_LCD_INVALID_BUS_READ_FUNC;
    }

    if (g_lcd_disp_bus->bus_lp_ctrl != NULL) {
        g_lcd_disp_bus->bus_lp_ctrl(LP_CTRL_EXIT_SDLP);
    }

    ret = g_lcd_disp_bus->bus_read_cmd(cmd, buff, buff_len);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    if (g_lcd_disp_bus->bus_lp_ctrl != NULL) {
        g_lcd_disp_bus->bus_lp_ctrl(LP_CTRL_ENTER_SDLP);
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_enter_sleep_mode(void)
{
    ext_errno ret;

    if (g_lcd_config->sleep_in_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_DISP_ON_SEQU;
    }
    ret = lcd_drv_write(g_lcd_config->sleep_in_cmds->cmd, g_lcd_config->sleep_in_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_exit_sleep_mode(void)
{
    ext_errno ret;

    if (g_lcd_config->sleep_out_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_DISP_ON_SEQU;
    }
    ret = lcd_drv_write(g_lcd_config->sleep_out_cmds->cmd, g_lcd_config->sleep_out_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("lcd_drv_write fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_enter_idle_mode(void)
{
    ext_errno ret;

    if (g_lcd_config->enter_idle_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_IDLE_ON_CMD;
    }

    ret = lcd_drv_write(g_lcd_config->enter_idle_cmds->cmd, g_lcd_config->enter_idle_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_exit_idle_mode(void)
{
    ext_errno ret;

    if (g_lcd_config->exit_idle_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_IDLE_OFF_CMD;
    }

    ret = lcd_drv_write(g_lcd_config->exit_idle_cmds->cmd, g_lcd_config->exit_idle_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_te_on(void)
{
    ext_errno ret;

    if (g_lcd_config->te_on_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_TE_ON_CMD;
    }

    ret = lcd_drv_write(g_lcd_config->te_on_cmds->cmd, g_lcd_config->te_on_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_te_off(void)
{
    ext_errno ret;

    if (g_lcd_config->te_off_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_TE_OFF_CMD;
    }

    ret = lcd_drv_write(g_lcd_config->te_off_cmds->cmd, g_lcd_config->te_off_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_set_brightness(uint16_t value)
{
    ext_errno ret;
    uint32_t cmd_cnt;

    if (g_lcd_config->set_brightnes_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_WRITE_CMD;
    }

    cmd_cnt = g_lcd_config->set_brightnes_cmds->cmd_cnt;
    (g_lcd_config->set_brightnes_cmds->cmd[cmd_cnt - 1]).data[1] = (uint8_t)value;

    ret = lcd_drv_write(g_lcd_config->set_brightnes_cmds->cmd, g_lcd_config->set_brightnes_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("lcd_drv_write fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_get_brightness(uint16_t *value)
{
    if (g_lcd_config->set_brightnes_cmds == NULL || value == NULL) {
        return EXT_ERR_LCD_INVALID_WRITE_CMD;
    }

    *value = (g_lcd_config->set_brightnes_cmds->cmd[g_lcd_config->set_brightnes_cmds->cmd_cnt - 1]).data[1];
    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_get_chip_id(uint32_t *id)
{
    ext_errno ret;
    uint32_t i;
    uint32_t retry;
    uint8_t buff[MAX_LCD_ID_LEN] = {0};

    lcd_drv_cfg *lcd_ops = lcd_get_driver_ops();
    uint32_t lcd_ops_num = lcd_get_ops_num();
    if (lcd_ops == NULL || lcd_ops_num == 0) {
        ret = EXT_ERR_LCD_INVALID_DRIVER_OPS;
        lcd_err("lcd_drv_write fail, ret = %X\r\n", ret);
        return ret;
    }

    for (i = 0; i < lcd_ops_num; i++) {
        if (lcd_ops[i].lcd_id_info.read_len > MAX_LCD_ID_LEN) {
            continue;
        }

        for (retry = 3; retry > 0; retry--) { /* retry 3 times */
            ret = lcd_drv_read(lcd_ops[i].lcd_id_info.reg_addr, buff, (uint8_t)lcd_ops[i].lcd_id_info.read_len);
            if (ret == EXT_ERR_SUCCESS) {
                break;
            }
        }
        if (*((uint32_t *)buff) == lcd_ops[i].lcd_id) {
            *id = *((uint32_t *)buff);
            g_lcd_config = lcd_get_driver_config(*id);
            if (g_lcd_config == NULL) {
                lcd_err("Get Lcd config fail.\r\n");
                return EXT_ERR_LCD_INVALID_SCREEN_OPS;
            }
            lcd_print("Succ read lcd id! Lcd id: 0x%x\r\n", *((uint32_t *)buff));
            return EXT_ERR_SUCCESS;
        }
    }

    return EXT_ERR_LCD_GET_ID_FAIL;
}

static ext_errno lcd_drv_set_lcd_config(lcd_drv_cfg *config)
{
    if (config == NULL) {
        return EXT_ERR_LCD_INVALID_SCREEN_OPS;
    }

    if ((config->bus_cfg == NULL) || (config->lcd_id == 0)) {
        return EXT_ERR_LCD_INVALID_SCREEN_OPS;
    }

    g_lcd_config = config;

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_chip_id_check(void)
{
    ext_errno ret;
    uint32_t retry;
    uint8_t buff[MAX_LCD_ID_LEN] = {0};

    if (g_lcd_config == NULL) {
        return EXT_ERR_LCD_INVALID_SCREEN_OPS;
    }

    if (g_lcd_config->lcd_id_info.read_len == 0 || g_lcd_config->lcd_id == 0) {
        return EXT_ERR_LCD_INVALID_SCREEN_OPS;
    }

    for (retry = 3; retry > 0; retry--) { /* retry 3 times */
        ret = lcd_drv_read(g_lcd_config->lcd_id_info.reg_addr, buff, (uint8_t)g_lcd_config->lcd_id_info.read_len);
        if (ret == EXT_ERR_SUCCESS) {
            break;
        }
    }

    if (g_lcd_config->lcd_id == *((uint32_t *)buff)) {
        return EXT_ERR_SUCCESS;
    }

    return EXT_ERR_LCD_INVALID_CHIP_ID;
}

static lcd_drv_cfg *lcd_drv_get_lcd_config(void)
{
    return g_lcd_config;
}

static ext_errno lcd_drv_display_on(uint16_t brightness)
{
    ext_errno ret;

    if (g_lcd_config->display_on_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_DISP_ON_SEQU;
    }
    if (g_lcd_config->display_on_cmds->cmd == NULL || g_lcd_config->display_on_cmds->cmd_cnt == 0) {
        return EXT_ERR_LCD_INVALID_DISP_ON_CMD;
    }
    ret = lcd_drv_write(g_lcd_config->display_on_cmds->cmd, g_lcd_config->display_on_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("lcd_drv_write fail, ret = %X\r\n", ret);
        return ret;
    }

    ret = lcd_drv_set_brightness(brightness);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("lcd_drv_set_brightness fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static ext_errno lcd_drv_display_off(void)
{
    ext_errno ret;

    if (g_lcd_config->display_off_cmds == NULL) {
        return EXT_ERR_LCD_INVALID_DISP_OFF_SEQU;
    }
    if (g_lcd_config->display_off_cmds->cmd == NULL || g_lcd_config->display_off_cmds->cmd_cnt == 0) {
        return EXT_ERR_LCD_INVALID_DISP_OFF_CMD;
    }
    ret = lcd_drv_write(g_lcd_config->display_off_cmds->cmd, g_lcd_config->display_off_cmds->cmd_cnt);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("lcd_drv_write fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

static lcd_drv_common g_lcd_common_api = {
    .display_on = lcd_drv_display_on,
    .display_off = lcd_drv_display_off,
    .enter_sleep_mode = lcd_drv_enter_sleep_mode,
    .exit_sleep_mode = lcd_drv_exit_sleep_mode,
    .enter_idle_mode = lcd_drv_enter_idle_mode,
    .exit_idle_mode = lcd_drv_exit_idle_mode,
    .te_on = lcd_drv_te_on,
    .te_off = lcd_drv_te_off,
    .set_brightness = lcd_drv_set_brightness,
    .get_brightness = lcd_drv_get_brightness,
    .write = lcd_drv_write,
    .read = lcd_drv_read,
    .get_chip_id = lcd_drv_get_chip_id,
    .get_lcd_config = lcd_drv_get_lcd_config,
    .set_lcd_config = lcd_drv_set_lcd_config,
    .chip_id_check = lcd_drv_chip_id_check,
};

lcd_drv_common *lcd_drv_get_common_api(void)
{
    return &g_lcd_common_api;
}