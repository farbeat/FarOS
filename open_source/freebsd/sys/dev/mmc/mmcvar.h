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
 * $FreeBSD: releng/12.2/sys/dev/mmc/mmcvar.h 326255 2017-11-27 14:52:40Z pfg $
 */

#ifndef DEV_MMC_MMCVAR_H
#define DEV_MMC_MMCVAR_H

#include "mmcbus_if.h"

enum mmc_device_ivars {
    MMC_IVAR_SPEC_VERS,
    MMC_IVAR_DSR_IMP,
    MMC_IVAR_MEDIA_SIZE,
    MMC_IVAR_RCA,
    MMC_IVAR_SECTOR_SIZE,
    MMC_IVAR_TRAN_SPEED,
    MMC_IVAR_READ_ONLY,
    MMC_IVAR_HIGH_CAP,
    MMC_IVAR_CARD_TYPE,
    MMC_IVAR_BUS_WIDTH,
    MMC_IVAR_ERASE_SECTOR,
    MMC_IVAR_MAX_DATA,
    MMC_IVAR_CMD6_TIMEOUT,
    MMC_IVAR_QUIRKS,
    MMC_IVAR_CARD_ID_STRING,
    MMC_IVAR_CARD_SN_STRING,
    MMC_IVAR_AU_SIZE,
    MMC_IVAR_CARD_PRESENT,
    MMC_IVAR_AUTO_CMD23,
    MMC_IVAR_CCC_ERASE,
    MMC_IVAR_CCC_SC,	/* Speed_class of mmc_sd_status */
    MMC_IVAR_CCC_GRADE,	/* Grade of mmc_sd_status */
    MMC_IVAR_CID,
    MMC_IVAR_CAPACITY,
    MMC_IVAR_CCCR_CAPS,
    MMC_IVAR_CIS_MANID,
    MMC_IVAR_CIS_PROID,
};

/*
 * Simplified accessors for mmc devices
 */
static __inline uint8_t
mmc_get_spec_vers(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_SPEC_VERS, &v);
	return ((uint8_t)v);
}

static __inline void
mmc_set_spec_vers(device_t dev, uint8_t t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_SPEC_VERS, v);
}

static __inline int
mmc_get_dsr_imp(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_DSR_IMP, &v);
	return ((int)v);
}

static __inline void
mmc_set_dsr_imp(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_DSR_IMP, v);
}

static __inline uint32_t
mmc_get_media_size(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_MEDIA_SIZE, &v);
	return ((uint32_t)v);
}

static __inline void
mmc_set_media_size(device_t dev, uint32_t t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_MEDIA_SIZE, v);
}

static __inline int
mmc_get_rca(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_RCA, &v);
	return ((int)v);
}

static __inline void
mmc_set_rca(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_RCA, v);
}
static __inline int
mmc_get_sector_size(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_SECTOR_SIZE, &v);
	return ((int)v);
}

static __inline void
mmc_set_sector_size(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_SECTOR_SIZE, v);
}

static __inline int
mmc_get_tran_speed(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_TRAN_SPEED, &v);
	return ((int)v);
}

static __inline void
mmc_set_tran_speed(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_TRAN_SPEED, v);
}

static __inline int
mmc_get_read_only(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_READ_ONLY, &v);
	return ((int)v);
}

static __inline void
mmc_set_read_only(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_READ_ONLY, v);
}
static __inline int
mmc_get_high_cap(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_HIGH_CAP, &v);
	return ((int)v);
}

static __inline void
mmc_set_high_cap(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_HIGH_CAP, v);
}

static __inline int
mmc_get_card_type(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_TYPE, &v);
	return ((int)v);
}

static __inline void
mmc_set_card_type(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_TYPE, v);
}


static __inline int
mmc_get_bus_width(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_BUS_WIDTH, &v);
	return ((int)v);
}

static __inline void
mmc_set_bus_width(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_BUS_WIDTH, v);
}

static __inline int
mmc_get_erase_sector(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_ERASE_SECTOR, &v);
	return ((int)v);
}

static __inline void
mmc_set_erase_sector(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_ERASE_SECTOR, v);
}

static __inline int
mmc_get_max_data(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_MAX_DATA, &v);
	return ((int)v);
}

static __inline void
mmc_set_max_data(device_t dev, int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_MAX_DATA, v);
}

static __inline u_int
mmc_get_cmd6_timeout(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CMD6_TIMEOUT, &v);
	return ((u_int)v);
}

static __inline void
mmc_set_cmd6_timeout(device_t dev, u_int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_CMD6_TIMEOUT, v);
}

static __inline u_int
mmc_get_quirks(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_QUIRKS, &v);
	return ((u_int)v);
}

static __inline void
mmc_set_quirks(device_t dev, u_int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_QUIRKS, v);
}

static __inline const char *
mmc_get_card_id_string(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_ID_STRING, &v);
	return ((const char *)v);
}

static __inline void
mmc_set_card_id_string(device_t dev, const char *t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_ID_STRING, v);
}
static __inline const char *
mmc_get_card_sn_string(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_SN_STRING, &v);
	return ((const char *)v);
}

static __inline void
mmc_set_card_sn_string(device_t dev, const char *t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_SN_STRING, v);
}

static __inline u_int
mmc_get_au_size(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_AU_SIZE, &v);
	return ((u_int)v);
}

static __inline u_int
mmc_get_card_present(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_PRESENT, &v);
	return ((u_int)v);
}

static __inline void
mmc_set_card_present(device_t dev, u_int t)
{
	uintptr_t v = (uintptr_t)t;
	MMCBUS_WRITE_IVAR(device_get_parent(dev), dev, MMC_IVAR_CARD_PRESENT, v);
}

static __inline u_int
mmc_get_cmd23_cap(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_AUTO_CMD23, &v);
	return ((u_int)v);
}

static __inline u_int
mmc_get_erase_cap(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CCC_ERASE, &v);
	return ((u_int)v);
}

/*
 * speed class of mmc_sd_status
 */
static __inline
u_int mmc_get_sc(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CCC_SC, &v);
	return ((u_int)v);
}

/*
 * grade class of mmc_sd_status
 */
static __inline u_int
mmc_get_grade(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CCC_GRADE, &v);
	return ((u_int)v);
}

static __inline void *
mmc_get_cid(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CID, &v);
	return ((void *)v);
}

static __inline u_int
mmc_get_capacity(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CAPACITY, &v);
	return ((u_int)v);
}

static __inline u_int
mmc_get_cccr_caps(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CCCR_CAPS, &v);
	return ((u_int)v);
}

static __inline u_int
mmc_get_cis_manid(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CIS_MANID, &v);
	return ((u_int)v);
}

static __inline u_int
mmc_get_cis_proid(device_t dev)
{
	uintptr_t v;
	MMCBUS_READ_IVAR(device_get_parent(dev), dev, MMC_IVAR_CIS_PROID, &v);
	return ((u_int)v);
}
#endif /* DEV_MMC_MMCVAR_H */
