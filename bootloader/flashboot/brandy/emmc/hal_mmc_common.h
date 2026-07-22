/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:   HAL MMC COMMON DRIVER HEADER FILE
 * Author:
 * Create:  2020-04
 */
#ifndef HAL_MMC_COMMON_H
#define HAL_MMC_COMMON_H

#include "mmcsd_host.h"
#include "chip_io.h"
#include "platform_core.h"

#define MMC_ADMA_MAX_DESC              8
#define MMC_AMDA_32BIT_DESC_LINE_SIZE  8
#define CACHE_ALIGNED_SIZE             64
#define MAX_BLOCK_SIZE                 512
#define MAX_BLOCK_COUNT                4096
#define MAX_SEG_SIZE                   65536
#define OFF_CLK2CARD_ON_DELAY          25
#define SDIO_REG_CCCR_IO_ABORT         0x06    /* function abort/card reset */

#define CONFIG_MMC0_CCLK_MIN        400000          // 400KHz
#define CONFIG_MMC1_CCLK_MIN        100000          // 100KHz

// sdio host & mmc base addr
#define SDIO_HOST_BASE_ADDR         0x52061000      // SDIO HOST base address
#define SDIO_EMMC_BASE_ADDR         0x52062000      // SDIO EMMC base address

// clock config
#define M_CTL_RB_BASE_ADDR          0x52000000

#define SDIO_32M_CLKDIV_NUM_MASK    0x7F
#define SDIO_32M_CLKDIV_NUM_SHIFT   2
#define SDIO_32M_DIV_EN             BIT(1)
#define SDIO_32M_CLKSEL             BIT(0)

#define SDIO_CLKDIV_NUM_MASK        0xF
#define SDIO_CLKDIV_NUM_POST_SHIFT  6
#define SDIO_CLKDIV_NUM_PRE_SHIFT   1
#define SDIO_CLKDIV_SEL_SOC         BIT(10)
#define SDIO_CLKDIV_POST_ENABLE     BIT(5)
#define SDIO_CLKDIV_PRE_ENABLE      BIT(0)
#define SDIO_CLKDIV_NUM_MAX         4

// emmc/sdio host clock crg in suxun CS
#define GLB_CLKEN_REG              (M_CTL_RB_BASE_ADDR + 0x0548)
#if CHIP == TARGET_CHIP_SUSONG
#define SDIOM_CR_CLK_REG           (M_CTL_RB_BASE_ADDR + 0x0568)
#else
#define SDIOM_CR_CLK_REG           (M_CTL_RB_BASE_ADDR + 0x0564)
#endif
#define EMMC_CCLK_DIV_REG          (M_CTL_RB_BASE_ADDR + 0x0100)
#define EMMC_DIV_REG               (M_CTL_RB_BASE_ADDR + 0x0120)
#define SDIO_DIV_REG               (M_CTL_RB_BASE_ADDR + 0x0124)
#define SD_HOST_CCLK_REG           (M_CTL_RB_BASE_ADDR + 0x0110)
#define SDIO_CRG_HOST_100M_REG       (M_CTL_RB_BASE_ADDR + 0x120)
#define SDIO_TM_DIV_REG            (M_CTL_RB_BASE_ADDR + 0x0124)
#define SDIO_CRG_EMMC_32M_REG        (M_CTL_RB_BASE_ADDR + 0x128)
#define SDIO_CRG_HOST_32M_REG        (M_CTL_RB_BASE_ADDR + 0x124)
#define SDIO_CRG_EMMC_100M_REG       (M_CTL_RB_BASE_ADDR + 0x12C)

#define SDIO_EMMC_CCLK_EN                   BIT(0)
#define SDIO_EMMC_CCLK_EN_POS               0
#define SDIO_EMMC_ORI_DIV_EN                BIT(1)
#define SDIO_EMMC_ORI_LOAD_DIV_EN           BIT(2)
#define SDIO_DIV_NUM_MASK                   0xF8  // bit3---bit7
#define EMMC_DIV_NUM_MASK                   0x70  // bit4---bit6
#define SDIO_DIV_NUM_POST_SHIFT             3
#define EMMC_CCLK_DIV_NUM_MASK              0x70
#define EMMC_CCLK_LOAD_DIV_EN               BIT(1)
#define EMMC_EMMC_CCLK_EN                   BIT(0)
#define SDIO_EMMC_DIV_NUM_POST_SHIFT        4
#define SDIO_TM_DIV_EN                      BIT(0)
#define SDIO_TM_DIV_LOAD_EN                 BIT(1)
#define SDIO_TM_DIV_MASK                    0x1FF0
#define SDIO_TM_DIV_NUM_POST_SHIFT          4
#define GLB_CLKEN_SDIOM_EN                  BIT(4)
#define SDIOM_CR_EN                         BIT(0)
#define SDIOM_CR_CLK_SEL_32M                0x2
#define SDIOM_CR_CLK_SEL_400M               0x4
#define SDIOM_CR_CLK_SEL_POST_SHIFT         1
#define SDIOM_CR_CLK_MASK                   0xF

#define SDIO_LGC_EN_CS              BIT(1)
#define SDIO_CRG_EN_CS              BIT(0)
#define SDIO_HOST_RESET_EMMC_CS     BIT(0)
#define SDIO_HOST_RESET_SDIO_CS     BIT(1)

#define SD_BUS_LOAD_DIV_EN          BIT(8)
#define SD_BUS_DIV_NUM_MASK         0xF8 // bit3---bit7
#define SD_BUS_DIV_NUM_POST_SHIFT   3
#define SD_EMMC_CCLK_DIV_4          4
#define SD_BUS_DIV_1                1
#define SD_BUS_DIV_2                2
#define SD_BUS_PVSA1                0xE8
#define SD_BUS_PVSA1_POS            0
#define SD_BUS_PVSA1_LEN            12
#define SD_BUS_MBURST_POS           0
#define SD_BUS_MBURST_LEN           4
#define SD_BUS_MBURST_VAL           8

#define MMC_100M_DIV_NUM            2  // clock source is 193.3MHz
#define MMC_50M_DIV_NUM             4  // clock source is 193.3MHz

// When the clock is less than 32 MHz, the clock source is TCXO(32MHz).
#define MMC_32M_DIV_NUM             1
#define MMC_16M_DIV_NUM             2
#define MMC_400K_DIV_NUM            80
#define MMC_100K_DIV_NUM            320
#define MMC_SRC_32M_TM_DIV_NUM      32
#define MMC_SRC_200M_TM_DIV_NUM     200
#define MMC_FREQ_100K               100000
#define MMC_FREQ_400K               400000
#define MMC_FREQ_16M                16000000   // For FPGA Only
#define MMC_FREQ_32M                32000000
#define MMC_FREQ_50M                50000000
#define MMC_FREQ_100M               100000000

// reset config
#define MCPU_SOFT_RST               (M_CTL_RB_BASE_ADDR + 0x0054)
#define SOFT_RST_MCU2DIS_H2H_N      BIT(7)
#define SOFT_RST_MDMA_H2X_N         BIT(6)
#define SOFT_RST_MEM_SUB_BUS_N      BIT(5)
#define SOFT_RST_MDIAG_N            BIT(4)
#define SOFT_RST_SEC_N              BIT(3)
#define SOFT_RST_AUX_ADC_N          BIT(2)
#define SOFT_RST_SDIO_BUS_N         BIT(1)
#define SOFT_RST_IR_N               BIT(0)

#define SDIO_CRG_SOFT_RST           (M_CTL_RB_BASE_ADDR + 0x0104)
#define SOFT_RST_DEVICE_SD_N        BIT(7)
#define SOFT_RST_DEVICE_BUS_N       BIT(6)
#define SOFT_RST_HOST_M_B_TM_N      BIT(5)
#define SOFT_RST_HOST_H_N           BIT(4)
#define SOFT_RST_HOST_TX_RX_N       BIT(3)
#define SOFT_RST_EMMC_M_B_TM_N      BIT(2)
#define SOFT_RST_EMMC_H_N           BIT(1)
#define SOFT_RST_EMMC_TX_RX_N       BIT(0)

#define PMU_EMMC_POWER_ON           0xFFFF

#define SDHCI_PLUG_STATE(h)     (mmc_readl(h, MSHC_PRESENT_STATE) & MSHC_CARD_PRESENT)

// sdio slave reg
#define HISDIO_REG_FUNC1_FIFO                      0x00        /* Read Write FIFO */
#define HISDIO_REG_FUNC1_INT_STATUS                0x08        /* interrupt mask and clear reg */
#define HISDIO_REG_FUNC1_INT_ENABLE                0x09        /* interrupt */
#define HISDIO_REG_FUNC1_XFER_COUNT                0x0c        /* notify number of bytes to be read */
#define HISDIO_REG_FUNC1_WRITE_MSG                 0x24        /* write msg to device */
#define HISDIO_REG_FUNC1_MSG_FROM_DEV              0x28        /* notify Host that device has got the msg */
#define HISDIO_REG_FUNC1_MSG_HIGH_FROM_DEV         0x2b        /* Host receive the msg ack */
/* sdio extend function, add 64B register for hcc */
#define HISDIO_FUNC1_EXTEND_REG_BASE        0x3c
#define HISDIO_FUNC1_EXTEND_REG_LEN         64
#define HISDIO_FUNC1_INT_DREADY      (1 << 0)           /* data ready interrupt */
#define HISDIO_FUNC1_INT_RERROR      (1 << 1)           /* data read error interrupt */
#define HISDIO_FUNC1_INT_MFARM       (1 << 2)           /* ARM Msg interrupt */
#define HISDIO_FUNC1_INT_ACK         (1 << 3)           /* ACK interrupt */
#define HISDIO_FUNC1_INT_MASK        (HISDIO_FUNC1_INT_DREADY | HISDIO_FUNC1_INT_RERROR | HISDIO_FUNC1_INT_MFARM)
#define HAL_SDIOM_CLK_MODE_MAX_NUM  4

#define BRANDY_INTERRUPT_SDIO 76
#define BRANDY_INTERRUPT_EMMC 78

#define S_HGPIO 0x52065800
#define S_HGPIO_PAD 0x5700d000

#define S_HGPIO0_OFFSET 0
#define S_HGPIO0_PAD_OFFSET 0x200

#define S_HGPIO6_OFFSET 0x18
#define S_HGPIO6_PAD_OFFSET 0x218
#define FUNC_SDIO 0x01
#define FUNC_EMMC 0x02
#ifdef PRE_FPGA
#define IE_PINMUX_EN 0x02
#else
#define IE_PINMUX_EN 0xC7
#endif

#define COM_CTL_RB_BASE 0x55000000
#define SDIOM_CR_REG (COM_CTL_RB_BASE + 0x0628)
#define SDIOM_CR_CTRL_CH_ENABLE BIT(0)
#define SDIOM_CR_CTRL_DIV_EN BIT(4)

#define SDIOM_CR_CTRL_DIV_OFFSET 5
#define SDIOM_CR_CTRL_MIN_DIV 2

#define SDIOM_CR_CTRL_CLK_RESOURCE_RC 0
#define SDIOM_CR_CTRL_CLK_RESOURCE_XO 1
#define SDIOM_CR_CTRL_CLK_RESOURCE_XO_DLL2 2
#define SDIOM_CR_CTRL_CLK_RESOURCE_PLL 3

#define SDIOM_CR_CTRL_CLK_RESOURCE_PLL_EN (SDIOM_CR_CTRL_CH_ENABLE | (SDIOM_CR_CTRL_CLK_RESOURCE_PLL << 1))
#define SDIOM_CR_CTRL_XO_32M_CLK_EN (SDIOM_CR_CTRL_CH_ENABLE | (SDIOM_CR_CTRL_CLK_RESOURCE_XO << 1))

typedef enum {
    SDIO_EMMC_ID,
    SDIO_HOST_ID,
    MAX_MMC_NUM
} hal_mmc_bus_num_t;

typedef struct {
    int id;
    uint32_t clock;
    uint32_t amend_div_reg;    // Registers that need to be adjusted
    uint32_t amend_div_num;    // The frequency division coefficient of one side needs to be adjusted.
} hal_sdiom_clk_mode_t;

enum dma_data_direction {
    DMA_BIDIRECTIONAL = 0,
    DMA_TO_DEVICE = 1,
    DMA_FROM_DEVICE = 2,
    DMA_NONE = 3,
};

enum hi_mmc_power_mode {
    POWER_OFF = 0,
    POWER_UP,
    POWER_ON,
};

enum mmc_chip_select {
    CS_DONTCARE = 0,
    CS_HIGH,
    CS_LOW,
};

enum hi_mmc_bus_width {
    BUS_WIDTH_1 = 0,
    BUS_WIDTH_4 = 2,
    BUS_WIDTH_8 = 3,
};

enum hi_mmc_bus_mode {
    MMC_BUSMODE_OPENDRAIN,
    MMC_BUSMODE_PUSHPULL,
};

enum mmc_bus_timing {
    TIMING_MMC_DS = 0,
    TIMING_MMC_HS,
    TIMING_SD_HS,
    TIMING_UHS_SDR12,
    TIMING_UHS_SDR25,
    TIMING_UHS_SDR50,
    TIMING_UHS_SDR104,
    TIMING_UHS_DDR50,
    TIMING_UHS_DDR52,
    TIMING_MMC_HS200,  /* for emmc */
    TIMING_MMC_HS400,  /* for emmc */
};

struct hi_mmc_drv {
    struct rt_mmcsd_host *host;
    struct rt_mmcsd_req  *req;
    struct rt_mmcsd_cmd  *cmd;
    struct rt_mmcsd_data *data;

    uint8_t id;
    uint32_t hw_base_addr;
    uint32_t msc_clock;
    uint32_t irq_num;
    uint32_t irq_enable;
    uint32_t version;
    uint32_t freq_min;
    uint32_t freq_max;
    uint32_t freq_default;
    uint32_t preset_enabled;
    uint32_t timing;
    uint32_t flags;
#define MMC_FLAG_USE_SDMA             (1 << 0)
#define MMC_FLAG_USE_ADMA             (1 << 1)
#define MMC_FLAG_REQ_USE_DMA          (1 << 2)
#define MMC_FLAG_DEVICE_DEAD          (1 << 3)
#define MMC_FLAG_SDR50_NEEDS_TUNING   (1 << 4) //
#define MMC_FLAG_NEEDS_RETUNING       (1 << 5) //
#define MMC_FLAG_AUTO_CMD12           (1 << 6) //
#define MMC_FLAG_AUTO_CMD23           (1 << 7)
#define MMC_FLAG_PV_ENABLED           (1 << 8) //
#define MMC_FLAG_SDIO_IRQ_ENABLED     (1 << 9)
#define MMC_FLAG_SDR104_NEEDS_TUNING  (1 << 10) //
#define MMC_FLAG_USING_RETUNING_TIMER (1 << 11)
#define MMC_FLAG_USE_ADMA_64BIT       (1 << 12)
#define MMC_FLAG_NON_INT_HANDLE       (1 << 13)
#define MMC_FLAG_NON_OS_HANDLE        (1 << 14)
#define MMC_FLAG_OS_EVENT_INIT        (1 << 15)
#define MMC_FLAG_NO_DETECT_INSERT     (1 << 16)
    uint32_t type;
#define MMC_TYPE_MMC                  (1 << 0)
#define MMC_TYPE_SDIO                 (1 << 1)
#define MMC_TYPE_SD                   (1 << 2)

    uint32_t pwr;
    char *adma_desc;
    uint32_t adma_desc_sz;
    uint32_t adma_desc_line_sz;
    uint32_t align_buf_sz;
    uint32_t align_bytes;
    uint32_t adma_max_desc;
    unsigned long adma_addr;
    unsigned long align_addr;
    uint16_t vdd;
// MMCSD_BUSMODE_OPENDRAIN 1;MMCSD_BUSMODE_PUSHPULL 2
    uint8_t  bus_mode;       /* command output mode */
// MMCSD_CS_IGNORE 0;MMCSD_CS_HIGH 1;MMCSD_CS_LOW 2
    uint8_t  chip_select;        /* SPI chip select */
// MMC_POWER_OFF 0;MMC_POWER_UP 1;MMC_POWER_ON 2
    uint8_t  power_mode;     /* power supply mode */
// MMCSD_BUS_WIDTH_1 0;MMCSD_BUS_WIDTH_4 2;MMCSD_BUS_WIDTH_8 3
    uint8_t  bus_width;      /* data bus width */
    unsigned int cmdat;
    uint32_t clock;
    uint32_t phy_driver_cap;  /* phy mode driver capability */
    uint8_t *_dma_buffer;
    int sdio_clk; /* clock for sdio */
    uint32_t current_status;

    struct scatterlist    *dma_sg;
    unsigned int    dma_sg_num;
    unsigned int        *dma_vaddr;

    // mmc_event hi_mmc_event;
#define HISI_WAIT_REQUEST_DONE     (1 << 0)
#define HISI_WAIT_ACCIDENT         (1 << 1)
    uint32_t err_int_count;
};

typedef void (* HWI_PROC_FUNC)(void);

static inline void mmc_writel(struct hi_mmc_drv *host, unsigned int val, uint32_t regs)
{
    writel(host->hw_base_addr + regs, val);
}

static inline void mmc_writew(struct hi_mmc_drv *host, unsigned short val, uint32_t regs)
{
    writew(host->hw_base_addr + regs, val);
}

static inline void mmc_writeb(struct hi_mmc_drv *host, unsigned char val, uint32_t regs)
{
    writeb(host->hw_base_addr + regs, val);
}

static inline void mmc_reg32_setbits(struct hi_mmc_drv *host, uint32_t regs, uint32_t p, uint32_t b, uint32_t val)
{
    reg32_setbits(host->hw_base_addr + regs, p, b, val);
}

static inline unsigned int mmc_reg32_getbits(struct hi_mmc_drv *host, uint32_t regs, uint32_t p, uint32_t b)
{
    return reg32_getbits(host->hw_base_addr + regs, p, b);
}

static inline unsigned int mmc_readl(struct hi_mmc_drv *host, uint32_t regs)
{
    return readl(host->hw_base_addr + regs);
}

static inline unsigned short mmc_readw(struct hi_mmc_drv *host, uint32_t regs)
{
    return readw(host->hw_base_addr + regs);
}

static inline unsigned char mmc_readb(struct hi_mmc_drv *host, uint32_t regs)
{
    return readb(host->hw_base_addr + regs);
}

void hal_mmc_set_emmc_ctrl(struct hi_mmc_drv *mmc_drv, uint32_t offset, uint32_t pos, uint32_t bits, uint32_t val);

uint32_t hal_mmc_get_cap1(struct hi_mmc_drv *mmc_drv);

uint32_t hal_mmc_get_cap2(struct hi_mmc_drv *mmc_drv);

void hal_mmc_reset(struct hi_mmc_drv *mmc_drv);

void hi_mmc_dumpregs(struct hi_mmc_drv *host);

void hal_mmc_set_power(struct hi_mmc_drv *mmc_drv, uint8_t power_mode);

void hal_mmc_set_bus_mode(struct hi_mmc_drv *mmc_drv, uint8_t bus_mode);

void hal_mmc_set_bus_width(struct hi_mmc_drv *mmc_drv, uint8_t bus_width);

void hal_mmc_set_chip_set(struct hi_mmc_drv *mmc_drv, uint8_t chip_sel);

void hal_mmc_set_voltage(struct hi_mmc_drv *mmc_drv, uint16_t vdd);

void hi_mmc_enable_plug_irq(struct hi_mmc_drv* mmc_drv, uint32_t irq);

void hi_mmc_set_sample_phase(struct hi_mmc_drv *mmc_drv, uint32_t phase);

void hal_mmc_set_clock(struct hi_mmc_drv *mmc_drv, unsigned int clocknum);

void hal_mmc_enable_plug_irq(struct hi_mmc_drv* mmc_drv, uint32_t irq);

void hal_mmc_set_sample_phase(struct hi_mmc_drv *mmc_drv, uint32_t phase);

void hal_mmc_deassert_status(struct hi_mmc_drv *mmc_drv);

void hal_mmc_clr_reg(struct hi_mmc_drv *mmc_drv, unsigned int mask);

void hal_mmc_set_timing(struct hi_mmc_drv *mmc_drv, int timing);

void hal_mmc_set_preset_val(struct hi_mmc_drv *mmc_drv, bool en);

void hal_mmc_adma_config(struct hi_mmc_drv *mmc_drv);

void hal_mmc_set_card_clk(struct hi_mmc_drv *mmc_drv, bool en);

unsigned int hi_mmc_set_clock_in_fgpa(struct hi_mmc_drv *mmc_drv, unsigned int cfg_clock);

void hal_mmc_set_sdiom_clk_mode(struct hi_mmc_drv** drv, int mode);

void hal_mmc_set_clk_en(hal_mmc_bus_num_t mmc_bus, bool en);

#if (SUPPORT_HI_EMMC_PHY == YES)
void hi_mmc_set_phy_driver_capability(struct hi_mmc_drv *mmc_drv, uint32_t val);
#endif

bool hi_mmc_set_clock_switch(uint32_t fisrt_div, uint32_t second_div, uint32_t sd_bus_div);

#endif
