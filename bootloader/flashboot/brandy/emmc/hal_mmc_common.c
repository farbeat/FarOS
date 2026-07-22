/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: HAL MMC DRIVER.
 * Author:
 * Create:  2020-04
 */
#include "hal_mmc_common.h"
#include "mshc_reg.h"
#include "tcxo.h"
#include "hal_pmu_peripheral_application.h"
#include "non_os.h"
#include "clocks_core.h"
#include "platform_core.h"

#define SDIOM_SRC_CLK_LOW_FREQ_VAL          32000000     // 32MHz
#define SDIOM_SRC_CLK_FREQ_LEVEL_50M_VAL    50000000     // 50MHz
#define SDIOM_SRC_CLK_FREQ_LEVEL_64M_VAL    64000000     // 64MHz
#define SDIOM_SRC_CLK_HIGH_FREQ_VAL         196000000    // 196MHz
#define SDIOM_WITH_SET_TM_AND_FIRST_FREQ_DIV       1
#define SDIOM_WITHOUT_SET_TM_AND_SECOND_FREQ_DIV   2

hal_sdiom_clk_mode_t g_sdiom_clk_cfg[HAL_SDIOM_CLK_MODE_MAX_NUM] = {
    { 0, 50000000, SD_HOST_CCLK_REG,  4 },  // sdio host clk 50MHz, emmc clk 50MHz 50000000
    { 1, 50000000, EMMC_CCLK_DIV_REG, 6 },  // sdio host clk 50MHz, emmc clk 32MHz 50000000
    { 0, 50000000, SD_HOST_CCLK_REG,  6 },  // sdio host clk 32MHz, emmc clk 50Mhz 50000000
    { 1, 32000000, EMMC_CCLK_DIV_REG, 1 },  // sdio host clk 50MHz, emmc clk 32MHz 50000000
};

#if (SUPPORT_HI_EMMC_PHY == YES)
static void hi_mmc_set_phy_ioctl(struct hi_mmc_drv *mmc_drv)
{
    mmc_writel(mmc_drv, PHY_IOCTL_PUPD_VAL, MSHC_PHY_IOCTL_PUPD);
    mmc_writel(mmc_drv, PHY_IOCTL_IOE_VAL, MSHC_PHY_IOCTL_IOE);
}

void hi_mmc_set_phy_driver_capability(struct hi_mmc_drv *mmc_drv, uint32_t val)
{
    uint32_t ronsel = 0;
    uint32_t ronsel2 = 0;
    if (val & PHY_IOCTL_RONSEL_BIT0) { ronsel = PHY_IOCTL_RONSEL_BIT0_VAL_1; }
    if (val & PHY_IOCTL_RONSEL_BIT1) { ronsel |= PHY_IOCTL_RONSEL_BIT1_VAL_1; }
    if (val & PHY_IOCTL_RONSEL_BIT2) { ronsel2 = PHY_IOCTL_RONSEL_BIT2_VAL_1; }

    mmc_writel(mmc_drv, ronsel, MSHC_PHY_IOCTL_RONSEL_1_0);
    mmc_writel(mmc_drv, ronsel2, MSHC_PHY_IOCTL_OD_RONSEL_2);
}

static void hi_mmc_set_phy_invert_phase(struct hi_mmc_drv *mmc_drv)
{
    mmc_writel(mmc_drv, PHY_INV_CLK, DLY_CTL1);
}

static void hi_mmc_set_phy_delay_by_measurement(struct hi_mmc_drv *mmc_drv)
{
    uint32_t val;
    mmc_writel(mmc_drv, PHYINITCTRL_DISABLE, PHYINITCTRL);
    mmc_writel(mmc_drv, PHY_SET_DLY_MEAS_EN, PHYCTRL2);
    mmc_writel(mmc_drv, PHYINITCTRL_ENABLE, PHYINITCTRL);
    while (mmc_readl(mmc_drv, PHYINITCTRL) & PHY_INIT_EN_MASK) {}
    val = mmc_reg32_getbits(mmc_drv, DLY_CTL, PHY_DLY_CODE_R_POS, PHY_DLY_CODE_R_LEN);
    // set the 1/4 T delay (meas val) to DLY_CLT1.dly1_code and DLY_CTL1.dly3.code
    mmc_reg32_setbits(mmc_drv, DLY_CTL1, PHY_DLAY1_CODE_POS, PHY_DLAY1_CODE_LEN, val / 4); // 4
    mmc_reg32_setbits(mmc_drv, DLY_CTL1, PHY_DLAY3_CODE_POS, PHY_DLAY3_CODE_LEN, val / 4); // 4
    mmc_writel(mmc_drv, PHY_SET_DLY_MEAS_DIS, PHYCTRL2);
}
#endif

uint32_t hal_mmc_get_cap1(struct hi_mmc_drv *mmc_drv)
{
    return mmc_readl(mmc_drv, MSHC_CAPABILITIES_1);
}

uint32_t hal_mmc_get_cap2(struct hi_mmc_drv *mmc_drv)
{
    return mmc_readl(mmc_drv, MSHC_CAPABILITIES_2);
}

void hal_mmc_enable_plug_irq(struct hi_mmc_drv* mmc_drv, uint32_t irq)
{
    mmc_writew(mmc_drv, irq & MSHC_NOR_INT_MASK, MSHC_NOR_INT_ENABLE);
    mmc_writew(mmc_drv, (irq & MSHC_ERR_INT_MASK) >> MSHC_ERR_INT_OFFSET, MSHC_ERR_INT_ENABLE);

    mmc_writew(mmc_drv, irq & MSHC_NOR_INT_MASK, MSHC_NOR_SIGNAL_ENABLE);
    mmc_writew(mmc_drv, (irq & MSHC_ERR_INT_MASK) >> MSHC_ERR_INT_OFFSET, MSHC_ERR_SIGNAL_ENABLE);
}

void hal_mmc_set_sample_phase(struct hi_mmc_drv *mmc_drv, uint32_t phase)
{
    uint32_t fields;
    fields = mmc_readl(mmc_drv, MSHC_AUTO_TUNING_STATUS);
    fields &= ~MSHC_CENTER_PH_CODE_MASK;
    fields |= (uint32_t)phase;
    mmc_writel(mmc_drv, fields, MSHC_AUTO_TUNING_STATUS);
}

static void hi_mmc_set_sdio_clk_in_cs(uint32_t fields, uint8_t div_num)
{
    uint16_t value;

    value = readw(fields);
    value &= ~SDIO_EMMC_ORI_LOAD_DIV_EN;
    writew(fields, value);
    value &= SDIO_EMMC_DIV_NUM_MASK;
    value |= (div_num << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    writew(fields, value);
    value |= SDIO_EMMC_ORI_LOAD_DIV_EN;
    writew(fields, value);
    value |= SDIO_EMMC_ORI_DIV_EN;
    writew(fields, value);
    value |= SDIO_EMMC_CCLK_EN;
    writew(fields, value);
}

#if (USE_SDIOM_CLK_CONFIG_WAY == SDIOM_WITH_SET_TM_AND_FIRST_FREQ_DIV)
static void hal_mmc_adapte_clock(unsigned int cfg_clock, unsigned int *div_num,
                                 unsigned int *tm_div_num, unsigned int *clock)
{
    if (cfg_clock >= MMC_FREQ_100M) {
        *div_num = MMC_100M_DIV_NUM;
        *tm_div_num = MMC_SRC_200M_TM_DIV_NUM;
        *clock = MMC_FREQ_100M;
    } else if (cfg_clock >= MMC_FREQ_50M) {
        *div_num = MMC_50M_DIV_NUM;
        *tm_div_num = MMC_SRC_200M_TM_DIV_NUM;
        *clock = MMC_FREQ_50M;
    } else if (cfg_clock >= MMC_FREQ_32M) {
        *div_num = MMC_32M_DIV_NUM;
        *tm_div_num = MMC_SRC_32M_TM_DIV_NUM;
        *clock = MMC_FREQ_32M;
    } else if (cfg_clock >= MMC_FREQ_16M) {
        *div_num = MMC_16M_DIV_NUM;
        *tm_div_num = MMC_SRC_32M_TM_DIV_NUM;
        *clock = MMC_FREQ_16M;
    } else if (cfg_clock >= MMC_FREQ_400K) {
        *div_num = MMC_400K_DIV_NUM;
        *tm_div_num = MMC_SRC_32M_TM_DIV_NUM;
        *clock = MMC_FREQ_400K;
    } else if (cfg_clock >= MMC_FREQ_100K) {
        *div_num = MMC_100K_DIV_NUM;
        *tm_div_num = MMC_SRC_32M_TM_DIV_NUM;
        *clock = MMC_FREQ_100K;
    } else {
        *div_num = MMC_32M_DIV_NUM;
        *tm_div_num = MMC_SRC_32M_TM_DIV_NUM;
        *clock = 0;
    }
}

unsigned int hi_mmc_set_clock_with_way1(struct hi_mmc_drv *mmc_drv, unsigned int cfg_clock)
{
    unsigned int div_num = 0;
    unsigned int tm_div_num = 0;
    unsigned int real_clock;
    unsigned int clk_div[MAX_MMC_NUM] = {EMMC_CCLK_DIV_REG, SD_HOST_CCLK_REG};
    unsigned int sdio_tm_div = SDIO_TM_DIV_REG;
    unsigned int sdio_emmc_reg;
    system_sdio_freq_t sdiom_clk = CLOCKS_SDIO_FREQ_32M;
    uint16_t value;
    non_os_enter_critical();
    hal_mmc_adapte_clock(cfg_clock, &div_num, &tm_div_num, &real_clock);
    if (cfg_clock > MMC_FREQ_32M) { sdiom_clk = CLOCKS_SDIO_FREQ_H; } // high freq is 193.3MHz in suxun chip.
    sdio_emmc_reg = clk_div[mmc_drv->id]; // 0--emmc, 1--sdio host
    value = readw(sdio_emmc_reg);
    value &= ~SDIO_EMMC_CCLK_EN;
    value &= ~SDIO_EMMC_ORI_DIV_EN;
    writew(sdio_emmc_reg, value);
    value = readw(sdio_tm_div);
    value &= ~SDIO_TM_DIV_EN;
    writew(sdio_tm_div, value);
    // Sets the eMMC source clock.
    clocks_set_sdio_freq(sdiom_clk);
    // Sets emmc or sdio host frequency divider number.
    hi_mmc_set_sdio_clk_in_cs(sdio_emmc_reg, (uint8_t)div_num);
    // Sets sdio host and emmc TMCLK(about 1MHz).
    value = readw(sdio_tm_div);
    value &= ~SDIO_TM_DIV_LOAD_EN;
    writew(sdio_tm_div, value);
    value &= ~SDIO_TM_DIV_MASK;
    value |= (uint16_t)(tm_div_num << SDIO_TM_DIV_NUM_POST_SHIFT);
    writew(sdio_tm_div, value);
    value |= SDIO_TM_DIV_LOAD_EN;
    writew(sdio_tm_div, value);
    value |= SDIO_TM_DIV_EN;
    writew(sdio_tm_div, value);
    non_os_exit_critical();
    PRINT("Clock cfg:%#x div:%#x %#x real_clock %d\n", cfg_clock, div_num, sdio_tm_div, real_clock);
    return real_clock;
}

#elif (USE_SDIOM_CLK_CONFIG_WAY == SDIOM_WITHOUT_SET_TM_AND_SECOND_FREQ_DIV)
static void hi_mmc_set_sd_bus_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;
    value = readw(div_reg);
    value &= ~SD_BUS_LOAD_DIV_EN;
    writew(div_reg, value);
    value &= ~SD_BUS_DIV_NUM_MASK;
    value |= (uint16_t)(freq_div << SD_BUS_DIV_NUM_POST_SHIFT);
    writew(div_reg, value);
    value |= SD_BUS_LOAD_DIV_EN;
    writew(div_reg, value);
}

static void hi_mmc_set_second_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;
    value = readw(div_reg);
    value &= ~SDIO_EMMC_CCLK_EN;
    value &= ~SDIO_EMMC_ORI_DIV_EN;
    writew(div_reg, value);
    if (div_reg == EMMC_DIV_REG) {
        value &= ~EMMC_DIV_NUM_MASK;
        value |= (uint16_t)(freq_div << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    } else {
        value &= ~SDIO_DIV_NUM_MASK;
        value |= (uint16_t)(freq_div << SDIO_DIV_NUM_POST_SHIFT);
    }
    writew(div_reg, value);
    value |= EMMC_CCLK_LOAD_DIV_EN;
    writew(div_reg, value);
    value |= EMMC_EMMC_CCLK_EN;
    writew(div_reg, value);
}

static void hi_mmc_set_first_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;
    value = readw(div_reg);
    value &= ~SDIO_EMMC_CCLK_EN;
    value &= ~SDIO_EMMC_ORI_DIV_EN;
    writew(div_reg, value);
    value &= ~SDIO_EMMC_DIV_NUM_MASK;
    value |= (uint16_t)(freq_div << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    writew(div_reg, value);
    value |= EMMC_CCLK_LOAD_DIV_EN;
    writew(div_reg, value);
    value |= EMMC_EMMC_CCLK_EN;
    writew(div_reg, value);
    value |= BIT(14); // 14
    writew(div_reg, value);
}

static void hi_mmc_set_sdiom_src_clk(uint32_t cfg_clock, uint32_t *freq_div, uint32_t *clk_level)
{
    system_sdio_freq_t sdiom_freq_level;
    if (cfg_clock <= SDIOM_SRC_CLK_LOW_FREQ_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_32M;
        *clk_level = SDIOM_SRC_CLK_LOW_FREQ_VAL;
        *freq_div = SDIOM_SRC_CLK_LOW_FREQ_VAL / cfg_clock;
        if (SDIOM_SRC_CLK_LOW_FREQ_VAL % cfg_clock != 0) { *freq_div += 1; }
    } else if (cfg_clock == SDIOM_SRC_CLK_FREQ_LEVEL_50M_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_50M;
        *clk_level = SDIOM_SRC_CLK_FREQ_LEVEL_50M_VAL;
        *freq_div = 1;
    } else if (cfg_clock == SDIOM_SRC_CLK_FREQ_LEVEL_64M_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_64M;
        *clk_level = SDIOM_SRC_CLK_FREQ_LEVEL_64M_VAL;
        *freq_div = 1;
    } else if (cfg_clock <= SDIOM_SRC_CLK_HIGH_FREQ_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_H;
        *clk_level = SDIOM_SRC_CLK_HIGH_FREQ_VAL;
        *freq_div = SDIOM_SRC_CLK_HIGH_FREQ_VAL / cfg_clock;
        if (SDIOM_SRC_CLK_HIGH_FREQ_VAL % cfg_clock != 0) { *freq_div += 1; }
    } else {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_32M;
        *clk_level = SDIOM_SRC_CLK_FREQ_LEVEL_64M_VAL;
        *freq_div = 1;
    }
}

unsigned int hi_mmc_set_clock_with_way2(struct hi_mmc_drv *mmc_drv, unsigned int cfg_clock)
{
    uint32_t clock_level;
    uint32_t first_freq_div = 0;
    uint32_t freq_div = 0;
    uint32_t second_freq_div = 0;
    uint32_t sec_freq_div = 1;  // Reserved for future expansion

    if (mmc_drv == NULL || mmc_drv->id >= MAX_MMC_NUM) {
        return 0;
    }

    hi_mmc_set_sdiom_src_clk(cfg_clock, &freq_div, &clock_level);

    if (mmc_drv->id == SDIO_EMMC_ID) {
        first_freq_div = EMMC_CCLK_DIV_REG;
        second_freq_div = EMMC_DIV_REG;
    } else {
        first_freq_div = SD_HOST_CCLK_REG;
        second_freq_div = SDIO_DIV_REG;
    }
    hi_mmc_set_first_level_clk_div(first_freq_div, freq_div);
    hi_mmc_set_second_level_clk_div(second_freq_div, sec_freq_div);
    return (clock_level / freq_div);
}

bool hi_mmc_set_clock_switch(uint32_t fisrt_div, uint32_t second_div, uint32_t sd_bus_div)
{
    hi_mmc_set_first_level_clk_div(EMMC_CCLK_DIV_REG, fisrt_div); // 198M 4div 49M
    hi_mmc_set_second_level_clk_div(EMMC_DIV_REG, second_div); // 1div 49M
    hi_mmc_set_sd_bus_clk_div(SDIO_DIV_REG, sd_bus_div); // sdbus 1div, 196M
    return true;
}
#endif

static unsigned int hal_mmc_set_soc_clock(struct hi_mmc_drv *mmc_drv, unsigned int cfg_clock)
{
    UNUSED(mmc_drv);
    UNUSED(cfg_clock);
#if (USE_SDIOM_CLK_CONFIG_WAY == SDIOM_WITH_SET_TM_AND_FIRST_FREQ_DIV)
    return hi_mmc_set_clock_with_way1(mmc_drv, cfg_clock);
#elif (USE_SDIOM_CLK_CONFIG_WAY == SDIOM_WITHOUT_SET_TM_AND_SECOND_FREQ_DIV)
    return hi_mmc_set_clock_with_way2(mmc_drv, cfg_clock);
#else
    return 0; // for the further extending
#endif
} /*lint +e550 +e574*/

void hal_mmc_set_card_clk(struct hi_mmc_drv *mmc_drv, bool en)
{
    uint16_t clk;
    non_os_enter_critical();
    clk = mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL);
    if (en) {
        clk |= MSHC_CLOCK_CARD_EN;
    } else {
        clk &= ~MSHC_CLOCK_CARD_EN;
    }
    mmc_writew(mmc_drv, clk, MSHC_CLOCK_CONTROL);
    non_os_exit_critical();
}

void hal_mmc_set_clock(struct hi_mmc_drv *mmc_drv, unsigned int clocknum)
{
    unsigned short clk;
    unsigned long timeout_val;
    unsigned int ret;

    non_os_enter_critical();
    // turn off clk2card_on
    clk = mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL);
    clk &= ~(MSHC_CLOCK_CARD_EN);
    mmc_writew(mmc_drv, clk, MSHC_CLOCK_CONTROL);

    // turn off card_clk_en
    clk &= ~(MSHC_CLOCK_INT_EN | MSHC_CLOCK_PLL_EN);
    mmc_writew(mmc_drv, clk, MSHC_CLOCK_CONTROL);

    if (clocknum == 0) {
        non_os_exit_critical();
        return;
    }
    if (clocknum >= mmc_drv->freq_max) {
        clocknum = mmc_drv->freq_max;
    }

    ret = hal_mmc_set_soc_clock(mmc_drv, clocknum);
    if (!ret) {
        LOG_E("Set clock fail!\n");
        non_os_exit_critical();
        return;
    }
    mmc_drv->clock = ret;
    // turn on card_clk_en
    clk |= MSHC_CLOCK_INT_EN | MSHC_CLOCK_PLL_EN;
    mmc_writew(mmc_drv, clk, MSHC_CLOCK_CONTROL);

    timeout_val = 20; // max wait clock stable 20ms
    while (timeout_val) {
        clk = mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL);
        if (clk & (unsigned short)MSHC_CLOCK_INT_STABLE) { break; }
        if (timeout_val == 1) {
            LOG_E("Internal clock never stabilized.\n");
            non_os_exit_critical();
            return;
        }
        timeout_val--;
        uapi_tcxo_delay_ms((uint64_t)1);
    }

    // turn on clk2card_on
    clk |= MSHC_CLOCK_CARD_EN;
    mmc_writew(mmc_drv, clk, MSHC_CLOCK_CONTROL);
    non_os_exit_critical();
}

void hal_mmc_clr_reg(struct hi_mmc_drv *mmc_drv, unsigned int mask)
{
    unsigned long timeout_val;

    mmc_writeb(mmc_drv, (uint8_t)mask, MSHC_SOFTWARE_RESET);
    if (mask & MSHC_RESET_ALL) {
        mmc_drv->clock = 0;
    }

    timeout_val = 100; // timeout 100ms

    /* hw clears the bit when it's done */
    while (mmc_readb(mmc_drv, MSHC_SOFTWARE_RESET) & mask) {
        if (timeout_val == 0) {
            LOG_E("host%d: Reset 0x%x never completed.\n",
                  mmc_drv->id, (int)mask);
            hi_mmc_dumpregs(mmc_drv);
            return;
        }
        timeout_val--;
        uapi_tcxo_delay_ms((uint64_t)1);
    }

    if (mask & MSHC_RESET_ALL) { mmc_drv->preset_enabled = false; }
}

void hal_mmc_deassert_status(struct hi_mmc_drv *mmc_drv)
{
    unsigned int value;
    non_os_enter_critical();
    pmu_sdio_glb_common_reset(true);
    if (mmc_drv->id == SDIO_EMMC_ID) {
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_RESET);
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_DERESET);
    } else {
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_RESET);
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_DERESET);
    }
    non_os_exit_critical();
    // open the clk of card:
    value = mmc_readl(mmc_drv, MSHC_CLK_CTRL);
    value |= MSHC_CLK_CTRL_INT_CLK;
    mmc_writel(mmc_drv, value, MSHC_CLK_CTRL);

    /* clock config with 400khz */
    (void)hal_mmc_set_clock(mmc_drv, MMC_FREQ_400K);
#if (SUPPORT_HI_EMMC_PHY == YES)
    if (mmc_drv->id == SDIO_EMMC_ID) {
        hi_mmc_set_phy_ioctl(mmc_drv); // config emmc PHY mode
        hi_mmc_set_phy_driver_capability(mmc_drv, mmc_drv->phy_driver_cap);
        hi_mmc_set_phy_invert_phase(mmc_drv);
        hi_mmc_set_phy_delay_by_measurement(mmc_drv);
    }
#endif
}

static void hal_mmc_set_uhs_signal(struct hi_mmc_drv *mmc_drv, enum mmc_bus_timing timing)
{
    unsigned short ctrl_2;

    ctrl_2 = mmc_readw(mmc_drv, MSHC_HOST_CONTROL2);

    /* Select Bus Speed Mode for host */
    ctrl_2 &= ~MSHC_CTRL_UHS_MASK;
    if ((timing == TIMING_UHS_SDR104) || (timing == TIMING_MMC_HS200)) {
        ctrl_2 |= MSHC_CTRL_UHS_SDR104;
    } else if (timing == TIMING_UHS_SDR12) {
        ctrl_2 |= MSHC_CTRL_UHS_SDR12;
    } else if ((timing == TIMING_UHS_SDR25) || (timing == TIMING_MMC_HS)) {
        ctrl_2 |= MSHC_CTRL_UHS_SDR25;
    } else if (timing == TIMING_UHS_SDR50) {
        ctrl_2 |= MSHC_CTRL_UHS_SDR50;
    } else if ((timing == TIMING_UHS_DDR50) || (timing == TIMING_UHS_DDR52)) {
        ctrl_2 |= MSHC_CTRL_UHS_DDR50;
    } else if (timing == TIMING_MMC_HS400) {
        ctrl_2 |= MSHC_CTRL_HS400;
    }
    ctrl_2 |= MSHC_CTRL_VDD_180;
    mmc_writew(mmc_drv, ctrl_2, MSHC_HOST_CONTROL2);
}

void hal_mmc_set_preset_val(struct hi_mmc_drv *mmc_drv, bool en)
{
    unsigned short ctrl_val = 0;
    if (mmc_drv->version < MSHC_SPEC_300) { return; }

    if (mmc_drv->preset_enabled != (uint32_t)en) {
        ctrl_val = mmc_readw(mmc_drv, MSHC_HOST_CONTROL2);
        if (en) {
            ctrl_val |= MSHC_CTRL_PRESET_VAL_ENABLE;
        } else {
            ctrl_val &= ~MSHC_CTRL_PRESET_VAL_ENABLE;
        }

        mmc_writew(mmc_drv, ctrl_val, MSHC_HOST_CONTROL2);
        mmc_drv->preset_enabled = en;
    }
}

void hal_mmc_set_timing(struct hi_mmc_drv *mmc_drv, int timing)
{
    unsigned short clockval = 0;
    unsigned short ctrl_2 = 0;
    uint8_t ctrl;

    ctrl = mmc_readb(mmc_drv, MSHC_HOST_CONTROL);

    if (timing >= TIMING_MMC_HS) {
        ctrl |= MSHC_CTRL_HISPD;
    }
    if (mmc_drv->version < MSHC_SPEC_300) {
        mmc_writeb(mmc_drv, ctrl, MSHC_HOST_CONTROL);
        return;
    }
    if (!mmc_drv->preset_enabled) {
        mmc_writeb(mmc_drv, ctrl, MSHC_HOST_CONTROL);
        ctrl_2 = mmc_readw(mmc_drv, MSHC_HOST_CONTROL2);
        ctrl_2 &= ~MSHC_CTRL_DRV_TYPE_MASK;

        mmc_writew(mmc_drv, ctrl_2, MSHC_HOST_CONTROL2);
    } else {
        clockval = mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL);
        clockval &= ~MSHC_CLOCK_CARD_EN;
        mmc_writew(mmc_drv, clockval, MSHC_CLOCK_CONTROL);

        mmc_writeb(mmc_drv, ctrl, MSHC_HOST_CONTROL);
        hal_mmc_set_clock(mmc_drv, mmc_drv->clock);
    }
    clockval = mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL);
    clockval &= ~MSHC_CLOCK_CARD_EN;
    mmc_writew(mmc_drv, clockval, MSHC_CLOCK_CONTROL);

    hal_mmc_set_uhs_signal(mmc_drv, (enum mmc_bus_timing)timing);
    mmc_drv->timing = (uint32_t)timing;
    if (mmc_drv->preset_enabled) {
        hal_mmc_set_preset_val(mmc_drv, true);
    }
    hal_mmc_set_clock(mmc_drv, mmc_drv->clock);
}

void hal_mmc_adma_config(struct hi_mmc_drv *mmc_drv)
{
    mmc_writel(mmc_drv, (uintptr_t)mmc_drv->adma_desc, MSHC_ADMA_ADDRESS_LOW);
}

void hal_mmc_set_sdiom_clk_mode(struct hi_mmc_drv** drv, int mode)
{
    int id = g_sdiom_clk_cfg[mode].id;
    uint32_t clock = g_sdiom_clk_cfg[mode].clock;
    uint32_t fields = g_sdiom_clk_cfg[mode].amend_div_reg;
    uint32_t div_num = g_sdiom_clk_cfg[mode].amend_div_num;
    non_os_enter_critical();
    hal_mmc_set_clock(drv[id], clock);
    hi_mmc_set_sdio_clk_in_cs(fields, (uint8_t)div_num);
    non_os_exit_critical();
}

void hal_mmc_set_emmc_ctrl(struct hi_mmc_drv *mmc_drv, uint32_t offset, uint32_t pos, uint32_t bits, uint32_t val)
{
    reg_setbits(mmc_drv->hw_base_addr, offset, pos, bits, val);
}

void hal_mmc_set_clk_en(hal_mmc_bus_num_t mmc_bus, bool en)
{
    if (mmc_bus >= MAX_MMC_NUM) { return; }

    uint32_t clk_reg[MAX_MMC_NUM] = { EMMC_CCLK_DIV_REG, SD_HOST_CCLK_REG };
    uint32_t reg_addr = clk_reg[mmc_bus];
    non_os_enter_critical();
    if (en) {
        reg16_setbit((void *)((uintptr_t)reg_addr), SDIO_EMMC_CCLK_EN_POS);
    } else {
        reg16_clrbit((void *)((uintptr_t)reg_addr), SDIO_EMMC_CCLK_EN_POS);
    }
    non_os_exit_critical();
}

void hi_mmc_dumpregs(struct hi_mmc_drv *mmc_drv)
{
    LOG_E(": =========== DUMP (host%d) REGISTER===========\n", mmc_drv->id);

    LOG_E(": Sys addr[00]: 0x%08x | Version[FE]:  0x%04x\n", mmc_readl(mmc_drv, MSHC_DMA_ADDRESS),
          mmc_readw(mmc_drv, MSHC_HOST_VERSION));
    LOG_E(": Blk size[04]: 0x%04x | Blk cnt[06]:  0x%04x\n", mmc_readw(mmc_drv, MSHC_BLOCK_SIZE),
          mmc_readw(mmc_drv, MSHC_BLOCK_COUNT));
    LOG_E(": Argument[08]: 0x%08x | Trn mode[0C]: 0x%04x\n", mmc_readl(mmc_drv, MSHC_ARGUMENT),
          mmc_readw(mmc_drv, MSHC_TRANSFER_MODE));
    LOG_E(": Present[24]:  0x%08x | Host ctl[28]: 0x%02x\n", mmc_readl(mmc_drv, MSHC_PRESENT_STATE),
          mmc_readb(mmc_drv, MSHC_HOST_CONTROL));
    LOG_E(": Power[29]:    0x%02x | Blk gap[2A]:  0x%02x\n", mmc_readb(mmc_drv, MSHC_POWER_CONTROL),
          mmc_readb(mmc_drv, MSHC_BLOCK_GAP_CONTROL));
    LOG_E(": Wake-up[2B]:  0x%02x | Clock[2C]:    0x%04x\n", mmc_readb(mmc_drv, MSHC_WAKE_UP_CONTROL),
          mmc_readw(mmc_drv, MSHC_CLOCK_CONTROL));
    LOG_E(": Timeout[2E]:  0x%08x | Int stat[30]: 0x%08x\n", mmc_readb(mmc_drv, MSHC_TIMEOUT_CONTROL),
          ((mmc_readw(mmc_drv, MSHC_ERR_INT_STATUS) << MSHC_ERR_INT_OFFSET) |
           mmc_readw(mmc_drv, MSHC_NOR_INT_STATUS)));
    LOG_E(": Int enab[34]: 0x%08x | Sig enab[38]: 0x%08x\n",
          ((mmc_readw(mmc_drv, MSHC_ERR_INT_ENABLE) << MSHC_ERR_INT_OFFSET) |
           mmc_readw(mmc_drv, MSHC_NOR_INT_ENABLE)),
          ((mmc_readw(mmc_drv, MSHC_ERR_SIGNAL_ENABLE) << MSHC_ERR_INT_OFFSET) |
           mmc_readw(mmc_drv, MSHC_NOR_SIGNAL_ENABLE)));
    LOG_E(": ACMD err[3C]: 0x%04x | Slot int[FC]: 0x%04x\n", mmc_readw(mmc_drv, MSHC_AUTO_CMD_ERR),
        mmc_readw(mmc_drv, MSHC_SLOT_INT_STATUS));
    LOG_E(": Caps_1[40]:   0x%08x | Caps_2][44]: 0x%08x\n", mmc_readl(mmc_drv, MSHC_CAPABILITIES_1),
        mmc_readl(mmc_drv, MSHC_CAPABILITIES_2));
    LOG_E(": Cmd[0E]:      0x%04x | Max curr[48]: 0x%08x | opcode = %d\n",
          mmc_readw(mmc_drv, MSHC_COMMAND), mmc_readl(mmc_drv, MSHC_MAX_CURRENT),
          (mmc_readw(mmc_drv, MSHC_COMMAND) >> 8) & 0x3f); // 8 0x3f for get opcode what is bit13--bit8
    LOG_E(": Resp:0x%08x   0x%08x   0x%08x   0x%08x\n",
          mmc_readl(mmc_drv, MSHC_RESPONSE), mmc_readl(mmc_drv, MSHC_RESPONSE + 0x4),
          mmc_readl(mmc_drv, MSHC_RESPONSE + 0x8), mmc_readl(mmc_drv, MSHC_RESPONSE + 0xC));
    LOG_E(": Host ctl2[3E]: 0x%04x\n", mmc_readw(mmc_drv, MSHC_HOST_CONTROL2));
    LOG_E(": Emmc ctrl[52C]: 0x%04x\n", mmc_readw(mmc_drv, MSHC_EMMC_CTRL));
    if (mmc_drv->flags & MMC_FLAG_USE_ADMA) {
        LOG_E(": ADMA Err[54]: 0x%02x | ADMA Ptr[58]: 0x%08x\n", mmc_readb(mmc_drv, MSHC_ADMA_ERROR),
              mmc_readl(mmc_drv, MSHC_ADMA_ADDRESS_LOW));
    }
    LOG_E(": ===========================================\n");
}
