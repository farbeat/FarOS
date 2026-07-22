/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: mipi tx hal header file.
 * Author: @CompanyNameTag
 * Create: 2020-04-14
 */

#ifndef __MIPI_TX_HAL_H__
#define __MIPI_TX_HAL_H__

#include "soc_mipi_tx.h"
#include "chip_io.h"
#include "mipi_tx_reg.h"

#define HIWING_PHY
#define MIPI_TX_INTERRUPTS_ENABLE 0

#define MIPI_TX_REGS_BASE_ADDR   0x56000000
/* crg */
#define MIPI_TX_CRG              0x90091110

#define MIPI_TX_CRG2        (MIPI_TX_REGS_BASE_ADDR + 0x3FC)
#define CMU_CTL_BASE        0x0 /* todo */
#define CMU_CLK_MIPI_GT     (CMU_CTL_BASE + 0x0404)

/* mipi_tx interrupt number */
#define MIPI_TX_IRQ         21
/* mipi_tx phy ref_clk freq */
#define MIPI_TX_REF_CLK     32
/* mipi_tx dfx define */
#define MIPI_INT_DFX_NUM    2
#define MIPI_TX_INT0_STATUS (MIPI_TX_REGS_BASE_ADDR + 0x00BC)
#define MIPI_TX_INT1_STATUS (MIPI_TX_REGS_BASE_ADDR + 0x00C0)

/* mipi_tx max cmd length */
#define MIPI_TX_SET_DATA_SIZE 128
#define MIPI_TX_GET_DATA_SIZE 64

/* mipi_tx fifo define */
#define MIPI_TX_BYTES_PER_WORD 4
#define MIPI_TX_CMD_BYTE1 0
#define MIPI_TX_CMD_BYTE2 1
#define MIPI_TX_CMD_BYTE3 2
#define MIPI_TX_CMD_BYTE4 3

#ifdef PRE_FPGA

#if defined ALTERA_PHY
/* phy addr */
#define PLL_SET_0X14           0x14
#define PLL_SET_0X15           0x15
#define PLL_SET_0X16           0x16
#define PLL_SET_0X17           0x17
#define PLL_SET_0X1E           0x1E

#define DATA0_TPRE_DELAY    0x30
#define DATA1_TPRE_DELAY    0x40
#define DATA2_TPRE_DELAY    0x50
#define DATA3_TPRE_DELAY    0x60

/* clk */
#define CLK_POST_DELAY      0x21
#define CLK_TLPX            0x22
#define CLK_TCLK_PREPARE    0x23
#define CLK_TCLK_ZERO       0x24
#define CLK_TCLK_TRAIL      0x25

/* data0 */
#define DATA0_TLPX          0x32
#define DATA0_THS_PREPARE   0x33
#define DATA0_THS_ZERO      0x34
#define DATA0_THS_TRAIL     0x35

/* data1 */
#define DATA1_TLPX          0x42
#define DATA1_THS_PREPARE   0x43
#define DATA1_THS_ZERO      0x44
#define DATA1_THS_TRAIL     0x45

/* data2 */
#define DATA2_TLPX          0x52
#define DATA2_THS_PREPARE   0x53
#define DATA2_THS_ZERO      0x54
#define DATA2_THS_TRAIL     0x55

/* data3 */
#define DATA3_TLPX          0x62
#define DATA3_THS_PREPARE   0x63
#define DATA3_THS_ZERO      0x64
#define DATA3_THS_TRAIL     0x65

#define DATA0_TPRE_DELAY_VALUE 0xC
#define DATA1_TPRE_DELAY_VALUE 0xC
#define DATA2_TPRE_DELAY_VALUE 0xC
#define DATA3_TPRE_DELAY_VALUE 0xC

#define CLK_POST_DELAY_VALUE   0x0
#define CLK_TLPX_VALUE         0x8
#define CLK_TCLK_PREPARE_VALUE 0x7
#define CLK_TCLK_ZERO_VALUE    0x1C
#define CLK_TCLK_TRAIL_VALUE   0x8

#define DATA0_TLPX_VALUE        0x8
#define DATA0_THS_PREPARE_VALUE 0x7
#define DATA0_THS_ZERO_VALUE    0xC
#define DATA0_THS_TRAIL_VALUE   0x8

#define DATA1_TLPX_VALUE        0x8
#define DATA1_THS_PREPARE_VALUE 0x7
#define DATA1_THS_ZERO_VALUE    0xC
#define DATA1_THS_TRAIL_VALUE   0x8

#elif defined HIWING_PHY

/* timing parameter */
#define TLPX                80
#define TCLK_PREPARE        70
#define TCLK_ZERO           260
#define TCLK_TRAIL          80
#define TCLK_POST           95
#define TPRE_DELAY          120
#define THS_PREPARE         70
#define THS_ZERO            120
#define THS_TRAIL           80

#define PLL_CFG_D28_020     0x78
#define PLL_CFG_D20         0x3

#endif

#else

/* timing parameter */
#define TLPX                60
#define TCLK_PREPARE        70
#define TCLK_ZERO           260
#define TCLK_TRAIL          80
#define THS_PREPARE         70
#define THS_ZERO            120
#define THS_TRAIL           80

/* pll */
#define PLL_SET_CKG_SEL     (MIPI_TX_REGS_BASE_ADDR + 0x580)
#define PLL_SET_FBDIV       (MIPI_TX_REGS_BASE_ADDR + 0x590)
#define PLL_SET_FBDIV_PRDIV (MIPI_TX_REGS_BASE_ADDR + 0x594)

/* phy analog */
#define RG_LPTX_SELENFOR_SELFORHI (MIPI_TX_REGS_BASE_ADDR + 0x588)

/* clk */
#define CLK_TLPX            (MIPI_TX_REGS_BASE_ADDR + 0x440)
#define CLK_TCLK_PREPARE    (MIPI_TX_REGS_BASE_ADDR + 0x444)
#define CLK_TCLK_ZERO       (MIPI_TX_REGS_BASE_ADDR + 0x448)
#define CLK_TCLK_TRAIL      (MIPI_TX_REGS_BASE_ADDR + 0x44c)
#define CKG_EN_WAIT         (MIPI_TX_REGS_BASE_ADDR + 0x43C)
#define CLK_TCLK_PRE_DELAY  (MIPI_TX_REGS_BASE_ADDR + 0x45C)
#define CLK_TCLK_POST_DELAY (MIPI_TX_REGS_BASE_ADDR + 0x460)

/* data0 */
#define DATA0_TLPX          (MIPI_TX_REGS_BASE_ADDR + 0x480)
#define DATA0_THS_PREPARE   (MIPI_TX_REGS_BASE_ADDR + 0x484)
#define DATA0_THS_ZERO      (MIPI_TX_REGS_BASE_ADDR + 0x488)
#define DATA0_THS_TRAIL     (MIPI_TX_REGS_BASE_ADDR + 0x48C)
#define DATA0_TPRE_DELAY    (MIPI_TX_REGS_BASE_ADDR + 0x4A0)

/* data1 */
#define DATA1_TLPX          (MIPI_TX_REGS_BASE_ADDR + 0x4C0)
#define DATA1_THS_PREPARE   (MIPI_TX_REGS_BASE_ADDR + 0x4C4)
#define DATA1_THS_ZERO      (MIPI_TX_REGS_BASE_ADDR + 0x4C8)
#define DATA1_THS_TRAIL     (MIPI_TX_REGS_BASE_ADDR + 0x4CC)
#define DATA1_TPRE_DELAY    (MIPI_TX_REGS_BASE_ADDR + 0x4E0)

#define SET_CKG_EN_WAIT_VALUE     16

#define BIT_RG_LPTX_SELENFOR      4
#define BIT_RG_LPTX_SELFORHI      5

#endif /* end of #ifdef PRE_FPGA */

#define MIPI_TX_READ_TIMEOUT_CNT 30000
#define MIPI_TX_MAX_RESET_PHY_TIMES 10
#define MIPI_TX_EXIT_SDLP_TIMEOUT_CNT 200
#define MIPI_TX_EXIT_WAIT_ULPS_TIMEOUT_CNT 100

#define PREPARE_COMPENSATE    10
#define ROUNDUP_VALUE     7999
#define INNER_PEROID    8000   /* 8 * 1000 ,1000 is 1us = 1000ns, 8 is division ratio */

#define mipi_tx_align_up(x, a) ((((x) + ((a) - 1)) / (a)) * (a))
#define mipi_tx_ceil(x, a)     (((x) + (a)-1) / (a))

#define MIPI_TX_BITS_PER_BYTE 8
#define MIPI_TX_PHY_READY_TIMEOUT_CNT 40

/*
 * global operation timing parameters
 */
typedef struct {
    uint8_t data_tpre_delay;
    uint8_t clk_post_delay;
    uint8_t clk_tlpx;
    uint8_t clk_tclk_prepare;
    uint8_t clk_tclk_zero;
    uint8_t clk_tclk_trail;
    uint8_t data_tlpx;
    uint8_t data_ths_prepare;
    uint8_t data_ths_zero;
    uint8_t data_ths_trail;
} mipi_tx_phy_timing_parameters;

typedef struct {
    uint32_t vall_det;
    uint32_t vact_det;
    uint32_t hall_det;
    uint32_t hact_det;
    uint32_t hbp_det;
    uint32_t hsa_det;
    uint32_t vsa_det;
    uint32_t vall_ppi;
    uint32_t vact_ppi;
    uint32_t hact_ppi;
    uint32_t lane_num;
} mipi_tx_dev_phy_t;

typedef struct {
    uint32_t idle_active;
    uint32_t exit_ulps;
    uint32_t exit_sdlp;
} mipi_tx_lane_state_t;

typedef enum {
    INPUT_COLOUR_MODE = 0,
    INPUT_BLACK_MODE = 1,
    INPUT_ALTERNATING_MODE = 2,

    INPUT_MODE_BUTT
} colorbar_input_mode;

typedef enum {
    ORIEN_HORIZONTAL_MODE = 0,
    ORIEN_VERTICAL_MODE = 1,

    ORIEN_MODE_BUTT
} colorbar_orien_mode;

typedef enum {
    MIPI_TX_LANE_DATA0,
    MIPI_TX_LANE_DATA1,
    MIPI_TX_LANE_CLK,
} mipi_tx_lane_id;

__inline static uint8_t get_low_byte(uint16_t short_data)
{
    return (uint8_t)(short_data & 0xff);
}

__inline static uint8_t get_high_byte(uint16_t short_data)
{
    return (uint8_t)((short_data & 0xff00) >> MIPI_TX_BITS_PER_BYTE);
}

void mipi_tx_drv_set_irq_num(uint32_t irq_num);
void mipi_tx_drv_set_regs(mipi_tx_regs_type_t *regs);

void mipi_tx_drv_set_phy_cfg(const combo_dev_cfg_t *dev_cfg);
void mipi_tx_drv_get_dev_status(mipi_tx_dev_phy_t *mipi_tx_phy_ctx);
void mipi_tx_drv_set_controller_cfg(const combo_dev_cfg_t *dev_cfg);

uint32_t mipi_tx_drv_set_cmd_info(const cmd_info_t *cmd_info);

uint32_t mipi_tx_drv_get_cmd_info(get_cmd_info_t *get_cmd_info);
void mipi_tx_drv_enable_input(const out_mode_t output_mode);
void mipi_tx_drv_disable_input(void);

uint32_t mipi_tx_drv_init(void);
void mipi_tx_drv_exit(void);
void mipi_tx_hal_colorbar_mode(combo_dev_cfg_t *dev_cfg, colorbar_orien_mode orien, colorbar_input_mode mode);
void mipi_tx_hal_unset_colorbar(const combo_dev_cfg_t *dev_cfg);

void mipi_tx_drv_get_int_status(uint32_t *int_dfx_info, uint32_t info_len);

uint32_t mipi_tx_get_actual_phy_data_rate(void);

void mipi_tx_hal_auto_enter_sdlp(void);

uint32_t mipi_tx_hal_exit_sdlp(void);

uint32_t mipi_tx_hal_get_ulps_entry_count(void);

uint32_t mipi_tx_hal_get_ulps_exit_count(void);

void mipi_tx_hal_exit_ulps(void);

void mipi_tx_hal_clear_ulps_entry_cnt(void);

void mipi_tx_hal_clear_exit_ulps(void);

void mipi_tx_hal_autoulps_en(uint8_t enable);

void mipi_tx_hal_get_lane_state(mipi_tx_lane_id lane_num, mipi_tx_lane_state_t *status);

#endif /* end of #ifndef __MIPI_TX_HAL_H__ */
