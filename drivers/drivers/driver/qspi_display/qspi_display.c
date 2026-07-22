/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: Display with qspi code encapsulation.
 * Author: @CompanyNameTag
 * Create: 2022-09-05
 */

#include "qspi_display.h"
#include "stdio.h"
#include "soc_osal.h"
#include "soc_errno.h"
#include "debug_print.h"
#include "tcxo.h"
#include "hal_dma.h"
#include "pinctrl_porting.h"
#include "dma.h"
#include "chip_io.h"
#include "osal_semaphore.h"
#include "gpio.h"
#include "tcxo_porting.h"

#define QSPI_DMA_HANDSHAKE     HAL_DMA_HANDSHAKING_QSPI2_1CS_TX
#define DST_WORLD_WIDTH        HAL_DMA_TRANSFER_WIDTH_32
#define SRC_WORD_WIDTH         HAL_DMA_TRANSFER_WIDTH_32

#define QSPI_LCD_BASE_ADDR   QSPI_2_BASE_ADDR

#define QSPI_CTRLR0         (QSPI_LCD_BASE_ADDR + 0x0)
#define QSPI_CTRLR1         (QSPI_LCD_BASE_ADDR + 0x4)
#define QSPI_SSIENR         (QSPI_LCD_BASE_ADDR + 0x8)
#define QSPI_MWCR           (QSPI_LCD_BASE_ADDR + 0xc)
#define QSPI_SER            (QSPI_LCD_BASE_ADDR + 0x10)
#define QSPI_BAUDR          (QSPI_LCD_BASE_ADDR + 0x14)
#define QSPI_TXFTLR         (QSPI_LCD_BASE_ADDR + 0x18)
#define QSPI_RXFTLR         (QSPI_LCD_BASE_ADDR + 0x1c)
#define QSPI_TXFLR          (QSPI_LCD_BASE_ADDR + 0x20)
#define QSPI_RXFLR          (QSPI_LCD_BASE_ADDR + 0x24)
#define QSPI_SR             (QSPI_LCD_BASE_ADDR + 0x28)
#define QSPI_IMR            (QSPI_LCD_BASE_ADDR + 0x2c)
#define QSPI_ISR            (QSPI_LCD_BASE_ADDR + 0x30)
#define QSPI_RISR           (QSPI_LCD_BASE_ADDR + 0x34)
#define QSPI_TXOICR         (QSPI_LCD_BASE_ADDR + 0x38)
#define QSPI_RXOICR         (QSPI_LCD_BASE_ADDR + 0x3c)
#define QSPI_RXUICR         (QSPI_LCD_BASE_ADDR + 0x40)
#define QSPI_MSTICR         (QSPI_LCD_BASE_ADDR + 0x44)
#define QSPI_ICR            (QSPI_LCD_BASE_ADDR + 0x48)
#define QSPI_DMACR          (QSPI_LCD_BASE_ADDR + 0x4c)
#define QSPI_DMATDLR        (QSPI_LCD_BASE_ADDR + 0x50)
#define QSPI_DMARDLR        (QSPI_LCD_BASE_ADDR + 0x54)
#define QSPI_IDR            (QSPI_LCD_BASE_ADDR + 0x58)
#define QSPI_SSI_VERSION_ID (QSPI_LCD_BASE_ADDR + 0x5c)
#define QSPI_DR             (QSPI_LCD_BASE_ADDR + 0x60)
#define QSPI_RX_SAMPLE_DLY  (QSPI_LCD_BASE_ADDR + 0xf0)
#define QSPI_SPI_CTRLR0     (QSPI_LCD_BASE_ADDR + 0xf4)
#define QSPI_TXD_DRIVE_EDGE (QSPI_LCD_BASE_ADDR + 0xf8)

#define PAD_CTRL_IE_EN             0x2
#define S_MGPIO_PAD_CTRL_BASE      0x5700C800
#define S_MGPIO27_PAD_CTRL         (S_MGPIO_PAD_CTRL_BASE + 0x26c)
#define S_MGPIO28_PAD_CTRL         (S_MGPIO_PAD_CTRL_BASE + 0x270)
#define S_MGPIO29_PAD_CTRL         (S_MGPIO_PAD_CTRL_BASE + 0x274)
#define S_MGPIO30_PAD_CTRL         (S_MGPIO_PAD_CTRL_BASE + 0x278)

#define QSPI_BUS_PRI5              (M_CTL_RB_BASE + 0xA14)
#define QSPI_M1_HIGHEST_PRI        0x22

#define QSPI_RESET_LCD_GPIO        S_HGPIO0

#define QSPI_CTRL0_8BIT_TONLY_STD     0x70100
#define QSPI_CTRL0_32BIT_EEPROM_QUAD  0x5F0300
#define QSPI_CTRL0_8BIT_TONLY_QUAD    0x470100
#define QSPI_CTRL0_24BIT_TONLY_QUAD   0x570100
#define QSPI_CTRL0_32BIT_TONLY_QUAD   0x5f0100
#define QSPI_SPI_CTRLR0_8INS_24ADDR   0x218
#define QSPI_SPI_CTRLR0_8INS_24QADDR  0x219
#define QSPI_SPI_CTRLR0_0INS_24QADDR  0x19
#define QSPI_INT_MASK                 0x1E

#define LCD_READ_CMD                  0x3
#define LCD_WRITE_STD_CMD             0x2
#define LCD_WRITE_ADDR_DATA_QUAD_CMD  0x12
#define LCD_WRITE_DATA_QUAD_CMD       0x32
#define LCD_WRITE_DATA_QUAD_ADDR      0x2C00

#define BITS_IN_WORD                  32
#define BITS_IN_BYTE                  8
#define ADDR_2BYTE_SHIFT              (2 * BITS_IN_BYTE)

#define QSPI_BAUD_CLK_DIV             32  /* baud = 32M / 32 */
#define QSPI_FIFO_THRESHOLD           0xD
#define TX_FIFO_NOT_FULL_MASK         0x2
#define QSPI_OPERATE_TIMEOUT          512

#define INPUT_HEX                     16
#define BYTES_PER_PIXEL               3
#define BYTES_PER_PIXEL_AMEND         4
#define DMA_TRANS_NUM_ALIGN           4092

#define QSPI_DMA_LLI_NUM              2
#define BYTE_MASK                     0xff
#define QSPI_SER_DELAY                10  /* us */
#define QSPI_TX_DELAY                 1   /* us */
#define QSPI_HW_DELAY                 10  /* us */

#define QSPI_SSI_DISABLE               0
#define QSPI_SSI_ENABLE                1
#define QSPI_WH_DISABLE                0
#define QSPI_WH_TX_ENABLE              2
#define QSPI_SLAVE_DISABLE             0
#define QSPI_SLAVE_ENABLE              1
#define QSPI_DMA_TX_LEVEL              0xC


typedef struct {
    uint32_t src_addr;
    dma_channel_t ch;
} qspi_ch_addr_info_t;

qspi_ch_addr_info_t g_qspi_ch_info[QSPI_DMA_LLI_NUM];

volatile bool g_qspi_display_is_init;
volatile bool g_qspi_dma_tranfer_done = true;
volatile bool g_qspi_dma_tranfer_success = false;

qspi_send_frame_cb_t g_qspi_display_callback = NULL;
volatile uint32_t g_qspi_clk_div = 2;

static void qspi_io_init(void)
{
    /* pinctrl */
    writel(S_MGPIO27_PAD_CTRL, PAD_CTRL_IE_EN);
    writel(S_MGPIO28_PAD_CTRL, PAD_CTRL_IE_EN);
    writel(S_MGPIO29_PAD_CTRL, PAD_CTRL_IE_EN);
    writel(S_MGPIO30_PAD_CTRL, PAD_CTRL_IE_EN);
}

static void qspi_set_m1_highest_pri(void)
{
    writel(QSPI_BUS_PRI5, QSPI_M1_HIGHEST_PRI);
}

bool is_qspi_transfer_done(void)
{
    return g_qspi_dma_tranfer_done;
}

ext_errno uapi_qspi_display_init(void)
{
    uint32_t irq_sts = osal_irq_lock();
    qspi_set_m1_highest_pri();
    qspi_io_init();
    writel(QSPI_SSIENR, 0x0);
    writel(QSPI_CTRLR0, QSPI_CTRL0_8BIT_TONLY_STD);
    writel(QSPI_SER, 0x1); /* slave_enable */
    writel(QSPI_BAUDR, QSPI_BAUD_CLK_DIV);
    writel(QSPI_IMR, QSPI_INT_MASK);
    writel(QSPI_TXFTLR, QSPI_FIFO_THRESHOLD);
    writel(QSPI_RXFTLR, QSPI_FIFO_THRESHOLD);
    writel(QSPI_SSIENR, 0x1);

    g_qspi_display_is_init = true;
    osal_irq_restore(irq_sts);
    return EXT_ERR_SUCCESS;
}

ext_errno uapi_qspi_display_deinit(void)
{
    if (!g_qspi_display_is_init) {
        return EXT_ERR_SUCCESS;
    }

    g_qspi_display_is_init = false;
    return EXT_ERR_SUCCESS;
}

static ext_errno qspi_create_lli(qspi_display_frame_info_t *frame_info, dma_channel_t ch)
{
    dma_ch_user_peripheral_config_t user_cfg = {0};

    user_cfg.dest = QSPI_DR;
    user_cfg.transfer_num = (uint16_t)(frame_info->frame_col_byte / (1 << SRC_WORD_WIDTH));
    user_cfg.dest_width = DST_WORLD_WIDTH;
    user_cfg.src_width = SRC_WORD_WIDTH;
    user_cfg.src_increment = HAL_DMA_ADDRESS_INC_INCREMENT;
    user_cfg.dest_increment = HAL_DMA_ADDRESS_INC_NO_CHANGE;
    user_cfg.burst_length = HAL_DMA_BURST_TRANSACTION_LENGTH_4;
    user_cfg.trans_type = HAL_DMA_TRANS_MEMORY_TO_PERIPHERAL_DMA;
    user_cfg.trans_dir = HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL;
    user_cfg.dest_handshaking = QSPI_DMA_HANDSHAKE;
    user_cfg.src_handshaking = QSPI_DMA_HANDSHAKE;

    for (uint32_t i = 0; i < frame_info->frame_raw; i++) {
        user_cfg.src = frame_info->frame_addr + i * frame_info->col_byte_with_stride;
        if (uapi_dma_configure_peripheral_transfer_lli(ch, &user_cfg, NULL) != ERRCODE_SUCC) {
            uapi_dma_end_transfer(ch);
            return EXT_ERR_QSPI_DISPLAY_FAILED;
        }
    }

    return EXT_ERR_SUCCESS;
}

static uint8_t qspi_display_addr_exist(uint32_t addr)
{
    uint8_t id;

    for (id = 0; id < QSPI_DMA_LLI_NUM; id++) {
        if (g_qspi_ch_info[id].src_addr == addr) {
            break;
        }
    }

    return id;
}

static uint8_t qspi_display_get_info_id(uint32_t addr)
{
    uint8_t id;

    UNUSED(addr);
    for (id = 0; id < QSPI_DMA_LLI_NUM; id++) {
        if (g_qspi_ch_info[id].src_addr == 0) {
            break;
        }
    }

    return id;
}

ext_errno uapi_qspi_display_enable(qspi_display_frame_info_t *frame_info)
{
    uint8_t id;
    ext_errno ret;

    if (!g_qspi_display_is_init) {
        return EXT_ERR_QSPI_D_NOT_INIT;
    }

    if (qspi_display_addr_exist(frame_info->frame_addr) < QSPI_DMA_LLI_NUM) {
        return EXT_ERR_QSPI_REENABLE;
    }

    id = qspi_display_get_info_id(frame_info->frame_addr);
    if (id >= QSPI_DMA_LLI_NUM) {
        return EXT_ERR_QSPI_DMA_LLI_FULL;
    }

    uint32_t irq_sts = osal_irq_lock();
    ret = qspi_create_lli(frame_info, frame_info->channel);
    if (ret != EXT_ERR_SUCCESS) {
        osal_irq_restore(irq_sts);
        return ret;
    }

    g_qspi_ch_info[id].ch = frame_info->channel;
    g_qspi_ch_info[id].src_addr = frame_info->frame_addr;
    osal_irq_restore(irq_sts);

    return EXT_ERR_SUCCESS;
}

ext_errno uapi_qspi_display_disable(void)
{
    uint32_t lli_id;

    for (lli_id = 0; lli_id < QSPI_DMA_LLI_NUM; lli_id++) {
        if (g_qspi_ch_info[lli_id].src_addr != 0) {
            uapi_dma_end_transfer(g_qspi_ch_info[lli_id].ch);
            g_qspi_ch_info[lli_id].src_addr = 0;
        }
    }
    return EXT_ERR_SUCCESS;
}

static bool qspi_tx_fifo_is_full(void)
{
    return (readl(QSPI_SR) & TX_FIFO_NOT_FULL_MASK) == 0;
}

static void qspi_write_data(uint32_t data)
{
    writel(QSPI_DR, data);
}

ext_errno uapi_qspi_display_write_cmd(qspi_cmd_info_t *cmd_info)
{
    uint32_t cnt = 0;

    if (!g_qspi_display_is_init) {
        return EXT_ERR_QSPI_D_NOT_INIT;
    }

    uint32_t irq_sts = osal_irq_lock();
    writel(QSPI_SSIENR, 0x0);
    writel(QSPI_CTRLR0, QSPI_CTRL0_8BIT_TONLY_STD);
    writel(QSPI_SPI_CTRLR0, QSPI_SPI_CTRLR0_8INS_24ADDR);

    writel(QSPI_SER, 0x0);
    writel(QSPI_SSIENR, 0x1);
    qspi_write_data(LCD_WRITE_STD_CMD);
    qspi_write_data((cmd_info->reg_addr >> 16) & BYTE_MASK); /* 16: reg_addr byte 2 */
    qspi_write_data((cmd_info->reg_addr >> 8) & BYTE_MASK);  /* 8: reg_addr byte 1 */
    qspi_write_data(cmd_info->reg_addr & BYTE_MASK);

    writel(QSPI_SER, 0x1);
    while (cnt < cmd_info->data_len) {
        if (!qspi_tx_fifo_is_full()) {
            qspi_write_data(cmd_info->data[cnt]);
            cnt++;
        }
    }

    while (readl(QSPI_TXFLR) != 0) {}
    uapi_tcxo_delay_us(5); /* 5: avoid missing last few bits  */

    osal_irq_restore(irq_sts);
    return EXT_ERR_SUCCESS;
}

static uint8_t quad_data_convert_to_u8(uint32_t quad_data)
{
    uint32_t bit = 0;
    uint8_t offset = 0;
    uint8_t sum = 0;
    while (bit < BITS_IN_WORD) {
        uint8_t valid_bit = (uint8_t)((quad_data >> bit) & 0x1); /* only 1bit valid in 4bits */
        sum += valid_bit * (1 << offset);
        bit += 4; /* 4: obtain 4bits data every loop */
        offset++;
    }

    return sum;
}

static ext_errno recv_data_by_byte(uint8_t *buffer, uint32_t length)
{
    uint32_t ul_loop = 0;
    uint32_t ul_recv_timeout = 0;
    uint32_t ul_recv_num = 0;
    uint32_t tmp_data;

    while (ul_loop < length) {
        ul_recv_num = readl(QSPI_RXFLR);

        while (ul_recv_num > 0) {
            tmp_data = ((uint32_t)readl(QSPI_DR));
            buffer[ul_loop] = quad_data_convert_to_u8(tmp_data);
            ul_loop++;
            ul_recv_num--;
            ul_recv_timeout = 0;
        }

        ul_recv_timeout++;

        if (ul_recv_timeout >= QSPI_OPERATE_TIMEOUT) {
            return EXT_ERR_QSPI_READ_TIMEOUT;
        }
    }

    return EXT_ERR_SUCCESS;
}

ext_errno uapi_qspi_display_read_cmd(uint32_t cmd, uint8_t *buff, uint8_t len)
{
    if (!g_qspi_display_is_init) {
        return EXT_ERR_QSPI_D_NOT_INIT;
    }

    uint32_t irq_sts = osal_irq_lock();
    writel(QSPI_SSIENR, 0x0);
    writel(QSPI_CTRLR0, QSPI_CTRL0_32BIT_EEPROM_QUAD);
    writel(QSPI_SPI_CTRLR0, QSPI_SPI_CTRLR0_8INS_24ADDR);
    writel(QSPI_CTRLR1, len - 1);
    writel(QSPI_BAUDR, QSPI_BAUD_CLK_DIV);
    writel(QSPI_SSIENR, 0x1);

    writel(QSPI_SER, 0x0);
    qspi_write_data(LCD_READ_CMD);
    qspi_write_data(cmd);
    writel(QSPI_SER, 0x1);

    ext_errno ret = recv_data_by_byte(buff, len);
    osal_irq_restore(irq_sts);

    return ret;
}

static void qspi_dma_tranfer_callback(hal_dma_interrupt_t int_type)
{
    uapi_pin_set_mode(S_MGPIO32, (pin_mode_t)HAL_PIO_FUNC_QSPI2);

    switch (int_type) {
        case HAL_DMA_INTERRUPT_TFR:
            g_qspi_dma_tranfer_done = true;
            g_qspi_dma_tranfer_success = true;
            break;
        case HAL_DMA_INTERRUPT_BLOCK:
            g_qspi_dma_tranfer_done = true;
            g_qspi_dma_tranfer_success = false;
            break;
        case HAL_DMA_INTERRUPT_ERR:
            g_qspi_dma_tranfer_done = true;
            g_qspi_dma_tranfer_success = false;
            break;
        default:
            break;
    }

    if (g_qspi_display_callback != NULL && g_qspi_dma_tranfer_success) {
        g_qspi_display_callback();
    }
}

ext_errno uapi_qspi_display_show_on(uintptr_t frame_addr, qspi_send_frame_cb_t callback)
{
    uint8_t id;

    if (!g_qspi_display_is_init) {
        return EXT_ERR_QSPI_D_NOT_INIT;
    }

    id = qspi_display_addr_exist(frame_addr);
    if (id >= QSPI_DMA_LLI_NUM) {
        return EXT_ERR_QSPI_NOT_ENABLE;
    }

    g_qspi_display_callback = callback;
    g_qspi_dma_tranfer_done = false;
    g_qspi_dma_tranfer_success = false;

    writel(QSPI_SSIENR, QSPI_SSI_DISABLE);
    writel(QSPI_CTRLR0, QSPI_CTRL0_24BIT_TONLY_QUAD);
    writel(QSPI_SPI_CTRLR0, QSPI_SPI_CTRLR0_8INS_24ADDR);
    writel(QSPI_BAUDR, g_qspi_clk_div);
    writel(QSPI_DMACR, QSPI_WH_DISABLE);
    writel(QSPI_DMATDLR, QSPI_DMA_TX_LEVEL);
    writel(QSPI_SER, QSPI_SLAVE_DISABLE);
    writel(QSPI_SSIENR, QSPI_SSI_ENABLE);
    writel(QSPI_DR, LCD_WRITE_DATA_QUAD_CMD);
    writel(QSPI_DR, LCD_WRITE_DATA_QUAD_ADDR);

    uapi_dma_enable_lli((uint8_t)(g_qspi_ch_info[id].ch), (dma_transfer_cb_t)qspi_dma_tranfer_callback,\
                        (uintptr_t)NULL);
    writel(QSPI_DMACR, QSPI_WH_TX_ENABLE);
    tcxo_port_delay_us(QSPI_SER_DELAY);
    uapi_pin_set_mode(S_MGPIO32, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(S_MGPIO32, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(S_MGPIO32, GPIO_LEVEL_LOW);
    writel(QSPI_SER, QSPI_SLAVE_ENABLE);
    tcxo_port_delay_us(QSPI_TX_DELAY);
    writel(QSPI_DMACR, QSPI_WH_DISABLE);
    tcxo_port_delay_us(QSPI_HW_DELAY);
    writel(QSPI_SSIENR, QSPI_SSI_DISABLE);
    writel(QSPI_SPI_CTRLR0, QSPI_SPI_CTRLR0_0INS_24QADDR);
    writel(QSPI_SSIENR, QSPI_SSI_ENABLE);
    writel(QSPI_DMACR, QSPI_WH_TX_ENABLE);

    if (g_qspi_display_callback == NULL) {
        while (!g_qspi_dma_tranfer_done) {}

        if (!g_qspi_dma_tranfer_success) {
            PRINT("QSPI DISPLAY failed, dma tranfer wrong!\r\n");
            return EXT_ERR_QSPI_DISPLAY_FAILED;
        }
    }

    return EXT_ERR_SUCCESS;
}
