/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Qspi display bus code encapsulation.
 * Author: @CompanyNameTag
 * Create: 2022-08-05
 */

#include "lcd_qspi.h"
#include "stdio.h"
#include "soc_errno.h"
#include "soc_lcd_api.h"
#include "lcd_bus.h"
#include "tcxo.h"
#include "qspi_display.h"
#include "hal_gpio.h"
#include "hal_dma.h"
#include "gpio.h"
#include "platform_core.h"
#include "dma_porting.h"
#include "lcd_common.h"

#define FORMAT_PIXEL_BYTE         4
#define STRIDE_POWER              2
#define QSPI_DR             (QSPI_2_BASE_ADDR + 0x60)

static uint32_t g_lcd_qspi_col_byte;
static uint32_t g_lcd_qspi_row;

static uint32_t get_stride_num(uint32_t col_byte)
{
    uint32_t res = 0;
    uint32_t temp = col_byte;
    uint32_t count = 0;
    const uint32_t shift = 1;

    if (col_byte == 1) {
        return 1;
    }
#ifdef SUPPORT_GPU_GMMU
    while (temp > 0) {
        count++;
        temp /= 2; /* 2 for sqrt */
    }
    res = shift << count;
    if ((col_byte < res) && (2 * col_byte > res)) { /* 2 for sqrt */
        return res;
    } else {
        return col_byte;
    }
#else
    res = (col_byte + 64 - 1) & ~(64 - 1); /* Non-GMMU mode: 64-byte alignment */
    return res;
#endif
}

ext_errno lcd_qspi_init(combo_dev_cfg_t *bus_cfg)
{
    if (bus_cfg->out_format != OUT_FORMAT_RGB_24BIT) {
        return EXT_ERR_LCD_QSPI_NOT_SUPPORT_MODE;
    }

    uapi_qspi_display_init();

    g_lcd_qspi_col_byte = bus_cfg->sync_info.hact_pixels * FORMAT_PIXEL_BYTE;
    g_lcd_qspi_row = bus_cfg->sync_info.vact_lines;

    return EXT_ERR_SUCCESS;
}

ext_errno lcd_qspi_deinit(void)
{
    return uapi_qspi_display_deinit();
}

static ext_errno lcd_qspi_enable_by_addr(uint32_t addr, uint32_t ch)
{
    qspi_display_frame_info_t info = {0};

    info.frame_addr = addr;
    info.frame_col_byte = g_lcd_qspi_col_byte;
    info.frame_raw = g_lcd_qspi_row;
    info.col_byte_with_stride = get_stride_num(g_lcd_qspi_col_byte);
    info.channel = ch;

    return uapi_qspi_display_enable(&info);
}

ext_errno lcd_qspi_enable(void)
{
    ext_errno ret = EXT_ERR_SUCCESS;

#ifdef DISPLAY_RAM1
    ret = lcd_qspi_enable_by_addr(DISPLAY_RAM1, DMA_CHANNEL_RESERVED_0);
    if (ret != EXT_ERR_SUCCESS) {
        return ret;
    }
#endif

#ifdef DISPLAY_RAM2
    ret = lcd_qspi_enable_by_addr(DISPLAY_RAM2, DMA_CHANNEL_RESERVED_1);
    if (ret != EXT_ERR_SUCCESS) {
        uapi_qspi_display_disable();
    }
#endif

    return ret;
}

ext_errno lcd_qspi_disable(void)
{
    return uapi_qspi_display_disable();
}

ext_errno lcd_qspi_write_cmd(lcd_cmd_sequ *cmd)
{
    ext_errno ret;
    qspi_cmd_info_t qspi_cmd = {0};

    if (cmd->data_len == 0) {
        return EXT_ERR_LCD_INVALID_DATA_LENGTH;
    }

    /* qspi reg addr is 24bit */
    qspi_cmd.reg_addr = (uint32_t)cmd->data[0] << 8; /* 8: reg_addr byte 1 */

    qspi_cmd.data_len = cmd->data_len - 1;
    if (qspi_cmd.data_len > 0) {
        qspi_cmd.data = &(cmd->data[1]);
    }

    ret = uapi_qspi_display_write_cmd(&qspi_cmd);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("line[%d] qspi cmd send failed, ret=0x%col_byte\r\n", __LINE__, ret);
        return ret;
    }

    if (cmd->delay_ms != 0) {
        uapi_tcxo_delay_ms(cmd->delay_ms);
    }

    return EXT_ERR_SUCCESS;
}

ext_errno lcd_qspi_read_cmd(uint32_t cmd, uint8_t *buff, uint8_t len)
{
    return uapi_qspi_display_read_cmd(cmd << 8, buff, len); /* 8: reg_addr byte 1 */
}

ext_errno lcd_qspi_send_frame(uint32_t addr, qspi_send_frame_cb_t callback)
{
    return uapi_qspi_display_show_on(addr, callback);
}

ext_errno lcd_qspi_register_te_isr(gpio_callback_t callback)
{
    ext_errno ret;

    uapi_gpio_set_dir(DISPLAY_TE_GPIO, GPIO_DIRECTION_INPUT);

    if (uapi_gpio_register_isr_func(DISPLAY_TE_GPIO, GPIO_INTERRUPT_RISING_EDGE, callback) == ERRCODE_SUCC) {
        return EXT_ERR_SUCCESS;
    }

    return EXT_ERR_LCD_QSPI_TE_REGISTER_FAILED;
}

ext_errno lcd_qspi_suspend(void)
{
    ext_errno ret;

    while (is_qspi_transfer_done() != true) {}

    ret = lcd_qspi_deinit();
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("bus_deinit fail, ret = %X\r\n", ret);
        return ret;
    }

    return EXT_ERR_SUCCESS;
}

ext_errno lcd_qspi_resume(void)
{
    ext_errno ret;
    lcd_drv_common *lcd_drv_comm_api = NULL;
    lcd_drv_cfg *lcd_cfg = NULL;
    hal_dma_transfer_base_config_t transfer_config = {0};
    hal_dma_transfer_peri_config_t peripheral_config = {0};

    lcd_drv_comm_api = lcd_drv_get_common_api();
    if (lcd_drv_comm_api == NULL) {
        lcd_err("[resume]lcd_drv_comm_api is null.\r\n");
        return EXT_ERR_FAILURE;
    }

    lcd_cfg = lcd_drv_comm_api->get_lcd_config();
    if (lcd_cfg == NULL) {
        lcd_err("[resume]lcd_cfg is null.\r\n");
        return EXT_ERR_FAILURE;
    }

    ret = lcd_qspi_init(lcd_cfg->bus_cfg);
    if (ret != EXT_ERR_SUCCESS) {
        lcd_err("[resume]bus_init fail, ret = %X\r\n", ret);
        return ret;
    }

    transfer_config.dest = QSPI_DR;
    transfer_config.transfer_num = (uint16_t)(g_lcd_qspi_col_byte / (1 << HAL_DMA_TRANSFER_WIDTH_32));
    transfer_config.src_width = HAL_DMA_TRANSFER_WIDTH_32;
    transfer_config.dest_width = HAL_DMA_TRANSFER_WIDTH_32;
    transfer_config.src_burst_trans_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    transfer_config.dest_burst_trans_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    transfer_config.src_inc = HAL_DMA_ADDRESS_INC_INCREMENT;
    transfer_config.dest_inc = HAL_DMA_ADDRESS_INC_NO_CHANGE;

    peripheral_config.trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    peripheral_config.hs_source = HAL_DMA_HANDSHAKING_QSPI2_1CS_TX;
    peripheral_config.hs_dest = HAL_DMA_HANDSHAKING_QSPI2_1CS_TX;
    peripheral_config.trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;

#ifdef DISPLAY_RAM1
    dmac_port_config_periph(DMA_CHANNEL_RESERVED_0, &transfer_config, &peripheral_config);
#endif
#ifdef DISPLAY_RAM2
    dmac_port_config_periph(DMA_CHANNEL_RESERVED_1, &transfer_config, &peripheral_config);
#endif
    return EXT_ERR_SUCCESS;
}
