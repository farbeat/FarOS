/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides dma port \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-16， Create file. \n
 */

#ifndef DMA_PORTING_H
#define DMA_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "dma.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_dma DMA
 * @ingroup  drivers_port
 * @{
 */

#define S_DMA_CHANNEL_MAX_NUM    4  /*!< Max number of SM_DMA available */
#define B_DMA_CHANNEL_MAX_NUM    8  /*!< Max number of M_DMA available */

#define DMA_CHANNEL_MAX_NUM      (S_DMA_CHANNEL_MAX_NUM + B_DMA_CHANNEL_MAX_NUM)
#define ERRCODE_DMA_MEMORY_ALLOC_ERROR              0x80001111

/**
 * @brief  DMA channel ID.
 */
typedef enum {
    DMA_CHANNEL_RESERVED_0,    /*!< DMA channel 0 */
    DMA_CHANNEL_RESERVED_1,    /*!< DMA channel 1 */
    DMA_CHANNEL_0,    /*!< DMA channel 2 */
    DMA_CHANNEL_1,    /*!< DMA channel 3 */
    DMA_CHANNEL_2,    /*!< DMA channel 4 */
    DMA_CHANNEL_3,    /*!< DMA channel 5 */
    DMA_CHANNEL_4,    /*!< DMA channel 6 */
    DMA_CHANNEL_5,    /*!< DMA channel 7 */
    DMA_CHANNEL_8,    /*!< DMA channel 8 */
    DMA_CHANNEL_9,    /*!< DMA channel 9 */
    DMA_CHANNEL_10,   /*!< DMA channel 10 */
    DMA_CHANNEL_11,   /*!< DMA channel 11 */
    DMA_CHANNEL_NONE = DMA_CHANNEL_MAX_NUM
} dma_channel_t;

/**
 * @brief  DMA handshaking source select.
 */
typedef enum {
    /* MDMA */
    HAL_DMA_HANDSHAKING_SPI_MS0_TX,
    HAL_DMA_HANDSHAKING_SPI_MS0_RX,
    HAL_DMA_HANDSHAKING_SPI_MS1_TX,
    HAL_DMA_HANDSHAKING_SPI_MS1_RX,
    HAL_DMA_HANDSHAKING_SPI_M_TX,
    HAL_DMA_HANDSHAKING_SPI_M_RX,
    HAL_DMA_HANDSHAKING_OPI_TX,
    HAL_DMA_HANDSHAKING_OPI_RX,
    HAL_DMA_HANDSHAKING_QSPI0_2CS_TX,
    HAL_DMA_HANDSHAKING_QSPI0_2CS_RX,
    HAL_DMA_HANDSHAKING_QSPI1_2CS_TX,
    HAL_DMA_HANDSHAKING_QSPI1_2CS_RX,
    HAL_DMA_HANDSHAKING_QSPI2_1CS_TX,
    HAL_DMA_HANDSHAKING_QSPI2_1CS_RX,
    HAL_DMA_HANDSHAKING_SPI3_M_TX,
    HAL_DMA_HANDSHAKING_SPI3_M_RX,
    HAL_DMA_HANDSHAKING_QSPI3_1CS_TX,
    HAL_DMA_HANDSHAKING_QSPI3_1CS_RX,
    HAL_MDMA_HANDSHAKING_MAX_NUM,
    /* SMDMA */
    HAL_DMA_HANDSHAKING_SDMA = HAL_MDMA_HANDSHAKING_MAX_NUM,
    HAL_DMA_HANDSHAKING_I2C0_TX = HAL_DMA_HANDSHAKING_SDMA + 0,
    HAL_DMA_HANDSHAKING_I2C0_RX,
    HAL_DMA_HANDSHAKING_I2C1_TX,
    HAL_DMA_HANDSHAKING_I2C1_RX,
    HAL_DMA_HANDSHAKING_I2C2_TX,
    HAL_DMA_HANDSHAKING_I2C2_RX,
    HAL_DMA_HANDSHAKING_I2C3_TX,
    HAL_DMA_HANDSHAKING_I2C3_RX,
    HAL_DMA_HANDSHAKING_UART_L_TX,
    HAL_DMA_HANDSHAKING_UART_L_RX,
    HAL_DMA_HANDSHAKING_UART_H0_TX,
    HAL_DMA_HANDSHAKING_UART_H0_RX,
    HAL_DMA_HANDSHAKING_UART_H1_TX,
    HAL_DMA_HANDSHAKING_UART_H1_RX,
    HAL_DMA_HANDSHAKING_IR_TX,
    HAL_DMA_HANDSHAKING_IR_RX,
    HAL_DMA_HANDSHAKING_I2C4_TX,
    HAL_DMA_HANDSHAKING_I2C4_RX,
    HAL_DMA_HANDSHAKING_I2C5_TX,
    HAL_DMA_HANDSHAKING_I2C5_RX,
    HAL_DMA_HANDSHAKING_I2C6_TX,
    HAL_DMA_HANDSHAKING_I2C6_RX,
    HAL_DMA_HANDSHAKING_MAX_NUM
} hal_dma_handshaking_source_t;

/**
 * @brief  DMA Hardshaking channel.
 */
typedef enum hal_dma_hardshaking_channel {
    HAL_DMA_HARDSHAKING_CHANNEL_0,
    HAL_DMA_HARDSHAKING_CHANNEL_1,
    HAL_DMA_HARDSHAKING_CHANNEL_2,
    HAL_DMA_HARDSHAKING_CHANNEL_3,
    HAL_DMA_HARDSHAKING_CHANNEL_4,
    HAL_DMA_HARDSHAKING_CHANNEL_5,
    HAL_DMA_HARDSHAKING_CHANNEL_6,
    HAL_DMA_HARDSHAKING_CHANNEL_7,
    HAL_DMA_HARDSHAKING_CHANNEL_MAX_NUM,
    HAL_DMA_HARDSHAKING_CHANNEL_NONE = HAL_DMA_HARDSHAKING_CHANNEL_MAX_NUM
} hal_dma_hardshaking_channel_t;

extern uintptr_t g_dma_base_addr;
extern uintptr_t g_sdma_base_addr;

/**
 * @brief  Register hal funcs objects into hal_dma module.
 */
void dma_port_register_hal_funcs(void);

/**
 * @brief  Unregister hal funcs objects from hal_dma module.
 */
void dma_port_unregister_hal_funcs(void);

/**
 * @brief  Register the interrupt of dma.
 */
void dma_port_register_irq(void);

/**
 * @brief  Unregister the interrupt of dma.
 */
void dma_port_unregister_irq(void);

/**
 * @brief  Set the channel status of handshaking.
 * @param  [in]  ch  The handshaking select. For details, see @ref hal_dma_handshaking_source_t
 * @param  [in]  on  Set or clear
 */
void dma_port_set_handshaking_channel_status(hal_dma_handshaking_source_t ch, bool on);

/**
 * @brief  add sleep veto before dma transfer.
 */
void dma_port_add_sleep_veto(void);

/**
 * @brief  remove sleep veto after dma transfer.
 */
void dma_port_remove_sleep_veto(void);

/**
 * @brief  dma suspend when system comes into deep sleep.
 */
void dma_suspend(void);

/**
 * @brief  dma resume when system wakes up from deep sleep.
 */
void dma_resume(void);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
