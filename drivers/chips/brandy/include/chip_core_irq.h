/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:   Chip core irq >= LOCAL_INTERRUPT0 + 0 define.
 * Author: @CompanyNameTag
 * Create:  2021-04-21
 */

#ifndef CHIP_CORE_IRQ_H
#define CHIP_CORE_IRQ_H

#define LOCAL_INTERRUPT0 16

typedef enum core_irq {
/* -------------------  Processor Interrupt Numbers  ------------------------------ */
    BT_INT0_IRQN                         = LOCAL_INTERRUPT0 + 0,
    BT_INT1_IRQN                         = LOCAL_INTERRUPT0 + 1,
    DSP0_INT0_IRQn                       = LOCAL_INTERRUPT0 + 2,
    DSP0_INT1_IRQn                       = LOCAL_INTERRUPT0 + 3,
    DSP1_INT0_IRQn                       = LOCAL_INTERRUPT0 + 4,
    DSP1_INT1_IRQn                       = LOCAL_INTERRUPT0 + 5,
    MCPU_PCLR_OK_IRQn                    = LOCAL_INTERRUPT0 + 6,
    ULP_GPIO_IRQN                        = LOCAL_INTERRUPT0 + 7,
    GPIO_0_IRQN                          = LOCAL_INTERRUPT0 + 8,
    GPIO_1_IRQN                          = LOCAL_INTERRUPT0 + 9,
    GPIO_2_IRQn                          = LOCAL_INTERRUPT0 + 10,
    GPIO_3_IRQn                          = LOCAL_INTERRUPT0 + 11,
    GPIO_4_IRQn                          = LOCAL_INTERRUPT0 + 12,
    ULP_TO_MCU_IRQn                      = LOCAL_INTERRUPT0 + 13,
    RESERVED_14_IRQn                     = LOCAL_INTERRUPT0 + 14,
    UART_1_IRQN                          = LOCAL_INTERRUPT0 + 15,
    UART_0_IRQN                          = LOCAL_INTERRUPT0 + 16,      // uart bus0 is uart1
    MEM2MCU_QSPI0_2CS_IRQN               = LOCAL_INTERRUPT0 + 17,
    QSPI1_CS_IRQn                        = LOCAL_INTERRUPT0 + 18,
    QSPI2_CS_IRQn                        = LOCAL_INTERRUPT0 + 19,
    OPI_INT_IRQn                         = LOCAL_INTERRUPT0 + 20,
    M_WAKEUP_IRQN                        = LOCAL_INTERRUPT0 + 21,
    M_SLEEP_IRQN                         = LOCAL_INTERRUPT0 + 22,
    RTC_1_IRQN                           = LOCAL_INTERRUPT0 + 23,
    RTC_2_IRQN                           = LOCAL_INTERRUPT0 + 24,
    RTC_3_IRQN                           = LOCAL_INTERRUPT0 + 25,
    RTC_0_IRQN                           = LOCAL_INTERRUPT0 + 26,
    TIMER_0_IRQN                         = LOCAL_INTERRUPT0 + 27,
    TIMER_1_IRQN                         = LOCAL_INTERRUPT0 + 28,
    TIMER_2_IRQN                         = LOCAL_INTERRUPT0 + 29,
    TIMER_3_IRQN                         = LOCAL_INTERRUPT0 + 30,
    M_SDMA_IRQN                          = LOCAL_INTERRUPT0 + 31,
    M_DMA_IRQN                           = LOCAL_INTERRUPT0 + 32,
    SPI_M_IRQN                           = LOCAL_INTERRUPT0 + 33,
    SPI_M_S_0_IRQN                       = LOCAL_INTERRUPT0 + 34,
    SPI_M_S_1_IRQN                       = LOCAL_INTERRUPT0 + 35,
    I2C_0_IRQN                           = LOCAL_INTERRUPT0 + 36,
    I2C_1_IRQN                           = LOCAL_INTERRUPT0 + 37,
    I2C_2_IRQN                           = LOCAL_INTERRUPT0 + 38,
    I2C_3_IRQN                           = LOCAL_INTERRUPT0 + 39,
    MAD_IRQn                             = LOCAL_INTERRUPT0 + 40,
    VAD_MERGE_IRQn                       = LOCAL_INTERRUPT0 + 41,
    NUM_INTERRUPT_GMMU                   = LOCAL_INTERRUPT0 + 42,
    RESERVED_43_IRQN                     = LOCAL_INTERRUPT0 + 43,
    SEC_INT_IRQN                         = LOCAL_INTERRUPT0 + 44,
    PWM_0_IRQN                           = LOCAL_INTERRUPT0 + 45,
    PWM_1_IRQN                           = LOCAL_INTERRUPT0 + 46,
    PWM_2_IRQN                           = LOCAL_INTERRUPT0 + 47,
    PWM_3_IRQN                           = LOCAL_INTERRUPT0 + 48,
    PWM_4_IRQN                           = LOCAL_INTERRUPT0 + 49,
    PWM_5_IRQN                           = LOCAL_INTERRUPT0 + 50,
    RESERVED_51_IRQn                     = LOCAL_INTERRUPT0 + 51,
    PMU_CMU_ERR_IRQN                     = LOCAL_INTERRUPT0 + 52,
    RESERVED_53_IRQn                     = LOCAL_INTERRUPT0 + 53,
    ADC_IRQn                             = LOCAL_INTERRUPT0 + 54,
    MEM_SUB_MONITOR_IRQn                 = LOCAL_INTERRUPT0 + 55,
    B_SUB_MONITOR_IRQn                   = LOCAL_INTERRUPT0 + 56,
    COMRAM_MONITOR_IRQN                  = LOCAL_INTERRUPT0 + 57,
    RESERVED_58_IRQn                     = LOCAL_INTERRUPT0 + 58,
    RESERVED_59_IRQn                     = LOCAL_INTERRUPT0 + 59,
    SDIO2MCU_SDIO_H_IRQn                 = LOCAL_INTERRUPT0 + 60,
    SDIO2MCU_SDIO_H_WAKEUP_IRQn          = LOCAL_INTERRUPT0 + 61,
    EMMC_IRQn                            = LOCAL_INTERRUPT0 + 62,
    SDIO2MCU_EMMC_WAKEUP_IRQn            = LOCAL_INTERRUPT0 + 63,
    RESERVED_64_IRQn                     = LOCAL_INTERRUPT0 + 64,
    EH2H_INT_IRQn                        = LOCAL_INTERRUPT0 + 65,
    NUM_INTERRUPT_DPU                    = LOCAL_INTERRUPT0 + 66,
    NUM_INTERRUPT_VAU                    = LOCAL_INTERRUPT0 + 67,
    NUM_INTERRUPT_MIPI                   = LOCAL_INTERRUPT0 + 68,
    NUM_INTERRUPT_SENSOR_HUB_DPU1_TE     = LOCAL_INTERRUPT0 + 69,
    NUM_INTERRUPT_JPEG                   = LOCAL_INTERRUPT0 + 70,
    RESERVED_71_IRQn                     = LOCAL_INTERRUPT0 + 71,
    DSP0_ADDR_MON_IRQn                   = LOCAL_INTERRUPT0 + 72,
    RESERVED_73_IRQn                     = LOCAL_INTERRUPT0 + 73,
    DSP_WDT_IRQn                         = LOCAL_INTERRUPT0 + 74,
    D2MCU_DSP0_WDOG1_INT_IRQn            = LOCAL_INTERRUPT0 + 75,
    RESERVED_76_IRQn                     = LOCAL_INTERRUPT0 + 76,
    RESERVED_77_IRQn                     = LOCAL_INTERRUPT0 + 77,
    RESERVED_78_IRQn                     = LOCAL_INTERRUPT0 + 78,
    RESERVED_79_IRQn                     = LOCAL_INTERRUPT0 + 79,
    RESERVED_80_IRQn                     = LOCAL_INTERRUPT0 + 80,

    BUTT_IRQN
} core_irq_t;

#endif
