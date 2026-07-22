/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Block Header File
 * Author: @CompanyNameTag
 * Create: 2019-06-23
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
#ifndef _DEV_MMC_BLOCK_H
#define _DEV_MMC_BLOCK_H

#include "stdint.h"

#define	MMC_CARDBUS_BLOCK_SIZE	512

extern struct disk_divide_info emmc;

uint32_t mmc_direct_write(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors);
uint32_t mmc_direct_read(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors);
uint32_t emmc_raw_write(char *buffer, uint32_t start_sector, uint32_t nsectors);
uint32_t emmc_raw_read(char *buffer, uint32_t start_sector, uint32_t nsectors);

/*
 * Only can be called in exception or interrupt, ensure that it is not in the mmc logic process
 * before called in interrupt.
 */
uint32_t mmc_read_in_exception(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors);
uint32_t mmc_write_in_exception(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors);

#endif
