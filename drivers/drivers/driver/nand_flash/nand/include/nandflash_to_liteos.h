/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_TO_LITEOS_H
#define NANDFLASH_TO_LITEOS_H
#include "stdint.h"
#include "linux/mtd/mtd.h"


/**
 * @brief  nand register
 * @param  mtd mtd
 */
void nand_register(struct mtd_info *mtd);

/**
 * @brief  nandflash erase
 * @param  mtd mtd
 * @param  instr.addr is erase start addr. instr.len is erase length
 * @return 0 SUCESS 1 FAILURE
 */
int liteos_nand_erase(struct mtd_info *mtd, struct erase_info *instr);

/**
 * @brief  Not currently used
 */
int liteos_nand_read(struct mtd_info *mtd, uint64_t from, size_t len, size_t *retlen, char *buf);

/**
 * @brief  Not currently used
 */
int liteos_nand_write(struct mtd_info *mtd, uint64_t to, size_t len, size_t *retlen, const char *buf);

/**
 * @brief  nandflash read data
 * @param  mtd mtd
 * @param  from start address of read data
 * @param  len read len
 * @param  retlen euqal len
 * @param  buf The read data is saved here
 * @return 0 SUCESS 1 FAILURE
 */
int liteos_nand_read_oob(struct mtd_info *mtd, uint64_t from, size_t len, size_t *retlen, char *buf);

/**
 * @brief  nandflash write data
 * @param  mtd mtd
 * @param  to start address of write data
 * @param  len write len
 * @param  retlen euqal len
 * @param  buf The read data is saved here
 * @return 0 SUCESS 1 FAILURE
 */
int liteos_nand_write_oob(struct mtd_info *mtd, uint64_t to, size_t len, size_t *retlen, const char *buf);

/**
 * @brief  check nandflash is bad
 * @param  mtd mtd
 * @param  ofs addr
 * @return 1 is bad block
 */
int liteos_nand_block_isbad(struct mtd_info *mtd, uint64_t ofs);

/**
 * @brief  mark nandflash is bad
 * @param  mtd mtd
 * @param  ofs addr
 * @return 0 SUCESS 1 FAILURE
 */
int liteos_nand_block_markbad(struct mtd_info *mtd, uint64_t ofs);
#endif
