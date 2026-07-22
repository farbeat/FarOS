/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm clocks manage header
 */

#ifndef SRC_LIB_PM_CLOCKS_H
#define SRC_LIB_PM_CLOCKS_H

#include "chip_io.h"
#include "pm_bsp.h"

typedef struct {
    clocks_src_t dsp_src;
    uint8_t dsp_div;
    uint64_t dsp_freq_val;
    clocks_src_t codec_src;
    uint8_t codec_div;
    uint64_t codec_freq_val;
}pm_hifi_freq_t;

typedef enum {
    HIFI_LPM_CLK_LOW_LEVEL,  // dsp:64MHz, codec:0
    HIFI_LPM_CLK_LEVEL1,     // dsp:147Mhz, codec:24.576Mhz
    HIFI_LPM_CLK_LEVEL2,     // dsp:176Mhz, codec:24.576Mhz
    HIFI_LPM_CLK_HIGH_LEVEL, // dsp:294Mhz, codec:24.576Mhz
    HIFI_LPM_CLK_MAX,
} pm_hifi_mode_t;

typedef void (*hifi_inform_handler)(void);
typedef void (* clock_config_pre_func)(void);

void clock_config_pre_handler_register(clock_config_pre_func func, clocks_id_t module);

void uapi_clocks_codec_config(clocks_src_t clk_src, uint8_t clk_div);

void uapi_clocks_hifi_config(clocks_src_t clk_src, uint8_t clk_div);

void uapi_get_current_pm_mode_info(void);

void pm_set_hifi_mode(pm_hifi_mode_t mode);

pm_hifi_freq_t uapi_pm_get_hifi_freq(pm_hifi_mode_t mode);

pm_hifi_mode_t uapi_pm_get_hifi_mode(void);

void uapi_register_hifi_inform_callback(hifi_inform_handler callback);

void uapi_set_hifi_mode(pm_hifi_mode_t mode);

#endif