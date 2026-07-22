/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test touch source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-09, Create file. \n
 */
#include "test_touch.h"
#include "tcxo.h"
#include "touch_screen_def.h"
#include "ztw523_touch1.h"
#include "ztw523_ctrl.h"
#include "test_suite.h"
#include "test_suite_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TP_READ_DATA_LEN 2
#define ZTW_SAMPLE_TASK_PRIO  5
#define ZTW_SAMPLE_TASK_SIZE  0x1000
#define ZTW523_TIMEOUT        100
#define ZTW523_FILE_MODE      0777
#define ZTW523_IRQ_CLR_REG    0x56002020

static void ztw523_drv_case(input_event_info touch_info)
{
    uint16_t ret = 0;
    switch (touch_info.tp_event) {
        case MC_TP_PRESS:
            test_suite_log_stringf("touch event press!\r\n");
            break;
        case MC_TP_RELEASE:
            test_suite_log_stringf("touch event release!\r\n");
            break;
        case MC_TP_MOVE:
            test_suite_log_stringf("touch event move!\r\n");
            break;
        case MC_TP_COVER:
            test_suite_log_stringf("touch event cover!\r\n");
            ret = ztw523_set_power_mode(MC_TP_STANDBY_WORK_MODE);
            if (ret != 0) {
                test_suite_log_stringf("set standy mode fail! ret = 0x%x\r\n", ret);
            }
            break;
        case MC_TP_SHORT_CLICK:
            test_suite_log_stringf("gesture event single click!\r\n");
            ret = ztw523_resume();
            if (ret != 0) {
                test_suite_log_stringf("exit standy mode fail! ret = 0x%x\r\n", ret);
            }
            break;
        case MC_TP_DOUBLE_CLICK:
            test_suite_log_stringf("gesture event double click!\r\n");
            break;
        case MC_TP_SLIDE_UP:
            test_suite_log_stringf("gesture event slide up!\r\n");
            break;
        case MC_TP_SLIDE_DOWN:
            test_suite_log_stringf("gesture event slide down!\r\n");
            break;
        default:
            break;
    }
}

static void *ztw523_drv_task(void)
{
    input_event_info touch_info;
    uint16_t ret = 0;

    ret = (uint16_t)ztw523_irq_callback((uint8_t *)(&touch_info), sizeof(input_event_info));
    if (ret != 0) {
        test_suite_log_stringf("ztw523 get touch info fail! ret=0x%x", ret);
    }
    ztw523_drv_case(touch_info);
    if ((touch_info.tp_event >= MC_TP_PRESS) && (touch_info.tp_event < MC_TP_COVER)) {
        test_suite_log_stringf("x_axis: %d\r\n", touch_info.tp_event_info.x_axis[0]);
        test_suite_log_stringf("y_axis: %d\r\n", touch_info.tp_event_info.y_axis[0]);
        test_suite_log_stringf("tv_usec: %lld\r\n", touch_info.tv_usec);
    }

    return NULL;
}

static void touch_screen_sample_hal(void)
{
    uint16_t ret = ts_init();
    ztw523_host_init_sample();
    ts_register_report_event_cb((ReportEventCb)ztw523_drv_task);
    if (ret != 0) {
        test_suite_log_stringf("ztw523_sample_init fail! ret=0x%x\r\n", ret);
    }
}

static uint16_t tp_i2c_w4(int argc, char* argv[])
{
    uint16_t start_addr;
    uint16_t write_val;
    uint16_t read_val;
    ext_errno ret;
    uint16_t rdlen = 1;
    if (argc != 2) {
        test_suite_log_stringf("format: test tp_i2c_w4 [start_addr] [val]\r\n");
        return 0;
    }

    start_addr = (uint16_t)strtoul((const char*)argv[0], NULL, 16);
    write_val = (uint16_t)strtoul((const char*)argv[1], NULL, 16);

    ret = tp_i2c_reg_write(start_addr, write_val);
    if (ret != 0) {
        test_suite_log_stringf("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    test_suite_log_stringf("Write [ 0x%x ] : [ 0x%x ]\r\n", start_addr, write_val);

    return 0;
}

static uint16_t tp_i2c_mem32(int argc, char* argv[])
{
    uint16_t start_addr;
    uint16_t read_val = 0;
    uint16_t ret;
    uint16_t rdlen = 0;
    if (argc != 1) {
        test_suite_log_stringf("format: test tp_i2c_w4 [start_addr] [val]\r\n");
        return 0;
    }

    start_addr = (uint16_t)strtoul((const char*)argv[0], NULL, 16);

    ret = tp_i2c_data_read(start_addr, &read_val, TP_READ_DATA_LEN);
    if (ret != 0) {
        test_suite_log_stringf("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    test_suite_log_stringf("Read [ 0x%x ] : [ 0x%x ]\r\n", start_addr, read_val);

    return 0;
}

static uint16_t tp_i2c_send_cmd(int argc, char* argv[])
{
    uint16_t start_addr;
    uint16_t write_val;
    uint16_t read_val;
    uint16_t ret, cmd;
    uint16_t rdlen = 1;
    if (argc != 1) {
        test_suite_log_stringf("format: test tp_i2c_w4 [start_addr] [val]\r\n");
        return 0;
    }

    cmd = (uint16_t)strtoul((const char*)argv[0], NULL, 16);

    ret = tp_i2c_cmd_write(cmd);
    if (ret != 0) {
        test_suite_log_stringf("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }
    test_suite_log_stringf("send cmd ok ^_^\r\n");

    return 0;
}

void add_test_touch_case(void)
{
    uapi_test_suite_add_function("touch_screen_hal", "touch_screen_hal", touch_screen_sample_hal);
    uapi_test_suite_add_function("tp_i2c_w4", "tp_i2c_w4 <start_addr> <val>", tp_i2c_w4);
    uapi_test_suite_add_function("tp_i2c_mem32", "tp_i2c_mem32 <start_addr>", tp_i2c_mem32);
    uapi_test_suite_add_function("tp_i2c_send_cmd", "tp_i2c_send_cmd <cmd>", tp_i2c_send_cmd);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif