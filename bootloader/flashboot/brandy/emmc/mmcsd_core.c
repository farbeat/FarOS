/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2011-07-25     weety         first version
 */

#include "mmcsd_core.h"
#include "hal_mmc_common.h"
#include "mmc.h"

#define DBG_TAG               "SDIO"
#ifdef RT_SDIO_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* RT_SDIO_DEBUG */

#ifndef RT_MMCSD_STACK_SIZE
#define RT_MMCSD_STACK_SIZE 1024
#endif
#ifndef RT_MMCSD_THREAD_PREORITY
#if (RT_THREAD_PRIORITY_MAX == 32)
#define RT_MMCSD_THREAD_PREORITY  0x16
#else
#define RT_MMCSD_THREAD_PREORITY  0x40
#endif
#endif
#define EMMC_PINMUX_NUM           10
#define DELAY_10MS                10
#define TIME_1000                 1000

const uint8_t __lowest_bit_bitmap[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 00 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 10 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 20 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 30 */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 40 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 50 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 60 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 70 */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 80 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* 90 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* A0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* B0 */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* C0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* D0 */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, /* E0 */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0  /* F0 */
};

/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
static int __rt_ffs(int value)
{
    if (value == 0) { return 0; }

    if (value & 0xff) {
        return __lowest_bit_bitmap[value & 0xff] + 1;
    }

    if (value & 0xff00) {
        return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 9; // 8 9
    }

    if (value & 0xff0000) {
        return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 17; // 16 17
    }

    return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 25; // 24 25
}

int32_t rt_mmcsd_blk_probe(struct rt_mmcsd_card *card)
{
    UNUSED(card);
    return 0;
}

void rt_mmcsd_blk_remove(struct rt_mmcsd_card *card)
{
    UNUSED(card);
}

void mmcsd_host_lock(struct rt_mmcsd_host *host)
{
    if (host->flags & MMCSD_NO_USE_MUTEX) {
        return;
    }
    non_os_enter_critical();
}

void mmcsd_host_unlock(struct rt_mmcsd_host *host)
{
    if (host->flags & MMCSD_NO_USE_MUTEX) {
        return;
    }
    non_os_exit_critical();
}

void mmcsd_req_complete(struct rt_mmcsd_host *host)
{
    UNUSED(host);
}

void mmcsd_send_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{
    do {
        req->cmd->retries--;
        req->cmd->err = 0;
        req->cmd->mrq = req;
        if (req->data) {
            req->cmd->data = req->data;
            req->data->err = 0;
            req->data->mrq = req;
            if (req->stop) {
                req->data->stop = req->stop;
                req->stop->err = 0;
                req->stop->mrq = req;
            }
        }
        host->ops->request(host, req);
    } while (req->cmd->err && (req->cmd->retries > 0));
}

int32_t mmcsd_send_cmd(struct rt_mmcsd_host *host, struct rt_mmcsd_cmd *cmd, int retries)
{
    struct rt_mmcsd_req req;

    (void)memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    (void)memset_s(cmd->resp, sizeof(cmd->resp), 0, sizeof(cmd->resp));
    cmd->retries = retries;

    req.cmd = cmd;
    cmd->data = RT_NULL;

    mmcsd_send_request(host, &req);
    return cmd->err;
}

int32_t mmcsd_go_idle(struct rt_mmcsd_host *host)
{
    int32_t err;
    struct rt_mmcsd_cmd cmd;

    if (!controller_is_spi(host)) {
        mmcsd_set_chip_select(host, MMCSD_CS_HIGH);
        uapi_tcxo_delay_ms(1);
    }

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = GO_IDLE_STATE;
    cmd.arg = 0;
    cmd.flags = RESP_SPI_R1 | RESP_NONE | CMD_BC;

    err = mmcsd_send_cmd(host, &cmd, 0);
    uapi_tcxo_delay_ms(1);
    if (!controller_is_spi(host)) {
        mmcsd_set_chip_select(host, MMCSD_CS_IGNORE);
        uapi_tcxo_delay_ms(1);
    }

    return err;
}

int32_t mmcsd_spi_read_ocr(struct rt_mmcsd_host *host, int32_t high_capacity, uint32_t *ocr)
{
    struct rt_mmcsd_cmd cmd;
    int32_t err;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SPI_READ_OCR;
    cmd.arg = high_capacity ? (1 << 30) : 0; // 30 bits
    cmd.flags = RESP_SPI_R3;

    err = mmcsd_send_cmd(host, &cmd, 0);
    *ocr = cmd.resp[1];
    return err;
}

int32_t mmcsd_all_get_cid(struct rt_mmcsd_host *host, uint32_t *cid)
{
    int32_t err;
    struct rt_mmcsd_cmd cmd;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = ALL_SEND_CID;
    cmd.arg = 0;
    cmd.flags = RESP_R2 | CMD_BCR;
    err = mmcsd_send_cmd(host, &cmd, 3); // 3 times
    if (err) {
        return err;
    }

    (void)memcpy_s(cid, sizeof(uint32_t) * 4, cmd.resp, sizeof(uint32_t) * 4); // 4 Bytes
    return 0;
}

int32_t mmcsd_get_cid(struct rt_mmcsd_host *host, uint32_t *cid)
{
    int32_t err, i;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    uint32_t *buf = RT_NULL;

    if (!controller_is_spi(host)) {
        if (!host->card) {
            return -RT_ERROR;
        }
        (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));

        cmd.cmd_code = SEND_CID;
        cmd.arg = host->card->rca << 16; // 16 Bits
        cmd.flags = RESP_R2 | CMD_AC;
        err = mmcsd_send_cmd(host, &cmd, 3); // 3 times
        if (err) {
            return err;
        }

        (void)memcpy_s(cid, sizeof(uint32_t) * 4, cmd.resp, sizeof(uint32_t) * 4); // 4 Bytes
        return 0;
    }

    buf = (uint32_t *)irmalloc(16); // 16
    if (!buf) {
        LOG_E("allocate memory failed!");
        return -RT_ENOMEM;
    }

    (void)memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    (void)memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;

    cmd.cmd_code = SEND_CID;
    cmd.arg = 0;

    /* NOTE HACK:  the RESP_SPI_R1 is always correct here, but we
     * rely on callers to never use this with "native" calls for reading
     * CSD or CID.  Native versions of those commands use the R2 type,
     * not R1 plus a data block.
     */
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;

    data.blksize = 16; // 16
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = buf;
    /*
     * The spec states that CSR and CID accesses have a timeout
     * of 64 clock cycles.
     */
    data.timeout_ns = 0;
    data.timeout_clks = 64; // 64

    mmcsd_send_request(host, &req);

    if (cmd.err || data.err) {
        irfree(buf);
        return -RT_ERROR;
    }

    for (i = 0;i < 4; i++) { // 4 times
        cid[i] = buf[i];
    }
    irfree(buf);

    return 0;
}

int32_t mmcsd_get_csd(struct rt_mmcsd_card *card, uint32_t *csd)
{
    int32_t err, i;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    uint32_t *buf = RT_NULL;

    if (!controller_is_spi(card->host)) {
        (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));

        cmd.cmd_code = SEND_CSD;
        cmd.arg = card->rca << 16; // 16 bits
        cmd.flags = RESP_R2 | CMD_AC;
        err = mmcsd_send_cmd(card->host, &cmd, 3); // 3 times
        if (err) {
            return err;
        }
        (void)memcpy_s(csd, sizeof(uint32_t) * 4, cmd.resp, sizeof(uint32_t) * 4); // 4 Bytes
        return 0;
    }

    buf = (uint32_t*)irmalloc(16); // 16 Bytes
    if (!buf) {
        LOG_E("allocate memory failed!");
        return -RT_ENOMEM;
    }

    (void)memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    (void)memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;

    cmd.cmd_code = SEND_CSD;
    cmd.arg = 0;

    /* NOTE HACK:  the RESP_SPI_R1 is always correct here, but we
     * rely on callers to never use this with "native" calls for reading
     * CSD or CID.  Native versions of those commands use the R2 type,
     * not R1 plus a data block.
     */
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;

    data.blksize = 16; // blksize 16
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = buf;

    /*
     * The spec states that CSR and CID accesses have a timeout
     * of 64 clock cycles.
     */
    data.timeout_ns = 0;
    data.timeout_clks = 64; // to clk 64

    mmcsd_send_request(card->host, &req);

    if (cmd.err || data.err) {
        irfree(buf);
        return -RT_ERROR;
    }

    for (i = 0;i < 4; i++) { // 4 times
        csd[i] = buf[i];
    }
    irfree(buf);
    return 0;
}

static int32_t _mmcsd_select_card(struct rt_mmcsd_host *host, struct rt_mmcsd_card *card)
{
    int32_t err;
    struct rt_mmcsd_cmd cmd;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SELECT_CARD;

    if (card) {
        cmd.arg = card->rca << 16; // 16 bits
        cmd.flags = RESP_R1 | CMD_AC;
    } else {
        cmd.arg = 0;
        cmd.flags = RESP_NONE | CMD_AC;
    }

    err = mmcsd_send_cmd(host, &cmd, 3); // 3 times
    if (err) {
        return err;
    }
    return 0;
}

int32_t mmcsd_select_card(struct rt_mmcsd_card *card)
{
    return _mmcsd_select_card(card->host, card);
}

int32_t mmcsd_deselect_cards(struct rt_mmcsd_card *card)
{
    return _mmcsd_select_card(card->host, RT_NULL);
}

int32_t mmcsd_spi_use_crc(struct rt_mmcsd_host *host, int32_t use_crc)
{
    struct rt_mmcsd_cmd cmd;
    int32_t err;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));

    cmd.cmd_code = SPI_CRC_ON_OFF;
    cmd.flags = RESP_SPI_R1;
    cmd.arg = use_crc;

    err = mmcsd_send_cmd(host, &cmd, 0);
    if (!err) {
        host->spi_use_crc = use_crc;
    }
    return err;
}

rt_inline void mmcsd_set_iocfg(struct rt_mmcsd_host *host)
{
    struct rt_mmcsd_io_cfg *io_cfg = &host->io_cfg;

    mmcsd_dbg("clock %uHz busmode %u powermode %u cs %u Vdd %u ""width %u \n",
        io_cfg->clock, io_cfg->bus_mode,
        io_cfg->power_mode, io_cfg->chip_select, io_cfg->vdd,
        io_cfg->bus_width);

    host->ops->set_iocfg(host, io_cfg);
}

/*
 * Control chip select pin on a host.
 */
void mmcsd_set_chip_select(struct rt_mmcsd_host *host, int32_t mode)
{
    host->io_cfg.chip_select = mode;
    mmcsd_set_iocfg(host);
}

/*
 * Sets the host clock to the highest possible frequency that
 * is below "hz".
 */
void mmcsd_set_clock(struct rt_mmcsd_host *host, uint32_t clk)
{
    if (clk < host->freq_min) {
        LOG_W("clock too low!");
    }

    host->io_cfg.clock = clk;
    mmcsd_set_iocfg(host);
}

/*
 * Change the bus mode (open drain/push-pull) of a host.
 */
void mmcsd_set_bus_mode(struct rt_mmcsd_host *host, uint32_t mode)
{
    host->io_cfg.bus_mode = mode;
    mmcsd_set_iocfg(host);
}

/*
 * Change data bus width of a host.
 */
void mmcsd_set_bus_width(struct rt_mmcsd_host *host, uint32_t width)
{
    host->io_cfg.bus_width = width;
    mmcsd_set_iocfg(host);
}

void mmcsd_set_data_timeout(struct rt_mmcsd_data       *data,
                            const struct rt_mmcsd_card *card)
{
    uint32_t mult;

    if (card->card_type == CARD_TYPE_SDIO) {
        data->timeout_ns = 1000000000;  /* SDIO card 1s, 1000000000ns */
        data->timeout_clks = 0;
        return;
    }

    // SD cards use a 100 multiplier rather than 10
    mult = (card->card_type == CARD_TYPE_SD) ? 100 : 10;

    /*
     * Scale up the multiplier (and therefore the timeout) by
     * the r2w factor for writes.
     */
    if (data->flags & DATA_DIR_WRITE) {
        mult <<= card->csd.r2w_factor;
    }

    data->timeout_ns = card->tacc_ns * mult;
    data->timeout_clks = card->tacc_clks * mult;

    /*
     * SD cards also have an upper limit on the timeout.
     */
    if (card->card_type == CARD_TYPE_SD) {
        uint32_t timeout_us, limit_us;

        timeout_us = data->timeout_ns / TIME_1000;
        timeout_us += data->timeout_clks * TIME_1000 /
            (card->host->io_cfg.clock / TIME_1000);

        if (data->flags & DATA_DIR_WRITE)
            /*
             * The limit is really 250 ms, but that is
             * insufficient for some crappy cards.
             */
            limit_us = 300000; // 300000
        else
            limit_us = 100000; // 100000

        /*
         * SDHC cards always use these fixed values.
         */
        if ((timeout_us > limit_us) || (card->flags & CARD_FLAG_SDHC)) {
            data->timeout_ns = limit_us * TIME_1000; /* SDHC card fixed 250ms */
            data->timeout_clks = 0;
        }
    }

    if (controller_is_spi(card->host)) {
        if (data->flags & DATA_DIR_WRITE) {
            if (data->timeout_ns < 1000000000) { // 1000000000
                data->timeout_ns = 1000000000;  /* 1s, 1000000000ns */
            }
        } else {
            if (data->timeout_ns < 100000000) { // 100000000
                data->timeout_ns =  100000000;  /* 100ms, 100000000ns */
            }
        }
    }
}

/*
 * Mask off any voltages we don't support and select
 * the lowest voltage
 */
uint32_t mmcsd_select_voltage(struct rt_mmcsd_host *host, uint32_t ocr)
{
    int bit;
    extern int __rt_ffs(int value);

    ocr &= host->valid_ocr;

    bit = __rt_ffs(ocr);
    if (bit) {
        bit -= 1;
        ocr &= 3 << bit; // 3 bit
        host->io_cfg.vdd = bit;
        mmcsd_set_iocfg(host);
    } else {
        LOG_W("host doesn't support card's voltages!");
        ocr = 0;
    }

    return ocr;
}

static void mmcsd_power_up(struct rt_mmcsd_host *host)
{
    int bit = __rt_fls(host->valid_ocr) - 1;

    host->io_cfg.vdd = bit;
    if (controller_is_spi(host)) {
        host->io_cfg.chip_select = MMCSD_CS_HIGH;
        host->io_cfg.bus_mode = MMCSD_BUSMODE_PUSHPULL;
    } else {
        host->io_cfg.chip_select = MMCSD_CS_IGNORE;
        host->io_cfg.bus_mode = MMCSD_BUSMODE_OPENDRAIN;
    }
    host->io_cfg.power_mode = MMCSD_POWER_UP;
    host->io_cfg.bus_width = MMCSD_BUS_WIDTH_1;
    mmcsd_set_iocfg(host);

    /*
     * This delay should be sufficient to allow the power supply
     * to reach the minimum voltage.
     */
    uapi_tcxo_delay_ms(DELAY_10MS);

    host->io_cfg.clock = host->freq_min;
    host->io_cfg.power_mode = MMCSD_POWER_ON;
    mmcsd_set_iocfg(host);

    /*
     * This delay must be at least 74 clock sizes, or 1 ms, or the
     * time required to reach a stable voltage.
     */
    uapi_tcxo_delay_ms(DELAY_10MS);
}

static void mmcsd_power_off(struct rt_mmcsd_host *host)
{
    host->io_cfg.clock = 0;
    host->io_cfg.vdd = 0;
    if (!controller_is_spi(host)) {
        host->io_cfg.bus_mode = MMCSD_BUSMODE_OPENDRAIN;
        host->io_cfg.chip_select = MMCSD_CS_IGNORE;
    }
    host->io_cfg.power_mode = MMCSD_POWER_OFF;
    host->io_cfg.bus_width = MMCSD_BUS_WIDTH_1;
    mmcsd_set_iocfg(host);
}

struct rt_mmcsd_host *mmcsd_alloc_host(void)
{
    struct rt_mmcsd_host *host;

    host = irmalloc(sizeof(struct rt_mmcsd_host));
    if (!host) {
        PRINT("alloc host failed");
        return RT_NULL;
    }

    (void)memset_s(host, sizeof(struct rt_mmcsd_host), 0, sizeof(struct rt_mmcsd_host));

    host->max_seg_size = 65535; // seg size 65535
    host->max_dma_segs = 1;
    host->max_blk_size = 512; // blk_size 512
    host->max_blk_count = 4096; // count 4096

    hi_mmc_mutex_init(&host->bus_lock);
    hi_mmc_sem_init(&host->sem_ack);
    return host;
}

void mmcsd_free_host(struct rt_mmcsd_host *host)
{
    irfree(host);
}


// added by hisi for init, no hotpluge, no detect, just init.
int hi_mmc_detect(struct rt_mmcsd_host *host)
{
    uint32_t  ocr;
    int32_t  err;
    int n;

    if (host->card == RT_NULL) {
        mmcsd_host_lock(host);
        mmcsd_power_up(host);
        // config mmc pinmux
        for (n = 0; n < EMMC_PINMUX_NUM; n++) {
            writew(S_HGPIO + S_HGPIO6_OFFSET + n * 0x4, FUNC_EMMC);
        }
        mmcsd_go_idle(host);
        mmcsd_send_if_cond(host, host->valid_ocr);

        err = sdio_io_send_op_cond(host, 0, &ocr);
        if (!err) {
            if (init_sdio(host, ocr)) {
                mmcsd_power_off(host);
                mmcsd_host_unlock(host);
                return -1;
            }
            mmcsd_host_unlock(host);
            return 0;
        }

        /*
         * detect SD card
         */
        err = mmcsd_send_app_op_cond(host, 0, &ocr);
        if (!err) {
            if (init_sd(host, ocr)) {
                mmcsd_power_off(host);
                mmcsd_host_unlock(host);
                return -1;
            }
            mmcsd_host_unlock(host);
            return 0;
        }

        /*
         * detect mmc card
         */
        err = mmc_send_op_cond(host, 0, &ocr);
        if (!err) {
            if (init_mmc(host, ocr)) {
                mmcsd_power_off(host);
                mmcsd_host_unlock(host);
                return -1;
            }
            mmcsd_host_unlock(host);
            return 0;
        }

        mmcsd_host_unlock(host);
    } else {
        /* card removed */
        mmcsd_host_lock(host);
        if (host->card->sdio_function_num == 0) {
            irfree(host->card);
            host->card = RT_NULL;
        }
        mmcsd_host_unlock(host);
        return -1;
    }
    return -1;
}