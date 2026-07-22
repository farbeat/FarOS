/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)kern_time.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/12.2/sys/kern/kern_time.c 366292 2020-09-30 16:11:37Z kib $");

#include "dpal.h"
#ifdef DPALCFG_DRIVERS_SDIO
#include "sdio.h"
#endif
#include "block.h"

#include "mmc_subr_bus.h"
#include "mmc_private.h"
#include "mmc.h"
#include "mmcsd.h"
#include "mmc_adapt.h"

MMC_MUTEX mmc_resource_mutex = INVALID_MMC_MUTEX;

/*
 * ppsratecheck(): packets (or events) per second limitation.
 *
 * Return 0 if the limit is to be enforced (e.g. the caller
 * should drop a packet because of the rate limitation).
 *
 * maxpps of 0 always causes zero to be returned.  maxpps of -1
 * always causes 1 to be returned; this effectively defeats rate
 * limiting.
 *
 * Note that we maintain the struct timeval for compatibility
 * with other bsd systems.  We reuse the storage and just monitor
 * clock ticks for minimal overhead.
 */
int
ppsratecheck(mmc_timeval *lasttime, int *curpps, int maxpps)
{
	int now;
	/*
	 * Reset the last time and counter if this is the first call
	 * or more than a second has passed since the last update of
	 * lasttime.
	 */
	now = dpal_gettime_mono(true);
	if (lasttime->tv_sec == 0 || (u_int)(now - lasttime->tv_sec) >=
		CFG_BASE_CORE_TICK_PER_SECOND) {
		lasttime->tv_sec = now;
		*curpps = 1;
		return (maxpps != 0);
	} else {
		(*curpps)++;		/* NB: ignore potential overflow */
		return (maxpps < 0 || *curpps <= maxpps);
	}
}

device_t
get_any_card(device_t busdev)
{
	int index;
	device_t dev = NULL;
	struct mmc_softc *sc = NULL;

	if (busdev != NULL)
		dev = mmcdev_get_child(busdev);
	if (dev != NULL)
		sc = device_get_softc(dev);
	if (sc == NULL || sc->child_list == NULL)
		return (NULL);
	for (index = 0; index < sc->child_count; index++) {
		if (sc->child_list[index] == NULL)
			continue;
		break;
	}
	return ((index != sc->child_count) ? sc->child_list[index] : NULL);
}

device_t
mmc_new_device(device_t busdev)
{
    uint32_t retval;
    device_t dev = busdev;
	device_t newdev;
	struct mmc_softc *sc;

    newdev = (device_t)dpal_zalloc(sizeof(struct device_dpal));
    if (newdev == NULL)
        return (NULL);
    sc = (struct mmc_softc *)dpal_zalloc(sizeof(struct mmc_softc));
    if (sc == NULL)
        goto err3;

    sc->dev = newdev;

	device_set_nameunit(sc->dev, "mmc");
	device_set_parent(sc->dev, dev);

    retval = (uint32_t)mmcdev_priv_create(sc->dev, &mmc_ops, sc, NULL);
    if (retval)
        goto err2;

    mmcdev_set_child(dev, newdev);
    retval = dpal_event_init(&sc->host_sync_event);
    if (retval)
        goto err1;

    return (sc->dev);
err1:
	mmcdev_set_child(dev, NULL);
    mmcdev_priv_destroy(sc->dev);
err2:
    dpal_free(sc);
err3:
    dpal_free(newdev);
    mmc_err("Failed to new mmc device!\n");
    return (NULL);
}

void
mmc_del_device(device_t busdev)
{
    device_t dev = mmcdev_get_child(busdev);
    struct mmc_softc *sc = device_get_softc(dev);

    (void)dpal_event_destroy(&sc->host_sync_event);
    mmcdev_set_child(busdev, NULL);
    device_set_softc(dev, NULL);
    mmcdev_priv_destroy(sc->dev);
    dpal_free(sc->dev);
    sc->dev = NULL;
    dpal_free(sc);
}

device_t
mmcsd_new_device(device_t busdev)
{
    int ret = 0;
    device_t dev = busdev;
	device_t newdev;
	struct mmcsd_softc *sc;

    newdev = dpal_zalloc(sizeof(struct device_dpal));
    if (newdev == NULL)
        return (NULL);
    sc = dpal_zalloc(sizeof(struct mmcsd_softc));
    if (sc == NULL) {
        goto err1;
    }

    sc->dev = newdev;
	device_set_parent(sc->dev, dev);
    ret = mmcdev_priv_create(sc->dev, NULL, sc, NULL);
    if (ret) {
        goto err2;
    }
    mmcdev_set_child(dev, newdev);

    return (sc->dev);
err2:
    dpal_free(sc);
err1:
    dpal_free(newdev);
    mmc_err("Failed to new mmcsd sdevice!\n");
    return (NULL);
}

void
mmcsd_del_device(device_t busdev)
{
	device_t dev = mmcdev_get_child(busdev);
	struct mmcsd_softc *sc = device_get_softc(dev);
	mmcdev_set_child(busdev, NULL);
	device_set_softc(dev, NULL);
	mmcdev_priv_destroy(sc->dev);
	dpal_free(sc->dev);
	sc->dev = NULL;
	free(sc);
}
#ifdef DPALCFG_DRIVERS_SDIO
device_t
sdio_new_device(device_t busdev)
{
    int ret = 0;
    device_t newdev = NULL;
    struct sdio_softc *sc = NULL;

    newdev = dpal_zalloc(sizeof(struct device_dpal));
    if (newdev == NULL)
        return (NULL);

    sc = dpal_zalloc(sizeof(struct sdio_softc));
    if (sc == NULL) {
        goto err1;
    }

    sc->dev = newdev;
 	device_set_nameunit(sc->dev, "sdio");
	device_set_parent(sc->dev, busdev);
    sc->mmcbus = busdev;
    ret = mmcdev_priv_create(sc->dev, NULL, sc, NULL);
    if (ret) {
        goto err2;
    }
    mmcdev_set_child(busdev, newdev);

    return (sc->dev);
err2:
    dpal_free(sc);
err1:
    dpal_free(newdev);
    mmc_err("Failed to new sdio device!\n");
    return (NULL);
}

void
sdio_del_device(device_t busdev)
{
	device_t dev = mmcdev_get_child(busdev);
	struct sdio_softc *sc = device_get_softc(dev);
	mmcdev_set_child(busdev, NULL);
	device_set_softc(dev, NULL);
	mmcdev_priv_destroy(sc->dev);
	dpal_free(sc->dev);
	sc->dev = NULL;
	free(sc);
}
#endif

int
mmc_device_del_child(device_t child)
{
	struct mmcsd_softc *sc; /* For mmcsd_softc and sdio_softc */
	struct mmc_ivars *ivars;
	struct mmcsd_part *part;
	uint32_t card_type;
	int ret = 0;
	if (child == NULL) {
		return (MMC_ERR_INVALID);
	}
	sc = (struct mmcsd_softc *)device_get_softc(child);
	if (sc == NULL) {
		ret = MMC_ERR_INVALID;
		goto out1;
	}
	ivars = device_get_ivars(child);
	if (ivars == NULL) {
		ret = MMC_ERR_INVALID;
		goto out2;
	}
	card_type = ivars->mode;
	if (card_type == mode_sd || card_type == mode_mmc) {
		part = sc->part[EXT_CSD_PART_CONFIG_ACC_DEFAULT];
		if (part == NULL)
			goto out3;
		mmc_set_card_present(child, MMC_CAP_CARD_ABSENT);
		if (mmcsd_exit(part) != 0)
			mmc_err("mmcsd exit failed!\n");
#ifdef DPALCFG_FS_VFS
		int disk_id = los_get_diskid_byname(part->name);
		(void)los_disk_deinit(disk_id);
#endif
		deinit_part(part);
		sc->part[EXT_CSD_PART_CONFIG_ACC_DEFAULT] = NULL;
	}
#ifdef DPALCFG_DRIVERS_SDIO
	else if (card_type == mode_sdio) {
		mmc_set_card_present(child, MMC_CAP_CARD_ABSENT);
		sdio_dettach(child);
	}
#endif

out3:
	dpal_free(ivars);
out2:
	dpal_free(sc);
out1:
	mmcdev_priv_destroy(child);
	dpal_free(child);
	return (ret);
}

int
mmcbus_generic_attach(device_t dev)
{
	int status = 0;
	device_t child;
	int card_type;

	struct mmc_softc *sc = device_get_softc(dev);
	dev_logd(sc->dev, "attached child_count:%d.\n", sc->child_count);
	for (int i = 0; i < sc->child_count; i++) {
		child = sc->child_list[i];
		if (child != NULL) {
			card_type = mmc_get_card_type(child);
			if (card_type == mode_sd || card_type == mode_mmc)
				status = mmcsd_attach(child);
#ifdef DPALCFG_DRIVERS_SDIO
			else if (card_type == mode_sdio)
				status = sdio_attach(child);
#endif
			if (status != 0)
				return (status);
		}
	}
	return (0);
}

int
bus_width_value(device_t dev)
{
	if (mmc_get_bus_width(dev) == bus_width_1)
		return (1);
	if (mmc_get_bus_width(dev) == bus_width_4)
		return (4);
	return (8);
}

