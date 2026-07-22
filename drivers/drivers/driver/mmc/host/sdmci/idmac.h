/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Himci Idma Header File
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
#ifndef _SDMCI_IDAMC_H_
#define _SDMCI_IDAMC_H_

#include "hci.h"

#define SDMCI_IDAMC_SLICES    2048
#define IDAMC_ADMA2_ALIGN    CACHE_ALIGNED_SIZE
#define IDAMC_ADMA2_MASK    (IDAMC_ADMA2_ALIGN - 1)

#define DIMAC_ADDR_ALIGN_OFF(addr)    ((IDAMC_ADMA2_ALIGN - (addr & IDAMC_ADMA2_MASK)) & IDAMC_ADMA2_MASK)

typedef struct {
    u_char rw;        /* Read/write operation */
    u_char *desc;        /* Memory of IDAMC Descriptor Line */
    uint32_t nslices;    /* Number of slice */
    uint32_t desc_size;    /* Size of desc */
    uint32_t slice;        /* Size of slice */
    uint32_t buf_size;    /* Size of buff */
    void *head_buff;    /* Buff for the head data which the data addr is not aligned */
    uint32_t head_consume;    /* Valid data in the buff for the head data */
    void *tail_buff;    /* Buff for the tail data which the tail data addr is not aligned */
    uint32_t tail_consume;    /* Valid data in the buff for the tail data */
} idamc;

void sdmci_idma_reset(const struct hci_softc *sc);
void sdmci_idma_start(const struct hci_softc *sc, u_char *desc, uint32_t size);
void sdmci_idma_stop(const struct hci_softc *sc);
int sdmci_idmac_finish(const idamc *dma, void *mem, uint32_t len);
int sdmci_sgs_idmac_finish(const idamc *dma, const struct mmc_data *data);
uint32_t sdmci_idmac_align_size(const idamc *dma);
int sdmci_idmac_setup(idamc *dma, const struct mmc_data *data);
int sdmci_idmac_create(idamc *dma, uint32_t nslices);
void sdmci_idmac_delete(idamc *dma);

#endif /* _SDMCI_IDAMC_H_ */
