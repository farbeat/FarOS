/*-
 * Copyright (c) 2008 Alexander Motin <mav@FreeBSD.org>
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
 *
 * $FreeBSD: releng/12.2/sys/dev/sdhci/sdhci.h 346394 2019-04-19 15:50:51Z bz $
 */

#ifndef DEV_MMC_HCI_H
#define DEV_MMC_HCI_H

#include "mmc_subr_bus.h"
#include "mmcreg.h"

#define	HCI_CAPABILITIES		0x40
#define	 HCI_TIMEOUT_CLK_MASK		0x0000003F
#define	 HCI_TIMEOUT_CLK_SHIFT		0
#define	 HCI_TIMEOUT_CLK_UNIT		0x00000080
#define	 HCI_CLOCK_BASE_MASK		0x00003F00
#define	 HCI_CLOCK_V3_BASE_MASK		0x0000FF00
#define	 HCI_CLOCK_BASE_SHIFT		8
#define	 HCI_MAX_BLOCK_MASK		0x00030000
#define	 HCI_MAX_BLOCK_SHIFT		16
#define	 HCI_CAN_DO_8BITBUS		0x00040000
#define	 HCI_CAN_DO_ADMA2		0x00080000
#define	 HCI_CAN_DO_HISPD		0x00200000
#define	 HCI_CAN_DO_SDMA		0x00400000
#define	 HCI_CAN_DO_SUSPEND		0x00800000
#define	 HCI_CAN_VDD_330		0x01000000
#define	 HCI_CAN_VDD_300		0x02000000
#define	 HCI_CAN_VDD_180		0x04000000

#define	 HCI_CAN_DO_ADMA2_64BIT		0x10000000
#define	 HCI_CAN_ASYNC_INTR		0x20000000
#define	 HCI_SLOTTYPE_MASK		0xC0000000

#define	 HCI_SLOTTYPE_REMOVABLE		0x00000000	/* used for SD slot */
#define	 HCI_SLOTTYPE_EMBEDDED		0x40000000	/* used for EMMC slot */
#define	 HCI_SLOTTYPE_SDIO		0x80000000

#define	HCI_CAPABILITIES2		0x44
#define	 HCI_CAN_SDR50			0x00000001
#define	 HCI_CAN_SDR104			0x00000002
#define	 HCI_CAN_HS200			HCI_CAN_SDR104
#define	 HCI_CAN_DDR50			0x00000004
#define	 HCI_CAN_DRIVE_TYPE_A		0x00000010
#define	 HCI_CAN_DRIVE_TYPE_C		0x00000020
#define	 HCI_CAN_DRIVE_TYPE_D		0x00000040
#define	 HCI_RETUNE_CNT_MASK		0x00000F00
#define	 HCI_RETUNE_CNT_SHIFT		8
#define	 HCI_TUNE_SDR50			0x00002000
#define	 HCI_RETUNE_MODES_MASK		0x0000C000
#define	 HCI_RETUNE_MODES_SHIFT		14
#define	 HCI_CLOCK_MULT_MASK		0x00FF0000
#define	 HCI_CLOCK_MULT_SHIFT		16
#define	 MCI_CAN_DO_IDMAC		0x01000000	/* non-standard */
#define	 MCI_CAN_DO_IDMAC_64BIT		0x02000000	/* non-standard */
#define	 HCI_CAN_MMC_HS400		0x80000000	/* non-standard */
#define	 HCI_CAN_MMC_HS400ES		0x40000000	/* non-standard */

#define	HCI_QUIRK_POLL_CARD_PRESENT	(1 << 17)

/*
 * Hci: host controller interface
 * capacities of host controller
 * that all host will have
 */
typedef struct {
	uint32_t caps;
	uint32_t caps2;
	uint32_t min_clk;
	uint32_t max_clk;
	uint32_t quirks;
} hci_caps;

typedef void (*sdio_irq_cb)(void *arg);

/*
 * hci: host controller interface
 */
struct hci_softc {
	uint32_t num;
	device_t dev;
	void *peri_base;
	dpal_resource_t *irq_res;
	void  *slot;
	uint32_t mmchs_clk_id;
	uint32_t peri_crg_reg;
    uint32_t base;
	uint32_t baseclk_hz;
	uint32_t cmd_and_mode;
	uint32_t sdhci_clkdiv;
    bool disable_highspeed;
    bool force_card_present;
	hci_caps caps;	/* The low-level interface between platform and the drivers. */
	uint32_t tuning_phase;
#define	HCI_EVENT_PLUGGED	(1U << 0)
#define	HCI_EVENT_UNPLUGGED	(1U << 1)

/* Note: the mask does not include POST, it's done-event. */
#define HCI_EVENT_MASK	(HCI_EVENT_PLUGGED | HCI_EVENT_UNPLUGGED)
	/*
	 * Power control of cd-sw,
	 * when set, the host would not set the power again,
	 * that will result in miss-behave of power on/off
	 */
	uint32_t no_power;
	sdio_irq_cb sdio_cb;
	void *sdio_arg;
	bool sdio_rescan_allow;
};

/*
 * hci: host controller interface
 */
typedef struct {
	int (*read_ivar)(device_t bus, device_t child, int which, uintptr_t *result);
	int (*write_ivar)(device_t bus, device_t child, int which, uintptr_t value);
	void (*update_ios)(device_t brdev, device_t reqdev);
	int (*request)(device_t brdev, device_t reqdev, struct mmc_request *req);
	void (*handle_error)(device_t reqdev, const struct mmc_request *req);
	int (*tune)(device_t brdev, device_t reqdev);
	int (*switch_vccq)(device_t brdev, device_t reqdev);
	u_int (*get_card_busy)(device_t brdev, device_t reqdev);
	int (*retune)(device_t brdev, device_t reqdev, bool reset);
	int (*get_ro)(device_t brdev, device_t reqdev);
	int (*acquire_host)(device_t brdev, device_t reqdev);
	int (*release_host)(device_t brdev, device_t reqdev);
	uint8_t (*readb)(device_t dev, uint32_t off);
	uint16_t (*readw)(device_t dev, uint32_t off);
	uint32_t (*readl)(device_t dev, uint32_t off);
	void (*writeb)(device_t dev, uint32_t off, uint8_t val);
	void (*writew)(device_t dev, uint32_t off, uint16_t val);
	void (*writel)(device_t dev, uint32_t off, uint32_t val);
	bool (*get_card_present)(device_t brdev);
	void (*set_card_present)(device_t brdev, uint32_t p);
	bool (*proc_get_card_present)(device_t brdev);
	bool (*proc_get_card_attach)(device_t brdev);
	void (*set_uhs_timing)(device_t brdev);
	void (*enable_sdio_irq)(device_t brdev);
	int (*registor_sdio_irq)(device_t brdev, sdio_irq_cb cb, void *arg);
	uint32_t (*get_host_id)(device_t reqdev);
    bool (*set_ext_clk_switch)(device_t bus, bool en);
} hci_ops;

/*
 * The HISI platform, HI3516EV200,HI3516EV300,HI3518EV300 are using
 *  the same host controller which are very alike.
 * Register the board special methods which are different between the above boards
 *  that host can normally use it.
 */
typedef struct {
	uint32_t (*set_clock)(struct hci_softc *sc, uint32_t clock);
	void (*extra_init)(struct hci_softc *sc);
	void (*slot_init)(struct hci_softc *sc);
	void (*pre_clock)(struct hci_softc *sc);
	void (*post_clock)(struct hci_softc *sc);
	void (*set_phase)(struct hci_softc *sc, uint32_t timing);
	void (*enable_drv_sample)(struct hci_softc *sc, uint32_t en);
	void (*select_sample_phase)(struct hci_softc *sc, uint32_t phase);
	int (*switch_volt)(struct hci_softc *sc, uint32_t vccq);
	void (*soft_reset)(struct hci_softc *sc);
	uint32_t (*get_taps)(struct hci_softc *sc);
	void (*set_drv)(struct hci_softc *sc, uint32_t timing, uint32_t clk);
	void (*dump_regs)(struct hci_softc *sc);
    void (*drv_ext_clk_switch)(struct hci_softc *sc, bool en);
} hci_slot_ops;

/*
 * This interface would be used when device is not in bus
 * cannot use READ_IVAR because it has 2 parameters, bus and device.
 * that the slot_* interface may not have device in slot.
 */
static __inline void slot_set_card_present(device_t bus, uint32_t p)
{
	hci_ops *ops = (hci_ops *)get_mmcbus_ops(bus);
	ops->set_card_present(bus, p);
}

static __inline bool slot_get_card_present(device_t bus)
{
	hci_ops *ops = (hci_ops *)get_mmcbus_ops(bus);
	return (ops->get_card_present(bus));
}

static __inline bool slot_proc_get_card_present(device_t bus)
{
	hci_ops *ops = (hci_ops *)get_mmcbus_ops(bus);
	return (ops->proc_get_card_present(bus));
}

static __inline bool slot_proc_get_card_attach(device_t bus)
{
	hci_ops *ops = (hci_ops *)get_mmcbus_ops(bus);
	return (ops->proc_get_card_attach(bus));
}

static __inline bool slot_set_ext_clk_switch(device_t bus, bool en)
{
	hci_ops *ops = (hci_ops *)get_mmcbus_ops(bus);
	return (ops->set_ext_clk_switch(bus, en));
}

void init_slot_methods(int slot, hci_slot_ops *slot_ops);
void hci_dma_clean_cache(void *addr, uint32_t size);
void hci_dma_inv_cache(void *addr, uint32_t size);
int hci_registor_sdio_irq(device_t brdev, sdio_irq_cb cb, void *arg);
struct hci_softc *get_mmc_resource(int index);
int set_mmc_resource(struct hci_softc *sc, int index);
#endif /* DEV_MMC_HCI_H */
