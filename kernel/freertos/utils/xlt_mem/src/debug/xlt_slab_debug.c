/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description:  Memory debug Implementation
 * --------------------------------------------------------------------------- */

#include "xlt_slab_pri.h"
#include "xlt_memory.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static void *os_slab_allocator_get_nth(const os_slab_allocator *allocator, UINT32 idx)
{
    if (!os_atomic_bitset_get_bit(allocator->bitset, idx)) {
        return NULL;
    }

    return allocator->data_chunks + allocator->item_sz * idx;
}

static void *os_slab_allocator_get_idx_p(const os_slab_allocator *allocator, UINT32 idx)
{
    return allocator->data_chunks + allocator->item_sz * idx;
}

static UINT32 os_slab_allocator_get_index(const os_slab_allocator *allocator, const void *ptr)
{
    UINT8 *ptr_tmp = (UINT8*)ptr;
    UINT32 item_offset = (UINT32)(ptr_tmp - allocator->data_chunks);
    UINT32 item_idx = item_offset / allocator->item_sz;

    if ((item_offset % allocator->item_sz) ||
        (item_idx >= os_atomic_bitset_get_num_bits(allocator->bitset)) ||
        !(os_atomic_bitset_get_bit(allocator->bitset, item_idx))) {
        return (UINT32)(-1);
    }

    return item_idx;
}

static UINT32 os_slab_allocator_get_num_items(const os_slab_allocator *allocator)
{
    return os_atomic_bitset_get_num_bits(allocator->bitset);
}

static UINT32 os_slab_allocator_get_used_item_cnt(const os_slab_allocator *allocator)
{
    UINT32 used;
    UINT32 idx;
    struct atomic_bitset *bitset = allocator->bitset;
    for (used = 0, idx = 0; idx < bitset->num_bits; idx++) {
        if (os_atomic_bitset_get_bit(bitset, idx)) {
            used++;
        }
    }
    return used;
}

static void os_slab_allocator_get_slab_info(const os_slab_allocator *allocator,
    UINT32 *pitem_sz, UINT32 *item_cnt, UINT32 *cur_usage)
{
    *pitem_sz = allocator->item_sz;
    *item_cnt = os_atomic_bitset_get_num_bits(allocator->bitset);
    *cur_usage = os_slab_allocator_get_used_item_cnt(allocator);
}

UINT32 os_slab_get_max_free_blk_size(const void *pool)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    os_slab_allocator *slab_alloc = NULL;
    UINT32 item_sz = 0;
    UINT32 item_cnt = 0;
    UINT32 cur_usage = 0;
    INT32 idx;

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_mem_allocator *bucket = NULL;
#endif

    for (idx = SLAB_MEM_COUNT - 1; idx >= 0; idx--) {
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
        bucket = slab_mem->slab_class[idx].bucket;
        slab_alloc = bucket->slab_alloc;
#else
        slab_alloc = slab_mem->slab_class[idx].alloc;
#endif
        if (slab_alloc != NULL) {
            os_slab_allocator_get_slab_info(slab_alloc, &item_sz, &item_cnt, &cur_usage);
            if (cur_usage != item_cnt) {
                return item_sz;
            }
        }
    }

    return 0;
}

static void os_slab_status_init(xlt_slab_status *status, UINT32 total_mem, UINT32 total_usage,
    UINT32 total_alloc_count, UINT32 total_free_count)
{
    status->total_size  = total_mem;
    status->used_size   = total_usage;
    status->free_size   = status->total_size - status->used_size;
    status->alloc_count = total_alloc_count;
    status->free_count  = total_free_count;
}

UINT32 os_slab_statistics_get(const void *pool, xlt_slab_status *status)
{
    struct xlt_slab_control_header *slab_mem = NULL;
    os_slab_allocator *slab_alloc = NULL;
    UINT32 item_sz = 0;
    UINT32 item_cnt = 0;
    UINT32 cur_usage = 0;
    UINT32 total_usage = 0;
    UINT32 total_mem = 0;
    UINT32 total_alloc_count = 0;
    UINT32 total_free_count = 0;
    UINT32 idx;
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_mem_allocator *bucket = NULL;
#endif

    if ((status == NULL) || (pool == NULL)) {
        return XLT_NOK;
    }

    slab_mem = os_slab_ctrl_hdr_get(pool);

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
        bucket = slab_mem->slab_class[idx].bucket;
        while (bucket != NULL) {
            slab_alloc = bucket->slab_alloc;

            os_slab_allocator_get_slab_info(slab_alloc, &item_sz, &item_cnt, &cur_usage);
            total_usage += (cur_usage * item_sz);
            total_mem += (item_cnt * item_sz);

            bucket = bucket->next;
        }

        total_alloc_count += slab_mem->slab_class[idx].blk_used_cnt;
        total_free_count  += (slab_mem->slab_class[idx].blk_cnt * slab_mem->slab_class[idx].allocator_cnt) -
                                                slab_mem->slab_class[idx].blk_used_cnt;
#else
        slab_alloc = slab_mem->slab_class[idx].alloc;
        if (slab_alloc == NULL) {
            continue;
        }

        os_slab_allocator_get_slab_info(slab_alloc, &item_sz, &item_cnt, &cur_usage);
        total_usage += (cur_usage * item_sz);
        total_mem += (item_cnt * item_sz);
        total_alloc_count += slab_mem->slab_class[idx].blk_used_cnt;
        total_free_count  += slab_mem->slab_class[idx].blk_cnt - slab_mem->slab_class[idx].blk_used_cnt;
#endif
    }

    if (total_mem < total_usage) {
        return XLT_NOK;
    }

    os_slab_status_init(status, total_mem, total_usage, total_alloc_count, total_free_count);
    return XLT_OK;
}

bool os_slab_task_id_get(const void *ptr, UINTPTR *task_id)
{
    os_slab_block_node *slab_node = OS_SLAB_BLOCK_HEAD_GET(ptr);

    if (slab_node == NULL || !OS_ALLOC_FROM_SLAB_CHECK(slab_node)) {
        return FALSE;
    }

    *task_id = OS_SLAB_BLOCK_TASK_ID_GET(slab_node);

    return TRUE;
}

#if defined(LOSCFG_MEM_LEAKCHECK) || defined(LOSCFG_MEM_LR_BACKUP)
static INLINE void os_slab_print_link_pc(os_slab_block_node *slab_node)
{
    UINTPTR *link_reg;
#if defined(LOSCFG_MEM_LEAKCHECK)
        link_reg = slab_node->link_reg;
#elif defined(LOSCFG_MEM_LR_BACKUP)
        link_reg = (UINTPTR *)((UINTPTR)slab_node + sizeof(os_slab_block_node));
#endif

    for (UINT32 i = 0; i < LOS_RECORD_LR_CNT; i++) {
        xlt_printk("0x%-10x", link_reg[i]);
    }
}
#endif

void os_mem_no_print(const char *fmt, ...)
{
}

UINT32 os_slab_nodes_used_with_task_id(const void *pool, UINT32 task_id, UINT32 *mem_usage,
    mem_info_data_proc info_proc)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    UINT32 idx;
    UINT32 node_count = 0;
    info_proc("\nSlab node");

    if ((pool == NULL) || (mem_usage == NULL)) {
        return XLT_NOK;
    }

    for (int i = 0; i < SLAB_MEM_COUNT; i++) {
        os_slab_allocator *alloc = slab_mem->slab_class[i].alloc;
        if (alloc == NULL) {
            continue;
        }

        struct atomic_bitset *bitset = alloc->bitset;
        for (idx = 0; idx < bitset->num_bits; idx++) {
            if (!os_atomic_bitset_get_bit(bitset, idx)) {
                continue;
            }

            os_slab_block_node *slab_node = (os_slab_block_node *)os_slab_allocator_get_idx_p(alloc, idx);
            if (task_id == slab_node->task_id || (task_id == (UINT32)(-1))) {
                info_proc("\n0x%-10x%-10u%-11u", slab_node, slab_node->blk_sz, slab_node->task_id);
#ifdef XLTCFG_MEM_MUL_MODULE
                info_proc("%u  ", slab_node->module_id);
#endif
#if defined(LOSCFG_MEM_LEAKCHECK) || defined(LOSCFG_MEM_LR_BACKUP)
                if (info_proc != os_mem_no_print) {
                    os_slab_print_link_pc(slab_node);
                }
#endif
                node_count++;
                (*mem_usage) += slab_node->blk_sz;
            }
        }
    }

    return node_count;
}

UINT32 os_slab_integrity_check(const void *pool, os_slab_block_node **tmp_node)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    os_slab_block_node *slab_node = NULL;
    UINT32 idx;

    for (int i = 0; i < SLAB_MEM_COUNT; i++) {
        os_slab_allocator *alloc = slab_mem->slab_class[i].alloc;
        if (alloc == NULL) {
            continue;
        }

        struct atomic_bitset *bitset = alloc->bitset;
        for (idx = 0; idx < bitset->num_bits; idx++) {
            slab_node = (os_slab_block_node *)os_slab_allocator_get_nth(alloc, idx);
            if (slab_node == NULL) {
                continue;
            }

            if (slab_node->magic != (UINT16)OS_SLAB_MAGIC) {
                print_err("[%s], %d, memory slab check error! node:%p task id %u, magic %u\n", __FUNCTION__, __LINE__,
                    slab_node, slab_node->task_id, slab_node->magic);
                xlt_printk("BackTrace: ");
#if defined(XLTCFG_MEM_LEAKCHECK) || defined(XLTCFG_MEM_LR_BACKUP)
                os_slab_print_link_pc(slab_node);
#endif
                xlt_printk("\n");
                *tmp_node = slab_node;
                return XLT_NOK;
            }
        }
    }

    return XLT_OK;
}

#ifdef FREERTOS_DEBUG
void os_slab_info_print(const void *pool)
{
    struct xlt_slab_control_header *slab_mem = NULL;
    slab_mem = os_slab_ctrl_hdr_get(pool);
    xlt_slab_status status_check = {0};
    if (os_slab_statistics_get(pool, &status_check) == XLT_NOK) {
        return;
    }
    xlt_printk("slab addr          slab size    used size     free size    \n");
    xlt_printk("---------------    --------     -------       --------     \n");
    xlt_printk("0x%-14lx   0x%-8x   0x%-8x    0x%-8x\n",
        (UINTPTR)slab_mem, status_check.total_size, status_check.used_size,
        status_check.free_size);
}
#endif /* FREERTOS_DEBUG */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
