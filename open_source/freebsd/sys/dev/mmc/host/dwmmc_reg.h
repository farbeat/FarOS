/*-
 * Copyright (c) 2014 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: releng/12.2/sys/dev/mmc/host/dwmmc_reg.h 287258 2015-08-28 15:27:55Z andrew $
 */

#ifndef _SDMCI_H_
#define _SDMCI_H_

#include "linux/interrupt.h"
#include "bridge.h"
#include "idmac.h"
#include "mmc_adapt_os.h"

/* SDMMC_FIFOTH(0x4c) details */
#define BURST_SIZE		(0x6 << 28)
#define RX_WMARK		(0x7f << 16)
#define TX_WMARK		0x80
#define HI_MCI_IO_SIZE	0x1000

#define	SDMMC_CTRL		0x0	/* Control Register */
#define	 SDMMC_CTRL_USE_IDMAC	(1 << 25)	/* Use Internal DMAC */
#define	 SDMMC_CTRL_DMA_ENABLE	(1 << 5)	/* */
#define	 SDMMC_CTRL_INT_ENABLE	(1 << 4)	/* Enable interrupts */
#define	 SDMMC_CTRL_DMA_RESET	(1 << 2)	/* Reset DMA */
#define	 SDMMC_CTRL_FIFO_RESET	(1 << 1)	/* Reset FIFO */
#define	 SDMMC_CTRL_RESET	(1 << 0)	/* Reset SD/MMC controller */
#define	SDMMC_PWREN		0x4	/* Power Enable Register */
#define	 SDMMC_PWREN_PE		(1 << 0)	/* Power On */
#define	SDMMC_CLKDIV		0x8	/* Clock Divider Register */
#define	SDMMC_CLKSRC		0xC	/* SD Clock Source Register */
#define	SDMMC_CLKENA		0x10	/* Clock Enable Register */
#define	SDMMC_CLKENA_LP		(1 << 16)	/* Low-power mode */
#define	SDMMC_CLKENA_CCLK_EN	(1 << 0)	/* SD/MMC Enable */
#define	SDMMC_TMOUT		0x14	/* Timeout Register */
#define	SDMMC_CTYPE		0x18	/* Card Type Register */
#define	 SDMMC_CTYPE_8BIT	(1 << 16)
#define	 SDMMC_CTYPE_4BIT	(1 << 0)
#define	SDMMC_BLKSIZ		0x1C	/* Block Size Register */
#define	SDMMC_BYTCNT		0x20	/* Byte Count Register */
#define	SDMMC_INTMASK		0x24	/* Interrupt Mask Register */
#define	 SDMMC_INTMASK_SDIO	(1 << 16)	/* SDIO Interrupt Enable */
#define	 SDMMC_INTMASK_EBE	(1 << 15)	/* End-bit error */
#define	 SDMMC_INTMASK_ACD	(1 << 14)	/* Auto command done */
#define	 SDMMC_INTMASK_SBE	(1 << 13)	/* Start-bit error */
#define	 SDMMC_INTMASK_HLE	(1 << 12)	/* Hardware locked write err */
#define	 SDMMC_INTMASK_FRUN	(1 << 11)	/* FIFO underrun/overrun err */
#define	 SDMMC_INTMASK_HTO	(1 << 10)	/* Data starvation by host timeout */
#define	 SDMMC_INTMASK_DRT	(1 << 9)	/* Data read timeout  */
#define	 SDMMC_INTMASK_RTO	(1 << 8)	/* Response timeout */
#define	 SDMMC_INTMASK_DCRC	(1 << 7)	/* Data CRC error */
#define	 SDMMC_INTMASK_RCRC	(1 << 6)	/* Response CRC error */
#define	 SDMMC_INTMASK_RXDR	(1 << 5)	/* Receive FIFO data request */
#define	 SDMMC_INTMASK_TXDR	(1 << 4)	/* Transmit FIFO data request */
#define	 SDMMC_INTMASK_DTO	(1 << 3)	/* Data transfer over */
#define	 SDMMC_INTMASK_CMD_DONE	(1 << 2)	/* Command done */
#define	 SDMMC_INTMASK_RE	(1 << 1)	/* Response error */
#define	 SDMMC_INTMASK_CD	(1 << 0)	/* Card Detected */
#define	SDMMC_CMDARG		0x28	/* Command Argument Register */
#define	SDMMC_CMD		0x2C	/* Command Register */
#define	 SDMMC_CMD_START	(1 << 31)
#define	 SDMMC_CMD_USE_HOLD_REG	(1 << 29)
#define	SDMMC_CMD_VOLT_SWITCH	(1 << 28)	/* Volt Switch, set for cmd11 only */
#define	 SDMMC_CMD_UPD_CLK_ONLY	(1 << 21)	/* Update clk only */
#define SDMMC_CARD_NUMBER0    (0xFFE0FFFF) /* Card number 0*/
#define	 SDMMC_CMD_SEND_INIT	(1 << 15)	/* Send initialization */
#define	 SDMMC_CMD_STOP_ABORT	(1 << 14)	/* stop current data transfer */
#define	 SDMMC_CMD_WAIT_PRVDATA	(1 << 13)	/* Wait for prev data transfer completion */
#define	 SDMMC_CMD_SEND_ASTOP	(1 << 12)	/* Send stop command at end of data tx/rx */
#define	 SDMMC_CMD_MODE_STREAM	(1 << 11)	/* Stream data transfer */
#define	 SDMMC_CMD_DATA_WRITE	(1 << 10)	/* Write to card */
#define	 SDMMC_CMD_DATA_EXP	(1 << 9)	/* Data transfer expected */
#define	 SDMMC_CMD_RESP_CRC	(1 << 8)	/* Check Response CRC */
#define	 SDMMC_CMD_RESP_LONG	(1 << 7)	/* Long response expected */
#define	 SDMMC_CMD_RESP_EXP	(1 << 6)	/* Response expected */
#define	SDMMC_RESP0		0x30	/* Response Register 0 */
#define	SDMMC_RESP1		0x34	/* Response Register 1 */
#define	SDMMC_RESP2		0x38	/* Response Register 2 */
#define	SDMMC_RESP3		0x3C	/* Response Register 3 */
#define	SDMMC_RESP_SIZE		0x4
#define	SDMMC_MINTSTS		0x40	/* Masked Interrupt Status Register */
#define	SDMMC_RINTSTS		0x44	/* Raw Interrupt Status Register */
#define	SDMMC_STATUS		0x48	/* Status Register */
#define	 SDMMC_STATUS_DATA_BUSY	(1 << 9) /* card_data[0] */
#define	 SDMMC_STATUS_FIFO_FULL	(1 << 3) /* FIFO full */
#define	 SDMMC_STATUS_FIFO_EMPTY (1 << 2) /* FIFO empty */
#define	SDMMC_FIFOTH		0x4C	/* FIFO Threshold Watermark Register */
#define	 SDMMC_FIFOTH_MSIZE_S	28	/* Burst size of multiple transaction */
#define	 SDMMC_FIFOTH_RXWMARK_S	16	/* FIFO threshold watermark level */
#define	 SDMMC_FIFOTH_TXWMARK_S	0	/* FIFO threshold watermark level */
#define	SDMMC_CDETECT		0x50	/* Card Detect Register */
#define	SDMMC_CARD_PRESENT	(1 << 0)
#define	SDMMC_WRTPRT		0x54	/* Write Protect Register */
#define	SDMMC_GPIO			0x58	/* Gpio Register */
#define DTO_FIX_BYPASS		(1 << 23)
#define	SDMMC_TCBCNT		0x5C	/* Transferred CIU Card Byte Count */
#define	SDMMC_TBBCNT		0x60	/* Transferred Host to BIU-FIFO Byte Count */
#define	SDMMC_DEBNCE		0x64	/* Debounce Count Register */
#define SDMMC_DEBNCE_MS		25
#define	SDMMC_USRID		0x68	/* User ID Register */
#define	SDMMC_VERID		0x6C	/* Version ID Register */
#define	SDMMC_HCON		0x70	/* Hardware Configuration Register */
#define	SDMMC_UHS_REG		0x74	/* UHS-1 Register */
#define	 SDMMC_UHS_REG_DDR	(1 << 16) /* DDR mode */
#define	SDMMC_RST_N		0x78	/* Hardware Reset Register */
#define SDMMC_RST_N_VALUE	(1 << 0)
#define	SDMMC_BMOD		0x80	/* Bus Mode Register */
#define	SDMMC_BMOD_BURST_16	(3 << 8)
#define	SDMMC_BMOD_BURST_8	(1 << 8)
#define	 SDMMC_BMOD_DE		(1 << 7) /* IDMAC Enable */
#define	 SDMMC_BMOD_FB		(1 << 1) /* AHB Master Fixed Burst */
#define	 SDMMC_BMOD_SWR		(1 << 0) /* Reset DMA */
#define	SDMMC_BMOD_VALUE	(SDMMC_BMOD_FB | SDMMC_BMOD_BURST_16)
#define	SDMMC_PLDMND		0x84	/* Poll Demand Register */
#define	SDMMC_DBADDR		0x88	/* Descriptor List Base Address */
#define	SDMMC_IDSTS		0x8C	/* Internal DMAC Status Register */
#define	SDMMC_IDINTEN		0x90	/* Internal DMAC Interrupt Enable */
#define	 SDMMC_IDINTEN_AI	(1 << 9) /* Abnormal Interrupt Summary */
#define	 SDMMC_IDINTEN_NI	(1 << 8) /* Normal Interrupt Summary */
#define	 SDMMC_IDINTEN_CES	(1 << 5) /* Card Error Summary */
#define	 SDMMC_IDINTEN_DU	(1 << 4) /* Descriptor Unavailable */
#define	 SDMMC_IDINTEN_FBE	(1 << 2) /* Fatal Bus Error */
#define	 SDMMC_IDINTEN_RI	(1 << 1) /* Receive Interrupt */
#define	 SDMMC_IDINTEN_TI	(1 << 0) /* Transmit Interrupt */
#define	 SDMMC_IDINTEN_MASK	(SDMMC_IDINTEN_AI | SDMMC_IDINTEN_NI | SDMMC_IDINTEN_CES | \
				 SDMMC_IDINTEN_DU | SDMMC_IDINTEN_FBE | SDMMC_IDINTEN_RI | \
				 SDMMC_IDINTEN_TI)
#define	SDMMC_DSCADDR		0x94	/* Current Host Descriptor Address */
#define	SDMMC_BUFADDR		0x98	/* Current Buffer Descriptor Address */
#define	SDMMC_CARDTHRCTL	0x100	/* Card Threshold Control Register */
#define	READ_THRESHOLD_SIZE		0x2000005
#define	MCI_BSYCLRINTEN			(1 << 1)
#define	SDMMC_BACK_END_POWER_R	0x104	/* Back End Power Register */
#define	SDMMC_UHS_EXT			0x108	/* UHS Register */
#define	SDMMC_EMMC_DDR_REG		0x10C	/* Emmc DDR Register */
#define	SDMMC_ENABLE_SHIFT		0x110	/* Enable Phase Shift Register */
#define	SDMMC_TUNING_CTRL		0x118	/* Tuning Ctrl Register */
#define	SDMMC_DATA		0x200	/* Data FIFO Access */

/* eMMC */
#define	EMMCP_MPSBEGIN0			0x1200	/*  */
#define	EMMCP_SEND0			0x1204	/*  */
#define	EMMCP_CTRL0			0x120C	/*  */
#define	 MPSCTRL_SECURE_READ_BIT	(1 << 7)
#define	 MPSCTRL_SECURE_WRITE_BIT	(1 << 6)
#define	 MPSCTRL_NON_SECURE_READ_BIT	(1 << 5)
#define	 MPSCTRL_NON_SECURE_WRITE_BIT	(1 << 4)
#define	 MPSCTRL_USE_FUSE_KEY		(1 << 3)
#define	 MPSCTRL_ECB_MODE		(1 << 2)
#define	 MPSCTRL_ENCRYPTION		(1 << 1)
#define	 MPSCTRL_VALID			(1 << 0)
/*
 * MCI_INTMASK(0x24) details:
 * bit 16-1: mask MMC host controller each interrupt
 */
#define	ALL_INT_MASK			0x1ffff
#define	ALL_SD_INT_MASK			0xefff
/*
 * SDMMC_INTSTS(0x44) details :
 * Generally, DTO will be triggered together with DRT, but when we mask DRT in
 * SDMMC_INTMASK, DRT may come separately, we need deal with the DRT in this situation
 */
#define	DATA_INT_MASK	(SDMMC_INTMASK_DTO | SDMMC_INTMASK_DCRC |	\
	    SDMMC_INTMASK_SBE | SDMMC_INTMASK_EBE | SDMMC_INTMASK_DRT)
#define	CMD_INT_MASK	(SDMMC_INTMASK_RTO | SDMMC_INTMASK_RCRC |	\
	    SDMMC_INTMASK_RE | SDMMC_INTMASK_CMD_DONE | SDMMC_INTMASK_HTO)
/*
 * SDMMC_RINTSTS(0x44) details:bit 16-1:
 * clear MMC host controller each interrupt but hardware locked write error interrupt
 */
#define	ALL_INT_CLR		0x1efff

/* SDMMC_CDETECT(0x50)/SDMMC_WRTPRT(0X54) details */
#define	SDMCI_CARD0		(1 << 0)

/* SDMMC_UHS_REG(0x74) details */
#define	HI_SDXC_CTRL_VDD_180	(1 << 0)
#define	HI_SDXC_CTRL_DDR_REG	(1 << 16)

/* SDMMC_UHS_EXT(0x108) details */
#define	SMPL_PHASE_SHIFT		1
#define	CLK_SMPL_PHS_OFFSET		16
#define	CLK_DRV_PHS_OFFSET		23
#define	CLK_SMPL_PHS_MASK		(7 << CLK_SMPL_PHS_OFFSET)
#define	CLK_DRV_PHS_MASK		(7 << CLK_DRV_PHS_OFFSET)

/* SDMMC_TUNING_CTRL(0x118) details */
#define	HI_TUNING_ENABLE	(1 << 0)
#define	HI_FOUND_EDGE		(1 << 5)

#define	EMMC_CARD_WIDTH_1	(2 << 0)
#define	EMMC_CARD_WIDTH_0	(2 << 16)

#define	DRV_PHASE_180	(4 << 23)
#define	DRV_PHASE_135	(3 << 23)
#define	DRV_PHASE_90	(2 << 23)

#define	SMP_PHASE_135	(3 << 16)
#define	SMP_PHASE_45	(1 << 16)
#define	SMP_PHASE_0	(0 << 16)

#define	MMC_RST_N		(1 << 0)

enum card_status {
	card_pluged = 0,
	card_unpluged
};

#define	MAX_RETRY_COUNT		100
#define	SDMCI_PAGE_SIZE		4096

#define	MMC_SPEC_100		0
#define	MMC_SPEC_200		1
#define	MMC_SPEC_300		2
#define	MMC_SPEC_400		3
#define	MMC_SPEC_410		4
#define	MMC_SPEC_420		5

#define	DES0_DIC	(1 << 1)
#define	DES0_LD		(1 << 2)
#define	DES0_FS		(1 << 3)
#define	DES0_CH		(1 << 4)
#define	DES0_ER		(1 << 5)
#define	DES0_CES	(1 << 30)
#define	DES0_OWN	(1 << 31)

/* the slot has no detection PIN, use software detection instead */
#define	 SDMCI_QUIRK_POLL_CARD_PRESENT		(1U << 17)
#define	 SDMCI_QUIRK_ONLY_1_BIT_DATA		(1U << 19)
#define	 SDMCI_QUIRK_SIG_3V3_ONLY		(1U << 21)
#define	 SDMCI_QUIRK_MMC_DDR52			(1U << 23)
#define	 SDMCI_QUIRK_BROKEN_UHS_DDR50		(1U << 24)
#define	 SDMCI_QUIRK_BROKEN_MMC_HS200		(1U << 25)
#define	 SDMCI_QUIRK_BROKEN_AUTO_STOP		(1U << 28)
#define	 SDMCI_QUIRK_SIG_1V8_ONLY		(1U << 29)
#define	 SDMCI_QUIRK_BROKEN_ACMD23		(1U << 31)

#define	SDMCI_LOCK(_slot)			mmc_mtx_lock(&((_slot)->slot_mtx))
#define	SDMCI_UNLOCK(_slot)			mmc_mtx_unlock(&((_slot)->slot_mtx))
#define	SDMCI_LOCK_INIT(_slot)			mmc_mtx_init(&((_slot)->slot_mtx), NULL, NULL, 0)
#define	SDMCI_LOCK_DESTROY(_slot)		mmc_mtx_destroy(&((_slot)->slot_mtx))
#define	SDMCI_BUS_LOCK(_slot)			mmc_mtx_lock(&((_slot)->bus_mtx))
#define	SDMCI_BUS_UNLOCK(_slot)			mmc_mtx_unlock(&((_slot)->bus_mtx))
#define	SDMCI_BUS_LOCK_INIT(_slot)		mmc_mtx_init(&((_slot)->bus_mtx), NULL, NULL, 0)
#define	SDMCI_BUS_LOCK_DESTROY(_slot)		mmc_mtx_destroy(&((_slot)->bus_mtx))

enum task_action {
	task_action_run,
	task_action_exit
};

typedef struct tag_sdmci_slot {
	MMC_MUTEX slot_mtx;	/* Slot mutex */
	MMC_MUTEX bus_mtx;	/* Bus mutex */
	struct mmc_host host;	/* Host parameters */
	u_int quirks;		/* Chip specific quirks */
	device_t dev;		/* Slot device */
	uint32_t num;		/* Slot number */
#define	SDMCI_SDR50_NEEDS_TUNING	0x20
	u_int opt;			/* Slot options */
	u_char version;
	struct mmc_request *req;	/* Current request */

#define	SDMCI_NON_REMOVABLE		0x04	/* Used for EMMC/SDIO slot */
#define	SDMCI_TUNING_SUPPORTED	0x08
#define	SDMCI_TUNING_ENABLED	0x10
#define	SDMCI_SLOT_EMBEDDED		0x40	/* Used for EMMC slot */
#define	SDMCI_SLOT_SDIO			0x80
#define	SDMCI_PEND_DTO			1
#define	SDMCI_ACQUIRE_HOST (1 << 1)
	EVENT_CB_S req_event;	/* Request event */
#define	SDMCI_HAVE_DMA			0x01
	hci_slot_ops *ops;
	device_t bus;
	idamc dma;
	uint32_t max_clk;		/* Max possible freq */
	uint32_t min_clk;		/* Min possible freq */
	uint32_t timeout_clk;	/* Timeout freq */
	u_int caps;				/* Host capabilities */
	u_int caps2;			/* Override CAPABILITIES2 */
	int timeout;			/* Transfer timeout */
	uint16_t swtmr;			/* Software timer for cmd/data response timeout */
	uint32_t cd_tsk_id;		/* Card detect task */
	struct mmc_command *curcmd; /* Current command of current request */
#define	CMD_STARTED		(1 << 1)
#define	STOP_STARTED	(1 << 2)
#define	BC_STARTED		(1 << 3)
	u_char flags;		/* Request execution flags */
	u_char cmd_done;	/* CMD command part done flag */
#define	SDMCI_RETUNE_REQ_NEEDED		0x01	/* Re-tuning w/o circuit reset needed */
#define	SDMCI_RETUNE_REQ_RESET 		0x02	/* Re-tuning w/ circuit reset needed */
	uint8_t retune_req;	/* Re-tuning request status */
	u_char data_done;	/* DAT command part done flag */
	u_char power;		/* Current power */
	uint32_t clock;		/* Current clock freq. */
	uint32_t timing;
	uint32_t width;
	uint32_t error_count;	/* Error count of the current card */
	uint32_t intmask;		/* Current interrupt mask */
#define	SDMCI_TUNE_PHASE_NONE	0xffffffff
	uint32_t tune_phase;	/* Tuning result when the tune is done */
	uint32_t is_tuning;
} sdmci_slot;

extern int tuning_reset_flag;
extern device_t mmc_new_device(device_t busdev);
extern void mmc_del_device(device_t dev);
extern int mmc_device_del_child(device_t child);

extern int mmc_attach(device_t dev);
int mmc_detach(device_t dev);

void sdmci_clock_enable(const sdmci_slot *slot, bool en);
void sdmci_start(sdmci_slot *slot);
void sdmci_slot_reset(const sdmci_slot *slot);
uint32_t sdmci_card_state(const sdmci_slot *slot);

#ifdef __OHOS__
int sdmci_attach(device_t pdev);
int sdmci_detach(device_t pdev);
#else
int sdmci_attach(struct platform_device *pdev);
int sdmci_detach(struct platform_device *pdev);
#endif

#endif /* _SDMCI_H_ */
