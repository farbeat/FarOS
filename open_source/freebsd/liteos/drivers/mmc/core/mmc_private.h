/*-
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
 * $FreeBSD: releng/12.2/sys/dev/mmc/mmc_private.h 321385 2017-07-23 16:11:47Z marius $
 */

#ifndef DEV_MMC_PRIVATE_H
#define	DEV_MMC_PRIVATE_H

#include "mmc_type.h"
#include "bridge.h"
#include "sdio_reg.h"
#include "mmcreg.h"
#include "mmc_adapt_os.h"
#include "dpal_mux.h"
#include "stddef.h"

struct mmc_softc {
	device_t dev;
    dpal_mux_cb sc_mtx;
	device_t owner;
	device_t *child_list;
	int child_count;
	uint16_t last_rca;
	uint16_t retune_paused;
	uint8_t retune_needed;
	uint8_t retune_ongoing;
	uint16_t squelched;	/* suppress reporting of (expected) errors */
	int log_count;
    mmc_timeval log_time;
    dpal_event_t host_sync_event; /* Request event*/
#define	MMC_PEND_REQUEST_DONE (1U << 0)
	uint32_t card_init_status;
#define	MMC_CARD_UNINIT		0
#define	MMC_CARD_INIT		1
#define	MMC_CARD_INIT_FAIL	2
	bool recovery;
};

/*
 * Per-card data
 */
struct mmc_ivars {
	uint32_t raw_cid[4];	/* Raw bits of the CID */
	uint32_t raw_csd[4];	/* Raw bits of the CSD */
	uint32_t raw_scr[2];	/* Raw bits of the SCR */
	uint8_t raw_ext_csd[MMC_EXTCSD_SIZE];	/* Raw bits of the EXT_CSD */
	uint32_t raw_sd_status[16];	/* Raw bits of the SD_STATUS */
	uint16_t rca;
	u_char read_only;	/* True when the device is read-only */
	u_char high_cap;	/* High Capacity device (block addressed) */
	enum mmc_card_mode mode;
	enum mmc_bus_width bus_width;	/* Bus width to use */
	struct mmc_cid cid;	/* cid decoded */
	struct mmc_csd csd;	/* csd decoded */
	struct mmc_scr scr;	/* scr decoded */
	struct mmc_sd_status sd_status;	/* SD_STATUS decoded */
	uint32_t sec_count;	/* Card capacity in 512byte blocks */
	uint32_t timings;	/* Mask of bus timings supported */
	uint32_t vccq_120;	/* Mask of bus timings at VCCQ of 1.2 V */
	uint32_t vccq_180;	/* Mask of bus timings at VCCQ of 1.8 V */
	uint32_t tran_speed;	/* Max speed in normal mode */
	uint32_t hs_tran_speed;	/* Max speed in high speed mode */
	uint32_t erase_sector;	/* Card native erase sector size */
	uint32_t cmd6_time;	/* Generic switch timeout [ms] */
	uint32_t quirks;	/* Quirks as per mmc_quirk->quirks */
	char card_id_string[64];/* Formatted CID info (serial, MFG, etc) */
	char card_sn_string[16];/* Formatted serial # for disk->d_ident */
	u_char max_current_limit;
#define	MMC_MAX_CURR_LTD_200	0
#define	MMC_MAX_CURR_LTD_400	1
#define	MMC_MAX_CURR_LTD_600	2
#define	MMC_MAX_CURR_LTD_800	3
	u_char sdio_rev;
	u_char sdio_cccr_caps;
	struct sdio_card_cis cis;
};

extern void host_hw_init(void *slot);

#endif /* DEV_MMC_PRIVATE_H */
