/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Support that yaffs code normalization.
 * Author: @CompanyNameMagicTag
 * Create: 2022-02-08
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
#ifndef YAFFS_ADAPT_OS_H
#define YAFFS_ADAPT_OS_H
/* vfs_yaffs2.c */
#include "los_vm_filemap.h"
/* yaffs_nandcfg.c */
#include "inode/inode.h"
/* yaffs_osglue.c */
#include "los_sys_pri.h"
/* yaffs_guts.h */
#include "los_list.h"
#define YAFFS_MAGIC			0x5941ff53
/* mtd_nandcfg.h */
#define yaffs_nand_mtd nand_mtd
/* mtd_nandcfg.c */
#define os_nand_yaffs_read                 nand_yaffs_read
#define os_nand_yaffs_write                nand_yaffs_write
#define os_nand_yaffs_erase                nand_erase
#define os_nand_yaffs_nand_block_isbad     nand_yaffs_nand_block_isbad
#define os_nand_yaffs_nand_block_markbad   nand_yaffs_nand_block_markbad
/* yaffs_osglue.c */
#define YAFFS_MUTEX LosMux
#endif
