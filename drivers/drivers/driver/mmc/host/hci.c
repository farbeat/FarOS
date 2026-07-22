/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: mmc block Implementation
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

#include "hci.h"
#include "mmcbrvar.h"
#include "mmc.h"
#include "soc_mmc.h"
#include "mmc_adapt.h"
#include "dpal.h"

struct hci_softc *mmc_sus_res[MAX_MMC_NUM];

void hci_dma_clean_cache(void *addr, uint32_t size)
{
    assert(size != 0);
    assert((size & (CACHE_ALIGNED_SIZE - 1)) == 0);
    assert(((uintptr_t)addr & (CACHE_ALIGNED_SIZE - 1)) == 0);

    uintptr_t start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    uintptr_t end = (uintptr_t)addr + size;
    end = dpal_align(end, CACHE_ALIGNED_SIZE);
    dpal_dma_cache_clean(start, end);
}

void hci_dma_inv_cache(void *addr, uint32_t size)
{
    assert(size != 0);
    assert((size & (CACHE_ALIGNED_SIZE - 1)) == 0);
    assert(((uintptr_t)addr & (CACHE_ALIGNED_SIZE - 1)) == 0);

    uintptr_t start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    uintptr_t end = (uintptr_t)addr + size;
    end = dpal_align(end, CACHE_ALIGNED_SIZE);
    dpal_dma_cache_inv(start, end);
}

int hci_registor_sdio_irq(device_t brdev, sdio_irq_cb cb, void *arg)
{
    struct hci_softc *sc = device_get_softc(brdev);
    if (sc == NULL) {
        return (-EINVAL);
    }

    if ((sc->sdio_cb == NULL) && (sc->sdio_arg == NULL)) {
        sc->sdio_cb = cb;
        sc->sdio_arg = arg;
        return (0);
    } else if (cb == NULL) {
        sc->sdio_cb = NULL;
        sc->sdio_arg = NULL;
        return (0);
    } else {
        return (-EACCES);
    }
}

struct hci_softc *get_mmc_resource(int index)
{
    if ((index < 0) || (index >= MAX_MMC_NUM)) {
        return (NULL);
    }
    return (mmc_sus_res[index]);
}

int set_mmc_resource(struct hci_softc *sc, int index)
{
    if ((index < 0) || (index >= MAX_MMC_NUM)) {
        return (-EINVAL);
    }
    mmc_sus_res[index] = sc;
    return (0);
}
