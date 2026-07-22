/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Test pmp header \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-31, Create file. \n
 */

#include <stdbool.h>
#include "debug_print.h"
#include "test_suite.h"
#include "test_suite_errors.h"
#include "osal_debug.h"
#include "osal_addr.h"
#include "securec.h"
#include "common_def.h"
#include "drv_pmp.h"
#include "pmp_porting.h"
#include "test_pmp.h"

#define read_mem(addr)                              (*(volatile uint32_t*)(uintptr_t)(addr))
#define write_mem(addr, d)                          (*(volatile uint32_t*)(uintptr_t)(addr) = (uint32_t)(d))

static int test_pmp_config(int argc, char* argv[])
{
    if (argc != 4) { /* 4: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t mode = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t addr = (uint32_t)strtol(argv[1], NULL, 0);
    uint32_t test_size = (uint32_t)strtol(argv[2], NULL, 0);
    uint32_t rwx = (uint32_t)strtol(argv[3], NULL, 0);
    if (rwx != PMPCFG_NO_ACCESS && rwx != PMPCFG_READ_ONLY_NEXECUTE && \
        rwx != PMPCFG_RW_NEXECUTE && rwx != PMPCFG_READ_ONLY_EXECUTE && rwx != PMPCFG_RW_EXECUTE) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }
    uint32_t length = mode == PMPCFG_ADDR_MATCH_TOR ? 2 : 1;
    pmp_conf_t pmp_cfg[2] = {
        {
            .idx = 0,
            .addr = addr,
            .size = test_size,
            .conf = {
                .rwx_permission = (rwx_permission_t)rwx,
                .addr_match = (addr_match_t)mode,
                .lock = true,
                .pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE
            }
        },
        {
            .idx = 1,
            .addr = test_size,
            .size = test_size,
            .conf = {
                .rwx_permission =(rwx_permission_t)rwx,
                .addr_match = (addr_match_t)mode,
                .lock = true,
                .pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE
            }
        }
    };
    uint32_t ret = uapi_pmp_config(pmp_cfg, length);
    PRINT("config success! data : %u.\n", ret);
    return TEST_SUITE_OK;
}
static int test_pmp_read(int argc, char* argv[])
{
    if (argc != 1) {
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t read_addr = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t read_data = read_mem(read_addr);

    PRINT("read success! data : %d.\n", read_data);
    return TEST_SUITE_OK;
}

static int test_pmp_write(int argc, char* argv[])
{
    if (unlikely(argc != 2)) { /* 2: Indicates the number of input parameters */
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    uint32_t write_addr = (uint32_t)strtol(argv[0], NULL, 0);
    uint32_t write_data = (uint32_t)strtol(argv[1], NULL, 0);
    write_mem(write_addr, write_data);

    PRINT("write success! data : %d.\n", write_data);
    return TEST_SUITE_OK;
}

void add_pmp_test_case(void)
{
    uapi_test_suite_add_function("test_pmp_config", "Params: <AddrMatch>(0x1[TOR], 0x2[NA4], 0x3[NAPOT]), <BeginAddr>,\
                <EndAddr/Size>(EndAddr in TOR mode, Size in NAPOT mode),\
                <rwx_permission(0[NO_ACCESS], 1[READ_ONLY_NEXCUTE], 3[RW_NEXECUTE],\
                5[PMPCFG_READ_NWRITE_EXECUTE] 7[RW_EXECUTE])>", test_pmp_config);
    uapi_test_suite_add_function("test_pmp_read", "Params: <ReadAddr(0x0, 0x10000, 0x20000)>", test_pmp_read);
    uapi_test_suite_add_function(
        "test_pmp_write", "Params: <WriteAddr(0x0, 0x10000, 0x20000)>, <WriteData>", test_pmp_write);
}
