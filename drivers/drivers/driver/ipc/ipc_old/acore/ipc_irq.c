/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: IPC HAL implementation - protocol/apps specific.
 * Author: @CompanyNameTag
 * Create:  2020-06-10
 */

#include "ipc.h"
#include "hal_ipc.h"
#include "debug_print.h"

#if defined(USE_CMSIS_OS) && defined(__LITEOS__)
#include "los_hwi.h"
#endif
#include "non_os.h"
#if !(defined BUILD_APPLICATION_SSB || defined UNIT_TEST)
#include "log_oam_logger.h"
#include "log_def.h"
#endif
#include "soc_osal.h"

#if MCU_ONLY
#if (CHIP != TARGET_CHIP_SOCMN1)
#include "trng.h"
#endif
#endif
#include "chip_core_irq.h"
#include "chip_definitions.h"
#include "ipc_irq.h"

#if defined(IPC_DEBUG_INTERRUPTS)
static uint32_t g_numints = 0;
#endif

extern void bt_int0_handler(void);
#if DSP_EXIST == YES
extern void dsp_int0_handler(void);
#endif
#if DUAL_DSP_EXIST == YES
extern void dsp1_int0_handler(void);
#endif

#if CHIP_LIBRA
extern void gnss_int0_handler(void);
#endif
/*
 * Lowest level interrupt handler for IPC mechanism: IPC from BCPU CORE or MCPU core. Should not happen.
 */
#if DSP_EXIST == YES
__IRQ void dsp_int0_handler(void)
{
    hal_ipc_interrupt_clear_core(CORES_PROTOCOL_CORE);

#if defined(IPC_DEBUG_INTERRUPTS)
    g_numints++;
#endif

    ipc_receive_message(CORES_PROTOCOL_CORE);
    ipc_check_idle(CORES_PROTOCOL_CORE);
}
#endif

#if DUAL_DSP_EXIST == YES
__IRQ void dsp1_int0_handler(void)
{
    hal_ipc_interrupt_clear_core(CORES_HIFI1_CORE);

#if defined(IPC_DEBUG_INTERRUPTS)
    g_numints++;
#endif

    ipc_receive_message(CORES_HIFI1_CORE);
    ipc_check_idle(CORES_HIFI1_CORE);
}
#endif

/*
 * Lowest level interrupt handler for IPC mechanism: IPC from BCPU CORE. Dispatch to driver.
 */
__IRQ void bt_int0_handler(void)
{
    hal_ipc_interrupt_clear_core(CORES_BT_CORE);
#if ARCH == RISCV31
    int_clear_pending_irq(BCPU_INT0_ID);
#endif

#if defined(IPC_DEBUG_INTERRUPTS)
    g_numints++;
#endif
    ipc_receive_message(CORES_BT_CORE);
    ipc_check_idle(CORES_BT_CORE);
}

#if CHIP_LIBRA && !(defined BUILD_APPLICATION_SSB)
__IRQ void gnss_int0_handler(void)
{
    hal_ipc_interrupt_clear_core(CORES_GNSS_CORE);

#if defined(IPC_DEBUG_INTERRUPTS)
    g_numints++;
#endif

    ipc_receive_message(CORES_GNSS_CORE);
    ipc_check_idle(CORES_GNSS_CORE);
}
#endif

/*
 * Turn on ALL IPC interrupts.
 */
void ipc_init_interrupts(void)
{
#ifdef USE_CMSIS_OS
#if (CHIP_LIBRA) && CHIP_ASIC
    uapi_drv_cipher_trng_random_buffer_init(TRNG_RAND_BUF_UPDATA_ALL);
    uapi_drv_cipher_trng_ipc_updata_reigister();
#endif
#endif
#ifdef __FREERTOS__
    osal_irq_request(BT_INT0_IRQN, bt_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(BT_INT0_IRQN, irq_prio(BT_INT0_IRQN));
    osal_irq_enable(BT_INT0_IRQN);
#if DSP_EXIST == YES
    osal_irq_request(DSP0_INT0_IRQn, dsp_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(DSP0_INT0_IRQn, irq_prio(DSP0_INT0_IRQn));
    osal_irq_enable(DSP0_INT0_IRQn);
#endif
#if DUAL_DSP_EXIST == YES
    osal_irq_request(DSP1_INT0_IRQN, dsp1_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(DSP1_INT0_IRQN, irq_prio(DSP1_INT0_IRQN));
    osal_irq_enable(DSP1_INT0_IRQN);
#endif
#if CHIP_LIBRA
    osal_irq_request(GNSS_INT0_IRQn, gnss_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(GNSS_INT0_IRQn, irq_prio(GNSS_INT0_IRQn));
    osal_irq_enable(GNSS_INT0_IRQn);
#endif
#else
    osal_irq_request(BT_INT0_IRQN, (osal_irq_handler)bt_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(BT_INT0_IRQN, irq_prio(BT_INT0_IRQN));
    osal_irq_enable(BT_INT0_IRQN);
#if DSP_EXIST == YES
    osal_irq_request(DSP0_INT0_IRQn, (osal_irq_handler)dsp_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(DSP0_INT0_IRQn, irq_prio(DSP0_INT0_IRQn));
    osal_irq_enable(DSP0_INT0_IRQn);
#endif
#if DUAL_DSP_EXIST == YES
    osal_irq_request(DSP1_INT0_IRQN, (osal_irq_handler)dsp1_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(DSP1_INT0_IRQN, irq_prio(DSP1_INT0_IRQN));
    osal_irq_enable(DSP1_INT0_IRQN);
#endif
#if CHIP_LIBRA
    osal_irq_request(GNSS_INT0_IRQn, (osal_irq_handler)gnss_int0_handler, NULL, NULL, NULL);
    osal_irq_set_priority(GNSS_INT0_IRQn, irq_prio(GNSS_INT0_IRQn));
    osal_irq_enable(GNSS_INT0_IRQn);
#endif
#endif
}

/*
 * Turn off all IPC interrupts.
 */
void ipc_deinit_interrupts(void)
{
    osal_irq_disable(BT_INT0_IRQN);
#if DSP_EXIST == YES
    osal_irq_disable(DSP0_INT0_IRQn);
#endif
#if DUAL_DSP_EXIST == YES
    osal_irq_disable(DSP1_INT0_IRQN);
#endif
#if CHIP_LIBRA
    osal_irq_disable(GNSS_INT0_IRQn);
#endif
}
