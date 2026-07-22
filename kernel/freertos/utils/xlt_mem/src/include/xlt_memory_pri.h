/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: memory module private headfile
 */
#ifndef XLT_MEMORY_PRI_H
#define XLT_MEMORY_PRI_H

#include "xlt_list.h"
#include "xlt_config.h"
#include "xlt_slab_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/* Memory pool information structure */
#if defined(XLTCFG_KERNEL_MEM_BESTFIT)

typedef struct {
    void *pool;                    /* Starting address of a memory pool */
    unsigned int pool_size;        /* Memory pool size */
    unsigned int alloc_fail_count; /* Memory pool malloc failed times */
#ifdef XLTCFG_MEM_MUL_POOL
    void *next_pool;
#endif

#ifdef XLTCFG_MEM_SLAB_EXTENTION
    struct xlt_slab_control_header slab_ctrl_hdr;
#endif
} xlt_mem_pool_info;

/* Memory linked list control node structure */
typedef struct {
    union {
        xlt_dl_list free_node_info; /* Free memory node */
        struct {
            UINTPTR magic;
            UINTPTR task_id;
#ifdef XLTCFG_MEM_MUL_MODULE
            UINTPTR module_id : 16;
#endif
        };
    };

    struct tag_xlt_mem_dyn_node *pre_node; /* Pointer to the previous memory node */

#ifdef XLTCFG_AARCH64
    unsigned int reserve2; /* 64-bit alignment */
#endif
    /* Size and flag of the current node (the high two bits represent a flag,and the rest bits specify the size) */
    unsigned int size_and_flag;
} xlt_mem_ctl_node;

/* Memory linked list node structure */
typedef struct __attribute__((aligned(XLT_MEM_ALIGN_SIZE))) tag_xlt_mem_dyn_node {
    xlt_mem_ctl_node self_node;
} xlt_mem_dyn_node;

typedef struct tag_os_memory_ops {
    unsigned int (*mem_integrity_check)(const void *pool, xlt_mem_dyn_node **tmp_node, xlt_mem_dyn_node **pre_node);
    void (*mem_integrity_check_error)(const xlt_mem_dyn_node *tmp_node, const xlt_mem_dyn_node *pre_node);
    void (*mem_info_print)(const void *pool);
} os_memory_ops;

extern os_memory_ops g_memory_ops;

static inline unsigned int os_mem_integrity_check_hook(const void *pool, xlt_mem_dyn_node **tmp_node,
    xlt_mem_dyn_node **pre_node)
{
    if (g_memory_ops.mem_integrity_check != NULL)
        return g_memory_ops.mem_integrity_check(pool, tmp_node, pre_node);
    return XLT_OK;
}

static inline void os_mem_integrity_check_error_hook(const xlt_mem_dyn_node *tmp_node, const xlt_mem_dyn_node *pre_node)
{
    if (g_memory_ops.mem_integrity_check_error != NULL)
        g_memory_ops.mem_integrity_check_error(tmp_node, pre_node);
}

static inline void os_mem_info_print_hook(const void *pool)
{
    if (g_memory_ops.mem_info_print != NULL)
        g_memory_ops.mem_info_print(pool);
}
#endif

#define is_aligned(value, alignSize) ((((UINTPTR)(value)) & ((UINTPTR)((alignSize)-1))) == 0)

/* spinlock for mem module, only available on SMP mode */
static inline unsigned int xlt_mem_lock(void)
{
#if defined(__LITEOS__)
    extern SPIN_LOCK_S g_memSpin;
    unsigned int int_save;
    LOS_SpinLockSave(&g_memSpin, &(int_save)) return int_save;
#elif defined(__FREERTOS__)
    return uxHwiLock();
#endif
}

static inline void xlt_mem_unlock(unsigned int int_save)
{
#if defined(__LITEOS__)
    extern SPIN_LOCK_S g_memSpin;
    LOS_SpinUnlockRestore(&g_memSpin, (int_save))
#elif defined(__FREERTOS__)
    vHwiRestore(int_save);
#endif
}

#ifdef XLTCFG_MEM_MUL_POOL
typedef struct {
    unsigned int (*mem_mul_pool_init)(void *pool, unsigned int size);
    unsigned int (*mem_mul_pool_deinit)(const void *pool);
    void *(*mem_mul_pool_head_get)(void);
} os_mem_mul_pool_ops;

extern os_mem_mul_pool_ops g_mul_pool_ops;
extern unsigned int os_mem_mul_pool_init(void *pool, unsigned int size);
extern unsigned int os_mem_mul_pool_deinit(const void *pool);
extern void *os_mem_mul_pool_head_get(void);


static inline unsigned int os_mem_mul_pool_init_hook(void *pool, unsigned int size)
{
    if (g_mul_pool_ops.mem_mul_pool_init != NULL) {
        return g_mul_pool_ops.mem_mul_pool_init(pool, size);
    } else {
        return os_mem_mul_pool_init(pool, size);
    }
    return XLT_OK;
}

static inline unsigned int os_mem_mul_pool_deinit_hook(const void *pool)
{
    if (g_mul_pool_ops.mem_mul_pool_deinit != NULL) {
        return g_mul_pool_ops.mem_mul_pool_deinit(pool);
    } else {
        return os_mem_mul_pool_deinit(pool);
    }

    return XLT_OK;
}

static inline void *os_mem_mul_pool_head_get_hook(void)
{
    if (g_mul_pool_ops.mem_mul_pool_head_get != NULL) {
        return g_mul_pool_ops.mem_mul_pool_head_get();
    } else {
        return os_mem_mul_pool_head_get();
    }

    return NULL;
}
#endif /* XLTCFG_MEM_MUL_POOL */

extern void os_mem_info_print(const void *pool);

/* SLAB extention needs memory algorithms provide following internal apis */
#ifdef XLTCFG_MEM_SLAB_EXTENTION
extern void *os_mem_alloc(void *pool, UINT32 size);
extern unsigned int os_mem_free(void *pool, const void *ptr);
#endif /* LOSCFG_KERNEL_MEM_SLAB_EXTENTION */

/* Memory Info Print Apis */
#ifdef FREERTOS_DEBUG
extern unsigned int xlt_mem_tasknodes_print(UINT32 taskId, mem_info_data_proc infoProc);
#endif /* FREERTOS_DEBUG */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* XLT_MEMORY_PRI_H */
