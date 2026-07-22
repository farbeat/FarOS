/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: CLOCK CALIBRATION DRIVER.
 * Author: @CompanyNameTag
 * Create: 2020-03-17
 */
#include "clock_calibration.h"
#include "platform_core.h"
#include "hal_32k_clock.h"
#include "otp_map.h"
#include "otp.h"
#include "hal_tcxo.h"
#include "chip_io.h"
#include "soc_osal.h"
#include "tcxo.h"
#include "tcxo_porting.h"
#define CALIBRATION_XO_CORE_TRIM_REG            XO_CORE_TRIM_REG
#define CALIBRATION_XO_CORE_CTRIM_REG           XO_CORE_CTRIM_REG

#define CALIBRATION_XO_CORE_TRIM_DEFAULT        1
#define CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT   3
#define CALIBRATION_XO_CORE_CTRIM_INTEGER_LEN   4
#define CALIBRATION_XO_CORE_CTRIM_INTEGER_MAX   0xE
#define CALIBRATION_XO_CORE_CTRIM_INTEGER_MIN   1
#define CALIBRATION_XO_CORE_CTRIM_INTEGER_POS   0x0f

#define CALIBRATION_XO_CORE_CTRIM_DECIMAL_BIT   0
#define CALIBRATION_XO_CORE_CTRIM_DECIMAL_LEN   3
#define CALIBRATION_XO_CORE_CTRIM_DECIMAL_MAX   0x07
#define CALIBRATION_XO_CORE_CTRIM_DECIMAL_MIN   0
#define CALIBRATION_XO_CORE_CTRIM_DECIMAL_POS   0x07

#define CALIBRATION_CLOCK_MUL                   100
#define CALIBRATION_CLOCK_FREQ_32K_100          3276800

#define XO_CTRIM_CONFIG_WAIT_US                 20ULL
#define XO_CTRIM_ADJUST_INTERVAL_VALUE          8
#define XO_CTRIM_LOW_BIT_ADJUST_MASK            7

static uint32_t g_clock_32k = CALIBRATION_CLOCK_FREQ_32K_100;

uint32_t calibration_get_clock_frq(uint16_t cali_cycle)
{
    uint32_t result;
    hal_32k_clock_calibration_cycle_config(cali_cycle);
    result = hal_32k_clock_get_detect_result();
    if (result != 0) {
        g_clock_32k = (uint32_t)(((uint64_t)(cali_cycle) * HAL_TCXO_TICKS_PER_SECOND *
                    CALIBRATION_CLOCK_MUL) / result);
    }

    return g_clock_32k; // return 32K clock
}

#if(EXTERNAL_CLOCK_CALIBRATION == YES)
uint32_t calibration_get_external_clock_frq(uint32_t cali_count)
{
    uint32_t count_result;
    uint32_t clock_result = 0;
    hal_external_clock_calibration_count_config(cali_count);
    count_result = hal_external_clock_get_detect_result();
    if (count_result != 0) {
        clock_result = (uint32_t)(((uint64_t)(count_result) * HAL_TCXO_TICKS_PER_SECOND) / cali_count);
    }

    return clock_result; // return external clock detect frequency
}
#endif

uint32_t calibration_get_clock_frq_result(void)
{
    return g_clock_32k;
}

void calibration_xo_core_trim_init(void)
{
    // need use trim value from otp
    // the xo_trim and xo_ctrim should update at same time like this:
    // xo_trim:    0x1  0x03  ...  0x0F
    // xo_ctrim:   0x53 0x56  ...  0x76
    writew(CALIBRATION_XO_CORE_TRIM_REG, CALIBRATION_XO_CORE_TRIM_DEFAULT);
}

void calibration_xo_core_ctrim_init(void)
{
    uint8_t xo_ctrim_cur_value;
    uint8_t xo_ctrim_value = 0;
    uint8_t value;
    uint8_t interval_value = XO_CTRIM_ADJUST_INTERVAL_VALUE;
    otp_read_byte(&xo_ctrim_value, XO_CORE_CTRIM);
    if (xo_ctrim_value == 0) {
        // use default value if NO otp config
        xo_ctrim_value = XO_CTRIM_VALUR_DEFAULT;
    }

    xo_ctrim_cur_value = (uint8_t)readw(CALIBRATION_XO_CORE_CTRIM_REG);
    value = (xo_ctrim_value & XO_CTRIM_LOW_BIT_ADJUST_MASK) | (xo_ctrim_cur_value & ~XO_CTRIM_LOW_BIT_ADJUST_MASK);
    writew(CALIBRATION_XO_CORE_CTRIM_REG, value);
    while (value != xo_ctrim_value) {
        value = (value > xo_ctrim_value) ? (value - interval_value) : (value + interval_value);
        uapi_tcxo_delay_us(XO_CTRIM_CONFIG_WAIT_US);
        writew(CALIBRATION_XO_CORE_CTRIM_REG, (uint16_t)value);
    }
}

void calibration_set_xo_core_ctrim(uint8_t xo_ctrim_integer, xo_ctrim_decimal_t xo_ctrim_decimal)
{
    if (xo_ctrim_integer > CALIBRATION_XO_CORE_CTRIM_INTEGER_MAX) {
        return;
    }
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT,
        CALIBRATION_XO_CORE_CTRIM_INTEGER_LEN, xo_ctrim_integer);
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_DECIMAL_BIT,
        CALIBRATION_XO_CORE_CTRIM_DECIMAL_LEN, xo_ctrim_decimal);
}

void calibration_save_xo_core_ctrim(uint8_t xo_ctrim_integer, xo_ctrim_decimal_t xo_ctrim_decimal)
{
    uint8_t value = 0;
    if (xo_ctrim_integer > CALIBRATION_XO_CORE_CTRIM_INTEGER_MAX) {
        return;
    }
    otp_read_byte(&value, XO_CORE_CTRIM);

    if (value == 0) {
        value = (uint8_t)((xo_ctrim_integer << CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT) | (uint8_t)xo_ctrim_decimal);
        otp_write_byte(XO_CORE_CTRIM, value);
    }
}

void calibration_read_xo_core_ctrim(uint8_t *xo_ctrim_value)
{
    if (xo_ctrim_value == NULL) {
        return;
    }
    otp_read_byte(xo_ctrim_value, XO_CORE_CTRIM);
}

void calibration_xo_core_ctrim_algorithm(bool increase, uint8_t integer_step_num, uint8_t decimal_step_num)
{
    uint8_t xo_integer = reg16_getbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT,
        CALIBRATION_XO_CORE_CTRIM_INTEGER_LEN);

    uint8_t xo_decimal = reg16_getbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_DECIMAL_BIT,
        CALIBRATION_XO_CORE_CTRIM_DECIMAL_LEN);

    if (increase) {
        if ((xo_integer + integer_step_num) > CALIBRATION_XO_CORE_CTRIM_INTEGER_MAX) {
            xo_integer = CALIBRATION_XO_CORE_CTRIM_INTEGER_MAX;
        } else {
            xo_integer = xo_integer + integer_step_num;
        }

        if ((xo_decimal + decimal_step_num) > CALIBRATION_XO_CORE_CTRIM_DECIMAL_MAX) {
            xo_decimal = CALIBRATION_XO_CORE_CTRIM_DECIMAL_MAX;
        } else {
            xo_decimal = xo_decimal + decimal_step_num;
        }
    } else {
        if (xo_integer < integer_step_num) {
            xo_integer = CALIBRATION_XO_CORE_CTRIM_INTEGER_MIN;
        } else {
            xo_integer = xo_integer - integer_step_num;
        }

        if (xo_decimal < decimal_step_num) {
            xo_decimal = CALIBRATION_XO_CORE_CTRIM_DECIMAL_MIN;
        } else {
            xo_decimal = xo_decimal - decimal_step_num;
        }
    }

    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT,
        CALIBRATION_XO_CORE_CTRIM_INTEGER_LEN, xo_integer);
    reg16_setbits(CALIBRATION_XO_CORE_CTRIM_REG, CALIBRATION_XO_CORE_CTRIM_DECIMAL_BIT,
        CALIBRATION_XO_CORE_CTRIM_DECIMAL_LEN, xo_decimal);
}

void calibration_get_xo_core_ctrim_reg(uint8_t *xo_ctrim_value, uint8_t *xo_ctrim_integer, uint8_t *xo_ctrim_decimal)
{
    if ((xo_ctrim_value == NULL) || (xo_ctrim_integer == NULL) || (xo_ctrim_decimal == NULL)) {
        return;
    }

    *xo_ctrim_value = (uint8_t)readw(CALIBRATION_XO_CORE_CTRIM_REG);
    *xo_ctrim_integer = ((*xo_ctrim_value) >> CALIBRATION_XO_CORE_CTRIM_INTEGER_BIT) &
                         CALIBRATION_XO_CORE_CTRIM_INTEGER_POS;
    *xo_ctrim_decimal = ((*xo_ctrim_value) & CALIBRATION_XO_CORE_CTRIM_DECIMAL_POS);
}

void calibration_set_xo_core_trim_reg(uint8_t xo_trim)
{
    uint32_t irq_sts = osal_irq_lock();
    writew(CALIBRATION_XO_CORE_TRIM_REG, xo_trim);
    osal_irq_restore(irq_sts);
}
