/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Mem Pool Driver Header. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2018-10-15， Create file. \n
 */

#include <stdio.h>
#include <string.h>
#include "chip_mem.h"

static unsigned char *g_mem_base = NULL; /* Memory Pool start address */
static unsigned int g_mem_max_size = 0;  /* Maximum memory pool size */
static unsigned char *g_mem_curr_pos = NULL;

/**
 * Memory pool information global variable, which stores all memory pool information in the entire memory management
 * All memory management functions operate based on this global variable,
 * except for the netbuf memory pool management structure
 */
static conn_mem_pool_stru g_ast_mem_pool[CONN_MEM_POOL_ID_BUTT];

/* Local data memory pool configuration information global variables */
/* The actual size of each memory footprint is
 * Playload_Size + mem_info_size + dog_tag_size + sizeof(conn_mem_stru *) + sizeof(conn_mem_stru)
 */
#define conn_mem_pool_size(size) ((size) + CONN_MEM_INFO_SIZE + CONN_DOG_TAG_SIZE)

static conn_mem_subpool_cfg_stru g_ast_local_cfg_table[] = {
#if CHIP_BS25
    { conn_mem_pool_size(32),    256 }, /* First level size, number of blocks */
    { conn_mem_pool_size(64),    256 }, /* Second level size, number of blocks */
    { conn_mem_pool_size(128),   64 },
#else
    { conn_mem_pool_size(32),    512 }, /* First level size, number of blocks */
    { conn_mem_pool_size(64),    512 }, /* Second level size, number of blocks */
    { conn_mem_pool_size(128),   128 },
#endif
    { conn_mem_pool_size(1024),  32 },
    { conn_mem_pool_size(4096),  8 },
    { conn_mem_pool_size(8192),  2 },
};

/* Global variables for naked system total memory pool configuration information */
static conn_mem_pool_cfg_stru g_ast_mem_pool_cfg_table[] = {
    { CONN_MEM_POOL_ID_LOCAL,                  // Memory Pool ID
      conn_array_size(g_ast_local_cfg_table),  // Number of memory pools
      { 0, 0 },                                  // Four-byte aligned
      g_ast_local_cfg_table },                   // Memory pool configuration information
};

static unsigned char *conn_mem_ctrl_blk_alloc(unsigned int ul_size)
{
    unsigned char *puc_alloc = NULL;

    ul_size = conn_get_4byte_align_value(ul_size);
    if ((g_mem_curr_pos + ul_size) > (g_mem_base + g_mem_max_size - 1)) {
        CONN_IO_PRINT("conn_mem_ctrl_blk_alloc failed,request size=%u, max_size=%u\n", ul_size, g_mem_max_size);
        return NULL;
    }

    puc_alloc = g_mem_curr_pos;
    g_mem_curr_pos += ul_size;

    return puc_alloc;
}

static conn_mem_pool_stru *conn_mem_get_pool(unsigned int en_pool_id)
{
    if (conn_unlikely(en_pool_id >= CONN_MEM_POOL_ID_BUTT)) {
        CONN_IO_PRINT("[file = %s, line = %d], conn_mem_get_pool, array overflow!\n", __FILE__, __LINE__);
        return NULL;
    }

    return &g_ast_mem_pool[en_pool_id];
}

/**
 * Prototype    : conn_mem_get_pool_cfg_table
 * Description  : Get the corresponding memory pool configuration information based on the memory pool ID
 * Input        : en_pool_id: the memory pool ID
 * Return Value : success : Address corresponding to the memory pool configuration information structure
 *                failure: Null pointer
 */
static conn_mem_pool_cfg_stru *conn_mem_get_pool_cfg_table(unsigned char en_pool_id)
{
    if (conn_unlikely(en_pool_id >= CONN_MEM_POOL_ID_BUTT)) {
        CONN_IO_PRINT("[line = %d], conn_mem_get_pool_cfg_table, array overflow!\n", __LINE__);
        return NULL;
    }

    return &g_ast_mem_pool_cfg_table[en_pool_id];
}

void conn_mem_print_pool_info(void)
{
#ifdef SW_UART_DEBUG
    unsigned int pool_id;
    unsigned int subpool_id;
    conn_mem_pool_stru *pst_mem_pool = NULL;
    conn_mem_subpool_stru *pst_mem_subpool = NULL;

    for (pool_id = 0; pool_id < CONN_MEM_POOL_ID_BUTT; pool_id++) {
        pst_mem_pool = conn_mem_get_pool(pool_id);
        if (pst_mem_pool == NULL) {
            CONN_IO_PRINT("[line = %d], conn_mem_create_subpool, pointer is NULL!\n", __LINE__);
            return;
        }

        for (subpool_id = 0; subpool_id < pst_mem_pool->uc_subpool_cnt; subpool_id++) {
            /* Get information about each level of sub memory pool */
            pst_mem_subpool = &(pst_mem_pool->ast_subpool_table[subpool_id]);

            CONN_IO_PRINT("pool_id=%u  pool_size=%u pool_used:%u\n",
                          pool_id, pst_mem_subpool->us_len,
                          pst_mem_subpool->us_total_cnt - pst_mem_subpool->us_free_cnt);
        }
    }
    UNUSED(pst_mem_subpool);
#endif
}

static int conn_mem_create_subpool(unsigned char en_pool_id)
{
    conn_mem_pool_stru *pst_mem_pool = NULL;
    conn_mem_subpool_stru *pst_mem_subpool = NULL;
    conn_mem_stru *pst_mem = NULL;
    conn_mem_stru **ppst_stack_mem;
    unsigned char uc_subpool_id;
    unsigned int ul_blk_id;

    pst_mem_pool = conn_mem_get_pool(en_pool_id);
    if (pst_mem_pool == NULL) { return -1; }

    /*
     * Apply for an available memory address index table, apply once for each memory pool,
     * and then divide it for each child memory pool
     */
    ppst_stack_mem = (conn_mem_stru **)conn_mem_ctrl_blk_alloc(sizeof(conn_mem_stru *) *
                                                                   pst_mem_pool->us_mem_total_cnt);
    /* Apply for memory from a piece of array space, no need to release when the application fails */
    if (ppst_stack_mem == NULL) { return -1; }

    /*
     * Apply for conn_mem_stru structure, apply once for each memory pool,
     * and then divide it for each child memory pool.
     */
    pst_mem = (conn_mem_stru *)conn_mem_ctrl_blk_alloc(sizeof(conn_mem_stru) * pst_mem_pool->us_mem_total_cnt);
    /* Apply for memory from a piece of array space, no need to release when the application fails */
    if (pst_mem == NULL) { return -1; }

    if (memset_s(ppst_stack_mem, sizeof(uintptr_t) * pst_mem_pool->us_mem_total_cnt,
                 0, sizeof(uintptr_t) * pst_mem_pool->us_mem_total_cnt) != EOK) { return -1; }
    if (memset_s((void *)pst_mem, sizeof(conn_mem_stru) * pst_mem_pool->us_mem_total_cnt,
                 0, sizeof(conn_mem_stru) * pst_mem_pool->us_mem_total_cnt) != EOK) { return -1; }

    /*
     * Record the pointer of the initial conn_mem_stru structure of this memory pool,
     * and use it when checking the memory information
     */
    pst_mem_pool->pst_mem_start_addr = pst_mem;

    /*
     * Set the structure information of all memory blocks in each subpool,
     * and establish the relationship between each memory block and the payload
     */
    for (uc_subpool_id = 0; uc_subpool_id < pst_mem_pool->uc_subpool_cnt; uc_subpool_id++) {
        /* Get information about each level of sub memory pool */
        pst_mem_subpool = &(pst_mem_pool->ast_subpool_table[uc_subpool_id]);

        /* Establish the relationship between the sub memory number index table and the available memory index table */
        pst_mem_subpool->ppst_free_stack = (void **)ppst_stack_mem;

        for (ul_blk_id = 0; ul_blk_id < pst_mem_subpool->us_total_cnt; ul_blk_id++) {
            if ((g_mem_curr_pos + pst_mem_subpool->us_len) > (g_mem_base + g_mem_max_size - 1)) {
                CONN_IO_PRINT("pst_mem overflow, curr size=%u ,subpool size=%u, \
                              blk_id=%u max_size=%u, free size = %u\n",
                              g_mem_curr_pos - g_mem_base, pst_mem_subpool->us_len,
                              ul_blk_id, g_mem_max_size,
                              g_mem_max_size - ((uint32_t)g_mem_curr_pos - (uint32_t)g_mem_base));
                return -1;
            }

            pst_mem->en_pool_id = en_pool_id;
            pst_mem->uc_subpool_id = uc_subpool_id;
            pst_mem->us_len = pst_mem_subpool->us_len;
            pst_mem->en_mem_state_flag = CONN_MEM_STATE_FREE;
            pst_mem->uc_user_cnt = 0;
            /* Establish the relationship between conn_mem_st and the corresponding payload */
            pst_mem->puc_origin_data = g_mem_curr_pos;
            pst_mem->puc_data = pst_mem->puc_origin_data + CONN_MEM_INFO_SIZE;
            *(unsigned long *)pst_mem->puc_origin_data = (unsigned long)(uintptr_t)pst_mem;

            *ppst_stack_mem = pst_mem;
            ppst_stack_mem++;
            pst_mem++;

            g_mem_curr_pos += pst_mem_subpool->us_len;
        }
    }

    return 0;
}

static int conn_mem_create_pool(unsigned char en_pool_id)
{
    unsigned char uc_subpool_id;
    unsigned char uc_subpool_cnt;
    conn_mem_pool_stru *pst_mem_pool = NULL;
    conn_mem_subpool_stru *pst_mem_subpool = NULL;
    const conn_mem_pool_cfg_stru *pst_mem_pool_cfg = NULL;
    const conn_mem_subpool_cfg_stru *pst_mem_subpool_cfg = NULL;

    /* Entry judgment */
    if (en_pool_id >= CONN_MEM_POOL_ID_BUTT) {
        CONN_IO_PRINT("[line = %d], conn_mem_create_pool, array overflow!\n", __LINE__);
        return -1;
    }

    pst_mem_pool = conn_mem_get_pool(en_pool_id);
    if (pst_mem_pool == NULL) {
        CONN_IO_PRINT("[line = %d], conn_mem_create_pool, pointer is NULL!\n", __LINE__);
        return -1;
    }

    pst_mem_pool_cfg = conn_mem_get_pool_cfg_table(en_pool_id);
    if (pst_mem_pool_cfg == NULL) {
        CONN_IO_PRINT("[line = %d], conn_mem_create_pool, pointer is NULL!\n", __LINE__);
        return -1;
    }

    /* Initialize the common variables of the memory pool */
    uc_subpool_cnt = pst_mem_pool_cfg->uc_subpool_cnt;

    pst_mem_pool->uc_subpool_cnt = uc_subpool_cnt;
    pst_mem_pool->us_mem_used_cnt = 0;

    /* Maximum length that can be applied for each level of subpool */
    pst_mem_pool->us_max_byte_len = pst_mem_pool_cfg->pst_subpool_cfg_info[uc_subpool_cnt - 1].us_size;

    /* Child pool index table is initially cleared */
    if (memset_s((void *)pst_mem_pool->ast_subpool_table, sizeof(pst_mem_pool->ast_subpool_table),
                 0, sizeof(pst_mem_pool->ast_subpool_table)) != EOK) {
        return -1;
    }
    /* Set each level of sub memory pool */
    for (uc_subpool_id = 0; uc_subpool_id < uc_subpool_cnt; uc_subpool_id++) {
        pst_mem_subpool_cfg = pst_mem_pool_cfg->pst_subpool_cfg_info + uc_subpool_id;
        pst_mem_subpool = &(pst_mem_pool->ast_subpool_table[uc_subpool_id]);

        pst_mem_subpool->us_free_cnt = pst_mem_subpool_cfg->us_cnt;
        pst_mem_subpool->us_total_cnt = pst_mem_subpool_cfg->us_cnt;
        pst_mem_subpool->us_len = pst_mem_subpool_cfg->us_size;

        pst_mem_pool->us_mem_total_cnt += pst_mem_subpool_cfg->us_cnt; /* Set the total number of memory blocks */
    }

    /* Create a normal memory pool */
    return conn_mem_create_subpool(en_pool_id);
}

static int conn_mem_exit(void)
{
    if (memset_s(g_ast_mem_pool, sizeof(g_ast_mem_pool), 0, sizeof(g_ast_mem_pool)) != EOK) {
        return -1;
    }
    return 0;
}

static int conn_mem_init_globals(void)
{
    int ul_ret;
    ul_ret = conn_mem_exit();
    return ul_ret;
}

int conn_mem_init_pool(unsigned char *start, unsigned int size)
{
    unsigned int ul_pool_id;
    int ul_ret;

    if ((start == NULL) || (size == 0)) {
        CONN_IO_PRINT("invalid input, size=%u\r\n", size);
        conn_bug_on(1);
        return -1;
    }

    if ((((unsigned long)(uintptr_t)start) & 0x3) != 0) {
        CONN_IO_PRINT("start address unaligned\r\n");
        conn_bug_on(1);
        return -1;
    }

    if (sizeof(conn_mem_stru *) > CONN_MEM_INFO_SIZE) {
        CONN_IO_PRINT("mem info size too small %d , ptr=%u\n", CONN_MEM_INFO_SIZE, sizeof(conn_mem_stru *));
        conn_bug_on(1);
        return -1;
    }

    g_mem_base = start;
    g_mem_max_size = size;

    g_mem_curr_pos = start;

    /* Initialize memory pool global variables */
    ul_ret = conn_mem_init_globals();
    if (ul_ret != 0) {
        CONN_IO_PRINT("[line = %d], conn_mem_init_pool, conn_mem_init_globals failed!\r\n", __LINE__);
    }

    for (ul_pool_id = 0; ul_pool_id < CONN_MEM_POOL_ID_BUTT; ul_pool_id++) {
        ul_ret = conn_mem_create_pool((unsigned char)ul_pool_id);
        if (ul_ret != 0) {
            CONN_IO_PRINT("[line = %d], conn_mem_init_pool, conn_mem_create_pool failed!\n", __LINE__);
            return ul_ret;
        }
    }

    CONN_IO_PRINT("actual mempool size=%u, free size=%u max_size=%u\n",
                  g_mem_curr_pos - g_mem_base,
                  g_mem_max_size - ((uint32_t)g_mem_curr_pos - (uint32_t)g_mem_base),
                  g_mem_max_size);
    CONN_IO_PRINT("sizeof(conn_mem_stru *)=%u sizeof(conn_mem_stru)=%u\n", sizeof(conn_mem_stru *),
                  sizeof(conn_mem_stru));
    CONN_IO_PRINT("mem manage cost size %u\n",
                  CONN_MEM_INFO_SIZE + CONN_DOG_TAG_SIZE + sizeof(conn_mem_stru *) + sizeof(conn_mem_stru));

    return 0;
}

void *conn_mem_alloc_enhanced(unsigned long return_addr,
                              unsigned int us_len)
{
    conn_mem_pool_stru *pst_mem_pool = NULL;
    conn_mem_subpool_stru *pst_mem_subpool = NULL;
    conn_mem_stru *pst_mem = NULL;
    unsigned char uc_subpool_id;

    /* Get memory pool */
    pst_mem_pool = &g_ast_mem_pool[CONN_MEM_POOL_ID_LOCAL];
#if defined(_PRE_MEM_DEBUG_MODE)
    us_len += CONN_DOG_TAG_SIZE + CONN_MEM_INFO_SIZE;
#else
    us_len += CONN_MEM_INFO_SIZE;
#endif

    /* abnormal: Application length is not in the memory pool */
    if (conn_unlikely(us_len > pst_mem_pool->us_max_byte_len)) {
        return NULL;
    }

    for (uc_subpool_id = 0; uc_subpool_id < pst_mem_pool->uc_subpool_cnt; uc_subpool_id++) {
        pst_mem_subpool = &(pst_mem_pool->ast_subpool_table[uc_subpool_id]);

        conn_irq_disable();  // Lock

        if ((us_len > pst_mem_subpool->us_len) || (pst_mem_subpool->us_free_cnt == 0)) {
            conn_irq_enable();  // Unlock
            continue;
        }

        /* Get an unused conn_mem_stru node */
        pst_mem_subpool->us_free_cnt--;
        pst_mem = (conn_mem_stru *)pst_mem_subpool->ppst_free_stack[pst_mem_subpool->us_free_cnt];

        if (pst_mem == NULL) {
            conn_irq_enable();  // Lock
            conn_bug_on(pst_mem == NULL);
            continue;
        }

        pst_mem->puc_data = pst_mem->puc_origin_data;
        pst_mem->uc_user_cnt = 1;
        pst_mem->en_mem_state_flag = CONN_MEM_STATE_ALLOC;

        pst_mem_pool->us_mem_used_cnt++;

#if defined(_PRE_MEM_DEBUG_MODE)
        /* Set up dog tags */
        *((unsigned int *)(pst_mem->puc_origin_data + pst_mem->us_len - CONN_DOG_TAG_SIZE)) =
            (unsigned int)CONN_DOG_TAG;
        pst_mem->ul_return_addr = return_addr;
#endif

        conn_irq_enable();  // Unlock

        break;
    }

    if (pst_mem == NULL) {
        return NULL;
    }

#ifdef CONN_MEM_TRACE_DEBUG
    CONN_IO_PRINT("himem alloc %x %d" NEWLINE, (unsigned long)pst_mem->puc_data + CONN_MEM_INFO_SIZE, us_len);
#endif
    return (pst_mem->puc_data + CONN_MEM_INFO_SIZE);
}

static conn_mem_stru *conn_mem_get_pst_mem(const void * const free_mem)
{
    conn_mem_stru *pst_mem = NULL;

    if (conn_unlikely((((unsigned char *)free_mem < g_mem_base) ||
                         ((unsigned char *)free_mem > (g_mem_base + g_mem_max_size))))) {
        CONN_IO_PRINT("invalid free mem\n");
        if (free_mem != NULL) {
            conn_bug_on(1);
        }

        return NULL;
    }

    pst_mem = (conn_mem_stru *)(uintptr_t)(*(unsigned long *)((unsigned char *)free_mem - CONN_MEM_INFO_SIZE));
    if (conn_unlikely((((unsigned char *)pst_mem < g_mem_base) ||
                         ((unsigned char *)pst_mem > (g_mem_base + g_mem_max_size))))) {
        CONN_IO_PRINT("invalid pst mem \n");
        conn_bug_on(1);
        return NULL;
    }

    return pst_mem;
}

void conn_mem_free_enhanced(const void *free_mem)
{
    conn_mem_stru *pst_mem;
    conn_mem_pool_stru *pst_mem_pool = NULL;
    conn_mem_subpool_stru *pst_mem_subpool = NULL;
#if defined(_PRE_MEM_DEBUG_MODE)
    unsigned int ul_dog_tag;
#endif

    pst_mem = conn_mem_get_pst_mem(free_mem);
    if (pst_mem == NULL) {
        return;
    }

    pst_mem_pool = &g_ast_mem_pool[CONN_MEM_POOL_ID_LOCAL];

    pst_mem_subpool = &(pst_mem_pool->ast_subpool_table[pst_mem->uc_subpool_id]);

    conn_irq_disable();  // Lock

    /* abnormal: Memory write out of bounds */
#if defined(_PRE_MEM_DEBUG_MODE)
    ul_dog_tag = (*((unsigned int *)(pst_mem->puc_origin_data + pst_mem->us_len - CONN_DOG_TAG_SIZE)));

    if (ul_dog_tag != CONN_DOG_TAG) {
        conn_irq_enable();
        conn_bug_on(1);
        return;
    }

#endif

    /* abnormal: Free a piece of memory that has been freed */
    /* abnormal: Free a block of memory with a reference count of 0 */
    if (conn_unlikely(pst_mem->en_mem_state_flag == CONN_MEM_STATE_FREE) ||
        conn_unlikely(pst_mem->uc_user_cnt == 0)) {
        conn_irq_enable();
        conn_bug_on(1);
        return;
    }

    pst_mem->uc_user_cnt--;

    /* If there are other shared users on this memory block, return directly */
    if (pst_mem->uc_user_cnt != 0) {
        conn_irq_enable();  // Unlock
        return;
    }

    /*
     * abnormal: The number of available memory blocks in this sub memory pool exceeds
     *           the total number of memory blocks in the entire sub memory pool
     */
    if (conn_unlikely(pst_mem_subpool->us_free_cnt >= pst_mem_subpool->us_total_cnt)) {
        conn_irq_enable();  // Unlock
        conn_bug_on(1);
        return;
    }

    pst_mem->en_mem_state_flag = CONN_MEM_STATE_FREE;

    pst_mem_subpool->ppst_free_stack[pst_mem_subpool->us_free_cnt] = (void *)pst_mem;
    pst_mem_subpool->us_free_cnt++;

    pst_mem_pool->us_mem_used_cnt--;
#ifdef CONN_MEM_TRACE_DEBUG
    CONN_IO_PRINT("himem free %x" NEWLINE, free_mem);
#endif
    conn_irq_enable();  // Unlock

    return;
}

void *conn_mem_realloc_enhanced(unsigned long return_addr, const void *mem, unsigned int us_len)
{
    void *nbuf;
    unsigned int osize;
    conn_mem_stru *pst_mem = NULL;

    nbuf = conn_mem_alloc_enhanced(return_addr, us_len);
    if (conn_unlikely(nbuf == NULL)) {
        return NULL;
    }

    if (mem == NULL) {
        return nbuf;
    }

    pst_mem = conn_mem_get_pst_mem(mem);
    if (pst_mem == NULL) {
        return NULL;
    }

#if defined(_PRE_MEM_DEBUG_MODE)
    osize = pst_mem->us_len - CONN_DOG_TAG_SIZE - CONN_MEM_INFO_SIZE;
#else
    osize = pst_mem->us_len - CONN_MEM_INFO_SIZE;
#endif

    if (memcpy_s((char *)nbuf, us_len, (char *)mem, ((us_len < osize) ? us_len : osize)) != EOK) {
        return NULL;
    }

    conn_mem_free_enhanced(mem);
#ifdef CONN_MEM_TRACE_DEBUG
    CONN_IO_PRINT("himem realloc %x %d osize=%u" NEWLINE, (unsigned int *)nbuf, us_len, osize);
#endif
    return nbuf;
}

