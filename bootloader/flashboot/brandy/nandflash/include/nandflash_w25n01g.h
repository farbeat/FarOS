/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_W25N01G_H
#define NANDFLASH_W25N01G_H

#define W25N01G_BYTES_PER_PAGE  2048
#define W25N01G_BYTES_PER_OOB   64
#define W25N01G_BYTES_PER_OOB_ENABLE_ECC 32
#define W25N01G_PAGES_PER_BLOCK 64
#define W25N01G_TOTAL_BLOCKS    1024
#define W25N01G_CHIP_ID         0xefba
#define W25N01G_OOB_INVALID     0
#define W25N01G_OOB_TOTAL_LEN   4
#define W25N01G_OOB_PER_SIZE    8
#define W25N01G_OOB_1_OFFSET    0
#define W25N01G_OOB_2_OFFSET    16
#define W25N01G_OOB_3_OFFSET    32
#define W25N01G_OOB_4_OFFSET    48

extern struct nand_driver_struct g_w25n01g_driver;

#endif /* End of #ifndef MC_NAND_W25N01G_H */
