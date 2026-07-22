/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_CONFIG_H
#define NANDFLASH_CONFIG_H
#include "stdint.h"
#include "hal_nandflash.h"
#include "nandflash.h"

#define DEFALUT_BUFLENGTH 2176

typedef struct {
    uint32_t bytes_per_page;
    uint32_t bytes_per_oob;
    uint32_t pages_per_block;
    uint32_t total_blocks;
    uint16_t chip_id;
} flash_info_type_def;

typedef struct {
    uint16_t oob_total_len;
    uint16_t oob1_per_size;
    uint16_t oob2_per_size;
    uint16_t oob3_per_size;
    uint16_t oob4_per_size;
    uint16_t oob1_offset; /* first oob address shift */
    uint16_t oob2_offset; /* second oob address shift */
    uint16_t oob3_offset; /* third oob address shift */
    uint16_t oob4_offset; /* fourth oob address shift */
} oob_info_type_def;

extern qspi_data_type_def g_qspi_data_read_page[1];
extern qspi_data_type_def g_qspi_data_read_quad[1];
extern qspi_data_type_def g_qspi_data_write_page[1];
extern qspi_data_type_def g_qspi_data_write_quad[1];
extern qspi_data_type_def g_qspi_data_write_random[1];
extern qspi_data_type_def g_qspi_data_read[1];
extern qspi_data_type_def g_qspi_data_erase_block[1];
extern const flash_info_type_def g_flash_info[1];

struct nand_driver_struct *get_nand_flash(void);
int nandflash_filesystem_init(void);
void nandflash_qspi_init(void);
void nandflash_qspi_resume(void);
void test_nand_flash_read_8(struct nand_driver_struct  *test_nand_flash_driver, uint32_t nand_page);
void test_nand_flash_write_width8(struct nand_driver_struct *test_nand_flash_driver, uint32_t nand_page);
void nandflash_yaffs_sync(void);
void nandflash_yaffs_format(void);
#endif /* End of #ifndef MC_NAND_CONFIG_H */
