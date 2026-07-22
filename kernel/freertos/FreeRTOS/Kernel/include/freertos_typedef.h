#ifndef _FREERTOS_TYPEDEF_H_
#define _FREERTOS_TYPEDEF_H_


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_STRING(x)  #x
#define X_STRING(x) OS_STRING(x)

/* type definitions */
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef signed char        INT8;
typedef signed short       INT16;
typedef signed int         INT32;
typedef float              FLOAT;
typedef double             DOUBLE;
typedef char               CHAR;

#if __riscv_xlen == 64
typedef long unsigned int  UINT64;
typedef long signed int    INT64;
typedef unsigned long      UINTPTR;
typedef signed long        INTPTR;
typedef __uint128_t        UINT128;

typedef INT64              ssize_t;
typedef UINT64             size_t;
#else
typedef unsigned long long UINT64;
typedef signed long long   INT64;
typedef unsigned int       UINTPTR;
typedef signed int         INTPTR;

typedef INT32              ssize_t;
typedef UINT32             size_t;
#endif

typedef UINTPTR            AARCHPTR;
typedef size_t             BOOL;

typedef volatile INT32     Atomic;
typedef volatile INT64     Atomic64;

#define VOID               void
#define STATIC             static
#define INLINE             inline

#ifndef FALSE
#define FALSE              0U
#endif

#ifndef TRUE
#define TRUE               1U
#endif

#ifndef NULL
#define NULL               ((VOID *)0)
#endif

#ifdef YES
#undef YES
#endif
#define YES                1

#ifdef NO
#undef NO
#endif
#define NO                 0

#define OS_NULL_BYTE       ((UINT8)0xFF)
#define OS_NULL_SHORT      ((UINT16)0xFFFF)
#define OS_NULL_INT        ((UINT32)0xFFFFFFFF)

#define OS_HEX_ADDR_WIDTH  (sizeof(UINTPTR) * 2)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _FREERTOS_TYPEDEF_H_ */
