/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "nandflash.h"
#include "securec.h"
#include "stdint.h"
#include "watchdog.h"
#include "hal_nandflash.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "tcxo.h"
#include "hal_nandflash.h"
#include "nandflash_config.h"
#include "nandflash_qspi.h"
#include "hal_nandflash.h"

nandflash_trans_type_t g_trans_type = TRANS_BY_DMA_QUAD;

#define FLASH_GET_P_FAIL_STATUS(x) (0x08 & (x))
#define FLASH_GET_E_FAIL_STATUS(x) (0x04 & (x))
#define TIMEOUT 1000
#define MC_FLASH_WAIT_10US 10
#define NAND_ID_DATA_SIZE 2
static qspi_data_type_def g_nand_qspi_data = {0};

errcode_t uapi_nandflash_read_id(uint16_t *id)
{
    if (id == NULL) {
        return ERRCODE_INVALID_PARAM;
    }
    static qspi_data_type_def qspi_data = {0};
    qspi_data.instruction = CMD_FLASH_ID;
    qspi_data.address_mode = MC_OSPI_ADDRESS_NONE;
    qspi_data.address = 0;
    qspi_data.address_size = 0;
    qspi_data.dummy_cycles = MC_OSPI_CYCLES_8_BITS;
    qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    qspi_data.data_size = NAND_ID_DATA_SIZE;
    *id =  read_id(&qspi_data);
    return ERRCODE_SUCC;
}

uint32_t mc_flash_read_sr(uint8_t addr, uint8_t *data)
{
    uapi_watchdog_kick();
    qspi_data_type_def qspi_data = {0};
    qspi_data.instruction = CMD_FLASH_RDSR;
    qspi_data.address_mode = MC_OSPI_ADDRESS_1_LINE;
    qspi_data.address = addr;
    qspi_data.address_size = MC_OSPI_ADDRESS_8_BITS;
    qspi_data.dummy_cycles = MC_OSPI_CYCLES_NONE;
    qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    qspi_data.data_size = 1; /* 1: one byte */
    *data = read_sr(&qspi_data, addr);
    return MC_SUCCESS;
}

uint32_t mc_flash_write_sr(uint8_t addr, uint8_t data)
{
    uapi_watchdog_kick();
    uint32_t ret;
    qspi_data_type_def qspi_data = {0};
    qspi_data.instruction = CMD_FLASH_WRSR;
    qspi_data.address_mode = MC_OSPI_ADDRESS_1_LINE;
    qspi_data.address = addr;
    qspi_data.address_size = MC_OSPI_ADDRESS_8_BITS;
    qspi_data.dummy_cycles = MC_OSPI_CYCLES_NONE;
    qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    qspi_data.data_size = 1; /* 1: one byte */
    ret = write_sr(&qspi_data, addr, data);
    return ret;
}

void mc_flash_write_cmd(uint8_t cmd)
{
    uapi_watchdog_kick();
    write_cmd(cmd);
}

void mc_flash_busy_wait(void)
{
    uapi_watchdog_kick();
    qspi_data_type_def qspi_data = {0};
    qspi_data.instruction = CMD_FLASH_RDSR;
    qspi_data.address_mode = MC_OSPI_ADDRESS_1_LINE;
    qspi_data.address = CMD_FLASH_SR3_ADDR;
    qspi_data.address_size = MC_OSPI_ADDRESS_8_BITS;
    qspi_data.dummy_cycles = MC_OSPI_CYCLES_NONE;
    qspi_data.data_mode = MC_OSPI_DATA_1_LINE;
    qspi_data.data_size = 1;
    uint8_t status = read_sr(&qspi_data, (uint8_t)qspi_data.address);
    uint32_t timeout = TIMEOUT;
    uint32_t i = 1;
    while ((status & 0x1) != 0 && timeout != 0) {
        uapi_tcxo_delay_us(i * MC_FLASH_WAIT_10US);
        i++;
        status = read_sr(&qspi_data, (uint8_t)qspi_data.address);
        timeout = timeout - 1;
    }
    if ((status & 0x1) != 0) {
        PRINT("mc_flash_busy_wait , nandflash status:%x not equal zero!!!\r\n", status);
    }
    return;
}

uint32_t mc_flash_read_page(uint32_t nand_page, uint8_t *data, uint32_t data_len,
                            uint8_t *oob, uint32_t oob_len, uint8_t *status_reg, enum nandflash_trans_type type)
{
    uapi_watchdog_kick();
    NFPRINT("mc_flash_read_page nand_page = %d\r\n", nand_page);
    uint32_t page_addr;
    errno_t err_code;
    uint8_t error_cnt = 0;
    if (nand_page >= (get_nand_flash()->pages_per_block * get_nand_flash()->total_block)) {
        return MC_FAILURE;
    }
    mc_flash_busy_wait();
    page_addr = SWAP_U32(nand_page);
    NFPRINT("page addr = %u \r\n", page_addr);
    qspi_data_type_def qspi_data = {0};
    qspi_data.data_ptr = (uint8_t *)&page_addr;
    qspi_data.instruction = 0x13;
    read_cell_array(&qspi_data);
    mc_flash_busy_wait();

    qspi_data_type_def qspi_read_buffer = {0};
    qspi_read_buffer.instruction = 0x03;
    qspi_read_buffer.data_size = get_nand_flash()->bytes_per_page + get_nand_flash()->bytes_per_oob;
    read_buffer(&qspi_read_buffer, data, data_len, oob, oob_len, type);
    mc_flash_read_sr(CMD_FLASH_SR3_ADDR, status_reg);
    NFPRINT("mc_flash_read_page success status = %x\r\n", *status_reg);
    return MC_SUCCESS;
}

uint32_t mc_flash_write_page(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len)
{
    uapi_watchdog_kick();
    uint32_t ret;
    uint32_t page_addr;
    uint8_t status_reg;
    write_cmd(CMD_FLASH_WREN);
    qspi_data_type_def p_load_qspi_data = {0};
    p_load_qspi_data.instruction = 0x02;
    p_load_qspi_data.data_size = get_nand_flash()->bytes_per_page + get_nand_flash()->bytes_per_oob;
    program_load(&p_load_qspi_data, data, data_len, oob, oob_len);
    qspi_data_type_def e_load_qspi_data = {0};
    e_load_qspi_data.instruction = CMD_FLASH_PP_EXE;
    page_addr = SWAP_U32(nand_page);
    e_load_qspi_data.data_ptr = (uint8_t *)&page_addr;
    program_execute(&e_load_qspi_data);
    mc_flash_busy_wait();
    mc_flash_read_sr(CMD_FLASH_SR3_ADDR, &status_reg);
    if (FLASH_GET_P_FAIL_STATUS(status_reg) != 0) {
        ret = MC_FAILURE;
    } else {
        ret = MC_SUCCESS;
    }
    write_cmd(CMD_FLASH_WRDI);
    NFPRINT("mc_flash_write_page nand_page = %d, ret = %d , status = %x\r\n", nand_page, ret, status_reg);
    return ret;
}

uint32_t mc_flash_erase_block(uint32_t block_no)
{
    uapi_watchdog_kick();
    NFPRINT("mc_flash_erase_block block_no = %d\r\n", block_no);
    uint32_t ret;
    uint32_t page_addr;
    uint8_t reg_status;
    page_addr = SWAP_U32(block_no * get_nand_flash()->pages_per_block);
    write_cmd(CMD_FLASH_WREN);
    qspi_data_type_def erase_qspi_data = {0};
    erase_qspi_data.data_ptr = (uint8_t *)&page_addr;
    erase_qspi_data.instruction = 0xd8;
    block_erase(&erase_qspi_data);
    mc_flash_busy_wait();
    mc_flash_read_sr(CMD_FLASH_SR3_ADDR, &reg_status);
    if (FLASH_GET_E_FAIL_STATUS(reg_status) != 0) {
        ret = MC_FAILURE;
    } else {
        ret = MC_SUCCESS;
    }
    write_cmd(CMD_FLASH_WRDI);
    NFPRINT("mc_flash_erase_block ret = %d , status =%x\r\n", ret, reg_status);
    return ret;
}

uint32_t mc_flash_mark_bad_block(uint32_t block_no)
{
    uapi_watchdog_kick();
    PRINT("mark block %d is bad= %d\r\n", block_no);
    write_cmd(CMD_FLASH_WREN);
    uint32_t mark_sign = 0xbbccffff;
    qspi_data_type_def mark = {0};
    mark.instruction = CMD_FLASH_PP_RANDOM;
    mark.data_ptr = (uint8_t *)&mark_sign;
    mark.address = (uint16_t)(get_nand_flash()->bytes_per_page);
    mark_bad_block(&mark);
    qspi_data_type_def execute = {0};
    execute.instruction = CMD_FLASH_PP_EXE;
    uint32_t page_addr = SWAP_U32(block_no * get_nand_flash()->pages_per_block);
    execute.data_ptr = (uint8_t *)&page_addr;
    program_execute(&execute);
    mc_flash_busy_wait();
    write_cmd(CMD_FLASH_WRDI);
    return MC_SUCCESS;
}

uint32_t mc_flash_check_bad_block(uint32_t block_no)
{
    uapi_watchdog_kick();
    uint32_t ret = NOT_BAD_BLOCK;
    uint32_t page_addr = SWAP_U32(block_no * get_nand_flash()->pages_per_block);
    qspi_data_type_def cell_read = {0};
    cell_read.data_ptr = (uint8_t *)&page_addr;
    cell_read.instruction = CMD_FLASH_READ_PAGE;
    read_cell_array(&cell_read);

    mc_flash_busy_wait();
    qspi_data_type_def random_read = {0};
    uint32_t check_value;
    random_read.data_ptr = &check_value;
    random_read.instruction = CMD_FLASH_READ;
    random_read.data_size = 4; // 4 is data length
    random_read.address = (uint16_t)(get_nand_flash()->bytes_per_page);
    read_buffer_single_line_width_8(&random_read);
    /* check the oob fisrt byte 2 and byte 3 bytes, they are badblock bits */
    check_value = check_value >> 16; // 16 is mark pos need
    if (check_value == 0xbbcc) {
        return BAD_BLOCK;
    }
    NFPRINT("block %d is ret = %d , good ok 1 bad\r\n", block_no, ret);
    return ret;
}

void set_trans_type(nandflash_trans_type_t type)
{
    g_trans_type = type;
}

nandflash_trans_type_t get_trans_type(void)
{
    return g_trans_type;
}
