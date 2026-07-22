/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides psram driver source \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-02-07, Create file. \n
 */
#include "string.h"
#include "securec.h"
#include "lib_utils.h"
#include "tcxo.h"
#include "panic.h"
#include "pinctrl_porting.h"
#include "pinctrl.h"
#include "hal_xip.h"
#include "chip_io.h"
#include "gpio.h"
#ifdef USE_CMSIS_OS
#include "pmu_cmu.h"
#endif
#include "debug_print.h"
#include "arch_barrier.h"
#include "hal_cpu_clocks.h"
#include "hal_spi.h"
#include "platform_core.h"
#include "clocks_switch.h"
#include "spi_porting.h"
#include "opi.h"
#include "soc_osal.h"
#include "psram_drv.h"


/*------opi paramter -----------------*/
#define OPI_TRANSFER_MAX_TIME_MS 100
#define OPI_CLK_166_MHZ          16
#define OPI_CLK_DIV_10           10

#define OPI_FIFO_DEEP             0x20
#define OPI_FIFO_THRESHOLD_TX     0x1C
#define OPI_FIFO_THRESHOLD_RX     0x1C
#define OPI_FIFO_THRESHOLD_RX_DMA 0x1

#define OPI_SLAVE_NUMBER 0

// PLL clock div: 8
#define OPI_PLL_POSTDIV1_NUM            0x4
#define OPI_PLL_POSTDIV2_NUM            0x2

#define OPI_NON_ALIGNED_ACCESS_REG      (XIP_CACHE_CTL + 0x1C)
#define OPI_NON_ALIGNED_ACCESS_EN       1

/*------xip paramter -------*/
#define XIP_OPI_ENABLE      1
#define XIP_OPI_DISABLE     0
#define XIP_CACHE_OVER_TIME 0x1000
#define XIP_CONFIG_SYNC     1

/*------OPI lpc config reg--------*/
#define OPI_CTRL0_REG             (SPI_BUS_6_BASE_ADDR + 0x00)
#define OPI_SSIENR_REG            (SPI_BUS_6_BASE_ADDR + 0x08)
#define OPI_BAUDRATE_REG          (SPI_BUS_6_BASE_ADDR + 0x14)
#define OPI_TX_FIFO_LR_REG        (SPI_BUS_6_BASE_ADDR + 0x18)
#define OPI_SPI_CTRL_LR0_REG      (SPI_BUS_6_BASE_ADDR + 0xF4)

#define OPI_XIP_WRITE_FIFO_EMPTY_OFFSET   3

#define opi_get_array_element(array, data_type) (sizeof(array) / sizeof(data_type))

typedef enum {
    OPI_FRAME_FORMAT_STD = 0,
    OPI_FRAME_FORMAT_DUAL = 1,
    OPI_FRAME_FORMAT_QUAD = 2,
    OPI_FRAME_FORMAT_OCTAL = 3,
    OPI_FRAME_FORMAT_DOUBLE_OCTAL = 4,
    OPI_FRAME_FORMAT_SIXT = 5,
    OPI_FRAME_FORMAT_MAX_NUM,
    OPI_FRAME_FORMAT_NONE = OPI_FRAME_FORMAT_MAX_NUM,
} opi_frame_format_e;

typedef enum {
    OPI_INST_0BIT = 0,
    OPI_INST_4BIT = 1,
    OPI_INST_8BIT = 2,
    OPI_INST_16BIT = 3,
    OPI_INST_MAX_NUM = 4,
    OPI_INST_NONE = OPI_INST_MAX_NUM,
} opi_inst_len_e;

typedef enum {
    OPI_ADDR_0BIT = 0,
    OPI_ADDR_4BIT = 1,
    OPI_ADDR_8BIT = 2,
    OPI_ADDR_16BIT = 4,
    OPI_ADDR_24BIT = 6,
    OPI_ADDR_32BIT = 8,
} opi_addr_len_e;

typedef enum {
    OPI_FRAME_SIZE_8BIT = HAL_SPI_FRAME_SIZE_8,
    OPI_FRAME_SIZE_16BIT = HAL_SPI_FRAME_SIZE_16,
    OPI_FRAME_SIZE_32BIT = HAL_SPI_FRAME_SIZE_32,
    OPI_FRAME_SIZE_MAX_NUM,
    OPI_FRAME_SIZE_NONE = OPI_FRAME_SIZE_MAX_NUM,
} opi_frame_size_e;

typedef enum {
    OPI_TRANS_TYPE_0 = 0, // Instruction and Address will be sent in Standard OPI Mode.
    OPI_TRANS_TYPE_1 = 1, // Instruction send to standard OPI model, addr will be sent to OPI_INTERFACE specified model
    OPI_TRANS_TYPE_2 = 2, // Both Instruction and Address will be sent in the mode specified by OPI_INTERFACE.
    OPI_TRANS_TYPE_MAX_NUM,
    OPI_TRANS_TYPE_NONE = OPI_TRANS_TYPE_MAX_NUM,
} opi_trans_type_e;

typedef enum {
    OPI_TRANS_MODE_TX_RX = 0,
    OPI_TRANS_MODE_TX = 1,
    OPI_TRANS_MODE_RX = 2,
    OPI_TRANS_MODE_EEPROM,
    OPI_TRANS_MODE_MAX_NUM,
    OPI_TRANS_MODE_NONE = OPI_TRANS_MODE_MAX_NUM,
} opi_trans_mode_e;

typedef struct {
    opi_frame_format_e frame_format;
    opi_inst_len_e inst_len;
    opi_addr_len_e addr_len;
    opi_frame_size_e frame_size;
    opi_trans_type_e trans_type;
    opi_trans_mode_e trans_mode;
    uint8_t clk_mhz;
    uint32_t frame_number;
} opi_config_t;

typedef struct {
    uint32_t ssienr_value;
    uint32_t ctrl_rl0;
    uint32_t baudrate;
    uint32_t tx_fifo_lr;
    uint32_t spi_ctrlr0;
} opi_pm_config_t;

static opi_pm_config_t opi_pm_initial_value = {
    .ssienr_value = 0,
    .ctrl_rl0 = 0x1070000,
    .baudrate = 0,
    .tx_fifo_lr = 0,
    .spi_ctrlr0 = 0x200,
};
static opi_pm_config_t opi_pm_storage_value;

static volatile bool g_opi_sleeped = false;

/*---------------------------------psram---------------------------------*/
typedef struct {
    uint32_t reg_to_write;
    uint32_t reg_to_read;
    uint32_t mem_write;
    uint32_t mem_read;
    uint32_t reset;
    uint32_t en_half_sleep;
    uint32_t exit_half_sleep;
} psram_cmd_t;

typedef struct {
    uint32_t addr;
    uint32_t vaule;
} paramter_info_t;

typedef struct {
    opi_frame_format_e frame_format;
    psram_cmd_t *cmd;
    uint8_t psram_param_num;
    paramter_info_t *psram_param;
    uint8_t xip_param_num;
    paramter_info_t *xip_param;
} psram_parameter_t;

#define PSRAM_MEM_ADDR_TRANS_LEN_OFFSET_BYTE   4

/*------psram cmd -------*/
#define PSRAM_CMD_NONE 0x5A5A
// ap
#define PSRAM_CMD_AP_X16_RESET     0xFFFF
#define PSRAM_CMD_AP_X16_WRITE_REG 0xC0C0
#define PSRAM_CMD_AP_X16_READ_REG  0x4040
#define PSRAM_CMD_AP_X16_WRITE_MEM 0x8080
#define PSRAM_CMD_AP_X16_READ_MEM  0x0000
// winbon
#define PSRAM_CMD_WINBON_X8_WRITE_REG 0x6000
#define PSRAM_CMD_WINBON_X8_READ_REG  0xE000
#define PSRAM_CMD_WINBON_X8_WRITE_MEM 0x2000
#define PSRAM_CMD_WINBON_X8_READ_MEM  0xA000

/*------psram addr-------*/
// ap
#define PSRAM_REG_AP_X16_MR0 0
#define PSRAM_REG_AP_X16_MR4 4
#define PSRAM_REG_AP_X16_MR6 6
#define PSRAM_REG_AP_X16_MR8 8

// winbon
#define PSRAM_REG_WINBON_X8_CR0 0x01000000
#define PSRAM_REG_WINBON_X8_CR1 0x01000001

/*------psram addr value-------*/
// ap
#define PSRAM_AP_X16_MR0_READ_LATENCY_166MHZ  0x0D
#define PSRAM_AP_X16_MR0_RL_CODE_MASK         0x1C
#define PSRAM_AP_X16_MR4_WRITE_LATENCY_166MHZ 0xD8 // 0.5x self refresh
#define PSRAM_AP_X16_MR6_ENTER_HALF_SLEEP     0xF0
#define PSRAM_AP_X16_MR8_EN_X16_2K_WRAP       0x47
#define PSRAM_AP_X8_MR8_EN_X8_2K_WRAP         0x7
// winbon
#define PSRAM_WINBON_X8_CR0_INITIAL_LATENCY_200MHZ 0x8F24
#define PSRAM_WINBON_X8_CR1_ENTER_HYBIRD_SLEEP     BIT(5)

/*-----psram xip paramter -----*/
// AP X16
#define PSRAM_AP_X16_REG_WRITE_DATA_SHIFT 8
#define XIP_WRITE_REDUNDANT_DATA_NUM_AP_X16_166MHZ    5
#define XIP_WRITE_REDUNDANT_DATA_NUM_WINBON_X8_200MHZ 0x103
#define XIP_WRITE_CMD_L_WINBON_X8_200MHZ              1
#define XIP_READ_CMD_L_WINBON_X8_200MHZ               5


/*------psram paramter ----------------*/
#define PSRAM_CMD_FRAMSIZE             1
#define PSRAM_ADDR_FRAMSIZE            1
#define PSRAM_REG_RW_TRANSFER_DATA_LEN 1
#define PSRAM_MEMORY_ADDR_REGION       0x0C000000
#define PSRAM_REDUNDANT_DATA           0xFFFFFFFF
#define PSRAM_CE_PULL_UP_WAIT_MIN_TIME 150ULL
#define PSRAM_MAX_WAKE_TIME            3

#define PSRAM_OPI_DELAY_US_10          10
#define PSRAM_EID_SECOND_PARAM_OFFSET  70
#define PSRAM_EID_LENGTH               8

// PSRAM_50MHZ Config, PLL:200M
#define PSRAM_50M_FBDIV_NUM             0x19
#define PSRAM_50M_FRAC_H_NUM            0x0
#define PSRAM_50M_FRAC_L_NUM            0x0

// PSRAM_75MHZ Config, PLL:300M
#define PSRAM_75M_FBDIV_NUM             0x25
#define PSRAM_75M_FRAC_H_NUM            0x80
#define PSRAM_75M_FRAC_L_NUM            0x0

// PSRAM_100MHZ Config, PLL:400M
#define PSRAM_100M_FBDIV_NUM            0x32
#define PSRAM_100M_FRAC_H_NUM           0x0
#define PSRAM_100M_FRAC_L_NUM           0x0

// PSRAM_133MHZ Config, PLL:532M
#define PSRAM_133M_FBDIV_NUM            0x42
#define PSRAM_133M_FRAC_H_NUM           0x80
#define PSRAM_133M_FRAC_L_NUM           0x0

// PSRAM_157MHZ Config, PLL:628M
#define PSRAM_157M_FBDIV_NUM            0x4E
#define PSRAM_157M_FRAC_H_NUM           0x07
#define PSRAM_157M_FRAC_L_NUM           0x1F2D

// PSRAM_159MHZ Config, PLL:198.796M
#define PSRAM_159M_FBDIV_NUM            0x4F
#define PSRAM_159M_FRAC_H_NUM           0x05
#define PSRAM_159M_FRAC_L_NUM           0x94FA

// PSRAM_162MHz Config, PLL:636M
#define PSRAM_162M_FBDIV_NUM            0x4f
#define PSRAM_162M_FRAC_H_NUM           0x80
#define PSRAM_162M_FRAC_L_NUM           0x0

// PSRAM_166MHz Config, PLL:664M
#define PSRAM_166M_FBDIV_NUM            0x53
#define PSRAM_166M_FRAC_H_NUM           0x0
#define PSRAM_166M_FRAC_L_NUM           0x0

// PSRAM_168MHZ Config, PLL:672M
#define PSRAM_168M_FBDIV_NUM            0x54
#define PSRAM_168M_FRAC_H_NUM           0x0
#define PSRAM_168M_FRAC_L_NUM           0x0

// PSRAM_169MHz Config, PLL:676M
#define PSRAM_169M_FBDIV_NUM            0x54
#define PSRAM_169M_FRAC_H_NUM           0x80
#define PSRAM_169M_FRAC_L_NUM           0x0

// PSRAM_170MHZ Config, PLL:680
#define PSRAM_170M_FBDIV_NUM            0x55
#define PSRAM_170M_FRAC_H_NUM           0x0
#define PSRAM_170M_FRAC_L_NUM           0x0

// PSRAM_171MHz Config, PLL:684M
#define PSRAM_171M_FBDIV_NUM            0x55
#define PSRAM_171M_FRAC_H_NUM           0x80
#define PSRAM_171M_FRAC_L_NUM           0x0

/*----- psram cmd -----*/
static psram_cmd_t g_psram_ap_x16_cmd = {
    PSRAM_CMD_AP_X16_WRITE_REG,
    PSRAM_CMD_AP_X16_READ_REG,
    PSRAM_CMD_AP_X16_WRITE_MEM,
    PSRAM_CMD_AP_X16_READ_MEM,
    PSRAM_CMD_AP_X16_RESET,
    PSRAM_CMD_AP_X16_WRITE_REG,
    PSRAM_CMD_NONE,
};

static psram_cmd_t g_psram_winbon_x8_cmd = {
    PSRAM_CMD_WINBON_X8_WRITE_REG,
    PSRAM_CMD_WINBON_X8_READ_REG,
    PSRAM_CMD_WINBON_X8_WRITE_MEM,
    PSRAM_CMD_WINBON_X8_READ_MEM,
    PSRAM_CMD_NONE,
    PSRAM_CMD_NONE,
    PSRAM_CMD_NONE,
};

#if OPI_USE_MCU_HS_CLK == NO
static cmu_pll_config_t g_psram_pll_cfg[] = {
    {PSRAM_50M_FBDIV_NUM, PSRAM_50M_FRAC_H_NUM, PSRAM_50M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_75M_FBDIV_NUM, PSRAM_75M_FRAC_H_NUM, PSRAM_75M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_100M_FBDIV_NUM, PSRAM_100M_FRAC_H_NUM, PSRAM_100M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_133M_FBDIV_NUM, PSRAM_133M_FRAC_H_NUM, PSRAM_133M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_157M_FBDIV_NUM, PSRAM_157M_FRAC_H_NUM, PSRAM_157M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_159M_FBDIV_NUM, PSRAM_159M_FRAC_H_NUM, PSRAM_159M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_162M_FBDIV_NUM, PSRAM_162M_FRAC_H_NUM, PSRAM_162M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_166M_FBDIV_NUM, PSRAM_166M_FRAC_H_NUM, PSRAM_166M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_168M_FBDIV_NUM, PSRAM_168M_FRAC_H_NUM, PSRAM_168M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_169M_FBDIV_NUM, PSRAM_169M_FRAC_H_NUM, PSRAM_169M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_170M_FBDIV_NUM, PSRAM_170M_FRAC_H_NUM, PSRAM_170M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
    {PSRAM_171M_FBDIV_NUM, PSRAM_171M_FRAC_H_NUM, PSRAM_171M_FRAC_L_NUM, OPI_PLL_POSTDIV1_NUM, OPI_PLL_POSTDIV2_NUM},
};
#else
static clocks_clk_cfg_t g_psram_clk_cfg[] = {
    { CLOCKS_CLK_SRC_FNPLL_MCU_HS, CLOCK_DIV_1},
    { CLOCKS_CLK_SRC_FNPLL_MCU_HS, CLOCK_DIV_2},
    { CLOCKS_CLK_SRC_FNPLL_AUDIO, CLOCK_DIV_2},
    { CLOCKS_CLK_SRC_FNPLL_AUDIO, CLOCK_DIV_3},
};
#endif

/*----- psram addr information -----*/
#define PSRAM_PARAM_ARRAY_INDEX_DRIVER_STRENGTH 0
#define XIP_PARAMTER_ARRAY_INDEX_DEVICE         0

#define PSRAM_PARAMTER_DRIVER_STRENGTH_POS_WINBON_X8 12
#define PSRAM_PARAMTER_DRIVER_STRENGTH_POS_AP_X16    0
#define PSRAM_DEVICE_MODE_CLEAR                      0
// ap
static paramter_info_t g_psram_parm_ap_x16[] = {
    {PSRAM_REG_AP_X16_MR0, PSRAM_AP_X16_MR0_READ_LATENCY_166MHZ}, // MRO[1:0]: driver strength, MRO[4:2]: read laterncy
    {PSRAM_REG_AP_X16_MR4, PSRAM_AP_X16_MR4_WRITE_LATENCY_166MHZ},
    {PSRAM_REG_AP_X16_MR8, PSRAM_AP_X16_MR8_EN_X16_2K_WRAP},
};

// winbon
static paramter_info_t g_psram_parm_winbon_x8[] = {
    {PSRAM_REG_WINBON_X8_CR0, PSRAM_WINBON_X8_CR0_INITIAL_LATENCY_200MHZ}, // CR0[14:12]: driver strength
};

static paramter_info_t g_psram_parm_ap_x8[] = {
    {PSRAM_REG_AP_X16_MR8, PSRAM_AP_X8_MR8_EN_X8_2K_WRAP},
};

/*----- xip paramter -----*/
// ap
static paramter_info_t g_xip_parm_ap_x16[] = {
    {WRITE_REDUNDANT_CNT_REG, XIP_WRITE_REDUNDANT_DATA_NUM_AP_X16_166MHZ},
};

// winbon
static paramter_info_t g_xip_parm_winbon_x8[] = {
    {WRITE_REDUNDANT_CNT_REG, XIP_WRITE_REDUNDANT_DATA_NUM_WINBON_X8_200MHZ},
    {CFG_XIP_WRITE_PSRAM_CMD_L_REG, XIP_WRITE_CMD_L_WINBON_X8_200MHZ},
    {CFG_XIP_READ_PSRAM_CMD_L_REG, XIP_READ_CMD_L_WINBON_X8_200MHZ},
};

static paramter_info_t g_xip_parm_ap_x8[] = {
    {WRITE_REDUNDANT_CNT_REG, 2},
};

/*--- device config ----*/
#ifdef PSRAM_TEST_8LINE
volatile psram_device_e g_psram_device = PSRAM_DEVICE_AP_X8;
#else
volatile psram_device_e g_psram_device = PSRAM_DEVICE_AP_X16;
#endif

static volatile bool g_psram_sleeped = false;

static const psram_parameter_t g_psram_device_parm[] = {
    // PSRAM_DEVICE_AP_X16
    { OPI_FRAME_FORMAT_SIXT, &g_psram_ap_x16_cmd,
      opi_get_array_element(g_psram_parm_ap_x16, paramter_info_t), g_psram_parm_ap_x16,
      opi_get_array_element(g_xip_parm_ap_x16, paramter_info_t), g_xip_parm_ap_x16, },
    // PSRAM_DEVICE_WINBON_X8
    { OPI_FRAME_FORMAT_OCTAL, &g_psram_winbon_x8_cmd,
      opi_get_array_element(g_psram_parm_winbon_x8, paramter_info_t), g_psram_parm_winbon_x8,
      opi_get_array_element(g_xip_parm_winbon_x8, paramter_info_t), g_xip_parm_winbon_x8, },
    // PSRAM_DEVICE_AP_X8
    { OPI_FRAME_FORMAT_OCTAL, &g_psram_ap_x16_cmd,
      opi_get_array_element(g_psram_parm_ap_x8, paramter_info_t), g_psram_parm_ap_x8,
      opi_get_array_element(g_xip_parm_ap_x8, paramter_info_t), g_xip_parm_ap_x8, },
};

static void test_opi_config_check(void)
{
#ifdef OPI_DEBUG
    PRINT("OPI ssienr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x08));
    PRINT("OPI ctrl_rl0: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x00));
    PRINT("OPI ctrl_rl1: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x04));
    PRINT("OPI baudr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x14));
    PRINT("OPI txftlr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x18));
    PRINT("OPI rxftlr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x1C));
    PRINT("OPI dmacr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x4C));
    PRINT("OPI dmatdlr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x50));
    PRINT("OPI dmardlr: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0x54));
    PRINT("OPI spi_ctrlr0: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0xF4));
    PRINT("OPI rsvd: 0x%x. \r\n", reg32(SPI_BUS_6_BASE_ADDR + 0xFC));
#endif
}

static void opi_pinmux(void)
{
#if defined(BUILD_APPLICATION_SSB)
    for (uint32_t i = 0; i < (L_MGPIO19 - L_MGPIO0 + 1); i++) {
        uapi_pin_set_mode((pin_t)((uint32_t)L_MGPIO0 + i), (pin_mode_t)HAL_PIO_FUNC_OPI);
        uapi_pin_set_ds((pin_t)((uint32_t)L_MGPIO0 + i), (pin_drive_strength_t)HAL_PIO_DRIVE_0);
    }
#endif
}

static opi_ret_e uapi_opi_trans_wait(void)
{
    uint64_t start_time = uapi_tcxo_get_ms();
    uint32_t check_times = 0;
    spi_bus_t bus = OPI_BUS;

    while (!hal_opi_tx_fifo_is_empty(bus) || hal_opi_rx_fifo_is_not_empty(bus) || hal_opi_is_busy(bus)) {
        if (lib_timeout_check_timeout_periodic((uint64_t)OPI_TRANSFER_MAX_TIME_MS, start_time, &check_times)) {
            return OPI_RET_TIMEOUT;
        }
    }

    return OPI_RET_OK;
}

static opi_ret_e uapi_opi_open(opi_config_t *opi_config)
{
    if (opi_config == NULL) {
        return OPI_RET_WRONG_PARA;
    }
    if (uapi_psram_xip_enabled()) {
        return OPI_RET_XIP_EN;
    }
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return OPI_RET_TIMEOUT;
    }

    uint32_t irq_sts = osal_irq_lock();
    // opi config
    hal_opi_disable(OPI_BUS);
    hal_opi_reset_config(OPI_BUS);
    hal_opi_set_frame_size(OPI_BUS, opi_config->frame_size);
    hal_opi_set_frame_format(OPI_BUS, (hal_spi_frame_format_t)(opi_config->frame_format));
    hal_opi_set_trans_mode(OPI_BUS, opi_config->trans_mode);
    hal_opi_set_freq(OPI_BUS, opi_config->clk_mhz);
    hal_opi_set_inst_len(OPI_BUS, (hal_spi_inst_len_t)(opi_config->inst_len));
    hal_opi_set_addr_len(OPI_BUS, (hal_spi_addr_len_t)(opi_config->addr_len));
    hal_opi_cmd_trans_mode(OPI_BUS, (uint32_t)(opi_config->trans_type));
    hal_opi_set_received_data_num(OPI_BUS, opi_config->frame_number);
    hal_opi_set_tx_fifo_threshold(OPI_BUS, OPI_FIFO_THRESHOLD_TX);
    hal_opi_ddr_en(OPI_BUS, true);
    hal_opi_enable(OPI_BUS);
    osal_irq_restore(irq_sts);

    test_opi_config_check();

    return OPI_RET_OK;
}

static opi_ret_e uapi_opi_xip_wait_idle(void)
{
    uint64_t start_time = uapi_tcxo_get_ms();
    uint32_t check_times = 0;

    while (((readw(WRITE_FIFO_STS_REG) & BIT(OPI_XIP_WRITE_FIFO_EMPTY_OFFSET)) == 0) ||
           (readw(XIP_WRITE_READ_CUR_STS_WRITE_REG) != 0) || (readw(XIP_WRITE_READ_CUR_STS_READ_REG) != 0)) {
        if (lib_timeout_check_timeout_periodic((uint64_t)OPI_TRANSFER_MAX_TIME_MS, start_time, &check_times)) {
            return OPI_RET_TIMEOUT;
        }
    }
    return OPI_RET_OK;
}

opi_ret_e uapi_opi_suspend(void)
{
    if (g_opi_sleeped) {
        return OPI_RET_SLEEP;
    }
    opi_pm_storage_value.ssienr_value = reg32(OPI_SSIENR_REG);
    opi_pm_storage_value.baudrate = reg32(OPI_BAUDRATE_REG);
    opi_pm_storage_value.ctrl_rl0 = reg32(OPI_CTRL0_REG);
    opi_pm_storage_value.tx_fifo_lr = reg32(OPI_TX_FIFO_LR_REG);
    opi_pm_storage_value.spi_ctrlr0 = reg32(OPI_SPI_CTRL_LR0_REG);

    writel(OPI_SSIENR_REG, opi_pm_initial_value.ssienr_value);
    writel(OPI_BAUDRATE_REG, opi_pm_initial_value.baudrate);
    writel(OPI_CTRL0_REG, opi_pm_initial_value.ctrl_rl0);
    writel(OPI_TX_FIFO_LR_REG, opi_pm_initial_value.tx_fifo_lr);
    writel(OPI_SPI_CTRL_LR0_REG, opi_pm_initial_value.spi_ctrlr0);

    g_opi_sleeped = true;
    return OPI_RET_OK;
}

opi_ret_e uapi_opi_resume(void)
{
    opi_pinmux();
    if (!g_opi_sleeped) {
        return OPI_RET_WAKE;
    }
    writel(OPI_BAUDRATE_REG, opi_pm_storage_value.baudrate);
    writel(OPI_CTRL0_REG, opi_pm_storage_value.ctrl_rl0);
    writel(OPI_TX_FIFO_LR_REG, opi_pm_storage_value.tx_fifo_lr);
    writel(OPI_SPI_CTRL_LR0_REG, opi_pm_storage_value.spi_ctrlr0);
    writel(OPI_SSIENR_REG, opi_pm_storage_value.ssienr_value);

    g_opi_sleeped = false;
    return OPI_RET_OK;
}

static errcode_t psram_reset_ap_x16(void)
{
    opi_config_t opi_config = {OPI_FRAME_FORMAT_OCTAL, OPI_INST_16BIT, OPI_ADDR_32BIT, OPI_FRAME_SIZE_16BIT,
                               OPI_TRANS_TYPE_2, OPI_TRANS_MODE_TX, OPI_CLK_166_MHZ, PSRAM_REG_RW_TRANSFER_DATA_LEN};
    errcode_t ret;

    ret = uapi_opi_open(&opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }

    uint32_t irq_sts = osal_irq_lock();

    hal_opi_disable_slave(OPI_BUS);

    // inst 16bits
    hal_opi_write_data(OPI_BUS, g_psram_device_parm[PSRAM_DEVICE_AP_X16].cmd->reset);
    // addr 32bits
    hal_opi_write_data(OPI_BUS, PSRAM_REDUNDANT_DATA);
    // data 16bits
    hal_opi_write_data(OPI_BUS, PSRAM_REDUNDANT_DATA);

    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);

    osal_irq_restore(irq_sts);

    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }
    non_os_set_driver_initalised(DRIVER_INIT_PSRAM, false);

    return ERRCODE_SUCC;
}

static errcode_t psram_reset_winbon_x8(void)
{
    non_os_set_driver_initalised(DRIVER_INIT_PSRAM, false);
    return ERRCODE_SUCC;
}

errcode_t uapi_psram_reset(void)
{
    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    switch (g_psram_device) {
        case PSRAM_DEVICE_AP_X8:
            /* fall-through */
        case PSRAM_DEVICE_AP_X16:
            return psram_reset_ap_x16();
        case PSRAM_DEVICE_WINBON_X8:
            return psram_reset_winbon_x8();
        default:
            return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }
    return ERRCODE_SUCC;
}

errcode_t uapi_psram_init(void)
{
    if (non_os_is_driver_initialised(DRIVER_INIT_PSRAM) == true) {
        return ERRCODE_PSRAM_RET_INITED;
    }
    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    opi_pinmux();
    // addr write: read latency, write latency, wrap, X16;
    for (uint32_t i = 0 ; i < g_psram_device_parm[g_psram_device].psram_param_num; i++) {
        psram_device_e psramdevice = g_psram_device;
        if (uapi_psram_write_reg((g_psram_device_parm[psramdevice].psram_param)[i].addr,
                                 (g_psram_device_parm[psramdevice].psram_param)[i].vaule) != ERRCODE_SUCC) {
            return ERRCODE_PSRAM_RET_ERROR;
        }
    }
    reg16_write(OPI_NON_ALIGNED_ACCESS_REG, OPI_NON_ALIGNED_ACCESS_EN);
    non_os_set_driver_initalised(DRIVER_INIT_PSRAM, true);
    return ERRCODE_SUCC;
}

errcode_t uapi_psram_write_reg(uint32_t addr, uint32_t value)
{
    errcode_t ret;
    uint32_t value_temp = value;
    opi_config_t opi_config = {g_psram_device_parm[g_psram_device].frame_format, OPI_INST_16BIT, OPI_ADDR_32BIT,
        OPI_FRAME_SIZE_16BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_TX, OPI_CLK_166_MHZ, PSRAM_REG_RW_TRANSFER_DATA_LEN};

    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }
    if ((g_psram_device == PSRAM_DEVICE_AP_X16) || (g_psram_device == PSRAM_DEVICE_AP_X8)) {
        opi_config.frame_format = OPI_FRAME_FORMAT_OCTAL;
        value_temp = value_temp << PSRAM_AP_X16_REG_WRITE_DATA_SHIFT;
    }
    if (g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_SLEEP;
    }
    ret = uapi_opi_open(&opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }

    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable_slave(OPI_BUS);
    // inst 16bits
    hal_opi_write_data(OPI_BUS, g_psram_device_parm[g_psram_device].cmd->reg_to_write);
    // addr 32bits
    hal_opi_write_data(OPI_BUS, addr);
    // data 16bits
    hal_opi_write_data(OPI_BUS, value_temp);
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    // config device mode
    reg32_write((uintptr_t)((g_psram_device_parm[g_psram_device].xip_param)[XIP_PARAMTER_ARRAY_INDEX_DEVICE].addr),
                PSRAM_DEVICE_MODE_CLEAR);
    osal_irq_restore(irq_sts);
#ifdef OPI_DEBUG
    PRINT("cmd:0x%x, addr:0x%x value_temp:0x%x\r\n",
          g_psram_device_parm[g_psram_device].cmd->reg_to_write, addr, value_temp);
#endif
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_psram_read_reg(uint32_t addr, uint32_t *value)
{
    errcode_t ret;
    uint32_t check_times = 0;
    uint64_t start_time = uapi_tcxo_get_ms();
    opi_config_t opi_config = {g_psram_device_parm[g_psram_device].frame_format, OPI_INST_16BIT, OPI_ADDR_32BIT,
        OPI_FRAME_SIZE_32BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_EEPROM, OPI_CLK_166_MHZ, PSRAM_REG_RW_TRANSFER_DATA_LEN};

    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }
    if (g_psram_device == PSRAM_DEVICE_AP_X16) {
        opi_config.frame_format = OPI_FRAME_FORMAT_OCTAL;
    }
    if (g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_SLEEP;
    }

    ret = uapi_opi_open(&opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }

    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable_slave(OPI_BUS);
    hal_opi_write_data(OPI_BUS, g_psram_device_parm[g_psram_device].cmd->reg_to_read);
    hal_opi_write_data(OPI_BUS, addr);
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    // config device mode
    reg32_write((uintptr_t)((g_psram_device_parm[g_psram_device].xip_param)[XIP_PARAMTER_ARRAY_INDEX_DEVICE].addr),
                PSRAM_DEVICE_MODE_CLEAR);
    osal_irq_restore(irq_sts);

    while (!hal_opi_rx_fifo_is_not_empty(OPI_BUS)) {
        if (lib_timeout_check_timeout_periodic((uint64_t)OPI_TRANSFER_MAX_TIME_MS, start_time, &check_times)) {
            return ERRCODE_PSRAM_RET_TIMEOUT;
        }
    }
    *value = hal_opi_read_data(OPI_BUS);
#ifdef OPI_DEBUG
    PRINT("cmd:0x%x, addr:0x%x, value:0x%x. \r\n", g_psram_device_parm[g_psram_device].cmd->reg_to_read, addr, *value);
#endif
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    return ERRCODE_SUCC;
}

static opi_ret_e check_psram_awake(void)
{
    uint8_t wake_times = 0;
    while (g_psram_sleeped && wake_times < PSRAM_MAX_WAKE_TIME) {
        PRINT("psram is slept, can not enter xip. try to exit sleep.\r\n");
        uapi_psram_exit_sleep();
        wake_times++;
    }
    if (g_psram_sleeped) {
        return OPI_RET_SLEEP;
    }
    return OPI_RET_OK;
}

errcode_t uapi_psram_enter_xip(void)
{
    opi_config_t opi_config = {g_psram_device_parm[g_psram_device].frame_format, OPI_INST_16BIT, OPI_ADDR_32BIT,
        OPI_FRAME_SIZE_32BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_TX, OPI_CLK_166_MHZ, PSRAM_REG_RW_TRANSFER_DATA_LEN};
    errcode_t ret;

    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    if (uapi_psram_xip_enabled()) {
        return ERRCODE_PSRAM_RET_XIP_EN;
    }

    if (check_psram_awake() != OPI_RET_OK) {
        return OPI_RET_SLEEP;
    }

    if (g_psram_device == PSRAM_DEVICE_AP_X16) {
        opi_config.frame_format = OPI_FRAME_FORMAT_SIXT;
    }

    ret = uapi_opi_open(&opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }

    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable(OPI_BUS);
    hal_opi_set_dma_rx_data_level(OPI_BUS, OPI_FIFO_THRESHOLD_RX_DMA);
    hal_opi_dma_control(OPI_BUS, HAL_SPI_DMA_CONTROL_RX_ENABLE);
    hal_opi_enable(OPI_BUS);
    dsb();
    isb();
    reg16_write(CFG_CACHE2HABM_OVER_TIME_H_REG, XIP_CACHE_OVER_TIME);

    reg16_write(CFG_XIP_OPI_READ_OVER_TIME_L_REG, 0xFFFF);
    reg16_write(CFG_XIP_OPI_READ_OVER_TIME_H_REG, 0xFFFF);
    reg16_write(CFG_XIP_WRITE_OVER_TIME_L_REG, 0xFFFF);
    reg16_write(CFG_XIP_WRITE_OVER_TIME_H_REG, 0xFFFF);

    reg16_setbit(XIP_WRITE_READ_ERROR_RESP_MASK_REG, 0x1);

    for (uint32_t i = 0 ; i < g_psram_device_parm[g_psram_device].xip_param_num; i++) {
        psram_device_e psramdevice = g_psram_device;
        reg32_write((uintptr_t)((g_psram_device_parm[psramdevice].xip_param)[i].addr),
                    (g_psram_device_parm[psramdevice].xip_param)[i].vaule);
#ifdef OPI_DEBUG
        PRINT("xip_param_num:0x%x, addr:0x%x value:0x%x\r\n", g_psram_device_parm[g_psram_device].xip_param_num,
              (g_psram_device_parm[g_psram_device].xip_param)[i].addr,
              (g_psram_device_parm[g_psram_device].xip_param)[i].vaule);
#endif
    }
#if (PSRAM_RXDS_DELAY_ADJUST == YES)
    reg16_write(RXDS_SEL_REG, RXDS_SEL_REG_VAL);
    reg16_write(RXDS_HIGH_SEL_REG, RXDS_HIGH_SEL_REG_VAL);
#endif
    reg16_write(XIP_WRITE_READ_SYNC_REG, XIP_CONFIG_SYNC);
    reg16_write(XIP_WRITE_READ_ENABLE_REG, XIP_OPI_ENABLE);
    dsb();
    isb();
    osal_irq_restore(irq_sts);

#if defined(DCACHE_IS_ENABLE) && (DCACHE_IS_ENABLE == YES)
    SCB_EnableDCache();
#endif

    UNUSED(opi_config);
    return ERRCODE_SUCC;
}

errcode_t uapi_psram_exit_xip(void)
{
    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    if (!uapi_psram_xip_enabled()) {
        return ERRCODE_PSRAM_RET_XIP_DIS;
    }

#if defined(DCACHE_IS_ENABLE) && (DCACHE_IS_ENABLE == YES)
    SCB_DisableDCache();
#endif

    if (uapi_opi_xip_wait_idle() != ERRCODE_SUCC) {
        return OPI_RET_TIMEOUT;
    }

    dsb();
    isb();
    uint32_t irq_sts = osal_irq_lock();

    reg16_write(XIP_WRITE_READ_ENABLE_REG, XIP_OPI_DISABLE);
    hal_opi_dma_control(OPI_BUS, HAL_SPI_DMA_CONTROL_DISABLE);

    osal_irq_restore(irq_sts);

    return ERRCODE_SUCC;
}

static errcode_t psram_opi_write_data(uint32_t address, uint32_t *buffer,
                                      uint32_t buffer_length, uint8_t redundant_data)
{
    uint32_t *data = buffer;
    uint32_t inst = g_psram_device_parm[g_psram_device].cmd->mem_write;
    uint32_t addr = address - PSRAM_MEMORY_ADDR_REGION;

    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    uint8_t redundant_data_temp = redundant_data;
    uint32_t buffer_length_temp = buffer_length;
#ifdef OPI_DEBUG
    PRINT("redundant_data: %d\r\n", redundant_data_temp);
    PRINT("inst: 0x%x, addr: 0x%x. \r\n", inst, addr);
#endif

    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable_slave(OPI_BUS);
    hal_opi_write_data(OPI_BUS, inst);
    hal_opi_write_data(OPI_BUS, addr);
    while (redundant_data_temp != 0) {
        hal_opi_write_data(OPI_BUS, PSRAM_REDUNDANT_DATA);
        redundant_data_temp--;
    }
    while (buffer_length_temp != 0) {
        if (hal_opi_tx_fifo_is_not_full(OPI_BUS)) {
            hal_opi_write_data(OPI_BUS, *data);
            data++;
            buffer_length_temp--;
        }
    }
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    osal_irq_restore(irq_sts);

    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    return ERRCODE_SUCC;
}

static errcode_t psram_opi_read_data(uint32_t address, uint32_t *buffer, uint32_t buffer_length)
{
    uint32_t *data = buffer;
    uint32_t inst = g_psram_device_parm[g_psram_device].cmd->mem_read;
    uint32_t addr = address - PSRAM_MEMORY_ADDR_REGION;

    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    uint32_t irq_sts = osal_irq_lock();

    hal_opi_disable(OPI_BUS);
    hal_opi_set_received_data_num(OPI_BUS, buffer_length);
    hal_opi_enable(OPI_BUS);

    hal_opi_disable_slave(OPI_BUS);
    hal_opi_write_data(OPI_BUS, inst);
    hal_opi_write_data(OPI_BUS, addr);
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    osal_irq_restore(irq_sts);

    PRINT("inst: 0x%x, addr: 0x%x. \r\n", inst, addr);

    while (buffer_length != 0) {
        if (hal_opi_rx_fifo_is_full(OPI_BUS)) {
            return ERRCODE_PSRAM_RET_ERROR;
        }
        if (hal_opi_rx_fifo_is_not_empty(OPI_BUS)) {
            *data = hal_opi_read_data(OPI_BUS);
            data++;
            buffer_length--;
        }
    }

    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    return ERRCODE_SUCC;
}

static errcode_t psram_operate_memory_prepare(opi_config_t *opi_config)
{
    errcode_t ret;
    if (g_psram_device >= PSRAM_DEVICE_NONE) {
        return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }
    if (g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_SLEEP;
    }

    ret = uapi_opi_open(opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }
    /* config device */
    psram_device_e psramdevice = g_psram_device;
    reg32_write((uintptr_t)((g_psram_device_parm[psramdevice].xip_param)[XIP_PARAMTER_ARRAY_INDEX_DEVICE].addr),
                (g_psram_device_parm[psramdevice].xip_param)[XIP_PARAMTER_ARRAY_INDEX_DEVICE].vaule);
    return ERRCODE_SUCC;
}

errcode_t uapi_psram_write_memory(uint32_t address, uint32_t *buffer, uint32_t buffer_length)
{
    uint32_t trans_address = address;
    uint32_t current_buffer_length = buffer_length;
    opi_config_t opi_config = {g_psram_device_parm[g_psram_device].frame_format, OPI_INST_16BIT, OPI_ADDR_32BIT,
        OPI_FRAME_SIZE_32BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_TX, OPI_CLK_166_MHZ, current_buffer_length};
    uint32_t transfer_length;
    uint32_t *transfer_buffer = buffer;
    uint32_t max_transfer_length;
    uint8_t redundant_data = (uint8_t)g_psram_device_parm[g_psram_device].xip_param->vaule;
    errcode_t ret = psram_operate_memory_prepare(&opi_config);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    max_transfer_length = OPI_FIFO_DEEP - PSRAM_CMD_FRAMSIZE - PSRAM_ADDR_FRAMSIZE - redundant_data;
    while (current_buffer_length != 0) {
        if (current_buffer_length >= max_transfer_length) {
            transfer_length = max_transfer_length;
        } else {
            transfer_length = current_buffer_length;
        }

        ret = psram_opi_write_data(trans_address, transfer_buffer, transfer_length, redundant_data);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
        trans_address += (transfer_length * PSRAM_MEM_ADDR_TRANS_LEN_OFFSET_BYTE);
        transfer_buffer += transfer_length;
        current_buffer_length -= transfer_length;
    }

    return ERRCODE_SUCC;
}

errcode_t uapi_psram_read_memory(uint32_t address, uint32_t *buffer, uint32_t buffer_length)
{
    uint32_t current_address = address;
    uint32_t *target_buffer = buffer;
    uint32_t target_buffer_length = buffer_length;
    opi_config_t opi_config = {g_psram_device_parm[g_psram_device].frame_format, OPI_INST_16BIT, OPI_ADDR_32BIT,
        OPI_FRAME_SIZE_32BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_EEPROM, OPI_CLK_166_MHZ, target_buffer_length};
    uint32_t transfer_length;
    errcode_t ret = psram_operate_memory_prepare(&opi_config);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    while (target_buffer_length != 0) {
        if (target_buffer_length >= OPI_FIFO_DEEP) {
            transfer_length = OPI_FIFO_DEEP;
        } else {
            transfer_length = target_buffer_length;
        }

        ret = psram_opi_read_data(current_address, target_buffer, transfer_length);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
        current_address += (transfer_length * PSRAM_MEM_ADDR_TRANS_LEN_OFFSET_BYTE);
        target_buffer += transfer_length;
        target_buffer_length -= transfer_length;
    }

    return ERRCODE_SUCC;
}

bool uapi_psram_xip_enabled(void)
{
    return(reg16(XIP_WRITE_READ_ENABLE_REG) == XIP_OPI_ENABLE);
}

static bool psram_get_reg_value(psram_drivers_e driver, uint32_t *mask, uint32_t *pos)
{
    switch (g_psram_device) {
        case PSRAM_DEVICE_AP_X16:
            if (driver >= PSRAM_DRIVER_3) {
                return false;
            }
            *mask = PSRAM_DRIVER_3;
            *pos = PSRAM_PARAMTER_DRIVER_STRENGTH_POS_AP_X16;
            break;
        case PSRAM_DEVICE_WINBON_X8:
            if (driver >= PSRAM_DRIVER_7) {
                return false;
            }
            *mask = PSRAM_DRIVER_7;
            *pos = PSRAM_PARAMTER_DRIVER_STRENGTH_POS_WINBON_X8;
            break;
        default :
            return false;
    }
    UNUSED(driver);
    UNUSED(mask);
    UNUSED(pos);
    return true;
}

errcode_t uapi_psram_config_driver(psram_drivers_e driver)
{
    uint32_t value;
    uint32_t mask = 0;
    uint32_t pos = 0;

    if (uapi_psram_xip_enabled()) {
        return ERRCODE_PSRAM_RET_XIP_EN;
    }
    if (g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_SLEEP;
    }
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    if (!psram_get_reg_value(driver, &mask, &pos)) {
        return ERRCODE_PSRAM_RET_WRONG_PARA;
    }

    if (uapi_psram_read_reg((g_psram_device_parm[g_psram_device].psram_param) \
                            [PSRAM_PARAM_ARRAY_INDEX_DRIVER_STRENGTH].addr, &value) != ERRCODE_SUCC) {
        return ERRCODE_PSRAM_RET_ERROR;
    }
    if (g_psram_device == PSRAM_DEVICE_AP_X16) {
        value = (value & 0xFFFF) >> PSRAM_AP_X16_REG_WRITE_DATA_SHIFT;
    }
    value &= ~(mask << pos);
    value |= (uint32_t)((uint32_t)driver << pos);
    if (uapi_psram_write_reg((g_psram_device_parm[g_psram_device].psram_param) \
                             [PSRAM_PARAM_ARRAY_INDEX_DRIVER_STRENGTH].addr, value) != ERRCODE_SUCC) {
        return ERRCODE_PSRAM_RET_ERROR;
    }

    return ERRCODE_SUCC;
}

static errcode_t psram_enter_sleep_device_ap_x16(void)
{
    return uapi_psram_write_reg(PSRAM_REG_AP_X16_MR6, PSRAM_AP_X16_MR6_ENTER_HALF_SLEEP);
}

static errcode_t psram_enter_sleep_device_winbon_x8(void)
{
    uint32_t value;
    if (uapi_psram_read_reg(PSRAM_REG_WINBON_X8_CR1, &value) != ERRCODE_SUCC) {
        return ERRCODE_PSRAM_RET_ERROR;
    }

    value |= PSRAM_WINBON_X8_CR1_ENTER_HYBIRD_SLEEP;
    if (uapi_psram_write_reg(PSRAM_REG_WINBON_X8_CR1, value) != ERRCODE_SUCC) {
        return ERRCODE_PSRAM_RET_ERROR;
    }

    return ERRCODE_SUCC;
}

static errcode_t psram_exit_sleep_device_ap_x16(void)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable_slave(OPI_BUS);
    hal_opi_write_data(OPI_BUS, 0x0);
    hal_opi_write_data(OPI_BUS, 0x100);
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    osal_irq_restore(irq_sts);

    hal_opi_disable(OPI_BUS);
    uapi_tcxo_delay_us(PSRAM_CE_PULL_UP_WAIT_MIN_TIME);
    uint32_t reg_value = 0;
    g_psram_sleeped = false;
    uapi_psram_read_reg(PSRAM_REG_AP_X16_MR0, &reg_value);
    g_psram_sleeped = true;
    // Read variable latency configuration codes MR0[4:2] --- 011
    if (((reg_value >> PSRAM_AP_X16_REG_WRITE_DATA_SHIFT) & PSRAM_AP_X16_MR0_RL_CODE_MASK) ==
        (PSRAM_AP_X16_MR0_READ_LATENCY_166MHZ & PSRAM_AP_X16_MR0_RL_CODE_MASK)) {
        return OPI_RET_OK;
    }
    return OPI_RET_ERROR;
}

static errcode_t psram_exit_sleep_device_winbon_x8(void)
{
    return psram_exit_sleep_device_ap_x16();
}

errcode_t uapi_psram_enter_sleep(void)
{
    errcode_t ret;

    if (uapi_psram_xip_enabled()) {
        return ERRCODE_PSRAM_RET_XIP_EN;
    }
    if (g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_SLEEP;
    }
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }

    switch (g_psram_device) {
        case PSRAM_DEVICE_AP_X16:
            ret = psram_enter_sleep_device_ap_x16();
            break;
        case PSRAM_DEVICE_WINBON_X8:
            ret = psram_enter_sleep_device_winbon_x8();
            break;
        default :
            return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    if (ret == ERRCODE_SUCC) {
        g_psram_sleeped = true;
        return ERRCODE_SUCC;
    }

    return ERRCODE_PSRAM_RET_ERROR;
}

errcode_t uapi_psram_exit_sleep(void)
{
    errcode_t ret;

    if (uapi_psram_xip_enabled()) {
        return ERRCODE_PSRAM_RET_XIP_EN;
    }
    if (!g_psram_sleeped) {
        return ERRCODE_PSRAM_RET_WAKE;
    }

    switch (g_psram_device) {
        case PSRAM_DEVICE_AP_X16:
            ret = psram_exit_sleep_device_ap_x16();
            break;
        case PSRAM_DEVICE_WINBON_X8:
            ret = psram_exit_sleep_device_winbon_x8();
            break;
        default :
            return ERRCODE_PSRAM_RET_WRONG_DEVICE;
    }

    if (ret == ERRCODE_SUCC) {
        g_psram_sleeped = false;
        return ERRCODE_SUCC;
    }

    return ERRCODE_PSRAM_RET_ERROR;
}

void uapi_psram_xip_suspend(void)
{
    uapi_psram_exit_xip();
    uapi_psram_enter_sleep();
    uapi_opi_suspend();
}

void uapi_psram_xip_resume(void)
{
    uapi_opi_resume();
    uapi_psram_exit_sleep();
    uapi_psram_enter_xip();
}

void uapi_psram_switch_to_high_fre(psram_clk_e sel)
{
    if (sel >= PSRAM_MAX) { return; }
#if OPI_USE_MCU_HS_CLK == NO
    pmu_cmu_pll_deinit(PMU_CMU_AUDIO);
    pmu_cmu_pll_reinit(PMU_CMU_AUDIO, g_psram_pll_cfg[sel]);
#else
    system_ccrg_clock_config(CLOCKS_CCRG_MODULE_XIP_OPI, g_psram_clk_cfg[sel].clk_src, g_psram_clk_cfg[sel].clk_div);
#endif
}

static void psram_eid_read_config(uint32_t addr, uint16_t *data)
{
    uint32_t irq_sts = osal_irq_lock();
    hal_opi_disable(OPI_BUS);
    g_spi_base_addrs[OPI_BUS]->baudr = OPI_CLK_DIV_10; // read eid needs freq lower than 50MHz
    hal_opi_enable(OPI_BUS);
    hal_opi_disable_slave(OPI_BUS);
    hal_opi_write_data(OPI_BUS, PSRAM_CMD_AP_X16_READ_REG);
    hal_opi_write_data(OPI_BUS, addr);
    hal_opi_select_slave(OPI_BUS, OPI_SLAVE_NUMBER);
    reg32(WRITE_REDUNDANT_CNT_REG) = PSRAM_DEVICE_MODE_CLEAR; // config device mode
    osal_irq_restore(irq_sts);
    uint32_t value = 0;
    uint32_t data_temp = 0;
#ifdef OPI_DEBUG
    PRINT("*******************addr is %d******************\r\n", addr);
#endif
    while (!hal_opi_rx_fifo_is_not_empty(OPI_BUS)) { // waiting for rx
        uapi_tcxo_delay_us(PSRAM_OPI_DELAY_US_10);
    }
    for (int n = 0; n < 2; n++) { // FIFO deep is 32 bytes, uint16 needs loop 2 times
        data_temp = 0;
        // 提取最后一位，依次拼接
        for (int i = 0; i < 16; i++) {  // uint16 has 16 bits
            if (hal_opi_rx_fifo_is_not_empty(OPI_BUS)) {
                value = hal_opi_read_data(OPI_BUS);
            } else {
                return;
            }
            data_temp |= (value & 1);
#ifdef OPI_DEBUG
            PRINT("%d\t: receive data: %#010x, bit = %d, data_temp = 0x%x\r\n", i, value, value & 1, data_temp);
#endif
            data_temp = data_temp << 1;
        }
        data_temp = data_temp >> 1;
        data[n] = (uint16_t)data_temp;
    }
}

errcode_t uapi_psram_read_eid(uint16_t *id, uint8_t length)
{
    if (id == NULL || length < PSRAM_EID_LENGTH) {
        return ERRCODE_PSRAM_RET_WRONG_PARA;
    }
    uint32_t irq_sts = osal_irq_lock();
    uapi_psram_exit_xip();
    uint32_t value = 0;
    errcode_t ret;
    opi_config_t opi_config = {OPI_FRAME_FORMAT_OCTAL, OPI_INST_16BIT, OPI_ADDR_32BIT,
                               OPI_FRAME_SIZE_8BIT, OPI_TRANS_TYPE_2, OPI_TRANS_MODE_EEPROM,
                               OPI_CLK_166_MHZ, OPI_FIFO_DEEP};
    ret = uapi_opi_open(&opi_config);
    if (ret != OPI_RET_OK) {
        return ret;
    }
    psram_eid_read_config(PSRAM_REG_AP_X16_MR6, id);
    // FIFO deep is 32 bytes, uint16 needs loop 2 times
    psram_eid_read_config(PSRAM_EID_SECOND_PARAM_OFFSET, &id[2]);

#ifdef PSRAM_EID_TEST
    PRINT("************eid test************\r\n");
    uint16 eid_test[PSRAM_EID_TEST_PARAM_MAX];
    for (int i = 0; i < PSRAM_EID_TEST_PARAM_MAX / LOOP_2_TIMES; i++) {
        psram_eid_read_config(PSRAM_REG_MR6 + i * PSRAM_EID_PER_4BIT_NEED_ADDR_OFFSET_8BYTES,
                              &eid_test[i * LOOP_2_TIMES]);
    }
    for (int i = 0; i < PSRAM_EID_TEST_PARAM_MAX; i++) {
        PRINT("PSRAM EID test value %d is 0x%x\r\n", i, eid_test[i]);
    }
    PRINT("************eid test done************\r\n");
#endif

    for (uint32_t i = 0; i < PSRAM_EID_LENGTH / sizeof(uint16_t); i++) {
        PRINT("PSRAM EID[%d] is 0x%x\r\n", i, id[i]);
    }
    hal_opi_disable(OPI_BUS);
    hal_opi_set_freq(OPI_BUS, OPI_CLK_166_MHZ); // recovery the psram freq
    hal_opi_enable(OPI_BUS);
    if (uapi_opi_trans_wait() != OPI_RET_OK) {
        return ERRCODE_PSRAM_RET_TIMEOUT;
    }
    uapi_psram_enter_xip();
    osal_irq_restore(irq_sts);
    return ERRCODE_SUCC;
}