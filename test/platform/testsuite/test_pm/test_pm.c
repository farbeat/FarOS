/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test pm source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#include "test_pm.h"
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "lpc_core.h"
#include "cmsis_os2.h"
#include "securec.h"
#include "pm_sleep.h"
#include "pm_lpc.h"
#include "pmu_interrupt.h"
#include "pmu.h"
#include "ulp_aon_control.h"
#include "hal_pmu_ldo.h"
#include "clocks_core.h"
#include "los_status.h"
#include "oal_interface.h"
#include "rtc.h"

static void print_app_system_sleep_info(void)
{
    uint32_t deepsleep_time, lightsleep_time, deepsleep_pg_time;
    uint32_t deepsleep_cnt, lightsleep_cnt, deepsleep_pg_cnt;
    sleep_info_t *sleep_info = pm_get_sleep_info();

    deepsleep_time = sleep_info->sleep_history[PM_SLEEP_DS].total_slp_time;
    deepsleep_cnt = sleep_info->sleep_history[PM_SLEEP_DS].total_slp_count;

    lightsleep_time = sleep_info->sleep_history[PM_SLEEP_LS].total_slp_time;
    lightsleep_cnt = sleep_info->sleep_history[PM_SLEEP_LS].total_slp_count;

    bool m_core_sleep = pmu_msystem_sleep_history();

    pmu_msystem_sleep_history_clear();
    PRINT("M core deepsleep time:%d, light time:%d, m sleep flag:%d\r\n",
        deepsleep_time, lightsleep_time, (uint8_t)m_core_sleep);
    PRINT("M core deepsleep cnt:%d, light cnt:%d\r\n", deepsleep_cnt, lightsleep_cnt);
    PRINT("M core last veto id:%d, last veto count:%d\r\n", sleep_info->sleep_veto.last_veto_id,
        sleep_info->sleep_veto.last_veto_count);
}

static int tesetsuit_m_sys_enter_lightsleep(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_pm_add_sleep_veto(PM_ID_DEBUG);
    osDelay(1000); // 延时1000ms后，查询状态
    print_app_system_sleep_info();
    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_enter_deepsleep(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uapi_pm_remove_sleep_veto(PM_ID_DEBUG);
    osDelay(1000); // 延时1000ms后，查询状态
    print_app_system_sleep_info();

    return TEST_SUITE_OK;
}

#define PMU_CLDO1_VSET_AUTO_REG                 (PMU1_CTL_RB_BASE + 0x894)
#define PMU_CLDO2_VSET_AUTO_REG                 (PMU1_CTL_RB_BASE + 0x898)
#define PMU_CLDO3_VSET_AUTO_REG                 (PMU1_CTL_RB_BASE + 0x89c)
static int tesetsuit_m_sys_auto_vset(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint8_t ldo = (uint8_t)strtol(argv[0], NULL, 0);
    if (ldo > 3 || ldo == 0) { // valid reg:1 ~ 3
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t ldo_reg = PMU_CLDO1_VSET_AUTO_REG + 4 * (ldo - 1);

    // 实际电压状态需通过示波器观测，默认开启
    uint16_t val = readw(ldo_reg);
    PRINT("1.Reg: 0x%x, Val: %d\r\n", ldo_reg, val);
    if (val != 0) {
        PRINT("CLDO%d AUTO VSET ENABLE\r\n", ldo);
    } else {
        writew(ldo_reg, 1);
        PRINT("CLDO%d AUTO VSET ENABLE now\r\n", ldo);
    }
    val = readw(ldo_reg);
    PRINT("2.Reg: 0x%x, Val: %d\r\n", ldo_reg, val);
    return TEST_SUITE_OK;
}

#define PMU_MCU_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x880)
#define PMU_BSOC_AUTO_PG_REG               (PMU1_CTL_RB_BASE + 0X884)
#define PMU_COM_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x888)
#define PMU_MEM_AUTO_PG_REG                (PMU1_CTL_RB_BASE + 0x88C)
#define PMU_BMEM_AUTO_PG_REG               (PMU1_CTL_RB_BASE + 0x890)
static int tesetsuit_m_sys_auto_powergating(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    // SUB1~5: MUC\BSOC\COM\MEM\BMEM
    uint8_t sub = (uint8_t)strtol(argv[0], NULL, 0);
    if (sub > 5 || sub == 0) { // valid reg:1 ~ 5
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t pg_reg = PMU_MCU_AUTO_PG_REG + 4 * (sub - 1);
    // 实际电压状态需通过示波器观测，默认开启
    uint16_t val = readw(pg_reg);
    PRINT("1.Reg: 0x%x, Val: %d\r\n", pg_reg, val);
    if (val != 0) {
        PRINT("SUB%d AUTO PG ENABLE\r\n", sub);
    } else {
        writew(pg_reg, 1);
        PRINT("SUB%d AUTO PG ENABLE now\r\n", sub);
    }
    val = readw(pg_reg);
    PRINT("2.Reg: 0x%x, Val: %d\r\n", pg_reg, val);
    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_sleep_status(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    print_app_system_sleep_info();

    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_sleep_wkup_event(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    sleep_info_t *sleep_info = pm_get_sleep_info();
    if (sleep_info->event.slp_event != 0) {
        PRINT("Sleep event has occurred\r\n");
    } else {
        PRINT("Sleep event has not occurred\r\n");
    }

    if (sleep_info->event.wkup_event != 0) {
        PRINT("Wkup event has occurred\r\n");
    } else {
        PRINT("Wkup event has not occurred\r\n");
    }

    return TEST_SUITE_OK;
}

#define TEST_WAKEUP_M_PC_L            (GLB_CTL_M_RB_BASE + 0x4)
#define TEST_WAKEUP_M_PC_H            (GLB_CTL_M_RB_BASE + 0x8)
static int tesetsuit_m_sys_wkup_addr(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uint32_t addr = 0;
    addr = readw(TEST_WAKEUP_M_PC_L) | ((uint32_t)readw(TEST_WAKEUP_M_PC_H) << 16); // shift 16bit
    PRINT("Current wkup addr: 0x%x\r\n", addr);
    print_app_system_sleep_info();
    return TEST_SUITE_OK;
}

rtc_handle_t g_test_rtc_handle = NULL;
uint32_t g_test_rtc_timeout_s = 0;
static void test_sleep_rtc_callback(void)
{
    PRINT("[Test]RTC wkup! last sleep time: %ds\r\n", g_test_rtc_timeout_s);
    uapi_rtc_stop(g_test_rtc_handle);
    uapi_rtc_delete(g_test_rtc_handle);
    g_test_rtc_timeout_s = 0;
}

static int tesetsuit_m_sys_wkup_by_rtc(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    uint16_t wakeup_time = 5000; // set 5000ms timeout;
    uapi_rtc_create(RTC_0, &g_test_rtc_handle);
    uapi_rtc_start(g_test_rtc_handle, wakeup_time, (rtc_callback_t)test_sleep_rtc_callback, 0);
    g_test_rtc_timeout_s = wakeup_time / 1000; // 1000ms per s
    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_veto_test(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    errcode_t ret = 0;
    uint8_t veto_type = (uint8_t)strtol(argv[0], NULL, 0);
    switch (veto_type) {
        case 1: // 1:add veto
            ret = uapi_pm_add_sleep_veto(PM_ID_DEBUG);
            break;
        case 2: // 2:add timeout veto
            ret = uapi_pm_add_sleep_veto_with_timeout(PM_ID_DEBUG, 10000); // timeout 10000ms
            break;
        case 3: // 3:remove veto
            ret = uapi_pm_remove_sleep_veto(PM_ID_DEBUG);
            break;
        default:
            PRINT("Invalid veto type: %d\r\n", veto_type);
            break;
    }
    if (ret != ERRCODE_SUCC) {
        PRINT("Veto test fail! ret:%d, veto type: %d", ret, veto_type);
    } else {
        PRINT("Veto test success! ret:%d, veto type: %d", ret, veto_type);
    }

    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_sleep_threshold(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t ms = (uint32_t)strtol(argv[0], NULL, 0);
    pm_set_sleep_threshold(ms);
    PRINT("Sleep threshold set %dms!\r\n", ms);

    return TEST_SUITE_OK;
}

static void test_slp_ls_handler(void)
{
    PRINT("Sleep ls handler!\r\n");
}

static void test_slp_ds_handler(void)
{
    PRINT("Sleep ds handler!\r\n");
}

static void test_wkup_ls_handler(void)
{
    PRINT("Wkup ls handler!\r\n");
}

static void test_wkup_ds_handler(void)
{
    PRINT("Wkup ds handler!\r\n");
}

static int tesetsuit_m_sys_register_slp_cb(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    errcode_t ret = 0;
    pm_fsm_content_t content = {NULL, 0, NULL};
    content.handler = test_slp_ls_handler;
    ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_LIGHT_SLEEP, &content);
    content.handler = test_slp_ds_handler;
    ret |= uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_DEEP_SLEEP, &content);
    content.handler = test_wkup_ls_handler;
    ret |= uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_WKUP_FROM_LIGHT_SLEEP, &content);
    content.handler = test_wkup_ds_handler;
    ret |= uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_WKUP_FROM_DEEP_SLEEP, &content);
    if (ret == ERRCODE_SUCC) {
        PRINT("Sleep handler register success!\r\n");
    } else {
        PRINT("Sleep handler register fail! ret%d\r\n", ret);
    }
    return TEST_SUITE_OK;
}

static int tesetsuit_m_sys_sleep_stability(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t wakeup_time = 43200; // set 86400s -24h timeout;
    uapi_rtc_create(RTC_0, &g_test_rtc_handle);
    uapi_rtc_start(g_test_rtc_handle, wakeup_time, (rtc_callback_t)test_sleep_rtc_callback, 0);
    g_test_rtc_timeout_s = wakeup_time;
    while (g_test_rtc_timeout_s) {
        PRINT("timeout: %d\r\n", wakeup_time);
        osDelay(1000);     // 延迟1000ms
        wakeup_time -= 10;
        print_app_system_sleep_info();
    }
    return TEST_SUITE_OK;
}

static void m_sys_exeception_wkup_addr(void)
{
    // Register ds handle
    pm_fsm_content_t content = {NULL, 0, NULL};
    errcode_t ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_DEEP_SLEEP,  &content);
    if (ret == ERRCODE_SUCC) {
        PRINT("m_sys_exeception_wkup_addr: Set addr success!\r\n");
    } else {
        PRINT("m_sys_exeception_wkup_addr: Set addr fail! ret = %d\r\n", ret);
    }
}

static void m_sys_exeception_invalid_slp_handler(void)
{
    errcode_t ret = 0;
    ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_LIGHT_SLEEP, NULL);
    if (ret != ERRCODE_SUCC) {
        PRINT("Register sleep ls handler fail: ret = %d\r\n", ret);
    }
    ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_DEEP_SLEEP, NULL);
    if (ret != ERRCODE_SUCC) {
        PRINT("Register sleep ds handler fail: ret = %d\r\n", ret);
    }
    ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_WKUP_FROM_LIGHT_SLEEP, NULL);
    if (ret != ERRCODE_SUCC) {
        PRINT("Register wkup ls handler fail: ret = %d\r\n", ret);
    }
    ret = uapi_pm_register_fsm_handler(PM_ID_SYS, PM_STATE_WKUP_FROM_DEEP_SLEEP, NULL);
    if (ret != ERRCODE_SUCC) {
        PRINT("Register wkup ds handler fail: ret = %d\r\n", ret);
    }
}

static void m_sys_exeception_invalid_veto_param(void)
{
    errcode_t ret = 0;
    uint8_t veto_id = 100;
    // 添加无效id
    ret = uapi_pm_add_sleep_veto(veto_id);
    if (ret != ERRCODE_SUCC) {
        PRINT("Add sleep veto fail: ret = %d, veto_id = %d\r\n", ret, veto_id);
    } else {
        PRINT("Add sleep veto success: veto_id = %d\r\n", ret, veto_id);
    }
    // 移除无效id
    ret = uapi_pm_remove_sleep_veto(veto_id);
    if (ret != ERRCODE_SUCC) {
        PRINT("remove sleep veto fail: ret = %d, veto_id = %d\r\n", ret, veto_id);
    } else {
        PRINT("remove sleep veto success: veto_id = %d\r\n", ret, veto_id);
    }
    // 移除没有投票的有效id
    veto_id = 1;
    ret = uapi_pm_remove_sleep_veto(veto_id);
    if (ret != ERRCODE_SUCC) {
        PRINT("remove sleep veto fail: ret = %d, veto_id = %d\r\n", ret, veto_id);
    } else {
        PRINT("remove sleep veto success: veto_id = %d\r\n", ret, veto_id);
    }
}

static int tesetsuit_m_sys_exception_test(int argc, char *argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint8_t exception_case = (uint8_t)strtol(argv[0], NULL, 0);
    switch (exception_case) {
        case 1: // 1-无效唤醒地址
            m_sys_exeception_wkup_addr();
            break;
        case 2: // 2-无效回调函数
            m_sys_exeception_invalid_slp_handler();
            break;
        case 3: // 3-无效回调函数
            m_sys_exeception_invalid_veto_param();
            break;
        default:
            break;
    }

    return TEST_SUITE_OK;
}

void add_pm_test_case(void)
{
    uapi_test_suite_add_function(
        "m_enter_lightsleep", "MCPU lightsleep test",             tesetsuit_m_sys_enter_lightsleep);
    uapi_test_suite_add_function(
        "m_enter_deepsleep",  "MCPU deepsleep test",              tesetsuit_m_sys_enter_deepsleep);
    uapi_test_suite_add_function("m_sys_auto_vset", "MCPU enable vset auto config[1\2\3]", tesetsuit_m_sys_auto_vset);
    uapi_test_suite_add_function(
        "m_sys_auto_pg", "MCPU enable sub auto pg[1\2\3\4\5]",    tesetsuit_m_sys_auto_powergating);
    uapi_test_suite_add_function(
        "m_sys_sleep_status", "Get MCPU sleep status",            tesetsuit_m_sys_sleep_status);
    uapi_test_suite_add_function(
        "m_sys_event_test", "MCPU sleep evt& wkup evt test",      tesetsuit_m_sys_sleep_wkup_event);
    uapi_test_suite_add_function("m_sys_wkup_addr", "MCPU wakeup from resetpc test",       tesetsuit_m_sys_wkup_addr);
    uapi_test_suite_add_function("m_sys_wkup_by_rtc",  "MCPU wkup by rtc interrupt",       tesetsuit_m_sys_wkup_by_rtc);
    uapi_test_suite_add_function("m_sys_veto_test", "MCPU sleep veto interface test",      tesetsuit_m_sys_veto_test);
    uapi_test_suite_add_function(
        "m_sys_sleep_threshold", "MCPU set sleep threshold",      tesetsuit_m_sys_sleep_threshold);
    uapi_test_suite_add_function(
        "m_sys_register_slp_cb",  "MCPU register sleep&wkup handler", tesetsuit_m_sys_register_slp_cb);
    uapi_test_suite_add_function(
        "m_sys_sleep_stability", "MCPU stability test: sleep 24h",    tesetsuit_m_sys_sleep_stability);
    uapi_test_suite_add_function(
        "m_sys_exception_test", "MCPU exception test cases",          tesetsuit_m_sys_exception_test);
}
