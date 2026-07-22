/*
 * Copyright (c) @CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description:  Application Core Platform Definitions
 * Author: @CompanyNameTag
 * Date:
 */

#ifndef PLATFORM_CORE_H
#define PLATFORM_CORE_H

#include "product.h"
#include "chip_core_definition.h"

/**
 * @defgroup DRIVER_PLATFORM_CORE CHIP Platform CORE Driver
 * @ingroup DRIVER_PLATFORM
 * @{
 */
#define GLB_CTL_M_RB_BASE        0x57000000
#define GLB_CTL_B_RB_BASE        0x57000400
#define GLB_CTL_D_RB_BASE        0x57000800
#define DISPLAY_CTL_RB_BASE      0x56000000
#define GPU_BASE_ADDR            0x56200000
#define DSS_BASE_ADDR            0x56100000
#define L2RAM_ENABLE_BASE_ADDR   0x57004000

#define B_CTL_RB_BASE            0x59000000
#define M_CTL_RB_BASE            0x52000000
#define COM_CTL_RB_BASE          0x55000000
#define PMU1_CTL_RB_BASE         0x57004000
#define PMU2_CMU_CTL_RB_BASE     0x57008000
#define ULP_AON_CTL_RB_ADDR      0x5702c000
#define FUSE_CTL_RB_ADDR         0x57028000
#define XO_CORE_TRIM_REG         0x57028308
#define XO_CORE_CTRIM_REG        0x5702830c
#define XIP_CACHE_CTL            0xA3006000
#define NMI_CTL_REG_BASE_ADDR    0x52000700
#define DIAG_BASE_ADDR           0x52004000

#define FLASH_START_ADDR 0x70000000

#define UART0_BASE    0x52081000  /* UART1 */
#define UART1_BASE    0x52080000  /* HS_UART */
#define DMA_BASE_ADDR 0x52070000  /* M_DMA */
#define SDMA_BASE_ADDR 0x520A0000  // use for cs chip sdma

/* IR digit and analog reg base addr */
#define IR_BASE_ADDR        0x52083000
#define IR_ANALOG_BASE_ADDR 0x57004000

/* rgb888 reg base addr */
#define VDP_BASE_ADDR       0x56040000

/* I2C reg base addr */
#define I2C_BUS_0_BASE_ADDR 0x52084000
#define I2C_BUS_1_BASE_ADDR 0x52085000
#define I2C_BUS_2_BASE_ADDR 0x52086000
#define I2C_BUS_3_BASE_ADDR 0x52087000
#define I2C_BUS_4_BASE_ADDR 0x56003000
#define I2C_BUS_5_BASE_ADDR 0x56004000

#define SPI_BUS_0_BASE_ADDR 0x5208A000  // SPI_M0
#define SPI_BUS_1_BASE_ADDR 0x52088000  // SPI_MS_1
#define SPI_BUS_2_BASE_ADDR 0x52089000  // SPI_MS_2
#define SPI_BUS_3_BASE_ADDR 0xA3000000  // QSPI_0
#define SPI_BUS_4_BASE_ADDR 0x52065000  // QSPI_1
#define SPI_BUS_5_BASE_ADDR 0x52090000  // QSPI_2
#define SPI_BUS_6_BASE_ADDR 0xA3004000  // OSPI

#define DMA_HANDSHAKE_I2C_BUS_0_TX HAL_DMA_HANDSHAKING_I2C0_TX
#define DMA_HANDSHAKE_I2C_BUS_0_RX HAL_DMA_HANDSHAKING_I2C0_RX
#define DMA_HANDSHAKE_I2C_BUS_1_TX HAL_DMA_HANDSHAKING_I2C1_TX
#define DMA_HANDSHAKE_I2C_BUS_1_RX HAL_DMA_HANDSHAKING_I2C1_RX
#define DMA_HANDSHAKE_I2C_BUS_2_TX HAL_DMA_HANDSHAKING_I2C2_TX
#define DMA_HANDSHAKE_I2C_BUS_2_RX HAL_DMA_HANDSHAKING_I2C2_RX
#define DMA_HANDSHAKE_I2C_BUS_3_TX HAL_DMA_HANDSHAKING_I2C3_TX
#define DMA_HANDSHAKE_I2C_BUS_3_RX HAL_DMA_HANDSHAKING_I2C3_RX
#define DMA_HANDSHAKE_I2C_BUS_4_TX HAL_DMA_HANDSHAKING_I2C4_TX
#define DMA_HANDSHAKE_I2C_BUS_4_RX HAL_DMA_HANDSHAKING_I2C4_RX

#define DMA_HANDSHAKE_IR_TX        HAL_DMA_HANDSHAKING_IR_TX
#define DMA_HANDSHAKE_IR_RX        HAL_DMA_HANDSHAKING_IR_RX

#define HAL_SPI_DEVICE_MODE_SET_REG   (*(volatile unsigned short *)(0x52000950))

/* PWM reg base addr */
#define PWM_0_BASE 0x52000840
#define PWM_1_BASE 0x52000860
#define PWM_2_BASE 0x52000880
#define PWM_3_BASE 0x520008A0
#define PWM_4_BASE 0x520008C0
#define PWM_5_BASE 0x520008E0

/* PWM INTR REG */
#define PWM_INTR_ENABLE_REG (*(volatile unsigned short *)0x52000900)
#define PWM_INTR_CLEAR_REG  (*(volatile unsigned short *)0x52000904)
#define PWM_INTR_STATUS_REG (*(volatile unsigned short *)0x52000908)

// GPIO regs
#define GPIO0_BASE_ADDR 0x57010000
#define GPIO1_BASE_ADDR 0x57014000
#define GPIO2_BASE_ADDR 0x57018000
#define GPIO3_BASE_ADDR 0x5701C000
#define GPIO4_BASE_ADDR 0x57020000
#define ULP_GPIO_BASE_ADDR 0x57030000 // ULP GPIO

// GPIO select core
#define HAL_GPIO_D_CORE_SET_REG             0x570001B0
#define HAL_GPIO_NON_D_CORE_SET_REG         0x57000180
#define HAL_GPIO_CORE_SET_CHANNEL_OFFSET    0x08
#define HAL_GPIO_CORE_SET_REG_OFFSET        2
#define HAL_GPIO_CORE_SET_GPIOS             16

// ULP GPIO int clk config
#define HAL_GPIO_ULP_AON_GP_REG                 0x5702C024
#define HAL_GPIO_ULP_AON_PCLK_INT_EN_BIT        0
#define HAL_GPIO_ULP_AON_PCLK_INT_CLK_SEL_BIT   1
#define HAL_GPIO_ULP_PCLK_INTR_STATUS_BITS      0x3

#define RTC_TIMER_BASE_ADDR 0x57024000
#define SYSTICK_BASE_ADDR 0x570003e4
#define MCU_XIP_FLASH_ADDR  0x70000000
#define BCPU_XIP_FLASH_ADDR 0x10000000
#define TCXO_COUNT_BASE_ADDR 0x55000580
#define WDT_BASE_ADDR 0x52003000
#define CHIP_WDT_BASE_ADDRESS 0x57034000

#define HAL_SOFT_RST_CTL_BASE               (GLB_CTL_M_RB_BASE)
#define HAL_GLB_CTL_M_ATOP1_L_REG_OFFSET    0x168
#define HAL_CHIP_WDT_ATOP1_RST_BIT          2

// SEC BASE ADDR
#define SEC_CTL_RB_BASE 0x52009000
#define RSAV2_S_RB_BASE 0x52030000
#define TRNG_RB_BASE 0x52009800

// Timer reg base addr.
#define TIMER_BASE_ADDR                         0x52002000
#define TIMER_0_BASE_ADDR                       (TIMER_BASE_ADDR + 0x00)
#define TIMER_1_BASE_ADDR                       (TIMER_BASE_ADDR + 0x14)
#define TIMER_2_BASE_ADDR                       (TIMER_BASE_ADDR + 0x28)
#define TIMER_3_BASE_ADDR                       (TIMER_BASE_ADDR + 0x3C)
#define TIMER_SYSCTL_BASE_ADDR                  (TIMER_BASE_ADDR + 0xA0)

#define TICK_TIMER_BASE_ADDR                    TIMER_3_BASE_ADDR

// pio base address
#define HAL_PIO_PINMAX_C_PINMAX_CTL_ADDR     0x52008000
#define HAL_PIO_PINMAX_PINMAX_CTL_ADDR       0x5700C000
#define HAL_PIO_MEM_PINMUX_CTL_ADDR          0xA3005000
#define HAL_PIO_HS_PINMUX_CTL_ADDR           0x52065800
#define HAL_PIO_ULPON_CTL_ADDR               0x5702C000

// FPGA PSRAM PINMUX
// Temporary configuration, which needs to be modified in the future.
#define S_E_GPIO_PINMUX_BASEADDR   0xa3005000
#define S_E_GPIO_PINMUX_OPI_OFF    0x18
#define S_E_GPIO_IOCTRL_BASEADDR   0x5700cc00
#define S_G_GPIO_IOCTRL_OPI_OFF    0x218

// SDIO SLAVE
#define SDIO_SLAVE_BASE_ADDR 0x52060000

/*
 * Maximum UART buses
 * Defined here rather than in the uart_bus_t enum, due to needing to use it for conditional compilation
 */
#define UART_BUS_MAX_NUMBER 2  // !< Max number of UARTS available

#define I2C_BUS_MAX_NUMBER  4  // !< Max number of I2C available
#define I2C_4_IRQn I2C_0_IRQN
#define I2C_5_IRQn I2C_0_IRQN

#define TIMER_MAX_AVAILABLE_NUMBER    4  // !< Max number of timer available
#define RTC_MAX_AVAILABLE_NUMBER      4  // !< Max number of rtc available

#define SPI_BUS_MAX_NUMBER  7  // !< Max number of SPI available
#define GPIO_MAX_NUMBER     5  // !< Max number of GPIO available
#define PWM_MAX_NUMBER      6  // !< Max number of PWM available

#define S_DMA_CHANNEL_MAX_NUM    4  // !< Max number of SM_DMA available
#define B_DMA_CHANNEL_MAX_NUM    8  // !< Max number of M_DMA available

#define DMA_CHANNEL_MAX_NUM      (S_DMA_CHANNEL_MAX_NUM + B_DMA_CHANNEL_MAX_NUM)

#define CHIP_BCPU_SWDDIO  0
#define CHIP_BCPU_SWDCLK  0

#define TEST_SUITE_UART_BUS UART_BUS_0
#define TEST_SUITE_UART_TX_PIN  S_MGPIO19
#define TEST_SUITE_UART_RX_PIN  S_MGPIO20

#define CODELOADER_UART_BUS UART_BUS_1
#define CODELOADER_UART_TX_PIN S_MGPIO33
#define CODELOADER_UART_RX_PIN S_MGPIO34

#define LOG_UART_BUS    UART_BUS_1
#define LOG_UART_TX_PIN     S_MGPIO33
#define LOG_UART_RX_PIN     S_MGPIO34

#if defined(BUILD_APPLICATION_ROM)
#define SW_DEBUG_UART_BUS UART_BUS_1
#define CHIP_FIXED_TX_PIN S_MGPIO33
#define CHIP_FIXED_RX_PIN S_MGPIO34
#else
#ifndef FT_SINGLE_UART
#define SW_DEBUG_UART_BUS UART_BUS_0
#define CHIP_FIXED_TX_PIN S_MGPIO19
#define CHIP_FIXED_RX_PIN S_MGPIO20
#else
#define SW_DEBUG_UART_BUS UART_BUS_1
#define CHIP_FIXED_TX_PIN S_MGPIO33
#define CHIP_FIXED_RX_PIN S_MGPIO34
#endif
#endif
#define CHIP_FIXED_UART_BUS UART_BUS_0

#define QSPI_MAX_NUMBER    3
#define FLASH_QSPI_ID      QSPI_BUS_0
#define QSPI_0_BASE_ADDR   0xA3000000
#define QSPI_1_BASE_ADDR   0x52065000
#define QSPI_2_BASE_ADDR   0x52090000

/**
 * @brief  Definition of QSPI bus index.
 */
typedef enum {
    QSPI_BUS_0 = 0,
    QSPI_BUS_1,
    QSPI_BUS_2,
    QSPI_BUS_NONE = QSPI_MAX_NUMBER
} qspi_bus_t;

/**
 * @brief  Definition of UART bus index.
 */
typedef enum {
    UART_BUS_0 = 0,  // !< UART1
#if UART_BUS_MAX_NUMBER > 1
    UART_BUS_1 = 1,  // !< HS_UART
#endif
#if UART_BUS_MAX_NUMBER > 2
    UART_BUS_2 = 2,
#endif
    UART_BUS_NONE = UART_BUS_MAX_NUMBER  // !< Value used as invalid/unused UART number
} uart_bus_t;

/**
 * @brief  Definition of I2C bus index.
 */
typedef enum {
    I2C_BUS_0,               // !< I2C0 msater/slave
    I2C_BUS_1,               // !< I2C1 msater/slave
    I2C_BUS_2,               // !< I2C2 msater/slave
    I2C_BUS_3,               // !< I2C3 msater/slave
#if I2C_BUS_MAX_NUMBER > 4
    I2C_BUS_4,               // !< I2C_PHY msater/slave
#endif
#if I2C_BUS_MAX_NUMBER > 5
    I2C_BUS_5,               // !< I2C_TP msater/slave
#endif
    I2C_BUS_NONE = I2C_BUS_MAX_NUMBER
} i2c_bus_t;

/**
 * @brief  Definition of SPI bus index.
 */
typedef enum {
    SPI_BUS_0 = 0,
    SPI_BUS_1,
    SPI_BUS_2,
    SPI_BUS_3,
    SPI_BUS_4,
    SPI_BUS_5,
    SPI_BUS_6,
    OPI_BUS = SPI_BUS_6,
    SPI_BUS_NONE = SPI_BUS_MAX_NUMBER
} spi_bus_t;

/* !< SLAVE CPU */
typedef enum {
    SLAVE_CPU_DSP0,
    SLAVE_CPU_DSP1,
    SLAVE_CPU_BT,
    SLAVE_CPU_MAX_NUM,
} slave_cpu_t;

/**********************************************************************/
/************************* MPU config base addr ***********************/
/**********************************************************************/
// config register region
#define MPU_REG_ADDR0_BASE           0x50000000
#define MPU_REG_ADDR1_BASE           0xA3000000

// XIP PSRAM read & execute region
#define MPU_XIP_PSRAM_RO_ADDR_BASE      0x08000000

// XIP PSRAM bypass(read/write) region
#define MPU_XIP_PSRAM_RW_ADDR_BASE      0x0C000000

// XIP NorFlash region
#define MPU_XIP_FLASE_RO_ADDR_BASE      0x10000000

// Sharemem region
#define MPU_SHAREMEM_ADDR_BASE          0x87000000

// MDMA(m0) address judge
#define MEM_X2P_MEMORY_START    0xA3000000
#define MEM_X2P_MEMORY_END      0xA3008FFF
#define L2RAM_MEMORY_START      0x60100000
#define L2RAM_MEMORY_END        0x61FFFFFF
#define QSPI_XIP_MEMORY_START   0x68000000
#define QSPI_XIP_MEMORY_END     0x77FFFFFF

#if defined(MEMORY_MINI)
#define DISPLAY_RAM1            0x60252000
#define DISPLAY_RAM2            0x6029cc00
#else
#if defined(SUPPORT_GPU_GMMU)
#define DISPLAY_RAM1            0x61240000
#define DISPLAY_RAM2            0x61a40000
#else
#define DISPLAY_RAM1            0x60280000
#define DISPLAY_RAM2            0x602fe000
#endif
#endif

// CHIP RESET offset address
#define CHIP_RESET_OFF   0x850

// Power on the flash memory on the FPGA
#define PMU_RESERV1      0x57004244

// pmu auxldo for flash
#define PMU_AUXLDO0_CFG   (PMU1_CTL_RB_BASE + 0x0214)
#define PMU_AUXLDO1_CFG   (PMU1_CTL_RB_BASE + 0x0218)
#define PMU_AUXLDO2_CFG   (PMU1_CTL_RB_BASE + 0x021c)
#define PMU_AUXLDO_ENABLE_DELAY_BIT 12
#define PMU_AUXLDO_ENABLE_BIT       11
#define PMU_AUXLDO_POWER_ON_DELAY_MS  5ULL
#define PMU_AUXLDO_POWER_OFF_DELAY_MS  50ULL
#define PMU_AUXLDO_POWER_ON_DELAY_US  150ULL

/**
 * @brief Definition of pin.
 */
typedef enum {
    S_EGPIO0 = 0,
    S_EGPIO1 = 1,
    S_EGPIO2 = 2,
    S_EGPIO3 = 3,
    S_EGPIO4 = 4,
    S_EGPIO5 = 5,
    S_EGPIO6 = 6,
    S_EGPIO7 = 7,
    S_EGPIO8 = 8,
    S_EGPIO9 = 9,
    S_EGPIO10 = 10,
    S_EGPIO11 = 11,
    S_EGPIO12 = 12,
    S_EGPIO13 = 13,
    S_EGPIO14 = 14,
    S_EGPIO15 = 15,
    S_EGPIO16 = 16,
    S_EGPIO17 = 17,
    S_EGPIO18 = 18,
    S_EGPIO19 = 19,
    S_EGPIO20 = 20,
    S_EGPIO21 = 21,
    S_EGPIO22 = 22,
    S_EGPIO23 = 23,
    S_EGPIO24 = 24,
    S_EGPIO25 = 25,

    S_HGPIO0 = 26,
    S_HGPIO1 = 27,
    S_HGPIO2 = 28,
    S_HGPIO3 = 29,
    S_HGPIO4 = 30,
    S_HGPIO5 = 31,
    S_HGPIO6 = 32,
    S_HGPIO7 = 33,
    S_HGPIO8 = 34,
    S_HGPIO9 = 35,
    S_HGPIO10 = 36,
    S_HGPIO11 = 37,
    S_HGPIO12 = 38,
    S_HGPIO13 = 39,
    S_HGPIO14 = 40,
    S_HGPIO15 = 41,

    S_MGPIO0  = 42,
    S_MGPIO1  = 43,
    S_MGPIO2  = 44,
    S_MGPIO3  = 45,
    S_MGPIO4  = 46,
    S_MGPIO5  = 47,
    S_MGPIO6  = 48,
    S_MGPIO7  = 49,
    S_MGPIO8  = 50,
    S_MGPIO9  = 51,
    S_MGPIO10 = 52,
    S_MGPIO11 = 53,
    S_MGPIO12 = 54,
    S_MGPIO13 = 55,
    S_MGPIO14 = 56,
    S_MGPIO15 = 57,
    S_MGPIO16 = 58,
    S_MGPIO17 = 59,
    S_MGPIO18 = 60,
    S_MGPIO19 = 61,
    S_MGPIO20 = 62,
    S_MGPIO21 = 63,
    S_MGPIO22 = 64,
    S_MGPIO23 = 65,
    S_MGPIO24 = 66,
    S_MGPIO25 = 67,
    S_MGPIO26 = 68,
    S_MGPIO27 = 69,
    S_MGPIO28 = 70,
    S_MGPIO29 = 71,
    S_MGPIO30 = 72,
    S_MGPIO31 = 73,
    S_MGPIO32 = 74,
    S_MGPIO33 = 75,
    S_MGPIO34 = 76,
    S_MGPIO35 = 77,
    S_MGPIO36 = 78,
    S_MGPIO37 = 79,
    S_MGPIO38 = 80,

    S_AGPIO_L0 = 81,
    S_AGPIO_L1 = 82,
    S_AGPIO_L2 = 83,
    S_AGPIO_L3 = 84,
    S_AGPIO_L4 = 85,
    S_AGPIO_L5 = 86,
    S_AGPIO_L6 = 87,
    S_AGPIO_L7 = 88,
    S_AGPIO_L8 = 89,
    S_AGPIO_L9 = 90,
    S_AGPIO_L10 = 91,
    S_AGPIO_L11 = 92,
    S_AGPIO_L12 = 93,
    S_AGPIO_L13 = 94,
    S_AGPIO_L14 = 95,
    S_AGPIO_L15 = 96,
    S_AGPIO_L16 = 97,
    S_AGPIO_L17 = 98,
    S_AGPIO_L18 = 99,
    S_AGPIO_L19 = 100,
    S_AGPIO_L20 = 101,
    S_AGPIO_L21 = 102,
    S_AGPIO_L22 = 103,
    S_AGPIO_L23 = 104,
    S_AGPIO_L24 = 105,
    S_AGPIO_L25 = 106,
    S_AGPIO_L26 = 107,
    S_AGPIO_L27 = 108,
    S_AGPIO_L28 = 109,
    S_AGPIO_L29 = 110,
    S_AGPIO_L30 = 111,
    S_AGPIO_L31 = 112,
    S_AGPIO_L32 = 113,

    S_AGPIO_R0 = 114,
    S_AGPIO_R1 = 115,
    S_AGPIO_R2 = 116,
    S_AGPIO_R3 = 117,
    S_AGPIO_R4 = 118,
    S_AGPIO_R5 = 119,
    S_AGPIO_R6 = 120,
    S_AGPIO_R7 = 121,
    S_AGPIO_R8 = 122,
    S_AGPIO_R9 = 123,
    S_AGPIO_R10 = 124,

    ULP_GPIO0 = 125,
    ULP_GPIO1 = 126,
    ULP_GPIO2 = 127,
    ULP_GPIO3 = 128,
    ULP_GPIO4 = 129,
    ULP_GPIO5 = 130,
    ULP_GPIO6 = 131,
    PIN_NONE  = 132,

    PMUIC_IRQ = ULP_GPIO6,
    ULP_PIN   = ULP_GPIO0,
    HRESET    = ULP_GPIO5,
} pin_t;

typedef enum {
    L_MGPIO0 = S_EGPIO6,
    L_MGPIO1 = S_EGPIO7,
    L_MGPIO2 = S_EGPIO8,
    L_MGPIO3 = S_EGPIO9,
    L_MGPIO4 = S_EGPIO10,
    L_MGPIO5 = S_EGPIO11,
    L_MGPIO6 = S_EGPIO12,
    L_MGPIO7 = S_EGPIO13,
    L_MGPIO8 = S_EGPIO14,
    L_MGPIO9 = S_EGPIO15,
    L_MGPIO10 = S_EGPIO16,
    L_MGPIO11 = S_EGPIO17,
    L_MGPIO12 = S_EGPIO18,
    L_MGPIO13 = S_EGPIO19,
    L_MGPIO14 = S_EGPIO20,
    L_MGPIO15 = S_EGPIO21,
    L_MGPIO16 = S_EGPIO22,
    L_MGPIO17 = S_EGPIO23,
    L_MGPIO18 = S_EGPIO24,
    L_MGPIO19 = S_EGPIO25,
} psram_pin_t;

#define PIN_MAX_NUMBER                    PIN_NONE // value USED to iterate in arrays

#define TCXO_COUNT_ENABLE                 YES

#define WATCHDOG_ROM_ENABLE               NO

#define GPIO_WITH_ULP                     YES
#define GPIO_FUNC                         HAL_PIO_FUNC_GPIO
#define AON_SPECIAL_PIO                   NO

#define I2C_AUTO_SEND_STOP_CMD            NO
#define I2C_WITH_BUS_RECOVERY             YES

#define SPI_WITH_OPI                      YES
#define SPI_DMA_TRANSFER_NUM_BY_BYTE      NO

#define DMA_TRANSFER_DEBUG                YES
#define DMA_USE_HIDMA                     NO
#define DMA_WITH_MDMA                     YES // dma
#define DMA_WITH_SMDMA                    YES // Small dma
#define ADC_WITH_AUTO_SCAN                YES
#define DMA_TRANS_BY_LLI                  YES

#define XIP_WITH_OPI                      YES
#define XIP_INT_BY_NMI                    YES
#define FLASH_SLAVE_NOTIFY_MASTER_BOOTUP YES

#define ENABLE_CPU_TRACE                  3
#define TRACE_MEM_REGION_START            MCPU_TRACE_MEM_REGION_START
#define TRACE_MEM_REGION_LENGTH           CPU_TRACE_MEM_REGION_LENGTH

#define OTP_HAS_READ_PERMISSION           YES
#define OTP_HAS_WRITE_PERMISSION          YES
#define OTP_HAS_CLKLDO_VSET               NO

#define SEC_IAMGE_AES_DECRYPT_EN          NO
#define SEC_BOOT_SIGN_CHECK_EN            YES
#define SEC_SUB_RST_BY_SECURITY_CORE      NO
#define TRNG_WITH_SEC_COMMON              YES
#define IS_MAIN_CORE                      YES
#define EXTERNAL_CLOCK_CALIBRATION        NO

#define SUPPORT_HI_EMMC_PHY               NO

#define OPI_PIN_FIX_DM1_DRIVER            NO
#define OPI_USE_MCU_HS_CLK                NO
#define QSPI0_FUNC  HAL_PIO_FUNC_QSPI0
#define QSPI0_D0    S_EGPIO0
#define QSPI0_D1    S_EGPIO1
#define QSPI0_D2    S_EGPIO2
#define QSPI0_D3    S_EGPIO3
#define QSPI0_CLK   S_EGPIO4
#define QSPI0_CS    S_EGPIO5
#ifdef ATE_FLASH_CHECK
#define QSPI1_FUNC  HAL_PIO_FUNC_QSPI0
#define QSPI1_D0    S_MGPIO0
#define QSPI1_D1    S_MGPIO1
#define QSPI1_D2    S_MGPIO2
#define QSPI1_D3    S_MGPIO3
#define QSPI1_CLK   S_MGPIO4
#define QSPI1_CS    S_MGPIO5
#else
#define QSPI1_FUNC  HAL_PIO_FUNC_QSPI1
#define QSPI1_D0    S_MGPIO6
#define QSPI1_D1    S_MGPIO7
#define QSPI1_D2    S_MGPIO8
#define QSPI1_D3    S_MGPIO9
#define QSPI1_CLK   S_MGPIO10
#define QSPI1_CS    S_MGPIO11
#endif

#define XIP_EXIST                         YES
#define CHIP_FLASH_ID                     0
#define USE_DISPLAY_SUB                   YES
#define BCPU_INT0_ID                      16
#define USE_XIP_INDEX                     0

#define DSS_MIPI_DSI_TYPE_CFG             1
#define UART_BAUD_RATE_DIV_8              NO
#define FIXED_IN_ROM                      NO
#define USE_SDIOM_CLK_CONFIG_WAY          2
#define PMU_LPM_WAKEUP_SRC_NUM            16
#define ENABLE_GPIO_INTERRUPT             YES
#ifdef BUILD_APPLICATION_STANDARD
#define CLK_AUTO_CG_ENABLE                YES
#else
#define CLK_AUTO_CG_ENABLE                NO
#endif
#if defined(BUILD_APPLICATION_ROM)
#define BOOT_ROM_DFR_PRINT                YES
#else
#define BOOT_ROM_DFR_PRINT                NO
#endif
#define FLASH_WRITE_CLK_DIV_AUTO_ADJ     NO
#define DMA_WITH_MUX_CHANNEL              YES
#define CRITICAL_INT_RESTORE              YES
#define SEC_TRNG_ENABLE                  NO
#define DCACHE_IS_ENABLE                  NO
#define CODELOADER_SINGLE_PARTITION_EXP   YES

#ifdef PRE_ASIC
#define AUXLDO_ENABLE_FLASH               YES
#else
#define AUXLDO_ENABLE_FLASH               NO
#endif

#define CONFIG_MMC0_CCLK_MAX             32000000         // 32MHz
#define CONFIG_MMC1_CCLK_MAX             32000000         // 32MHz
#define SDIO_EMMC_DIV_NUM_MASK           0xFF0

// PSRAM RXDS delay configuration
#define PSRAM_RXDS_DELAY_ADJUST          YES
#define RXDS_SEL_REG_VAL                 0x401F // TT TTR value
#define RXDS_HIGH_SEL_REG_VAL            0xBC00
#if CHIP_FPGA
#define TCXO_CLK_DYN_ADJUST              YES
#else
#define TCXO_CLK_DYN_ADJUST              NO
#endif
#define SUPPORT_PARTITION_FEATURE        YES
#define SUPPORT_SINGLE_DSP_DUAL_IMAGE    YES

#define NAND_QSPI_BASE_ADDR 0x52065000
/**
 * @}
 */
#endif
