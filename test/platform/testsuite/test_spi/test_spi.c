/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved. \n
 *
 * Description: Test spi source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-11-5, Create file. \n
 */
#include "securec.h"
#include "osal_debug.h"
#include "osal_addr.h"
#include "tcxo.h"
#include "timer.h"
#include "arch_port.h"
#include "spi.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_spi.h"

static uint8_t *g_w_data = NULL;
static uint8_t *g_r_data = NULL;
static uint64_t g_tx_time_before = 0;
static uint64_t g_rx_time_before = 0;
static bool g_int_mode_is_enable = false;

static bool g_spi_bus_inited[SPI_BUS_MAX_NUMBER] = { SPI_BUS_NONE };

#define SPI_SEND_DATA_SIZE 0x07
#define SPI_SEND_DATA_TIMER_COUNT 1000000

static void test_spi_show_parameters_attr(spi_bus_t bus, spi_attr_t *attr)
{
    osal_printk("SPI Parameters:\n");
    osal_printk("    bus: %d\n", bus);
    osal_printk("    attr->is_slave: %d\n", attr->is_slave);
    osal_printk("    attr->slave_num: %d\n", attr->slave_num);
    osal_printk("    attr->bus_clk: %d\n", attr->bus_clk);
    osal_printk("    attr->freq_mhz: %d\n", attr->freq_mhz);
    osal_printk("    attr->clk_polarity: %d\n", attr->clk_polarity);
    osal_printk("    attr->clk_phase: %d\n", attr->clk_phase);
    osal_printk("    attr->frame_format: %d\n", attr->frame_format);
    osal_printk("    attr->spi_frame_format: %d\n", attr->spi_frame_format);
    osal_printk("    attr->frame_size: %d\n", attr->frame_size);
    osal_printk("    attr->tmod: %d\n", attr->tmod);
    osal_printk("    attr->sste: %d\n", attr->sste);
    osal_printk("    attr->ndf: %d\n\n", attr->ndf);
}

static void test_spi_show_parameters_extra_attr(spi_extra_attr_t *extra_attr)
{
    osal_printk("    extra_attr->tx_use_dma: %d\n", extra_attr->tx_use_dma);
    osal_printk("    extra_attr->rx_use_dma: %d\n", extra_attr->rx_use_dma);
    osal_printk("    extra_attr->qspi_param.trans_type: %d\n", extra_attr->qspi_param.trans_type);
    osal_printk("    extra_attr->qspi_param.inst_len: %d\n", extra_attr->qspi_param.inst_len);
    osal_printk("    extra_attr->qspi_param.addr_len: %d\n", extra_attr->qspi_param.addr_len);
    osal_printk("    extra_attr->qspi_param.wait_cycles: %d\n", extra_attr->qspi_param.wait_cycles);
}

static int test_spi_init(int argc, char *argv[])
{
    unused(argc);

    uint32_t param_index = 0;
    spi_bus_t bus;
    spi_attr_t attr;
    spi_extra_attr_t extra_attr;

    bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
    attr.is_slave = (bool)strtol(argv[param_index++], NULL, 0);
    attr.slave_num = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.bus_clk = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.freq_mhz = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.clk_polarity = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.clk_phase = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.frame_format = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.spi_frame_format = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.frame_size = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.tmod = (uint32_t)strtol(argv[param_index++], NULL, 0);
    attr.ndf = 1;

    extra_attr.tx_use_dma = (bool)strtol(argv[param_index++], NULL, 0);
    extra_attr.rx_use_dma = (bool)strtol(argv[param_index++], NULL, 0);
    extra_attr.qspi_param.trans_type = (hal_spi_trans_type_t)strtol(argv[param_index++], NULL, 0);
    extra_attr.qspi_param.inst_len = (hal_spi_inst_len_t)strtol(argv[param_index++], NULL, 0);
    extra_attr.qspi_param.addr_len = (hal_spi_addr_len_t)strtol(argv[param_index++], NULL, 0);
    extra_attr.qspi_param.wait_cycles = (uint32_t)strtol(argv[param_index++], NULL, 0);

    if (argc >= 18) { // 第18个参数作为 sste, 新增参数
        attr.sste = (uint32_t)strtol(argv[param_index++], NULL, 0);
    } else {
        attr.sste = 0;
    }

    test_spi_show_parameters_attr(bus, &attr);
    test_spi_show_parameters_extra_attr(&extra_attr);
#ifndef TEST_SPI_PINMUX_MANUAL
    spi_porting_test_spi_init_pin();
#endif
    errcode_t ret = uapi_spi_init(bus, &attr, &extra_attr);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    g_spi_bus_inited[bus] = true;
    return TEST_SUITE_OK;
}

static int test_spi_deinit(int argc, char *argv[])
{
    unused(argc);

    spi_bus_t bus;
    bus = (spi_bus_t)strtol(argv[0], NULL, 0);

    errcode_t ret = uapi_spi_deinit(bus);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    g_spi_bus_inited[bus] = false;
    return TEST_SUITE_OK;
}

#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) && (CONFIG_SPI_SUPPORT_INTERRUPT == 1)
static void test_spi_write_int_handler(const void *buffer, uint32_t length)
{
    osal_printk("uapi spi interrupt write time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_tx_time_before), length);

    unused(buffer);
    unused(length);
    osal_printk("spi master write interrupt start!\n");
}

static void spi_rx_callback(const void *buffer, uint32_t length, bool error)
{
    if (error) {
        osal_printk("test_spi_master_read_int error!!!\r\n");
        return;
    }
    osal_printk("uapi spi interrupt read time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_rx_time_before), length);

    osal_printk("test_spi_master_read_int success!!!\r\n");
    uint8_t *buf = (uint8_t *)buffer;
    for (uint32_t i = 0; i < length; i++) {
        osal_printk("%x ", buf[i]);
    }
    osal_printk("\n");
}
#endif  /* CONFIG_SPI_SUPPORT_INTERRUPT */

#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
static void test_spi_clear_transfer_mode(spi_bus_t bus, uint8_t mode)
{
    if (mode == 1) { /* 1: dma mode */
#if defined(CONFIG_SPI_SUPPORT_DMA) && (CONFIG_SPI_SUPPORT_DMA == 1)
        spi_dma_config_t dma_cfg = { 0 };
        uapi_spi_set_dma_mode(bus, false, &dma_cfg);
#endif  /* CONFIG_SPI_SUPPORT_DMA */
    } else if (mode == 2) { /* 2: interrupt mode */
#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) && (CONFIG_SPI_SUPPORT_INTERRUPT == 1)
        uapi_spi_set_irq_mode(bus, false, NULL, NULL);
#endif  /* CONFIG_SPI_SUPPORT_INTERRUPT */
    }
    unused(bus);
}

static int test_spi_set_transfer_mode(spi_bus_t bus, int argc, char *argv[], uint8_t mode, uint32_t index)
{
    static uint8_t flag_mode = 0;
    test_spi_clear_transfer_mode(bus, flag_mode);
    if (mode == 1) { /* 1: dma mode */
#if defined(CONFIG_SPI_SUPPORT_DMA) && (CONFIG_SPI_SUPPORT_DMA == 1)
        if (argc < (int)index + 3) { /* 3: Indicates the number of input dma parameters */
            return TEST_SUITE_ERROR_BAD_PARAMS;
        }
        uint32_t width = (uint32_t)strtol(argv[index++], NULL, 0);
        uint8_t burst_length = (uint8_t)strtol(argv[index++], NULL, 0);
        uint8_t priority = (uint8_t)strtol(argv[index++], NULL, 0);

        spi_dma_config_t dma_cfg = {
            .src_width = width,
            .dest_width = width,
            .burst_length = burst_length,
            .priority = priority
        };

        if (uapi_spi_set_dma_mode(bus, true, &dma_cfg) != ERRCODE_SUCC) {
            osal_printk("[SPI] *** memory to spi fail!\r\n");
            return TEST_SUITE_TEST_FAILED;
        }
        osal_printk("spi transfer mode: DMA\n");
#endif  /* CONFIG_SPI_SUPPORT_DMA */
    } else if (mode == 2) { /* 2: interrupt mode */
#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) && (CONFIG_SPI_SUPPORT_INTERRUPT == 1)
        if (uapi_spi_set_irq_mode(bus, true, spi_rx_callback, test_spi_write_int_handler) != ERRCODE_SUCC) {
            return TEST_SUITE_TEST_FAILED;
        }
        g_int_mode_is_enable = true;
        osal_printk("spi transfer mode: INTERRUPT\n");
#endif  /* CONFIG_SPI_SUPPORT_INTERRUPT */
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }

    flag_mode = mode;

    unused(argc);
    unused(argv);
    unused(index);
    return TEST_SUITE_OK;
}
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

static int test_spi_transfer_prepare(int argc, char *argv[], spi_bus_t *bus, uint32_t *data_len)
{
    uint32_t param_index = 0;
    *bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
    if (!g_spi_bus_inited[*bus]) {
        return TEST_SUITE_TEST_FAILED;
    }
    bool long_data = (bool)strtol(argv[param_index++], NULL, 0);
    *data_len = (uint32_t)strtol(argv[param_index++], NULL, 0);

    g_w_data = (uint8_t *)osal_kmalloc(*data_len, 0);
    if (g_w_data == NULL) {
        return TEST_SUITE_ALLOC_FAILED;
    }

    if (long_data == true) {
        uint8_t data = (uint8_t)strtol(argv[param_index++], NULL, 0);
        (void)memset_s(g_w_data, *data_len, data, *data_len);
    } else {
        if ((argc - param_index) < *data_len) {
            return TEST_SUITE_ERROR_BAD_PARAMS;
        }
        for (uint32_t i = 0; i < *data_len; i++) {
            g_w_data[i] = (uint8_t)strtol(argv[param_index++], NULL, 0);
        }
    }

#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
#if defined(CONFIG_SPI_SUPPORT_INTERRUPT) || defined(CONFIG_SPI_SUPPORT_DMA)
    if (argc > (int)param_index) {
        uint8_t mode = (uint8_t)strtol(argv[param_index++], NULL, 0);
        if (test_spi_set_transfer_mode(*bus, argc, argv, mode, param_index) != TEST_SUITE_OK) {
            return TEST_SUITE_TEST_FAILED;
        }
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* CONFIG_SPI_SUPPORT_INTERRUPT || CONFIG_SPI_SUPPORT_DMA */
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */
    return TEST_SUITE_OK;
}

static int test_spi_master_send_poll(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len,
        .rx_buff = NULL,
        .rx_bytes = 0,
        .cmd = 0xA5,
        .addr = 0x5A5A5A
    };

    g_tx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_write(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi master write failed, ret = %d\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
    if (!g_int_mode_is_enable) {
        osal_printk("uapi spi master write time: %uus, length: %u\r\n",
                    (uint32_t)(uapi_tcxo_get_us() - g_tx_time_before), data_len);
	}

    return TEST_SUITE_OK;
}

static int test_spi_master_read_poll(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

    g_rx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_read(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_master_read_poll failed, ret = %d\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("uapi spi master read time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_rx_time_before), data_len);

    for (uint32_t i = 0; i < data_len; i++) {
        osal_printk("Read: 0x%0x\n", g_r_data[i]);
    }

    return TEST_SUITE_OK;
}

static int test_spi_master_write(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len,
        .rx_buff = NULL,
        .rx_bytes = 0,
        .cmd = 0xA5,
        .addr = 0x5A5A5A
    };

#if defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH) && (CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH == 1)
    if (data.tx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD) {
        osal_printk("spi transfer mode: DMA\n");
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

    g_tx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_write(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi master write failed, ret = %d\n", ret);
        osal_kfree(g_w_data);
        return TEST_SUITE_TEST_FAILED;
    }
    osal_printk("uapi spi master write time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_tx_time_before), data_len);

    osal_kfree(g_w_data);
    return TEST_SUITE_OK;
}

static int test_spi_read_param_pre(spi_bus_t bus, uint32_t data_len)
{
    if (!g_spi_bus_inited[bus]) {
        return TEST_SUITE_TEST_FAILED;
    }
    g_r_data = (uint8_t *)osal_kmalloc(data_len, 0);
    if (g_r_data == NULL) {
        return TEST_SUITE_ALLOC_FAILED;
    }
    return TEST_SUITE_OK;
}

static int test_spi_master_read(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    spi_bus_t bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
    uint32_t data_len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    int ret_read = test_spi_read_param_pre(bus, data_len);
    if (ret_read != TEST_SUITE_OK) {
        return ret_read;
    }

    spi_xfer_data_t data = {
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
    if (argc > (int)param_index) {
        uint8_t mode = (uint8_t)strtol(argv[param_index++], NULL, 0);
        if (test_spi_set_transfer_mode(bus, argc, argv, mode, param_index) != TEST_SUITE_OK) {
            osal_kfree(g_r_data);
            return TEST_SUITE_TEST_FAILED;
        }
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#else
    if (data.rx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD) {
        osal_printk("spi transfer mode: DMA\n");
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

    g_rx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_read(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_master_read_poll failed, ret = %d\n", ret);
        osal_kfree(g_r_data);
        return TEST_SUITE_CONFIG_FAILED;
    }
    if (!g_int_mode_is_enable) {
        osal_printk("uapi spi master read time: %uus, length: %u\r\n",
                    (uint32_t)(uapi_tcxo_get_us() - g_rx_time_before), data_len);

        osal_printk("Read: ");
        for (uint32_t i = 0; i < data_len; i++) {
            osal_printk("%0x ", g_r_data[i]);
        }
        osal_printk("\n");
    }

    osal_kfree(g_r_data);
    return TEST_SUITE_OK;
}

static int test_spi_slave_write(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len
    };

#if defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH) && (CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH == 1)
    if (data.tx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD) {
        osal_printk("spi transfer mode: DMA\n");
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

    g_tx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_slave_write(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi slave write failed, ret = %d\n", ret);
        osal_kfree(g_w_data);
        return TEST_SUITE_CONFIG_FAILED;
    }
    if (!g_int_mode_is_enable) {
        osal_printk("uapi spi slave write time: %uus, length: %u\r\n",
                    (uint32_t)(uapi_tcxo_get_us() - g_tx_time_before), data_len);
    }

    osal_kfree(g_w_data);
    return TEST_SUITE_OK;
}

static int test_spi_slave_read(int argc, char *argv[])
{
    unused(argc);
    uint32_t param_index = 0;
    spi_bus_t bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
    uint32_t data_len = (uint32_t)strtol(argv[param_index++], NULL, 0);
    int ret_read = test_spi_read_param_pre(bus, data_len);
    if (ret_read != TEST_SUITE_OK) {
        return ret_read;
    }

    spi_xfer_data_t data = {
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

#if !defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH)
    if (argc > (int)param_index) {
        uint8_t mode = (uint8_t)strtol(argv[param_index++], NULL, 0);
        if (test_spi_set_transfer_mode(bus, argc, argv, mode, param_index) != TEST_SUITE_OK) {
            osal_kfree(g_r_data);
            return TEST_SUITE_TEST_FAILED;
        }
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#else
    if (data.rx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD) {
        osal_printk("spi transfer mode: DMA\n");
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* NOT CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

    g_rx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_slave_read(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_slave_read failed, ret = %d\n", ret);
        osal_kfree(g_r_data);
        return TEST_SUITE_CONFIG_FAILED;
    }
    if (!g_int_mode_is_enable) {
        osal_printk("uapi spi slave read time: %uus, length: %u\r\n",
                    (uint32_t)(uapi_tcxo_get_us() - g_rx_time_before), data_len);

        osal_printk("Read: ");
        for (uint32_t i = 0; i < data_len; i++) {
            osal_printk("%0x ", g_r_data[i]);
        }
        osal_printk("\n");
    }

    osal_kfree(g_r_data);
    return TEST_SUITE_OK;
}

static int test_spi_master_writeread(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    g_r_data = (uint8_t *)osal_kmalloc(data_len, 0);
    if (g_r_data == NULL) {
        osal_kfree(g_w_data);
        return TEST_SUITE_ALLOC_FAILED;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len,
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

#if defined(CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH) && (CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH == 1)
    if ((data.tx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD) ||
        (data.rx_bytes > CONFIG_SPI_AUTO_SWITCH_DMA_THRESHOLD)) {
        osal_printk("spi transfer mode: DMA\n");
    } else {
        osal_printk("spi transfer mode: POLL\n");
    }
#endif  /* CONFIG_SPI_SUPPORT_POLL_AND_DMA_AUTO_SWITCH */

    uint64_t time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_writeread(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_master_writeread_poll failed, ret = %d\n", ret);
        osal_kfree(g_r_data);
        osal_kfree(g_w_data);
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("uapi spi master writeread time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - time_before), data_len);

    for (uint32_t i = 0; i < data_len; i++) {
        osal_printk("%x ", g_r_data[i]);
    }

    osal_kfree(g_r_data);
    osal_kfree(g_w_data);
    return TEST_SUITE_OK;
}

static int test_spi_master_writeread_poll(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len,
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

    uint64_t time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_master_writeread(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_master_writeread_poll failed, ret = %d\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("uapi spi master writeread time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - time_before), data_len);

    for (uint32_t i = 0; i < data_len; i++) {
        osal_printk("%x ", g_r_data[i]);
    }
    return TEST_SUITE_OK;
}

static int test_spi_slave_send_poll(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .tx_buff = g_w_data,
        .tx_bytes = data_len
    };

    g_tx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_slave_write(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi slave write failed, ret = %d\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("uapi spi slave write time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_tx_time_before), data_len);

    return TEST_SUITE_OK;
}

static int test_spi_slave_read_poll(int argc, char *argv[])
{
    uint32_t data_len;
    spi_bus_t bus;

    int ret_transfer = test_spi_transfer_prepare(argc, argv, &bus, &data_len);
    if (ret_transfer != TEST_SUITE_OK) {
        return ret_transfer;
    }

    spi_xfer_data_t data = {
        .rx_buff = g_r_data,
        .rx_bytes = data_len
    };

    g_rx_time_before = uapi_tcxo_get_us();
    errcode_t ret = uapi_spi_slave_read(bus, &data, 0xFFFFFFFF);
    if (ret != ERRCODE_SUCC) {
        osal_printk("spi_slave_read failed, ret = %d\n", ret);
        return TEST_SUITE_CONFIG_FAILED;
    }
    osal_printk("uapi spi slave read time: %uus, length: %u\r\n",
                (uint32_t)(uapi_tcxo_get_us() - g_rx_time_before), data_len);

    for (uint32_t i = 0; i < data_len; i++) {
        osal_printk("Read: 0x%0x\n", g_r_data[i]);
    }

    return TEST_SUITE_OK;
}

static int test_spi_slave_select(int argc, char *argv[])
{
    if (argc != 2) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint8_t param_index = 0;
    spi_bus_t bus = (spi_bus_t)strtol(argv[param_index++], NULL, 0);
    if (!g_spi_bus_inited[bus]) {
        return TEST_SUITE_TEST_FAILED;
    }

    spi_slave_t slave = (spi_slave_t)strtol(argv[param_index++], NULL, 0);

    errcode_t ret = uapi_spi_select_slave(bus, slave);
    osal_printk("spi_slave_select ret = %d\n", ret);
    if (ret != ERRCODE_SUCC) {
        return TEST_SUITE_CONFIG_FAILED;
    }

    return TEST_SUITE_OK;
}

void add_spi_test_case(void)
{
#if defined(CONFIG_SPI_SUPPORT_DMA) && (CONFIG_SPI_SUPPORT_DMA == 1)
    uapi_dma_init();
    uapi_dma_open();
#endif  /* CONFIG_SPI_SUPPORT_DMA */
    uapi_test_suite_add_function("test_spi_init",
        "Params: <bus>, <mode>, <slave_num>, <bus_clk>, <freq_mhz>, <cpol>, <cpha>,"
        "<frame_format>, <spi_frame_format>, <frame_size>, <tmod>", test_spi_init);
    uapi_test_suite_add_function("test_spi_deinit", "Params: <bus>", test_spi_deinit);
    uapi_test_suite_add_function("test_spi_master_send_poll", "Params: <bus>, <long_data>, <data_len>, <data> ...",
        test_spi_master_send_poll);
    uapi_test_suite_add_function("test_spi_master_read_poll", "Params: <bus>, <long_data>, <data_len>, <data> ...",
        test_spi_master_read_poll);
    uapi_test_suite_add_function("test_spi_master_writeread_poll", "", test_spi_master_writeread_poll);
    uapi_test_suite_add_function("test_spi_slave_send_poll", "", test_spi_slave_send_poll);
    uapi_test_suite_add_function("test_spi_slave_read_poll", "", test_spi_slave_read_poll);
    uapi_test_suite_add_function("test_spi_master_read", "Params: <bus>, <data_len>, <dma mode> ...",
                                 test_spi_master_read);
    uapi_test_suite_add_function("test_spi_master_write", "Params: <bus>, <long_data>, <data_len>, "
                                "<data> ..., <dma mode> ...", test_spi_master_write);
    uapi_test_suite_add_function("test_spi_master_writeread", "Params: <bus>, <long_data>, <data_len>, "
                                "<data> ..., <dma mode> ...", test_spi_master_writeread);
    uapi_test_suite_add_function("test_spi_slave_read", "Params: <bus>, <data_len>, <dma mode> ...",
                                 test_spi_slave_read);
    uapi_test_suite_add_function("test_spi_slave_write", "Params: <bus>, <long_data>, <data_len>, "
                                "<data> ..., <dma mode> ...", test_spi_slave_write);
    uapi_test_suite_add_function("test_spi_slave_select", "Params: <bus>, <slave selcet>", test_spi_slave_select);
}
