/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#ifndef NANDFLASH_TC58CYG_H
#define NANDFLASH_TC58CYG_H

#define TC58CYG_BYTES_PER_PAGE  2048
#define TC58CYG_BYTES_PER_OOB   128
#define TC58CYG_PAGES_PER_BLOCK 64
#define TC58CYG_TOTAL_BLOCKS    1024
#define TC58CYG_CHIP_ID         0xB298
#define TC58CYG_OOB_INVALID     0

extern struct nand_driver_struct g_tc58cyg_driver;

#endif /* End of #ifndef MC_NAND_TC58CYG_H */
