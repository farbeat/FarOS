/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "nandflash_qspi.h"
#include "chip_io.h"
#include "debug_print.h"
#include "hal_nandflash.h"

void nand_qspi_disable(void)
{
    writel(SSIENR, 0x0);
}

void nand_qspi_ctlr0_cfg(uint32_t cfg)
{
    writel(CTRLR0, cfg);
}

void nand_qspi_ctlr1_cfg(uint32_t cfg)
{
    writel(CTRLR1, cfg);
}

void nand_qspi_enable(void)
{
    writel(SSIENR, 0x1);
}

void nand_qspi_write_data(uint32_t data)
{
    writel(DR, data);
}

uint32_t nand_qspi_read_data(void)
{
    return ((uint32_t)readl(DR));
}

uint32_t nand_qspi_get_state(void)
{
    return ((uint32_t)readl(SR));
}

bool is_trans_busy(void)
{
    return !((nand_qspi_get_state() & TRANS_NOT_BUSY) == TRANS_NOT_BUSY);
}

bool is_rx_fifo_not_empty(void)
{
    return ((nand_qspi_get_state() & FIFO_EMPTY) == FIFO_EMPTY);
}

bool is_qspi_busy(void)
{
    return ((nand_qspi_get_state() & QSPI_BUFY) == QSPI_BUFY);
}

bool is_fifo_full(void)
{
    return ((nand_qspi_get_state() & FIFO_FULL) == 0);
}

void qspi_read_width_8(uint8_t *buffer, uint32_t length)
{
    uint32_t ul_loop;
    uint32_t ul_recv_timeout;
    uint32_t ul_recv_num;
    for (ul_loop = 0, ul_recv_timeout = 0; ul_loop < length;) {
        if (((nand_qspi_get_state() & 0x10) == 0x10)) {
            PRINT("qspi read fail because rx fifo is full, readLength=%d !!\r\n", length);
            return;
        }
        while (((readl(SR)) & 0x8) == 0x8) {
            if (ul_loop < length) {
                buffer[ul_loop++] = ((uint8_t)readl(DR));
            } else {
                (void)readl(DR);
            }
            ul_recv_timeout = 0;
        }
        ul_recv_timeout++;
        if (ul_recv_timeout >= RECV_TIMEOUT) {
            PRINT("qspi read timeout!!! \r\n");
            return;
        }
    }
    return;
}

void qspi_read_width_32(uint32_t *recvbuf, uint32_t length)
{
    uint32_t ul_loop;
    uint32_t ul_recv_timeout;
    uint32_t ul_recv_num;
    for (ul_loop = 0, ul_recv_timeout = 0; ul_loop < length;) {
        if (((nand_qspi_get_state() & 0x10) == 0x10)) {
            PRINT("qspi read fail because rx fifo is full, readLength=%d !!\r\n", length);
            return;
        }
        while (((readl(SR)) & 0x8) == 0x8) {
            if (ul_loop < length) {
                recvbuf[ul_loop++] = ((uint32_t)readl(DR));
            } else {
                (void)readl(DR);
            }
            ul_recv_timeout = 0;
        }
        ul_recv_timeout++;
        if (ul_recv_timeout >= RECV_TIMEOUT) {
            PRINT("qspi read timeout!!! \r\n");
            return;
        }
    }
    return;
}

void nand_qspi_spi_ctlr0_cfg(uint32_t val)
{
    writel(SPI_CTRLR0, val);
}

void qspi_busy_wait(void)
{
    while (is_trans_busy() || is_qspi_busy()) {
        NFPRINT("qspi trans busy!!!\r\n");
    }
    if (is_rx_fifo_not_empty()) {
        NFPRINT("rx fifo not empty \r\n");
        while (is_rx_fifo_not_empty()) {
            uint8_t recv = (uint8_t)nand_qspi_read_data();
            NFPRINT("rx fifo recvnum = %x \r\n", recv);
        }
    }
    while (is_trans_busy() || is_qspi_busy()) {
    }
}