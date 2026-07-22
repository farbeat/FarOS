/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test adc source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "adc.h"
#if defined(CONFIG_ADC_SUPPORT_AFE)
#include "hal_adc_v152.h"
#include "hal_adc_v152_regs_op.h"
#else
#include "hal_adc_v151.h"
#include "hal_adc_v151_regs_op.h"
#endif
#include "adc_porting.h"

#include "pinctrl.h"

#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"

#include "soc_osal.h"
#include "tcxo.h"
#include "irmalloc.h"
#include "errcode.h"
#include "common_def.h"
#include "test_suite_log.h"
#include "test_adc.h"

#if defined(CONFIG_ADC_SUPPORT_AFE)
#define ADC_AUTO_SAMPLE_TEST_TIMES      1000
#define HADC_DEFAULT_CALI               \
{                                       \
    .hadc_os_code = 0x68,               \
    .hadc_wt_b22  = 0x75fdd,            \
    .hadc_wt_b21  = 0x3ef67,            \
    .hadc_wt_b20  = 0x1f7ae,            \
    .hadc_wt_b19  = 0xfc3d,             \
    .hadc_wt_b18  = 0x7e22,             \
    .hadc_wt_b17  = 0x7dc9,             \
    .hadc_wt_b16  = 0x3f01,             \
    .hadc_wt_b15  = 0x1f80,             \
    .hadc_wt_b14  = 0xfc5,              \
    .hadc_wt_b13  = 0x800,              \
    .hadc_wt_b12  = 0x800,              \
    .hadc_wt_b11  = 0x400,              \
    .hadc_wt_b10  = 0x200,              \
    .hadc_wt_b9   = 0x100,              \
    .dac_code1    = 0x200,              \
    .dac_code2    = 0x1d5,              \
}

afe_config_t g_afe_config = HADC_DEFAULT_CALI;
static bool g_afe_open_flag = false;

static int test_gadc_open_test(int argc, char *argv[])
{
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    adc_v152_gadc_channel_sel_t gadc_channel = (adc_v152_gadc_channel_sel_t)strtol(argv[0], NULL, 0);
    if (gadc_channel > SINGLE_END_AINN4) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    errcode_t ret = uapi_adc_init(ADC_CLOCK_NONE);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    uapi_adc_open_channel(gadc_channel);
    uapi_adc_power_en(AFE_GADC_MODE, true);
    adc_calibration(AFE_GADC_MODE, true, false, false);
    hadc_gain_set(PGA_GAIN_EIGHT, IA_GAIN_SIXTEEN);
    uapi_adc_manual_sample((adc_channel_t)AFE_GADC_MODE);
    g_afe_open_flag = true;
    return TEST_SUITE_OK;
}

static int test_hadc_open_test(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    bool cali_en = (bool)strtol(argv[0], NULL, 0);
    errcode_t ret = uapi_adc_init(ADC_CLOCK_NONE);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }
    uapi_adc_power_en(AFE_HADC_MODE, true);
    if (cali_en) {
        adc_calibration(AFE_HADC_MODE, true, true, true);
        g_afe_config.hadc_os_code = reg32(0x57036244);  // hadc_os_code
        g_afe_config.hadc_wt_b22  = reg32(0x570361B0);  // hadc_wt_b22
        g_afe_config.hadc_wt_b21  = reg32(0x570361B4);  // hadc_wt_b21
        g_afe_config.hadc_wt_b20  = reg32(0x570361B8);  // hadc_wt_b20
        g_afe_config.hadc_wt_b19  = reg32(0x570361BC);  // hadc_wt_b19
        g_afe_config.hadc_wt_b18  = reg32(0x570361C0);  // hadc_wt_b18
        g_afe_config.hadc_wt_b17  = reg32(0x570361C4);  // hadc_wt_b17
        g_afe_config.hadc_wt_b16  = reg32(0x570361C8);  // hadc_wt_b16
        g_afe_config.hadc_wt_b15  = reg32(0x570361CC);  // hadc_wt_b15
        g_afe_config.hadc_wt_b14  = reg32(0x570361D0);  // hadc_wt_b14
        g_afe_config.hadc_wt_b13  = reg32(0x570361D4);  // hadc_wt_b13
        g_afe_config.hadc_wt_b12  = reg32(0x570361D8);  // hadc_wt_b12
        g_afe_config.hadc_wt_b11  = reg32(0x570361DC);  // hadc_wt_b11
        g_afe_config.hadc_wt_b10  = reg32(0x570361E0);  // hadc_wt_b10
        g_afe_config.hadc_wt_b9   = reg32(0x570361E4);  // hadc_wt_b9
        g_afe_config.dac_code1    = reg32(0x570363D4);  // dac_code1
        g_afe_config.dac_code2    = reg32(0x570363D8);  // dac_code2
    } else {
        afe_config_t afe_config = g_afe_config;
        adc_set_cali_code(AFE_HADC_MODE, &afe_config);
    }
    uapi_adc_manual_sample(HADC_CHANNEL_0);
    g_afe_open_flag = true;
    return TEST_SUITE_OK;
}

static int test_adc_auto_sample_test(int argc, char *argv[])
{
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (!g_afe_open_flag) {
        return TEST_SUITE_TEST_FAILED;
    }
    adc_v152_gadc_channel_sel_t channel = (adc_v152_gadc_channel_sel_t)strtol(argv[0], NULL, 0);
    uapi_adc_open_channel(channel);
    int adc_value = 0;
    adc_channel_t ch = channel <= SINGLE_END_AINN4 ? (adc_channel_t)AFE_GADC_MODE : HADC_CHANNEL_0;
    uint32_t irq_sts = osal_irq_lock();
    for (int i = 0; i < ADC_AUTO_SAMPLE_TEST_TIMES; i++) {
        adc_value =  uapi_adc_auto_sample(ch);
        test_suite_log_stringf("adc: 0x%x = %d\n", adc_value, adc_value);
    }
    UNUSED(adc_value);
    osal_irq_restore(irq_sts);
    return TEST_SUITE_OK;
}

static int test_hadc_power_on(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uapi_adc_power_en(AFE_HADC_MODE, true);
    adc_set_cali_code(AFE_HADC_MODE, &g_afe_config);
    return TEST_SUITE_OK;
}

static int test_hadc_power_off(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uapi_adc_power_en(AFE_HADC_MODE, false);
    return TEST_SUITE_OK;
}

#else
#define HAL_ADC_SAMPLE_NUMBER_CONFIG_0 10  /* Channel with buffer manual sample average number. */
#define HAL_ADC_SAMPLE_NUMBER_CONFIG_1 10  /* Channel without buffer manual sample average number. */
#define HAL_ADC_DISCARD_NUMBER_CONFIG_0 8  /* Channel with buffer manual sample discard number. */
#define HAL_ADC_DISCARD_NUMBER_CONFIG_1 8  /* Channel without buffer manual sample discard number. */

#define adc_trans_sytick_to_voltage(x) ((float)(((float)(x) * 17) / 40960))
#define adc_trans_sytick_to_voltage_diff(x) ((float)(((float)((x) - 2048) * 17) / 20480))
#define adc_trans_sytick_to_voltage_without_auto_scan(x) ((float)(((float)(x) * 18) / 40960))

static uint32_t g_adc_sample_times = 0;
static uint32_t g_adc_sample_discard = 0;
static uint32_t g_adc_sample_stick = 0;

#define TESTADC_DISACRD_DELAY_MS      3
#define MINIMUM_VOLTAGE_RANGE         2
#define MAXIMUM_VOLTAGE_RANGE         17

static uint32_t adc_get_sample_times(void)
{
    return g_adc_sample_times;
}

static uint32_t adc_get_sample_discard(void)
{
    return g_adc_sample_discard;
}

static void adc_set_sample_stick(uint32_t stick)
{
    g_adc_sample_stick = stick;
}

static uint32_t adc_get_sample_stick(void)
{
    return g_adc_sample_stick;
}

static float adc_stick_transfer_voltage(uint16_t stick, adc_channel_t channel)
{
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    hal_adc_type_info_t *adc_cfg_info = NULL;
    adc_cfg_info = adc_port_get_cfg();
    /* diff channel: (stick - 2048)/ 2048 * 1.8 ; */
    if (adc_cfg_info != NULL && adc_cfg_info[channel].channel_type == HAL_ADC_CHANNEL_TYPE_DIF_BUF) {
        return adc_trans_sytick_to_voltage_diff(stick);
    } else {
        return adc_trans_sytick_to_voltage(stick);
    }
#else
    UNUSED(channel);
    return adc_trans_sytick_to_voltage_without_auto_scan(stick);
#endif
}

static bool test_adc_get_sample_result(float *voltage, uint8_t channel)
{
    uint32_t sample_number = 0;
    uint32_t discard_number = 0;
    uint32_t stick = 0;
    uint32_t sample_zero_count = 0;

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    if (uapi_adc_auto_scan_is_enabled()) { return false; }
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */

    if (channel >= ADC_CHANNEL_4) {
        sample_number = HAL_ADC_SAMPLE_NUMBER_CONFIG_1;
        discard_number = HAL_ADC_DISCARD_NUMBER_CONFIG_1;
    } else {
        sample_number = HAL_ADC_SAMPLE_NUMBER_CONFIG_0;
        discard_number = HAL_ADC_DISCARD_NUMBER_CONFIG_0;
    }

    if (g_adc_sample_times != 0) {
        sample_number = adc_get_sample_times();
    }

    if (g_adc_sample_discard != 0) {
        discard_number = adc_get_sample_discard();
    }

    /* after channel is enabled, sample result is error in previous samplings, need to be discarded */
    for (uint32_t i = 0; i < discard_number; i++) {
        uapi_adc_manual_sample(ADC_CHANNEL_NONE);
        uapi_tcxo_delay_ms((uint64_t)TESTADC_DISACRD_DELAY_MS);
    }

    for (uint32_t i = 0; i < sample_number; i++) {
        uint32_t irq_sts = osal_irq_lock();
        uint32_t sample_result = (uint32_t)uapi_adc_manual_sample(ADC_CHANNEL_NONE);
        osal_printk("sample_result = %d.\r\n", sample_result);
        if (sample_result == 0) { sample_zero_count++; }
        stick += sample_result;
        osal_irq_restore(irq_sts);
    }
    sample_number -= sample_zero_count;
    if (sample_number > 0) {
        stick = (stick / sample_number);
    } else {
        *voltage = 0;
        return false;
    }

    adc_set_sample_stick(stick);
    *voltage = adc_stick_transfer_voltage((uint16_t)stick, channel);

    UNUSED(discard_number);
    return true;
}

/**
 * @brief  Get voltage by the channel. Available voltage rang is 0V ~ 1.7V.
 * @param  voltage The sample result of voltage.
 * @param  channel The adc channel.
 * @return true when select channel success, false when seclect channel failed.
 */
static bool test_adc_get_voltage_by_channel(float *voltage, uint8_t channel)
{
    bool ret = false;

    uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, true);
    ret = uapi_adc_open_channel(channel);
    if (ret == ERRCODE_SUCC) {
        ret = test_adc_get_sample_result(voltage, channel);
        if (ret) {
            uapi_adc_close_channel(channel);
            uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, false);
            return true;
        }
    } else {
        osal_printk("uapi_adc_open_channel failed : %d\r\n", ret);
    }

    *voltage = 0;
    uapi_adc_close_channel(channel);
    uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, false);
    return false;
}

static int test_adc_manual_test(int argc, char *argv[])
{
    adc_channel_t channel;
    float voltage = 0;
    uint64_t time1, time2;
    pin_t pin;
    pin_mode_t mode;

    /* Get the parameters */
    if (argc != 5) { /* 5: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t param_index = 0;
    channel = (adc_channel_t)strtol(argv[param_index++], NULL, 0);
    g_adc_sample_times = (uint32_t)strtol(argv[param_index++], NULL, 0);
    g_adc_sample_discard = (uint32_t)strtol(argv[param_index++], NULL, 0);
    pin = (pin_t)strtol(argv[param_index++], NULL, 0);
    mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
    osal_printk("channel: %d, sample times: %d, discard: %d.\r\n", channel, g_adc_sample_times, g_adc_sample_discard);

    uapi_pin_set_mode(pin, mode);
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    uapi_adc_auto_scan_disable();
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
    osal_printk("PINMUX SET DONE\r\n");

    time1 = uapi_tcxo_get_us();

    if (!test_adc_get_voltage_by_channel(&voltage, channel)) {
        osal_printk("adc get sample result failed.\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    time2 = uapi_tcxo_get_us();

    osal_printk("tick: %d, Sample time: %d. \r\n", adc_get_sample_stick(),
        (uint64_t)((time2 - time1) / (g_adc_sample_times + g_adc_sample_discard)));
    osal_printk("voltage: %.4f.\r\n", voltage);

    UNUSED(argc);
    UNUSED(argv);

    return TEST_SUITE_OK;
}

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
static bool g_adc_scan_continue = false;
static void test_adc_callback(uint8_t ch, uint32_t *buffer, uint32_t length, bool *next)
{
    osal_printk("[IRQ]channel: %d, buffer:0x%x, len: %d. \r\n", ch, buffer, length);
    uint64_t data = 0;
#if defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
    osal_printk("sample data head: %d %d %d %d\r\n", buffer[0], buffer[TEST_PARAM_ARGC_1],
        buffer[TEST_PARAM_ARGC_2], buffer[TEST_PARAM_ARGC_3]);
    osal_printk("sample data tail: %d %d %d %d\r\n", buffer[length - TEST_PARAM_ARGC_4],
        buffer[length - TEST_PARAM_ARGC_3], buffer[length - TEST_PARAM_ARGC_2], buffer[length - TEST_PARAM_ARGC_1]);
#else
    for (uint32_t i = 0; i < length; i++) {
        osal_printk("[%d]data: %d\r\n", i, buffer[i]);
        data += buffer[i];
    }

    if (length != 0) {
        data = data / length;
    }
    osal_printk("average data: %d\r\n", (uint32_t)data);
#endif
    *next = g_adc_scan_continue;
    UNUSED(ch);
    UNUSED(buffer);
    UNUSED(length);
    UNUSED(next);
}

static int test_adc_stop_auto_scan(int argc, char *argv[])
{
    adc_channel_t channel;
    if (argc != 1) { /* 1: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    channel = (adc_channel_t)strtol(argv[0], NULL, 0);
    if (uapi_adc_auto_scan_ch_disable((uint8_t)channel) == ERRCODE_SUCC) {
#if defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
        uapi_tcxo_delay_ms(2000); /* 2000ms */
#endif
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }
}

static int test_adc_start_auto_scan(int argc, char *argv[])
{
    pin_t pin;
    pin_mode_t mode;
#if defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
    if (argc != 7) { /* 7: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uapi_tcxo_delay_ms(2000); /* 2000ms */
#else
    if (argc != 6) { /* 6: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
#endif
    adc_channel_t channel;
    channel = (adc_channel_t)strtol(argv[0], NULL, 0);
    g_adc_scan_continue = (bool)strtol(argv[TEST_PARAM_ARGC_1], NULL, 0);
    pin = (pin_t)strtol(argv[TEST_PARAM_ARGC_2], NULL, 0);
    mode = (pin_mode_t)strtol(argv[TEST_PARAM_ARGC_3], NULL, 0);
    adc_scan_config_t config = {
        .type = (uint8_t)strtol(argv[TEST_PARAM_ARGC_4], NULL, 0),
        .threshold_l = 0.5, /* 0.5: Threshold scan volatage(v) lower limit. */
        .threshold_h = 1.5, /* 1.5: Threshold scan volatage(v) upper limit. */
        .freq = (uint8_t)strtol(argv[TEST_PARAM_ARGC_5], NULL, 0),
#if defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
        .long_sample_time = (uint32_t)strtol(argv[TEST_PARAM_ARGC_6], NULL, 0),
#endif
    };
    osal_printk("chan: %d, freq: %d\r\n", channel, config.freq);
#if defined(CONFIG_ADC_SUPPORT_LONG_SAMPLE)
    osal_printk("long_sample_time = %d\n", config.long_sample_time);
#endif

    uapi_pin_set_mode(pin, mode);
    uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, true);
    if (uapi_adc_auto_scan_ch_enable((uint8_t)channel, config, test_adc_callback) == ERRCODE_SUCC) {
        return TEST_SUITE_OK;
    } else {
        return TEST_SUITE_TEST_FAILED;
    }
}
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
#endif /* CONFIG_ADC_SUPPORT_AFE */

void add_adc_test_case(void)
{
    uapi_tcxo_init();
    uapi_pin_init();
#if defined(CONFIG_ADC_SUPPORT_AFE)
    uapi_test_suite_add_function("gadc_open_test", "GAFE init & enable test <Sample end: 0~7>", test_gadc_open_test);
    uapi_test_suite_add_function("hadc_open_test", "HAFE init & enable test <Cali enable>", test_hadc_open_test);
    uapi_test_suite_add_function("auto_sample_test", "AFE auto sample test Params: <Sample end: GAFE:0~7 HAFE:8>",
                                 test_adc_auto_sample_test);
    uapi_test_suite_add_function("hadc_power_off", "HAFE low power test, power off hafe", test_hadc_power_off);
    uapi_test_suite_add_function("hadc_power_on", "HAFE low power test, power on hafe ", test_hadc_power_on);
#else
    uapi_adc_init(ADC_CLOCK_500KHZ);
    uapi_adc_power_en(AFE_SCAN_MODE_MAX_NUM, true);
    uapi_test_suite_add_function("adc_manual_test", "Params: <Channel>, <sample_times>, <discard_times>,"
        "<pin number>, <pin mode>", test_adc_manual_test);
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    uapi_test_suite_add_function("adc_start_auto_scan", "Params: <Channel>, <scan mode:0|1 >, <freq>, <bool: continue>,"
        "<pin number>, <pin mode>", test_adc_start_auto_scan);
    uapi_test_suite_add_function("adc_stop_auto_scan", "Params: <Channel>", test_adc_stop_auto_scan);
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
#endif /* CONFIG_ADC_SUPPORT_AFE */
}