/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2006 M. Warner Losh <imp@FreeBSD.org>
 * Copyright (c) 2017 Marius Strobl <marius@FreeBSD.org>
 * Copyright (c) 2015-2016 Ilya Bakulin <kibab@FreeBSD.org>
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
 * $FreeBSD$
 */
#ifndef DEV_MMC_SDIO_REG_H
#define DEV_MMC_SDIO_REG_H

#include "mmc_type.h"

#define	IO_SEND_OP_COND		5
#define	SD_IO_RW_DIRECT		52
#define	SD_IO_RW_EXTENDED	53

#define R5_RESP_CHECK			(R5_ERROR | R5_FUNCTION_NUMBER | R5_OUT_OF_RANGE)

/* cccr/fbr version */
#define	CCCR_REV_1_00			0x00 /*1.00 */
#define	CCCR_REV_1_10			0x01 /*1.10 */
#define	CCCR_REV_2_00			0x02 /*2.00 */
#define	CCCR_REV_3_00 			0x03 /*3.00 */
#define	CCCR_SDIO_REV_1_00		0x00 /*1.00 */
#define	CCCR_SDIO_REV_1_10		0x01 /*1.10 */
#define	CCCR_SDIO_REV_1_20		0x02 /*1.20 */
#define	CCCR_SDIO_REV_2_00		0x03 /*2.00 */
#define	CCCR_SDIO_REV_3_00		0x04 /*3.00 */	
#define	CCCR_INT_FUNC_EN_MASK	0xFE
#define	CCCR_INT_EN_MASTER		0x01	
#define	CCCR_BUS_WIDTH_MASK		0x03
#define	CCCR_BUS_WIDTH_8		(3u << 0) /* only for embedded sdio */
#define CCCR_CD_DISABLE			0x80

#define  CCCR_CC_SMB			(1u << 1) /* CMD53 block mode support */
#define	CCCR_CC_LSC				(1u << 6)
#define	CCCR_CC_4BLS			(1u << 7)

#define	SD_IO_CCCR_PWR_CTL		0x12
/* POWER CONTROL */
#define	CCCR_POWER_SMPC			(1u << 0)
#define	SD_IO_CCCR_SPEED_SEL	0x13
#define	CCCR_SPEED_SHS			0x01
#define	CCCR_SPEED_EHS			(1u << 1)

/* Function Basic Registers (FBR) */
#define	SD_IO_FBR_EXT_IF		0x01
#define SD_IO_FBR_F_SIZE		0x00100 /* Size of each function */
#define SD_IO_FBR_CIS_OFFSET	0x9  /* Offset of this function's info block within CIS area */
#define SD_IO_FBR_IOBLKSZ		0x10 /* Block size for CMD53 block mode operations */
#define	SD_IO_FBR_CSA			0x0C
/* CIS tuple codes (based on PC Card 16) */
#define	SD_IO_CISTPL_NULL		0x00
/* ivar->sdio_cccr_caps self_defined */
#define	SDIO_CCCR_MULTI_BLK	(1u << 0)  /* the card's ability to exec CMD53 in the block mode */
#define	SDIO_CCCR_HI_POWER	(1u << 1)
#define	SD_HS_MAX			50000000
#define	MAX_TPL_NUM			20

struct sdio_card_cis {
	uint16_t man_id;
	uint16_t prod_id;
	uint16_t blk_size;
};

#endif /* DEV_MMC_SDIO_REG_H */
