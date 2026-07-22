/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os platform header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _ASM_PLATFORM_H
#define _ASM_PLATFORM_H

#include "los_typedef.h"
#include "register_config.h"
#include "soc/timer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOS_EMG_LEVEL   0

#define LOS_COMMOM_LEVEL   (LOS_EMG_LEVEL + 1)

#define LOS_ERR_LEVEL   (LOS_COMMOM_LEVEL + 1)

#define LOS_WARN_LEVEL  (LOS_ERR_LEVEL + 1)

#define LOS_INFO_LEVEL  (LOS_WARN_LEVEL + 1)

#define LOS_DEBUG_LEVEL (LOS_INFO_LEVEL + 1)

#define PRINT_LEVEL LOS_ERR_LEVEL

#define OS_SYS_CLOCK    (32000000ULL)

#ifndef CACHE_ALIGNED_SIZE
#define CACHE_ALIGNED_SIZE        32
#endif

#ifdef LOSCFG_PLATFORM_OSAPPINIT
extern VOID app_init(VOID);
#endif

#ifdef BUFSIZ
#undef BUFSIZ
#define BUFSIZ 1024
#endif

extern uint32_t oal_get_sleep_ticks(void);
extern void oal_ticks_restore(uint32_t ticks);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ASM_PLATFORM_H */
