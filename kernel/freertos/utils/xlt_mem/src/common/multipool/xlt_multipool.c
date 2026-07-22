/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: memory multi pool
 */

#include "xlt_config.h"
#ifdef XLTCFG_MEM_MUL_POOL
#include "xlt_memory.h"
#include "xlt_memory_pri.h"
static void *g_pool_head = NULL;

unsigned int os_mem_mul_pool_init(void *pool, unsigned int size)
{
    void *next_pool = g_pool_head;
    void *cur_pool = g_pool_head;
    UINTPTR pool_end;
    while (next_pool != NULL) {
        pool_end = (UINTPTR)next_pool + xlt_mem_pool_size_get(next_pool);
        if (((pool <= next_pool) && (((UINTPTR)pool + size) > (UINTPTR)next_pool)) ||
            (((UINTPTR)pool < pool_end) && (((UINTPTR)pool + size) >= pool_end))) {
            print_err("pool [%p, %p) conflict with pool [%p, %p)\n", pool, (UINTPTR)pool + size, next_pool,
                (UINTPTR)next_pool + xlt_mem_pool_size_get(next_pool));
            return XLT_NOK;
        }
        cur_pool = next_pool;
        next_pool = ((xlt_mem_pool_info *)next_pool)->next_pool;
    }

    if (g_pool_head == NULL) {
        g_pool_head = pool;
    } else {
        ((xlt_mem_pool_info *)cur_pool)->next_pool = pool;
    }

    ((xlt_mem_pool_info *)pool)->next_pool = NULL;
    return XLT_OK;
}

unsigned int os_mem_mul_pool_deinit(const void *pool)
{
    unsigned int ret = XLT_NOK;
    void *next_pool = NULL;
    void *cur_pool = NULL;

    do {
        if (pool == NULL) {
            break;
        }

        if (pool == g_pool_head) {
            g_pool_head = ((xlt_mem_pool_info *)g_pool_head)->next_pool;
            ret = XLT_OK;
            break;
        }

        cur_pool = g_pool_head;
        next_pool = g_pool_head;
        while (next_pool != NULL) {
            if (pool == next_pool) {
                ((xlt_mem_pool_info *)cur_pool)->next_pool = ((xlt_mem_pool_info *)next_pool)->next_pool;
                ret = XLT_OK;
                break;
            }
            cur_pool = next_pool;
            next_pool = ((xlt_mem_pool_info *)next_pool)->next_pool;
        }
    } while (0);

    return ret;
}

void *os_mem_mul_pool_head_get(void)
{
    return g_pool_head;
}

unsigned int xlt_mem_deinit(void *pool)
{
    unsigned int ret;
    unsigned int int_save;

    int_save = xlt_mem_lock();
    ret = os_mem_mul_pool_deinit(pool);
    xlt_mem_unlock(int_save);

    return ret;
}
#endif
