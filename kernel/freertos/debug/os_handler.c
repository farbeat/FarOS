#include "task_debug.h"
#include "queue_debug.h"
#include "swtmr_debug.h"
#include "sysinfo_debug.h"
#include "os_handler.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

uint32_t os_mem_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;
#ifdef XLTCFG_MEM_MUL_POOL
    xlt_mem_pool_list();
#endif

    return 0;
}

uint32_t os_mem_nodes_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

#ifdef XLTCFG_MEM_DEBUG
        xlt_mem_tasknodes_print((UINT32)(-1), xlt_printk);
#endif

    return 0;
}

uint32_t os_task_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vTaskInfoPrint();

    return 0;
}

uint32_t os_queue_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vQueueInfoPrint();

    return 0;
}

uint32_t os_swtmr_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vSwtmrInfoPrint();

    return 0;
}

uint32_t os_sys_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vSystemInfoPrint();

    return 0;
}

uint32_t os_semphr_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vSemaphoreInfoPrint();

    return 0;
}

uint32_t os_mutex_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vMutexInfoPrint();

    return 0;
}

uint32_t os_hwi_handler(uint8_t *para, uint32_t para_len, char *str_cmd)
{
    (void)para;
    (void)para_len;
    (void)str_cmd;

    vHwiInfoPrint();

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */