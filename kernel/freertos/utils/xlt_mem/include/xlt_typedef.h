/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description: type define
 */

#ifndef _XLT_TYPEDEF_H
#define _XLT_TYPEDEF_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef FALSE
#define FALSE 0U
#endif

#ifndef TRUE
#define TRUE 1U
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef YES
#undef YES
#endif
#define YES 1

#ifdef NO
#undef NO
#endif
#define NO 0

#define XLT_NULL_BYTE ((unsigned char)0xFF)
#define XLT_NULL_SHORT ((unsigned short)0xFFFF)
#define XLT_NULL_INT ((unsigned int)0xFFFFFFFF)

#ifndef XLT_OK
#define XLT_OK 0
#endif

#ifndef XLT_NOK
#define XLT_NOK 1
#endif

#define OS_FAIL 1
#define OS_ERROR (unsigned int)(-1)
#define OS_INVALID (UINTPTR)(-1)

/* And corresponding power of two alignment */
#ifndef XLTARC_P2ALIGNMENT
#ifdef XLTCFG_AARCH64
#define XLTARC_P2ALIGNMENT 3
#else
#define XLTARC_P2ALIGNMENT 2
#endif
#endif

#ifdef __LP64__
typedef long unsigned int UINT64;
typedef long signed int INT64;
typedef unsigned long UINTPTR;
typedef signed long INTPTR;
typedef __uint128_t UINT128;
#define XLTCFG_AARCH64
#ifndef XLTCFG_LIB_LIBC
typedef INT64 ssize_t;
typedef UINT64 size_t;
#endif
#else
#ifndef STD_DEF_H
typedef unsigned long long UINT64;
#endif
typedef signed long long INT64;
typedef unsigned int UINTPTR;
typedef signed int INTPTR;
#ifndef XLTCFG_LIB_LIBC
typedef signed int ssize_t;
typedef unsigned int size_t;
#endif
#endif

#ifndef _STDBOOL_H
typedef unsigned int bool;
#endif

#ifndef STD_DEF_H
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef signed int         INT32;
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_TYPEDEF_H */
