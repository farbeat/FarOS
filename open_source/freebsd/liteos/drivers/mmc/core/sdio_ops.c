/*-
 * Copyright (c) 2017 Ilya Bakulin.  All rights reserved.
 * Copyright (c) 2018-2019 The FreeBSD Foundation
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * Portions of this software were developed by Björn Zeeb
 * under sponsorship from the FreeBSD Foundation.
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
/*
 * Implements the (kernel specific) SDIO parts.
 * This will hide all cam(4) functionality from the SDIO driver implementations
 * which will just be newbus/device(9) and hence look like any other driver for,
 * e.g., PCI.
 * The sdiob(4) parts effetively "translate" between the two worlds "bridging"
 * messages from MMCCAM to newbus and back.
 */

#include "securec.h"
#include "mmc_subr.h"
#include "mmc_subr_bus.h"
#include "mmcreg.h"
#include "bridge.h"
#include "mmcbrvar.h"
#include "mmcvar.h"
#include "mmc_private.h"
#include "mmc_adapt.h"
#include "sdio_reg.h"
#include "sdio.h"
#include "sdio_ops.h"

#define	CMD_RETRIES	3

static const uint8_t dtr_val[16] = {0, 10, 12, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80};
static const uint32_t dtr_unit[8] = {10000, 100000, 1000000, 10000000, 0, 0, 0, 0};

int
sdio_io_send_op_cond(const struct mmc_softc *sc, uint32_t ocr, uint32_t *rocr)
{
	struct mmc_command cmd = {0};
	int err = MMC_ERR_NONE;
	int i;

	cmd.opcode = IO_SEND_OP_COND;
	cmd.arg = ocr;
	cmd.flags = MMC_RSP_R4 | MMC_CMD_BCR;
	cmd.data = NULL;

	for (i = 0; i < 1000; i++) {
		err = mmc_wait_for_cmd(sc->dev, sc->dev, &cmd, CMD_RETRIES);
		if (err != MMC_ERR_NONE)
			break;
		if ((cmd.resp[0] & MMC_OCR_CARD_BUSY) || (ocr & MMC_OCR_VOLTAGE) == 0)
			break;
		err = -ETIMEDOUT;
		mmc_delay_ms(10);
	}
	if (rocr != 0 && err == MMC_ERR_NONE)
		*rocr = cmd.resp[0];
	return (err);
}

static int
sdio_rw_direct(const struct mmc_softc *sc, uint8_t fn, uint32_t addr, bool wr, uint8_t *val)
{
	struct mmc_command cmd = {0};
	int error;

	if ((addr & ~0x1FFFF) || (val == NULL)) {
		dev_loge(sc->dev, "invalid sdio card addr.\n ");
		return (-EINVAL);
	}

	cmd.opcode = SD_IO_RW_DIRECT;
	cmd.flags = MMC_RSP_R5 | MMC_CMD_AC;
	cmd.arg = SD_IO_RW_FUNC(fn) | SD_IO_RW_ADR(addr);

	if (wr)
		cmd.arg |= SD_IO_RW_WR | SD_IO_RW_RAW | SD_IO_RW_DAT(*val);
	error = mmc_wait_for_cmd(sc->dev, sc->dev, &cmd, CMD_RETRIES);
	if (error != MMC_ERR_NONE)
		return (-error);
	if (cmd.resp[0] & R5_RESP_CHECK)
		return (-EPERM);
	if (wr == false)
		*val = cmd.resp[0] & 0xFF;
	return (0);
}

/* This file invokes sdio_rw_direct, and other files invoke sdio_rw_direct_ext. */
int
sdio_rw_direct_ext(device_t mmcbus, uint8_t fn, uint32_t addr, bool wr, uint8_t *val)
{
	return (sdio_rw_direct(device_get_softc(mmcbus), fn, addr, wr, val));
}

int
sdio_rw_direct_acqr_bus(device_t mmcbus, device_t child, uint8_t fn,
    uint32_t addr, bool wr, uint8_t *val)
{
	int error;

	MMCBUS_ACQUIRE_BUS(mmcbus, child);
	error = sdio_rw_direct(device_get_softc(mmcbus), fn, addr, wr, val);
	MMCBUS_RELEASE_BUS(mmcbus, child);
	return (error);
}

static int
sdio_rw_direct_fun0_merge(const struct mmc_softc *sc, uint32_t addr, uint8_t in_bits, uint8_t in_mask)
{
	int error;
	uint8_t val;

	error = sdio_rw_direct(sc, 0, addr, false, &val);
	if (error != 0)
		return (error);
	val &= ~in_mask;
	val |= in_bits & in_mask;
	error = sdio_rw_direct(sc, 0, addr, true, &val);

	return (error);
}

static int
sdio_read_direct_sc(const struct mmc_softc *sc, uint8_t fn, uint32_t addr,
    uint8_t *val)
{
	int error;
	uint8_t v;

	error = sdio_rw_direct(sc, fn, addr, false, &v);
	if (error == 0 && val != NULL)
		*val = v;
	return (error);
}

static int
sdio_get_common_cis_addr(const struct mmc_softc *sc, uint32_t base_addr, uint32_t *addr)
{
	int error;
	uint32_t a = 0;
	uint8_t val;

	error = sdio_read_direct_sc(sc, 0, SD_IO_CCCR_CISPTR + 0 + base_addr, &val);
	if (error != 0)
		goto err;
	a = val;
	error = sdio_read_direct_sc(sc, 0, SD_IO_CCCR_CISPTR + 1 + base_addr, &val);
	if (error != 0)
		goto err;
	a |= (val << 8);
	error = sdio_read_direct_sc(sc, 0, SD_IO_CCCR_CISPTR + 2 + base_addr, &val);
	if (error != 0)
		goto err;
	a |= (val << 16);

	if (a < SD_IO_CIS_START || a > (SD_IO_CIS_START + SD_IO_CIS_SIZE)) {
err:
		/*
		 * workaround: for some sdio dev, CIS address not in scope of agreement requirements,
		 * just output warning information, but the output param is still assigned.
		 */
		dev_logd(sc->dev, "bad CIS address: %#04x, error %d\n", a, error);
	}
	if (error == 0 && addr != NULL)
		*addr = a;

	return (error);
}

static int
parse_cistpl_manfid(const struct mmc_softc *sc, struct sdio_func *func, uint32_t addr, struct mmc_ivars *ivar)
{
	uint16_t man_id;
	uint16_t device_id;
	int error;
	uint8_t v;

#define	MANFID_ERR_OUT(error)	\
	if (error != 0)		\
		goto manfid_err;

	/* TPLMID */
	error = sdio_read_direct_sc(sc, 0, addr++, &v);
	MANFID_ERR_OUT(error);
	man_id = v;
	error = sdio_read_direct_sc(sc, 0, addr++, &v);
	MANFID_ERR_OUT(error);
	man_id |= (v << 8);
	/* TPLMID */
	error = sdio_read_direct_sc(sc, 0, addr++, &v);
	MANFID_ERR_OUT(error);
	device_id = v;
	error = sdio_read_direct_sc(sc, 0, addr, &v);
	MANFID_ERR_OUT(error);
	device_id |= (v << 8);
	if (func != NULL) { /* fn 1 -7 */
		func->manufacturer_id = man_id;
		func->device_id = device_id;
	} else if (ivar != NULL) { /* fn0 */
		ivar->cis.man_id = man_id;
		ivar->cis.prod_id = device_id;
	} else {
		device_printf(sc->dev, "no manfid parsed.\n");
	}
	dev_logd(sc->dev, "Vendor 0x%04X product 0x%04X\n", man_id, device_id);
manfid_err:
#undef MANFID_ERR_OUT
	return (error);
}

static int
parse_cistpl_funce(const struct mmc_softc *sc, struct sdio_func *func, uint32_t addr, struct mmc_ivars *ivar)
{
	uint8_t tplfe_type;
	uint8_t max_speed;
	int error;
	uint8_t v;

#define	CISTPL_ERR_OUT(error)	\
	if (error != 0)		\
		goto cistpl_err;

	error = sdio_read_direct_sc(sc, 0, addr++, &tplfe_type);
	CISTPL_ERR_OUT(error);
	if (tplfe_type == 0) {
		if (ivar == NULL) {
			dev_loge(sc->dev, "ivar is NULL.\n");
			return (-EIO);
		}
		error = sdio_read_direct_sc(sc, 0, addr++, &v);
		CISTPL_ERR_OUT(error);
		ivar->cis.blk_size = v;
		error = sdio_read_direct_sc(sc, 0, addr++, &v);
		CISTPL_ERR_OUT(error);
		ivar->cis.blk_size |= (v << 8);
		error = sdio_read_direct_sc(sc, 0, addr++, &max_speed);
		CISTPL_ERR_OUT(error);
		ivar->tran_speed = dtr_unit[max_speed & 0x7] * dtr_val[(max_speed >> 3) & 0xF];
		dev_logi(sc->dev, "Func 0: max_blk_size 0x%04X max_trans_speed %u\n",
			ivar->cis.blk_size, ivar->tran_speed);
	} else {
		if ((func == NULL) || (ivar == NULL)) {
			dev_loge(sc->dev, "func or ivar is NULL.\n");
			return (-EIO);
		}
		error = sdio_read_direct_sc(sc, 0, addr + 0xB, &v);
		CISTPL_ERR_OUT(error);
		func->max_blk_size = v;
		error = sdio_read_direct_sc(sc, 0, addr + 0xC, &v);
		CISTPL_ERR_OUT(error);
		func->max_blk_size |= (v << 8);
		if (ivar->sdio_rev > CCCR_SDIO_REV_1_00) {
			error = sdio_read_direct_sc(sc, 0, addr + 0x1B, &v);
			CISTPL_ERR_OUT(error);
			func->en_timeout_ms = v;
			error = sdio_read_direct_sc(sc, 0, addr + 0x1C, &v);
			CISTPL_ERR_OUT(error);
			func->en_timeout_ms |= (v << 8);
		} else {
			func->en_timeout_ms = 100;
		}
		dev_logi(sc->dev, "Fun1~7: max_blk_size 0x%08X en_timeout_ms 0x%08X\n",
		    func->max_blk_size, func->en_timeout_ms);
	}
cistpl_err:
#undef CISTPL_ERR_OUT
	return (error);
}

int
sdio_func_read_cis(device_t mmcbus, struct sdio_func *func, void *ivar_in)
{
	struct mmc_ivars *ivar = (struct mmc_ivars *)ivar_in;
	struct mmc_softc *sc = device_get_softc(mmcbus);
	uint32_t cis_addr = 0;
	uint32_t addr;
	int error;
	uint8_t tpl_count, tuple_id, tuple_len;
	uint32_t func_addr_base = 0;

	if (func != NULL)
		func_addr_base = FUNC_ADDR_BASE(func->func_num);

#define	ERR_OUT(error)	\
	if (error != 0)	\
		goto err;

	error = sdio_get_common_cis_addr(sc, func_addr_base, &cis_addr);
	ERR_OUT(error);
	tpl_count = 0;
	do {
		addr = cis_addr;
		error = sdio_read_direct_sc(sc, 0, addr++, &tuple_id);
		ERR_OUT(error);
		dev_logt(sc->dev, "cur_tpl id: 0x%04X\n", tuple_id);
		if (tuple_id == SD_IO_CISTPL_END) {
			dev_logt(sc->dev, "normal exit from do-while.\n");
			break;
		}
		if (tuple_id == SD_IO_CISTPL_NULL) {
			dev_logd(sc->dev, "ignore the CISTPL_NULL tuple.\n");
			cis_addr++;
			continue;
		}
		error = sdio_read_direct_sc(sc, 0, addr++, &tuple_len);
		ERR_OUT(error);
		dev_logt(sc->dev, "cur_tpl len: 0x%02X\n", tuple_len);
		if (tuple_len == SD_IO_CISTPL_END) {
			dev_logt(sc->dev, "normal exit from do-while.\n");
			break;
		}
		if (tuple_len == SD_IO_CISTPL_NULL) {
			dev_loge(sc->dev, "0-length tuple. cur_tpl id: 0x%04X\n", tuple_id);
			return (-EIO);
		}
		switch (tuple_id) {
		case SD_IO_CISTPL_MANFID:
			error = parse_cistpl_manfid(sc, func, addr, ivar);
			break;
		case SD_IO_CISTPL_FUNCE:
			error = parse_cistpl_funce(sc, func, addr, ivar);
			break;
		default:
			break;
		}
		cis_addr += tuple_len + 2;
		tpl_count++;
	} while(tpl_count < MAX_TPL_NUM);
err:
#undef ERR_OUT
	return (error);
}

static int
sdio_read_cccr(const struct mmc_softc *sc, struct mmc_ivars *ivar)
{
	int error;
	uint8_t cccr_rev;
	uint8_t val;

	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_START, false, &cccr_rev);
	if (error != 0)
		return (error);
	if ((cccr_rev & 0xF) > CCCR_REV_3_00) {
		dev_loge(sc->dev, "unknow cccr version %u\n", cccr_rev);
		return (-EINVAL);
	}
	if ((cccr_rev & 0xF) == CCCR_REV_2_00 || (cccr_rev & 0xF) == CCCR_REV_3_00) {
		error = sdio_rw_direct(sc, 0, SD_IO_CCCR_SPEED_SEL, false, &val);
		if (error != 0)
			return (error);
		if (val & CCCR_SPEED_SHS) {
			mmc_setbit(&ivar->timings, bus_timing_sd_hs);
			if (mmcbr_get_caps(sc->dev) & MMC_CAP_4_BIT_DATA)
				ivar->bus_width = bus_width_4;
			ivar->hs_tran_speed = SD_HS_MAX;
		}
	}
	if ((cccr_rev & 0xF) >= CCCR_REV_1_10 && (cccr_rev & 0xF) <= CCCR_REV_3_00) {
		error = sdio_rw_direct(sc, 0, SD_IO_CCCR_PWR_CTL, false, &val);
		if (error != 0)
			return (error);
		if (val & CCCR_POWER_SMPC)
			ivar->sdio_cccr_caps |= SDIO_CCCR_HI_POWER;
	}
	ivar->sdio_rev = (cccr_rev >> 4) & 0xF;
	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_CARDCAP, false, &val);
	if (error != 0)
		return (error);
	if ((val & CCCR_CC_4BLS) && (mmcbr_get_caps(sc->dev) & MMC_CAP_4_BIT_DATA))
		ivar->bus_width = bus_width_4;
	if (val & CCCR_CC_LSC)
		mmc_setbit(&ivar->timings, bus_timing_normal);
	if (val & CCCR_CC_SMB)
		ivar->sdio_cccr_caps |= SDIO_CCCR_MULTI_BLK;
	return (error);
}

static int
sdio_get_card_info(const struct mmc_softc *sc, struct mmc_ivars *ivar)
{

	int error;
	error = sdio_func_read_cis(sc->dev, NULL, ivar);
	if (error != 0)
		return (error);
	error = sdio_read_cccr(sc,ivar);
	return (error);
}

int
sdio_set_card_bus_width(const struct mmc_softc *sc, enum mmc_bus_width bw)
{
	int error;
	uint8_t val;
	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_BUS_WIDTH, 0, &val);
	if (error != 0)
		return (error);
	val &= ~CCCR_BUS_WIDTH_MASK;
	switch (bw) {
	case bus_width_1:
		val |= CCCR_BUS_WIDTH_1;
		break;
	case bus_width_4:
		val |= CCCR_BUS_WIDTH_4;
		break;
	case bus_width_8:
		val |= CCCR_BUS_WIDTH_8;
		break;
	}
	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_BUS_WIDTH, 1, &val);

	return (error);
}

int
sdio_disable_cd(const struct mmc_softc *sc, const struct mmc_ivars *ivar)
{
	if (ivar != NULL && !(ivar->quirks & MMC_QUIRK_DIS_CD))
		return (0);
	return (sdio_rw_direct_fun0_merge(sc, SD_IO_CCCR_BUS_WIDTH, CCCR_CD_DISABLE, CCCR_CD_DISABLE));
}

int
sdio_switch_hispeed(const struct mmc_softc *sc, bool enable)
{
	int error;
	uint8_t speed_mode;

	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_SPEED_SEL, false, &speed_mode);
	if (error != 0)
		return (error);
	speed_mode = enable ? (speed_mode | CCCR_SPEED_EHS) : (speed_mode & (~CCCR_SPEED_EHS));
	error = sdio_rw_direct(sc, 0, SD_IO_CCCR_SPEED_SEL, true, &speed_mode);

	return (error);
}

int
sdio_io_card_reset(const struct mmc_softc *sc)
{
	return (sdio_rw_direct_fun0_merge(sc, SD_IO_CCCR_CTL, CCCR_CTL_RES, CCCR_CTL_RES));
}

int
mmc_discover_sdio(const struct mmc_softc *sc, struct mmc_ivars *ivar)
{
	if ((sc == NULL) || (ivar == NULL))
		return (-EINVAL);
	return (sdio_get_card_info(sc, ivar));
}
