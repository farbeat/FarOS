/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pinctrl port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-25， Create file. \n
 */
#include "pinctrl_porting.h"
#include "hal_pinctrl.h"
#include "hal_pinctrl_brandy.h"
#ifdef PRE_FPGA
#include "board_fpga.h"
#elif PRE_ASIC
#include "board_evb.h"
#endif
#include "pinctrl.h"

#ifndef BIT
#define BIT(x) (1UL << (uint32_t)(x))
#endif
#define PAD_STEP 0x4
#define IE_ENABLE_BIT 1
#define S_EGPIO0_PAD_REG 0x5700ce00
#define S_EGPIO25_PAD_REG 0x5700ce64

#define S_HGPIO0_PAD_REG 0x5700d200
#define S_HGPIO15_PAD_REG 0x5700d23c
#define S_MGPIO0_PAD_REG 0x5700ca00

#define S_MGPIO38_PAD_REG 0x5700ca98
#define S_MGPIO20_PAD_REG 0x5700ca50
#define S_MGPIO34_PAD_REG 0x5700ca88

#define S_AGPIOL0_PAD_REG 0x5700c600
#define S_AGPIOL32_PAD_REG 0x5700c680
#define S_AGPIOL3_PAD_REG 0x5700c60c
#define S_AGPIOL4_PAD_REG 0x5700c610
#define S_AGPIOL10_PAD_REG 0x5700c628

#define S_AGPIOR0_PAD_REG 0x5700da00
#define S_AGPIOR10_PAD_REG 0x5700da28
#define S_AGPIOR5_PAD_REG 0x5700da14

#define ULP_GPIO0_PAD_REG 0x5702c200
#define ULP_GPIO4_PAD_REG 0x5702c210


typedef enum {
    PIN_FUNC_0 = BIT(PIN_MODE_0),
    PIN_FUNC_1 = BIT(PIN_MODE_1),
    PIN_FUNC_2 = BIT(PIN_MODE_2),
    PIN_FUNC_3 = BIT(PIN_MODE_3),
    PIN_FUNC_4 = BIT(PIN_MODE_4),
    PIN_FUNC_5 = BIT(PIN_MODE_5),
    PIN_FUNC_6 = BIT(PIN_MODE_6),
    PIN_FUNC_7 = BIT(PIN_MODE_7),
    PIN_FUNC_NONE = 0,
} pin_mode_func_t;


typedef enum {
    PIO_MODE_0 = BIT(0),
    PIO_MODE_1 = BIT(1),
    PIO_MODE_2 = BIT(2),
    PIO_MODE_3 = BIT(3),
    PIO_MODE_4 = BIT(4),
    PIO_MODE_5 = BIT(5),
    PIO_MODE_6 = BIT(6),
    PIO_MODE_7 = BIT(7),
    PIO_MODE_NONE = 0,
} pio_mode_t;

static uint8_t const g_pio_pins_avaliable_mode[PIN_MAX_NUMBER] = {
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO0
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO1
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO2
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO3
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO4
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO5
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO6
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO7
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO8
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO9
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO10
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO11
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO12
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO13
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO14
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO15
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO16
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO17
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO18
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO19
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO20
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO21
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO22
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO23
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO24
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_EGPIO25
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO0
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO1
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO2
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO3
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO4
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_HGPIO5
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO6
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO7
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO8
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO9
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO10
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO11
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO12
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_7,                                         // S_HGPIO13
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_7,                                         // S_HGPIO14
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_HGPIO15
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO0
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_5 | PIO_MODE_7,               // S_MGPIO1
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_5 | PIO_MODE_7,               // S_MGPIO2
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_5 | PIO_MODE_7,               // S_MGPIO3
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_5 | PIO_MODE_7,               // S_MGPIO4
    PIO_MODE_0 | PIO_MODE_5 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO5
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_6 | PIO_MODE_7,               // S_MGPIO6
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO7
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO8
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO9
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO10
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO11
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO12
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO13
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO14
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO15
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO16
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_MGPIO17
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_MGPIO18
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_MGPIO19
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_MGPIO20
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_MGPIO21
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_MGPIO22
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO23
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO24
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_MGPIO25
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_MGPIO26
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO27
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO28
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO29
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO30
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO31
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_6 | PIO_MODE_7,                            // S_MGPIO32
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,  // S_MGPIO33
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,  // S_MGPIO34
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,  // S_MGPIO35
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,  // S_MGPIO36
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,  // S_MGPIO37
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_5 | PIO_MODE_7,               // S_MGPIO38
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_7,                                         // S_AGPIO_L0
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_AGPIO_L1
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_AGPIO_L2
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_AGPIO_L3
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_AGPIO_L4
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_7,               // S_AGPIO_L5
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_7,               // S_AGPIO_L6
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_7,  // S_AGPIO_L7
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_7,  // S_AGPIO_L8
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_4 | PIO_MODE_7,                            // S_AGPIO_L9
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_4 | PIO_MODE_7,                            // S_AGPIO_L10
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_7,                            // S_AGPIO_L11
    PIO_MODE_0 | PIO_MODE_7,                                                      // S_AGPIO_L12
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_5 | PIO_MODE_7,               // S_AGPIO_L13
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_5 | PIO_MODE_7,               // S_AGPIO_L14
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_5 | PIO_MODE_7,               // S_AGPIO_L15
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_5 | PIO_MODE_7,               // S_AGPIO_L16
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_4 | PIO_MODE_6 | PIO_MODE_7,  // S_AGPIO_L17
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_4 | PIO_MODE_6 | PIO_MODE_7,               // S_AGPIO_L18
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_6 | PIO_MODE_7,  // S_AGPIO_L19
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_6 | PIO_MODE_7,  // S_AGPIO_L20
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_6 | PIO_MODE_7,  // S_AGPIO_L21
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_3 | PIO_MODE_4 | PIO_MODE_7,               // S_AGPIO_L22
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_4 | PIO_MODE_5 | PIO_MODE_7,               // S_AGPIO_L23
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_5 | PIO_MODE_7,                            // S_AGPIO_L24
    PIO_MODE_0 | PIO_MODE_5 | PIO_MODE_7,                                         // S_AGPIO_L25
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L26
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L27
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L28
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L29
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,               // S_AGPIO_L30
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L31
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_L32
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,               // S_AGPIO_R0
    PIO_MODE_0 | PIO_MODE_1 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,               // S_AGPIO_R1
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R2
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R3
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R4
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R5
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R6
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R7
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R8
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R9
    PIO_MODE_0 | PIO_MODE_2 | PIO_MODE_3 | PIO_MODE_7,                            // S_AGPIO_R10

#if CORE == APPS
    PIO_MODE_NONE,                                                                                // ULP_GPIO0
    PIO_MODE_NONE,                                                                                // ULP_GPIO1
    PIO_MODE_NONE,                                                                                // ULP_GPIO2
    PIO_MODE_NONE,                                                                                // ULP_GPIO3
    PIO_MODE_0 | PIO_MODE_1,                                                                      // ULP_GPIO4
    PIO_MODE_0 | PIO_MODE_1,                                                                      // ULP_GPIO5
    PIO_MODE_0 | PIO_MODE_1,                                                                      // ULP_GPIO6
#endif
};

void get_pio_func_config(size_t *pin_num, hal_pio_config_t **pin_func_array)
{
    if (pin_num == NULL || pin_func_array == NULL) { return; }
    *pin_num = sizeof(g_pio_function_config) / sizeof(g_pio_function_config[0]);
    *pin_func_array = g_pio_function_config;
}

void get_gpio_func_config(size_t *pin_num, hal_gpio_config_t **pin_func_array)
{
    if (pin_num == NULL || pin_func_array == NULL) { return; }
    *pin_num = sizeof(g_gpio_function_config) / sizeof(g_gpio_function_config[0]);
    *pin_func_array = g_gpio_function_config;
}

bool pin_check_mode_is_valid(pin_t pin, pin_mode_t mode)
{
    return (bool)((g_pio_pins_avaliable_mode[pin] >> (uint8_t)mode) & 1U);
}

void pin_port_register_hal_funcs(void)
{
    hal_pin_register_funcs(hal_pin_brandy_funcs_get());
}

void pin_port_unregister_hal_funcs(void)
{
    hal_pin_unregister_funcs();
}

void pin_port_recovery_function(void)
{
    size_t pin_num;
    hal_pio_config_t *pio_func = NULL;

    get_pio_func_config(&pin_num, &pio_func);
    for (pin_t i = S_EGPIO6; i <= S_MGPIO38; i++) {
        if (pio_func[i].func != HAL_PIO_FUNC_INVALID && pio_func[i].func != HAL_PIO_FUNC_GPIO) {
            uapi_pin_set_mode(i, (pin_mode_t)pio_func[i].func);
        }
    }
}

void pin_of_poweroff_suspend(void)
{
    uint32_t base_addr = 0;
    // egpio
    for (base_addr = S_EGPIO0_PAD_REG; base_addr <= S_EGPIO25_PAD_REG; base_addr += PAD_STEP) {
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }
    // hgpio
    for (base_addr = S_HGPIO0_PAD_REG; base_addr <= S_HGPIO15_PAD_REG; base_addr += PAD_STEP) {
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }
    // mgpio
    for (base_addr = S_MGPIO0_PAD_REG; base_addr <= S_MGPIO38_PAD_REG; base_addr += PAD_STEP) {
        if (base_addr == S_MGPIO20_PAD_REG || base_addr == S_MGPIO34_PAD_REG) {
            continue;
        }
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }
}

void pin_of_poweroff_resume(void)
{
    size_t pin_num;
    hal_pio_config_t *pio_func = NULL;
    get_pio_func_config(&pin_num, &pio_func);

    for (pin_t i = S_EGPIO0; i < S_MGPIO38; i++) {
        if (pio_func[i].ie != HAL_PIO_IE_MAX) {
            uapi_pin_set_ie(i, (pin_input_enable_t)pio_func[i].ie);
        }
    }
}

void pin_of_poweron_suspend(void)
{
    uint32_t base_addr = 0;
    // agpio l
    for (base_addr = S_AGPIOL0_PAD_REG; base_addr <= S_AGPIOL32_PAD_REG; base_addr += PAD_STEP) {
        if (base_addr == S_AGPIOL3_PAD_REG || base_addr == S_AGPIOL4_PAD_REG || base_addr == S_AGPIOL10_PAD_REG) {
            continue;
        }
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }
    // agpio r
    for (base_addr = S_AGPIOR0_PAD_REG; base_addr <= S_AGPIOR10_PAD_REG; base_addr += PAD_STEP) {
        if (base_addr == S_AGPIOR5_PAD_REG) { // S_AGPIO_R5 touch int
            continue;
        }
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }

    // ulp gpio
    for (base_addr = ULP_GPIO0_PAD_REG; base_addr <= ULP_GPIO4_PAD_REG; base_addr += PAD_STEP) {
        reg16_clrbit(base_addr, IE_ENABLE_BIT);
    }
}