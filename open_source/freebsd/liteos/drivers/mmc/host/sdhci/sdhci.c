/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2008 Alexander Motin <mav@FreeBSD.org>
 * Copyright (c) 2017 Marius Strobl <marius@FreeBSD.org>
 * All rights reserved.
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/12.2/sys/dev/sdhci/sdhci.c 353675 2019-10-17 01:30:37Z ian $");

#include "sdhci.h"
#include "sdhci_private.h"
#include "mmc_adapt.h"
#include "mmcbrvar.h"
#include "soc_mmc.h"
#include "dpal.h"
#include "securec.h"
#include "mmc_adapt_os.h"

const static int sdhci_debug = 0;

#define	SDHCI_GET_CARD_PRESENT(b)	(((hci_ops *)get_mmcbus_ops(b))->get_card_present(b))
#define	SDHCI_IS_TUNING(cmd)	(cmd->opcode == MMC_SEND_TUNING_BLOCK ||	\
    cmd->opcode == MMC_SEND_TUNING_BLOCK_HS200)

#ifdef MMCCAM
/* CAM-related */
static void sdhci_cam_action(struct cam_sim *sim, union ccb *ccb);
static int sdhci_cam_get_possible_host_clock(const struct sdhci_slot *slot,
		    int proposed_clock);
static void sdhci_cam_handle_mmcio(struct cam_sim *sim, union ccb *ccb);
static void sdhci_cam_poll(struct cam_sim *sim);
static int sdhci_cam_request(struct sdhci_slot *slot, union ccb *ccb);
static int sdhci_cam_settran_settings(struct sdhci_slot *slot, union ccb *ccb);
static int sdhci_cam_update_ios(struct sdhci_slot *slot);
#endif

static int sdhci_exec_tuning(struct sdhci_slot *slot, bool reset);
static void sdhci_set_clock(struct sdhci_slot *slot, uint32_t clock);
static void sdhci_start_data(struct sdhci_slot *slot, const struct mmc_data *data);
static void sdhci_req_done(struct sdhci_slot *slot);
static uint32_t sdhci_tuning_intmask(const struct sdhci_slot *slot);

#define	SDHCI_DEFAULT_MAX_FREQ	50

#define	SDHCI_200_MAX_DIVIDER	256
#define	SDHCI_300_MAX_DIVIDER	2046

#define	SDHCI_CARD_PRESENT_TICKS	(hz / 5)
#define	SDHCI_INSERT_DELAY_TICKS	(hz / 2)

/*
 * Broadcom BCM577xx Controller Constants
 */
/* Maximum divider supported by the default clock source. */
#define	BCM577XX_DEFAULT_MAX_DIVIDER	256
/* Alternative clock's base frequency. */
#define	BCM577XX_ALT_CLOCK_BASE		63000000

#define	BCM577XX_HOST_CONTROL		0x198
#define	BCM577XX_CTRL_CLKSEL_MASK	0xFFFFCFFF
#define	BCM577XX_CTRL_CLKSEL_SHIFT	12
#define	BCM577XX_CTRL_CLKSEL_DEFAULT	0x0
#define	BCM577XX_CTRL_CLKSEL_64MHZ	0x3

#define	__predict_true(exp)	exp
#define	__predict_false(exp)	exp

#ifdef DPALCFG_DRIVERS_MMC_DBG
static void
sdhci_dump_irqstatus(const struct sdhci_slot *slot)
{
	struct mmc_request *req = slot->req;
	if (slot->req != NULL) {
		if (req->bc != NULL)
			slot_printf(slot, "req->bc: cmd:0x%x irq_status:0x%x\n", req->bc->opcode, req->bc->irq_status);
		if (req->cmd != NULL)
			slot_printf(slot, "req->cmd: cmd:0x%x irq_status:0x%x\n", req->cmd->opcode, req->cmd->irq_status);
		if (req->stop != NULL)
			slot_printf(slot, "req->stop: cmd:0x%x irq_status:0x%x\n", req->stop->opcode, req->stop->irq_status);
	}
}

#ifndef TRUNCATE
#define truncate(addr, size) ((uintptr_t)(addr) & ~((size) - 1))
#endif

#ifndef IS_ALIGNED
#define is_aligned(value, align_size) ((((uintptr_t)(value)) & ((uintptr_t)((align_size) - 1))) == 0)
#endif
static void sdhci_dump_mem_byte(size_t length, uintptr_t addr)
{
    debug_printf();
    size_t data_len;
    uintptr_t *align_addr = NULL;
    uint32_t count = 0;

    data_len = dpal_align(length, sizeof(uintptr_t));
    align_addr = (uintptr_t *)truncate(addr, sizeof(uintptr_t));
    if ((data_len == 0) || (align_addr == NULL)) {
        return;
    }
    while (data_len != 0) {
        if (is_aligned(count, sizeof(char *))) {
            dprintf("\n 0x%lx :", align_addr);
        }
#ifdef __LP64__
        dprintf("%0+16lx ", *align_addr);
#else
        dprintf("%0+8lx ", *align_addr);
#endif
        align_addr++;
        data_len -= sizeof(char *);
        count++;
    }
    dprintf("\n");
    return;
}

#endif

void
sdhci_dump_regs(const struct sdhci_slot *slot)
{
#ifdef DPALCFG_DRIVERS_MMC_DBG
	device_t dev = slot->bus;
	struct hci_softc *sc = device_get_softc(dev);
	if (sc == NULL) {
		slot_printf(slot, "ERR: sc is NULL in %s\n", __FUNCTION__);
		return;
	}

	sdhci_dump_irqstatus(slot);

	slot_printf(slot,
	    "============== REGISTER DUMP ==============\n");
	slot_printf(slot, "Sys addr: 0x%08x | Version:  0x%08x\n",
	    RD4(slot, SDHCI_DMA_ADDRESS), RD2(slot, SDHCI_HOST_VERSION));
	slot_printf(slot, "Blk size: 0x%08x | Blk cnt:  0x%08x\n",
	    RD2(slot, SDHCI_BLOCK_SIZE), RD2(slot, SDHCI_BLOCK_COUNT));
	slot_printf(slot, "Argument: 0x%08x | Trn mode: 0x%08x\n",
	    RD4(slot, SDHCI_ARGUMENT), RD2(slot, SDHCI_TRANSFER_MODE));
	slot_printf(slot, "Present:  0x%08x | Host ctl: 0x%08x\n",
	    RD4(slot, SDHCI_PRESENT_STATE), RD1(slot, SDHCI_HOST_CONTROL));
	slot_printf(slot, "Power:    0x%08x | Blk gap:  0x%08x\n",
	    RD1(slot, SDHCI_POWER_CONTROL), RD1(slot, SDHCI_BLOCK_GAP_CONTROL));
	slot_printf(slot, "Wake-up:  0x%08x | Clock:    0x%08x\n",
	    RD1(slot, SDHCI_WAKE_UP_CONTROL), RD2(slot, SDHCI_CLOCK_CONTROL));
	slot_printf(slot, "Timeout:  0x%08x | Int stat: 0x%08x\n",
	    RD1(slot, SDHCI_TIMEOUT_CONTROL), RD4(slot, SDHCI_INT_STATUS));
	slot_printf(slot, "Int enab: 0x%08x | Sig enab: 0x%08x\n",
	    RD4(slot, SDHCI_INT_ENABLE), RD4(slot, SDHCI_SIGNAL_ENABLE));
	slot_printf(slot, "AC12 err: 0x%08x | Slot int: 0x%08x\n",
	    RD2(slot, SDHCI_ACMD12_ERR), RD2(slot, SDHCI_SLOT_INT_STATUS));
	slot_printf(slot, "Caps:     0x%08x | Caps2:    0x%08x\n",
	    RD4(slot, SDHCI_CAPABILITIES), RD4(slot, SDHCI_CAPABILITIES2));
	slot_printf(slot, "Cmdflag:  0x%08x | Max curr: 0x%08x\n",
	    RD2(slot, SDHCI_COMMAND_FLAGS), RD4(slot, SDHCI_MAX_CURRENT));
	slot_printf(slot, "Resp 1:   0x%08x | Resp 0:   0x%08x\n",
	    RD4(slot, SDHCI_RESPONSE + 0x4), RD4(slot, SDHCI_RESPONSE));
	slot_printf(slot, "Resp 3:   0x%08x | Resp 2:   0x%08x\n",
	    RD4(slot, SDHCI_RESPONSE + 0xC), RD4(slot, SDHCI_RESPONSE + 0x8));
	slot_printf(slot, "Hostctrl2:0x%08x | opcode:     %u\n",
	    RD2(slot, SDHCI_HOST_CONTROL2), (RD2(slot, SDHCI_COMMAND_FLAGS) >> 8) & 0x3f);
	slot_printf(slot, "Emmcctrl: 0x%08x | Multi cycle 0x%08x\n",
	    RD2(slot, SDHCI_EMMC_CTRL), RD4(slot, SDHCI_MULTI_CYCLE));
	slot_printf(slot, "ADMA Err: 0x%08x | ADMA Ptr: 0x%08x\n",
	    RD4(slot, SDHCI_ADMA_ERR), RD4(slot, SDHCI_ADMA_ADDRESS_LO));
	/* 0x80: length of former host register */
	sdhci_dump_mem_byte(0x80, (uintptr_t)sc->base);
	/* 0x50: length of latter host register (offset 0x500) */
	sdhci_dump_mem_byte(0x50, (uintptr_t)sc->base + 0x500);
	slot_printf(slot, "==================BOARD REGS ==============\n");
	if (slot->ops->dump_regs != NULL) {
		slot->ops->dump_regs(sc);
	}
#endif
}

void
sdhci_reset(struct sdhci_slot *slot, uint8_t mask)
{
	int timeout;
	uint32_t clock;

	if (slot->quirks & SDHCI_QUIRK_NO_CARD_NO_RESET) {
		if (!SDHCI_GET_CARD_PRESENT(slot->bus))
			return;
	}

	/* Some controllers need this kick or reset won't work. */
	if ((mask & SDHCI_RESET_ALL) == 0 &&
	    (slot->quirks & SDHCI_QUIRK_CLOCK_BEFORE_RESET)) {
		/* This is to force an update */
		clock = slot->clock;
		slot->clock = 0;
		sdhci_set_clock(slot, clock);
	}

	if (mask & SDHCI_RESET_ALL) {
		slot->clock = 0;
		slot->power = 0;
	}

	WR1(slot, SDHCI_SOFTWARE_RESET, mask);

	if (slot->quirks & SDHCI_QUIRK_WAITFOR_RESET_ASSERTED) {
		/*
		 * Resets on TI OMAPs and AM335x are incompatible with SDHCI
		 * specification.  The reset bit has internal propagation delay,
		 * so a fast read after write returns 0 even if reset process is
		 * in progress.  The workaround is to poll for 1 before polling
		 * for 0.  In the worst case, if we miss seeing it asserted the
		 * time we spent waiting is enough to ensure the reset finishes.
		 */
		timeout = 10000;
		while ((RD1(slot, SDHCI_SOFTWARE_RESET) & mask) != mask) {
			if (timeout <= 0)
				break;
			timeout--;
			DELAY(1);
		}
	}

	/* Wait max 100 ms */
	timeout = 10000;
	/* Controller clears the bits when it's done */
	while (RD1(slot, SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout <= 0) {
			slot_printf(slot, "Reset 0x%x never completed.\n",
			    mask);
			sdhci_dump_regs(slot);
			return;
		}
		timeout--;
		DELAY(10);
	}
}

static uint32_t
sdhci_tuning_intmask(const struct sdhci_slot *slot)
{
	uint32_t intmask;

	intmask = 0;
	if (slot->opt & SDHCI_TUNING_ENABLED) {
		intmask |= SDHCI_INT_TUNEERR;
		if (slot->retune_mode == SDHCI_RETUNE_MODE_2 ||
		    slot->retune_mode == SDHCI_RETUNE_MODE_3)
			intmask |= SDHCI_INT_RETUNE;
	}
	return (intmask);
}

static void
sdhci_init(struct sdhci_slot *slot)
{
	struct hci_softc *sc;
	sdhci_reset(slot, SDHCI_RESET_ALL);

	/* Enable interrupts. */
	slot->intmask = SDHCI_INT_BUS_POWER | SDHCI_INT_DATA_END_BIT |
	    SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_INDEX |
	    SDHCI_INT_END_BIT | SDHCI_INT_CRC | SDHCI_INT_TIMEOUT |
	    SDHCI_INT_DMA_END | SDHCI_INT_ADMAERR | SDHCI_INT_DATA_END | SDHCI_INT_RESPONSE |
	    SDHCI_INT_ACMD12ERR;

	WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
	WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);

	/* board init */
	if (slot->ops->extra_init != NULL) {
		sc = device_get_softc(slot->bus);
		slot->ops->extra_init(sc);
	}
}

void
host_hw_init(void *slot)
{
	sdhci_init((struct sdhci_slot *)slot);
}

static void
sdhci_set_clock(struct sdhci_slot *slot, uint32_t clock)
{
	uint32_t clk_base;
	uint16_t clk;
	int timeout;
	struct hci_softc *sc = device_get_softc(slot->bus);

	if (clock > slot->max_clk)
		clock = slot->max_clk;

	if (clock == slot->clock)
		return;
	slot->clock = clock;

	/* Turn off the clock. */
	clk = RD2(slot, SDHCI_CLOCK_CONTROL);
	clk &= ~SDHCI_CLOCK_CARD_EN;
	WR2(slot, SDHCI_CLOCK_CONTROL, clk);
	DELAY(25);
	/* Turn off card_clk_en */
	clk &= ~(SDHCI_CLOCK_INT_EN | SDHCI_CLOCK_PLL_EN);
	WR2(slot, SDHCI_CLOCK_CONTROL, clk);

	/* If no clock requested - leave it so. */
	if (clock == 0)
		return;

	if (slot->ops->pre_clock != NULL)
		slot->ops->pre_clock(sc);
	if (slot->ops->set_phase != NULL)
		slot->ops->set_phase(sc, slot->host.ios.timing);
	if (slot->ops->set_clock != NULL) {
		clk_base = slot->ops->set_clock(sc, clock);
		if (!clk_base) {
			dev_loge(slot->bus, "Set clock fail!\n");
			return;
		}
	}
	if (slot->ops->post_clock != NULL && slot->opt != SDHCI_NON_REMOVABLE)
		slot->ops->post_clock(sc);

	/* Enable clock. */
	clk = RD2(slot, SDHCI_CLOCK_CONTROL);
	clk |= (SDHCI_CLOCK_INT_EN | SDHCI_CLOCK_PLL_EN);
	WR2(slot, SDHCI_CLOCK_CONTROL, clk);
	/* Wait up to 10 ms until it stabilize. */
	timeout = 10;
	while (!((clk = RD2(slot, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			slot_printf(slot,
			    "Internal clock never stabilised.\n");
			sdhci_dump_regs(slot);
			return;
		}
		timeout--;
		DELAY(1000);
	}

	if (slot->ops->post_clock != NULL)
		slot->ops->post_clock(sc);
	/* Pass clock signal to the bus. */
	clk |= SDHCI_CLOCK_CARD_EN;
	WR2(slot, SDHCI_CLOCK_CONTROL, clk);
}

static void
sdhci_set_power(struct sdhci_slot *slot, u_char power)
{
	int i;
	uint8_t pwr;

	if (slot->power == power)
		return;

	slot->power = power;

	/* Turn off the power. */
	pwr = 0;
	WR1(slot, SDHCI_POWER_CONTROL, pwr);
	mmc_delay_ms(100); /* Controllers need an extra 100ms delay to ensure power off completely */
	/* If power down requested - leave it so. */
	if (power == 0)
		return;
	/* Set voltage. */
	switch (1 << power) {
	case MMC_OCR_LOW_VOLTAGE:
		pwr |= SDHCI_POWER_180;
		break;
	case MMC_OCR_290_300:
	case MMC_OCR_300_310:
		pwr |= SDHCI_POWER_300;
		break;
	case MMC_OCR_320_330:
	case MMC_OCR_330_340:
		pwr |= SDHCI_POWER_330;
		break;
	}
	WR1(slot, SDHCI_POWER_CONTROL, pwr);
	/*
	 * Turn on VDD1 power.  Note that at least some Intel controllers can
	 * fail to enable bus power on the first try after transiting from D3
	 * to D0, so we give them up to 2 ms.
	 */
	pwr |= SDHCI_POWER_ON;
	for (i = 0; i < 20; i++) {
		WR1(slot, SDHCI_POWER_CONTROL, pwr);
		if (RD1(slot, SDHCI_POWER_CONTROL) & SDHCI_POWER_ON)
			break;
		DELAY(100);
	}
	if (!(RD1(slot, SDHCI_POWER_CONTROL) & SDHCI_POWER_ON))
		slot_printf(slot, "Bus power failed to enable\n");
}

static void
sdhci_card_detect_unplugged(struct sdhci_slot *slot)
{
	int err;
	device_t d;

	mmc_resource_mutex_lock();
	SDHCI_LOCK(slot);
	dev_logt(slot->bus, "watch unplug event!");
	slot->host.card_present = MMC_CAP_CARD_ABSENT;
	if (slot->req != NULL) {
		dev_logt(slot->bus, "unplug event occur when req is pending!");
	}
	/* If no card present - detach mmc bus. */
	if (!(slot->opt & SDHCI_NON_REMOVABLE))
		slot_printf(slot, "Card removed\n");
	if (slot->dev != NULL) {
		d = slot->dev;
		slot->dev = NULL;
		slot->intmask &= ~sdhci_tuning_intmask(slot);
		WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
		WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);
		slot->opt &= ~SDHCI_TUNING_ENABLED;
		slot->error_count = 0;
		SDHCI_UNLOCK(slot);
		err = mmc_detach(d);
		if (err != 0) {
			dev_logd(slot->bus, "mmc detach failed!\n");
		}
		SDHCI_LOCK(slot);
		mmc_del_device(slot->bus);
	} else
		dev_logw(slot->bus, "unplugged event occur, but slot->dev has not exist.\n");
	SDHCI_UNLOCK(slot);
	dev_logt(slot->bus, "_EVENT_UNPLUGGED end\n");
	mmc_resource_mutex_unlock();
}

static void
sdhci_card_detect_plugged(struct sdhci_slot *slot)
{
	device_t d;

	if (!(slot->opt & SDHCI_NON_REMOVABLE))
		/* If card is present - attach mmc bus. */
		slot_printf(slot, "Card inserted\n");
	mmc_resource_mutex_lock();
	SDHCI_LOCK(slot);
	if (slot->dev == NULL) {
		sdhci_init(slot);
		d = mmc_new_device(slot->bus);
		slot->dev = d;
		SDHCI_UNLOCK(slot);
		if (d) {
			device_set_ivars(d, slot);
			if (mmc_attach(d) != 0) {
				mmc_del_device(slot->bus);
				slot->dev = NULL;
			}
			slot->host.card_present = MMC_CAP_CARD_PRESENT;
		}
	} else {
		SDHCI_UNLOCK(slot);
		dev_logw(slot->bus, "plugged event occur, but slot->dev has existed.\n");
	}
	dev_logt(slot->bus, "_EVENT_PLUGGED end\n");
	mmc_resource_mutex_unlock();
}
void
sdhci_card_detect(void *arg, uint32_t event)
{
	struct hci_softc *sc = (struct hci_softc *)arg;
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;

	if (event & HCI_EVENT_UNPLUGGED)
		sdhci_card_detect_unplugged(slot);

	if (event & HCI_EVENT_PLUGGED)
		sdhci_card_detect_plugged(slot);
}

#define	SDHCI_PLUG_STATE(sc)	(hci_readl(sc, SDHCI_PRESENT_STATE) & SDHCI_CARD_PRESENT)

uint32_t
sdhci_card_state(struct sdhci_slot *slot)
{
	struct hci_softc *sc = device_get_softc(slot->bus);

	if (slot->opt & SDHCI_NON_REMOVABLE)
		return (MMC_CAP_CARD_PRESENT);

	return (SDHCI_PLUG_STATE(sc) ? MMC_CAP_CARD_PRESENT : MMC_CAP_CARD_ABSENT);
}


int
sdhci_init_slot(device_t dev, struct sdhci_slot *slot, int num)
{
	uint32_t caps, caps2, freq, host_caps;
	uint32_t err;

	hci_ops *ops = get_mmcbus_ops(slot->bus);
	struct hci_softc *sc = device_get_softc(slot->bus);

	err = SDHCI_LOCK_INIT(slot);
	if (err)
		return (err);

	err = dpal_mux_create(&(slot->bus_mtx.mux_id));
	if (err) {
		SDHCI_LOCK_DESTROY(slot);
		return (err);
	}

	slot->num = num;
	slot->bus = dev;

#define SDMCI_ADMA_SLICES    128

	/* Allocate ADMA memory. */
	err = sdhci_adma2_create(&slot->adma2, SDMCI_ADMA_SLICES, ADMA2_ATTR_NOPDESC);
	if (err) {
		dpal_mux_delete(slot->bus_mtx.mux_id);
		SDHCI_LOCK_DESTROY(slot);
		return (err);
	}

	(void)dpal_event_init(&slot->req_event);

	slot->version = (RD2(slot, SDHCI_HOST_VERSION)
		>> SDHCI_SPEC_VER_SHIFT) & SDHCI_SPEC_VER_MASK;
	if (slot->quirks & SDHCI_QUIRK_MISSING_CAPS) {
		caps = slot->caps;
		caps2 = slot->caps2;
	} else {
		caps = RD4(slot, SDHCI_CAPABILITIES);
		if (slot->version >= SDHCI_SPEC_300)
			caps2 = RD4(slot, SDHCI_CAPABILITIES2);
		else
			caps2 = 0;
	}
	if (slot->version >= SDHCI_SPEC_300) {
		if ((caps & SDHCI_SLOTTYPE_MASK) == SDHCI_SLOTTYPE_SDIO)
			slot->opt |= (SDHCI_SLOT_SDIO | SDHCI_NON_REMOVABLE);
		if ((caps & SDHCI_SLOTTYPE_MASK) == SDHCI_SLOTTYPE_EMBEDDED)
			slot->opt |= SDHCI_SLOT_EMBEDDED | SDHCI_NON_REMOVABLE;
	}
	/* Calculate base clock frequency. */
	if (slot->version >= SDHCI_SPEC_300)
		freq = (caps & SDHCI_CLOCK_V3_BASE_MASK) >>
		    SDHCI_CLOCK_BASE_SHIFT;
	else
		freq = (caps & SDHCI_CLOCK_BASE_MASK) >>
		    SDHCI_CLOCK_BASE_SHIFT;
	if (slot->max_clk == 0 && freq != 0 && freq < 64)
		slot->max_clk = freq * 1000000;
	/*
	 * If the frequency wasn't in the capabilities and the hardware driver
	 * hasn't already set max_clk we're probably not going to work right
	 * with an assumption, so complain about it.
	 */
	if (slot->max_clk == 0) {
		slot->max_clk = SDHCI_DEFAULT_MAX_FREQ * 1000000;
		slot_printf(slot, "Hardware doesn't specify base clock "
		    "frequency, using %dMHz as default.\n",
		    SDHCI_DEFAULT_MAX_FREQ);
	}
	/* Calculate/set timeout clock frequency. */
	if (slot->quirks & SDHCI_QUIRK_DATA_TIMEOUT_USES_SDCLK) {
		slot->timeout_clk = slot->max_clk / 1000;
	} else if (slot->quirks & SDHCI_QUIRK_DATA_TIMEOUT_1MHZ) {
		slot->timeout_clk = 1000;
	} else {
		slot->timeout_clk = (caps & SDHCI_TIMEOUT_CLK_MASK) >>
		    SDHCI_TIMEOUT_CLK_SHIFT;
		if (caps & SDHCI_TIMEOUT_CLK_UNIT)
			slot->timeout_clk *= 1000;
	}
	/*
	 * If the frequency wasn't in the capabilities and the hardware driver
	 * hasn't already set timeout_clk we'll probably work okay using the
	 * max timeout, but still mention it.
	 */
	if (slot->timeout_clk == 0) {
		slot_printf(slot, "Hardware doesn't specify timeout clock "
		    "frequency, setting BROKEN_TIMEOUT quirk.\n");
		slot->quirks |= SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;
	}

	slot->host.f_min = slot->min_clk;
	slot->host.f_max = slot->max_clk;
	slot->host.host_ocr = 0;
	if (caps & SDHCI_CAN_VDD_330)
	    slot->host.host_ocr |= MMC_OCR_320_330 | MMC_OCR_330_340;
	else if (caps & SDHCI_CAN_VDD_300)
	    slot->host.host_ocr |= MMC_OCR_290_300 | MMC_OCR_300_310;
	/*
	 * 1.8V VDD is not supposed to be used for removable cards.  Hardware
	 * prior to v3.0 had no way to indicate embedded slots, but did
	 * sometimes support 1.8v for non-removable devices.
	 */
	if ((caps & SDHCI_CAN_VDD_180) &&
	    (slot->version < SDHCI_SPEC_300 || (slot->opt & SDHCI_SLOT_EMBEDDED)))
	    slot->host.host_ocr |= MMC_OCR_LOW_VOLTAGE;
	if (slot->host.host_ocr == 0) {
		slot_printf(slot, "Hardware doesn't report any "
		    "support voltages.\n");
	}
#ifdef CONFIG_BOARD_TYPE_ASIC
	host_caps = MMC_CAP_8_BIT_DATA;
#else
	host_caps = MMC_CAP_4_BIT_DATA;
#endif
	if (caps & SDHCI_CAN_DO_8BITBUS)
		host_caps |= MMC_CAP_8_BIT_DATA;
	if (slot->quirks & SDHCI_QUIRK_ONLY_1_BIT_DATA)
		host_caps &= ~(MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA);

	if (caps & SDHCI_CAN_DO_HISPD)
		host_caps |= MMC_CAP_HSPEED;

	if (slot->quirks & SDHCI_QUIRK_WAIT_WHILE_BUSY)
		host_caps |= MMC_CAP_WAIT_WHILE_BUSY;

	/* Determine supported UHS-I and eMMC modes. */
	host_caps |= MMC_CAP_MAX_CURRENT_200;	/* default after reset */
	if (caps2 & (SDHCI_CAN_SDR50 | SDHCI_CAN_SDR104 | SDHCI_CAN_DDR50)) {
		host_caps |= MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25;
		host_caps &= ~MMC_CAP_MAX_CURRENT_200;
		/* the val read from the maxcurr register(SDHCI_MAX_CURRENT, 0x48) is all zeros */
		host_caps |= MMC_CAP_MAX_CURRENT_400;
	}

	if (caps2 & SDHCI_CAN_SDR104) {
		host_caps |= MMC_CAP_UHS_SDR104 | MMC_CAP_UHS_SDR50;
		if (!(slot->quirks & SDHCI_QUIRK_BROKEN_MMC_HS200))
			host_caps |= MMC_CAP_MMC_HS200;
	} else if (caps2 & SDHCI_CAN_SDR50)
		host_caps |= MMC_CAP_UHS_SDR50;
	if (caps2 & SDHCI_CAN_DDR50 &&
	    !(slot->quirks & SDHCI_QUIRK_BROKEN_UHS_DDR50))
		host_caps |= MMC_CAP_UHS_DDR50;
	if (slot->quirks & SDHCI_QUIRK_MMC_DDR52)
		host_caps |= MMC_CAP_MMC_DDR52;
	if (caps2 & SDHCI_CAN_MMC_HS400)
		host_caps |= MMC_CAP_MMC_HS400;

	if (caps2 & SDHCI_CAN_MMC_HS400ES)
		host_caps |= MMC_CAP_MMC_HS400 | MMC_CAP_MMC_ENH_STROBE;

	/*
	 * Disable UHS-I and eMMC modes if the set_uhs_timing method is the
	 * default NULL implementation.
	 */
	if (slot->quirks & SDHCI_QUIRK_SIG_3V3_ONLY) {
		if (!(slot->opt & SDHCI_NON_REMOVABLE) || (slot->opt & SDHCI_SLOT_SDIO)) { /* SD Cards or SDIO dev */
			host_caps &= ~(MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
				MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_DDR50 | MMC_CAP_UHS_SDR104);
		} else if (slot->opt & SDHCI_SLOT_EMBEDDED) { /* eMMC */
			host_caps &= ~(MMC_CAP_MMC_DDR52 | MMC_CAP_MMC_HS200 |
				MMC_CAP_MMC_HS400 | MMC_CAP_MMC_ENH_STROBE);
		} else {
			/* do nothing */
		}
	}

#define	SDHCI_CAP_MODES_TUNING(caps2)					\
    (((caps2) & SDHCI_TUNE_SDR50 ? MMC_CAP_UHS_SDR50 : 0) |		\
    MMC_CAP_UHS_DDR50 | MMC_CAP_UHS_SDR104 | MMC_CAP_MMC_HS200 |	\
    MMC_CAP_MMC_HS400)

	/*
	 * Disable UHS-I and eMMC modes that require (re-)tuning if either
	 * the tune or re-tune method is the default NULL implementation.
	 */
	if ((ops->tune == NULL) || (ops->retune == NULL))
		host_caps &= ~(SDHCI_CAP_MODES_TUNING(caps2));

	/* Allocate tuning structures and determine tuning parameters. */
	if (host_caps & SDHCI_CAP_MODES_TUNING(caps2)) {
		slot->opt |= SDHCI_TUNING_SUPPORTED;
        slot->tune_req = dpal_malloc(sizeof(*slot->tune_req));
		if (slot->tune_req == NULL) {
			err = ENOMEM;
			goto error;
		}
        slot->tune_cmd = dpal_malloc(sizeof(*slot->tune_cmd));
		if (slot->tune_cmd == NULL) {
			err = ENOMEM;
			goto error;
		}
        slot->tune_data = dpal_malloc(sizeof(*slot->tune_data));
		if (slot->tune_data == NULL) {
			err = ENOMEM;
			goto error;
		}
		if (caps2 & SDHCI_TUNE_SDR50)
			slot->opt |= SDHCI_SDR50_NEEDS_TUNING;
		slot->retune_mode = (caps2 & SDHCI_RETUNE_MODES_MASK) >>
		    SDHCI_RETUNE_MODES_SHIFT;
		if (slot->retune_mode == SDHCI_RETUNE_MODE_1) {
			slot->retune_count = (caps2 & SDHCI_RETUNE_CNT_MASK) >>
			    SDHCI_RETUNE_CNT_SHIFT;
			if (slot->retune_count > 0xb) {
				slot_printf(slot, "Unknown re-tuning count "
				    "%x, using 1 sec\n", slot->retune_count);
				slot->retune_count = 1;
			} else if (slot->retune_count != 0)
				slot->retune_count =
				    1 << (slot->retune_count - 1);
		}
	}

#undef SDHCI_CAP_MODES_TUNING

	/* Determine supported VCCQ signaling levels. */
	host_caps |= MMC_CAP_SIGNALING_330;
	if (host_caps & (MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
	    MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_DDR50 | MMC_CAP_UHS_SDR104 |
	    MMC_CAP_MMC_DDR52_180 | MMC_CAP_MMC_HS200_180 |
	    MMC_CAP_MMC_HS400_180))
		host_caps |= MMC_CAP_SIGNALING_120 | MMC_CAP_SIGNALING_180;

	/*
	 * Disable 1.2 V and 1.8 V signaling if the switch_vccq method is the
	 * default NULL implementation.  Disable 1.2 V support if it's the
	 * generic SDHCI implementation.
	 */
	if (slot->quirks & SDHCI_QUIRK_SIG_1V8_ONLY) {
		host_caps &= ~(MMC_CAP_SIGNALING_120 | MMC_CAP_SIGNALING_180);
		host_caps |= MMC_CAP_SIGNALING_180;
	}

	if (slot->quirks & SDHCI_QUIRK_SIG_3V3_ONLY)
		host_caps &= ~(MMC_CAP_SIGNALING_120 | MMC_CAP_SIGNALING_180);

	/* Determine supported driver types (type B is always mandatory). */
	if (caps2 & SDHCI_CAN_DRIVE_TYPE_A)
		host_caps |= MMC_CAP_DRIVER_TYPE_A;
	if (caps2 & SDHCI_CAN_DRIVE_TYPE_C)
		host_caps |= MMC_CAP_DRIVER_TYPE_C;
	if (caps2 & SDHCI_CAN_DRIVE_TYPE_D)
		host_caps |= MMC_CAP_DRIVER_TYPE_D;
	slot->host.caps = host_caps;

	/* Decide if we have usable DMA. */
	if ((caps & SDHCI_CAN_DO_SDMA) || (caps & SDHCI_CAN_DO_ADMA2) ||
	    (caps & SDHCI_CAN_DO_ADMA2_64BIT))
		slot->opt |= SDHCI_HAVE_DMA;

	if (slot->quirks & SDHCI_QUIRK_BROKEN_DMA)
		slot->opt &= ~SDHCI_HAVE_DMA;
	if (slot->quirks & SDHCI_QUIRK_FORCE_DMA)
		slot->opt |= SDHCI_HAVE_DMA;
	if (slot->quirks & SDHCI_QUIRK_ALL_SLOTS_NON_REMOVABLE)
		slot->opt |= SDHCI_NON_REMOVABLE;

	/* Config sdhci dma. */
	if (caps & SDHCI_CAN_DO_SDMA)
		slot->dma_flags |= SDHCI_USE_SDMA;
	if ((slot->version >= SDHCI_SPEC_200) && (caps & SDHCI_CAN_DO_ADMA2)) {
		slot->dma_flags |= SDHCI_USE_ADMA2;
		if (caps & SDHCI_CAN_DO_ADMA2_64BIT)
			slot->dma_flags |= SDHCI_USE_ADMA2_64BIT;
	}

	/*
	 * Use platform-provided transfer backend
	 * with PIO as a fallback mechanism
	 */
	if (slot->opt & SDHCI_PLATFORM_TRANSFER)
		slot->opt &= ~SDHCI_HAVE_DMA;
#ifdef DPALCFG_DRIVERS_MMC_DBG
	if (bootverbose || sdhci_debug) {
		slot_printf(slot,
		    "%uMHz%s %s VDD:%s%s%s VCCQ: 3.3V%s%s DRV: B%s%s%s %s %s\n",
		    slot->max_clk / 1000000,
		    (caps & SDHCI_CAN_DO_HISPD) ? " HS" : "",
		    (host_caps & MMC_CAP_8_BIT_DATA) ? "8bits" :
		    ((host_caps & MMC_CAP_4_BIT_DATA) ? "4bits" : "1bit"),
		    (caps & SDHCI_CAN_VDD_330) ? " 3.3V" : "",
		    (caps & SDHCI_CAN_VDD_300) ? " 3.0V" : "",
		    ((caps & SDHCI_CAN_VDD_180) &&
		    (slot->opt & SDHCI_SLOT_EMBEDDED)) ? " 1.8V(SLOT_EMBEDDED)" : "",
		    (host_caps & MMC_CAP_SIGNALING_180) ? " 1.8V" : "",
		    (host_caps & MMC_CAP_SIGNALING_120) ? " 1.2V" : "",
		    (host_caps & MMC_CAP_DRIVER_TYPE_A) ? "A" : "",
		    (host_caps & MMC_CAP_DRIVER_TYPE_C) ? "C" : "",
		    (host_caps & MMC_CAP_DRIVER_TYPE_D) ? "D" : "",
		    (slot->opt & SDHCI_HAVE_DMA) ? "DMA" : "PIO",
		    (slot->opt & SDHCI_SLOT_EMBEDDED) ? "embedded" :
		    (slot->opt & SDHCI_SLOT_SDIO) ? "non-removable" :
		    "removable");
		if (slot->opt & SDHCI_SLOT_EMBEDDED) { /* EMMC */
			if (host_caps & (MMC_CAP_MMC_DDR52 | MMC_CAP_MMC_HS200 |
			    MMC_CAP_MMC_HS400 | MMC_CAP_MMC_ENH_STROBE))
				slot_printf(slot, "eMMC:%s%s%s%s\n",
				    (host_caps & MMC_CAP_MMC_DDR52) ? " DDR52" : "",
				    (host_caps & MMC_CAP_MMC_HS200) ? " HS200" : "",
				    (host_caps & MMC_CAP_MMC_HS400) ? " HS400" : "",
				    ((host_caps &
				    (MMC_CAP_MMC_HS400 | MMC_CAP_MMC_ENH_STROBE)) ==
				    (MMC_CAP_MMC_HS400 | MMC_CAP_MMC_ENH_STROBE)) ?
				    " HS400ES" : "");
		} else { /* SD or SDIO */
			if (host_caps & (MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 |
			    MMC_CAP_UHS_SDR50 | MMC_CAP_UHS_SDR104))
				slot_printf(slot, "UHS-I:%s%s%s%s%s\n",
				    (host_caps & MMC_CAP_UHS_SDR12) ? " SDR12" : "",
				    (host_caps & MMC_CAP_UHS_SDR25) ? " SDR25" : "",
				    (host_caps & MMC_CAP_UHS_SDR50) ? " SDR50" : "",
				    (host_caps & MMC_CAP_UHS_SDR104) ? " SDR104" : "",
				    (host_caps & MMC_CAP_UHS_DDR50) ? " DDR50" : "");
		}
		if (slot->opt & SDHCI_TUNING_SUPPORTED)
			slot_printf(slot, "Re-tuning count %u secs, mode %u\n",
			    slot->retune_count, slot->retune_mode + 1);
	}
#endif
	/*
	 * The card_present indicates the of the device is detected or not
	 * The default value is MMC_CAP_CARD_ABSENT (undetected)
	 */
	slot->host.card_present = MMC_CAP_CARD_ABSENT;
	/*
	 * The plugged_state indicates the status of the device at the first time.
	 * The default value is MMC_CAP_CARD_ABSENT (unplugged), ensuring that the
	 * card can be detected for the first time.
	 */
	slot->host.plugged_state = MMC_CAP_CARD_ABSENT;
	sc->no_power = false;

	sdhci_init(slot);

	return (0);

error:
	sdhci_deinit_slot(slot);
	return (err);
}

void
sdhci_deinit_slot(struct sdhci_slot *slot)
{
	if (slot->tune_data != NULL) {
		free(slot->tune_data);
		slot->tune_data = NULL;
	}
	if (slot->tune_cmd != NULL) {
		free(slot->tune_cmd);
		slot->tune_cmd = NULL;
	}
	if (slot->tune_req != NULL) {
		free(slot->tune_req);
		slot->tune_req = NULL;
	}
	(void)dpal_event_destroy(&slot->req_event);
	sdhci_adma2_delete(&slot->adma2);
	dpal_mux_delete(slot->bus_mtx.mux_id);
	SDHCI_LOCK_DESTROY(slot);
	return;
}

int
sdhci_cleanup_slot(struct sdhci_slot *slot)
{
	device_t d;
	struct hci_softc *sc = device_get_softc(slot->bus);

	(void)dpal_task_delete(slot->cd_tsk_id);
	sdhci_card_detect(sc, HCI_EVENT_UNPLUGGED);

	SDHCI_LOCK(slot);
	d = slot->dev;
	slot->dev = NULL;
	SDHCI_UNLOCK(slot);

	if (d != NULL)
		(void)mmc_device_del_child(d);

	SDHCI_LOCK(slot);
	sdhci_reset(slot, SDHCI_RESET_ALL);
	SDHCI_UNLOCK(slot);

	sdhci_deinit_slot(slot);

	return (0);
}

int
sdhci_generic_suspend(struct sdhci_slot *slot)
{
	/*
	 * We expect the MMC layer to issue initial tuning after resume.
	 * Otherwise, we'd need to indicate re-tuning including circuit reset
	 * being required at least for re-tuning modes 1 and 2 ourselves.
	 */
	SDHCI_LOCK(slot);
	slot->opt &= ~SDHCI_TUNING_ENABLED;
	sdhci_reset(slot, SDHCI_RESET_ALL);
	SDHCI_UNLOCK(slot);

	return (0);
}

int
sdhci_generic_resume(struct sdhci_slot *slot)
{
	SDHCI_LOCK(slot);
	sdhci_init(slot);
	SDHCI_UNLOCK(slot);
	return (0);
}

uint32_t
sdhci_generic_min_freq(device_t brdev, struct sdhci_slot *slot)
{

	if (slot->version >= SDHCI_SPEC_300)
		return (slot->max_clk / SDHCI_300_MAX_DIVIDER);
	else
		return (slot->max_clk / SDHCI_200_MAX_DIVIDER);
}

/* first brdev is a SDHCI device, second reqdev is a MMC device. */
void
sdhci_generic_update_ios(device_t brdev, device_t reqdev)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	struct mmc_ios *ios = &slot->host.ios;
	hci_ops *ops = get_mmcbus_ops(brdev);
	struct hci_softc *sc = device_get_softc(brdev);

	SDHCI_LOCK(slot);
	/* Do full reset on bus power down to clear from any state. */
	if (ios->power_mode == power_off) {
		WR4(slot, SDHCI_SIGNAL_ENABLE, 0);
		sdhci_init(slot);
	}
	/* Configure the bus. */
	sdhci_set_clock(slot, ios->clock);

	if (sc->no_power == false) /* testcase power on */
		sdhci_set_power(slot, (ios->power_mode == power_off) ? 0 : ios->vdd);

	if (ios->bus_width == bus_width_8) {
		slot->hostctrl |= SDHCI_CTRL_8BITBUS;
		slot->hostctrl &= ~SDHCI_CTRL_4BITBUS;
	} else if (ios->bus_width == bus_width_4) {
		slot->hostctrl &= ~SDHCI_CTRL_8BITBUS;
		slot->hostctrl |= SDHCI_CTRL_4BITBUS;
	} else if (ios->bus_width == bus_width_1) {
		slot->hostctrl &= ~SDHCI_CTRL_8BITBUS;
		slot->hostctrl &= ~SDHCI_CTRL_4BITBUS;
	} else {
		dprintf("Invalid bus width: %d", ios->bus_width);
	}
	if (ios->clock > SD_SDR12_MAX &&
	    !(slot->quirks & SDHCI_QUIRK_DONT_SET_HISPD_BIT))
		slot->hostctrl |= SDHCI_CTRL_HISPD;
	else
		slot->hostctrl &= ~SDHCI_CTRL_HISPD;
	WR1(slot, SDHCI_HOST_CONTROL, slot->hostctrl);

	if (slot->version >= SDHCI_SPEC_300) {
		sdhci_set_clock(slot, 0);
		if (ops->set_uhs_timing != NULL)
			ops->set_uhs_timing(brdev);
		sdhci_set_clock(slot, ios->clock);
	}

	/* Some controllers like reset after bus changes. */
	if (slot->quirks & SDHCI_QUIRK_RESET_ON_IOS)
		sdhci_reset(slot, SDHCI_RESET_CMD | SDHCI_RESET_DATA);

	SDHCI_UNLOCK(slot);
}

int
sdhci_generic_switch_vccq(device_t brdev, device_t reqdev)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	enum mmc_vccq vccq;
	int err;
	struct hci_softc *sc = device_get_softc(brdev);

	if (slot->version < SDHCI_SPEC_300)
		return (0);
	if (slot->ops->switch_volt == NULL)
		return (0);

	vccq = slot->host.ios.vccq;
	SDHCI_LOCK(slot);
	sdhci_set_clock(slot, 0);
	err = slot->ops->switch_volt(sc, vccq);
	sdhci_set_clock(slot, slot->host.ios.clock);
	SDHCI_UNLOCK(slot);
	return (err);
}

int
sdhci_generic_retune(device_t brdev, device_t reqdev, bool reset)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	int err;

	if (!(slot->opt & SDHCI_TUNING_ENABLED))
		return (0);

	/* HS400 must be tuned in HS200 mode. */
	if (slot->host.ios.timing == bus_timing_mmc_hs400)
		return (EINVAL);

	SDHCI_LOCK(slot);
	err = sdhci_exec_tuning(slot, reset);
	SDHCI_UNLOCK(slot);
	return (err);
}

static int
sdhci_exec_tuning(struct sdhci_slot *slot, bool reset)
{
	struct mmc_request *tune_req;
	struct mmc_command *tune_cmd;
	int i;
	uint32_t intmask;
	uint16_t hostctrl2;
	u_char opt;

	SDHCI_ASSERT_LOCKED(slot);
	if (slot->req != NULL)
		return (EBUSY);

	/* Tuning doesn't work with DMA enabled. */
	opt = slot->opt;
	slot->opt = opt & ~SDHCI_HAVE_DMA;

	/*
	 * Ensure that as documented, SDHCI_INT_DATA_AVAIL is the only
	 * kind of interrupt we receive in response to a tuning request.
	 */
	intmask = slot->intmask;
	slot->intmask = SDHCI_INT_DATA_AVAIL;
	WR4(slot, SDHCI_INT_ENABLE, SDHCI_INT_DATA_AVAIL);
	WR4(slot, SDHCI_SIGNAL_ENABLE, SDHCI_INT_DATA_AVAIL);

	hostctrl2 = RD2(slot, SDHCI_HOST_CONTROL2);
	if (reset)
		hostctrl2 &= ~SDHCI_CTRL2_SAMPLING_CLOCK;
	else
		hostctrl2 |= SDHCI_CTRL2_SAMPLING_CLOCK;
	WR2(slot, SDHCI_HOST_CONTROL2, hostctrl2 | SDHCI_CTRL2_EXEC_TUNING);

	tune_req = slot->tune_req;
	tune_cmd = slot->tune_cmd;
	for (i = 0; i < MMC_TUNING_MAX; i++) {
		memset(tune_req, 0, sizeof(*tune_req));
		tune_req->cmd = tune_cmd;
		tune_req->done = sdhci_req_wakeup;
		tune_req->done_data = slot;
		slot->req = tune_req;
		slot->flags = 0;
		sdhci_start(slot);
		/* this event will be wake up by sdhci_timeout if timeout, or by sdhci_req_wakeup if succeed */
		(void)dpal_event_read(&slot->req_event, SDHCI_ACQUIRE_HOST, DPAL_WAITMODE_AND | DPAL_WAITMODE_CLR,
			DPAL_WAIT_FOREVER);
		if (!(tune_req->flags & MMC_TUNE_DONE))
			break;
		hostctrl2 = RD2(slot, SDHCI_HOST_CONTROL2);
		if (!(hostctrl2 & SDHCI_CTRL2_EXEC_TUNING))
			break;
		if (tune_cmd->opcode == MMC_SEND_TUNING_BLOCK)
			DELAY(1000);
	}

	/*
	 * Restore DMA usage and interrupts.
	 * Note that the interrupt aggregation code might have cleared
	 * SDHCI_INT_DMA_END and/or SDHCI_INT_RESPONSE in slot->intmask
	 * and SDHCI_SIGNAL_ENABLE respectively so ensure SDHCI_INT_ENABLE
	 * doesn't lose these.
	 */
	slot->opt = opt;
	slot->intmask = intmask;
	WR4(slot, SDHCI_INT_ENABLE, intmask | SDHCI_INT_DMA_END |
	    SDHCI_INT_RESPONSE);
	WR4(slot, SDHCI_SIGNAL_ENABLE, intmask);

	if ((hostctrl2 & (SDHCI_CTRL2_EXEC_TUNING |
	    SDHCI_CTRL2_SAMPLING_CLOCK)) == SDHCI_CTRL2_SAMPLING_CLOCK) {
		slot->retune_req = 0;
		return (0);
	}

	slot_printf(slot, "Tuning failed, using fixed sampling clock\n");
	WR2(slot, SDHCI_HOST_CONTROL2, hostctrl2 & ~(SDHCI_CTRL2_EXEC_TUNING |
	    SDHCI_CTRL2_SAMPLING_CLOCK));
	sdhci_reset(slot, SDHCI_RESET_CMD | SDHCI_RESET_DATA);
	return (EIO);
}

static int
sdhci_read_req_data_process(const struct mmc_data *cur_data, struct sdhci_slot *slot)
{
	aligned_buf_s *buf;
	uint32_t start_len = 0;
	uint32_t end_len = 0;
	uint32_t align_len;

	buf = &slot->adma2.aligned_buf_start;
	if (buf->unaligned_flag == 1) {
		hci_dma_inv_cache(buf->ptr, buf->buf_size_max);
		if (memcpy_s(cur_data->data, cur_data->len, buf->ptr, buf->buf_size) != EOK) {
			return (EINVAL);
		}
		start_len = buf->buf_size;
	}
	buf = &slot->adma2.aligned_buf_end;
	if (buf->unaligned_flag == 1) {
		hci_dma_inv_cache(buf->ptr, buf->buf_size_max);
		if (memcpy_s((void *)((uintptr_t)cur_data->data + cur_data->len - buf->buf_size),
		    buf->buf_size, buf->ptr, buf->buf_size) != EOK) {
			return (EINVAL);
		}
		end_len = buf->buf_size;
	}
	align_len = cur_data->len - start_len - end_len;
	if (align_len != 0) {
		hci_dma_inv_cache((void *)((uintptr_t)cur_data->data + start_len), align_len);
	}
	return (0);
}

#ifdef MMCCAM
static void
sdhci_req_done(struct sdhci_slot *slot)
{
	union ccb *ccb;

	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "%s\n", __func__);
	if (slot->ccb != NULL && slot->curcmd != NULL) {
		callout_stop(&slot->timeout_callout);
		ccb = slot->ccb;
		slot->ccb = NULL;
		slot->curcmd = NULL;

		/* Tell CAM the request is finished */
		struct ccb_mmcio *mmcio;
		mmcio = &ccb->mmcio;

		ccb->ccb_h.status =
		    (mmcio->cmd.error == 0 ? CAM_REQ_CMP : CAM_REQ_CMP_ERR);
		xpt_done(ccb);
	}
}
#else
/* Should be ensure no subsequent interruption for slot->curcmd will be use in interruption */
static void
sdhci_req_done(struct sdhci_slot *slot)
{
	struct mmc_request *req;
	struct mmc_command *cur_cmd;
	const struct mmc_data *cur_data;

	uint32_t opcode;
	if (slot->req != NULL && slot->curcmd != NULL) {
		req = slot->req;
		cur_cmd = slot->curcmd;
		cur_data = cur_cmd->data;
		if (cur_data != NULL && (cur_data->flags & MMC_DATA_READ) && cur_data->data != NULL) {
			if (sdhci_read_req_data_process(cur_data, slot)) /* process failed */
				return;
		}
		if (((cur_cmd->flags & MMC_RSP_R1) == MMC_RSP_R1) &&
		    ((cur_cmd->flags & MMC_CMD_MASK) != MMC_CMD_BCR) &&
		    (cur_cmd->resp[0] & MMC_CMD_ERRORS)) {
			opcode = cur_cmd->opcode;
			if ((opcode != MMC_SEND_TUNING_BLOCK) && (opcode != MMC_SEND_TUNING_BLOCK_HS200)) {
				slot->error_count++;
				slot->curcmd->error = -EACCES;
			}
		}

		slot->req = NULL;
		slot->curcmd = NULL;

		if (req->done != NULL)
			req->done(req);
	}
}
#endif

void
sdhci_req_wakeup(struct mmc_request *req)
{
	struct sdhci_slot *slot = req->done_data;
	(void)dpal_event_write(&slot->req_event, SDHCI_ACQUIRE_HOST);
}

static void
sdhci_set_transfer_mode(struct sdhci_slot *slot, const struct mmc_data *data)
{
	uint16_t mode;

	if (data == NULL) {
		mode = RD2(slot, SDHCI_TRANSFER_MODE);
		mode &= ~(SDHCI_TRNS_ACMD23 | SDHCI_TRNS_ACMD12);
		WR2(slot, SDHCI_TRANSFER_MODE, mode);
		return;
	}

	mode = SDHCI_TRNS_BLK_CNT_EN;
	if (data->flags & MMC_DATA_MULTI)
		mode |= SDHCI_TRNS_MULTI;
	if (data->flags & MMC_DATA_READ)
		mode |= SDHCI_TRNS_READ;
	if (slot->req->bc != NULL)
		mode |= SDHCI_TRNS_ACMD23;
	else if (slot->req->stop && !(slot->quirks & SDHCI_QUIRK_BROKEN_AUTO_STOP))
		mode |= SDHCI_TRNS_ACMD12;
	if (slot->flags & SDHCI_USE_DMA)
		mode |= SDHCI_TRNS_DMA;

	WR2(slot, SDHCI_TRANSFER_MODE, mode);
}

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
static int
sdhci_wait_command_rsp(struct sdhci_slot *slot)
{
	uint32_t intmask;
	uint64_t timeout, interval;

	/* For some old cards, erase time is more than 1 sec */
	if ((slot->curcmd->data == NULL) && (slot->curcmd->opcode != MMC_ERASE))
		interval = DPAL_NS_PER_SECOND;
	else
		interval = (uint64_t)DPAL_NS_PER_SECOND * 10; /* 10s wait */

	timeout = dpal_sched_clock() + interval;

	while (dpal_sched_clock() < timeout) {
		intmask = RD4(slot, SDHCI_INT_STATUS);
		if (intmask == 0 || intmask == 0xffffffff) {
			dpal_udelay(1);
			continue;
		}

		return (0);
	}

	return (-ETIMEDOUT);
}

static void
sdhci_handle_cmd_in_isr(struct sdhci_slot *slot, const struct mmc_request *req)
{
	int ret;

	ret = sdhci_wait_command_rsp(slot);
	if (ret != 0) {
		req->cmd->error = MMC_ERR_NO_REP;
		return;
	}
	sdhci_generic_intr(slot); /* Handle the current interrupt */

	/* A data cmd, data irq could come behind cmd irq, handle it */
	if (req->cmd->data && slot->data_done == 0) {
		ret = sdhci_wait_command_rsp(slot);
		if (ret != 0) {
			req->cmd->error = MMC_ERR_NO_REP;
			return;
		}
		sdhci_generic_intr(slot);
	}
}
#endif

static void
sdhci_start_command(struct sdhci_slot *slot, struct mmc_command *cmd)
{
	int flags, timeout;
	uint32_t mask;

	slot->curcmd = cmd;
	slot->cmd_done = 0;
	cmd->irq_status = 0;
	cmd->error = MMC_ERR_NONE;

	/* This flags combination is not supported by controller. */
	if ((cmd->flags & MMC_RSP_136) && (cmd->flags & MMC_RSP_BUSY)) {
		slot_printf(slot, "Unsupported response type!\n");
		cmd->error = MMC_ERR_FAILED;
		sdhci_req_done(slot);
		return;
	}

	/*
	 * Do not issue command if there is no card, clock or power.
	 * Controller will not detect timeout without clock active.
	 */
	if (!SDHCI_GET_CARD_PRESENT(slot->bus) ||
	    slot->power == 0 ||
	    slot->clock == 0) {
		slot_printf(slot,
			 "Cannot issue a command (power=%d clock=%d)",
			 slot->power, slot->clock);
		if (slot->power == 0 || slot->clock == 0)
			cmd->error = MMC_ERR_FAILED;
		else
			cmd->error = MMC_ERR_UNPLUGGED;
		dev_loge(slot->bus, "Card is not present! Read or write will return failed now! "
		    "cmd->error:%d\n", cmd->error);
		sdhci_req_done(slot);
		return;
	}
	/* Always wait for free CMD bus. */
	mask = SDHCI_CMD_INHIBIT;
	/* Wait for free DAT if we have data or busy signal. */
	if (cmd->data != NULL || (cmd->flags & MMC_RSP_BUSY))
		mask |= SDHCI_DAT_INHIBIT;
	/*
	 * We shouldn't wait for DAT for stop commands or CMD19/CMD21.  Note
	 * that these latter are also special in that SDHCI_CMD_DATA should
	 * be set below but no actual data is ever read from the controller.
	*/
#ifdef MMCCAM
	if (cmd == &slot->ccb->mmcio.stop ||
#else
	if (cmd == slot->req->stop ||
#endif
    		__predict_false(cmd->opcode == MMC_SEND_TUNING_BLOCK ||
	    cmd->opcode == MMC_SEND_TUNING_BLOCK_HS200))
		mask &= ~SDHCI_DAT_INHIBIT;
	/*
	 *  Wait for bus no more then 250 ms.  Typically there will be no wait
	 *  here at all, but when writing a crash dump we may be bypassing the
	 *  host platform's interrupt handler, and in some cases that handler
	 *  may be working around hardware quirks such as not respecting r1b
	 *  busy indications.  In those cases, this wait-loop serves the purpose
	 *  of waiting for the prior command and data transfers to be done, and
	 *  SD cards are allowed to take up to 250ms for write and erase ops.
	 *  (It's usually more like 20-30ms in the real world.)
	 */
	timeout = 250;
	while (mask & RD4(slot, SDHCI_PRESENT_STATE)) {
		if (timeout == 0) {
			slot_printf(slot, "Controller never released "
			    "inhibit bit(s).\n");
			sdhci_dump_regs(slot);
			cmd->error = MMC_ERR_FAILED;
			sdhci_req_done(slot);
			return;
		}
		timeout--;
		DELAY(1000);
	}

	/* Prepare command flags. */
	if (!(cmd->flags & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->flags & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->flags & MMC_RSP_BUSY)
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
	else
		flags = SDHCI_CMD_RESP_SHORT;
	if (cmd->flags & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->flags & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;
	if (cmd->data != NULL)
		flags |= SDHCI_CMD_DATA;
	if (cmd->opcode == MMC_STOP_TRANSMISSION)
		flags |= SDHCI_CMD_TYPE_ABORT;
	/* Prepare data. */
	sdhci_start_data(slot, cmd->data);
	/*
	 * Interrupt aggregation: To reduce total number of interrupts
	 * group response interrupt with data interrupt when possible.
	 * If there going to be data interrupt, mask response one.
	 */
	if (slot->data_done == 0) {
		WR4(slot, SDHCI_SIGNAL_ENABLE,
		    slot->intmask &= ~SDHCI_INT_RESPONSE);
	}
	/* Set command argument. */
	WR4(slot, SDHCI_ARGUMENT, cmd->arg);
	/* Set data transfer mode. */
	sdhci_set_transfer_mode(slot, cmd->data);
	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "Starting command!\n");

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
	/*
	 * The condition judgment must be performed before the register is written,
	 * slot->req will be clearing in sdhci isr(triggers by write command register).
	 */
	if (slot->req->flags & MMC_REQ_IN_EXC) {
		WR2(slot, SDHCI_COMMAND_FLAGS, (cmd->opcode << 8) | (flags & 0xff));
		sdhci_handle_cmd_in_isr(slot, slot->req);
	} else
#endif
	{
		/* Start command. */
		WR2(slot, SDHCI_COMMAND_FLAGS, (cmd->opcode << 8) | (flags & 0xff));
	}
}

static void
sdhci_finish_command(struct sdhci_slot *slot)
{
	int i;
	uint32_t val;
	uint8_t extra;

	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "%s: called, err %d flags %u\n",
			__func__, slot->curcmd->error, slot->curcmd->flags);
	slot->cmd_done = 1;
	/*
	 * Interrupt aggregation: Restore command interrupt.
	 * Main restore point for the case when command interrupt
	 * happened first.
	 */
	if (slot->curcmd->opcode != MMC_SEND_TUNING_BLOCK &&
	    slot->curcmd->opcode != MMC_SEND_TUNING_BLOCK_HS200) {
		slot->intmask |= SDHCI_INT_RESPONSE;
		WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);
	}
	/* In case of error - reset host and return. */
	if (slot->curcmd->error) {
		if (slot->curcmd->error == MMC_ERR_BADCRC &&
		    !SDHCI_IS_TUNING(slot->curcmd))
			slot->retune_req |= SDHCI_RETUNE_REQ_RESET;
		sdhci_reset(slot, SDHCI_RESET_CMD);
		sdhci_reset(slot, SDHCI_RESET_DATA);
		sdhci_start(slot);
		return;
	}
	/* If command has response - fetch it. */
	if (slot->curcmd->flags & MMC_RSP_PRESENT) {
		if (slot->curcmd->flags & MMC_RSP_136) {
			/* CRC is stripped so we need one byte shift. */
			extra = 0;
			for (i = 0; i < 4; i++) {
				val = RD4(slot, SDHCI_RESPONSE + i * 4);
				if (slot->quirks &
				    SDHCI_QUIRK_DONT_SHIFT_RESPONSE)
					slot->curcmd->resp[3 - i] = val;
				else {
					slot->curcmd->resp[3 - i] =
					    (val << 8) | extra;
					extra = val >> 24;
				}
			}
		} else
			slot->curcmd->resp[0] = RD4(slot, SDHCI_RESPONSE);
	}
	if (__predict_false(sdhci_debug > 1))
		printf("Resp: %02x %02x %02x %02x\n",
			slot->curcmd->resp[0], slot->curcmd->resp[1],
			slot->curcmd->resp[2], slot->curcmd->resp[3]);

	/* If data ready - finish. */
	if (slot->data_done)
		sdhci_start(slot);
}

static void sdhci_adma_config(const struct sdhci_slot *slot)
{
	hci_ops *ops = get_mmcbus_ops(slot->bus);
    if (slot->dma_flags & SDHCI_USE_ADMA2_64BIT) {
        (ops->writel)(slot->bus, SDHCI_ADMA_ADDRESS_HI, 0);
    }
	(ops->writel)(slot->bus, SDHCI_ADMA_ADDRESS_LO, (uintptr_t)(slot->adma2.desc));
}

static void
sdhci_start_data(struct sdhci_slot *slot, const struct mmc_data *data)
{
	uint8_t ctrl;
	uint16_t ctl2;
	int32_t err;
	uint32_t target_timeout, current_timeout;
	uint8_t div;

	if (data == NULL && (slot->curcmd->flags & MMC_RSP_BUSY) == 0) {
		slot->data_done = 1;
		return;
	}

	slot->data_done = 0;

	/* Calculate and set data timeout.*/
	/* XXX: We should have this from mmc layer, now assume 20 sec. */
	if (slot->quirks & SDHCI_QUIRK_BROKEN_TIMEOUT_VAL) {
		div = 0xE;
	} else {
		target_timeout = 20000000;
		div = 0;
		current_timeout = (1 << 13) * 1000 / slot->timeout_clk;
		while (current_timeout < target_timeout && div < 0xE) {
			++div;
			current_timeout <<= 1;
		}
		/* Compensate for an off-by-one error in the CaFe chip.*/
		if (div < 0xE &&
		    (slot->quirks & SDHCI_QUIRK_INCR_TIMEOUT_CONTROL)) {
			++div;
		}
	}
	WR1(slot, SDHCI_TIMEOUT_CONTROL, div);

	if (data == NULL)
		return;

	/* Use DMA if possible. */
	if ((slot->opt & SDHCI_HAVE_DMA))
		slot->flags |= SDHCI_USE_DMA;
	/* If data is small, broken DMA may return zeroes instead of data. */
	if ((slot->quirks & SDHCI_QUIRK_BROKEN_TIMINGS) &&
	    (data->len <= 512))
		slot->flags &= ~SDHCI_USE_DMA;
	/* Some controllers require even block sizes. */
	if ((slot->quirks & SDHCI_QUIRK_32BIT_DMA_SIZE) &&
	    ((data->len) & 0x3))
		slot->flags &= ~SDHCI_USE_DMA;
	/* Load DMA buffer. */
	if (slot->flags & SDHCI_USE_DMA) {
		if (slot->dma_flags & SDHCI_USE_ADMA2) {
			err = sdhci_adma2_tab_setup(&slot->adma2, (uintptr_t)(data->data), data->len, data->flags);
			if (err) {
				slot_printf(slot, "dma setup failed=%d", err);
			}
#ifdef SET_BLOCK_COUNT_TO_DMA_ADDRESS
			WR2(slot, SDHCI_DMA_ADDRESS, (data->len + 511) / 512);
#else
			WR4(slot, SDHCI_DMA_ADDRESS, 0);
#endif
			sdhci_adma_config(slot);
		} else if (slot->dma_flags & SDHCI_USE_SDMA)
			WR4(slot, SDHCI_DMA_ADDRESS, (uintptr_t)(slot->adma2.desc));
	}

	if (slot->flags & SDHCI_USE_DMA) {
		ctrl = RD1(slot, SDHCI_HOST_CONTROL);
		ctrl &= ~SDHCI_CTRL_DMA_MASK;
		ctl2 = RD2(slot, SDHCI_HOST_CONTROL2);
		if (slot->dma_flags & SDHCI_USE_ADMA2) {
			ctrl |= SDHCI_CTRL_ADMA2;
		} else if (slot->dma_flags & SDHCI_USE_SDMA)
			ctrl |= SDHCI_CTRL_SDMA;
		WR1(slot, SDHCI_HOST_CONTROL, ctrl);
		WR2(slot, SDHCI_HOST_CONTROL2, ctl2); /* Enable 26-bit length */
	}
	/* Current data offset for both PIO and DMA. */
	slot->offset = 0;
	if (data->flags & MMC_DATA_SDIO) { /* Sdio */
		/* Set block size */
		WR2(slot, SDHCI_BLOCK_SIZE, SDHCI_MAKE_BLKSZ(DMA_BOUNDARY, data->blk_size));
		/* Set block count. */
		WR2(slot, SDHCI_BLOCK_COUNT, data->blk_num);
	} else { /* sd or emmc */
		/* Set block size */
		WR2(slot, SDHCI_BLOCK_SIZE, SDHCI_MAKE_BLKSZ(DMA_BOUNDARY,
		    (data->len < 512) ? data->len : 512));
		/* Set block count. */
		WR2(slot, SDHCI_BLOCK_COUNT, (data->len + 511) / 512);
	}
	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "Blk size: 0x%08x | Blk cnt:  0x%08x\n",
			SDHCI_BLOCK_SIZE, SDHCI_BLOCK_COUNT);
}

void
sdhci_finish_data(struct sdhci_slot *slot)
{

	/* Interrupt aggregation: Restore command interrupt.
	 * Auxiliary restore point for the case when data interrupt
	 * happened first. */
	if (!slot->cmd_done) {
		slot->intmask |= SDHCI_INT_RESPONSE;
		WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);
	}
	slot->data_done = 1;
	/* If there was error - reset the host. */
	if (slot->curcmd->error) {
		if ((slot->curcmd->error == MMC_ERR_BADCRC) &&
		    !SDHCI_IS_TUNING(slot->curcmd))
			slot->retune_req |= SDHCI_RETUNE_REQ_RESET;
		sdhci_reset(slot, SDHCI_RESET_CMD);
		sdhci_reset(slot, SDHCI_RESET_DATA);
		sdhci_start(slot);
		return;
	}
	/* If we already have command response - finish. */
	if (slot->cmd_done)
		sdhci_start(slot);
}

#ifdef MMCCAM
static void
sdhci_start(struct sdhci_slot *slot)
{
	union ccb *ccb;
	struct ccb_mmcio *mmcio;

	ccb = slot->ccb;
	if (ccb == NULL)
		return;

	mmcio = &ccb->mmcio;
	if (!(slot->flags & CMD_STARTED)) {
		slot->flags |= CMD_STARTED;
		sdhci_start_command(slot, &mmcio->cmd);
		return;
	}

	/*
	 * Old stack doesn't use this!
	 * Enabling this code causes significant performance degradation
	 * and IRQ storms on BBB, Wandboard behaves fine.
	 * Not using this code does no harm...
	if (!(slot->flags & STOP_STARTED) && mmcio->stop.opcode != 0) {
		slot->flags |= STOP_STARTED;
		sdhci_start_command(slot, &mmcio->stop);
		return;
	}
	*/
	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "result: %d\n", mmcio->cmd.error);
	if (mmcio->cmd.error == 0 &&
		(slot->quirks & SDHCI_QUIRK_RESET_AFTER_REQUEST)) {
		sdhci_reset(slot, SDHCI_RESET_CMD);
		sdhci_reset(slot, SDHCI_RESET_DATA);
	}

	sdhci_req_done(slot);
}
#else
void
sdhci_start(struct sdhci_slot *slot)
{
	const struct mmc_request *req;

	req = slot->req;
	if (req == NULL)
		return;
	/* open cmd done irq */
	slot->intmask |= SDHCI_INT_RESPONSE;
	WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
	WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);

	if ((slot->quirks & SDHCI_QUIRK_BROKEN_ACMD23) &&
	    !(slot->flags & BC_STARTED) && req->bc) {
		slot->flags |= BC_STARTED;
		sdhci_start_command(slot, req->bc);
		return;
	}
	if (!(slot->flags & CMD_STARTED)) {
		slot->flags |= CMD_STARTED;
		sdhci_start_command(slot, req->cmd);
		return;
	}
	if ((slot->quirks & SDHCI_QUIRK_BROKEN_AUTO_STOP) &&
	    !(slot->flags & STOP_STARTED) && req->stop) {
		slot->flags |= STOP_STARTED;
		sdhci_start_command(slot, req->stop);
		return;
	}
	if (__predict_false(sdhci_debug > 1))
		slot_printf(slot, "result: %d\n", req->cmd->error);
	if (!req->cmd->error &&
	    ((slot->curcmd == req->stop &&
	     (slot->quirks & SDHCI_QUIRK_BROKEN_AUTO_STOP)) ||
	     (slot->quirks & SDHCI_QUIRK_RESET_AFTER_REQUEST))) {
		sdhci_reset(slot, SDHCI_RESET_CMD);
		sdhci_reset(slot, SDHCI_RESET_DATA);
	}

	/* close cmd done irq */
	slot->intmask &= ~SDHCI_INT_RESPONSE;
	WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
	WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);
	WR4(slot, SDHCI_INT_STATUS, SDHCI_INT_RESPONSE);

	sdhci_req_done(slot);
}
#endif

int
sdhci_generic_request(device_t brdev, device_t reqdev,
    struct mmc_request *req)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);

	SDHCI_LOCK(slot);
	if (slot->req != NULL) {
		slot_printf(slot, "slot->req end\n");
		SDHCI_UNLOCK(slot);
		return (EBUSY);
	}
	if (__predict_false(sdhci_debug > 1)) {
		slot_printf(slot,
		    "CMD%u arg %#x flags %#x dlen %u dflags %#x\n",
		    req->cmd->opcode, req->cmd->arg, req->cmd->flags,
		    (req->cmd->data)?(u_int)req->cmd->data->len:0,
		    (req->cmd->data)?req->cmd->data->flags:0);
	}

	slot->req = req;
	slot->flags = 0;
	sdhci_start(slot);
	SDHCI_UNLOCK(slot);

	return (0);
}

int
sdhci_generic_get_ro(device_t brdev, device_t reqdev)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	uint32_t val;

	SDHCI_LOCK(slot);
	val = RD4(slot, SDHCI_PRESENT_STATE);
	SDHCI_UNLOCK(slot);
	return ((slot->quirks & SDHCI_QUIRK_WRITE_PROTECT) ?
	    (val & SDHCI_WRITE_PROTECT): !(val & SDHCI_WRITE_PROTECT));
}

int
sdhci_generic_acquire_host(device_t brdev, device_t reqdev)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	dpal_mux_pend(slot->bus_mtx.mux_id, DPAL_WAIT_FOREVER);
	dev_logd(reqdev, "SDHCI_ACQUIRE_HOST acquired.\n");

	return (0);
}

int
sdhci_generic_release_host(device_t brdev, device_t reqdev)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	dpal_mux_post(slot->bus_mtx.mux_id);
	dev_logd(reqdev, "SDHCI_ACQUIRE_HOST released.\n");
	return (0);
}

static void
sdhci_cmd_irq(struct sdhci_slot *slot, uint32_t intmask)
{

	if (!slot->curcmd) {
		slot_printf(slot, "Got command interrupt 0x%08x, but "
		    "there is no active command.\n", intmask);
		sdhci_dump_regs(slot);
		return;
	}
	if (intmask & SDHCI_INT_TIMEOUT)
		slot->curcmd->error = MMC_ERR_TIMEOUT;
	else if (intmask & SDHCI_INT_CRC)
		slot->curcmd->error = MMC_ERR_BADCRC;
	else if (intmask & (SDHCI_INT_END_BIT | SDHCI_INT_INDEX))
		slot->curcmd->error = MMC_ERR_FIFO;

	sdhci_finish_command(slot);
}

static void
sdhci_data_irq(struct sdhci_slot *slot, uint32_t intmask)
{
	uint32_t opcode;

	if (!slot->curcmd) {
		slot_printf(slot, "Got data interrupt 0x%08x, but "
		    "there is no active command.\n", intmask);
		sdhci_dump_regs(slot);
		return;
	}
	if (slot->curcmd->data == NULL &&
	    (slot->curcmd->flags & MMC_RSP_BUSY) == 0) {
		slot_printf(slot, "Got data interrupt 0x%08x, but "
		    "there is no active data operation.\n",
		    intmask);
		sdhci_dump_regs(slot);
		return;
	}
	if (intmask & SDHCI_INT_DATA_TIMEOUT)
		slot->curcmd->error = MMC_ERR_TIMEOUT;
	else if (intmask & (SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_END_BIT))
		slot->curcmd->error = MMC_ERR_BADCRC;
	else if (intmask & SDHCI_INT_ADMAERR)
		slot->curcmd->error = MMC_ERR_ADMA;

	if (slot->curcmd->data == NULL &&
	    (intmask & (SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL |
	    SDHCI_INT_DMA_END))) {
		slot_printf(slot, "Got data interrupt 0x%08x, but "
		    "there is busy-only command.\n", intmask);
		slot->curcmd->error = MMC_ERR_INVALID;
	}
	opcode = slot->curcmd->opcode;
	if (slot->curcmd->error) {
		if (opcode != MMC_SEND_TUNING_BLOCK &&
		    opcode != MMC_SEND_TUNING_BLOCK_HS200) {
			sdhci_dump_regs(slot);
			slot->error_count++;
		}
		/* No need to continue after any error. */
		sdhci_finish_data(slot);
		return;
	}

	/* Handle tuning completion interrupt. */
	if (((intmask & SDHCI_INT_DATA_AVAIL) &&
	    (opcode == MMC_SEND_TUNING_BLOCK ||
	    opcode == MMC_SEND_TUNING_BLOCK_HS200))) {
		slot->req->flags |= MMC_TUNE_DONE;
		sdhci_finish_command(slot);
		sdhci_finish_data(slot);
		return;
	}

	/* We have got all data. */
	if (intmask & SDHCI_INT_DATA_END)
		sdhci_finish_data(slot);
}

static void
sdhci_acmd_irq(struct sdhci_slot *slot)
{
	uint16_t err;

	err = RD4(slot, SDHCI_ACMD12_ERR);
	if (!slot->curcmd) {
		slot_printf(slot, "Got AutoCMD12 or AutoCMD23 error 0x%04x, but "
		    "there is no active command.\n", err);
		sdhci_dump_regs(slot);
		return;
	}
	slot_printf(slot, "Got AutoCMD12 or AutoCMD23 error 0x%04x\n", err);
	sdhci_reset(slot, SDHCI_RESET_CMD);
}

void
sdhci_generic_intr(struct sdhci_slot *slot)
{
	uint32_t intmask;
	struct hci_softc *sc = device_get_softc(slot->bus);
	/* Read slot interrupt status. */
	intmask = RD4(slot, SDHCI_INT_STATUS);
	if (intmask == 0 || intmask == 0xffffffff) {
		return;
	}
	if (__predict_false(sdhci_debug > 2))
		slot_printf(slot, "Interrupt %#x\n", intmask);

	if (slot->curcmd)
		slot->curcmd->irq_status |= intmask;

	/* Handle tuning error interrupt. */
	if (__predict_false(intmask & SDHCI_INT_TUNEERR)) {
		slot_printf(slot, "Tuning error indicated\n");
		slot->retune_req |= SDHCI_RETUNE_REQ_RESET;
		if (slot->curcmd) {
			slot->curcmd->error = MMC_ERR_BADCRC;
			sdhci_finish_command(slot);
		}
	}
	/* Handle re-tuning interrupt. */
	if (__predict_false(intmask & SDHCI_INT_RETUNE))
		slot->retune_req |= SDHCI_RETUNE_REQ_NEEDED;

	/* Handle command interrupts. */
	if (intmask & SDHCI_INT_CMD_MASK) {
		WR4(slot, SDHCI_INT_STATUS, intmask & SDHCI_INT_CMD_MASK);
		sdhci_cmd_irq(slot, intmask & SDHCI_INT_CMD_MASK);
	}
	/* Handle data interrupts. */
	if (intmask & SDHCI_INT_DATA_MASK) {
		WR4(slot, SDHCI_INT_STATUS, intmask & SDHCI_INT_DATA_MASK);
		/* Don't call data_irq in case of errored command. */
		if ((intmask & SDHCI_INT_CMD_ERROR_MASK) == 0)
			sdhci_data_irq(slot, intmask & SDHCI_INT_DATA_MASK);
	}
	/* Handle AutoCMD12 or AutoCMD23 error interrupt. */
	if (intmask & SDHCI_INT_ACMD12ERR) {
		WR4(slot, SDHCI_INT_STATUS, SDHCI_INT_ACMD12ERR);
		sdhci_acmd_irq(slot);
	}
	/* Handle bus power interrupt. */
	if (intmask & SDHCI_INT_BUS_POWER) {
		WR4(slot, SDHCI_INT_STATUS, SDHCI_INT_BUS_POWER);
		slot_printf(slot, "Card is consuming too much power!\n");
	}

	if (intmask & SDHCI_INT_CARD_INT) {
		WR4(slot, SDHCI_INT_STATUS, SDHCI_INT_CARD_INT);
		slot->intmask &= ~SDHCI_INT_CARD_INT;
		slot->intmask |= SDHCI_INT_RESPONSE | SDHCI_INT_DMA_END; /* Make sure cmd or data completed intr enabled */
		WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
		WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);
		if (sc->sdio_cb != NULL)
			sc->sdio_cb(sc->sdio_arg);
	}

	intmask &= ~(SDHCI_INT_ERROR | SDHCI_INT_TUNEERR | SDHCI_INT_RETUNE |
	    SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE | SDHCI_INT_CMD_MASK |
	    SDHCI_INT_DATA_MASK | SDHCI_INT_ACMD12ERR | SDHCI_INT_BUS_POWER | SDHCI_INT_CARD_INT);

	/* The rest is unknown. */
	if (intmask) {
		WR4(slot, SDHCI_INT_STATUS, intmask);
		slot_printf(slot, "Unexpected interrupt 0x%08x.\n",
		    intmask);
		sdhci_dump_regs(slot);
	}
}

int
sdhci_generic_read_ivar(device_t bus, device_t child, int which,
    uintptr_t *result)
{
	const struct sdhci_slot *slot = device_get_ivars(child);

	switch (which) {
	default:
		return (EINVAL);
	case MMCBR_IVAR_BUS_MODE:
		*result = slot->host.ios.bus_mode;
		break;
	case MMCBR_IVAR_BUS_WIDTH:
		*result = slot->host.ios.bus_width;
		break;
	case MMCBR_IVAR_CHIP_SELECT:
		*result = slot->host.ios.chip_select;
		break;
	case MMCBR_IVAR_CLOCK:
		*result = slot->host.ios.clock;
		break;
	case MMCBR_IVAR_F_MIN:
		*result = slot->host.f_min;
		break;
	case MMCBR_IVAR_F_MAX:
		*result = slot->host.f_max;
		break;
	case MMCBR_IVAR_HOST_OCR:
		*result = slot->host.host_ocr;
		break;
	case MMCBR_IVAR_MODE:
		*result = slot->host.mode;
		break;
	case MMCBR_IVAR_OCR:
		*result = slot->host.ocr;
		break;
	case MMCBR_IVAR_POWER_MODE:
		*result = slot->host.ios.power_mode;
		break;
	case MMCBR_IVAR_VDD:
		*result = slot->host.ios.vdd;
		break;
	case MMCBR_IVAR_RETUNE_REQ:
		if (slot->opt & SDHCI_TUNING_ENABLED) {
			if (slot->retune_req & SDHCI_RETUNE_REQ_RESET) {
				*result = retune_req_reset;
				break;
			}
			if (slot->retune_req & SDHCI_RETUNE_REQ_NEEDED) {
				*result = retune_req_normal;
				break;
			}
		}
		*result = retune_req_none;
		break;
	case MMCBR_IVAR_VCCQ:
		*result = slot->host.ios.vccq;
		break;
	case MMCBR_IVAR_CAPS:
		*result = slot->host.caps;
		break;
	case MMCBR_IVAR_TIMING:
		*result = slot->host.ios.timing;
		break;
	case MMCBR_IVAR_MAX_DATA:
		/*
		 * Re-tuning modes 1 and 2 restrict the maximum data length
		 * per read/write command to 4 MiB.
		 */
		if (slot->opt & SDHCI_TUNING_ENABLED &&
		    (slot->retune_mode == SDHCI_RETUNE_MODE_1 ||
		    slot->retune_mode == SDHCI_RETUNE_MODE_2)) {
			*result = 4 * 1024 * 1024 / MMC_SECTOR_SIZE;
			break;
		}
		*result = 65535;
		break;
	case MMCBR_IVAR_MAX_BUSY_TIMEOUT:
		/*
		 * Currently, sdhci_start_data() hardcodes 20 s for all CMDs.
		 */
		*result = 20000000;
		break;
	case MMCBR_IVAR_DMA_ALIGNMENT:
		*result = sdhci_adma_alian_size((adma2_s *)&slot->adma2);
		break;
	case MMCBR_IVAR_CARD_ERROR_CNT:
		*result = slot->error_count;
		break;
	}
	return (0);
}

int
sdhci_generic_write_ivar(device_t bus, device_t child, int which,
    uintptr_t value)
{
	struct sdhci_slot *slot = device_get_ivars(child);

	if (sdhci_debug > 1)
		slot_printf(slot, "%s: var=%d\n", __func__, which);
	switch (which) {
	default:
		return (EINVAL);
	case MMCBR_IVAR_BUS_MODE:
		slot->host.ios.bus_mode = value;
		break;
	case MMCBR_IVAR_BUS_WIDTH:
		slot->host.ios.bus_width = value;
		break;
	case MMCBR_IVAR_CHIP_SELECT:
		slot->host.ios.chip_select = value;
		break;
	case MMCBR_IVAR_CLOCK:
		if (value > 0)
			slot->host.ios.clock = value;
		else
			slot->host.ios.clock = 0;
		break;
	case MMCBR_IVAR_MODE:
		slot->host.mode = value;
		break;
	case MMCBR_IVAR_OCR:
		slot->host.ocr = value;
		break;
	case MMCBR_IVAR_POWER_MODE:
		slot->host.ios.power_mode = value;
		break;
	case MMCBR_IVAR_VDD:
		slot->host.ios.vdd = value;
		break;
	case MMCBR_IVAR_VCCQ:
		slot->host.ios.vccq = value;
		break;
	case MMCBR_IVAR_TIMING:
		slot->host.ios.timing = value;
		break;
	case MMCBR_IVAR_CAPS:
	case MMCBR_IVAR_HOST_OCR:
	case MMCBR_IVAR_F_MIN:
	case MMCBR_IVAR_F_MAX:
	case MMCBR_IVAR_MAX_DATA:
	case MMCBR_IVAR_RETUNE_REQ:
		return (EINVAL);
	}
	return (0);
}

#ifdef MMCCAM
void
sdhci_start_slot(struct sdhci_slot *slot)
{

	if ((slot->devq = cam_simq_alloc(1)) == NULL)
		goto fail;

	mtx_init(&slot->sim_mtx, "sdhcisim", NULL, MTX_DEF);
	slot->sim = cam_sim_alloc(sdhci_cam_action, sdhci_cam_poll,
		"sdhci_slot", slot, device_get_unit(slot->bus),
		&slot->sim_mtx, 1, 1, slot->devq);

	if (slot->sim == NULL) {
		cam_simq_free(slot->devq);
		slot_printf(slot, "cannot allocate CAM SIM\n");
		goto fail;
	}

	mtx_lock(&slot->sim_mtx);
	if (xpt_bus_register(slot->sim, slot->bus, 0) != 0) {
		slot_printf(slot, "cannot register SCSI pass-through bus\n");
		cam_sim_free(slot->sim, FALSE);
		cam_simq_free(slot->devq);
		mtx_unlock(&slot->sim_mtx);
		goto fail;
	}
	mtx_unlock(&slot->sim_mtx);

	/* End CAM-specific init */
	slot->card_present = 0;
	sdhci_card_task(slot, 0);
	return;

fail:
	if (slot->sim != NULL) {
		mtx_lock(&slot->sim_mtx);
		xpt_bus_deregister(cam_sim_path(slot->sim));
		cam_sim_free(slot->sim, FALSE);
		mtx_unlock(&slot->sim_mtx);
	}

	if (slot->devq != NULL)
		cam_simq_free(slot->devq);
}

static void
sdhci_cam_handle_mmcio(struct cam_sim *sim, union ccb *ccb)
{
	struct sdhci_slot *slot;

	slot = cam_sim_softc(sim);

	sdhci_cam_request(slot, ccb);
}

void
sdhci_cam_action(struct cam_sim *sim, union ccb *ccb)
{
	struct sdhci_slot *slot;

	slot = cam_sim_softc(sim);
	if (slot == NULL) {
		ccb->ccb_h.status = CAM_SEL_TIMEOUT;
		xpt_done(ccb);
		return;
	}

	mtx_assert(&slot->sim_mtx, MA_OWNED);

	switch (ccb->ccb_h.func_code) {
	case XPT_PATH_INQ:
	{
		struct ccb_pathinq *cpi;

		cpi = &ccb->cpi;
		cpi->version_num = 1;
		cpi->hba_inquiry = 0;
		cpi->target_sprt = 0;
		cpi->hba_misc = PIM_NOBUSRESET | PIM_SEQSCAN;
		cpi->hba_eng_cnt = 0;
		cpi->max_target = 0;
		cpi->max_lun = 0;
		cpi->initiator_id = 1;
		cpi->maxio = MAXPHYS;
		strncpy(cpi->sim_vid, "FreeBSD", SIM_IDLEN);
		strncpy(cpi->hba_vid, "Deglitch Networks", HBA_IDLEN);
		strncpy(cpi->dev_name, cam_sim_name(sim), DEV_IDLEN);
		cpi->unit_number = cam_sim_unit(sim);
		cpi->bus_id = cam_sim_bus(sim);
		cpi->base_transfer_speed = 100; /* XXX WTF? */
		cpi->protocol = PROTO_MMCSD;
		cpi->protocol_version = SCSI_REV_0;
		cpi->transport = XPORT_MMCSD;
		cpi->transport_version = 0;

		cpi->ccb_h.status = CAM_REQ_CMP;
		break;
	}
	case XPT_GET_TRAN_SETTINGS:
	{
		struct ccb_trans_settings *cts = &ccb->cts;

		if (sdhci_debug > 1)
			slot_printf(slot, "Got XPT_GET_TRAN_SETTINGS\n");

		cts->protocol = PROTO_MMCSD;
		cts->protocol_version = 1;
		cts->transport = XPORT_MMCSD;
		cts->transport_version = 1;
		cts->xport_specific.valid = 0;
		cts->proto_specific.mmc.host_ocr = slot->host.host_ocr;
		cts->proto_specific.mmc.host_f_min = slot->host.f_min;
		cts->proto_specific.mmc.host_f_max = slot->host.f_max;
		cts->proto_specific.mmc.host_caps = slot->host.caps;
		memcpy(&cts->proto_specific.mmc.ios, &slot->host.ios, sizeof(struct mmc_ios));
		ccb->ccb_h.status = CAM_REQ_CMP;
		break;
	}
	case XPT_SET_TRAN_SETTINGS:
	{
		if (sdhci_debug > 1)
			slot_printf(slot, "Got XPT_SET_TRAN_SETTINGS\n");
		sdhci_cam_settran_settings(slot, ccb);
		ccb->ccb_h.status = CAM_REQ_CMP;
		break;
	}
	case XPT_RESET_BUS:
		if (sdhci_debug > 1)
			slot_printf(slot, "Got XPT_RESET_BUS, ACK it...\n");
		ccb->ccb_h.status = CAM_REQ_CMP;
		break;
	case XPT_MMC_IO:
		/*
		 * Here is the HW-dependent part of
		 * sending the command to the underlying h/w
		 * At some point in the future an interrupt comes.
		 * Then the request will be marked as completed.
		 */
		if (__predict_false(sdhci_debug > 1))
			slot_printf(slot, "Got XPT_MMC_IO\n");
		ccb->ccb_h.status = CAM_REQ_INPROG;

		sdhci_cam_handle_mmcio(sim, ccb);
		return;
		/* NOTREACHED */
		break;
	default:
		ccb->ccb_h.status = CAM_REQ_INVALID;
		break;
	}
	xpt_done(ccb);
	return;
}

void
sdhci_cam_poll(struct cam_sim *sim)
{
	return;
}

static int
sdhci_cam_get_possible_host_clock(const struct sdhci_slot *slot,
	int proposed_clock)
{
	int max_clock, clock, i;

	if (proposed_clock == 0)
		return 0;
	max_clock = slot->max_clk;
	clock = max_clock;

	if (slot->version < SDHCI_SPEC_300) {
		for (i = 0; i < SDHCI_200_MAX_DIVIDER; i <<= 1) {
			if (clock <= proposed_clock)
				break;
			clock >>= 1;
		}
	} else {
		for (i = 0; i < SDHCI_300_MAX_DIVIDER; i += 2) {
			if (clock <= proposed_clock)
				break;
			clock = max_clock / (i + 2);
		}
	}
	return clock;
}

static int
sdhci_cam_settran_settings(struct sdhci_slot *slot, union ccb *ccb)
{
	struct mmc_ios *ios;
	const struct mmc_ios *new_ios;
	const struct ccb_trans_settings_mmc *cts;

	ios = &slot->host.ios;
	cts = &ccb->cts.proto_specific.mmc;
	new_ios = &cts->ios;

	/* Update only requested fields */
	if (cts->ios_valid & MMC_CLK) {
		ios->clock = sdhci_cam_get_possible_host_clock(slot, new_ios->clock);
		slot_printf(slot, "Clock => %d\n", ios->clock);
	}
	if (cts->ios_valid & MMC_VDD) {
		ios->vdd = new_ios->vdd;
		slot_printf(slot, "VDD => %d\n", ios->vdd);
	}
	if (cts->ios_valid & MMC_CS) {
		ios->chip_select = new_ios->chip_select;
		slot_printf(slot, "CS => %d\n", ios->chip_select);
	}
	if (cts->ios_valid & MMC_BW) {
		ios->bus_width = new_ios->bus_width;
		slot_printf(slot, "Bus width => %d\n", ios->bus_width);
	}
	if (cts->ios_valid & MMC_PM) {
		ios->power_mode = new_ios->power_mode;
		slot_printf(slot, "Power mode => %d\n", ios->power_mode);
	}
	if (cts->ios_valid & MMC_BT) {
		ios->timing = new_ios->timing;
		slot_printf(slot, "Timing => %d\n", ios->timing);
	}
	if (cts->ios_valid & MMC_BM) {
		ios->bus_mode = new_ios->bus_mode;
		slot_printf(slot, "Bus mode => %d\n", ios->bus_mode);
	}

	/* XXX Provide a way to call a chip-specific IOS update, required for TI */
	return (sdhci_cam_update_ios(slot));
}

static int
sdhci_cam_update_ios(struct sdhci_slot *slot)
{
	struct mmc_ios *ios = &slot->host.ios;

	slot_printf(slot, "%s: power_mode=%d, clk=%d, bus_width=%d, timing=%d\n",
			__func__, ios->power_mode, ios->clock, ios->bus_width, ios->timing);
	SDHCI_LOCK(slot);
	/* Do full reset on bus power down to clear from any state. */
	if (ios->power_mode == power_off) {
		WR4(slot, SDHCI_SIGNAL_ENABLE, 0);
		sdhci_init(slot);
	}
	/* Configure the bus. */
	sdhci_set_clock(slot, ios->clock);
	sdhci_set_power(slot, (ios->power_mode == power_off) ? 0 : ios->vdd);
	if (ios->bus_width == bus_width_8) {
		slot->hostctrl |= SDHCI_CTRL_8BITBUS;
		slot->hostctrl &= ~SDHCI_CTRL_4BITBUS;
	} else if (ios->bus_width == bus_width_4) {
		slot->hostctrl &= ~SDHCI_CTRL_8BITBUS;
		slot->hostctrl |= SDHCI_CTRL_4BITBUS;
	} else if (ios->bus_width == bus_width_1) {
		slot->hostctrl &= ~SDHCI_CTRL_8BITBUS;
		slot->hostctrl &= ~SDHCI_CTRL_4BITBUS;
	} else {
		panic("Invalid bus width: %d", ios->bus_width);
	}
	if (ios->timing == bus_timing_hs &&
	    !(slot->quirks & SDHCI_QUIRK_DONT_SET_HISPD_BIT))
		slot->hostctrl |= SDHCI_CTRL_HISPD;
	else
		slot->hostctrl &= ~SDHCI_CTRL_HISPD;
	WR1(slot, SDHCI_HOST_CONTROL, slot->hostctrl);
	/* Some controllers like reset after bus changes. */
	if(slot->quirks & SDHCI_QUIRK_RESET_ON_IOS)
		sdhci_reset(slot, SDHCI_RESET_CMD | SDHCI_RESET_DATA);

	SDHCI_UNLOCK(slot);
	return (0);
}

static int
sdhci_cam_request(struct sdhci_slot *slot, union ccb *ccb)
{
	const struct ccb_mmcio *mmcio;

	mmcio = &ccb->mmcio;

	SDHCI_LOCK(slot);
/*	if (slot->req != NULL) {
		SDHCI_UNLOCK(slot);
		return (EBUSY);
	}
*/
	if (__predict_false(sdhci_debug > 1)) {
		slot_printf(slot, "CMD%u arg %#x flags %#x dlen %u dflags %#x\n",
			    mmcio->cmd.opcode, mmcio->cmd.arg, mmcio->cmd.flags,
			    mmcio->cmd.data != NULL ? (unsigned int) mmcio->cmd.data->len : 0,
			    mmcio->cmd.data != NULL ? mmcio->cmd.data->flags: 0);
	}
	if (mmcio->cmd.data != NULL) {
		if (mmcio->cmd.data->len == 0 || mmcio->cmd.data->flags == 0)
			panic("data->len = %d, data->flags = %d -- something is b0rked",
			    (int)mmcio->cmd.data->len, mmcio->cmd.data->flags);
	}
	slot->ccb = ccb;
	slot->flags = 0;
	sdhci_start(slot);
	SDHCI_UNLOCK(slot);
	if (dumping) {
		while (slot->ccb != NULL) {
			sdhci_generic_intr(slot);
			DELAY(10);
		}
	}
	return (0);
}
#endif /* MMCCAM */

uint32_t
sdhci_get_host_id(device_t reqdev)
{
	device_t dev;
	struct hci_softc *sc;
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	if (slot == NULL)
		return ((uint32_t)-1);

	dev = slot->bus;
	if (dev == NULL)
		return ((uint32_t)-1);

	sc = device_get_softc(dev);
	if (sc == NULL)
		return ((uint32_t)-1);

	return (sc->num);
}

void sdhci_set_ext_clk_switch(device_t reqdev, bool enable)
{
    struct hci_softc *sc = device_get_softc(reqdev);
    struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;
    SDHCI_LOCK(slot);
    slot->ops->drv_ext_clk_switch(sc, enable);
    SDHCI_UNLOCK(slot);
}