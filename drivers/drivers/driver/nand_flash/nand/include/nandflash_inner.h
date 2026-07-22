/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_INNER_H
#define NANDFLASH_INNER_H
#include <stdint.h>
#include <stddef.h>
#include "errcode.h"

#define BYTES_PER_PAGE 2048
#define OOB_TOTAL_LEN  16
#define OOB1_PER_SIZE  4
#define OOB2_PER_SIZE  12
#define OOB3_PER_SIZE  16
#define OOB1_OFFSET    (BYTES_PER_PAGE + 4)
#define OOB2_OFFSET    (BYTES_PER_PAGE + 20)
#define OOB3_OFFSET    (BYTES_PER_PAGE + 36)
#define OOB4_OFFSET    (BYTES_PER_PAGE + 52)
#define OOB5_OFFSET    (BYTES_PER_PAGE + 32)

#define CMD_FLASH_WREN      0x06 /* Write Enable */
#define CMD_FLASH_WRDI      0x04 /* Write Disable */
#define CMD_FLASH_RDSR      0x0F /* Read Status Register */
#define CMD_FLASH_WRSR      0x1F /* Write Status Register */
#define CMD_FLASH_READ_PAGE 0x13 /* Read Data Bytes */
#define CMD_FLASH_READ      0x03 /* Read Data Bytes */

#define CMD_FLASH_READ_QUAD_OUTPUT 0x3 /* Read Data Quad Output */
#define CMD_FLASH_READ_QUAD_IO     0xEB /* Read Data Quad IO */

#define CMD_FLASH_PP        0x02 /* Page Program */
#define CMD_FLASH_PP_RANDOM 0x84 /* Page Program Random */

#define CMD_FLASH_PP_QUAD 0x32 /* Page Program Quad */
#define CMD_FLASH_PP_EXE  0x10 /* Page Program Execute */

#define CMD_FLASH_ERASE_BLOCK 0xD8 /* Block Erase */

#define CMD_FLASH_RESET 0xFF /* Device Reset */
#define CMD_FLASH_ID    0x9F /* Device ID */

#define CMD_FLASH_SR1_ADDR 0xA0 /* Status Register-1 Addr */
#define CMD_FLASH_SR2_ADDR 0xB0 /* Status Register-2 Addr */
#define CMD_FLASH_SR3_ADDR 0xC0 /* Status Register-3 Addr */

#define NAND_TMIEOUT 500 /* 50ms timeout */

typedef enum nandflash_trans_type {
    TRANS_BY_DMA_QUAD,
    TRANS_BY_CPU_SINGLE_LINE,
    TRANS_DEFAULT
} nandflash_trans_type_t;

typedef enum nandflash_trans_speed {
    SPEED_HIGH = 8,
    SPEED_MID = 12,
    SPEED_SLOW = 16,
} nandflash_speed_type_t;

struct nand_driver_struct {
    uint32_t (*nand_flash_init)(void);
    uint16_t (*check_id)(void);
    uint32_t (*read_page)(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len,
        uint8_t *status_reg, enum nandflash_trans_type type);
    uint32_t (*write_page)(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len);
    uint32_t (*erase_block)(uint32_t block_no);
    uint32_t (*mark_bad_block)(uint32_t block_no);
    uint32_t (*check_bad_block)(uint32_t block_no);
    uint32_t bytes_per_page;
    uint32_t bytes_per_oob;
    uint32_t pages_per_block;
    uint32_t total_block;
    uint32_t chip_id;
    uint16_t oob_total_len;
    uint16_t oob1_per_size;
    uint16_t oob2_per_size;
    uint16_t oob3_per_size;
    uint16_t oob4_per_size;
    uint16_t oob1_offset; /* first oob address shift */
    uint16_t oob2_offset; /* second oob address shift */
    uint16_t oob3_offset; /* third oob address shift */
    uint16_t oob4_offset; /* fourth oob address shift */
    bool enable_ecc;
};

uint32_t mc_flash_read_sr(uint8_t addr, uint8_t *data);
uint32_t mc_flash_write_sr(uint8_t addr, uint8_t data);
void mc_flash_write_cmd(uint8_t cmd);
void mc_flash_busy_wait(void);
uint32_t mc_flash_read_page(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len,
    uint8_t *status_reg, enum nandflash_trans_type type);
uint32_t mc_flash_write_page(uint32_t nand_page, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len);
uint32_t mc_flash_erase_block(uint32_t block_no);
uint32_t mc_flash_mark_bad_block(uint32_t block_no);
uint32_t mc_flash_check_bad_block(uint32_t block_no);

/**
 * @if Eng
 * @brief  obtain nandflash id
 * @param  [in]  id The address of the returned value
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_nandflash_read_id(uint16_t *id);
void set_trans_type(nandflash_trans_type_t type);
nandflash_trans_type_t get_trans_type(void);
extern uint8_t *g_nand_buf;

#endif /* End of #ifndef MC_NAND_FLASH_H */