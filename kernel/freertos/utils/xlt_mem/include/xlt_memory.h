/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: mem module headfile
 */

#ifndef _XLT_MEMORY_H
#define _XLT_MEMORY_H

#if defined(__FREERTOS__)
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"
#include "arch_base.h"
#include "cache.h"
#include "hwi.h"
#include "asm/memmap_config.h"
#endif

#include "xlt_errno.h"
#include "xlt_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(__FREERTOS__)
#define print_warn(fmt, ...) vPrintk(ePrintkWarn, fmt, ##__VA_ARGS__)
#define print_err(fmt, ...) vPrintk(ePrintkErr, fmt, ##__VA_ARGS__)
extern void xlt_printk(const char *fmt, ...);
#endif

#define xlt_mem_align(p, align_size) (((UINTPTR)(p) + (align_size)-1) & ~((UINTPTR)((align_size)-1)))
#define XLT_MEM_ALIGN_SIZE (sizeof(UINTPTR))

#ifdef XLTCFG_MEM_MUL_MODULE
/**
 * @ingroup xlt_memory
 * The memory usage statistics depend on module, this is the max module number 0x20.
 * Note that this macro is defined only when XLTCFG_MEM_MUL_MODULE is defined.
 */
#define PI 314
#ifndef MEM_MODULE_MAX
#define MEM_MODULE_MAX 0x7
#endif

/**
 * @ingroup xlt_memory
 * @brief Allocate dynamic memory.
 *
 * @par Description:
 * This API is used to allocate a memory block of which the size is specified and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The size of the input parameter size must be four byte-aligned.</li>
 * <li>This function is defined only when XLTCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the memory block to be allocated.
 * @param  size     [IN] Size of the memory block to be allocated (unit: byte).
 * @param  module_id [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL       The memory fails to be allocated.
 * @retval #void*      The memory is successfully allocated, and the API returns the pointer to
 * the allocated memory block.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_mrealloc | xlt_mem_malloc_align | xlt_mem_mfree
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_malloc(void *pool, unsigned int size, unsigned int module_id);

/**
 * @ingroup xlt_memory
 * @brief Allocate aligned memory.
 *
 * @par Description:
 * This API is used to allocate memory blocks of specified size and of which the starting addresses are aligned on
 * a specified boundary and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The alignment parameter value must be a power of 2 with the minimum value being 4.</li>
 * <li>This function is defined only when XLTCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the memory blocks to be allocated.
 * @param  size      [IN] Size of the memory to be allocated.
 * @param  boundary  [IN] Boundary on which the memory is aligned.
 * @param  module_id  [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL          The memory fails to be allocated.
 * @retval #void*         The memory is successfully allocated, and the API returns the pointer to the allocated memory.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_malloc | xlt_mem_realloc | xlt_mem_mfree
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_malloc_align(void *pool, unsigned int size, unsigned int boundary, unsigned int module_id);

/**
 * @ingroup xlt_memory
 * @brief Free dynamic memory.
 *
 * @par Description:
 * This API is used to free specified dynamic memory that has been allocated and update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The input ptr parameter must be allocated by xlt_mem_malloc or xlt_mem_malloc_align or xlt_mem_mrealloc.</li>
 * <li>This function is defined only when XLTCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the dynamic memory block to be freed.
 * @param  ptr      [IN] Starting address of the memory block to be freed.
 * @param  module_id [IN] module ID (0~MODULE_MAX).
 *
 * @retval #XLT_NOK          The memory block fails to be freed because the starting address of the memory block is
 * invalid, or the memory overwriting occurs.
 * @retval #XLT_OK           The memory block is freed successfully.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_malloc | xlt_mem_mrealloc | xlt_mem_malloc_align
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_mfree(void *pool, void *ptr, unsigned int module_id);

/**
 * @ingroup xlt_memory
 * @brief Re-allocate a memory block.
 *
 * @par Description:
 * This API is used to allocate a new memory block of which the size is specified by size if the original memory
 * block size is insufficient. The new memory block will copy the data in the original memory block of which the
 * address is specified by ptr.The size of the new memory block determines the maximum size of data to be copied.
 * After the new memory block is created, the original one is freed. And update module mem used.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The input ptr parameter must be allocated by xlt_mem_malloc or xlt_mem_malloc_align.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The size of the input parameter size must be aligned as follows: 1) if the ptr is allocated by xlt_mem_alloc,
 * it must be four byte-aligned; 2) if the ptr is allocated by xlt_mem_malloc_align, it must be aligned with the size of
 * the input parameter boundary of xlt_mem_malloc_align.</li>
 * <li>This function is defined only when XLTCFG_MEM_MUL_MODULE is defined.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the original and new memory blocks.
 * @param  ptr       [IN] Address of the original memory block.
 * @param  size      [IN] Size of the new memory block.
 * @param  module_id  [IN] module ID (0~MODULE_MAX).
 *
 * @retval #NULL          The memory fails to be re-allocated.
 * @retval #void*         The memory is successfully re-allocated, and the API returns the pointer to
 * the new memory block.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_malloc | xlt_mem_malloc_align | xlt_mem_mfree
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_mrealloc(void *pool, void *ptr, unsigned int size, unsigned int module_id);

/**
 * @ingroup xlt_memory
 * @brief get the uesed memory size of the specified module.
 *
 * @par Description:
 * This API is used to get the specified module's memory consume size.
 * @attention This function is defined only when XLTCFG_MEM_MUL_MODULE is defined.
 *
 * @param  module_id   [IN] module ID (0~MODULE_MAX).
 *
 * @retval #unsigned int         The size of the specified module's consumed memory.
 * @retval #OS_NULL_INT    The input module id is illegal.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_mused_get(unsigned int module_id);

#ifdef XLTCFG_LIB_CONFIGURABLE
#if defined(__LITEOS__)
extern unsigned int *g_module_mem_used_size;
#endif
#endif

#ifndef XLTCFG_SUPPORT_COMPAT_MALLOC
#ifndef XLT_DEFAULT_MOD_ID
#define XLT_DEFAULT_MOD_ID 0x0
#endif

#define xlt_mem_alloc(pool, size) xlt_mem_malloc(pool, size, XLT_DEFAULT_MOD_ID)
#define xlt_mem_free(pool, ptr) xlt_mem_mfree(pool, ptr, XLT_DEFAULT_MOD_ID)
#define xlt_mem_realloc(pool, ptr, size) xlt_mem_mrealloc(pool, ptr, size, XLT_DEFAULT_MOD_ID)
#define xlt_mem_alloc_align(pool, size, boundary) xlt_mem_malloc_align(pool, size, boundary, XLT_DEFAULT_MOD_ID)
#endif

#else /* if not support XLTCFG_MEM_MUL_MODULE */
#define xlt_mem_malloc(pool, size, module_id) xlt_mem_alloc(pool, size)
#define xlt_mem_malloc_align(pool, size, boundary, module_id) xlt_mem_alloc_align(pool, size, boundary)
#define xlt_mem_mfree(pool, ptr, module_id) xlt_mem_free(pool, ptr)
#define xlt_mem_mrealloc(pool, ptr, size, module_id) xlt_mem_realloc(pool, ptr, size)
#endif


#ifdef XLTCFG_MEM_MUL_POOL
/**
 * @ingroup xlt_memory
 * @brief Deinitialize dynamic memory.
 *
 * @par Description:
 * This API is used to deinitialize the dynamic memory of a doubly linked list.
 * @attention This function is defined only when XLTCFG_MEM_MUL_POOL is defined.
 *
 * @param pool          [IN] Starting address of memory.
 *
 * @retval #XLT_NOK    The dynamic memory fails to be deinitialized.
 * @retval #XLT_OK     The dynamic memory is successfully deinitialized.
 * @par Dependency:
 * <ul>
 * <li>xlt_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_deinit(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Print information about all pools.
 *
 * @par Description:
 * This API is used to print information about all pools.
 *
 * @attention This function is defined only when XLTCFG_MEM_MUL_POOL is defined.
 *
 * @param None.
 *
 * @retval #unsigned int  The pool number.
 * @par Dependency:
 * <ul>
 * <li>xlt_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_pool_list(void);
#endif

/**
 * @ingroup xlt_memory
 * Memory pool extern information structure
 */
typedef struct {
    unsigned int uw_total_used_size;
    unsigned int uw_total_free_size;
    unsigned int uw_max_free_node_size;
    unsigned int uw_used_node_num;
    unsigned int uw_free_node_num;
} xlt_mem_pool_status;

/**
 * @ingroup xlt_memory
 * The attributes to config memory pool.
 * Bit 0 is used to config the memory pool containing slab.
 * The other 31 bits are reserved now.
 */
#define XLT_MEM_INIT_ATTR_SLAB_POS 0U
#define XLT_MEM_INIT_ATTR_SLAB_MSK 1U /* << XLT_MEM_INIT_ATTR_SLAB_POS */


/**
 * @ingroup xlt_memory
 * @brief Initialize dynamic memory.
 *
 * @par Description:
 * This API is used to initialize the dynamic memory of a doubly linked list.
 * @attention
 * <ul>
 * <li>The size parameter value should match the following two conditions :
 * 1) Be less than or equal to the Memory pool size;
 * 2) Be greater than the size of OS_MEM_MIN_POOL_SIZE.</li>
 * <li>Call this API when dynamic memory needs to be initialized during the startup of Huawei LiteOS.</li>
 * <li>The parameter input must be OS_MEM_ALIGN_SIZE byte-aligned.</li>
 * <li>The init area [pool, pool + size] should not conflict with other pools.</li>
 * </ul>
 *
 * @param pool         [IN] Starting address of memory.
 * @param size         [IN] Memory size.
 * @param attr         [IN] Attributes to config memory pool, see XLT_MEM_INIT_ATTR_SLAB_MSK above.
 *
 * @retval #XLT_NOK    The dynamic memory fails to be initialized.
 * @retval #XLT_OK     The dynamic memory is successfully initialized.
 * @par Dependency:
 * <ul>
 * <li>xlt_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V200R006C10
 */
extern unsigned int xlt_mem_pool_init(void *pool, unsigned int size, unsigned int attr);

/**
 * @ingroup xlt_memory
 * @brief Initialize dynamic memory.
 *
 * @par Description:
 * This API is used to initialize the dynamic memory of a doubly linked list.
 * @attention
 * <ul>
 * <li>The size parameter value should match the following two conditions :
 * 1) Be less than or equal to the Memory pool size;
 * 2) Be greater than the size of OS_MEM_MIN_POOL_SIZE.</li>
 * <li>Call this API when dynamic memory needs to be initialized during the startup of Huawei LiteOS.</li>
 * <li>The parameter input must be OS_MEM_ALIGN_SIZE byte-aligned.</li>
 * <li>The init area [pool, pool + size] should not conflict with other pools.</li>
 * </ul>
 *
 * @param pool         [IN] Starting address of memory.
 * @param size         [IN] Memory size.
 *
 * @retval #XLT_NOK    The dynamic memory fails to be initialized.
 * @retval #XLT_OK     The dynamic memory is successfully initialized.
 * @par Dependency:
 * <ul>
 * <li>xlt_memory.h: the header file that contains the API declaration.</li>
 * </ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_init(void *pool, unsigned int size);
#if !defined(XLTCFG_MEM_MUL_MODULE) || defined(XLTCFG_SUPPORT_COMPAT_MALLOC)
/**
 * @ingroup xlt_memory
 * @brief Allocate dynamic memory.
 *
 * @par Description:
 * This API is used to allocate a memory block of which the size is specified.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The size of the input parameter size must be four byte-aligned.</li>
 * </ul>
 *
 * @param  pool    [IN] Pointer to the memory pool that contains the memory block to be allocated.
 * @param  size    [IN] Size of the memory block to be allocated (unit: byte).
 *
 * @retval #NULL          The memory fails to be allocated.
 * @retval #void*         The memory is successfully allocated, and the API returns the pointer to
 * the allocated memory block.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_realloc | xlt_mem_alloc_align | xlt_mem_free
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_alloc(void *pool, unsigned int size);
/**
 * @ingroup xlt_memory
 * @brief Free dynamic memory.
 *
 * This API is used to free specified dynamic memory that has been allocated.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The input ptr parameter must be allocated by xlt_mem_alloc or xlt_mem_alloc_align or xlt_mem_realloc.</li>
 * </ul>
 *
 * @param  pool  [IN] Pointer to the memory pool that contains the dynamic memory block to be freed.
 * @param  ptr   [IN] Starting address of the memory block to be freed.
 *
 * @retval #XLT_NOK      The memory block fails to be freed because the starting address of the memory block is
 * invalid, or the memory overwriting occurs.
 * @retval #XLT_OK       The memory block is successfully freed.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_alloc | xlt_mem_realloc | xlt_mem_alloc_align
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_free(void *pool, void *ptr);

/**
 * @ingroup xlt_memory
 * @brief Re-allocate a memory block.
 *
 * @par Description:
 * This API is used to allocate a new memory block of which the size is specified by size if the original memory
 * block size is insufficient. The new memory block will copy the data in the original memory block of which the
 * address is specified by ptr. The size of the new memory block determines the maximum size of data to be copied.
 * After the new memory block is created, the original one is freed.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The input ptr parameter must be allocated by xlt_mem_alloc; if the ptr is allocated by xlt_mem_alloc_align,
 * the alignment of the newly allocated memory address cannot be guaranteed to be consistent with the original
 * memory address.</li>
 * <li>The input size parameter can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The input size parameter must be aligned as follows: 1) if the ptr is allocated by xlt_mem_alloc,
 * it must be four byte-aligned; 2) if the ptr is allocated by xlt_mem_alloc_align, it must be aligned with the size of
 * the input parameter boundary of xlt_mem_alloc_align.</li>
 * <li> If the user has special requirements for address alignment, it is not recommended to use this realloc function
 * for address allocated by xlt_mem_alloc_align. For example, on a 32-bit system, LiteOS's default address alignment
 * is 4 bytes, and if the boundary specified in xlt_mem_alloc_align is 8 bytes, it is cannot be ensured that the
 * reallocated address is 8 bytes aligned.</li>
 * </ul>
 *
 * @param  pool     [IN] Pointer to the memory pool that contains the original and new memory blocks.
 * @param  ptr      [IN] Address of the original memory block.
 * @param  size     [IN] Size of the new memory block.
 *
 * @retval #NULL     The memory fails to be re-allocated.
 * @retval #void*    The memory is successfully re-allocated, and the API returns the pointer to the new memory block.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_alloc | xlt_mem_alloc_align | xlt_mem_free
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_realloc(void *pool, void *ptr, unsigned int size);

/**
 * @ingroup xlt_memory
 * @brief Allocate aligned memory.
 *
 * @par Description:
 * This API is used to allocate memory blocks of specified size and of which the starting addresses are aligned on
 * a specified boundary.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The size of the input parameter size can not be greater than the memory pool size that specified at the second
 * input parameter of xlt_mem_init.</li>
 * <li>The alignment parameter value must be a power of 2 with the minimum value being 4.</li>
 * </ul>
 *
 * @param  pool      [IN] Pointer to the memory pool that contains the memory blocks to be allocated.
 * @param  size      [IN] Size of the memory to be allocated.
 * @param  boundary  [IN] Boundary on which the memory is aligned.
 *
 * @retval #NULL    The memory fails to be allocated.
 * @retval #void*   The memory is successfully allocated, and the API returns the pointer to the allocated memory.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_alloc | xlt_mem_realloc | xlt_mem_free
 * @since Huawei LiteOS V100R001C00
 */
extern void *xlt_mem_alloc_align(void *pool, unsigned int size, unsigned int boundary);
#endif

/**
 * @ingroup xlt_memory
 * @brief Get the size of memory pool's size.
 *
 * @par Description:
 * This API is used to get the size of memory pool' total size.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #XLT_NOK        The input parameter pool is NULL.
 * @retval #unsigned int         The size of the memory pool.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_pool_size_get(const void *pool);

/**
 * @ingroup xlt_memory
 * @brief Get the size of memory totally used.
 *
 * @par Description:
 * This API is used to get the size of memory totally used in memory pool.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #XLT_NOK        The input parameter pool is NULL.
 * @retval #unsigned int         The size of the used memory pool.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_total_used_get(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Get the number of free memory nodes.
 *
 * @par Description:
 * This API is used to get the number of free memory nodes in memory pool.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #XLT_NOK        The input parameter pool is NULL.
 * @retval #unsigned int         The number of free memory nodes.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_free_blks_get(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Get the number of used memory nodes.
 *
 * @par Description:
 * This API is used to get the number of used memory nodes in memory pool.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool           [IN] A pointer pointed to the memory pool.
 *
 * @retval #OS_INVALID        The input parameter pool is NULL.
 * @retval #unsigned int         The number of used memory nodes.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_used_blks_get(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Get the task ID of a used memory node.
 *
 * @par Description:
 * This API is used to get the task ID of a used memory node.
 * @attention
 * <ul>
 * <li>The input ptr parameter must be allocated byXLT_MemAlloc orXLT_MemAllocAlign.</li>
 * <li>This interface only support obtain the task ID of a used memory node which is allocated from the system memory
 * pool (OS_SYS_MEM_ADDR) at present.</li>
 * <li>This API can be enabled when you set the macro value of XLTCFG_MEM_DEBUG or XLTCFG_MEM_TASK_STAT is defined
 * in LiteOS.</li>
 * </ul>
 *
 * @param  ptr               [IN] A used memory node.
 *
 * @retval #OS_INVALID        The input parameter ptr is illegal.
 * @retval #unsigned int            The task ID of used memory node ptr.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINTPTR xlt_mem_task_id_get(const void *ptr);

/**
 * @ingroup xlt_memory
 * @brief Get the address of the next byte of the last used byte.
 *
 * @par Description:
 * This API is used to get the address of the next byte of the last used byte.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The last used byte may the last byte of an used node or the last byte of an unused node head</li>
 * </ul>
 *
 * @param  pool               [IN] A pointer pointed to the memory pool.
 *
 * @retval #XLT_NOK           The input parameter pool is NULL.
 * @retval #UINTPTR           The pointer to the next byte of the last used byte.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern UINTPTR xlt_mem_last_used_get(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Get the information of memory pool.
 *
 * @par Description:
 * This API is used to get the information of memory pool.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool                 [IN] A pointer pointed to the memory pool.
 * @param  poolStatus           [OUT] A pointer for storage the pool status
 *
 * @retval #XLT_NOK           The input parameter pool is NULL or invalid.
 * @retval #XLT_OK            Get memory information successfully.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_info_get(const void *pool, xlt_mem_pool_status *pool_status);

/**
 * @ingroup xlt_memory
 * @brief Get the number of free node in every size.
 *
 * @par Description:
 * This API is used to print the number of free node in every size.
 * @attention
 * The input pool parameter must be initialized via func xlt_mem_init.
 *
 * @param  pool               [IN] A pointer pointed to the memory pool.
 *
 * @retval #XLT_NOK           The input parameter pool is NULL.
 * @retval #XLT_OK            Print the number of free node in every size successfully.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_free_node_show(void *pool);

/**
 * @ingroup xlt_memory
 * @brief Check the memory pool integrity.
 *
 * @par Description:
 * This API is used to check the memory pool integrity.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>xlt_mem_integrity_check will be called by malloc function when the macro of XLTCFG_BASE_MEM_NODE_INTEGRITY_CHECK
 * is defined in LiteOS.</li>
 * <li>xlt_mem_integrity_check function can be called by user anytime.</li>
 * </ul>
 *
 * @param  pool              [IN] A pointer to the memory pool.
 *
 * @retval #XLT_NOK           The memory pool (pool) is impaired.
 * @retval #XLT_OK            The memory pool (pool) is integrated.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_integrity_check(void *pool);

#ifdef XLTCFG_BASE_MEM_NODE_SIZE_CHECK
/**
 * @ingroup xlt_memory
 * @brief Check the size of the specified memory node.
 *
 * @par Description:
 * This API is used to check the size of memory node.
 * @attention
 * <ul>
 * <li>The input pool parameter must be initialized via func xlt_mem_init.</li>
 * <li>The input ptr parameter must be allocated by xlt_mem_alloc or xlt_mem_alloc_align.</li>
 * <li>The function will be called by function specified, such as memset or memcpy.</li>
 * <li>The feature can be enabled when you set the macro value of XLTCFG_BASE_MEM_NODE_SIZE_CHECK as YES.</li>
 * <li>You had better set memory check level as XLT_MEM_CHECK_LEVEL_DISABLE when copy bin file.</li>
 * </ul>
 *
 * @param  pool            [IN]  A pointer pointed to the memory pool.
 * @param  ptr             [IN]  A pointer pointed to the source node.
 * @param  totalSize       [OUT] A pointer to save total size, must point to valid memory.
 * @param  availSize       [OUT] A pointer to save available size, must point to valid memory.
 *
 * @retval #XLT_ERRNO_MEMCHECK_DISABLED         Memcheck function does not open.
 * @retval #XLT_ERRNO_MEMCHECK_PARA_NULL        The pool or ptr is NULL.
 * @retval #XLT_ERRNO_MEMCHECK_OUTSIDE          The ptr address is not in the reasonable range.
 * @retval #XLT_ERRNO_MEMCHECK_NO_HEAD          Can't find the control head node from ptr.
 * @retval #XLT_ERRNO_MEMCHECK_WRONG_LEVEL      The memory check level is illegal.
 * @retval #XLT_OK                              Success to get total size and available size of the memory node (ptr).
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_check_level_set | xlt_mem_check_level_get
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_node_size_check(void *pool, void *ptr, unsigned int *total_size, unsigned int *avail_size);

/**
 * @ingroup xlt_memory
 * @brief Set the memory check level.
 *
 * @par Description:
 * This API is used to set the memory check level.
 * @attention
 * <ul>
 * <li>There are three level you can set.</li>
 * <li>The legal level are XLT_MEM_CHECK_LEVEL_LOW, XLT_MEM_CHECK_LEVEL_HIGH, XLT_MEM_CHECK_LEVEL_DISABLE.</li>
 * </ul>
 *
 * @param   check_level                               [IN] The level what you want to set.
 *
 * @retval #XLT_ERRNO_MEMCHECK_WRONG_LEVEL          The input memory check level is illegal.
 * @retval #XLT_OK                                  Set the memory check level successfully.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_node_size_check | xlt_mem_check_level_get
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned int xlt_mem_check_level_set(unsigned char check_level);

/**
 * @ingroup xlt_memory
 * @brief Get the memory check level.
 *
 * @par Description:
 * This API is used to get the current memory check level.
 * @attention None.
 *
 * @param  None.
 *
 * @retval #unsigned char           The current memory check level.
 * <ul><li>xlt_memory.h: the header file that contains the API declaration.</li></ul>
 * @see xlt_mem_node_size_check | xlt_mem_check_level_set
 * @since Huawei LiteOS V100R001C00
 */
extern unsigned char xlt_mem_check_level_get(void);
#endif

/**
 * @ingroup xlt_memory
 * Define a mem size check intensity
 *
 * Lowest mem check.
 */
#define XLT_MEM_CHECK_LEVEL_LOW 0

/**
 * @ingroup xlt_memory
 * Define a mem size check intensity
 *
 * Highest mem check.
 */
#define XLT_MEM_CHECK_LEVEL_HIGH 1

/**
 * @ingroup xlt_memory
 * Define a mem size check intensity
 *
 * disable mem check.
 */
#define XLT_MEM_CHECK_LEVEL_DISABLE 0xff

/**
 * @ingroup xlt_memory
 * Define a mem size check intensity.
 *
 * default intensity set mem check.
 */
#define XLT_MEM_CHECK_LEVEL_DEFAULT XLT_MEM_CHECK_LEVEL_DISABLE


/**
 * @ingroup xlt_memory
 * Define a mem size check intensity.
 *
 * default intensity set mem check.
 */
#define XLT_MEM_CHECK_LEVEL_DEFAULT XLT_MEM_CHECK_LEVEL_DISABLE

/**
 * @ingroup xlt_memory
 * memcheck error code: the pointer or pool is NULL.
 *
 * Value: 0x02000101.
 *
 * Solution: don't give a NULL parameter.
 */
#define XLT_ERRNO_MEMCHECK_PARA_NULL xlt_errno_os_error(XLT_MOD_MEM, 0x1)

/**
 * @ingroup xlt_memory
 * memcheck error code: the pointer address is not in the suitable range.
 *
 * Value: 0x02000102.
 *
 * Solution: check pointer and comfirm it is in stack.
 */
#define XLT_ERRNO_MEMCHECK_OUTSIDE xlt_errno_os_error(XLT_MOD_MEM, 0x2)

/**
 * @ingroup xlt_memory
 * memcheck error code: can't find the control node.
 *
 * Value: 0x02000103.
 *
 * Solution: check if the node which the pointer points to has been freed or not been allocated.
 */
#define XLT_ERRNO_MEMCHECK_NO_HEAD xlt_errno_os_error(XLT_MOD_MEM, 0x3)

/**
 * @ingroup xlt_memory
 * memcheck error code: the memcheck level is wrong.
 *
 * Value: 0x02000104.
 *
 * Solution: check the memcheck level by the function "xlt_mem_check_level_get".
 */
#define XLT_ERRNO_MEMCHECK_WRONG_LEVEL xlt_errno_os_error(XLT_MOD_MEM, 0x4)

/**
 * @ingroup xlt_memory
 * memcheck error code: memcheck function is not enable.
 *
 * Value: 0x02000105.
 *
 * Solution: enable memcheck by the function "xlt_mem_check_level_set".
 */
#define XLT_ERRNO_MEMCHECK_DISABLED xlt_errno_os_error(XLT_MOD_MEM, 0x5)

/**
 * @ingroup xlt_memory
 * memcheck error code: memcheck integrity is err.
 *
 * Value: 0x02000106.
 *
 * Solution: check the memory integrity.
 */
#define XLT_ERRNO_MEMCHECK_ERR xlt_errno_os_error(XLT_MOD_MEM, 0x6)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_MEMORY_H */
