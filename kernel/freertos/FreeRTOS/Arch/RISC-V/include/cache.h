/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef _ARCH_CACHE_H
#define _ARCH_CACHE_H

#include "stdint.h"

typedef enum {
    CACHE_4KB  = 0,
    CACHE_8KB  = 2,
    CACHE_16KB = 4,
    CACHE_32KB = 6,
} CacheSize;

typedef enum {
    CACHE_PREF_1_LINES = 0,
    CACHE_PREF_2_LINES = 1,
    CACHE_PREF_3_LINES = 2,
    CACHE_PREF_4_LINES = 3,
    CACHE_PREF_5_LINES = 4,
    CACHE_PREF_6_LINES = 5,
    CACHE_PREF_7_LINES = 6,
    CACHE_PREF_8_LINES = 7,
} CachePrefLines;

/* riscv cache register config */
/*
 * csr_bit[0]  ICEN   Instruction cache is enabled when this bit is set to 1b1.  Default is disabled.
 * Bit[2:1]    ICS    Instruction cache size: 2b00=4KB, 2b01=8KB, 2b10=16KB, 2b11=32KB.  Default is 32KB
 */
#define ICCTL                   0x7C0
#define ICCTL_ENABLE            0x1

/*
 * Bit[0]   DCEN  Data cache is enabled when this bit is set to 1b1.  Default is disabled.
 * Bit[2:1] DCS   Data cache size: 2b00=4KB, 2b01=8KB, 2b10=16KB, 2b11=32KB.  Default is 32KB
 */
#define DCCTL                   0x7C1
#define DCCTL_ENABLE            0x1

/*
 * Bit[0]  VA   Instruction cache invalidation by all or VA: 1b0=all, 1b1=VA.
 * Bit[2]  ICIV Initiate instruction cache invalidation when this bit is set to 1b1.
           When the instruction cache invalidation is by VA, the virtual address is specified in icinva CSR
*/
#define ICMAINT                 0x7C2
#define DCMAINT                 0x7C3
#define ICINCVA                 0x7C4
#define DCINCVA                 0x7C5

#define VA                      0x1
#define ICIV                    (0x1U << 2)
#define DCIV                    (0x1U << 2)
#define DCC                     (0x1U << 3)

#define ICACHE_BY_ALL           ICIV
#define ICACHE_BY_VA            (ICIV | VA)

#define DCACHE_INV_BY_VA        (DCIV | VA)
#define DCACHE_CLEAN_ALL        DCC
#define DCACHE_CLEAN_BY_VA      (DCC | VA)
#define DCACHE_FLUSH_BY_VA      (DCC | DCIV | VA)
#define DCACHE_FLUSH_ALL        (DCC | DCIV)

#define APREFI                  0x7C6
#define APREFD                  0x7C7
#define IAPEN                   0x1
#define SAPEN                   (0x1U << 4)

#define CACHE_LINE_SIZE         32

uint32_t ArchICacheEnable(CacheSize icclSize);

uint32_t ArchDCacheEnable(CacheSize dcclSize);

uint32_t ArchICachePrefetchEnable(CachePrefLines iclValue);

uint32_t ArchDCachePrefetchEnable(CachePrefLines iclValue, CachePrefLines sclValue);

void ArchDCacheFlushByVa(uintptr_t baseAddr, uint32_t size);

void ArchICacheFlushByVa(uintptr_t baseAddr, uint32_t size);

void ArchDCacheInvByVa(uintptr_t baseAddr, uint32_t size);

void ArchDCacheCleanByVa(uintptr_t baseAddr, uint32_t size);

void ArchICacheFlushByAddr(uintptr_t startAddr, uintptr_t endAddr);

void ArchDCacheFlushByAddr(uintptr_t startAddr, uintptr_t endAddr);

void ArchDCacheCleanByAddr(uintptr_t startAddr, uintptr_t endAddr);

void ArchDCacheInvByAddr(uintptr_t startAddr, uintptr_t endAddr);

void ArchICacheFlush(void);

void ArchDCacheFlush(void);

void ArchDCacheClean(void);

static inline uintptr_t ArchCacheAlign(uintptr_t addr, uint32_t boundary)
{
    return (addr + boundary - 1) & ~((uintptr_t)(boundary - 1));
}
#ifndef ALIGN
#define ALIGN(addr, boundary) ArchCacheAlign(addr, boundary)
#endif

#define flush_dcache(start, end) ArchDCacheFlushByAddr(start, end)
#define flush_icache() ArchICacheFlush()

#endif /* _ARCH_CACHE_H */
