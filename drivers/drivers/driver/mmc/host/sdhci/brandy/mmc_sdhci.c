/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: MMC of board
 * Author: @CompanyNameTag
 * Create: 2021-03-17
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT,ESTRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "soc_mmc.h"
#include "hci.h"
#include "sdhci.h"
#include "sdhci_private.h"
#include "hci_helper.h"
#include "mmc_adapt.h"
#include "chip_core_definition.h"
#include "common_def.h"
#include "clocks_switch.h"
#include "clocks_config_priv.h"
#include "hal_pmu_peripheral.h"
#include "debug_print.h"

#define CLOCKS_SDIO_FREQ_32M_VAL 32000000  // sdio:32M
#define CLOCKS_SDIO_FREQ_50M_VAL 50000000  // sdio:50M
#define CLOCKS_SDIO_FREQ_64M_VAL 64000000  // sdio:64M
#define CLOCKS_SDIO_FREQ_H_VAL   294912000 // sdio:294M
#define EMMC_SAVE_BUS_WIDTH_1BIT        0
#define EMMC_SAVE_BUS_WIDTH_4BIT        1
#define EMMC_SAVE_BUS_WIDTH_8BIT        2
#define EMMC_SAVE_BUS_TIMING_NORMAL     0
#define EMMC_SAVE_BUS_TIMING_MMC_HS     1
#define EMMC_SAVE_BUS_TIMING_MMC_HS200  3
#define EMMC_SAVE_BUS_TIMING_MMC_DDR52  4
#define EMMC_SAVE_BUS_TIMING_MMC_HS400  7
#define EMMC_CMD0_RESET_WAIT_TIME       900
#define PM_EMMC_CCLK_DIV_REG      0x52000100
#define PM_SD_HOST_CCLK_DIV_REG   0x52000110
#define RC_CLK_VALUE              49152000
#define XO_CLK_VALUE              32000000
#define XO_DLL2_CLK_VALUE         64000000
#define PLL_CLK_VALUE             884736000
#define PM_SDIOM_CLK_REG          0x55000628
#define PM_EMMC_DIV_REG           0x52000120

typedef struct {
    uint16_t sdiom_clk_reg;
    uint16_t emmc_cclk_reg;
    uint16_t emmc_div_reg;
    uint16_t sd_bus_reg;
    uint16_t mbiu_ctrl;
    uint32_t timing;
    uint32_t bus_width;
    uint32_t at_ctrl;
    uint32_t at_stat;
    uint16_t host_ctrl1;
    uint16_t host_ctrl2;
} mmc_save_info_t;

static mmc_save_info_t g_mmc_cur_info = { 0 };
bool g_mmc_sleep_inited = false;
bool g_mmc_inited = false;
static enum mmc_bus_timing g_max_timing = bus_timing_normal;

void mmc_set_first_init_flag(bool flag)
{
    g_mmc_inited = flag;
}

bool mmc_get_first_init_flag(void)
{
    return g_mmc_inited;
}

void mmc_set_sleep_init_flag(bool flag)
{
    g_mmc_sleep_inited = flag;
}

bool mmc_get_sleep_init_flag(void)
{
    return g_mmc_sleep_inited;
}

mmc_save_info_t* sdhci_get_save_info(void)
{
    return &g_mmc_cur_info;
}

void emmc_save_info(void)
{
    uint32_t tmp_div;
    if (!mmc_get_sleep_init_flag()) {
        return;
    }
    reg16_setbit(PM_EMMC_CCLK_DIV_REG, 0);
    g_mmc_cur_info.timing = regw_getbits(HOST_CTRL2, 0, 0x0, LEN_3BIT); // for speed mode
    g_mmc_cur_info.at_ctrl = readl(AT_CTRL);
    g_mmc_cur_info.at_stat = regw_getbits(AT_STAT, 0, 0x0, LEN_8BIT); // for phase
    if (reg8_getbit(HOST_CTRL1, EXT_DAT_XFER_BIT) == 1) {
        g_mmc_cur_info.bus_width = EMMC_SAVE_BUS_WIDTH_8BIT;
    } else {
        if (reg8_getbit(HOST_CTRL1, 0x1) == 1) {
            g_mmc_cur_info.bus_width = EMMC_SAVE_BUS_WIDTH_4BIT;
        } else {
            g_mmc_cur_info.bus_width = EMMC_SAVE_BUS_WIDTH_1BIT;
        }
    }
    g_mmc_cur_info.sdiom_clk_reg = readw(SDIOM_CR_REG);
    g_mmc_cur_info.emmc_cclk_reg = readw(EMMC_CCLK_DIV_REG);
    g_mmc_cur_info.emmc_div_reg = readw(EMMC_DIV_REG);
    g_mmc_cur_info.sd_bus_reg = readw(SDIO_BUS_CGDIV_REG);
    g_mmc_cur_info.mbiu_ctrl = readw(MBIIU_CTRL);
    g_mmc_cur_info.host_ctrl1 = readb(HOST_CTRL1);
    g_mmc_cur_info.host_ctrl2 = readw(HOST_CTRL2);

#if EMMC_BOARD_DEBUG_ON
    PRINT("emmc save sdiom_reg=%x, emmc_cclk_reg=%x, emmc_div_reg=%x, timing=%d, phase=%d, bus_width=%d\r\n",
          g_mmc_cur_info.sdiom_clk_reg, g_mmc_cur_info.emmc_cclk_reg, g_mmc_cur_info.emmc_div_reg,
          g_mmc_cur_info.timing, g_mmc_cur_info.at_stat, g_mmc_cur_info.bus_width);
#endif
    reg16_clrbit(PM_EMMC_CCLK_DIV_REG, 0);
    mmc_set_sleep_init_flag(false);
}

void emmc_send_cmd1(uint32_t args, uint16_t cmd, uint32_t *resp)
{
    uint16_t rd_data = 0;
    uint16_t err_data = 0;
    uint32_t cnt = 0;
    *resp = 0;

    while ((*resp & CMD1_RESP_READY_BIT) != CMD1_RESP_READY_BIT) {
        while ((readl(PSTATE_REG) & CMD_XFER_COMPLETE) != 0) { ; }
        writel(ARGUMENT, args); // argument
        writew(CMD, cmd); // cmd
        cnt = 0;
        rd_data = 0;
        while ((rd_data & 0x1) == 0 && cnt < 0xfffff) { // 0xfffff Prevents infinite loop suspension.
            rd_data = reg16_getbit(NORMAL_INT_STAT, 0); // cmd complete rsp
            err_data = readw(ERROR_INT_STAT);
            if (err_data != 0) {
                PRINT("cmd1 ERR INT=%x\r\n", err_data);
                writew(NORMAL_INT_STAT, ALL_NORMAL_ERR_INT);
                writew(ERROR_INT_STAT, ALL_NORMAL_ERR_INT);
                break;
            }
            if ((rd_data & 0x1) == 1) {
                reg16_setbit(NORMAL_INT_STAT, 0x0);
                *resp = readl(RESP01);
            }
            cnt++;
        }
        if (cnt == 0xfffff) { PRINT("CMD1 send no resp\r\n"); }
    }
}

void emmc_send_cmd6(uint32_t args, uint16_t cmd, uint32_t *resp)
{
    int rd_data = 0;
    uint32_t cnt = 0;
    *resp = 0;

    while ((rd_data & CMD_XFER_COMPLETE) != CMD_XFER_COMPLETE) {
        while ((readl(PSTATE_REG) & CMD_DATA_NOT_READY) != 0) { ; }
        writel(ARGUMENT, args); // argument
        writew(CMD, cmd);  // cmd
        rd_data = readw(NORMAL_INT_STAT); // cmd complete rsp
        cnt = 0;
        while (((rd_data & CMD_XFER_COMPLETE) != CMD_XFER_COMPLETE) && cnt < RETRY_CNT2) {
            rd_data = readw(NORMAL_INT_STAT); // cmd complete rsp
            if ((rd_data & ERR_INT_BIT) != 0) { PRINT("cmd6 ERR INT=%x\r\n", readw(ERROR_INT_STAT)); }
            cnt++;
        }
    }
    if ((rd_data & 0x1) == 0x1) { *resp = readl(RESP01); }
    writew(NORMAL_INT_STAT, rd_data);
    writew(NORMAL_INT_STAT, rd_data);
    writew(NORMAL_INT_STAT, rd_data);
    writew(NORMAL_INT_STAT, rd_data);
    if (*resp != 0x900 && *resp != 0x800) {
        PRINT("cmd6 resp err. CMD=%x, arg=%x, resp=%x\r\n", cmd, args, *resp);
    }
}

void emmc_send_cmd(uint32_t args, uint16_t cmd, uint8_t flag, uint32_t *resp)
{
    int rd_data = 0;
    uint32_t cnt = 0;
    *resp = 0;
    while ((readl(PSTATE_REG) & CMD_XFER_COMPLETE) != 0) { ; }
    writel(ARGUMENT, args); // argument
    writew(CMD, cmd);  // cmd
    if (flag == 0) { return; } // no response.

    while ((rd_data & 0x1) == 0 && cnt < RETRY_CNT2) {
        rd_data = readw(NORMAL_INT_STAT); // cmd complete rsp
        if ((rd_data & ERR_INT_BIT) != 0) { PRINT("cmd=%x ERR INT=%x\r\n", cmd, readw(ERROR_INT_STAT)); }
        cnt++;
    }

    if (cnt == RETRY_CNT2) { PRINT("cmd=%x ERR INT=%x\r\n", cmd, readw(ERROR_INT_STAT)); }
    reg16_setbit(NORMAL_INT_STAT, 0x0);
    *resp = readl(RESP01);
#if EMMC_BOARD_DEBUG_ON
    PRINT("CMD=%x, arg=%x, resp=%x\r\n", cmd, args, *resp);
#endif
}

void emmc_send_cmd_r2(uint32_t args, uint16_t cmd, uint32_t *resp)
{
    int rd_data = 0;
    uint32_t cnt = 0;
    while ((readl(PSTATE_REG) & CMD_DATA_NOT_READY) != 0) { ; }
    writel(ARGUMENT, args); // argument
    writew(CMD, cmd);  // cmd
    while ((rd_data & CMD_COMPLETE) == 0 && cnt < RETRY_CNT2) {
        while ((readl(PSTATE_REG) & CMD_COMPLETE) != 0) { ; }
        rd_data = reg16_getbit(NORMAL_INT_STAT, 0x0); // cmd complete rsp
        if ((rd_data & ERR_INT_BIT) != 0) { PRINT("cmd=%x ERR INT=%x\r\n", cmd, readw(ERROR_INT_STAT)); }
    }
    if (cnt == RETRY_CNT2) { PRINT("cmd=%x ERR INT=%x\r\n", cmd, readw(ERROR_INT_STAT)); }
    reg16_setbit(NORMAL_INT_STAT, 0x0); // clear cmd complete rsp
    resp[0] = readl(RESP01); // 0 for responese 39-08 bits of the Response Field
    resp[1] = readl(RESP23); // 1 for responese 71-40 bits of the Response Field
    resp[2] = readl(RESP45); // 2 for responese 103-72 bits of the Response Field
    resp[3] = readl(RESP67); // 3 for responese 135-104 bits of the Response Field
#if EMMC_BOARD_DEBUG_ON
    PRINT("CMD-r2=%x, arg=%x, resp:%x %x %x %x\r\n", cmd, args, resp[0], resp[1], resp[2], resp[3]); // 2,3 fro resp
#endif
}

void emmc_set_timing_and_buswidth(uint32_t bus_width, uint32_t timing)
{
    uint32_t width_args = 0;
    uint32_t timing_args = 0;
    uint32_t width_flag = 1;
    uint32_t timing_flag = 1;
    uint32_t resp = 0;
    if (bus_width == EMMC_SAVE_BUS_WIDTH_4BIT) { // 4bit width
        reg8_clrbit(HOST_CTRL1, EXT_DAT_XFER_BIT);
        reg8_setbit(HOST_CTRL1, 1);
        width_args = CMD6_4BIT_WIDTH_ARGS;
    } else if (bus_width == EMMC_SAVE_BUS_WIDTH_8BIT) { // 8bit width
        reg8_setbit(HOST_CTRL1, EXT_DAT_XFER_BIT);
        width_args = CMD6_8BIT_WIDTH_ARGS;
    } else { // 1BIT width
        reg8_clrbit(HOST_CTRL1, EXT_DAT_XFER_BIT);
        reg8_clrbit(HOST_CTRL1, 1);
        width_flag = 0;
        PRINT("1bit bus width in ds mode\r\n");
    }
    reg8_setbit(HOST_CTRL1, HIGH_SPEED_EN_BIT); // high speed enable
    reg16_setbit(HOST_CTRL2, SIGNALING_EN_BIT); // 1.8V Signaling Enable

    if (timing == EMMC_SAVE_BUS_TIMING_NORMAL) {
        regw_setbits(HOST_CTRL2, 0, UHS_MODE_SEL_BIT, UHS_MODE_SEL_LEN, 0x0); // SDR12/Legacy
        timing_flag = 0;
        reg8_clrbit(HOST_CTRL1, HIGH_SPEED_EN_BIT); // high speed disable
    } else if (timing == EMMC_SAVE_BUS_TIMING_MMC_HS) {
        regw_setbits(HOST_CTRL2, 0, UHS_MODE_SEL_BIT, UHS_MODE_SEL_LEN, HS_SDR); // SDR25/High Speed SDR
        timing_args = CMD6_TIMING_HS_ARGS;
    } else if (timing == EMMC_SAVE_BUS_TIMING_MMC_DDR52) {
        regw_setbits(HOST_CTRL2, 0, UHS_MODE_SEL_BIT, UHS_MODE_SEL_LEN, HS_DDR); // DDR50/High Speed DDR
        timing_args = CMD6_TIMING_HS_ARGS;
    } else if (timing == EMMC_SAVE_BUS_TIMING_MMC_HS200) {
        regw_setbits(HOST_CTRL2, 0, UHS_MODE_SEL_BIT, UHS_MODE_SEL_LEN, HS200); // SDR104/HS200
        timing_args = CMD6_TIMING_HS200_ARGS;
    } else if (timing == EMMC_SAVE_BUS_TIMING_MMC_HS400) {
        regw_setbits(HOST_CTRL2, 0, UHS_MODE_SEL_BIT, UHS_MODE_SEL_LEN, HS400); // UHS-II/HS400
        timing_args = CMD6_TIMING_HS400_ARGS;
    } else {
        PRINT("unknown timing=%x\r\n", timing);
    }

    timing_args |= 0x1;
    width_args |= 0x1;
    if (width_flag == 1) { emmc_send_cmd6(width_args, CMD6_INDEX, &resp); }
    if (timing_flag == 1) { emmc_send_cmd6(timing_args, CMD6_INDEX, &resp); }

#if EMMC_BOARD_DEBUG_ON
    PRINT("timing=%x width_args=%x tuning=%x \r\n", timing, width_args, g_mmc_cur_info.at_stat);
#endif
}

static void emmc_reg_init(void)
{
    writew(PERI_EMMC_CRG_RST_CTL, 0);
    writew(PERI_EMMC_CRG_RST_CTL, SOFT_RST_GLB_SDIO_DERESET);
    init_emmc_pinmux();

    // set emmc clk 400k
    writew(SDIOM_CR_REG, 0x3);
    writew(EMMC_CCLK_DIV_REG, 0x5507);
    writew(EMMC_DIV_REG, 0x13);
    reg16_setbit(CLK_CTRL, 0x0); // offset:2c bit0 internal clk en
    reg16_setbit(CLK_CTRL, 0x3); // offset:2c bit3 pll enable
    reg16_setbit(CLK_CTRL, 0x2); // offset:2c bit2 sd clk enable
    while (reg16_getbit(CLK_CTRL, 0x1) != 1) { ; } // wait clock is stability
    writeb(TOUT_CTRL, 0xe);

    writeb(PWR_CTRL, POWR_ON_1_8V); // pwr ctrl, 1.8v
    writew(NORMAL_INT_STAT_EN, NORMAL_ERR_INT_EN); // normal int enable
    writew(ERROR_INT_STAT_EN, NORMAL_ERR_INT_EN); // normal signal enable
    writew(NORMAL_INT_SIGNAL_EN, ALL_NORMAL_ERR_INT); // error int enable
    writew(ERROR_INT_SIGNAL_EN, ALL_NORMAL_ERR_INT); // error signal enable
    reg16_setbit(EMMC_CTRL, 0x0); // set emmc
}

uint32_t emmc_get_clock_value(void)
{
    uint8_t clk_flag = (readw(PM_SDIOM_CLK_REG) & 0xE) >> 0x1;
    uint32_t source_clk_val = XO_CLK_VALUE;
    uint16_t div1 = 1;
    uint16_t div2 = 1;
    uint16_t div3 = 1;
    if (clk_flag == 0x1) {
        source_clk_val = XO_CLK_VALUE;
    } else if (clk_flag == 0x2) {
        source_clk_val = XO_DLL2_CLK_VALUE;
    } else if (clk_flag == 0x3) {
        source_clk_val = PLL_CLK_VALUE;
    } else { // rc or no defined
        source_clk_val = RC_CLK_VALUE;
    }
    if (reg16_getbit(PM_SDIOM_CLK_REG, 0x4) == 1) {
        div1 = regw_getbits(PM_SDIOM_CLK_REG, 0, 0x5, 0x4);
    }
    if (reg16_getbit(PM_EMMC_CCLK_DIV_REG, 0x1) == 1) {
        div2 = regw_getbits(PM_EMMC_CCLK_DIV_REG, 0, 0x4, 0x8);
    }
    if (reg16_getbit(PM_EMMC_DIV_REG, 0x0) == 1) {
        div3 = regw_getbits(PM_EMMC_DIV_REG, 0, 0x4, 0x3);
    }
#if EMMC_BOARD_DEBUG_ON
    PRINT(" value =%lu, div1=%d div2=%d, div3=%d\r\n", source_clk_val, div1, div2, div3);
#endif
    return (source_clk_val / div1 / div2 / div3);
}
static void emmc_recovery_register(void)
{
    // set emmc clk 400k
    writew(SDIOM_CR_REG, 0x3);
    writew(EMMC_CCLK_DIV_REG, 0x5507);
    writew(EMMC_DIV_REG, 0x13);
    reg16_setbit(CLK_CTRL, 0x0); // offset:2c bit0 internal clk en
    reg16_setbit(CLK_CTRL, 0x3); // offset:2c bit3 pll enable
    reg16_setbit(CLK_CTRL, 0x2); // offset:2c bit2 sd clk enable
    while (reg16_getbit(CLK_CTRL, 0x1) != 1) { ; } // wait clock is stability
    writeb(TOUT_CTRL, 0xe);

    writeb(PWR_CTRL, POWR_ON_1_8V); // pwr ctrl, 1.8v
    writew(NORMAL_INT_STAT_EN, NORMAL_ERR_INT_EN); // normal int enable
    writew(ERROR_INT_STAT_EN, NORMAL_ERR_INT_EN); // normal signal enable
    writew(NORMAL_INT_SIGNAL_EN, ALL_NORMAL_ERR_INT); // error int enable
    writew(ERROR_INT_SIGNAL_EN, ALL_NORMAL_ERR_INT); // error signal enable
    reg16_setbit(EMMC_CTRL, 0x0); // set emmc
    writeb(HOST_CTRL1, g_mmc_cur_info.host_ctrl1);
    writew(HOST_CTRL2, g_mmc_cur_info.host_ctrl2);
    writew(SDIOM_CR_REG, g_mmc_cur_info.sdiom_clk_reg);
    writew(EMMC_CCLK_DIV_REG, g_mmc_cur_info.emmc_cclk_reg);
    writew(EMMC_DIV_REG, g_mmc_cur_info.emmc_div_reg);
    writew(SDIO_BUS_CGDIV_REG, g_mmc_cur_info.sd_bus_reg);
    writew(MBIIU_CTRL, g_mmc_cur_info.mbiu_ctrl);
    writew(EMMC_TUNING_BM0_REG, EMMC_TUNING_BM0_VAL);
    writew(EMMC_TUNING_BM1_REG, EMMC_TUNING_BM1_VAL);
    writel(AT_CTRL, g_mmc_cur_info.at_ctrl);
    reg16_setbits(AT_STAT, 0, LEN_8BIT, g_mmc_cur_info.at_stat);
    // The fixed clock is less than or equal to 52 MHz, and the tuning clock is not used.
    if (emmc_get_clock_value() <= SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
        reg16_clrbit(HOST_CTRL2, SAMPLE_CLK_SEL_BIT);
    }
}
void emmc_recovery_init(void)
{
    uint32_t irq_sts = osal_irq_lock();
    uint64_t time1 = uapi_tcxo_get_us();
    uint64_t time9 = 0;
#ifdef EMMC_RECOVERY_WITH_REENUME
    uint32_t args = 0;
    uint32_t resp = 0;
    uint32_t resp_r2[4] = { 0 };
    uint32_t flag = 1;
    mmc_save_info_t* ptr_mmc = sdhci_get_save_info();
    uint32_t timing = ptr_mmc->timing;
    uint32_t bus_width = ptr_mmc->bus_width;

    emmc_reg_init();
    tcxo_port_delay_us(1200); // wait 1200 cnt (1ms)
    emmc_send_cmd(0x0, CMD0_INDEX, 0, &resp); // cmd0
    tcxo_port_delay_us(1000); // wait 1000 cnt (900us) ok
    resp = 0;
    emmc_send_cmd1(CMD1_ARGS, CMD1_INDEX, &resp);
    emmc_send_cmd_r2(0x0, CMD2_INDEX, resp_r2); // cmd2
    emmc_send_cmd(CMD3_ARGS, CMD3_INDEX, 1, &resp); // cmd3
    emmc_send_cmd_r2(CMD9_ARGS, CMD9_INDEX, resp_r2); // cmd9
    emmc_send_cmd(CMD7_ARGS, CMD7_INDEX, 1, &resp); // cmd7
    emmc_set_timing_and_buswidth(bus_width, timing);
    // recovery for clock and phase
    writeb(HOST_CTRL1, g_mmc_cur_info.host_ctrl1);
    writew(HOST_CTRL2, g_mmc_cur_info.host_ctrl2);
    writew(SDIOM_CR_REG, g_mmc_cur_info.sdiom_clk_reg);
    writew(EMMC_CCLK_DIV_REG, g_mmc_cur_info.emmc_cclk_reg);
    writew(EMMC_DIV_REG, g_mmc_cur_info.emmc_div_reg);
    writew(SDIO_BUS_CGDIV_REG, g_mmc_cur_info.sd_bus_reg);
    writew(MBIIU_CTRL, g_mmc_cur_info.mbiu_ctrl);
    writew(EMMC_TUNING_BM0_REG, EMMC_TUNING_BM0_VAL);
    writew(EMMC_TUNING_BM1_REG, EMMC_TUNING_BM1_VAL);
    writel(AT_CTRL, g_mmc_cur_info.at_ctrl);
    reg16_setbits(AT_STAT, 0, LEN_8BIT, g_mmc_cur_info.at_stat);
    // The fixed clock is less than or equal to 52 MHz, and the tuning clock is not used.
    if (emmc_get_clock_value() <= SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
        reg16_clrbit(HOST_CTRL2, SAMPLE_CLK_SEL_BIT);
    }
#else
    emmc_recovery_register();
#endif
    time9 = uapi_tcxo_get_us();
    osal_irq_restore(irq_sts);
#if EMMC_BOARD_DEBUG_ON
    PRINT("OK~  time1=%llu tim9=%llu, time=%llu\r\n", time1, time9, time9-time1);
#endif
    reg16_clrbit(PM_EMMC_CCLK_DIV_REG, 0);
}

static void hal_mmc_set_preset_val(struct hci_softc *sc, bool en)
{
    unsigned short ctrl_val = 0;

    ctrl_val = hci_readw(sc, SDHCI_HOST_CONTROL2);
    if (en) {
        ctrl_val |= SDHCI_CTRL2_PRESET_VALUE;
    } else {
        ctrl_val &= ~SDHCI_CTRL2_PRESET_VALUE;
    }

    hci_writew(sc, SDHCI_HOST_CONTROL2, ctrl_val);
}

static int mmc_voltage_switch(struct hci_softc *sc, uint32_t vccq)
{
    unsigned int pwr = SDHCI_POWER_180;

    switch (vccq) {
        case vccq_180:
            pwr = SDHCI_POWER_180;
            break;
        case vccq_330:
            pwr = SDHCI_POWER_330;
            break;
        default:
            device_printf(sc->dev, "error voltage:[%u].\n", vccq);
            break;
    }

#if (EMMC_BOARD_DEBUG_ON)
    device_printf(sc->dev, "vccq:[%u], pwr:[%u].\n", vccq, pwr);
#endif

    hal_mmc_set_preset_val(sc, false);

    hci_writeb(sc, SDHCI_POWER_CONTROL, 0);
    hci_writeb(sc, SDHCI_POWER_CONTROL, (uint8_t)pwr);
    pwr |= SDHCI_POWER_ON;
    hci_writeb(sc, SDHCI_POWER_CONTROL, (uint8_t)pwr);
    dpal_udelay(MSHC_WAIT_INT_PER_MSEC);

    return 0;
}

static void mmc_set_sd_bus_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;
    value = DPAL_GET_UINT16(div_reg);
    value &= ~SD_BUS_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value &= ~SD_BUS_DIV_NUM_MASK;
    value |= (uint16_t)(freq_div << SD_BUS_DIV_NUM_POST_SHIFT);
    DPAL_WRITE_UINT16(value, div_reg);
    value |= SD_BUS_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
}

static void mmc_set_sdiom_src_clk(uint32_t cfg_clock, uint32_t *freq_div, uint32_t *second_freq_div)
{
    system_sdio_freq_t sdiom_freq_level;
    *second_freq_div = 1;

    if (cfg_clock <= SDIOM_SRC_CLK_LOW_FREQ_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_32M;
        *freq_div = SDIOM_SRC_CLK_LOW_FREQ_VAL / cfg_clock;
        if (SDIOM_SRC_CLK_LOW_FREQ_VAL % cfg_clock != 0) { *freq_div += 1; }
    } else if (cfg_clock <= SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_50M;
        *freq_div = 1;
    } else if (cfg_clock == SDIOM_SRC_CLK_FREQ_LVL_64M_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_64M;
        *freq_div = 1;
    } else if (cfg_clock <= SDIOM_SRC_CLK_HIGH_FREQ_LVL_VAL) {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_H;
        *freq_div = SDIOM_SRC_CLK_HIGH_FREQ_LVL_VAL / cfg_clock;
        if (SDIOM_SRC_CLK_HIGH_FREQ_LVL_VAL % cfg_clock != 0) { *freq_div += 1; }
    } else {
        sdiom_freq_level = CLOCKS_SDIO_FREQ_32M;
        *freq_div = 1;
    }
    if (!clocks_set_sdio_freq(sdiom_freq_level)) {
#if (EMMC_BOARD_DEBUG_ON)
        dprintf("%s, sdiom_freq_level:[%u] error\r\n", __FUNCTION__, sdiom_freq_level);
#endif
    }

#if (EMMC_BOARD_DEBUG_ON)
    if (*freq_div > 0xFF) {
        dprintf("%s, div:[%u] error\r\n", __FUNCTION__, *freq_div);
    }
    dprintf("clk_level:[%u], freq_div:[%u], second_freq_div:[%u], cfg_clock: [%u]!\n",
        sdiom_freq_level, *freq_div, *second_freq_div, cfg_clock);
#endif
}

static void mmc_set_first_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;

    value = DPAL_GET_UINT16(div_reg);
    value &= ~SDIO_EMMC_CCLK_EN;
    value &= ~SDIO_EMMC_ORI_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);

    value &= ~SDIO_EMMC_DIV_NUM_MASK;
    value |= (uint16_t)(freq_div << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    value |= EMMC_CARD_CLK_INV_SET;
#if (EMMC_BOARD_DEBUG_ON)
    dprintf("%s, freq_div:[%u]  value:[%x]!\n", __FUNCTION__, freq_div, value);
#endif
    // 当0x52000100的bit13设置emmc cclk tuning coarse mode后，soc要求其分频系数必须大于等于2
    if ((freq_div == 1) && ((value & EMMC_TUNING_COARSE_MODE) == EMMC_TUNING_COARSE_MODE)) {
        value &= (~EMMC_CCLK_ORI_DIV_NUM_MASK);
        value |= EMMC_CCLK_ORI_DIV_NUM_2;
    }
    DPAL_WRITE_UINT16(value, div_reg);

    value |= EMMC_CCLK_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value |= EMMC_EMMC_CCLK_EN;
    DPAL_WRITE_UINT16(value, div_reg);
}

static void mmc_set_second_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;

    value = DPAL_GET_UINT16(div_reg);
    value &= ~SDIO_EMMC_CCLK_EN;
    value &= ~SDIO_EMMC_ORI_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    if (div_reg == EMMC_DIV_REG) {
        value &= ~EMMC_DIV_NUM_MASK;
        value |= (uint16_t)(freq_div << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    } else {
        value &= ~SDIO_DIV_NUM_MASK;
        value |= (uint16_t)(freq_div << SDIO_DIV_NUM_POST_SHIFT);
    }
    DPAL_WRITE_UINT16(value, div_reg);
    value |= EMMC_CCLK_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value |= EMMC_EMMC_CCLK_EN;
    DPAL_WRITE_UINT16(value, div_reg);
}

static void mmc_host_set_first_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;
    value = DPAL_GET_UINT16(div_reg);
    value &= ~SDIO_HOST_CLKEN;
    value &= ~SDIO_CCLK_ORI_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value &= ~SDIO_CCLK_ORI_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value &= ~SDIO_EMMC_DIV_NUM_MASK;
    value |= (uint16_t)(freq_div << SDIO_EMMC_DIV_NUM_POST_SHIFT);
    DPAL_WRITE_UINT16(value, div_reg);
    value |= SDIO_CCLK_ORI_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value |= SDIO_CCLK_ORI_LOAD_DIV_EN;
    DPAL_WRITE_UINT16(value, div_reg);
    value |= SDIO_HOST_CLKEN;
    DPAL_WRITE_UINT16(value, div_reg);

    value = DPAL_GET_UINT16(div_reg);
}

static void mmc_host_set_second_level_clk_div(uint32_t div_reg, uint32_t freq_div)
{
    uint16_t value;

    value = DPAL_GET_UINT16(div_reg);
    value &= ~(SDIO_HOST_DIV_EN | SDIO_HOST_DIV_MASK);
    DPAL_WRITE_UINT16(value, div_reg);

    value |= SDIO_HOST_DIV_EN;
    value |= (uint16_t)(freq_div << SDIO_HOST_DIV_SHFIT);
    DPAL_WRITE_UINT16(value, div_reg);
}

unsigned int mmc_set_clock_with_way2(struct hci_softc *sc, unsigned int cfg_clock)
{
    uint32_t first_freq_div = 0;
    uint32_t freq_div = 0;
    uint32_t second_freq_div = 0;
    uint32_t sec_freq_div = 1;
    uint32_t real_clock = cfg_clock;
    if (cfg_clock > sc->caps.max_clk) {
#if EMMC_BOARD_DEBUG_ON
        dprintf("%s, clock:[%u] is greater than the max_clk:[%u]!\n", __FUNCTION__, cfg_clock, sc->caps.max_clk);
#endif
        real_clock = sc->caps.max_clk;
    }
    mmc_set_sdiom_src_clk(real_clock, &freq_div, &sec_freq_div);
    if (sc->num == 0) {
        first_freq_div = EMMC_CCLK_DIV_REG;
        second_freq_div = EMMC_DIV_REG;
        mmc_set_first_level_clk_div(first_freq_div, freq_div);
        mmc_set_second_level_clk_div(second_freq_div, sec_freq_div);
#ifdef DPALCFG_DRIVERS_SDIO
    } else if (sc->num == 1) {
        first_freq_div = SD_HOST_CCLK_DIV_REG;
        second_freq_div = SDIO_HOST_CCLK_DIV_REG;
        mmc_host_set_first_level_clk_div(first_freq_div, freq_div);
        mmc_host_set_second_level_clk_div(second_freq_div, sec_freq_div);
#endif
    }

    mmc_set_sd_bus_clk_div(SDIO_BUS_CGDIV_REG, 1);

    return 0;
}

static unsigned int hal_mmc_set_soc_clock(struct hci_softc *sc, unsigned int cfg_clock)
{
    return mmc_set_clock_with_way2(sc, cfg_clock);
}

static void sdhci_use_pll_clk(void)
{
    uint16_t val;
    uint16_t div = 3;

    // disable sdiom clken
    DPAL_WRITE_UINT16(GLB_CLKEN_DISABLE, GLB_CLKEN_REG);
    // set clk_sdiom=clk_pll/3
    val = SDIOM_CR_CTRL_CH_ENABLE | SDIOM_CR_CTRL_CLK_RESOURCE_PLL_EN | SDIOM_CR_CTRL_DIV_EN;
    val |= div << SDIOM_CR_CTRL_DIV_OFFSET;
    DPAL_WRITE_UINT16(val, SDIOM_CR_REG);
    // enable sdiom clken
    DPAL_WRITE_UINT16(GLB_CLKEN_ENABLE, GLB_CLKEN_REG);
}

static uint32_t sdhci_get_config_clock(struct hci_softc *sc, uint32_t clock)
{
    uint32_t actual_clock = clock;
    if (clock > sc->caps.max_clk) {
#if EMMC_BOARD_DEBUG_ON
        dprintf("%s, clock:[%u] is greater than the max_clk:[%u]!\n", __FUNCTION__, clock, sc->caps.max_clk);
#endif
        actual_clock = sc->caps.max_clk;
    }
    switch (g_max_timing) {
        case bus_timing_mmc_hs:
        case bus_timing_uhs_sdr50:
        case bus_timing_uhs_ddr50:
        case bus_timing_mmc_ddr52:
        case bus_timing_sd_hs:
            if (actual_clock >= SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
                actual_clock = SDIOM_SRC_CLK_FREQ_LVL_49M_VAL;
            }
            break;
        default:
            break;
    }
    return actual_clock;
}

uint32_t sdhci_set_clock(struct hci_softc *sc, uint32_t clock)
{
    unsigned int ret;
    static uint8_t inited = false;
    uint32_t actual_clk = 0;
    if (clock == 0) {
        device_printf(sc->dev, "set clock invalid param!\n");
        return clock;
    }
    actual_clk = sdhci_get_config_clock(sc, clock);
#if EMMC_BOARD_DEBUG_ON
    dprintf("%s, clk:%u,a_clk:%u,resource:[0x%x]!\n", __FUNCTION__, clock, actual_clk, DPAL_GET_UINT16(SDIOM_CR_REG));
#endif
    if (actual_clk <= SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
        reg16_clrbit(BRANDY_EXTERNAL_EMMC_CCLK_DIV, EMMC_TUNING_COARSE_MODE_BIT);
    } else {
        reg16_setbit(BRANDY_EXTERNAL_EMMC_CCLK_DIV, EMMC_TUNING_COARSE_MODE_BIT);
    }
    ret = hal_mmc_set_soc_clock(sc, actual_clk);
    if (ret != 0) {
        device_printf(sc->dev, "set clock fail!\n");
        return 0;
    }

    if ((sc->num == 0) && inited == 0) {
        uint8_t val = hci_readb(sc, SDHCI_AXI_MBIIU_CTRL);
        val = SDHCI_AXI_MBIIU_BURST_INCR16_EN;
        hci_writeb(sc, SDHCI_AXI_MBIIU_CTRL, val);
        inited = true;
    }
#ifdef CFG_SDIO_EMMC_COEXIST
    if (sc->num == 0) {
#endif
    // After the tuning phase value is configured, the tuning clk function is automatically enabled.
    if (actual_clk > SDIOM_SRC_CLK_FREQ_LVL_50M_VAL) {
        reg16_clrbit(HOST_CTRL2, SAMPLE_CLK_SEL_BIT);
        reg16_clrbit(AT_CTRL, 0x4); // 0x4 means SW_TUNE_EN
        dpal_writel(sc->tuning_phase, AT_STAT);
        reg16_setbit(HOST_CTRL2, SAMPLE_CLK_SEL_BIT);
        // The fixed clock is less than or equal to 52 MHz, and the tuning clock is not used.
    } else {
        reg16_clrbit(HOST_CTRL2, SAMPLE_CLK_SEL_BIT);
    }
#ifdef CFG_SDIO_EMMC_COEXIST
    }
#endif
#if EMMC_BOARD_DEBUG_ON
    dprintf("%s,clk_src=0x%X, at_ctrl=0x%X, at_stat=0x%X, host2_ctrl=0x%X, tuning_phase=0x%X\r\n", __FUNCTION__,
            readw(SDIOM_CR_REG), readw(AT_CTRL), readw(AT_STAT), readw(HOST_CTRL2), sc->tuning_phase);
#endif
    return actual_clk;
}

void mmc_set_internal_clk(uint16_t clk_level)
{
    struct hci_softc *sc = get_mmc_resource(0);
    uint32_t g_sdiom_val[CLOCKS_SDIO_FREQ_MAX] = {
        CLOCKS_SDIO_FREQ_32M_VAL, CLOCKS_SDIO_FREQ_50M_VAL, CLOCKS_SDIO_FREQ_64M_VAL, CLOCKS_SDIO_FREQ_H_VAL };
    clocks_clk_cfg_t sdio_freq = clocks_system_sdio_freq_get(clk_level);
    sdhci_set_clock(sc, g_sdiom_val[clk_level]);
}

static void sdhci_set_drv_phase(uint32_t host_id, uint32_t phase)
{
    unused(host_id);
    unused(phase);
}

static void sdhci_card_clk_ctrl(const struct hci_softc *sc, int action)
{
    uint32_t val;

    val = hci_readw(sc, SDHCI_CLOCK_CONTROL);
    if (action == 0) {
        val &= ~SDHCI_CLOCK_CARD_EN;
    } else {
        val |= SDHCI_CLOCK_CARD_EN;
    }
    hci_writew(sc, SDHCI_CLOCK_CONTROL, val);
}

static void sdhci_enhanced_strobe(const struct hci_softc *sc, uint32_t en)
{
    uint32_t value;

    value = hci_readw(sc, SDHCI_EMMC_CTRL);

    if (en != 0) {
        value |= SDHCI_EMMC_CTRL_ENH_STROBE_EN;
    } else {
        value &= ~(SDHCI_EMMC_CTRL_ENH_STROBE_EN);
    }
    hci_writew(sc, SDHCI_EMMC_CTRL, value);
}

static void sdhci_subplatform_init(const struct hci_softc *sc, hci_caps *cfg_info)
{
    device_printf(sc->dev, "LOSCFG_PLATFORM selected\n");
    if (sc->num == 0) { /* mmc0: eMMC or SD card */
        cfg_info->max_clk = SDIOM_SRC_CLK_FREQ_LVL_98M_VAL;
        cfg_info->min_clk = CONFIG_MMC0_CCLK_MIN;
        sdhci_enhanced_strobe(sc, false);
        /* overwrite the SDHCI standard because of the platform quirk */
        device_printf(sc->dev, "LOSCFG_DRIVERS_EMMC selected\n");
        cfg_info->caps |= HCI_SLOTTYPE_EMBEDDED;
        cfg_info->caps |= HCI_CAN_DO_8BITBUS;
        cfg_info->quirks |= SDHCI_QUIRK_SIG_1V8_ONLY; /* signal 1.2V unsupported */
#ifdef DPALCFG_DRIVERS_SDIO
    } else if (sc->num == 1) { /* SDIO device */
         // brandy最高频率有限制
        cfg_info->max_clk = SDIOM_SRC_CLK_FREQ_LVL_49M_VAL;
        cfg_info->min_clk = CONFIG_MMC1_CCLK_MIN;
        cfg_info->caps |= HCI_SLOTTYPE_SDIO;
        cfg_info->quirks |= SDHCI_QUIRK_SIG_3V3_ONLY; /* signal 1.2V 1.8V unsupported */
        cfg_info->quirks |= SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;
        cfg_info->caps &= ~HCI_CAN_VDD_180;
        cfg_info->caps &= ~HCI_CAN_DO_8BITBUS;
#endif
    }
}

static void sdhci_platform_init(struct hci_softc *sc)
{
    hci_caps *cfg_info = &sc->caps;

    cfg_info->quirks |= SDHCI_QUIRK_WRITE_PROTECT;
    cfg_info->quirks |= SDHCI_QUIRK_MISSING_CAPS;

    cfg_info->caps = hci_readl(sc, SDHCI_CAPABILITIES);
    cfg_info->caps &= ~SDHCI_SLOTTYPE_MASK;
    cfg_info->caps &= ~HCI_CAN_DO_8BITBUS;
    cfg_info->caps &= ~SDHCI_CAN_DO_ADMA2_64BIT;

    cfg_info->caps2 = hci_readl(sc, SDHCI_CAPABILITIES2);
    cfg_info->caps2 &= ~HCI_CAN_MMC_HS400;
    cfg_info->caps2 &= ~HCI_CAN_MMC_HS400ES;

    sdhci_subplatform_init(sc, cfg_info);
}


static void sdhci_set_emmc_ctrl(const struct hci_softc *sc)
{
    uint32_t value;

    value = hci_readw(sc, SDHCI_EMMC_CTRL);
    value |= SDHCI_EMMC_CTRL_EMMC;
    hci_writew(sc, SDHCI_EMMC_CTRL, value);
}

static void sdhci_extra_init(struct hci_softc *sc)
{
    uint32_t val;

    val = hci_readb(sc, SDHCI_MSHC_CTRL);
    val &= ~SDHCI_CMD_CONFLIT_CHECK;
    hci_writeb(sc, SDHCI_MSHC_CTRL, val);

    if (sc->num == 0) {
        sdhci_set_emmc_ctrl(sc);
    }
}

static void sdhci_slot_init(struct hci_softc *sc)
{
    static uint8_t inited = false;
    uint32_t val;

    dprintf("%s enter, sc_num:[%u]\r\n", __FUNCTION__, sc->num);

    if (inited == 0) {
        pmu_sdio_glb_common_reset(true);
        inited = true;
    }

    if (sc->num == 0) {
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_RESET);
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_EMMC, HAL_PMU_PERIP_REQUEST_DERESET);
#ifdef DPALCFG_DRIVERS_SDIO
    } else if (sc->num == 1) {
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_RESET);
        hal_pmu_mcpu_perip_reset(HAL_PMU_MCPU_PERIP_SDIO_HOST, HAL_PMU_PERIP_REQUEST_DERESET);
#endif
    }

    val = DPAL_GET_UINT16(COM_GP_REG0_ADDR);
    if (val != EMMC_SDIO_RAM_CONTROL_MASK) {
        val = EMMC_SDIO_RAM_CONTROL_MASK;
        DPAL_WRITE_UINT16((uint16_t)val, COM_GP_REG0_ADDR);
    }

    sdhci_extra_init(sc);
    sdhci_platform_init(sc);
}

static void sdhci_en_drv_dll_sample(struct hci_softc *sc, uint32_t en)
{
    uint16_t val = 0;

#if EMMC_BOARD_DEBUG_ON
    dprintf("%s, en:[%u]\r\n", __FUNCTION__, en);
#endif

    if (en != 0) {
        hci_setl(sc, SDHCI_AUTO_TUNING_CTRL, SDHCI_SW_TUNING_EN);

        val = DPAL_GET_UINT16(EMMC_CCLK_DIV_REG);
        val |= EMMC_TUNING_COARSE_MODE;
        DPAL_WRITE_UINT16(val, EMMC_CCLK_DIV_REG);

        DPAL_WRITE_UINT16(EMMC_TUNING_BM0_VAL, EMMC_TUNING_BM0_REG);
        DPAL_WRITE_UINT16(EMMC_TUNING_BM1_VAL, EMMC_TUNING_BM1_REG);
    } else {
        hci_clearl(sc, SDHCI_AUTO_TUNING_CTRL, SDHCI_SW_TUNING_EN);
    }
}

static void sdhci_set_sample_phase(struct hci_softc *sc, unsigned int phase)
{
    uint32_t fields;

    fields = hci_readl(sc, SDHCI_AUTO_TUNING_STATUS);
    fields &= ~SDHCI_CENTER_PH_CODE_MASK;
    fields |= (uint32_t)phase;
    hci_writel(sc, SDHCI_AUTO_TUNING_STATUS, fields);
}

static void sdhci_set_mmc_io_driver(const struct hci_softc *sc, uint32_t host_id, enum mmc_bus_timing timing)
{
    unused(sc);
    unused(host_id);
    unused(timing);
}

static bool sdhci_judge_dll_sample(uint32_t timing)
{
    if (g_max_timing < timing) { g_max_timing = timing; }
    if (timing == bus_timing_mmc_hs || timing == bus_timing_sd_hs || timing == bus_timing_normal ||
        timing == bus_timing_uhs_sdr25) {
        return false;
    } else {
        return true;
    }
}

static void sdhci_set_phase(struct hci_softc *sc, uint32_t timing)
{
    uint32_t drv_phase, phase;
    bool en_flag = true;

    if (timing == 0) { return; }
    if (sc->num == 0) {
        /* eMMC and SD card */
        if (timing == bus_timing_mmc_hs400es) {
            drv_phase = DRV_PHASE_112P5_DEGREE;
            phase = sc->tuning_phase;
            sdhci_enhanced_strobe(sc, true);
        } else {
            sdhci_enhanced_strobe(sc, false);
            if (timing == bus_timing_mmc_hs400) {
                drv_phase = DRV_PHASE_112P5_DEGREE;
                phase = sc->tuning_phase;
            } else if (timing == bus_timing_mmc_hs200) {
                drv_phase = DRV_PHASE_258P75_DEGREE;
                phase = sc->tuning_phase;
            } else if (timing == bus_timing_mmc_hs) {
                drv_phase = DRV_PHASE_180_DEGREE;
                phase = SAMPLE_PHASE;
            } else if (timing == bus_timing_sd_hs) {
                drv_phase = DRV_PHASE_225_DEGREE;
                phase = SAMPLE_PHASE;
            } else if (timing == bus_timing_normal) {
                drv_phase = DRV_PHASE_180_DEGREE;
                phase = 0;
            } else {
                drv_phase = DRV_PHASE_225_DEGREE;
                phase = SAMPLE_PHASE;
            }
        }
    } else {
        /* SDIO device */
        if ((timing == bus_timing_sd_hs) || (timing == bus_timing_uhs_sdr25)) {
            drv_phase = DRV_PHASE_180_DEGREE;
            phase = SAMPLE_PHASE;
        } else {
            /* UHS_SDR12 */
            drv_phase = DRV_PHASE_180_DEGREE;
            phase = 0;
        }
    }
    en_flag = sdhci_judge_dll_sample(timing);
#if EMMC_BOARD_DEBUG_ON
    device_printf(sc->dev, "timing:%u, g_max_timing =%u, phase:0x%x, drv_phase:0x%x, en_flag:%d \n", timing,
                  g_max_timing, phase, drv_phase, en_flag);
#endif
    sdhci_set_drv_phase(sc->num, drv_phase);
    sdhci_en_drv_dll_sample(sc, en_flag);
    sdhci_set_sample_phase(sc, phase);
    sdhci_set_mmc_io_driver(sc, sc->num, timing);
}

void sdhci_pre_set_clock(struct hci_softc *sc)
{
    unsigned int value;

    // open the clk of card:
    value = hci_readw(sc, SDHCI_CLOCK_CONTROL);
    value |= (SDHCI_CLOCK_INT_EN | SDHCI_CLOCK_PLL_EN | SDHCI_CLOCK_CARD_EN);
    hci_writew(sc, SDHCI_CLOCK_CONTROL, value);
}

void sdhci_post_clock(struct hci_softc *sc)
{
    unused(sc);
}

static void sdhci_select_sample_phase(struct hci_softc *sc, unsigned int phase)
{
    sdhci_card_clk_ctrl(sc, 0);
    sdhci_set_sample_phase(sc, phase);
    sdhci_card_clk_ctrl(sc, 1);
    dpal_udelay(1); /* delay 1 us */
}

void mmc_dump_regs(struct hci_softc *sc)
{
}

void mmc_ext_clk_switch(struct hci_softc *sc, bool enable)
{
    unused(sc);
    if (enable) {
        reg16_setbit(BRANDY_EXTERNAL_EMMC_CCLK_DIV, 0);
    } else {
        reg16_clrbit(BRANDY_EXTERNAL_EMMC_CCLK_DIV, 0);
    }
}

void init_slot_methods(int slot, hci_slot_ops *slot_ops)
{
    unused(slot);

    slot_ops->pre_clock = sdhci_pre_set_clock;
    slot_ops->post_clock = sdhci_post_clock;
    slot_ops->set_clock = sdhci_set_clock;
    slot_ops->extra_init = sdhci_extra_init;
    slot_ops->slot_init = sdhci_slot_init;
    slot_ops->set_phase = sdhci_set_phase;
    slot_ops->enable_drv_sample = sdhci_en_drv_dll_sample;
    slot_ops->select_sample_phase = sdhci_select_sample_phase;
    slot_ops->switch_volt = mmc_voltage_switch;
    slot_ops->dump_regs = mmc_dump_regs;
    slot_ops->drv_ext_clk_switch = mmc_ext_clk_switch;
}

