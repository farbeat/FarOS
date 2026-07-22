/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 * Description: Test reboot \n
 * Author: @CompanyNameTag \n
 */

#include "test_reboot.h"
#include "non_os_reboot.h"
#include "debug_print.h"
#include "test_suite.h"

/* ---------------------------------延时复位--------------------------------- */

/* 延时原因--异常值id测试 */
static int delay_reboot_test_id_001(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t ret = uapi_reboot_vote_against(0xff, 300);
    if (ret == ERRCODE_SUCC) {
        PRINT("delay reboot register success\r\n");
    } else {
        PRINT("delay reboot register failed\r\n");
    }
    return ERRCODE_SUCC;
}

/* 延时重启,基本功能测试 */
static int delay_reboot_test_id_002(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t ret = uapi_reboot_vote_against(VETO_REBOOT_REASON_SUIT_NET, 300);
    if (ret == ERRCODE_SUCC) {
        PRINT("delay reboot regdister success\r\n");
    } else {
        PRINT("delay reboot regdister failed\r\n");
    }

    return ERRCODE_SUCC;
}

/* 去除延时 */
static int remove_delay(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uapi_reboot_unvote_against(VETO_REBOOT_REASON_SUIT_NET);
    return ERRCODE_SUCC;
}

/* 延时复位, 延时原因边界测试 */
static int delay_reboot_boundary_001(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t ret = uapi_reboot_vote_against(VETO_REBOOT_REASON_WR_NV, 300);
    if (ret == ERRCODE_SUCC) {
        PRINT("delay reboot regdister success\r\n");
    } else {
        PRINT("delay reboot regdister failed\r\n");
    }

    return ERRCODE_SUCC;
}

/* 延时复位, 延时原因边界测试 */
static int delay_reboot_boundary_002(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t ret = uapi_reboot_vote_against(VETO_REBOOT_REASON_RELATING_RANDOM_NUM, 300);
    if (ret == ERRCODE_SUCC) {
        PRINT("delay reboot regdister success\r\n");
    } else {
        PRINT("delay reboot regdister failed\r\n");
    }
    return ERRCODE_SUCC;
}

static int test_reboot_check_vote(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uapi_reboot_system_check_veto(REBOOT_CAUSE_APPLICATION_SYSRESETREQ);
    PRINT("excute uapi_reboot_system_check_veto\r\n");
    return ERRCODE_SUCC;
}

void add_test_reboot(void)
{
    uapi_test_suite_add_function("delay_reboot_test_id_001", "delay reason is 0xff", delay_reboot_test_id_001);
    uapi_test_suite_add_function("delay_reboot_test_id_002", "delay 3 min", delay_reboot_test_id_002);
    uapi_test_suite_add_function("remove_delay", "remove delay regedist", remove_delay);
    uapi_test_suite_add_function("delay_reboot_boundary_001", "delay reason boundary test", delay_reboot_boundary_001);
    uapi_test_suite_add_function("delay_reboot_boundary_002", "delay reason boundary test", delay_reboot_boundary_002);
    uapi_test_suite_add_function("reboot_check_veto", "reboot_check_veto", test_reboot_check_vote);
}
