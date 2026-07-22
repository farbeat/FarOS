/*----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2015. All rights reserved.
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
 *---------------------------------------------------------------------------*/

#include "dpal.h"
#ifdef EMMC_BOOT
#include "securec_loader.h"
#endif

#define NUM_5   5
#define BIT_10  10
#define BIT_16  16
#define BIT_8   8
#define BIT_4   4
#define BIT_2   2
#define BUF_LEN 20
/*----------------------------------------------------------------------------*/
/* ulltostr */
/*----------------------------------------------------------------------------*/
char *ulltostr(unsigned long long size)
{
    int ix;
    static char buffer[BUF_LEN];
    unsigned long size_long;
    char *fmt[] = {"%lu", "%luK", "%luM", "%luG", "%luT", "%luP"};

    for (ix = 0; (ix < NUM_5) && ((size & 0x3FF) == 0) && (size != 0); ix++) {
        size = (size >> BIT_10);
    }

    size_long = (unsigned long)size;

    if (snprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, fmt[ix], size_long) < 0) {
        dpal_log_err("ulltostr snprintf_s fail!\r\n");
    }

    return buffer;
}

/*----------------------------------------------------------------------------*/
/* ffs */
/*----------------------------------------------------------------------------*/
int ffs(int x)
{
    int r = 1;
    unsigned int tmp = (unsigned int)x;

    if (x == 0) {
        return 0;
    }
    if ((tmp & 0xffff) == 0) {
        tmp >>= BIT_16;
        r += BIT_16;
    }
    if ((tmp & 0xff) == 0) {
        tmp >>= BIT_8;
        r += BIT_8;
    }
    if ((tmp & 0xf) == 0) {
        tmp >>= BIT_4;
        r += BIT_4;
    }
    if ((tmp & 0x3) == 0) {
        tmp >>= BIT_2;
        r += BIT_2;
    }
    if ((tmp & 0x1) == 0) {
        r += 1;
    }
    return r;
}

/*----------------------------------------------------------------------------*/
/* mtd_dma_cache_inv */
/*----------------------------------------------------------------------------*/
void mtd_dma_cache_inv(void *addr, unsigned int size)
{
    uintptr_t start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    uintptr_t end = (uintptr_t)addr + size;

    end = dpal_align(end, CACHE_ALIGNED_SIZE);

    dpal_dma_cache_inv(start, end);
}

/*----------------------------------------------------------------------------*/
/* mtd_dma_cache_clean */
/*----------------------------------------------------------------------------*/
void mtd_dma_cache_clean(void *addr, unsigned int size)
{
    uintptr_t start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    uintptr_t end = (uintptr_t)addr + size;

    end = dpal_align(end, CACHE_ALIGNED_SIZE);

    dpal_dma_cache_clean(start, end);
}

