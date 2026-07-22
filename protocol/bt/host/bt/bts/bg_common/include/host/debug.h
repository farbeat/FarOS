/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    debug.h
Abstract:
    This file contains generic debugging macros for driver development.
    If (DBG == 0) no code is generated; Otherwise macros will expand.
Environment:
    Development Only.
    debug.c must be linked into the driver code to support output.
--------------------------------------------------------------------------- */

#ifdef HUAWEI_ADAPT_VENDOR
#ifndef BTHCONF_INCLUDED
#include "bth_conf.h"
#endif

#if defined(CONFIG_OS_NONE) || defined(CONFIG_OS_LITEOS)
#endif /* debug switch */
#endif /* HUAWEI_ADAPT_VENDOR */

#ifndef DEBUG_H
#define DEBUG_H

/*
 * DEBUG FLAG DEFINITIONS
 */
#define DBG_ERROR_ON        (1 << 27)     /* Display DBG_ERROR messages */
#define DBG_WARNING_ON      (1 << 28)     /* Display DBG_WARNING messages */
#define DBG_DATA_ON         (1 << 29)     /* Enable packet print */
#define DBG_TRACE_ON        (1 << 30)     /* Display ENTER/TRACE/LEAVE messages */

#ifndef DEBUG_ON_PC
#include "log_common.h"
#include "log_def.h"
#endif

#ifdef DEBUG_ON_PC
#define bth_unused(var)
#else
#define bth_unused(var) do { (void)var; } while (0)
#endif

#ifndef STATIC
    #define STATIC
#endif

typedef enum {
    BTH_LOG_LEVEL_FIRST = 1,
    BTH_LOG_LEVEL_ERROR = BTH_LOG_LEVEL_FIRST,
    BTH_LOG_LEVEL_WARNING,
    BTH_LOG_LEVEL_INFO,
    BTH_LOG_LEVEL_BUTT
} bth_log_level_enum;


#ifdef DEBUG_ON_PC

#define bth_log0(ucLoglevel, fmt) printf(fmt); printf("\r\n") // bth_unused(ucLoglevel)

#define bth_log1(ucLoglevel, fmt, p1) printf((fmt), (p1)); printf("\r\n") // bth_unused(ucLoglevel) //

#define bth_log2(ucLoglevel, fmt, p1, p2) bth_unused(ucLoglevel)

#define bth_log3(ucLoglevel, fmt, p1, p2, p3) bth_unused(ucLoglevel)

#define bth_log4(ucLoglevel, fmt, p1, p2, p3, p4) bth_unused(ucLoglevel)
#define print(a)
#else

#define bth_log0(ucLoglevel, fmt)   oml_bt_log_print0(1, (ucLoglevel), (fmt))

#define bth_log1(ucLoglevel, fmt, p1) oml_bt_log_print1(1, (ucLoglevel), (fmt), (p1))

#define bth_log2(ucLoglevel, fmt, p1, p2) oml_bt_log_print2(1, (ucLoglevel), (fmt), (p1), (p2))

#define bth_log3(ucLoglevel, fmt, p1, p2, p3) oml_bt_log_print3(1, (ucLoglevel), (fmt), (p1), (p2), (p3))

#define bth_log4(ucLoglevel, fmt, p1, p2, p3, p4) oml_bt_log_print4(1, (ucLoglevel), (fmt), (p1), (p2), (p3), (p4))

#endif

#ifdef CONFIG_DEBUG

#define CONFIG_DBG_MAXBUFLEN        1200
void debug_init(void);
void debug_done(void);

int dbg_print(const char *fmt, ...);
void dbg_print_data(uint8_t *data, uint16_t num_bytes, uint16_t offset);

extern int32_t dbg_flags;
extern int32_t dbg_level;

typedef void (dbg_print_func)(unsigned int length, unsigned char *data);
void set_dbg_print(dbg_print_func* p);

void dbg_enter(int a, const char *func);
void dbg_leave(int a, const char *func);

/* ---------------------------------------------------------------------------
Description:
    DEBUG tool functions.
--------------------------------------------------------------------------- */
#define dbg_out(d_, a_)
/* other head file is better? */
#define dbg_str(d_, a_)
/* As a backspace, this may cause crash, becareful */
#define dbg_back(d_)        ((char *)(d_))[strlen((char *)(d_)) - 1] = 0

void dbg_remove_last_comma(uint8_t *dbg_str);
void dbg_space_align(uint8_t *s, uint8_t num);
void dbg_auto_warp_print(uint8_t *name, uint8_t *prompt, uint8_t *param);
uint8_t *dbg_get_buf(void);

/*
 *  A - is a protocol ID
 *  B - is a debug flag
 *  S - is a debug info string
 *  F - is a function name
 *  C - is a C conditional
 */
#define dbg_func(F)      static const char __FUNC__[] = (F)
#define dbg_print_data(B, S)   { if (dbg_flags&B) dbg_print S; }
#define dbg_trace(A)    { if ((dbg_flags & DBG_TRACE_ON) && (dbg_flags&(1 << A))) \
                        { dbg_print("%-d---%s:line %d\r\n", dbg_level, __FUNC__, __LINE__); }}

#define dbg_enter(a) dbg_enter((a), __FUNC__)
#define dbg_leave(a) dbg_leave((a), __FUNC__)

#define dbg_error(s)   { if (dbg_flags & DBG_ERROR_ON)   \
    { dbg_print("%s: ERROR: ", __FUNC__); \
    dbg_print (s); \
    }}
#define dbg_warning(s) { if (dbg_flags & DBG_WARNING_ON) \
    { dbg_print("%s: WARNING: ", __FUNC__); \
    dbg_print (s); \
    }}

#define dbg_msg(s) dbg_print (s)

#ifdef CONFIG_MEMORY_LEAK_DETECT
void *dbg_memory_leak_new(void *p, uint32_t off, uint32_t len, char *filename, uint32_t line);
void dbg_memory_leak_free(void *p);
void dbg_memory_leak_dump(void);
uint32_t dbg_memory_usage_summary(void);
#endif

#else /* CONFIG_DEBUG */

#ifdef dbg_print
#undef dbg_print
#endif

#ifdef CONFIG_MICROSOFT
#define dbg_print
#else
#define dbg_print(_fmt, ...)
#endif


#define dbg_func(F)
#define dbg_print_data(B, S)
#define dbg_enter(A)            stk_enter_func(A)
#define dbg_trace(A)
#define dbg_leave(A)            stk_leave_func(A)
#define dbg_error(S)
#define dbg_warning(S)
#define dbg_init(s)
#define dbg_done()
#define dbg_print_data(a, b, c)
#define dbg_msg(s)
#endif /* CONFIG_DEBUG */

#ifdef CONFIG_HW_MEMORY_DEBUG
VOID mem_debug_init(void);
VOID mem_debug_done(void);
VOID mem_debug_print_all_heap_stats_info(void);
VOID mem_debug_print_all_slab_stats_info(void);

#if defined(CONFIG_HW_MEMORY_LEAK_DEBUG) || defined(CONFIG_HW_MEMORY_LEAK_DEBUG_ON_BUFFER)
VOID mem_leak_debug_print_dump(void);
#endif
VOID *mem_leak_debug_new_ext(uint32_t len, uint32_t pos);
VOID *mem_leak_debug_new(uint32_t len, uint32_t pos);
VOID mem_leak_debug_free(VOID *p);

#ifdef CONFIG_HW_MEMORY_INTEGRITY_CHECK_DEBUG
VOID mem_debug_heap_integrity_check(uint16_t file_id, uint16_t line);
#endif

#ifdef CONFIG_HW_MEMORY_OVERFOLOW_DEBUG
int32_t mem_debug_mem_cpy(VOID *des, uint32_t des_size, const VOID *src, uint32_t src_size);
int32_t mem_debug_mem_set(VOID *des, uint32_t des_size, int32_t value, uint32_t src_size);
#endif /* CONFIG_HW_MEMORY_OVERFOLOW_DEBUG */
#endif /* CONFIG_HW_MEMORY_DEBUG */
#endif /* _DEBUG_H */
