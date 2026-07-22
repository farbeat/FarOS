/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides gpio port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-26， Create file. \n
 */
#ifndef GPIO_PORTING_H
#define GPIO_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_gpio GPIO
 * @ingroup  drivers_port
 * @{
 */

/**
 * @brief  The number of channels.
 * @note   This number might be different for each chip and core.
 */

#define GPIO_CHANNELS_NUM GPIO_MAX_NUMBER

#ifndef GPIO_WITH_ULP
#define GPIO_WITH_ULP                   YES
#endif

#ifndef ENABLE_GPIO_INTERRUPT
#define ENABLE_GPIO_INTERRUPT           YES
#endif

#ifndef CONFIG_GPIO_PWIDTH_A
#define CONFIG_GPIO_PWIDTH_A           32
#endif

#ifndef CONFIG_GPIO_PWIDTH_B
#define CONFIG_GPIO_PWIDTH_B           30
#endif

#ifndef CONFIG_GPIO_PWIDTH_C
#define CONFIG_GPIO_PWIDTH_C           30
#endif

#ifndef CONFIG_GPIO_PWIDTH_D
#define CONFIG_GPIO_PWIDTH_D           30
#endif

#ifndef CONFIG_GPIO_WIDTH
#define CONFIG_GPIO_WIDTH              30
#endif

#define GPIO_INTERRUPT_RISING_EDGE            0x00000001
#define GPIO_INTERRUPT_FALLING_EDGE           0x00000002
#define GPIO_INTERRUPT_LOW                    0x00000004
#define GPIO_INTERRUPT_HIGH                   0x00000008
#define GPIO_INTERRUPT_DEDGE                  (GPIO_INTERRUPT_RISING_EDGE | GPIO_INTERRUPT_FALLING_EDGE)

/**
 * @brief  Definition of GPIO Channel index.
 */
typedef enum gpio_channel {
    GPIO_CHANNEL_0,             /*!< GPIO Channel 0 */
    GPIO_CHANNEL_1,             /*!< GPIO Channel 1 */
    GPIO_CHANNEL_2,             /*!< GPIO Channel 2 */
    GPIO_CHANNEL_3,             /*!< GPIO Channel 3 */
#if (GPIO_WITH_ULP == YES)
    GPIO_CHANNEL_5 = 4,
#endif
    GPIO_CHANNEL_MAX_NUM
} gpio_channel_t;

typedef enum hal_gpio_direction {
    HAL_GPIO_DIRECTION_INPUT,
    HAL_GPIO_DIRECTION_OUTPUT,

    HAL_GPIO_DIRECTION_MAX,
} hal_gpio_direction_t;

typedef enum hal_gpio_level {
    HAL_GPIO_LEVEL_LOW,
    HAL_GPIO_LEVEL_HIGH,

    HAL_GPIO_LEVEL_MAX,
} hal_gpio_level_t;

typedef void (*hal_gpio_callback_t)(pin_t pin, uintptr_t param);

/**
 * @brief  Definition of GPIO config.
 */
typedef struct {
    pin_t gpio;
    hal_gpio_direction_t direction;
    hal_gpio_level_t level;
    uint32_t triger;
    hal_gpio_callback_t callback;
} hal_gpio_config_t;

/**
 * @brief  Get the base address of a specified GPIO.
 * @param  [in]  channel The channel of GPIO.
 * @return The base address of specified GPIO.
 */
uintptr_t gpio_porting_base_addr_get(gpio_channel_t channel);

/**
 * @brief  Register hal funcs objects into hal_gpio module.
 */
void gpio_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_gpio module.
 */
void gpio_port_unregister_hal_funcs(void);

/**
 * @brief  Register the interrupt of gpios.
 */
void gpio_port_register_irq(uint32_t int_id);

/**
 * @brief  Handler of GPIO IRQ.
 */
int irq_gpio_handler(int a, const void *tmp);

/**
 * @brief  Adapter the project.
 */
void gpio_context_adapter(void);

/**
 * @brief  Hal adapter the project.
 */
void gpio_hal_context_adapter(void);

/**
 * @brief  Enable ulp gpio interrupt, set ulp gpio clk as 32K.
 * @param  on True enable and set clk as 32K, false disable ulp gpio interrupt.
 */
void gpio_ulp_int_en(bool on);

/**
 * @brief  Switch ULP GPIO clk from 32K to 32M.
 * @param  on True set as 32M, false set as 32K.
 */
void gpio_ulp_clock_switch_to_pclk(bool on);

/**
 * @brief Gpio select core.
 * @param pin
 * @param core
 */
void gpio_select_core(pin_t pin, cores_t core);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif