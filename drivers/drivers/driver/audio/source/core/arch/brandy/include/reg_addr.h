/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: reg addr
 * Author: audio
 */

#ifndef __REG_ADDR_H__
#define __REG_ADDR_H__

#define AUDIO_SYS_BASE_ADDR     0xE8000000

#define REG_WDOG_BASE_ADDR      (AUDIO_SYS_BASE_ADDR + 0x003000)
#define DSP_UART_BASE_ADDR      (AUDIO_SYS_BASE_ADDR + 0x005000)
#define SIO_CRG_REG_ADDR        (AUDIO_SYS_BASE_ADDR + 0x007000)
#define DSP_UART_DIV_0_ADDR     (AUDIO_SYS_BASE_ADDR + 0x00700C)
#define DSP_WORK_ON_ADDR        (AUDIO_SYS_BASE_ADDR + 0x0070B0)
#define CODEC_DIG_CFG_REG_ADDR  (AUDIO_SYS_BASE_ADDR + 0x009000)
#define AUDIO_CODEC_BASE_ADDR   (AUDIO_SYS_BASE_ADDR + 0x379000)

#define DSP_WDG_REG_BASE        REG_WDOG_BASE_ADDR
#define WDT_LOAD                0x0000
#define WDT_VAL                 0x0004
#define WDT_CTRL                0x0008
#define WDT_INTCLR              0x000C
#define WDT_RIS                 0x0010
#define WDT_MIS                 0x0014
#define WDT_LOCK                0x0C00

#define SOC_M_CTL_RB            0x52000000

#define SOC_COM_CTL_RB          0x55000000
#define COM_HIFI_CR_CH1_OFFSET  0x0640
#define COM_CH_SEL_OFFSET       0x0AF0

#define SOC_GLB_CTL_M_RB        0x57000000
#define SOC_PMU1_CTL_RB         (SOC_GLB_CTL_M_RB + 0x4000)
#define SOC_AGPIO_MODE_RB       (SOC_GLB_CTL_M_RB + 0xC000)

#define SOC_AUD_DSP_CFG                0x58007000
#define R_DSP_REMAPPING_EN1            (SOC_AUD_DSP_CFG + 0x0064)
#define R_DSP0_REMAPPING_SRC_BASE_ADDR (SOC_AUD_DSP_CFG + 0x0068)
#define R_DSP0_REMAPPING_DES_BASE_ADDR (SOC_AUD_DSP_CFG + 0x0070)

#endif /* __REG_ADDR_H__ */
