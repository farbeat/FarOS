/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Sdhci Adma2 Header File
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

#ifndef _SDHCI_ADMA2_H_
#define _SDHCI_ADMA2_H_
#include <sys/types.h>

#include "mmc_type.h"
typedef struct {
    void *ptr;
    uint32_t buf_size_max;
    uint32_t buf_size;
    uint32_t unaligned_flag;
} aligned_buf_s;

typedef struct {
    u_char *desc;            /* Memory of ADMA2 Descriptor Line */
    uint32_t length;        /* Length of ADMA2 memory */
    uint32_t num_slices;        /* Number of slice */
    uint32_t slice;            /* Size of slice */
#define    ADMA2_ATTR_NOPDESC    (1u << 0)
#define    ADMA2_ATTR_MASK        (ADMA2_ATTR_NOPDESC)
    uint32_t attr;            /* Attribution of the ADMA2 Descriptor memory */
    aligned_buf_s aligned_buf_start;
    aligned_buf_s aligned_buf_end;
} adma2_s;

uint32_t sdhci_adma_alian_size(const adma2_s *adma2);
int32_t sdhci_adma2_tab_setup(adma2_s *dma, uintptr_t addr, uint32_t len, uint32_t write_flag);
void sdhci_adma2_delete(adma2_s *dma);
uint32_t sdhci_adma2_create(adma2_s *dma, uint32_t num_slices, uint32_t attr);

#endif /* _SDHCI_ADMA2_H_ */
