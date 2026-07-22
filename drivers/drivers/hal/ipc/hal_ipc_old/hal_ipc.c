/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: IPC HAL implementation.
 * Author: @CompanyNameTag
 * Create:  2020-06-10
 */

#include "hal_ipc.h"
#include "arch_barrier.h"
#include "chip_io.h"

#define IPC_TRY_SEND_MAX 10000
#define BT_DEEPSLEEP_FLAG 0x5a5a
#define IPC_TRY_INTERVAL_TIME 100
/*
 * MCPU core trigger a interrupt to BCPU core.
 */
void hal_ipc_interrupt_security(void)
{
    volatile uint32_t cnt = IPC_TRY_SEND_MAX;
    while (cnt > 0) {
        if (readw(GLB_CTL_B_RB_BASE + 0x1C) != BT_DEEPSLEEP_FLAG) {
            if (cnt < IPC_TRY_SEND_MAX) {
                uapi_tcxo_delay_us(IPC_TRY_INTERVAL_TIME);
            }
            MCPU_SOFT_INT_SET |= M_TO_BCPU_INT_OFFSET;
            break;
        }
        cnt--;
    }
}

/*
 * MCPU core trigger a interrupt to (dsp) or (dsp0) core
 */
static void hal_ipc_interrupt_dsp(void)
{
    MCPU_SOFT_INT_SET |= M_TO_DSP_INT_OFFSET;
}

#if DUAL_DSP_EXIST == YES
/*
 * MCPU core trigger a interrupt to DSP1 core.
 */
static void hal_ipc_interrupt_dsp1(void)
{
    MCPU_SOFT_INT_SET |= M_TO_DSP_1_INT_OFFSET;
}
#endif

#if CHIP_LIBRA
/*
 * MCPU core trigger a interrupt to GCPU core.
 */
static void hal_ipc_interrupt_gcpu(void)
{
    MCPU_SOFT_INT_SET |= M_TO_GNSS_INT0_OFFSET;
}

void hal_ipc_interrupt1_gcpu(void)
{
    MCPU_SOFT_INT_SET |= M_TO_GNSS_INT1_OFFSET;
}
#endif

void hal_ipc_interrupt_apps(void)
{
}

/*
 * Helper to get the last interrupt's clear status.
 */
bool hal_ipc_get_core_clr_sts(cores_t core)
{
    UNUSED(core);
    uint16_t core_clr_sts_bit = 0;
    switch (core) {
        case CORES_BT_CORE:
            core_clr_sts_bit = M_TO_BCPU_INT_OFFSET;
            break;

        case CORES_PROTOCOL_CORE:
            core_clr_sts_bit = M_TO_DSP_INT_OFFSET;
            break;
#if DUAL_DSP_EXIST == YES
        case CORES_HIFI1_CORE:
            core_clr_sts_bit = M_TO_DSP_1_INT_OFFSET;
            break;
#endif
#if CHIP_LIBRA
        case CORES_GNSS_CORE:
            core_clr_sts_bit = M_TO_GNSS_INT0_OFFSET;
            break;

        case CORES_SEC_CORE:
            core_clr_sts_bit = M_TO_SEC_INT0_OFFSET;
            break;
#endif
        default:
            break;
    }
    // if core int clear sts is 1, chip mustn't enable.
    if ((MCPU_SOFT_INT_CLR_STS & core_clr_sts_bit) != 0) {
        return true;
    }
    return false;
}

/*
 * Helper to interrupt a given core.
 */
void hal_ipc_interrupt_core(cores_t core)
{
    if (hal_ipc_get_core_clr_sts(core)) { return; }
    // Make sure to clear the M7 write buffer
    dsb();
    switch (core) {
        case CORES_BT_CORE:
            hal_ipc_interrupt_security();
            break;

        case CORES_PROTOCOL_CORE:
            hal_ipc_interrupt_dsp();
            break;

        case CORES_APPS_CORE:
            hal_ipc_interrupt_apps();
            break;
#if DUAL_DSP_EXIST == YES
        case CORES_HIFI1_CORE:
            hal_ipc_interrupt_dsp1();
            break;
#endif
#if CHIP_LIBRA
        case CORES_GNSS_CORE:
            hal_ipc_interrupt_gcpu();
            break;
#endif
        default:
            break;
    }
}

void hal_ipc_interrupt_en(void)
{
    MCPU_SOFT_INT_EN |= M_TO_ALL_INT;
}

void hal_ipc_interrupt_clear_core(cores_t core)
{
    // Make sure to clear the M7 write buffer
    switch (core) {
        case CORES_BT_CORE:
            MCPU_SOFT_INT_CLR |= M_TO_BCPU_INT_OFFSET;
            break;

        case CORES_PROTOCOL_CORE:
            MCPU_SOFT_INT_CLR |= M_TO_DSP_INT_OFFSET;
            break;

#if DUAL_DSP_EXIST == YES
        case CORES_HIFI1_CORE:
            MCPU_SOFT_INT_CLR |= M_TO_DSP_1_INT_OFFSET;;
            break;
#endif
#if CHIP_LIBRA
        case CORES_GNSS_CORE:
            MCPU_SOFT_INT_CLR |= M_TO_GNSS_INT0_OFFSET;
            break;
#endif
        default:
            break;
    }
}

void hal_ipc_interrupt1_clear_core(cores_t core)
{
#if CHIP_LIBRA
    // Make sure to clear the M7 write buffer
    if (core == CORES_GNSS_CORE) {
        MCPU_SOFT_INT_CLR |= M_TO_GNSS_INT1_OFFSET;
    }
#else
    UNUSED(core);
#endif
}