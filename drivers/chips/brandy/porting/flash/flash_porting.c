/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides flash porting \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-30， Create file. \n
 */
#include "flash_porting.h"
#include <common_def.h>
#include "soc_osal.h"
#include "flash.h"
#include "tcxo.h"
#include "debug_print.h"
#include "spi_porting.h"
#include "hal_xip.h"
#include "chip_io.h"
#include "qspi.h"

#define QSPI0_FLASH_D0_PIN        0xA3005000
#define QSPI0_FLASH_D1_PIN        0xA3005004
#define QSPI0_FLASH_D2_PIN        0xA3005008
#define QSPI0_FLASH_D3_PIN        0xA300500C
#define QSPI0_FLASH_CLK_PIN       0xA3005010
#define QSPI0_FLASH_CS0_PIN       0xA3005014
#define PIN_FUNC_QSPI0            0x1
#define QSPI0_FLASH_D0_PAD        0x5700CE00
#define QSPI0_FLASH_D1_PAD        0x5700CE04
#define QSPI0_FLASH_D2_PAD        0x5700CE08
#define QSPI0_FLASH_D3_PAD        0x5700CE0C
#define QSPI0_FLASH_CLK_PAD       0x5700CE10
#define QSPI0_FLASH_CS0_PAD       0x5700CE14
#define PAD_IE_BIT                0x1
#define PAD_PE_BIT                0x0
#define PAD_PS_BIT                0X2

static uint32_t g_flash_bus_clk = 40000000;
static uint32_t g_flash_enter_qspi_freq_mhz[FLASH_MAX] = { 4 };
static uint32_t g_flash_exit_xip_freq_mhz[FLASH_MAX] = { 1 };
static uint32_t g_flash_enter_xip_freq_mhz[FLASH_MAX] = { 20 };
static volatile bool g_bt_power_on = true;

static volatile bool g_flash1_power_on = true;

static flash_cfg_t g_flash_porting_config[FLASH_MAX] = {
    {
        .isinit = 0,
        .flash_manufacturer = FLASH_MANUFACTURER_MAX,
        .unique_id = 0,
        .bus = SPI_BUS_3,
        .mode = HAL_SPI_FRAME_FORMAT_STANDARD,
        .is_xip = true,
        .attr = {
            .is_slave = false,
            .slave_num = 1,
            .bus_clk = 40000000,
            .freq_mhz = 10,
            .clk_polarity = (uint32_t)SPI_CFG_CLK_CPOL_0,
            .clk_phase = (uint32_t)SPI_CFG_CLK_CPHA_0,
            .frame_format = SPI_CFG_FRAME_FORMAT_MOTOROLA_SPI,
            .spi_frame_format = (uint32_t)HAL_SPI_FRAME_FORMAT_STANDARD,
            .frame_size = (uint32_t)HAL_SPI_FRAME_SIZE_8,
            .sste = SPI_CFG_SSTE_DISABLE,
        },
        .extra_attr = {
            .tx_use_dma = false,
            .rx_use_dma = false,
            .qspi_param = { 0 },
        },
    },
};

void flash_porting_get_config(flash_id_t id, flash_cfg_t *config)
{
    config[id] = g_flash_porting_config[id];
}

void flash_porting_update_enter_qspi_buad(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_enter_qspi_freq_mhz[id] = freq_mhz;
}

static void flash_1_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_enter_qspi_freq_mhz[id];
}

static set_clk_div_t g_flash_enter_qspi_clk_div[FLASH_MAX] = {
    flash_1_enter_qspi_clk_div,
};

void flash_porting_set_enter_qspi_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_enter_qspi_clk_div[id](id, attr);
}

void flash_porting_update_exit_xip_buad(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_exit_xip_freq_mhz[id] = freq_mhz;
}

static void flash_1_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_exit_xip_freq_mhz[id];
}

static set_clk_div_t g_flash_exit_xip_clk_div[FLASH_MAX] = {
    flash_1_exit_xip_clk_div,
};

void flash_porting_set_exit_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_exit_xip_clk_div[id](id, attr);
}

void flash_porting_update_enter_xip_buad(flash_id_t id, uint32_t bus_clk, uint32_t freq_mhz)
{
    g_flash_bus_clk = bus_clk;
    g_flash_enter_xip_freq_mhz[id] = freq_mhz;
}

static void flash_1_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    attr->bus_clk = g_flash_bus_clk;
    attr->freq_mhz = g_flash_enter_xip_freq_mhz[id];
}

static set_clk_div_t g_flash_enter_xip_clk_div[FLASH_MAX] = {
    flash_1_enter_xip_clk_div,
};

void flash_porting_set_enter_xip_clk_div(flash_id_t id, spi_attr_t *attr)
{
    g_flash_enter_xip_clk_div[id](id, attr);
}

static void flash_1_power_on(flash_id_t id)
{
    unused(id);
    if (g_flash1_power_on == true) {
        return;
    }
    reg16_setbit(PMU_AUXLDO1_CFG, PMU_AUXLDO_ENABLE_BIT);
    uapi_tcxo_delay_ms(PMU_AUXLDO_POWER_ON_DELAY_US);
    reg16_setbit(PMU_AUXLDO1_CFG, PMU_AUXLDO_ENABLE_DELAY_BIT);
    uapi_tcxo_delay_ms(PMU_AUXLDO_POWER_ON_DELAY_MS);
    g_flash1_power_on = true;
}

static power_on_t g_flash_power_on_array[FLASH_MAX] = {
    flash_1_power_on,
};

static void flash_1_power_off(flash_id_t id)
{
    unused(id);
    if (g_flash1_power_on == false) {
        return;
    }
    reg16_clrbit(PMU_AUXLDO1_CFG, PMU_AUXLDO_ENABLE_BIT);
    reg16_clrbit(PMU_AUXLDO1_CFG, PMU_AUXLDO_ENABLE_DELAY_BIT);
    uapi_tcxo_delay_ms(PMU_AUXLDO_POWER_OFF_DELAY_MS);
    g_flash1_power_on = false;
}

static power_on_t g_flash_power_off_array[FLASH_MAX] = {
    flash_1_power_off,
};

void flash_porting_power_off(flash_id_t id)
{
    g_flash_power_off_array[id](id);
}

void flash_porting_power_on(flash_id_t id)
{
    g_flash_power_on_array[id](id);
}

void flash_porting_spi_lock_create(flash_id_t id)
{
    unused(id);
}

void flash_porting_spi_lock_delete(flash_id_t id)
{
    unused(id);
}

uint32_t flash_porting_spi_lock(flash_id_t id)
{
    unused(id);
    unsigned int irq_sts = osal_irq_lock();
    return irq_sts;
}

void flash_porting_spi_unlock(flash_id_t id, uint32_t status)
{
    unused(id);
    osal_irq_restore(status);
}

void flash_porting_wait_exit_xip_mode(void)
{
    while (flash_porting_get_bt_power_on_flag() == false) { }
    return;
}

void flash_porting_set_bt_power_on_flag(bool flag)
{
    g_bt_power_on = flag;
}

bool flash_porting_get_bt_power_on_flag(void)
{
    return g_bt_power_on;
}

// 适配
#define KV_PAGE_OFFSET 0
static uint32_t g_flash_size = 0;
static flash_support_manufacturer_t g_flash_manufacturer = FLASH_MANUFACTURER_MAX;

void flash_config_save_info(void)
{
#if defined(BUILD_APPLICATION_SSB)
    flash_info_t flash_info;
    uapi_flash_get_info(0, &flash_info);
    g_flash_size = flash_info.flash_size;
    PRINT("PRINT Eflash id:%x, size:0x%x\r\n", flash_info.flash_id, flash_info.flash_size);
#endif
}

uint32_t flash_config_get_end(void)
{
#if !defined(BUILD_APPLICATION_ROM)
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return EMBED_FLASH_LEN + FLASH_START;
    }
#endif
    flash_info_t flash_info;
    if (g_flash_size == 0) {
        uapi_flash_get_info(0, &flash_info);
        g_flash_size = flash_info.flash_size;
    }
    return (g_flash_size + FLASH_START);
#else
    return FLASH_END;
#endif
}

uint16_t flash_config_get_pages(void)
{
#if !defined(BUILD_APPLICATION_ROM)
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return EMBED_FLASH_LEN / FLASH_PAGE_SIZE;
    }
#endif
    flash_info_t flash_info;
    if (g_flash_size == 0) {
        uapi_flash_get_info(0, &flash_info);
        g_flash_size = flash_info.flash_size;
    }
    return (uint16_t)(g_flash_size / FLASH_PAGE_SIZE);
#else
    return FLASH_PAGES;
#endif
}

uint32_t flash_config_get_start_addr(void)
{
#if (EMBED_FLASH_EXIST == YES) && defined(BUILD_APPLICATION_SSB)
    if (bootloader_configuration_get_flash_mode() == EMBED_FLASH_XIP) {
        return EMBED_FLASH_START;
    }
#endif
    return FLASH_START;
}

uint16_t flash_get_page(uint32_t address)
{
    return (uint16_t)((address - FLASH_START) / FLASH_PAGE_SIZE);
}

uint16_t flash_config_get_kv_pages(void)
{
    return KV_PAGE_OFFSET;
}

extern flash_cfg_t g_flash_config[FLASH_MAX];

void flash_funcreg_adapt(flash_id_t id)
{
    flash_porting_get_config(id, g_flash_config);
    hal_xip_set_cur_mode((xip_id_t)id, XIP_MODE_NORMAL);
    g_flash_config[id].mode = HAL_SPI_FRAME_FORMAT_QUAD;

    spi_funcreg_adapt(g_flash_config[id].bus) ;
    g_flash_config[id].isinit = 1;
    return;
}

errcode_t flash_read_unique_id(flash_id_t id, uint8_t *unique_id)
{
    unused(id);
    uint8_t unique_cmd[] = { FLASH_UQID_CMD, FLASH_DUMMY, FLASH_DUMMY, FLASH_DUMMY, FLASH_DUMMY};
    uint8_t i = 0;
    uint8_t cnt = sizeof(unique_cmd);
    hal_spi_interface_ctrlr0_reg ctrl0 = { 0 };

    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;

    hal_qspi_disable(FLASH_QSPI_ID);
    hal_qspi_ctlr0_cfg(FLASH_QSPI_ID, ctrl0.d32);
    hal_qspi_ctlr1_cfg(FLASH_QSPI_ID, sizeof(uint8_t) - 1); // read buff_length byte data
    hal_qspi_enable(FLASH_QSPI_ID);
    hal_qspi_slave_disable(FLASH_QSPI_ID);
    do {
        hal_qspi_write_data(FLASH_QSPI_ID, unique_cmd[i]);
        i++;
    } while (i < cnt);
    hal_qspi_slave_enable(FLASH_QSPI_ID);

    qspi_ret_t ret = (qspi_ret_t)qspi_recv_data_by_byte(FLASH_QSPI_ID, unique_id, sizeof(uint8_t));
    if (ret != QSPI_RET_OK) {
        return ERRCODE_FLASH_SPI_TRANS_FAIL;
    }
    return ERRCODE_SUCC;
}

void qspi0_suspend(void)
{
    uapi_flash_exit_from_xip_mode(FLASH_1);
    hal_xip_set_cur_mode((xip_id_t)FLASH_1, XIP_MODE_DISABLE);
    spi_save_reg(SPI_BUS_3); // QSPI0 for norflash
    uapi_flash_switch_to_deeppower(FLASH_1);
    // make CS to high in deepsleep.
    reg16_setbit(QSPI0_FLASH_CS0_PAD, PAD_PE_BIT);
    reg16_setbit(QSPI0_FLASH_CS0_PAD, PAD_PS_BIT);
}

void qspi0_resume(void)
{
    // qspi0 pinmux restore
    writew(QSPI0_FLASH_D0_PIN, PIN_FUNC_QSPI0);
    writew(QSPI0_FLASH_D1_PIN, PIN_FUNC_QSPI0);
    writew(QSPI0_FLASH_D2_PIN, PIN_FUNC_QSPI0);
    writew(QSPI0_FLASH_D3_PIN, PIN_FUNC_QSPI0);
    writew(QSPI0_FLASH_CLK_PIN, PIN_FUNC_QSPI0);
    writew(QSPI0_FLASH_CS0_PIN, PIN_FUNC_QSPI0);
    reg16_setbit(QSPI0_FLASH_D0_PAD, PAD_IE_BIT);
    reg16_setbit(QSPI0_FLASH_D1_PAD, PAD_IE_BIT);
    reg16_setbit(QSPI0_FLASH_D2_PAD, PAD_IE_BIT);
    reg16_setbit(QSPI0_FLASH_D3_PAD, PAD_IE_BIT);
    reg16_setbit(QSPI0_FLASH_CLK_PAD, PAD_IE_BIT);
    reg16_setbit(QSPI0_FLASH_CS0_PAD, PAD_IE_BIT);
    // make CS to pull none in normal.
    reg16_clrbit(QSPI0_FLASH_CS0_PAD, PAD_PS_BIT);
    reg16_clrbit(QSPI0_FLASH_CS0_PAD, PAD_PE_BIT);

    spi_recovery_reg(SPI_BUS_3); // QSPI0 for norflash
    uapi_flash_resume_from_deeppower(FLASH_1);
    uapi_flash_switch_to_cache_mode(0);
    if (readb(FLASH_START) != 0x6f) {
        PRINT("Enter XIP with value: 0x%x failed\r\n", readw(FLASH_START) == 0x6f);
    }
}