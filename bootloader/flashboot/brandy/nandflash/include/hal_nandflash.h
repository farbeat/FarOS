/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef HAL_NANDFLASH_H
#define HAL_NANDFLASH_H
#include <stdint.h>
#include "debug_print.h"
#include "nandflash_inner.h"

#define ECC_RESULT_NO_ERROR 0
#define ECC_RESULT_FIXED 1
#define ECC_RESULT_UNFIXED 2
#define ECC_RESULT_UNKNOWN 3

#define MC_SUCCESS 0
#define MC_FAILURE 1
#define BAD_BLOCK 1
#define NOT_BAD_BLOCK 0

#define DIV 6
#define SUPPORT_STATISTIC 0
#define SER_SELECT 0x1
#define QUAD_DMA_READ 1
#define CPU_TRANS_BUFFER_DEEP 12

#ifdef NANDFLASH_PRINT
extern void print_str(const char *str, ...);
#define NFPRINT(fmt, arg...) print_str("NANDFLASH|" fmt, ##arg)
#else
#define NFPRINT(fmt, arg...)
#endif

#define HAL_PIO_HS_PINMUX_CTL_ADDR           0x52065800
#define HS_GPIO6_PINMUX            (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x18) // qspi d0
#define HS_GPIO7_PINMUX            (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x1c) // qspi d1
#define HS_GPIO8_PINMUX            (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x20) // qspi d2
#define HS_GPIO9_PINMUX            (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x24) // qspi d3
#define HS_GPIO11_PINMUX           (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x2c) // qspi cs1
#define HS_GPIO12_PINMUX           (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x30) // qspi cs2
#define HS_GPIO15_PINMUX           (HAL_PIO_HS_PINMUX_CTL_ADDR + 0x3c) // qspi clk

#define HS_GPIO_PAD_CTRL_BASE      0x5700d000
#define HS_GPIO6_PAD_CTRL          (HS_GPIO_PAD_CTRL_BASE + 0x218) // qspi d0
#define HS_GPIO7_PAD_CTRL          (HS_GPIO_PAD_CTRL_BASE + 0x21c) // qspi d1
#define HS_GPIO8_PAD_CTRL          (HS_GPIO_PAD_CTRL_BASE + 0x220) // qspi d2
#define HS_GPIO9_PAD_CTRL          (HS_GPIO_PAD_CTRL_BASE + 0x224) // qspi d3
#define PAD_CTRL_IE_EN             0x7
#define M_CTL_MTOP4_RST_SOFT_N_0   0x52000170
#define GLB_CTL_M_SOFT_RST         0x570000c8
#define PMU_CTL_MCU_HS_PWR_EN      0x57004030
#define MCU_HS_EN_MASK             0x10
#define QSPI_INT_MASK                 0x1E
#define QSPI_FIFO_THRESHOLD           4
#define DUMMY_CYCLE_BYTE 0x0

enum addr_type {
    MC_OSPI_ADDRESS_NONE,
    MC_OSPI_ADDRESS_1_LINE,
    MC_OSPI_ADDRESS_4_LINES
};

enum dummy_cycle {
    MC_OSPI_CYCLES_NONE,
    MC_OSPI_CYCLES_2_BITS,
    MC_OSPI_CYCLES_4_BITS,
    MC_OSPI_CYCLES_8_BITS,
};

enum data_mode {
    MC_OSPI_DATA_NONE,
    MC_OSPI_DATA_1_LINE,
    MC_OSPI_DATA_4_LINES
};

enum address_size {
    MC_OSPI_ADDRESS_8_BITS,
    MC_OSPI_ADDRESS_16_BITS
};

typedef struct {
    uint8_t instruction;
    enum addr_type address_mode;
    uint16_t address;
    enum address_size address_size;
    enum dummy_cycle dummy_cycles;
    enum data_mode data_mode;
    uint32_t data_size;
    uint8_t *data_ptr;
} qspi_data_type_def;

uint8_t mc_flash_get_offset(void);
uint16_t read_id(qspi_data_type_def *qspi_data);
uint32_t read_cell_array(qspi_data_type_def *qspi_data);
uint32_t read_buffer(qspi_data_type_def *qspi_data, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len);
uint8_t read_sr(qspi_data_type_def *qspi_data, uint8_t address);
uint32_t write_sr(qspi_data_type_def *qspi_data, uint8_t addr, uint8_t data);
void write_cmd(uint8_t command);
uint32_t program_load(qspi_data_type_def *qspi_data, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len);
uint32_t mark_bad_block(qspi_data_type_def *qspi_data);
uint32_t program_load_width_32(qspi_data_type_def *qspi_data);
uint32_t program_execute(qspi_data_type_def *qspi_data);
uint32_t block_erase(qspi_data_type_def *qspi_data);
void nand_driver_init(nandflash_speed_type_t speed);
void nand_driver_sem_init(void);
uint32_t nand_trans_wait(void);
void nand_recv_data(uint8_t *buffer, uint32_t length);
void nand_recv_data_width_32(uint8_t *buffer, uint32_t length);
void change_div(uint32_t div);
uint32_t read_buffer_single_line_width_8(qspi_data_type_def *qspi_data);
uint32_t read_buffer_single_line_width_32(qspi_data_type_def *qspi_data);
uint32_t write_by_cpu_single_line(qspi_data_type_def *qspi_data);
#endif