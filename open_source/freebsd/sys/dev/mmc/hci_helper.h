/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2004-2005 HighPoint Technologies, Inc.
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
 * $FreeBSD$
 */

#ifndef DEV_MMC_HCI_HELPER_H
#define DEV_MMC_HCI_HELPER_H

#include "hci.h"

typedef void *(*thrd_callback)(void *);

static __inline void
setl(void *p, uint32_t bits)
{
    dpal_writel(dpal_readl(p) | bits, p);
}

static __inline void
clearl(void *p, uint32_t bits)
{
    dpal_writel(dpal_readl(p) & (~bits), p);
}

static __inline void
mergel(void *p, uint32_t mask, uint32_t bits)
{
    uint32_t v = dpal_readl(p) & (~mask);
    v |= mask & bits;
    dpal_writel(v, p);
}

static __inline uint8_t
hci_readb(const struct hci_softc *sc, uintptr_t off)
{
    return dpal_readb((uintptr_t)sc->base + off);
}

static __inline void
hci_writeb(const struct hci_softc *sc, uintptr_t off, uint32_t v)
{
    dpal_writeb(v, (uintptr_t)sc->base + off);
}

static __inline uint16_t
hci_readw(const struct hci_softc *sc, uintptr_t off)
{
    return dpal_readw((uintptr_t)sc->base + off);
}

static __inline void
hci_writew(const struct hci_softc *sc, uintptr_t off, uint32_t v)
{
    dpal_writew(v, (uintptr_t)sc->base + off);
}

static __inline uint32_t
hci_readl(const struct hci_softc *sc, uintptr_t off)
{
    return dpal_readl((uintptr_t)sc->base + off);
}

static __inline void
hci_writel(const struct hci_softc *sc, uintptr_t off, uint32_t v)
{
    dpal_writel(v, (uintptr_t)sc->base + off);
}

static __inline void
hci_setl(const struct hci_softc *sc, uint32_t off, uint32_t bits)
{
	hci_writel(sc, off, hci_readl(sc, off) | bits);
}

static __inline void
hci_clearl(const struct hci_softc *sc, uint32_t off, uint32_t bits)
{
	hci_writel(sc, off, hci_readl(sc, off) & (~bits));
}

static __inline void
hci_mergel(const struct hci_softc *sc, uint32_t off, uint32_t mask, uint32_t bits)
{
	uint32_t v = hci_readl(sc, off) & (~mask);
	v |= mask & bits;
	hci_writel(sc, off, v);
}

static __inline void
hci_dev_readl(const device_t dev, uint32_t off)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_readl(sc, off);
}

static __inline void
hci_dev_setl(const device_t dev, uint32_t off, uint32_t bits)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_setl(sc, off, bits);
}

static __inline void
hci_dev_clearl(const device_t dev, uint32_t off, uint32_t bits)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_clearl(sc, off, bits);
}

static __inline void
hci_dev_mergel(const device_t dev, uint32_t off, uint32_t mask, uint32_t bits)
{
	struct hci_softc *sc = device_get_softc(dev);
	hci_mergel(sc, off, mask, bits);
}

#endif /* DEV_MMC_HCI_HELPER_H */
