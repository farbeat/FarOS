/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include "tcxo.h"
#include "soc_mipi_tx.h"
#include "soc_errno.h"
#include "mipi_tx.h"
#include "test_screen_edo.h"
#include "test_screen_ruxin.h"
#include "test_screen_kdk.h"
#include "test_screen_weitai.h"
#include "test_screen_ddic.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "debug_print.h"
#include "lcd_bus.h"
#include "gpio.h"
#include "test_screen_lcd.h"

#define TEST_LCD_RESET_GPIO_NUM       105
lcd_bus_api *g_test_lcd_bus_api;
test_lcd_cfg g_lcd_attr = {0};
static void test_lcd_power_on(void)
{
    /* set lcd reset gpio output */
    uapi_gpio_set_dir(TEST_LCD_RESET_GPIO_NUM, 1);
    /* set lcd reset signal high level */

    /* set lcd reset signal low level */
    gpio_clear(TEST_LCD_RESET_GPIO_NUM);
    uapi_tcxo_delay_us(20); /* delay 20us */
    /* set lcd reset signal high level */
    gpio_set(TEST_LCD_RESET_GPIO_NUM);
    uapi_tcxo_delay_us(5000); /* 5000: delay 5ms */
}

static void test_lcd_power_down(void)
{
    /* set lcd reset signal low level */
    gpio_clear(TEST_LCD_RESET_GPIO_NUM);
    uapi_tcxo_delay_us(120000); /* 120000: delay 120ms */
    /* set lcd reset gpio input */
    uapi_gpio_set_dir(TEST_LCD_RESET_GPIO_NUM, 0);
}

static uint32_t test_lcd_screen_init(void)
{
    uint32_t ret;

    if (g_lcd_attr.drv_api != NULL && g_lcd_attr.drv_api->lcd_init != NULL) {
        ret = g_lcd_attr.drv_api->lcd_init(g_lcd_attr.mipi_attr);
        if (ret != EXT_ERR_SUCCESS) {
            PRINT("lcd init fail, ret = %x \r\n", ret);
            return ret;
        }
    } else {
        PRINT("lcd drv_api not init!\r\n");
        return EXT_ERR_FAILURE;
    }

    return EXT_ERR_SUCCESS;
}

static int lcd_get_attr(lcd_sel lcd_num, int output_mode)
{
    int32_t ret = EXT_ERR_SUCCESS;

    switch (lcd_num) {
        case LCD_EDO:
            g_lcd_attr.mipi_attr = edo_get_dev_cfg(output_mode);
            g_lcd_attr.drv_api = edo_get_drv_api();
            break;
        case LCD_RUXIN:
            g_lcd_attr.mipi_attr = ruxin_get_dev_cfg(output_mode);
            g_lcd_attr.drv_api = ruxin_get_drv_api();
            break;
        case LCD_KDK:
            g_lcd_attr.mipi_attr = kdk_get_dev_cfg(output_mode);
            g_lcd_attr.drv_api = kdk_get_drv_api();
            break;
        case LCD_WEITAI:
            g_lcd_attr.mipi_attr = weitai_get_dev_cfg(output_mode);
            g_lcd_attr.drv_api = weitai_get_drv_api();
            break;
        case LCD_DDIC:
            g_lcd_attr.mipi_attr = ddic_get_dev_cfg(output_mode);
            g_lcd_attr.drv_api = ddic_get_drv_api();
            break;
        default:
            PRINT("error lcd number: %d\n", lcd_num);
            ret = EXT_ERR_FAILURE;
            break;
    }

    return EXT_ERR_SUCCESS;
}

int32_t test_lcd_assigned_init(lcd_sel lcd_num, int output_mode)
{
    uint32_t ret;
    g_test_lcd_bus_api = lcd_get_bus_api();

    ret = lcd_get_attr(lcd_num, output_mode);
    if (ret != EXT_ERR_SUCCESS) {
        PRINT("get lcd attr failed! err: 0x%x\n", ret);
        return ret;
    }

    /*
     * Turn on panel backlight and reset panel.
     */
    if (lcd_num != LCD_DDIC) {
        test_lcd_power_on();
    }

    /*
     * Initialize mipi_tx and reset gpio resource.
     */
    ret = g_test_lcd_bus_api->bus_init(g_lcd_attr.mipi_attr);
    if (ret != EXT_ERR_SUCCESS) {
        PRINT("bus init failed\n");
        return ret;
    }

    /*
     * Init screen or other peripheral unit.
     */
    if (lcd_num != LCD_DDIC) {
        ret = test_lcd_screen_init();
        if (ret != EXT_ERR_SUCCESS) {
            return ret;
        }
    }

    return EXT_ERR_SUCCESS;
}

int32_t test_lcd_assigned_deinit(void)
{
    int32_t ret;

    if (g_test_lcd_bus_api == NULL) {
        return EXT_ERR_LCD_NOT_INIT;
    }
    /*
     * Disable mipi tx controller.
     */
    ret = g_test_lcd_bus_api->bus_disable();
    if (ret != EXT_ERR_SUCCESS && ret != (int32_t)EXT_ERR_MIPI_NOT_INIT) {
        PRINT("disable failed, ret=0x%x\n", ret);
        return ret;
    }

    /* deinit mipi tx. */
    ret = g_test_lcd_bus_api->bus_deinit();
    if (ret != EXT_ERR_SUCCESS) {
        PRINT("mipi tx disable failed\n");
        return ret;
    }

    test_lcd_power_down();

    return EXT_ERR_SUCCESS;
}
