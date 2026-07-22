/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: XLT memory Module Implementation
 * --------------------------------------------------------------------------- */

/**
 * @defgroup xlt_slab Slab
 * @ingroup xlt
 */

#ifndef XLT_SLAB_H
#define XLT_SLAB_H

#include "xlt_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup xlt_slab
 * total size of each slab class.
 */
#define SLAB_MEM_ALLOCATOR_SIZE     512U

#define SLAB_BASIC_NEED_SIZE        0x1000

/**
 * @ingroup xlt_slab
 * slab bucket size type.
 */
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
enum slab_bucket_size_type {
    SLAB_BUCKET_SIZE_HALF_OF_HEAP    = 0x1UL,   /**< a half of heap size slab mem can use */
    SLAB_BUCKET_SIZE_QUARTER_OF_HEAP,           /**< one quarter of heap size slab mem can use */
    SLAB_BUCKET_SIZE_TYPE_MAX
};

#define SLAB_MEM_BUCKET_SIZE_TYPE   SLAB_BUCKET_SIZE_HALF_OF_HEAP
#endif

/**
 * @ingroup xlt_slab
 * @brief configure slab size.
 *
 * @par Description:
 * This API is used to configure slab size.
 * @attention
 * <ul>
 * <li>The function should be called before function xlt_mem_init if necessary.</li>
 * <li>The function takes effect when XLTCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE is NO.</li>
 * </ul>
 *
 * @param cfg     [IN] the pointer to slab size config table. Note that the pointer cannot be null.
 * @param cnt     [IN] slab class size, must be equal to SLAB_MEM_COUNT(the number of slab class).
 *
 * @retval None.
 * @par Dependency:
 * <ul>
 * <li>xlt_slab.h: the header file that contains the API declaration.</li>
 * </ul>
 */
extern void xlt_slab_size_cfg(UINT32 *cfg, UINT32 cnt, UINT32 step_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* XLT_SLAB_H */
