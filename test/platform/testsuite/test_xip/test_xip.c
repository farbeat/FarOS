/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test xip source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-09-23, Create file. \n
 */
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "securec.h"
#include "cmsis_os2.h"
#include "flash.h"
#include "flash_config_info.h"
#include "non_os.h"
#include "hal_qspi.h"
#include "hal_xip.h"
#include "xip_porting.h"
#include "chip_io.h"
#include "tcxo.h"
#include "test_xip.h"

#define TEST_FLASH_LENGTH    0x100000
#define TEST_REAL_FLASH_ADDR (FLASH_START + 0x4000000)
#define TEST_S_ADDR          (APP_FLASH_REGION_START + APP_FLASH_REGION_LENGTH - FLASH_START)
#define TEST_E_ADDR          (TEST_S_ADDR + TEST_FLASH_LENGTH)
#define COMPUTE_RIGHT_SHIFT_BIT 32

static int test_xip_cache(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    xip_cache_count_t cache_count;
    hal_xip_get_cache_count(&cache_count);

    PRINT("hit_count: %u--%u. \r\n", (uint32_t)(cache_count.hit_count >> COMPUTE_RIGHT_SHIFT_BIT), \
          (uint32_t)cache_count.hit_count);
    PRINT("miss_count: %u--%u. \r\n", (uint32_t)(cache_count.miss_count >> COMPUTE_RIGHT_SHIFT_BIT), \
          (uint32_t)cache_count.miss_count);
    PRINT("total_count: %u--%u. \r\n", (uint32_t)(cache_count.total_count >> COMPUTE_RIGHT_SHIFT_BIT), \
          (uint32_t)cache_count.total_count);
    return TEST_SUITE_OK;
}

static int test_xip_ctrl_intr(int argc, char *argv[])
{
    uint32_t xip_addr = 0;
    bool intr_hardfault = false;
    uint32_t operate_write = false;
    uint32_t write_value = 0x5A5A5A5A;
    if (argc >= 3) { /* 3: Indicates the number of input parameters */
        xip_addr = (uint32_t)strtol(argv[0], NULL, 0);
        intr_hardfault = (bool)strtol(argv[1], NULL, 0);
        operate_write = (uint32_t)strtol(argv[TEST_PARAM_ARGC_2], NULL, 0);
    } else {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    PRINT("[Params] Addr:0x%x . \r\n", xip_addr);
    PRINT("[Params] intr tpye:%d. \r\n", (uint32_t)intr_hardfault);
    PRINT("[Params] operate: %d . \r\n", (uint32_t)operate_write);

    if (intr_hardfault) {
        hal_xip_config_interrupt_type(XIP_0, XIP_INTR_HARDFAULT);
    } else {
        hal_xip_config_interrupt_type(XIP_0, XIP_INTR_NMI);
    }

    if (operate_write == 0) {
        uapi_flash_exit_from_xip_mode(0);
        readl(xip_addr + FLASH_START_ADDR);
    } else if (operate_write == 1) { /* 1: Indicates spi write */
        uapi_flash_switch_to_xip_mode(0);
        uapi_flash_write_data(0, xip_addr, (uint8_t *)&write_value, sizeof(write_value));
    } else if (operate_write == 2) { /* 2: Indicates xip write */
        uapi_flash_switch_to_xip_mode(0);
        reg32_write(xip_addr + TEST_REAL_FLASH_ADDR, write_value);
    }

    return TEST_SUITE_OK;
}

static int test_eflash_xip_read(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    uint32_t print_times = 0;
    uapi_flash_switch_to_xip_mode(0);
    PRINT("TEST_S_ADDR:%x\r\n", TEST_S_ADDR);
    PRINT("TEST_E_ADDR:%x\r\n", TEST_E_ADDR);
    PRINT("APP_FLASH_REGION_START:%x\r\n", APP_FLASH_REGION_START);
    PRINT("APP_FLASH_REGION_LENGTH:%x\r\n", APP_FLASH_REGION_LENGTH);
    PRINT("FLASH_START:%x\r\n", FLASH_START);

    for (uint32_t addr = FLASH_START_ADDR + TEST_S_ADDR; addr < (FLASH_START_ADDR + TEST_E_ADDR);
        addr += TEST_PARAM_4_BYTES) {
        print_times++;
        readl(addr);
        if (print_times >= 0x10000) {
            PRINT("check value addr:0x%x,actual:0x%x" NEWLINE, addr - FLASH_START_ADDR, readl(addr));
            print_times = 0;
        }
    }
    PRINT("test finish...\r\n");
    return TEST_SUITE_OK;
}

static int test_eflash_xip_write(int argc, char *argv[])
{
    uint32_t test_addr;
    uint32_t write_value;
    if (argc >= 2) { /* 2: Indicates the number of input parameters */
        test_addr = (uint32_t)strtol(argv[0], NULL, 0);
        write_value = (uint32_t)strtol(argv[1], NULL, 0);
    } else {
        PRINT("test_eflash_write_byte para num is err argc=%d." NEWLINE, argc);
        return TEST_SUITE_TEST_FAILED;
    }
    uapi_flash_switch_to_xip_mode(0);
    PRINT("addr is 0x%x, value is 0x%x, and write value is 0x%x" NEWLINE, test_addr,
          readl(test_addr + FLASH_START_ADDR), write_value);
    non_os_enter_critical();
    uapi_flash_exit_from_xip_mode(0);
    uapi_flash_sector_erase(0, test_addr, true);
    uapi_flash_write_data(0, test_addr, (uint8_t *)&write_value, sizeof(write_value));
    uapi_flash_switch_to_xip_mode(0);

    PRINT("addr is 0x%x, value is 0x%x" NEWLINE, test_addr, readl(test_addr + FLASH_START_ADDR));
    non_os_exit_critical();

    return TEST_SUITE_OK;
}

void add_xip_test_case(void)
{
    hal_xip_init();
    hal_xip_cache_miss_en();
    uapi_test_suite_add_function("xip_interrupt_type_config", "Parmas: <addr>, <interrupt type: 0:NMI, 1:HardFault>,"
                 "<operate: 0:read, 1: spi_write, 2: write>", test_xip_ctrl_intr);
    uapi_test_suite_add_function("xip_cache", "Parmas: NA", test_xip_cache);
    uapi_test_suite_add_function("eflash_xip_read", "Parmas: NA", test_eflash_xip_read);
    uapi_test_suite_add_function("eflash_xip_write", "XIP Parmas: <addr>, <value>", test_eflash_xip_write);
}
