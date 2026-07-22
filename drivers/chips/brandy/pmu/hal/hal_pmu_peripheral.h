/*
 * Copyright (c) @CompanyNameMagicTag 2018-2022. All rights reserved.
 * Description:   HAL PMU PERIPHERAL DRIVER HEADER FILE
 */

#ifndef HAL_PMU_PERIPHERALS_H
#define HAL_PMU_PERIPHERALS_H

#include "platform_core.h"
#include "chip_definitions.h"
/** @defgroup connectivity_drivers_hal_pmu_peripheral PMU Peripheral
  * @ingroup  connectivity_drivers_hal
  * @{
  */
// Pheripheral status bits
#define PMU_PERIPHERAL_STS_BITS 1
/**
 * @brief  Describe pheriphral status.
 */
typedef enum {
    HAL_PMU_PERIP_STATUS_CLOCK_OFF = 0,   // !< The peripheral clock is off
    HAL_PMU_PERIP_STATUS_TURNING_ON = 1,  // !< The peripheral clock is truning on
    HAL_PMU_PERIP_STATUS_ERROR,           // !< Get status error
} hal_pmu_perip_status_type_t;

/**
 * @brief  Describe pheriphral error report.
 */
typedef enum {
    HAL_PMU_PERIP_ERR_REPORT_NORMAL = 0,     // !< The peripheral work well
    HAL_PMU_PERIP_ERR_REPORT_EXCEPTION = 1,  // !< The peripheral has been access, when not in working satus.
} hal_pmu_perip_err_report_type_t;

/**
 * @brief  Describe pheriphral request behavior.
 */
typedef enum {
    HAL_PMU_PERIP_REQUEST_RESET = 0,   // !< Request reset the peripheral, make it in initial status, can't running.
    HAL_PMU_PERIP_REQUEST_DERESET = 1, // !< Request dereset the peripheral, make it can run.
    HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER = 2, // !< Request the peripheral enter low power mode.
    HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER = 3,  // !< Request the peripheral exit low power mode.
} hal_pmu_perip_request_type_t;

/**
 * @brief  Describe mcpu pheriphrals.
 */
typedef enum {
    HAL_PMU_MCPU_PERIP_TIMER        = 0,    // !< The mcpu peripheral timer
    HAL_PMU_MCPU_PERIP_SMDMA        = 1,    // !< The smcpu peripheral dma
    HAL_PMU_MCPU_PERIP_MDMA         = 2,    // !< The mcpu peripheral dma
    HAL_PMU_MCPU_PERIP_I2C_BUS_0    = 3,    // !< The mcpu peripheral I2C BUS0
    HAL_PMU_MCPU_PERIP_I2C_BUS_1    = 4,    // !< The mcpu peripheral I2C BUS1
    HAL_PMU_MCPU_PERIP_I2C_BUS_2    = 5,    // !< The mcpu peripheral I2C BUS2
    HAL_PMU_MCPU_PERIP_I2C_BUS_3    = 6,    // !< The mcpu peripheral I2C BUS3
    HAL_PMU_MCPU_PERIP_SPI0_M       = 7,    // !< The mcpu peripheral SPI BUS0
    HAL_PMU_MCPU_PERIP_SPI1_MS      = 8,    // !< The mcpu peripheral SPI BUS1
    HAL_PMU_MCPU_PERIP_SPI2_MS      = 9,    // !< The mcpu peripheral SPI BUS2
    HAL_PMU_MCPU_PERIP_QSPI2_1CS    = 10,   // !< The mcpu peripheral QSPI BUS2
    HAL_PMU_MCPU_PERIP_UART_H0      = 11,   // !< The mcpu peripheral UARTH0
    HAL_PMU_MCPU_PERIP_UART_H1      = 12,   // !< The mcpu peripheral UARTH1
    HAL_PMU_MCPU_PERIP_EMMC         = 13,   // !< The mcpu peripheral EMMC
    HAL_PMU_MCPU_PERIP_SDIO_HOST    = 14,   // !< The mcpu peripheral SDIO HOST
    HAL_PMU_MCPU_PERIP_PWM          = 15,     // !< The mcpu peripheral PWM
    HAL_PMU_MCPU_PERIP_MAX,
} hal_pmu_mcpu_perips_type_t;

/**
 * @brief  Describe common sub pheriphrals.
 */
typedef enum {
    HAL_PMU_MEM_PERIP_QSPI0_2CS = 0,  // !< The common sub peripheral qspi 1cs
    HAL_PMU_MEM_PERIP_QSPI1_2CS = 1,  // !< The common sub peripheral qspi 2cs
    HAL_PMU_MEM_PERIP_OPI       = 2,  // !< The common sub peripheral qspi xip
} hal_pmu_mem_perips_type_t;

/**
 * @brief  Describe bcpu pheriphrals.
 */
typedef enum {
    HAL_PMU_BCPU_PERIP_DMA      = 0,    // !< The bcpu peripheral dma, can not enter low power mode.
    HAL_PMU_BCPU_PERIP_TIMER    = 1,    // !< The bcpu peripheral timer, can not enter low power mode.
    HAL_PMU_BCPU_PERIP_WDT      = 2,    // !< The bcpu peripheral watchdog, can not enter low power mode.
    HAL_PMU_BCPU_PERIP_UART     = 3,    // !< The bcpu peripheral watchdog, can not enter low power mode.
} hal_pmu_bcpu_perips_type_t;

/**
 * @brief  Describe bcpu always on pheriphrals.
 */
typedef enum {
    HAL_PMU_BAON_PERIP_RTC      = 0,  // !< The bcpu always on peripheral rtc, can not enter low power mode.
    HAL_PMU_BAON_PERIP_GPIO0    = 1,  // !< The bcpu always on peripheral gpio0
    HAL_PMU_BAON_PERIP_GPIO1    = 2,  // !< The bcpu always on peripheral gpio1
    HAL_PMU_BAON_PERIP_GPIO2    = 3,  // !< The bcpu always on peripheral gpio0
    HAL_PMU_BAON_PERIP_GPIO3    = 4,  // !< The bcpu always on peripheral gpio1
    HAL_PMU_BAON_PERIP_GPIO4    = 5,  // !< The bcpu always on peripheral gpio1
} hal_pmu_baon_perips_type_t;

/**
 * @brief  Describe mcpu always on pheriphrals.
 */
typedef enum {
    HAL_PMU_MAON_PERIP_GPIO0    = 1,  // !< The mcpu always on peripheral gpio0
    HAL_PMU_MAON_PERIP_GPIO1    = 2,  // !< The mcpu always on peripheral gpio1
    HAL_PMU_MAON_PERIP_GPIO2    = 3,  // !< The mcpu always on peripheral gpio1
    HAL_PMU_MAON_PERIP_GPIO3    = 4,  // !< The mcpu always on peripheral gpio1
    HAL_PMU_MAON_PERIP_GPIO4    = 5,  // !< The mcpu always on peripheral gpio1
    HAL_PMU_MAON_PERIP_RTC      = 6,  // !< The mcpu always on peripheral rtc, can not enter low power mode.
    HAL_PMU_MAON_PERIP_CWDT     = 7,  // !< The mcpu always on peripheral gpio1
} hal_pmu_maon_perips_type_t;

/**
 * @brief  Describe dsp cpu always on pheriphrals.
 */
typedef enum {
    HAL_PMU_DAON_PERIP_GPIO     = 0,  // !< The dcpu always on peripheral gpio0
} hal_pmu_daon_perips_type_t;

/* AON peripherals lower power managment Register */
#define PMU_TOP_PERIPHERALS_ATOP0_L_REG (GLB_CTL_M_RB_BASE + 0x160)
#define PMU_TOP_PERIPHERALS_ATOP0_H_REG (GLB_CTL_M_RB_BASE + 0x164)
#define PMU_TOP_PERIPHERALS_ATOP1_L_REG (GLB_CTL_M_RB_BASE + 0x168)
#define PMU_TOP_PERIPHERALS_ATOP1_H_REG (GLB_CTL_M_RB_BASE + 0x16C)
#define PMU_TOP_PERIPHERALS_ATOP2_L_REG (GLB_CTL_M_RB_BASE + 0x170)
#define PMU_TOP_PERIPHERALS_ATOP2_H_REG (GLB_CTL_M_RB_BASE + 0x174)
#define PMU_TOP_PERIPHERALS_ATOP3_L_REG (GLB_CTL_M_RB_BASE + 0x178)
#define PMU_TOP_PERIPHERALS_ATOP3_H_REG (GLB_CTL_M_RB_BASE + 0x17C)

/* ATOP0 peripheral lower power managment status bit describe */
#define PMU_ATOP0_MGPIO3_REQUEST_LP     15
#define PMU_ATOP0_MGPIO2_REQUEST_LP     14
#define PMU_ATOP0_MGPIO1_REQUEST_LP     13
#define PMU_ATOP0_MGPIO0_REQUEST_LP     12
#define PMU_ATOP0_MGPIO3_RST            3
#define PMU_ATOP0_MGPIO2_RST            2
#define PMU_ATOP0_MGPIO1_RST            1
#define PMU_ATOP0_MGPIO0_RST            0

/* ATOP0 peripheral lower power managment status bit describe */
#define PMU_ATOP0_MGPIO3_STS_OFFSET     11
#define PMU_ATOP0_MGPIO2_STS_OFFSET     10
#define PMU_ATOP0_MGPIO1_STS_OFFSET     9
#define PMU_ATOP0_MGPIO0_STS_OFFSET     8

/* ATOP1 peripheral lower power managment status bit describe */
#if CHIP_LIBRA || CHIP_SOCMN1
#define PMU_ATOP1_REQUEST_LP_START      11
#else
#define PMU_ATOP1_REQUEST_LP_START      12
#endif
#define PMU_ATOP1_GRTC_RST              4
#define PMU_ATOP1_BRTC_RST              3
#define PMU_ATOP1_CWDT_RST              2
#define PMU_ATOP1_MRTC_RST              1
#define PMU_ATOP1_MGPIO4_RST            0

#define PMU_ATOP1_MGPIO4_REQUEST_LP     (PMU_ATOP1_REQUEST_LP_START + PMU_ATOP1_MGPIO4_RST)
#define PMU_ATOP1_MRTC_REQUEST_LP       (PMU_ATOP1_REQUEST_LP_START + PMU_ATOP1_MRTC_RST)
#define PMU_ATOP1_CWDT_REQUEST_LP       (PMU_ATOP1_REQUEST_LP_START + PMU_ATOP1_CWDT_RST)
#define PMU_ATOP1_BRTC_REQUEST_LP       (PMU_ATOP1_REQUEST_LP_START + PMU_ATOP1_BRTC_RST)
#define PMU_ATOP1_GRTC_REQUEST_LP       (PMU_ATOP1_REQUEST_LP_START + PMU_ATOP1_GRTC_RST)

/* ATOP1 peripheral lower power managment status bit describe */
#define PMU_ATOP1_GRTC_STS_OFFSET       12
#define PMU_ATOP1_BRTC_STS_OFFSET       11
#define PMU_ATOP1_CWDT_STS_OFFSET       10
#define PMU_ATOP1_MRTC_STS_OFFSET       9
#define PMU_ATOP1_MGPIO4_STS_OFFSET     8

/* ATOP2 peripheral lower power managment status bit describe */
#define PMU_ATOP2_BGPIO3_REQUEST_LP     15
#define PMU_ATOP2_BGPIO2_REQUEST_LP     14
#define PMU_ATOP2_BGPIO1_REQUEST_LP     13
#define PMU_ATOP2_BGPIO0_REQUEST_LP     12
#define PMU_ATOP2_BGPIO3_RST            3
#define PMU_ATOP2_BGPIO2_RST            2
#define PMU_ATOP2_BGPIO1_RST            1
#define PMU_ATOP2_BGPIO0_RST            0

/* ATOP2 peripheral lower power managment status bit describe */
#define PMU_ATOP2_BGPIO3_STS_OFFSET     11
#define PMU_ATOP2_BGPIO2_STS_OFFSET     10
#define PMU_ATOP2_BGPIO1_STS_OFFSET     9
#define PMU_ATOP2_BGPIO0_STS_OFFSET     8

/* ATOP3 peripheral lower power managment status bit describe */
#define PMU_ATOP3_DGPIO_REQUEST_LP      13
#define PMU_ATOP3_BGPIO4_REQUEST_LP     12
#define PMU_ATOP3_DGPIO_RST             1
#define PMU_ATOP3_BGPIO4_RST            0

/* ATOP3 peripheral lower power managment status bit describe */
#define PMU_ATOP3_DGPIO_STS_OFFSET      9
#define PMU_ATOP3_BGPIO4_STS_OFFSET     8

/* AON peripheral lower power managment status bit describe */
#define PMU_AON_UART_RST         3
#define PMU_AON_GPIO1_RST        2
#define PMU_AON_GPIO0_RST        1
#define PMU_AON_RTC_RST          0

/**
  * @}
  */
#endif
