/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include <stdlib.h>
#include "tcxo.h"
#include "soc_errno.h"
#include "soc_mipi_tx.h"
#include "soc_lcd_api.h"
#include "lcd_bus.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "debug_print.h"
#include "pinctrl_porting.h"
#include "test_lcd_api.h"

uint32_t g_te_isr_cnt = 0;
extern volatile uint32_t g_qspi_clk_div;

static int test_lcd_set_qspi_div(int argc, char *argv[])
{
    uint32_t div = strtol(argv[0], NULL, 0);

    g_qspi_clk_div = div;
    return TEST_SUITE_OK;
}

static int test_lcd_init(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uint32_t ret = uapi_lcd_init();
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Lcd init failed, ret = 0x%x!!\n", ret);
        return ret;
    }

    return TEST_SUITE_OK;
}

static int test_lcd_deinit(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    uint32_t ret = uapi_lcd_deinit();
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Lcd deinit failed, ret = 0x%x!!\n", ret);
        return ret;
    }

    return TEST_SUITE_OK;
}

static int test_lcd_is_connected(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    bool ret = uapi_is_lcd_connect();
    if (ret) {
        test_suite_log_stringf("Lcd screen is connect!!\n");
    } else {
        test_suite_log_stringf("No lcd screen is connect!!\n");
    }

    return TEST_SUITE_OK;
}

static int test_lcd_get_status(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    ext_lcd_status status = uapi_lcd_get_status();
    if (status.disp_status == LCD_NORMAL_DISPLAY) {
        test_suite_log_stringf("Local lcd status is 1 (normal display)\r\n");
    } else if (status.disp_status == LCD_IDLE_MODE) {
        test_suite_log_stringf("Local lcd status is 2 (IDLE mode)\r\n");
    } else if (status.disp_status == LCD_SLEEP_MODE) {
        test_suite_log_stringf("Local lcd status is 3 (SLEEP mode)\r\n");
    } else if (status.disp_status == LCD_DISPLAY_OFF) {
        test_suite_log_stringf("Local lcd status is 4 (display off)\r\n");
    } else {
        test_suite_log_stringf("lcd status invalid!!!\r\n");
    }

    if (status.te_status == LCD_TE_ON) {
        test_suite_log_stringf("lcd te on!!!\r\n");
    } else {
        test_suite_log_stringf("lcd te off!!!\r\n");
    }
    return TEST_SUITE_OK;
}

static int test_lcd_display_on(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    ext_errno ret = uapi_lcd_display_on();
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Set lcd screen display on succ!!\n");
    } else {
        test_suite_log_stringf("Failed to set lcd screen display on!!\n");
    }

    return ret;
}

static int test_lcd_display_off(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    ext_errno ret = uapi_lcd_display_off();
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Set lcd screen display off succ!!\n");
    } else {
        test_suite_log_stringf("Failed to set lcd screen display off!!\n");
    }

    return ret;
}

static int test_lcd_set_bright(int argc, char *argv[])
{
    uint16_t bright = 0;
    ext_errno ret;
    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    bright = (uint16_t)strtoul(argv[0], NULL, 16);
    test_suite_log_stringf("ready set bright to 0x%x\r\n", bright);
    ret = uapi_lcd_set_brightness(bright);
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Set lcd brightness level %d succ!!\n", bright);
    } else {
        test_suite_log_stringf("Failed to set lcd brightness!!\n");
    }

    return ret;
}

static int test_lcd_get_bright(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    ext_errno ret;
    uint16_t bright = 0;
    ret = uapi_lcd_get_brightness(&bright);
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Succ! brightness level: %d\n", bright);
    } else {
        test_suite_log_stringf("Failed to get lcd brightness!!\n");
    }

    return ret;
}

static int test_lcd_sleep(int argc, char *argv[])
{
    unsigned int ctrl;
    ext_errno ret = EXT_ERR_SUCCESS;
    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    ctrl = (unsigned int)strtol(argv[0], NULL, 0);
    if (ctrl != 0) {
        ret = uapi_lcd_enter_sleep_mode();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_enter_sleep_mode\n");
        } else {
            test_suite_log_stringf("Failed lcd_enter_sleep_mode!! ret = 0x%x\n", ret);
        }
    } else {
        ret = uapi_lcd_exit_sleep_mode();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_exit_sleep_mode\n");
        } else {
            test_suite_log_stringf("Failed lcd_exit_sleep_mode!! ret = 0x%x\n", ret);
        }
    }

    return ret;
}

static int test_lcd_idle(int argc, char *argv[])
{
    unsigned int ctrl;
    ext_errno ret = EXT_ERR_SUCCESS;
    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    ctrl = (unsigned int)strtol(argv[0], NULL, 0);
    if (ctrl != 0) {
        ret = uapi_lcd_enter_idle_mode();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_enter_idle_mode\n");
        } else {
            test_suite_log_stringf("Failed lcd_enter_idle_mode!! ret = 0x%x\n", ret);
        }
    } else {
        ret = uapi_lcd_exit_idle_mode();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_exit_idle_mode\n");
        } else {
            test_suite_log_stringf("Failed lcd_exit_idle_mode!! ret = 0x%x\n", ret);
        }
    }

    return ret;
}

static int test_lcd_te(int argc, char *argv[])
{
    unsigned int ctrl;
    ext_errno ret = EXT_ERR_SUCCESS;
    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    ctrl = (unsigned int)strtol(argv[0], NULL, 0);
    if (ctrl != 0) {
        ret = uapi_lcd_te_on();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_te_on\n");
        } else {
            test_suite_log_stringf("Failed lcd_te_on!! ret = 0x%x\n", ret);
        }
    } else {
        ret = uapi_lcd_te_off();
        if (ret == EXT_ERR_SUCCESS) {
            test_suite_log_stringf("Succ! lcd_te_off\n");
        } else {
            test_suite_log_stringf("Failed lcd_te_off!! ret = 0x%x\n", ret);
        }
    }

    return ret;
}

static int test_lcd_write(int argc, char *argv[])
{
    uint16_t bright = 0;
    ext_errno ret;
    uint32_t i;
    lcd_cmd_sequ wr_data = {0};

    if (argc < 2) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    wr_data.dcs_flag = 1;
    wr_data.data[0] = (uint8_t)strtoul(argv[0], NULL, 16); /* 16 */
    wr_data.data_len = (uint8_t)strtol(argv[1], NULL, 0) + 1;

    if (argc < wr_data.data_len + 1) {
        test_suite_log_stringf("write data(%d) less than write len(%d)\n", argc - 2, wr_data.data_len - 1); /* 2 */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    for (i = 0; i < wr_data.data_len - 1; i++) {
        wr_data.data[i + 1] = (uint8_t)strtoul(argv[i + 2], NULL, 16); /* 2, 16 */
    }

    ret = uapi_lcd_write_reg(&wr_data, 1);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Failed to write lcd reg!!\n");
        return ret;
    }

    test_suite_log_stringf("packet reg(0x%x), len: %d\n", wr_data.data[0], wr_data.data_len - 1);
    for (i = 0; i < wr_data.data_len - 1; i++) {
        test_suite_log_stringf("0x%x", wr_data.data[i + 1]);
        if (i == wr_data.data_len - 2) { /* 2 */
            test_suite_log_stringf("\n");
        } else {
            test_suite_log_stringf(" ");
        }
    }

    return TEST_SUITE_OK;
}

static int test_lcd_read(int argc, char *argv[])
{
    uint8_t reg_addr;
    uint8_t read_len;
    ext_errno ret;
    uint8_t i;
    uint8_t buff[100] = {0}; /* 100 */
    uint8_t *pbuff = buff;
    if (argc != 2) { /* 2 means para num */
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    reg_addr = (uint8_t)strtoul(argv[0], NULL, 16); /* 16 */
    read_len = (uint8_t)strtol(argv[1], NULL, 0);
    ret = uapi_lcd_read_reg(reg_addr, pbuff, read_len);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("Failed to read lcd reg!!\n");
        return ret;
    }

    test_suite_log_stringf("reg(0x%x):\n", reg_addr);
    for (int i = 0; i < read_len; i++) {
        test_suite_log_stringf("0x%x", buff[i]);
        if (i == read_len - 1) {
            test_suite_log_stringf("\n");
        } else {
            test_suite_log_stringf(" ");
        }
    }

    return TEST_SUITE_OK;
}

static void te_gpio_callback(pin_t pin, uintptr_t param)
{
    unused(param);

    if ((g_te_isr_cnt % 60) == 0) { /* 60Hz */
        PRINT("te(pin-%d) trigger %d times", pin, g_te_isr_cnt);
    }

    g_te_isr_cnt++;
}

static int test_lcd_te_isr(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    lcd_bus_api *bus_api = NULL;

    bus_api = lcd_get_bus_api();
    if (bus_api == NULL) {
        test_suite_log_stringf("get bus api error\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    if (bus_api->bus_register_te_isr == NULL) {
        test_suite_log_stringf("lcd_bus layer te_isr api unregister!!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    ext_errno ret = bus_api->bus_register_te_isr(te_gpio_callback);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("te_isr register failed!!, ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("te_isr register ok, waiting isr trigger\r\n");
    return TEST_SUITE_OK;
}

extern lcd_drv_cfg g_lcd_driver_ops[];
static int test_lcd_register_info(int argc, char *argv[])
{
    uint32_t index;
    ext_errno ret;

    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num, please input lcd_driver_ops index");
        return TEST_SUITE_TEST_FAILED;
    }

    index = strtol(argv[0], NULL, 0);
    ret = uapi_lcd_info_register(&g_lcd_driver_ops[index]);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("lcd info register failed!!, ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

void add_test_lcd(void)
{
    uapi_test_suite_add_function("lcd_info_reg", "test_lcd_info_register", test_lcd_register_info);
    uapi_test_suite_add_function("lcd_set_div", "lcd_set_div[div]", test_lcd_set_qspi_div);
    uapi_test_suite_add_function("lcd_init", "test_lcd_init", test_lcd_init);
    uapi_test_suite_add_function("lcd_deinit", "test_lcd_deinit", test_lcd_deinit);
    uapi_test_suite_add_function("lcd_display_on", "enable lcd display", test_lcd_display_on);
    uapi_test_suite_add_function("lcd_display_off", "disable lcd display", test_lcd_display_off);
    uapi_test_suite_add_function("lcd_set_bright", "set lcd brightness", test_lcd_set_bright);
    uapi_test_suite_add_function("lcd_get_bright", "get lcd brightness", test_lcd_get_bright);
    uapi_test_suite_add_function("lcd_sleep", "set lcd sleep [enable/disable]", test_lcd_sleep);
    uapi_test_suite_add_function("lcd_idle", "set lcd idle [enable/disable]", test_lcd_idle);
    uapi_test_suite_add_function("lcd_te", "set lcd te [enable/disable]", test_lcd_te);
    uapi_test_suite_add_function("lcd_write", "write lcd register[addr][len]<data0>...<data len-1>", test_lcd_write);
    uapi_test_suite_add_function("lcd_read", "read lcd register[addr][len]", test_lcd_read);
    uapi_test_suite_add_function("lcd_is_connect", "if lcd is connected", test_lcd_is_connected);
    uapi_test_suite_add_function("lcd_status", "screen lcd status: display/sleep/idle/..", test_lcd_get_status);
    uapi_test_suite_add_function("lcd_te_isr", "register te isr<gpio>", test_lcd_te_isr);
}
