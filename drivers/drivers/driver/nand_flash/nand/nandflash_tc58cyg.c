/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "nandflash_tc58cyg.h"
#include <stdint.h>
#include <stddef.h>
#include "hal_nandflash.h"
#include "nandflash_tc58cyg.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "debug_print.h"

#define TC58CYG_GET_ECC_STATUS(x) ((0x30 & (x)) >> 4)
#define TC58CYG_ECC_CORRECTION 0x00
#define TC58CYG_ECC_1BIT_CORRET 0x01
#define TC58CYG_ECC_4BIT_CORRET 0x03
#define TC58CYG_ECC_ERROR 0x02
#define NAND_ID_DATA_SIZE 0x2

static qspi_data_type_def g_qspi_data = {0};

static uint32_t mc_tc58cyg_read_page(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob,
    uint32_t oob_len, const uint32_t *ecc_result, enum nandflash_trans_type type)
{
    unused(ecc_result);
    uint32_t ret;
    uint8_t status_reg;
    ret = mc_flash_read_page(nand_page, data, data_len, oob, oob_len, &status_reg, type);
    if (ret != MC_SUCCESS) {
        return MC_FAILURE;
    }
    return ret;
}

static uint32_t mc_tc58cyg_init(void)
{
    nand_driver_init(SPEED_HIGH);
    uint32_t ret;
    PRINT("MC_TC58CYG_Init \r\n");
    mc_flash_write_cmd(CMD_FLASH_RESET); /* tRST 5us,no command will be accepted */
    mc_flash_busy_wait();
    ret = mc_flash_write_sr(CMD_FLASH_SR1_ADDR, 0x0); /* set SR1 0,Clear block protect bits */
    ret = mc_flash_write_sr(CMD_FLASH_SR2_ADDR, 0x6); /* set SR1 0,Clear block protect bits */

    uint8_t status_reg;
    mc_flash_read_sr(CMD_FLASH_SR1_ADDR, (uint8_t *)&status_reg);
    PRINT("nandflash A0 Reg:0x%x \r\n", status_reg);
    mc_flash_read_sr(CMD_FLASH_SR2_ADDR, (uint8_t *)&status_reg);
    PRINT("nandflash B0 Reg:0x%x \r\n", status_reg);
    if (ret != MC_SUCCESS) {
        return MC_FAILURE;
    }
    return MC_SUCCESS;
}

static uint16_t mc_tc58cyg_check_id(void)
{
    g_qspi_data.instruction = CMD_FLASH_ID;
    g_qspi_data.address_mode = MC_OSPI_ADDRESS_NONE;
    g_qspi_data.address = 0;
    g_qspi_data.address_size = 0;
    g_qspi_data.dummy_cycles = MC_OSPI_CYCLES_8_BITS;
    g_qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    g_qspi_data.data_size = NAND_ID_DATA_SIZE;
    g_qspi_data.data_ptr = g_nand_buf;
    return read_id(&g_qspi_data);
}

struct nand_driver_struct g_tc58cyg_driver = {mc_tc58cyg_init,
    mc_tc58cyg_check_id,
    mc_tc58cyg_read_page,
    mc_flash_write_page,
    mc_flash_erase_block,
    mc_flash_mark_bad_block,
    mc_flash_check_bad_block,
    TC58CYG_BYTES_PER_PAGE,
    TC58CYG_BYTES_PER_OOB,
    TC58CYG_PAGES_PER_BLOCK,
    TC58CYG_TOTAL_BLOCKS,
    TC58CYG_CHIP_ID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    TC58CYG_OOB_INVALID,
    false};
