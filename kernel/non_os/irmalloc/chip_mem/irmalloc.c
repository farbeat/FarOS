/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: malloc function. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2018-10-15， Create file. \n
 */

#include "panic.h"
#include "chip_mem.h"
#include "securec.h"
#include "hal_sectors.h"
#include "irmalloc.h"

void irmalloc_init_default(void)
{
#ifndef __FREERTOS__
    // Give irmalloc the heap to allocate from
    irmalloc_new_pool((unsigned char *)g_hal_sectors_ram_heap_start, (uint32_t)g_hal_sectors_ram_heap_size);
#endif
}

/**
 * Add an additional memory pool to allocate from; the irmalloc implementation is not obliged to use this memory
 * @param start is the start memory address of the new pool
 * @param size is number of bytes in the new memory pool
 */
void irmalloc_new_pool(unsigned char *start, size_t size)
{
    int ret;
    ret = conn_mem_init_pool(start, (unsigned int)size);
    if (ret != 0) {
        panic(PANIC_IRMALLOC_FAIL, __LINE__);
    }
}

static void *irmalloc_internal(size_t size)
{
    void *mem = NULL;
#if defined(__GNUC__)
    mem = conn_mem_alloc_enhanced((unsigned long)(uintptr_t)__builtin_return_address(0), (uint32_t)size);
#elif defined(__ICCARM__)
    mem = conn_mem_alloc_enhanced((unsigned long)(uintptr_t)__get_LR(), (uint32_t)size);
#endif
    if (mem == NULL) {
        panic(PANIC_IRMALLOC_FAIL, __LINE__);
    }

    return mem;
}

void *irmalloc(size_t size)
{
    void *mem = NULL;
#ifdef __FREERTOS__
    mem = malloc(size);
#else
    mem = irmalloc_internal(size);
#endif
    return mem;
}

void *irmalloc_panicking(size_t size)
{
    void *mem;
#ifdef __FREERTOS__
    mem = malloc(size);
#else
    mem = irmalloc_internal(size);
#endif
    if (mem == NULL) {
        panic(PANIC_IRMALLOC_FAIL, (uint32_t)size);
    }

    return mem;
}

void *irzalloc(size_t size)
{
    void *buf;
#ifdef __FREERTOS__
    buf = malloc(size);
#else
    buf = irmalloc_internal(size);
#endif
    if (buf != NULL) {
        if (memset_s(buf, size, 0, size) != EOK) {
            return (void *)NULL;
        }
    }
    return buf;
}

void *irrealloc(void *buf, size_t size)
{
    void *mem = NULL;
#if defined(__GNUC__)
    mem = conn_mem_realloc_enhanced((unsigned long)(uintptr_t)__builtin_return_address(0), buf, (uint32_t)size);
#elif defined(__ICCARM__)
    mem = conn_mem_realloc_enhanced((unsigned long)(uintptr_t)__get_LR(), buf, (uint32_t)size);
#endif
    if (mem == NULL) {
        panic(PANIC_IRMALLOC_FAIL, __LINE__);
    }

    return mem;
}

void *irrealloc_panicking(void *buf, size_t size)
{
    void *mem = NULL;
#if defined(__GNUC__)
    mem = conn_mem_realloc_enhanced((unsigned long)(uintptr_t)__builtin_return_address(0), buf, (uint32_t)size);
#elif defined(__ICCARM__)
    mem = conn_mem_realloc_enhanced((unsigned long)(uintptr_t)__get_LR(), buf, (uint32_t)size);
#endif
    if (mem == NULL) {
        panic(PANIC_IRMALLOC_FAIL, (uint32_t)size);
    }

    return mem;
}

void irfree(void *buf)
{
    if (buf == NULL) {
        return;
    }

#ifdef __FREERTOS__
    free(buf);
#else
    conn_mem_free_enhanced(buf);
#endif
}

void *irmalloc_dup(const void *orig, size_t size)
{
    void *ret = NULL;

    if (orig == NULL) {
        return (void *)NULL;
    }

#ifdef __FREERTOS__
    ret = malloc(size);
#else
    ret = irmalloc_internal(size);
#endif
    if (ret == NULL) {
        return (void *)NULL;
    }

    if (memcpy_s(ret, size, orig, size) != EOK) {
        return (void *)NULL;
    }

    return ret;
}