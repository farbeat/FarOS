/**
 * Copyright (c) CompanyNameMagicTag 2013-2023. All rights reserved
 * Description: memory config header
 */

#ifndef _MEMMAP_CONFIG_H
#define _MEMMAP_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern void* g_heap;
extern void* g_extend_heap;

#define OS_SYS_MEM_POOL  g_heap
#define OS_EXTEND_MEM_ADDR  g_extend_heap

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MEMMAP_CONFIG_H */
