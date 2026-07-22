/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void vTimerTickInit( void );
void vTimerTickStart( void );

void vTimerEnable( void );
void vTimerDisable( void );

uint64_t ullTickGetCycles( void );

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HAL_TIMER_H */
