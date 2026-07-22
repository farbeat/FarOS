/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides adc port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */

#include "adc_porting.h"
#include "pmu_ldo.h"
#include "chip_core_irq.h"
#include "soc_osal.h"
#include "hal_clocks.h"
#include "common_def.h"
#include "pinctrl.h"
#include "efuse.h"
#include "pinctrl_porting.h"
#include "hal_adc_v151.h"
#include "platform_core.h"
#include "tcxo.h"
#include "cmsis_os.h"
#include "hal_adc.h"
#include "otp_map.h"
#include "cpu_trace.h"
#include "pm_veto.h"

#define ADC_BASE_ADDR 0x52000B00

#define HAL_ADC_CHANNEL01_SELCET  0x3
#define HAL_ADC_CHANNEL23_SELCET  0xC
#define HAL_ADC_CHANNEL0_SELCET   1
#define HAL_ADC_CHANNEL2_SELCET   4
#define HAL_ADC_CHANNEL_SHIFT_VAL   8

#define HAL_ADC_SCAN_DIS_NUM_0        HAL_ADC_SCAN_DIS_16  /* diff with buffer channel and single with buffer channel */
#define HAL_ADC_SCAN_DIS_NUM_1        HAL_ADC_SCAN_DIS_16  /* single without buffer channel */
#define HAL_ADC_SCAN_AVG_NUM_0        HAL_ADC_SCAN_AVG_16  /* diff with buffer channel and single with buffer channel */
#define HAL_ADC_SCAN_AVG_NUM_1        HAL_ADC_SCAN_AVG_16  /* single without buffer channel */

#define ADC_CLOCK_015KHZ_DIV_VALUE 0x80
#define ADC_CLOCK_125KHZ_DIV_VALUE 0x10
#define ADC_CLOCK_250KHZ_DIV_VALUE 0x08
#define ADC_CLOCK_500KHZ_DIV_VALUE 0x04

#define HAL_ADC_M_DIV_3_OFFSET 1

#define CMU_CLOCKS_M_DIV3_REG                   (M_CTL_RB_BASE + 0x7C)
#define CMU_CLOCKS_AUX_ADC_DIV                  0
#define CMU_CLOCKS_AUX_ADC_DIV_LEN              8
#define CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN          8
#define CMU_CLOCKS_AUX_ADC_DIV_EN               9

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN) && defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
#define SAMPLE_OFFSET               2
#define AUX_ADC_CH_SEL_OFFSET       5
#define ADDR_OFFSET                 4
#define DIAG_MAX_LEN                2048 // 2048 * 4 = 8K
#define ALL_CLK_EN                  0x1ff
#define CPU_TRACE                   0x1
#define RAW_DATA                    0x1
#define AUX_ADC_CALI_EN             0x1
#define AUX_ADC_CH_SEL              (g_adc_chan_sel << AUX_ADC_CH_SEL_OFFSET)
#define DIAG_SAMPLE_LENGTH_L        (0x2000)
#define DIAG_SAMPLE_SEL_AUX_ADC     0x6
#define DIAG_SAMPLE_START_ADDR_L    0x6000
#define DIAG_SAMPLE_START_ADDR_H    0x5200
#define DIAG_SAMPLE_END_ADDR_L      (0x6000 + 0x2000)
#define DIAG_SAMPLE_END_ADDR_H      0x5200
#define AUX_ADC_SAMPLE_PERIOD       (4000 / g_adc_clk)
#define DIAG_SAMPLE_EN              0x200
#define DIAG_SAMPLE_SYNC            0x300

// reg offset
#define CFG_MCU_DIAG_SAMPLE_MODE            0x204
#define CFG_MCU_DIAG_MONITOR_CLOCK          0x10C
#define CFG_MONITOR_SEL                     0x100
#define AUX_ADC_CALI_BYPASS                 0xB5C
#define AUX_ADC_CALI_CFG_SYNC               0xB30
#define AUX_ADC_AUTO_CFG                    0xB14
#define CFG_MCU_DIAG_SAMPLE_LENGTH_L        0x208
#define CFG_MCU_DIAG_SAMPLE_SEL             0x200
#define CFG_MCU_DIAG_SAMPLE_START_ADDR_L    0x210
#define CFG_MCU_DIAG_SAMPLE_START_ADDR_H    0x214
#define CFG_MCU_DIAG_SAMPLE_END_ADDR_L      0x218
#define CFG_MCU_DIAG_SAMPLE_END_ADDR_H      0x21C
#define CFG_AUX_ADC_SAMPLE_PERIOD           0x22C
uint8_t g_adc_chan_sel = 0;
float g_adc_clk = 0.0;
uint32_t g_adc_long_sample_time = 0;
static hal_adc_callback_t g_adc_receive_data_cb = NULL;

osTimerId_t g_adc_timer = NULL;
bool g_adc_auto_sample_working = false;
#endif

#define adc_trans_sytick_to_voltage_without_auto_scan(x) ((float)(((float)(x) * 18) / 40960))

#define AUX_ADC_CALI_GAIN0_POS  0
#define AUX_ADC_CALI_GAIN0_LEN  8
#define AUX_ADC_CALI_GAIN1_POS  8
#define AUX_ADC_CALI_GAIN1_LEN  5

#define AUX_ADC_CALI_OFFSET0_POS  0
#define AUX_ADC_CALI_OFFSET0_LEN  3
#define AUX_ADC_CALI_OFFSET1_POS  3
#define AUX_ADC_CALI_OFFSET1_LEN  8
#define AUX_ADC_CALI_OFFSET2_POS  11
#define AUX_ADC_CALI_OFFSET2_LEN  3

typedef struct {
    uint32_t cali_gain0 : 8;
    uint32_t cali_gain1 : 5;
    uint32_t cali_offset0 : 3;
    uint32_t cali_offset1 : 8;
    uint32_t cali_offset2 : 3;
    uint32_t reserved : 5;
} cali_date_t;

typedef union {
    uint32_t raw;
    cali_date_t date;
} aux_adc_cali_gain_t;

hal_adc_type_info_t g_adc_cfg_info[] = {
    { ADC_CHANNEL_0, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_1, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_2, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_3, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_4, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_5, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_6, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_7, HAL_ADC_CHANNEL_TYPE_SIN_NOBUF, 0 },
    { ADC_CHANNEL_NONE, HAL_ADC_CHANNEL_TYPE_NONE, 0}
};

uintptr_t g_adc_base_addr =  (uintptr_t)ADC_BASE_ADDR;

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN) && defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
static void hal_diag_init(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_MODE, 0);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_MONITOR_CLOCK, ALL_CLK_EN);
    reg_writew(DIAG_BASE_ADDR, CFG_MONITOR_SEL, CPU_TRACE);
}

static void hal_aux_adc_set_channel(void)
{
    reg_writew(M_CTL_RB_BASE, AUX_ADC_CALI_BYPASS, RAW_DATA);
    reg_writew(M_CTL_RB_BASE, AUX_ADC_CALI_CFG_SYNC, AUX_ADC_CALI_EN);
    reg_writew(M_CTL_RB_BASE, AUX_ADC_AUTO_CFG, AUX_ADC_CH_SEL);
}

static void hal_diag_set_sample_sel(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_SEL, DIAG_SAMPLE_SEL_AUX_ADC);
}

static void hal_diag_set_addr_and_len(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_LENGTH_L, DIAG_SAMPLE_LENGTH_L);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_START_ADDR_L, DIAG_SAMPLE_START_ADDR_L);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_START_ADDR_H, DIAG_SAMPLE_START_ADDR_H);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_END_ADDR_L, DIAG_SAMPLE_END_ADDR_L);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_END_ADDR_H, DIAG_SAMPLE_END_ADDR_H);
}

static void hal_diag_set_sample_period(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_AUX_ADC_SAMPLE_PERIOD, AUX_ADC_SAMPLE_PERIOD);
}

static void hal_diag_set_sample_start(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_MODE, DIAG_SAMPLE_EN);
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_MODE, DIAG_SAMPLE_SYNC);
}

static void hal_diag_set_stop(void)
{
    reg_writew(DIAG_BASE_ADDR, CFG_MCU_DIAG_SAMPLE_MODE, 0);
}

static void adc_port_set_diag_reg(void)
{
    hal_diag_init();
    hal_aux_adc_set_channel();
    hal_diag_set_sample_sel();
    hal_diag_set_addr_and_len();
    hal_diag_set_sample_period();
    hal_diag_set_sample_start();
}

static float adc_port_transfer_freq(hal_adc_scan_freq_t freq)
{
    if (freq == HAL_ADC_SCAN_FREQ_100HZ) {
        return 0.1; // 0.1KHz
    } else if (freq == HAL_ADC_SCAN_FREQ_200HZ) {
        return 0.2; // 0.2KHz
    } else if (freq == HAL_ADC_SCAN_FREQ_250HZ) {
        return 0.25; // 0.25KHz
    } else if (freq == HAL_ADC_SCAN_FREQ_500HZ) {
        return 0.5; // 0.5KHz
    } else if (freq == HAL_ADC_SCAN_FREQ_1KHZ) {
        return 1.0; // 1.0KHz
    } else {
        osal_printk("Input wrong freq!\r\n");
        return -1.0; // -1.0非法KHz
    }
}

static void adc_port_sample_cb(void* argument)
{
    unused(argument);
    uint32_t buffer = 0x52006000;
    uint32_t len = g_adc_clk * g_adc_long_sample_time;
    bool next = false;

    hal_diag_set_stop();
    if (g_adc_receive_data_cb != NULL) {
        g_adc_receive_data_cb(g_adc_chan_sel, (uint32_t *)(uintptr_t)buffer, len, &next);
        adc_port_set_diag_reg();
    }
    return;
}

static bool adc_creat_timer(void)
{
    g_adc_timer = osTimerNew((osTimerFunc_t)adc_port_sample_cb, osTimerPeriodic, NULL, NULL);
    if (g_adc_timer == NULL) {
        osal_printk("creat adc timer fail.\n\r");
        return false;
    }
    return true;
}

static uint32_t adc_start_period_sample(adc_channel_t ch, hal_adc_scan_config_t adc_config, hal_adc_callback_t callback)
{
    // 低功耗场景需要增加投票
#if ENABLE_LOW_POWER == YES
    uapi_pm_add_sleep_veto(PM_ID_ADC);
#endif
    if (g_adc_auto_sample_working) {
        osal_printk("Adc auto sample is working!\r\n");
        return ERRCODE_FAIL;
    }

    g_adc_chan_sel = ch;
    g_adc_clk = adc_port_transfer_freq(adc_config.freq);
    g_adc_long_sample_time = adc_config.long_sample_time;
    g_adc_receive_data_cb = callback;
    osal_printk("ch = %d, clk = %fKHz, long_sample_time = %d\n", g_adc_chan_sel, g_adc_clk, g_adc_long_sample_time);

    if (g_adc_clk * g_adc_long_sample_time > DIAG_MAX_LEN) {
        osal_printk("Wrong params! The max adc auto sample data len is 2048!");
        return ERRCODE_FAIL;
    }

    if (adc_creat_timer() != true) {
        return ERRCODE_FAIL;
    }
    // adc auto与cpu trace不能同时使用diag，需要做互斥处理
    cpu_trace_disable();
    adc_port_set_diag_reg();
    (void)osTimerStart(g_adc_timer, g_adc_long_sample_time + SAMPLE_OFFSET);
    g_adc_auto_sample_working = true;
    return ERRCODE_SUCC;
}

static uint32_t adc_stop_period_sample(adc_channel_t ch, bool en)
{
    unused(ch);
    unused(en);
    osTimerStop(g_adc_timer);
    osTimerDelete(g_adc_timer);
    hal_diag_set_stop();
    cpu_trace_enable_bcpu_repeat();
    g_adc_chan_sel = 0;
    g_adc_clk = 0.0;
    g_adc_receive_data_cb = NULL;
    g_adc_auto_sample_working = false;
#if ENABLE_LOW_POWER == YES
    uapi_pm_remove_sleep_veto(PM_ID_ADC);
#endif
    return ERRCODE_SUCC;
}
#endif

uintptr_t adc_porting_base_addr_get(void)
{
    return g_adc_base_addr;
}

void adc_port_set_div_num(uint8_t div)
{
    hal_clocks_config_bit(CMU_CLOCKS_M_DIV3_REG, 1, (REG16_POS)CMU_CLOCKS_AUX_ADC_DIV_EN);
    reg16_clrbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
    reg16_setbits(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV, CMU_CLOCKS_AUX_ADC_DIV_LEN, div);
    reg16_setbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
    reg16_clrbit(CMU_CLOCKS_M_DIV3_REG, CMU_CLOCKS_AUX_ADC_DIV_LOAD_EN);
}

static void adc_port_set_cali_coef_gain0(void)
{
    aux_adc_cali_gain_t adc_cali;

    /* Read the calibration value from the eFUSE. */
    uapi_efuse_read_buffer((uint8_t *)&adc_cali, OTP_LOW_BAND, OTP_LOW_BAND_LENGTH);

    /* Configure the calibration value to the ADC register. */
    regw_setbits(M_CTL_RB_BASE, 0xB40, AUX_ADC_CALI_GAIN0_POS, AUX_ADC_CALI_GAIN0_LEN, adc_cali.date.cali_gain0);
    regw_setbits(M_CTL_RB_BASE, 0xB40, AUX_ADC_CALI_GAIN1_POS, AUX_ADC_CALI_GAIN1_LEN, adc_cali.date.cali_gain1);
    regw_setbits(M_CTL_RB_BASE, 0xB44, AUX_ADC_CALI_OFFSET0_POS, AUX_ADC_CALI_OFFSET0_LEN, adc_cali.date.cali_offset0);
    regw_setbits(M_CTL_RB_BASE, 0xB44, AUX_ADC_CALI_OFFSET1_POS, AUX_ADC_CALI_OFFSET1_LEN, adc_cali.date.cali_offset1);
    regw_setbits(M_CTL_RB_BASE, 0xB44, AUX_ADC_CALI_OFFSET2_POS, AUX_ADC_CALI_OFFSET2_LEN, adc_cali.date.cali_offset2);
}

void adc_port_init_clock(adc_clock_t clock)
{
    uint16_t val;
    switch (clock) {
        case ADC_CLOCK_500KHZ:
            val = ADC_CLOCK_500KHZ_DIV_VALUE;
            break;
        case ADC_CLOCK_250KHZ:
            val = ADC_CLOCK_250KHZ_DIV_VALUE;
            break;
        case ADC_CLOCK_125KHZ:
            val = ADC_CLOCK_125KHZ_DIV_VALUE;
            break;
        case ADC_CLOCK_015KHZ:
            val = ADC_CLOCK_015KHZ_DIV_VALUE;
            break;
        default:
            val = 0;
            break;
    }
    val = (uint8_t)(val << HAL_ADC_M_DIV_3_OFFSET);
    adc_port_clock_enable(true);
    adc_port_set_div_num((uint8_t)val);
    adc_port_set_cali_coef_gain0();
}

void adc_port_get_channel_config(adc_channel_t channel, uint32_t *reg_val, hal_adc_buffer_t *ch_buffer)
{
    unused(channel);
    unused(reg_val);
    unused(ch_buffer);
}
void adc_port_set_scan_discard_and_average_num(adc_channel_t channel)
{
    unused(channel);
}

void adc_port_register_hal_funcs(void)
{
    hal_adc_funcs_t *g_port_adc_func = hal_adc_v151_funcs_get();
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN) && defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
    g_port_adc_func->ch_config = adc_start_period_sample,
    g_port_adc_func->ch_enable = adc_stop_period_sample,
#endif
    hal_adc_register_funcs(g_port_adc_func);
}

void adc_port_unregister_hal_funcs(void)
{
    hal_adc_unregister_funcs();
}

hal_adc_type_info_t *adc_port_get_cfg(void)
{
    return g_adc_cfg_info;
}

void adc_port_clock_enable(bool on)
{
    unused(on);
}

static void irq_adc_handler(void)
{
    hal_adc_irq_handler();
}

void adc_port_register_irq(void)
{
    osal_irq_request(ADC_IRQn, (osal_irq_handler)irq_adc_handler, NULL, NULL, NULL);
    osal_irq_enable(ADC_IRQn);
}

void adc_port_unregister_irq(void)
{
    osal_irq_disable(ADC_IRQn);
    osal_irq_free(ADC_IRQn, NULL);
}

void adc_port_power_on(bool on)
{
    // After power-on, the ADC cannot be powered off. As a result, the voltage of the ADC digital-analog multiplexing
    // pin is uncontrollable.
    if (on == false) {
        return;
    }
    pmu_ldo_power_on(PMU_LDO_AUXLDO0, (switch_type_t)on);
}

/* calibrate single with buffer channle */
void adc_port_calibration(void)
{
    static bool is_calibration = false;

    if (is_calibration) {
        return;
    }
    hal_adc_v151_channel_set(ADC_CHANNEL_NONE, true);
    is_calibration = true;
}

void adc_port_pull_disable(adc_channel_t channel)
{
    pin_t pin = PIN_NONE;
    switch (channel) {
        case ADC_CHANNEL_0:
            pin = PIN_NONE;
            break;
        case ADC_CHANNEL_1:
            pin = S_AGPIO_L27;
            break;
        case ADC_CHANNEL_2:
            pin = PIN_NONE;
            break;
        case ADC_CHANNEL_3:
            pin = PIN_NONE;
            break;
        case ADC_CHANNEL_4:
            pin = PIN_NONE;
            break;
        case ADC_CHANNEL_5:
            pin = PIN_NONE;
            break;
        case ADC_CHANNEL_6:
            pin = S_MGPIO5;
            break;
        case ADC_CHANNEL_7:
            pin = S_MGPIO38;
            break;
        default:
            break;
    }
    if (pin != PIN_NONE) {
        uapi_pin_set_pull(pin, PIN_PULL_NONE);
    }
    return;
}

static uint32_t g_irq;

void adc_irq_lock(uint8_t channel)
{
    unused(channel);
    g_irq = osal_irq_lock();
}

void adc_irq_unlock(uint8_t channel)
{
    unused(channel);
    osal_irq_restore(g_irq);
}


float adc_port_stick_transfer_voltage(uint16_t stick)
{
    return adc_trans_sytick_to_voltage_without_auto_scan(stick);
}