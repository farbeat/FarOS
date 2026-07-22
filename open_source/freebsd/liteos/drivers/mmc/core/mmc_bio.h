/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1989, 1993
 *      The Regents of the University of California.  All rights reserved.
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
 *      @(#)buf.h       8.9 (Berkeley) 3/30/95
 * $FreeBSD: releng/12.2/sys/sys/bio.h 335066 2018-06-13 16:48:07Z imp $
 */

#ifndef DEV_MMC_BIO_H
#define	DEV_MMC_BIO_H

#include "mmc_type.h"
#include "stddef.h"
#include "dpal_mux.h"

/* bio_cmd */
#define BIO_READ	0x01	/* Read I/O data */
#define BIO_WRITE	0x02	/* Write I/O data */
#define BIO_DELETE	0x03	/* TRIM or free blocks, i.e. mark as unused */
#define BIO_GETATTR	0x04	/* Get GEOM attributes of object */
#define BIO_FLUSH	0x05	/* Commit outstanding I/O now */
#define BIO_CMD0	0x06	/* Available for local hacks */
#define BIO_CMD1	0x07	/* Available for local hacks */
#define BIO_CMD2	0x08	/* Available for local hacks */
#define BIO_ZONE	0x09	/* Zone command */

/* bio_flags */
#define BIO_ERROR	0x01	/* An error occurred processing this bio. */
#define BIO_DONE	0x02	/* This bio is finished. */
#define BIO_ONQUEUE	0x04	/* This bio is in a queue & not yet taken. */
/*
 * This bio must be executed after all previous bios in the queue have been
 * executed, and before any successive bios can be executed.
 */
#define BIO_ORDERED		0x08
#define	BIO_UNMAPPED	0x10
#define	BIO_TRANSIENT_MAPPING	0x20
#define	BIO_VLIST	0x40

/* Empty classifier tag, to prevent further classification. */
#define	BIO_NOTCLASSIFIED		(void *)(~0UL)

/*
 * The bio structure describes an I/O operation in the kernel.
 */
struct bio {
	uint16_t bio_cmd;		/* I/O operation. */
	uint16_t bio_flags;		/* General flags. */
	off_t	bio_offset;		/* Offset into file. */
	unsigned char *bio_data;	/* Memory, superblocks, indirect etc. */
	int	bio_error;		/* Errno for BIO_ERROR. */
	long	bio_resid;		/* Remaining I/O in bytes. */
	void	(*bio_done)(struct bio *);
	off_t	bio_length;		/* Like bio_bcount */
#ifdef LOSCFG_MMCSD_SG_SUPPORT
	struct mmc_sg *sgs;
	uint32_t sg_count;
	bool sg_mode;
#endif
	/* XXX: these go away when bio chaining is introduced */
	unsigned long sector_start;
	uint32_t sectorsize;
	uint32_t numsectors;
    dpal_event_t wait_event;
#define REQUEST_EVENT_ANSWER	(1U << 0)
	bool in_exc;
};

#endif
