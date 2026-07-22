/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: XLT memory Module Implementation
 * --------------------------------------------------------------------------- */
#ifdef XLTCFG_MEM_SLAB_EXTENTION

#include "securec.h"
#include "xlt_slab_pri.h"
#include "xlt_memory_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

static UINT32 g_slab_alloctor_size_cfg[SLAB_MEM_COUNT] = {
    [0 ... SLAB_MEM_COUNT - 1] = SLAB_MEM_ALLOCATOR_SIZE
};

UINT32 g_slab_mem_class_step_size = 0x10U;
static void os_slab_size_reset(void)
{
    INT32 idx;

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        if (g_slab_alloctor_size_cfg[idx] != SLAB_MEM_ALLOCATOR_SIZE) {
            g_slab_alloctor_size_cfg[idx] = SLAB_MEM_ALLOCATOR_SIZE;
        }
    }
}

#ifdef LOSCFG_MEM_LEAKCHECK
static __attribute__((always_inline)) inline void os_slab_link_register_record(os_slab_block_node *slab_node)
{
    (void)memset_s(slab_node->link_reg, (LOS_RECORD_LR_CNT * sizeof(UINTPTR)), 0,
        (LOS_RECORD_LR_CNT * sizeof(UINTPTR)));

#ifdef LOSCFG_BACKTRACE
    UINTPTR frame_ptr;
    frame_ptr = ArchGetFp();
    (void)ArchBackTraceGet(frame_ptr, slab_node->link_reg, LOS_RECORD_LR_CNT);
#else
    UINTPTR stack_pointer = (UINTPTR)ArchSPGet();
    OsBaceTraceParam param;
    param.taskCB = NULL;
    param.maxDepth = LOS_RECORD_LR_CNT;
    param.omitCount = LOS_OMIT_LR_CNT + 1;
    (void)ArchBackTraceGetWithSp(stack_pointer, slab_node->link_reg, &param);
#endif
}
#endif

static void *os_slab_block_head_fill(void *pool, os_slab_block_node *slab_node, UINT32 blk_sz)
{
    OS_SLAB_BLOCK_MAGIC_SET(slab_node);
    OS_SLAB_BLOCK_SIZE_SET(slab_node, blk_sz);
    UINT32 task_id;

#if defined(__FREERTOS__)
    task_id = (UINT32)xTaskGetCurrentTaskHandle();
    OS_SLAB_BLOCK_TASK_ID_SET(slab_node, task_id);
#else
#if defined(LOSCFG_MEM_DEBUG) || defined(LOSCFG_MEM_TASK_STAT)
    LosTaskCB *runTask = OsCurrTaskGet();
    xlt_mem_pool_info *poolInfo = (xlt_mem_pool_info *)pool;

    if ((runTask != NULL) && OS_INT_INACTIVE) {
        task_id = runTask->taskId;
    } else {
        /* If the task mode does not initialize, the field is the 0xffffffff */
        task_id = TASK_NUM - 1;
    }

    OS_SLAB_BLOCK_TASK_ID_SET(slab_node, task_id);
    OS_MEM_ADD_USED(&poolInfo->stat, blk_sz, task_id);
#endif
#endif

#ifdef LOSCFG_MEM_LEAKCHECK
    os_slab_link_register_record(slab_node);
#endif
    return (void *)(slab_node + 1);
}

#ifdef XLTCFG_MEM_MUL_MODULE
bool os_slab_set_module_info(void *ptr, UINT32 *module_mem_used_size, UINT32 module_id)
{
    os_slab_block_node *slab_node = OS_SLAB_BLOCK_HEAD_GET(ptr);
    if (slab_node == NULL || !OS_ALLOC_FROM_SLAB_CHECK(slab_node)) {
        return FALSE;
    }

    module_mem_used_size[module_id] += OS_SLAB_BLOCK_SIZE_GET(slab_node);
    OS_SLAB_BLOCK_MODULE_ID_SET(slab_node, module_id);

    return TRUE;
}

bool os_slab_get_node_size_and_module_id(void *ptr, UINT32 *module_id, UINT32 *size)
{
    os_slab_block_node *slab_node = OS_SLAB_BLOCK_HEAD_GET(ptr);
    if (slab_node == NULL ||  !OS_ALLOC_FROM_SLAB_CHECK(slab_node)) {
        return FALSE;
    }

    *size = OS_SLAB_BLOCK_SIZE_GET(slab_node);

    if (*module_id != OS_SLAB_BLOCK_MODULE_ID_GET(slab_node)) {
        print_err("slab node[%p] alloced in module %lu, but free in module %u, tid[0x%x]\n",
            ptr, OS_SLAB_BLOCK_MODULE_ID_GET(slab_node), *module_id, OS_SLAB_BLOCK_TASK_ID_GET(slab_node));
        *module_id = OS_SLAB_BLOCK_MODULE_ID_GET(slab_node);
    }

    return TRUE;
}
#endif

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
static bool os_slab_mem_bucket_init(void *pool, UINT32 size)
{
    UINT32 bucket_num, idx, i, blk_sz, blk_cnt;
    struct xlt_slab_control_header *slab_mem_head = os_slab_ctrl_hdr_get(pool);

    bucket_num = (size / SLAB_MEM_ALLOCATOR_SIZE) >> SLAB_MEM_BUCKET_SIZE_TYPE;
    if ((slab_mem_head == NULL) ||
        (SLAB_MEM_BUCKET_SIZE_TYPE >= SLAB_BUCKET_SIZE_TYPE_MAX) ||
        (bucket_num < SLAB_MEM_COUNT)) {
        print_err("invalid para, size type %d, bucket_num %u\n", SLAB_MEM_BUCKET_SIZE_TYPE, bucket_num);
        return FALSE;
    }

    slab_mem_head->allocator_bucket =
        os_slab_allocator_new(pool, sizeof(os_slab_mem_allocator), (UINT32)sizeof(UINT32), bucket_num);
    if (slab_mem_head->allocator_bucket == NULL) {
        return FALSE;
    }

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        blk_sz = (SLAB_MEM_CALSS_STEP_SIZE << idx);
        blk_cnt = SLAB_MEM_ALLOCATOR_SIZE / blk_sz;
        slab_mem_head->slab_class[idx].blk_sz = blk_sz;
        slab_mem_head->slab_class[idx].blk_cnt = blk_cnt;
        slab_mem_head->slab_class[idx].blk_used_cnt = 0;

        slab_mem_head->slab_class[idx].bucket = os_slab_allocator_alloc(slab_mem_head->allocator_bucket);
        if (slab_mem_head->slab_class[idx].bucket == NULL) {
            goto BUCKET_ALLOC_FAIL;
        }

        slab_mem_head->slab_class[idx].bucket->slab_alloc =
            os_slab_allocator_new(pool, blk_sz + sizeof(os_slab_block_node), (UINT32)sizeof(char *), blk_cnt);
        if (slab_mem_head->slab_class[idx].bucket->slab_alloc == NULL) {
            goto ALLOCATOR_ALLOC_FAIL;
        }

        slab_mem_head->slab_class[idx].bucket->next = NULL;
        slab_mem_head->slab_class[idx].allocator_cnt = 1;
    }

    return TRUE;

ALLOCATOR_ALLOC_FAIL:
    /* free all buckets that alloc before */
    for (i = 0; i < idx; i++) {
        (void)os_slab_allocator_free(slab_mem_head->allocator_bucket, slab_mem_head->slab_class[i].bucket);
    }

BUCKET_ALLOC_FAIL:
    os_slab_allocator_destroy(pool, slab_mem_head->allocator_bucket);

    return FALSE;
}

static bool os_slab_mem_alloc_stroll_buckets(void *pool, void **out_mem, os_slab_mem *slab_class)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    os_slab_mem *slab_mem_class = slab_class;
    os_slab_mem_allocator **bucket = &slab_class->bucket; /* slab_mem_class->bucket must not be NULL */

    *out_mem = NULL;

    while (*bucket != NULL) {
        *out_mem = os_slab_allocator_alloc((*bucket)->slab_alloc);
        if (*out_mem != NULL) {
            /* alloc success */
            return TRUE;
        }
        bucket = &((*bucket)->next);
    }

    *bucket = os_slab_allocator_alloc(slab_mem->allocator_bucket);
    if (*bucket == NULL) {
        return FALSE;
    }

    (*bucket)->slab_alloc = os_slab_allocator_new(pool, slab_mem_class->blk_sz + sizeof(os_slab_block_node),
        (UINT32)sizeof(char *), slab_mem_class->blk_cnt);
    if ((*bucket)->slab_alloc == NULL) {
        (void)os_slab_allocator_free(slab_mem->allocator_bucket, *bucket);
        *bucket = NULL;
        return FALSE;
    }

    (*bucket)->next = NULL;
    slab_mem_class->allocator_cnt++;
    *out_mem = os_slab_allocator_alloc((*bucket)->slab_alloc);

    return TRUE;
}


static bool os_slab_mem_free_stroll_buckets(void *pool, os_slab_block_node *slab_node,
    os_slab_mem *slab_class)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    os_slab_mem *slab_mem_class = slab_class;
    os_slab_mem_allocator **bucket = &(slab_class->bucket); /* slab_mem_class->bucket must not be NULL */
    os_slab_allocator *slab_alloc = NULL;

    while (*bucket != NULL) {
        slab_alloc = (*bucket)->slab_alloc;
        if (os_slab_allocator_free(slab_alloc, slab_node) == TRUE) {
            /* try to destroy slabAllocator if needed */
            if ((slab_mem_class->allocator_cnt > SLAB_MEM_DFEAULT_BUCKET_CNT) &&
                (os_slab_allocator_empty(slab_alloc) == TRUE)) {
                os_slab_mem_allocator *alloc = *bucket;
                (*bucket) = (*bucket)->next; /* delete node in list */
                os_slab_allocator_destroy(pool, alloc->slab_alloc); /* destory slabAllocator */
                (void)os_slab_allocator_free(slab_mem->allocator_bucket, alloc);

                slab_mem_class->allocator_cnt--;
            }
            return TRUE;
        }
        bucket = &((*bucket)->next);
    }

    return FALSE;
}

static UINT32 os_get_blk_sz_stroll_buckets(const os_slab_block_node *slab_node, const os_slab_mem *slab_class)
{
    os_slab_mem *slab_mem_class = (os_slab_mem *)slab_class;
    os_slab_mem_allocator *bucket = slab_mem_class->bucket; /* slab_mem_class->bucket must not be NULL */
    os_slab_allocator *slab_alloc = NULL;
    UINT32 ret_blk_sz = (UINT32)-1;

    while (bucket != NULL) {
        slab_alloc = bucket->slab_alloc;
        if (os_slab_allocator_check(slab_alloc, (void *)slab_node) == TRUE) {
            ret_blk_sz = slab_mem_class->blk_sz;
            return ret_blk_sz;
        }
        bucket = bucket->next;
    }

    return ret_blk_sz;
}
#endif

void os_slab_mem_init(void *pool, UINT32 size)
{
    struct xlt_slab_control_header *slab_mem_head = os_slab_ctrl_hdr_get(pool);

    (void)memset_s(slab_mem_head, sizeof(*slab_mem_head), 0, sizeof(struct xlt_slab_control_header));
    /*
     * Since if the size is not enough to create a SLAB pool, the memory pool
     * still works. We choose not treat that as failure.
     */
    if (size < SLAB_BASIC_NEED_SIZE) {
        xlt_printk("bad input size\n");
        return;
    }

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    if (!os_slab_mem_bucket_init(pool, size)) {
        xlt_printk("slab bucket init failed\n");
    }
#else
    UINT32 idx, blk_sz, blk_cnt;

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        blk_sz = (SLAB_MEM_CALSS_STEP_SIZE << idx);
        blk_cnt = g_slab_alloctor_size_cfg[idx] / blk_sz;
        slab_mem_head->slab_class[idx].blk_sz = blk_sz;
        slab_mem_head->slab_class[idx].blk_cnt = blk_cnt;
        slab_mem_head->slab_class[idx].blk_used_cnt = 0;
        slab_mem_head->slab_class[idx].alloc =
            os_slab_allocator_new(pool, blk_sz + sizeof(os_slab_block_node), (UINT32)sizeof(char *), blk_cnt);
        if (slab_mem_head->slab_class[idx].alloc == NULL) {
            print_err("slab alloc failed\n");
        }
    }
    /* for multi pool, cfg size should reset */
    os_slab_size_reset();
#endif
}

void *os_slab_mem_alloc(void *pool, UINT32 size)
{
    void *ret = NULL;
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    UINT32 idx;

    if ((slab_mem->enabled == FALSE) || (size > (SLAB_MEM_CALSS_STEP_SIZE << (SLAB_MEM_COUNT - 1)))) {
        return NULL;
    }

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        if (size <= slab_mem->slab_class[idx].blk_sz) {
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
            /* if slab mem alloc from current bucket succeesful, goto ALLOC_SUCC */
            /* otherwise, should alloc in slab_alloc with new bucket */
            if (os_slab_mem_alloc_stroll_buckets(pool, &ret, &(slab_mem->slab_class[idx])) == FALSE) {
                return NULL;
            }
#else
            if (slab_mem->slab_class[idx].blk_used_cnt >= slab_mem->slab_class[idx].blk_cnt) {
                return NULL;
            }

            if (slab_mem->slab_class[idx].alloc == NULL) {
                return NULL;
            }

            ret = os_slab_allocator_alloc(slab_mem->slab_class[idx].alloc);
#endif
            if (ret != NULL) {
                /* alloc success */
                ret = os_slab_block_head_fill(pool, (os_slab_block_node *)ret, slab_mem->slab_class[idx].blk_sz);
                slab_mem->slab_class[idx].blk_used_cnt++;
            }

            return ret;
        }
    }

    return NULL;
}

bool os_slab_mem_free(void *pool, void *ptr)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
#ifndef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_allocator *slab_alloc = NULL;
#endif
    UINT32 idx;
    os_slab_block_node *slab_node = OS_SLAB_BLOCK_HEAD_GET(ptr);

    if ((slab_mem->enabled == FALSE) || (!OS_ALLOC_FROM_SLAB_CHECK(slab_node))) {
        return FALSE;
    }

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        if (slab_mem->slab_class[idx].blk_sz < OS_SLAB_BLOCK_SIZE_GET(slab_node)) {
            continue;
        }
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
        if (os_slab_mem_free_stroll_buckets(pool, slab_node, &(slab_mem->slab_class[idx])) == FALSE) {
            break;
        }
#else
        slab_alloc = slab_mem->slab_class[idx].alloc;
        if (slab_alloc == NULL || os_slab_allocator_free(slab_alloc, slab_node) == FALSE) {
            break;
        }
#endif
        slab_mem->slab_class[idx].blk_used_cnt--;
#ifdef LOSCFG_MEM_TASK_STAT
        UINT8 task_id = OS_SLAB_BLOCK_TASK_ID_GET(slab_node);
        xlt_mem_pool_info *poolInfo = (xlt_mem_pool_info *)pool;
        OS_MEM_REDUCE_USED(&poolInfo->stat, slab_mem->slab_class[idx].blk_sz, task_id);
#endif

        return TRUE;
    }

    return FALSE;
}

void os_slab_mem_deinit(void *pool)
{
    UINT32 idx;
    struct xlt_slab_control_header *slab_mem = NULL;
    os_slab_allocator *slab_alloc = NULL;
    UINT32 blk_sz;
    UINT32 blk_cnt;
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_mem_allocator **bucket = NULL;
#endif

    if (pool == NULL) {
        return;
    }
    slab_mem = os_slab_ctrl_hdr_get(pool);

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        blk_sz = (SLAB_MEM_CALSS_STEP_SIZE << idx);
        blk_cnt = SLAB_MEM_ALLOCATOR_SIZE / blk_sz;
        slab_mem->slab_class[idx].blk_sz = blk_sz;
        slab_mem->slab_class[idx].blk_cnt = blk_cnt;

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
        bucket = &(slab_mem->slab_class[idx].bucket);
        while (*bucket != NULL) {
            slab_alloc = (*bucket)->slab_alloc;
            (*bucket) = (*bucket)->next;
            os_slab_allocator_destroy(pool, slab_alloc);
        }
#else
        if (slab_mem->slab_class[idx].alloc != NULL) {
            slab_alloc = slab_mem->slab_class[idx].alloc;
            os_slab_allocator_destroy(pool, slab_alloc);
            slab_mem->slab_class[idx].alloc = NULL;
        }
#endif
    }
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_allocator_destroy(pool, slab_mem->allocator_bucket);
    slab_mem->allocator_bucket = NULL;
#endif

    os_slab_size_reset();

    return;
}

UINT32 os_slab_mem_check(const void *pool, const void* ptr)
{
    struct xlt_slab_control_header *slab_mem = os_slab_ctrl_hdr_get(pool);
    UINT32 ret_blk_sz = (UINT32)-1;
#ifndef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_allocator *slab_alloc = NULL;
#endif
    UINT32 idx;
    os_slab_block_node *slab_node = OS_SLAB_BLOCK_HEAD_GET(ptr);

    if ((!OS_ALLOC_FROM_SLAB_CHECK(slab_node)) ||
        (slab_mem->slab_class[SLAB_MEM_COUNT - 1].blk_sz < (OS_SLAB_BLOCK_SIZE_GET(slab_node)))) {
        return ret_blk_sz;
    }

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        if (slab_mem->slab_class[idx].blk_sz >= OS_SLAB_BLOCK_SIZE_GET(slab_node)) {
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
            ret_blk_sz = os_get_blk_sz_stroll_buckets(slab_node, &(slab_mem->slab_class[idx]));
#else
            slab_alloc = slab_mem->slab_class[idx].alloc;
            if (slab_alloc != NULL &&
                os_slab_allocator_check(slab_alloc, slab_node) == TRUE) {
                ret_blk_sz = slab_mem->slab_class[idx].blk_sz;
            }
#endif
            return ret_blk_sz;
        }
    }

    return ret_blk_sz;
}

void *os_slab_ctrl_hdr_get(const void *pool)
{
    return &((xlt_mem_pool_info *)pool)->slab_ctrl_hdr;
}

void xlt_slab_size_cfg(UINT32 *cfg, UINT32 cnt, UINT32 step_size)
{
    INT32 idx;

    if ((cfg == NULL) || (cnt != SLAB_MEM_COUNT) || step_size == 0) {
        print_err("cfg is NULL, or cnt %u != SLAB_MEM_COUNT\n", cnt);
        return;
    }

    for (idx = 0; idx < SLAB_MEM_COUNT; idx++) {
        if (cfg[idx] < SLAB_MEM_ALLOCATOR_SIZE) {
            g_slab_alloctor_size_cfg[idx] = SLAB_MEM_ALLOCATOR_SIZE;
        } else {
            g_slab_alloctor_size_cfg[idx] = cfg[idx];
        }
    }

    g_slab_mem_class_step_size = step_size;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
