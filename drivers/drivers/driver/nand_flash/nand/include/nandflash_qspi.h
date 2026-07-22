/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_QSPI
#define NANDFLASH_QSPI
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SWAP_U16(x) ((uint16_t)((((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
                    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))
#define SWAP_U32(x) ((uint32_t)((((uint32_t)(x) & (uint32_t)0x000000ffU) << 24) | \
                    (((uint32_t)(x) & (uint16_t)0x0000ff00U) << 8) | \
                    (((uint32_t)(x) & (uint32_t)0x00ff0000U) >> 8) | \
                    (((uint32_t)(x) & (uint32_t)0xff000000U) >> 24)))
#define CTRLR0 (NAND_QSPI_BASE_ADDR + 0x0)
#define CTRLR1 (NAND_QSPI_BASE_ADDR + 0x4)
#define SSIENR (NAND_QSPI_BASE_ADDR + 0x8)
#define MWCR (NAND_QSPI_BASE_ADDR + 0xc)
#define SER (NAND_QSPI_BASE_ADDR + 0x10)
#define BAUDR (NAND_QSPI_BASE_ADDR + 0x14)
#define TXFTLR (NAND_QSPI_BASE_ADDR + 0x18)
#define RXFTLR (NAND_QSPI_BASE_ADDR + 0x1c)
#define TXFLR (NAND_QSPI_BASE_ADDR + 0x20)
#define RXFLR (NAND_QSPI_BASE_ADDR + 0x24)
#define SR (NAND_QSPI_BASE_ADDR + 0x28)
#define IMR (NAND_QSPI_BASE_ADDR + 0x2c)
#define ISR (NAND_QSPI_BASE_ADDR + 0x30)
#define RISR (NAND_QSPI_BASE_ADDR + 0x34)
#define TXOICR (NAND_QSPI_BASE_ADDR + 0x38)
#define RXOICR (NAND_QSPI_BASE_ADDR + 0x3c)
#define RXUICR (NAND_QSPI_BASE_ADDR + 0x40)
#define MSTICR (NAND_QSPI_BASE_ADDR + 0x44)
#define ICR (NAND_QSPI_BASE_ADDR + 0x48)
#define DMACR (NAND_QSPI_BASE_ADDR + 0x4c)
#define DMATDLR (NAND_QSPI_BASE_ADDR + 0x50)
#define DMARDLR (NAND_QSPI_BASE_ADDR + 0x54)
#define IDR (NAND_QSPI_BASE_ADDR + 0x58)
#define SSI_VERSION_ID (NAND_QSPI_BASE_ADDR + 0x5c)
#define DR (NAND_QSPI_BASE_ADDR + 0x60)
#define RX_SAMPLE_DLY (NAND_QSPI_BASE_ADDR + 0xf0)
#define SPI_CTRLR0 (NAND_QSPI_BASE_ADDR + 0xf4)
#define TXD_DRIVE_EDGE (NAND_QSPI_BASE_ADDR + 0xf8)
#define TRANS_NOT_BUSY 0x4
#define FIFO_EMPTY 0x8
#define QSPI_BUFY 0x1
#define FIFO_FULL 0x2
#define SEND_TIMEOUT 512
#define RECV_TIMEOUT 512

void nand_qspi_disable(void);
void nand_qspi_ctlr0_cfg(uint32_t cfg);
void nand_qspi_ctlr1_cfg(uint32_t cfg);
void nand_qspi_enable(void);
void nand_qspi_write_data(uint32_t data);
uint32_t nand_qspi_get_state(void);
uint32_t nand_qspi_read_data(void);
bool is_trans_busy(void);
bool is_rx_fifo_not_empty(void);
bool is_qspi_busy(void);
bool is_fifo_full(void);
void qspi_read_width_8(uint8_t *buffer, uint32_t length);
void qspi_read_width_32(uint32_t *recvbuf, uint32_t length);
void qspi_busy_wait(void);
void nand_qspi_spi_ctlr0_cfg(uint32_t val);
#endif