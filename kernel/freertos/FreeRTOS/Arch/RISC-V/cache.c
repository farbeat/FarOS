#include "FreeRTOS.h"
#include "encoding.h"
#include "cache.h"
#include "barrier.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CACHE_PREF_LINES_MAX 7

uint32_t ArchICacheEnable(CacheSize icclSize)
{
    if ((icclSize != CACHE_4KB) && (icclSize != CACHE_8KB) &&
        (icclSize != CACHE_16KB) && (icclSize != CACHE_32KB)) {
        return pdFAIL;
    }

    write_custom_csr(ICCTL, (uint32_t)((uint32_t)icclSize | ICCTL_ENABLE));
    return pdPASS;
}

uint32_t ArchDCacheEnable(CacheSize dcclSize)
{
    if ((dcclSize != CACHE_4KB) && (dcclSize != CACHE_8KB) &&
        (dcclSize != CACHE_16KB) && (dcclSize != CACHE_32KB)) {
        return pdFAIL;
    }

    write_custom_csr(DCCTL, (uint32_t)((uint32_t)dcclSize | DCCTL_ENABLE));
    return pdPASS;
}

void ArchICacheInvAll(void)
{
    write_custom_csr(ICMAINT, ICIV);
}

void ArchDCacheInvAll(void)
{
    write_custom_csr(DCMAINT, DCIV);
}

uint32_t ArchICachePrefetchEnable(CachePrefLines iclValue)
{
    if (iclValue > CACHE_PREF_LINES_MAX) {
        return pdFAIL;
    }

    write_custom_csr(APREFI, (uint32_t)(((uint32_t)iclValue << 1) | IAPEN)); /* 1: ICL control bits */
    return pdPASS;
}

uint32_t ArchDCachePrefetchEnable(CachePrefLines iclValue, CachePrefLines sclValue)
{
    uint32_t value;

    if ((iclValue > CACHE_PREF_LINES_MAX) || (sclValue > CACHE_PREF_LINES_MAX)) {
        return pdFAIL;
    }

    value = ((uint32_t)iclValue << 1) | IAPEN;  /* 1: ICL control bits */
    value |= ((uint32_t)sclValue << 5) | SAPEN; /* 5: SCL control bits; */
    write_custom_csr(APREFD, value);
    return pdPASS;
}

void ArchICacheFlush(void)
{
    write_custom_csr(ICMAINT, ICACHE_BY_ALL);
    mb();
}

void ArchDCacheFlush(void)
{
    unsigned int irq_status = uxHwiLock();

    write_custom_csr(DCMAINT, DCACHE_FLUSH_ALL);
    mb();

    vHwiRestore(irq_status);
}

void ArchDCacheClean(void)
{
    write_custom_csr(DCMAINT, DCACHE_CLEAN_ALL);
    mb();
}

void ArchDCacheInv(void)
{
    write_custom_csr(DCMAINT, DCIV);
    mb();
}

void ArchICacheFlushByVa(uintptr_t baseAddr, uint32_t size)
{
    uint32_t flushNum, count;
    uintptr_t endAddr;

    endAddr = baseAddr + size;
    baseAddr = baseAddr & ~(CACHE_LINE_SIZE - 1);

    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - baseAddr;

    flushNum = size / CACHE_LINE_SIZE;
    for (count = 0; count < flushNum; count++) {
        write_custom_csr(ICINCVA, baseAddr);  /* write cmo's va */
        write_custom_csr(ICMAINT, ICACHE_BY_VA);  /* enabel cmo(clean and invalidate) by va */
        baseAddr += CACHE_LINE_SIZE;
    }

    mb();
}

static void DCacheOperate(uintptr_t baseAddr, uint32_t size, uint32_t config)
{
    uint32_t flushNum, count;
    uintptr_t endAddr;

    unsigned int irq_status = uxHwiLock();

    endAddr = baseAddr + size;
    baseAddr = baseAddr & ~(CACHE_LINE_SIZE - 1);

    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - baseAddr;

    flushNum = size / CACHE_LINE_SIZE;
    for (count = 0; count < flushNum; count++) {
        write_custom_csr(DCINCVA, baseAddr);  /* write cmo's va */
        write_custom_csr(DCMAINT, config);
        baseAddr += CACHE_LINE_SIZE;
    }

    mb();
    vHwiRestore(irq_status);
}

void ArchDCacheFlushByVa(uintptr_t baseAddr, uint32_t size)
{
    DCacheOperate(baseAddr, size, DCACHE_FLUSH_BY_VA);
}

void ArchDCacheInvByVa(uintptr_t baseAddr, uint32_t size)
{
    uintptr_t OpStartAddr = baseAddr & ~(CACHE_LINE_SIZE - 1);
    uintptr_t OpEndAddr   = (baseAddr + size) & ~(CACHE_LINE_SIZE - 1);;

    if (OpStartAddr != baseAddr) {
        DCacheOperate(OpStartAddr, CACHE_LINE_SIZE, DCACHE_CLEAN_BY_VA);
    }

    if (OpEndAddr != (baseAddr + size)) {
        DCacheOperate(OpEndAddr, CACHE_LINE_SIZE, DCACHE_CLEAN_BY_VA);
    }
    DCacheOperate(baseAddr, size, DCACHE_INV_BY_VA);
}

void ArchDCacheCleanByVa(uintptr_t baseAddr, uint32_t size)
{
    DCacheOperate(baseAddr, size, DCACHE_CLEAN_BY_VA);
}

void ArchICacheFlushByAddr(uintptr_t startAddr, uintptr_t endAddr)
{
    uint32_t size;

    startAddr = (uint32_t)startAddr & ~(CACHE_LINE_SIZE - 1);
    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - startAddr;

    ArchICacheFlushByVa(startAddr, size);
}

void ArchDCacheFlushByAddr(uintptr_t startAddr, uintptr_t endAddr)
{
    uint32_t size;

    startAddr = (uint32_t)startAddr & ~(CACHE_LINE_SIZE - 1);
    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - startAddr;

    ArchDCacheFlushByVa(startAddr, size);
}

void ArchDCacheCleanByAddr(uintptr_t startAddr, uintptr_t endAddr)
{
    uint32_t size;

    startAddr = (uint32_t)startAddr & ~(CACHE_LINE_SIZE - 1);
    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - startAddr;

    ArchDCacheCleanByVa(startAddr, size);
}

void ArchDCacheInvByAddr(uintptr_t startAddr, uintptr_t endAddr)
{
    uint32_t size;

    startAddr = (uint32_t)startAddr & ~(CACHE_LINE_SIZE - 1);
    endAddr = ALIGN(endAddr, CACHE_LINE_SIZE);
    size = endAddr - startAddr;

    DCacheOperate(startAddr, size, DCACHE_INV_BY_VA);
}

void dma_cache_clean(uintptr_t start, uintptr_t end)
{
    ArchDCacheCleanByAddr(start, end);
}

void dma_cache_inv(uintptr_t start, uintptr_t end)
{
    ArchDCacheInvByAddr(start, end);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
