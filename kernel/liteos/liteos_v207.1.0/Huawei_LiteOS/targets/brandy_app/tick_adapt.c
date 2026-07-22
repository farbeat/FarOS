/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  os tick adapt source.
 * Author:
 * Create: 2023-03-09
 */

#include "los_tick_pri.h"
#include "los_task_pri.h"
#include "los_swtmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Description : reconfig systick, and clear SysTick_IRQn
 * Input       : cyclesPerTick --- cycles Per Tick
 */
LITE_OS_SEC_TEXT_MINOR VOID LOS_SysTickReload(uint32_t cyclesPerTick)
{
    (VOID)cyclesPerTick;
}

/*
 * Description : Get System cycle count
 * return      : hwCycle --- the system cycle count
 */
LITE_OS_SEC_TEXT_MINOR uint32_t LOS_SysTickCurrCycleGet(VOID)
{
    return 0;
}

/*
 * Description : Get Sys tick cycle count
 * output      : cntHi  --- SysTick count High 4 byte
 *               cntLo  --- SysTick count Low 4 byte
 */
LITE_OS_SEC_TEXT_MINOR VOID LOS_GetSystickCycle(uint32_t *cntHi, uint32_t *cntLo)
{
    (VOID)cntHi;
    (VOID)cntLo;
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
