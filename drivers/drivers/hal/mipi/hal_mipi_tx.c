/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: mipi tx hal file.
 * Author: @CompanyNameTag
 * Create: 2020-04-14
 */

#include "hal_mipi_tx.h"
#include "securec.h"
#include "tcxo.h"
#include "soc_osal.h"
#include "soc_errno.h"
#include "chip_io.h"
#include "mipi_tx.h"
#include "mipi_tx_reg.h"
#include "mipi_tx_def.h"
#include "chip_core_irq.h"
#ifdef PRE_FPGA
#include "i2c.h"
#include "platform_core.h"
#endif
#include "hal_i2c.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HAL_MIPI_READ_INDEX0 0
#define HAL_MIPI_READ_INDEX1 1
#define HAL_MIPI_READ_INDEX2 2
#define HAL_MIPI_READ_INDEX3 3
#define HAL_MIPI_HFP_OFFSET 16
#define HAL_MIPI_HBP_OFFSET 16

volatile mipi_tx_regs_type_t *g_mipi_tx_regs_va = NULL;
static uint32_t g_mipi_tx_irq_num = NUM_INTERRUPT_MIPI;
static uint32_t g_actual_phy_data_rate;
static uint32_t g_reg_map_flag = 0;

#if MIPI_TX_INTERRUPTS_ENABLE
static void mipi_tx_enable_int(uint32_t enable);
#endif

void mipi_tx_drv_set_irq_num(uint32_t irq_num)
{
    g_mipi_tx_irq_num = irq_num;
}

void mipi_tx_drv_set_regs(mipi_tx_regs_type_t *regs)
{
    g_mipi_tx_regs_va = regs;
}

static void mipi_tx_set_actual_phy_data_rate(uint32_t actual_date_rate)
{
    g_actual_phy_data_rate = actual_date_rate;
}

uint32_t mipi_tx_get_actual_phy_data_rate(void)
{
    return g_actual_phy_data_rate;
}

void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx)
{
    volatile reg_hori0_det hori0_det;
    volatile reg_hori1_det hori1_det;
    volatile reg_vert_det vert_det;
    volatile reg_vsa_det vsa_det;
    volatile reg_ppi_v_send1 vert_ppi;
    volatile reg_ppi_h_send1 hori_ppi;
    volatile reg_lane_num lane_num;

    hori0_det.u32 = g_mipi_tx_regs_va->hori0_det.u32;
    hori1_det.u32 = g_mipi_tx_regs_va->hori1_det.u32;
    vert_det.u32 = g_mipi_tx_regs_va->vert_det.u32;
    vsa_det.u32 = g_mipi_tx_regs_va->vsa_det.u32;
    vert_ppi.u32 = g_mipi_tx_regs_va->ppi_v_send1.u32;
    hori_ppi.u32 = g_mipi_tx_regs_va->ppi_h_send1.u32;
    lane_num.u32 = g_mipi_tx_regs_va->lane_num.u32;

    mipi_tx_phy_ctx->hact_det = hori0_det.bits.hact_det;
    mipi_tx_phy_ctx->hall_det = hori0_det.bits.hline_det;
    mipi_tx_phy_ctx->hbp_det  = hori1_det.bits.hbp_det;
    mipi_tx_phy_ctx->hsa_det  = hori1_det.bits.hsa_det;

    mipi_tx_phy_ctx->vact_det = vert_det.bits.vact_det;
    mipi_tx_phy_ctx->vall_det = vert_det.bits.vall_det;
    mipi_tx_phy_ctx->vsa_det  = vsa_det.bits.vsa_det;

    mipi_tx_phy_ctx->vall_ppi = vert_ppi.bits.vtotal_length;
    mipi_tx_phy_ctx->vact_ppi = vert_ppi.bits.vactive_length;
    mipi_tx_phy_ctx->hact_ppi = hori_ppi.bits.hactive_length;

    mipi_tx_phy_ctx->lane_num = lane_num.bits.lane_num + 1;
}

void mipi_tx_drv_get_int_status(uint32_t *int_dfx_info, uint32_t info_len)
{
    if (info_len == MIPI_INT_DFX_NUM) {
        int_dfx_info[0] = reg32(MIPI_TX_INT0_STATUS);
        int_dfx_info[1] = reg32(MIPI_TX_INT1_STATUS);
    }
}

#ifdef PRE_FPGA

#ifdef HIWING_PHY
typedef struct {
    uint32_t addr;
    uint32_t cfg_val;
} hiwing_i2c_cfg;

hiwing_i2c_cfg g_hiwing_cfg[] = {
    {0xF8A22024, 0x22000000},
    {0xF8A22028, 0x2001063},
    {0xF8A22358, 0x303019f},
    {0xF8A20A14, 0x00011080},
    {0xF8C20004, 0x1F},
    {0xF8C20008, 0x1},
    {0xF8C20064, 0x1},
    {0xF8C20068, 0x1},
    {0xF8C20070, 0x1F},
    {0xF8C2000C, 0x1},
    {0xF8C20800, 0x48},
    {0xF8C20800, 0x41},
    {0xF8C20960, 0x540},
    {0xF8C20840, 0x1},
    {0xF8C20840, 0x11},
    {0xF8C200B8, 0xa0002a00},
    {0xF8C21004, 0x10000000},
    {0xF8C21018, 0xc},
    {0xF8C2101C, 0x1},
    {0xF8C210A0, 0x53},
    {0xF8C2109C, 0x28033},
    {0xF8C21d28, PLL_CFG_D28_020},
    {0xF8C21020, PLL_CFG_D28_020},
    {0xF8C21d20, PLL_CFG_D20},
    {0xF8C21d2c, 0x1},
    {0xF8C21cb8, 0x93},
    {0xF8C21d40, 0x80},
    /* DATALANE0 */
    {0xF8C21dc0, 0x1B},
    {0xF8C21dc4, 0x1},
    {0xF8C21dc8, 0x3},
    {0xF8C21dcc, 0x3},
    {0xF8C21dd0, 0x5},
    {0xF8C21dd4, 0x4},
    {0xF8C21ddc, 0x23},
    /* DATALANE1 */
    {0xF8C21e40, 0x1B},
    {0xF8C21e44, 0x1},
    {0xF8C21e48, 0x3},
    {0xF8C21e4c, 0x3},
    {0xF8C21e50, 0x5},
    {0xF8C21e54, 0x4},
    {0xF8C21e5c, 0x23},
    /* CLKLANE */
    {0xF8C21ec0, 0x1},
    {0xF8C21ec4, 0x15},
    {0xF8C21ec8, 0x3},
    {0xF8C21ecc, 0x3},
    {0xF8C21ed0, 0xc},
    {0xF8C21ed4, 0x3},
    {0xF8C21edc, 0x23},
    /* DATALANE2 */
    {0xF8C21f40, 0x1B},
    {0xF8C21f44, 0x1},
    {0xF8C21f48, 0x3},
    {0xF8C21f4c, 0x3},
    {0xF8C21f50, 0x5},
    {0xF8C21f54, 0x4},
    {0xF8C21f5c, 0x23},
    /* DATALANE3 */
    {0xF8C21fc0, 0x1b},
    {0xF8C21fc4, 0x1},
    {0xF8C21fc8, 0x3},
    {0xF8C21fcc, 0x3},
    {0xF8C21fd0, 0x5},
    {0xF8C21fd4, 0x4},
    {0xF8C21fdc, 0x23},

    {0xf8c20030, 0x22a},
    {0xF8C21C80, 0x00},
    {0xF8C21C84, 0x01},
    {0xF8C21C84, 0x10},
    {0xf8c20008, 0x0},
    {0xf8c20008, 0x303}
};

#define HIWING_CONFIG_NUN (sizeof(g_hiwing_cfg) / sizeof(g_hiwing_cfg[0]))

#define HIWING_SLAVE_ADDR  0x5A
i2c_bus_t g_hiwing_i2c_device_id = I2C_BUS_4;

#define HIWING_REG_LEN  4
uint32_t mipi_hiwing_read_reg(uint32_t offset, uint32_t *read_val)
{
    errcode_t ret;
    i2c_data_t data;

    uint8_t reg_addr[HIWING_REG_LEN] = { 0 };
    reg_addr[HAL_MIPI_READ_INDEX0] = (offset >> 24) & 0xff; /* 24: high byte send first */
    reg_addr[HAL_MIPI_READ_INDEX1] = (offset >> 16) & 0xff; /* 16: adjust byte send order */
    reg_addr[HAL_MIPI_READ_INDEX2] = (offset >> 8) & 0xff;  /* 8: adjust byte send order */
    reg_addr[HAL_MIPI_READ_INDEX3] = offset & 0xff;

    data.send_buf = reg_addr;
    data.send_len = HIWING_REG_LEN;
    data.receive_buf = (uint8_t *)read_val;
    data.receive_len = HIWING_REG_LEN;

    /* 待使用uapi_i2c_master_writeread()接口替换 */
    ret = uapi_i2c_master_write(g_hiwing_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        mipi_tx_err("===== Error: I2C write ret = 0x%x! =====\r\n", ret);
        return EXT_ERR_MIPI_I2C_READ_FAILED;
    }

    ret = uapi_i2c_master_read(g_hiwing_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        mipi_tx_err("===== Error: I2C read ret = 0x%x! =====\r\n", ret);
        return EXT_ERR_MIPI_I2C_READ_FAILED;
    }
    return EXT_ERR_SUCCESS;
}

uint32_t mipi_hiwing_write_reg(uint32_t offset, uint32_t reg_val)
{
    errcode_t ret;
    i2c_data_t data;

    uint8_t reg_addr[HIWING_REG_LEN] = {0};
    reg_addr[HAL_MIPI_READ_INDEX0] = (offset >> 24) & 0xff; /* 24: high byte send first */
    reg_addr[HAL_MIPI_READ_INDEX1] = (offset >> 16) & 0xff; /* 16: adjust byte send order */
    reg_addr[HAL_MIPI_READ_INDEX2] = (offset >> 8) & 0xff;  /* 8: adjust byte send order */
    reg_addr[HAL_MIPI_READ_INDEX3] = offset & 0xff;

    data.send_buf = reg_addr;
    data.send_len = HIWING_REG_LEN;
    data.receive_buf = NULL;
    data.receive_len = 0;

    ret = uapi_i2c_master_write(g_hiwing_i2c_device_id, HIWING_SLAVE_ADDR, &data);
    if (ret != ERRCODE_SUCC) {
        mipi_tx_err("===== Error: I2C write ret = 0x%x! =====\r\n", ret);
        return EXT_ERR_MIPI_I2C_WRITE_FAILED;
    }

    return EXT_ERR_SUCCESS;
}

uint32_t mipi_tx_hiwing_i2c_cfg(void)
{
    errcode_t ret;

    ret = uapi_i2c_master_init(g_hiwing_i2c_device_id, 100000, I2C_SPEED_MODE_SS); /* 100000: 100KHz */
    if (ret != ERRCODE_SUCC) {
        return EXT_ERR_MIPI_I2C_INIT_FAILED;
    }

    mipi_tx_print("I2C init succ!\r\n");
    return EXT_ERR_SUCCESS;
}

uint32_t mipi_tx_hiwing_phy_write(uint32_t start_addr, uint32_t write_val)
{
    uint32_t read_val;
    uint32_t ret =  mipi_hiwing_read_reg(start_addr, &read_val);
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    ret = mipi_hiwing_write_reg(start_addr, write_val);
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    mipi_tx_print("Write [ 0x%x ] : [ 0x%x ] >> [ 0x%x ]\r\n", start_addr, read_val, write_val);

    return EXT_ERR_SUCCESS;
}

uint32_t mipi_tx_hiwing_phy_read(uint32_t start_addr)
{
    uint32_t read_val;
    uint32_t ret = mipi_hiwing_read_reg(start_addr, &read_val);
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("%x err at line: %d \r\n", ret, __LINE__);
        return ret;
    }

    mipi_tx_print("Read [ 0x%x ] : [ 0x%x ]\r\n", start_addr, read_val);
    return EXT_ERR_SUCCESS;
}

#define MIPI_PHY_REF_CLK 320
#define total_div(x) (((x) * 10 + (MIPI_PHY_REF_CLK - 1)) / MIPI_PHY_REF_CLK) /* 10: x对参考时钟向上取整 */

/*
* pll cfg
* datarate(eg.260 Mbps)=ref_clk(eg.19.2m)(*value of 0xF8C21d28,'h67~'hff)  (eg.0x6c)/2^ (value of 0xF8C21d20)(eg.0x3)
* eg.0xa0 with div8 at 19.2MHz ref_clk=data rate 384Mbps
* for gt2 panel: div2 rang [1..5]
*/
static void mipi_tx_gt2_cal_div(void)
{
    uint32_t div1;
    uint32_t div2;
    uint32_t i;
    uint32_t tmp_div;

    uint32_t phy_fre = mipi_tx_get_actual_phy_data_rate();

    for (div2 = 1; div2 <= 5; div2++) { /* for gt2 panel: div2 rang [1..5] */
        tmp_div = 1;
        for (i = 0; i < div2; i++) {
            tmp_div *= 2; /* cal exponent of 2 */
        }
        div1 = tmp_div * total_div(phy_fre);
        if ((div1 >= 0x67) && (div1 <= 0xff)) {
            break;
        }
    }
    if (div2 > 5) { /* for gt2 panel: div2 rang [1..5] */
        mipi_tx_err("mipi fpga phy div cal fail!\r\n");
        return;
    }

    mipi_tx_print("div1=0x%x  div2=0x%x\r\n", div1, div2);
    for (i = 0; i < HIWING_CONFIG_NUN; i++) {
        if (g_hiwing_cfg[i].addr == 0xF8C21d28 || g_hiwing_cfg[i].addr == 0xF8C21020) {
            g_hiwing_cfg[i].cfg_val = div1;
        }
        if (g_hiwing_cfg[i].addr == 0xF8C21d20) {
            g_hiwing_cfg[i].cfg_val = div2;
        }
    }
}

void mipi_tx_gt2_hiwing_phy_init(void)
{
    uint32_t ret;
    uint32_t i;

    mipi_tx_gt2_cal_div();
    for (i = 0; i < HIWING_CONFIG_NUN; i++) {
        ret = mipi_tx_hiwing_phy_write(g_hiwing_cfg[i].addr, g_hiwing_cfg[i].cfg_val);
        if (ret != EXT_ERR_SUCCESS) {
            return;
        }
    }
}

static void mipi_tx_drv_set_phy_pll_setx(uint32_t phy_data_rate)
{
    UNUSED(phy_data_rate);
    /* gt2 hiwing phy configuration */
    (void)mipi_tx_hiwing_i2c_cfg();
    mipi_tx_gt2_hiwing_phy_init();
}

#endif /* end of #ifdef HIWING_PHY */

#ifdef ALTERA_PHY

static void set_phy_reg(uint32_t addr, uint8_t value)
{
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = (0x10000 + addr);
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = value;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    ext_fence();
}

uint8_t get_phy_reg(uint32_t addr)
{
    uint32_t reg_value;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg1.u32 = (0x10000 + addr);
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x2;
    ext_fence();
    g_mipi_tx_regs_va->phy_reg_cfg0.u32 = 0x0;
    ext_fence();
    reg_value = g_mipi_tx_regs_va->phy_reg_cfg1.u32;
    ext_fence();

    return (uint8_t)((reg_value >> 0x8) & 0xff);
}

static void mipi_tx_drv_set_phy_pll_setx(uint32_t phy_data_rate)
{
    uint8_t pll_set0;
    uint8_t pll_set1;
    uint8_t pll_set2;
    uint8_t pll_set3;
    uint8_t pll_set4;

    /* step2 : */
    /* pll_set0 */
    pll_set0 = 0x0;
    set_phy_reg(PLL_SET_0X14, pll_set0);

    /* pll_set1 */
    pll_set1 = 0x11;
    set_phy_reg(PLL_SET_0X15, pll_set1);

    /* pll_set2 */
    pll_set2 = 0x21;
    set_phy_reg(PLL_SET_0X16, pll_set2);

    /* pll_set3 */
    pll_set3 = 0x1;
    set_phy_reg(PLL_SET_0X17, pll_set3);

    /* pll_set4 */
    pll_set4 = 0x68;
    set_phy_reg(PLL_SET_0X1E, pll_set4);

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("\r\n==========phy pll info=======\r\n");
    mipi_tx_print("pll_set0(0x14): 0x%x\r\n", pll_set0);
    mipi_tx_print("pll_set1(0x15): 0x%x\r\n", pll_set1);
    mipi_tx_print("pll_set2(0x16): 0x%x\r\n", pll_set2);
    mipi_tx_print("pll_set3(0x17): 0x%x\r\n", pll_set3);
    mipi_tx_print("pll_set4(0x1e): 0x%x\r\n", pll_set4);
    mipi_tx_print("=========================\r\n");
#endif
}

#endif /* end of #ifdef ALTERA_PHY */

#else  /* asic */

static uint8_t mipi_tx_drv_get_phy_pll_set_ckg_sel(uint32_t phy_data_rate, uint32_t *clk_pre)
{
    uint8_t ckg_sel;

    if ((phy_data_rate >= 750) && (phy_data_rate <= 1500)) { /* bandwidth range [750, 1500] */
        ckg_sel = 0;
        *clk_pre = phy_data_rate * 0x1;
    } else if (phy_data_rate >= 375) { /* bandwidth range [375, 750) */
        ckg_sel = 0x4;
        *clk_pre = phy_data_rate * 0x2;
    } else if (phy_data_rate >= 188) { /* bandwidth range [188, 375) */
        ckg_sel = 0x5;
        *clk_pre = phy_data_rate * 0x4;
    } else if (phy_data_rate >= 94) { /* bandwidth range [94, 188) */
        ckg_sel = 0x6;
        *clk_pre = phy_data_rate * 0x8;
    } else { /* bandwidth range [80, 94) */
        ckg_sel = 0x7;
        *clk_pre = phy_data_rate * 0xf;
    }

    return ckg_sel;
}

static uint32_t mipi_tx_drv_get_phy_pll_set_fbdiv(uint32_t clk_pre, uint32_t prdiv)
{
    return ((clk_pre * prdiv) / MIPI_TX_REF_CLK);
}

static void mipi_tx_drv_set_phy_pll_setx(uint32_t phy_data_rate)
{
    uint8_t pll_set_ckg_sel;
    uint8_t pll_set_prdiv_fbdiv;
    uint8_t pll_set_fbdiv;
    uint8_t set_ckg_sel;
    uint8_t set_prdiv = 0x1; /* always 2 div */
    uint32_t set_fbdiv;
    uint32_t clk_pre;

    /* cal set_fbdiv, set_fbdiv, set_prdiv */
    set_ckg_sel = mipi_tx_drv_get_phy_pll_set_ckg_sel(phy_data_rate, &clk_pre);
    set_fbdiv = mipi_tx_drv_get_phy_pll_set_fbdiv(clk_pre, 2); /* 2 div */

    pll_set_ckg_sel = (set_ckg_sel << 1) & 0xf;
    pll_set_fbdiv = (uint8_t)(set_fbdiv & 0xff);
    pll_set_prdiv_fbdiv =
        (((uint8_t)((set_fbdiv >> 8) & 0x7)) << 4) | (set_prdiv); /* val[8..4]: fbdiv[10..8] val[4..0]: prdiv */

    writeb(PLL_SET_CKG_SEL, pll_set_ckg_sel);
    writeb(PLL_SET_FBDIV, pll_set_fbdiv);
    writeb(PLL_SET_FBDIV_PRDIV, pll_set_prdiv_fbdiv);

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("\r\n==========phy pll info=======\r\n");
    mipi_tx_print("pll_set_ckg_sel: 0x%x\r\n", pll_set_ckg_sel);
    mipi_tx_print("pll_set_prdiv: 0x%x\r\n", set_prdiv);
    mipi_tx_print("pll_set_fbdiv: 0x%x\r\n", set_fbdiv);
    mipi_tx_print("=========================\r\n");
#endif
}

#endif /* end of #ifdef PRE_FPGA */

#ifdef PRE_FPGA

static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
#if defined HIWING_PHY
    uint32_t actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();

    /* DATA0~1 TPRE-DELAY */
    tp->data_tpre_delay = (actual_phy_data_rate * TPRE_DELAY + ROUNDUP_VALUE) / INNER_PEROID;
    /* CLK_TLPX */
    tp->clk_tlpx = (actual_phy_data_rate * TLPX + ROUNDUP_VALUE) / INNER_PEROID;
    /* CLK_TCLK_PREPARE */
    tp->clk_tclk_prepare = (actual_phy_data_rate * TCLK_PREPARE + ROUNDUP_VALUE) / INNER_PEROID;
    /* CLK_TCLK_ZERO */
    tp->clk_tclk_zero = (actual_phy_data_rate * TCLK_ZERO + ROUNDUP_VALUE) / INNER_PEROID;
    /* CLK_TCLK_TRAIL */
    tp->clk_tclk_trail = (actual_phy_data_rate * TCLK_TRAIL + ROUNDUP_VALUE) / INNER_PEROID;
    /* DATA_TLPX */
    tp->data_tlpx = (actual_phy_data_rate * TLPX + ROUNDUP_VALUE) / INNER_PEROID;
    /* DATA_THS_PREPARE */
    tp->data_ths_prepare = (actual_phy_data_rate * THS_PREPARE + ROUNDUP_VALUE) / INNER_PEROID;
    /* DATA_THS_ZERO */
    tp->data_ths_zero = (actual_phy_data_rate * THS_ZERO + ROUNDUP_VALUE) / INNER_PEROID;
    /* DATA_THS_TRAIL */
    tp->data_ths_trail = (actual_phy_data_rate * THS_TRAIL + ROUNDUP_VALUE) / INNER_PEROID;
#else
    tp->data_tpre_delay = DATA0_TPRE_DELAY_VALUE;
    tp->clk_post_delay = CLK_POST_DELAY_VALUE;
    tp->clk_tlpx = CLK_TLPX_VALUE;
    tp->clk_tclk_prepare = CLK_TCLK_PREPARE_VALUE;
    tp->clk_tclk_zero = CLK_TCLK_ZERO_VALUE;
    tp->clk_tclk_trail = CLK_TCLK_TRAIL_VALUE;
    tp->data_tlpx = DATA0_TLPX_VALUE;
    tp->data_ths_prepare = DATA0_THS_PREPARE_VALUE;
    tp->data_ths_zero = DATA0_THS_ZERO_VALUE;
    tp->data_ths_trail = DATA0_THS_TRAIL_VALUE;
#endif /* end of #if defined HIWING_PHY */
}

/*
 * set global operation timing parameters.
 */
static void mipi_tx_drv_set_phy_timing_parameters(const mipi_tx_phy_timing_parameters *tp)
{
#if defined HIWING_PHY
    UNUSED(tp);
#else
    /* DATA0~3 TPRE-DELAY */
    set_phy_reg(DATA0_TPRE_DELAY, tp->data_tpre_delay);
    set_phy_reg(DATA1_TPRE_DELAY, tp->data_tpre_delay);
    /* CLK_TLPX */
    set_phy_reg(CLK_TLPX, tp->clk_tlpx);

    /* CLK_TCLK_PREPARE */
    set_phy_reg(CLK_TCLK_PREPARE, tp->clk_tclk_prepare);

    /* CLK_TCLK_ZERO */
    set_phy_reg(CLK_TCLK_ZERO, tp->clk_tclk_zero);

    /* CLK_TCLK_TRAIL */
    set_phy_reg(CLK_TCLK_TRAIL, tp->clk_tclk_trail);

    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     */
    set_phy_reg(DATA0_TLPX, tp->data_tlpx);
    set_phy_reg(DATA0_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA0_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA0_THS_TRAIL, tp->data_ths_trail);
    set_phy_reg(DATA1_TLPX, tp->data_tlpx);
    set_phy_reg(DATA1_THS_PREPARE, tp->data_ths_prepare);
    set_phy_reg(DATA1_THS_ZERO, tp->data_ths_zero);
    set_phy_reg(DATA1_THS_TRAIL, tp->data_ths_trail);
#endif /* end of #if defined HIWING_PHY */

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("\r\n==========phy timing parameters=======\r\n");
    mipi_tx_print("data_tpre_delay(0x30/40/50/60): 0x%x\r\n", tp->data_tpre_delay);
    mipi_tx_print("clk_post_delay(0x21): 0x%x\r\n", tp->clk_post_delay);
    mipi_tx_print("clk_tlpx(0x22): 0x%x\r\n", tp->clk_tlpx);
    mipi_tx_print("clk_tclk_prepare(0x23): 0x%x\r\n", tp->clk_tclk_prepare);
    mipi_tx_print("clk_tclk_zero(0x24): 0x%x\r\n", tp->clk_tclk_zero);
    mipi_tx_print("clk_tclk_trail(0x25): 0x%x\r\n", tp->clk_tclk_trail);
    mipi_tx_print("data_tlpx(0x32/42/52/62): 0x%x\r\n", tp->data_tlpx);
    mipi_tx_print("data_ths_prepare(0x33/43/53/63): 0x%x\r\n", tp->data_ths_prepare);
    mipi_tx_print("data_ths_zero(0x34/44/54/64): 0x%x\r\n", tp->data_ths_zero);
    mipi_tx_print("data_ths_trail(0x35/45/55/65): 0x%x\r\n", tp->data_ths_trail);
    mipi_tx_print("=========================\r\n");
#endif
}

#else

static inline uint8_t mipi_tx_drv_get_phy_timing_parameters_by_type(uint32_t timing_type)
{
    uint32_t actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();

    return (uint8_t)((actual_phy_data_rate * timing_type + ROUNDUP_VALUE) / INNER_PEROID);
}

/*
 * get global operation timing parameters.
 */
static void mipi_tx_drv_get_phy_timing_parameters(mipi_tx_phy_timing_parameters *tp)
{
    /* CLK_TLPX */
    tp->clk_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX);

    /* CLK_TCLK_PREPARE */
    tp->clk_tclk_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_PREPARE);

    /* CLK_TCLK_ZERO */
    tp->clk_tclk_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_ZERO);

    /* CLK_TCLK_TRAIL */
    tp->clk_tclk_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(TCLK_TRAIL);

    /* DATA_TLPX */
    tp->data_tlpx = mipi_tx_drv_get_phy_timing_parameters_by_type(TLPX);

    /* DATA_THS_PREPARE */
    tp->data_ths_prepare = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_PREPARE);

    /* DATA_THS_ZERO */
    tp->data_ths_zero = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_ZERO);

    /* DATA_THS_TRAIL */
    tp->data_ths_trail = mipi_tx_drv_get_phy_timing_parameters_by_type(THS_TRAIL);

    /* DATA0~1 TPRE-DELAY */
    tp->data_tpre_delay = tp->clk_tlpx + tp->clk_tclk_prepare + tp->clk_tclk_zero + 9; /* 9: 3 + 5 + clk_pre_delay */

    /* CLK post delay */
    tp->clk_post_delay = tp->data_ths_trail + 11; /* 11 : 10+data_post_delay */
}

/*
 * set global operation timing parameters.
 */
static void mipi_tx_drv_set_phy_timing_parameters(const mipi_tx_phy_timing_parameters *tp)
{
    /*
     * CLK_TLPX
     * CLK_TCLK_PREPARE
     * CLK_TCLK_ZERO
     * CLK_TCLK_TRAIL
     * CLK_TCLK_PRE_DELAY
     * CLK_TCLK_POST_DELAY
     */
    writeb(CLK_TLPX, tp->clk_tlpx);
    writeb(CLK_TCLK_PREPARE, tp->clk_tclk_prepare);
    writeb(CLK_TCLK_ZERO, tp->clk_tclk_zero);
    writeb(CLK_TCLK_TRAIL, tp->clk_tclk_trail);
    writeb(CLK_TCLK_POST_DELAY, tp->clk_post_delay);
    writeb(CLK_TCLK_PRE_DELAY, 1);

    /* CKG_EN_WAIT */
    writeb(CKG_EN_WAIT, SET_CKG_EN_WAIT_VALUE);

    /*
     * DATA_TLPX
     * DATA_THS_PREPARE
     * DATA_THS_ZERO
     * DATA_THS_TRAIL
     * DATA0~1 TPRE-DELAY
     */
    writeb(DATA0_TLPX, tp->data_tlpx);
    writeb(DATA0_THS_PREPARE, tp->data_ths_prepare);
    writeb(DATA0_THS_ZERO, tp->data_ths_zero);
    writeb(DATA0_THS_TRAIL, tp->data_ths_trail);
    writeb(DATA0_TPRE_DELAY, tp->data_tpre_delay);
    writeb(DATA1_TLPX, tp->data_tlpx);
    writeb(DATA1_THS_PREPARE, tp->data_ths_prepare);
    writeb(DATA1_THS_ZERO, tp->data_ths_zero);
    writeb(DATA1_THS_TRAIL, tp->data_ths_trail);
    writeb(DATA1_TPRE_DELAY, tp->data_tpre_delay);

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("\r\n==========phy timing parameters=======\r\n");
    mipi_tx_print("data_tpre_delay(0x4A0/4E0): 0x%x\r\n", tp->data_tpre_delay);
    mipi_tx_print("clk_tlpx(0x440): 0x%x\r\n", tp->clk_tlpx);
    mipi_tx_print("clk_tclk_prepare(0x444): 0x%x\r\n", tp->clk_tclk_prepare);
    mipi_tx_print("clk_tclk_zero(0x448): 0x%x\r\n", tp->clk_tclk_zero);
    mipi_tx_print("clk_tclk_trail(0x44C): 0x%x\r\n", tp->clk_tclk_trail);
    mipi_tx_print("data_tlpx(0x480/4C0): 0x%x\r\n", tp->data_tlpx);
    mipi_tx_print("data_ths_prepare(0x484/4C4): 0x%x\r\n", tp->data_ths_prepare);
    mipi_tx_print("data_ths_zero(0x488/4C8): 0x%x\r\n", tp->data_ths_zero);
    mipi_tx_print("data_ths_trail(0x48C/4CC): 0x%x\r\n", tp->data_ths_trail);
    mipi_tx_print("=========================\r\n");
#endif
}

#endif /* end of #ifdef PRE_FPGA */

#ifdef PRE_FPGA
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
#if defined HIWING_PHY
    uint8_t data_tpre_delay = tp->data_tpre_delay;
    uint8_t clk_tlpx = tp->clk_tlpx;
    uint8_t clk_tclk_prepare = tp->clk_tclk_prepare;
    uint8_t clk_tclk_zero = tp->clk_tclk_zero;
    uint8_t data_tlpx = tp->data_tlpx;
    uint8_t data_ths_prepare = tp->data_ths_prepare;
    uint8_t data_ths_zero = tp->data_ths_zero;
    uint8_t data_ths_trail = tp->data_ths_trail;

    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = ((data_ths_trail - 2) << 16) + /* 2, 16 tx time param */
        data_tpre_delay + data_tlpx + data_ths_prepare + data_ths_zero + 8;  /* 8 tx time param */
    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = ((30 + data_ths_trail) << 16) +  /* 30, 16 tx time param */
        clk_tlpx + clk_tclk_prepare + clk_tclk_zero + 7;   /* 7 tx time param */
#else
    /* kdk panel data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = 0xA002D;
    /* kdk panel clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = 0x260032;
#endif /* end of #if defined HIWING_PHY */

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("datalane_time(0x9C): 0x%x\r\n", g_mipi_tx_regs_va->datalane_time.u32);
    mipi_tx_print("clklane_time(0x98): 0x%x\r\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}

#if defined ALTERA_PHY
void mipi_tx_drv_get_dl_phy_cfg(void)
{
    uint8_t phy_val;
    phy_val = get_phy_reg(DATA0_TLPX);
    mipi_tx_print("(0x32): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA0_THS_PREPARE);
    mipi_tx_print("(0x33): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA0_THS_ZERO);
    mipi_tx_print("(0x34): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA0_THS_TRAIL);
    mipi_tx_print("(0x35): 0x%x\r\n", phy_val);

    phy_val = get_phy_reg(DATA1_TLPX);
    mipi_tx_print("(0x42): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA1_THS_PREPARE);
    mipi_tx_print("(0x43): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA1_THS_ZERO);
    mipi_tx_print("(0x44): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA1_THS_TRAIL);
    mipi_tx_print("(0x45): 0x%x\r\n", phy_val);
}


void mipi_tx_drv_get_phy_cfg(void)
{
    uint8_t phy_val;

    phy_val = get_phy_reg(PLL_SET_0X14);
    mipi_tx_print("(0x14): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(PLL_SET_0X15);
    mipi_tx_print("(0x15): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(PLL_SET_0X16);
    mipi_tx_print("(0x16): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(PLL_SET_0X17);
    mipi_tx_print("(0x17): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(PLL_SET_0X1E);
    mipi_tx_print("(0x1e): 0x%x\r\n", phy_val);

    phy_val = get_phy_reg(DATA0_TPRE_DELAY);
    mipi_tx_print("(0x30): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(DATA1_TPRE_DELAY);
    mipi_tx_print("(0x40): 0x%x\r\n", phy_val);

    phy_val = get_phy_reg(CLK_TLPX);
    mipi_tx_print("(0x22): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(CLK_TCLK_PREPARE);
    mipi_tx_print("(0x23): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(CLK_TCLK_ZERO);
    mipi_tx_print("(0x24): 0x%x\r\n", phy_val);
    phy_val = get_phy_reg(CLK_TCLK_TRAIL);
    mipi_tx_print("(0x25): 0x%x\r\n", phy_val);

    mipi_tx_drv_get_dl_phy_cfg();
}
#endif /* end of #if defined ALTERA_PHY */
#else

/*
 * set data lp2hs,hs2lp time
 * set clk lp2hs,hs2lp time
 * unit: hsclk
 */
static void mipi_tx_drv_set_phy_hs_lp_switch_time(const mipi_tx_phy_timing_parameters *tp)
{
    uint8_t data_tpre_delay = tp->data_tpre_delay;
    uint8_t clk_tlpx = tp->clk_tlpx;
    uint8_t clk_tclk_prepare = tp->clk_tclk_prepare;
    uint8_t clk_tclk_zero = tp->clk_tclk_zero;
    uint8_t data_tlpx = tp->data_tlpx;
    uint8_t data_ths_zero = tp->data_ths_zero;
    uint8_t data_ths_trail = tp->data_ths_trail;
    uint8_t data_ths_prepare = tp->data_ths_prepare;

    /* data lp2hs,hs2lp time */
    g_mipi_tx_regs_va->datalane_time.u32 = ((data_ths_trail - 2) << 16) + /* 2, 16 tx time param */
        data_tpre_delay + data_tlpx + data_ths_prepare + data_ths_zero + 8;  /* 8 tx time param */
    /* clk lp2hs,hs2lp time */
    g_mipi_tx_regs_va->clklane_time.u32 = ((30 + data_ths_trail) << 16) +  /* 30, 16 tx time param */
        clk_tlpx + clk_tclk_prepare + clk_tclk_zero + 7;   /* 7 tx time param */

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("datalane_time(0x9C): 0x%x\r\n", g_mipi_tx_regs_va->datalane_time.u32);
    mipi_tx_print("clklane_time(0x98): 0x%x\r\n", g_mipi_tx_regs_va->clklane_time.u32);
#endif
}

#endif /* end of #ifdef PRE_FPGA */

static void mipi_tx_drv_set_input_enable(uint8_t input_en)
{
    volatile reg_operation_mode operation_mode = {0};

    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    if (input_en == 0) {
        operation_mode.u32 = 0;
        g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
        return;
    }

    operation_mode.bits.mem_ck_en = 1;
    operation_mode.bits.input_en = 1;
    operation_mode.bits.hss_abnormal_rst = 1;
    operation_mode.bits.read_empty_vsync_en = 1;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;
}

static void mipi_tx_drv_wait_phy_ready(const short lane_id[], uint32_t lane_id_len)
{
    uint32_t lane_num;
    uint32_t ready_flag = 0;
    uint32_t wait_count = 0;
    uint32_t data_ready = 0;
    uint32_t clk_ready = 0;

    lane_num = mipi_tx_get_lane_num(lane_id, lane_id_len);

    do {
        clk_ready = (g_mipi_tx_regs_va->ppi_status.bits.phy_pll_lock &
            g_mipi_tx_regs_va->ppi_status.bits.stopstateclklane);

        if (lane_num == 1) {
            if (lane_id[0] != MIPI_TX_DISABLE_LANE_ID) {
                data_ready = g_mipi_tx_regs_va->ppi_status.bits.stopstate0lane;
            } else if (lane_id[1] != MIPI_TX_DISABLE_LANE_ID) {
                data_ready = g_mipi_tx_regs_va->ppi_status.bits.stopstate1lane;
            }
        } else if (lane_num == 2) { /* 2: data_lane number */
            data_ready = (g_mipi_tx_regs_va->ppi_status.bits.stopstate0lane &
                g_mipi_tx_regs_va->ppi_status.bits.stopstate1lane);
        }

        wait_count++;
        uapi_tcxo_delay_us(5); /* delay 5 us */

        if (wait_count > MIPI_TX_PHY_READY_TIMEOUT_CNT) {
            mipi_tx_err("timeout when wait phy ready \r\n");
            break;
        }
    } while ((clk_ready & data_ready) == 0);
}

void mipi_tx_drv_set_phy_cfg(const combo_dev_cfg_t *dev_cfg)
{
    uint32_t actual_data_rate = mipi_tx_get_actual_phy_data_rate();
    mipi_tx_phy_timing_parameters tp = {0};

    /* disable input, done in mipi_tx_drv_set_controller_cfg */
    mipi_tx_drv_set_input_enable(0);

    /* pwr_up unreset */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    uapi_tcxo_delay_us(1);
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;

    /* set phy pll parameters setx to generate data rate */
    mipi_tx_drv_set_phy_pll_setx(actual_data_rate);

    /* get global operation timing parameters */
    mipi_tx_drv_get_phy_timing_parameters(&tp);

    /* set global operation timing parameters */
    mipi_tx_drv_set_phy_timing_parameters(&tp);

    /*
     * dsi controller
     * set hs switch to lp and lp switch to hs time
     */
    mipi_tx_drv_set_phy_hs_lp_switch_time(&tp);

#ifdef PRE_ASIC
    /* set RG_LPTX_SELENFOR/RG_LPTX_SELFORHI */
    reg8_setbits(RG_LPTX_SELENFOR_SELFORHI, BIT_RG_LPTX_SELENFOR, 2, 0x3); /* set 2 bits */
#endif

    /*
     * phy:
     * forcepll: pll enable
     * enableclk: enable clk lane
     * rstz: unreset
     * shutdownz: powerup
     */
    g_mipi_tx_regs_va->phy_ctrl.u32 = 0xF;

    /* wait phy ready */
    mipi_tx_drv_wait_phy_ready(dev_cfg->lane_id, LANE_MAX_NUM);

#ifdef PRE_ASIC
    /* clear RG_LPTX_SELENFOR/RG_LPTX_SELFORHI */
    reg8_clrbits(RG_LPTX_SELENFOR_SELFORHI, BIT_RG_LPTX_SELENFOR, 2); /* clear 2 bits */
#endif
}

static void set_output_format(const combo_dev_cfg_t *dev_cfg)
{
    uint32_t color_coding = 0;

    if (dev_cfg->out_format == OUT_FORMAT_RGB_16BIT) {
        color_coding = MIPI_TX_DSI_RGB_16BIT;
    } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT) {
        color_coding = MIPI_TX_DSI_RGB_18BIT;
    } else if (dev_cfg->out_format == OUT_FORMAT_RGB_18BIT_LOOSELY) {
        color_coding = MIPI_TX_DSI_RGB_18BIT_LOOSELY;
    } else if (dev_cfg->out_format == OUT_FORMAT_RGB_24BIT) {
        color_coding = MIPI_TX_DSI_RGB_24BIT;
    } else if (dev_cfg->out_format == OUT_FORMAT_YUV420_12BIT) {
        color_coding = MIPI_TX_DSI_YCBCR420_12BIT;
    } else if (dev_cfg->out_format == OUT_FORMAT_YUV422_16BIT) {
        color_coding = MIPI_TX_DSI_YCBCR422_16BIT;
    }

    g_mipi_tx_regs_va->data_type.u32 = (uint32_t)color_coding;
#ifdef MIPI_TX_DEBUG
    mipi_tx_print("set_output_format: 0x%x\r\n", color_coding);
#endif
}

static void mipi_tx_hal_set_cmd_video_mode(cmd_video_mode cmd_video)
{
    volatile reg_mode_cfg mode_cfg;
    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.cmd_video_mode = cmd_video;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void mipi_tx_hal_set_video_mode_tran_type(uint32_t video_mode_tran_type)
{
    volatile reg_mode_cfg mode_cfg;

    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.video_mode_tran_type = video_mode_tran_type;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

#ifdef MIPI_SUPPORT_VIDEO
static void mipi_tx_hal_set_video_mode_type(uint32_t video_mode_type)
{
    volatile reg_mode_cfg mode_cfg;

    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;
    mode_cfg.bits.video_mode_type = video_mode_type;
    g_mipi_tx_regs_va->mode_cfg.u32 = mode_cfg.u32;
}

static void set_video_mode_cfg(const combo_dev_cfg_t *dev_cfg)
{
    uint32_t video_mode_type;
    volatile reg_read_memory_delay_ctrl read_memory_delay_ctrl;

    if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) {
        video_mode_type = 0; /* 0: Non-burst with sync pulse */
    } else if (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS) {
        video_mode_type = 1; /* 1: Non-burst with sync */
    } else {
        video_mode_type = 2; /* 2: burst mode */
    }

    if (dev_cfg->out_mode == OUT_MODE_DSI_CMD) {
        video_mode_type = 2; /* 2: burst mode */
    }

    /*
     * mode config
     * other: burst mode
     * 00: Non-burst with sync pulse
     * 01: Non-burst with sync
     */
    mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    mipi_tx_hal_set_video_mode_type(video_mode_type);

    g_mipi_tx_regs_va->video_lp_en.u32 = 0x3f;

    /* delay value setting. */
    read_memory_delay_ctrl.u32 = g_mipi_tx_regs_va->read_memory_delay_ctrl.u32;
    if ((dev_cfg->video_mode == NON_BURST_MODE_SYNC_PULSES) ||
        (dev_cfg->video_mode == NON_BURST_MODE_SYNC_EVENTS)) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x1;
    } else if (dev_cfg->video_mode == BURST_MODE) {
        read_memory_delay_ctrl.bits.delay_abnormal = 0x0;
    }
    g_mipi_tx_regs_va->read_memory_delay_ctrl.u32 = read_memory_delay_ctrl.u32;
}
#endif

static void set_timing_config(const combo_dev_cfg_t *dev_cfg)
{
    uint32_t hsa_time;
    uint32_t hbp_time;
    uint32_t hline_time;
    uint32_t actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    uint32_t htime_tmp = 1000 / 8; /* 1000, 8: 1000Hz / 8bit */

    if (dev_cfg->pixel_clk == 0) {
        mipi_tx_print("dev_cfg->pixel_clk is 0, illegal.\r\n");
        return;
    }

    hsa_time = (actual_phy_data_rate * dev_cfg->sync_info.hsa_pixels * htime_tmp + (dev_cfg->pixel_clk - 1)) /
        dev_cfg->pixel_clk;
    hbp_time = (actual_phy_data_rate * dev_cfg->sync_info.hbp_pixels  * htime_tmp  + (dev_cfg->pixel_clk - 1)) /
        dev_cfg->pixel_clk;
    hline_time = (actual_phy_data_rate * (dev_cfg->sync_info.hsa_pixels +
        dev_cfg->sync_info.hact_pixels +
        dev_cfg->sync_info.hbp_pixels +
        dev_cfg->sync_info.hfp_pixels) * htime_tmp + (dev_cfg->pixel_clk - 1)) / dev_cfg->pixel_clk;

    g_mipi_tx_regs_va->command_pkt_size.u32 = dev_cfg->sync_info.hact_pixels;
    g_mipi_tx_regs_va->videom_hsa_size.u32 = hsa_time;
    g_mipi_tx_regs_va->videom_hbp_size.u32 = hbp_time;
    g_mipi_tx_regs_va->videom_hline_size.u32 = hline_time;

    g_mipi_tx_regs_va->videom_vsa_lines.u32 = dev_cfg->sync_info.vsa_lines;
    g_mipi_tx_regs_va->videom_vbp_lines.u32 = dev_cfg->sync_info.vbp_lines;
    g_mipi_tx_regs_va->videom_vfp_lines.u32 = dev_cfg->sync_info.vfp_lines;
    g_mipi_tx_regs_va->videom_vactive_lines.u32 = dev_cfg->sync_info.vact_lines;

    /* read cmd hsclk */
    g_mipi_tx_regs_va->read_cmd_time.u32 = hline_time - hsa_time - 4 - 4; /* 4 4:eotp time; redundancy time */

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("hsa_pixels(0x48): 0x%x\r\n", dev_cfg->sync_info.hsa_pixels);
    mipi_tx_print("hbp_pixels(0x4c): 0x%x\r\n", dev_cfg->sync_info.hbp_pixels);
    mipi_tx_print("hact_pixels(0x50): 0x%x\r\n", dev_cfg->sync_info.hact_pixels);
    mipi_tx_print("hfp_pixels(0x50): 0x%x\r\n", dev_cfg->sync_info.hfp_pixels);
    mipi_tx_print("vact_lines(0x54): 0x%x\r\n", dev_cfg->sync_info.vact_lines);
    mipi_tx_print("vbp_lines(0x58): 0x%x\r\n", dev_cfg->sync_info.vbp_lines);
    mipi_tx_print("vfp_lines(0x5c): 0x%x\r\n", dev_cfg->sync_info.vfp_lines);
    mipi_tx_print("vsa_lines(0x60): 0x%x\r\n", dev_cfg->sync_info.vsa_lines);
#endif
}

static void mipi_tx_set_lane_id(uint32_t lane_idx, short lane_id)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (lane_id) {
        case 0:
            lane_id_reg.bits.lane0_id = (uint32_t)lane_idx;
            break;

        case 1:
            lane_id_reg.bits.lane1_id = (uint32_t)lane_idx;
            break;

        default:
            break;
    }
    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void mipi_tx_set_disable_lane_id(uint32_t lane_idx, short invalid_lane_num)
{
    reg_lane_id lane_id_reg;
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;

    switch (invalid_lane_num) {
        case 1:
            lane_id_reg.bits.lane1_id = (uint32_t)lane_idx;
            break;

        default:
            break;
    }

    g_mipi_tx_regs_va->lane_id.u32 = lane_id_reg.u32;
}

static void set_lane_config(const short lane_id[], uint32_t lane_id_len)
{
    uint32_t max_lane_num = LANE_MAX_NUM;
    uint32_t i;
    uint32_t invalid_lane_num = 0;
    uint32_t unused_id = 3; /* 3, for unused lane */
#ifdef MIPI_TX_DEBUG
    reg_lane_id lane_id_reg;
#endif

    g_mipi_tx_regs_va->lane_num.u32 = mipi_tx_get_lane_num(lane_id, lane_id_len) - 1;

    for (i = 0; i < max_lane_num; i++) {
        if (lane_id[i] == MIPI_TX_DISABLE_LANE_ID) {
            invalid_lane_num++;
            mipi_tx_set_disable_lane_id(unused_id, (short)invalid_lane_num);
            continue;
        }
        mipi_tx_set_lane_id(i, lane_id[i]);
    }

#ifdef MIPI_TX_DEBUG
    lane_id_reg.u32 = g_mipi_tx_regs_va->lane_id.u32;
    mipi_tx_print("LANE_ID: 0x%x\r\n", lane_id_reg.u32);
#endif
}

static void mipi_tx_hal_set_request_hsclk_en(uint8_t clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.txrequesthsclk = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_hal_set_clklane_continue_en(uint8_t clk_en)
{
    volatile reg_clklane_ctrl clk_lane_ctrl;

    clk_lane_ctrl.u32 = g_mipi_tx_regs_va->clklane_ctrl.u32;
    clk_lane_ctrl.bits.clklane_continue = clk_en;
    g_mipi_tx_regs_va->clklane_ctrl.u32 = clk_lane_ctrl.u32;
}

static void mipi_tx_drv_phy_reset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x0;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_phy_unreset(void)
{
    volatile reg_phy_ctrl phy_ctrl;

    phy_ctrl.u32 = g_mipi_tx_regs_va->phy_ctrl.u32;
    phy_ctrl.bits.rstz = 0x1;
    g_mipi_tx_regs_va->phy_ctrl.u32 = phy_ctrl.u32;
}

static void mipi_tx_drv_set_clkmgr_cfg(void)
{
    uint32_t actual_phy_data_rate;

    actual_phy_data_rate = mipi_tx_get_actual_phy_data_rate();
    /* set lp_clk frequence to (0, 20]MHz */
    if (actual_phy_data_rate / 160 < 2) { /* 160: half of 320, 2: half */
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x102; /* 258 */
    } else {
        g_mipi_tx_regs_va->crg_cfg.u32 = 0x100 +
            (actual_phy_data_rate + 160) / 160; /* align to 160 */
    }
#ifdef MIPI_TX_DEBUG
    mipi_tx_print("crg_cfg: 0x%x\r\n", g_mipi_tx_regs_va->crg_cfg.u32);
#endif
}

static void mipi_tx_drv_set_dpu2mipi_cmd(uint8_t cmd_en)
{
    volatile reg_command_tran_mode command_tran_mode;
    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    command_tran_mode.bits.dpu2mipi_cmd_en = cmd_en;
    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}
static void mipi_tx_drv_set_frmtail_gencmd(uint8_t frmtail_en)
{
    volatile reg_command_tran_mode command_tran_mode;
    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    command_tran_mode.bits.cfg_frmtail_gencmd = frmtail_en;
    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}

static void mipi_tx_drv_set_controller_cmd_tran_mode(uint8_t low_power_en)
{
    volatile reg_command_tran_mode command_tran_mode;

    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    command_tran_mode.bits.max_rd_pkt_size_tran = low_power_en;
    command_tran_mode.bits.gen_sw_0p = low_power_en;
    command_tran_mode.bits.gen_sw_1p = low_power_en;
    command_tran_mode.bits.gen_sw_2p = low_power_en;
    command_tran_mode.bits.gen_sr_0p = low_power_en;
    command_tran_mode.bits.gen_sr_1p = low_power_en;
    command_tran_mode.bits.gen_sr_2p = low_power_en;
    command_tran_mode.bits.gen_lw = low_power_en;
    command_tran_mode.bits.dcs_sw_0p = low_power_en;
    command_tran_mode.bits.dcs_sw_1p = low_power_en;
    command_tran_mode.bits.dcs_sr_0p = low_power_en;
    command_tran_mode.bits.dcs_lw = MIPI_TX_WORK_MODE_HS;
    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}

static void mipi_tx_drv_set_cmd_tran_mode(uint8_t low_power_en, uint16_t data_type)
{
    volatile reg_command_tran_mode command_tran_mode;

    command_tran_mode.u32 = g_mipi_tx_regs_va->command_tran_mode.u32;
    switch (data_type) {
        case MAX_READ_PKT_LEN:
            command_tran_mode.bits.max_rd_pkt_size_tran = low_power_en;
            break;
        case GEN_SHORT_WRITE_0P:
            command_tran_mode.bits.gen_sw_0p = low_power_en;
            break;
        case GEN_SHORT_WRITE_1P:
            command_tran_mode.bits.gen_sw_1p = low_power_en;
            break;
        case GEN_SHORT_WRITE_2P:
            command_tran_mode.bits.gen_sw_2p = low_power_en;
            break;
        case GEN_SHORT_READ_0P:
            command_tran_mode.bits.gen_sr_0p = low_power_en;
            break;
        case GEN_SHORT_READ_1P:
            command_tran_mode.bits.gen_sr_1p = low_power_en;
            break;
        case GEN_SHORT_READ_2P:
            command_tran_mode.bits.gen_sr_2p = low_power_en;
            break;
        case GEN_LONG_WRITE:
            command_tran_mode.bits.gen_lw = low_power_en;
            break;
        case DCS_SHORT_WRITE_0P:
            command_tran_mode.bits.dcs_sw_0p = low_power_en;
            break;
        case DCS_SHORT_WRITE_1P:
            command_tran_mode.bits.dcs_sw_1p = low_power_en;
            break;
        case DCS_SHORT_READ_0P:
            command_tran_mode.bits.dcs_sr_0p = low_power_en;
            break;
        case DCS_LONG_WRITE:
            command_tran_mode.bits.dcs_lw = low_power_en;
            break;
        default:
            mipi_tx_err("invalid data type!\r\n");
            break;
    }

    g_mipi_tx_regs_va->command_tran_mode.u32 = command_tran_mode.u32;
}
static void mipi_tx_drv_set_controller_cmd_datatype(const combo_dev_cfg_t *dev_cfg)
{
    UNUSED(dev_cfg);
    volatile reg_datatype0 datatype0;
    g_mipi_tx_regs_va->datatype0.u32 = 0x111213D;
    g_mipi_tx_regs_va->datatype1.u32 = 0x31081909;
    g_mipi_tx_regs_va->csi_ctrl.u32 = 0x10100;
}

static void mipi_tx_cal_actual_phy_data_rate(uint32_t input_data_rate)
{
    uint32_t data_rate;
    uint32_t actual_data_rate;
    data_rate = input_data_rate; /* data_rate * 1.2 for datatype, ECC, Checksum, lp~hs turn */

#ifdef PRE_FPGA
    actual_data_rate = input_data_rate;
#else
    actual_data_rate = mipi_tx_align_up(data_rate, MIPI_TX_REF_CLK);  /* 实际数据率要求参考时钟（MHz）向上对齐 */
#endif /* end of #ifdef PRE_FPGA */
    mipi_tx_set_actual_phy_data_rate(actual_data_rate);
}

static void mipi_tx_hal_set_controller_peck_en(const pck_en_t *pck_en_cfg)
{
    volatile reg_pck_en pck_en;
    /* crc,ecc,eotp tran */
    g_mipi_tx_regs_va->pck_en.u32 = 0x1c; /* dsi */

    pck_en.u32 = g_mipi_tx_regs_va->pck_en.u32;
    pck_en.bits.eotp_tx_en = pck_en_cfg->eotp_tx_en;
    pck_en.bits.eotp_rx_en = pck_en_cfg->eotp_rx_en;
    g_mipi_tx_regs_va->pck_en.u32 = pck_en.u32;
}

void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg)
{
    /* set 1st level int mask */
    g_mipi_tx_regs_va->gint_msk = 0x1ffff;

    /* OPERATION_MODE, disable input */
    mipi_tx_drv_set_input_enable(0);

    /* vc_id */
    g_mipi_tx_regs_va->video_vc.u32 = 0x0;

    /* output format */
    set_output_format(dev_cfg);

    /* txescclk,timeout */
    mipi_tx_cal_actual_phy_data_rate(dev_cfg->phy_data_rate);
    mipi_tx_drv_set_clkmgr_cfg();

    mipi_tx_drv_set_dpu2mipi_cmd(1);
    mipi_tx_drv_set_frmtail_gencmd(0);

    /* cmd transmission mode */
    mipi_tx_drv_set_controller_cmd_tran_mode(MIPI_TX_WORK_MODE_LP);

    /* crc,ecc,eotp tran */
    mipi_tx_hal_set_controller_peck_en(&(dev_cfg->pck_en));

    /* gen_vcid_rx */
    g_mipi_tx_regs_va->gen_vc.u32 = 0x0;

#ifdef MIPI_SUPPORT_VIDEO
    /* video mode cfg */
    set_video_mode_cfg(dev_cfg);
    if (dev_cfg->out_mode == OUT_MODE_DSI_VIDEO) {
        g_mipi_tx_regs_va->videom_pkt_size.u32 = dev_cfg->sync_info.hact_pixels;
    } else {
        g_mipi_tx_regs_va->command_pkt_size.u32 = dev_cfg->sync_info.hact_pixels;
    }

    /* num_chunks/null_size */
    g_mipi_tx_regs_va->videom_num_chunks.u32 = 0x0;
    g_mipi_tx_regs_va->videom_null_size.u32 = 0x0;
#endif

    /* timing config */
    set_timing_config(dev_cfg);

    /* invact,outvact time */
    g_mipi_tx_regs_va->lp_cmd_byte.u32 = 0x0;
    g_mipi_tx_regs_va->hs_lp_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->lprd_to_set.u32 = 0x0;
    g_mipi_tx_regs_va->hswr_to_set.u32 = 0x0;
    /* lp_wr_to_cnt */
    g_mipi_tx_regs_va->lpwr_to_set.u32 = 0x0;
    /* bta_to_cnt */
    g_mipi_tx_regs_va->bta_to_set.u32 = 0x0;

    /* set data type */
    mipi_tx_drv_set_controller_cmd_datatype(dev_cfg);

    /* lanes num and id */
    set_lane_config(dev_cfg->lane_id, LANE_MAX_NUM);

    /* phy_txrequlpsclk */
    g_mipi_tx_regs_va->ulps_ctrl.u32 = 0x0;

    /* tx triggers */
    g_mipi_tx_regs_va->tx_triggers.u32 = 0x0;

    g_mipi_tx_regs_va->vid_shadow_ctrl.u32 = 0x0;

    /* int0 mask disable */
    g_mipi_tx_regs_va->int0_mask.u32 = 0x0;

    /* clk continue enable */
    mipi_tx_hal_set_request_hsclk_en(1);
    mipi_tx_hal_set_clklane_continue_en(1);
}

static uint32_t mipi_tx_wait_cmd_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    uint32_t wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        uapi_tcxo_delay_us(1);

        if (wait_count > MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send cmd buffer \r\n");
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }
    } while (cmd_pkt_status.bits.command_empty == 0);

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_wait_write_fifo_empty(void)
{
    reg_command_status cmd_pkt_status;
    uint32_t wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;

        wait_count++;

        uapi_tcxo_delay_us(1);

        if (wait_count > MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when send data buffer \r\n");
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }
    } while (cmd_pkt_status.bits.pld_write_empty == 0);

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_wait_write_fifo_not_full(void)
{
    reg_command_status cmd_pkt_status;
    uint32_t wait_count;

    wait_count = 0;
    do {
        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
        if (wait_count > 0) {
            uapi_tcxo_delay_us(1);
            mipi_tx_err("write fifo full happened wait count = %d\r\n", wait_count);
        }
        if (wait_count > MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when wait write fifo not full buffer \r\n");
            return EXT_ERR_MIPI_FIFO_FULL;
        }
        wait_count++;
    } while (cmd_pkt_status.bits.pld_write_full == 1);

    return EXT_ERR_SUCCESS;
}

/*
 * set payloads data by writing register
 * each 4 bytes in cmd corresponds to one register
 */
static void mipi_tx_drv_set_payload_data(const uint8_t *cmd, uint16_t cmd_size)
{
    reg_command_payload gen_pld_data;
    uint32_t i, j;

    gen_pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;

    for (i = 0; i < (cmd_size / MIPI_TX_BYTES_PER_WORD); i++) {
        gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        gen_pld_data.bits.command_pld_b4 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4];

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

    gen_pld_data.u32 = 0;
    j = cmd_size % MIPI_TX_BYTES_PER_WORD;
    if (j != 0) {
        if (j > 0) {
            gen_pld_data.bits.command_pld_b1 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1];
        }
        if (j > 1) {
            gen_pld_data.bits.command_pld_b2 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2];
        }
        if (j > 2) { /* 2 bytes */
            gen_pld_data.bits.command_pld_b3 = cmd[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3];
        }

        mipi_tx_wait_write_fifo_not_full();
        g_mipi_tx_regs_va->command_payload.u32 = gen_pld_data.u32;
    }

#ifdef MIPI_TX_DEBUG
        mipi_tx_print("\r\n=====set cmd=======\r\n");
        mipi_tx_print("gen_pld_data(0x70): 0x%x\r\n", gen_pld_data.u32);
#endif
}

static void mipi_tx_drv_set_cmd_work_param(mipi_tx_work_mode_t work_mode, uint16_t data_type)
{
    reg_mode_cfg mode_cfg;
    mode_cfg.u32 = g_mipi_tx_regs_va->mode_cfg.u32;

    if (mode_cfg.bits.cmd_video_mode == 1) { /* command mode */
        /* set command work mode */
        mipi_tx_drv_set_cmd_tran_mode(work_mode, data_type);
    } else {
        /* set video work mode */
        mipi_tx_hal_set_video_mode_tran_type(work_mode);
    }
}

static void mipi_tx_drv_set_command_header(uint16_t data_type, uint16_t data_param)
{
    volatile reg_command_header gen_hdr;

    gen_hdr.u32 = g_mipi_tx_regs_va->command_header.u32;
    gen_hdr.bits.command_virtualchannel = 0; /* always 0 */
    gen_hdr.bits.command_datatype = data_type;
    gen_hdr.bits.command_wordcount_low = get_low_byte(data_param);
    gen_hdr.bits.command_wordcount_hi = get_high_byte(data_param);
    g_mipi_tx_regs_va->command_header.u32 = gen_hdr.u32;
#ifdef MIPI_TX_DEBUG
    mipi_tx_print("\r\n=====set cmd=======\r\n");
    mipi_tx_print("gen_hdr(0x6C): 0x%x\r\n", gen_hdr.u32);
#endif
}

uint32_t mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info)
{
    uint8_t *cmd = NULL;

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(cmd_info->work_mode, cmd_info->data_type);

    if (cmd_info->cmd != NULL) {
        cmd = (uint8_t *)osal_kmalloc(cmd_info->cmd_size, OSAL_GFP_KERNEL);
        if (cmd == NULL) {
            mipi_tx_err("malloc fail,please check,need %d bytes\r\n", cmd_info->cmd_size);
            return EXT_ERR_MIPI_MALLOC_FAIL;
        }

        if (memcpy_s(cmd, cmd_info->cmd_size, cmd_info->cmd, cmd_info->cmd_size) != EOK) {
            osal_kfree(cmd);
            return EXT_ERR_MIPI_MEMCPY_FAIL;
        }

        mipi_tx_drv_set_payload_data(cmd, cmd_info->cmd_size);

        osal_kfree(cmd);
    }

    mipi_tx_drv_set_command_header(cmd_info->data_type, cmd_info->cmd_size);

    /* wait transfer end */
    uapi_tcxo_delay_us(350); /* 350us */

    mipi_tx_wait_cmd_fifo_empty();
    mipi_tx_wait_write_fifo_empty();
    /* resume work mode and clk lane en */
    if ((cmd_info->data_type) == DCS_LONG_WRITE && (cmd_info->work_mode == MIPI_TX_WORK_MODE_LP)) {
        mipi_tx_drv_set_cmd_work_param(MIPI_TX_WORK_MODE_HS, DCS_LONG_WRITE);
    }

#ifdef MIPI_TX_DEBUG
    mipi_tx_print("cmd_info->data_type: 0x%x\r\n", cmd_info->data_type);
    mipi_tx_print("cmd_info->cmd_size: 0x%x\r\n", cmd_info->cmd_size);
#endif

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_wait_read_fifo_not_empty(void)
{
    reg_int0_status int_st0;
    reg_int1_status int_st1;
    uint32_t wait_count;
    reg_command_status cmd_pkt_status;

    wait_count = 0;
    do {
        int_st1.u32 =  g_mipi_tx_regs_va->int1_status.u32;
        int_st0.u32 =  g_mipi_tx_regs_va->int0_status.u32;

        /*
         * 0x0000101e:
         * to_lp_rx, rxecc_multi_err, rxecc_single_err, rxcrc_err, rxpkt_size_err
         */
        if ((int_st1.u32 & 0x0000101e) != 0) {
            mipi_tx_err("err happened when read data, int_st1 = 0x%x,int_st0 = %x\r\n", int_st1.u32, int_st0.u32);
            return EXT_ERR_MIPI_READ_FAILED;
        }

        if (wait_count > MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when read data\r\n");
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }

        wait_count++;

        uapi_tcxo_delay_us(1);

        cmd_pkt_status.u32 = g_mipi_tx_regs_va->command_status.u32;
    } while (cmd_pkt_status.bits.pld_read_empty == 0x1);

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_wait_read_fifo_empty(void)
{
    reg_command_payload pld_data;
    reg_int1_status int_st1;
    uint32_t wait_count;

    wait_count = 0;
    do {
        int_st1.u32 = g_mipi_tx_regs_va->int1_status.u32;
        if ((int_st1.bits.rxpld_rd_err) == 0x0) {
            pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        }
        wait_count++;
        uapi_tcxo_delay_us(1);
        if (wait_count > MIPI_TX_READ_TIMEOUT_CNT) {
            mipi_tx_err("timeout when clear cmd data buffer, the last read data is 0x%x\r\n", pld_data.u32);
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }
    } while ((int_st1.bits.rxpld_rd_err) == 0x0);

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_send_short_packet(uint16_t data_type, uint16_t data_param)
{
    mipi_tx_drv_set_command_header(data_type, data_param);

    if (mipi_tx_wait_cmd_fifo_empty() != EXT_ERR_SUCCESS) {
        return EXT_ERR_MIPI_FIFO_EMPTY;
    }

    return EXT_ERR_SUCCESS;
}

static uint32_t mipi_tx_get_read_fifo_data(uint32_t get_data_size, uint8_t *data_buf)
{
    reg_command_payload pld_data;
    uint32_t i, j;

    for (i = 0; i < get_data_size / MIPI_TX_BYTES_PER_WORD; i++) {
        if (mipi_tx_wait_read_fifo_not_empty() != EXT_ERR_SUCCESS) {
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE4] = pld_data.bits.command_pld_b4;
    }

    j = get_data_size % MIPI_TX_BYTES_PER_WORD;

    if (j != 0) {
        if (mipi_tx_wait_read_fifo_not_empty() != EXT_ERR_SUCCESS) {
            return EXT_ERR_MIPI_FIFO_EMPTY;
        }
        pld_data.u32 = g_mipi_tx_regs_va->command_payload.u32;
        if (j > 0) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE1] = pld_data.bits.command_pld_b1;
        }
        if (j > 1) {
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE2] = pld_data.bits.command_pld_b2;
        }
        if (j > 2) { /* 2 bytes */
            data_buf[i * MIPI_TX_BYTES_PER_WORD + MIPI_TX_CMD_BYTE3] = pld_data.bits.command_pld_b3;
        }
    }

    return EXT_ERR_SUCCESS;
}

void mipi_tx_reset(void)
{
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    mipi_tx_drv_phy_reset();
    uapi_tcxo_delay_us(1);

    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x1;
    mipi_tx_drv_phy_unreset();
    uapi_tcxo_delay_us(1);
}

uint32_t mipi_tx_drv_get_cmd_info(get_cmd_info_t *get_cmd_info)
{
    uint8_t* data_buf = NULL;
    uint32_t ret = EXT_ERR_SUCCESS;

    data_buf = (uint8_t*)osal_kmalloc(get_cmd_info->get_data_size, OSAL_GFP_KERNEL);
    if (data_buf == NULL) {
        return EXT_ERR_MIPI_MALLOC_FAIL;
    }

    if (mipi_tx_wait_read_fifo_empty() != EXT_ERR_SUCCESS) {
        ret = EXT_ERR_MIPI_FIFO_EMPTY;
        goto fail0;
    }

    /* set work mode and clk lane en */
    mipi_tx_drv_set_cmd_work_param(get_cmd_info->work_mode, get_cmd_info->data_type);

    if (mipi_tx_send_short_packet(get_cmd_info->data_type, get_cmd_info->data_param) != EXT_ERR_SUCCESS) {
        ret = EXT_ERR_MIPI_SEND_SHORT_PACKET;
        mipi_tx_err("mipi: short packet send error\r\n");
        goto fail0;
    }

    if (mipi_tx_get_read_fifo_data(get_cmd_info->get_data_size, data_buf) != EXT_ERR_SUCCESS) {
        /* fail will block mipi data lane ,so need reset */
        ret = EXT_ERR_MIPI_READ_FIFO_DATA;
        mipi_tx_err("mipi: fifo read error\r\n");
        mipi_tx_reset();
        goto fail0;
    }

    (void)memcpy_s(get_cmd_info->get_data, get_cmd_info->get_data_size, data_buf, get_cmd_info->get_data_size);

fail0:
    osal_kfree(data_buf);
    return ret;
}

void mipi_tx_drv_enable_input(const out_mode_t output_mode)
{
    if (output_mode == OUT_MODE_DSI_VIDEO) {
        mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    }

    /* enable input */
    mipi_tx_drv_set_input_enable(1);

    /* reset mipi_tx */
    mipi_tx_reset();
    mipi_tx_hal_set_request_hsclk_en(1);
    mipi_tx_hal_set_clklane_continue_en(1);
    uapi_tcxo_delay_us(1);

#if MIPI_TX_INTERRUPTS_ENABLE
    /* enable int */
    mipi_tx_enable_int(0x1);
#endif
}

void mipi_tx_drv_disable_input(void)
{
    volatile reg_operation_mode operation_mode;

    /* disable int */
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_enable_int(0x0);
#endif

    /* set to disable input */
    operation_mode.u32 = g_mipi_tx_regs_va->operation_mode.u32;
    operation_mode.bits.input_en = 0x0;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;

    /* set to command mode */
    mipi_tx_hal_set_cmd_video_mode(COMMAND_MODE);

    /* default: clklane disable */
    mipi_tx_hal_set_request_hsclk_en(0);
    mipi_tx_hal_set_clklane_continue_en(0);

    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    mipi_tx_drv_phy_reset();
}

void mipi_tx_hal_colorbar_mode(combo_dev_cfg_t *dev_cfg, colorbar_orien_mode orien, colorbar_input_mode mode)
{
    if (dev_cfg->out_mode == OUT_MODE_DSI_VIDEO) {
        mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    }

    /* enable colorbar */
    g_mipi_tx_regs_va->hfp_hact_in.u32 = (dev_cfg->sync_info.hfp_pixels << HAL_MIPI_HFP_OFFSET) +
                                         dev_cfg->sync_info.hact_pixels;
    g_mipi_tx_regs_va->hbp_hsa_in.u32 = (dev_cfg->sync_info.hbp_pixels << HAL_MIPI_HBP_OFFSET) +
                                        dev_cfg->sync_info.hsa_pixels;
    volatile reg_operation_mode operation_mode = {0};
    operation_mode.bits.read_empty_vsync_en = 1;
    operation_mode.bits.colorbar_en = 1;
    operation_mode.bits.colorbar_orien = orien;
    operation_mode.bits.colorbar_mode = mode;
    operation_mode.bits.cmd_colorbar_en = 1;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;

    /* reset mipi_tx */
    mipi_tx_reset();

    uapi_tcxo_delay_us(1);
    mipi_tx_hal_set_request_hsclk_en(1);
    mipi_tx_hal_set_clklane_continue_en(1);
    uapi_tcxo_delay_us(1);

#if MIPI_TX_INTERRUPTS_ENABLE
    /* enable int */
    mipi_tx_enable_int(0x1);
#endif
}

void mipi_tx_hal_unset_colorbar(const combo_dev_cfg_t *dev_cfg)
{
    /* disable int */
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_enable_int(0x0);
#endif

    if (dev_cfg->out_mode == OUT_MODE_DSI_VIDEO) {
        mipi_tx_hal_set_cmd_video_mode(VIDEO_MODE);
    }

    /* disable colorbar input */
    g_mipi_tx_regs_va->hfp_hact_in.u32 = 0;
    g_mipi_tx_regs_va->hbp_hsa_in.u32 = 0;

    volatile reg_operation_mode operation_mode = {0};
    operation_mode.bits.mem_ck_en = 1;
    operation_mode.bits.input_en = 1;
    operation_mode.bits.hss_abnormal_rst = 1;
    operation_mode.bits.read_empty_vsync_en = 1;
    g_mipi_tx_regs_va->operation_mode.u32 = operation_mode.u32;

    mipi_tx_hal_set_request_hsclk_en(0);
    mipi_tx_hal_set_clklane_continue_en(0);

    /* reset mipi_tx */
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    mipi_tx_drv_phy_reset();
}

void mipi_tx_drv_set_partial_refresh(combo_dev_cfg_t *dev_cfg)
{
    set_timing_config(dev_cfg);
}

void mipi_tx_hal_auto_enter_sdlp(void)
{
    volatile reg_mipitx_sdlp_cfg0 mipitx_sdlp_cfg0 = {0};

    mipitx_sdlp_cfg0.u32 = g_mipi_tx_regs_va->mipitx_sdlp_cfg0.u32;

#ifdef MIPI_ULPS_SUPPORT
    volatile reg_ppi_status mipitx_phy_status = {0};
    uint32_t wait_count = 0;

    mipitx_phy_status.u32 = g_mipi_tx_regs_va->ppi_status.u32;

    while ((mipitx_phy_status.bits.ulpsactivenotclk == 0x1) && (wait_count < MIPI_TX_EXIT_WAIT_ULPS_TIMEOUT_CNT)) {
        wait_count++;
        uapi_tcxo_delay_us(1);
    }

    if (wait_count == MIPI_TX_EXIT_WAIT_ULPS_TIMEOUT_CNT) {
        mipi_tx_err("timeout when wait mipi_tx into ulps, 0xb0 status is 0x%x\r\n", mipitx_phy_status.u32);
    }
#endif

    mipitx_sdlp_cfg0.bits.sdlp_enter_ovr_en = 0;
    g_mipi_tx_regs_va->mipitx_sdlp_cfg0.u32 = mipitx_sdlp_cfg0.u32;
}

uint32_t mipi_tx_hal_exit_sdlp(void)
{
    volatile reg_mipitx_sdlp_cfg0 mipitx_sdlp_cfg0 = {0};
    volatile reg_sdlp_dbg sdlp_dbg = {0};
    uint32_t wait_count;

    mipitx_sdlp_cfg0.u32 = g_mipi_tx_regs_va->mipitx_sdlp_cfg0.u32;
    sdlp_dbg.u32 = g_mipi_tx_regs_va->sdlp_dbg.u32;

    mipitx_sdlp_cfg0.bits.sdlp_enter_ovr_en = 1;
    g_mipi_tx_regs_va->mipitx_sdlp_cfg0.u32 = mipitx_sdlp_cfg0.u32;

    if (sdlp_dbg.bits.mipi_ready == 1) {
        return EXT_ERR_SUCCESS;
    }

    wait_count = 0;
    do {
        sdlp_dbg.u32 = g_mipi_tx_regs_va->sdlp_dbg.u32;

        wait_count++;
        uapi_tcxo_delay_us(1);
        if (wait_count > MIPI_TX_EXIT_SDLP_TIMEOUT_CNT) {
            mipitx_sdlp_cfg0.bits.sdlp_enter_ovr_en = 0;
            g_mipi_tx_regs_va->mipitx_sdlp_cfg0.u32 = mipitx_sdlp_cfg0.u32;
            mipi_tx_err("timeout when wait mipi_tx wake from sdlp, the status is 0x%x\r\n", sdlp_dbg.u32);
            return EXT_ERR_MIPI_EXIT_SDLP_TIMEOUT;
        }
    } while ((sdlp_dbg.bits.mipi_ready) == 0x0);

    return EXT_ERR_SUCCESS;
}

void mipi_tx_hal_get_lane_state(mipi_tx_lane_id lane_num, mipi_tx_lane_state_t *status)
{
    volatile reg_sdlp_dbg sdlp_dbg;
    volatile reg_ppi_status ppi_status;
    sdlp_dbg.u32 = g_mipi_tx_regs_va->sdlp_dbg.u32;
    ppi_status.u32 = g_mipi_tx_regs_va->ppi_status.u32;

    if (lane_num == MIPI_TX_LANE_DATA0) {
        status->idle_active = ppi_status.bits.stopstate0lane;
        status->exit_ulps = ppi_status.bits.ulpsactivenot0lane;
    } else if (lane_num == MIPI_TX_LANE_DATA1) {
        status->idle_active = ppi_status.bits.stopstate1lane;
        status->exit_ulps = ppi_status.bits.ulpsactivenot1lane;
    } else {
        status->idle_active = ppi_status.bits.stopstateclklane;
        status->exit_ulps = ppi_status.bits.ulpsactivenotclk;
    }
    status->exit_sdlp = sdlp_dbg.bits.mipi_ready;
}

uint32_t mipi_tx_hal_get_ulps_entry_count(void)
{
    volatile reg_sdlp_dbg sdlp_dbg = {0};
    if (g_mipi_tx_regs_va != NULL) {
        sdlp_dbg.u32 = g_mipi_tx_regs_va->sdlp_dbg.u32;
        return sdlp_dbg.bits.ulps_entry_cnt;
    }

    return 0;
}

uint32_t mipi_tx_hal_get_ulps_exit_count(void)
{
    volatile reg_sdlp_dbg sdlp_dbg = {0};
    if (g_mipi_tx_regs_va != NULL) {
        sdlp_dbg.u32 = g_mipi_tx_regs_va->sdlp_dbg.u32;
        return sdlp_dbg.bits.ulps_exit_cnt;
    }

    return 0;
}

void mipi_tx_hal_exit_ulps(void)
{
    volatile reg_auto_ulps_exit_pulse auto_ulps_exit_pulse;     /* 0xec */
    if (g_mipi_tx_regs_va != NULL) {
        auto_ulps_exit_pulse.u32 = g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32;
        auto_ulps_exit_pulse.bits.ulps_exit_pulse = 0x1;
        g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32 = auto_ulps_exit_pulse.u32;
    }
}

void mipi_tx_hal_clear_ulps_entry_cnt(void)
{
    volatile reg_auto_ulps_exit_pulse auto_ulps_exit_pulse;     /* 0xec */
    if (g_mipi_tx_regs_va != NULL) {
        auto_ulps_exit_pulse.u32 = g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32;
        auto_ulps_exit_pulse.bits.ulps_entry_cnt_clr = 0x1;
        g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32 = auto_ulps_exit_pulse.u32;
    }
}

void mipi_tx_hal_clear_exit_ulps(void)
{
    volatile reg_auto_ulps_exit_pulse auto_ulps_exit_pulse;     /* 0xec */
    if (g_mipi_tx_regs_va != NULL) {
        auto_ulps_exit_pulse.u32 = g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32;
        auto_ulps_exit_pulse.bits.ulps_exit_pulse_clr = 0x1;
        g_mipi_tx_regs_va->auto_ulps_exit_pulse.u32 = auto_ulps_exit_pulse.u32;
    }
}

void mipi_tx_hal_autoulps_en(uint8_t enable)
{
    volatile reg_auto_ulps_mode auto_ulps_mode;                 /* 0xe0 */
    volatile reg_auto_ulps_wakeup_time auto_ulps_wakeup_time;   /* 0xe8 */
    volatile reg_auto_ulps_min_time auto_ulps_min_time;         /* 0xf8 */
    if (g_mipi_tx_regs_va == NULL) {
        return;
    }

    auto_ulps_mode.u32 = g_mipi_tx_regs_va->auto_ulps_mode.u32;
    auto_ulps_wakeup_time.u32 = g_mipi_tx_regs_va->auto_ulps_wakeup_time.u32;
    auto_ulps_min_time.u32 = g_mipi_tx_regs_va->auto_ulps_min_time.u32;
    if (enable != 0) {
        auto_ulps_mode.bits.auto_ulps = 0x1; /* enable auto ulps */
        auto_ulps_mode.bits.ulps_exit_mux = 0x3; /* 0x3 : ulps counter can be controlled by software and logic */
        auto_ulps_mode.bits.pll_off_ulps = 0x1; /* enable ulps close phy pll */
        auto_ulps_wakeup_time.bits.twakeup_cnt = 0x2; /* 0x2: cycle count */
        auto_ulps_wakeup_time.bits.twakeup_clk_div = 0x5e00; /* 0x5e00: clk division of hsclk */
        auto_ulps_min_time.bits.auto_ulps_min_time = 0x200; /* 0x200 :minimum period to next ulps_exiting */
        g_mipi_tx_regs_va->auto_ulps_entry_delay = 0x200; /* 0x200 :enter ulps period */
        g_mipi_tx_regs_va->auto_ulps_wakeup_time.u32 = auto_ulps_wakeup_time.u32; /* exit ulps period */
        g_mipi_tx_regs_va->auto_ulps_min_time.u32 = auto_ulps_min_time.u32;
        g_mipi_tx_regs_va->auto_ulps_mode.u32 = auto_ulps_mode.u32;
    } else {
        mipi_tx_hal_exit_ulps();
        auto_ulps_mode.u32 = 0x20; /* 0x20 : disable auto ulps */
        g_mipi_tx_regs_va->auto_ulps_mode.u32 = auto_ulps_mode.u32;
        uapi_tcxo_delay_us(1500); /* 1500 : delay 1500us */
        mipi_tx_hal_clear_exit_ulps();
        mipi_tx_hal_clear_ulps_entry_cnt();
    }
}

static void mipi_tx_drv_reg_init(void)
{
    g_mipi_tx_regs_va = (mipi_tx_regs_type_t *)(uintptr_t)MIPI_TX_REGS_BASE_ADDR;
}

static void mipi_tx_drv_reg_exit(void)
{
    g_mipi_tx_regs_va = NULL;
}

#if MIPI_TX_INTERRUPTS_ENABLE
static void mipi_tx_enable_int(uint32_t enable)
{
    volatile reg_int1_mask int1_mask;
    volatile reg_int0_mask int0_mask;

    int0_mask.u32 = g_mipi_tx_regs_va->int0_mask.u32;
    int1_mask.u32 = g_mipi_tx_regs_va->int1_mask.u32;
    if (enable) {
        int0_mask.u32 = 0x001fffff;
        int1_mask.u32 = 0x0f067fff; /* mask b15, b16, b20 */
    } else {
        int0_mask.u32 = 0;
        int1_mask.u32 = 0;
    }
    g_mipi_tx_regs_va->int0_mask.u32 = int0_mask.u32;
    g_mipi_tx_regs_va->int1_mask.u32 = int1_mask.u32;
}

static void mipi_tx_clear_int(void)
{
    volatile reg_int1_status int1_stat;
    volatile reg_int0_status int0_stat;

    /* warning: read register to clear interrupt status, do not delete it. */
    int1_stat.u32 = g_mipi_tx_regs_va->int1_status.u32;
    int0_stat.u32 = g_mipi_tx_regs_va->int0_status.u32;

    /* ignore compile warning: variable 'int_stat' set but not used */
    (void)int1_stat.u32;
    (void)int0_stat.u32;
}

static void mipi_tx_interrupt_route(uint32_t arg)
{
    mipi_tx_clear_int();
    UNUSED(arg);
}

static uint32_t mipi_tx_register_irq(void)
{
    uint32_t ret;

    ret = osal_irq_request(g_mipi_tx_irq_num, (osal_irq_handler)mipi_tx_interrupt_route, NULL, NULL, NULL);
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("mipi irq request fail:%x\r\n", ret);
        return EXT_ERR_MIPI_IRQ_REQUEST_FAILED;
    }
    ret = osal_irq_enable(g_mipi_tx_irq_num);
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("mipi irq enable fail:%x\r\n", ret);
        ret = EXT_ERR_MIPI_IRQ_ENABLE_FAILED;
        goto fail;
    }

    return EXT_ERR_SUCCESS;

fail:
    osal_irq_free(g_mipi_tx_irq_num, NULL);
    return ret;
}

static void mipi_tx_unregister_irq(void)
{
    osal_irq_disable(g_mipi_tx_irq_num);
    osal_irq_free(g_mipi_tx_irq_num, NULL);
}
#endif

uint32_t mipi_tx_drv_init(void)
{
    uint32_t ret;

    mipi_tx_drv_reg_init();

#if MIPI_TX_INTERRUPTS_ENABLE
    ret = mipi_tx_register_irq();
    if (ret != EXT_ERR_SUCCESS) {
        mipi_tx_err("mipi_tx_register_irq fail!\r\n");
        mipi_tx_drv_reg_exit();
        return ret;
    }
#endif

    return EXT_ERR_SUCCESS;
}

void mipi_tx_drv_exit(void)
{
#if MIPI_TX_INTERRUPTS_ENABLE
    mipi_tx_unregister_irq();
#endif
    g_mipi_tx_regs_va->ctrl_reset.u32 = 0x0;
    mipi_tx_drv_phy_reset();

    mipi_tx_drv_reg_exit();
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif
