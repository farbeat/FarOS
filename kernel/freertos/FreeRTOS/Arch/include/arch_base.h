#ifndef ARCH_BASE_H
#define ARCH_BASE_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define dsb()                           portMEMORY_BARRIER()
#define TRUNCATE(addr, size)            ((uintptr_t)(addr) & ~((size) - 1))

#ifndef READ_UINT8
#define READ_UINT8(value, addr)         ({ (value) = *((volatile uint8_t *)((uintptr_t)(addr))); dsb(); })
#endif

#ifndef READ_UINT16
#define READ_UINT16(value, addr)        ({ (value) = *((volatile uint16_t *)((uintptr_t)(addr))); dsb(); })
#endif

#ifndef READ_UINT32
#define READ_UINT32(value, addr)        ({ (value) = *((volatile uint32_t *)((uintptr_t)(addr))); dsb(); })
#endif

#ifdef __LP64__
#ifndef READ_UINT64
#define READ_UINT64(value, addr)        ({ (value) = *((volatile uint64_t *)((uintptr_t)(addr))); dsb(); })
#endif
#endif

#ifndef WRITE_UINT8
#define WRITE_UINT8(value, addr)        ({ dsb(); *((volatile uint8_t *)((uintptr_t)(addr))) = (value); })
#endif

#ifndef WRITE_UINT16
#define WRITE_UINT16(value, addr)       ({ dsb(); *((volatile uint16_t *)((uintptr_t)(addr))) = (value); dsb(); })
#endif

#ifndef WRITE_UINT32
#define WRITE_UINT32(value, addr)       ({ dsb(); *((volatile uint32_t *)((uintptr_t)(addr))) = (value); dsb(); })
#endif

#ifdef __LP64__
#ifndef WRITE_UINT64
#define WRITE_UINT64(value, addr)       ({ dsb(); *((volatile uint64_t *)((uintptr_t)(addr))) = (value); })
#endif
#endif

#ifndef GET_UINT8
#define GET_UINT8(addr)                 ({ uint8_t r = *((volatile uint8_t *)((uintptr_t)(addr))); dsb(); r; })
#endif

#ifndef GET_UINT16
#define GET_UINT16(addr)                 ({ uint16_t r = *((volatile uint16_t *)((uintptr_t)(addr))); dsb(); r; })
#endif

#ifndef GET_UINT32
#define GET_UINT32(addr)                 ({ uint32_t r = *((volatile uint32_t *)((uintptr_t)(addr))); dsb(); r; })
#endif

#ifdef __LP64__
#ifndef GET_UINT64
#define GET_UINT64(addr)                 ({ uint64_t r = *((volatile uint64_t *)((uintptr_t)(addr))); dsb(); r; })
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
