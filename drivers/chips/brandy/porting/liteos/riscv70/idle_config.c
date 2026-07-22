/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: RISCV70 idle task config for LiteOS
 */

#include "idle_config.h"
#include "core.h"
#include "chip_io.h"
#include "los_task.h"
#include "cmsis_extension.h"
#if CORE == MASTER_BY_ALL
#if ((USE_COMPRESS_LOG_INSTEAD_OF_SDT_LOG == NO) && defined(LIBLOG))
#include "log_oam_msg.h"
#endif
#endif
#include "watchdog.h"
#include "soc_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

cb_uart_is_need_awake_fn g_uart_is_need_awake_callback = NULL;

static void idle_task_process(void)
{
    uapi_watchdog_kick();
    uint32_t irq_sts = osal_irq_lock();
    if (g_uart_is_need_awake_callback != NULL) {
        if (!g_uart_is_need_awake_callback()) {
            osal_irq_restore(irq_sts);
            return;
        }
    }
    osal_irq_restore(irq_sts);
#if SYS_DEBUG_MODE_ENABLE == YES
    log_oam_prase_message();
#endif
}

void osUartVetoCallbackRegister(cb_uart_is_need_awake_fn cb)
{
    if (cb != NULL) {
        g_uart_is_need_awake_callback = (cb_uart_is_need_awake_fn)cb;
    }
}

void idle_task_config(void)
{
    LOS_IdleHandlerHookReg(idle_task_process);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
