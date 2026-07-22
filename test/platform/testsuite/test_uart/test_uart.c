/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test uart source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-13, Create file. \n
 */

#include "test_uart.h"
#include "uart.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "osal_debug.h"
#include "osal_addr.h"
#include "osal_interrupt.h"
#include "securec.h"
#include "common_def.h"

#include "tcxo.h"
#include "osal_debug.h"

#if defined(CONFIG_UART_SUPPORT_DMA)
#include "hal_dma.h"
#endif  /* CONFIG_UART_SUPPORT_DMA */

#define TEST_UART_CONFIG_PARAM_NUM 5
#define TEST_UART_RX_PARAM_NUM 4
#define TEST_UART_RX_BUFF_SIZE 1024
#define TEST_UART_TX_PARAM_NUM 4
#define TEST_UART_TX_BUFF_SIZE 2048
#define TEST_UART_READ_MAX_TIME 100000
#define TEST_UART_DMA_SEND_PARAM_NUM 3
#define TEST_UART_DMA_RX_PARAM_NUM 2
#define TEST_UART_DMA_SEND_BUFF_SIZE 1024

typedef enum {
    UART_TRANSFER_MODE_POLL,
    UART_TRANSFER_MODE_INT,
    UART_TRANSFER_MODE_DMA
} uart_transfer_mode_t;

uint8_t g_uart_rx_buff[TEST_UART_RX_BUFF_SIZE] = { 0 };
static uint32_t g_uart_read_count = 0;
static uint32_t g_test_uart_read_int_num = 0;
uart_buffer_config_t g_uart_buffer_config = {
    .rx_buffer = g_uart_rx_buff,
    .rx_buffer_size = TEST_UART_RX_BUFF_SIZE
};
uart_bus_t g_uart_bus = UART_BUS_MAX_NUM;

static int32_t test_uart_config(int32_t argc, char *argv[])
{
    if (argc < TEST_UART_CONFIG_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    bool tx_dma_enable = false;
    bool rx_dma_enable = false;
    uart_attr_t attr = { 0 };

    uint32_t param_index = 0;
    uart_bus_t bus = (uart_bus_t)strtol(argv[param_index++], NULL, 0);

    attr.baud_rate = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.data_bits = (uint8_t)strtol(argv[param_index++], NULL, 0);
    attr.stop_bits = (uint8_t)strtol(argv[param_index++], NULL, 0);
    attr.parity = (uint8_t)strtol(argv[param_index++], NULL, 0);

    osal_printk("uapi_uart_init bus=%d baud_rate=%d, data_bits=%d, stop_bits=%d, parity=%d\r\n", bus, attr.baud_rate, \
                attr.data_bits, attr.stop_bits, attr.parity);

    if (argc == TEST_UART_CONFIG_PARAM_NUM + 2) {  /* 2: Indicates the number of dma input parameters  */
        tx_dma_enable = (bool)strtol(argv[param_index++], NULL, 0);
        rx_dma_enable = (bool)strtol(argv[param_index++], NULL, 0);
    }

    uart_pin_config_t pin_config = {
        .tx_pin = S_MGPIO19,
        .rx_pin = S_MGPIO20,
        .cts_pin = PIN_NONE,
        .rts_pin = PIN_NONE
    };
    uapi_uart_deinit(bus);

    uart_extra_attr_t extra_attr = {
        .tx_dma_enable = tx_dma_enable,
        .tx_int_threshold = 0,
        .rx_dma_enable = rx_dma_enable,
        .rx_int_threshold = 0
    };

    errcode_t errcode = uapi_uart_init(bus, &pin_config, &attr, &extra_attr, &g_uart_buffer_config);
    osal_printk("uapi_uart_init ret=%x\r\n", errcode);
    if (errcode != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }
    g_uart_bus = bus;

    return TEST_SUITE_OK;
}

static int test_uart_read_poll(int32_t size, uint8_t value, uint32_t times)
{
    while (times--) {
        int32_t len = uapi_uart_read(g_uart_bus, g_uart_rx_buff, size, 0);
        if (len != size) {
            return TEST_SUITE_TEST_FAILED;
        }

        for (int32_t i = 0; i < size; i++) {
            osal_printk("0x%2x ", g_uart_rx_buff[i]);
            if (g_uart_rx_buff[i] != value) {
                return TEST_SUITE_TEST_FAILED;
            }
        }
        osal_printk("\n");
    }
    return TEST_SUITE_OK;
}

static void test_uart_read_int_handler(const void *buffer, uint16_t length, bool error)
{
    unused(error);
    uint8_t *buff = (uint8_t *)buffer;

    /* 优化testsuit中断模式读数据超出问题 */
    if (g_uart_read_count >= g_test_uart_read_int_num) {
        return;
    }

    for (uint32_t i = 0; i < length; i++) {
        g_uart_rx_buff[g_uart_read_count++] = buff[i];
    }
}

static int test_uart_read_int(uint32_t size, uint8_t value, uint32_t times)
{
    unused(value);
    g_uart_read_count = 0;
    g_test_uart_read_int_num = size * times;

    errcode_t errcode = uapi_uart_register_rx_callback(g_uart_bus, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                                       1, test_uart_read_int_handler);
    if (errcode != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint64_t start_time = uapi_tcxo_get_ms();
    while (g_uart_read_count < (times * size) && (uapi_tcxo_get_ms() - start_time < TEST_UART_READ_MAX_TIME)) {
    }
    for (uint32_t i = 0; i < g_uart_read_count; i++) {
        osal_printk("0x%2x ", g_uart_rx_buff[i]);
        if (g_uart_rx_buff[i] != value) {
            return TEST_SUITE_TEST_FAILED;
        }
    }
    osal_printk("\n");

    if (uapi_tcxo_get_ms() - start_time >= TEST_UART_READ_MAX_TIME) {
        return TEST_SUITE_ERROR_TIMED_OUT;
    }

    g_test_uart_read_int_num = 0;
    return TEST_SUITE_OK;
}

static int32_t test_uart_read(int32_t argc, char *argv[])
{
    if (argc != TEST_UART_RX_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (g_uart_bus == UART_BUS_MAX_NUM) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t param_index = 0;
    uart_transfer_mode_t mode = (uart_transfer_mode_t)strtol(argv[param_index++], NULL, 0);
    uint8_t value = (uint8_t)strtol(argv[param_index++], NULL, 0);
    int32_t size = (int32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t times = (uint32_t)strtol(argv[param_index++], NULL, 0);

    if (size > TEST_UART_RX_BUFF_SIZE) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (mode == UART_TRANSFER_MODE_POLL) {
        return test_uart_read_poll(size, value, times);
    } else if (mode == UART_TRANSFER_MODE_INT) {
        return test_uart_read_int(size, value, times);
    }

    return TEST_SUITE_ERROR_BAD_PARAMS;
}

static uint8_t g_uart_send_buff[TEST_UART_TX_BUFF_SIZE] = { 0 };

static int test_uart_write_poll(uint32_t size, uint8_t value, uint32_t times)
{
    uint8_t *tx_buff = g_uart_send_buff;
    if (memset_s(tx_buff, TEST_UART_TX_BUFF_SIZE, value, size) != EOK) {
        return TEST_SUITE_TEST_FAILED;
    }
    while (times--) {
        int32_t ret = uapi_uart_write(g_uart_bus, tx_buff, size, 0);
        if (ret == -1) {
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

static int test_uart_write_poll_stat(uint32_t size, uint8_t value, uint32_t times)
{
    uint8_t *tx_buff = g_uart_send_buff;
    if (memset_s(tx_buff, TEST_UART_TX_BUFF_SIZE, value, size) != EOK) {
        return TEST_SUITE_TEST_FAILED;
    }
    uint32_t old_time = times;
    uint32_t delta_t = 0;
    while (times--) {
        uint32_t irq_sts = osal_irq_lock();
        uint64_t t1 = uapi_tcxo_get_us();
        int32_t ret = uapi_uart_write(g_uart_bus, tx_buff, size, 0);
        if (ret == -1) {
            osal_irq_restore(irq_sts);
            return TEST_SUITE_TEST_FAILED;
        }
        uint64_t t2 = uapi_tcxo_get_us();
        osal_irq_restore(irq_sts);
        delta_t += (uint32_t)(t2 - t1);
    }

    osal_printk("uapi write time %u us, length %u\r\n", (delta_t / old_time), size);
    return TEST_SUITE_OK;
}

static void test_uart_write_int_handler(const void *buffer, uint32_t length, const void *params)
{
    void *buff = (void *)buffer;
    unused(length);
    unused(params);
    osal_kfree(buff);
}

static int test_uart_write_int(uint32_t size, uint8_t value, uint32_t times)
{
    while (times--) {
        uint8_t *tx_buff = (uint8_t *)osal_kmalloc(size * sizeof(uint8_t), 0);
        if (memset_s(tx_buff, size, value, size) != EOK) {
            return TEST_SUITE_TEST_FAILED;
        }
        errcode_t errcode = uapi_uart_write_int(g_uart_bus, tx_buff, size, 0, test_uart_write_int_handler);
        if (errcode != ERRCODE_SUCC) {
            return TEST_SUITE_TEST_FAILED;
        }
    }
    return TEST_SUITE_OK;
}

static int32_t test_uart_write(int32_t argc, char *argv[])
{
    if (argc != TEST_UART_TX_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (g_uart_bus == UART_BUS_MAX_NUM) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t param_index = 0;
    uart_transfer_mode_t mode = (uart_transfer_mode_t)strtol(argv[param_index++], NULL, 0);
    uint8_t value = (uint8_t)strtol(argv[param_index++], NULL, 0);
    int32_t size = (int32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t times = (uint32_t)strtol(argv[param_index++], NULL, 0);

    if (mode == UART_TRANSFER_MODE_POLL) {
        return test_uart_write_poll(size, value, times);
    } else if (mode == UART_TRANSFER_MODE_INT) {
        return test_uart_write_int(size, value, times);
    }

    return TEST_SUITE_ERROR_BAD_PARAMS;
}

static int32_t test_uart_write_stat(int32_t argc, char *argv[])
{
    if (argc != 3) { // 3 参数数量
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (g_uart_bus == UART_BUS_MAX_NUM) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t param_index = 0;
    uint8_t value = (uint8_t)strtol(argv[param_index++], NULL, 0);
    int32_t size = (int32_t)strtol(argv[param_index++], NULL, 0);
    uint32_t times = (uint32_t)strtol(argv[param_index++], NULL, 0);

    return test_uart_write_poll_stat(size, value, times);
}

static void test_uart_read_int1_handler(const void *buffer, uint16_t length, bool error)
{
    unused(error);
    uint8_t *buff = (uint8_t *)buffer;
    for (uint32_t i = 0; i < length; i++) {
        osal_printk("%c", buff[i]);
    }
}

static int32_t test_uart_read_hellocfbb(int32_t argc, char *argv[])
{
    unused(argc);
    unused(argv);

    errcode_t errcode = uapi_uart_register_rx_callback(g_uart_bus, UART_RX_CONDITION_FULL_OR_SUFFICIENT_DATA_OR_IDLE,
                                                       1, test_uart_read_int1_handler);
    if (errcode != ERRCODE_SUCC) {
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}
#if defined(CONFIG_UART_SUPPORT_DMA)
static int32_t test_uart_write_by_dma(int32_t argc, char *argv[])
{
    if (argc != TEST_UART_DMA_SEND_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (g_uart_bus == UART_BUS_MAX_NUM) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t param_index = 0;
    uint8_t value = (uint8_t)strtol(argv[param_index++], NULL, 0);
    uint16_t len = (uint16_t)strtol(argv[param_index++], NULL, 0);
    uint32_t width = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint8_t dma_buff[TEST_UART_DMA_SEND_BUFF_SIZE] = { 0 };
    if (memset_s(dma_buff, TEST_UART_DMA_SEND_BUFF_SIZE, value, TEST_UART_DMA_SEND_BUFF_SIZE) != EOK) {
        return TEST_SUITE_TEST_FAILED;
    }

    uart_write_dma_config_t dma_cfg = {
        .src_width = width,
        .dest_width = 0,
        .burst_length = (uint8_t)HAL_DMA_BURST_TRANSACTION_LENGTH_8,
        .priority = 0
    };
    if (uapi_uart_write_by_dma(g_uart_bus, dma_buff, len, &dma_cfg) != (len >> width)) {
        osal_printk("[UART] *** memory to uart fail!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

static int32_t test_uart_read_by_dma(int32_t argc, char *argv[])
{
    if (argc != TEST_UART_DMA_RX_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    if (g_uart_bus == UART_BUS_MAX_NUM) {
        return TEST_SUITE_TEST_FAILED;
    }

    uint32_t param_index = 0;
    uint16_t len = (uint16_t)strtol(argv[param_index++], NULL, 0);
    uint32_t width = (uint32_t)strtol(argv[param_index++], NULL, 0);
    uint8_t dma_buff[TEST_UART_DMA_SEND_BUFF_SIZE] = { 0 };
    if (memset_s(dma_buff, TEST_UART_DMA_SEND_BUFF_SIZE, 0, TEST_UART_DMA_SEND_BUFF_SIZE) != EOK) {
        return TEST_SUITE_TEST_FAILED;
    }

    uart_write_dma_config_t dma_cfg = {
        .src_width = 0,
        .dest_width = width,
        .burst_length = (uint8_t)HAL_DMA_BURST_TRANSACTION_LENGTH_8,
        .priority = 0
    };
    if (uapi_uart_read_by_dma(g_uart_bus, dma_buff, len, &dma_cfg) != len) {
        osal_printk("[UART] *** memory to uart fail!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }

    uapi_uart_write(UART_BUS_0, (uint8_t*)dma_buff, len, 0);
    return TEST_SUITE_OK;
}
#endif  /* CONFIG_UART_SUPPORT_DMA */

void add_uart_test_case(void)
{
    uapi_test_suite_add_function("test_cfbb_uart_config",
        "Params: <bus>, <baud_rate>, <data_bits>, <stop_bits>, <parity>, <tx dma>, <rx dma>", test_uart_config);
    uapi_test_suite_add_function("test_cfbb_uart_read", "Params: <mode>, <value>, <size>, <times>", test_uart_read);
    uapi_test_suite_add_function("test_cfbb_uart_write", "Params: <mode>, <value>, <size>, <times>", test_uart_write);
    uapi_test_suite_add_function("test_cfbb_uart_write_stat", "Params: <value>, <size>, <times>", test_uart_write_stat);
    uapi_test_suite_add_function("test_uart_read_hellocfbb", "Params: None", test_uart_read_hellocfbb);
#if defined(CONFIG_UART_SUPPORT_DMA)
    uapi_test_suite_add_function("test_cfbb_uart_write_by_dma", "Params: <value>, <len>, <width>",
                                 test_uart_write_by_dma);
    uapi_test_suite_add_function("test_cfbb_uart_read_by_dma", "Params: <len>, <width>",
                                 test_uart_read_by_dma);
#endif  /* CONFIG_UART_SUPPORT_DMA */
}
