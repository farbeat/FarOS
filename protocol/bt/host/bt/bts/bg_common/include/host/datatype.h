/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */


#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "stdint.h"
#include "errcode.h"

#ifdef BTH_TEST
#ifndef size_t
typedef unsigned int            size_t;
#endif
#endif

#ifndef BT_IS_DIGIT
#define bt_is_digit(c)    (((c) >= '0') && ((c) <= '9'))
#endif

/* Data type definition */

// #ifndef ULONG
// #define ULONG    unsigned long
// #endif

// #ifndef LONG
// #define LONG    signed long
// #endif

#ifndef DWORD
#define DWORD    unsigned long
#endif

#ifndef PBYTE
#define PBYTE   unsigned char *
#endif

#ifndef PDWORD
#define PDWORD  unsigned long *
#endif

#ifndef PVOID
#define PVOID    void *
#endif

#ifndef PCHAR
#define PCHAR   signed char *
#endif

#ifndef SYSTEM_LOCK
#define SYSTEM_LOCK        void *
#endif

#ifndef TIMER_ID
#define TIMER_ID    DWORD
#endif

#ifndef NULL
#define NULL    0
#endif
#ifndef TRUE
#define TRUE    1
#define FALSE    0
#endif

#ifdef CONFIG_CPU_BIT32
#define PTR_BITS 2
#else
#define PTR_BITS 2
#endif

#ifndef LPCSTR
#define LPCSTR     const char *
#endif

#ifndef LPSTR
#define LPSTR     char *
#endif

#ifndef LPVOID
#define LPVOID    void *
#endif

#ifndef LPDWORD
#define LPDWORD unsigned long *
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (-1)
#endif

#ifndef HANDLE
#define HANDLE void *
#endif

typedef    void *bt_handle;
typedef    void *bt_file_hdl;
typedef void *bt_find_hdl;

#ifndef INVALID_FILE_HDL
#define INVALID_FILE_HDL    NULL
#endif

#ifndef INVALID_FIND_HDL
#define INVALID_FIND_HDL    NULL
#endif

#define BDLENGTH        6
#define LINKKEYLENGTH    16
#define MAXPINLENGTH    16
#define DEVCLSLENGTH    3
#define MAX_NAME_LEN    64
#define IRK_HBKP_LEN    16
#define RANDOM_LEN    16
#define RANDOM_SHORT_LEN    3

enum {
    BT_FAIL = 0,
    BT_SUCCESS = 1,
    BT_DIRECT_RETURN = 2,
    BT_CONTINUE = 3,
};

#define INVALID_STATUS    (-1)
#define BT_SHIFT_BITS_29    29
#define BT_SHIFT_BITS_24    24
#define BT_SHIFT_BITS_16    16
#define BT_SHIFT_BITS_14    14
#define BT_SHIFT_BITS_12    12
#define BT_SHIFT_BITS_10    10
#define BT_SHIFT_BITS_9     9
#define BT_SHIFT_BITS_8     8
#define BT_SHIFT_BITS_7     7
#define BT_SHIFT_BITS_6     6
#define BT_SHIFT_BITS_5     5
#define BT_SHIFT_BITS_4     4
#define BT_SHIFT_BITS_3     3
#define BT_SHIFT_BITS_2     2
#define BT_SHIFT_BITS_1     1

#define BT_INDEX_24    24
#define BT_INDEX_16    16
#define BT_INDEX_15    15
#define BT_INDEX_14    14
#define BT_INDEX_13    13
#define BT_INDEX_12    12
#define BT_INDEX_11    11
#define BT_INDEX_10    10
#define BT_INDEX_9     9
#define BT_INDEX_8     8
#define BT_INDEX_7     7
#define BT_INDEX_6     6
#define BT_INDEX_5     5
#define BT_INDEX_4     4
#define BT_INDEX_3     3
#define BT_INDEX_2     2
#define BT_INDEX_1     1
#define BT_INDEX_0     0

#define BT_OCTETS_32    32
#define BT_OCTETS_31    31
#define BT_OCTETS_30    30
#define BT_OCTETS_29    29
#define BT_OCTETS_24    24
#define BT_OCTETS_20    20
#define BT_OCTETS_19    19
#define BT_OCTETS_18    18
#define BT_OCTETS_17    17
#define BT_OCTETS_16    16
#define BT_OCTETS_15    15
#define BT_OCTETS_11    11
#define BT_OCTETS_10    10
#define BT_OCTETS_9     9
#define BT_OCTETS_8     8
#define BT_OCTETS_7     7
#define BT_OCTETS_6     6
#define BT_OCTETS_5     5
#define BT_OCTETS_4     4
#define BT_OCTETS_3     3
#define BT_OCTETS_2     2
#define BT_OCTETS_1     1
#define BT_OCTETS_0     0

#define BTH_OCTET_BIT_LEN 8

#define BD_ADDR_LEN    6    /* bluetooth address length */
#define UUID_16BIT_LEN 2
#define UUID_128BIT_LEN 16

#define minself(x, y)     if ((uint32_t)(y) < (uint32_t)(x)) { (x) = (y); }
#define maxself(x, y)     if ((uint32_t)(y) > (uint32_t)(x)) { (x) = (y); }

#define u16_low_u8(val)  ((uint8_t)((uint16_t)(val) & 0xff))
#define u16_high_u8(val) ((uint8_t)(((uint16_t)(val) >> 8) & 0xff))
#ifndef min
#define min(a, b) (((uint32_t)(a) > (uint32_t)(b)) ? (b) : (a))
#endif

#ifndef max
#define max(a, b) (((uint32_t)(a) > (uint32_t)(b)) ? (a) : (b))
#endif

#ifndef uuid16_t
typedef uint8_t uuid16_t[UUID_16BIT_LEN];
#endif

typedef struct _bt_uint64stru {
    uint32_t    low32;
    uint32_t    high32;
} bt_uint64stru;

#endif

