/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os memmap header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _MEMMAP_CONFIG_H
#define _MEMMAP_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern void *g_intheap_begin;
extern void *g_intheap_size;
extern void *g_ram_begin;
extern void *g_ram_size;
extern void *g_extend_heap_begin;
extern void *g_extend_heap_size;

#ifdef LOSCFG_LIB_CONFIGURABLE

extern void *g_sysMemAddr;
extern unsigned int g_sysMemSize;
#define OS_SYS_MEM_ADDR     g_sysMemAddr
#define OS_SYS_MEM_SIZE     g_sysMemSize

#else
#define OS_SYS_MEM_ADDR    ((void *)&g_extend_heap_begin)
#define OS_SYS_MEM_SIZE    ((unsigned)&g_extend_heap_size)

#define OS_EXTEND_MEM_ADDR    ((void *)&g_intheap_begin)
#define OS_EXTEND_MEM_SIZE    ((unsigned)&g_intheap_size)

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MEMMAP_CONFIG_H */
