/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi phy configuration.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include <stdlib.h>
#include "securec.h"
#include "i2c.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "debug_print.h"
#include "platform_core.h"
#include "hal_i2c.h"
#include "test_mipi_phy.h"

#define HIWING_SLAVE_ADDR  0x5a
#define HIWING_REG_ARRAY_LEN  16

uint32_t g_i2c_device_id = I2C_BUS_3;

int32_t hiwing_read_byte(uint32_t offset, uint32_t *read_val)
{
    errcode_t ret;
    i2c_data_t data;

    uint8_t reg_addr[4] = { 0 };
    reg_addr[0] = (offset >> 24) & 0xff;
    reg_addr[1] = (offset >> 16) & 0xff;
    reg_addr[2] = (offset >> 8) & 0xff;
    reg_addr[3] = offset & 0xff;

    data.send_buf = reg_addr;
    data.send_len = 4;
    data.receive_buf = (uint8_t *)read_val;
    data.receive_len = 4;

    // TODO：待使用uapi_i2c_master_writeread()接口替换
    ret = uapi_i2c_master_write(g_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        PRINT("===== Error: I2C write ret = 0x%x! =====\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    ret = uapi_i2c_master_read(g_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        PRINT("===== Error: I2C read ret = 0x%x! =====\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }

    return TEST_SUITE_OK;
}

int32_t hiwing_write_byte(uint32_t offset, uint32_t reg_val)
{
    errcode_t ret;
    i2c_data_t data;

    int32_t status;
    uint8_t send_buf[HIWING_REG_ARRAY_LEN] = { 0 };
    send_buf[0] = (offset >> 24) & 0xff;
    send_buf[1] = (offset >> 16) & 0xff;
    send_buf[2] = (offset >> 8) & 0xff;
    send_buf[3] = offset & 0xff;
    send_buf[4] = reg_val & 0xff;
    send_buf[5] = (reg_val >> 8) & 0xff;
    send_buf[6] = (reg_val >> 16) & 0xff;
    send_buf[7] = (reg_val >> 24) & 0xff;

    data.send_buf = send_buf;
    data.send_len = 8;
    data.receive_buf = NULL;
    data.receive_len = 0;

    ret = uapi_i2c_master_write(g_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        PRINT("===== Error: I2C write ret = 0x%x! =====\r\n", ret);
        return TEST_SUITE_TEST_FAILED;
    }
    return TEST_SUITE_OK;
}

int32_t mipi_phy_hiwing_write(int argc, char* argv[])
{
    uint32_t start_addr;
    uint32_t write_val;
    uint32_t read_val;
    int32_t ret;

    if (argc != 2) { /* 2:命令参数个数 */
        PRINT("format: test mipi_phy_w4 [addr] [val]\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    start_addr = strtoul((const char*)argv[0], NULL, 16); /* 0:命令1 16:16进制 */
    write_val = strtoul((const char*)argv[1], NULL, 16); /* 0:命令1 16:16进制 */

    ret =  hiwing_read_byte(start_addr, &read_val);
    if (ret != TEST_SUITE_OK) {
        PRINT("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    ret = hiwing_write_byte(start_addr, write_val);
    if (ret != TEST_SUITE_OK) {
        PRINT("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    PRINT("Write [ 0x%x ] : [ 0x%x ] >> [ 0x%x ]\r\n", start_addr, read_val, write_val);

    return TEST_SUITE_OK;
}

int32_t mipi_phy_hiwing_read(int argc, char* argv[])
{
    uint32_t start_addr;
    int32_t ret;
    uint32_t read_val;

    if (argc != 1) { /* 2:命令参数个数 */
        PRINT("format: test mipi_phy_mem32 [addr]\r\n");
        return TEST_SUITE_ERROR_BAD_PARAMS;
    }

    start_addr = strtoul((const char*)argv[0], NULL, 16); /* 0:命令1 10:10进制 */

    ret = hiwing_read_byte(start_addr, &read_val);
    if (ret != TEST_SUITE_OK) {
        PRINT("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    PRINT("Read [ 0x%x ] : [ 0x%x ]\r\n", start_addr, read_val);
    return TEST_SUITE_OK;
}

/* i2c init for mipi */
void mipi_phy_hiwing_default_i2c(void)
{
    (void)uapi_i2c_master_init(g_i2c_device_id, 100000, I2C_SPEED_MODE_SS); /* 100000: 100KHz */
}
