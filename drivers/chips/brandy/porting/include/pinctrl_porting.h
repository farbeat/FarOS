/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pinctrl port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-25， Create file. \n
 */
#ifndef PINCTRL_PORTING_H
#define PINCTRL_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "securec.h"
#include "platform_core.h"
#include "chip_io.h"
#include "gpio_porting.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_pinctrl Pinctrl
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief  Definition of mode-multiplexing.
 */
typedef enum {
    PIN_MODE_0 = 0,
    PIN_MODE_1 = 1,
    PIN_MODE_2 = 2,
    PIN_MODE_3 = 3,
    PIN_MODE_4 = 4,
    PIN_MODE_5 = 5,
    PIN_MODE_6 = 6,
    PIN_MODE_7 = 7,
    PIN_MODE_MAX
} pin_mode_t;

/**
 * @brief  Definition of drive-strength.
 */
typedef enum {
    PIN_DS_0  = 0,
    PIN_DS_1  = 1,
    PIN_DS_2  = 2,
    PIN_DS_3  = 3,
    PIN_DS_4  = 3,
    PIN_DS_5  = 3,
    PIN_DS_6  = 3,
    PIN_DS_7  = 3,
    PIN_DS_8  = 3,
    PIN_DS_9  = 3,
    PIN_DS_10 = 3,
    PIN_DS_11 = 3,
    PIN_DS_12 = 3,
    PIN_DS_13 = 3,
    PIN_DS_14 = 3,
    PIN_DS_15 = 3,
    PIN_DS_MAX
} pin_drive_strength_t;

/**
 * @brief  Definition of pull-up/pull-down.
 */
typedef enum {
    PIN_PULL_NONE = 0,
    PIN_PULL_DOWN = 1,
    PIN_PULL_UP   = 2,
    PIN_PULL_MAX
} pin_pull_t;

/**
 * @brief  Definition of input enable.
 */
typedef enum {
    PIN_IE_DISABLE = 0,
    PIN_IE_ENABLE = 1,
    PIN_IE_MAX
} pin_input_enable_t;

typedef enum {
    /* pinmux mode 0 funciton */
    HAL_PIO_FUNC_GPIO = 0, // Default NON ULP GPIO
    HAL_PIO_ULP_GPIO_M0 = 0,

    /* pinmux mode 1 funciton */
    HAL_PIO_FUNC_QSPI0 = 1,
    HAL_PIO_FUNC_QSPI1 = 1,
    HAL_PIO_FUNC_QSPI2 = 1,
    HAL_PIO_FUNC_OPI = 1,
    HAL_PIO_FUNC_SDIO = 1,
    HAL_PIO_FUNC_SPI0 = 1,
    HAL_PIO_FUNC_SPI1 = 1,
    HAL_PIO_FUNC_SPI2_M1 = 1,
    HAL_PIO_FUNC_I2C0_M1 = 1,
    HAL_PIO_FUNC_I2C1_M1 = 1,
    HAL_PIO_FUNC_I2C2_M1 = 1,
    HAL_PIO_FUNC_I2C3_M1 = 1,
    HAL_PIO_FUNC_UART1 = 1,
    HAL_PIO_FUNC_UART2_M1 = 1,
    HAL_PIO_FUNC_UART_HS_UART = 1,
    HAL_PIO_FUNC_I2S0_M1 = 1,
    HAL_PIO_FUNC_I2S1_M1 = 1,
    HAL_PIO_FUNC_I2S2_M1 = 1,
    HAL_PIO_FUNC_BT_WIFI_SW = 1,
    HAL_PIO_FUNC_BT_ACT = 1,
    HAL_PIO_FUNC_BT_FREQ = 1,
    HAL_PIO_FUNC_BT_STATUS = 1,
    HAL_PIO_FUNC_WLAN_ACT = 1,
    HAL_PIO_FUNC_EXTLNA = 1,
    HAL_PIO_FUNC_TE = 1,
    HAL_PIO_FUNC_CLKOUT0_M1 = 1,
    HAL_PIO_FUNC_CLKOUT1_M1 = 1,
    HAL_PIO_FUNC_PWMP1_M1 = 1,
    HAL_PIO_FUNC_PWMP2_M1 = 1,
    HAL_PIO_FUNC_PWMP3_M1 = 1,
    HAL_PIO_FUNC_PWR = 1,
    HAL_PIO_FUNC_SWD = 1,

    /* pinmux mode 2 funciton */
    HAL_PIO_FUNC_EMMC = 2,
    HAL_PIO_FUNC_CLKOUT0_M2 = 2,
    HAL_PIO_FUNC_CLKOUT1_M2 = 2,
    HAL_PIO_FUNC_CLKIN_M2 = 2,
    HAL_PIO_FUNC_PWMN1_M2 = 2,
    HAL_PIO_FUNC_PWMN2_M2 = 2,
    HAL_PIO_FUNC_PWMN3_M2 = 2,
    HAL_PIO_FUNC_PWMP0_M2 = 2,
    HAL_PIO_FUNC_PWMP1_M2 = 2,
    HAL_PIO_FUNC_PWMP2_M2 = 2,
    HAL_PIO_FUNC_PWMP3_M2 = 2,
    HAL_PIO_FUNC_UART2_M2 = 2,
    HAL_PIO_FUNC_PDM_M2 = 2,

    /* pinmux mode 3 funciton */
    HAL_PIO_FUNC_I2C2_M3 = 3,
    HAL_PIO_FUNC_I2C3_M3 = 3,
    HAL_PIO_FUNC_PWMP0_M3 = 3,
    HAL_PIO_FUNC_PWMP1_M3 = 3,
    HAL_PIO_FUNC_PWMP2_M3 = 3,
    HAL_PIO_FUNC_PWMP3_M3 = 3,
    HAL_PIO_FUNC_PWMN0_M3 = 3,
    HAL_PIO_FUNC_PWMN1_M3 = 3,
    HAL_PIO_FUNC_PWMN2_M3 = 3,
    HAL_PIO_FUNC_PWMN3_M3 = 3,
    HAL_PIO_FUNC_CLKOUT0_M3 = 3,
    HAL_PIO_FUNC_CLKOUT1_M3 = 3,

    /* pinmux mode 4 funciton */
    HAL_PIO_FUNC_PWMN0_M4 = 4,
    HAL_PIO_FUNC_PWMN1_M4 = 4,
    HAL_PIO_FUNC_PWMN2_M4 = 4,
    HAL_PIO_FUNC_PWMN3_M4 = 4,
    HAL_PIO_FUNC_CLKIN = 4,
    HAL_PIO_FUNC_DSI_TE = 4,
    HAL_PIO_FUNC_DSP_I2C = 4,
    HAL_PIO_FUNC_DSP_UART_BRANDY = 4,

    /* pinmux mode 5 funciton */
    HAL_PIO_FUNC_I2C0_M5 = 5,
    HAL_PIO_FUNC_I2C1_M5 = 5,
    HAL_PIO_FUNC_I2C2_M5 = 5,
    HAL_PIO_FUNC_I2C3_M5 = 5,
    HAL_PIO_FUNC_I2S2_M5 = 5,

    /* pinmux mode 6 funciton */
    HAL_PIO_FUNC_BT_WIFI_SW_M6 = 6,
    HAL_PIO_FUNC_BT_ACT_M6 = 6,
    HAL_PIO_FUNC_BT_FREQ_M6 = 6,
    HAL_PIO_FUNC_BT_STATUS_M6 = 6,
    HAL_PIO_FUNC_WLAN_ACT_M6 = 6,
    HAL_PIO_FUNC_BT_DUAL_ANT_SW = 6,
    HAL_PIO_FUNC_EXTLNA_M6 = 6,

    /* pinmux mode 7 funciton */
    HAL_PIO_FUNC_DIAG = 7, // DIAG[0] ~ DIAG[15]

    HAL_PIO_FUNC_MAX = 8,
    // input high resistance. Need config as HAL_PIO_FUNC_BT_GPIO and GPIO input mode
    HAL_PIO_FUNC_DEFAULT_HIGH_Z = 0xf,
} hal_pio_func_t;

typedef enum {
    HAL_PIO_PULL_NONE,  //!< No pull down or pull up enabled.
    HAL_PIO_PULL_DOWN,  //!< Pull down enabled for this pin.
    HAL_PIO_PULL_UP,    //!< Pull up enabled for this pin.
    HAL_PIO_PULL_MAX,
} hal_pio_pull_t;

typedef enum {
    HAL_PIO_DRIVE_0 = 0, // !< lowest pio current dirve strength.
    HAL_PIO_DRIVE_1 = 1,
    HAL_PIO_DRIVE_2 = 2,
    HAL_PIO_DRIVE_3 = 3,
    HAL_PIO_DRIVE_4 = 3,
    HAL_PIO_DRIVE_5 = 3,
    HAL_PIO_DRIVE_6 = 3,
    HAL_PIO_DRIVE_7 = 3,
    HAL_PIO_DRIVE_8 = 3,
    HAL_PIO_DRIVE_9 = 3,
    HAL_PIO_DRIVE_10 = 3,
    HAL_PIO_DRIVE_11 = 3,
    HAL_PIO_DRIVE_12 = 3,
    HAL_PIO_DRIVE_13 = 3,
    HAL_PIO_DRIVE_14 = 3,
    HAL_PIO_DRIVE_15 = 3, // !< highest pio current dirve strength.
    HAL_PIO_DRIVE_MAX, // 表示无效值，board_evb.h中g_pio_function_config配置HAL_PIO_DRIVE_MAX表示不配置，使用默认值
} hal_pio_drive_t;

typedef enum {
    HAL_PIO_IE_DISABLE = 0,
    HAL_PIO_IE_ENBALE  = 1,
    HAL_PIO_IE_MAX,
} hal_pio_ie_t;

typedef struct {
    hal_pio_func_t func;
    hal_pio_drive_t drive;
    hal_pio_pull_t pull;
#if defined(CONFIG_PINCTRL_SUPPORT_IE)
    hal_pio_ie_t ie;
#endif
} hal_pio_config_t;

/**
 * @brief  Check whether the mode configured for the pin is valid.
 * @param  [in]  pin  The index of pins. see @ref pin_t
 * @param  [in]  mode The Multiplexing mode. see @ref pin_mode_t
 * @return The value 'true' indicates that the mode is valid and the value 'false' indicates that the mode is invalid.
 */
bool pin_check_mode_is_valid(pin_t pin, pin_mode_t mode);

/**
 * @brief  Register hal funcs objects into hal_pinctrl module.
 */
void pin_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_pinctrl module.
 */
void pin_port_unregister_hal_funcs(void);

/**
 * @brief  Recovery the function of pinmux except of norflash pin after wakipng up.
 */
void pin_port_recovery_function(void);

void get_pio_func_config(size_t *pin_num, hal_pio_config_t **pin_func_array);
void get_gpio_func_config(size_t *pin_num, hal_gpio_config_t **pin_func_array);

/**
 * @brief  IE disable enable for the power-off pin.
 */
void pin_of_poweroff_suspend(void);

/**
 * @brief  Initial configuration of the power-off pin restore IE enable.
 */
void pin_of_poweroff_resume(void);

/**
 * @brief  Disables the IE function for the pin that is not powered off.
 */
void pin_of_poweron_suspend(void);

/**
 * @}
 */
#define HAL_PIO_FUNC_INVALID        HAL_PIO_FUNC_MAX

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
