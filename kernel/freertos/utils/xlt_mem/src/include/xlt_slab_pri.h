/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: XLT memory Module Implementation
 * --------------------------------------------------------------------------- */

/**
 * @defgroup xlt_slab Slab
 * @ingroup xlt_mem
 */

#ifndef XLT_SLAB_PRI_H
#define XLT_SLAB_PRI_H

#include "xlt_typedef.h"
#include "xlt_slab.h"
#include "xlt_memory.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef void (*mem_info_data_proc)(const char *format, ...);
void os_mem_no_print(const char *fmt, ...);

#ifdef XLTCFG_MEM_SLAB_EXTENTION

/* number of slab class */
#define SLAB_MEM_COUNT              4

/* step size of each class */
#define SLAB_MEM_CALSS_STEP_SIZE    g_slab_mem_class_step_size

/* max slab block size */
#define SLAB_MEM_MAX_SIZE           (SLAB_MEM_CALSS_STEP_SIZE << (SLAB_MEM_COUNT - 1))

typedef struct tag_xlt_slab_status {
    UINT32 total_size;
    UINT32 used_size;
    UINT32 free_size;
    UINT32 alloc_count;
    UINT32 free_count;
} xlt_slab_status;

typedef struct __attribute__ ((aligned(XLT_MEM_ALIGN_SIZE))) tag_os_slab_block_node {
    UINT16 magic;
    UINT16 blk_sz;
#ifdef XLTCFG_MEM_MUL_MODULE
    UINT16 module_id;
#endif
    UINTPTR task_id;
#ifdef LOSCFG_MEM_LEAKCHECK
    UINTPTR link_reg[LOS_RECORD_LR_CNT];
#endif
} os_slab_block_node;

struct atomic_bitset {
    UINT32 num_bits;
    UINT32 words[0];
};

typedef struct tag_os_slab_allocator {
    UINT32 item_sz;
    UINT8 *data_chunks;
    struct atomic_bitset *bitset;
} os_slab_allocator;

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
typedef struct tag_os_slab_mem_allocator {
    struct tag_os_slab_mem_allocator *next;
    os_slab_allocator *slab_alloc;
} os_slab_mem_allocator;
#endif

typedef struct tag_os_slab_mem {
    UINT32 blk_sz;
    UINT32 blk_cnt;
    UINT32 blk_used_cnt;
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    UINT32 allocator_cnt;
    os_slab_mem_allocator *bucket;
#else
    os_slab_allocator *alloc;
#endif
} os_slab_mem;

struct xlt_slab_control_header {
    bool enabled;
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
    os_slab_allocator *allocator_bucket;
#endif
    os_slab_mem slab_class[SLAB_MEM_COUNT];
};

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
#define SLAB_MEM_DFEAULT_BUCKET_CNT                1
#endif

#define OS_SLAB_MAGIC                              0xdede
#define OS_SLAB_BLOCK_HEAD_GET(ptr)                ((os_slab_block_node *)(void *)((UINT8 *)(ptr) - \
                                                      sizeof(os_slab_block_node)))
#define OS_SLAB_BLOCK_MAGIC_SET(slab_node)         (((os_slab_block_node *)(slab_node))->magic = (UINT16)OS_SLAB_MAGIC)
#define OS_SLAB_BLOCK_MAGIC_GET(slab_node)         (((os_slab_block_node *)(slab_node))->magic)
#define OS_SLAB_BLOCK_SIZE_SET(slab_node, size)    (((os_slab_block_node *)(slab_node))->blk_sz = (UINT8)(size))
#define OS_SLAB_BLOCK_SIZE_GET(slab_node)          (((os_slab_block_node *)(slab_node))->blk_sz)
#define OS_SLAB_BLOCK_MODULE_ID_SET(slab_node, id) (((os_slab_block_node *)(slab_node))->module_id = (id))
#define OS_SLAB_BLOCK_MODULE_ID_GET(slab_node)     (((os_slab_block_node *)(slab_node))->module_id)
#define OS_SLAB_BLOCK_TASK_ID_SET(slab_node, id)   (((os_slab_block_node *)(slab_node))->task_id = (id))
#define OS_SLAB_BLOCK_TASK_ID_GET(slab_node)       (((os_slab_block_node *)(slab_node))->task_id)
#define OS_ALLOC_FROM_SLAB_CHECK(slab_node)        (((os_slab_block_node *)(slab_node))->magic == (UINT16)OS_SLAB_MAGIC)
#define OS_SLAB_LOG2(value)                        ((UINT32)(32 - CLZ(value) - 1)) /* get highest bit one position */
#define OS_SLAB_CLASS_LEVEL_GET(size) \
        (OS_SLAB_LOG2((size - 1) >> (OS_SLAB_LOG2(SLAB_MEM_CALSS_STEP_SIZE - 1))))

extern UINT32 g_slab_mem_class_step_size;

extern os_slab_allocator *os_slab_allocator_new(void *pool, UINT32 item_sz, UINT32 item_align, UINT32 num_items);
extern void os_slab_allocator_destroy(void *pool, os_slab_allocator *allocator);
extern void *os_slab_allocator_alloc(os_slab_allocator *allocator);
extern bool os_slab_allocator_free(os_slab_allocator *allocator, void* ptr);
extern bool os_slab_allocator_empty(const os_slab_allocator *allocator);
extern bool os_slab_allocator_check(const os_slab_allocator *allocator, const void *ptr);
extern void os_slab_mem_init(void *pool, UINT32 size);
extern void os_slab_mem_deinit(void *pool);
extern void *os_slab_mem_alloc(void *pool, UINT32 sz);
extern bool os_slab_mem_free(void *pool, void *ptr);
extern UINT32 os_slab_mem_check(const void *pool, const void *ptr);
extern UINT32 os_slab_statistics_get(const void *pool, xlt_slab_status *status);
extern UINT32 os_slab_get_max_free_blk_size(const void *pool);
extern void *os_slab_ctrl_hdr_get(const void *pool);
extern bool os_atomic_bitset_get_bit(const struct atomic_bitset *set, UINT32 num);
extern UINT32 os_atomic_bitset_get_num_bits(const struct atomic_bitset *set);
#ifdef XLTCFG_MEM_MUL_MODULE
extern bool os_slab_set_module_info(void *ptr, UINT32 *module_mem_used_size, UINT32 module_id);
extern bool os_slab_get_node_size_and_module_id(void *ptr, UINT32 *module_id, UINT32 *size);
#endif
extern bool os_slab_task_id_get(const void *ptr, UINT32 *task_id);
extern UINT32 os_slab_nodes_used_with_task_id(const void *pool, UINT32 task_id, UINT32 *mem_usage,
    mem_info_data_proc info_proc);
extern UINT32 os_slab_integrity_check(const void *pool, os_slab_block_node **tmp_node);
#ifdef FREERTOS_DEBUG
extern void os_slab_info_print(const void *pool);
#endif

static inline void os_slab_mem_proc_init_flag(void *pool, UINT32 size, bool slab_enable)
{
    struct xlt_slab_control_header *header = (struct xlt_slab_control_header *)os_slab_ctrl_hdr_get(pool);

    if (slab_enable) {
        os_slab_mem_init(pool, size);
    }

    /* flag MUSL be set after os_slab_mem_init */
    header->enabled = slab_enable;
}

#else /* !XLTCFG_MEM_SLAB_EXTENTION */

static inline void os_slab_mem_proc_init_flag(void *pool, UINT32 size, bool slab_enable)
{
    (void)pool;
    (void)size;
    (void)slab_enable;
}

static inline void *os_slab_mem_alloc(void *pool, UINT32 size)
{
    (void)pool;
    (void)size;
    return NULL;
}

static inline bool os_slab_mem_free(void *pool, void *ptr)
{
    (void)pool;
    (void)ptr;
    return FALSE;
}

static inline UINT32 os_slab_mem_check(const void *pool, const void *ptr)
{
    (void)pool;
    (void)ptr;
    return (UINT32)-1;
}

#endif /* XLTCFG_MEM_SLAB_EXTENTION */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* XLT_SLAB_PRI_H */
