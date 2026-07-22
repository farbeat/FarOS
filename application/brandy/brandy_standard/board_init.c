/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: Application core board init function for standard
 * Author:
 * Create:
 */

#include "board_init.h"
#include "stddef.h"
#include "chip_io.h"
#include "flash.h"
#include "uart.h"
#include "gpio.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"
#include "uart_porting.h"
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#include "lcd_qspi.h"
#include "lcd_mipi.h"
#include "lcd_bus.h"
#endif

#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
static const lcd_bus_api g_display_api_qspi = {
    .bus_init = lcd_qspi_init,
    .bus_deinit = lcd_qspi_deinit,
    .bus_enable = lcd_qspi_enable,
    .bus_disable = lcd_qspi_disable,
    .bus_write_cmd = lcd_qspi_write_cmd,
    .bus_read_cmd = lcd_qspi_read_cmd,
    .bus_send_frame = lcd_qspi_send_frame,
    .bus_suspend = lcd_qspi_suspend,
    .bus_resume = lcd_qspi_resume,
    .bus_register_te_isr = lcd_qspi_register_te_isr,
};

static const lcd_bus_api g_display_api_mipi = {
    .bus_init = mipi_bus_init,
    .bus_deinit = mipi_bus_deinit,
    .bus_enable = mipi_bus_enable,
    .bus_disable = mipi_bus_disable,
    .bus_write_cmd = mipi_bus_write,
    .bus_read_cmd = mipi_bus_read,
    .bus_lp_ctrl = mipi_bus_lp_ctrl,
    .bus_register_panel_info = mipi_bus_register_panel_info,
    .bus_get_panel_timing = mipi_bus_get_panel_timing,
    .bus_get_partial_display_cmd = mipi_bus_get_partial_display_cmd,
    .bus_update_panel = mipi_bus_update_panel,
    .bus_refresh_area = mipi_bus_refresh_area,
    .bus_suspend = mipi_bus_suspend,
    .bus_resume = mipi_bus_resume,
};
#endif

display_class g_display_init_select = DISPLAY_CLASS_UNDEFINED;
board_class g_board_init_select = BOARD_CLASS_E0B2;

display_class get_display_init_select(void)
{
    return g_display_init_select;
}

board_class get_board_init_select(void)
{
    return g_board_init_select;
}

void flash_funcreg_adapt(flash_id_t id);

static void pio_func_init(void)
{
    size_t pin_num;
    hal_pio_config_t *pio_func = NULL;

    get_pio_func_config(&pin_num, &pio_func);

    for (pin_t i = S_EGPIO0; i < pin_num; i++) {
        if (pio_func[i].func != HAL_PIO_FUNC_INVALID) {
            if (pio_func[i].func == HAL_PIO_FUNC_DEFAULT_HIGH_Z) {
                uapi_pin_set_mode(i, (pin_mode_t)HAL_PIO_FUNC_GPIO);
                uapi_gpio_set_dir(i, GPIO_DIRECTION_INPUT);
            } else {
                uapi_pin_set_mode(i, (pin_mode_t)pio_func[i].func);
                uapi_pin_set_ds(i, (pin_drive_strength_t)pio_func[i].drive);
            }
        }
        if (pio_func[i].pull != HAL_PIO_PULL_MAX) {
            uapi_pin_set_pull(i, (pin_pull_t)pio_func[i].pull);
        }
        if (pio_func[i].ie != HAL_PIO_IE_MAX) {
            uapi_pin_set_ie(i, (pin_input_enable_t)pio_func[i].ie);
        }
    }
}

static void gpio_func_init(void)
{
    size_t pin_num = 0;
    hal_gpio_config_t *gpio_func = NULL;

    get_gpio_func_config(&pin_num, &gpio_func);

    for (pin_t i = 0; i < pin_num; i++) {
        uapi_gpio_set_dir(gpio_func[i].gpio, (gpio_direction_t)gpio_func[i].direction);
        if (gpio_func[i].direction == HAL_GPIO_DIRECTION_INPUT && gpio_func[i].callback != NULL) {
            uapi_gpio_register_isr_func(gpio_func[i].gpio, gpio_func[i].triger, gpio_func[i].callback);
        }
        if (gpio_func[i].direction == HAL_GPIO_DIRECTION_OUTPUT) {
            uapi_gpio_set_val(gpio_func[i].gpio, (gpio_level_t)gpio_func[i].level);
        }
    }
}

void board_init(uint32_t flash_id, uint32_t flash_unique_id)
{
    /* flash init */
    flash_funcreg_adapt(0);
    flash_save_manufacturer(0, flash_id);
    uapi_flash_init(0);
    uapi_flash_config_cmd_at_xip_mode(0, (uint8_t)flash_unique_id);
    uapi_flash_switch_to_cache_mode(0);
    PRINT("Flash id is %x, init success"NEWLINE, flash_id);

    pio_func_init();
    gpio_func_init();

#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#ifdef QSPI_DISPLAY
    lcd_bus_register_api(&g_display_api_qspi, BUS_DISPLAY_QSPI);
    g_display_init_select = DISPLAY_CLASS_QSPI;
#else
    lcd_bus_register_api(&g_display_api_mipi, BUS_DISPLAY_MIPI);
    g_display_init_select = DISPLAY_CLASS_MIPI;
#endif
#endif

#ifdef BRANDY_PRODUCT_EVB
    g_board_init_select = BOARD_CLASS_E0B2;
#else
    g_board_init_select = BOARD_CLASS_E0B4;
#endif
}