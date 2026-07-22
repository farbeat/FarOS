/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/12.2/sys/dev/mmc/mmcsd.c 350876 2019-08-11 22:32:52Z ian $");

#include "securec.h"
#include "mmc_type.h"
#include "mmc_adapt.h"
#include "mmcbrvar.h"
#include "mmcvar.h"
#include "block.h"
#include "dpal.h"
#include "dpal_reg_config.h"
#include "dpal_errno.h"

#ifdef EMMC_BOOT
#include "serial_dw.h"
#include "securec_loader.h"
#endif

#ifdef DPALCFG_FS_VFS
#include "fs/fs.h"
#include "sys/mount.h"
#endif
#include "mmcsd.h"

#define MMC_DEV_NAME_LEN    20
static char dev_name[MMC_DEV_NAME_LEN]; /* Store dev name card-%u */
#define	MMC_BIO_QUEUE_LEN	128

#define	MMCSD_CMD_RETRIES	5

#define	MMCSD_FMT_BOOT		"mmcsd%dboot"
#define	MMCSD_FMT_GP		"mmcsd%dgp"
#define	MMCSD_FMT_RPMB		"mmcsd%drpmb"
#define	MMCSD_LABEL_ENH		"enh"

static const char *errmsg[] =
{
	"None",
	"Timeout",
	"Bad CRC",
	"Fifo",
	"Failed",
	"Invalid",
	"NO MEMORY",
	"DMA",
	"NO response",
	"Card unplugged",
	"Unknown",
};

#define	LOG_PPS		5 /* Log no more than 5 errors per second. */

/* Disk routines */
static void mmcsd_task(void *arg);

/* RMPB cdev interface */
static int mmcsd_add_part(struct mmcsd_softc *sc, u_int type,
    uint64_t media_size, bool ro);
static unsigned long mmcsd_delete(struct mmcsd_part *part, struct bio *bp);
static const char *mmcsd_errmsg(int e);
static uintmax_t mmcsd_pretty_size(uint64_t size, char *unit);
static int mmcsd_switch_part(device_t bus, device_t dev, uint16_t rca,
    u_int part);

static void
biodone(struct bio *bp)
{
	bp->bio_flags |= BIO_DONE;
	(void)dpal_event_write(&bp->wait_event, REQUEST_EVENT_ANSWER);
}

static void
clear_req_queue(struct mmcsd_part *part)
{
	struct bio *bp;
	MMCSD_DISK_LOCK(part);
	part->bio_queue_state = BIO_QUEUE_ABNORMAL;
	while (dpal_queue_read(part->bio_queue_id, &bp, sizeof(char *), 0) == 0) {
		bp->bio_flags |= BIO_ERROR;
		(void)dpal_event_write(&bp->wait_event, REQUEST_EVENT_ANSWER);
	}
	(void)dpal_queue_delete(part->bio_queue_id);
	MMCSD_DISK_UNLOCK(part);
}

static uint32_t
mmcsd_thread_create(uintptr_t arg, uint32_t *id)
{
    dpal_task_attr_t task_attr;
    (void)memset_s(&task_attr, sizeof(task_attr), 0, sizeof(task_attr));
    task_attr.task_prio = MMCSD_TASK_PRIORITY;
    task_attr.stack_size = MMCSD_TASK_STACK_SIZE;
    task_attr.task_name = "mmcsd_task";
    task_attr.reserved = NULL;
    debug_printf();
    return dpal_task_create(id, &task_attr, (dpal_tsk_entry_func)mmcsd_task, (void *)arg);
}

static uint32_t
mmcsd_thread_delete(uint32_t id)
{
    return (dpal_task_delete(id));
}

int
mmcsd_attach(device_t dev)
{
	device_t mmcbus;
	struct mmcsd_softc *sc;
	const uint8_t *ext_csd;
	int sector_size;
	int err;
	uint32_t quirks;
	bool ro;

	sc = device_get_softc(dev);
	sc->dev = dev;
	sc->mmcbus = mmcbus = device_get_parent(dev);
	sc->mode = mmcbr_get_mode(mmcbus);
	/*
	 * Note that in principle with an SDHCI-like re-tuning implementation,
	 * the maximum data size can change at runtime due to a device removal/
	 * insertion that results in switches to/from a transfer mode involving
	 * re-tuning, iff there are multiple devices on a given bus.  Until now
	 * mmc(4) lacks support for rescanning already attached buses, however,
	 * and sdhci(4) to date has no support for shared buses in the first
	 * place either.
	 */
	sc->max_data = mmc_get_max_data(dev);
	sc->high_cap = mmc_get_high_cap(dev);
	sc->rca = mmc_get_rca(dev);
	sc->cmd6_time = mmc_get_cmd6_timeout(dev);
	quirks = mmc_get_quirks(dev);

	(void)sprintf_s(dev_name, sizeof(dev_name), "card-%u", sc->rca);
	device_set_nameunit(dev, dev_name);

	sc->part_curr = 0xff; /* No part */

	/* Only MMC >= 4.x devices support EXT_CSD. */
	if (mmc_get_spec_vers(dev) >= 4) {
		MMCBUS_ACQUIRE_BUS(mmcbus, dev);
		err = mmc_send_ext_csd(mmcbus, dev, sc->ext_csd);
		MMCBUS_RELEASE_BUS(mmcbus, dev);
		if (err != MMC_ERR_NONE) {
			device_printf(dev, "Error reading EXT_CSD %s\n",
			    mmcsd_errmsg(err));
			return (ENXIO);
		}
	}
	ext_csd = sc->ext_csd;

	if ((quirks & MMC_QUIRK_INAND_CMD38) != 0) {
		if (mmc_get_spec_vers(dev) < 4) {
			device_printf(dev,
			    "MMC_QUIRK_INAND_CMD38 set but no EXT_CSD\n");
			return (EINVAL);
		}
		sc->flags |= MMCSD_INAND_CMD38;
	}

	/*
	 * EXT_CSD_SEC_FEATURE_SUPPORT_GB_CL_EN denotes support for both
	 * insecure and secure TRIM.
	 */
	if ((ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT] &
	    EXT_CSD_SEC_FEATURE_SUPPORT_GB_CL_EN) != 0 &&
	    (quirks & MMC_QUIRK_BROKEN_TRIM) == 0) {
#ifdef DPALCFG_DRIVERS_MMC_DBG
		if (bootverbose)
			device_printf(dev, "taking advantage of TRIM\n");
#endif
		sc->flags |= MMCSD_USE_TRIM;
		sc->erase_sector = 1;
	} else
		sc->erase_sector = mmc_get_erase_sector(dev);

	if (mmc_get_cmd23_cap(dev) &&
	    (quirks & MMC_QUIRK_BROKEN_CMD23) == 0)
		sc->flags |= MMCSD_USE_CMD23;

	/*
	 * Determine partition switch timeout (provided in units of 10 ms)
	 * and ensure it's at least 300 ms as some eMMC chips lie.
	 */
	sc->part_time = DPAL_MAX(ext_csd[EXT_CSD_PART_SWITCH_TO] * 10 * 1000,
	    300 * 1000);

	sector_size = mmc_get_sector_size(dev);
	ro = mmc_get_read_only(dev);
	err = mmcsd_add_part(sc, EXT_CSD_PART_CONFIG_ACC_DEFAULT,
	    (uint64_t)mmc_get_media_size(dev) * sector_size, ro);
	if (err != MMC_ERR_NONE) {
		device_printf(dev, "Error add part!\n");
		return (ENODEV);
	}

	return (0);
}

static uintmax_t
mmcsd_pretty_size(uint64_t size, char *unit)
{
	uintmax_t bytes;
	int i;

	/*
	 * Display in most natural units.  There's no card < 1MB.  However,
	 * RPMB partitions occasionally are smaller than that, though.  The
	 * SD standard goes to 2 GiB due to its reliance on FAT, but the data
	 * format supports up to 4 GiB and some card makers push it up to this
	 * limit.  The SDHC standard only goes to 32 GiB due to FAT32, but the
	 * data format supports up to 2 TiB however.  2048 GB isn't too ugly,
	 * so we note it in passing here and don't add the code to print TB).
	 * Since these cards are sold in terms of MB and GB not MiB and GiB,
	 * report them like that.  We also round to the nearest unit, since
	 * many cards are a few percent short, even of the power of 10 size.
	 */
	bytes = size;
	unit[0] = unit[1] = '\0';
	for (i = 0; i <= 2 && bytes >= 1000; i++) {
		bytes = (bytes + 1000 / 2 - 1) / 1000;
		switch (i) {
		case 0:
			unit[0] = 'k';
			break;
		case 1:
			unit[0] = 'M';
			break;
		case 2:
			unit[0] = 'G';
			break;
		default:
			break;
		}
	}
	return (bytes);
}

static int
mmcsd_add_part(struct mmcsd_softc *sc, u_int type, uint64_t media_size, bool ro)
{
	int retval = 0;
	device_t dev = sc->dev;

	sc->part[type] = (struct mmcsd_part *)dpal_zalloc(sizeof(struct mmcsd_part));
	struct mmcsd_part *part = sc->part[type];
	if (part == NULL) {
		dev_loge(dev, "malloc memory for block fail!\n");
		return (-ENOMEM);
	}

	part->align = mmcbr_get_dma_alignment(device_get_parent(dev));
	if (part->align > 0) { /* We don't need allocate memory in case of slot type SDHCI */
		part->align -= 1;
		part->size = ALIGNED_BUFF_BLK_NUM * 512;
		part->buffer = dpal_memalign(CACHE_ALIGNED_SIZE, part->size);
		if (part->buffer == NULL) {
			dev_loge(dev, "allocate memory for buffer fail!\n");
			retval = -ENOMEM;
			goto free_part;
		}
	}

	part->sc = sc;
	part->type = type;
	part->ro = ro;
	part->card_capacity = mmc_get_media_size(dev);
	if (sc->high_cap == 0)
		part->card_capacity = mmc_get_media_size(dev) * MMC_SECTOR_SIZE;

	MMCSD_DISK_LOCK_INIT(part);
	retval = (int)dpal_queue_create(&part->bio_queue_id, MMC_BIO_QUEUE_LEN, sizeof(char *));
	if (retval != EOK) {
		mmc_err("%s,%d, queue create error:0x%x\n", __FUNCTION__,__LINE__, retval);
		goto free_buffer;
	}
	retval = (int)mmcsd_thread_create((uintptr_t)part, &part->pthread_id);
	if (retval != EOK) {
		mmc_err("mmcsd_thread_create error:0x%x\n", retval);
		goto delete_queue;
	}

	part->bio_queue_state = BIO_QUEUE_NORMAL;
	part->atom_reqs = ATOMIC_INIT(0);
	part->part_status = PART_NORMAL;

	uint32_t card_mode = (uint32_t)mmc_get_card_type(sc->dev);
	if (card_mode == mode_sd || card_mode == mode_mmc) {
		retval = mmcsd_part_ops_register(part, card_mode);
		if (retval != ENOERR) {
			if (mmcsd_exit(part) != 0)
				device_printf(dev, "mmcsd exit failed!\n");
			goto delete_thread;
		}
	}
	char unit[2];
	device_t mmcbus = sc->mmcbus;
	uintmax_t bytes = mmcsd_pretty_size(media_size, unit);
	if (type == EXT_CSD_PART_CONFIG_ACC_DEFAULT) {
		uint32_t speed = (uint32_t)mmcbr_get_clock(mmcbus);
		(void)speed;
		(void)bytes;
#ifdef __FREERTOS__
		printf("%s: %ju%sB <%s>%s at %s %d.%01dMHz/%dbit/%d-block\n",
			part->name, bytes, unit, mmc_get_card_id_string(dev),
			ro ? " (read-only)" : "", device_get_nameunit(mmcbus),
			speed / 1000000, (speed / 100000) % 10,
			bus_width_value(dev), sc->max_data);
#else
		device_printf(dev, "%s: %ju%sB <%s>%s at %s %d.%01dMHz/%dbit/%d-block\n",
		    part->name, bytes, unit, mmc_get_card_id_string(dev),
		    ro ? " (read-only)" : "", device_get_nameunit(mmcbus),
		    speed / 1000000, (speed / 100000) % 10,
		    bus_width_value(dev), sc->max_data);
#endif

#ifdef DPALCFG_DRIVERS_MMC_DBG
		char *devInfo = dpal_zalloc(DEVICE_INFO_SIZE);
		snprintf_s(devInfo, DEVICE_INFO_SIZE, DEVICE_INFO_SIZE - 1, "%s: %ju%sB <%s>%s at %s %d.%01dMHz/%dbit/%d-block",
			part->name, bytes, unit, mmc_get_card_id_string(dev),
			ro ? " (read-only)" : "", device_get_nameunit(mmcbus),
			speed / 1000000, (speed / 100000) % 10,
			bus_width_value(dev), sc->max_data);
		boot_msg0("card-1: ");
		boot_msg0(devInfo);
		dpal_free((void*)devInfo);
		devInfo = NULL;
#endif
	}
    return (retval);

delete_thread:
	(void)mmcsd_thread_delete(part->pthread_id);

delete_queue:
	(void)dpal_queue_delete(part->bio_queue_id);

free_buffer:
	if (part->buffer != NULL) {
		dpal_free(part->buffer);
		part->buffer = NULL;
	}
free_part:
	if (part != NULL) {
		dpal_free(part);
		sc->part[type] = NULL;
	}
	return (retval);
}

static int
mmcsd_switch_part(device_t bus, device_t dev, uint16_t rca, u_int part)
{
	struct mmcsd_softc *sc;
	int err;
	uint8_t	value;

	sc = device_get_softc(dev);

	if (sc->mode == mode_sd)
		return (MMC_ERR_NONE);

	/*
	 * According to section "6.2.2 Command restrictions" of the eMMC
	 * specification v5.1, CMD19/CMD21 aren't allowed to be used with
	 * RPMB partitions.  So we pause re-tuning along with triggering
	 * it up-front to decrease the likelihood of re-tuning becoming
	 * necessary while accessing an RPMB partition.  Consequently, an
	 * RPMB partition should immediately be switched away from again
	 * after an access in order to allow for re-tuning to take place
	 * anew.
	 */
	if (part == EXT_CSD_PART_CONFIG_ACC_RPMB)
		MMCBUS_RETUNE_PAUSE(sc->mmcbus, sc->dev, true);

	if (sc->part_curr == part)
		return (MMC_ERR_NONE);

	value = (sc->ext_csd[EXT_CSD_PART_CONFIG] &
	    ~EXT_CSD_PART_CONFIG_ACC_MASK) | part;
	/* Jump! */
	err = mmc_switch(bus, dev, rca, EXT_CSD_CMD_SET_NORMAL,
	    EXT_CSD_PART_CONFIG, value, sc->part_time, true);
	if (err != MMC_ERR_NONE) {
		if (part == EXT_CSD_PART_CONFIG_ACC_RPMB)
			MMCBUS_RETUNE_UNPAUSE(sc->mmcbus, sc->dev);
		return (err);
	}

	sc->ext_csd[EXT_CSD_PART_CONFIG] = value;
	if (sc->part_curr == EXT_CSD_PART_CONFIG_ACC_RPMB)
		MMCBUS_RETUNE_UNPAUSE(sc->mmcbus, sc->dev);
	sc->part_curr = part;
	return (MMC_ERR_NONE);
}

static const char *
mmcsd_errmsg(int e)
{
	if (e < 0 || e >= MMC_ERR_MAX)
		return "Bad error code";
	return (errmsg[e]);
}

int wait_card_ready(device_t dev, device_t mmcbus, bool in_exc)
{
#define MAX_WAIT_TIME 6000
	struct mmc_command cmd;
	struct mmc_request req;
	uint64_t time_cost;
	memset(&req, 0, sizeof(req));
	memset(&cmd, 0, sizeof(cmd));
	req.cmd = &cmd;
	cmd.opcode = MMC_SEND_STATUS;
	cmd.arg = (uint32_t)mmc_get_rca(dev) << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	time_cost = dpal_gettime_mono(true) + MAX_WAIT_TIME;

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
	req.flags |= (!in_exc) ? 0 : MMC_REQ_IN_EXC;
#endif

	do {
		MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req);
		if ((req.cmd->error != MMC_ERR_NONE)  || (cmd.resp[0] & STATUS_REG_ERR_MASK)) {
			(void)mmcsd_errmsg(req.cmd->error);
			device_printf(dev, "wait_card_ready Issuing command 13 failed %s ,status = 0x%x\n",
			    mmcsd_errmsg(req.cmd->error), cmd.resp[0]);
			return (req.cmd->error);
		}
		if (dpal_gettime_mono(true) >= time_cost) {
			return (-ETIMEDOUT);
		}
	} while (!(cmd.resp[0] & R1_READY_FOR_DATA) || (R1_CURRENT_STATE(cmd.resp[0]) == R1_STATE_PRG));

	return (0);
}

int
sent_stop(device_t mmcbus, device_t dev, uint32_t write_flag, uint32_t *status)
{
    struct mmc_command cmd = {0};
    int err;

    cmd.opcode = MMC_STOP_TRANSMISSION;
    if (write_flag)
        cmd.flags = MMC_RSP_R1B | MMC_CMD_AC;
    else
        cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

    err = mmc_wait_for_cmd(mmcbus, dev, &cmd, 3); /* retry three times*/
    if (err) {
        return err;
    }

    *status = cmd.resp[0];

    /* No need to check card status in case of READ. */
    if (write_flag == 0)
        return 0;

    return wait_card_ready(dev, mmcbus, false);
}

static int
mmc_cmd_error_check(const struct mmc_command *cmd, uint32_t status, bool status_flag)
{
	if ((cmd->error == MMC_ERR_BADCRC) || (cmd->error == MMC_ERR_FIFO)) {
		mmc_err("%s,%d, response CRC or fifo error, cmd->error:0x%x\n", __FUNCTION__,__LINE__,cmd->error);
		return (MMC_ERR_NONE);
	} else if (cmd->error == MMC_ERR_TIMEOUT) {
		mmc_err("%s,%d, response timeout\n", __FUNCTION__,__LINE__);
		if (status_flag == false) {
			mmc_err("%s,%d, get card status failed, retry\n", __FUNCTION__,__LINE__);
			return (MMC_ERR_NONE);
		} else if (status & (R1_COM_CRC_ERROR | R1_ILLEGAL_COMMAND)) {
			mmc_err("%s,%d, get card status failed, status:0x%x\n", __FUNCTION__,__LINE__, status);
			return (MMC_ERR_NONE);
		}
	}
	return (MMC_ERR_FAILED);
}

static int
card_recovery(device_t dev, device_t mmcbus, const struct mmc_command *cmd, const struct mmc_command *stop)
{
	int err;
	uint32_t status;
	uint32_t stop_status = 0;
	bool status_flag = true;
	uint32_t write_flag = 0;
	int i = 0;

	if (mmc_get_card_present(dev) == MMC_CAP_CARD_ABSENT) {
		mmc_err("card is absent!\n");
		return (MMC_ERR_FAILED);
	}

	if (!slot_proc_get_card_attach(device_get_parent(mmcbus))) {
		mmc_err("card is not attached!\n");
		return (MMC_ERR_FAILED);
	}

	/* 3: try three times to get card status */
	for (i = 0; i < 3; i++) {
		err = mmc_send_status(mmcbus, dev, mmc_get_rca(dev), &status);
		if (err == 0)
			break;
		status_flag = false;
	}

	if (err) {
		mmc_err("get card status fail!\n");
		goto hw_reset;
	}

	if (status & R1_CARD_ECC_FAILED)
		mmc_err("cmd->resp[0] = 0x%x\n", cmd->resp[0]);

	if (!(cmd->data->flags & MMC_DATA_READ)) {
		if ((status & R1_ERROR)) {
			mmc_err("cmd->resp[0] = 0x%x\n", cmd->resp[0]);
		}
	}
	/*
	 * if card status at R1_STATE_DATA and R1_STATE_RCV status,
	 * try to sent stop cmd
	 */
	if (R1_CURRENT_STATE(status) == R1_STATE_DATA ||
	    R1_CURRENT_STATE(status) == R1_STATE_RCV) {
		if (!(cmd->data->flags & MMC_DATA_READ))
			write_flag = 1;
		err = sent_stop(mmcbus, dev, write_flag, &stop_status);
		if (err) {
			mmc_err("stop_status = 0x%x, err:0x%x\n", stop_status, err);
			goto hw_reset;
		}

		if (stop_status & R1_CARD_ECC_FAILED)
			mmc_err("card ecc fail\n");
	}

	if (cmd->error) {
		if (mmc_cmd_error_check(cmd, status, status_flag) == MMC_ERR_NONE)
			return (MMC_ERR_NONE);
	}

	if (stop->error) {
		if (mmc_cmd_error_check(stop, status, status_flag) == MMC_ERR_NONE)
			return (MMC_ERR_NONE);
	}

hw_reset:
	if (mmc_hw_reset(mmcbus, dev) == 0)
		return (MMC_ERR_NONE);

	return (MMC_ERR_FAILED);
}

unsigned long
mmcsd_rw(struct mmcsd_part *part, struct bio *bp)
{
#define MMCSD_RW_RETRY_TIMES    3
	int status;
	unsigned long block, end, remain_blocks;
	struct mmc_command cmd;
	struct mmc_command stop;
	struct mmc_request req;
	struct mmc_data data;
	struct mmcsd_softc *sc = part->sc;
	device_t dev = sc->dev;
	device_t mmcbus = sc->mmcbus;
	u_int numblocks, sz;
	unsigned char *vaddr;
	uintptr_t mem = (uintptr_t)bp->bio_data;
	uint32_t bc = sc->flags & MMCSD_USE_CMD23;
	uint32_t align = part->align;
	uint32_t max_blk = sc->max_data;
	uint32_t retries = 0;

	if (align & mem)
		max_blk = DPAL_MIN(sc->max_data, (part->size / bp->sectorsize));
	block = bp->sector_start;

#ifdef LOSCFG_MMCSD_SG_SUPPORT
	if (bp->sg_mode == true && bp->numsectors > max_blk) {
		device_printf(dev, "Truncation transfer is not supported in sg mode\n");
		return bp->sector_start;
	}
#endif

	sz = bp->sectorsize;
	end = bp->sector_start + bp->numsectors;
	while (block < end) {
		vaddr = bp->bio_data + (block - bp->sector_start) * sz;
		mem = (uintptr_t)vaddr;
		remain_blocks = end - block;
		numblocks = DPAL_MIN(remain_blocks, max_blk);
		memset(&req, 0, sizeof(req));
		memset(&cmd, 0, sizeof(cmd));
		memset(&stop, 0, sizeof(stop));

		req.cmd = &cmd;
		cmd.data = &data;
		if (bp->bio_cmd == BIO_READ) {
			if (numblocks > 1)
				cmd.opcode = MMC_READ_MULTIPLE_BLOCK;
			else
				cmd.opcode = MMC_READ_SINGLE_BLOCK;
		} else {
			if (numblocks > 1)
				cmd.opcode = MMC_WRITE_MULTIPLE_BLOCK;
			else
				cmd.opcode = MMC_WRITE_BLOCK;
		}
		cmd.arg = block;
		if (sc->high_cap == 0)
			cmd.arg <<= 9;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
		data.data = vaddr;

		data.len = numblocks * sz;
		if (bp->bio_cmd == BIO_READ) {
			data.flags = MMC_DATA_READ;
			if (align & mem)
				data.data = part->buffer;
		} else {
			data.flags = MMC_DATA_WRITE;
			if (align & mem) {
				status = memcpy_s(part->buffer, part->size, vaddr, data.len);
				if (status != EOK) {
					device_printf(dev, "Error indicated: copy mem %d\n", status);
					break;
				}
				data.data = part->buffer;
			}
		}
#ifdef LOSCFG_MMCSD_SG_SUPPORT
		if (bp->sg_mode == true) {
			data.flags |= MMC_DATA_SG;
			data.sg = bp->sgs;
			data.sg_count = bp->sg_count;
		}
#endif

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
		if (bp->in_exc == true) {
			req.flags |= MMC_REQ_IN_EXC;
		}
#endif

		if (numblocks > 1) {
			data.flags |= MMC_DATA_MULTI;
			if (bc) {
				stop.opcode = MMC_SET_BLOCK_COUNT;
				stop.arg = numblocks;
				stop.flags = MMC_RSP_R1 | MMC_CMD_AC;
				req.bc = &stop;
			} else {
				stop.opcode = MMC_STOP_TRANSMISSION;
				stop.flags = MMC_RSP_R1B | MMC_CMD_AC;
				req.stop = &stop;
			}
		}
		if (MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req) != 0)
			device_printf(dev, "wait for request error \n");
		if (cmd.error != MMC_ERR_NONE || stop.error != MMC_ERR_NONE) {
			if (ppsratecheck(&sc->log_time, &sc->log_count,
			    LOG_PPS)) {
				if (stop.error != MMC_ERR_NONE)
					device_printf(dev, "Error indicated: %d %s\n", stop.error,
					    mmcsd_errmsg(stop.error));
				else
					device_printf(dev, "Error indicated: %d %s\n", cmd.error,
					    mmcsd_errmsg(cmd.error));
			}
			if ((card_recovery(dev, mmcbus, &cmd, &stop) == MMC_ERR_NONE) && (retries < MMCSD_RW_RETRY_TIMES)) {
				retries++;
				device_printf(dev, "card_recovery success, retry time:%u\n", retries);
				continue;
			}
			break;
		} else if ((bp->bio_cmd == BIO_READ) && (align & mem)) {
			status = memcpy_s(vaddr, remain_blocks * sz, part->buffer, data.len);
			if (status != EOK) {
				device_printf(dev, "copy mem error %d\n", status);
				break;
			}
		}
		block += numblocks;
		if (bp->bio_cmd == BIO_WRITE) {
			if (wait_card_ready(dev, mmcbus, bp->in_exc)) {
				return bp->sector_start;
			}
		}
	}
	return (block);
}

static unsigned long
mmcsd_delete(struct mmcsd_part *part, struct bio *bp)
{
#define MAX_WAIT_TIME 6000
	unsigned long block, end, start, stop;
	struct mmc_command cmd;
	struct mmc_request req;
	struct mmcsd_softc *sc;
	device_t dev, mmcbus;
	u_int erase_sector;
	int err;
	bool use_trim;
	uint64_t time_cost;

	sc = part->sc;
	dev = sc->dev;
	mmcbus = sc->mmcbus;

	block = bp->sector_start;
	end = bp->sector_start + bp->numsectors;
	use_trim = sc->flags & MMCSD_USE_TRIM;
	dev_logt(dev, "block:%lu, end:%lu\n", block, end);
	if (mmc_get_erase_cap(dev) == 0) {
		dev_loge(dev,"the present card dont support erase!\n");
		return (block);
	}
	if (use_trim == true) {
		start = block;
		stop = end;
	} else {
		/* Coalesce with the remainder of the previous request. */
		if (block > part->eblock && block <= part->eend)
			block = part->eblock;
		if (end >= part->eblock && end < part->eend)
			end = part->eend;
		/* Safely round to the erase sector boundaries. */
		erase_sector = sc->erase_sector;
		if (erase_sector == 0) {
			dev_loge(dev,"erase sector size is zero, erase operation will return fail!\n");
			return (block);
		}
		start = block + erase_sector - 1;	 /* Round up. */
		start -= start % erase_sector;
		stop = end;				/* Round down. */
		stop -= end % erase_sector;
		/*
		 * We can't erase an area smaller than an erase sector, so
		 * store it for later.
		 */
		if (start >= stop) {
			part->eblock = block;
			part->eend = end;
			return (end);
		}
	}

	if ((sc->flags & MMCSD_INAND_CMD38) != 0) {
		err = mmc_switch(mmcbus, dev, sc->rca, EXT_CSD_CMD_SET_NORMAL,
		    EXT_CSD_INAND_CMD38, use_trim == true ?
		    EXT_CSD_INAND_CMD38_TRIM : EXT_CSD_INAND_CMD38_ERASE,
		    sc->cmd6_time, true);
		if (err != MMC_ERR_NONE) {
			device_printf(dev,
			    "Setting iNAND erase command failed %s\n",
			    mmcsd_errmsg(err));
			return (block);
		}
	}

	/*
	 * Pause re-tuning so it won't interfere with the order of erase
	 * commands.  Note that these latter don't use the data lines, so
	 * re-tuning shouldn't actually become necessary during erase.
	 */
	MMCBUS_RETUNE_PAUSE(mmcbus, dev, false);
	/* Set erase start position. */
	memset(&req, 0, sizeof(req));
	memset(&cmd, 0, sizeof(cmd));
	req.cmd = &cmd;
	if (mmc_get_card_type(dev) == mode_sd)
		cmd.opcode = SD_ERASE_WR_BLK_START;
	else
		cmd.opcode = MMC_ERASE_GROUP_START;
	cmd.arg = start;
	if (sc->high_cap == 0)
		cmd.arg <<= 9;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	if (MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req) != 0)
		device_printf(dev, "wait for request error \n");
	if (req.cmd->error != MMC_ERR_NONE) {
		device_printf(dev, "Setting erase start position failed %s\n",
		    mmcsd_errmsg(req.cmd->error));
		block = bp->sector_start;
		goto unpause;
	}
	/* Set erase stop position. */
	memset(&req, 0, sizeof(req));
	memset(&cmd, 0, sizeof(cmd));
	req.cmd = &cmd;
	if (mmc_get_card_type(dev) == mode_sd)
		cmd.opcode = SD_ERASE_WR_BLK_END;
	else
		cmd.opcode = MMC_ERASE_GROUP_END;
	cmd.arg = stop;
	if (sc->high_cap == 0)
		cmd.arg <<= 9;
	cmd.arg--;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	if (MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req) != 0)
		device_printf(dev, "wait for request error \n");
	if (req.cmd->error != MMC_ERR_NONE) {
		device_printf(dev, "Setting erase stop position failed %s\n",
		    mmcsd_errmsg(req.cmd->error));
		block = bp->sector_start;
		goto unpause;
	}
	/* Erase range. */
	memset(&req, 0, sizeof(req));
	memset(&cmd, 0, sizeof(cmd));
	req.cmd = &cmd;
	cmd.opcode = MMC_ERASE;
	cmd.arg = use_trim == true ? MMC_ERASE_TRIM : MMC_ERASE_ERASE;
	cmd.flags = MMC_RSP_R1B | MMC_CMD_AC;
	if (MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req) != 0)
		device_printf(dev, "wait for request error \n");
	if (req.cmd->error != MMC_ERR_NONE) {
		device_printf(dev, "mmcsd_delete Issuing erase command failed %s\n",
		    mmcsd_errmsg(req.cmd->error));
		block = bp->sector_start;
		goto unpause;
	}

	memset(&req, 0, sizeof(req));
	memset(&cmd, 0, sizeof(cmd));
	req.cmd = &cmd;
	cmd.opcode = MMC_SEND_STATUS;
	cmd.arg = (uint32_t)mmc_get_rca(dev) << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	time_cost = dpal_gettime_mono(true) + MAX_WAIT_TIME;
	do {
		if (MMCBUS_WAIT_FOR_REQUEST(mmcbus, dev, &req) != 0)
			device_printf(dev, "wait for request error \n");
		if (req.cmd->error != MMC_ERR_NONE || (cmd.resp[0] & STATUS_REG_ERR_MASK)) {
			device_printf(dev, "Issuing command 13 failed %s ,status = 0x%x\n",
			    mmcsd_errmsg(req.cmd->error), cmd.resp[0]);
			block = bp->sector_start;
			goto unpause;
		}
		if (dpal_gettime_mono(true) >= time_cost) {
			block = bp->sector_start;
			goto unpause;
		}
	} while (!(cmd.resp[0] & R1_READY_FOR_DATA) || (R1_CURRENT_STATE(cmd.resp[0]) == R1_STATE_PRG));

	if (use_trim == false) {
		/* Store one of the remaining parts for the next call. */
		if (bp->sector_start >= part->eblock || block == start) {
			part->eblock = stop;	/* Predict next forward. */
			part->eend = end;
		} else {
			part->eblock = block;	/* Predict next backward. */
			part->eend = start;
		}
	}
	dev_logt(dev, "part->eblock:%lu,  part->eend:%lu\n", part->eblock, part->eend);
	block = end;
unpause:
	MMCBUS_RETUNE_UNPAUSE(mmcbus, dev);
	return (block);
}


static void
mmcsd_task(void *arg)
{
	struct mmcsd_part *part;
	struct mmcsd_softc *sc;
	struct bio *bp;
	device_t dev, mmcbus;

	part = arg;
	sc = part->sc;
    (void)sc;

	while (1) {
		(void)dpal_queue_read(part->bio_queue_id, &bp, sizeof(char *), DPAL_WAIT_FOREVER);
		if (bp->bio_cmd != BIO_READ && part->ro) {
			bp->bio_error = EROFS;
			bp->bio_resid = (long)(bp->numsectors * bp->sectorsize);
			bp->bio_flags |= BIO_ERROR;
			biodone(bp);
			continue;
		}
		if (bp->bio_cmd == BIO_CMD0) {
			clear_req_queue(part);
			goto out;
		}
        (void)mmcsd_opr(part, bp);
        biodone(bp);
    }
out:
    /* tell parent we're done */
    dev_logd(part->sc->dev, "read or write thread has been canceled!\n");
    biodone(bp);
}

int
mmcsd_opr(struct mmcsd_part *part, struct bio *bp)
{
    unsigned long block, end;
    struct mmcsd_softc *sc;
    device_t dev, mmcbus;
    int err, sz;

    sc = part->sc;
    dev = sc->dev;
    mmcbus = sc->mmcbus;

    if (bp->bio_cmd != BIO_READ && part->ro) {
        bp->bio_error = EROFS;
        bp->bio_resid = (long)(bp->numsectors * bp->sectorsize);
        bp->bio_flags |= BIO_ERROR;
        bp->bio_flags |= BIO_DONE;
        return 0;
    }
    if (bp->bio_cmd == BIO_CMD0) {
        clear_req_queue(part);
        return 0;
    }

    MMCBUS_ACQUIRE_BUS(mmcbus, dev);
    block = bp->sector_start;
    sz = (int)bp->sectorsize;
    end = bp->sector_start + bp->numsectors;
    err = mmcsd_switch_part(mmcbus, dev, sc->rca, part->type);
    if (err != MMC_ERR_NONE) {
        device_printf(dev, "Partition switch error\n");
        goto release;
    }
    if (bp->bio_cmd == BIO_READ || bp->bio_cmd == BIO_WRITE) {
        /* Access to the remaining erase block obsoletes it. */
        if (block < part->eend && end > part->eblock)
            part->eblock = part->eend = 0;
        block = mmcsd_rw(part, bp);
    } else if (bp->bio_cmd == BIO_DELETE) {
        block = mmcsd_delete(part, bp);
    }
release:
    MMCBUS_RELEASE_BUS(mmcbus, dev);
    if (block < end) {
        bp->bio_error = EIO;
        bp->bio_resid = (end - block) * sz;
        bp->bio_flags |= BIO_ERROR;
    } else {
        bp->bio_resid = 0;
    }

    return bp->numsectors;
}
