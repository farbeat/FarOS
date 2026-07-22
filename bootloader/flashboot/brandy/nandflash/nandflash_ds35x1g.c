/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include <stdint.h>
#include "debug_print.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "hal_nandflash.h"
#include "nandflash_config.h"
#include "nandflash_ds35x1g.h"

#define DS35X1G_GET_ECC_STATUS(x) ((0x30 & (x)) >> 4)
#define DS35X1G_ECC_CORRECTION 0x00
#define DS35X1G_ECC_1BIT_CORRET 0x01
#define DS35X1G_ECC_4BIT_CORRET 0x02
#define DS35X1G_ECC_ERROR 0x03
#define NAND_ID_DATA_SIZE 0x2

static qspi_data_type_def g_qspi_data = {0};

static uint32_t mc_ds35x1g_read_page(
    uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len, const uint32_t *ecc_result)
{
    uint32_t ret;
    uint8_t status_reg;

    UNUSED(ecc_result);
    ret = mc_flash_read_page(nand_page, data, data_len, oob, oob_len, &status_reg);
    if (get_nand_flash()->enable_ecc == true) {
        if (DS35X1G_GET_ECC_STATUS(status_reg) >= DS35X1G_ECC_ERROR) {
            return MC_FAILURE;
        } else {
            return MC_SUCCESS;
        }
    }
    return ret;
}

static uint32_t mc_ds35x1g_init(void)
{
    nand_driver_init(SPEED_MID);
    PRINT("ds35x1g_init \r\n");
    uint32_t ret;
    mc_flash_write_cmd(CMD_FLASH_RESET); /* tRST 5us,no command will be accepted */
    mc_flash_busy_wait();
    write_cmd(CMD_FLASH_WREN);
    ret = mc_flash_write_sr(CMD_FLASH_SR1_ADDR, 0x0); /* set SR1 0,Clear block protect bits */
    if (get_nand_flash()->enable_ecc == true) {
        ret = mc_flash_write_sr(CMD_FLASH_SR2_ADDR, 0x11); /* set SR1 0,Clear block protect bits */
    } else {
        ret = mc_flash_write_sr(CMD_FLASH_SR2_ADDR, 0x01); /* set SR1 0,Clear block protect bits */
    }
    uint8_t status_reg;
    mc_flash_read_sr(CMD_FLASH_SR1_ADDR, (uint8_t *)&status_reg);
    PRINT("nandflash A0 Reg:0x%x \r\n", status_reg);
    mc_flash_read_sr(CMD_FLASH_SR2_ADDR, (uint8_t *)&status_reg);
    PRINT("nandflash B0 Reg:0x%x \r\n", status_reg);
    mc_flash_read_sr(CMD_FLASH_SR3_ADDR, (uint8_t *)&status_reg);
    PRINT("nandflash C0 Reg:0x%x \r\n", status_reg);
    if (ret != MC_SUCCESS || status_reg != 0x2) {
        return MC_FAILURE;
    }
    return MC_SUCCESS;
}

static uint16_t mc_ds35x1g_check_id(void)
{
    g_qspi_data.instruction = CMD_FLASH_ID;
    g_qspi_data.address_mode = MC_OSPI_ADDRESS_NONE;
    g_qspi_data.address = 0;
    g_qspi_data.address_size = 0;
    g_qspi_data.dummy_cycles = MC_OSPI_CYCLES_8_BITS;
    g_qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    g_qspi_data.data_size = NAND_ID_DATA_SIZE;
    return read_id(&g_qspi_data);
}

struct nand_driver_struct g_ds35x1g_driver = {mc_ds35x1g_init,
    mc_ds35x1g_check_id,
    mc_ds35x1g_read_page,
    mc_flash_write_page,
    mc_flash_erase_block,
    mc_flash_mark_bad_block,
    mc_flash_check_bad_block,
    DS35X1G_BYTES_PER_PAGE,
    DS35X1G_BYTES_PER_OOB,
    DS35X1G_PAGES_PER_BLOCK,
    DS35X1G_TOTAL_BLOCKS,
    DS35X1G_CHIP_ID,
    DS35X1G_OOB_TOTAL_LEN,
    DS35X1G_OOB_PER_SIZE,
    DS35X1G_OOB_PER_SIZE,
    DS35X1G_OOB_PER_SIZE,
    DS35X1G_OOB_PER_SIZE,
    DS35X1G_OOB_1_OFFSET,
    DS35X1G_OOB_2_OFFSET,
    DS35X1G_OOB_3_OFFSET,
    DS35X1G_OOB_4_OFFSET,
#ifdef ENABLE_ECC
    true
#else
    false
#endif
    };
