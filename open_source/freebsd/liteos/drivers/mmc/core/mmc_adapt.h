/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)param.h	8.3 (Berkeley) 4/4/95
 * $FreeBSD: releng/12.2/sys/sys/param.h 365618 2020-09-10 23:56:59Z gjb $
 */

#ifndef DEV_MMC_ADAPT_H
#define DEV_MMC_ADAPT_H

#include "dpal.h"
#include "mmc_mutex.h"
#include "mmc_adapt_os.h"
#include "mmc_type.h"
#include "stddef.h"

extern MMC_MUTEX mmc_resource_mutex;
#define mmc_resource_mutex_lock() dpal_mux_pend(mmc_resource_mutex, DPAL_WAIT_FOREVER)
#define mmc_resource_mutex_unlock() dpal_mux_post(mmc_resource_mutex)

#if defined(__FREERTOS__)
#define	slot_printf(slot, fmt, args...)
#else
#define	slot_printf(slot, fmt, args...)	dprintf("%s-slot%u: " fmt, device_get_nameunit(slot->bus), slot->num, ##args)
#endif

#define	dev_loge(dev, fmt, args...)	dprintf("[%s|ERROR] " fmt, device_get_nameunit(dev), ##args)
#define	dev_logw(dev, fmt, args...)	dprintf("[%s|WARN] " fmt, device_get_nameunit(dev), ##args)
#define	mmc_printf(fmt, args...)	dprintf(fmt, ##args)
#define	mmc_trace(fmt, args...)	do {	\
		dprintf("%s:%d: ", __FUNCTION__, __LINE__);	\
		dprintf(fmt, ##args);	\
	} while (0)
#define	mmc_err(fmt, args...)	do {	\
		dprintf("[ERROR] %s:%d: ", __FUNCTION__, __LINE__);	\
		dprintf(fmt, ##args);	\
	} while (0)

#ifdef MMC_TRACE_LEVEL
#define	dev_logi(dev, fmt, args...)	\
    dprintf("[%s|INFO] " fmt, device_get_nameunit(dev), ##args)
#define	dev_logd(dev, fmt, args...)	\
    dprintf("[%s|%s@%s,%d] " fmt, device_get_nameunit(dev), __FUNCTION__, __FILE__, __LINE__, ##args)
#define	dev_logt(dev, fmt, args...)	\
    dprintf("[%s|%s@%s,%d] " fmt, device_get_nameunit(dev), __FUNCTION__, __FILE__, __LINE__, ##args)
#else
#define	dev_logi(dev, fmt, args...)
#define	dev_logd(dev, fmt, args...)
#define	dev_logt(dev, fmt, args...)
#endif

#define	DELAY(t)	dpal_udelay(t)
#define	mmc_align(args, align)	((args) & ~((align) - 1))
#define	mmc_delay_ms(ms)	do {	\
    uint32_t ms_delay = mmc_align(ms, (DPAL_MS_PER_SECOND / DPALCFG_BASE_CORE_TICK_PER_SECOND));	\
    uint32_t rest_delay = ms - ms_delay ;	\
    if (ms_delay)	\
        dpal_msleep(ms_delay);	\
    if (rest_delay) {	\
        dpal_mdelay(rest_delay);	\
    }	\
} while (0)

#define	mmc_setbit(a, i)	*(uint32_t *)(a) |= 1U << (i)
#define	mmc_clrbit(a, i)	*(uint32_t *)(a) &= ~(1U << (i))
#define	mmc_isset(a, i)		((*(uint32_t *)(a) & (1U << (i))) != 0)
#define	mmc_isclr(a, i)		((*(uint32_t *)(a) & (1U << (i))) == 0)

int ppsratecheck(mmc_timeval *lasttime, int *curpps, int maxpps);

device_t mmc_new_device(device_t busdev);
void mmc_del_device(device_t dev);
device_t mmcsd_new_device(device_t busdev);
void mmcsd_del_device(device_t dev);
#ifdef DPALCFG_DRIVERS_SDIO
device_t sdio_new_device(device_t busdev);
void sdio_del_device(device_t dev);
#endif
int mmc_device_del_child(device_t child);

int mmcbus_generic_attach(device_t dev);
device_t get_any_card(device_t busdev);
int bus_width_value(device_t dev);
int wait_card_ready(device_t dev, device_t mmcbus, bool in_exc);
#endif
