/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description: PM config header file
 */

#ifndef SRC_LIB_PM_CONFIG_H
#define SRC_LIB_PM_CONFIG_H

#include "chip_io.h"
#include "pm_definition.h"

typedef enum {
    PM_PWR_CLDO1,
    PM_PWR_CLDO2,
    PM_PWR_CLDO3,
    PM_PWR_XLDO,
    PM_PWR_CMU_LDO,
    PM_PWR_INTLDO1,
    PM_PWR_INTLDO2,
    PM_PWR_AUXLDO0,
    PM_PWR_AUXLDO1,
    PM_PWR_AUXLDO2,
    PM_PWR_AUXLDO3,
    PM_PWR_IOLDO,
    PM_PWR_VDD0P8,
    PM_PWR_VDD1P9,
    PM_PWR_VDD1P0,
    PM_PWR_VDD1P3,
    PM_PWR_MAX,
} pm_pwr_t;

typedef struct {
    uint16_t res1           : 3;
    uint16_t aefuse         : 1;
    uint16_t res2           : 4;
    uint16_t ulp_aon_bus    : 1;
    uint16_t glb_32k        : 1;
    uint16_t sdiotm         : 1;
    uint16_t mcu_core       : 1;  // mcu glb clk
    uint16_t bt_xo_rc       : 1;  // bt glb clk
    uint16_t hpm            : 1;
    uint16_t ulp_rtc        : 1;
    uint16_t mcpu_clk_test  : 1;
} m_glb_clk_en_t;

typedef struct {
    uint16_t sec            : 1;
    uint16_t mdiag          : 1;
    uint16_t res1           : 5;
    uint16_t pwm            : 1;
    uint16_t res2           : 5;
    uint16_t x2h_mem_bus    : 1;
    uint16_t res3           : 2;
} m_sub_clk0_en_t;

typedef struct {
    uint16_t mtimer         : 1;
    uint16_t spi0_m         : 1;
    uint16_t spi1_ms        : 1;
    uint16_t spi2_ms        : 1;
    uint16_t uart_h0        : 1;
    uint16_t uart_h1        : 1;
    uint16_t i2c0           : 1;
    uint16_t i2c1           : 1;
    uint16_t i2c2           : 1;
    uint16_t i2c3           : 1;
    uint16_t qspi2          : 1;
    uint16_t res1           : 2;
    uint16_t xip_bus        : 1;
    uint16_t com_bus_dsp    : 1;
    uint16_t bt_tgtws       : 1;
} m_sub_clk1_en_t;

typedef struct {
    uint16_t res1           : 4;
    uint16_t pulse_capture  : 1;
    uint16_t m_dap          : 1;
    uint16_t res2           : 1;
    uint16_t rc             : 1;
    uint16_t xo             : 1;
    uint16_t res3           : 7;
} m_sub_clk2_en_t;

typedef struct {
    uint16_t res1           : 1;
    uint16_t bcpu_hclken    : 1;
    uint16_t bcpu_fclken    : 1;
    uint16_t bcpu_stclken   : 1;
    uint16_t rf_test        : 1;
    uint16_t bmem           : 1;
    uint16_t bcbb           : 1;
    uint16_t buart          : 1;
    uint16_t b_trng         : 1;
    uint16_t bt_diag        : 1;
    uint16_t bt_cpu_trace   : 1;
    uint16_t btimer         : 1;
    uint16_t bwdt           : 1;
    uint16_t btsensor       : 1;
    uint16_t res2           : 2;
} b_sub_clk_en_t;

typedef struct {
    uint16_t res1           : 4;
    uint16_t qspi0_div      : 1;
    uint16_t res2           : 3;
    uint16_t qspi0          : 1;
    uint16_t res3           : 2;
    uint16_t opi            : 1;
    uint16_t qspi0_xip      : 1;
    uint16_t res4           : 1;
    uint16_t xip_qspi_cache : 1;
    uint16_t opi_xip        : 1;
} mem_glb_clk0_en_t;

typedef struct {
    uint16_t xip_cache      : 1;
    uint16_t xip_sub_diag   : 1;
    uint16_t pinmux_ctl     : 1;
    uint16_t res1           : 13;
} mem_glb_clk1_en_t;

typedef struct {
    uint16_t clk_en         : 1;
    uint16_t clk_sel        : 3;  // clock src select
    uint16_t div_num        : 7;  // divide number
    uint16_t res            : 5;
} pm_clk_cfg_t;

typedef struct {
    uint8_t xo_en           : 1;  // com xo 32M
    uint8_t xo_2x_en        : 1;  // com dll2 64M
    uint8_t clk_rc_en       : 1;  // com rc 49.152M
    uint8_t pll_en          : 1;  // com pll 884.7M
    uint8_t b32m_en         : 1;  // bsoc 32M
    uint8_t b32m_2x_en      : 1;  // bsoc 64M
} pm_clk_en_t;

typedef struct {
    uint8_t bcore_en        : 1;
    uint8_t audio_en        : 1;
    uint8_t display_en      : 1;
    uint8_t sdio_en         : 1;
    uint8_t res             : 4;
} pm_sub_en_t;

typedef struct {
    uint8_t pwr_en          : 1;
    uint8_t pwr_sw_en       : 1;
    uint8_t pwr_vset        : 6;
} pm_pwr_cfg_t;

typedef struct {
    pm_pwr_cfg_t pwr_cfg;  /* 电源开关及电压状态 */
    pm_sub_en_t  sub_en;   /* 子系统电源开关 */
    pm_clk_en_t  clk_en;   /* 时钟源开关 */
    pm_clk_cfg_t clk_cfg;  /* 子系统时钟配置 */
} pm_mode_config_t;

bool pm_config_vset_first(pm_mode_t mode);

void pm_config_pwr_cfg(pm_mode_t mode);

void pm_config_clk_cfg(pm_mode_t mode);

void pm_config_clk_en(pm_mode_t mode);

void pm_config_pwr_en(pm_mode_t mode);
#endif