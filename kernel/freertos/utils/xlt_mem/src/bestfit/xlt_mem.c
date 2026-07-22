/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: mem module implementation
 */

#ifdef XLTCFG_LIB_LIBC
#include "stdio.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <securec.h>
/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "xlt_memory.h"
#include "xlt_memory_internal.h"
#include "xlt_multipledlinkhead_pri.h"
#include "xlt_slab_pri.h"
#include "xlt_typedef.h"

void xlt_printk(const char *fmt, ...)
{
#if defined(__FREERTOS__)
    va_list ap;
    va_start(ap, fmt);
    vUartVprintk(ePrintkErr, fmt, ap);
    va_end(ap);
#endif
}

#define COLUMN_NUM 8 /* column num of the output info of mem node */

static void os_check_null_return(void *param)
{
    do {
        if ((param) == NULL) {
            print_err("%s %d\n", __FUNCTION__, OS_DEBUG_LINE);
            return;
        }
    } while (0);
}

#ifdef XLTCFG_MEM_MUL_POOL
os_mem_mul_pool_ops g_mul_pool_ops = {};
#endif

os_memory_ops g_memory_ops = {};

#ifdef XLTCFG_BASE_MEM_NODE_SIZE_CHECK
static unsigned char g_mem_check_level = XLT_MEM_CHECK_LEVEL_DEFAULT;
#endif

#ifdef XLTCFG_MEM_MUL_MODULE
#if defined(__LITEOS__)
unsigned int *g_module_mem_used_size = NULL;
#else
unsigned int g_module_mem_used_size[MEM_MODULE_MAX + 1] = { 0 };
#endif
#endif


static void *os_mem_alloc_with_check(xlt_mem_pool_info *pool, unsigned int size);

static inline void os_mem_taskid_set(xlt_mem_dyn_node *node, unsigned int task_id)
{
    node->self_node.task_id = task_id;
}


#ifdef XLTCFG_MEM_MUL_MODULE

static void os_mem_modid_set(xlt_mem_dyn_node *node, unsigned int module_id)
{
    node->self_node.module_id = module_id;
}

static unsigned int os_mem_modid_get(const xlt_mem_dyn_node *node)
{
    return (unsigned int)node->self_node.module_id;
}

#endif

/*
 * Description : set magic & taskid
 * Input       : node -- the node which will be set magic & taskid
 */
static void os_mem_set_magic_num_and_task_id(xlt_mem_dyn_node *node)
{
    os_mem_taskid_set(node, (unsigned int)xTaskGetCurrentTaskHandle());
    xlt_mem_set_magic(node->self_node.magic);
}

#ifdef XLTCFG_BASE_MEM_NODE_SIZE_CHECK
const void *os_mem_find_node_ctrl(const void *pool, const void *ptr);
#endif

static void os_mem_list_delete(xlt_dl_list *node, const void *first_node)
{
    (void)first_node;
    node->pst_next->pst_prev = node->pst_prev;
    node->pst_prev->pst_next = node->pst_next;
    node->pst_next = NULL;
    node->pst_prev = NULL;
}

static void os_mem_list_add(xlt_dl_list *list_node, xlt_dl_list *node, const void *first_node)
{
    (void)first_node;
    node->pst_next = list_node->pst_next;
    node->pst_prev = list_node;
    list_node->pst_next->pst_prev = node;
    list_node->pst_next = node;
}

#ifdef XLTCFG_MEM_SLAB_EXTENTION
static void *os_mem_realloc_slab(void *pool, void *ptr, bool *is_slab_mem, UINT32 size, unsigned int *int_save)
{
    errno_t rc;
    UINT32 blk_sz;
    void *new_ptr = NULL;
    void *free_ptr = ptr;

    blk_sz = os_slab_mem_check(pool, ptr);
    if (blk_sz == (UINT32)-1) {
        *is_slab_mem = FALSE;
        return NULL;
    }
    *is_slab_mem = TRUE;

    if (size <= blk_sz) {
        return ptr;
    }

    /* Unlock the memory spin, to allow the memory alloc API to be called */
    xlt_mem_unlock(*int_save);

    new_ptr = xlt_mem_alloc(pool, size);
    if (new_ptr != NULL) {
        rc = memcpy_s(new_ptr, size, ptr, blk_sz);
        if (rc != EOK) {
            print_err("%s[%d] memcpy_s failed, error type = %d\n", __FUNCTION__, OS_DEBUG_LINE, rc);
            free_ptr = new_ptr;
            new_ptr = NULL;
        }
        if (xlt_mem_free((void *)pool, free_ptr) != XLT_OK) {
            print_err("%s, %d free failed\n", __FUNCTION__, OS_DEBUG_LINE);
        }
    }

    /* Reacquire the spin */
    *int_save = xlt_mem_lock();

    return new_ptr;
}

void *os_mem_alloc(void *pool, UINT32 size)
{
    return os_mem_alloc_with_check(pool, size);
}
#endif

/*
 * Description : find suitable free block use "best fit" algorithm
 * Input       : pool      --- Pointer to memory pool
 * alloc_size --- _size of memory in bytes which note need allocate
 * Return      : NULL      --- no suitable block found
 * tmp_node   --- pointer a suitable free block
 */
static xlt_mem_dyn_node *os_mem_find_suitable_free_block(void *pool, unsigned int alloc_size)
{
    xlt_dl_list *list_node_head = NULL;
    xlt_mem_dyn_node *tmp_node = NULL;

    for (list_node_head = xlt_mem_head(pool, alloc_size); list_node_head != NULL;
        list_node_head = os_dlnk_next_multi_head(xlt_mem_head_addr(pool), list_node_head)) {
        xlt_dl_list_for_each_entry(tmp_node, list_node_head, xlt_mem_dyn_node, self_node.free_node_info)
        {
#ifdef XLTCFG_MEM_DEBUG
            if (((UINTPTR)tmp_node < (UINTPTR)pool) ||
                ((UINTPTR)tmp_node > ((UINTPTR)pool + ((xlt_mem_pool_info *)pool)->pool_size)) ||
                (((UINTPTR)tmp_node & (XLT_MEM_ALIGN_SIZE - 1)) != 0)) {
                print_err("[%s:%d]Mem node data error:xlt_mem_head_addr(pool)=%p, list_node_head:%p,"
                    "alloc_size=%u, tmp_node=%p\n",
                    __FUNCTION__, OS_DEBUG_LINE, xlt_mem_head_addr(pool), list_node_head, alloc_size, tmp_node);
                break;
            }
#endif
            if (tmp_node->self_node.size_and_flag >= alloc_size)
                return tmp_node;
        }
    }

    return NULL;
}

/*
 * Description : clear a mem node, set every member to NULL
 * Input       : node    --- Pointer to the mem node which will be cleared up
 */
static void os_mem_clear_node(xlt_mem_dyn_node *node)
{
    (void)memset_s((void *)node, sizeof(xlt_mem_dyn_node), 0, sizeof(xlt_mem_dyn_node));
}

/*
 * Description : merge this node and pre node, then clear this node info
 * Input       : node    --- Pointer to node which will be merged
 */
static void os_mem_merge_node(xlt_mem_dyn_node *node)
{
    xlt_mem_dyn_node *next_node = NULL;

    node->self_node.pre_node->self_node.size_and_flag += node->self_node.size_and_flag;
    next_node = (xlt_mem_dyn_node *)((UINTPTR)node + node->self_node.size_and_flag);
    next_node->self_node.pre_node = node->self_node.pre_node;
    os_mem_clear_node(node);
}

/*
 * Description : split new node from alloc_node, and merge remainder mem if necessary
 * Input       : pool      -- Pointer to memory pool
 * alloc_node -- the source node which the new node will split from it.
 * After pick up it's node info, change to point to the new node
 * alloc_size -- the size of new node
 * Output      : alloc_node -- save new node addr
 */
static void os_mem_split_node(void *pool, xlt_mem_dyn_node *alloc_node, unsigned int alloc_size)
{
    xlt_mem_dyn_node *new_free_node = NULL;
    xlt_mem_dyn_node *next_node = NULL;
    xlt_dl_list *list_node_head = NULL;
    const void *first_node = (const void *)((unsigned char *)xlt_mem_head_addr(pool) + XLT_DLNK_HEAD_SIZE);

    new_free_node = (xlt_mem_dyn_node *)(void *)((unsigned char *)alloc_node + alloc_size);
    new_free_node->self_node.pre_node = alloc_node;
    new_free_node->self_node.size_and_flag = alloc_node->self_node.size_and_flag - alloc_size;
    alloc_node->self_node.size_and_flag = alloc_size;
    next_node = xlt_mem_next_node(new_free_node);
    next_node->self_node.pre_node = new_free_node;
    if (!xlt_mem_node_get_used_flag(next_node->self_node.size_and_flag)) {
        os_mem_list_delete(&next_node->self_node.free_node_info, first_node);
        os_mem_merge_node(next_node);
    }
    list_node_head = xlt_mem_head(pool, new_free_node->self_node.size_and_flag);
    os_check_null_return(list_node_head);

    os_mem_list_add(list_node_head, &new_free_node->self_node.free_node_info, first_node);
}

/*
 * Description : free the node from memory & if there are free node beside, merger them.
 * at last update "list_node_head' which saved all free node control head
 * Input       : node -- the node which need be freed
 * pool -- Pointer to memory pool
 */
static void os_mem_free_node(xlt_mem_dyn_node *node, xlt_mem_pool_info *pool)
{
    xlt_mem_dyn_node *next_node = NULL;
    xlt_dl_list *list_node_head = NULL;
    const void *first_node = (const void *)((unsigned char *)xlt_mem_head_addr(pool) + XLT_DLNK_HEAD_SIZE);
    node->self_node.size_and_flag = xlt_mem_node_get_size(node->self_node.size_and_flag);
    if ((node->self_node.pre_node != NULL) &&
        !xlt_mem_node_get_used_flag(node->self_node.pre_node->self_node.size_and_flag)) {
        xlt_mem_dyn_node *pre_node = node->self_node.pre_node;
        os_mem_merge_node(node);
        next_node = xlt_mem_next_node(pre_node);
        if (!xlt_mem_node_get_used_flag(next_node->self_node.size_and_flag)) {
            os_mem_list_delete(&next_node->self_node.free_node_info, first_node);
            os_mem_merge_node(next_node);
        }

        os_mem_list_delete(&(pre_node->self_node.free_node_info), first_node);
        list_node_head = xlt_mem_head(pool, pre_node->self_node.size_and_flag);
        os_check_null_return(list_node_head);

        os_mem_list_add(list_node_head, &pre_node->self_node.free_node_info, first_node);
    } else {
        next_node = xlt_mem_next_node(node);
        if (!xlt_mem_node_get_used_flag(next_node->self_node.size_and_flag)) {
            os_mem_list_delete(&next_node->self_node.free_node_info, first_node);
            os_mem_merge_node(next_node);
        }

        list_node_head = xlt_mem_head(pool, node->self_node.size_and_flag);
        os_check_null_return(list_node_head);

        os_mem_list_add(list_node_head, &node->self_node.free_node_info, first_node);
    }
}


#ifdef XLTCFG_MEM_DEBUG
/*
 * Description : check the result if pointer memory node belongs to pointer memory pool
 * Input       : pool -- Pointer to memory pool
 * node -- the node which need be checked
 * Return      : XLT_OK or XLT_NOK
 */
static bool os_mem_is_node_valid(const xlt_mem_dyn_node *node, const xlt_mem_dyn_node *start_node,
    const xlt_mem_dyn_node *end_node, const unsigned char *start_pool, const unsigned char *end_pool)
{
    if (!xlt_mem_middle_addr(start_node, node, end_node))
        return FALSE;
    if (xlt_mem_node_get_used_flag(node->self_node.size_and_flag)) {
        if (!xlt_mem_magic_valid(node->self_node.magic))
            return FALSE;
        return TRUE;
    }

    if (!xlt_mem_middle_addr_open_end(start_pool, node->self_node.free_node_info.pst_prev, end_pool))
        return FALSE;

    return TRUE;
}

static void os_mem_check_used_node(const void *pool, const xlt_mem_dyn_node *node)
{
    const xlt_mem_pool_info *pool_info = (const xlt_mem_pool_info *)pool;
    const xlt_mem_dyn_node *start_node = (const xlt_mem_dyn_node *)xlt_mem_first_node(pool);
    const xlt_mem_dyn_node *end_node = (const xlt_mem_dyn_node *)xlt_mem_end_node(pool, pool_info->pool_size);
    const unsigned char *end_pool = (const unsigned char *)pool + pool_info->pool_size;
    const xlt_mem_dyn_node *next_node = NULL;

    if ((!os_mem_is_node_valid(node, start_node, end_node, (unsigned char *)pool, end_pool)) ||
        (!xlt_mem_node_get_used_flag(node->self_node.size_and_flag)))
        print_err("The node:%x %x %d has been damaged1!\n", node, node->self_node.size_and_flag,
            xlt_mem_node_get_used_flag(node->self_node.size_and_flag));

    next_node = xlt_mem_next_node(node);
    if ((!os_mem_is_node_valid(next_node, start_node, end_node, (unsigned char *)pool, end_pool)) ||
        (next_node->self_node.pre_node != node))
        print_err("The node:%x has been damaged2!\n", next_node);

    if (node != start_node) {
        if ((!os_mem_is_node_valid(node->self_node.pre_node, start_node, end_node, (unsigned char *)pool, end_pool)) ||
            (xlt_mem_next_node(node->self_node.pre_node) != node))
            print_err("The node:%x has been damaged3!\n", node->self_node.pre_node);
    }
}
#else
static void os_mem_check_used_node(const void *pool, const xlt_mem_dyn_node *node) {}
#endif

static void os_mem_node_debug_operate(void *pool, xlt_mem_dyn_node *alloc_node, unsigned int size) {}

static void os_mem_info_alert(const void *pool, unsigned int alloc_size)
{
#ifdef XLTCFG_MEM_DEBUG
    print_err("---------------------------------------------------"
        "--------------------------------------------------------\n");
    os_mem_info_print_hook(pool);
    print_err("[%s] No suitable free block, require free node size: 0x%x\n", __FUNCTION__, alloc_size);
    print_err("----------------------------------------------------"
        "-------------------------------------------------------\n");
#endif
}

/*
 * Description : Allocate node from Memory pool
 * Input       : pool  --- Pointer to memory pool
 * size  --- _size of memory in bytes to allocate
 * Return      : Pointer to allocated memory
 */
static void *os_mem_alloc_with_check(xlt_mem_pool_info *pool, unsigned int size)
{
    xlt_mem_dyn_node *alloc_node = NULL;
    unsigned int alloc_size;
    xlt_mem_dyn_node *tmp_node = NULL;
    xlt_mem_dyn_node *pre_node = NULL;
    const void *first_node = (const void *)((unsigned char *)xlt_mem_head_addr(pool) + XLT_DLNK_HEAD_SIZE);

    if (os_mem_integrity_check_hook(pool, &tmp_node, &pre_node) != XLT_OK) {
        os_mem_integrity_check_error_hook(tmp_node, pre_node);
        return NULL;
    }
    alloc_size = xlt_mem_align(size + XLT_MEM_NODE_HEAD_SIZE, XLT_MEM_ALIGN_SIZE);
    alloc_node = os_mem_find_suitable_free_block(pool, alloc_size);
    if (alloc_node == NULL) {
        pool->alloc_fail_count++;
        os_mem_info_alert(pool, alloc_size);
        return NULL;
    }
    if ((alloc_size + XLT_MEM_NODE_HEAD_SIZE + XLT_MEM_ALIGN_SIZE) <= alloc_node->self_node.size_and_flag)
        os_mem_split_node(pool, alloc_node, alloc_size);

    os_mem_list_delete(&alloc_node->self_node.free_node_info, first_node);
    os_mem_set_magic_num_and_task_id(alloc_node);
    xlt_mem_node_set_used_flag(alloc_node->self_node.size_and_flag);
    os_mem_node_debug_operate(pool, alloc_node, size);
    return (alloc_node + 1);
}

/*
 * Description : reAlloc a smaller memory node
 * Input       : pool      --- Pointer to memory pool
 * alloc_size --- the size of new node which will be alloced
 * node      --- the node which will be realloced
 * node_size  --- the size of old node
 * Output      : node      --- pointer to the new node after realloc
 */
static void os_mem_realloc_smaller(xlt_mem_pool_info *pool, unsigned int alloc_size, xlt_mem_dyn_node *node,
    unsigned int node_size)
{
    if ((alloc_size + XLT_MEM_NODE_HEAD_SIZE + XLT_MEM_ALIGN_SIZE) <= node_size) {
        node->self_node.size_and_flag = node_size;
        os_mem_split_node(pool, node, alloc_size);
        xlt_mem_node_set_used_flag(node->self_node.size_and_flag);
    }
}

/*
 * Description : reAlloc a Bigger memory node after merge node and next_node
 * Input       : pool      --- Pointer to memory pool
 * alloc_size --- the size of new node which will be alloced
 * node      --- the node which will be realloced
 * node_size  --- the size of old node
 * next_node  --- pointer next node which will be merged
 * Output      : node      --- pointer to the new node after realloc
 */
static void os_mem_merge_node_for_realloc_bigger(xlt_mem_pool_info *pool, unsigned int alloc_size,
    xlt_mem_dyn_node *node, unsigned int node_size, xlt_mem_dyn_node *next_node)
{
    const void *first_node = (const void *)((unsigned char *)xlt_mem_head_addr(pool) + XLT_DLNK_HEAD_SIZE);

    node->self_node.size_and_flag = node_size;
    os_mem_list_delete(&next_node->self_node.free_node_info, first_node);
    os_mem_merge_node(next_node);

    if ((alloc_size + XLT_MEM_NODE_HEAD_SIZE + XLT_MEM_ALIGN_SIZE) <= node->self_node.size_and_flag)
        os_mem_split_node(pool, node, alloc_size);

    xlt_mem_node_set_used_flag(node->self_node.size_and_flag);
}

static unsigned int os_mem_init(void *pool, unsigned int size)
{
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    xlt_mem_dyn_node *new_node = NULL;
    xlt_mem_dyn_node *end_node = NULL;
    xlt_dl_list *list_node_head = NULL;
    unsigned int pool_size = size;

    pool_info->pool = pool;
    pool_info->pool_size = pool_size;

    os_dlnk_init_multi_head(xlt_mem_head_addr(pool));
    new_node = xlt_mem_first_node(pool);
    new_node->self_node.size_and_flag =
        (pool_size - (unsigned int)((UINTPTR)new_node - (UINTPTR)pool) - XLT_MEM_NODE_HEAD_SIZE);
    new_node->self_node.pre_node = (xlt_mem_dyn_node *)xlt_mem_end_node(pool, pool_size);
    list_node_head = xlt_mem_head(pool, new_node->self_node.size_and_flag);
    if (list_node_head == NULL)
        return XLT_NOK;

    xlt_list_tail_insert(list_node_head, &(new_node->self_node.free_node_info));
    end_node = (xlt_mem_dyn_node *)xlt_mem_end_node(pool, pool_size);
    (void)memset_s(end_node, sizeof(*end_node), 0, sizeof(*end_node));
    end_node->self_node.pre_node = new_node;
    end_node->self_node.size_and_flag = XLT_MEM_NODE_HEAD_SIZE;
    xlt_mem_node_set_used_flag(end_node->self_node.size_and_flag);
    os_mem_set_magic_num_and_task_id(end_node);

    return XLT_OK;
}


unsigned int xlt_mem_pool_init(void *pool, unsigned int size, unsigned int attr)
{
    unsigned int int_save;

    if ((pool == NULL) || (size < XLT_MEM_MIN_POOL_SIZE)) {
        print_err("size is smaller min pool size = %d\n", XLT_MEM_MIN_POOL_SIZE);
        return XLT_NOK;
    }

    if (!is_aligned(size, XLT_MEM_ALIGN_SIZE) || !is_aligned(pool, XLT_MEM_ALIGN_SIZE)) {
        print_warn("pool [%p, %p) size 0x%x should be aligned with XLT_MEM_ALIGN_SIZE\n", pool, (UINTPTR)pool + size,
            size);
        size = xlt_mem_align(size, XLT_MEM_ALIGN_SIZE) - XLT_MEM_ALIGN_SIZE;
    }
    int_save = (unsigned int)xlt_mem_lock();
#ifdef XLTCFG_MEM_MUL_POOL
    if (os_mem_mul_pool_init_hook(pool, size) != XLT_OK) {
        xlt_mem_unlock(int_save);
        return XLT_NOK;
    }
#endif

    if (os_mem_init(pool, size) != XLT_OK) {
#ifdef XLTCFG_MEM_MUL_POOL
        (void)os_mem_mul_pool_deinit_hook(pool);
#endif
        xlt_mem_unlock(int_save);
        return XLT_NOK;
    }

    os_slab_mem_proc_init_flag(pool, size, attr & XLT_MEM_INIT_ATTR_SLAB_MSK);
    xlt_mem_unlock(int_save);
    return XLT_OK;
}


unsigned int xlt_mem_init(void *pool, unsigned int size)
{
    return xlt_mem_pool_init(pool, size, XLT_MEM_INIT_ATTR_SLAB_MSK);
}

void *xlt_mem_alloc(void *pool, unsigned int size)
{
    void *ptr = NULL;
    unsigned int int_save;

    if ((pool == NULL) || (size == 0) || xlt_mem_node_get_used_flag(size) || xlt_mem_node_get_aligned_flag(size))
        return NULL;

    int_save = (unsigned int)xlt_mem_lock();
    ptr = os_slab_mem_alloc(pool, size);
    if (ptr == NULL) {
        ptr = os_mem_alloc_with_check(pool, size);
    }
    xlt_mem_unlock(int_save);
    return ptr;
}

void *xlt_mem_alloc_align(void *pool, unsigned int size, unsigned int boundary)
{
    unsigned int use_size;
    unsigned int gap_size;
    void *ptr = NULL;
    void *aligned_ptr = NULL;
    xlt_mem_dyn_node *alloc_node = NULL;
    unsigned int int_save;

    if ((pool == NULL) || (size == 0) || (boundary == 0) || !is_pow_two(boundary) ||
        !is_aligned(boundary, sizeof(void *)))
        return NULL;

    int_save = (unsigned int)xlt_mem_lock();
    do {
        /*
         * sizeof(gap_size) bytes stores offset between aligned_ptr and ptr,
         * the ptr has been XLT_MEM_ALIGN_SIZE(4 or 8) aligned, so maximum
         * offset between aligned_ptr and ptr is boundary - XLT_MEM_ALIGN_SIZE
         */
        if ((boundary - sizeof(gap_size)) > ((unsigned int)(-1) - size)) {
            break;
        }

        use_size = (size + boundary) - sizeof(gap_size);
        if (xlt_mem_node_get_used_flag(use_size) || xlt_mem_node_get_aligned_flag(use_size)) {
            break;
        }

        ptr = os_mem_alloc_with_check(pool, use_size);

        aligned_ptr = (void *)xlt_mem_align(ptr, boundary);
        if (ptr == aligned_ptr) {
            break;
        }

        /* store gap_size in address (ptr -4), it will be checked while free */
        gap_size = (unsigned int)((UINTPTR)aligned_ptr - (UINTPTR)ptr);
        alloc_node = (xlt_mem_dyn_node *)ptr - 1;
        xlt_mem_node_set_aligned_flag(alloc_node->self_node.size_and_flag);
        xlt_mem_node_set_aligned_flag(gap_size);
        *(unsigned int *)((UINTPTR)aligned_ptr - sizeof(gap_size)) = gap_size;
        ptr = aligned_ptr;
    } while (0);

    xlt_mem_unlock(int_save);
    return ptr;
}

static void os_do_mem_free(void *pool, const void *ptr, xlt_mem_dyn_node *node)
{
    os_mem_check_used_node(pool, node);

    os_mem_free_node(node, pool);
}

unsigned int os_mem_free(void *pool, const void *ptr)
{
    unsigned int ret = XLT_OK;
    unsigned int gap_size;
    xlt_mem_dyn_node *node = NULL;

    do {
        gap_size = *(unsigned int *)((UINTPTR)ptr - sizeof(unsigned int));
        if (xlt_mem_node_get_aligned_flag(gap_size) && xlt_mem_node_get_used_flag(gap_size)) {
            print_err("[%s:%d]gap_size:0x%x error\n", __FUNCTION__, OS_DEBUG_LINE, gap_size);
            goto OUT;
        }

        node = (xlt_mem_dyn_node *)((UINTPTR)ptr - XLT_MEM_NODE_HEAD_SIZE);

        if (xlt_mem_node_get_aligned_flag(gap_size) != 0) {
            gap_size = xlt_mem_node_get_aligned_gapsize(gap_size);
            if ((gap_size & (XLT_MEM_ALIGN_SIZE - 1)) || (gap_size > ((UINTPTR)ptr - XLT_MEM_NODE_HEAD_SIZE))) {
                print_err("illegal gap_size: 0x%x\n", gap_size);
                break;
            }
            node = (xlt_mem_dyn_node *)((UINTPTR)ptr - gap_size - XLT_MEM_NODE_HEAD_SIZE);
        }
        os_do_mem_free(pool, ptr, node);
    } while (0);
OUT:
    return ret;
}

unsigned int xlt_mem_free(void *pool, void *ptr)
{
    unsigned int ret;
    unsigned int int_save;
    if ((pool == NULL) || (ptr == NULL) || !is_aligned(pool, sizeof(void *)) || !is_aligned(ptr, sizeof(void *)))
        return XLT_NOK;

    int_save = (unsigned int)xlt_mem_lock();
    if (os_slab_mem_free(pool, ptr) == TRUE) {
        ret = XLT_OK;
        goto OUT;
    }
    ret = os_mem_free(pool, ptr);
OUT:
    xlt_mem_unlock(int_save);
    return ret;
}

static void *os_get_real_ptr(const void *pool, void *ptr)
{
    void *real_ptr = ptr;
    unsigned int gap_size = *((unsigned int *)((UINTPTR)ptr - sizeof(unsigned int)));

    if (xlt_mem_node_get_aligned_flag(gap_size) && xlt_mem_node_get_used_flag(gap_size)) {
        print_err("[%s:%d]gap_size:0x%x flag error\n", __FUNCTION__, OS_DEBUG_LINE, gap_size);
        return NULL;
    }
    if (xlt_mem_node_get_aligned_flag(gap_size) != 0) {
        gap_size = xlt_mem_node_get_aligned_gapsize(gap_size);
        if ((gap_size & (XLT_MEM_ALIGN_SIZE - 1)) ||
            (gap_size > ((UINTPTR)ptr - XLT_MEM_NODE_HEAD_SIZE - (UINTPTR)pool))) {
            print_err("[%s:%d]gap_size:0x%x error\n", __FUNCTION__, OS_DEBUG_LINE, gap_size);
            return NULL;
        }
        real_ptr = (void *)((UINTPTR)ptr - (UINTPTR)gap_size);
    }
    return real_ptr;
}

#define EOK 0
static void *os_mem_realloc(void *pool, void *ptr, unsigned int size)
{
    xlt_mem_dyn_node *node = NULL;
    xlt_mem_dyn_node *next_node = NULL;
    void *tmp_ptr = NULL;
    void *real_ptr = NULL;
    unsigned int node_size;
    unsigned int alloc_size = xlt_mem_align(size + XLT_MEM_NODE_HEAD_SIZE, XLT_MEM_ALIGN_SIZE);

    real_ptr = os_get_real_ptr(pool, ptr);
    if (real_ptr == NULL)
        return NULL;

    node = (xlt_mem_dyn_node *)((UINTPTR)real_ptr - XLT_MEM_NODE_HEAD_SIZE);
    os_mem_check_used_node(pool, node);

    node_size = xlt_mem_node_get_size(node->self_node.size_and_flag);
    if (node_size >= alloc_size) {
        os_mem_realloc_smaller(pool, alloc_size, node, node_size);
        return (void *)ptr;
    }

    next_node = xlt_mem_next_node(node);
    if (!xlt_mem_node_get_used_flag(next_node->self_node.size_and_flag) &&
        ((next_node->self_node.size_and_flag + node_size) >= alloc_size)) {
        os_mem_merge_node_for_realloc_bigger(pool, alloc_size, node, node_size, next_node);
        return (void *)ptr;
    }
    tmp_ptr = os_mem_alloc_with_check(pool, size);
    if (tmp_ptr != NULL) {
        unsigned int gap_size = (unsigned int)((UINTPTR)ptr - (UINTPTR)real_ptr);
        if (memcpy_s(tmp_ptr, size, ptr, (node_size - XLT_MEM_NODE_HEAD_SIZE - gap_size)) != EOK) {
            (void)os_mem_free((void *)pool, (void *)tmp_ptr);
            return NULL;
        }
        os_mem_free_node(node, pool);
    }
    return tmp_ptr;
}

void *xlt_mem_realloc(void *pool, void *ptr, unsigned int size)
{
    unsigned int int_save;
    void *new_ptr = NULL;

    if (xlt_mem_node_get_used_flag(size) || xlt_mem_node_get_aligned_flag(size) || (pool == NULL))
        return NULL;

    if (ptr == NULL) {
        new_ptr = xlt_mem_alloc(pool, size);
        goto OUT;
    }

    if (size == 0) {
        (void)xlt_mem_free(pool, ptr);
        goto OUT;
    }

    int_save = (unsigned int)xlt_mem_lock();

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    bool is_slab_mem = FALSE;
    new_ptr = os_mem_realloc_slab(pool, ptr, &is_slab_mem, size, &int_save);
    if (is_slab_mem == TRUE) {
        goto OUT_UNLOCK;
    }
#endif

    new_ptr = os_mem_realloc(pool, ptr, size);

#ifdef XLTCFG_MEM_SLAB_EXTENTION
OUT_UNLOCK:
#endif
    xlt_mem_unlock(int_save);
OUT:

    return new_ptr;
}


#ifdef XLTCFG_MEM_DEBUG
unsigned int xlt_mem_used_blks_get(void *pool)
{
    xlt_mem_dyn_node *tmp_node = NULL;
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned int blk_nums = 0;
    unsigned int int_save;

    if (pool == NULL)
        return OS_INVALID;

    int_save = xlt_mem_lock();

    for (tmp_node = xlt_mem_first_node(pool); tmp_node <= xlt_mem_end_node(pool, pool_info->pool_size);
        tmp_node = xlt_mem_next_node(tmp_node)) {
        if (xlt_mem_node_get_used_flag(tmp_node->self_node.size_and_flag))
            blk_nums++;
    }

    xlt_mem_unlock(int_save);

    return blk_nums;
}

unsigned int xlt_mem_free_blks_get(void *pool)
{
    xlt_mem_dyn_node *tmp_node = NULL;
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned int blk_nums = 0;
    unsigned int int_save;

    if (pool == NULL)
        return XLT_NOK;

    int_save = xlt_mem_lock();

    for (tmp_node = xlt_mem_first_node(pool); tmp_node <= xlt_mem_end_node(pool, pool_info->pool_size);
        tmp_node = xlt_mem_next_node(tmp_node)) {
        if (!xlt_mem_node_get_used_flag(tmp_node->self_node.size_and_flag))
            blk_nums++;
    }

    xlt_mem_unlock(int_save);

    return blk_nums;
}

UINTPTR xlt_mem_last_used_get(void *pool)
{
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    xlt_mem_dyn_node *node = NULL;

    if (pool == NULL)
        return XLT_NOK;

    node = xlt_mem_end_node(pool, pool_info->pool_size)->self_node.pre_node;
    if (xlt_mem_node_get_used_flag(node->self_node.size_and_flag)) {
        return (UINTPTR)((char *)node + xlt_mem_node_get_size(node->self_node.size_and_flag) +
            sizeof(xlt_mem_dyn_node));
    } else {
        return (UINTPTR)((char *)node + sizeof(xlt_mem_dyn_node));
    }
}
#endif

unsigned int xlt_mem_pool_size_get(const void *pool)
{
    if (pool == NULL)
        return XLT_NOK;

    return ((xlt_mem_pool_info *)pool)->pool_size;
}

#ifdef XLTCFG_MEM_DEBUG
static void os_show_free_node(unsigned int index, unsigned int length, const unsigned int *count_num)
{
    unsigned int count = 0;
    xlt_printk("\n    block size:  ");
    while (count < length) {
        xlt_printk("2^%-5u", (index + OS_MIN_MULTI_DLNK_LOG2 + count));
        count++;
    }
    xlt_printk("\n    node number: ");
    count = 0;
    while (count < length) {
        xlt_printk("  %-5u", count_num[count + index]);
        count++;
    }
}
#endif

#ifdef XLTCFG_MEM_DEBUG
unsigned int xlt_mem_free_node_show(void *pool)
{
    xlt_dl_list *list_node_head = NULL;
    xlt_multiple_dlink_head *head_addr = (xlt_multiple_dlink_head *)((UINTPTR)pool + sizeof(xlt_mem_pool_info));
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned int link_head_index;
    unsigned int count_num[OS_MULTI_DLNK_NUM] = { 0 };
    unsigned int int_save;

    if ((pool_info == NULL) || ((UINTPTR)pool != (UINTPTR)pool_info->pool)) {
        print_err("wrong mem pool addr: %p, line:%d\n", pool_info, OS_DEBUG_LINE);
        return XLT_NOK;
    }

    xlt_printk("\n   ************************ left free node number**********************");
    int_save = xlt_mem_lock();

    for (link_head_index = 0; link_head_index <= (OS_MULTI_DLNK_NUM - 1); link_head_index++) {
        list_node_head = head_addr->list_head[link_head_index].pst_next;
        while (list_node_head != &(head_addr->list_head[link_head_index])) {
            list_node_head = list_node_head->pst_next;
            count_num[link_head_index]++;
        }
    }

    link_head_index = 0;
    while (link_head_index < OS_MULTI_DLNK_NUM) {
        if (link_head_index + COLUMN_NUM < OS_MULTI_DLNK_NUM) {
            os_show_free_node(link_head_index, COLUMN_NUM, count_num);
            link_head_index += COLUMN_NUM;
        } else {
            os_show_free_node(link_head_index, (OS_MULTI_DLNK_NUM - 1 - link_head_index), count_num);
            break;
        }
    }

    xlt_mem_unlock(int_save);
    xlt_printk("\n   ********************************************************************\n\n");

    return XLT_OK;
}
#endif

#ifdef XLTCFG_BASE_MEM_NODE_SIZE_CHECK

unsigned int xlt_mem_node_size_check(void *pool, void *ptr, unsigned int *total_size, unsigned int *avail_size)
{
    const void *head = NULL;
    xlt_mem_pool_info *pool_info = (xlt_mem_pool_info *)pool;
    unsigned char *end_pool = NULL;

    if (g_mem_check_level == XLT_MEM_CHECK_LEVEL_DISABLE)
        return XLT_ERRNO_MEMCHECK_DISABLED;

    if ((pool == NULL) || (ptr == NULL) || (total_size == NULL) || (avail_size == NULL))
        return XLT_ERRNO_MEMCHECK_PARA_NULL;

    end_pool = (unsigned char *)pool + pool_info->pool_size;
    if (!(xlt_mem_middle_addr_open_end(pool, ptr, end_pool)))
        return XLT_ERRNO_MEMCHECK_OUTSIDE;

    if (g_mem_check_level == XLT_MEM_CHECK_LEVEL_HIGH) {
        head = os_mem_find_node_ctrl(pool, ptr);
        if ((head == NULL) || (xlt_mem_node_get_size(((xlt_mem_dyn_node *)head)->self_node.size_and_flag) <
            ((UINTPTR)ptr - (UINTPTR)head)))
            return XLT_ERRNO_MEMCHECK_NO_HEAD;

        *total_size =
            xlt_mem_node_get_size(((xlt_mem_dyn_node *)head)->self_node.size_and_flag - sizeof(xlt_mem_dyn_node));
        *avail_size =
            xlt_mem_node_get_size(((xlt_mem_dyn_node *)head)->self_node.size_and_flag - ((UINTPTR)ptr - (UINTPTR)head));
        return XLT_OK;
    }
    if (g_mem_check_level == XLT_MEM_CHECK_LEVEL_LOW) {
        if (ptr != (void *)xlt_mem_align(ptr, XLT_MEM_ALIGN_SIZE))
            return XLT_ERRNO_MEMCHECK_NO_HEAD;

        head = (const void *)((UINTPTR)ptr - sizeof(xlt_mem_dyn_node));
        if (xlt_mem_magic_valid(((xlt_mem_dyn_node *)head)->self_node.magic)) {
            *total_size =
                xlt_mem_node_get_size(((xlt_mem_dyn_node *)head)->self_node.size_and_flag - sizeof(xlt_mem_dyn_node));
            *avail_size =
                xlt_mem_node_get_size(((xlt_mem_dyn_node *)head)->self_node.size_and_flag - sizeof(xlt_mem_dyn_node));
            return XLT_OK;
        } else {
            return XLT_ERRNO_MEMCHECK_NO_HEAD;
        }
    }

    return XLT_ERRNO_MEMCHECK_WRONG_LEVEL;
}

/*
 * Description : get a pool's memCtrl
 * Input       : ptr -- point to source ptr
 * Return      : search forward for ptr's memCtrl or "NULL"
 * attention : this func couldn't ensure the return memCtrl belongs to ptr it just find forward the most nearly one
 */
const void *os_mem_find_node_ctrl(const void *pool, const void *ptr)
{
    const void *head = ptr;

    if (ptr == NULL)
        return NULL;

    head = (const void *)xlt_mem_align(head, XLT_MEM_ALIGN_SIZE);
    while (!xlt_mem_magic_valid(((xlt_mem_dyn_node *)head)->self_node.magic)) {
        head = (const void *)((unsigned char *)head - sizeof(char *));
        if (head <= pool)
            return NULL;
    }
    return head;
}

unsigned int xlt_mem_check_level_set(unsigned char check_level)
{
    if (check_level == XLT_MEM_CHECK_LEVEL_LOW) {
        xlt_printk("%s: XLT_MEM_CHECK_LEVEL_LOW \n", __FUNCTION__);
    } else if (check_level == XLT_MEM_CHECK_LEVEL_HIGH) {
        xlt_printk("%s: XLT_MEM_CHECK_LEVEL_HIGH \n", __FUNCTION__);
    } else if (check_level == XLT_MEM_CHECK_LEVEL_DISABLE) {
        xlt_printk("%s: XLT_MEM_CHECK_LEVEL_DISABLE \n", __FUNCTION__);
    } else {
        xlt_printk("%s: wrong param, setting failed !! \n", __FUNCTION__);
        return XLT_ERRNO_MEMCHECK_WRONG_LEVEL;
    }
    g_mem_check_level = check_level;
    return XLT_OK;
}

unsigned char xlt_mem_check_level_get(void)
{
    return g_mem_check_level;
}
#endif /* XLTCFG_BASE_MEM_NODE_SIZE_CHECK */

#ifdef XLTCFG_MEM_MUL_MODULE
static unsigned int os_mem_mod_check(unsigned int module_id)
{
    if (module_id > MEM_MODULE_MAX) {
        print_err("error module ID input!\n");
        return XLT_NOK;
    }
    return XLT_OK;
}

static void *os_mem_ptr_to_node(void *ptr)
{
    unsigned int gap_size;

    if ((UINTPTR)ptr & (XLT_MEM_ALIGN_SIZE - 1)) {
        print_err("[%s:%d]ptr:%p not align by 4byte\n", __FUNCTION__, OS_DEBUG_LINE, ptr);
        return NULL;
    }

    gap_size = *((unsigned int *)((UINTPTR)ptr - sizeof(unsigned int)));
    if (xlt_mem_node_get_aligned_flag(gap_size) && xlt_mem_node_get_used_flag(gap_size)) {
        print_err("[%s:%d]gap_size:0x%x flag error\n", __FUNCTION__, OS_DEBUG_LINE, gap_size);
        return NULL;
    }
    if (xlt_mem_node_get_aligned_flag(gap_size)) {
        gap_size = xlt_mem_node_get_aligned_gapsize(gap_size);
        if ((gap_size & (XLT_MEM_ALIGN_SIZE - 1)) || (gap_size > ((UINTPTR)ptr - XLT_MEM_NODE_HEAD_SIZE))) {
            print_err("[%s:%d]gap_size:0x%x error\n", __FUNCTION__, OS_DEBUG_LINE, gap_size);
            return NULL;
        }

        ptr = (void *)((UINTPTR)ptr - gap_size);
    }

    return (void *)((UINTPTR)ptr - XLT_MEM_NODE_HEAD_SIZE);
}

static unsigned int os_mem_node_size_get(void *ptr)
{
    xlt_mem_dyn_node *node = (xlt_mem_dyn_node *)os_mem_ptr_to_node(ptr);
    if (node == NULL)
        return 0;

    return xlt_mem_node_get_size(node->self_node.size_and_flag);
}

static void os_mem_set_module_info(void *ptr, unsigned int module_id)
{
    xlt_mem_dyn_node *node = NULL;

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    if (os_slab_set_module_info(ptr, g_module_mem_used_size, module_id) == TRUE) {
        return;
    }
#endif

    g_module_mem_used_size[module_id] += os_mem_node_size_get(ptr);
    node = (xlt_mem_dyn_node *)os_mem_ptr_to_node(ptr);
    if (node != NULL)
        os_mem_modid_set(node, module_id);
}

void *xlt_mem_malloc(void *pool, unsigned int size, unsigned int module_id)
{
    unsigned int int_save;
    void *ptr = NULL;

    if (os_mem_mod_check(module_id) == XLT_NOK)
        return NULL;

    ptr = xlt_mem_alloc(pool, size);
    if (ptr == NULL)
        return ptr;

    int_save = xlt_mem_lock();
    os_mem_set_module_info(ptr, module_id);
    xlt_mem_unlock(int_save);
    return ptr;
}

void *xlt_mem_malloc_align(void *pool, unsigned int size, unsigned int boundary, unsigned int module_id)
{
    unsigned int int_save;
    void *ptr = NULL;
    void *node = NULL;

    if (os_mem_mod_check(module_id) == XLT_NOK)
        return NULL;

    ptr = xlt_mem_alloc_align(pool, size, boundary);
    if (ptr != NULL) {
        int_save = xlt_mem_lock();
        g_module_mem_used_size[module_id] += os_mem_node_size_get(ptr);
        node = os_mem_ptr_to_node(ptr);
        if (node != NULL)
            os_mem_modid_set(node, module_id);

        xlt_mem_unlock(int_save);
    }

    return ptr;
}

static unsigned int os_mem_get_node_size_and_module_id(void *ptr, unsigned int *module_id, unsigned int *size)
{
#ifdef XLTCFG_MEM_SLAB_EXTENTION
    if (os_slab_get_node_size_and_module_id(ptr, module_id, size) == TRUE) {
        return XLT_OK;
    }
#endif
    xlt_mem_dyn_node *node = (xlt_mem_dyn_node *)os_mem_ptr_to_node(ptr);
    if (node == NULL)
        return XLT_NOK;

    *size = xlt_mem_node_get_size(node->self_node.size_and_flag);

    if (*module_id != os_mem_modid_get(node)) {
        print_err("node[%p] alloced in module %lu, but free in module %u\n", ptr, os_mem_modid_get(node), *module_id);
        *module_id = os_mem_modid_get(node);
    }

    return XLT_OK;
}

unsigned int xlt_mem_mfree(void *pool, void *ptr, unsigned int module_id)
{
    unsigned int int_save;
    unsigned int ret;
    unsigned int size;

    if ((os_mem_mod_check(module_id) == XLT_NOK) || (ptr == NULL) || (pool == NULL))
        return XLT_NOK;

    ret = os_mem_get_node_size_and_module_id(ptr, &module_id, &size);
    if (ret != XLT_OK)
        return ret;

    ret = xlt_mem_free(pool, ptr);
    if (ret == XLT_OK) {
        int_save = xlt_mem_lock();
        g_module_mem_used_size[module_id] -= size;
        xlt_mem_unlock(int_save);
    }
    return ret;
}

void *xlt_mem_mrealloc(void *pool, void *ptr, unsigned int size, unsigned int module_id)
{
    unsigned int ret;
    void *new_ptr = NULL;
    unsigned int old_node_size;
    unsigned int int_save;
    unsigned int old_module_id = module_id;

    if ((os_mem_mod_check(module_id) == XLT_NOK) || (pool == NULL))
        return NULL;

    if (ptr == NULL)
        return xlt_mem_malloc(pool, size, module_id);

    ret = os_mem_get_node_size_and_module_id(ptr, &old_module_id, &old_node_size);
    if (ret != XLT_OK)
        return NULL;

    if (size == 0) {
        (void)xlt_mem_mfree(pool, ptr, old_module_id);
        return NULL;
    }

    new_ptr = xlt_mem_realloc(pool, ptr, size);
    if (new_ptr == NULL)
        return new_ptr;
    int_save = xlt_mem_lock();
    os_mem_set_module_info(new_ptr, module_id);
    g_module_mem_used_size[old_module_id] -= old_node_size;
    xlt_mem_unlock(int_save);

    return new_ptr;
}

unsigned int xlt_mem_mused_get(unsigned int module_id)
{
    if (os_mem_mod_check(module_id) == XLT_NOK)
        return XLT_NULL_INT;
    return g_module_mem_used_size[module_id];
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
