/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Lcd private interface.
 * Author: @CompanyNameTag
 * Create: 2022-03-05
 */

#include "lcd_private.h"
#include "tcxo.h"
#include "chip_io.h"
#include "platform_core.h"
#include "gpio.h"
#include "pinctrl_porting.h"
#include "product.h"

static void lcd_drv_power_on(void)
{
#ifndef QSPI_DISPLAY
    uapi_gpio_set_dir(VCI_LCD_GPIO, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(VCI_LCD_GPIO, GPIO_LEVEL_HIGH);
#endif
    uapi_gpio_set_dir(RESET_LCD_GPIO, GPIO_DIRECTION_OUTPUT);

    uapi_gpio_set_val(RESET_LCD_GPIO, GPIO_LEVEL_LOW);
    uapi_tcxo_delay_ms(10); /* delay 10 ms */

    uapi_gpio_set_val(RESET_LCD_GPIO, GPIO_LEVEL_HIGH);
    uapi_tcxo_delay_ms(10); /* delay 10 ms */
}

static void lcd_drv_power_off(void)
{
    uapi_gpio_set_val(RESET_LCD_GPIO, GPIO_LEVEL_LOW);
    uapi_gpio_set_dir(RESET_LCD_GPIO, GPIO_DIRECTION_INPUT);
#ifndef QSPI_DISPLAY
    uapi_gpio_set_val(VCI_LCD_GPIO, GPIO_LEVEL_LOW);
    uapi_gpio_set_dir(VCI_LCD_GPIO, GPIO_DIRECTION_INPUT);
#endif
    uapi_tcxo_delay_ms(10); /* delay 10 ms */
}

static lcd_drv_private g_lcd_private_api = {
    .power_on = lcd_drv_power_on,
    .power_off = lcd_drv_power_off,
};

lcd_drv_private *lcd_drv_get_private_api(void)
{
    return &g_lcd_private_api;
}
