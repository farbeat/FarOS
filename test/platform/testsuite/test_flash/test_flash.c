/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test flash source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-11-11, Create file. \n
 */
#include "securec.h"
#include "debug/osal_debug.h"
#include "interrupt/osal_interrupt.h"
#include "common_def.h"
#include "memory_config.h"
#include "flash.h"
#include "hal_spi.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "test_suite_log.h"
#include "hal_xip.h"
#include "test_flash.h"

#define TEST_PARAM_4_BYTES 4
#define TEST_READ_LEN  10

#define READ_MODE 0
#define WRITE_MODE 1
#define ERASE_MODE 2

#define TEST_LEFT_SHIFT_8   8
#define TEST_LEFT_SHIFT_16  16
#define TEST_LEFT_SHIFT_24  24

#define ERASE_LEN   0x4000
#define ERASE_32K   0x8000

#define TEST_FLASH_LENGTH    0x100000
#define TEST_S_ADDR          (0x6C0000)
#define TEST_E_ADDR          (0x7C0000)

#define writel(addr, d)      (*(volatile uint32_t*)(uintptr_t)(addr) = (uint32_t)(d))
#define readl(addr)          (*(volatile uint32_t*)(uintptr_t)(addr))
#define reg(base, offset)    (*(volatile uint32_t *)(uintptr_t)((uint32_t)(base) + (offset)))

uint32_t g_testflash_src[ERASE_LEN / TEST_PARAM_4_BYTES];
uint32_t g_testflash_dst[ERASE_LEN / TEST_PARAM_4_BYTES];

void flash_porting_pinmux_cfg(flash_id_t id) __attribute__((weak, alias("flash_porting_none")));

static void flash_porting_none(flash_id_t id)
{
    UNUSED(id);
}

#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
void flash_porting_xip_disable(void) __attribute__((weak, alias("flash_xip_none")));

static void flash_xip_none(void)
{
}

static int test_flash_xip_read(int argc, char *argv[])
{
    uint32_t test_time;
    uint32_t print_times = 0;

    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    test_time = (uint32_t)strtol(argv[0], NULL, 0);

    for (uint32_t i = 0; i < test_time; i++) {
        test_suite_log_stringf("test times: %d\r\n", i);
        for (uint32_t addr = FLASH_START + TEST_S_ADDR; addr < (FLASH_START + TEST_E_ADDR);
             addr += TEST_PARAM_4_BYTES) {
            print_times++;
            readl(addr);
            if (print_times >= 0x10000) {
                test_suite_log_stringf("check value addr:0x%x,actual:0x%x\r\n", addr - FLASH_START, readl(addr));
                print_times = 0;
            }
        }
    }
    test_suite_log_stringf("test finish...\r\n");

    return TEST_SUITE_OK;
}
#endif

static int test_flash_write_data(int argc, char *argv[])
{
    uint32_t test_addr;
    uint32_t write_value;
    if (argc >= 2) {    /* 2: Indicates the number of input parameters. */
        test_addr = (uint32_t)strtol(argv[0], NULL, 0);
        write_value = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        test_suite_log_stringf("test_flash_write_byte para num is err argc=%d.\r\n", argc);
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    unsigned int irq = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x, and write value is 0x%x\r\n", test_addr,
                           readl(test_addr + FLASH_START), write_value);
#endif

#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
    uapi_flash_sector_erase(FLASH_1, test_addr, true);
    uapi_flash_write_data(FLASH_1, test_addr, (uint8_t *)&write_value, sizeof(write_value));
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr, readl(test_addr + FLASH_START));
#endif
    osal_irq_restore(irq);
    return TEST_SUITE_OK;
}

static int test_flash_power(int argc, char *argv[])
{
    if (argc < 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    bool mode = (uint32_t)strtol(argv[0], NULL, 0);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
    if (mode) {
        uapi_flash_switch_to_deeppower(FLASH_1);
        test_suite_log_stringf("switch to power down.\r\n");
    } else {
        uapi_flash_resume_from_deeppower(FLASH_1);
        test_suite_log_stringf("switch to power down resume.\r\n");
    }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
    return TEST_SUITE_OK;
}

static int test_flash_read_data(int argc, char *argv[])
{
    unused(argc);
    uint32_t test_addr;
    test_addr = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t dst[TEST_READ_LEN] = { 0 };

    unsigned int irq = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr,
                           readl(test_addr + FLASH_START));
#endif
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
    uapi_flash_read_data(FLASH_1, test_addr, (uint8_t*)dst, sizeof(dst));
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
    test_suite_log_stringf("addr is 0x%x, value is 0x%x\r\n", test_addr, readl(test_addr + FLASH_START));
#endif
    osal_irq_restore(irq);
    for (int i = 0; i < TEST_READ_LEN; i++) {
        test_suite_log_stringf("dst = 0x%x\r\n", dst[i]);
    }
    return TEST_SUITE_OK;
}

static void test_flash_print(uint32_t mode, uint32_t* src, uint32_t length)
{
    uint32_t i = 0;
    if (mode == READ_MODE) {
        test_suite_log_stringf("read value:");
    }
    if (mode == WRITE_MODE) {
        test_suite_log_stringf("write value:");
    }
    for (i = 0; i < (length / TEST_PARAM_4_BYTES); i++) {
        test_suite_log_stringf("%x ", src[i]);
    }
    test_suite_log_stringf("\r\n");
}

static void test_flash_erase(void)
{
    uint32_t irq_sts = osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
    for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
        uapi_flash_block_erase(FLASH_1, i, ERASE_32K, true);
    }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
    osal_irq_restore(irq_sts);
}

static int test_flash_press(int argc, char *argv[])
{
    uint32_t mode, value;

    if (argc >= 2) { /* 2: Indicates the number of input parameters. */
        mode = (uint32_t)strtol(argv[0], NULL, 0);
        value  = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    for (uint32_t j = 0; j < ERASE_LEN / TEST_PARAM_4_BYTES; j++) {
        g_testflash_src[j] = 0xffffffff & ((value << TEST_LEFT_SHIFT_24) | (value << TEST_LEFT_SHIFT_16) |
                             (value << TEST_LEFT_SHIFT_8) | value);
        g_testflash_dst[j] = 0xffffffff;
    }

    if (mode == READ_MODE) {
        uint32_t irq_sts =  osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
        for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
            uapi_flash_read_data(FLASH_1, i, (uint8_t*)g_testflash_dst, ERASE_LEN);
            uapi_flash_read_data(FLASH_1, (i + ERASE_LEN), (uint8_t*)g_testflash_dst, ERASE_LEN);
        }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
        test_flash_print(mode, g_testflash_dst, sizeof(g_testflash_dst));
        osal_irq_restore(irq_sts);
    } else if (mode == ERASE_MODE) {
        test_flash_erase();
    } else {
        uint32_t irq_sts =  osal_irq_lock();
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_exit_from_xip_mode(FLASH_1);
#endif
        for (uint32_t i = TEST_S_ADDR; i < TEST_E_ADDR; i += ERASE_32K) {
            uapi_flash_block_erase(FLASH_1, i, ERASE_32K, true);
            uapi_flash_write_data(FLASH_1, i, (uint8_t*)g_testflash_src, ERASE_LEN);
            uapi_flash_write_data(FLASH_1, (i + ERASE_LEN), (uint8_t*)g_testflash_src, ERASE_LEN);
        }
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
        uapi_flash_switch_to_xip_mode(FLASH_1);
#endif
        osal_irq_restore(irq_sts);
    }
    test_suite_log_stringf("test finish...\r\n");
    return TEST_SUITE_OK;
}

void add_flash_test_case(void)
{
    flash_porting_pinmux_cfg(FLASH_1);
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    hal_xip_init();
    flash_porting_xip_disable();
#endif
#if !defined(CONFIG_FLASH_ALREADY_START)
    uint32_t manufacture_id;
    uapi_flash_init(FLASH_1);
    uapi_flash_read_id(FLASH_1, &manufacture_id);
    test_suite_log_stringf("manufacture_id = %x\r\n", manufacture_id);
    flash_save_manufacturer(FLASH_1, manufacture_id);
    uapi_flash_switch_to_qspi_init(FLASH_1);
#endif
#if defined(CONFIG_FLASH_SUPPORT_XIP) && (CONFIG_FLASH_SUPPORT_XIP == 1)
    uapi_flash_switch_to_cache_mode(FLASH_1);  // need remove
    uapi_test_suite_add_function("flash_xip_read", "Parmas: <test times>", test_flash_xip_read);
#endif
    uapi_test_suite_add_function("flash_write_data", "Parmas: <addr>, <value>", test_flash_write_data);
    uapi_test_suite_add_function("flash_power_down", "Parmas: <power 1: power dowen, 0: resume>", test_flash_power);
    uapi_test_suite_add_function("flash_read_data", "Parmas: <addr>", test_flash_read_data);
    uapi_test_suite_add_function(
        "flash_press", "Parmas: <mode  0: read, 1: write, 2: erase>, <value>", test_flash_press);
}
