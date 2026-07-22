/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2017 Ilya Bakulin
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#ifndef DEV_MMC_SDIO_FUNC_H
#define DEV_MMC_SDIO_FUNC_H

#include "sdio_reg.h"

struct sdio_func;
typedef void (sdio_irq_handler)(struct sdio_func *);

struct sdio_func {
	struct sdio_softc *sc;
	union {
		uint32_t data;
		struct {
			uint32_t present : 1;
			uint32_t reserved : 31;
		} bits;
	} state;

	uint32_t max_blk_size;
	uint16_t cur_blk_size;
	uint32_t func_num;
	uint8_t  func_class;
	uint16_t manufacturer_id;
	uint16_t device_id;
	uint32_t en_timeout_ms;
	sdio_irq_handler *sdio_irq_handler;
	void *data;
};

struct mmc_sg {
	size_t length;
	void *data;
};

int sdio_reset_comm(const struct sdio_func *func);

/* Funcions declaration to caller, SDIO I/O operations */
struct sdio_func *sdio_get_func(uint32_t func_num, uint32_t manufacturer_id, uint32_t device_id);
int sdio_en_func(const struct sdio_func *func);
int sdio_dis_func(const struct sdio_func *func);
int sdio_set_cur_blk_size(struct sdio_func *func, uint32_t blksz);
int sdio_require_irq(struct sdio_func *func, sdio_irq_handler *handler);
int sdio_release_irq(struct sdio_func *func);
/*
 * The sdio_readv/writev functions caller must ensure rule:
 * 1.sg[i]->data buffer address must be cacheline align.
 * 2.sg[i]->data alloced size must be cacheline align.
 * 3.sg[i]->data buffer only can be used by sg transfer.
 */
int sdio_readv_incr_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums);
int sdio_writev_incr_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums);
int sdio_readv_fifo_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums);
int sdio_writev_fifo_block(const struct sdio_func *func, uint32_t addr, struct mmc_sg *sg, uint32_t sg_nums);

uint8_t sdio_read_byte(const struct sdio_func *func, uint32_t addr, int *err_ret);
void sdio_write_byte(const struct sdio_func *func, uint8_t byte, uint32_t addr, int *err_ret);
int sdio_read_incr_block(const struct sdio_func *func, void *dst, uint32_t addr, int count);
int sdio_write_incr_block(const struct sdio_func *func, uint32_t addr, void *src, int count);
int sdio_read_fifo_block(const struct sdio_func *func, void *dst, uint32_t addr, int count);
int sdio_write_fifo_block(const struct sdio_func *func, uint32_t addr, void *src, int count);

#if !defined(__FREERTOS__)
uint8_t sdio_read_byte_ext(const struct sdio_func *func, uint32_t addr, int *err_ret, unsigned in);
#endif
uint8_t sdio_write_byte_raw(const struct sdio_func *func, uint8_t write_byte, uint32_t addr, int *err_ret);
uint8_t sdio_func0_read_byte(const struct sdio_func *func, uint32_t addr, int *err_ret);
void sdio_func0_write_byte(const struct sdio_func *func, unsigned char byte, uint32_t addr, int *err_ret);

#endif /* DEV_MMC_SDIO_FUNC_H */