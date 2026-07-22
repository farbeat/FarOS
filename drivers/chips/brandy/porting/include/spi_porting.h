/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides spi porting template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-18， Create file. \n
 */
#ifndef SPI_PORTING_H
#define SPI_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"
#include "dma_porting.h"
#include "hal_spi_v100_regs_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_spi SPI
 * @ingroup  drivers_port
 * @{
 */

#define SPI_BUS_MAX_NUM SPI_BUS_MAX_NUMBER
#define CONFIG_SPI_MAX_TIMEOUT 0xFFFFFFFF

#define CONFIG_SPI_SUPPORT_MASTER  1
#define CONFIG_SPI_SUPPORT_SLAVE  1
#define CONFIG_SPI_SUPPORT_LOOPBACK  1
#define CONFIG_SPI_SUPPORT_CRC  1
#define CONFIG_SPI_TXFTLR  1
#define CONFIG_SPI_RXFTLR  1

#define SPI_DMA_TX_DATA_LEVEL_4     4
#define QSPI_DMA_TX_DATA_LEVEL_8    8
#define DMA_HANDSHAKE_SPI_BUS_0_TX HAL_DMA_HANDSHAKING_SPI_M_TX
#define DMA_HANDSHAKE_SPI_BUS_0_RX HAL_DMA_HANDSHAKING_SPI_M_RX
#define DMA_HANDSHAKE_SPI_BUS_1_TX HAL_DMA_HANDSHAKING_SPI_MS0_TX
#define DMA_HANDSHAKE_SPI_BUS_1_RX HAL_DMA_HANDSHAKING_SPI_MS0_RX
#define DMA_HANDSHAKE_SPI_BUS_2_TX HAL_DMA_HANDSHAKING_SPI_MS1_TX
#define DMA_HANDSHAKE_SPI_BUS_2_RX HAL_DMA_HANDSHAKING_SPI_MS1_RX
#define DMA_HANDSHAKE_SPI_BUS_3_TX HAL_DMA_HANDSHAKING_QSPI0_2CS_TX
#define DMA_HANDSHAKE_SPI_BUS_3_RX HAL_DMA_HANDSHAKING_QSPI0_2CS_RX
#define DMA_HANDSHAKE_SPI_BUS_4_TX HAL_DMA_HANDSHAKING_QSPI1_2CS_TX
#define DMA_HANDSHAKE_SPI_BUS_4_RX HAL_DMA_HANDSHAKING_QSPI1_2CS_RX
#define DMA_HANDSHAKE_SPI_BUS_5_TX HAL_DMA_HANDSHAKING_QSPI2_1CS_TX
#define DMA_HANDSHAKE_SPI_BUS_5_RX HAL_DMA_HANDSHAKING_QSPI2_1CS_RX
#define DMA_HANDSHAKE_SPI_BUS_6_TX HAL_DMA_HANDSHAKING_OPI_TX
#define DMA_HANDSHAKE_SPI_BUS_6_RX HAL_DMA_HANDSHAKING_OPI_RX

/**
 * @brief  Spi dma control register.
 */
typedef enum {
    HAL_SPI_DMA_CONTROL_DISABLE = 0,        //!< Disables the transmit fifo and the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_RX_ENABLE = 1,      //!< Enables the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_TX_ENABLE = 2,      //!< Enables the transmit fifo dma channel.
    HAL_SPI_DMA_CONTROL_TXRX_ENABLE = 3,    //!< Enables the transmit fifo and the receive fifo dma channel.
    HAL_SPI_DMA_CONTROL_MAX_NUM,
    HAL_SPI_DMA_CONTROL_NONE = HAL_SPI_DMA_CONTROL_MAX_NUM,
} hal_spi_dma_control_t;

/**
 * @brief  SPI mode.
 */
typedef enum spi_mode {
    SPI_MODE_MASTER,        /*!< SPI Master mode. */
    SPI_MODE_SLAVE,         /*!< SPI Slave mode. */
    SPI_MODE_MAX_NUM,
    SPI_MODE_NONE = SPI_MODE_MAX_NUM
} spi_mode_t;

/**
 * @brief  SPI slave select.
 */
typedef enum spi_slave {
    SPI_SLAVE0 = 0,         /*!< SPI Slave index 0. */
    SPI_SLAVE1,             /*!< SPI Slave index 1. */
    SPI_SLAVE2,             /*!< SPI Slave index 2. */
    SPI_SLAVE_MAX_NUM,
    SPI_SLAVE_NONE = SPI_SLAVE_MAX_NUM
} spi_slave_t;

typedef struct spi_recovery_reg_s {
    volatile uint32_t ctrlr0; // offset0x0
    volatile uint32_t ctrlr1; // Offset: 04h
    volatile uint32_t ser;    // Offset: 10h
    volatile uint32_t baudr;  // Offset: 14h
    volatile uint32_t txftlr; // Offset: 18h
    volatile uint32_t rxftlr; // Offset: 1Ch
    volatile uint32_t txflr;  // Offset: 20h
    volatile uint32_t rxflr;  // Offset: 24h
    volatile uint32_t imr;    // Offset: 2Ch
    volatile uint32_t dmacr;  // Offset: 4Ch
    volatile uint32_t dmatdlr;  // Offset: 50h
    volatile uint32_t dmardlr;  // Offset: 54h
    volatile uint32_t rx_sample_dly; // Offset: F0h
    volatile uint32_t spi_ctrlr0;    // Offset: F4h
} spi_recovery_cfg_t;

/**
 * @brief  Base address list for all of the IPs.
 */
extern ssi_regs_t * g_spi_base_addrs[SPI_BUS_MAX_NUM];

/**
 * @brief Register hal funcs objects into hal_spi module.
 */
void spi_port_register_hal_funcs(void);

/**
 * @brief  Get the base address of specified spi.
 * @param  [in]  bus The bus index of SPI.
 * @return The base address of specified spi.
 */
uintptr_t spi_porting_base_addr_get(spi_bus_t bus);

/**
 * @brief  Get the max slave number can be selected.
 * @param  [in]  bus The bus index of SPI.
 * @return The  max slave number can be selected.
 */
uint32_t spi_porting_max_slave_select_get(spi_bus_t bus);

/**
 * @brief  Set the spi work mode.
 * @param  [in]  bus The bus index of SPI.
 * @param  [in]  mode The mode of SPI.
 */
void spi_porting_set_device_mode(spi_bus_t bus, spi_mode_t mode);

/**
 * @brief  Get the spi work mode.
 * @param  [in]  bus The bus index of SPI.
 * @return The mode of SPI.
 */
spi_mode_t spi_porting_get_device_mode(spi_bus_t bus);

/**
 * @brief  SPI lock.
 * @param [in]  bus The bus index of SPI.
 * @return The irq lock number of SPI.
 */
uint32_t spi_porting_lock(spi_bus_t bus);

/**
 * @brief  SPI unlock.
 * @param [in]  bus The bus index of SPI.
 * @param [in]  irq_sts The irq lock number of SPI.
 */
void spi_porting_unlock(spi_bus_t bus, uint32_t irq_sts);

/**
 * @brief  SPI clock enable or disable.
 * @param [in]  bus The bus index of I2C.
 * @param [in]  on Enable or disable.
 */
void spi_port_clock_enable(spi_bus_t bus, bool on);

/**
 * @brief  flash save registers.
 * @param [in]  bus The bus index of SPI.
 * @return none.
 */
void spi_save_reg(spi_bus_t bus);

/**
 * @brief  flash recovery registers.
 * @param [in]  bus The bus index of SPI.
 * @return none.
 */
void spi_recovery_reg(spi_bus_t bus);

#ifdef TEST_SUITE
/**
 * @brief  Init spi pin for test.
 */
void spi_porting_test_spi_init_pin(void);
#endif

uint8_t spi_port_tx_data_level_get(spi_bus_t bus);

uint8_t spi_port_rx_data_level_get(spi_bus_t bus);

uint8_t spi_port_get_dma_trans_dest_handshaking(spi_bus_t bus);

uint8_t spi_port_get_dma_trans_src_handshaking(spi_bus_t bus);

/* for opi stub */
bool hal_opi_set_fifo_threshold(spi_bus_t bus, uint32_t threshold);
bool hal_opi_reset_config(spi_bus_t bus);
bool hal_opi_set_frame_size(spi_bus_t bus, uint32_t frame_size);
bool hal_opi_set_frame_format(spi_bus_t bus, uint8_t frame_format);
bool hal_opi_set_trans_mode(spi_bus_t bus, uint32_t tmod);
bool hal_opi_set_freq(spi_bus_t bus, uint32_t clk_in_mhz);
void hal_opi_set_inst_len(spi_bus_t bus, uint8_t inst_len);
void hal_opi_set_addr_len(spi_bus_t bus, uint8_t addr_len);
void hal_opi_cmd_trans_mode(spi_bus_t bus, uint32_t mode);
bool hal_opi_set_received_data_num(spi_bus_t bus, uint32_t number);
bool hal_opi_set_tx_fifo_threshold(spi_bus_t bus, uint32_t threshold);
void hal_opi_ddr_en(spi_bus_t bus, bool on);
bool hal_qspi_set_wait_cycles(spi_bus_t bus, uint32_t wait_cyc);
bool hal_opi_set_trans_type(spi_bus_t bus, uint8_t trans_type);
void hal_opi_disable_slave(spi_bus_t bus);
uint32_t hal_opi_read_data(spi_bus_t bus);
bool hal_opi_write_data(spi_bus_t bus, uint32_t data);
bool hal_opi_select_slave(spi_bus_t bus, uint32_t slave_num);
bool hal_opi_disable(spi_bus_t bus);
bool hal_opi_enable(spi_bus_t bus);
bool hal_opi_tx_fifo_is_not_full(spi_bus_t bus);
bool hal_opi_tx_fifo_is_empty(spi_bus_t bus);
bool hal_opi_rx_fifo_is_full(spi_bus_t bus);
bool hal_opi_rx_fifo_is_not_empty(spi_bus_t bus);
bool hal_opi_is_busy(spi_bus_t bus);
bool hal_opi_set_rx_sample_dly(spi_bus_t bus, uint8_t delay);
bool hal_opi_set_dma_rx_data_level(spi_bus_t bus, uint32_t data_level);
bool hal_opi_dma_control(spi_bus_t bus, hal_spi_dma_control_t operation);
void spi_funcreg_adapt(spi_bus_t bus);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif