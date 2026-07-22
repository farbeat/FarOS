/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V151 HAL adc \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-16， Create file. \n
 */
#include "common_def.h"
#include "tcxo.h"
#include "hal_adc.h"
#include "hal_adc_v151.h"

/* provide a reference voltage as 1.7V, meanwhile sample register has 12 bits which means accuracy is 1/2^12 */
#define hal_adc_trans_voltage_to_sytick(x) ((uint16_t)((x) * 40960 / 17))
#define hal_adc_trans_voltage_to_sytick_diff(x) ((uint16_t)(((x) * 20480 / 17) + 2048))

#define HAL_ADC_SCAN_PTR_OFFSET 2
/* Calculation of the Register Addresses Corresponding to the Channel */
#define hal_adc_get_scan_ptr(channel)   ((uint32_t)(((uint32_t)(channel)) / HAL_ADC_SCAN_PTR_OFFSET))
/* Calculation of the Bits Corresponding to the Channel */
#define hal_adc_get_scan_pos(channel)   ((uint32_t)(((uint32_t)(channel)) % HAL_ADC_SCAN_PTR_OFFSET))

#define HAL_ADC_MANUAL_SAMPLE_DELAY_US 10ULL
#define HAL_ADC_SAMPLE_TIMEOUT_MS     500
#define HAL_ADC_TIMEOUT_CHECK_PERIOD  10000
#define HAL_ADC_TIMEOUT_CHECK_CAIL  10000

#define HAL_ADC_COUNT_VALUE_B_VALID_TRUE 1

#define HAL_ADC_CONTROL_REG_INIT_VALUE 0x0
#define HAL_ADC_CONTROL_REG_PD_VALUE 0x385

#define HAL_ADC_POWER_REG_BUFFER_POWER_ON_VAULE  0x10
#define HAL_ADC_POWER_REG_BUFFER_POWER_OFF_VAULE 0x00

#define HAL_ADC_CALI_REF_POWER_0    0
#define HAL_ADC_CALI_REF_POWER_1    3

#define HAL_ADC_CALI_SYNC           0x01

#define HAL_ADC_CLEAR_RESULT 0x01

#define HAL_ADC_MANUAL_MODE   0x0C
#define HAL_ADC_MANUAL_SAMPLE 0x08

#define HAL_ADC_THRESHOLD_SCAN_DATA_LEN 1
#define HAL_ADC_FIFO_FULL_SCAN_DATA_LEN 128

#define HAL_ADC_CALI_SYNC           0x01
#define HAL_ADC_CALI_STATUS_DONE    0x1
#define HAL_ADC_CALI_OFFSET_COMPLEMENT_VALUE 16384
#define HAL_ADC_CALI_COEF_GAIN_ERR_VALUE 0x1000
#define HAL_ADC_CALI_COEF_OFFSET_ERR_VALUE 0x8000

#define HAL_ADC_CALI_GOAL_1         0x800
#define HAL_ADC_CALI_GOAL_2         0x878
#define HAL_ADC_CALI_GOAL_DIFF_LEN  (HAL_ADC_CALI_GOAL_2 - HAL_ADC_CALI_GOAL_1)
/* gain = (Goal1 - Goal2) / (cali1 -c ali2) */
#define hal_adc_get_cali_gain(cali1, cali2)  ((float)HAL_ADC_CALI_GOAL_DIFF_LEN / (float)((cali2) - (cali1)))
/* offset = Goal1 - gain * cali1 */
#define hal_adc_get_cali_offset(cali1, gain) ((float)HAL_ADC_CALI_GOAL_1 - (float)(gain) * (float)(cali1))

/* 4096 : The calculated gain needs to be shifted leftwards by 12 bits and configured to the corresponding register. */
/* 8192 : Gain is a 13-bit binary decimal. */
#define hal_adc_get_cali_gain_source_code(cali1, cali2) ((uint16_t)(hal_adc_get_cali_gain(cali1, cali2) * 4096) % 8192)

/* 4 : The calculated offset needs to be shifted leftwards by 2 bits and configured to the corresponding register. */
#define hal_adc_get_cali_offset_complement(cali1, gain) ((int16_t)(hal_adc_get_cali_offset(cali1, gain) * 4) % 8192)

/* If the channel is triggered by a threshold,
   the formula for calculating the interrupt value storage address is as follows:
   Data_addr = Start_addr + (Ptr - 1) << 2; */
#define hal_adc_calc_threshold_scan_addr_offset(ptr) (((ptr) - 1) << 2)

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
static hal_adc_type_info_t *g_adc_cfg_info = NULL;
static hal_adc_callback_t g_adc_receive_data_callback[ADC_CHANNEL_MAX_NUM] = { NULL };
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */

static void hal_adc_calibration_config(adc_channel_t ch, uint16_t gain, int16_t offset)
{
    hal_adc_cali_t reg;
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    hal_adc_channel_type_t type = g_adc_cfg_info[ch].channel_type;
    switch (type) {
        case HAL_ADC_CHANNEL_TYPE_DIF_BUF:
            reg = AUX_ADC_CALI_COEF0;
            break;
        case HAL_ADC_CHANNEL_TYPE_SIN_BUF:
            reg = AUX_ADC_CALI_COEF1;
            break;
        case HAL_ADC_CHANNEL_TYPE_SIN_NOBUF:
            reg = AUX_ADC_CALI_COEF2;
            break;
        default:
            return;
    }
#else /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
    reg = 0;
    unused(ch);
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
    g_adc_regs->aux_adc_cali_coef[reg].gain = gain;
    g_adc_regs->aux_adc_cali_coef[reg].ofst = (uint32_t)offset;
}

errcode_t hal_adc_v151_channel_set(adc_channel_t ch, bool on)
{
    static hal_adc_channel_type_t g_adc_channle_type = HAL_ADC_CHANNEL_TYPE_NONE;

    if (ch == ADC_CHANNEL_NONE) {
        /* clear channel and buffer config */
        g_adc_regs->aux_adc_sel = HAL_ADC_CONTROL_REG_INIT_VALUE;
        hal_adc_reg_power_set_en((uint32_t)0);
        g_adc_channle_type = HAL_ADC_CHANNEL_TYPE_NONE;
        return ERRCODE_SUCC;
    }

    adc_port_pull_disable(ch);
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    if (g_adc_channle_type == HAL_ADC_CHANNEL_TYPE_NONE) {
        g_adc_channle_type = g_adc_cfg_info[ch].channel_type;
    } else if (g_adc_channle_type != g_adc_cfg_info[ch].channel_type) {
        return ERRCODE_ADC_INVALID_CH_TYPE;
    }

    hal_adc_buffer_t ch_buffer = HAL_ADC_BUFFER_MAX;
    uint32_t reg_val;
    adc_port_get_channel_config(ch, &reg_val, &ch_buffer);

    if (on) {
        reg_val |= g_adc_regs->aux_adc_sel;
    } else {
        g_adc_channle_type = HAL_ADC_CHANNEL_TYPE_NONE;
        reg_val = g_adc_regs->aux_adc_sel & (~reg_val);
    }
    /* select channel buffer */
    hal_adc_reg_power_set_en((uint32_t)ch_buffer);
    /* select channel */
    g_adc_regs->aux_adc_sel = reg_val;
#else /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
    unused(g_adc_channle_type);
    if (on) {
        hal_adc_reg_auto_cfg_set_ch((uint32_t)ch);
    } else {
        hal_adc_reg_auto_cfg_set_ch((uint32_t)0);
    }
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */

    /* updata channel infor */
    /* if enable calibtation, sample results = in * gain + offset, otherwizse,  sample results is raw data */
    g_adc_regs->aux_adc_cali_bypass = (uint32_t)0;
    g_adc_regs->aux_adc_cali_cfg_sync = (uint32_t)HAL_ADC_CALI_SYNC;

    return ERRCODE_SUCC;
}

static bool hal_adc_calibration_output_get(uint32_t copm_vref, uint16_t *data)
{
    uint64_t start_time = uapi_tcxo_get_ms();
    hal_adc_reg_cali_en_set((uint32_t)0);

    /* config calibration reference voltagee */
    hal_adc_reg_power_set_comp(copm_vref);

    /* enable calibration and wait calibration done */
    hal_adc_reg_cali_en_set((uint32_t)1);

    /* enable calibration and wait calibration done */
    while ((uint16_t)g_adc_regs->aux_adc_cali_status != HAL_ADC_CALI_STATUS_DONE) {
        if ((uapi_tcxo_get_ms() - start_time) > HAL_ADC_SAMPLE_TIMEOUT_MS) {
            *data = 0;
            return false;
        }
    }
    *data = (uint16_t)g_adc_regs->aux_adc_cali_out;
    return true;
}

void hal_adc_calibration_channel(adc_channel_t ch)
{
    uint16_t gain = 0;
    int16_t offset = 0;
    uint16_t vo_cali1, vo_cali2;
    (void)hal_adc_v151_channel_set(ADC_CHANNEL_NONE, true);
    (void)hal_adc_v151_channel_set(ch, true);
    /* adc calibration config and sync */
    g_adc_regs->aux_adc_cali_cfg = 0;

    /* enable calibration config */
    g_adc_regs->aux_adc_cali_cfg_sync = HAL_ADC_CALI_SYNC;

    /* first calibration */
    bool cail1_ret = hal_adc_calibration_output_get((uint32_t)HAL_ADC_CALI_REF_POWER_0, &vo_cali1);

    /* second calibration */
    bool cail2_ret = hal_adc_calibration_output_get((uint32_t)HAL_ADC_CALI_REF_POWER_1, &vo_cali2);

    /* disable calibration */
    hal_adc_reg_cali_en_set((uint32_t)0);

    if ((vo_cali1 != vo_cali2) && cail1_ret && cail2_ret) {
        gain = hal_adc_get_cali_gain_source_code(vo_cali1, vo_cali2);
        offset = hal_adc_get_cali_offset_complement(vo_cali1, hal_adc_get_cali_gain(vo_cali1, vo_cali2));
        if (offset < 0) { offset += HAL_ADC_CALI_OFFSET_COMPLEMENT_VALUE; }
    } else { /* Calibration error */
        gain = HAL_ADC_CALI_COEF_GAIN_ERR_VALUE;
        offset = (int16_t)HAL_ADC_CALI_COEF_OFFSET_ERR_VALUE;
    }

    hal_adc_calibration_config(ch, gain, offset);

    /* config calibration reference voltagee */
    hal_adc_reg_power_set_comp((uint32_t)0);
}

static errcode_t hal_adc_v151_init(void)
{
    if (hal_adc_v151_regs_init() != ERRCODE_SUCC) {
        return ERRCODE_ADC_REG_ADDR_INVALID;
    }

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    g_adc_cfg_info = adc_port_get_cfg();
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */

    return ERRCODE_SUCC;
}

static errcode_t hal_adc_v151_deinit(void)
{
    hal_adc_v151_regs_deinit();
    return ERRCODE_SUCC;
}

static void hal_adc_v151_power_en(afe_scan_mode_t afe_scan_mode, bool on)
{
    unused(afe_scan_mode);
    if (on) {
        g_adc_regs->aux_adc_reg = (uint32_t)HAL_ADC_POWER_REG_BUFFER_POWER_ON_VAULE;
        g_adc_regs->aux_adc_cfg = HAL_ADC_CONTROL_REG_INIT_VALUE;
        hal_adc_reg_ctrl_set_rstn();
        adc_port_calibration();
    } else {
        g_adc_regs->aux_adc_reg = (uint32_t)HAL_ADC_POWER_REG_BUFFER_POWER_OFF_VAULE;
        g_adc_regs->aux_adc_cfg = HAL_ADC_CONTROL_REG_PD_VALUE;
    }
}

#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
static void hal_adc_interrupt_enable(adc_channel_t ch)
{
    hal_adc_reg_scan_start_set(AUX_ADC_SCAN_INT_EN, ch, (uint32_t)1);
}

static void hal_adc_v151_auto_scan_enable(bool en)
{
    if (en) {
        hal_adc_reg_scan_en_set_en((uint32_t)1);
    } else {
        hal_adc_reg_scan_en_set_en((uint32_t)0);
    }
}

static bool hal_adc_timeout_check_timeout_periodic(uint64_t timeout, uint64_t start_time, uint32_t *check_times)
{
    (*check_times)++;
    if ((*check_times) > HAL_ADC_TIMEOUT_CHECK_PERIOD) {
        if ((timeout == 0) || ((uapi_tcxo_get_ms() - start_time) > timeout)) {
            return true;
        }
        (*check_times) = 0;
    }

    return false;
}

static errcode_t hal_adc_v151_auto_scan_ch_enable(adc_channel_t ch, bool en)
{
    uint32_t check_times = 0;
    uint64_t start_time = uapi_tcxo_get_ms();

    if (en) {
        hal_adc_reg_scan_en_set_ch(ch, (uint32_t)1);
    } else {
        /* when disable channel, wait for scan disable success */
        hal_adc_reg_scan_en_set_ch(ch, (uint32_t)0);
        while (hal_adc_reg_scan_start_get(AUX_ADC_SCAN_EN_STAT, ch) != 0) {
            if (hal_adc_timeout_check_timeout_periodic((uint64_t)HAL_ADC_SAMPLE_TIMEOUT_MS, start_time, &check_times)) {
                return ERRCODE_ADC_TIMEOUT;
            }
        }
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_adc_v151_auto_scan_ch_config(adc_channel_t ch, hal_adc_scan_config_t config,
                                                  hal_adc_callback_t callback)
{
    uint16_t val_l = 0;
    uint16_t val_h = 0;

    if (hal_adc_v151_channel_set(ch, true) != ERRCODE_SUCC) {
        return ERRCODE_ADC_INVALID_CH_TYPE;
    }

    g_adc_receive_data_callback[ch] = callback;

    /* set scan freq */
    hal_adc_reg_freq_set_freq((uint32_t)config.freq);
    /* set scan interrupt trigger type */
    hal_adc_reg_scan_mode_set(AUX_ADC_SCAN_MODE1, ch, (uint32_t)config.type);
    /* set scan discard num and average num */
    adc_port_set_scan_discard_and_average_num(ch);

    if (config.type ==  HAL_ADC_SCAN_TYPE_THRESHOLD) {
        if (g_adc_cfg_info[ch].channel_type == HAL_ADC_CHANNEL_TYPE_DIF_BUF) {
            val_l = hal_adc_trans_voltage_to_sytick_diff(config.threshold_l);
            val_h = hal_adc_trans_voltage_to_sytick_diff(config.threshold_h);
        } else {
            val_l = hal_adc_trans_voltage_to_sytick(config.threshold_l);
            val_h = hal_adc_trans_voltage_to_sytick(config.threshold_h);
        }
        /* set interrupt threshold */
        uint8_t reg_tmp = (uint8_t)ch * 2; /* 2 : High and low of each channel */
        g_adc_regs->aux_adc_scan_th[reg_tmp] = (uint32_t)val_l;
        reg_tmp++;
        g_adc_regs->aux_adc_scan_th[reg_tmp] = (uint32_t)val_h;
    }
    hal_adc_interrupt_enable(ch);

    hal_adc_v151_auto_scan_enable(true);
    hal_adc_v151_auto_scan_ch_enable(ch, true);

    return ERRCODE_SUCC;
}

static bool hal_adc_v151_auto_scan_is_enabled(void)
{
    return (bool)hal_adc_reg_scan_en_get_en();
}

static bool hal_adc_is_interrupt_asserted(adc_channel_t ch)
{
    return (bool)hal_adc_reg_scan_start_get(AUX_ADC_SCAN_STAT, ch);
}

static void hal_adc_clear_interrupt(adc_channel_t ch)
{
    hal_adc_reg_scan_start_set(AUX_ADC_SCAN_INT_CLR, ch, (uint32_t)1);
}

static hal_adc_scan_type_t hal_adc_get_scan_type(adc_channel_t ch)
{
    return (hal_adc_scan_type_t)(hal_adc_reg_scan_mode_get(AUX_ADC_SCAN_MODE1, ch));
}

static void hal_adc_invoke_irq_callback(adc_channel_t ch)
{
    uint32_t buffer, len;
    bool next = false;

    if (hal_adc_get_scan_type(ch) == HAL_ADC_SCAN_TYPE_THRESHOLD) {
        uint32_t addr = (uint32_t)hal_adc_get_scan_ptr(ch);
        bool pos = (bool)hal_adc_get_scan_pos(ch);
        uint32_t ptr = hal_adc_reg_scan_ptr_get(addr, pos);

        buffer = g_adc_cfg_info[ch].scan_fifo_start_addr +  hal_adc_calc_threshold_scan_addr_offset(ptr);
        len = HAL_ADC_THRESHOLD_SCAN_DATA_LEN;
    } else {
        buffer = g_adc_cfg_info[ch].scan_fifo_start_addr;
        len = HAL_ADC_FIFO_FULL_SCAN_DATA_LEN;
    }

    if (hal_adc_v151_auto_scan_ch_enable(ch, false) != ERRCODE_SUCC) {
        len = 0;
    }

    if (g_adc_receive_data_callback[ch] != NULL) {
        g_adc_receive_data_callback[ch](ch, (uint32_t *)(uintptr_t)buffer, len, &next);
    }

    hal_adc_v151_auto_scan_ch_enable(ch, next);
}

void hal_adc_irq_handler(void)
{
    for (uint8_t ch = (uint8_t)0; ch < ADC_CHANNEL_MAX_NUM; ch++) {
        if (hal_adc_is_interrupt_asserted((adc_channel_t)ch)) {
            hal_adc_clear_interrupt((adc_channel_t)ch);
            hal_adc_invoke_irq_callback((adc_channel_t)ch);
        }
    }
}
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */

static int32_t hal_adc_v151_manual_sample(adc_channel_t channel)
{
    unused(channel);
    hal_adc_reg_auto_cfg_set_man_sel((uint32_t)1);
    g_adc_regs->aux_adc_stick_clr = (uint32_t)HAL_ADC_CLEAR_RESULT;
    hal_adc_reg_auto_cfg_set_man((uint32_t)1);
    uapi_tcxo_delay_us(HAL_ADC_MANUAL_SAMPLE_DELAY_US);
    hal_adc_reg_auto_cfg_set_man((uint32_t)0);
    uapi_tcxo_delay_us(HAL_ADC_MANUAL_SAMPLE_DELAY_US);
    uint32_t current_tcxo = (uint32_t)uapi_tcxo_get_ms();
    while (!(bool)hal_adc_reg_spl_get_flag()) {
        if (uapi_tcxo_get_ms() - current_tcxo > HAL_ADC_SAMPLE_TIMEOUT_MS) {
            return 0;
        }
    }

    uapi_tcxo_delay_us(HAL_ADC_MANUAL_SAMPLE_DELAY_US);

    return (int32_t)hal_adc_reg_spl_get_stick();
}

static hal_adc_funcs_t g_hal_adc_v151_funcs = {
    .init = hal_adc_v151_init,
    .deinit = hal_adc_v151_deinit,
    .power_en = hal_adc_v151_power_en,
    .ch_set = hal_adc_v151_channel_set,
#if defined(CONFIG_ADC_SUPPORT_AUTO_SCAN)
    .ch_config = hal_adc_v151_auto_scan_ch_config,
    .ch_enable = hal_adc_v151_auto_scan_ch_enable,
    .enable = hal_adc_v151_auto_scan_enable,
    .isenable = hal_adc_v151_auto_scan_is_enabled,
#endif /* CONFIG_ADC_SUPPORT_AUTO_SCAN */
    .manual = hal_adc_v151_manual_sample
};

hal_adc_funcs_t *hal_adc_v151_funcs_get(void)
{
    return &g_hal_adc_v151_funcs;
}
