/*
#-
# Copyright (c) 2006 M. Warner Losh
# All rights reserved.
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# Portions of this software may have been developed with reference to
# the SD Simplified Specification.  The following disclaimer may apply:
#
# The following conditions apply to the release of the simplified
# specification ("Simplified Specification") by the SD Card Association and
# the SD Group. The Simplified Specification is a subset of the complete SD
# Specification which is owned by the SD Card Association and the SD
# Group. This Simplified Specification is provided on a non-confidential
# basis subject to the disclaimers below. Any implementation of the
# Simplified Specification may require a license from the SD Card
# Association, SD Group, SD-3C LLC or other third parties.
#
# Disclaimers:
#
# The information contained in the Simplified Specification is presented only
# as a standard specification for SD Cards and SD Host/Ancillary products and
# is provided "AS-IS" without any representations or warranties of any
# kind. No responsibility is assumed by the SD Group, SD-3C LLC or the SD
# Card Association for any damages, any infringements of patents or other
# right of the SD Group, SD-3C LLC, the SD Card Association or any third
# parties, which may result from its use. No license is granted by
# implication, estoppel or otherwise under any patent or other rights of the
# SD Group, SD-3C LLC, the SD Card Association or any third party. Nothing
# herein shall be construed as an obligation by the SD Group, the SD-3C LLC
# or the SD Card Association to disclose or distribute any technical
# information, know-how or other confidential information to any third party.
#
# $FreeBSD: releng/12.2/sys/dev/mmc/mmcbr_if.m 321385 2017-07-23 16:11:47Z marius $
#
*/

#ifndef DEV_MMC_MMCBUS_IF_H
#define DEV_MMC_MMCBUS_IF_H

#include "mmcreg.h"
#include "mmc_subr_bus.h"

typedef struct {
	int (*read_ivar)(device_t bus, device_t child, int which, uintptr_t *result);
	int (*write_ivar)(device_t bus, device_t child, int which, uintptr_t value);
	int (*child_location_str)(device_t dev, device_t child, char *buf, size_t buflen);
	void (*retune_pause)(device_t busdev, device_t dev, bool retune);
	void (*retune_unpause)(device_t busdev, device_t dev);
	int (*wait_for_request)(device_t busdev, device_t dev, struct mmc_request *req);
	int (*acquire_bus)(device_t busdev, device_t dev);
	int (*release_bus)(device_t busdev, device_t dev);
} mmcbus_ops;

static __inline int MMCBUS_READ_IVAR(device_t dev, device_t child, int index, uintptr_t *r)
{
	mmcbus_ops *ops = get_mmcbus_ops(dev);
	return (ops->read_ivar(dev, child, index, r));
}

static __inline int MMCBUS_WRITE_IVAR(device_t dev, device_t child, int index, uintptr_t v)
{
	mmcbus_ops *ops = get_mmcbus_ops(dev);
	return (ops->write_ivar(dev, child, index, v));
}

static __inline void MMCBUS_RETUNE_PAUSE(device_t busdev, device_t reqdev,
    bool retune)
{
	mmcbus_ops *ops = get_mmcbus_ops(busdev);
	ops->retune_pause(busdev, reqdev, retune);
}

static __inline void MMCBUS_RETUNE_UNPAUSE(device_t busdev, device_t reqdev)
{
	mmcbus_ops *ops = get_mmcbus_ops(busdev);
	ops->retune_unpause(busdev, reqdev);
}

static __inline int MMCBUS_WAIT_FOR_REQUEST(device_t busdev, device_t reqdev,
    struct mmc_request *req)
{
	mmcbus_ops *ops = get_mmcbus_ops(busdev);
	return (ops->wait_for_request(busdev, reqdev, req));
}

static __inline void MMCBUS_ACQUIRE_BUS(device_t busdev, device_t reqdev)
{
	mmcbus_ops *ops = get_mmcbus_ops(busdev);
	ops->acquire_bus(busdev, reqdev);
}

static __inline void MMCBUS_RELEASE_BUS(device_t busdev, device_t reqdev)
{
	mmcbus_ops *ops = get_mmcbus_ops(busdev);
	ops->release_bus(busdev, reqdev);
}

#endif /* DEV_MMC_MMCBUS_IF_H */
