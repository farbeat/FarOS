/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi_tx self test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include <stdlib.h>
#include "tcxo.h"
#include "soc_errno.h"
#include "soc_mipi_tx.h"
#include "mipi_tx.h"
#include "hal_mipi_tx.h"
#include "soc_lcd_api.h"
#include "test_suite.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "test_mipi_phy.h"
#include "test_mipi_reg.h"
#include "debug_print.h"
#include "sec_random.h"
#include "cmsis_os2.h"
#include "lcd_mipi.h"
#include "test_mipi_tx.h"

#define MIPI_PRESSURE_TEST_TIMES 1000
#define MIPI_PRESSURE_DELAY      500
int mipi_tx_colorbar_set(int argc, char *argv[])
{
    uint32_t ret;
    int32_t colorbar_en;
    int32_t colorbar_orien;

    if (argc < 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    colorbar_en = strtol(argv[0], NULL, 0);
    if (colorbar_en == 1) {
        if (argc > 1) {
            colorbar_orien = strtol(argv[1], NULL, 0);
        } else {
            colorbar_orien = 0;
        }
        /* Set colorbar configuration and enable display. */
        ret = mipi_tx_set_colorbar(colorbar_orien, 0);
        if (ret != EXT_ERR_SUCCESS) {
            test_suite_log_stringf("set colorbar failed\n");
            return ret;
        }
    } else if (colorbar_en == 0) {
        /* Set colorbar configuration and enable display. */
        uint32_t ret = mipi_tx_unset_colorbar();
        if (ret != EXT_ERR_SUCCESS) {
            test_suite_log_stringf("unset colorbar failed\n");
            return ret;
        }
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    return TEST_SUITE_OK;
}

int mipi_tx_get_proc(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    /* get mipi_tx configuration. */
    return mipi_tx_proc_show(0);
}

int mipi_tx_delay_us(int argc, char *argv[])
{
    uint32_t delay_us;

    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    delay_us = (uint32_t)strtol(argv[0], NULL, 0);

    uapi_tcxo_delay_us(delay_us);

    return TEST_SUITE_OK;
}

int test_mipi_tx_reset(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    mipi_tx_reset();
    return TEST_SUITE_OK;
}

int test_mipi_read(int argc, char *argv[])
{
    uint8_t reg_addr;
    uint8_t read_cnt;
    uint8_t work_mode;
    int ret;
    unsigned char rd_data[96] = {0}; /* 96 */
    get_cmd_info_t get_info = { 0 };

    if (argc != 3) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    work_mode = (uint8_t)strtol(argv[0], NULL, 0);
    reg_addr = (uint8_t)strtoul(argv[1], NULL, 16); /* 16 */
    read_cnt = (uint8_t)strtol(argv[2], NULL, 0);
    if (read_cnt > 96) { /* max array count 96 */
        test_suite_log_stringf("Invalid read_cnt: %d\n", read_cnt);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    get_info.work_mode = work_mode;
    get_info.data_type = 0x14;
    get_info.data_param = reg_addr;
    get_info.get_data_size = read_cnt;
    get_info.get_data = rd_data;
    ret = uapi_mipi_tx_get_cmd(&get_info);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("test mipi read fail, ret = 0x%x \r\n", ret);
        return ret;
    }

    test_suite_log_stringf("reg(0x%x):\n", reg_addr);
    for (int i = 0; i < read_cnt; i++) {
        test_suite_log_stringf("0x%x", get_info.get_data[i]);
        if (i == read_cnt - 1) {
            test_suite_log_stringf("\n");
        } else {
            test_suite_log_stringf(" ");
        }
    }

    return TEST_SUITE_OK;
}

int test_mipi_write_short(int argc, char *argv[])
{
    uint8_t reg_addr;
    uint8_t cfg_value;
    uint8_t cfg_len;
    int work_mode;
    int ret;
    cmd_info_t set_info = { 0 };

    if (argc < 2 || argc > 3) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    work_mode = (uint8_t)strtol(argv[0], NULL, 0);
    reg_addr = (uint8_t)strtoul(argv[1], NULL, 16); /* 16 */
    if (argc == 3) {
        cfg_value = (uint8_t)strtoul(argv[2], NULL, 16); /* 16 */
    } else {
        cfg_value = 0;
    }

    set_info.work_mode = work_mode;
    set_info.data_type = argc == 3 ? 0x15 : 0x05;
    set_info.cmd_size = test_dsi_short_packet(cfg_value, reg_addr);

    ret = uapi_mipi_tx_set_cmd(&set_info);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("test mipi write short packet fail, ret = 0x%x \r\n", ret);
        return ret;
    }

    test_suite_log_stringf("short packet reg(0x%x):\n", reg_addr);
    test_suite_log_stringf("cfg_data: 0x%x\n", cfg_value);

    return TEST_SUITE_OK;
}

int test_fix_long_packet(uint8_t reg_addr, uint8_t case_num, uint8_t work_mode)
{
    int32_t ret;
    int32_t i;
    uint8_t wr_data[160] = {0};
    cmd_info_t long_packet = {0};
    wr_data[0] = reg_addr;

    long_packet.work_mode = work_mode;
    long_packet.data_type = 0x29;
    long_packet.cmd = wr_data;

    switch (case_num) {
        case 0:
            long_packet.cmd_size = 64;
            break;
        case 1:
            long_packet.cmd_size = 128;
            break;
        case 2:
            long_packet.cmd_size = 160;
            break;
        default:
            test_suite_log_stringf("err: invalid long packet case: %d!\n", case_num);
            return TEST_SUITE_TEST_FAILED;
    }

    ret = uapi_mipi_tx_set_cmd(&long_packet);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("test mipi write long packet fail, ret = 0x%x \r\n", ret);
        return ret;
    }
    test_suite_log_stringf("fix long packet reg(0x%x), len: %d\n", reg_addr, long_packet.cmd_size - 1);
    for (i = 0; i < long_packet.cmd_size - 1; i++) {
        test_suite_log_stringf("0x%x", long_packet.cmd[i + 1]);
        if (i == long_packet.cmd_size - 2) {
            test_suite_log_stringf("\n");
        } else {
            test_suite_log_stringf(" ");
        }
    }

    return ret;
}

int test_mipi_write_long(int argc, char *argv[])
{
    uint8_t reg_addr;
    uint8_t packet_len;
    uint8_t work_mode;
    uint8_t case_num;
    int i;
    int ret;
    uint8_t wr_data[96] = {0};
    cmd_info_t long_packet = {0};

    if (argc < 4) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    work_mode = (uint8_t)strtol(argv[0], NULL, 0);
    reg_addr = (uint8_t)strtoul(argv[1], NULL, 16); /* 16 */
    packet_len = (uint8_t)strtoul(argv[2], NULL, 16); /* 16 */
    if (packet_len == 0) {
        case_num = (uint8_t)strtoul(argv[3], NULL, 16); /* 16 */
        return test_fix_long_packet(reg_addr, case_num, work_mode);
    }
    if (argc < packet_len + 3) {
        test_suite_log_stringf("Invalid packet_len: %d, argc: %d\n", packet_len, argc);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    wr_data[0] = reg_addr;
    for (i = 0; i < packet_len; i++) {
        wr_data[i + 1] = (uint8_t)strtoul(argv[i + 3], NULL, 16);
    }

    long_packet.work_mode = work_mode;
    long_packet.data_type = 0x29;
    long_packet.cmd_size = packet_len + 1;
    long_packet.cmd = wr_data;
    ret = uapi_mipi_tx_set_cmd(&long_packet);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("test mipi write long packet fail, ret = 0x%x \r\n", ret);
        return ret;
    }

    test_suite_log_stringf("long packet reg(0x%x), len: %d\n", reg_addr, packet_len);
    for (i = 0; i < packet_len; i++) {
        test_suite_log_stringf("0x%x", long_packet.cmd[i + 1]);
        if (i == packet_len - 1) {
            test_suite_log_stringf("\n");
        } else {
            test_suite_log_stringf(" ");
        }
    }

    return TEST_SUITE_OK;
}

void test_controller_phy_reset(void)
{
    reg32_write(0x90080000, 0x0);
    reg32_write(0x900800a0, 0x0);
}

void test_controller_phy_unreset(void)
{
    reg32_write(0x900800a0, 0xf);
    reg32_write(0x90080000, 0x1);
}

void test_001_mipi_reset(void)
{
    uint32_t i;

    for (i = 1; i <= MIPI_PRESSURE_TEST_TIMES; i++) {
        test_controller_phy_reset();
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000
        test_controller_phy_unreset();
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000
        if (i % 100 == 0) {
            test_suite_log_stringf("[reset]total/running: %d/%d\n", MIPI_PRESSURE_TEST_TIMES, i);
        }
    }
}

void test_002_mipi_resolution(void)
{
    uint32_t i;

    for (i = 1; i <= MIPI_PRESSURE_TEST_TIMES; i++) {
        // 异常分辨率：454*454
        reg32_write(0x9008030c, 0x1401c6);
        reg32_write(0x90080310, 0x280014);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        // 正常分辨率：800*1280
        reg32_write(0x9008030c, 0x120320);
        reg32_write(0x90080310, 0x120012);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        if (i % 100 == 0) {
            test_suite_log_stringf("[resolution]total/running: %d/%d\n", MIPI_PRESSURE_TEST_TIMES, i);
        }
    }
}

void test_003_vdp_output_enable(void)
{
    uint32_t i;

    for (i = 1; i <= MIPI_PRESSURE_TEST_TIMES; i++) {
        // 关闭vdp输出使能
        reg32_write(0x9006d000, 0x80000011);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        // 开启vdp输出使能
        reg32_write(0x9006d000, 0xc0000011);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        if (i % 100 == 0) {
            test_suite_log_stringf("[vdp_output_enable]total/running: %d/%d\n", MIPI_PRESSURE_TEST_TIMES, i);
        }
    }
}

void test_004_mipi_input_enable(void)
{
    uint32_t i;

    for (i = 1; i <= MIPI_PRESSURE_TEST_TIMES; i++) {
        // 关闭mipi_tx输入使能
        reg32_clrbit(0x90080308, 31);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        // 开启mipi_tx输入使能
        reg32_setbit(0x90080308, 31);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        if (i % 100 == 0) {
            test_suite_log_stringf("[input_enable]total/running: %d/%d\n", MIPI_PRESSURE_TEST_TIMES, i);
        }
    }
}

void test_005_mipi_video_mode_type(void)
{
    uint32_t i;

    for (i = 1; i <= MIPI_PRESSURE_TEST_TIMES; i++) {
        // Non-burst with sync pulse
        reg32_setbits(0x90080034, 8, 2, 0);
        reg32_setbit(0x90080354, 20);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        // Non-burst with sync events
        reg32_setbits(0x90080034, 8, 2, 1);
        reg32_setbit(0x90080354, 20);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        // burst mode
        reg32_setbits(0x90080034, 8, 2, 2);
        reg32_clrbit(0x90080354, 20);
        uapi_tcxo_delay_ms(MIPI_PRESSURE_DELAY); // 1000:延时1s
        if (i % 100 == 0) {
            test_suite_log_stringf("[video_mode]total/running: %d/%d\n", MIPI_PRESSURE_TEST_TIMES, i);
        }
    }
}

int test_mipi_pressure(int argc, char *argv[])
{
    unsigned int case_num;
    test_suite_error_e ret = TEST_SUITE_OK;
    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    case_num = (unsigned int)strtol(argv[0], NULL, 0);

    switch (case_num) {
        case 1:
            test_001_mipi_reset();
            break;
        case 2:
            test_002_mipi_resolution();
            break;
        case 3:
            test_003_vdp_output_enable();
            break;
        case 4:
            test_004_mipi_input_enable();
        case 5:
            test_005_mipi_video_mode_type();
            break;
        default:
            break;
    }

    return ret;
}

static void mipi_proc_help(void)
{
    mipi_proc_print("mipi_proc help..\r\n");
    mipi_proc_print("mipi_proc <proc_item(0..2)> [item_ctrl(0..2)]\r\n");
    mipi_proc_print("mipi_proc 0 .......show proc information\r\n");
    mipi_proc_print("mipi_proc 1 .......clear autoulps exit&entry counter\r\n");
    mipi_proc_print("mipi_proc 1 0 .....set autoulps exit counter\r\n");
    mipi_proc_print("mipi_proc 1 1 .....clear autoulps exit counter\r\n");
    mipi_proc_print("mipi_proc 1 2 .....clear autoulps entry counter\r\n");
    mipi_proc_print("mipi_proc 2 .......close autoulps\r\n");
}

int mipi_get_proc_info(int argc, char *argv[])
{
    uint32_t proc_item;
    uint32_t item_ctrl;

    if (argc == 1) {
        proc_item = (uint32_t)strtol(argv[0], NULL, 0);
        if (proc_item == 0) {
            mipi_tx_proc_show(0);
        } else if (proc_item == 1) {
            mipi_tx_hal_clear_exit_ulps();
            mipi_tx_hal_clear_ulps_entry_cnt();
        } else if (proc_item == 2) { /* 2: close ulps */
            mipi_tx_hal_autoulps_en(0);
        }
    } else if (argc == 2) { /* 2 argv */
        proc_item = (uint32_t)strtol(argv[0], NULL, 0);
        item_ctrl = (uint32_t)strtol(argv[1], NULL, 0);
        if (proc_item == 0) {
            mipi_tx_proc_show(0);
        } else if (proc_item == 1) {
            if (item_ctrl == 0) {
                mipi_tx_hal_exit_ulps();
            } else if (item_ctrl == 1) {
                mipi_tx_hal_clear_exit_ulps();
            } else if (item_ctrl == 2) { /* 2: clear entry ulps */
                mipi_tx_hal_clear_ulps_entry_cnt();
            }
        }
    } else {
        mipi_proc_help();
    }

    return TEST_SUITE_OK;
}

static ext_errno mipi_read_brightness(mipi_tx_work_mode_t mode, uint32_t *brightness)
{
    get_cmd_info_t get_info = {0};
    unsigned char rd_data[4] = {0}; /* 4 */

    get_info.work_mode = mode;
    get_info.data_type = 0x14;
    get_info.data_param = 0x51;
    get_info.get_data_size = 1;
    get_info.get_data = rd_data;
    ext_errno ret = uapi_mipi_tx_get_cmd(&get_info);
    *brightness = (uint32_t)rd_data[0];

    return ret;
}

static ext_errno mipi_write_brightness(mipi_tx_work_mode_t mode, uint32_t brightness)
{
    cmd_info_t set_info = {0};

    set_info.work_mode = mode;
    set_info.data_type = 0x15;
    set_info.cmd_size = test_dsi_short_packet(brightness, 0x51);
    return uapi_mipi_tx_set_cmd(&set_info);
}

void *ulps_random_cmd_task(void *arg)
{
    ext_errno ret;
    uint32_t *item = (uint32_t *)arg;
    uint32_t item_val = 2;
    uint32_t delay;
    uint32_t brightness;
    uint32_t read_ness;
    uint32_t round = 0;
    mipi_tx_work_mode_t mode;
    char *mode_str[] = {"hs", "lp"};

    if (arg != NULL) {
        item_val = *item;
    }
    while (1) {
        round++;
        read_ness = 0;
        mode = item_val;
        if (item_val == 2) {
            mode = (mipi_tx_work_mode_t)(round % 2);
        }
        delay = sec_rand() % 500; /* max delay 500 ms */
        brightness = sec_rand() % 255; /* max level 255 */

        /* suqin can not enter sdlp mode, sdlp function with bug */
        //uapi_mipi_tx_exit_sdlp();
        ret = mipi_write_brightness(mode, brightness);
        if (ret != EXT_ERR_SUCCESS) {
            //uapi_mipi_tx_auto_enter_sdlp();
            test_suite_log_stringf("[%s] write short packet fail, ret = 0x%x \r\n", mode_str[mode], ret);
            osDelay(500); /* sleep 500ms */
            continue;
        }

        ret = mipi_read_brightness(mode, &read_ness);
        if (ret != EXT_ERR_SUCCESS) {
            //uapi_mipi_tx_auto_enter_sdlp();
            test_suite_log_stringf("[%s] mipi read fail, ret = 0x%x \r\n", mode_str[mode], ret);
            osDelay(500); /* sleep 500ms */
            continue;
        }
        //uapi_mipi_tx_auto_enter_sdlp();
        if (brightness == read_ness) {
            test_suite_log_stringf("[%s] write cmd succ! brightness=%d\r\n", mode_str[mode], read_ness);
        } else {
            test_suite_log_stringf("[%s] write cmd fail! write_ness=%d read_ness=%d\r\n", mode_str[mode], brightness, read_ness);
        }
        test_suite_log_stringf("[%s] sleep %dms\r\n", mode_str[mode], delay);
        osDelay(delay);
    }

    return NULL;
}

int test_mipi_ulps_random(int argc, char *argv[])
{
    uint32_t item;

    if (argc != 1) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    item = (unsigned int)strtol(argv[0], NULL, 0);

    osThreadAttr_t attr = {"mipi_cmd", 0, NULL, 0, NULL, 0x4000, 18, 0, 0 };
    uint32_t *task_handle = osThreadNew(ulps_random_cmd_task, &item, &attr);
    if (task_handle == NULL) {
        test_suite_log_stringf("thread create failed!!!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int test_mipi_lp_ctrl(int argc, char *argv[])
{
    if (argc == 0) {
        test_suite_log_stringf("Invalid Parameters num");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t mode = strtol(argv[0], NULL, 0);
    ext_errno ret;

    switch (mode) {
        case LP_CTRL_ENTER_SDLP:
            test_suite_log_stringf("auto_enter_sdlp\r\n");
            ret = uapi_mipi_tx_auto_enter_sdlp();
            break;
        case LP_CTRL_EXIT_SDLP:
            test_suite_log_stringf("exit_sdlp\r\n");
            ret = uapi_mipi_tx_exit_sdlp();
            break;
        case LP_CTRL_EXIT_ULPS:
            test_suite_log_stringf("exit_ulps\r\n");
            ret = uapi_mipi_tx_exit_ulps();
            break;
        case LP_CTRL_CLEAR_EXIT_ULPS:
            test_suite_log_stringf("clear_exit_ulps\r\n");
            ret = uapi_mipi_tx_clear_exit_ulps();
            break;
        default:
            ret = EXT_ERR_LCD_INVALID_MODE;
    }

    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("lp ctrl failed, ret = 0x%x!!\n", ret);
        return ret;
    }

    return TEST_SUITE_OK;
}

int test_mipi_init(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    ext_errno ret = mipi_bus_init(lcd_get_bus_escape_config());
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("mipi init failed! ret = 0x%x\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}
int test_mipi_deinit(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    mipi_bus_deinit();
    return TEST_SUITE_OK;
}

int test_mipi_abnormal_test1(int argc, char *argv[])
{
    UNUSED(argc);

    writel(0x56010000, 0xff);
    writel(0x56010004, 0x400);
    writel(0x56010008, 0xc126014);

    uint32_t time = strtol(argv[0], NULL, 0);
    for (uint32_t i = 0; i < time; i++) {

        mipi_tx_set_colorbar(0, 0);
        uapi_tcxo_delay_us(10000ULL);

        writel(0x56000000, 0);
        writel(0x560000a0, 0);
        uapi_tcxo_delay_us(10ULL);
        writel(0x560000a0, 0xf);
        writel(0x56000000, 1);
        uapi_tcxo_delay_us(10ULL);
        mipi_tx_set_colorbar(1, 0);
        uapi_tcxo_delay_us(10000ULL);
    }

    return TEST_SUITE_OK;
}

int test_mipi_abnormal_test2(int argc, char *argv[])
{
    UNUSED(argc);

    writel(0x56010000, 0xff);
    writel(0x56010004, 0x400);
    writel(0x56010008, 0xc126014);

    uint32_t time = strtol(argv[0], NULL, 0);
    for (uint32_t i = 0; i < time; i++) {

        mipi_tx_set_colorbar(0, 0);
        uapi_tcxo_delay_us(10000ULL);

        reg32_clrbit(0x56000308, 31);
        uapi_tcxo_delay_us(10ULL);
        reg32_setbit(0x56000308, 31);
        uapi_tcxo_delay_us(10ULL);
        mipi_tx_set_colorbar(1, 0);
        uapi_tcxo_delay_us(10000ULL);
    }

    return TEST_SUITE_OK;
}

void add_test_mipi_tx(void)
{
#ifdef BOARD_FPGA
    mipi_phy_hiwing_default_i2c();
#endif
    uapi_test_suite_add_function("mipi_tx_colorbar_set", "mipi_tx_colorbar_set <en> <mode>", mipi_tx_colorbar_set);
    uapi_test_suite_add_function(
        "mipi_tx_proc_func", "multiple mipi_tx proc function, help show without paramter.", mipi_get_proc_info);
    uapi_test_suite_add_function("mipi_ulps_random", "mipi_ulps_random <0-hs, 1-lp, 2-lp&hs>", test_mipi_ulps_random);
    uapi_test_suite_add_function("mipi_tx_get_proc", "get mipi_tx proc info.", mipi_tx_get_proc);
    uapi_test_suite_add_function("mipi_tx_reset", "reset mipi and phy.", test_mipi_tx_reset);
    uapi_test_suite_add_function("usleep", "usleep <us>", mipi_tx_delay_us);
    uapi_test_suite_add_function("mipi_phy_w4", "mipi_phy_w4 <addr> <val>", mipi_phy_hiwing_write);
    uapi_test_suite_add_function("mipi_phy_mem32", "mipi_phy_mem32 <addr>", mipi_phy_hiwing_read);
    uapi_test_suite_add_function("mipi_read", "<work_mode> <reg> <read_len>", test_mipi_read);
    uapi_test_suite_add_function("mipi_write_short", "<work_mode> <reg> [value]", test_mipi_write_short);
    uapi_test_suite_add_function(
        "mipi_write_long", "<work_mode> <reg> <packet_len> <data0/fix_case> [data...]", test_mipi_write_long);
    uapi_test_suite_add_function("mipi_pressure", "<case number>", test_mipi_pressure);
    uapi_test_suite_add_function("mipi_lp_ctrl", "<mode>", test_mipi_lp_ctrl);
    uapi_test_suite_add_function("mipi_init", "", test_mipi_init);
    uapi_test_suite_add_function("mipi_deinit", "", test_mipi_deinit);
    uapi_test_suite_add_function("mipi_test1", "", test_mipi_abnormal_test1);
    uapi_test_suite_add_function("mipi_test2", "", test_mipi_abnormal_test2);
    uapi_test_suite_add_function("mipi_screen_init_reg_cfg", "mipi_screen_init_reg_cfg", mipi_screen_init_reg_cfg);
    uapi_test_suite_add_function("mipi_colorbar_on",  "mipi_colorbar_on",  mipi_colorbar_on);
    uapi_test_suite_add_function("mipi_colorbar_off", "mipi_colorbar_off", mipi_colorbar_off);
    uapi_test_suite_add_function("dpu_colorbar_on",   "dpu_colorbar_on",   dpu_colorbar_on);
}
