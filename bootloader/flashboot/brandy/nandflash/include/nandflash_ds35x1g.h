/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_DS35X1G_H
#define NANDFLASH_DS35X1G_H

#define DS35X1G_BYTES_PER_PAGE  2048
#define DS35X1G_BYTES_PER_OOB   64
#define DS35X1G_PAGES_PER_BLOCK 64
#define DS35X1G_TOTAL_BLOCKS    1024
#define DS35X1G_CHIP_ID         0xE521
#define DS35X1G_OOB_TOTAL_LEN   4
#define DS35X1G_OOB_PER_SIZE    8
#define DS35X1G_OOB_1_OFFSET    0
#define DS35X1G_OOB_2_OFFSET    16
#define DS35X1G_OOB_3_OFFSET    32
#define DS35X1G_OOB_4_OFFSET    48

extern struct nand_driver_struct g_ds35x1g_driver;

#endif /* End of #ifndef MC_NAND_DS35X1G_H */
