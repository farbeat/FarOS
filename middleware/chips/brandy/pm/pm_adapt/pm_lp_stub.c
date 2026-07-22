/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: pm lp stub code
 */

#include "chip_io.h"
#include "platform.h"
#include "flash.h"
#include "hal_xip.h"
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
#include "touch_screen_def.h"
#include "lcd_bus.h"
#endif
#include "pmu_ldo.h"
#include "hal_pmu_ldo.h"
#include "pm_control.h"
#include "i2c_porting.h"
#include "spi_porting.h"
#include "uart_porting.h"
#ifdef CFG_DRIVERS_MMC
#include "soc_mmc.h"
#endif
#include "chip_io.h"

#define UART_SUSPEND_REGS_NUM     8

#define OPI_PSRAM_CE_PAD          0x5700CE5c
#define QSPI1_NANDFLASH_CS0_PAD   0x5700DA2C
#define QSPI1_NANDFLASH_CS1_PAD   0x5700DA30
#define PM_EMMC_CCLK_DIV_REG      0x52000100
#define PM_SD_HOST_CCLK_DIV_REG   0x52000110
#define RC_CLK_VALUE              49152000
#define XO_CLK_VALUE              32000000
#define XO_DLL2_CLK_VALUE         64000000
#define PLL_CLK_VALUE             884736000
#define PM_SDIOM_CLK_REG          0x55000628
#define PM_EMMC_DIV_REG           0x52000120
#define M_AXI_BRG_LP_CFG0         0x52000180
#define M_AXI_BRG_LP_DEFAULT_VAL  0x23fc
#define EFUSE_CTL_CFG             0x52000210
#define EFUSE_ALL_ENABLE          0xf
#define PAD_IE_BIT                0x1
#define PAD_PE_BIT                0x0
#define PAD_PS_BIT                0X2
#define XIP_CACHE_INTR_MASK                     0xa3006128
#define XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK    0x8
#define XIP_CACHE_ENABLE_WRITE_INTR_MASK        0x1
#define XIP_CACHE_2AHBM_INTR_MASK               0x2
#define PM_DISPLAY_LOGIC_CRG_RESET 0x570000c0
#define PM_DISPLAY_PWR_EN_CFG 0x57004034
#define PM_DISPLAY_PWR_CLOSE_POWER 0x100

static uint32_t g_uart_h0_regs_tmp[UART_SUSPEND_REGS_NUM];
static uint32_t g_uart_h1_regs_tmp[UART_SUSPEND_REGS_NUM];
static bool g_display_pwr_on_for_pm = true;
#if defined(SUPPORT_OHOSFWK)
bool g_frs_sleep_flag = 0;
#else
bool g_frs_sleep_flag = 1;
#endif
static uint8_t g_resume_touch_flag = 1;

void set_touch_resume_flag(uint8_t flag)
{
    g_resume_touch_flag = flag;
}

void storage_device_suspend(void)
{
#ifdef CFG_DRIVERS_NANDFLASH
    reg16_setbit(QSPI1_NANDFLASH_CS0_PAD, PAD_PS_BIT);
    reg16_setbit(QSPI1_NANDFLASH_CS0_PAD, PAD_PE_BIT); // make CS1 to high in deepsleep.
#elif defined (CFG_DRIVERS_MMC)
    emmc_save_info();
#endif
}

void storage_device_resume(void)
{
#ifdef CFG_DRIVERS_NANDFLASH
    nandflash_qspi_resume();
    reg16_clrbit(QSPI1_NANDFLASH_CS0_PAD, PAD_PS_BIT);
    reg16_clrbit(QSPI1_NANDFLASH_CS0_PAD, PAD_PE_BIT); // make CS1 to pull none in normal.
#endif
}

void opi_psram_suspend(void)
{
#ifdef CONFIG_PSRAM_SUPPORT
    uapi_psram_exit_xip();
    uapi_psram_enter_sleep();
    spi_save_reg(SPI_BUS_6); // QSPI0 for norflash
    reg16_setbit(OPI_PSRAM_CE_PAD, PAD_PE_BIT);
    reg16_setbit(OPI_PSRAM_CE_PAD, PAD_PS_BIT); // make CS to high in deepsleep.
#endif
}

void opi_psram_resume(void)
{
#ifdef CONFIG_PSRAM_SUPPORT
    spi_recovery_reg(SPI_BUS_6); // QSPI0 for norflash
    uapi_psram_exit_sleep();
    uapi_psram_enter_xip();
    reg16_clrbit(OPI_PSRAM_CE_PAD, PAD_PS_BIT);
    reg16_clrbit(OPI_PSRAM_CE_PAD, PAD_PE_BIT); // make CS to pull none in normal.
#endif
}

void display_resume_power_on(void)
{
    if (g_display_pwr_on_for_pm == false) {
        uapi_pm_control(PM_DISPLAY_POWER_UP_AND_RUN);
        g_display_pwr_on_for_pm = true;
    }
}

void display_suspend_power_off(void)
{
    g_display_pwr_on_for_pm = false;
}
void display_suspend(void)
{
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    if (lcd_get_status() || g_frs_sleep_flag) {
        if (g_resume_touch_flag != 0) { touch_suspend_for_i2c(); }
        int ret = drv_vau_suspend(NULL);
        if (ret != 0) { PRINT("drv vau suspend ret =%d\n", ret); }
#ifdef SUPPORT_GPU_JPEG
        (void)drv_jpeg_suspend(NULL);
#endif
#ifndef QSPI_DISPLAY
        ret = drv_dpu_suspend(NULL);
        if (ret != 0) { PRINT("drv dpu suspend ret =%d\n", ret); }
#endif
#ifdef SUPPORT_GPU_GMMU
        ret = drv_gmmu_suspend(NULL);
        if (ret != 0) { PRINT("drv_gmmu_suspend ret =%d\n", ret); }
#endif
        lcd_bus_api *bus_api = lcd_get_bus_api();
        ret = bus_api->bus_suspend();
        if (ret != 0) { PRINT("bus suspend ret =%d\n", ret); }
#ifndef QSPI_DISPLAY
        pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_OFF);
        hal_pmu_auxldo_enable_high_resistance(HAL_PMU_AUXLDO3, TURN_ON);
#endif
        writew(PM_DISPLAY_LOGIC_CRG_RESET, 0x0);                // display logic crg reset
        writew(PM_DISPLAY_PWR_EN_CFG, PM_DISPLAY_PWR_CLOSE_POWER); // display close power
        if (g_frs_sleep_flag) { touch_suspend_for_i2c(); }
        g_frs_sleep_flag = 0;
    }
    display_suspend_power_off();
    hal_pmu1_dig_pg_l2m_gmmu_ret1_en(true); // The GMMU memory is set to the retention state.
    hal_pmu1_dig_pg_l2m_gmmu_pwr_en(false);
#endif
}

void display_resume(void)
{
#if defined(ENABLE_UIKIT) || defined(ENABLE_LVGL)
    hal_pmu1_dig_pg_l2m_gmmu_pwr_en(true); // The GMMU memory is set to the normal state.
    hal_pmu1_dig_pg_l2m_gmmu_ret1_en(false);
    if (lcd_get_status() != 0) {
#ifndef QSPI_DISPLAY
        pmu_ldo_power_on(PMU_LDO_AUXLDO3, TURN_ON);
#endif
        display_resume_power_on();
#ifdef CONFIG_CLOSED_ALL_IE
        uapi_pin_set_ie(DISPLAY_TE_GPIO, PIN_IE_ENABLE);
#endif
        lcd_bus_api *bus_api = lcd_get_bus_api();
        int ret = bus_api->bus_resume();
        if (ret != 0) { PRINT("bus resume ret =%d\n", ret); }
#ifndef QSPI_DISPLAY
        (void)drv_dpu_resume(NULL);
#endif
#ifdef SUPPORT_GPU_JPEG
        (void)drv_jpeg_resume(NULL);
#endif
        (void)drv_vau_resume(NULL);
#ifdef SUPPORT_GPU_GMMU
        (void)drv_gmmu_resume(NULL);
#endif
        if (g_resume_touch_flag != 0) { touch_resume_for_i2c(); }
    }
#endif
}
void pm_misc_regs_resume(void)
{
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_WRITE_DATA_DELAY_INTR_MASK);
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_ENABLE_WRITE_INTR_MASK);
    reg16_clrbit(XIP_CACHE_INTR_MASK, XIP_CACHE_2AHBM_INTR_MASK);

    // xip config param needs to re-config
    non_os_nmi_config(NMI_CWDT, true);
    hal_xip_config_interrupt_type(XIP_0, XIP_INTR_NMI);

    // set l2ram for dsp
    pmu1_dsp_l2ram_config(DSP_L2RAM_AREA_SIZE);

    hal_ipc_interrupt_en();
    writel(M_AXI_BRG_LP_CFG0, M_AXI_BRG_LP_DEFAULT_VAL); // M_axi
    writel(EFUSE_CTL_CFG, EFUSE_ALL_ENABLE);    // enable efuse
#ifdef CFG_DRIVERS_NANDFLASH
    reg16_clrbit(PM_SD_HOST_CCLK_DIV_REG, 0); // close sd host clock enable.
    reg16_clrbit(PM_EMMC_CCLK_DIV_REG, 0); // close sd mmc clock enable.
#endif
#ifdef CFG_DRIVERS_MMC
    reg16_clrbit(PM_SD_HOST_CCLK_DIV_REG, 0); // close sd host clock enable.
#endif
    hal_pmu_set_cmu_xo_trim(HAL_PMU_XO_CORE_DS_1_025X);
}
