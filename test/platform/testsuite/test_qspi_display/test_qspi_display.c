/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test lcd source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */
#include "test_qspi_display.h"
#include "test_suite_log.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "chip_io.h"
#include "soc_osal.h"
#include "tcxo.h"
#include "platform_core.h"
#include "non_os.h"
#include "gpio.h"
#include "qspi_display.h"
#include "hal_dma.h"
#include "dma.h"

#ifdef PRE_ASIC
#define QSPI_RESET_LCD_GPIO        S_AGPIO_R3
#else
#define QSPI_RESET_LCD_GPIO        S_HGPIO0
#endif
#define LCD_PIXEL_RAW                 368
#define LCD_PIXEL_COL                 194
#define LCD_STRIDE                    1024
#define COLORBAR_NUM                  8

#define INPUT_HEX                     16
#define BYTES_PER_PIXEL               3
volatile bool g_dma_trans_done = false;
volatile bool g_dma_trans_succ = false;

static int lcd_power_on(int argc, char* argv[])
{
    unused(argc);
    unused(argv);

    uapi_gpio_set_dir(QSPI_RESET_LCD_GPIO, GPIO_DIRECTION_OUTPUT);

    uapi_gpio_set_val(QSPI_RESET_LCD_GPIO, GPIO_LEVEL_LOW);
    uapi_tcxo_delay_ms(20); /* delay 20 ms */

    uapi_gpio_set_val(QSPI_RESET_LCD_GPIO, GPIO_LEVEL_HIGH);
    test_suite_log_stringf("reset lcd ok!");
    return TEST_SUITE_OK;
}

static int test_qspi_display_init(int argc, char* argv[])
{
    unused(argc);
    unused(argv);

    ext_errno ret = uapi_qspi_display_init();
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("qspi display init failed, ret = 0x%x", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("qspi display init success");
    return TEST_SUITE_OK;
}

static int test_qspi_display_deinit(int argc, char* argv[])
{
    unused(argc);
    unused(argv);

    ext_errno ret = uapi_qspi_display_deinit();
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("qspi display deinit success");
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("qspi display deinit failed, ret = 0x%x", ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

static void test_dma_trans_done_callback(uint8_t int_type, uint8_t channel)
{
    unused(channel);
    test_suite_log_stringf("[DMA] int_type is %d.\r\n", int_type);
    switch (int_type) {
        case HAL_DMA_INTERRUPT_TFR:
            g_dma_trans_done = true;
            g_dma_trans_succ = true;
            break;
        case HAL_DMA_INTERRUPT_BLOCK:
            g_dma_trans_done = true;
            g_dma_trans_succ = true;
            break;
        case HAL_DMA_INTERRUPT_ERR:
            g_dma_trans_done = true;
            g_dma_trans_succ = false;
            break;
        default:
            break;
    }
    test_suite_log_stringf("[DMA] int_type is %d.\r\n", int_type);
}

static void set_pixel_value(uint32_t addr, uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
    uint8_t i = 0;

    writeb(addr, r_val);
    i++;
    writeb(addr + i, g_val);
    i++;
    writeb(addr + i, b_val);
}

static void draw_frame_to_mem(uint32_t addr, uint32_t stride)
{
    uint32_t pixel = LCD_PIXEL_RAW / COLORBAR_NUM;

    non_os_enter_critical();
    uint32_t offset = 0;
    test_suite_log_stringf("drawing to [0x%x] with stride %d....\r\n", addr, stride);

    for (uint32_t i = 0; i < LCD_PIXEL_RAW; i++) {
        offset = stride * i;
        for (uint32_t j = 0; j < LCD_PIXEL_COL; j++) {
            if (i < pixel) {
                set_pixel_value(addr + offset, 0xff, 0x00, 0x00); /* red */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x2) {
                set_pixel_value(addr + offset, 0xff, 0xff, 0x00); /* yellow */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x3) {
                set_pixel_value(addr + offset, 0xff, 0x00, 0xff); /* purple */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x4) {
                set_pixel_value(addr + offset, 0x00, 0x00, 0x00); /* black */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x5) {
                set_pixel_value(addr + offset, 0x00, 0xff, 0x00); /* green */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x6) {
                set_pixel_value(addr + offset, 0x00, 0xff, 0xff); /* cyan */
                offset += BYTES_PER_PIXEL;
            } else if (i < pixel * 0x7) {
                set_pixel_value(addr + offset, 0x00, 0x00, 0xff); /* blue */
                offset += BYTES_PER_PIXEL;
            } else {
                set_pixel_value(addr + offset, 0xff, 0xff, 0xff); /* white */
                offset += BYTES_PER_PIXEL;
            }
        }
    }

    non_os_exit_critical();
    test_suite_log_stringf("drawing frame ok.\r\n");
}

static void refresh_frame(uint32_t addr, uint32_t stride)
{
    non_os_enter_critical();
    uint32_t offset = 0;
    for (uint32_t i = 0; i < LCD_PIXEL_RAW; i++) {
        offset = stride * i;
        for (uint32_t j = 0; j < LCD_PIXEL_COL * BYTES_PER_PIXEL; j++) {
            writeb(addr + offset, 0xff); /* red */
            offset++;
        }
    }
    non_os_exit_critical();
    test_suite_log_stringf("refresh frame ok.\r\n");
}

static int test_qspi_display_enable(int argc, char* argv[])
{
    qspi_display_frame_info_t info = {0};

    if (argc != 1) {
        test_suite_log_stringf("parameter error ,please enter addr!!\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    info.frame_addr = strtoul((const char*)argv[0], NULL, INPUT_HEX);
    info.frame_col_byte = LCD_PIXEL_COL * 3;
    info.frame_raw = LCD_PIXEL_RAW;
    info.col_byte_with_stride = LCD_PIXEL_COL * 3;
    ext_errno ret = uapi_qspi_display_enable(&info);
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("enable success!!\r\n");
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("enable failed, error code is 0x%x!!\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

static int test_qspi_display_disable(int argc, char* argv[])
{
    unused(argc);
    unused(argv);

    ext_errno ret = uapi_qspi_display_disable();
    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("qspi display disable success");
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("qspi display disable failed, ret = 0x%x", ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

typedef struct {
    uint32_t reg_addr;
    uint8_t  data_len; /* payload length, 小于等于4 */
    uint8_t  data[4]; /* payload, 4 enough for test  */
} lcd_cmd;

lcd_cmd g_qspi_cmd[] = {
    {0x3500, 1, {0}},  /* TE ON */
    {0x3a00, 1, {0x77}}, /* pixel mode 24bit */
    {0x5100, 1, {0x80}}, /* set brightness */
    {0x2a00, 4, {0x0, 0x0, 0x0, 0xc1}}, /* col start and end pixel */
    {0x1100, 0, {0}}, /* sleep out */
    {0x2900, 0, {0}}, /* display on */
    {0x3900, 0, {0}}, /* ilde mode */
};

static int test_qspi_display_set_cmd(int argc, char* argv[])
{
    qspi_cmd_info_t cmd_info = {0};

    if (argc < 2) {
        test_suite_log_stringf("parameter error!! please input[addr][len]<data0><data1>...\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t addr = strtoul((const char*)argv[0], NULL, INPUT_HEX);
    uint32_t payload_num = strtol(argv[1], NULL, 0);

    uint8_t *data = (uint8_t *)osal_kmalloc(payload_num, OSAL_GFP_KERNEL);

    for (uint32_t i = 0; i < payload_num; i++) {
        if (i + 2 < (uint32_t)argc) {
            data[i] = strtoul((const char*)argv[2 + i], NULL, INPUT_HEX);
        } else {
            data[i] = 0;
        }
    }
    cmd_info.reg_addr = addr;

    cmd_info.data_len = payload_num;
    cmd_info.data = data;
    ext_errno ret = uapi_qspi_display_write_cmd(&cmd_info);
    osal_kfree(data);

    if (ret == EXT_ERR_SUCCESS) {
        test_suite_log_stringf("set_cmd success!!\r\n");
        return TEST_SUITE_OK;
    } else {
        test_suite_log_stringf("set cmd failed, error code is 0x%x!!\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
}

static int test_qspi_display_lcd_init(int argc, char* argv[])
{
    unused(argc);
    unused(argv);

    for (uint32_t i = 0; i < sizeof(g_qspi_cmd) / sizeof(lcd_cmd); i++) {
        qspi_cmd_info_t cmd_info = {0};
        cmd_info.reg_addr = g_qspi_cmd[i].reg_addr;
        cmd_info.data_len = g_qspi_cmd[i].data_len;
        cmd_info.data = g_qspi_cmd[i].data;
        ext_errno ret = uapi_qspi_display_write_cmd(&cmd_info);
        if (ret != EXT_ERR_SUCCESS) {
            test_suite_log_stringf("set cmd 0x%x failed, error code is 0x%x!!\r\n", cmd_info.reg_addr, ret);
            return TEST_SUITE_TEST_FAILED;
        }
    }
    test_suite_log_stringf("lcd init success!!\r\n");
    return TEST_SUITE_OK;
}

static int test_qspi_display_read_cmd(int argc, char* argv[])
{
    if (argc != 2) {
        test_suite_log_stringf("parameter error!! please input[addr][len]\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t addr = strtoul((const char*)argv[0], NULL, INPUT_HEX);
    uint32_t len = strtol(argv[1], NULL, 0);

    uint8_t *data = (uint8_t *)osal_kmalloc(len, OSAL_GFP_KERNEL);
    if (data == NULL) {
        test_suite_log_stringf("malloc failed!!\r\n");
        return TEST_SUITE_ALLOC_FAILED;
    }

    ext_errno ret = uapi_qspi_display_read_cmd(addr, data, len);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("read_cmd failed, error code is 0x%x!!\r\n", ret);
        osal_kfree(data);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("read_cmd success!!\r\n");
    for (uint32_t i = 0; i < len; i++) {
        test_suite_log_stringf("read[0x%x] = 0x%x\r\n", addr + i, data[i]);
    }

    osal_kfree(data);
    return TEST_SUITE_OK;
}

static void dma_callback(void)
{
    test_dma_trans_done_callback(0, NULL);
}

static int test_qspi_display_show_on(int argc, char* argv[])
{
    uint32_t option = 0;
    ext_errno ret;

    if (argc < 2) {
        test_suite_log_stringf("parameter error!! input[addr][stride]<option 1-all_white, other-colorbar>\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t addr = strtoul((const char*)argv[0], NULL, INPUT_HEX);
    uint32_t stride = strtol(argv[1], NULL, 0);

    if (argc > 2) {
        option = strtol(argv[2], NULL, 0);
    }

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (option == 1) {
        refresh_frame(addr, (stride == 0 ? LCD_PIXEL_COL * BYTES_PER_PIXEL : 1024)); /* 1024: stride */
    } else {
        draw_frame_to_mem(addr, (stride == 0 ? LCD_PIXEL_COL * BYTES_PER_PIXEL : 1024)); /* 1024: stride */
    }

    uint64_t start_time = uapi_tcxo_get_us();
    ret = uapi_qspi_display_show_on(addr, dma_callback);
    if (ret != EXT_ERR_SUCCESS) {
        test_suite_log_stringf("qspi display show on error, error ret is 0x%xn\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("[DMA_TX] waitting ... \r\n");

    while (!g_dma_trans_done) {
    }

    uint64_t end_time = uapi_tcxo_get_us();
    test_suite_log_stringf("frame send take %lld us\r\n", end_time - start_time);

    if (!g_dma_trans_succ) {
        test_suite_log_stringf("[DMA_TX] tranfer failed! error = %d.\r\n", g_dma_trans_succ);
        return TEST_SUITE_TEST_FAILED;
    }

    test_suite_log_stringf("frame send ok\r\n");
    return TEST_SUITE_OK;
}


void add_qspi_test_case(void)
{
    uapi_test_suite_add_function("qspi_display_init", "qspi_init", test_qspi_display_init);
    uapi_test_suite_add_function("qspi_display_deinit", "qspi_deinit", test_qspi_display_deinit);
    uapi_test_suite_add_function("qspi_display_enable", "display enable[addr]", test_qspi_display_enable);
    uapi_test_suite_add_function("qspi_display_disable", "display disable", test_qspi_display_disable);
    uapi_test_suite_add_function(
        "qspi_display_write", "[addr][len]<data(0)>...<data(len-1)>", test_qspi_display_set_cmd);
    uapi_test_suite_add_function("qspi_display_read", "[addr][len]", test_qspi_display_read_cmd);
    uapi_test_suite_add_function(
        "qspi_display_show_on", "[addr][stride 0--no_stride]<refresh>", test_qspi_display_show_on);

    uapi_test_suite_add_function("power_on", "power_on", lcd_power_on);
    uapi_test_suite_add_function("qspi_dispaly_lcd_init", "lcd init", test_qspi_display_lcd_init);
}
