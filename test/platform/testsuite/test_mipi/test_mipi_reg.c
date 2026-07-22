/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi_tx self test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#include <stdlib.h>
#include "securec.h"
#include "i2c.h"
#include "tcxo.h"
#include "test_mipi_phy.h"
#include "test_suite_log.h"
#include "test_suite_errors.h"
#include "chip_io.h"
#include "test_mipi_reg.h"

void mipi_tx_phy_cfg(void)
{
    hiwing_write_byte(0xF8C20004, 0x1F);
    hiwing_write_byte(0xF8C20008, 0x1);
    hiwing_write_byte(0xF8C20064, 0x1);
    hiwing_write_byte(0xF8C20068, 0x1);
    hiwing_write_byte(0xF8C20070, 0x1F);
    hiwing_write_byte(0xF8C2000C, 0x1);
    hiwing_write_byte(0xF8C20050, 0x1B6D);
    hiwing_write_byte(0xF8C20800, 0x48);
    hiwing_write_byte(0xF8C20800, 0x41);
    hiwing_write_byte(0xF8C20960, 0x540);
    hiwing_write_byte(0xF8C20840, 0x1);
    hiwing_write_byte(0xF8C20840, 0x11);
    hiwing_write_byte(0xF8C200B8, 0xa0002a00);
    hiwing_write_byte(0xF8C21004, 0x10000000);
    hiwing_write_byte(0xF8C21018, 0xc);
    hiwing_write_byte(0xF8C2101C, 0x1);
    hiwing_write_byte(0xF8C210A0, 0x53);
    hiwing_write_byte(0xF8C2109C, 0x28033);
    hiwing_write_byte(0xF8C21d28, 0x78);
    hiwing_write_byte(0xF8C21020, 0x78);
    hiwing_write_byte(0xF8C21d20, 0x3);
    hiwing_write_byte(0xF8C21d2c, 0x1);
    hiwing_write_byte(0xF8C21cb8, 0x93);
    hiwing_write_byte(0xF8C21d40, 0x80);
    hiwing_write_byte(0xF8C21dc0, 0x1B);
    hiwing_write_byte(0xF8C21dc4, 0x1);
    hiwing_write_byte(0xF8C21dc8, 0x3);
    hiwing_write_byte(0xF8C21dcc, 0x3);
    hiwing_write_byte(0xF8C21dd0, 0x5);
    hiwing_write_byte(0xF8C21dd4, 0x4);
    hiwing_write_byte(0xF8C21ddc, 0x23);
    hiwing_write_byte(0xF8C21e40, 0x1B);
    hiwing_write_byte(0xF8C21e44, 0x1);
    hiwing_write_byte(0xF8C21e48, 0x3);
    hiwing_write_byte(0xF8C21e4c, 0x3);
    hiwing_write_byte(0xF8C21e50, 0x5);
    hiwing_write_byte(0xF8C21e54, 0x4);
    hiwing_write_byte(0xF8C21e5c, 0x23);
    hiwing_write_byte(0xF8C21ec0, 0x1);
    hiwing_write_byte(0xF8C21ec4, 0x15);
    hiwing_write_byte(0xF8C21ec8, 0x3);
    hiwing_write_byte(0xF8C21ecc, 0x3);
    hiwing_write_byte(0xF8C21ed0, 0xc);
    hiwing_write_byte(0xF8C21ed4, 0x3);
    hiwing_write_byte(0xF8C21edc, 0x23);
}

void mipi_tx_control_init(void)
{
    writel(0x0, 0x56000308);
    writel(0x104, 0x56000008);
    writel(0x0, 0x5600000c);
    writel(0x5, 0x56000010);
    writel(0x0, 0x56000018);
    writel(0x1c, 0x5600002c);
    writel(0x0, 0x56000030);
    writel(0x1, 0x56000034);
    writel(0x3f, 0x56000038);
    writel(0x1c6, 0x5600003c);
    writel(0x1, 0x56000040);
    writel(0x4f, 0x56000044);
    writel(0x20, 0x56000048);
    writel(0x20, 0x5600004c);
    writel(0x60c, 0x56000050);
    writel(0x4, 0x56000054);
    writel(0xc, 0x56000058);
    writel(0x14, 0x5600005c);
    writel(0x1c6, 0x56000060);
    writel(0x1c6, 0x56000064);
    writel(0x0, 0x56000068);
    writel(0x0, 0x56000078);
    writel(0x0, 0x5600007c);
    writel(0x0, 0x56000080);
    writel(0x0, 0x56000084);
    writel(0x0, 0x56000088);
    writel(0x0, 0x5600008c);
    writel(0x180013  , 0x56000098);
    writel(0x50026   , 0x5600009c);
    writel(0x0, 0x560000a4);
    writel(0x0, 0x560000a8);
    writel(0x0, 0x560000ac);
    writel(0xffffffff, 0x560000c4);
    writel(0xffffffff, 0x560000c8);
}

void mipi_tx_lcd_init(void)
{
    writel(0x0000fe15, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x00773a15, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x00205315, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x00ff5115, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x00003515, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x010e002a, 0x56000070);
    writel(0x000000d3, 0x56000070);
    writel(0x00000529, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x0100002b, 0x56000070);
    writel(0x000000c5, 0x56000070);
    writel(0x00000529, 0x5600006c);
    uapi_tcxo_delay_us(1000ULL);
    writel(0x00001105, 0x5600006c);
    uapi_tcxo_delay_us(150000ULL);
    writel(0x00002905, 0x5600006c);
    uapi_tcxo_delay_us(150000ULL);
    writel(0x0337, 0x5600006c);
    writel(0x0414, 0x5600006c);
    writel(0x0437, 0x5600006c);
    writel(0x0914, 0x5600006c);
    writel(0x0137, 0x5600006c);
    writel(0x0a14, 0x5600006c);
    writel(0x0b14, 0x5600006c);
    writel(0x0c14, 0x5600006c);
    writel(0x0d14, 0x5600006c);
    writel(0x0e14, 0x5600006c);
    writel(0x0f14, 0x5600006c);
}

int32_t mipi_screen_init_reg_cfg(int argc, char* argv[])
{
    // 屏复位 ---------------reset start --------
    writel(3, 0x5600201c);
    uapi_tcxo_delay_ms(2);
    writel(0, 0x5600201c);
    uapi_tcxo_delay_us(20ULL);
    writel(3, 0x5600201c);
    uapi_tcxo_delay_ms(120ULL);

    // i2c init
    mipi_phy_hiwing_default_i2c();

    // tx phy cfg
    hiwing_write_byte(0xF8A22024, 0x22000000);
    hiwing_write_byte(0xF8A22028, 0x2001063);
    hiwing_write_byte(0xF8A22358, 0x303019f);
    hiwing_write_byte(0xF8A20A14, 0x00011080);
    mipi_tx_phy_cfg();

    // 初始化mipi控制器
    mipi_tx_control_init();

    // 复位phy和mipi控制器
    writel(0x0, 0x56000000);
    writel(0x0, 0x560000A0);
    uapi_tcxo_delay_us(2ULL);
    writel(0xf, 0x560000A0);
    writel(0x1, 0x56000000);
    uapi_tcxo_delay_ms(10ULL);
    writel(0x1, 0x56000094);

    // GT2 EDO(和辉)屏初始化2
    mipi_tx_lcd_init();
    return TEST_SUITE_OK;
}

int32_t mipi_colorbar_on(int argc, char* argv[])
{
    writel(0x0, 0x56000068);
    writel(0x1401c6, 0x5600030c);
    writel(0x280014, 0x56000310);
    writel(0x8300, 0x56000308);
    uapi_tcxo_delay_us(10000ULL);
    writel(0x0, 0x56000000);
    uapi_tcxo_delay_us(10000ULL);
    writel(0x0, 0x560000A0);
    uapi_tcxo_delay_us(100000ULL);
    writel(0xf, 0x560000A0);
    uapi_tcxo_delay_us(10000ULL);
    writel(0x1, 0x56000000);
    uapi_tcxo_delay_us(10000ULL);
    writel(0x1, 0x56000094);

    test_suite_log_stringf("\n===== mipi_colorbar_on success! =====\n");

    return TEST_SUITE_OK;
}

int32_t mipi_colorbar_off(int argc, char* argv[])
{
    writel(0x0, 0x56000068);
    writel(0x80000002, 0x56000308);
    uapi_tcxo_delay_us(10000);
    writel(0x0, 0x56000000);
    uapi_tcxo_delay_us(10000);
    writel(0x0, 0x560000A0);
    uapi_tcxo_delay_us(100000);
    writel(0xf, 0x560000A0);
    uapi_tcxo_delay_us(10000);
    writel(0x1, 0x56000000);
    uapi_tcxo_delay_us(10000);
    writel(0x1, 0x56000094);

    test_suite_log_stringf("\n===== mipi_colorbar_off success! =====\n");

    return TEST_SUITE_OK;
}

int32_t dpu_colorbar_on(int argc, char* argv[])
{
    mipi_colorbar_off(NULL, NULL);

    writel(0x56020000, 0x40000000);
    writel(0x5602000c, 0x0010100e);
    writel(0x56020308, 0x12);

    //layer0
    writel(0x56027000, 0xc00000ff);
    writel(0x56027080, 0x00000000);
    writel(0x56027084, 0x01c501c5);
    writel(0x56027088, 0x00000000);
    writel(0x5602708c, 0x01c501c5);

    // dhd
    writel(0x5602d000, 0xc0000010);
    writel(0x5602d004, 0x000f01c5);
    writel(0x5602d008, 0x00000009);
    writel(0x5602d00c, 0x001301c5);
    writel(0x5602d010, 0x00000009);
    writel(0x5602d01c, 0x08030009);
    writel(0x5602d020, 0x0000001c);
    writel(0x5602d024, 0x01de0030);
    writel(0x5602d028, 0x00000002);
    writel(0x5602d02c, 0x00100000);

    // force te
    writel(0x5602d080, 0x8000000f);

    // fdr0
    writel(0x56030e00, 0x1000);
    writel(0x56030e08, 0x00000003);
    writel(0x56030e14, 0x00080200);
    writel(0x56030e44, 0x01c501c5);
    writel(0x56030e4c, 0x01c501c5);
    writel(0x56030e54, 0x20010000);

    //regup
    writel(0x56027004, 0x1);
    writel(0x5602d000, 0xc0000011);
    writel(0x5602d07c, 0x1);

    test_suite_log_stringf("\n===== dpu_colorbar_on success! =====\n");

    return TEST_SUITE_OK;
}
