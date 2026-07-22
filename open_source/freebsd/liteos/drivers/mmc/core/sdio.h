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

#ifndef DRIVERS_MMCSDIO_H
#define	DRIVERS_MMCSDIO_H

#include "mmc_type.h"
#include "mmc_private.h"
#include "sdio_ops.h"

#define	SDIO_MAX_FUNC	7
#define	SDIO_DRV_NODE_NAME	17

struct sdio_softc {
	device_t dev;
	device_t mmcbus;
	uint32_t sdio_func_num;
	struct sdio_func *sdio_funcs[SDIO_MAX_FUNC];
	struct sdio_func *func;
	int sdio_irq_refs;
	uint32_t sdio_thread;
    dpal_event_t sdio_event;
	bool sdio_event_enabled;
	uint32_t align;
	uint32_t buf_size;
	uint8_t *buf;
	uint32_t drv_no;
	char drv_node_name[SDIO_DRV_NODE_NAME];
	u_int max_data; /* Maximum data size [blocks] */
	uint16_t rca;
};
struct sdio_func *sdio_get_func(uint32_t func_num, uint32_t manufacturer_id, uint32_t device_id);
int sdio_set_card_bus_width(const struct mmc_softc *sc, enum mmc_bus_width bw);
int sdio_io_send_op_cond(const struct mmc_softc *sc, uint32_t ocr, uint32_t *rocr);
int mmc_discover_sdio(const struct mmc_softc *sc, struct mmc_ivars *ivar);
int sdio_switch_hispeed(const struct mmc_softc *sc, bool enable);
int sdio_io_card_reset(const struct mmc_softc *sc);
int sdio_disable_cd(const struct mmc_softc *sc, const struct mmc_ivars *ivar);
void sdio_dettach(device_t dev);
int sdio_attach(device_t dev);

uint32_t *get_sdio_card_no(void);
uint32_t get_sdio_card_no_bits(void);

static inline int
sdio_read_direct(const struct sdio_softc *sc, uint8_t fn, uint32_t addr, uint8_t *val)
{
	return (sdio_rw_direct_acqr_bus(sc->mmcbus, sc->dev, fn, addr, 0, val));
}

static inline int
sdio_write_direct(const struct sdio_softc *sc, uint8_t fn, uint32_t addr, uint8_t val)
{
	return (sdio_rw_direct_acqr_bus(sc->mmcbus, sc->dev, fn, addr, 1, &val));
}
#endif /* DRIVERS_MMCSDIO_H */
