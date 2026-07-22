/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Provides i2c port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-15， Create file. \n
 */
#ifndef I2C_PORTING_H
#define I2C_PORTING_H

#include <stdint.h>
#include <stdbool.h>
#include "platform_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CONFIG_I2C_SUPPORT_MASTER 1
#define CONFIG_I2C_SUPPORT_SLAVE 1

#ifdef TEST_SUITE
#define CONFIG_I2C_WAIT_CONDITION_TIMEOUT 0xFFFFFFFFULL
#else
#define CONFIG_I2C_WAIT_CONDITION_TIMEOUT 0x3ULL
#endif

#define I2C_BUS_MAX_NUM I2C_BUS_MAX_NUMBER

#define I2C_0_SCL_PIN                 S_MGPIO11
#define I2C_0_SDA_PIN                 S_MGPIO12
#define I2C_3_SCL_PIN                 S_MGPIO17
#define I2C_3_SDA_PIN                 S_MGPIO18

#define I2C_DMA_TIMEOUT_MS        100

typedef struct {
    volatile uint32_t con;              /*!< I2c control register. <i>Offset: 00h</i>. */
    volatile uint32_t tar;              /*!< I2c target address register. <i>Offset: 04h</i>. */
    volatile uint32_t sar;              /*!< I2c slave address register. <i>Offset: 08h</i>. */
    volatile uint32_t hs_maddr;         /*!< I2c high speed master mode code address register. <i>Offset: 0Ch</i>. */
    volatile uint32_t ss_scl_hcnt;      /*!< I2c standard speed clock scl high count register. <i>Offset: 14h</i>. */
    volatile uint32_t ss_scl_lcnt;      /*!< I2c standard speed clock scl low count register. <i>Offset: 18h</i>. */
    volatile uint32_t fs_scl_hcnt;      /*!< I2c fast speed clock scl high count register. <i>Offset: 1ch</i>. */
    volatile uint32_t fs_scl_lcnt;      /*!< I2c fast speed clock scl low count register. <i>Offset: 20h</i>. */
    volatile uint32_t hs_scl_hcnt;      /*!< I2c high speed clock scl high count register. <i>Offset: 24h</i>. */
    volatile uint32_t hs_scl_lcnt;      /*!< I2c high speed clock scl low count register. <i>Offset: 28h</i>. */
    volatile uint32_t intr_mask;        /*!< I2c interrupt mask register. <i>Offset: 30h</i>. */
    volatile uint32_t rx_tl;            /*!< I2c receive fifo threshold register. <i>Offset: 38h</i>. */
    volatile uint32_t tx_tl;            /*!< I2c Transmit fifo threshold register. <i>Offset: 3ch</i>. */
    volatile uint32_t enable;           /*!< I2c enable register. <i>Offset: 6ch</i>. */
    volatile uint32_t sda_hold;         /*!< I2c sda hold time length register. <i>Offset: 7ch</i>. */
    volatile uint32_t slave_data_nack_only; /*!< I2c generate slave data nack register. <i>Offset: 84h</i>. */
    volatile uint32_t dma_cr;           /*!< I2c dma control register. <i>Offset: 88h</i>. */
    volatile uint32_t dma_tdlr;         /*!< I2c dma transmit data level register. <i>Offset: 8ch</i>. */
    volatile uint32_t dma_rdlr;         /*!< I2c dma receive data level register. <i>Offset: 90h</i>. */
    volatile uint32_t sda_setup;        /*!< I2c sda setup register. <i>Offset: 94h</i>. */
    volatile uint32_t ack_general_call; /*!< I2c ack general call register. <i>Offset: 98h</i>. */
} i2c_save_reg_t;

/**
 * @brief  Get i2c base address.
 */
uintptr_t i2c_porting_base_addr_get(i2c_bus_t bus);

/**
 * @brief  Register hal funcs objects into hal_i2c module.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t
 */
void i2c_port_register_hal_funcs(i2c_bus_t bus);

/**
 * @brief  Unregister hal funcs objects from hal_i2c module.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t
 */
void i2c_port_unregister_hal_funcs(i2c_bus_t bus);

/**
 * @brief  Get the bus clock of specified i2c.
 * @param  [in]  bus The I2C bus. see @ref i2c_bus_t
 * @return The bus clock of specified I2C.
 */
uint32_t i2c_port_get_clock_value(i2c_bus_t bus);

/**
 * @brief  Register the interrupt of I2C.
 */
void i2c_port_register_irq(i2c_bus_t bus);

/**
 * @brief  Unregister the interrupt of I2C.
 */
void i2c_port_unregister_irq(i2c_bus_t bus);

/**
 * @brief  I2C lock.
 * @param [in]  bus The bus index of I2C.
 * @return The irq lock number of I2C.
 */
uint32_t i2c_porting_lock(i2c_bus_t bus);

/**
 * @brief  I2C unlock.
 * @param [in]  bus The bus index of I2C.
 * @param [in]  irq_sts The irq lock number of I2C.
 */
void i2c_porting_unlock(i2c_bus_t bus, uint32_t irq_sts);

/**
 * @brief  Save I2C register for suspend.
 * @param [in]  bus The bus index of I2C.
 */
void i2c_save_reg(i2c_bus_t bus);

/**
 * @brief  Save I2C register for resume.
 * @param [in]  bus The bus index of I2C.
 */
void i2c_recovery_reg(i2c_bus_t bus);

#ifdef TEST_SUITE
/**
 * @brief  Init i2c pin for test.
 */
void i2c_port_test_i2c_init_pin(void);
#endif

#if defined(CONFIG_I2C_SUPPORT_DMA)
uint8_t i2c_port_get_dma_trans_dest_handshaking(i2c_bus_t bus);

uint8_t i2c_port_get_dma_trans_src_handshaking(i2c_bus_t bus);
#endif  /* CONFIG_I2C_SUPPORT_DMA */

#if defined(CONFIG_I2C_SUPPORT_LPC)
void i2c_port_clock_enable(i2c_bus_t bus, bool on);
#endif

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
