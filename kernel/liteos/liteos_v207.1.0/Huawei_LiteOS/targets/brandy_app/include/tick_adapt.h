/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os tick header.
 * Author:
 * Create: 2023-03-09
 */

#ifndef _TICK_ADAPT_H
#define _TICK_ADAPT_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern VOID LOS_SysTickReload(uint32_t uwCyclesPerTick);
extern uint32_t LOS_SysTickCurrCycleGet(VOID);
extern VOID LOS_GetSystickCycle(uint32_t *cntHi, uint32_t *cntLo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _TICK_ADAPT_H */