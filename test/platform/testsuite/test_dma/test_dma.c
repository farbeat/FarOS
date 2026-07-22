/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test dma source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-16， Create file. \n
 */

#include "test_dma.h"
#include "dma.h"
#include "tcxo.h"
#if defined(CONFIG_DMA_UART_SUPPORT_V151)
#include "hal_dmac_v151.h"
#include "hal_uart_v151_regs_op.h"
#else
#include "hal_dmac_v100.h"
#include "hal_uart_v120_regs_op.h"
#endif
#include "dma_porting.h"

#include "test_suite.h"
#include "test_suite_errors.h"

#include "soc_osal.h"
#include "securec.h"
#include "common_def.h"

#include "uart_porting.h"
#include "uart.h"
#ifdef CONFIG_DMA_TMP
#include "spi.h"
#include "hal_spi.h"

#define TESTSUIT_SPI_SEND_DATA_LEN 8
#define SPI_DR_REG_OFFSET          0x60
#define SPI_DMA_TX_DATA_LEVEL_4     4
#define QSPI_DMA_TX_DATA_LEVEL_8    8
#define TEST_DATA_LEN           16
#define TEST_DATA_ONCE_SEND_LEN 8

static volatile uint32_t const g_spi_dr_addr[SPI_BUS_MAX_NUMBER] = {
    SPI_BUS_0_BASE_ADDR + SPI_DR_REG_OFFSET,  // SPI_BUS 0 DR addr addr
    SPI_BUS_1_BASE_ADDR + SPI_DR_REG_OFFSET,  // SPI_BUS 1 DR addr addr
    SPI_BUS_2_BASE_ADDR + SPI_DR_REG_OFFSET,  // SPI_BUS 2 DR addr addr
    SPI_BUS_4_BASE_ADDR + SPI_DR_REG_OFFSET,  // SPI_BUS 4 DR addr addr
    SPI_BUS_5_BASE_ADDR + SPI_DR_REG_OFFSET,  // SPI_BUS 5 DR addr addr
};
#endif

#define TEST_DMA_TRF_WORD_NUM  4100
#define TEST_DMA_UART_BAUD     115200
#define TEST_UART_RX_BUFF_SIZE 1024
#define TEST_UART_TO_DATA_LEN  1024
#define DMA_ADDR_ALIGN_LEN     4
#define UARTH0_RX_FIFO_8_1     0

static volatile bool g_dma_trans_done = false;
static volatile bool g_dma_trans_succ = false;

static uint32_t g_dma_src_data[TEST_DMA_TRF_WORD_NUM];
static uint32_t g_dma_desc_data[TEST_DMA_TRF_WORD_NUM];
static uint32_t g_dma_lli_index = 0;
static uint8_t g_dma_uart_init_msg[] = { "\r\n[DMA] UART_H init OK.\r\n" };
static dma_channel_t dma_channel = DMA_CHANNEL_NONE;
static uint32_t g_dma_li_destindex = 0;
static uint32_t g_dma_uart_li_destindex = 0;
static char g_uart_to_dma_data[TEST_UART_TO_DATA_LEN];
static char g_dma_uart_data[] = {
    ".\"\".    .\"\",\r\n"
    "|  |   /  /\r\n"
    "|  |  /  /\r\n"
    "|  | /  /\r\n"
    "|  |/  ;-._\r\n"
    "}  ` _/  / ;\r\n"
    "|  /` ) /  /\r\n"
    "| /  /_/\\_/\\\r\n"
    "|/  /      |\r\n"
    "(  ' \\ '-  |\r\n"
    " \\    `.  /\r\n"
    "  |      |\r\n"
    "  |      |\r\n"
};
#if defined(TARGET_CHIP_BS25) && (TARGET_CHIP_BS25 == 1)
static uint8_t g_dma_test_uart = UART_BUS_0;
#else
static uint8_t g_dma_test_uart = UART_BUS_1;
#endif
static uint32_t g_dma_uart_base[UART_BUS_MAX_NUM] = {
    UART0_BASE,
    UART1_BASE,
#if UART_BUS_MAX_NUM > 2
    UART2_BASE
#endif
};
static uint8_t g_src_shaking_source[UART_BUS_MAX_NUM] = {
    HAL_DMA_HANDSHAKING_UART_L_RX,
    HAL_DMA_HANDSHAKING_UART_H0_RX,
#if UART_BUS_MAX_NUM > 2
    HAL_DMA_HANDSHAKING_UART_H1_RX
#endif
};
static uint8_t g_dst_shaking_source[UART_BUS_MAX_NUM] = {
    HAL_DMA_HANDSHAKING_UART_L_TX,
    HAL_DMA_HANDSHAKING_UART_H0_TX,
#if UART_BUS_MAX_NUM > 2
    HAL_DMA_HANDSHAKING_UART_H1_TX
#endif
};
#ifdef CONFIG_DMA_TMP
/* psram transfer define */
#define TESTSUITE_PSRAM_START_ADDR 0x0C000000

#define TESTSUIT_SPI_SEND_DATA_LEN 8
#define SPI_DR_REG_OFFSET          0x60
#define SPI_DMA_TX_DATA_LEVEL_4     4
#define QSPI_DMA_TX_DATA_LEVEL_8    8
#define TEST_DATA_LEN           16
#define TEST_DATA_ONCE_SEND_LEN 8

static volatile uint32_t const g_spi_dr_addr[SPI_BUS_MAX_NUMBER] = {
    SPI_BUS_0_BASE_ADDR + SPI_DR_REG_OFFSET,  /* SPI_BUS 0 DR addr addr */
    SPI_BUS_1_BASE_ADDR + SPI_DR_REG_OFFSET,  /* SPI_BUS 1 DR addr addr */
    SPI_BUS_2_BASE_ADDR + SPI_DR_REG_OFFSET,  /* SPI_BUS 2 DR addr addr */
    SPI_BUS_4_BASE_ADDR + SPI_DR_REG_OFFSET,  /* SPI_BUS 4 DR addr addr */
    SPI_BUS_5_BASE_ADDR + SPI_DR_REG_OFFSET,  /* SPI_BUS 5 DR addr addr */
};
#endif
static uint8_t g_uart_rx_buff[TEST_UART_RX_BUFF_SIZE] = { 0 };

static void test_dma_trans_done_callback(uint8_t int_type, uint8_t channel, uintptr_t arg)
{
    unused(arg);
    UNUSED(channel);
    PRINT("dma callback in:%u", (uint32_t)int_type);
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
    osal_printk("[DMA] int_type is %d. \r\n", int_type);
}

static void test_dma_trans_done_lli_single_callback(uint8_t int_type, uint8_t channel, uintptr_t arg)
{
    unused(arg);
    UNUSED(channel);
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
}

static void test_fill_test_buffer(void *data, uint16_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        *((uint8_t *)data + i) = (uint8_t)i;
    }
}

static void test_clear_test_buffer(void *data, uint16_t length)
{
    memset_s(data, length, 0, length);
}

static int test_check_buffer_match(void *src, void *dest, uint16_t length)
{
    return memcmp(src, dest, length);
}

static void test_uart_config(void)
{
    uapi_uart_deinit(g_dma_test_uart);
    uart_attr_t attr = {
        .baud_rate = TEST_DMA_UART_BAUD,
        .data_bits = UART_DATA_BIT_8,
        .stop_bits = UART_STOP_BIT_1,
        .parity = UART_PARITY_NONE
    };

    uart_pin_config_t pin_config = {
#if (defined(SOCMN1_PRODUCT_EVB) || defined(SOCMN1_PRODUCT_EVB_K))
        .tx_pin = ULP_GPIO0,
        .rx_pin = ULP_GPIO1,
#else
        .tx_pin = S_MGPIO0,
        .rx_pin = S_MGPIO1,
#endif
        .cts_pin = PIN_NONE,
        .rts_pin = PIN_NONE
    };

    uart_buffer_config_t g_uart_buffer_config = {
    .rx_buffer = g_uart_rx_buff,
    .rx_buffer_size = TEST_UART_RX_BUFF_SIZE
    };
    // *(volatile uint32_t *)(0x5702C300) = 0x105
    *(volatile uint32_t *)(g_dma_uart_base[g_dma_test_uart] + 0x48) = 0x03;

    errcode_t errcode = uapi_uart_init(g_dma_test_uart, &pin_config, &attr, NULL, &g_uart_buffer_config);
    hal_uart_uartifls_set_rxiflsel(g_dma_test_uart, (uint32_t)UARTH0_RX_FIFO_8_1);
    hal_uart_int_set(g_dma_test_uart, UARTIMSC, UART_INT_RTI, 0);
    hal_uart_int_set(g_dma_test_uart, UARTIMSC, UART_INT_RXI, 0);
    osal_printk("uapi_uart_init ret=%d\r\n", errcode);
    uapi_uart_write(g_dma_test_uart, g_dma_uart_init_msg, sizeof(g_dma_uart_init_msg) - 1, 0);
}

static int test_dma_mem_lli_set(int argc, char *argv[])
{
    dma_ch_user_memory_config_t transfer_config;

    if (g_dma_lli_index == 0) {
        dma_channel = uapi_dma_get_lli_channel(0, HAL_DMA_HANDSHAKING_MAX_NUM);
        if (dma_channel == DMA_CHANNEL_NONE) {
            return TEST_SUITE_TEST_FAILED;
        }
    }
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    test_fill_test_buffer((void *)(uintptr_t)g_dma_src_data, sizeof(g_dma_src_data));
    test_clear_test_buffer((void *)(uintptr_t)g_dma_desc_data, sizeof(g_dma_desc_data));
    transfer_config.src = ((uint32_t)(uintptr_t)g_dma_src_data);
    transfer_config.dest = ((uint32_t)(uintptr_t)g_dma_desc_data) + g_dma_li_destindex;
    transfer_config.transfer_num = (uint16_t)strtol(argv[0], NULL, 0);
    transfer_config.priority = 0;
    transfer_config.width = (uint8_t)strtol(argv[1], NULL, 0);
    if (uapi_dma_transfer_memory_lli(dma_channel, &transfer_config, test_dma_trans_done_callback) != ERRCODE_SUCC) {
        osal_printk("[DMA] config failed.\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    g_dma_li_destindex = g_dma_li_destindex + transfer_config.transfer_num * (1 << transfer_config.width);
    g_dma_lli_index++;
    return TEST_SUITE_OK;
}

static int test_dma_mem_to_mem_single(int argc, char *argv[])
{
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    dma_ch_user_memory_config_t transfer_config;

    osal_printk("[DMA] fill test data.\r\n");
    test_fill_test_buffer((void *)(uintptr_t)g_dma_src_data, sizeof(g_dma_src_data));
    test_clear_test_buffer((void *)(uintptr_t)g_dma_desc_data, sizeof(g_dma_desc_data));

    osal_printk("[DMA] fill config data.\r\n");
    uint8_t dma_index = 0;
    uint8_t offset = ((uint8_t)strtol(argv[dma_index++], NULL, 0)) % DMA_ADDR_ALIGN_LEN;

    transfer_config.src = ((uint32_t)(uintptr_t)g_dma_src_data) + (uint32_t)offset;
    transfer_config.dest = ((uint32_t)(uintptr_t)g_dma_desc_data) + (uint32_t)offset;
    transfer_config.transfer_num = (uint16_t)strtol(argv[dma_index++], NULL, 0);
    transfer_config.priority = 0;
    transfer_config.width = (uint8_t)strtol(argv[dma_index++], NULL, 0);

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (uapi_dma_transfer_memory_single(&transfer_config, test_dma_trans_done_callback,
        (uintptr_t)NULL) != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    osal_printk("[DMA] waitting ...\r\n");
    while (!g_dma_trans_done) { }

    osal_printk("[DMA] checking transfer from 0x%08x to 0x%08x...\r\n",
        transfer_config.src, transfer_config.dest);
    if (test_check_buffer_match((void *)transfer_config.src,
        (void *)transfer_config.dest, transfer_config.transfer_num) != 0) {
        return TEST_SUITE_TEST_FAILED;
    }

    osal_printk("[DMA] DMA memory copy test succ lengh=%d block\r\n", transfer_config.transfer_num);
    return TEST_SUITE_OK;
}

static int test_dma_lli_start(int argc, char *argv[])
{
    unused(argc);
    unused(argv);

    osal_printk("[DMA] fill test data.\r\n");

    if (g_dma_lli_index == 0) {
        osal_printk("[DMA] please set link list item by function(test_dma_mem_lli_set).\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (uapi_dma_enable_lli(dma_channel, test_dma_trans_done_callback, (uintptr_t)NULL) != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    osal_printk("[DMA] waitting ...\r\n");
    while (!g_dma_trans_done) {}

    uapi_dma_end_transfer(dma_channel);
    osal_printk("[DMA] g_dma_uart_li_destindex:%u\r\n", g_dma_uart_li_destindex);
    if (g_dma_uart_li_destindex != 0 ) {
       osal_printk("[DMA] recv:");
       uapi_uart_write(UART_BUS_0, (uint8_t*)g_dma_uart_data, g_dma_uart_li_destindex, 0);
    }
    g_dma_lli_index = 0;
    g_dma_li_destindex = 0;
    g_dma_uart_li_destindex = 0;

    if (!g_dma_trans_succ) {
        return TEST_SUITE_TEST_FAILED;
    }

    osal_printk("[DMA] transfer success ...\r\n");
    return TEST_SUITE_OK;
}

static int test_dma_uart_single(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    dma_ch_user_peripheral_config_t transfer_config;
    dma_channel_t channel;
    osal_printk("[DMA]before test_dma_uart_config.\r\n");
    test_uart_config();
    osal_printk("[DMA] fill config data.\r\n");
    transfer_config.src = ((uint32_t)g_dma_uart_data);
    transfer_config.transfer_num = (uint16_t)strlen(g_dma_uart_data);
    transfer_config.src_handshaking =  0;
    transfer_config.dest_handshaking = g_dst_shaking_source[g_dma_test_uart];
    transfer_config.dest = g_dma_uart_base[g_dma_test_uart];
    transfer_config.trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;
    transfer_config.priority = 0;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;

    g_dma_trans_done = false;
    g_dma_trans_succ = false;
    if (uapi_dma_configure_peripheral_transfer_single(&transfer_config, &channel,
        test_dma_trans_done_callback, (uintptr_t)NULL) != ERRCODE_SUCC) {
        osal_printk("[DMA] config failed.\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("[DMA] fill config over.\r\n");

    uapi_dma_start_transfer((dma_channel_t)channel);

    osal_printk("[DMA] waitting ... %d\r\n", channel);

    while (!g_dma_trans_done) { }

    if (!g_dma_trans_succ) {
        return TEST_SUITE_TEST_FAILED;
    }
    osal_printk("[DMA] transfer success ...\r\n");
    return TEST_SUITE_OK;
}

static int test_dma_uart_to_mem_single(int argc, char *argv[])
{
    if (argc != 2) { // 2 参数数量
        return TEST_SUITE_CONFIG_FAILED;
    }

    dma_ch_user_peripheral_config_t transfer_config;
    dma_channel_t channel;

    osal_printk("[DMA] init test uart bus.");
    osal_printk("[DMA] init test uart init done.");
    test_uart_config();
    osal_printk("[DMA] fill config data.");
    transfer_config.src = g_dma_uart_base[g_dma_test_uart];
    transfer_config.dest = ((uint32_t)g_uart_to_dma_data);
    transfer_config.transfer_num = (uint16_t)strtol(argv[0], NULL, 0);
    transfer_config.src_handshaking = g_src_shaking_source[g_dma_test_uart];
    transfer_config.dest_handshaking = 0;
    transfer_config.trans_type = HAL_DMA_TRANS_PERIPHERAL_TO_MEMORY_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM;
    transfer_config.priority = 0;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.burst_length = (uint16_t)strtol(argv[1], NULL, 0);
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    osal_printk("[DMA] uart to memory start, length [%u]\r\n", transfer_config.transfer_num);

    if (uapi_dma_configure_peripheral_transfer_single(&transfer_config, &channel,
        test_dma_trans_done_callback, (uintptr_t)NULL) == ERRCODE_SUCC) {
        uapi_dma_start_transfer((dma_channel_t)channel);
    } else {
        osal_printk("[DMA] *** memory to uart_l fail!\r\n");
    }

    osal_printk("[DMA] waitting ...\r\n");

    while (!g_dma_trans_done) { };

    uapi_uart_write(g_dma_test_uart, (uint8_t*)g_uart_to_dma_data, transfer_config.transfer_num, 0);
    if (g_dma_trans_succ) {
        osal_printk("-------DMA uart transfer test succ.\r\n");
    } else {
        osal_printk("-------DMA uart transfer test fail!\r\n");
    }

    return TEST_SUITE_OK;
}

static int test_dma_mem_to_uart_lli_set(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    dma_ch_user_peripheral_config_t transfer_config;

    if (g_dma_lli_index == 0) {
        test_uart_config();
        dma_channel = uapi_dma_get_lli_channel(HAL_DMA_BURST_TRANSACTION_LENGTH_16,
            g_dst_shaking_source[g_dma_test_uart]);
        if (dma_channel == DMA_CHANNEL_NONE) {
            return TEST_SUITE_TEST_FAILED;
        }
    }

    transfer_config.src = ((uint32_t)g_dma_uart_data);
    transfer_config.transfer_num = (uint16_t)strlen(g_dma_uart_data);
    transfer_config.src_handshaking =  0;
    transfer_config.dest_handshaking = g_dst_shaking_source[g_dma_test_uart];
    transfer_config.dest = g_dma_uart_base[g_dma_test_uart];
    transfer_config.trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;
    transfer_config.priority = 0;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_16;
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;
    if (uapi_dma_configure_peripheral_transfer_lli(dma_channel, &transfer_config,
        test_dma_trans_done_callback) != ERRCODE_SUCC) {
        osal_printk("[DMA] config failed.\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    g_dma_lli_index++;
    return TEST_SUITE_OK;
}

static int test_dma_uart_to_mem_lli_set(int argc, char *argv[])
{
    unused(argc);
    unused(argv);
    errcode_t ret = 0;
    dma_ch_user_peripheral_config_t transfer_config;

    if (g_dma_lli_index == 0) {
        test_uart_config();
        dma_channel = uapi_dma_get_lli_channel(HAL_DMA_BURST_TRANSACTION_LENGTH_16,
            g_src_shaking_source[g_dma_test_uart]);
        if (dma_channel == DMA_CHANNEL_NONE) {
            return TEST_SUITE_TEST_FAILED;
        }
    }

    transfer_config.src = g_dma_uart_base[g_dma_test_uart];
    // transfer_config.transfer_num = (uint16_t)strlen(g_dma_uart_data)
    transfer_config.transfer_num = 4;
    transfer_config.src_handshaking = g_src_shaking_source[g_dma_test_uart];
    transfer_config.dest_handshaking =  0;
    transfer_config.dest = ((uint32_t)g_dma_uart_data) + g_dma_uart_li_destindex;
    transfer_config.trans_type = HAL_DMA_TRANS_PERIPHERAL_TO_MEMORY_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM;
    transfer_config.priority = 0;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_16;
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;
    ret = uapi_dma_configure_peripheral_transfer_lli(dma_channel, &transfer_config,
        test_dma_trans_done_callback);
    if (ret != ERRCODE_SUCC) {
        osal_printk("[DMA] config failed %u.\r\n", (uint32_t)ret);
        return TEST_SUITE_TEST_FAILED;
    }

    g_dma_lli_index++;
    g_dma_uart_li_destindex = g_dma_uart_li_destindex + transfer_config.transfer_num;
    return TEST_SUITE_OK;
}

#ifdef CONFIG_DMA_TMP
static int test_dma_mem_to_psram_single(int argc, char *argv[])
{
    uint32_t src_addr = TESTSUITE_PSRAM_START_ADDR;
#if (SPI_WITH_OPI == YES)
    if (!psram_xip_enabled()) {
        osal_printk("[DMA] opi xip is not enable.\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }
#endif
    dma_ch_user_memory_config_t transfer_config;

    osal_printk("[DMA] fill test data.\r\n");
    test_fill_test_buffer((void *)g_dma_src_data, sizeof(g_dma_src_data));
    test_clear_test_buffer((void *)(uintptr_t)(src_addr), sizeof(g_dma_desc_data));

    osal_printk("[DMA] fill config data.\r\n");
    transfer_config.src = ((uint32_t)g_dma_src_data);
    transfer_config.dest = ((uint32_t)src_addr);
    transfer_config.transfer_num = (MEMORY_COPY_TEST_LENGTH);
    transfer_config.priority = HAL_DMA_CH_PRIORITY_0;

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (uapi_dma_transfer_memory_single(0, &transfer_config, test_dma_trans_done_callback) != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    osal_printk("[DMA] waitting ... ");
    while (!g_dma_trans_done);

    osal_printk("[DMA] checking transfer from 0x%08x to 0x%08x...\r\n",
        transfer_config.src, transfer_config.dest);
    if (test_check_buffer_match((void *)transfer_config.src,
        (void *)transfer_config.dest, MEMORY_COPY_TEST_LENGTH) != 0) {
        osal_printk("***DMA_MemTOMem_Test fail!!!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    unused(argc);
    unused(argv);
    return TEST_SUITE_OK;
}

static int test_dma_psram_to_mem_single(int argc, char *argv[])
{
    uint32_t src_addr = TESTSUITE_PSRAM_START_ADDR;
#if (SPI_WITH_OPI == YES)
    if (!psram_xip_enabled()) {
        osal_printk("[DMA] opi xip is not enable.\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }
#endif
    dma_ch_user_memory_config_t transfer_config;

    osal_printk("[DMA] fill test data.\r\n");
    test_fill_test_buffer((void *)(uintptr_t)(src_addr), sizeof(g_dma_src_data));
    test_clear_test_buffer((void *)g_dma_desc_data, sizeof(g_dma_desc_data));

    osal_printk("[DMA] fill config data.\r\n");
    transfer_config.src = ((uint32_t)src_addr);
    transfer_config.dest = ((uint32_t)(uintptr_t)g_dma_desc_data);
    transfer_config.transfer_num = (MEMORY_COPY_TEST_LENGTH);
    transfer_config.priority = HAL_DMA_CH_PRIORITY_0;

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (uapi_dma_transfer_memory_single(0, &transfer_config, test_dma_trans_done_callback) != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    osal_printk("[DMA] waitting ...\r\n");
    while (!g_dma_trans_done);

    osal_printk("[DMA] checking transfer from 0x%08x to 0x%08x...\r\n", transfer_config.src, transfer_config.dest);
    if (test_check_buffer_match((void *)transfer_config.src,
        (void *)transfer_config.dest, MEMORY_COPY_TEST_LENGTH) != 0) {
        osal_printk("***DMA_MemTOMem_Test fail!!!\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    unused(argc);
    unused(argv);
    return TEST_SUITE_OK;
}

static void test_dma_spi_config_lli(void)
{
    uint32_t frame_size = 0;
    spi_config_t config;

    config.clk_in_mhz = 1;
    config.trans_interface = SPI_INTERFACE_STANDARD;
    config.device_mode = SPI_DEVICE_MASTER;
    config.frame_size = SPI_FRAME_SIZE_8;
    config.slave_num = 0;

    spi_init();
    spi_claim(SPI_BUS_1);
    spi_ret_t ret = spi_open(SPI_BUS_1, config);
    if (ret != SPI_RET_OK) {
        PRINT("config failed, ret: %d\r\n", ret);
    }
    PRINT("config success. \r\n");

    spi_set_dma_data_level(SPI_BUS_1, SPI_DMA_TX_DATA_LEVEL, SPI_DMA_TX_DATA_LEVEL_4);
    spi_dma_control(SPI_BUS_1, SPI_DMA_CONTROL_TX_ENABLE);
}

static int test_dma_spi_lli_set(int argc, char *argv[])
{
    pin_t pin;
    pin_mode_t mode;
    dma_ch_user_peripheral_config_t transfer_config;

    /* Get the parameters */
    if (argc == 2) { /* 2: Indicates the number of input parameters */
        uint32_t param_index = 0;
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    if (g_dma_lli_index == 0) {
        uapi_pin_set_mode(pin, mode);
        test_dma_spi_config_lli();
        dma_channel = uapi_dma_get_lli_channel(transfer_config.burst_length, transfer_config.dest_handshaking);
        if (dma_channel == DMA_CHANNEL_NONE) {
            return TEST_SUITE_TEST_FAILED;
        }
    }

    transfer_config.src = ((uint32_t)g_dma_uart_data);
    transfer_config.transfer_num = (uint16_t)strlen(g_dma_uart_data);
    transfer_config.src_handshaking =  0;
    transfer_config.dest_handshaking =  DMA_HANDSHAKE_SPI_BUS_1_TX;
    transfer_config.dest = g_spi_dr_addr[SPI_BUS_1];
    transfer_config.trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    transfer_config.trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;
    transfer_config.priority = 0;
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config.burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    transfer_config.src_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.dest_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config.protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;
    if (uapi_dma_configure_peripheral_transfer_lli(dma_channel, &transfer_config, NULL) != ERRCODE_SUCC) {
        PRINT("[DMA] config failed.\r\n");
        return TEST_SUITE_TEST_FAILED;
    }
    g_dma_lli_index++;
    return TEST_SUITE_OK;
}

static void spi_rx_config(spi_bus_t bus, dma_ch_user_peripheral_config_t *transfer_config)
{
    transfer_config->trans_type = HAL_DMA_TRANS_PERIPHERAL_TO_MEMORY_DMA;
    transfer_config->trans_dir = HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM;
    transfer_config->priority = 0;
    transfer_config->src_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config->dest_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config->burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_1;
    transfer_config->src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config->dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config->dest_handshaking = 0;
    transfer_config->protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;

    switch (bus) {
        case SPI_BUS_0:
            transfer_config->src_handshaking = DMA_HANDSHAKE_SPI_BUS_0_RX;
            break;
        case SPI_BUS_1:
            transfer_config->src_handshaking = DMA_HANDSHAKE_SPI_BUS_1_RX;
            break;
        case SPI_BUS_2:
            transfer_config->src_handshaking = DMA_HANDSHAKE_SPI_BUS_2_RX;
            break;
        case SPI_BUS_4:
            transfer_config->src_handshaking = DMA_HANDSHAKE_SPI_BUS_4_RX;
            break;
        case SPI_BUS_5:
            transfer_config->src_handshaking = DMA_HANDSHAKE_SPI_BUS_5_RX;
            break;
        default:
            break;
    }
}

static int test_spi_dma_rx_config(spi_bus_t bus, void *dest_addr, uint32_t len)
{
    dma_ch_user_peripheral_config_t transfer_config;
    dma_channel_t channel = DMA_CHANNEL_NONE;

    transfer_config.src = g_spi_dr_addr[bus];
    transfer_config.dest = (uint32_t)(uintptr_t)dest_addr;
    transfer_config.transfer_num = (uint16_t)len;

    spi_rx_config(bus, &transfer_config);

    if (uapi_dma_configure_peripheral_transfer_single(&transfer_config, &channel,
        (hal_dma_transfer_cb_t)test_dma_trans_done_callback, (uintptr_t)NULL) != ERRCODE_SUCC) {
        PRINT("[DMA] config failed.\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }

    spi_dma_control(bus, SPI_DMA_CONTROL_RX_ENABLE);
    spi_set_dma_data_level(bus, SPI_DMA_RX_DATA_LEVEL, 0);

    PRINT("[DMA] fill config over." NEWLINE);

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    uapi_dma_start_transfer((dma_channel_t)channel);

    PRINT("[DMA] waitting ... " NEWLINE);

    while (!g_dma_trans_done) {
    }

    if (!g_dma_trans_succ) {
        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("[DMA] transfer success ... " NEWLINE);
    return TEST_SUITE_OK;
}

static void spi_send_config(spi_bus_t bus, dma_ch_user_peripheral_config_t *transfer_config)
{
    transfer_config->src_handshaking =  0;
    transfer_config->trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    transfer_config->trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;
    transfer_config->priority = 0;
    transfer_config->src_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config->dest_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    transfer_config->burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    transfer_config->src_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config->dest_width = HAL_DMA_TRANSFER_WIDTH_8;
    transfer_config->protection = HAL_DMA_PROTECTION_CONTROL_BUFFERABLE;

    switch (bus) {
        case SPI_BUS_0:
            transfer_config->dest_handshaking = DMA_HANDSHAKE_SPI_BUS_0_TX;
            spi_set_dma_data_level(bus, SPI_DMA_TX_DATA_LEVEL, SPI_DMA_TX_DATA_LEVEL_4);
            break;
        case SPI_BUS_1:
            transfer_config->dest_handshaking = DMA_HANDSHAKE_SPI_BUS_1_TX;
            spi_set_dma_data_level(bus, SPI_DMA_TX_DATA_LEVEL, SPI_DMA_TX_DATA_LEVEL_4);
            break;
        case SPI_BUS_2:
            transfer_config->dest_handshaking = DMA_HANDSHAKE_SPI_BUS_2_TX;
            spi_set_dma_data_level(bus, SPI_DMA_TX_DATA_LEVEL, SPI_DMA_TX_DATA_LEVEL_4);
            break;
        case SPI_BUS_4:
            transfer_config->dest_handshaking = DMA_HANDSHAKE_SPI_BUS_4_TX;
            spi_set_dma_data_level(bus, SPI_DMA_TX_DATA_LEVEL, QSPI_DMA_TX_DATA_LEVEL_8);
            transfer_config->burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_8;
            break;
        case SPI_BUS_5:
            transfer_config->dest_handshaking = DMA_HANDSHAKE_SPI_BUS_5_TX;
            spi_set_dma_data_level(bus, SPI_DMA_TX_DATA_LEVEL, QSPI_DMA_TX_DATA_LEVEL_8);
            transfer_config->burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_8;
            break;
        default:
            break;
    }
}

static int test_spi_send_by_dma(spi_bus_t bus, void *src_addr, uint32_t len)
{
    hal_spi_disable(bus);
    hal_spi_set_trans_mode(bus, HAL_SPI_TRANS_MODE_TX);
    hal_spi_enable(bus);
    dma_ch_user_peripheral_config_t transfer_config;
    dma_channel_t channel = DMA_CHANNEL_NONE;

    transfer_config.src = (uint32_t)(uintptr_t)src_addr;
    transfer_config.dest = g_spi_dr_addr[bus];
    transfer_config.transfer_num = (uint16_t)len;

    spi_send_config(bus, &transfer_config);

    if (uapi_dma_configure_peripheral_transfer_single(&transfer_config, &channel,
        (hal_dma_transfer_cb_t)test_dma_trans_done_callback, (uintptr_t)NULL) != ERRCODE_SUCC) {
        PRINT("[DMA] config failed.\r\n");
        return TEST_SUITE_CONFIG_FAILED;
    }

    spi_dma_control(bus, SPI_DMA_CONTROL_TX_ENABLE);

    PRINT("[DMA] fill config over." NEWLINE);

    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    uapi_dma_start_transfer((dma_channel_t)channel);

    PRINT("[DMA] waitting ... " NEWLINE);

    while (!g_dma_trans_done) {
    }

    if (!g_dma_trans_succ) {
        return TEST_SUITE_TEST_FAILED;
    }
    PRINT("[DMA] transfer success ... " NEWLINE);
    return TEST_SUITE_OK;
}

static int test_dma_mem_to_spi_single(int argc, char *argv[])
{
    spi_bus_t bus = SPI_BUS_NONE;
    uint32_t frame_size = 0;
    spi_interface_t frame_format = SPI_INTERFACE_STANDARD;
    spi_config_t config;
    uint32_t send_data[TEST_DATA_LEN] = { 0 };
    pin_t pin;
    pin_mode_t mode;

    /* Get the parameters */
    if (argc == 4) { /* 4: Indicates the number of input parameters */
        uint32_t param_index = 0;
        bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
        frame_size = (uint32_t)strtol(argv[param_index++], NULL, 0);
        frame_size -= 1;
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("SPI Bus: %d, frame_size: %d, frame_format: %d. \r\n", bus, frame_size + 1, frame_format);

    config.clk_in_mhz = 1;
    config.trans_interface = frame_format;
    config.device_mode = SPI_DEVICE_MASTER;
    config.frame_size = frame_size;
    config.slave_num = 0;

    spi_init();
    spi_claim(bus);
    uapi_pin_set_mode(pin, mode);
    spi_ret_t ret = spi_open(bus, config);
    if (ret != SPI_RET_OK) {
        PRINT("config failed, ret: %d\r\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    PRINT("config success. \r\n");

    send_data[0] = 0x38;
    send_data[1] = 0x00;
    test_fill_test_buffer(&send_data[TEST_PARAM_ARGC_2], sizeof(send_data) - TEST_DATA_ONCE_SEND_LEN);

    return test_spi_send_by_dma(bus, (uint8_t *)send_data, sizeof(send_data));
}

static int test_dma_spi_to_mem_single(int argc, char *argv[])
{
    spi_bus_t bus = SPI_BUS_NONE;
    uint32_t frame_size = 0;
    spi_interface_t frame_format = SPI_INTERFACE_STANDARD;
    spi_config_t config;
    uint8_t recv_data[TESTSUIT_SPI_SEND_DATA_LEN] = { 0 };
    pin_t pin;
    pin_mode_t mode;

    /* Get the parameters */
    if (argc == 4) { /* 4: Indicates the number of input parameters */
        uint32_t param_index = 0;
        bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
        frame_size = (uint32_t)strtol(argv[param_index++], NULL, 0);
        frame_size -= 1;
        pin = (pin_t)strtol(argv[param_index++], NULL, 0);
        mode = (pin_mode_t)strtol(argv[param_index++], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    PRINT("SPI Bus: %d, frame_size: %d, frame_format: %d. \r\n", bus, frame_size + 1, frame_format);

    config.clk_in_mhz = 1;
    config.trans_interface = frame_format;
    config.device_mode = SPI_DEVICE_SLAVE;
    config.frame_size = frame_size;
    config.slave_num = 0;

    spi_init();
    spi_claim(bus);
    uapi_pin_set_mode(pin, mode);
    spi_ret_t ret = spi_open(bus, config);
    if (ret != SPI_RET_OK) {
        PRINT("config failed, ret: %d\r\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    PRINT("config success. \r\n");

    test_clear_test_buffer((void *)recv_data, sizeof(recv_data));

    return test_spi_dma_rx_config(bus, (uint8_t *)recv_data, sizeof(recv_data));
}
#endif

#define TEST_DMA_UART_CONFIG_PARAM_NUM 1
static int test_dma_uart_config(int argc, char *argv[])
{
    if (argc != TEST_DMA_UART_CONFIG_PARAM_NUM) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t param_index = 0;
    uint8_t bus = (uint8_t)strtol(argv[param_index], NULL, 0);
    if (bus >= UART_BUS_MAX_NUM) {
        PRINT("bus is not exsit, id[%u]\r\n", bus);
        return TEST_SUITE_CONFIG_FAILED;
    }
    g_dma_test_uart = bus;
    PRINT("uart id %u, uart base %x, src shake %u, dst shake %u\r\n", g_dma_test_uart,
        g_dma_uart_base[g_dma_test_uart], g_src_shaking_source[g_dma_test_uart], g_dst_shaking_source[g_dma_test_uart]);
    return TEST_SUITE_OK;
}

static int test_dma_mem_lli_single(int argc, char *argv[])
{
    dma_ch_user_memory_config_t transfer_config;

    dma_channel = uapi_dma_get_lli_channel(0, HAL_DMA_HANDSHAKING_MAX_NUM);
    if (dma_channel == DMA_CHANNEL_NONE) {
        return TEST_SUITE_TEST_FAILED;
    }
    if (argc != 3) { /* 3: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint16_t length = (uint16_t)strtol(argv[0], NULL, 0);
    uint8_t width = (uint8_t)strtol(argv[1], NULL, 0);
    test_fill_test_buffer((void *)(uintptr_t)g_dma_src_data, sizeof(g_dma_src_data));
    test_clear_test_buffer((void *)(uintptr_t)g_dma_desc_data, sizeof(g_dma_desc_data));
    uint16_t loop = (uint16_t)strtol(argv[2], NULL, 0); // 2: num 2
    uint64_t start_us = uapi_tcxo_get_us();
    for (uint16_t i = 0; i < loop; i++) {
        transfer_config.src = ((uint32_t)(uintptr_t)g_dma_src_data);
        transfer_config.dest = ((uint32_t)(uintptr_t)g_dma_desc_data) + g_dma_li_destindex;
        transfer_config.transfer_num = length;
        transfer_config.priority = 0;
        transfer_config.width = width;
        if (uapi_dma_transfer_memory_lli(dma_channel, &transfer_config,
            test_dma_trans_done_lli_single_callback) != ERRCODE_SUCC) {
            osal_printk("[DMA] config failed.\r\n");
            return TEST_SUITE_TEST_FAILED;
        }
    }
    uint64_t sec_us = uapi_tcxo_get_us();
    g_dma_trans_done = false;
    g_dma_trans_succ = false;

    if (uapi_dma_enable_lli(dma_channel, test_dma_trans_done_lli_single_callback, (uintptr_t)NULL) != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }
    uint64_t middle_us = uapi_tcxo_get_us();
    while (!g_dma_trans_done) {}
    uint64_t end_us = uapi_tcxo_get_us();
    uapi_dma_end_transfer(dma_channel);
    uint64_t last_us = uapi_tcxo_get_us();
    if (!g_dma_trans_succ) {
        return TEST_SUITE_TEST_FAILED;
    }
    osal_printk("lli end time %llu, hardware time %llu, enable time %llu, add lli time %llu\r\n", (last_us - end_us),
                (end_us - middle_us), (middle_us - sec_us), (sec_us - start_us));
    osal_printk("length %u, loop %u\r\n", length * loop, loop);
    return TEST_SUITE_OK;
}

void add_dma_test_case(void)
{
    uapi_dma_init();
    uapi_dma_open();
    uapi_test_suite_add_function("test_dma_uart_config", "DMA test for uart config"
                                 "Params: <bus_id(0|1|2)>", test_dma_uart_config);
    uapi_test_suite_add_function("test_dma_mem_to_mem", "DMA test for memory to memory."
                                 "Params: <offset(0|1|2|3)>, <transfer num(0~4095)>"
                                 ", <width(0|1|2)>", test_dma_mem_to_mem_single);
    uapi_test_suite_add_function("test_dma_mem_to_uart", "DMA test for memory to uart.", test_dma_uart_single);
    uapi_test_suite_add_function("test_dma_uart_to_mem", "Params:<len:0~1023> <busrt:0~7> DMA test for uart to memory.",
        test_dma_uart_to_mem_single);
    uapi_test_suite_add_function("test_dma_mem_lli_set", "DMA test for set link list item of memory to memory."
                                 "Params: <transfer num(0~4095)>, <width(0|1|2)>", test_dma_mem_lli_set);
    uapi_test_suite_add_function("test_dma_mem_to_uart_lli_set", "DMA test for memory to uart.",
                                 test_dma_mem_to_uart_lli_set);
    uapi_test_suite_add_function("test_dma_uart_to_mem_lli_set", "DMA test for uart to memory.",
                                 test_dma_uart_to_mem_lli_set);
    uapi_test_suite_add_function("test_dma_lli_start", "DMA test for memory to memory.", test_dma_lli_start);
    uapi_test_suite_add_function("test_dma_mem_lli_single", "DMA test for set link list item of memory to memory single"
                                "Params: <transfer num(0~4095)>, <width(0|1|2)>", test_dma_mem_lli_single);
#ifdef CONFIG_DMA_TMP
    uapi_test_suite_add_function("test_dma_mem_to_psram", "DMA test for memory to psram.",
                                 test_dma_mem_to_psram_single);
    uapi_test_suite_add_function("test_dma_psram_to_mem", "DMA test for psram to memory.",
                                 test_dma_psram_to_mem_single);
    uapi_test_suite_add_function("dma_mem_to_spi", "DMA test for memory to spi."
                                 "Params: <bus>, <frame size>, <pin number>, <pin mode>", test_dma_mem_to_spi_single);
    uapi_test_suite_add_function("dma_spi_to_mem", "DMA test for spi to memory."
                                 "Params: <bus>, <frame size>, <pin number>, <pin mode>", test_dma_spi_to_mem_single);
    uapi_test_suite_add_function("test_dma_spi_lli_set", "DMA test for memory to spi.",
                                 test_dma_spi_lli_set);
#endif
}
