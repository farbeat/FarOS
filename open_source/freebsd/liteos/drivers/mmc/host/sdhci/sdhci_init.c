/*-
 * Copyright (c) 2013 Ian Lepore <ian@freebsd.org>
 * Copyright (c) 2011 Ben Gray <ben.r.gray@gmail.com>.
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
 */

#include "proc.h"
#include "mmcreg.h"
#include "sdhci.h"
#include "sdhci_private.h"
#include "host.h"
#include "dpal.h"
#include "hci.h"
#include "soc_mmc.h"

/*
 * The MMCHS hardware has a few control and status registers at the beginning of
 * the device's memory map, followed by the standard sdhci register block.
 * Different SoCs have the register blocks at different offsets from the
 * beginning of the device.  Define some constants to map out the registers we
 * access, and the various per-SoC offsets.  The SDHCI_REG_OFFSET is how far
 * beyond the MMCHS block the SDHCI block is found; it's the same on all SoCs.
 */
#define MMCHS_SYSCONFIG				0x010
#define MMCHS_SYSCONFIG_RESET		(1U << 1)
#define MMCHS_SYSSTATUS				0x014
#define MMCHS_SYSSTATUS_RESETDONE	(1U << 0)
#define MMCHS_CON					0x02C
#define MMCHS_CON_DW8				(1U << 5)
#define MMCHS_CON_DVAL_8_4MS		(3U << 9)
#define MMCHS_CON_OD				(1U << 0)
#define MMCHS_SYSCTL				0x12C
#define MMCHS_SYSCTL_CLKD_MASK		0x3FF
#define MMCHS_SYSCTL_CLKD_SHIFT		6
#define MMCHS_SD_CAPA				0x140
#define MMCHS_SD_CAPA_VS18			(1U << 26)
#define MMCHS_SD_CAPA_VS30			(1U << 25)
#define MMCHS_SD_CAPA_VS33			(1U << 24)

typedef enum {
    IRQ_NONE        = (0U << 0),  /* interrupt was not from this device. */
    IRQ_HANDLED     = (1U << 0),  /* interrupt was handled by this device. */
    IRQ_WAKE_THREAD = (1U << 1)   /* handler requests to wake the handler thread. */
} irqreturn_t;

static uint8_t
hi_sdhci_readb(device_t dev, uint32_t off)
{
	struct hci_softc *sc = device_get_softc(dev);
	return (hci_readb(sc, off));
}

static uint16_t
hi_sdhci_readw(device_t dev, uint32_t off)
{
	struct hci_softc *sc = device_get_softc(dev);
	return (hci_readw(sc, off));
}

static uint32_t
hi_sdhci_readl(device_t dev, uint32_t off)
{
	struct hci_softc *sc = device_get_softc(dev);
	uint32_t val32;

	val32 = hci_readl(sc, off);
	/* Force the card-present state if necessary. */
	if (off == SDHCI_PRESENT_STATE && sc->force_card_present)
		val32 |= SDHCI_CARD_PRESENT;

	return (val32);
}

static void
hi_sdhci_writeb(device_t dev, uint32_t off, uint8_t val)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_writeb(sc, off, val);
}

static void
hi_sdhci_writew(device_t dev, uint32_t off, uint16_t val)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_writew(sc, off, val);
}

static void
hi_sdhci_writel(device_t dev, uint32_t off, uint32_t val)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_writel(sc, off, val);
}

static int hi_sdhci_intr(int undef, void *arg)
{
	(void)undef;
	struct hci_softc *sc = arg;

	sdhci_generic_intr((struct sdhci_slot *)sc->slot);
	return (IRQ_HANDLED);
}

static int
hi_sdhci_get_ro(device_t brdev, device_t reqdev)
{
	(void)reqdev;
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;

	return (sdhci_generic_get_ro(slot->bus, slot->dev));
}

static bool
hi_sdhci_get_card_present(device_t brdev)
{
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;

	return (sdhci_card_state(slot) ? true : false);
}

static bool
hi_sdhci_proc_get_card_present(device_t brdev)
{
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;

	if (slot->opt & SDHCI_NON_REMOVABLE)
		return (slot->dev != NULL);

	return (sdhci_card_state(slot) ? true : false);
}

static bool
hi_sdhci_proc_card_attach(device_t brdev)
{
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;
	return (slot->host.card_present ? true : false);
}

static int
hi_sdhci_detach(struct dpal_platform_device *pdev)
{
	struct hci_softc *sc = device_get_softc(&pdev->dev);
	if (sc == NULL)
		return (-EINVAL);
    dpal_irq_delete(sc->irq_res->start, sc);
    (void)sdhci_cleanup_slot((struct sdhci_slot *)sc->slot);
    dpal_free(sc->slot);
    sc->slot = NULL;
    mmcdev_priv_destroy(sc->dev);
    dpal_free(sc);
    return (0);
}

static void
hi_set_uhs_timing(device_t brdev)
{
	uint16_t host_ctrl2;
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;
	struct mmc_ios *ios = &slot->host.ios;

	host_ctrl2 = hci_readw(sc, SDHCI_HOST_CONTROL2);
	/* Select Bus Speed Mode for host */
	host_ctrl2 &= ~SDHCI_CTRL2_UHS_MASK;
	if ((ios->timing == bus_timing_mmc_hs200) ||
	    (ios->timing == bus_timing_uhs_sdr104))
		host_ctrl2 |= SDHCI_CTRL2_UHS_SDR104;
	else if (ios->timing == bus_timing_uhs_sdr12)
		host_ctrl2 |= SDHCI_CTRL2_UHS_SDR12;
	else if (ios->timing == bus_timing_uhs_sdr25)
		host_ctrl2 |= SDHCI_CTRL2_UHS_SDR25;
	else if (ios->timing == bus_timing_uhs_sdr50)
		host_ctrl2 |= SDHCI_CTRL2_UHS_SDR50;
	else if ((ios->timing == bus_timing_uhs_ddr50) ||
	    (ios->timing == bus_timing_mmc_ddr52))
		host_ctrl2 |= SDHCI_CTRL2_UHS_DDR50;
	else if ((ios->timing == bus_timing_mmc_hs400) ||
	    (ios->timing == bus_timing_mmc_hs400es))
		host_ctrl2 |= SDHCI_CTRL2_MMC_HS400;
	else /* Legacy mode, no need to set SDHCI_HOST_CONTROL2 */
		return;

	hci_writew(sc, SDHCI_HOST_CONTROL2, host_ctrl2);
}

void
sdhci_handle_error(device_t reqdev, const struct mmc_request *req)
{
	struct sdhci_slot *slot = device_get_ivars(reqdev);
	struct mmc_command *cur_cmd = slot->curcmd;
	(void)req;
	if (cur_cmd == NULL)
		return;

	if (cur_cmd->error == MMC_ERR_NO_REP) {
		dev_loge(slot->bus, "Controller timeout, cmd opcode:%u.\n", slot->curcmd->opcode);
		SDHCI_LOCK(slot);
		sdhci_reset(slot, SDHCI_RESET_CMD);
		sdhci_reset(slot, SDHCI_RESET_DATA);
		slot->req = NULL;
		slot->curcmd = NULL;
		sdhci_dump_regs(slot);
		SDHCI_UNLOCK(slot);
	}
}

void
sdhci_set_card_present(device_t brdev, uint32_t p)
{
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;
	slot->host.card_present = p;
}

static uint32_t
sdhci_get_card_busy(device_t brdev, device_t reqdev)
{
	(void)reqdev;
	uint32_t present_state;
	present_state = hi_sdhci_readl(brdev, SDHCI_PRESENT_STATE);
	/* Check whether DAT[0] is 0 */
	return (!(present_state & SDHCI_STATE_DAT0_MASK));
}

void sdhci_enable_sdio_irq(device_t brdev)
{
	struct hci_softc *sc = device_get_softc(brdev);
	struct sdhci_slot *slot = (struct sdhci_slot *)sc->slot;
	SDHCI_LOCK(slot);
	slot->intmask |= SDHCI_INT_CARD_INT;
	hi_sdhci_writel(brdev, SDHCI_INT_ENABLE, slot->intmask);
	hi_sdhci_writel(brdev, SDHCI_SIGNAL_ENABLE, slot->intmask);
	SDHCI_UNLOCK(slot);
}

/* Methods for the SDHCI layer. */
static hci_ops hi_sdhci_methods = {
	/* Bus interface */
	.read_ivar = sdhci_generic_read_ivar,
	.write_ivar = sdhci_generic_write_ivar,

	/* MMC bridge interface */
	.update_ios = sdhci_generic_update_ios,
	.request = sdhci_generic_request,
	.get_ro = hi_sdhci_get_ro,
	.acquire_host = sdhci_generic_acquire_host,
	.release_host = sdhci_generic_release_host,
	.set_uhs_timing = hi_set_uhs_timing,
	.tune = tune_edge,
	.retune = sdhci_generic_retune,
	.switch_vccq = sdhci_generic_switch_vccq,
	.get_card_busy = sdhci_get_card_busy,
	/* SDHCI registers accessors */
	.readb = hi_sdhci_readb,
	.readw = hi_sdhci_readw,
	.readl = hi_sdhci_readl,
	.writeb = hi_sdhci_writeb,
	.writew = hi_sdhci_writew,
	.writel = hi_sdhci_writel,
	.get_card_present = hi_sdhci_get_card_present,
	.proc_get_card_present = hi_sdhci_proc_get_card_present,
	.proc_get_card_attach = hi_sdhci_proc_card_attach,
	.set_card_present = sdhci_set_card_present,
	.handle_error = sdhci_handle_error,
	.enable_sdio_irq = sdhci_enable_sdio_irq,
	.registor_sdio_irq = hci_registor_sdio_irq,
	.get_host_id = sdhci_get_host_id,
	.set_ext_clk_switch = sdhci_set_ext_clk_switch,
};

/* Methods for the internal use only */
void
sdhci_clock_ctrl(struct hci_softc *sc, bool enable)
{
	uint32_t val;

	if (sc == NULL || sc->dev == NULL)
		return;

	hci_ops *ops = get_mmcbus_ops(sc->dev);
	val = (ops->readl)(sc->dev, SDHCI_CLOCK_CONTROL);

	if (enable)
		val |= (SDHCI_CLOCK_CONTROL);
	else
		val &= ~(SDHCI_CLOCK_CONTROL);
	(ops->writel)(sc->dev, (SDHCI_CLOCK_CONTROL), val);
	return;
}

static hci_slot_ops slot_methods;

void
sdhci_task(void)
{
#define WEIGHTED_TIMES    5
	uint32_t i, j;
	uint32_t state;
	uint32_t tmp_state;
	struct hci_softc *sc;
	struct sdhci_slot *slot;

#if !defined(EMMC_BOOT) && defined(DPALCFG_DRIVERS_SD)
	while (1) {
#endif
		for (i = 0; i < MAX_MMC_NUM; i++) {
			sc = get_mmc_resource(i);
			if (sc == NULL || sc->sdio_rescan_allow)
				continue;
			slot = sc->slot;
			if (slot == NULL)
				continue;

			tmp_state = state = sdhci_card_state(slot);
			if (slot->opt & SDHCI_NON_REMOVABLE && state != slot->host.plugged_state) {
				slot->host.plugged_state = state;
				sdhci_card_detect(sc, slot->host.plugged_state ? HCI_EVENT_PLUGGED : HCI_EVENT_UNPLUGGED);
				sc->sdio_rescan_allow = true;
				continue;
			}

			for (j = 1; j < WEIGHTED_TIMES; j++) {
				tmp_state += sdhci_card_state(slot);
				dpal_udelay(10); /* Deley 10us. */
			}
			if (slot->host.plugged_state != state &&
			    tmp_state == state * WEIGHTED_TIMES) {
				slot->host.plugged_state = state;
				sdhci_card_detect(sc, slot->host.plugged_state ? HCI_EVENT_PLUGGED : HCI_EVENT_UNPLUGGED);
			}
		}
#if !defined(EMMC_BOOT) && defined(DPALCFG_DRIVERS_SD)
		dpal_msleep(50); /* Delay 50ms. */
	}
#endif
}

#define SDHCI_THTEAD_STACK_SIZE  0x1000
#define SDHCI_THTEAD_PRIORITY 0X5
static uint32_t
sdhci_thread_create(dpal_tsk_entry_func func, char *name, uint32_t *id)
{
#if !defined(EMMC_BOOT) && defined(DPALCFG_DRIVERS_SD)
    dpal_task_attr_t task_attr;
    (void)memset_s(&task_attr, sizeof(task_attr), 0, sizeof(task_attr));
    task_attr.task_prio = SDHCI_THTEAD_PRIORITY;
    task_attr.stack_size = SDHCI_THTEAD_STACK_SIZE;
    task_attr.task_name = name;
    task_attr.reserved = NULL;
    return dpal_task_create(id, &task_attr, func, NULL);
#else
    (void)func(NULL);
    return 0;
#endif
}

static int
hi_sdhci_attach(struct dpal_platform_device *pdev)
{
	int err;
	struct hci_softc *sc;
	struct sdhci_slot *slot;
	dpal_resource_t *res;
#ifdef CFG_SDIO_EMMC_COEXIST
	static bool emmc_initialized = false;
	static bool sdio_initialized = false;
#else
	static bool mmc_initialized = false;
#endif
	sc = dpal_zalloc(sizeof(struct hci_softc));
	if (sc == NULL)
		return (-ENOMEM);

	sc->slot = dpal_zalloc(sizeof(struct sdhci_slot));
	if (sc->slot == NULL) {
		err = -ENOMEM;
		goto free_sc;
	}

	slot = sc->slot;
	slot->ops = &slot_methods;
	slot->bus = &pdev->dev;
	slot->num = (uint32_t)pdev->id;
	init_slot_methods(slot->num, slot->ops);
	sc->dev = &pdev->dev;
	sc->sdio_rescan_allow = false;
	err = mmcdev_priv_create(sc->dev, &hi_sdhci_methods, sc, slot);
	if (err != 0)
		goto free_slot;

	/*
	* The chip can inherently do dual-voltage (1p8v, 3p3v) on the first
	* device, and only 3p3v on second devices for 16ev200/300 18ev300 dev-board.
	*
	* but it also depend on the hardware implementation, say if emmc is enabled
	* it should do 1p8v, and if SD mode is enabled, it should do 3p3v.
	*/
	res = dpal_platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_loge(sc->dev, "cannot found memory resource\n");
		err = -ENXIO;
		goto destroy_priv;
	}
	sc->base = (uint32_t)res->start;

	/* Resource setup. */
	sc->irq_res = dpal_platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (sc->irq_res == NULL) {
		dev_loge(sc->dev, "cannot found interrupt resource\n");
		err = -ENXIO;
		goto destroy_priv;
	}

	err = (int)dpal_irq_create(sc->irq_res->start, hi_sdhci_intr, 0, "MMC-SD", sc);
	if (err != 0) {
		mmc_err("cannot setup interrupt handler\n");
		err = -EACCES;
		goto destroy_priv;
	}

	res = dpal_platform_get_resource(pdev, IORESOURCE_REG, 1);
	if (res == NULL) {
		device_printf(sc->dev, "cannot found register resource\n");
		err = -ENXIO;
		goto free_irq;
	}

	sc->peri_crg_reg = res->start;
	sc->num = (uint32_t)pdev->id;

	/* Board init */
	if (slot->ops->slot_init != NULL)
		slot->ops->slot_init(sc);

	/* Copy the low-level caps to the slot */
	slot->caps = sc->caps.caps;
	slot->caps2 = sc->caps.caps2;
	slot->min_clk = sc->caps.min_clk;
	slot->max_clk = sc->caps.max_clk;
	slot->quirks = sc->caps.quirks;

	/*
	 * Set up the hardware and go.  Note that this sets many of the
	 * slot.host.* fields, so we have to do this before overriding any of
	 * those values based on spec board, below.
	 */
	err = sdhci_init_slot(sc->dev, slot, pdev->id);
	if (err != 0)
		goto free_irq;

    err = mmc_proc_create(slot->num, slot->bus);
    if (err != 0)
		goto deinit_slot;

	err = set_mmc_resource(sc, slot->num);
	if (err != 0)
		goto clear_mmc_proc;
#ifdef CFG_SDIO_EMMC_COEXIST
    if (slot->num == 0 && !emmc_initialized) {
        emmc_initialized = true;
        err = sdhci_thread_create((dpal_tsk_entry_func)sdhci_task,
            "cdsw_emmc_task", (uint32_t *)&slot->cd_tsk_id);
        if (err) {
            goto clear_mem_resource;
        }
    }
    if (slot->num == 1 && !sdio_initialized) {
        sdio_initialized = true;
        err = sdhci_thread_create((dpal_tsk_entry_func)sdhci_task,
            "cdsw_sdio_task", (uint32_t *)&slot->cd_tsk_id);
        if (err) {
            goto clear_mem_resource;
        }
    }
#else
    if (!mmc_initialized) {
		mmc_initialized = true;
		err = sdhci_thread_create((dpal_tsk_entry_func)sdhci_task,
		    "cdsw_task", (uint32_t *)&slot->cd_tsk_id);
		if (err) {
			goto clear_mem_resource;
		}
	}
#endif
    return (0);

clear_mem_resource:
	(void)set_mmc_resource(NULL, slot->num);

clear_mmc_proc:
deinit_slot:
	sdhci_deinit_slot(slot);

free_irq:
	dpal_irq_delete(sc->irq_res->start, sc);

destroy_priv:
	mmcdev_priv_destroy(sc->dev);

free_slot:
    dpal_free(sc->slot);
    sc->slot = NULL;

free_sc:
    dpal_free(sc);
    return (err);
}

static struct dpal_platform_driver sd_mci_driver[2] = {
	{
		.probe		= hi_sdhci_attach,
		.remove		= hi_sdhci_detach,
		.drv        = {
			.name       = "sdhci0",
		},
	},
	{
		.probe		= hi_sdhci_attach,
		.remove		= hi_sdhci_detach,
		.drv        = {
			.name       = "sdhci1",
		},
	}
};

void
sd_mci_exit(void)
{
	int status;
	uint32_t i;
	uint32_t num = ARRAY_SIZE(sd_mci_driver);

	mmc_proc_exit();
	for (i = 0; i < num; ++i) {
		status = dpal_platform_driver_unregister(&sd_mci_driver[i]);
		if (status != -EINVAL && status != 0)
			dprintf("platform driver unregister error,host id=%u, status=%d\n", i, status);
	}
}

int
SD_MMC_Host_init(void)
{
    int status;
    uint32_t i;
    uint32_t num = ARRAY_SIZE(sd_mci_driver);

    if (mmc_resource_mutex == INVALID_MMC_MUTEX) {
        if (dpal_mux_create(&mmc_resource_mutex))
            return (-ENOMEM);
    }

    status = mmc_proc_init(num);
    if (status != 0) {
        (void)dpal_mux_delete(mmc_resource_mutex);
        dprintf("mmc proc init failed!, status=%d\n", status);
        return (status);
    }

    for (i = 0; i < num; ++i) {
        status = dpal_platform_driver_register(&sd_mci_driver[i]);
        if (status != -EBUSY && status != 0) {
            (void)dpal_mux_delete(mmc_resource_mutex);
            (void)mmc_proc_exit();
            dprintf("platform driver register error,host id=%u, status=%d\n", i, status);
            return (status);
        }
    }

    return (DPAL_OK);
}

int
MMC_HostInitById(unsigned int id)
{
    int status;
    unsigned int count = ARRAY_SIZE(sd_mci_driver);

    if (id >= count) {
        return (-EINVAL);
    }

    if (mmc_resource_mutex == INVALID_MMC_MUTEX) {
        if (dpal_mux_create(&mmc_resource_mutex))
            return (-ENOMEM);
    }

    status = mmc_proc_init(count);
    if (status != 0) {
        (void)dpal_mux_delete(mmc_resource_mutex);
        dprintf("mmc proc init failed!, status=%d\n", status);
        return (status);
    }

    status = dpal_platform_driver_register(&sd_mci_driver[id]);
	if (status != -EBUSY && status != 0) {
        dprintf("platform driver register error, host id=%u, status=%d\n", id, status);
        (void)dpal_mux_delete(mmc_resource_mutex);
        (void)mmc_proc_exit();
        return (status);
    }
    return (0);
}
