/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: PM definitions header file
 */

#ifndef LIB_PM_DEFINITION_H
#define LIB_PM_DEFINITION_H

#include "chip_io.h"

/**
 * @defgroup middleware_utils_pm_api PM
 * @ingroup  middleware_utils
 * @{
 */

/**
 * @if Eng
 * @brief  PM module Id.
 * @else
 * @brief  PM 模块 ID
 * @endif
 */
typedef enum {
    PM_ID_BSP,
    PM_ID_SYS,
    PM_ID_BTH,
    PM_ID_BTC,
    PM_ID_AUDIO,
    PM_ID_VIDEO,
    PM_ID_DMA,
    PM_ID_DPU,
    PM_ID_VAU,
    PM_ID_BUTTON,
    PM_ID_DEBUG,
    PM_ID_ADC,
    PM_ID_DFX,
    PM_ID_SCREEN,
    PM_ID_STORAGE,
    PM_ID_TOUCH,
    PM_ID_JPEG,
    PM_ID_JS,
    PM_ID_MSGCENTER,
    PM_ID_SPI_DMA,
    PM_ID_I2C_DMA,
    PM_ID_UART_DMA,
    PM_ID_MAX,
} pm_id_t;

/**
 * @if Eng
 * @brief  Key running status of the system.
 * @else
 * @brief  系统关键运行状态
 * @endif
 */
typedef enum {
    PM_STATE_LIGHT_SLEEP,           /*!< @if Eng Light Sleep state
                                         @else   浅睡状态 @endif */
    PM_STATE_DEEP_SLEEP,            /*!< @if Eng Deep sleep state
                                         @else   深睡状态 @endif */
    PM_STATE_WKUP_FROM_LIGHT_SLEEP, /*!< @if Eng Wakeup from LightSleep
                                         @else   浅睡唤醒 @endif */
    PM_STATE_WKUP_FROM_DEEP_SLEEP,  /*!< @if Eng Wakeup from DeepSleep
                                         @else   深睡唤醒 @endif */
    PM_STATE_POWER_ON,              /*!< @if Eng Power ON state
                                         @else   上电模式 @endif */
    PM_STATE_POWER_OFF,             /*!< @if Eng Power off state
                                         @else   下电模式 @endif */
    PM_STATE_WORK,                  /*!< @if Eng Working On state
                                         @else   工作模式 @endif */
    PM_STATE_MAX,
} pm_states_t;

typedef enum {
    PM_MODE_NONE,
    PM_MODE_LP,
    PM_MODE_NORMAL,
    PM_MODE_PERFORMACE,
    PM_MODE_RESET,
    PM_MODE_MAX,
} pm_mode_t;

typedef enum {
    PM_DEV_NONE,
    PM_DEV_M_MGPIO,
    PM_DEV_M_TIMER,
    PM_DEV_M_RTC,
    PM_DEV_M_CWDT,
    PM_DEV_M_I2C0,
    PM_DEV_M_I2C1,
    PM_DEV_M_I2C2,
    PM_DEV_M_I2C3,
    PM_DEV_M_MDMA,
    PM_DEV_M_MSDMA,
    PM_DEV_M_SPI0,
    PM_DEV_M_SPI1,
    PM_DEV_M_SPI2,
    PM_DEV_M_QSPI2,
    PM_DEV_M_UART_H0,
    PM_DEV_M_UART_H1,

    PM_DEV_HS_SDIO,
    PM_DEV_HS_QSPI1,
    PM_DEV_HS_HGPIO,

    PM_DEV_MEM_XIP,
    PM_DEV_MEM_QSPI0,
    PM_DEV_MEM_OPI,
    PM_DEV_MEM_EGPIO,

    PM_DEV_B_WDT,
    PM_DEV_B_TIMER,
    PM_DEV_B_UART_L1,

    PM_DEV_DISPLAY,

    PM_DEV_AUDIO,
    PM_DEV_MAX,
} pm_dev_id_t;

typedef enum {
    /* sys sub region. */
    PMU_ID_SUB_B_SOC = 0,
    PMU_ID_SUB_BT_CBB,
    PMU_ID_SUB_B_MEM,
    PMU_ID_SUB_BT_EM,
    PMU_ID_SUB_MCU_HS,
    PMU_ID_SUB_DISPLAY,
    PMU_ID_SUB_AUDIO,
    PMU_ID_SUB_S_EGPIO,
    PMU_ID_SUB_BT_RF,
    PMU_ID_SUB_MCU,
    PMU_ID_SUB_MEM,
    PMU_ID_SUB_COM,
    PMU_ID_SUB_L2M_GMMU,
    PMU_ID_SUB_L2M_DSP,
    PMU_ID_SUB_XIP_MEM,
    PMU_ID_SUB_BMEM_RAM,

    /* ldo region. */
    PMU_ID_LDO_CLDO1,
    PMU_ID_LDO_CLDO2,
    PMU_ID_LDO_CLDO3,
    PMU_ID_LDO_XLDO,
    PMU_ID_LDO_CMU_LDO,
    PMU_ID_LDO_INTLDO1,
    PMU_ID_LDO_INTLDO2,
    PMU_ID_LDO_AUXLDO0,
    PMU_ID_LDO_AUXLDO1,
    PMU_ID_LDO_AUXLDO2,
    PMU_ID_LDO_AUXLDO3,
    PMU_ID_LDO_IOLDO,

    /* simo region. */
    PMU_ID_SIMO_VDD0P8,
    PMU_ID_SIMO_VDD1P9,
    PMU_ID_SIMO_VDD1P0,
    PMU_ID_SIMO_VDD1P3,
    PMU_ID_SIMO_VDD0P8_BCORE,
    PMU_ID_SIMO_MAX,
    PMU_ID_MAX,
} pmu_id_t;

typedef enum {
    PMU_CLDO_LEVEL_0P400V = 0,
    PMU_CLDO_LEVEL_0P425V = 1,
    PMU_CLDO_LEVEL_0P450V = 2,
    PMU_CLDO_LEVEL_0P475V = 3,
    PMU_CLDO_LEVEL_0P500V = 4,
    PMU_CLDO_LEVEL_0P525V = 5,
    PMU_CLDO_LEVEL_0P550V = 6,
    PMU_CLDO_LEVEL_0P575V = 7,
    PMU_CLDO_LEVEL_0P600V = 8,
    PMU_CLDO_LEVEL_0P625V = 9,
    PMU_CLDO_LEVEL_0P650V = 10,
    PMU_CLDO_LEVEL_0P675V = 11,
    PMU_CLDO_LEVEL_0P700V = 12,
    PMU_CLDO_LEVEL_0P750V = 13,
    PMU_CLDO_LEVEL_0P800V = 14,
    PMU_CLDO_LEVEL_0P850V = 15,
} pmu_cldo_level_t;

typedef enum {
    PMU_AUXLDO_LEVEL_1P600V = 0,
    PMU_AUXLDO_LEVEL_1P625V = 1,
    PMU_AUXLDO_LEVEL_1P650V = 2,
    PMU_AUXLDO_LEVEL_1P675V = 3,
    PMU_AUXLDO_LEVEL_1P700V = 4,
    PMU_AUXLDO_LEVEL_1P725V = 5,
    PMU_AUXLDO_LEVEL_1P750V = 6,
    PMU_AUXLDO_LEVEL_1P775V = 7,
    PMU_AUXLDO_LEVEL_1P800V = 8,
    PMU_AUXLDO_LEVEL_1P825V = 9,
    PMU_AUXLDO_LEVEL_1P850V = 10,
    PMU_AUXLDO_LEVEL_1P875V = 11,
    PMU_AUXLDO_LEVEL_1P900V = 12,
    PMU_AUXLDO_LEVEL_1P925V = 13,
    PMU_AUXLDO_LEVEL_1P950V = 14,
    PMU_AUXLDO_LEVEL_1P975V = 15,
} pmu_auxldo_level_t;

typedef enum {
    PMU_IOLDO_LEVEL_1P600V = 0,
    PMU_IOLDO_LEVEL_1P625V = 1,
    PMU_IOLDO_LEVEL_1P650V = 2,
    PMU_IOLDO_LEVEL_1P675V = 3,
    PMU_IOLDO_LEVEL_1P700V = 4,
    PMU_IOLDO_LEVEL_1P725V = 5,
    PMU_IOLDO_LEVEL_1P750V = 6,
    PMU_IOLDO_LEVEL_1P775V = 7,
    PMU_IOLDO_LEVEL_1P800V = 8,
    PMU_IOLDO_LEVEL_1P825V = 9,
    PMU_IOLDO_LEVEL_1P850V = 10,
    PMU_IOLDO_LEVEL_1P875V = 11,
    PMU_IOLDO_LEVEL_1P900V = 12,
    PMU_IOLDO_LEVEL_1P925V = 13,
    PMU_IOLDO_LEVEL_1P950V = 14,
    PMU_IOLDO_LEVEL_1P975V = 15,
} pmu_ioldo_level_t;

typedef enum {
    PMU_INTLDO1_LEVEL_1P80V  = 0,
    PMU_INTLDO1_LEVEL_1P78V  = 1,
    PMU_INTLDO1_LEVEL_1P82V  = 2,
    PMU_INTLDO1_LEVEL_1P84V  = 3,
    PMU_INTLDO1_LEVEL_1P86V  = 4,
    PMU_INTLDO1_LEVEL_1P88V  = 5,
    PMU_INTLDO1_LEVEL_1P90V  = 6,
    PMU_INTLDO1_LEVEL_1P92V  = 7,
} pmu_intldo1_level_type_t;

typedef enum {
    PMU_INTLDO2_LEVEL_1P78V  = 0,
    PMU_INTLDO2_LEVEL_1P80V  = 1,
    PMU_INTLDO2_LEVEL_1P82V  = 2,
    PMU_INTLDO2_LEVEL_1P84V  = 3,
    PMU_INTLDO2_LEVEL_1P86V  = 4,
    PMU_INTLDO2_LEVEL_1P88V  = 5,
    PMU_INTLDO2_LEVEL_1P90V  = 6,
    PMU_INTLDO2_LEVEL_1P92V  = 7,
} pmu_intldo2_level_type_t;

typedef enum {
    PMU_XLDO_LEVEL_1P600V = 0,
    PMU_XLDO_LEVEL_1P625V = 1,
    PMU_XLDO_LEVEL_1P650V = 2,
    PMU_XLDO_LEVEL_1P675V = 3,
    PMU_XLDO_LEVEL_1P700V = 4,
    PMU_XLDO_LEVEL_1P725V = 5,
    PMU_XLDO_LEVEL_1P750V = 6,
    PMU_XLDO_LEVEL_1P775V = 7,
    PMU_XLDO_LEVEL_1P800V = 8,
    PMU_XLDO_LEVEL_1P825V = 9,
    PMU_XLDO_LEVEL_1P850V = 10,
} pmu_xldo_level_type_t;

typedef enum {
    PMU_CMU_LDO_LEVEL_0P720V = 0,
    PMU_CMU_LDO_LEVEL_0P740V = 1,
    PMU_CMU_LDO_LEVEL_0P760V = 2,
    PMU_CMU_LDO_LEVEL_0P780V = 3,
    PMU_CMU_LDO_LEVEL_0P800V = 4,
    PMU_CMU_LDO_LEVEL_0P820V = 5,
    PMU_CMU_LDO_LEVEL_0P840V = 6,
    PMU_CMU_LDO_LEVEL_0P860V = 7,
    PMU_CMU_LDO_LEVEL_0P880V = 8,
    PMU_CMU_LDO_LEVEL_0P900V = 9,
    PMU_CMU_LDO_LEVEL_0P920V = 10,
    PMU_CMU_LDO_LEVEL_0P940V = 11,
    PMU_CMU_LDO_LEVEL_0P960V = 12,
    PMU_CMU_LDO_LEVEL_0P980V = 13,
    PMU_CMU_LDO_LEVEL_1P000V = 14,
    PMU_CMU_LDO_LEVEL_1P020V = 15,
} pmu_cmu_ldo_level_type_t;

/* simo1_vo2&simo2_vo2 */
typedef enum {
    PMU_VDD0P8_1P0_0P600V    = 0,
    PMU_VDD0P8_1P0_0P625V    = 1,
    PMU_VDD0P8_1P0_0P650V    = 2,
    PMU_VDD0P8_1P0_0P675V    = 3,
    PMU_VDD0P8_1P0_0P700V    = 4,
    PMU_VDD0P8_1P0_0P725V    = 5,
    PMU_VDD0P8_1P0_0P750V    = 6,
    PMU_VDD0P8_1P0_0P775V    = 7,
    PMU_VDD0P8_1P0_0P800V    = 8,
    PMU_VDD0P8_1P0_0P825V    = 9,
    PMU_VDD0P8_1P0_0P850V    = 10,
    PMU_VDD0P8_1P0_0P875V    = 11,
    PMU_VDD0P8_1P0_0P900V    = 12,
    PMU_VDD0P8_1P0_0P925V    = 13,
    PMU_VDD0P8_1P0_0P950V    = 14,
    PMU_VDD0P8_1P0_0P975V    = 15,
    PMU_VDD0P8_1P0_1P000V    = 16,
    PMU_VDD0P8_1P0_1P025V    = 17,
    PMU_VDD0P8_1P0_1P050V    = 18,
    PMU_VDD0P8_1P0_1P075V    = 19,
    PMU_VDD0P8_1P0_1P100V    = 20,
} pmu_vdd0p8_level_t;

/* simo1_vo1&simo2_vo1 */
typedef enum {
    PMU_VDD1P9_1P3_0P800V    = 0,
    PMU_VDD1P9_1P3_0P850V    = 2,
    PMU_VDD1P9_1P3_0P900V    = 4,
    PMU_VDD1P9_1P3_0P950V    = 6,
    PMU_VDD1P9_1P3_1P000V    = 8,
    PMU_VDD1P9_1P3_1P050V    = 10,
    PMU_VDD1P9_1P3_1P100V    = 12,
    PMU_VDD1P9_1P3_1P150V    = 14,
    PMU_VDD1P9_1P3_1P200V    = 16,
    PMU_VDD1P9_1P3_1P250V    = 18,
    PMU_VDD1P9_1P3_1P300V    = 20,
    PMU_VDD1P9_1P3_1P350V    = 22,
    PMU_VDD1P9_1P3_1P400V    = 24,
    PMU_VDD1P9_1P3_1P450V    = 26,
    PMU_VDD1P9_1P3_1P500V    = 28,
    PMU_VDD1P9_1P3_1P550V    = 30,
    PMU_VDD1P9_1P3_1P600V    = 32,
    PMU_VDD1P9_1P3_1P650V    = 34,
    PMU_VDD1P9_1P3_1P700V    = 36,
    PMU_VDD1P9_1P3_1P750V    = 38,
    PMU_VDD1P9_1P3_1P800V    = 40,
    PMU_VDD1P9_1P3_1P850V    = 42,
    PMU_VDD1P9_1P3_1P900V    = 44,
    PMU_VDD1P9_1P3_1P950V    = 46,
    PMU_VDD1P9_1P3_2P000V    = 48,
} pmu_vdd1p9_level_t;

typedef enum {
    // MSUB Perip & BUS
    CLOCKS_ID_M_TIMER,
    CLOCKS_ID_M_SMDMA,
    CLOCKS_ID_M_MDMA,
    CLOCKS_ID_M_I2C0,
    CLOCKS_ID_M_I2C1,
    CLOCKS_ID_M_I2C2,
    CLOCKS_ID_M_I2C3,
    CLOCKS_ID_M_SPI0,
    CLOCKS_ID_M_SPI1,
    CLOCKS_ID_M_SPI2,
    CLOCKS_ID_M_UART_H0,
    CLOCKS_ID_M_UART_H1,
    CLOCKS_ID_M_EMMC,
    CLOCKS_ID_M_SDIO,
    CLOCKS_ID_M_PWM,
    // BSUB Perip & BUS
    CLOCKS_ID_B_UART,
    CLOCKS_ID_B_TRNG,
    CLOCKS_ID_B_DIAG,
    CLOCKS_ID_B_CPU_TRACE,
    CLOCKS_ID_B_TIMER,
    CLOCKS_ID_B_WDT,
    CLOCKS_ID_B_TSENSOR,
    // AON
    CLOCKS_ID_AON_GPIO0,
    CLOCKS_ID_AON_GPIO1,
    CLOCKS_ID_AON_GPIO2,
    CLOCKS_ID_AON_GPIO3,
    CLOCKS_ID_AON_GPIO4,
    CLOCKS_ID_AON_RTC,
    CLOCKS_ID_AON_CWDT,
    // MEM SUB
    CLOCKS_ID_MEM_QSPI0,
    CLOCKS_ID_MEM_OPI,
    // DAON SUB
    CLOCKS_ID_DAON_GPIO,

    // Module
    CLOCKS_ID_SOC_MCU_CORE,
    CLOCKS_ID_SOC_MCU_PERP_LS,
    CLOCKS_ID_SOC_MCU_PERP_UART,
    CLOCKS_ID_SOC_MCU_PERP_SPI,
    CLOCKS_ID_SOC_COM_BUS,
    CLOCKS_ID_SOC_SDIOM,
    CLOCKS_ID_SOC_MEM_BUS,
    CLOCKS_ID_SOC_XIP_OPI,
    CLOCKS_ID_SOC_XIP_QSPI,
    CLOCKS_ID_SOC_GPU,
    CLOCKS_ID_SOC_HIFI,
    CLOCKS_ID_SOC_CODEC,
    CLOCKS_ID_SOC_BT_CORE,
    CLOCKS_ID_SOC_MAX,
    CLOCKS_ID_MAX,
} clocks_id_t;

typedef enum {
    CLOCKS_SRC_RC        = 0,
    CLOCKS_SRC_TCXO      = 1,
    CLOCKS_SRC_TCXO_2X   = 2,
    CLOCKS_SRC_PLL       = 3,
    CLOCKS_SRC_32K       = 4,
    CLOCKS_SRC_PAD_CLKIN = 5,
    CLOCKS_SRC_MAX,
} clocks_src_t;

typedef struct {
    uint8_t *data;
    uint32_t len;
    uint32_t (*handler)(uint8_t *data, uint32_t len);
} pm_fsm_content_t;

/**
 * @if Eng
 * @brief  Operating functions of peripheral devices.
 * @else
 * @brief  外设的设备处理函数
 * @endif
 */
typedef struct {
    uint32_t (*suspend)(void);
    uint32_t (*resume)(void);
    uint32_t (*ioctl)(uint8_t type, uint8_t *data, uint32_t len);
} pm_dev_ops_t;

/**
 * @if Eng
 * @brief  The global list of device functions is filled by the driver and
           bus and registered with the PM module.Supports single-point registration with the PM module in services.
 * @else
 * @brief  设备函数全局列表，由驱动、总线填充，统一注册到pm模块；同时支持在业务中单点注册至pm模块
 * @endif
 */
#define PM_ALL_DEV { \
    {PM_DEV_M_TIMER,   {NULL, NULL, pm_dev_ostimer_ioctl}}, \
    {PM_DEV_M_CWDT,    {NULL, NULL, pm_dev_wdt_ioctl}},   \
    {PM_DEV_M_RTC,     {NULL, NULL, pm_dev_rtc_ioctl}}, \
    {PM_DEV_M_UART_H0, {NULL, NULL, NULL}}, \
}

/**
 * @}
 */

#endif
