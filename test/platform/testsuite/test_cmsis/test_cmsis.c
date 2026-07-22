/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: Test cap sensor source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-05-28， Create file. \n
 */

#include "cmsis_os2.h"
#include "los_swtmr_pri.h"
#include "los_queue_pri.h"
#include "los_task_pri.h"
#include "math.h"
#include "common_def.h"
#include "cts_common.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "nv_porting.h"
#include "los_mux.h"
#include "tcxo.h"
#include "timer.h"
#include "watchdog.h"
#include "chip_core_irq.h"
#include "test_cmsis.h"

#define EXT_DEFAULT_TSKNAME    "default"
#define EXT_DEFAULT_TSKPRIO    20
#define EXT_DEFAULT_STACKSIZE  (4 * 1024)
#define EXT_SYS_MSG_PARAM_NUM_MAX 4
#define KERNEL_UNLOCKED 0
#define KERNEL_LOCKED   1
#define check_test_result(a, b, c, d) check_test_result_print(a, b, c, d, __FUNCTION__, __LINE__)
#define TEST_SUITE_TASK_PRIO 20
#define TIMER_MAX_TIMERS  10
#define TEST_TIMER        1
#define TEST_TIMER_IRQ TIMER_1_IRQN
#define PARAM_10        10
#define PARAM_2         2
#define PARAM_3         3
#define PARAM_4         4
#define PARAM_5         5
#define PARAM_6         6
#define PARAM_7         7
#define PARAM_8         8
#define PARAM_9         9
#define PARAM_16        16
#define PARAM_20        20
#define PARAM_30        30
#define PARAM_32        32
#define PARAM_35        35
#define PARAM_50        50
#define PARAM_100       100
#define PARAM_200       200
#define PARAM_1000      1000
#define PARAM_5000      5000
#define PARAM_10000     10000
#define PARAM_100000    100000
#define PARAM_10_MILLION 10000000
#define PARAM_X_1    0x1
#define PARAM_X_5    0x5
#define PARAM_X_10   0x10
#define PARAM_X_22   0x22
#define PARAM_X_33   0x33
#define PARAM_X_2000000 0x2000000
#define PARAM_X_FFFFFFFF 0xFFFFFFFF
#define PARAM_X_02000000 0x02000000
#define PARAM_X_00000004 0x00000004
#define CB_SIZE         100
#define STACK_SIZE      2048
#define STACK_SIZE_1    1024
#define STACK_SIZE_2    4096
#define SLEEP_10000     10000
#define SLEEP_5000      5000
#define SLEEP_2000      2000
#define SLEEP_1000      1000
#define SLEEP_800       800
#define SLEEP_500       500
#define SLEEP_300       300
#define SLEEP_100       100
#define SLEEP_50        50
#define SLEEP_20        20
#define SLEEP_10        10
#define FREQ_32000000   32000000
#define PRODUCT_CFG_TASK_STACK_APP_CTS 2048
#define PRODUCT_CFG_TASK_PRIO_APP_CTS 26
#define TIMER1_ID                   5
#define TEST_TIMER_FREQ 25000000
#define TEST_TIMER_FREQ_1 50000000
#define NUM_HAL_INTERRUPT_TIMER5    (5 + OS_RISCV_SYS_VECTOR_CNT)
#define G_VECTOR                    NUM_HAL_INTERRUPT_TIMER5
#define MAX_COUNT       65534

static uint32_t s_msg_index;
static uint32_t test_index;
static uint32_t g_timer_start; // timer 开始的时间，用于测试定时器准确度
static uint32_t g_timer_arrive; // timer 到期的时间，用于测试定时器准确度
static uint64_t g_timer_start_u64; // timer 开始的时间，用于测试定时器准确度
static uint64_t g_timer_arrive_u64; // timer 到期的时间，用于测试定时器准确度
static uint32_t g_test_bits = 5;
static uint32_t g_test_bits1 = 8;
static uint32_t g_event_id = 0;
static uint32_t g_mux_handle = 0;
static uint32_t g_task_index = 0;
static uint32_t g_task_index1 = 0;
static uint8_t g_test_index = 0;
static uint32_t g_thread_arg = 5;
static uint32_t g_count = 0;
static osThreadId_t  g_threadid = 0;
static osThreadId_t  g_threadid_1 = 0;
static osThreadId_t  g_testid[50] = {0};
static osPriority_t g_priority = 30;
static uint32_t g_press_test_cnt = 100000; // 压力测试次数:100000
static uint32_t g_press_test_cnt_1 = 10000; // 压力测试次数:10000
static uint32_t g_press_test_cnt_2 = 1000; // 压力测试次数:1000
timer_handle_t timer_tmp[TIMER_MAX_TIMERS] = {0};
static uint32_t time_out = 100;

osThreadAttr_t thread_attr = {"test_treadname", 0, NULL, 100, NULL, 2048, osPriorityAboveNormal, 0, 0};
osTimerId_t g_timer_id;
osTimerId_t g_timer_id1;
uint32_t cb_mem[100];
uint32_t timer_count = 0;
uint32_t timer_out = 0;
osTimerAttr_t time_attr = {"test_Timer", 32, &cb_mem, 100};
osMutexId_t g_mutex_id;
osMutexAttr_t g_mutex_attr = {"myThreadMutex", osMutexRecursive | osMutexPrioInherit, NULL, 0U};
osThreadAttr_t muxthread_attr[2] = {0};
osEventFlagsId_t g_evt_id;
osEventFlagsId_t g_evt_id1;
osEventFlagsAttr_t g_flags_attr = {"", 0, NULL, 0};
osSemaphoreId_t g_semaphore_id;
osMessageQueueId_t g_mq_id[40];

typedef struct {
    uint32_t msg_id; /* < Message ID.CNcomment:消息ID CNend */
    uintptr_t param[EXT_SYS_MSG_PARAM_NUM_MAX]; /* < Message parameter.CNcomment:消息参数 CNend */
} ext_sys_queue_msg;

typedef struct {
    uint8_t timer_usage;  /* Number of used system timers.CNcomment:当前使用系统定时器个数 CNend */
    uint8_t task_usage;   /* Number of used tasks.CNcomment:当前使用任务个数 CNend */
    uint8_t sem_usage;    /* Number of used semaphores.CNcomment:当前使用信号量个数 CNend */
    uint8_t queue_usage;  /* Number of used message queues.CNcomment:当前使用消息队列个数 CNend */
    uint8_t mux_usage;    /* Number of used mutexes.CNcomment:当前使用互斥锁个数 CNend */
    uint8_t event_usage;  /* Number of used events.CNcomment:当前使用事件个数 CNend */
    uint16_t err_info;    /* Error statistic EXT_OS_STAT_XXX, used to log occurred errors.
                           CNcomment:错误统计量EXT_OS_STAT_XXX，用于记录曾经出现过的错误 CNend */
} ext_os_resource_use_stat;

typedef struct {
    td_u16 task_prio;  /* Task priority.CNcomment:任务优先级 CNend */
    td_u32 stack_size; /* Task stack size.CNcomment:栈大小 CNend */
    td_u32 task_policy; /* Task schedule policy. Not used by default.CNcomment:任务调度策略。默认没有使用。CNend */
    td_u32 task_nice;   /* Task nice value. Not used by default.CNcomment:任务优先权。默认没有使用。CNend */
    td_u32 task_cpuid;  /* CPUID to which the task belongs. Not used by default.
                             CNcomment:任务所属的CPUID。默认没有使用。CNend */
    td_char *task_name; /* Task name.CNcomment:任务名 CNend */
    td_void *resved; /* Reserved. Not used by default.CNcomment:预留字段。默认没有使用。CNend */
} ext_task_attr;

static void thread_cntbody1(void *argument)
{
    unused(argument);
    test_suite_log_stringf("thread_cntbody1\r\n");
    while (1) {
        g_task_index ++;
        if (g_task_index > PARAM_10) {
            break;
        }
    }
}
static void thread_cntbody2(void *argument)
{
    unused(argument);
    test_suite_log_stringf("thread_cntbody2\r\n");
    while (1) {
        g_task_index1 ++;
        if (g_task_index1 > PARAM_10) {
            break;
        }
    }
}

int cts_os_kernel_initialize(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t ret = (osStatus_t)TEST_SUITE_OK;
    switch (param) {
        case NORMAL_TEST:
            ret = osKernelInitialize(); // 不支持重复初始化
            if (ret != osError) {
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

/*
    获取RTOS内核信息
    argv[1] : 内核标识字符串缓冲区大小
*/
int cts_os_kernel_getinfo(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    osStatus_t ret = osOK;
    osVersion_t ver = {0};
    uint32_t id_size = strtol(argv[1], NULL, 0);
    char *id_buf = TD_NULL;
    id_buf = kv_malloc(id_size);
    if (id_buf == TD_NULL) {
        test_suite_log_stringf("malloc failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    switch (param) {
        case NORMAL_TEST:
            ret = osKernelGetInfo(&ver, id_buf, id_size);
            test_suite_log_stringf("ret = %d,ver.api = %d,ver.kernel = %d,id_buf = %s, id_size= %d\r\n", ret, ver.api,
                                   ver.kernel, id_buf, id_size);
            if (ret != osOK) {
                test_suite_log_stringf("err ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case ABNORMAL_TEST:
            ret = osKernelGetInfo(&ver, id_buf, 0);
            if (ret != osError) {
                test_suite_log_stringf("id_size err case, ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("id_size get succ, ret = %d\r\n", ret);
            ret = osKernelGetInfo(NULL, id_buf, id_size);
            if (ret != osError) {
                test_suite_log_stringf("ver err case, ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("ver get succ, ret = %d\r\n", ret);
            ret = osKernelGetInfo(&ver, NULL, id_size);
            if (ret != osError) {
                test_suite_log_stringf("id_buf err case, ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("id_buf get succ, ret = %d\r\n", ret);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(id_buf);
    return test_result;
}

int cts_os_kernel_getstate(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osKernelState_t ret;

    switch (param) {
        case NORMAL_TEST:
            ret = osKernelGetState();
            test_suite_log_stringf("ret : %d***\r\n", ret);
            if (ret != osKernelRunning) {
                test_suite_log_stringf("err ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_kernel_start(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t ret = osOK;
    osKernelState_t ret1;

    switch (param) {
        case NORMAL_TEST:
            ret = osKernelStart();
            test_suite_log_stringf("ret = %d\r\n", ret);
            if (ret != (osStatus_t)osKernelError) {
                test_suite_log_stringf("os Kernel Start Fail, ret = %d\r\n", ret);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            ret1 = osKernelGetState();
            test_suite_log_stringf("ret_state = %d\r\n", ret1);
            if (ret1 != osKernelRunning) {
                test_suite_log_stringf("os Kernel GetState Fail, ret_state = %d\r\n", ret1);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_kernel_lock(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    int32_t ret_lock;
    int32_t ret_unlock;
    osKernelState_t ret_state;
    g_task_index = 0;
    g_task_index1 = 0;
    test_index = 0;
    thread_attr.name = "test_thread_yield";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = TEST_SUITE_TASK_PRIO + 1;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    switch (param) {
        case NORMAL_TEST:
            ret_lock = osKernelLock();
            check_test_result(ret_lock, KERNEL_UNLOCKED, TD_TRUE, &test_result);

            ret_state = osKernelGetState();
            check_test_result(ret_state, osKernelLocked, TD_TRUE, &test_result);

            ret_unlock = osKernelUnlock();
            check_test_result(ret_unlock, KERNEL_LOCKED, TD_TRUE, &test_result);

            ret_state = osKernelGetState();
            check_test_result(ret_state, osKernelRunning, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1:
            ret_lock = osKernelLock();
            check_test_result(ret_lock, KERNEL_UNLOCKED, TD_TRUE, &test_result);

            g_threadid = osThreadNew(thread_cntbody1, &g_thread_arg, &thread_attr);
            PRINTK("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                osKernelUnlock();
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_attr.name = "test_thread_yield002";
            g_threadid_1 = osThreadNew(thread_cntbody2, &g_thread_arg, &thread_attr);
            PRINTK("g_threadid_1 = %d\r\n", g_threadid_1);
            if (g_threadid == NULL) {
                osKernelUnlock();
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            if (test_index < g_press_test_cnt_1) { // 延时
                test_index ++;
            }
            if (!((g_task_index == 0) && (g_task_index1 == 0))) {
                test_suite_log_stringf("[uapi_task_lock]fail.\n");
                LOS_TaskUnlock();
                return TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("uapi_task_lock success.\n");

            ret_unlock = osKernelUnlock();
            check_test_result(ret_unlock, KERNEL_LOCKED, TD_TRUE, &test_result);
            osal_msleep(SLEEP_5000);
            if (!((g_task_index > 0) && (g_task_index1 > 0))) {
                test_suite_log_stringf(" uapi_task_unlock fail.\n");
                return TEST_SUITE_TEST_FAILED;
            }
            break;
        case PRESSURE_TEST:
            for (uint32_t i = 0; i < g_press_test_cnt ;i++) {
                ret_lock = osKernelLock();
                if (KERNEL_UNLOCKED != ret_lock) {
                    test_suite_log_stringf("ret_lock =%d\r\n", ret_lock);
                    return TEST_SUITE_TEST_FAILED;
                }
                ret_unlock = osKernelUnlock();
                if (KERNEL_LOCKED != ret_unlock) {
                    test_suite_log_stringf("ret_lock =%d\r\n", ret_lock);
                    return TEST_SUITE_TEST_FAILED;
                }
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_kernel_unlock(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    int32_t ret_unlock;
    test_index = 0;

    switch (param) {
        case NORMAL_TEST:
            if (test_index < g_press_test_cnt_1) {
                test_index ++;
                ret_unlock = osKernelUnlock();
                check_test_result(ret_unlock, KERNEL_UNLOCKED, TD_TRUE, &test_result);
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void test_tick_count_body(uint32_t data)
{
    uint64_t tick = 0;
    tick = osKernelGetTickCount();
    uint64_t timer_delay = 0;
    uint32_t ret;
    uint32_t test_result = TEST_SUITE_OK;

    timer_delay = tick - data;
    test_suite_log_stringf("data = %u,tick = %llu tick, timer_delay is = %llu tick\r\n", data, tick, timer_delay);
    if ((timer_delay - PARAM_10) > PARAM_2) { // 10是时钟的超时时间，这里写死了
        test_result = TEST_SUITE_TEST_FAILED;
    }
    if (test_result == (uint32_t)TEST_SUITE_TEST_FAILED) {
        test_suite_log_stringf("[test_tick_count_body] fail");
    } else {
        test_suite_log_stringf("[test_tick_count_body] success");
    }

    ret = uapi_timer_delete(timer_tmp);
    if (ret != ERRCODE_SUCC) {
            test_suite_log_stringf("ret FAIL.\r\n");
    } else {
        test_suite_log_stringf("ret SUCC.\r\n");
    }
}

int cts_os_kernel_gettickcount(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t tick;
    uint32_t tick1 = 0;
    uint32_t tick2 = 0;
    osStatus_t status = osOK;
    uint32_t expire = PARAM_100000; // 10tick

    switch (param) {
        case NORMAL_TEST: // 相同时钟源
            tick = osKernelGetTickCount();
            tick1 = tick + PARAM_1000; // delay 1000 ticks periodically
            status = osDelayUntil(tick1);
            tick2 = osKernelGetTickCount();
            test_suite_log_stringf("tick = %d,Delay_status = %d,tick2 = %d\r\n", tick, status, tick2);
            break;
        case NORMAL_TEST_1: // 不同时钟源
            uapi_timer_init();
            uapi_timer_adapter(TEST_TIMER, TEST_TIMER_IRQ, 1);
            uapi_timer_create(TEST_TIMER, &timer_tmp[0]);

            tick = osKernelGetTickCount();
            uapi_timer_start(timer_tmp, expire, test_tick_count_body, 1);
            test_suite_log_stringf("tick = %d,Delay_status = %d\r\n", tick, status);
            break;
        case PRESSURE_TEST: // 压测
            for (uint32_t i = 0; i < g_press_test_cnt_2; i++) {
                tick1 = osKernelGetTickCount() + PARAM_3; // delay 1 ticks periodically
                status = osDelayUntil(tick1);
                tick2 = osKernelGetTickCount();
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_kernel_gettickfreq(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t freq;

    switch (param) {
        case NORMAL_TEST:
            freq = osKernelGetTickFreq();
            test_suite_log_stringf("os Kernel Get_Tick tick freq = %d\r\n", freq);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_kernel_getsystimercount(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t sys_timer_count;

    switch (param) {
        case NORMAL_TEST:
            sys_timer_count = osKernelGetSysTimerCount();
            test_suite_log_stringf("sys_timer_count = %u\r\n", sys_timer_count);
            if (sys_timer_count == 0) {
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case PRESSURE_TEST: // 压测
            for (uint32_t i = 0; i < g_press_test_cnt; i++) {
                sys_timer_count = osKernelGetSysTimerCount();
                if (sys_timer_count == 0) {
                    test_result = TEST_SUITE_TEST_FAILED;
                    test_suite_log_stringf("cnt = %u, sys_timer_count = %u\r\n", i, sys_timer_count);
                    break;
                }
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_kernel_getsystimerfreq(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t freq;

    switch (param) {
        case NORMAL_TEST:
            freq = osKernelGetSysTimerFreq();
            test_suite_log_stringf("freq =  %d\r\n", freq);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void threadfunc(void *argument)
{
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *param = (uint32_t *)argument;
    osThreadId_t get_id;
    osThreadState_t state;
    uint32_t stacksize;
    uint32_t stackspace;
    osStatus_t pristatus;
    int pro_get_pri;
    int aft_get_pri;
    const char *name = TD_NULL;
    if (param != NULL && param != (uint32_t *)NULL_POINTER_TEST_1) {
        test_suite_log_stringf("argument = %d\r\n", *param);
    } else if (param == (uint32_t *)NULL_POINTER_TEST_1) {
        test_suite_log_stringf("attr = NULL,properties are configured as follows \r\n");
        thread_attr.name = EXT_DEFAULT_TSKNAME;
        thread_attr.priority = EXT_DEFAULT_TSKPRIO;
        thread_attr.stack_size = EXT_DEFAULT_STACKSIZE;
    }

    osDelay(SLEEP_100);
    while (1) {
        // 获取线程id
        get_id = osThreadGetId();
        test_suite_log_stringf("get_id = %d\r\n", get_id);
        if (get_id == g_threadid) {
            g_test_index++;
            test_suite_log_stringf("os ThreadGetId get_id = %d,g_test_index = %d\r\n", get_id, g_test_index);
        } else {
            test_suite_log_stringf("first run get_id = %d\r\n", get_id);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }

        state = osThreadGetState(g_threadid);
        check_test_result(state, osThreadRunning, TD_TRUE, &test_result);
        stacksize = osThreadGetStackSize(g_threadid);
        stackspace = osThreadGetStackSpace(g_threadid);
        if (thread_attr.stack_size % PARAM_16 != 0) {
                thread_attr.stack_size = thread_attr.stack_size + PARAM_16 - thread_attr.stack_size % PARAM_16;
                test_suite_log_stringf("thread_attr.stack_size = %d\r\n", thread_attr.stack_size);
        }
        if (stacksize != thread_attr.stack_size || stackspace == 0) {
            test_suite_log_stringf("fail,please check,stacksize = %d, stackspace = %d\r\n", stacksize, stackspace);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }

        if (g_test_index == 1) {
            pro_get_pri = osThreadGetPriority(g_threadid);
            check_test_result(pro_get_pri, thread_attr.priority, TD_TRUE, &test_result);
        }
        pristatus = osThreadSetPriority(g_threadid, g_priority);
        check_test_result(pristatus, osOK, TD_TRUE, &test_result);
        aft_get_pri = osThreadGetPriority(g_threadid);
        check_test_result(aft_get_pri, g_priority, TD_TRUE, &test_result);

        name = osThreadGetName(g_threadid);
        test_suite_log_stringf("osThreadGetName err ,name = %s\r\n", name);
        if (name != thread_attr.name) {
            test_suite_log_stringf("osThreadGetName err ,name = %s\r\n", name);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }
        if (g_test_index >= PARAM_2 && g_threadid == get_id) {
            break;
        }
        osDelay(SLEEP_100); // 1S
    }

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("test threadfunc success\r\n");
    } else {
        test_suite_log_stringf("test threadfunc fail,Please cheak!!!\r\n");
    }
    g_test_index = 0;
    return;
}

static void thread_susbody(void *ulparam)
{
    uint32_t test_result = TEST_SUITE_OK;
    unused(ulparam);
    osThreadState_t state;
    osStatus_t sus_status;

    osDelay(SLEEP_10);
    state = osThreadGetState(g_threadid);
    check_test_result(state, osThreadRunning, TD_TRUE, &test_result);
    sus_status = osThreadSuspend(g_threadid);
    check_test_result(sus_status, osOK, TD_TRUE, &test_result);
    // 任务体内获取一定是运行态,任务体外一般不是运行态，是就绪态或者阻塞态
    state = osThreadGetState(g_threadid);
    check_test_result(state, osThreadRunning, TD_TRUE, &test_result);
    osDelay(SLEEP_10000); // 10s为删除中使用，不能删除

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("task_susbody success\r\n");
    } else {
        test_suite_log_stringf("task_susbody fail\r\n");
    }
    return;
}

static void thread_susbody1(void *ulparam)
{
    uint32_t test_result = TEST_SUITE_OK;
    unused(ulparam);
    osDelay(SLEEP_10);
    osStatus_t sus_status;
    test_suite_log_stringf("g_test_index = %d\r\n", g_test_index);

    sus_status = osThreadSuspend(g_testid[g_test_index]);
    check_test_result(sus_status, osOK, TD_TRUE, &test_result);
    osal_msleep(SLEEP_10000); // 10s为删除中使用，不能删除
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("task_susbody success\r\n");
    } else {
        test_suite_log_stringf("task_susbody fail\r\n");
    }
    return;
}

static void thread_rsmbody(void *ulparam)
{
    uint32_t test_result = TEST_SUITE_OK;
    unused(ulparam);
    osThreadId_t get_id = 0;
    osStatus_t res_status;
    osDelay(SLEEP_10);

    // 获取线程id
    get_id = osThreadGetId();
    test_suite_log_stringf("get_id = %d\r\n", get_id);
    if (get_id == g_threadid) {
        g_test_index++;
        test_suite_log_stringf("os ThreadGetId get_id = %d,g_test_index = %d\r\n", get_id, g_test_index);
    } else {
        test_suite_log_stringf("first run get_id = %d\r\n", get_id);
        test_result = TEST_SUITE_TEST_FAILED;
    }

    res_status = osThreadResume(get_id);
    check_test_result(res_status, osOK, TD_TRUE, &test_result);

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("thread_rsmbody success\r\n");
    } else {
        test_suite_log_stringf("thread_rsmbody fail\r\n");
    }
    return;
}

static void thread_cntbody(void *ulparam)
{
    ext_errno ret;
    uint32_t test_result = TEST_SUITE_OK;
    unused(ulparam);
    osThreadId_t get_id = 0;
    osDelay(SLEEP_10);

    while (g_task_index <= PARAM_10) {
        get_id = osThreadGetId();
        test_suite_log_stringf("get_id = %d\r\n", get_id);
        g_task_index ++;
        if (get_id == g_threadid) {
            g_test_index++;
            test_suite_log_stringf("os ThreadGetId get_id = %d,g_test_index = %d\r\n", get_id, g_test_index);
        } else {
            test_suite_log_stringf("first run get_id = %d\r\n", get_id);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }

        ret = osal_msleep(SLEEP_1000);
        check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
        if (g_task_index > PARAM_10) {
            if (test_result == TEST_SUITE_OK) {
                test_suite_log_stringf("thread_cntbody success\r\n");
            } else {
                test_suite_log_stringf("thread_cntbody fail\r\n");
            }
            break;
        }
    }
    g_task_index = 0;
    return;
}

static void thread_setpri_body(void *ulparam)
{
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t param = *(uint32_t*)ulparam;
    test_suite_log_stringf("case param = %d!\r\n", param);
    osStatus_t status;
    osPriority_t priority;
    osDelay(SLEEP_10);

    priority = osThreadGetPriority(g_threadid);
    check_test_result(priority, thread_attr.priority, TD_TRUE, &test_result);

    if (param < osPriorityLow3 || param > osPriorityHigh) {
        status = osThreadSetPriority(g_threadid, g_priority);
        check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
    } else {
        status = osThreadSetPriority(g_threadid, g_priority);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        priority = osThreadGetPriority(g_threadid);
        check_test_result(priority, g_priority, TD_TRUE, &test_result);
    }

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("thread_setpri_body success\r\n");
    } else {
        test_suite_log_stringf("thread_setpri_body fail\r\n");
    }
    return;
}

static void thread_delay_body(void *ulparam)
{
    unused(ulparam);
    osThreadId_t get_id = 0;
    osDelay(SLEEP_10);
     // 获取线程id
    get_id = osThreadGetId();
    test_suite_log_stringf("get_id = %d\r\n", get_id);
    if (get_id == g_threadid) {
        test_suite_log_stringf("os ThreadGetId get_id = %d\r\n", get_id);
    } else {
        test_suite_log_stringf("first run get_id = %d\r\n", get_id);
    }
    osDelay(SLEEP_1000);
    return;
}

void cts_stackspace(void)
{
    uint32_t aft_space = 0;
    aft_space = osThreadGetStackSpace(g_threadid);
    test_suite_log_stringf("aft_space = %d\r\n", aft_space);
}

void cts_stackspace1(void)
{
    uint32_t aft_space = 0;

    aft_space = osThreadGetStackSpace(g_threadid);
    test_suite_log_stringf("aft_space = %d\r\n", aft_space);
}

static void thread_stackspace_body(void *ulparam)
{
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t param = *(uint32_t*)ulparam;
    test_suite_log_stringf("case param = %d!\r\n", param);
    osDelay(SLEEP_10);
    uint32_t pre_space = 0;

    pre_space = osThreadGetStackSpace(g_threadid);
    test_suite_log_stringf("pre_space = %d\r\n", pre_space);
    if (param == NORMAL_TEST) {
        cts_stackspace();
    } else {
        cts_stackspace1();
    }

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("thread_stackspace_body success\r\n");
    } else {
        test_suite_log_stringf("thread_stackspace_body fail\r\n");
    }
    return;
}


int cts_os_threadnew(int argc, char *argv[])
{
    if (argc != PARAM_8) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t ret = TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t arg = strtol(argv[1], NULL, 0);
    thread_attr.name = argv[PARAM_2];
    thread_attr.attr_bits = strtol(argv[PARAM_3], NULL, 0);
    thread_attr.cb_size = strtol(argv[PARAM_4], NULL, 0);
    thread_attr.stack_size = strtol(argv[PARAM_5], NULL, 0);
    thread_attr.priority = strtol(argv[PARAM_6], NULL, 0);
    thread_attr.tz_module = strtol(argv[PARAM_7], NULL, 0);
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_log_stringf("arg = %d,name = %s,attr_bits = %d,priority = %d,tz_module = %d\r\n", arg, thread_attr.name,
                           thread_attr.attr_bits, thread_attr.priority, thread_attr.tz_module);
    osThreadState_t state;
    osStatus_t status;
    osThreadId_t thread_id[PARAM_35] = {0};
    ext_mdm_mem_info mem_info = {0};
    ext_mdm_mem_info mem_info1 = {0};
    uint32_t create_cnt = 1;
    ext_os_resource_use_stat os_resource_use = {0};
    g_test_index = 0;

    switch (param) {
        case NORMAL_TEST:
            g_threadid = osThreadNew(threadfunc, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            test_suite_log_stringf("stack_size = %d\r\n", thread_attr.stack_size);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1: // 挂起后创建 （高优先级挂起，低优先级执行）
        case NORMAL_TEST_2: // 释放后创建
            g_threadid = osThreadNew(thread_susbody, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_100);
            state = osThreadGetState(g_threadid);
            check_test_result(state, osThreadBlocked, TD_TRUE, &test_result);

            // 再创建
            thread_attr.priority = thread_attr.priority - 1;
            g_threadid_1 = osThreadNew(thread_cntbody1, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid_1 = %d\r\n", g_threadid_1);
            if (g_threadid_1 == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 释放
            status = osThreadResume(g_threadid);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            if (param == NORMAL_TEST_2) {
                // 再创建
                thread_id[0] = osThreadNew(thread_cntbody2, &arg, &thread_attr);
                osDelay(SLEEP_10);
                test_suite_log_stringf("thread_id[0] = %d\r\n", thread_id[0]);
                if (thread_id[0] == NULL) {
                    test_suite_log_stringf("osThreadNew not success\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }
            break;
        case NORMAL_TEST_3: // 高优先级延时，低优先级执行
            g_threadid = osThreadNew(thread_delay_body, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_attr.priority = thread_attr.priority - PARAM_2;
            // 再创建
            g_threadid_1 = osThreadNew(thread_cntbody1, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid_1 = %d\r\n", g_threadid_1);
            if (g_threadid_1 == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_4: // 多任务，次高任务挂起
            g_threadid = osThreadNew(thread_susbody, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_100);
            state = osThreadGetState(g_threadid);
            check_test_result(state, osThreadBlocked, TD_TRUE, &test_result);

            // 再创建
            thread_attr.priority = thread_attr.priority - PARAM_2;
            g_testid[g_test_index] = osThreadNew(thread_susbody1, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_testid[g_test_index] = %d, g_test_index = %d\r\n", g_testid[g_test_index],
                                   g_test_index);
            if (g_testid[g_test_index] == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            osDelay(SLEEP_100);
            state = osThreadGetState(g_testid[g_test_index]);
            check_test_result(state, osThreadBlocked, TD_TRUE, &test_result);
            // 释放
            status = osThreadResume(g_threadid);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osThreadResume(g_testid[g_test_index]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            g_threadid = osThreadNew(threadfunc, NULL, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 =	%d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST_1:
            arg = NULL_POINTER_TEST_1;
            g_threadid = osThreadNew(threadfunc, &arg, NULL);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew should not success,g_threadId3 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST_2:
            g_threadid = osThreadNew(NULL, &arg, &thread_attr);
            if (g_threadid != NULL) {
                test_suite_log_stringf("osThreadNew success,g_threadId1 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_attr.name = NULL;
            g_threadid = osThreadNew(threadfunc, &arg, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid != NULL) {
                test_suite_log_stringf("osThreadNew success,g_threadId2 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case PRESSURE_TEST:
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            test_suite_log_stringf("before mem total size = %d,mem free size = %d\r\n", mem_info.total, mem_info.free);

            for (uint32_t i = 0; i < g_press_test_cnt_2; i++) {
                g_threadid = osThreadNew(thread_susbody, &arg, &thread_attr);
                osDelay(SLEEP_10);
                if (g_threadid == NULL) {
                    test_suite_log_stringf("osThreadNew not success\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
                status = osThreadTerminate(g_threadid);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            osal_msleep(SLEEP_1000);

            test_suite_log_stringf("after mem total size =%d,mem free size = %d\r\n", mem_info1.total, mem_info1.free);
            check_test_result(mem_info1.total, mem_info.total, TD_TRUE, &test_result);
            check_test_result(mem_info1.free, mem_info.free, TD_TRUE, &test_result);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST_1:
            test_suite_log_stringf("already use task num: %d \r\n", os_resource_use.task_usage);

            for (g_test_index = os_resource_use.task_usage; g_test_index < LOSCFG_BASE_CORE_TSK_LIMIT; g_test_index++) {
                g_testid[g_test_index] = osThreadNew(thread_susbody1, &arg, &thread_attr);
                osDelay(SLEEP_10);
                test_suite_log_stringf("thread_id[create_cnt]:%d cnt:%u\r\n", g_testid[g_test_index], g_test_index);
                if (g_testid[g_test_index] == NULL) {
                    test_suite_log_stringf("osThreadNew not success,task_id[create_cnt]:%d cnt:%u\r\n",
                                           g_testid[g_test_index], g_test_index);
                    test_result = TEST_SUITE_TEST_FAILED;
                    break;
                }
            }
            if (test_result == TEST_SUITE_OK) {
                test_suite_log_stringf("after task create max, begin create cnt:%u\r\n", g_test_index);
                thread_id[create_cnt] = osThreadNew(thread_susbody1, &arg, &thread_attr); // 继续创建失败task_susbody1
                osDelay(SLEEP_10);
                if (g_testid[g_test_index] != NULL) {
                    test_suite_log_stringf("osThreadNew not success,task_id[create_cnt]:%d cnt:%u\r\n",
                                           g_testid[g_test_index], g_test_index);
                    test_result = TEST_SUITE_TEST_FAILED;
                    break;
                }
            }
            g_test_index--; // 去掉create失败的这个句柄
            while (g_test_index >= os_resource_use.task_usage) {
                test_suite_log_stringf("uapi_task_delete, begin create cnt:%u\r\n", g_test_index);
                status = osThreadTerminate(g_testid[g_test_index]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                g_test_index--;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void threadgetname_normaltest(uint32_t *test_result)
{
    g_threadid = osThreadNew(threadfunc, &g_thread_arg, &thread_attr);
    test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
    if (g_threadid == NULL) {
        test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

void threadgetname_nullpointertest(const char *name, uint32_t *test_result)
{
    name = osThreadGetName(NULL);
    test_suite_log_stringf("name = %s\r\n", name);
    if (name != NULL) {
        test_suite_log_stringf("os Thread_GetName Fail,name = %s\r\n", name);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

int cts_os_threadgetname(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    const char *name = TD_NULL;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    thread_attr.name = argv[1];
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            threadgetname_normaltest(&test_result);
            break;
        case ABNORMAL_TEST:
            name = osThreadGetName(&threadid);
            if (name != NULL) {
                test_suite_log_stringf("os Thread_GetName Fail,name = %s\r\n", name);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            threadgetname_nullpointertest(name, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_threadgetid(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osThreadId_t get_id;
    thread_attr.name = "test_treadname";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_threadid = osThreadNew(threadfunc, &g_thread_arg, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadGetId Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1:
            get_id = osThreadGetId();
            test_suite_log_stringf("get_id = %d\r\n", get_id);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadGetId Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_threadgetstate(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    threadid.taskStatus = 0;
    osThreadState_t state;
    thread_attr.name = "test_thread_get_state";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    switch (param) {
        case NORMAL_TEST:
            g_threadid = osThreadNew(threadfunc, &g_thread_arg, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_800);
            state = osThreadGetState(g_threadid); // 释放后的状态是0：inactive
            test_suite_log_stringf("state = %d\r\n", state);
            check_test_result(state, osThreadInactive, TD_TRUE, &test_result);
            break;

        case ABNORMAL_TEST:
            state = osThreadGetState(&threadid);
            test_suite_log_stringf("state = %d\r\n", state);
            check_test_result(state, osThreadError, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            state = osThreadGetState(NULL);
            test_suite_log_stringf("state = %d\r\n", state);
            check_test_result(state, osThreadError, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void threadgetstacksize_abnormaltest(uint32_t param, uint32_t get_stack_size, osStatus_t status, uint32_t *test_result)
{
    if (param == NORMAL_TEST) {
        g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        get_stack_size = osThreadGetStackSize(g_threadid);
        test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_1) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        get_stack_size = osThreadGetStackSize(g_threadid);
        test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    }
    osal_msleep(SLEEP_1000);
    get_stack_size = osThreadGetStackSize(g_threadid);
    test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
    if (param != ABNORMAL_TEST) {
        check_test_result(get_stack_size, thread_attr.stack_size, TD_TRUE, *&test_result);
        status = osThreadTerminate(g_threadid);
        test_suite_log_stringf("! abnoraml status = %d\r\n", status);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else {
        test_suite_log_stringf("get_stack_size = %d\r\n", get_stack_size);
        check_test_result(get_stack_size, STACK_SIZE_2, TD_TRUE, *&test_result);
    }
}

void threadgetstacksize_abnormaltest2(uint32_t param, uint32_t get_stack_size, osStatus_t status, uint32_t *test_result)
{
    if (param == NORMAL_TEST_2) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_100);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        get_stack_size = osThreadGetStackSize(g_threadid);
        test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        osal_msleep(SLEEP_5000);
        status = osThreadResume(g_threadid);
        test_suite_log_stringf("osthreadresume status = %d\r\n", status);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else if (param == ABNORMAL_TEST) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        get_stack_size = osThreadGetStackSize(g_threadid);
        test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    }
    osal_msleep(SLEEP_1000);
    get_stack_size = osThreadGetStackSize(g_threadid);
    test_suite_log_stringf("g_threadid get_stack_size is = %d\r\n", get_stack_size);
    if (param != ABNORMAL_TEST) {
        check_test_result(get_stack_size, thread_attr.stack_size, TD_TRUE, *&test_result);
        status = osThreadTerminate(g_threadid);
        test_suite_log_stringf("! abnoraml status = %d\r\n", status);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else {
        test_suite_log_stringf("get_stack_size = %d\r\n", get_stack_size);
        check_test_result(get_stack_size, STACK_SIZE_2, TD_TRUE, *&test_result);
    }
}

void threadgetstacksize_abnormaltest1(uint32_t get_stack_size, uint32_t threadid, uint32_t *test_result)
{
    get_stack_size = osThreadGetStackSize(&threadid);
    test_suite_log_stringf("stack_size = %d\r\n", get_stack_size);
    if (get_stack_size != 0) {
        test_suite_log_stringf("stack_size = %d\r\n", get_stack_size);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

void threadgetstacksize_nullpointertest1(uint32_t get_stack_size, uint32_t *test_result)
{
    get_stack_size = osThreadGetStackSize(TD_NULL);
    test_suite_log_stringf("stack_size = %d\r\n", get_stack_size);
    if (get_stack_size != 0) {
        test_suite_log_stringf("stack_size = %d\r\n", get_stack_size);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

int cts_os_threadgetstacksize(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t threadid = SLEEP_1000;
    uint32_t get_stack_size = 0;
    osStatus_t status = osOK;
    thread_attr.name = "test_thread_stack_size";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE_2;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST: // 任务体外获取
        case NORMAL_TEST_1: // 挂起后获取
            threadgetstacksize_abnormaltest(param, get_stack_size, status, &test_result);
            break;
        case NORMAL_TEST_2: // 释放后获取
        case ABNORMAL_TEST: // 终止后获取
            threadgetstacksize_abnormaltest2(param, get_stack_size, status, &test_result);
            break;
        case ABNORMAL_TEST_1:
            threadgetstacksize_abnormaltest1(get_stack_size, threadid, &test_result);
            break;
        case NULL_POINTER_TEST:
            threadgetstacksize_nullpointertest1(get_stack_size, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_threadgetstackspace(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    uint32_t stack_space;
    thread_attr.name = "test_thread_stack_space";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = strtol(argv[1], NULL, 0);
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST: // 剩余栈空间减少
        case NORMAL_TEST_1: // 剩余栈空间溢出
            g_threadid = osThreadNew(thread_stackspace_body, &param, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case ABNORMAL_TEST_1:
            stack_space = osThreadGetStackSpace(&threadid);
            check_test_result(stack_space, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            stack_space = osThreadGetStackSpace(NULL);
            check_test_result(stack_space, 0, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void threadsetpriority_abnormal(uint32_t param, osStatus_t status, osPriority_t get_pri, uint32_t *test_result)
{
    if (param == NORMAL_TEST_1) {
        g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_2) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_3) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        osal_msleep(SLEEP_5000);
        status = osThreadResume(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else if (param == ABNORMAL_TEST) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    }
    status = osThreadSetPriority(g_threadid, g_priority);
    if (param != ABNORMAL_TEST) {
        check_test_result(status, osOK, TD_TRUE, *&test_result);
        get_pri = osThreadGetPriority(g_threadid);
        check_test_result(get_pri, g_priority, TD_TRUE, *&test_result);
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else {
        check_test_result(status, osErrorResource, TD_TRUE, *&test_result);
    }
}

void threadsetpriority_normal(uint32_t *test_result)
{
    g_threadid = osThreadNew(thread_setpri_body, &g_priority, &thread_attr);
    osDelay(SLEEP_10);
    test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
    if (g_threadid == NULL) {
        test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

int cts_os_threadsetpriority(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    osStatus_t status = osOK;
    osPriority_t get_pri = osPriorityNone;
    g_priority = strtol(argv[1], NULL, 0);
    thread_attr.name = "test_thread_set_priority";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    switch (param) {
        case NORMAL_TEST:
            threadsetpriority_normal(&test_result);
            break;
        case NORMAL_TEST_1: // 任务体外设置
        case NORMAL_TEST_2: // 挂起后设置
        case NORMAL_TEST_3: // 释放后设置
        case ABNORMAL_TEST: // 终止后设置
            threadsetpriority_abnormal(param, status, get_pri, &test_result);
            break;
        case ABNORMAL_TEST_1:
            status = osThreadSetPriority(g_threadid, g_priority);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_2:
            status = osThreadSetPriority(&threadid, g_priority);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osThreadSetPriority(TD_NULL, g_priority);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void threadgetpriority_abnormaltest(uint32_t param, osStatus_t status, osPriority_t priority, uint32_t *test_result)
{
        if (param == NORMAL_TEST) {
        g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        priority = osThreadGetPriority(g_threadid);
        test_suite_log_stringf("g_threadid priority is = %d\r\n", priority);
    if (g_threadid == NULL) {
        test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_1) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        priority = osThreadGetPriority(g_threadid);
        test_suite_log_stringf("g_threadid priority is = %d\r\n", priority);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_2) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        priority = osThreadGetPriority(g_threadid);
        test_suite_log_stringf("g_threadid priority is = %d\r\n", priority);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        osal_msleep(SLEEP_5000);
        status = osThreadResume(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else if (param == ABNORMAL_TEST) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        priority = osThreadGetPriority(g_threadid);
        test_suite_log_stringf("g_threadid priority is = %d\r\n", priority);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    }
    osal_msleep(SLEEP_1000);
    priority = osThreadGetPriority(g_threadid);
    test_suite_log_stringf("++++++g_threadid priority is = %d\r\n", priority);
    if (param != ABNORMAL_TEST) {
        check_test_result(priority, thread_attr.priority, TD_TRUE, *&test_result);
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else {
        check_test_result(priority, osPriorityError, TD_TRUE, *&test_result);
    }
}

int cts_os_threadgetpriority(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    osPriority_t priority = osPriorityNone;
    osStatus_t status = osOK;
    thread_attr.name = "test_thread_get_priority";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    switch (param) {
        case NORMAL_TEST: // 任务体外获取
        case NORMAL_TEST_1: // 挂起后获取
        case NORMAL_TEST_2: // 释放后获取
        case ABNORMAL_TEST: // 终止后获取
            threadgetpriority_abnormaltest(param, status, priority, &test_result);
            break;
        case ABNORMAL_TEST_1:
            priority = osThreadGetPriority(g_threadid);
            test_suite_log_stringf("priority=%d\r\n", priority);
            check_test_result(priority, osPriorityError, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_2:
            priority = osThreadGetPriority(&threadid);
            test_suite_log_stringf("priority=%d\r\n", priority);
            check_test_result(priority, osPriorityError, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            priority = osThreadGetPriority(TD_NULL);
            test_suite_log_stringf("priority=%d\r\n", priority);
            check_test_result(priority, osPriorityError, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void thread_yield_test1(void *argument)
{
    unused(argument);
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t status;
    test_suite_log_stringf("g_testIndex1 = %d\r\n", g_test_index);
    if (g_test_index != 0) {
        test_suite_log_stringf("g_test_index = %d\r\n", g_test_index);
    } else {
        g_test_index ++;
        status = osThreadYield();
        check_test_result(status, osOK, TD_TRUE, &test_result);
        if (g_test_index != PARAM_2) {
            test_suite_log_stringf("g_test_index = %d\r\n", g_test_index);
            test_result = TEST_SUITE_TEST_FAILED;
        }
        g_test_index ++;
    }
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("thread_yield_test1 test success\r\n");
    } else {
        test_suite_log_stringf("thread_yield_test1 test fail\r\n");
    }
    status = osThreadTerminate(g_threadid);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    return;
}

void thread_yield_test2(void *argument)
{
    unused(argument);
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t status;
    PRINTK("g_testIndex2 = %d\r\n", g_test_index);
    if (g_test_index != 1) {
        PRINTK("g_test_index error = %d\r\n", g_test_index);
        test_result = TEST_SUITE_TEST_FAILED;
    } else {
        g_test_index ++;
    }
    if (test_result == TEST_SUITE_OK) {
        PRINTK("thread_yield_test2 test success\r\n");
    } else {
        PRINTK("thread_yield_test2 test fail\r\n");
    }
    status = osThreadTerminate(g_threadid_1);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    return;
}

int cts_os_threadyield(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    int32_t ret_lock = 0;
    int32_t ret_unlock = 0;
    osStatus_t status;
    thread_attr.name = "test_thread_yield";
    thread_attr.cb_size = PARAM_100;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = PARAM_35;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_test_index = 0;
            ret_lock = osKernelLock();
            PRINTK("ret_lock = %d\r\n", ret_lock);
            check_test_result(ret_lock, KERNEL_UNLOCKED, TD_TRUE, &test_result);

            g_threadid = osThreadNew(thread_yield_test1, &g_thread_arg, &thread_attr);
            PRINTK("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            thread_attr.name = "test_thread_yield002";
            g_threadid_1 = osThreadNew(thread_yield_test2, &g_thread_arg, &thread_attr);
            PRINTK("g_threadid_1 = %d\r\n", g_threadid_1);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            ret_unlock = osKernelUnlock();
            PRINTK("ret_unlock = %d\r\n", ret_unlock);
            check_test_result(ret_unlock, KERNEL_LOCKED, TD_TRUE, &test_result);

            PRINTK("main g_test_index = %d\r\n", g_test_index);
            check_test_result(g_test_index, PARAM_3, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1:
            status = osThreadYield();
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osError, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_threadsuspend(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    osStatus_t status;
    osThreadState_t state;
    g_test_index = 0;

    thread_attr.name = "test_thread_suspend";
    thread_attr.cb_size = PARAM_100;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_500); // 5s
            state = osThreadGetState(g_threadid);
            check_test_result(state, osThreadBlocked, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 创建后挂起
        case NORMAL_TEST_2: // 释放后挂起
        case ABNORMAL_TEST: // 终止后挂起
            if (param == NORMAL_TEST_1) {
                g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
                osDelay(SLEEP_10);
                test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
                if (g_threadid == NULL) {
                    test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            } else if (param == NORMAL_TEST_2) {
                g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
                osDelay(SLEEP_10);
                test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
                if (g_threadid == NULL) {
                    test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                    test_result = TEST_SUITE_TEST_FAILED;
                }
                osal_msleep(SLEEP_5000);
                status = osThreadResume(g_threadid);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            } else if (param == ABNORMAL_TEST) {
                g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
                osDelay(SLEEP_10);
                test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
                if (g_threadid == NULL) {
                    test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                    test_result = TEST_SUITE_TEST_FAILED;
                }
                status = osThreadTerminate(g_threadid);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }

            status = osThreadSuspend(g_threadid);
            if (param == ABNORMAL_TEST) {
                check_test_result(status, osErrorResource, TD_TRUE, &test_result);
            } else {
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            break;
        case NORMAL_TEST_3:
            status = osThreadSuspend(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1:
            status = osThreadSuspend(&threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osThreadSuspend(TD_NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_threadresume(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    osStatus_t status;
    g_task_index = 0;
    switch (param) {
        case NORMAL_TEST:
            status = osThreadResume(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 任务执行中只释放
            g_threadid = osThreadNew(thread_rsmbody, &g_thread_arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_2: // 任务执行中只释放
            g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osThreadResume(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osThreadResume(&threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osThreadResume(TD_NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void thread_terminate_body(void *argument)
{
    uint32_t *param = (uint32_t *)argument;
    osThreadId_t get_id;
    osThreadState_t state;
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;
    osDelay(SLEEP_10);
    if (param != NULL) {
        test_suite_log_stringf("argument = %d\r\n", *param);
    }
    while (1) {
        get_id = osThreadGetId();
        test_suite_log_stringf("get_id = %d\r\n", get_id);
        if (get_id == g_threadid) {
            g_test_index++;
            test_suite_log_stringf("os ThreadGetId get_id = %d,g_test_index = %d\r\n", get_id, g_test_index);
        } else {
            test_suite_log_stringf("first run get_id = %d\r\n", get_id);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }
        state = osThreadGetState(g_threadid);
        check_test_result(state, osThreadRunning, TD_TRUE, &test_result);
        g_test_index = 0;
        test_suite_log_stringf("thread_terminate_body success! \r\n");
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, &test_result);
    }
    return;
}

int cts_os_threadterminate(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osThreadState_t  state;
    osStatus_t status = osOK;
    LosTaskCB threadid;
    threadid.taskId = PARAM_100;
    g_test_index = 0;
    thread_attr.name = "test_thread_terminate";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_threadid = osThreadNew(thread_terminate_body, &g_thread_arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_300); // 3s
            state = osThreadGetState(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(state, osThreadInactive, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 挂起后终止
        case NORMAL_TEST_2: // 释放后终止
            g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_100);
            if (param == NORMAL_TEST_2) {
                // 释放
                status = osThreadResume(g_threadid);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            status = osThreadTerminate(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osThreadTerminate(g_threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1:
            status = osThreadTerminate(&threadid);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osThreadTerminate(TD_NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

void testgetcountfunc(void *argument)
{
    uint32_t *param = (uint32_t *)argument;
    osThreadId_t get_id;
    uint32_t count;
    uint32_t test_result = TEST_SUITE_OK;
    if (param != NULL) {
        test_suite_log_stringf("argument = %d\r\n", *param);
    }
    osDelay(SLEEP_100); // 1S

    while (1) {
        get_id = osThreadGetId();
        test_suite_log_stringf("GetId = %d\r\n", get_id);
        if (get_id == g_threadid) {
            g_test_index++;
            test_suite_log_stringf("os ThreadGetId get_id = %d,g_test_index = %d\r\n", get_id, g_test_index);
        } else {
            test_suite_log_stringf("first run get_id = %d\r\n", get_id);
            test_result = TEST_SUITE_TEST_FAILED;
            break;
        }
        count = osThreadGetCount();
        test_suite_log_stringf("os Thread Get count = %d\r\n", count);
        check_test_result(count, (g_count + 1), TD_TRUE, &test_result);
        if (g_test_index >= 1 && g_threadid == get_id) {
            g_test_index = 0;
            break;
        }
        osDelay(SLEEP_100); // 1S
    }
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("TestGetCountFunc success!!! \r\n");
    } else {
        test_suite_log_stringf("TestGetCountFunc fail ,Please check!!! \r\n");
    }
    return;
}

void threadgetcount_normaltest(uint32_t *test_result)
{
    g_count = osThreadGetCount();
    test_suite_log_stringf("os Thread Get count = %d\r\n", g_count);

    g_threadid = osThreadNew(testgetcountfunc, &g_thread_arg, &thread_attr);
    osDelay(SLEEP_10);
    test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
    if (g_threadid == NULL) {
        test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
}

void threadgetcount_abnormaltest(uint32_t param, osStatus_t status, uint32_t pre_count, uint32_t aft_count,
                                 uint32_t *test_result)
{
    pre_count = osThreadGetCount();
    test_suite_log_stringf("os Thread Get pre_count = %d\r\n", pre_count);
    if (param == NORMAL_TEST) {
        g_threadid = osThreadNew(thread_cntbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_1) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_2) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
    } else if (param == NORMAL_TEST_3) {
        g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        osDelay(SLEEP_5000);
        status = osThreadResume(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else if (param == ABNORMAL_TEST) {
        g_threadid = osThreadNew(threadfunc, &g_thread_arg, &thread_attr);
        osDelay(SLEEP_10);
        test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
        if (g_threadid == NULL) {
            test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
            *test_result = TEST_SUITE_TEST_FAILED;
        }
        osal_msleep(SLEEP_10000);
    }
    osDelay(SLEEP_1000);
    aft_count = osThreadGetCount();
    test_suite_log_stringf("os Thread Get aft_count = %d\r\n", aft_count);
    if (param != ABNORMAL_TEST) {
        check_test_result(aft_count, (pre_count+1), TD_TRUE, *&test_result);
        status = osThreadTerminate(g_threadid);
        check_test_result(status, osOK, TD_TRUE, *&test_result);
    } else {
        check_test_result(aft_count, pre_count, TD_TRUE, *&test_result);
    }
}

void threadgetcount_abnormaltest1(osStatus_t status, uint32_t pre_count, uint32_t aft_count, uint32_t *test_result)
{
    g_threadid = osThreadNew(thread_susbody, &g_thread_arg, &thread_attr);
    osDelay(SLEEP_10);
    test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
    if (g_threadid == NULL) {
        test_suite_log_stringf("osThreadNew Fail,g_threadid = %d\r\n", g_threadid);
        *test_result = TEST_SUITE_TEST_FAILED;
    }
    pre_count = osThreadGetCount();
    test_suite_log_stringf("os Thread Get pre_count = %d\r\n", pre_count);
    pre_count = pre_count - 1;
    status = osThreadTerminate(g_threadid);
    check_test_result(status, osOK, TD_TRUE, *&test_result);
    aft_count = osThreadGetCount();
    test_suite_log_stringf("os Thread Get aft_count = %d\r\n", aft_count);
    check_test_result(aft_count, pre_count, TD_TRUE, *&test_result);
}

int cts_os_threadgetcount(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t pre_count = 0;
    uint32_t aft_count = 0;
    g_count = 0;
    osStatus_t status = osOK;
    thread_attr.name = "test_thread_get_count";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityAboveNormal;
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            threadgetcount_normaltest(&test_result);
            break;
        case NORMAL_TEST_1: // 任务体外获取
        case NORMAL_TEST_2: // 挂起后获取
        case NORMAL_TEST_3: // 释放后获取
        case ABNORMAL_TEST: // 任务执行完后获取
            threadgetcount_abnormaltest(param, status, pre_count, aft_count, &test_result);
            break;
        case ABNORMAL_TEST_1: // 终止后获取
            threadgetcount_abnormaltest1(status, pre_count, aft_count, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_delay(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    uint32_t tick1;
    uint32_t tick2;
    uint32_t delay_tick = strtol(argv[1], NULL, 0);
    osStatus_t status;
    switch (param) {
        case NORMAL_TEST:
            tick1 = osKernelGetTickCount() + delay_tick;
            status = osDelay(delay_tick);
            tick2 = osKernelGetTickCount();
            test_suite_log_stringf("tick1 = %d,status = %d,tick2 = %d\r\n", tick1, status, tick2);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            check_test_result(tick1, tick2, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 不同时钟源
            tick1 = uapi_tcxo_get_us() / PARAM_10000 + delay_tick;
            status = osDelay(delay_tick);
            tick2 = uapi_tcxo_get_us() / PARAM_10000;
            test_suite_log_stringf("tick1 = %d,status = %d,tick2 = %d\r\n", tick1, status, tick2);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            if (tick1 - tick2 > delay_tick) {
                test_result = TEST_SUITE_TEST_FAILED;
                test_suite_log_stringf("osDelay fail\r\n");
            }
            break;
        case ABNORMAL_TEST:
            status = osDelay(0);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_delayuntil(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t tick1;
    uint32_t tick2;
    uint32_t delay_tick = strtol(argv[1], NULL, 0);
    osStatus_t status;
    switch (param) {
        case NORMAL_TEST:
            tick1 = osKernelGetTickCount() + delay_tick;
            status = osDelayUntil(tick1);
            tick2 = osKernelGetTickCount();
            test_suite_log_stringf("tick1 = %d,status = %d,tick2 = %d\r\n", tick1, status, tick2);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            check_test_result(tick2, tick1, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 不同时钟源
            tick1 = uapi_tcxo_get_us() / PARAM_1000 + delay_tick;
            status = osDelayUntil(tick1);
            tick2 = uapi_tcxo_get_us() / PARAM_1000;
            test_suite_log_stringf("tick1 = %d,status = %d,tick2 = %d\r\n", tick1, status, tick2);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osDelayUntil(0);
            test_suite_log_stringf("status = %d\r\n", status);
            if (status != osError) {
                test_suite_log_stringf("status = \r\n", status);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void timer1_callback(void *arg)
{
    uint32_t param = *(uint32_t*)arg;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t timer_delay;
    test_suite_log_stringf("timer1_callback in ,param = %d\r\n", param);

    g_timer_arrive = (uint32_t)LOS_TickCountGet();
    timer_delay = g_timer_arrive - g_timer_start;
    timer_count ++;
    test_suite_log_stringf("g_timer_arrive = %d tick ,timer_delay = %d tick,timer_count = %d \r\n", g_timer_arrive,
                           timer_delay, timer_count);

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("timer1_callback success\r\n");
    } else {
        test_suite_log_stringf("timer1_callback fail\r\n");
    }
    return;
}

void timer2_callback(void *arg)
{
    unused(arg);
    uint32_t ret;
    uint32_t test_result = TEST_SUITE_OK;
    // uint32_t timer_delay;
    g_timer_arrive = (uint32_t)LOS_TickCountGet();

    timer_count ++;

    ret = osTimerIsRunning(g_timer_id);
    check_test_result(ret, 1, TD_TRUE, &test_result); // 1:runnig 0:not run

    if (timer_count >= PARAM_5) {
        osTimerStop(g_timer_id);
        osTimerDelete(g_timer_id);
    }
    g_timer_start = g_timer_arrive; // 周期定时器刷新时间用
    return;
}

void timer4_callback(void *data)
{
    unused(data);
    g_timer_arrive = (uint32_t)LOS_TickCountGet();
    test_suite_log_stringf("g_timer_arrive = %d tick\r\n", g_timer_arrive);
    uint32_t timer_delay = 0;
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;

    timer_delay = g_timer_arrive - g_timer_start;
    test_suite_log_stringf("g_timer_arrive = %d tick ,timer_delay = %d tick,timer_count = %d \r\n", g_timer_arrive,
                           timer_delay, timer_count);
    check_test_result(timer_out, timer_delay, TD_TRUE, &test_result);

    timer_count++;
    g_timer_start = (uint32_t)LOS_TickCountGet();
    status = osTimerStart(g_timer_id1, timer_out / PARAM_5);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    if (timer_count >= PARAM_5) {
        status = osTimerDelete(g_timer_id);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        status = osTimerDelete(g_timer_id1);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        if (test_result == (uint32_t)TEST_SUITE_TEST_FAILED) {
            test_suite_log_stringf("timer4_callback fail\r\n");
        } else {
            test_suite_log_stringf("timer4_callback success\r\n");
        }
    }
}

void timer5_callback(void *arg)
{
    uint32_t param = *(uint32_t*)arg;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t timer_delay;
    test_suite_log_stringf("timer5_callback in ,param = %d\r\n", param);

    g_timer_arrive = (uint32_t)LOS_TickCountGet();
    timer_delay = g_timer_arrive - g_timer_start;
    test_suite_log_stringf("g_timer_arrive = %d tick ,timer_delay = %d tick,timer_count = %d \r\n", g_timer_arrive,
                           timer_delay, timer_count);
    check_test_result(timer_out / 5, timer_delay, TD_TRUE, &test_result);

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("timer5_callback success\r\n");
    } else {
        test_suite_log_stringf("timer5_callback fail\r\n");
    }
    return;
}

int cts_os_timernew(int argc, char *argv[])
{
    if (argc != PARAM_4) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osTimerId_t timer_id[PARAM_5] = {0};
    ext_mdm_mem_info mem_info = {0};
    uint32_t mem_start = 0;
    uint32_t mem_end = 0;
    osStatus_t status;
    uint32_t arg = PARAM_5;
    time_attr.name = argv[1];
    time_attr.attr_bits = strtol(argv[PARAM_2], NULL, 0);
    time_attr.cb_size = strtol(argv[PARAM_3], NULL, 0);
    time_attr.cb_mem = kv_malloc(time_attr.cb_size);
    if (time_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    timer_count = 0;
    timer_out = PARAM_100; // 1s
    test_suite_log_stringf("arg = %d,name = %s,attr_bits = %d,cb_size = %d\r\n", arg, time_attr.name,
                           time_attr.attr_bits, time_attr.cb_size);
    switch (param) {
        case NORMAL_TEST: // 一次性
            g_timer_id = osTimerNew(timer1_callback, osTimerOnce, &arg, &time_attr);
            test_suite_log_stringf("timer_id = %d\r\n", g_timer_id);
            if (g_timer_id == NULL) {
                test_suite_log_stringf("Create one-shoot timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1:  // 周期性
            g_timer_id = osTimerNew(timer2_callback, osTimerPeriodic, &arg, &time_attr);
            test_suite_log_stringf("timer_id = %d\r\n", g_timer_id);
            if (g_timer_id == NULL) {
                test_suite_log_stringf("Create periodic t timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_2: // 启动后创建
        case NORMAL_TEST_3: // 停止后创建
            g_timer_start = LOS_TickCountGet();
            timer_id[0] = osTimerNew(timer1_callback, osTimerOnce, &arg, &time_attr);
            test_suite_log_stringf("timer_id[0] = %d\r\n", timer_id[0]);
            if (timer_id[0] == NULL) {
                test_suite_log_stringf("Create periodic t timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            if (param == NORMAL_TEST_2) {
                g_timer_start_u64 = LOS_TickCountGet();
                status = osTimerStart(timer_id[0], timer_out);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            } else if (param == NORMAL_TEST_3) {
                status = osTimerStart(timer_id[0], timer_out);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                status = osTimerStop(timer_id[0]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            timer_id[1] = osTimerNew(timer1_callback, osTimerOnce, &arg, &time_attr);
            test_suite_log_stringf("timer_id[1] = %d\r\n", timer_id[1]);
            if (timer_id[1] == NULL) {
                test_suite_log_stringf("Create periodic t timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osTimerDelete(timer_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osTimerDelete(timer_id[1]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST: // 反复创建、启动、停止、删除
            mem_start = mem_info.free;
            test_suite_log_stringf("mem_start = %d\r\n", mem_start);

            for (uint32_t i = 0; i < g_press_test_cnt; i++) {
                timer_id[0] = osTimerNew(timer1_callback, osTimerOnce, &arg, &time_attr);
                if (timer_id[0] == NULL) {
                    test_suite_log_stringf("Create periodic t timer Fail\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
                status = osTimerStart(timer_id[0], timer_out);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osTimerStop(timer_id[0]);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osTimerDelete(timer_id[0]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            mem_end = mem_info.free;
            test_suite_log_stringf("mem_end = %d\r\n", mem_end);
            check_test_result(mem_end, mem_start, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            g_timer_id = osTimerNew(timer1_callback, PARAM_2, &arg, &time_attr);
            test_suite_log_stringf("timer_id = %d\r\n", g_timer_id);
            if (g_timer_id != NULL) {
                test_suite_log_stringf("should not Create timer success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            timer_id[0] = osTimerNew(timer1_callback, osTimerOnce, TD_NULL, &time_attr);
            test_suite_log_stringf("timer_id1 = %d\r\n", timer_id[0]);
            if (timer_id[0] == NULL) {
                test_suite_log_stringf("Create one-shoot timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            timer_id[1] = osTimerNew(timer1_callback, osTimerOnce, &arg, TD_NULL);
            test_suite_log_stringf("timer_id2 = %d\r\n", timer_id[1]);
            if (timer_id[1] == NULL) {
                test_suite_log_stringf("Create one-shoot timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            time_attr.name =NULL;
            timer_id[PARAM_2] = osTimerNew(timer1_callback, osTimerPeriodic, &arg, &time_attr);
            test_suite_log_stringf("timer_id3 = %d\r\n", timer_id[PARAM_2]);
            if (timer_id[PARAM_2] == NULL) {
                test_suite_log_stringf("should not Create timer success\\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            status = osTimerDelete(timer_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osTimerDelete(timer_id[1]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osTimerDelete(timer_id[PARAM_2]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST_1:
            g_timer_id = osTimerNew(NULL, osTimerOnce, &arg, &time_attr);
            test_suite_log_stringf("timer_id3 = %d\r\n", g_timer_id);
            if (g_timer_id != NULL) {
                test_suite_log_stringf("should not Create timer success\\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            g_timer_id = osTimerNew(NULL, osTimerPeriodic, &arg, &time_attr);
            test_suite_log_stringf("timer_id4 = %d\r\n", g_timer_id);
            if (g_timer_id != NULL) {
                test_suite_log_stringf("should not Create timer success\\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(time_attr.cb_mem);
    return test_result;
}

int cts_os_timerstart(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t ret = TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;

    timer_out = strtol(argv[1], NULL, 0);
    uint32_t timer_id = PARAM_10;
    timer_count = 0;
    g_timer_start = 0;
    g_timer_arrive = 0;
    test_suite_log_stringf("timer_out = %d\r\n", timer_out);
    osStatus_t status;
    switch (param) {
        case NORMAL_TEST:
            status = osTimerStart(g_timer_id, timer_out);
            g_timer_start = LOS_TickCountGet();
            test_suite_log_stringf("g_timer_id = %d, Timer Start status = %d,g_timer_start= %d tick\r\n", g_timer_id,
                                   status, g_timer_start);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 启动过程中再启动
        case NORMAL_TEST_2: // 启动过程中停止再启动
            status = osTimerStart(g_timer_id, timer_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            ret = osal_msleep(timer_out * PARAM_10 / PARAM_2);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);

            if (param == NORMAL_TEST_2) {
                osTimerStop(g_timer_id);
            }
            g_timer_start = LOS_TickCountGet();
            status = osTimerStart(g_timer_id, timer_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_3: // 在周期定时器中启动一次性定时器
            g_timer_id = osTimerNew(timer4_callback, osTimerPeriodic, &timer_out, &time_attr);
            test_suite_log_stringf("timer_id = %d\r\n", g_timer_id);
            if (g_timer_id == NULL) {
                test_suite_log_stringf("Create periodic timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_timer_id1 = osTimerNew(timer5_callback, osTimerOnce, &timer_out, &time_attr);
            test_suite_log_stringf("timer_id1 = %d\r\n", g_timer_id1);
            if (g_timer_id1 == NULL) {
                test_suite_log_stringf("Create once timer Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_timer_start = LOS_TickCountGet();
            status = osTimerStart(g_timer_id, timer_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            break;
        case ABNORMAL_TEST:
            timer_out = 0;
            status = osTimerStart(g_timer_id, timer_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);

            // 不存在的id
            timer_out = PARAM_100000;
            status = osTimerStart(&timer_id, timer_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osTimerStart(TD_NULL, timer_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_timerstop(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    osStatus_t status;
    uint32_t timer_id = PARAM_10;
    switch (param) {
        case NORMAL_TEST:
            status = osTimerStop(g_timer_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osTimerStop(&timer_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osTimerStop(TD_NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_timerisrunning(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret;
    uint32_t timer_id = PARAM_10;
    switch (param) {
        case NORMAL_TEST:
            ret = osTimerIsRunning(g_timer_id);
            test_suite_log_stringf("g_timer_id = %d, ret = %d\r\n", g_timer_id, ret);
            if (ret == 1) {
            test_suite_log_stringf("is running\r\n");
            } else {
                test_suite_log_stringf("not running\r\n");
            }
            break;
        case ABNORMAL_TEST:
            ret = osTimerIsRunning(&timer_id);
            test_suite_log_stringf("timer_id = %d, ret = %d\r\n", timer_id, ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osTimerIsRunning(TD_NULL);
            test_suite_log_stringf("ret = %d\r\n", ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_timerdelete(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t status;
    LosSwtmrCB tmp;
    tmp.timerId = OS_SWTMR_MAX_TIMERID + 1;
    switch (param) {
        case NORMAL_TEST:
            status = osTimerDelete(g_timer_id);
            test_suite_log_stringf("g_timer_id = %d,status = %d\r\n", g_timer_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osTimerDelete(&tmp);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osTimerDelete(TD_NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

td_void cmsis_muxbody(td_void *param)
{
    ext_errno ret = (ext_errno)TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *args = (uint32_t*)param;
    test_suite_log_stringf("args = %d\r\n", *args);

    while (1) {
        test_index++;
        if (test_index == 1) {
            ret = LOS_MuxCreate(&g_mux_handle);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
        }
        test_suite_log_stringf("test_index = %d, g_mux_handle = %d\r\n", test_index, g_mux_handle);
        ret = LOS_MuxPend(g_mux_handle, PARAM_100);
        check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
        ret = LOS_MuxPost(g_mux_handle);
        check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
        if (test_index >= PARAM_10) {
            test_index = 0;
            ret = LOS_MuxDelete(g_mux_handle);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            if (test_result == TEST_SUITE_OK) {
                test_suite_log_stringf("task_muxbody success\r\n");
            } else {
                test_suite_log_stringf("task_muxbody fail\r\n");
            }
            break;
        }
    }
}

td_void cmsis_mux_pendbody(td_void *param)
{
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *timeout = (uint32_t*)param;
    test_suite_log_stringf("timeout = %d\r\n", *timeout);
    uint32_t tick1 = 0;
    uint32_t tick2 = 0;
    uint32_t wait_time = 0;

    tick1 = LOS_TickCountGet();
    status = osMutexAcquire(g_mutex_id, *timeout);
    tick2 = LOS_TickCountGet();
    wait_time = tick2 - tick1;
    check_test_result(status, osErrorTimeout, TD_TRUE, &test_result);
    test_suite_log_stringf("tick1 = %d, tick2 = %d, wait_time = %d \r\n", tick1, tick2, wait_time);
    check_test_result(*timeout, wait_time, TD_TRUE, &test_result);
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("mux_pendbody time_out success\r\n");
    } else {
        test_suite_log_stringf("mux_pendbody fail\r\n");
    }
}

td_void cmsis_mux_pend_postbody(td_void *param)
{
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *args = (uint32_t*)param;
    test_suite_log_stringf("args = %d\r\n", *args);

    status = osMutexAcquire(g_mutex_id, TD_SYS_WAIT_FOREVER);
    check_test_result(status, osOK, TD_TRUE, &test_result);

    status = osMutexRelease(g_mutex_id);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("mux_pend_postbody success\r\n");
    } else {
        test_suite_log_stringf("mux_pend_postbody fail\r\n");
    }
}

void cmsis_muxfunctest1(void *argument)
{
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *param = (uint32_t *)argument;
    if (param != NULL) {
        test_suite_log_stringf("argument = %d\r\n", *param);
    }

    test_suite_log_stringf("task1 try to get mutex, wait 10 Tick.\n");
    /* 申请互斥锁 */
    status = osMutexAcquire((osMutexId_t)param, PARAM_10);
    test_suite_log_stringf("g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
    if (status == osOK) {
        /* 释放互斥锁 */
        status = osMutexRelease((osMutexId_t)param);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        return;
    } else if (status == osErrorTimeout) {
        test_suite_log_stringf("task1 timeout and try to get mutex, wait forever.\n");
        /* 申请互斥锁 */
        status = osMutexAcquire((osMutexId_t)param, LOS_WAIT_FOREVER);
        if (status == osOK) {
            test_suite_log_stringf("task1 wait forever,get mutex g_Testmux01.\n");
            /* 释放互斥锁 */
            status = osMutexRelease((osMutexId_t)param);
            check_test_result(status, osOK, TD_TRUE, &test_result);
        }
    }
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("cmsis_muxfunctest1 success\r\n");
    } else {
        test_suite_log_stringf("cmsis_muxfunctest1 fail\r\n");
    }
    return;
}

void cmsis_muxfunctest2(void *argument)
{
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t *param = (uint32_t *)argument;
    if (param != NULL) {
        test_suite_log_stringf("argument = %d\r\n", *param);
    }

    test_suite_log_stringf("task2 try to get mutex, wait forever.\n");
    /* 申请互斥锁 */
    status = osMutexAcquire((osMutexId_t)param, LOS_WAIT_FOREVER);
    test_suite_log_stringf("task2 get mutex g_Testmux01 and suspend 100 Tick.\n");
    check_test_result(status, osOK, TD_TRUE, &test_result);
    /* 任务休眠100 Tick */
    LOS_TaskDelay(PARAM_100);
    test_suite_log_stringf("task2 resumed and post the g_Testmux01\n");
    /* 释放互斥锁 */
    status = osMutexRelease((osMutexId_t)param);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("cmsis_muxfunctest2 success\r\n");
    } else {
        test_suite_log_stringf("cmsis_muxfunctest2 fail\r\n");
    }
    return;
}

int cts_os_mutexnew(int argc, char *argv[])
{
    if (argc != PARAM_4) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret = 0;
    osStatus_t status;
    osMutexId_t mux_handle[LOSCFG_BASE_IPC_MUX_LIMIT + 1] = {0};
    uint32_t create_cnt;
    ext_os_resource_use_stat os_resource_use = {0};
    uint32_t timeout = PARAM_100;
    ext_mdm_mem_info mem_inf = {0};
    ext_mdm_mem_info mem_inf1 = {0};
    osThreadId_t thread_id;
    osThreadAttr_t mutex_attr = {0};
    mutex_attr.priority = PRODUCT_CFG_TASK_PRIO_APP_CTS;
    mutex_attr.stack_size = PRODUCT_CFG_TASK_STACK_APP_CTS;
    mutex_attr.name = "mux_task";
    g_mutex_attr.name = argv[1];
    g_mutex_attr.attr_bits  = strtol(argv[PARAM_2], NULL, 0);
    g_mutex_attr.cb_size = strtol(argv[PARAM_3], NULL, 0);
    g_mutex_attr.cb_mem = kv_malloc(g_mutex_attr.cb_size);
    if (g_mutex_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_mutex_id = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("MutexId = %d\r\n", g_mutex_id);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1: // 等待超时前创建，超时后创建, 释放后创建
            g_mutex_id = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("g_mutex_id = %d\r\n", g_mutex_id);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMutexAcquire(g_mutex_id, timeout);
            test_suite_log_stringf("g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            osDelay(timeout / PARAM_2);
            // 等待超时前
            mux_handle[0] = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("mux_handle[0]= %d\r\n", mux_handle[0]);
            if (mux_handle[0] == NULL) {
                test_suite_log_stringf("os MutexNew Fail,mux_handle[0] = %d\r\n", mux_handle[0]);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(timeout / PARAM_2);
            // 等待超时后
            mux_handle[1] = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("mux_handle[1]= %d\r\n", mux_handle[1]);
            if (mux_handle[1] == NULL) {
                test_suite_log_stringf("os MutexNew Fail,mux_handle[1] = %d\r\n", mux_handle[1]);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 释放后创建
            status = osMutexRelease(g_mutex_id);
            test_suite_log_stringf("g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            mux_handle[PARAM_2] = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("mux_handle[2]= %d\r\n", mux_handle[PARAM_2]);
            if (mux_handle[PARAM_2] == NULL) {
                test_suite_log_stringf("os MutexNew Fail,mux_handle[2] = %d\r\n", mux_handle[PARAM_2]);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("g_mutex_id=%d,mux_handle[0]=%d,mux_handle[1]=%d,mux_handle[2]=%d\r\n", g_mutex_id,
                                   mux_handle[0], mux_handle[1], mux_handle[PARAM_2]);
            status = osMutexDelete(g_mutex_id);
            test_suite_log_stringf("g_mutex_id delete, status = %d\r\n", status);
            status += osMutexDelete(mux_handle[0]);
            test_suite_log_stringf("mux_handle[0] delete, status = %d\r\n", status);
            status += osMutexDelete(mux_handle[1]);
            test_suite_log_stringf("mux_handle[1] delete, status = %d\r\n", status);
            status += osMutexDelete(mux_handle[PARAM_2]);
            test_suite_log_stringf("mux_handle[2] delete, status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2:
            g_threadid = osThreadNew(cmsis_muxbody, &timeout, &mutex_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == 0) {
                test_suite_log_stringf("osThreadNew not success,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            g_mutex_id = osMutexNew(NULL);
            test_suite_log_stringf("MutexId = %d\r\n", g_mutex_id);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case PRESSURE_TEST:
            test_suite_log_stringf("before test, mem_used = %d, mem_free = %d\r\n", mem_inf.used, mem_inf.free);

            for (uint32_t i = 0; i < g_press_test_cnt; i++) {
                g_mutex_id = osMutexNew(&g_mutex_attr);
                if (g_mutex_id == NULL) {
                    test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                    test_result = TEST_SUITE_TEST_FAILED;
                    break;
                }
                status = osMutexAcquire(g_mutex_id, timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                thread_id = osMutexGetOwner(g_mutex_id);
                if (thread_id == NULL) {
                    test_suite_log_stringf("os MutexGetOwner Fail,thread_id = %d\r\n", thread_id);
                    test_result = TEST_SUITE_TEST_FAILED;
                    break;
                }
                status = osMutexRelease(g_mutex_id);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                status = osMutexDelete(g_mutex_id);
                check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            }

            check_test_result(mem_inf1.used, mem_inf.used, TD_TRUE, &test_result);
            check_test_result(mem_inf1.free, mem_inf.free, TD_TRUE, &test_result);
            test_suite_log_stringf("after test, mem_used = %d, mem_free = %d\r\n", mem_inf1.used, mem_inf1.free);
            break;
        case PRESSURE_TEST_1: // 创建最大个数
            test_suite_log_stringf("current use mux:%d, LOSCFG_BASE_IPC_MUX_LIMIT = %d\r\n", os_resource_use.mux_usage,
                                   LOSCFG_BASE_IPC_MUX_LIMIT);
            for (create_cnt = os_resource_use.mux_usage; create_cnt < LOSCFG_BASE_IPC_MUX_LIMIT; create_cnt++) {
                mux_handle[create_cnt] = osMutexNew(NULL);
                test_suite_log_stringf("create_cnt = %d, mux_handle = %d\r\n", create_cnt, mux_handle[create_cnt]);
                check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
                timeout = timeout + PARAM_10;
                status = osMutexAcquire(mux_handle[create_cnt], timeout);
                if (status != osOK) {
                    break;
                }
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            if (test_result == TEST_SUITE_OK) {
                // 继续create应该返回失败
                test_suite_log_stringf("after mux create max, begin create cnt:%u\r\n", create_cnt);
                mux_handle[create_cnt] = osMutexNew(NULL); // 继续创建失败
                check_test_result((uint32_t)mux_handle[create_cnt], 0, TD_TRUE, &test_result);
            }
            create_cnt --;

            while (create_cnt >= os_resource_use.mux_usage) {
                status = osMutexRelease(mux_handle[create_cnt]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                osal_msleep(SLEEP_1000);
                status = osMutexDelete(mux_handle[create_cnt]);
                test_suite_log_stringf("delete_cnt = %d, mux_handle = %d, status = %d\r\n", create_cnt,
                                       mux_handle[create_cnt], status);
                if (create_cnt == 0) {
                    break;
                }
                check_test_result(status, osOK, TD_TRUE, &test_result);
                create_cnt --;
            }
            break;
        case PRESSURE_TEST_2: // 创建最大个数
            test_suite_log_stringf("current use mux:%d, LOSCFG_BASE_IPC_MUX_LIMIT = %d\r\n", os_resource_use.mux_usage,
                                   LOSCFG_BASE_IPC_MUX_LIMIT);
            for (create_cnt = os_resource_use.mux_usage; create_cnt < LOSCFG_BASE_IPC_MUX_LIMIT; create_cnt++) {
                mux_handle[create_cnt] = osMutexNew(NULL);
                test_suite_log_stringf("create_cnt = %d, mux_handle = %d\r\n", create_cnt, mux_handle[create_cnt]);
                check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);

                timeout = timeout + PARAM_10;
                status = osMutexAcquire(mux_handle[create_cnt], timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(g_mutex_attr.cb_mem);
    return test_result;
}

int cts_os_mutexacquire(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t mutex_id = PARAM_10;
    uint32_t arg = PARAM_10;
    osStatus_t status;
    uint32_t timeout = strtol(argv[1], NULL, 0);
    osThreadAttr_t mutex_attr = {0};
    mutex_attr.priority = PRODUCT_CFG_TASK_PRIO_APP_CTS;
    mutex_attr.stack_size = PRODUCT_CFG_TASK_STACK_APP_CTS;
    mutex_attr.name = "mux_task";

    switch (param) {
        case NORMAL_TEST:
            status = osMutexAcquire(g_mutex_id, timeout);
            test_suite_log_stringf("g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 永久等待,有互斥锁的时候，永久等待会立即返回
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            g_mutex_id = osMutexNew(&g_mutex_attr);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMutexAcquire(g_mutex_id, TD_SYS_WAIT_FOREVER);
            check_test_result(status, TEST_SUITE_OK, TD_TRUE, &test_result);

            status = osMutexRelease(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osMutexDelete(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 永久等待，等待过程中释放
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            g_mutex_id = osMutexNew(&g_mutex_attr);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMutexAcquire(g_mutex_id, TD_SYS_WAIT_FOREVER);
            test_suite_log_stringf("g_mutex_id = %d Acquire success\r\n", g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            g_threadid = osThreadNew(cmsis_mux_pend_postbody, &arg, &mutex_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew should not success,g_threadid  =	%d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_100);
            status = osMutexRelease(g_mutex_id);
            test_suite_log_stringf("g_mutex_id = %d release success\r\n", g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            osDelay(SLEEP_50);
            status = osMutexDelete(g_mutex_id);
            test_suite_log_stringf("g_mutex_id = %d delete success\r\n", g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST: // 等待超时
            test_suite_log_stringf("time_out test\r\n");
            g_mutex_id = osMutexNew(&g_mutex_attr);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMutexAcquire(g_mutex_id, TD_SYS_WAIT_FOREVER);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            g_threadid = osThreadNew(cmsis_mux_pendbody, &timeout, &mutex_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew should not success,g_threadid  =	%d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(SLEEP_100);
            status = osMutexRelease(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            osDelay(SLEEP_50);
            status = osMutexDelete(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1:
            status = osMutexAcquire(&mutex_id, timeout);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osMutexAcquire(NULL, timeout);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_mutexrelease(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t ret;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t mutex_id = PARAM_10;
    osStatus_t status;
    switch (param) {
        case NORMAL_TEST:
            status = osMutexRelease(g_mutex_id);
            test_suite_log_stringf("g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osMutexRelease(&mutex_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osMutexRelease(NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case MULTI_TASK_TEST: // 多任务竞争锁
            muxthread_attr[0].priority = TEST_SUITE_TASK_PRIO - 1;
            muxthread_attr[0].name = "muxTask1";
            muxthread_attr[1].priority = TEST_SUITE_TASK_PRIO + PARAM_2;
            muxthread_attr[1].name = "muxTask2";
            g_mutex_id = osMutexNew(&g_mutex_attr);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("create mux_id = %d\r\n", g_mutex_id);

            ret = osKernelLock();
            check_test_result(ret, osOK, TD_TRUE, &test_result);

            g_threadid = osThreadNew(cmsis_muxfunctest1, &g_mutex_id, &muxthread_attr[0]);
            if (g_threadid == NULL) {
                test_suite_log_stringf("os ThreadNew Fail,g_threadid = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_threadid_1 = osThreadNew(cmsis_muxfunctest2, &g_mutex_id, &muxthread_attr[1]);
            if (g_threadid == NULL) {
                test_suite_log_stringf("os ThreadNew Fail,g_threadid_1 = %d\r\n", g_threadid_1);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            ret = osKernelUnlock();
            check_test_result(ret, KERNEL_LOCKED, TD_TRUE, &test_result);

            osDelay(SLEEP_1000);
            status = osMutexDelete(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_mutexgetowner(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    uint32_t mutex_id = 0;
    osThreadId_t threadid = NULL;

    switch (param) {
        case NORMAL_TEST:
            test_suite_log_stringf("g_mutex_id = %d\r\n", g_mutex_id);
            threadid = osMutexGetOwner(g_mutex_id);
            test_suite_log_stringf("threadid = %d\r\n", threadid);
            if (threadid == NULL) {
                test_suite_log_stringf("os MutexGetOwner Fail,threadid = %d\r\n", threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case ABNORMAL_TEST:
            threadid = osMutexGetOwner(&mutex_id);
            test_suite_log_stringf("threadid = %d\r\n", threadid);
            if (threadid != NULL) {
                test_suite_log_stringf("os MutexGetOwner Fail,threadid = %d\r\n", threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            threadid = osMutexGetOwner(NULL);
            test_suite_log_stringf("threadid = %d\r\n", threadid);
            if (threadid != NULL) {
                test_suite_log_stringf("os MutexGetOwner Fail,threadid = %d\r\n", threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_mutexdelete(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t timeout = PARAM_100;

    uint32_t mutex_id = PARAM_10;
    osStatus_t status;

    switch (param) {
        case NORMAL_TEST:
            test_suite_log_stringf("g_mutex_id = %d\r\n", g_mutex_id);
            status = osMutexDelete(g_mutex_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osMutexDelete(&mutex_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1: // 等待超时前删除, 等待超时后删除, 释放后删除
            g_mutex_id = osMutexNew(&g_mutex_attr);
            test_suite_log_stringf("g_mutex_id = %d\r\n", g_mutex_id);
            if (g_mutex_id == NULL) {
                test_suite_log_stringf("os MutexNew Fail,g_mutex_id = %d\r\n", g_mutex_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMutexAcquire(g_mutex_id, timeout);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            g_threadid = osThreadNew(cmsis_mux_pendbody, &timeout, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew should not success,g_threadid  =%d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osDelay(timeout / PARAM_2);
            // 等待超时前删除
            status = osMutexDelete(g_mutex_id);
            test_suite_log_stringf("osMutexDelete: g_mutex_id = %d, status = %d\r\n", g_mutex_id, status);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);
            osDelay(timeout / PARAM_2);
            // 等待超时后删除
            status = osMutexDelete(g_mutex_id);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);
            // 释放后删除
            status = osMutexRelease(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osMutexDelete(g_mutex_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osMutexDelete(NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void cmsis_event_wait_forever_body(void *param)
{
    uint32_t flags = *(uint32_t *)param;
    uint32_t ret = TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;
    test_suite_log_stringf("flags = %d,g_evt_id = %d \r\n", flags, g_evt_id);
    test_index++;

    ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAll, TD_SYS_WAIT_FOREVER);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, flags, TD_TRUE, &test_result);

    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("event_wait_forever_body success\r\n");
    } else {
        test_suite_log_stringf("event_wait_forever_body fail\r\n");
    }
}

td_void *cmsis_event_task_send(uint32_t data)
{
    uint32_t ret = TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;

    if (data == PARAM_2) {
        ret = osEventFlagsSet(g_evt_id, g_test_bits);
        test_suite_log_stringf("g_evt_id = %d, ret = 0x%x, g_test_bits = 0x%x\r\n", g_evt_id, ret, g_test_bits);
        check_test_result(ret, g_test_bits, TD_TRUE, &test_result);
    } else {
        ret = osEventFlagsSet(g_evt_id1, g_test_bits1);
        test_suite_log_stringf("g_evt_id1 = %d, ret = 0x%x, g_test_bits1 = 0x%x\r\n", g_evt_id1, ret, g_test_bits1);
        check_test_result(ret, g_test_bits1, TD_TRUE, &test_result);
    }

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_send fail \r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_send success\r\n");
    }
    return (td_void*)data;
}

td_void *cmsis_event_task_send1(td_void* data)
{
    uint32_t ret;
    uint32_t get_flags;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t event_bits = *(uint32_t*)data;
    test_suite_log_stringf("event_bits= %d \r\n", event_bits);

    ret = osEventFlagsSet(g_evt_id, event_bits);
    get_flags = osEventFlagsGet(g_evt_id);
    test_suite_log_stringf("g_evt_id = %d, ret = 0x%x, get_flags = 0x%x\r\n", g_evt_id, ret, get_flags);
    check_test_result(ret, get_flags, TD_TRUE, &test_result);

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_send1 fail \r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_send1 success\r\n");
    }
    return (td_void*)data;
}

td_void *cmsis_event_task_wait(uint32_t data)
{
    uint32_t ret;
    osStatus_t status;
    uint32_t test_result = TEST_SUITE_OK;

    ret = osEventFlagsWait(g_evt_id, g_test_bits, osFlagsWaitAll, TD_SYS_WAIT_FOREVER);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, g_test_bits, TD_TRUE, &test_result);

    ret = osEventFlagsWait(g_evt_id1, g_test_bits1, osFlagsWaitAll, TD_SYS_WAIT_FOREVER);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, g_test_bits1, TD_TRUE, &test_result);

    status = osEventFlagsDelete(g_evt_id);
    test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    status = osEventFlagsDelete(g_evt_id1);
    test_suite_log_stringf("g_evt_id1 = %d, status = %d\r\n", g_evt_id1, status);
    check_test_result(status, osOK, TD_TRUE, &test_result);

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_wait fail\r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_wait success\r\n");
    }
    return (td_void*)data;
}

td_void *cmsis_event_task_wait1(uint32_t data)
{
    uint32_t ret;
    uint32_t time_out = PARAM_5000;
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t mask = g_test_bits + g_test_bits1;
    osStatus_t status;

    ret = osEventFlagsWait(g_evt_id, mask, osFlagsWaitAll, time_out);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, mask, TD_TRUE, &test_result);

    status = osEventFlagsDelete(g_evt_id);
    test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
    check_test_result(status, osOK, TD_TRUE, &test_result);

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_wait1 fail\r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_wait1 success\r\n");
    }
    return (td_void*)data;
}

td_void *cmsis_event_task_wait2(uint32_t data)
{
    uint32_t ret;
    uint32_t time_out = PARAM_5000;
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t status;

    ret = osEventFlagsWait(g_evt_id, g_test_bits, osFlagsWaitAll, time_out);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, g_test_bits, TD_TRUE, &test_result);

    status = osEventFlagsDelete(g_evt_id);
    test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
    check_test_result(status, osOK, TD_TRUE, &test_result);

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_wait2 fail\r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_wait2 success\r\n");
    }
    return (td_void*)data;
}

td_void *cmsis_event_task_wait3(uint32_t data)
{
    uint32_t ret;
    uint32_t time_out = PARAM_5000;
    uint32_t test_result = TEST_SUITE_OK;
    osStatus_t status;

    ret = osEventFlagsWait(g_evt_id1, g_test_bits1, osFlagsWaitAll, time_out);
    test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
    check_test_result(ret, g_test_bits1, TD_TRUE, &test_result);

    status = osEventFlagsDelete(g_evt_id1);
    test_suite_log_stringf("g_evt_id1 = %d, status = %d\r\n", g_evt_id1, status);
    check_test_result(status, osOK, TD_TRUE, &test_result);

    if (test_result != TEST_SUITE_OK) {
        test_suite_log_stringf("ctsevent_task_wait3 fail\r\n");
    } else {
        test_suite_log_stringf("ctsevent_task_wait3 success\r\n");
    }
    return (td_void*)data;
}

int cts_os_eventflagsnew(int argc, char *argv[])
{
    if (argc != PARAM_4) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    g_flags_attr.name = argv[1];
    g_flags_attr.attr_bits  = strtol(argv[PARAM_2], NULL, 0);
    g_flags_attr.cb_size = strtol(argv[PARAM_3], NULL, 0);
    g_flags_attr.cb_mem = kv_malloc(g_flags_attr.cb_size);
    if (g_flags_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_evt_id = osEventFlagsNew(&g_flags_attr);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(g_flags_attr.cb_mem);
    return test_result;
}

int cts_os_eventflagsset(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret = 0;
    uint32_t ret_lock = 0;
    uint32_t ret_unlock = 0;
    uint32_t evt_id = PARAM_10;
    uint32_t get_flags = 0;
    uint32_t flags = (uint32_t)strtoul(argv[1], TD_NULL, PARAM_16);
    test_suite_log_stringf("flags = 0x%x\r\n", flags);
    uint32_t time_out = 10000;
    osThreadId_t thread_id;
    osThreadId_t thread_id1;
    osThreadId_t thread_id2;
    osThreadId_t thread_id3;
    osStatus_t status;

    thread_attr.name = "test_Event";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityBelowNormal; // ;osPriorityAboveNormal osPriorityNormal
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            g_evt_id = osEventFlagsNew(NULL);
            ret = osEventFlagsSet(g_evt_id, flags);
            get_flags = osEventFlagsGet(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, ret = 0x%x, get_flags = 0x%x\r\n", g_evt_id, ret, get_flags);
            check_test_result(ret, get_flags, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 等待过程中发送      ，等待完成后发送; 删除后发送
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_threadid = osThreadNew(cmsis_event_wait_forever_body, &flags, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 等待过程中发送
            ret = osal_msleep(time_out / PARAM_2);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            ret = osEventFlagsSet(g_evt_id, flags);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            // 等待完成后发送
            ret = osal_msleep(time_out / PARAM_2);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            ret = osEventFlagsSet(g_evt_id, flags);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            // 删除后发送 (对空指针操作会死机，删除此场景)
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 中断中发成功
        case NORMAL_TEST_3: // 锁任务中发成功
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, event_bits = %d\n", g_event_id, g_test_bits1);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            // 锁任务
            test_suite_log_stringf("start lock task!\r\n");
            ret_lock = osKernelLock();
            test_suite_log_stringf("ret_lock = %d\r\n", ret_lock);
            ret = osEventFlagsSet(g_evt_id, flags);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            osal_udelay(PARAM_10_MILLION);
            ret_unlock = osKernelUnlock();
            test_suite_log_stringf("ret_unlock = %d\r\n", ret_unlock);

            ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAll, TD_SYS_WAIT_FOREVER);
            test_suite_log_stringf("Wait_EventFlag = 0x%x\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            // 删除
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            ret = osEventFlagsSet(g_evt_id, PARAM_X_2000000);
            test_suite_log_stringf("ret = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);

            ret = osEventFlagsSet(g_evt_id, PARAM_X_FFFFFFFF);
            test_suite_log_stringf("ret = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);

            ret = osEventFlagsSet(&evt_id, PARAM_X_1);
            test_suite_log_stringf("ret = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osEventFlagsSet(NULL, flags);
            test_suite_log_stringf("ret = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        case MULTI_TASK_TEST: // 一等(等多个)对多发
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, event_bits = %d\n", g_evt_id, g_test_bits);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_evt_id1 = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, event_bits = %d\n", g_evt_id1, g_test_bits1);
            if (g_evt_id1 == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id = osThreadNew((osThreadFunc_t)cmsis_event_task_wait, NULL, &thread_attr);
            test_suite_log_stringf("thread_id = %d\r\n", thread_id);
            if (thread_id == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id1 = osThreadNew((osThreadFunc_t)cmsis_event_task_send, (td_void*)PARAM_2, &thread_attr);
            test_suite_log_stringf("thread_id1 = %d\r\n", thread_id1);
            if (thread_id1 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id1);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id2 = osThreadNew((osThreadFunc_t)cmsis_event_task_send, (td_void*)PARAM_3, &thread_attr);
            test_suite_log_stringf("thread_id2 = %d\r\n", thread_id2);
            if (thread_id2 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id2);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case MULTI_TASK_TEST_1: // 一等（等一个）对多发
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, g_test_bits = %d, g_test_bits1 = %d\n", g_evt_id, g_test_bits,
                                   g_test_bits1);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id = osThreadNew((osThreadFunc_t)cmsis_event_task_wait1, NULL, &thread_attr);
            test_suite_log_stringf("thread_id = %d\r\n", thread_id);
            if (thread_id == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id1 = osThreadNew((osThreadFunc_t)cmsis_event_task_send1, &g_test_bits, &thread_attr);
            test_suite_log_stringf("thread_id1 = %d\r\n", thread_id1);
            if (thread_id1 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id1);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id2 = osThreadNew((osThreadFunc_t)cmsis_event_task_send1, &g_test_bits1, &thread_attr);
            test_suite_log_stringf("thread_id2 = %d\r\n", thread_id2);
            if (thread_id2 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id2);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case MULTI_TASK_TEST_2: // 多等对多发
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, event_bits = %d\n", g_event_id, g_test_bits1);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_evt_id1 = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_event_id = %d, event_bits = %d\n", g_event_id, g_test_bits1);
            if (g_evt_id1 == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id = osThreadNew((osThreadFunc_t)cmsis_event_task_wait2, (td_void*)1, &thread_attr);
            test_suite_log_stringf("thread_id = %d\r\n", thread_id);
            if (thread_id == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id1 = osThreadNew((osThreadFunc_t)cmsis_event_task_wait3, (td_void*)1, &thread_attr);
            test_suite_log_stringf("thread_id1 = %d\r\n", thread_id1);
            if (thread_id1 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id1);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id2 = osThreadNew((osThreadFunc_t)cmsis_event_task_send, (td_void*)PARAM_2, &thread_attr);
            test_suite_log_stringf("thread_id2 = %d\r\n", thread_id2);
            if (thread_id2 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", thread_id2);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            thread_id3 = osThreadNew((osThreadFunc_t)cmsis_event_task_send, (td_void*)PARAM_3, &thread_attr);
            test_suite_log_stringf("thread_id3 = %d\r\n", thread_id3);
            if (thread_id3 == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId3 = %d\r\n", thread_id3);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);

    return test_result;
}

int cts_os_eventflagsclear(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret = 0;
    uint32_t evt_id = PARAM_10;
    uint32_t pro_flags = 0;
    uint32_t timeout = PARAM_10000;
    osStatus_t status;
    uint32_t flags = (uint32_t)strtoul(argv[1], TD_NULL, PARAM_16);
    test_suite_log_stringf("flags = 0x%x\r\n", flags);
    thread_attr.name = "test_Event";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityBelowNormal; // ;osPriorityAboveNormal osPriorityNormal
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    switch (param) {
        case NORMAL_TEST:
            pro_flags = osEventFlagsGet(g_evt_id);
            ret = osEventFlagsClear (g_evt_id, flags);
            test_suite_log_stringf("g_evt_id = %d;pro_flags = %d, EventFlags = 0x%x\r\n", g_evt_id, pro_flags, ret);
            check_test_result(ret, pro_flags, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 等待过程中清除      ，等待完成后清除; 删除后清除
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_evt_id = %d\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            g_threadid = osThreadNew(cmsis_event_wait_forever_body, &flags, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 等待过程中清除
            ret = osal_msleep(timeout / PARAM_2);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            ret = osEventFlagsClear (g_evt_id, flags);
            test_suite_log_stringf("g_evt_id = %d;Clear_EventFlags = 0x%x\r\n", g_evt_id, ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            // 等待完成后清除
            ret = osEventFlagsSet(g_evt_id, flags);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            ret = osal_msleep(timeout / PARAM_2);
            check_test_result(ret, TEST_SUITE_OK, TD_TRUE, &test_result);
            ret = osEventFlagsClear (g_evt_id, flags);
            test_suite_log_stringf("g_evt_id = %d;Clear_EventFlags = 0x%x\r\n", g_evt_id, ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            // 删除后清除
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            ret = osEventFlagsClear (g_evt_id, flags);
            test_suite_log_stringf("g_evt_id = %d;Clear_EventFlags = 0x%x\r\n", g_evt_id, ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            ret = osEventFlagsClear (&evt_id, flags);
            test_suite_log_stringf("Clear_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osEventFlagsClear (NULL, flags);
            test_suite_log_stringf("Clear_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_eventflagsget(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    uint32_t evt_id = PARAM_10;
    uint32_t flags;
    switch (param) {
        case NORMAL_TEST:
            flags = osEventFlagsGet(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, Get_EventFlags = 0x%x\r\n", g_evt_id, flags);
            break;
        case ABNORMAL_TEST:
            flags = osEventFlagsGet(&evt_id);
            test_suite_log_stringf("Get_EventFlags = 0x%x\r\n", flags);
            check_test_result(flags, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            flags = osEventFlagsGet(NULL);
            test_suite_log_stringf("Get_EventFlags = 0x%x\r\n", flags);
            check_test_result(flags, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_eventflagswait(int argc, char *argv[])
{
    if (argc != PARAM_3) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret = 0;
    uint32_t evt_id = PARAM_10;
    uint64_t start_us = 0;
    uint64_t arrive_us = 0;
    uint64_t wait_time = 0;
    uint32_t get_flags = 0;
    osStatus_t status;
    uint32_t flags = (uint32_t)strtoul(argv[1], TD_NULL, PARAM_16);
    uint32_t timeout = (uint32_t)strtol(argv[PARAM_2], NULL, 0);
    test_suite_log_stringf("flags = 0x%x ,timeout = %d\r\n", flags, timeout);

    switch (param) {
        case NORMAL_TEST:
            ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAny, timeout);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            if ((ret & flags) == 0) {
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1:
            ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAll, timeout);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2:
            ret = osEventFlagsWait(g_evt_id, flags, (osFlagsWaitAny | osFlagsNoClear), timeout);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            if ((ret & flags) == 0) {
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_3:
            ret = osEventFlagsWait(g_evt_id, flags, (osFlagsWaitAll | osFlagsNoClear), timeout);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            break;

        case NORMAL_TEST_4: // 永久等待,有事件的时候，永久等待会立即返回
            test_suite_log_stringf("wait event forever, please send event\r\n");
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            ret = osEventFlagsSet(g_evt_id, flags);
            get_flags = osEventFlagsGet(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, ret = 0x%x, get_flags = 0x%x\r\n", g_evt_id, ret, get_flags);
            check_test_result(ret, get_flags, TD_TRUE, &test_result);

            ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAll, TD_SYS_WAIT_FOREVER);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_5: // 永久等待,等待过程中释放
            test_suite_log_stringf("wait event forever, please send event\r\n");
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_threadid = osThreadNew(cmsis_event_wait_forever_body, &flags, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success,g_threadId2 = %d\r\n", g_threadid);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osal_msleep(SLEEP_5000);

            ret = osEventFlagsSet(g_evt_id, flags);
            get_flags = osEventFlagsGet(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, ret = 0x%x, get_flags = 0x%x\r\n", g_evt_id, ret, get_flags);
            check_test_result(ret, get_flags, TD_TRUE, &test_result);
            osal_msleep(SLEEP_5000);
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST: // 等待超时
            start_us = uapi_tcxo_get_us();
            ret = osEventFlagsWait(g_evt_id, flags, osFlagsWaitAll, timeout);
            arrive_us = uapi_tcxo_get_us();
            wait_time = (arrive_us - start_us) / PARAM_10000;
            test_suite_log_stringf("start_us = %llu us, arrive_us = %llu us, wait_time = %llu tick\r\n", start_us,
                                   arrive_us, wait_time);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorTimeout, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1:
            ret = osEventFlagsWait(&evt_id, flags, osFlagsWaitAll, timeout);
            test_suite_log_stringf("Wait_EventFlags1 = 0x%x\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);

            ret = osEventFlagsWait(g_evt_id, PARAM_X_FFFFFFFF, osFlagsWaitAll, timeout);
            test_suite_log_stringf("Wait_EventFlags2 = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);

            ret = osEventFlagsWait(g_evt_id, PARAM_X_02000000, osFlagsWaitAll, timeout);
            test_suite_log_stringf("Wait_EventFlags3 = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);

            ret = osEventFlagsWait(g_evt_id, flags, PARAM_X_00000004, timeout);
            test_suite_log_stringf("Wait_EventFlags4 = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osEventFlagsWait(NULL, flags, osFlagsWaitAll, timeout);
            test_suite_log_stringf("Wait_EventFlags = 0x%x\r\n", ret);
            check_test_result(ret, osFlagsErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_eventflagsdelete(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t ret = 0;
    uint32_t flags = 1;
    osStatus_t status;
    thread_attr.name = "test_Event";
    thread_attr.cb_size = CB_SIZE;
    thread_attr.stack_size = STACK_SIZE;
    thread_attr.priority = osPriorityBelowNormal; // ;osPriorityAboveNormal osPriorityNormal
    thread_attr.tz_module = 0;
    thread_attr.cb_mem = kv_malloc(thread_attr.cb_size);
    if (thread_attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    thread_attr.stack_mem = osal_kmalloc_align(thread_attr.stack_size, 0, 16);
    if (thread_attr.stack_mem == TD_NULL) {
        test_suite_log_stringf("stack_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    switch (param) {
        case NORMAL_TEST:
            status = osEventFlagsDelete(g_evt_id);
            test_suite_log_stringf("g_evt_id = %d, status = %d\r\n", g_evt_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_1: // 永久等待,等待过程中删除
            test_suite_log_stringf("wait event forever, please send event\r\n");
            g_evt_id = osEventFlagsNew(NULL);
            test_suite_log_stringf("g_evt_id = %d\r\n", g_evt_id);
            if (g_evt_id == NULL) {
                test_suite_log_stringf("EventFlagsNew Fail,g_evt_id = %d\r\n", g_evt_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_threadid = osThreadNew((td_void*)cmsis_event_wait_forever_body, &flags, &thread_attr);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            osal_msleep(SLEEP_5000);
            status = osEventFlagsDelete(&g_evt_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);

            test_suite_log_stringf("set_flags = %d\r\n", flags);
            ret = osEventFlagsSet(g_evt_id, flags);
            test_suite_log_stringf("set_ret = %d\r\n", ret);
            check_test_result(ret, flags, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osEventFlagsDelete(NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(thread_attr.cb_mem);
    kv_free(thread_attr.stack_mem);
    return test_result;
}

int cts_os_semaphorenew(int argc, char *argv[])
{
    if (argc != PARAM_6) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t timeout = PARAM_1000;
    osSemaphoreId_t sem_id[LOSCFG_BASE_IPC_SEM_LIMIT+1] = {0};
    osStatus_t status;
    ext_os_resource_use_stat os_resource_use = {0};
    uint32_t create_cnt = 1;
    ext_mdm_mem_info mem_info = {0, 0, 0, 0, 0, 0, 0};
    uint32_t mem_start = 0;
    uint32_t mem_end = 0;
    uint32_t max_count = strtol(argv[1], NULL, 0);
    uint32_t initial_count = strtol(argv[PARAM_2], NULL, 0);
    osSemaphoreAttr_t attr ;
    attr.name = argv[PARAM_3];
    attr.attr_bits  = strtol(argv[PARAM_4], NULL, 0);
    attr.cb_size = strtol(argv[PARAM_5], NULL, 0);
    attr.cb_mem = kv_malloc(attr.cb_size);
    if (attr.cb_mem == TD_NULL) {
        test_suite_log_stringf("cb_mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    test_suite_log_stringf("max_count = %d,initial_count = %d,attr.name = %s, attr_bits = %d,cb_size = %d\r\n",
                           max_count, initial_count, attr.name, attr.attr_bits, attr.cb_size);
    uapi_watchdog_init(time_out);
    switch (param) {
        case NORMAL_TEST:
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1: // 获取后创建
        case NORMAL_TEST_2: // 释放后创建
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            if (param == NORMAL_TEST_1) {
                status = osSemaphoreAcquire(g_semaphore_id, timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            } else if (param == NORMAL_TEST_2) {
                status = osSemaphoreRelease(g_semaphore_id);
                check_test_result(status, osOK, TD_TRUE, &test_result);
            }
            sem_id[0] = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("sem_id[0] = 0x%x\r\n", sem_id[0]);
            if (sem_id[0] == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,sem_id[0] = 0x%x\r\n", sem_id[0]);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            status = osSemaphoreDelete(g_semaphore_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osSemaphoreDelete(sem_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id != NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            sem_id[0] = osSemaphoreNew(max_count, initial_count, NULL);
            test_suite_log_stringf("sem_id[0] = 0x%x\r\n", sem_id[0]);
            if (sem_id[0] == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,sem_id[0] = 0x%x\r\n", sem_id[0]);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            attr.name = NULL;
            sem_id[1] = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("sem_id[1] = 0x%x\r\n", sem_id[1]);
            if (sem_id[1] == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,sem_id[1] = 0x%x\r\n", sem_id[1]);
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osSemaphoreDelete(sem_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osSemaphoreDelete(sem_id[1]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST: // 创建支持最大信号量个数后再继续创建
            test_suite_log_stringf("current use sem:%d\r\n", os_resource_use.sem_usage);
            for (create_cnt = os_resource_use.sem_usage; create_cnt < LOSCFG_BASE_IPC_SEM_LIMIT; create_cnt++) {
                max_count = 1 + max_count;
                initial_count = 1 + initial_count; // 构造不同的值
                sem_id[create_cnt] = osSemaphoreNew(max_count, initial_count, &attr);
                test_suite_log_stringf("create_cnt=%d,sem_id[create_cnt]=0x%x,max_count=%d, initial_count = %d\r\n",
                                       create_cnt, sem_id[create_cnt], max_count, initial_count);
                if (sem_id[create_cnt] == NULL) {
                    test_suite_log_stringf("osSemaphoreNew Fail,create_cnt = 0x%x\r\n", create_cnt);
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }

            if (test_result == TEST_SUITE_OK) {
                // 继续create应该返回失败
                test_suite_log_stringf("after sem create max, begin create cnt:%u\r\n", create_cnt);
                sem_id[create_cnt] = osSemaphoreNew(max_count, initial_count, &attr); // 继续创建失败
                if (sem_id[create_cnt] != NULL) {
                    test_suite_log_stringf("osSemaphoreNew Fail,sem_id[create_cnt] = 0x%x\r\n", sem_id[create_cnt]);
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }
            create_cnt --;

            while (create_cnt >= os_resource_use.sem_usage) {
                status = osSemaphoreDelete(sem_id[create_cnt]);
                test_suite_log_stringf("delete ret = 0x%x, create_cnt = %d\r\n", status, create_cnt);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                create_cnt--;
            }
            break;
        case PRESSURE_TEST_1: // 内存泄漏测试:反复创建和删除
            test_suite_log_stringf("start: mem total size = %d,------mem free size = %d \r\n", mem_info.total,
                                   mem_info.free);
            mem_start = mem_info.free;

            for (uint32_t i = 0; i < g_press_test_cnt; i++) {
                g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
                if (g_semaphore_id == NULL) {
                    test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                    test_result = TEST_SUITE_TEST_FAILED;
                }

                status = osSemaphoreAcquire(g_semaphore_id, timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osSemaphoreRelease(g_semaphore_id);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osSemaphoreDelete(g_semaphore_id);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                if (i % PARAM_1000 == 0) {
                    test_suite_log_stringf("num = %d,----mem total size = %d,------mem free size = %d \r\n", i,
                                           mem_info.total, mem_info.free);
                    uapi_watchdog_kick();
                }
            }
            test_suite_log_stringf("end: mem total size = %d,--mem free size = %d\r\n", mem_info.total, mem_info.free);
            mem_end = mem_info.free;
            check_test_result(mem_end, mem_start, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST_2: // 内存泄漏测试:反复获取和释放
            test_suite_log_stringf("start: mem total size=%d,---mem free size=%d\r\n", mem_info.total, mem_info.free);
            mem_start = mem_info.free;

            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            for (uint32_t i = 1; i <= g_press_test_cnt; i++) {
                status = osSemaphoreAcquire(g_semaphore_id, timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osSemaphoreRelease(g_semaphore_id);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                if (i % PARAM_1000 == 0) {
                    test_suite_log_stringf("num = %d,----mem total size = %d,------mem free size = %d \r\n", i,
                                           mem_info.total, mem_info.free);
                    uapi_watchdog_kick();
                }
            }
            status = osSemaphoreDelete(g_semaphore_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            test_suite_log_stringf("end: mem total size = %d,------mem free size = %d \r\n", mem_info.total,
                                   mem_info.free);
            mem_end = mem_info.free;
            check_test_result(mem_end, mem_start, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(attr.cb_mem);

    return test_result;
}

int cts_os_semaphoreacquire(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    test_index = 0;
    uint32_t timeout = strtol(argv[1], NULL, 0);
    osStatus_t status;
    uint32_t max_count = PARAM_30;
    uint32_t initial_count = PARAM_20;
    osSemaphoreAttr_t attr ;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    switch (param) {
        case NORMAL_TEST:
            status = osSemaphoreAcquire(g_semaphore_id, timeout);
            test_suite_log_stringf("g_semaphore_id = 0x%x, status = %d\r\n", g_semaphore_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // 永久等待,有信号量的时候，永久等待会立即返回
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            status = osSemaphoreAcquire(g_semaphore_id, TD_SYS_WAIT_FOREVER);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            status = osSemaphoreDelete(g_semaphore_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 永久等待,等待过程中释放
        case NORMAL_TEST_3: // 永久等待,等待过程中删除
        case NULL_POINTER_TEST:
            status = osSemaphoreAcquire(NULL, timeout);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_semaphorerelease(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    uint32_t sem_id = PARAM_10;
    osStatus_t status = 0;
    uint32_t max_count = 0;
    uint32_t initial_count = 0;
    uint32_t wait_time = 0;
    uint32_t timeout = PARAM_1000;
    g_timer_start_u64 = 0;
    g_timer_arrive_u64 = 0;
    osSemaphoreAttr_t attr ;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    switch (param) {
        case NORMAL_TEST:
            status = osSemaphoreRelease(g_semaphore_id);
            test_suite_log_stringf("g_semaphore_id = 0x%x , status = %d\r\n", g_semaphore_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1: // wait初始值为0的信号量失败，执行signal后可以wait成功
            max_count = PARAM_10;
            initial_count = 0;
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            test_suite_log_stringf("g_semaphore_id = %d\r\n", g_semaphore_id);
            g_timer_start_u64 = uapi_tcxo_get_ms();
            status = osSemaphoreAcquire(g_semaphore_id, timeout);
            g_timer_arrive_u64 = uapi_tcxo_get_ms();
            wait_time = (g_timer_arrive_u64 - g_timer_start_u64);
            check_test_result(status, osErrorTimeout, TD_TRUE, &test_result);
            test_suite_log_stringf("g_timer_start_u64 = %llu us,g_timer_arrive_u64 =%lluus,wait_time = %llu tick\r\n",
                                   g_timer_start_u64, g_timer_arrive_u64, wait_time);
            if (wait_time > timeout + PARAM_2 || wait_time < timeout - PARAM_2) {
                test_suite_log_stringf("wait_time err, please check!");
                return TEST_SUITE_TEST_FAILED;
            }

            status = osSemaphoreRelease(g_semaphore_id);
            test_suite_log_stringf("g_semaphore_id = 0x%x , status = %d\r\n", g_semaphore_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            status = osSemaphoreAcquire(g_semaphore_id, timeout);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            status = osSemaphoreDelete(g_semaphore_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 创建初始值为65534的信号量，连续wait65534次之后第65535次wait返回超时，执行signal后可以继续wait
            max_count = MAX_COUNT;
            initial_count = MAX_COUNT;
            g_semaphore_id = osSemaphoreNew(max_count, initial_count, &attr);
            test_suite_log_stringf("g_semaphore_id = 0x%x\r\n", g_semaphore_id);
            if (g_semaphore_id == NULL) {
                test_suite_log_stringf("osSemaphoreNew Fail,g_semaphore_id = 0x%x\r\n", g_semaphore_id);
                test_result = TEST_SUITE_TEST_FAILED;
            }

            for (uint32_t i = 0; i < initial_count; i++) {
                status = osSemaphoreAcquire(g_semaphore_id, timeout);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                if (i % PARAM_10000 == 0) {
                    test_suite_log_stringf("cnt = %u,s_sem_id[2] = %d, osSemaphoreAcquire status = 0x%x \r\n", i,
                                           g_semaphore_id, status);
                    uapi_watchdog_kick();
                }
            }
            // 第65535次wait失败
            test_suite_log_stringf("g_semaphore_id = %d\r\n", g_semaphore_id);
            g_timer_start_u64 = uapi_tcxo_get_ms();
            status = osSemaphoreAcquire(g_semaphore_id, timeout);
            g_timer_arrive_u64 = uapi_tcxo_get_ms();
            wait_time = (g_timer_arrive_u64 - g_timer_start_u64);
            check_test_result(status, osErrorTimeout, TD_TRUE, &test_result);
            test_suite_log_stringf("g_timer_start_u64 = %llu us, g_timer_arrive_u64=%lluus,wait_time = %llu tick\r\n",
                                   g_timer_start_u64, g_timer_arrive_u64, wait_time);
            if (wait_time > timeout + PARAM_2 || wait_time < timeout - PARAM_2) {
                test_suite_log_stringf("wait_time err, please check!");
                return TEST_SUITE_TEST_FAILED;
            }

            status = osSemaphoreRelease(g_semaphore_id);
            test_suite_log_stringf("g_semaphore_id = 0x%x , status = %d\r\n", g_semaphore_id, status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            // 再次wait成功
            status = osSemaphoreAcquire(g_semaphore_id, timeout);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            status = osSemaphoreDelete(g_semaphore_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osSemaphoreRelease(&sem_id);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osSemaphoreRelease(NULL);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_semaphoregetcount(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t sem_id = PARAM_10;
    uint32_t ret = 0;
    switch (param) {
        case NORMAL_TEST:
            ret = osSemaphoreGetCount(g_semaphore_id);
            test_suite_log_stringf("g_semaphore_id = 0x%x, get_count_ret = %d\r\n", g_semaphore_id, ret);
            break;
        case ABNORMAL_TEST:
            ret = osSemaphoreGetCount(&sem_id);
            test_suite_log_stringf("get_count_ret = %d\r\n", ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osSemaphoreGetCount(NULL);
            test_suite_log_stringf("get_count_ret = %d\r\n", ret);
            check_test_result(ret, 0, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_semaphoredelete(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    td_s32 ret = 0;
    uint32_t test_result = TEST_SUITE_OK;
    switch (param) {
        case NORMAL_TEST:
            test_suite_log_stringf("g_semaphore_id = %d\r\n", g_semaphore_id);
            ret = osSemaphoreDelete(g_semaphore_id);
            test_suite_log_stringf("dele_ret = %d\r\n", ret);
            check_test_result(ret, osOK, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            ret = osSemaphoreDelete(NULL);
            test_suite_log_stringf("dele_ret = %d\r\n", ret);
            check_test_result(ret, osErrorParameter, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

void cmsis_msg_task_send(void *data)
{
    unused(data);
    uint32_t test_result = TEST_SUITE_OK;
    uint8_t msge[PARAM_2] = {0x11, 0x22};
    uint32_t msg_size = sizeof(msge);
    osStatus_t status;
    uint8_t msg_prio = 0;
    osMessageQueueAttr_t attr ;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    g_mq_id[0] = osMessageQueueNew(PARAM_1000, msg_size, &attr);
    test_suite_log_stringf("g_mq_id[0] = %d, msg_size = %d \r\n", g_mq_id[0], msg_size);
    if (g_mq_id[0] == NULL) {
        test_suite_log_stringf("osMessageQueueNew Fail\r\n");
        test_result = TEST_SUITE_TEST_FAILED;
    }
    for (uint32_t i = 0; i < PARAM_100; i++) { // i=5000
        status = osMessageQueuePut(g_mq_id[0], msge, msg_prio, 0);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        osal_msleep(SLEEP_20);
    }
    if (test_result == (uint32_t)TEST_SUITE_TEST_FAILED) {
        test_suite_log_stringf("ctsmsg_task_send fail,please check!\r\n");
    } else {
        test_suite_log_stringf("ctsmsg_task_send success!\r\n");
    }
}

void cmsis_msg_task_send_1(void *data)
{
    unused(data);
    uint32_t test_result = TEST_SUITE_OK;
    uint8_t msge[PARAM_10] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAB, 0xBC, 0xCD, 0xDE};
    uint32_t msg_size = sizeof(msge);
    osStatus_t status;
    uint8_t msg_prio = 0;
    osMessageQueueAttr_t attr ;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    g_mq_id[1] = osMessageQueueNew(PARAM_1000, msg_size, &attr);
    test_suite_log_stringf(" g_mq_id[1] = %d, msg_size = %d\r\n", g_mq_id[1], msg_size);
    if (g_mq_id[1] == NULL) {
        test_suite_log_stringf("osMessageQueueNew Fail\r\n");
        test_result = TEST_SUITE_TEST_FAILED;
    }
    for (uint32_t i = 0; i < PARAM_100; i++) {
        status = osMessageQueuePut(g_mq_id[1], msge, msg_prio, 0);
        check_test_result(status, osOK, TD_TRUE, &test_result);
        osal_msleep(SLEEP_20);
    }
    if (test_result == (uint32_t)TEST_SUITE_TEST_FAILED) {
        test_suite_log_stringf("ctsmsg_task_send_1 fail,please check!\r\n");
    } else {
        test_suite_log_stringf("ctsmsg_task_send_1 success!\r\n");
    }
}

void cmsis_msg_task_wait(void *data)
{
    unused(data);
    uint32_t ret = TEST_SUITE_OK;
    uint32_t test_result = TEST_SUITE_OK;
    uint8_t msge[PARAM_2] = {0x11, 0x22};
    uint8_t get_mage[PARAM_2] = {0};
    uint8_t msge1[PARAM_10] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAB, 0xBC, 0xCD, 0xDE};
    uint8_t get_mage1[PARAM_10] = {0};
    uint8_t get_prio = 0;
    osStatus_t status;
    for (int i = 0; i < PARAM_100; i++) { // 5000
        status = osMessageQueueGet(g_mq_id[0], &get_mage, &get_prio, TD_SYS_WAIT_FOREVER);
        if (status != osOK || msge[0] != get_mage[0] || msge[1] != get_mage[1]) {
            test_suite_log_stringf("the first msg queue wait error!!!!,the num = %d \r\n", i);
            test_suite_log_stringf("get_mage[0]= 0x%x,get_mage[0]=0x%x,ret = 0x%x\r\n", get_mage[0], get_mage[1], ret);
            test_result = TEST_SUITE_TEST_FAILED;
        }

        status = osMessageQueueGet(g_mq_id[1], &get_mage1, &get_prio, TD_SYS_WAIT_FOREVER);
        if (status != osOK || msge1[0] != get_mage1[0] || msge1[PARAM_2] != get_mage1[PARAM_2] || msge1[PARAM_5] !=
            get_mage1[PARAM_5] || msge1[PARAM_9] != get_mage1[PARAM_9]) {
            test_suite_log_stringf("the first msg queue wait error!!!!,the num = %d \r\n", i);
            test_suite_log_stringf("get_mage1[0]= 0x%x, get[2]= 0x%x, get[5]= 0x%x, get[9]= 0x%x,ret = 0x%x\r\n",
                                   get_mage1[0], get_mage1[PARAM_2], get_mage1[PARAM_5], get_mage1[PARAM_9], ret);
            test_result = TEST_SUITE_TEST_FAILED;
        }
        osal_msleep(SLEEP_20);
    }
    if (test_result == (uint32_t)TEST_SUITE_TEST_FAILED) {
        test_suite_log_stringf("ctsmsg_task_wait fail,please check!\r\n");
    } else {
        test_suite_log_stringf("ctsmsg_task_wait success!\r\n");
    }
    status = osMessageQueueDelete(g_mq_id[0]);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    status = osMessageQueueDelete(g_mq_id[1]);
    check_test_result(status, osOK, TD_TRUE, &test_result);
}

td_void cmsis_msg_wait_forver_body(void *param)
{
    unused(param);
    uint32_t test_result = TEST_SUITE_OK;
    ext_sys_queue_msg stmsg = {0};
    uint8_t get_prio = 0;
    osStatus_t status;

    status = osMessageQueueGet(g_mq_id[0], &stmsg, &get_prio, TD_SYS_WAIT_FOREVER);
    check_test_result(status, osOK, TD_TRUE, &test_result);
    test_suite_log_stringf("Msg.msg_id = 0x%x,Msg.param[0] = 0x%x,Msg[1] = 0x%x,Msg[2] = 0x%x,Msg[3]= 0x%x\r\n",
                           stmsg.msg_id, stmsg.param[0], stmsg.param[1], stmsg.param[PARAM_2], stmsg.param[PARAM_3]);
    if (test_result == TEST_SUITE_OK) {
        test_suite_log_stringf("msg_wait_forever_body success\r\n");
    } else {
        test_suite_log_stringf("msg_wait_forever_body fail\r\n");
    }
}

int cts_os_messagequeuenew(int argc, char *argv[])
{
    if (argc != PARAM_7) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t msg_count = strtol(argv[1], NULL, 0);
    uint32_t msg_size = strtol(argv[PARAM_2], NULL, 0);
    osMessageQueueAttr_t  attr;
    attr.name = argv[PARAM_3];
    attr.attr_bits = strtol(argv[PARAM_4], NULL, 0);
    attr.cb_size = strtol(argv[PARAM_5], NULL, 0);
    attr.mq_size = strtol(argv[PARAM_6], NULL, 0);
    attr.cb_mem = kv_malloc(attr.cb_size);
    attr.mq_mem = kv_malloc(attr.mq_size);
    if (attr.cb_mem == TD_NULL || attr.mq_mem == TD_NULL) {
        test_suite_log_stringf("mem failed!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    uint32_t time_out = TD_SYS_WAIT_FOREVER;
    osStatus_t put_status;
    osStatus_t get_status;
    osStatus_t del_status;
    uint8_t msge[PARAM_2] = {0x11, 0x22};
    uint8_t get_msge[PARAM_2] = {0};
    uint8_t msg_prio = PARAM_10;
    uint8_t get_prio = 0;
    s_msg_index =0;
    test_suite_log_stringf("msg_count = %d,msg_size = %d,attr_name = %s, attr_bits = %d,cb_size = %d,mq_size = %d\r\n",
                           msg_count, msg_size, attr.name, attr.attr_bits, attr.cb_size, attr.mq_size);

    switch (param) {
        case NORMAL_TEST:
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            test_suite_log_stringf(" g_mq_id[0] = %d\r\n", g_mq_id[0]);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NORMAL_TEST_1: // 发送后创建
            time_out = TD_SYS_WAIT_FOREVER;
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            put_status = osMessageQueuePut(g_mq_id[0], msge, msg_prio, time_out);
            check_test_result(put_status, osOK, TD_TRUE, &test_result);

            g_mq_id[1] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[1] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            // 清理环境
            del_status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            del_status = osMessageQueueDelete(g_mq_id[1]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 等待后创建,创建长度和发送等待长度要一致，否则会死机
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            put_status = osMessageQueuePut(g_mq_id[0], &msge, msg_prio, time_out);
            check_test_result(put_status, osOK, TD_TRUE, &test_result);

            get_status = osMessageQueueGet(g_mq_id[0], &get_msge, &get_prio, time_out);
            test_suite_log_stringf("get_status = %d, msg_prio = %d, get_msge[0] = 0x%x, get_msge[1] = 0x%x\r\n",
                                   get_status, get_prio, get_msge[0], get_msge[1]);
            check_test_result(get_status, osOK, TD_TRUE, &test_result);

            g_mq_id[1] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[1] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 清理环境
            del_status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            del_status = osMessageQueueDelete(g_mq_id[1]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            test_suite_log_stringf(" g_mq_id[0] = %d\r\n", g_mq_id[0]);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        case NULL_POINTER_TEST:
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, NULL);
            test_suite_log_stringf("g_mq_id[0] = %d\r\n", g_mq_id[0]);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            attr.cb_mem = NULL;
            g_mq_id[1] = osMessageQueueNew(msg_count, msg_size, &attr);
            test_suite_log_stringf("g_mq_id[1] = %d\r\n", g_mq_id[1]);
            if (g_mq_id[1] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            attr.mq_mem = NULL;
            g_mq_id[PARAM_2] = osMessageQueueNew(msg_count, msg_size, &attr);
            test_suite_log_stringf("g_mq_id[2] = %d\r\n", g_mq_id[PARAM_2]);
            if (g_mq_id[PARAM_2] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            // 清理环境
            del_status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            del_status = osMessageQueueDelete(g_mq_id[1]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            del_status = osMessageQueueDelete(g_mq_id[PARAM_2]);
            check_test_result(del_status, osOK, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    kv_free(attr.cb_mem);
    kv_free(attr.mq_mem);
    return test_result;
}

int cts_os_messagequeueput(int argc, char *argv[])
{
    if (argc != PARAM_3) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint8_t msg_prio = strtol(argv[1], NULL, 0);
    uint32_t time_out = strtol(argv[PARAM_2], NULL, 0);
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    uint32_t get_count = 0;
    uint32_t *msg_ptr_buf = NULL;
    osStatus_t status;
    uint32_t mil_sec = 0;
    uint32_t mil_sec1 = 0;
    ext_sys_queue_msg stmsg = { 0 };
    stmsg.msg_id = 1;
    stmsg.param[0] = PARAM_X_10;
    stmsg.param[1] = PARAM_X_22;
    stmsg.param[PARAM_2] = PARAM_X_33;
    uint32_t arg = PARAM_X_5;
    osMessageQueueAttr_t  attr;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    uint32_t msg_size = sizeof(stmsg);
    test_suite_log_stringf("msg_prio = %d, time_out = %d, msg_size = %d\r\n", msg_prio, time_out, msg_size);
    switch (param) {
        case NORMAL_TEST: // ulTimeOut = 100,msg_size=10
            status = osMessageQueuePut(g_mq_id[0], (td_pvoid)&stmsg, msg_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            get_count = osMessageQueueGetCount(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d, GetCurrentMsgNum = %d\r\n", g_mq_id[0], get_count);;
            break;
        case NORMAL_TEST_1:
            g_mq_id[0] = osMessageQueueNew(1, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMessageQueuePut(g_mq_id[0], (td_pvoid) & stmsg, msg_prio, time_out);
            mil_sec=uapi_tcxo_get_us();
            status += osMessageQueuePut(g_mq_id[0], (td_pvoid) & stmsg, msg_prio, time_out);
            mil_sec1=uapi_tcxo_get_us();
            check_test_result(status, osErrorTimeout, TD_TRUE, &test_result);
            test_suite_log_stringf("mil_sec = %d, mil_sec1 = %d\r\n", mil_sec, mil_sec1);
            // 清理环境
            status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osMessageQueuePut(&mq_id, (td_pvoid)&stmsg, msg_prio, time_out);
            test_suite_log_stringf("status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            test_suite_log_stringf("msg_ptr_buf =%d,msg_prio = %d, time_out=%d\r\n", msg_ptr_buf, msg_prio, time_out);
            status = osMessageQueuePut(NULL, msg_ptr_buf, msg_prio, time_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);

            test_suite_log_stringf("g_mq_id[0]=%d,msg_prio = %d, time_out = %d\r\n", g_mq_id[0], msg_prio, time_out);
            status = osMessageQueuePut(g_mq_id[0], NULL, msg_prio, time_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case MULTI_TASK_TEST:
            g_testid[0] = osThreadNew(cmsis_msg_task_send, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_testid[0] = %d\r\n", g_testid[0]);
            if (g_testid[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_testid[1] = osThreadNew(cmsis_msg_task_send_1, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_testid[1] = %d\r\n", g_testid[1]);
            if (g_testid[1] == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            g_testid[PARAM_2] = osThreadNew(cmsis_msg_task_wait, &arg, &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_testid[2] = %d\r\n", g_testid[PARAM_2]);
            if (g_testid[PARAM_2] == NULL) {
                test_suite_log_stringf("osThreadNew not success\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            break;
        default :
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

int cts_os_messagequeueget(int argc, char *argv[])
{
    if (argc != PARAM_2) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint8_t msg_prio = PARAM_10;
    uint8_t get_prio = 0;
    uint32_t time_out = strtol(argv[1], NULL, 0);
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    osStatus_t status;
    ext_sys_queue_msg stmsg = { 0 };
    stmsg.msg_id = 1;
    stmsg.param[0] = PARAM_X_10;
    stmsg.param[1] = PARAM_X_22;
    stmsg.param[PARAM_2] = PARAM_X_33;
    ext_sys_queue_msg get_msge = {0};
    osMessageQueueAttr_t  attr;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    uint32_t msg_size = sizeof(get_msge);
    uint32_t mil_sec = 0;
    uint32_t mil_sec1 = 0;
    uint32_t wait_time = 0;
    uint32_t msg_count = PARAM_10;
    switch (param) {
        case NORMAL_TEST:
            status = osMessageQueueGet(g_mq_id[0], &stmsg, &get_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_1:
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            time_out = TD_SYS_WAIT_FOREVER;
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMessageQueuePut(g_mq_id[0], &stmsg, msg_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            status = osMessageQueueGet(g_mq_id[0], &get_msge, &get_prio, time_out);
            test_suite_log_stringf("status = %d, get_prio = %d, msg_id = 0x%x\r\n", status, get_prio, get_msge.msg_id);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            // 清理环境
            status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_2: // 永久等待,等待过程中发送
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            time_out = TD_SYS_WAIT_FOREVER;
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            test_suite_log_stringf("g_mq_id[0] = %d \r\n", g_mq_id[0]);
            g_threadid = osThreadNew(cmsis_msg_wait_forver_body, &g_mq_id[0], &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osal_msleep(SLEEP_10000);
            status = osMessageQueuePut(g_mq_id[0], &stmsg, msg_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            osal_msleep(SLEEP_5000);
            // 清理环境
            status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case NORMAL_TEST_3: // 永久等待,等待过程中删除
            test_suite_log_stringf("wait msg forever, please send msg\r\n");
            time_out = TD_SYS_WAIT_FOREVER;
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }

            g_threadid = osThreadNew(cmsis_msg_wait_forver_body, &g_mq_id[0], &thread_attr);
            osDelay(SLEEP_10);
            test_suite_log_stringf("g_threadid = %d\r\n", g_threadid);
            if (g_threadid == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            osal_msleep(SLEEP_10000);
            status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(status, osErrorResource, TD_TRUE, &test_result);

            status = osMessageQueuePut(g_mq_id[0], &stmsg, msg_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            osal_msleep(SLEEP_5000);
            // 清理环境
            status = osMessageQueueDelete(g_mq_id[0]);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            mil_sec = uapi_tcxo_get_ms();
            status = osMessageQueueGet(g_mq_id[0], &get_msge, &get_prio, time_out);
            mil_sec1 = uapi_tcxo_get_ms();
            wait_time = mil_sec1 - mil_sec;
            check_test_result(status, osErrorTimeout, TD_TRUE, &test_result);
            test_suite_log_stringf("get_msge.msg_id = 0x%x\r\n", get_msge.msg_id);
            test_suite_log_stringf("status = %d\r\n", status);
            test_suite_log_stringf("mil_sec = %d, mil_sec1 =%d, wait_time=%d\r\n", mil_sec, mil_sec1, wait_time);
            break;
        case ABNORMAL_TEST_1: // 不存在的ID
            status = osMessageQueueGet(&mq_id, &get_msge, &get_prio, time_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST_2:
            time_out = 0;
            status = osMessageQueueGet(&mq_id, &get_msge, &get_prio, time_out);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
            test_suite_log_stringf(" g_mq_id[0] = %d\r\n", g_mq_id[0]);
            if (g_mq_id[0] == NULL) {
                test_suite_log_stringf("osMessageQueueNew Fail");
                test_result = TEST_SUITE_TEST_FAILED;
            }
            status = osMessageQueuePut(g_mq_id[0], (td_pvoid)&stmsg, msg_prio, time_out);
            check_test_result(status, osOK, TD_TRUE, &test_result);

            status = osMessageQueueGet(NULL, &get_msge, &get_prio, time_out);
            test_suite_log_stringf("status = %d, get_prio = %d\r\n", status, get_prio);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);

            status = osMessageQueueGet(g_mq_id[0], NULL, &get_prio, time_out);
            test_suite_log_stringf("status = %d, get_prio = %d\r\n", status, get_prio);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);

            status = osMessageQueueGet(g_mq_id[0], &get_msge, NULL, time_out);
            test_suite_log_stringf("status = %d, get_prio = %d\r\n", status, get_prio);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

int cts_os_messagequeuegetcapacity(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    osStatus_t status = osOK;
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    mq_id.queueState = 0;
    uint32_t get_capacity = 0;
    ext_os_resource_use_stat os_resource = {0};
    uint32_t sys_msg_num = 0;
    uint32_t create_cnt = 1;
    uint32_t msg_count = 0;
    uint32_t msg_size = PARAM_10;
    switch (param) {
        case NORMAL_TEST:
            get_capacity = osMessageQueueGetCapacity(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d, get_capacity = %d\r\n", g_mq_id[0], get_capacity);
            check_test_result(get_capacity, 0, TD_FALSE, &test_result);
            break;
        case ABNORMAL_TEST:
            get_capacity = osMessageQueueGetCapacity(&mq_id);
            test_suite_log_stringf("get_capacity = %d\r\n", get_capacity);
            check_test_result(get_capacity, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            get_capacity = osMessageQueueGetCapacity(NULL);
            test_suite_log_stringf("get_capacity = %d\r\n", get_capacity);
            check_test_result(get_capacity, 0, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST:
            sys_msg_num = os_resource.queue_usage;
            test_suite_log_stringf("queue_usage = %d\r\n", sys_msg_num);
            for (create_cnt = 1; create_cnt <= PARAM_32 - sys_msg_num; create_cnt++) {
                msg_count = create_cnt + PARAM_50;
                g_mq_id[create_cnt] = osMessageQueueNew(msg_count, msg_size, NULL);
                test_suite_log_stringf("create_cnt =%d,g_mq_id[create_cnt] =%d\r\n", create_cnt, g_mq_id[create_cnt]);
                if (g_mq_id[create_cnt] == NULL) {
                    test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }
            osal_msleep(SLEEP_100);
            create_cnt --;
            while (create_cnt) {
                get_capacity = osMessageQueueGetCapacity(g_mq_id[create_cnt]);
                test_suite_log_stringf("create_cnt = %d, g_mq_id[create_cnt] = %d, get_capacity = %d\r\n", create_cnt,
                                       g_mq_id[create_cnt], get_capacity);
                check_test_result(get_capacity, (create_cnt + PARAM_50), TD_TRUE, &test_result);

                status = osMessageQueueDelete(g_mq_id[create_cnt]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                osal_msleep(SLEEP_20);
                create_cnt--;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

int cts_os_messagequeuegetmsgsize(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t get_size = 0;
    uint32_t msg_size = 0;
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    mq_id.queueState = 0;
    ext_os_resource_use_stat os_resource = {0};
    uint32_t sys_msg_num = 0;
    uint32_t create_cnt = 1;
    uint32_t msg_count = PARAM_10;
    osStatus_t status;
    switch (param) {
        case NORMAL_TEST:
            get_size = osMessageQueueGetMsgSize(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d, Msgsize = %d\r\n", g_mq_id[0], get_size);
            check_test_result(get_size, 0, TD_FALSE, &test_result);
            break;
        case ABNORMAL_TEST:
            get_size = osMessageQueueGetMsgSize(&mq_id);
            test_suite_log_stringf("get_size = %d\r\n", get_size);
            check_test_result(get_size, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            get_size = osMessageQueueGetMsgSize(NULL);
            test_suite_log_stringf("get_size = %d\r\n", get_size);
            check_test_result(get_size, 0, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST:
            sys_msg_num = os_resource.queue_usage;
            test_suite_log_stringf("queue_usage = %d\r\n", sys_msg_num);
            for (create_cnt = 1; create_cnt <= PARAM_32 - sys_msg_num; create_cnt++) {
                msg_size = create_cnt + PARAM_50;
                g_mq_id[create_cnt] = osMessageQueueNew(msg_count, msg_size, NULL);
                test_suite_log_stringf("create_cnt = %d,g_mq_id[create_cnt]=%d\r\n", create_cnt, g_mq_id[create_cnt]);
                if (g_mq_id[create_cnt] == NULL) {
                    test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }
            osal_msleep(SLEEP_100);
            create_cnt --;
            while (create_cnt) {
                get_size = osMessageQueueGetMsgSize(g_mq_id[create_cnt]);
                test_suite_log_stringf("create_cnt = %d, g_mq_id[create_cnt] = %d, get_size = %d\r\n", create_cnt,
                                       g_mq_id[create_cnt], get_size);
                check_test_result(get_size, (create_cnt + PARAM_50), TD_TRUE, &test_result);

                status = osMessageQueueDelete(g_mq_id[create_cnt]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                osal_msleep(SLEEP_20);
                create_cnt--;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

int cts_os_messagequeuegetcount(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t get_count = 0;
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    mq_id.queueState = 0;
    switch (param) {
        case NORMAL_TEST:
            get_count = osMessageQueueGetCount(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d, get_count = %d\r\n", g_mq_id[0], get_count);
            check_test_result(get_count, 0, TD_FALSE, &test_result);
            break;
        case ABNORMAL_TEST:
            get_count = osMessageQueueGetCount(&mq_id);
            test_suite_log_stringf("get_count = %d\r\n", get_count);
            check_test_result(get_count, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            get_count = osMessageQueueGetCount(NULL);
            test_suite_log_stringf("get_count = %d\r\n", get_count);
            check_test_result(get_count, 0, TD_TRUE, &test_result);
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }
    return test_result;
}

int cts_os_messagequeuegetspace(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;
    uint32_t get_space = 0;
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    mq_id.queueState = 0;
    osStatus_t status;
    ext_os_resource_use_stat os_resource = {0};
    uint32_t sys_msg_num = 0;
    uint32_t create_cnt = 1;
    uint32_t msg_count = PARAM_10;
    uint32_t msg_size = PARAM_10;
    switch (param) {
        case NORMAL_TEST:
            get_space = osMessageQueueGetSpace(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d ,get_space = %d\r\n", g_mq_id[0], get_space);
            check_test_result(get_space, 0, TD_FALSE, &test_result);
            break;
        case ABNORMAL_TEST:
            get_space = osMessageQueueGetSpace(&mq_id);
            test_suite_log_stringf("get_space = %d\r\n", get_space);
            check_test_result(get_space, 0, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            get_space = osMessageQueueGetSpace(NULL);
            test_suite_log_stringf("get_space = %d\r\n", get_space);
            check_test_result(get_space, 0, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST:
            sys_msg_num = os_resource.queue_usage;
            test_suite_log_stringf("queue_usage = %d\r\n", sys_msg_num);
            for (create_cnt = 1; create_cnt <= PARAM_32 - sys_msg_num; create_cnt++) {
                msg_count = create_cnt + PARAM_50;
                g_mq_id[create_cnt] = osMessageQueueNew(msg_count, msg_size, NULL);
                test_suite_log_stringf("create_cnt = %d,g_mq_id[create_cnt]=%d\r\n", create_cnt, g_mq_id[create_cnt]);
                if (g_mq_id[create_cnt] == NULL) {
                    test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }
            }
            osal_msleep(SLEEP_100);
            create_cnt --;
            while (create_cnt) {
                get_space = osMessageQueueGetSpace(g_mq_id[create_cnt]);
                test_suite_log_stringf("create_cnt = %d, g_mq_id[create_cnt] = %d, get_space = %d\r\n", create_cnt,
                                       g_mq_id[create_cnt], get_space);
                check_test_result(get_space, (create_cnt + PARAM_50), TD_TRUE, &test_result);

                status = osMessageQueueDelete(g_mq_id[create_cnt]);
                check_test_result(status, osOK, TD_TRUE, &test_result);
                osal_msleep(SLEEP_20);
                create_cnt--;
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

int cts_os_messagequeuedelete(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_suite_log_stringf("***test case : %s***\r\n", argv[0]);
    uint32_t param = strtol(argv[0], NULL, 0);
    uint32_t test_result = TEST_SUITE_OK;

    osStatus_t status = osOK;
    LosQueueCB mq_id;
    mq_id.queueId = PARAM_100;
    ext_mdm_mem_info mem_info = {0};
    uint8_t msge[PARAM_2] = {0x11, 0x22};
    uint32_t msg_count = PARAM_5;
    uint32_t msg_size = sizeof(msge);
    uint8_t get_msge[PARAM_2] = {0};
    uint8_t msg_prio = PARAM_10;
    uint8_t get_prio = 0;
    uint32_t time_out = PARAM_10000;
    osMessageQueueAttr_t  attr;
    attr.name = "test_osSemaphoreAcquire";
    attr.attr_bits  = STACK_SIZE_1;
    attr.cb_size = STACK_SIZE_1;
    switch (param) {
        case NORMAL_TEST:
            status = osMessageQueueDelete(g_mq_id[0]);
            test_suite_log_stringf("g_mq_id[0] = %d, del_status = %d\r\n", g_mq_id[0], status);
            check_test_result(status, osOK, TD_TRUE, &test_result);
            break;
        case ABNORMAL_TEST:
            status = osMessageQueueDelete(&mq_id);
            test_suite_log_stringf("del_status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case NULL_POINTER_TEST:
            status = osMessageQueueDelete(NULL);
            test_suite_log_stringf("del_status = %d\r\n", status);
            check_test_result(status, osErrorParameter, TD_TRUE, &test_result);
            break;
        case PRESSURE_TEST:
            test_suite_log_stringf("before delete ,mem total size = %d,mem free size = %d \r\n", mem_info.total,
                                   mem_info.free);
            for (int i = 0; i < PARAM_10000; i++) {
                g_mq_id[0] = osMessageQueueNew(msg_count, msg_size, &attr);
                test_suite_log_stringf(" g_mq_id[0] = %d\r\n", g_mq_id[0]);
                if (g_mq_id[0] == NULL) {
                    test_suite_log_stringf("osMessageQueueNew Fail\r\n");
                    test_result = TEST_SUITE_TEST_FAILED;
                }

                status = osMessageQueuePut(g_mq_id[0], &msge, msg_prio, time_out);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osMessageQueueGet(g_mq_id[0], &get_msge, &get_prio, time_out);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                status = osMessageQueueDelete(g_mq_id[0]);
                check_test_result(status, osOK, TD_TRUE, &test_result);

                if (i % PARAM_1000 == 0) {
                    test_suite_log_stringf("num = %d,----mem total size = %d,------mem free size = %d \r\n", i,
                                           mem_info.total, mem_info.free);
                }
            }
            break;
        default:
            check_test_result(TD_TRUE, TD_FALSE, TD_TRUE, &test_result);
            break;
    }

    return test_result;
}

void add_cmsis_test_case(void)
{
    uapi_test_suite_add_function("cts_osMutexNew", "cts_osMutexNew test.", cts_os_mutexnew);
    uapi_test_suite_add_function("cts_osMutexAcquire", "cts_osMutexAcquire test.", cts_os_mutexacquire);
    uapi_test_suite_add_function("cts_osMutexRelease", "cts_osMutexRelease test.", cts_os_mutexrelease);
    uapi_test_suite_add_function("cts_osMutexGetOwner", "cts_osMutexGetOwner test.", cts_os_mutexgetowner);
    uapi_test_suite_add_function("cts_osMutexDelete", "cts_osMutexDelete test.", cts_os_mutexdelete);

    uapi_test_suite_add_function("cts_osEventFlagsNew", "cts_osEventFlagsNew test.", cts_os_eventflagsnew);
    uapi_test_suite_add_function("cts_osEventFlagsSet", "cts_osEventFlagsSet test.", cts_os_eventflagsset);
    uapi_test_suite_add_function("cts_osEventFlagsClear", "cts_osEventFlagsClear test.", cts_os_eventflagsclear);
    uapi_test_suite_add_function("cts_osEventFlagsGet", "cts_osEventFlagsGet test.", cts_os_eventflagsget);
    uapi_test_suite_add_function("cts_osEventFlagsWait", "cts_osEventFlagsWait test.", cts_os_eventflagswait);
    uapi_test_suite_add_function("cts_osEventFlagsDelete", "cts_osEventFlagsDelete test.", cts_os_eventflagsdelete);

    uapi_test_suite_add_function("cts_osKernelInitialize", "cts_osKernelInitialize test.", cts_os_kernel_initialize);
    uapi_test_suite_add_function("cts_osKernelGetInfo", "cts_osKernelGetInfo test.", cts_os_kernel_getinfo);
    uapi_test_suite_add_function("cts_osKernelGetState", "cts_osKernelGetState test.", cts_os_kernel_getstate);
    uapi_test_suite_add_function("cts_osKernelStart", "cts_osKernelStart test.", cts_os_kernel_start);
    uapi_test_suite_add_function("cts_osKernelLock", "cts_osKernelLock test.", cts_os_kernel_lock);
    uapi_test_suite_add_function("cts_osKernelUnlock", "cts_osKernelUnlock test.", cts_os_kernel_unlock);
    uapi_test_suite_add_function(
        "cts_osKernelGetTickCount", "cts_osKernelGetTickCount test.", cts_os_kernel_gettickcount);
    uapi_test_suite_add_function("cts_osKernelGetTickFreq", "cts_osKernelGetTickFreq test.", cts_os_kernel_gettickfreq);
    uapi_test_suite_add_function(
        "cts_osKernelGetSysTimerCount", "cts_osKernelGetSysTimerCount test.", cts_os_kernel_getsystimercount);
    uapi_test_suite_add_function(
        "cts_osKernelGetSysTimerFreq", "cts_osKernelGetSysTimerFreq test.", cts_os_kernel_getsystimerfreq);

    uapi_test_suite_add_function("cts_osThreadNew", "cts_osThreadNew test.", cts_os_threadnew);
    uapi_test_suite_add_function("cts_osThreadGetName", "cts_osThreadGetName test.", cts_os_threadgetname);
    uapi_test_suite_add_function("cts_osThreadGetId", "cts_osThreadGetId test.", cts_os_threadgetid);
    uapi_test_suite_add_function("cts_osThreadGetState", "cts_osThreadGetState test.", cts_os_threadgetstate);
    uapi_test_suite_add_function(
        "cts_osThreadGetStackSize", "cts_osThreadGetStackSize test.", cts_os_threadgetstacksize);
    uapi_test_suite_add_function(
        "cts_osThreadGetStackSpace", "cts_osThreadGetStackSpace test.", cts_os_threadgetstackspace);
    uapi_test_suite_add_function("cts_osThreadSetPriority", "cts_osThreadSetPriority test.", cts_os_threadsetpriority);
    uapi_test_suite_add_function("cts_osThreadGetPriority", "cts_osThreadGetPriority test.", cts_os_threadgetpriority);
    uapi_test_suite_add_function("cts_osThreadYield", "cts_osThreadYield test.", cts_os_threadyield);
    uapi_test_suite_add_function("cts_osThreadSuspend", "cts_osThreadSuspend test.", cts_os_threadsuspend);
    uapi_test_suite_add_function("cts_osThreadResume", "cts_osThreadResume test.", cts_os_threadresume);
    uapi_test_suite_add_function("cts_osThreadTerminate", "cts_osThreadTerminate test.", cts_os_threadterminate);
    uapi_test_suite_add_function("cts_osThreadGetCount", "cts_osThreadGetCount test.", cts_os_threadgetcount);

    uapi_test_suite_add_function("cts_osDelay", "cts_osDelay test.", cts_os_delay);
    uapi_test_suite_add_function("cts_osDelayUntil", "cts_osDelayUntil test.", cts_os_delayuntil);

    uapi_test_suite_add_function("cts_osTimerNew", "cts_osTimerNew test.", cts_os_timernew);
    uapi_test_suite_add_function("cts_osTimerStart", "cts_osTimerStart test.", cts_os_timerstart);
    uapi_test_suite_add_function("cts_osTimerStop", "cts_osTimerStop test.", cts_os_timerstop);
    uapi_test_suite_add_function("cts_osTimerIsRunning", "cts_osTimerIsRunning test.", cts_os_timerisrunning);
    uapi_test_suite_add_function("cts_osTimerDelete", "cts_osTimerDelete test.", cts_os_timerdelete);

    uapi_test_suite_add_function("cts_osSemaphoreNew", "cts_osSemaphoreNew test.", cts_os_semaphorenew);
    uapi_test_suite_add_function("cts_osSemaphoreAcquire", "cts_osSemaphoreAcquire test.", cts_os_semaphoreacquire);
    uapi_test_suite_add_function("cts_osSemaphoreRelease", "cts_osSemaphoreRelease test.", cts_os_semaphorerelease);
    uapi_test_suite_add_function("cts_osSemaphoreGetCount", "cts_osSemaphoreGetCount test.", cts_os_semaphoregetcount);
    uapi_test_suite_add_function("cts_osSemaphoreDelete", "cts_osSemaphoreDelete test.", cts_os_semaphoredelete);

    uapi_test_suite_add_function("cts_osMessageQueueNew", "cts_osMessageQueueNew test.", cts_os_messagequeuenew);
    uapi_test_suite_add_function("cts_osMessageQueuePut", "cts_osMessageQueuePut test.", cts_os_messagequeueput);
    uapi_test_suite_add_function("cts_osMessageQueueGet", "cts_osMessageQueueGet test.", cts_os_messagequeueget);
    uapi_test_suite_add_function(
        "cts_osMessageQueueGetCapacity", "cts_osMessageQueueGetCapacity.", cts_os_messagequeuegetcapacity);
    uapi_test_suite_add_function(
        "cts_osMessageQueueGetMsgSize", "cts_osMessageQueueGetMsgSize test.", cts_os_messagequeuegetmsgsize);
    uapi_test_suite_add_function(
        "cts_osMessageQueueGetCount", "cts_osMessageQueueGetCount test.", cts_os_messagequeuegetcount);
    uapi_test_suite_add_function(
        "cts_osMessageQueueGetSpace", "cts_osMessageQueueGetSpace test.", cts_os_messagequeuegetspace);
    uapi_test_suite_add_function(
        "cts_osMessageQueueDelete", "cts_osMessageQueueDelete test.", cts_os_messagequeuedelete);
}