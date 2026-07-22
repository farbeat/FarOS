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
 */

#include "mmc_adapt.h"
#include "mmcbrvar.h"
#include "mmc.h"

void
mmc_update_uhs_timing(const uint8_t *switch_res, const struct mmc_softc *sc, struct mmc_ivars *ivar)
{
	device_t dev = sc->dev;
	uint32_t host_caps = mmcbr_get_caps(dev);

	if (!(host_caps & MMC_CAP_SIGNALING_180))
		return;

	if ((switch_res[13] & (1 << SD_SWITCH_SDR104_MODE)) &&
	    (host_caps & MMC_CAP_UHS_SDR104)) {
		mmc_setbit(&ivar->timings, bus_timing_uhs_sdr104);
		mmc_setbit(&ivar->vccq_180, bus_timing_uhs_sdr104);
	}
	if ((switch_res[13] & (1 << SD_SWITCH_DDR50_MODE)) &&
	    (host_caps & MMC_CAP_UHS_DDR50)) {
		mmc_setbit(&ivar->timings, bus_timing_uhs_ddr50);
		mmc_setbit(&ivar->vccq_180, bus_timing_uhs_ddr50);
	}
	if ((switch_res[13] & (1 << SD_SWITCH_SDR50_MODE)) &&
	    (host_caps & MMC_CAP_UHS_SDR50)) {
		mmc_setbit(&ivar->timings, bus_timing_uhs_sdr50);
		mmc_setbit(&ivar->vccq_180, bus_timing_uhs_sdr50);
	}
	if ((switch_res[13] & (1 << SD_SWITCH_SDR25_MODE)) &&
	    (host_caps & MMC_CAP_UHS_SDR25)) {
		mmc_setbit(&ivar->timings, bus_timing_uhs_sdr25);
		mmc_setbit(&ivar->vccq_180, bus_timing_uhs_sdr25);
	}
	if ((switch_res[13] & (1 << SD_SWITCH_SDR12_MODE)) &&
	    (host_caps & MMC_CAP_UHS_SDR12)) {
		mmc_setbit(&ivar->timings, bus_timing_uhs_sdr12);
		mmc_setbit(&ivar->vccq_180, bus_timing_uhs_sdr12);
	}
	dev_logt(dev, "ivar->timings:0x%x, timing mode(switch_res[13]):0x%x, host_caps:0x%x.\n",
	    ivar->timings, switch_res[13], host_caps);
}

static void
update_uhs_pwr(const uint8_t *switch_res, const struct mmc_softc *sc, struct mmc_ivars *ivar)
{
	device_t dev = sc->dev;
	uint32_t hostCaps = mmcbr_get_caps(dev);

	if (mmc_isset(&ivar->timings, bus_timing_uhs_sdr104) ||
	    mmc_isset(&ivar->timings, bus_timing_uhs_ddr50) ||
	    mmc_isset(&ivar->timings, bus_timing_uhs_sdr50)) {
		if (hostCaps & MMC_CAP_MAX_CURRENT_800) {
			if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_800))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_800;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_600))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_600;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_400))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_400;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_200))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_200;
		} else if (hostCaps & MMC_CAP_MAX_CURRENT_600) {
			if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_600))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_600;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_400))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_400;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_200))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_200;
		} else if (hostCaps & MMC_CAP_MAX_CURRENT_400) {
			if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_400))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_400;
			else if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_200))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_200;
		} else if (hostCaps & MMC_CAP_MAX_CURRENT_200) {
			if (switch_res[7] & (1 << SD_SWITCH_MAX_CURRENT_200))
				ivar->max_current_limit = MMC_MAX_CURR_LTD_200;
		}
	} else
		ivar->max_current_limit = MMC_MAX_CURR_LTD_200;
}

int
mmc_get_cmd6_functions(uint8_t *switch_res, struct mmc_softc *sc,
    struct mmc_ivars *ivar)
{
	int err;
	device_t dev = sc->dev;

	/* We don't use driver length, just inquiry the card for info. This function is applied to embedded slots */
	err = mmc_sd_switch(sc, SD_SWITCH_MODE_CHECK,
	    SD_SWITCH_GROUP3, SD_SWITCH_NOCHANGE,
	    switch_res);
	if (err != MMC_ERR_NONE) {
		device_printf(dev, "Error reading driver length %d\n", err);
		goto error;
	}
	dev_logt(dev, "driver strength(switch_res[9]):0x%x.\n", switch_res[9]);

	/* We need to improve power limit for uhs-i mode */
	err = mmc_sd_switch(sc, SD_SWITCH_MODE_CHECK,
	    SD_SWITCH_GROUP4, SD_SWITCH_NOCHANGE,
	    switch_res);
	if (err != MMC_ERR_NONE) {
		device_printf(dev, "Error reading power limit %d\n", err);
		goto error;
	}
	update_uhs_pwr(switch_res, sc, ivar);
	dev_logt(dev, "ivar->max_current_limit:0x%x, driver strength(switch_res[7]):0x%x.\n",
	    ivar->max_current_limit, switch_res[7]);
error:
	return (err);
}

int
mmc_switch_uhs_current(struct mmc_softc *sc, const struct mmc_ivars *ivar)
{
	int err;
	device_t dev = sc->dev;
	u_char switch_res[64];

	err = mmc_sd_switch(sc, SD_SWITCH_MODE_SET,
	    SD_SWITCH_GROUP4, ivar->max_current_limit,
	    switch_res);
	if (err != MMC_ERR_NONE) {
		device_printf(dev, "Failed to switch power limit %d\n", err);
		return (err);
	}
	if (((switch_res[15] >> 4) & 0x0F) != ivar->max_current_limit)
		device_printf(dev, "Failed to switch power limit switch_res[15]:%u, max_current_limit:%u\n",
		    switch_res[15], ivar->max_current_limit);
	return (err);
}