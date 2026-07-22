/*-
 * Copyright (c) 2006 Bernd Walter.  All rights reserved.
 * Copyright (c) 2006 M. Warner Losh.  All rights reserved.
 * Copyright (c) 2017 Marius Strobl <marius@FreeBSD.org>
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
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
 */

#ifndef DEV_MMC_MMCSD_H
#define DEV_MMC_MMCSD_H

#include "bridge.h"
#include "mmc_subr.h"
#include "mmcbrvar.h"
#include "mmcvar.h"
#include "mmc_adapt.h"
#include "mmc_bio.h"

#define	MMCSD_TASK_STACK_SIZE	0x1000 //0x6000
#define	MMCSD_TASK_PRIORITY		0x6

#define	MMCSD_DISK_LOCK(_part)		mtx_lock((_part)->disk_mtx)
#define	MMCSD_DISK_UNLOCK(_part)	mtx_unlock((_part)->disk_mtx)
#define	MMCSD_DISK_LOCK_INIT(_part)	\
        mtx_init((_part)->disk_mtx, (_part)->name, "mmcsd disk", PTHREAD_MUTEX_NORMAL)
#define	MMCSD_DISK_LOCK_DESTROY(_part)	mtx_destroy((_part)->disk_mtx);

#define	MMCSD_PART_NAMELEN	17
struct mmcsd_softc {
	device_t dev;
	device_t mmcbus;
	struct mmcsd_part *part[MMC_PART_MAX];
	enum mmc_card_mode mode;
	u_int max_data;		/* Maximum data size [blocks] */
	uint8_t ext_csd[MMC_EXTCSD_SIZE]; /* Make sure that the start addr is aligned by 4-byte */
	u_int erase_sector;	/* Device native erase sector size [blocks] */
	uint8_t high_cap;	/* High Capacity device (block addressed) */
	uint8_t part_curr;	/* Partition currently switched to */
	uint16_t rca;
	uint32_t flags;
#define	MMCSD_INAND_CMD38	0x0001
#define	MMCSD_USE_TRIM		0x0002
#define	MMCSD_USE_CMD23		4U
	uint32_t cmd6_time;	/* Generic switch timeout [ms] */
	uint32_t part_time;	/* Partition switch timeout [ms] */
	int log_count;
    mmc_timeval log_time;
};

struct mmcsd_part {
    dpal_mux_cb disk_mtx;
	struct mmcsd_softc *sc;
	uint32_t pthread_id;
	int bio_queue_state;
#define	BIO_QUEUE_NORMAL	1
#define	BIO_QUEUE_ABNORMAL	0
	unsigned long eblock, eend;	/* Range remaining after the last erase. */
	u_int cnt;
	u_int type;
	int thread_running;
	int suspend;
	int ioctl;
	bool ro;
	char name[MMCSD_PART_NAMELEN];
	u_int errot_cnt;
    atomic atom_reqs;	/* Current requests in block */
	int part_status;
#define	PART_NORMAL	1
#define	PART_ABNORMAL	0
	uint8_t* buffer;	/* Buffer for non-aligned data transfer */
	uint32_t size;		/* Buffer size */
#define	ALIGNED_BUFF_BLK_NUM	128
	uint32_t align;
	uint32_t card_capacity;
	uint32_t bio_queue_id;
};

extern unsigned long mmcsd_rw(struct mmcsd_part *part, struct bio *bp);
extern int mmcsd_attach(device_t dev);
extern int mmc_hw_reset(device_t mmcbus, device_t dev);
extern int mmcsd_opr(struct mmcsd_part *part, struct bio *bp);
#endif
