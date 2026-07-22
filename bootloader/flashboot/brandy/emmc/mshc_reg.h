/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:   MSHC REG HEADER FILE
 * Author: @CompanyNameTag
 * Create:  2020-04
 */

#ifndef MSHC_REG_H
#define MSHC_REG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Controller registers
 */
#define MSHC_DMA_ADDRESS      0x00
#define MSHC_ARGUMENT2        MSHC_DMA_ADDRESS

#define MSHC_BLOCK_SIZE             0x04
#define SDMA_BUF_BOUNDARY_512K      7
#define MSHC_MAKE_BLKSZ(dma, blksz) ((((dma) & 0x7) << 12) | ((blksz) & 0xFFF))

#define MSHC_BLOCK_COUNT          0x06

#define MSHC_ARGUMENT             0x08

#define MSHC_TRANSFER_MODE       0x0C
#define MSHC_TRNS_DMA            0x01
#define MSHC_TRNS_BLK_CNT_EN     0x02
#define MSHC_TRNS_AUTO_CMD12     0x04
#define MSHC_TRNS_AUTO_CMD23     0x08
#define MSHC_TRNS_READ           0x10
#define MSHC_TRNS_MULTI          0x20

#define MSHC_COMMAND             0x0E
#define MSHC_CMD_RESP_MASK       0x03
#define MSHC_CMD_CRC             0x08
#define MSHC_CMD_INDEX           0x10
#define MSHC_CMD_DATA            0x20
#define MSHC_CMD_ABORTCMD        0xC0

#define MSHC_CMD_RESP_NONE       0x00
#define MSHC_CMD_RESP_LONG       0x01
#define MSHC_CMD_RESP_SHORT      0x02
#define MSHC_CMD_RESP_SHORT_BUSY 0x03

#define MSHC_MAKE_CMD(c, f)      ((((c) & 0xff) << 8) | ((f) & 0xff))
#define MSHC_GET_CMD(c)          (((c)>>8) & 0x3f)

#define MSHC_RESPONSE            0x10

#define MSHC_BUFFER              0x20
#define MSHC_BUFFER_LEN          128

#define MSHC_PRESENT_STATE       0x24
#define MSHC_CMD_INHIBIT         0x00000001
#define MSHC_DATA_INHIBIT        0x00000002
#define MSHC_DOING_WRITE         0x00000100
#define MSHC_DOING_READ          0x00000200
#define MSHC_SPACE_AVAILABLE     0x00000400
#define MSHC_DATA_AVAILABLE      0x00000800
#define MSHC_CARD_PRESENT        0x00010000
#define MSHC_WRITE_PROTECT       0x00080000
#define MSHC_DATA_LVL_MASK       0x00F00000
#define MSHC_DATA_LVL_SHIFT      20
#define MSHC_DATA_0_LVL_MASK     0x00100000

#define MSHC_HOST_CONTROL        0x28
#define MSHC_CTRL_LED            0x01
#define MSHC_CTRL_4BITBUS        0x02
#define MSHC_CTRL_HISPD          0x04
#define MSHC_CTRL_DMA_MASK       0x18
#define MSHC_CTRL_SDMA           0x00
#define MSHC_CTRL_ADMA1          0x08
#define MSHC_CTRL_ADMA32         0x10
#define MSHC_CTRL_ADMA64         0x18
#define MSHC_CTRL_8BITBUS        0x20

#define MSHC_POWER_CONTROL       0x29
#define MSHC_POWER_ON            0x01
#define MSHC_POWER_180           0x0A
#define MSHC_POWER_300           0x0C
#define MSHC_POWER_330           0x0E

#define MSHC_BLOCK_GAP_CONTROL   0x2A

#define MSHC_WAKE_UP_CONTROL     0x2B
#define MSHC_WAKE_ON_INT         0x01
#define MSHC_WAKE_ON_INSERT      0x02
#define MSHC_WAKE_ON_REMOVE      0x04

#define MSHC_CLOCK_CONTROL       0x2C
#define MSHC_DIVIDER_SHIFT       8
#define MSHC_DIVIDER_HI_SHIFT    6
#define MSHC_DIV_MASK            0xFF
#define MSHC_DIV_MASK_LEN        8
#define MSHC_DIV_HI_MASK         0x300
#define MSHC_PROG_CLOCK_MODE     0x0020
#define MSHC_CLOCK_PLL_EN        0x0008
#define MSHC_CLOCK_CARD_EN       0x0004
#define MSHC_CLOCK_INT_STABLE    0x0002
#define MSHC_CLOCK_INT_EN        0x0001

#define MSHC_TIMEOUT_CONTROL     0x2E
#define MSHC_DEFINE_TIMEOUT      0xE

#define MSHC_SOFTWARE_RESET      0x2F
#define MSHC_RESET_ALL           0x01
#define MSHC_RESET_CMD           0x02
#define MSHC_RESET_DATA          0x04

#define MSHC_TOUT_CTRL           0x2e

#define MSHC_PWR_CTRL            0x29
#define MSHC_PWR_CTRL_VDD2       0x1

#define MSHC_CLK_CTRL            0x2c
#define MSHC_CLK_CTRL_PLL        (1 << 3)
#define MSHC_CLK_CTRL_CLK_EN     (1 << 2)
#define MSHC_CLK_CTRL_INT_CLK    (1 << 0)

#define MSHC_NOR_INT_STATUS      0x30
#define MSHC_ERR_INT_STATUS      0x32
#define MSHC_NOR_INT_ENABLE      0x34
#define MSHC_ERR_INT_ENABLE      0x36
#define MSHC_NOR_SIGNAL_ENABLE   0x38
#define MSHC_ERR_SIGNAL_ENABLE   0x3A

#define MSHC_NOR_INT_MASK        0x0000FFFF
#define MSHC_ERR_INT_MASK        0xFFFF0000
#define MSHC_ERR_INT_OFFSET      16

#define MSHC_INT_RESPONSE        0x00000001
#define MSHC_INT_DATA_END        0x00000002
#define MSHC_INT_BLK_GAP         0x00000004
#define MSHC_INT_DMA_END         0x00000008
#define MSHC_INT_SPACE_AVAIL     0x00000010
#define MSHC_INT_DATA_AVAIL      0x00000020
#define MSHC_INT_CARD_INSERT     0x00000040
#define MSHC_INT_CARD_REMOVE     0x00000080
#define MSHC_INT_CARD_INT        0x00000100
#define MSHC_INT_ERROR           0x00008000
#define MSHC_INT_TIMEOUT         0x00010000
#define MSHC_INT_CRC             0x00020000
#define MSHC_INT_END_BIT         0x00040000
#define MSHC_INT_INDEX           0x00080000
#define MSHC_INT_DATA_TIMEOUT    0x00100000
#define MSHC_INT_DATA_CRC        0x00200000
#define MSHC_INT_DATA_END_ERR    0x00400000
#define MSHC_INT_BUS_POWER       0x00800000
#define MSHC_INT_AUTO_CMD_ERR    0x01000000
#define MSHC_INT_ADMA_ERROR      0x02000000
// do not set it to the HW register
#define MSHC_INT_RESCAN_SW       0x80000000

#define MSHC_INT_NORMAL_MASK     0x00007FFF
#define MSHC_INT_ERROR_MASK      0xFFFF8000

#define MSHC_INT_CMD_MASK    (MSHC_INT_RESPONSE | MSHC_INT_TIMEOUT | \
                              MSHC_INT_CRC | MSHC_INT_END_BIT | MSHC_INT_INDEX | \
                              MSHC_INT_AUTO_CMD_ERR)

#define  MSHC_INT_DATA_MASK    (MSHC_INT_DATA_END | MSHC_INT_DMA_END | \
                                MSHC_INT_DATA_AVAIL | MSHC_INT_SPACE_AVAIL | \
                                MSHC_INT_DATA_TIMEOUT | MSHC_INT_DATA_CRC | \
                                MSHC_INT_DATA_END_ERR | MSHC_INT_ADMA_ERROR | \
                                MSHC_INT_BLK_GAP)

#define MSHC_INT_CMDQ_EN     (0x1 << 14)
#define MSHC_INT_ALL_MASK    ((unsigned int)-1)

#define MSHC_AUTO_CMD_HOSTCTRL2    0x3c
#define MSHC_AUTO_CMD_ERR          0x3C
#define MSHC_AUTO_CMD12_NOT_EXEC   0x0001
#define MSHC_AUTO_CMD_TIMEOUT_ERR  0x0002
#define MSHC_AUTO_CMD_CRC_ERR      0x0004
#define MSHC_AUTO_CMD_ENDBIT_ERR   0x0008
#define MSHC_AUTO_CMD_INDEX_ERR    0x0010
#define MSHC_AUTO_CMD12_NOT_ISSUED 0x0080
#define MSHC_AUTO_CMD_SEL          0x10000000

#define MSHC_HOST_CONTROL2         0x3E
#define MSHC_CTRL_UHS_MASK         0x0007
#define MSHC_CTRL_UHS_SDR12        0x0000
#define MSHC_CTRL_UHS_SDR25        0x0001
#define MSHC_CTRL_UHS_SDR50        0x0002
#define MSHC_CTRL_UHS_SDR104       0x0003
#define MSHC_CTRL_UHS_DDR50        0x0004
#define MSHC_CTRL_HS_SDR200        0x0005 /* reserved value in SDIO spec */
#define MSHC_CTRL_HS400            0x0007
#define MSHC_CTRL_VDD_180          0x0008
#define MSHC_CTRL_DRV_TYPE_MASK    0x0030
#define MSHC_CTRL_DRV_TYPE_B       0x0000
#define MSHC_CTRL_DRV_TYPE_A       0x0010
#define MSHC_CTRL_DRV_TYPE_C       0x0020
#define MSHC_CTRL_DRV_TYPE_D       0x0030
#define MSHC_CTRL_EXEC_TUNING      0x0040
#define MSHC_CTRL_TUNED_CLK        0x0080
#define MSHC_CTRL_ASYNC_INT_ENABL  0x4000
#define MSHC_CTRL_PRESET_VAL_ENABLE 0x8000

#define MSHC_CAPABILITIES_1        0x40
#define MSHC_TIMEOUT_CLK_MASK      0x0000003F
#define MSHC_TIMEOUT_CLK_SHIFT     0
#define MSHC_TIMEOUT_CLK_UNIT      0x00000080
#define MSHC_CLOCK_BASE_MASK       0x00003F00
#define MSHC_CLOCK_V3_BASE_MASK    0x0000FF00
#define MSHC_CLOCK_BASE_SHIFT      8
#define MSHC_MAX_BLOCK_MASK        0x00030000
#define MSHC_MAX_BLOCK_SHIFT       16
#define MSHC_CAN_DO_8BIT           0x00040000
#define MSHC_CAN_DO_ADMA2          0x00080000
#define MSHC_CAN_DO_ADMA1          0x00100000
#define MSHC_CAN_DO_HISPD          0x00200000
#define MSHC_CAN_DO_SDMA           0x00400000
#define MSHC_CAN_VDD_330           0x01000000
#define MSHC_CAN_VDD_300           0x02000000
#define MSHC_CAN_VDD_180           0x04000000
#define MSHC_CAN_64BIT             0x10000000
#define MSHC_CAN_ASYNC_INT         0x20000000

#define MSHC_SUPPORT_SDR50              0x00000001
#define MSHC_SUPPORT_SDR104             0x00000002
#define MSHC_SUPPORT_DDR50              0x00000004
#define MSHC_DRIVER_TYPE_A              0x00000010
#define MSHC_DRIVER_TYPE_C              0x00000020
#define MSHC_DRIVER_TYPE_D              0x00000040
#define MSHC_RETUNING_TIMER_COUNT_MASK  0x00000F00
#define MSHC_RETUNING_TIMER_COUNT_SHIFT 8
#define MSHC_USE_SDR50_TUNING           0x00002000
#define MSHC_RETUNING_MODE_MASK         0x0000C000
#define MSHC_RETUNING_MODE_SHIFT        14
#define MSHC_CLOCK_MUL_MASK             0x00FF0000
#define MSHC_CLOCK_MUL_SHIFT            16
#define MSHC_CAN_DO_ADMA3               0x8000000
#define MSHC_SUPPORT_HS400              0x80000000

#define MSHC_CAPABILITIES_2             0x44

#define MSHC_MAX_CURRENT                0x48
#define MSHC_MAX_CURRENT_LIMIT          0xFF
#define MSHC_MAX_CURRENT_330_MASK       0x0000FF
#define MSHC_MAX_CURRENT_330_SHIFT      0
#define MSHC_MAX_CURRENT_300_MASK       0x00FF00
#define MSHC_MAX_CURRENT_300_SHIFT      8
#define MSHC_MAX_CURRENT_180_MASK       0xFF0000
#define MSHC_MAX_CURRENT_180_SHIFT      16
#define MSHC_MAX_CURRENT_MULTIPLIER     4

/* 4C-4F reserved for more max current */
#define MSHC_SET_ACMD12_ERROR 0x50
#define MSHC_SET_INT_ERROR    0x52

#define MSHC_ADMA_ERROR       0x54

/* 55-57 reserved */
#define MSHC_HI_SHIFT            32
#define MSHC_ADMA_ADDRESS_LOW    0x58 /* addr[0:31] */
#define MSHC_ADMA_ADDRESS_HIGH   0x5C /* addr[32:63] DO not support */

/* 60-FB reserved */
#define MSHC_PRESET_FOR_SDR12        0x66
#define MSHC_PRESET_FOR_SDR25        0x68
#define MSHC_PRESET_FOR_SDR50        0x6A
#define MSHC_PRESET_FOR_SDR104       0x6C
#define MSHC_PRESET_FOR_DDR50        0x6E
#define MSHC_PRESET_FOR_HS400        0x74 /* Non-standard */
#define MSHC_PRESET_DRV_MASK         0xC000
#define MSHC_PRESET_DRV_SHIFT        14
#define MSHC_PRESET_CLKGEN_SEL_MASK  0x400
#define MSHC_PRESET_CLKGEN_SEL_SHIFT 10
#define MSHC_PRESET_SDCLK_FREQ_MASK  0x3FF
#define MSHC_PRESET_SDCLK_FREQ_SHIFT 0

#define MSHC_SLOT_INT_STATUS         0xFC

#define MSHC_HOST_VERSION     0xFE
#define MSHC_VENDOR_VER_MASK  0xFF00
#define MSHC_VENDOR_VER_SHIFT 8
#define MSHC_SPEC_VER_MASK    0x00FF
#define MSHC_SPEC_VER_SHIFT   0
#define MSHC_SPEC_100         0
#define MSHC_SPEC_200         1
#define MSHC_SPEC_300         2
#define MSHC_SPEC_400         3
#define MSHC_SPEC_410         4
#define MSHC_SPEC_420         5

// The value is from the register value of 0xe8.
#define MSHC_PVENDOR_ADDR_OFFSET 0x500
#define MSHC_EMMC_CTRL           (MSHC_PVENDOR_ADDR_OFFSET + 0x2c)
#define MSHC_CARD_IS_EMMC_START  1
#define MSHC_CARD_IS_EMMC_BITS   1
#define MSHC_CARD_IS_EMMC        1
#define MSHC_ENH_STROBE_EN_START 8
#define MSHC_ENH_STROBE_EN_BITS  1
#define MSHC_ENH_STROBE_EN       1

#define MSHC_MSHC_CTRL             (MSHC_PVENDOR_ADDR_OFFSET + 0x08)
#define MSHC_CMD_CONFLIT_CHECK     0x01

#define MSHC_AXI_MBIIU_CTRL        (MSHC_PVENDOR_ADDR_OFFSET + 0x10)
#define MSHC_GM_WR_OSRC_LMT_MASK   (0x7 << 24)
#define MSHC_GM_WR_OSRC_LMT_SEL(x) ((x) << 24)
#define MSHC_GM_RD_OSRC_LMT_MASK   (0x7 << 16)
#define MSHC_GM_RD_OSRC_LMT_SEL(x) ((x) << 16)
#define MSHC_UNDEFL_INCR_EN        0x1
#define MSHC_GM_WR_OSRC_LMT_VAL    ((7) << 24)
#define MSHC_GM_RD_OSRC_LMT_VAL    ((7) << 16)

#define MSHC_AUTO_TUNING_CTRL      (MSHC_PVENDOR_ADDR_OFFSET + 0x40)
#define MSHC_SW_TUNING_EN          0x00000010
#define MSHC_AUTO_TUNING_STATUS    (MSHC_PVENDOR_ADDR_OFFSET + 0x44)

#define MSHC_CENTER_PH_CODE_MASK   0x000000ff
#define MSHC_PHASE_SEL_MASK        0xff
#define MSHC_FOUND_EDGE        (0x1 << 11)
#define MSHC_EDGE_DETECT_EN    (0x1 << 8)
#define MSHC_DOUT_EN_F_EDGE    (0x1 << 6)
#define MSHC_DATA_DLY_EN       (0x1 << 3)
#define MSHC_CMD_DLY_EN        (0x1 << 2)

/*
 * End of controller registers.
 */
#define REG_MISC_CTRL14         0x14
#define SDIO0_PWRSW_CTRL_1V8    (0x1 << 8)
#define SDIO0_PWRSW_EN          (0x1 << 9)
#define SDIO0_PWRSW_SEL_1V8     (0x1 << 10)

#define ADMA2_END               0x2
#define MSHC_PHY_ADDR_OFFSET    0x1000
#define MSHC_PHY_IOCTL_PUPD     (MSHC_PHY_ADDR_OFFSET + 0x260) // IO pul-up/pull-down
#define MSHC_PHY_IOCTL_IOE      (MSHC_PHY_ADDR_OFFSET + 0x26c) // IO direction
#define PHY_IOCTL_PUPD_VAL      0x2FF03FF // cmd and dat[0:7] pull up, DS pull down, clock pull disable
#define PHY_IOCTL_IOE_VAL       0x3FF06FF // cmd and dat[0:7] Input/Output enable, DS input enable, clk output enable
#define MSHC_PHY_IOCTL_RONSEL_1_0   (MSHC_PHY_ADDR_OFFSET + 0x264) // IO driver capability bit 0--bit1
#define MSHC_PHY_IOCTL_OD_RONSEL_2  (MSHC_PHY_ADDR_OFFSET + 0x268) // IO driver capability bit2
#define PHY_IOCTL_RONSEL_BIT0       0x1
#define PHY_IOCTL_RONSEL_BIT1       0x2
#define PHY_IOCTL_RONSEL_BIT2       0x4
#define PHY_IOCTL_RONSEL_BIT0_VAL_1 0x7FF     // value bit0 is 1, bit0--bit10 means [10:0] = {CLK, CMD, DS, DAT7[7:0]}
#define PHY_IOCTL_RONSEL_BIT1_VAL_1 0x7FF0000 // value bit1 is 1, bit16--bit26
#define PHY_IOCTL_RONSEL_BIT2_VAL_1 0x7FF     // value bit2 is 1, bit0--bit10

// This register control the initialization of the PHY.
// Please note that write to this register will be ignored if init_en is already at '1' state.
#define PHYINITCTRL             (MSHC_PHY_ADDR_OFFSET + 0x004)
#define PHYCTRL2                (MSHC_PHY_ADDR_OFFSET + 0x248)
#define DLY_CTL                 (MSHC_PHY_ADDR_OFFSET + 0x250)
#define DLY_CTL1                (MSHC_PHY_ADDR_OFFSET + 0x254)

#define PHY_INV_CLK             0x80000001 // Invert the TXCLK This register create 180 degree delay of TX clock.
#define PHYINITCTRL_DISABLE     0x30c
#define PHYINITCTRL_ENABLE      0x30d
#define PHY_INIT_EN_MASK        0x1
#define PHY_SET_DLY_MEAS_EN     0xC1674800
#define PHY_SET_DLY_MEAS_DIS    0xC1670800
#define PHY_DLAY1_CODE_POS      8
#define PHY_DLAY1_CODE_LEN      10
#define PHY_DLAY3_CODE_POS      21
#define PHY_DLAY3_CODE_LEN      10
#define PHY_DLY_CODE_R_POS      0
#define PHY_DLY_CODE_R_LEN      11

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
