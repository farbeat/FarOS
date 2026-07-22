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
#include "securec.h"
#include "inode/inode.h"
#include "mmc_type.h"
#include "mmc_adapt.h"
#include "mmcbrvar.h"
#include "mmcvar.h"
#include "sdio_reg.h"
#include "sdio_ops.h"
#include "sdio.h"
#include "sdio_func.h"

struct sdio_param {
	uint8_t fn;
	uint16_t blksz;
	uint32_t addr;
	bool wr;
	bool incaddr;
};

static int
sdio_io_rwv_extended(const struct sdio_softc *sc, const struct sdio_param *param, uint32_t blocks, struct mmc_sg *sg, uint32_t sg_nums)
{
#define SDIO_ADDR_MASK  (~0x1FFFF)

	device_t mmcbus = sc->mmcbus;
	device_t child = sc->dev;
	int err;
	struct mmc_data data;
	struct mmc_command cmd = {0};
	struct mmc_request req = {0};

	if ((param->addr & SDIO_ADDR_MASK) || blocks == 0) {
		dev_loge(child, "invalid sdio card addr.\n ");
		return (-EINVAL);
	}
	req.cmd = &cmd;
	cmd.data = &data;
	cmd.opcode = SD_IO_RW_EXTENDED;
	cmd.flags = MMC_RSP_R5 | MMC_CMD_ADTC;
	cmd.arg = SD_IO_RW_FUNC(param->fn) | SD_IO_RW_ADR(param->addr);
	if (param->wr)
		cmd.arg |= SD_IO_RW_WR;
	if (param->incaddr)
		cmd.arg |= SD_IO_RW_INCR;

	if ((blocks == 1) && (param->blksz <= MMC_SDIO_BLOCK_SIZE))
		cmd.arg |= (param->blksz == MMC_SDIO_BLOCK_SIZE) ? 0 : param->blksz;
	else
		cmd.arg |= SD_IOE_RW_BLK | blocks;

	data.blk_num = blocks;
	data.blk_size = param->blksz;
	data.len = blocks * param->blksz;

	data.sg = sg;
	data.sg_count = sg_nums;

	if (param->wr)
		data.flags = MMC_DATA_WRITE;
	else
		data.flags = MMC_DATA_READ;

	if (blocks > 1)
		data.flags |= MMC_DATA_MULTI;

	data.flags |= MMC_DATA_SDIO;
	data.flags |= MMC_DATA_SG;

	MMCBUS_ACQUIRE_BUS(mmcbus, child);
	err = MMCBUS_WAIT_FOR_REQUEST(mmcbus, child, &req);
	MMCBUS_RELEASE_BUS(mmcbus, child);
	if (err != 0)
		return (-EIO);
	if (cmd.error != MMC_ERR_NONE) {
		dev_loge(child, "Error indicated: %d.\n", cmd.error);
		return (cmd.error);
	}
	if (cmd.resp[0] & R5_RESP_CHECK)
		return (-EPERM);

	return (0);
}

static int
sdio_io_rw_extended(struct sdio_softc *sc, uint8_t fn, uint32_t addr, bool wr,
    bool incaddr, uint32_t b_count, uint16_t blksz, uint8_t *buffer)
{
	uintptr_t mem = (uintptr_t)buffer;
	device_t mmcbus = sc->mmcbus;
	device_t child = sc->dev;
	int err;
	struct mmc_data data;
	struct mmc_command cmd = {0};
	struct mmc_request req = {0};

	if ((addr & ~0x1FFFF) || b_count == 0) {
		dev_loge(child, "invalid sdio card addr.\n ");
		return (-EINVAL);
	}
	req.cmd = &cmd;
	cmd.data = &data;
	cmd.opcode = SD_IO_RW_EXTENDED;
	cmd.flags = MMC_RSP_R5 | MMC_CMD_ADTC;
	cmd.arg = SD_IO_RW_FUNC(fn) | SD_IO_RW_ADR(addr);
	if (wr)
		cmd.arg |= SD_IO_RW_WR;
	if (incaddr)
		cmd.arg |= SD_IO_RW_INCR;

	if ((b_count == 1) && (blksz <= MMC_SDIO_BLOCK_SIZE))
		cmd.arg |= (blksz == MMC_SDIO_BLOCK_SIZE) ? 0 : blksz;
	else
		cmd.arg |= SD_IOE_RW_BLK | b_count;

	data.blk_num = b_count;
	data.blk_size = blksz;
	data.len = b_count * blksz;

	if (sc->align & mem) {
		if (data.len > sc->buf_size) {
			dev_loge(child, "data len out of buffer size.\n ");
			return (-ENOMEM);
		}

		if (wr != 0) {
			err = memcpy_s(sc->buf, sc->buf_size, buffer, data.len);
			if (err != EOK)
				return (-ENOMEM);
		}
		data.data = sc->buf;
	} else
		data.data = buffer;

	if (wr)
		data.flags = MMC_DATA_WRITE;
	else
		data.flags = MMC_DATA_READ;

	if (b_count > 1)
		data.flags |= MMC_DATA_MULTI;

	data.flags |= MMC_DATA_SDIO;

	MMCBUS_ACQUIRE_BUS(mmcbus, child);
	err = MMCBUS_WAIT_FOR_REQUEST(mmcbus, child, &req);
	MMCBUS_RELEASE_BUS(mmcbus, child);
	if (err != 0)
		return (-EIO);
	if (cmd.error != MMC_ERR_NONE) {
		dev_loge(child, "Error indicated: %d.\n", cmd.error);
		return (cmd.error);
	}
	if (cmd.resp[0] & R5_RESP_CHECK)
		return (-EPERM);
	if ((sc->align & mem) && (wr == 0)) {
		err = memcpy_s(buffer, data.len, sc->buf, data.len);
		if (err != EOK)
			return (-ENOMEM);
	}
	return (0);
}

static int
sdio_set_bool_for_func(const struct sdio_softc *sc, uint32_t addr, uint8_t fn, int enable)
{
	int error;
	uint8_t val;
	bool enabled;

	error = sdio_read_direct(sc, 0, addr, &val);
	if (error != 0)
		return (error);
	enabled = (val & (1 << fn)) ? true : false;
	if (enabled == enable)
		return (0);

	if (enable != 0)
		val |= (1 << fn);
	else
		val &= ~(1 << fn);

	error = sdio_write_direct(sc, 0, addr, val);
	return (error);
}

struct sdio_func *
sdio_get_func(uint32_t func_num, uint32_t manf_id, uint32_t device_id)
{
	char name[30]; /* 30: enough space to store device name, sizeof("/dev/sdio") + part bits */
	struct sdio_func *func;
	struct sdio_softc *sc;
	FAR struct inode *node;
	uint32_t max_num = get_sdio_card_no_bits();
	struct inode_search_s desc;
	int error;

	for (uint32_t i = 0; i < max_num; i++) {
		if (mmc_isclr(get_sdio_card_no(), i))
			continue;

		memset(name, 0, sizeof(name));
		error = snprintf_s(name, sizeof(name), sizeof(name) - 1, "/dev/sdio%0u", i);
		if (error <= 0) {
			mmc_err("snprintf_s err=%d.\n", error);
			return (NULL);
		}

		SETUP_SEARCH(&desc, name, false);
		if (inode_find(&desc) < 0) {
			RELEASE_SEARCH(&desc);
			mmc_err("sdio drv not found!\n");
			return (NULL);
		}
		node = desc.node;
		sc = (struct sdio_softc *)(node->i_private);
		inode_release(node);
		RELEASE_SEARCH(&desc);

		if (sc == NULL) {
			mmc_err("sdio drv is invalid\n");
			return (NULL);
		}

		for (uint32_t j = 0; j < sc->sdio_func_num; j++) {
			func = sc->sdio_funcs[j];
			if (func != NULL && func->manufacturer_id == manf_id &&
			    func->device_id == device_id && func->func_num == func_num)
				return (func);
		}
	}

	mmc_err("no sdio found!\n");
	return (NULL);
}

static bool
sdio_func_valid_check(const struct sdio_func *func)
{
	if (func == NULL || func->func_num > MAX_SDIO_FUN_NUM ||
	    func->sc == NULL || func->sc->mmcbus == NULL || func->sc->dev == NULL)
		return false;
	return true;
}

int
sdio_en_func(const struct sdio_func *func)
{
	if (sdio_func_valid_check(func) == false)
		return (-EINVAL);
	dev_logd(func->sc->dev, "try to enable func:%u\n", func->func_num);
	return (sdio_set_bool_for_func(func->sc, SD_IO_CCCR_FN_ENABLE,
	    (uint8_t)func->func_num, true));
}

int
sdio_dis_func(const struct sdio_func *func)
{
	if (sdio_func_valid_check(func) == false)
		return (-EINVAL);
	return (sdio_set_bool_for_func(func->sc, SD_IO_CCCR_FN_ENABLE,
	    (uint8_t)func->func_num, false));
}

int
sdio_set_cur_blk_size(struct sdio_func *func, uint32_t blk_size)
{
	int error;
	uint32_t addr;
	uint8_t val;

	if (sdio_func_valid_check(func) == false)
		return (-EINVAL);

	if (blk_size > MMC_SDIO_BLOCK_SIZE)
		return (-EINVAL);

	if (blk_size == 0)
        blk_size = DPAL_MIN(func->max_blk_size, MMC_SDIO_BLOCK_SIZE);

	addr = FUNC_ADDR_BASE(func->func_num) + SD_IO_FBR_IOBLKSZ;
	val = (uint8_t)(blk_size & 0xff);
	error = sdio_write_direct(func->sc, 0, addr, val);
	if (error != 0)
		return (error);

	val = (uint8_t)((blk_size >> 8) & 0xff);
	error = sdio_write_direct(func->sc, 0, addr + 1, val);
	if (error != 0)
		return (error);

	func->cur_blk_size = (uint16_t)blk_size;
	return (0);
}

static inline int
sdio_rwv_multi_blocks(const struct sdio_func *func, struct sdio_param *param,
    struct mmc_sg *sg, uint32_t sg_nums)
{
	uint32_t i;
	uint32_t blocks;
	int error = 0;
	uint32_t size = 0;
	uint32_t max_blks_once;

	if (sdio_func_valid_check(func) == false || sg == NULL || func->cur_blk_size == 0)
        return (-EINVAL);

    max_blks_once = DPAL_MIN(mmcbr_get_max_data(func->sc->mmcbus), MAX_SDIO_BLOCK_NUM);
    for (i = 0; i < sg_nums; i++) {
        if (sg[i].data == NULL) {
            return (-EINVAL);
        }
        /* For segmented data, ensure that the each segment is cache aligned. */
        if ((uintptr_t)sg[i].data & (CACHE_ALIGNED_SIZE - 1)) {
            dev_loge(func->sc->dev, "segment data address mem is not cache(%u byte) aligned\n", CACHE_ALIGNED_SIZE);
            return (-EINVAL);
        }
        size += sg[i].length;
    }

    blocks = (size / func->cur_blk_size) ? : 1;
    param->blksz = DPAL_MIN(size, func->cur_blk_size);
    param->fn = func->func_num;

	if (blocks > max_blks_once) {
		dev_loge(func->sc->dev, "unsupport too much blocks %u max %u\n",blocks, max_blks_once);
		return -EINVAL;
	}

	error = sdio_io_rwv_extended(func->sc, param, blocks, sg, sg_nums);

	return (error);
}

int
sdio_readv_incr_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums)
{
	struct sdio_param param = {
		.fn = 0,
		.blksz = 0,
		.addr = addr,
		.wr = 0,
		.incaddr = 1,
	};

	return (sdio_rwv_multi_blocks(func, &param, sg, sg_nums));
}

int
sdio_writev_incr_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums)
{
	struct sdio_param param = {
		.fn = 0,
		.blksz = 0,
		.addr = addr,
		.wr = 1,
		.incaddr = 1,
	};

	return (sdio_rwv_multi_blocks(func, &param, sg, sg_nums));
}

int
sdio_readv_fifo_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums)
{
	struct sdio_param param = {
		.fn = 0,
		.blksz = 0,
		.addr = addr,
		.wr = 0,
		.incaddr = 0,
	};

	return (sdio_rwv_multi_blocks(func, &param, sg, sg_nums));
}

int
sdio_writev_fifo_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums)
{
	struct sdio_param param = {
		.fn = 0,
		.blksz = 0,
		.addr = addr,
		.wr = 1,
		.incaddr = 0,
	};

	return (sdio_rwv_multi_blocks(func, &param, sg, sg_nums));
}

int
sdio_rw_multi_blocks(const struct sdio_func *func, bool iswrite, uint32_t *addr, bool incaddr,
    uint8_t **buffer, uint32_t blk_num)
{
    uint32_t b_count;
    uint32_t len;
    uint32_t max_blks_once = DPAL_MIN(mmcbr_get_max_data(func->sc->mmcbus), MAX_SDIO_BLOCK_NUM);
    int error = 0;

    while (blk_num > 0) {
        b_count = DPAL_MIN(blk_num, max_blks_once);
        len = b_count * func->cur_blk_size;
        error = sdio_io_rw_extended(func->sc, (uint8_t)func->func_num, *addr, iswrite, incaddr,
            b_count, func->cur_blk_size, *buffer);
        if (error != 0)
            return (error);
        blk_num -= b_count;
        *buffer += len;
        if (incaddr)
            *addr += len;
    }
    return (error);
}

static inline int
sdio_rw_multi_bytes(const struct sdio_func *func, bool iswrite, uint32_t *addr, bool incaddr,
    uint8_t **buffer, uint32_t bytes_num)
{
    uint32_t len;
    uint32_t max_blk_size = DPAL_MIN(MMC_SDIO_BLOCK_SIZE, func->max_blk_size);
    int error = 0;

    while (bytes_num > 0) {
        len = DPAL_MIN(bytes_num, max_blk_size);
        error = sdio_io_rw_extended(func->sc, (uint8_t)func->func_num, *addr,
            iswrite, incaddr, 1, (uint16_t)len, *buffer);
        if (error != 0)
            return (error);
        bytes_num -= len;
        *buffer += len;
        if (incaddr)
            *addr += len;
    }
    return (error);
}

static int
sdio_rw_blocks(const struct sdio_func *func, bool iswrite, uint32_t addr, bool incaddr,
    uint8_t *buf, uint32_t buf_len)
{
	int error;
	uint32_t blk_num;

	if (sdio_func_valid_check(func) == false || buf == NULL || func->cur_blk_size == 0)
        return (-EINVAL);

	if (mmc_get_cccr_caps(func->sc->dev) & SDIO_CCCR_MULTI_BLK) {
		blk_num = buf_len / func->cur_blk_size;
		error = sdio_rw_multi_blocks(func, iswrite, &addr, incaddr, &buf, blk_num);
		if (error != 0) {
			dev_loge(func->sc->dev, "failed to rw:%d fun:%u, err:%d\n", iswrite, func->func_num, error);
			return (error);
		}
		buf_len %= func->cur_blk_size;
	}

	error = sdio_rw_multi_bytes(func, iswrite, &addr, incaddr, &buf, buf_len);
	if (error != 0)
		dev_loge(func->sc->dev, "failed to rw:%d fun:%u, err:%d\n", iswrite, func->func_num, error);
	return (error);
}

int
sdio_read_incr_block(const struct sdio_func *func, void *dst, uint32_t addr, int size)
{
	return (sdio_rw_blocks(func, 0, addr, 1, dst, (uint32_t)size));
}

int
sdio_write_incr_block(const struct sdio_func *func, uint32_t addr, void *src, int size)
{
	return (sdio_rw_blocks(func, 1, addr, 1, src, (uint32_t)size));
}

int
sdio_read_fifo_block(const struct sdio_func *func, void *dst, uint32_t addr, int size)
{
	return (sdio_rw_blocks(func, 0, addr, 0, dst, (uint32_t)size));
}

int
sdio_write_fifo_block(const struct sdio_func *func, uint32_t addr, void *src, int size)
{
	return (sdio_rw_blocks(func, 1, addr, 0, src, (uint32_t)size));
}

#if !defined(__FREERTOS__)
uint8_t
sdio_read_byte_ext(const struct sdio_func *func, uint32_t addr, int *err, uint32_t in)
{
	uint8_t val;
	int error;

    (void) in;
	if (sdio_func_valid_check(func) == false)
        return (0xff);
    error = sdio_rw_direct_acqr_bus(func->sc->mmcbus, func->sc->dev, (uint8_t)func->func_num, addr, false, &val);
    if (err != NULL)
        *err = error;
    if (error != 0)
        return (0xff);
    return (val);
}
#endif

uint8_t
sdio_read_byte(const struct sdio_func *func, uint32_t addr, int *err)
{
    int error;
    uint8_t v;
	if (sdio_func_valid_check(func) == false)
        return (0xff);
    error = sdio_rw_direct_acqr_bus(func->sc->mmcbus, func->sc->dev, (uint8_t)func->func_num, addr, false, &v);
    if (error != 0) {
        if (err != NULL)
            *err = error;
        return (0xff);
    } else {
        if (err != NULL)
            *err = 0;
        return (v);
    }
}

void
sdio_write_byte(const struct sdio_func *func, uint8_t byte, uint32_t addr, int *err)
{
    int error;
	if (sdio_func_valid_check(func) == false)
        return;

	error = sdio_rw_direct_acqr_bus(func->sc->mmcbus, func->sc->dev, (uint8_t)func->func_num, addr, true, &byte);
	if (err != NULL)
		*err = error;
}

uint8_t
sdio_write_byte_raw(const struct sdio_func *func, uint8_t byte, uint32_t addr, int *err)
{
	int error;

	if (sdio_func_valid_check(func) == false)
        return (0xff);

	error = sdio_rw_direct_acqr_bus(func->sc->mmcbus, func->sc->dev, (uint8_t)func->func_num, addr, true, &byte);
	if (err != NULL)
		*err = error;
	if (error != 0)
		return (0xff);
	return (0);
}

uint8_t
sdio_func0_read_byte(const struct sdio_func *func, uint32_t addr, int *err)
{
	int error;
	uint8_t v;

	if (sdio_func_valid_check(func) == false)
        return (0xff);

	error = sdio_read_direct(func->sc, 0, addr, &v);
	if (error != 0) {
		if (err != NULL)
			*err = error;
		return (0xff);
	} else {
		if (err != NULL)
			*err = 0;
		return (v);
	}
}

void
sdio_func0_write_byte(const struct sdio_func *func, uint8_t byte, uint32_t addr, int *err)
{
	int error;

	if (sdio_func_valid_check(func) == false)
        return;
    error = sdio_write_direct(func->sc, 0, addr, byte);
    if (err != NULL)
        *err = error;
}

static void
sdio_process_irqs(const struct sdio_softc *sc)
{
	int fn;
	uint8_t val;
	int ret;
	struct sdio_func *func = NULL;

    func = sc->func;
	if (func != NULL && func->sdio_irq_handler != NULL) {
        func->sdio_irq_handler(func);
        return;
    }

	ret = sdio_rw_direct_acqr_bus(sc->mmcbus, sc->dev, 0, SD_IO_CCCR_INT_PENDING, false, &val);
	if (ret != 0) {
		mmc_err("err:%d, while SD_IO_CCCR_INT_PENDING\n", ret);
		return;
	}

	val >>= 1;
	/* fn 1-7 */
	fn = 1;
	while (val != 0) {
		if (val & 0x1) {
			func = sc->sdio_funcs[fn - 1];
			if (func != NULL && func->sdio_irq_handler != NULL)
				func->sdio_irq_handler(func);
			else
				mmc_err("no function or no handler exists.\n");
		}
		val >>= 1;
		fn++;
	}

	return;
}

static void
sdio_irq_wake(void *arg)
{
    struct sdio_softc *sc = (struct sdio_softc *)arg;
    (void)dpal_event_write(&sc->sdio_event, 1);
}

static int
sdio_irq_thread(void *arg)
{
    struct sdio_softc *sc = (struct sdio_softc*)arg;
    device_t mmcbus = sc->mmcbus;
    const uint32_t mode = DPAL_WAITMODE_OR + DPAL_WAITMODE_CLR;

    do {
        mmcbr_enable_sdio_irq(mmcbus);
        (void)dpal_event_read(&sc->sdio_event, 1, mode, DPAL_WAIT_FOREVER);
        sdio_process_irqs(sc);
    } while (1);

	return (0);
}

#define	SDIO_THREAD_STACK_SIZE	0x2000
#define	SDIO_THTEAD_PRIORITY	0X2

static uint32_t
sdio_thread_create(uintptr_t arg, uint32_t *id)
{
    dpal_task_attr_t task_attr;
    (void)memset_s(&task_attr, sizeof(task_attr), 0, sizeof(task_attr));
    task_attr.task_prio = SDIO_THTEAD_PRIORITY;
    task_attr.stack_size = SDIO_THREAD_STACK_SIZE;
    task_attr.task_name = "sdio_Task";
    task_attr.reserved = NULL; /* boot: need set reserved volue to DPAL_TASK_STATUS_DETACHED */
    return dpal_task_create(id, &task_attr, (dpal_tsk_entry_func)sdio_irq_thread, (void *)arg);
}

int
sdio_require_irq(struct sdio_func *func, sdio_irq_handler *handler)
{
	int ret;
	uint8_t val;
	struct sdio_softc *sc;
	device_t mmcbus;
	device_t child;

	if (sdio_func_valid_check(func) == false || handler == NULL)
        return (-EINVAL);
    if (func->sdio_irq_handler != NULL) {
        dprintf("func irq is busy\n");
        return (-EBUSY);
    }

	sc = func->sc;
	mmcbus = sc->mmcbus;
	child = sc->dev;

	ret = sdio_rw_direct_acqr_bus(mmcbus, child, 0, SD_IO_CCCR_INT_ENABLE, false, &val);
	if (ret != 0) {
		dev_loge(child, "failed to read CCCR_INT_ENABLE regs!\n");
		return (ret);
	}
	val |= ((1U << func->func_num) | 1);
	ret = sdio_rw_direct_acqr_bus(mmcbus, child, 0, SD_IO_CCCR_INT_ENABLE, true, &val);
	if (ret != 0) {
		dev_loge(child, "failed to write CCCR_INT_ENABLE regs!\n");
		return (ret);
	}
	func->sdio_irq_handler = handler;

	sc->sdio_irq_refs++;
	if (sc->sdio_irq_refs != 1)
		return (0);

    (void)dpal_event_init(&sc->sdio_event);
    ret = (int)sdio_thread_create((uintptr_t)sc, &sc->sdio_thread);
    if (ret != 0) {
        dev_loge(child, "failed to create sdio thread!\n");
        (void)dpal_event_destroy(&sc->sdio_event);
		sc->sdio_irq_refs--;
        func->sdio_irq_handler = NULL;
        return (ret);
    }
    ret = mmcbr_registor_sdio_irq(mmcbus, sdio_irq_wake, sc);
    if (ret != 0) {
        dev_loge(child, "failed to set irq!\n");
        (void)dpal_event_destroy(&sc->sdio_event);
        (void)dpal_task_delete(sc->sdio_thread);
		sc->sdio_irq_refs--;
        func->sdio_irq_handler = NULL;
        return (-EACCES);
    }

	for (uint32_t i = 0; i < sc->sdio_func_num; i++) {
		if ((sc->sdio_funcs[i] != NULL) && (sc->sdio_funcs[i]->sdio_irq_handler != NULL)) {
			sc->func = sc->sdio_funcs[i];
			break;
		}
	}
	return (0);
}

int
sdio_release_irq(struct sdio_func *func)
{
	uint32_t ret;
	uint32_t i;
	uint8_t val;
	struct sdio_softc *sc;
	device_t mmcbus;
	device_t child;

	if (sdio_func_valid_check(func) == false)
        return (-EINVAL);

	sc = func->sc;
	mmcbus = sc->mmcbus;
	child = sc->dev;

	if (func->sdio_irq_handler == NULL)
		goto clr_irq;

	func->sdio_irq_handler = NULL;
	sc->sdio_irq_refs--;
	if (sc->sdio_irq_refs == 0) {
		ret = (uint32_t)mmcbr_registor_sdio_irq(mmcbus, NULL, NULL);
        ret |= dpal_task_delete(sc->sdio_thread);
        ret |= dpal_event_destroy(&sc->sdio_event);
		if (ret != 0)
			dprintf("failed to delete sdio thread or destroy event, err:%u\n", ret);
	} else if (sc->sdio_irq_refs == 1) { /* May be called twice */
		i = 0;
		while (i < sc->sdio_func_num) {
			if (sc->sdio_funcs[i] != NULL && sc->sdio_funcs[i]->sdio_irq_handler != NULL) {
				sc->func = sc->sdio_funcs[i];
				break;
			}
			i++;
		}
	}

clr_irq:
	ret = (uint32_t)sdio_rw_direct_acqr_bus(mmcbus, child, 0, SD_IO_CCCR_INT_ENABLE, false, &val);
	if (ret == 0) {
		val &= ~(1U << func->func_num);
		if ((val & CCCR_INT_FUNC_EN_MASK) == 0)
			val &= ~CCCR_INT_EN_MASTER;
		ret = (uint32_t)sdio_rw_direct_acqr_bus(mmcbus, child, 0, SD_IO_CCCR_INT_ENABLE, true, &val);
	}

	return (int)(ret);
}
