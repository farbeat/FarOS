/*
 * Copyright (c) CompanyNameMagicTag 2012-2021. All rights reserved.
 * Description:  dfx debug defines headfile
 */


#include "task_debug.h"
#include "queue_debug.h"
#include "swtmr_debug.h"
#include "sysinfo_debug.h"
#include "debug_cmd.h"

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

static int os_mem_handler_testsuite(int argc, const char *argv[])
{
    if ((argc == 0) ||
        ((argc == 1) && (strcmp(argv[0], "-k") == 0)) ||
        ((argc == 1) && (strcmp(argv[0], "-m") == 0))) {
        xlt_mem_pool_list();
    } else if ((argc == 1) && (strcmp(argv[0], "-a") == 0)) {
#ifdef XLTCFG_MEM_DEBUG
        xlt_mem_tasknodes_print((Uint)(-1), xlt_printk);
#endif
    } else {
        xlt_printk("free [-k/-m]       : show mem info\n");
#ifdef XLTCFG_MEM_DEBUG
        xlt_printk("free -a            : show all nodes info\n");
#endif
    }
    return 0;
}

static int os_task_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vTaskInfoPrint();
    return 0;
}

static int os_queue_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vQueueInfoPrint();
    return 0;
}

static int os_swtmr_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vSwtmrInfoPrint();
    return 0;
}

static int os_sys_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vSystemInfoPrint();
    return 0;
}

static int os_semphr_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vSemaphoreInfoPrint();
    return 0;
}

static int os_mutex_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vMutexInfoPrint();
    return 0;
}

static int os_hwi_handler_testsuite(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    vHwiInfoPrint();
    return 0;
}


void debug_cmd_reg(void)
{
    add_function("free", "print the information of mem pool and slab", os_mem_handler_testsuite);
    add_function("task", "display all task information ", os_task_handler_testsuite);
    add_function("queue", "display all queue information ", os_queue_handler_testsuite);
    add_function("swtmr", "display all software timer information ", os_swtmr_handler_testsuite);
    add_function("systeminfo", "display system information ", os_sys_handler_testsuite);
    add_function("sem", "display all semphore information ", os_semphr_handler_testsuite);
    add_function("mutex", "display all mutex information ", os_mutex_handler_testsuite);
    add_function("hwi", "display hwi information ", os_hwi_handler_testsuite);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */