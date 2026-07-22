/*
 * Copyright (c) CompanyNameMagicTag 2012-2022. All rights reserved.
 */

#include <FreeRTOS.h>
#include <portable.h>
#ifdef XLTCFG_SUPPORT_MEMMNG
#include <xlt_memory.h>
#endif
#include <securec.h>
#include "soc_osal.h"
#include "osal_inner.h"

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag)
{
    osal_unused(osal_gfp_flag);
    return pvPortMalloc(size);
}

void *osal_kmalloc_align(unsigned int size, unsigned int osal_gfp_flag, unsigned int boundary)
{
    osal_unused(osal_gfp_flag);
#ifdef XLTCFG_SUPPORT_MEMMNG
    return xlt_mem_alloc_align(OS_SYS_MEM_POOL, size, boundary);
#else
    if (boundary != portBYTE_ALIGNMENT) {
        return NULL;
    }
    return pvPortMalloc(size);
#endif
}

void osal_kfree(void *addr)
{
    vPortFree(addr);
}

void *osal_vmalloc(unsigned long size)
{
    return pvPortMalloc(size);
}

void osal_vfree(void *addr)
{
    vPortFree(addr);
}

int osal_pool_mem_init(void *pool, unsigned int size)
{
#ifdef XLTCFG_SUPPORT_MEMMNG
    int ret = xlt_mem_init(pool, size);
    if (ret != XLT_OK) {
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
#else
    osal_unused(pool);
    osal_unused(size);
    return OSAL_FAILURE;
#endif
}

void *osal_pool_mem_alloc(void *pool, unsigned int size)
{
#ifdef XLTCFG_SUPPORT_MEMMNG
    return xlt_mem_alloc(pool, size);
#else
    osal_unused(pool);
    osal_unused(size);
    return NULL;
#endif
}

void *osal_pool_mem_alloc_align(void *pool, unsigned int size, unsigned int boundary)
{
#ifdef XLTCFG_SUPPORT_MEMMNG
    return xlt_mem_alloc_align(pool, size, boundary);
#else
    osal_unused(pool);
    osal_unused(size);
    osal_unused(boundary);
    return NULL;
#endif
}

void osal_pool_mem_free(void *pool, const void *addr)
{
#ifdef XLTCFG_SUPPORT_MEMMNG
    (void)xlt_mem_free(pool, (void *)addr);
#else
    osal_unused(pool);
    osal_unused(addr);
#endif
}

int osal_pool_mem_deinit(void *pool)
{
#if (defined XLTCFG_SUPPORT_MEMMNG) && (defined XLTCFG_MEM_MUL_POOL)
    int ret = xlt_mem_deinit(pool);
    if (ret != XLT_OK) {
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
#else
    osal_unused(pool);
    return OSAL_FAILURE;
#endif
}

unsigned long osal_copy_from_user(void *to, const void *from, unsigned long n)
{
    if (to == NULL || from == NULL) {
        return n;
    }
    errno_t ret = memcpy_s(to, n, from, n);
    return (ret == EOK) ? 0 : n;
}

unsigned long osal_copy_to_user(void *to, const void *from, unsigned long n)
{
    if (to == NULL || from == NULL) {
        return n;
    }
    errno_t ret = memcpy_s(to, n, from, n);
    return (ret == EOK) ? 0 : n;
}

void *osal_ioremap_nocache(unsigned long phys_addr, unsigned long size)
{
    osal_unused(size);
    return (void *)(uintptr_t)phys_addr;
}

void osal_iounmap(void *addr, unsigned long size)
{
    osal_unused(addr, size);
}

void *osal_phys_to_virt(unsigned long addr)
{
    return (void *)(uintptr_t)addr;
}
