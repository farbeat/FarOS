/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2006 Bernd Walter.  All rights reserved.
 * Copyright (c) 2006 M. Warner Losh.  All rights reserved.
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
 * Portions of this software may have been developed with reference to
 * the SD Simplified Specification.  The following disclaimer may apply:
 *
 * The following conditions apply to the release of the simplified
 * specification ("Simplified Specification") by the SD Card Association and
 * the SD Group. The Simplified Specification is a subset of the complete SD
 * Specification which is owned by the SD Card Association and the SD
 * Group. This Simplified Specification is provided on a non-confidential
 * basis subject to the disclaimers below. Any implementation of the
 * Simplified Specification may require a license from the SD Card
 * Association, SD Group, SD-3C LLC or other third parties.
 *
 * Disclaimers:
 *
 * The information contained in the Simplified Specification is presented only
 * as a standard specification for SD Cards and SD Host/Ancillary products and
 * is provided "AS-IS" without any representations or warranties of any
 * kind. No responsibility is assumed by the SD Group, SD-3C LLC or the SD
 * Card Association for any damages, any infringements of patents or other
 * right of the SD Group, SD-3C LLC, the SD Card Association or any third
 * parties, which may result from its use. No license is granted by
 * implication, estoppel or otherwise under any patent or other rights of the
 * SD Group, SD-3C LLC, the SD Card Association or any third party. Nothing
 * herein shall be construed as an obligation by the SD Group, the SD-3C LLC
 * or the SD Card Association to disclose or distribute any technical
 * information, know-how or other confidential information to any third party.
 *
 * $FreeBSD: releng/12.2/sys/dev/mmc/mmcbrvar.h 338304 2018-08-24 21:08:05Z marius $
 */

#ifndef DEV_MMC_MMCBRVAR_H
#define	DEV_MMC_MMCBRVAR_H

#include "hci.h"

enum mmcbr_device_ivars {
    MMCBR_IVAR_BUS_MODE,
    MMCBR_IVAR_BUS_WIDTH,
    MMCBR_IVAR_CHIP_SELECT,
    MMCBR_IVAR_CLOCK,
    MMCBR_IVAR_F_MIN,
    MMCBR_IVAR_F_MAX,
    MMCBR_IVAR_HOST_OCR,
    MMCBR_IVAR_MODE,
    MMCBR_IVAR_OCR,
    MMCBR_IVAR_POWER_MODE,
    MMCBR_IVAR_RETUNE_REQ,
    MMCBR_IVAR_VDD,
    MMCBR_IVAR_VCCQ,
    MMCBR_IVAR_CAPS,
    MMCBR_IVAR_TIMING,
    MMCBR_IVAR_MAX_DATA,
    MMCBR_IVAR_MAX_BUSY_TIMEOUT,
    /*
     * Return the gap of the DMA which cannot be processed.
     * currently we have 0 for SDHCI and 4 for SDMCI,
     * while the system DMA asked for 64 or 32 alignment,
     * refer to the DMA spec for more details.
     */
     MMCBR_IVAR_DMA_ALIGNMENT,
     MMCBR_IVAR_CARD_ERROR_CNT,
     MMCBR_IVAR_ENABLE_SDIO_IRQ,
};

#define	READ_IVAR(d, c, i, r)		(((hci_ops *)get_mmcbus_ops(d))->read_ivar(d, c, i, r))
#define	WRITE_IVAR(d, c, i, r)		(((hci_ops *)get_mmcbus_ops(d))->write_ivar(d, c, i, r))
#define	MMCBR_REQUEST(b, d, r)		(((hci_ops *)get_mmcbus_ops(b))->request(b, d, r))
#define	MMCBR_HNDL_ERR(b, d)		(((hci_ops *)get_mmcbus_ops(b))->handle_error(b, d))
#define	MMCBR_GET_HOST_ID(b, d)		(((hci_ops *)get_mmcbus_ops(b))->get_host_id(d))

#define	MMCBR_ACQUIRE_HOST(b, r)	(((hci_ops *)get_mmcbus_ops(b))->acquire_host(b, r))
#define	MMCBR_RELEASE_HOST(b, r)	(((hci_ops *)get_mmcbus_ops(b))->release_host(b, r))

#define	MMCBR_EN_SDIO_IRQ(b, d)		(((hci_ops *)get_mmcbus_ops(b))->enable_sdio_irq(b))
#define	MMCBR_REG_SDIO_IRQ(b, c, a)	(((hci_ops *)get_mmcbus_ops(b))->registor_sdio_irq(b, c, a))

static __inline int
mmcbr_get_bus_mode(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_BUS_MODE, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_bus_mode(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_BUS_MODE, v);
}

static __inline int
mmcbr_get_bus_width(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_BUS_WIDTH, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_bus_width(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_BUS_WIDTH, v);
}

static __inline int
mmcbr_get_chip_select(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CHIP_SELECT, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_chip_select(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CHIP_SELECT, v);
}

static __inline int
mmcbr_get_clock(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CLOCK, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_clock(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CLOCK, v);
}

static __inline int
mmcbr_get_f_max(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_F_MAX, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_f_max(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_F_MAX, v);
}

static __inline int
mmcbr_get_f_min(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_F_MIN, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_f_min(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_F_MIN, v);
}
static __inline int
mmcbr_get_host_ocr(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_HOST_OCR, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_host_ocr(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_HOST_OCR, v);
}

static __inline int
mmcbr_get_mode(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_MODE, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_mode(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_MODE, v);
}
static __inline u_int
mmcbr_get_ocr(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_OCR, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_ocr(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_OCR, v);
}
static __inline int
mmcbr_get_power_mode(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_POWER_MODE, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_power_mode(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_POWER_MODE, v);
}

static __inline int
mmcbr_get_retune_req(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_RETUNE_REQ, &v);
	return ((int)v);
}

static __inline int
mmcbr_get_vdd(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_VDD, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_vdd(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_VDD, v);
}

static __inline int
mmcbr_get_vccq(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_VCCQ, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_vccq(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_VCCQ, v);
}

static __inline u_int
mmcbr_get_caps(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CAPS, &v);
	return ((u_int)v);
}

static __inline void
mmcbr_set_caps(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CAPS, v);
}
static __inline int
mmcbr_get_timing(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_TIMING, &v);
	return ((int)v);
}

static __inline void
mmcbr_set_timing(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_TIMING, v);
}

static __inline int
mmcbr_get_max_data(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_MAX_DATA, &v);
	return ((int)v);
}

static __inline u_int
mmcbr_get_max_busy_timeout(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_MAX_BUSY_TIMEOUT, &v);
	return ((u_int)v);
}

static __inline void
mmcbr_set_max_busy_timeout(device_t dev, u_int t)
{
	uintptr_t v = (uintptr_t)t;
	WRITE_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_MAX_BUSY_TIMEOUT, v);
}

static int __inline
mmcbr_update_ios(device_t dev)
{
	hci_ops *ops = get_mmcbus_ops(device_get_parent(dev));
	ops->update_ios(device_get_parent(dev), dev);
	return (0);
}

static int __inline
mmcbr_tune(device_t dev)
{
	hci_ops *ops = get_mmcbus_ops(device_get_parent(dev));
	return (ops->tune(device_get_parent(dev), dev));
}

static int __inline
mmcbr_retune(device_t dev, u_int reset)
{
	hci_ops *ops = get_mmcbus_ops(device_get_parent(dev));
	return (ops->retune(device_get_parent(dev), dev, reset));
}

static int __inline
mmcbr_switch_vccq(device_t dev)
{
	hci_ops *ops = get_mmcbus_ops(device_get_parent(dev));
	if (ops->switch_vccq == NULL)
		return (0); /* No need to switch. */
	return (ops->switch_vccq(device_get_parent(dev), dev));
}

static u_int __inline
mmcbr_get_card_busy(device_t dev)
{
	device_t slot_dev = device_get_parent(dev);
	hci_ops *ops = get_mmcbus_ops(slot_dev);
	if (ops->get_card_busy == NULL)
		return (1); /* Show card is busy. */
	return (ops->get_card_busy(slot_dev, dev));
}

static int __inline
mmcbr_get_ro(device_t dev)
{
	hci_ops *ops = get_mmcbus_ops(device_get_parent(dev));
	return (ops->get_ro(device_get_parent(dev), dev));
}

static u_int __inline
mmcbr_get_card_present(device_t dev)
{
	return (slot_get_card_present(device_get_parent(dev)));
}

static __inline void
mmcbr_set_card_present(device_t dev, u_int t)
{
	slot_set_card_present(device_get_parent(dev), t);
}

static __inline u_int
mmcbr_get_dma_alignment(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_DMA_ALIGNMENT, &v);
	return ((u_int)v);
}

static __inline u_int
mmcbr_get_card_ec(device_t dev)
{
	uintptr_t v;
	READ_IVAR(device_get_parent(dev), dev, MMCBR_IVAR_CARD_ERROR_CNT, &v);
	return ((u_int)v);
}

static __inline void
mmcbr_enable_sdio_irq(device_t dev)
{
	MMCBR_EN_SDIO_IRQ(device_get_parent(dev), dev);
}

static __inline int
mmcbr_registor_sdio_irq(device_t dev, sdio_irq_cb cb, void *arg)
{
	return (MMCBR_REG_SDIO_IRQ(device_get_parent(dev), cb, arg));
}

static __inline void
mmcbr_set_ext_clk_switch(device_t dev, bool enable)
{
	slot_set_ext_clk_switch(device_get_parent(dev), enable);
}
#endif /* DEV_MMC_MMCBRVAR_H */
