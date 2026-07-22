/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: MMC DRIVER ADAPTER.
 * Author:
 * Create:  2020-04
 */

#include "mshc_reg.h"
#include "hal_mmc_common.h"
#include "mmcsd_core.h"
#include "mmc.h"
#include "oal_interface.h"
#include "chip_core_irq.h"
#include "debug_print.h"
#include "mmc_block.h"
#include "panic.h"
#include "pinctrl_porting.h"
#include "chip_io.h"
#include "drv_mmc_adapter.h"

#define ADMA2_16BIT_LEN_MAX_VAL                  65536UL
#define ADMA2_FLAG_VALID_BIT_AND_TRAN_DATA_STAT  0x21
#define STOP_CMD_RETRY_TIMES                     500
#define TRY_TIMES                                3
#define DCACHE_LINE_ALIGN                        32
#define MPU_DTCM_ADDR_BASE                       0x20000000
#define MPU_DTCM_LEN                             0x00040000
#define EMMC_PINMUX_NUM                          10
#define EMMC_PAD_PINMUX_NUM                      9
#define EMMC_BASE_ADDR                           0x52062000
#define EMMC_STAB_MAX_LEN                        1024

#define hi_mmc_judge_buf_in_dtcm(addr) ((MPU_DTCM_ADDR_BASE <= (uint32_t)(addr)) && \
                               ((uint32_t)(addr) <= MPU_DTCM_ADDR_BASE + MPU_DTCM_LEN))

struct hi_mmc_drv *g_mmc_ptr[MAX_MMC_NUM] = { 0 };
static uint32_t g_mmc_int_status = 0;
volatile int g_mmc_init_process = 1;
static mmc_err_info_type_t g_mmc_err_info = { 0 };
struct rt_mmcsd_host* g_mmc = NULL;
char g_adma_buf[128]__attribute__((__aligned__(64)))  = { 0 };

static void hi_mmc_request(struct rt_mmcsd_host *mmc, struct rt_mmcsd_req *req);
static void hi_mmc_set_iocfg(struct rt_mmcsd_host *mmc, struct rt_mmcsd_io_cfg *io_cfg);
static void hi_mmc_enable_sdio_irq(struct rt_mmcsd_host *mmc, rt_int32_t enable);
static void hi_send_cmd(struct hi_mmc_drv *mmc_drv, struct rt_mmcsd_cmd *cmd, const struct rt_mmcsd_data *data);
static void hi_mmc_set_power(struct rt_mmcsd_host *mmc, uint8_t mode);
static void hi_mmc_set_bus_mode(const struct rt_mmcsd_host *mmc, int mode);
static void hi_mmc_poll_handler_all(struct hi_mmc_drv *mmc_drv);

uintptr_t mmc_align(uintptr_t addr, uint32_t boundary)
{
    return (addr + boundary - 1) & ~((uintptr_t)(boundary - 1));
}

void hi_mmc_set_clock(struct rt_mmcsd_host *mmc, unsigned int clocknum)
{
    if (mmc == NULL) {
        return;
    }
    struct hi_mmc_drv *mmc_drv = mmc->private_data;

    if (clocknum >= mmc_drv->freq_max) {
        clocknum = mmc_drv->freq_max;
    }
    hal_mmc_set_clock(mmc_drv, clocknum);
    // when the clock freq greater 16mhz, for performance, don't wait the int status register.
    if (clocknum >= 16000000) { // 16000000 means clock freq 16MHz
        g_mmc_init_process = 0;
        LOG_E("MODIFY THE CLOKNUM=%d", clocknum);
    }
}

static void hi_mmc_set_tsf_irqs(struct hi_mmc_drv *mmc)
{
    unsigned int pio_irqs = MSHC_INT_DATA_AVAIL | MSHC_INT_SPACE_AVAIL;
    unsigned int dma_irqs = MSHC_INT_DMA_END | MSHC_INT_ADMA_ERROR;

    if ((mmc->flags & MMC_FLAG_REQ_USE_DMA) != 0) {
        mmc->irq_enable = (mmc->irq_enable & ~pio_irqs) | dma_irqs;
    } else {
        mmc->irq_enable = (mmc->irq_enable & ~dma_irqs) | pio_irqs;
    }
    hal_mmc_enable_plug_irq(mmc, mmc->irq_enable);
}

static void hi_mmc_adma_set_desc_line(const struct hi_mmc_drv *mmc_drv, const char *desc,
                                      unsigned long addr, uint32_t len, unsigned cmd)
{
    unsigned short* cmdlen = (unsigned short *)desc;

    cmdlen[0] = (unsigned short)(cmd);
    cmdlen[1] = (unsigned short)(len);

    if ((mmc_drv->flags & MMC_FLAG_USE_ADMA_64BIT) != 0) {
        unsigned long *dataddr = (unsigned long*)(desc + ADMA_EVERY_ITEM_LEN);
        dataddr[0] = (addr);
    } else {
        unsigned int *dataddr = (unsigned int*)(desc + ADMA_EVERY_ITEM_LEN);
        dataddr[0] = (addr);
    }
}

static void hi_mmc_adma_set_end(char *desc)
{
    unsigned short *d = (unsigned short *)desc;
    d[0] |= ADMA2_TRANS_END;
}

static int hi_mmc_adma_set_desc_table(struct hi_mmc_drv *mmc_drv, struct rt_mmcsd_data *data)
{
    char *adma_desc = NULL;
    unsigned long addr;
    uint32_t len;

    adma_desc = mmc_drv->adma_desc;
    (void)memset_s(mmc_drv->adma_desc, mmc_align(mmc_drv->adma_desc_sz, CACHE_ALIGNED_SIZE),
        0, mmc_align(mmc_drv->adma_desc_sz, CACHE_ALIGNED_SIZE));
    addr = (uintptr_t)data->buf;
    len = data->blks * data->blksize;
    LOG_I("DMA addr:%x, len:%d dma_malloc_len=%d", addr, len, mmc_align(mmc_drv->adma_desc_sz, CACHE_ALIGNED_SIZE));

    do {
        if (len > ADMA2_16BIT_LEN_MAX_VAL) {
            hi_mmc_adma_set_desc_line(mmc_drv, adma_desc, addr, ADMA2_16BIT_LEN_MAX_VAL,
                ADMA2_FLAG_VALID_BIT_AND_TRAN_DATA_STAT);
            len -= ADMA2_16BIT_LEN_MAX_VAL;
            addr += ADMA2_16BIT_LEN_MAX_VAL;
        } else {
            hi_mmc_adma_set_desc_line(mmc_drv, adma_desc, addr, len, ADMA2_FLAG_VALID_BIT_AND_TRAN_DATA_STAT);
            len = 0;
        }
        adma_desc += mmc_drv->adma_desc_line_sz;
    } while (len != 0);

    if ((unsigned int)(adma_desc - mmc_drv->adma_desc) > mmc_drv->adma_desc_sz) {
        LOG_E("check dma wrong! adma_desc:%d, desc:%d, sz:%d",
              adma_desc, mmc_drv->adma_desc, mmc_drv->adma_desc_sz);
        return -1;
    }
    /* Mark the last descriptor as the terminating descriptor */
    if (adma_desc != mmc_drv->adma_desc) {
        adma_desc -= mmc_drv->adma_desc_line_sz;
        hi_mmc_adma_set_end(adma_desc);
    }
    return 0;
}

struct rt_mmcsd_host* hi_mmc_get_host(uint32_t id)
{
    return g_mmc_ptr[id]->host;
}

static void hi_mmc_non_os_req(void)
{
    uint32_t times = NON_OS_WAIT_SUM_TIMES;
    while (g_mmc_int_status == 0) {
        uapi_tcxo_delay_ms(NON_OS_WAIT_INT_PER_MSEC);
        times--;
        if (times == 0) {
            LOG_I("non-os mode the req timeout failed");
            break;
        }
    }
    g_mmc_int_status = 0;
}

void hal_mmc_reset(struct hi_mmc_drv *mmc_drv)
{
    if (mmc_drv == NULL) {
        return;
    }
    hal_mmc_clr_reg(mmc_drv, MSHC_RESET_ALL);
}

void hi_mmc_set_power(struct rt_mmcsd_host *mmc, uint8_t mode)
{
    if ((mmc == NULL) || (mmc->private_data == NULL)) { return; }
    struct hi_mmc_drv *mmc_drv = mmc->private_data;
    unsigned int pwr = MSHC_POWER_330;

    LOG_D("host->pwr = 0x%x, vdd value =%d, version=%d", mmc_drv->pwr, mmc->valid_ocr, mmc_drv->version);

    if (mode == POWER_OFF) {
        mmc_writew(mmc_drv, 0, MSHC_NOR_SIGNAL_ENABLE);
        mmc_writew(mmc_drv, 0, MSHC_ERR_SIGNAL_ENABLE);
        hal_mmc_reset(mmc_drv);
    } else {
        if (mmc_drv->version >= MSHC_SPEC_300) {
            hal_mmc_set_preset_val(mmc_drv, false);
        }

        if (mmc_drv->pwr == MSHC_POWER_330) {
            return;
        }

        mmc_drv->pwr = pwr;

        mmc_writeb(mmc_drv, 0, MSHC_POWER_CONTROL);
        mmc_writeb(mmc_drv, (uint8_t)pwr, MSHC_POWER_CONTROL);
        pwr |= MSHC_POWER_ON;
        mmc_writeb(mmc_drv, (uint8_t)pwr, MSHC_POWER_CONTROL);
        uapi_tcxo_delay_ms(NON_OS_WAIT_INT_PER_MSEC);
    }
}

void hi_mmc_set_bus_mode(const struct rt_mmcsd_host *mmc, int mode)
{
    UNUSED(mmc);
    UNUSED(mode);
}

void hi_mmc_set_bus_width(struct rt_mmcsd_host *mmc, int width)
{
    if (mmc == NULL) {
        return;
    }
    unsigned char ctrl_val;
    struct hi_mmc_drv *mmc_drv = mmc->private_data;

    ctrl_val = mmc_readb(mmc_drv, MSHC_HOST_CONTROL);
    if (width == BUS_WIDTH_8) {
        ctrl_val &= ~MSHC_CTRL_4BITBUS;
        if (mmc_drv->version >= MSHC_SPEC_300) {
            ctrl_val |= MSHC_CTRL_8BITBUS;
        }
    } else {
        if (mmc_drv->version >= MSHC_SPEC_300) {
            ctrl_val &= ~MSHC_CTRL_8BITBUS;
        }
        if (width == BUS_WIDTH_4) {
            ctrl_val |= MSHC_CTRL_4BITBUS;
        } else {
            ctrl_val &= ~MSHC_CTRL_4BITBUS;
        }
    }
    mmc_writeb(mmc_drv, ctrl_val, MSHC_HOST_CONTROL);
}

static void hi_mmc_set_voltage(const struct rt_mmcsd_host *mmc, uint16_t vdd)
{
    UNUSED(mmc);
    UNUSED(vdd);
}

void hi_mmc_set_timing(struct rt_mmcsd_host *mmc, int timing)
{
    hal_mmc_set_timing(mmc->private_data, timing);
}

void hi_mmc_set_iocfg(struct rt_mmcsd_host *mmc, struct rt_mmcsd_io_cfg *io_cfg)
{
    LOG_D("hisi emmc powr_mode=%d clock=%d, bus_mode=%d, bus_width=%d, vdd=%d",
          io_cfg->power_mode, io_cfg->clock, io_cfg->bus_mode, io_cfg->bus_width, io_cfg->vdd);
    hi_mmc_set_power(mmc, io_cfg->power_mode);
    hi_mmc_set_clock(mmc, io_cfg->clock);
    hi_mmc_set_bus_mode(mmc, io_cfg->bus_mode);
    hi_mmc_set_bus_width(mmc, io_cfg->bus_width);
    hi_mmc_set_voltage(mmc, io_cfg->vdd);
}

void hi_mmc_enable_sdio_irq(struct rt_mmcsd_host *mmc, rt_int32_t enable)
{
    UNUSED(mmc);
    UNUSED(enable);
}

static void hi_mmc_prepare_data(struct hi_mmc_drv *mmc_drv)
{
    unsigned char ctrl;
    struct rt_mmcsd_data *data = mmc_drv->data;
    int retval = 0;

    if (data == NULL) {
        return;
    }
    LOG_D("data blocksz = %d, blocks = %d, max_request_size = %d",
          data->blksize, data->blks, mmc_drv->host->max_dma_segs);
    mmc_assert(((data->blksize * data->blks) < (mmc_drv->host->max_dma_segs)));
    mmc_assert(data->blksize <= mmc_drv->host->max_blk_size);
    mmc_assert(data->blks < MMC_HOST_MAX_BLKS);
    /* set timeout value , use default value. */
    mmc_writeb(mmc_drv, MSHC_DEFINE_TIMEOUT, MSHC_TIMEOUT_CONTROL);

    mmc_drv->data->bytes_xfered = 0;
    // just only consider the ADMA2, default is ADMA2
    ctrl = mmc_readb(mmc_drv, MSHC_HOST_CONTROL);
    ctrl &= ~MSHC_CTRL_DMA_MASK;
    if ((mmc_drv->flags & MMC_FLAG_REQ_USE_DMA) != 0) {
        if ((mmc_drv->flags & MMC_FLAG_USE_ADMA) != 0) {
            /* ADMA config */
            retval = hi_mmc_adma_set_desc_table(mmc_drv, data);
            if (retval != 0) {
                mmc_drv->flags &= ~MMC_FLAG_REQ_USE_DMA;
            } else {
                mmc_writel(mmc_drv, 0, MSHC_DMA_ADDRESS);
                hal_mmc_adma_config(mmc_drv);
            }
            ctrl |= ((mmc_drv->flags & MMC_FLAG_USE_ADMA_64BIT) != 0 ? MSHC_CTRL_ADMA64 : MSHC_CTRL_ADMA32);
        }
    }

    mmc_writeb(mmc_drv, ctrl, MSHC_HOST_CONTROL);
    hi_mmc_set_tsf_irqs(mmc_drv);
    mmc_writew(mmc_drv, MSHC_MAKE_BLKSZ(SDMA_BUF_BOUNDARY_512K, data->blksize), MSHC_BLOCK_SIZE);
    mmc_writew(mmc_drv, (unsigned short)data->blks, MSHC_BLOCK_COUNT);
}

static void hi_mmc_set_tsf_mode(struct hi_mmc_drv *mmc_drv, const struct rt_mmcsd_cmd *cmd)
{
    uint16_t mode;
    struct rt_mmcsd_data *data = mmc_drv->data;
    struct rt_mmcsd_card *card = mmc_drv->host->card;
    if (data == NULL) {
        mode = mmc_readw(mmc_drv, MSHC_TRANSFER_MODE);
        mmc_writew(mmc_drv, mode & ~(MSHC_TRNS_AUTO_CMD12 |
                    MSHC_TRNS_AUTO_CMD23), MSHC_TRANSFER_MODE);
        return;
    }
    mode = MSHC_TRNS_BLK_CNT_EN;
    if (cmd->cmd_code == WRITE_MULTIPLE_BLOCK ||
        cmd->cmd_code == READ_MULTIPLE_BLOCK || data->blks > 1) {
        mode |= MSHC_TRNS_MULTI;
        // delete sd, just consider emmc
        if ((card->card_type == CARD_TYPE_MMC) && (data->flags & DATA_CMD_AUTO_STOP)) {
            if ((is_mmc_cmd23_support(card)) == 0) {
                mode |= MSHC_TRNS_AUTO_CMD12;
            } else {
                mode |= MSHC_TRNS_AUTO_CMD23;
            }
        }
    }

    if ((data->flags & DATA_DIR_READ) != 0) {
        mode |= MSHC_TRNS_READ;
    }
    if ((mmc_drv->flags & MMC_FLAG_REQ_USE_DMA) != 0) {
        mode |= MSHC_TRNS_DMA;
    }
    mmc_writew(mmc_drv, mode, MSHC_TRANSFER_MODE);
}

static void hi_mmc_bottom_finish(struct hi_mmc_drv *mmc_drv)
{
    if (mmc_drv == NULL) {
        return;
    }
    struct rt_mmcsd_req *mrq = NULL;

    mmc_assert(mmc_drv);
    if (mmc_drv->req == NULL) {
        return;
    }
    mrq = mmc_drv->req;
    mmc_assert(mrq);

    if ((mmc_drv->flags & MMC_FLAG_DEVICE_DEAD) == 0 && ((mrq->cmd != NULL && mrq->cmd->err != 0) ||
        (mrq->data != NULL && mrq->data->err != 0))) {
        g_mmc_err_info.mmc_cmd_err = mrq->cmd->err;
        g_mmc_err_info.mmc_cmd_data_err = mrq->data->err;
        hal_mmc_clr_reg(mmc_drv, MSHC_RESET_CMD);
        hal_mmc_clr_reg(mmc_drv, MSHC_RESET_DATA);
        hi_mmc_err_recovery();
    }
    mmc_drv->req = NULL;
    mmc_drv->cmd = NULL;
    mmc_drv->data = NULL;

    if ((mmc_drv->flags & MMC_FLAG_NON_OS_HANDLE) != 0) {
        if ((mmc_drv->flags & MMC_FLAG_NON_INT_HANDLE) == 0) {
            g_mmc_int_status = 1;
        }
    }
}

static void hi_mmc_make_cmd_flags(const struct rt_mmcsd_cmd *cmd, unsigned int *flags)
{
    switch (resp_type(cmd)) {
        case RESP_NONE:
            break;
        case RESP_R1:
        case RESP_R5:
        case RESP_R6:
        case RESP_R7:
            *flags |= (MSHC_CMD_RESP_SHORT | MSHC_CMD_CRC | MSHC_CMD_INDEX);
            break;
        case RESP_R1B:
            *flags |= (MSHC_CMD_RESP_SHORT_BUSY | MSHC_CMD_CRC | MSHC_CMD_INDEX);
            break;
        case RESP_R2:
            *flags |= (MSHC_CMD_RESP_LONG | MSHC_CMD_CRC);
            break;
        case RESP_R3:
        case RESP_R4:
            *flags |= MSHC_CMD_RESP_SHORT;
            break;
        default:
            LOG_E("ERROR: %s, unknown cmd type %x", __func__, resp_type(cmd));
            return;
    }
}

static void hi_mmc_set_cmd_type(uint32_t code, unsigned int *flags)
{
    if ((code == STOP_TRANSMISSION) || (code == SD_IO_RW_DIRECT)) {
        *flags |= MSHC_CMD_ABORTCMD;
    }
}

void hi_send_cmd(struct hi_mmc_drv *mmc_drv,
                 struct rt_mmcsd_cmd *cmd, const struct rt_mmcsd_data *data)
{
    unsigned int flags = 0;
    unsigned int mask;
    unsigned long timeout_val = NON_OS_WAIT_SUM_TIMES; // the longest time 5s

    LOG_D(">> Exec CMD%3d, Arg:%#x", cmd->cmd_code, cmd->arg);

    mask = MSHC_CMD_INHIBIT;
    if ((data != NULL) || (resp_type(cmd) == RESP_R1B)) { mask |= MSHC_DATA_INHIBIT; }

    if (data != NULL && data->stop != NULL) { mask &= ~MSHC_DATA_INHIBIT; }
    /* wait host ready */
    while ((mmc_readl(mmc_drv, MSHC_PRESENT_STATE) & mask) != 0) {
        if (timeout_val == 0) {
            LOG_E("exec cmd %d timeout!", cmd->cmd_code);
            hi_mmc_dumpregs(mmc_drv);
            cmd->err = -EIO;
            hi_mmc_bottom_finish(mmc_drv);
            return;
        }
        timeout_val--;
        uapi_tcxo_delay_us((uint64_t)WAIT_RSP_PER_USEC);
    }

    mmc_drv->cmd = cmd;
    hi_mmc_prepare_data(mmc_drv);
    mmc_writel(mmc_drv, cmd->arg, MSHC_ARGUMENT);
    hi_mmc_set_tsf_mode(mmc_drv, cmd);
    hi_mmc_make_cmd_flags(cmd, &flags);
    /* CMD19 is special in that the Data Present Select should be set */
    if (data != NULL || cmd->cmd_code == SEND_TUNING_BLOCK ||
            cmd->cmd_code == SEND_TUNING_BLOCK_HS200) {
        flags |= MSHC_CMD_DATA;
    }
    hi_mmc_set_cmd_type(cmd->cmd_code, &flags);
    mmc_writew(mmc_drv, MSHC_MAKE_CMD(cmd->cmd_code, flags), MSHC_COMMAND);
#ifdef SDIO_SLAVE_TEST_SUIT
    uapi_tcxo_delay_us(100); // wait 100us for sdio loop test.
#endif
}

static bool hi_mmc_check_data_aligned(struct rt_mmcsd_req *req)
{
    uint32_t *buf = (uint32_t *)req->data->buf;

    if (req->data == NULL) {
        return true;
    }
    if ((uint32_t)buf % EMMC_DMA_ALIGN != 0) {
        req->data->err = -ENODMAALIGN;
        LOG_E("check data dma aligned err buf=%d!", (uint32_t)req->data->buf);
        return false;
    }
    if (hi_mmc_judge_buf_in_dtcm(req->data->buf)) {
        return true;
    }
    if (non_os_is_dcache_enabled() && ((uint32_t)buf % DCACHE_LINE_ALIGN != 0)) {
        req->data->err = ENOCACHEALIGN;
        LOG_E("check data cacheline aligned err buf=%d!", (uint32_t)req->data->buf);
        return false;
    }
    return true;
}

static void hi_mmc_refresh_dcache_by_addr_in_write(struct rt_mmcsd_req *req)
{
    UNUSED(req);
}

static void hi_mmc_refresh_dcache_by_addr_in_read(struct rt_mmcsd_req *req)
{
    UNUSED(req);
}

void hi_mmc_request(struct rt_mmcsd_host *mmc, struct rt_mmcsd_req *req)
{
    struct hi_mmc_drv *mmc_drv = NULL;
    int present;
    uint32_t timeout = MMC_CMD_DATA_REQ_TIMEOUT;
    uint32_t ret = 0;

    if (mmc == NULL || req == NULL) {
        panic(0, __LINE__);
        return ;  //lint !e527  unreachable code
    }
    mmc_drv = mmc->private_data;
    if (mmc_drv == NULL) {
        panic(0, __LINE__);
        return ;  //lint !e527  unreachable code
    }
    hal_mmc_set_card_clk(mmc_drv, true);

    mmc_drv->req = req;
    mmc_drv->data = req->data;
    present = SDHCI_PLUG_STATE(mmc_drv);
    if (!hi_mmc_check_data_aligned(req)) { return; }
    hi_mmc_refresh_dcache_by_addr_in_write(req);
    if (present == 0 && (mmc_drv->flags & MMC_FLAG_NO_DETECT_INSERT) == 0) {
        req->cmd->err = -EIO;
        LOG_E("card is not present!");
        panic(0, __LINE__);
        return ;  //lint !e527 unreachable code
    } else {
        hi_send_cmd(mmc_drv, req->cmd, req->data);
    }

    if (req->data == NULL) { timeout = MMC_CMD_REQ_TIMEOUT; }
    g_mmc_err_info.mmc_cmd_code = (uint8_t)req->cmd->cmd_code;

    if ((mmc_drv->flags & MMC_FLAG_NON_INT_HANDLE) != 0) {
        hi_mmc_poll_handler_all(mmc_drv);
    } else {
        hi_mmc_non_os_req();
    }

    hi_mmc_refresh_dcache_by_addr_in_read(req);
    hal_mmc_set_card_clk(mmc_drv, false);
    UNUSED(ret);
}

static rt_int32_t hi_mmc_get_card_status(struct rt_mmcsd_host *mmc)
{
    UNUSED(mmc);
    return 0;
}

static const struct rt_mmcsd_host_ops g_hi_ops = {
    .request            = hi_mmc_request,
    .set_iocfg          = hi_mmc_set_iocfg,
    .get_card_status    = hi_mmc_get_card_status,
    .enable_sdio_irq    = hi_mmc_enable_sdio_irq,
};

static void hi_mmc_get_resp_r2(struct hi_mmc_drv *mmc_drv, struct rt_mmcsd_cmd *cmd)
{
    int i;
    for (i = 0; i < 4; i++) { // R2 len 32*4 bytes
        cmd->resp[i] = mmc_readl(mmc_drv, (uint32_t)(MSHC_RESPONSE + (3 - i) * 4)) << 8; // 3 4 8
        if (i != 3) { // 3
            // exclude bit135~bit128 with 3, 4, 24 and 0xff to get useful resp
            cmd->resp[i] |= (mmc_readl(mmc_drv, (uint32_t)(MSHC_RESPONSE + (3 - i) * 4 - 4)) >> 24) & 0xFF;
        }
        LOG_I("Resp %#x %#x %#x %#x", cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]); // Resp 0 1 2 3
    }
}

/*
 * Interrupt handling
 */
static void hi_mmc_finish_cmd(struct hi_mmc_drv *mmc_drv)
{
    struct rt_mmcsd_cmd *cmd = mmc_drv->cmd;
    int32_t ret;
    mmc_assert(mmc_drv->cmd);
    if (cmd == NULL) {
        return;
    }
    if ((cmd->flags & RESP_MASK) != 0) {
        if (resp_type(cmd) == RESP_R2) {
            hi_mmc_get_resp_r2(mmc_drv, cmd);
        } else {
            mmc_drv->cmd->resp[0] = mmc_readl(mmc_drv, MSHC_RESPONSE);
            LOG_I("Resp = %#x", mmc_drv->cmd->resp[0]);
        }
    }
    ret = memcpy_s(g_mmc_err_info.mmc_cmd_resp, sizeof(g_mmc_err_info.mmc_cmd_resp),
                   mmc_drv->cmd->resp, sizeof(g_mmc_err_info.mmc_cmd_resp));
    if (ret != 0) { return; }
    if (mmc_drv->data == NULL || cmd->cmd_code == STOP_TRANSMISSION) {
        hi_mmc_bottom_finish(mmc_drv);
    }
}

static void hi_mmc_cmd_irq(struct hi_mmc_drv *mmc_drv, uint32_t intmask, const unsigned int  *mask)
{
    UNUSED(mask);

    mmc_assert(intmask);

    if (mmc_drv->cmd == NULL) { return; }
    if ((intmask & MSHC_ERR_INT_MASK) != 0) {
        mmc_drv->cmd->err = (int)(intmask & MSHC_ERR_INT_MASK);
    } else {
        mmc_drv->cmd->err = 0;
    }

    if (mmc_drv->data == NULL && mmc_drv->cmd->err != 0) {
        hi_mmc_bottom_finish(mmc_drv);
        return;
    }

    if (resp_type(mmc_drv->cmd) == RESP_R1B) {
        return;
    }
    if ((intmask & MSHC_INT_RESPONSE) != 0) {
        hi_mmc_finish_cmd(mmc_drv);
    }
}

void hi_mmc_set_interrup_status(bool en)
{
    struct rt_mmcsd_host *mmc = NULL;
    struct hi_mmc_drv *mmc_drv;
    mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL) {
        panic(0, __LINE__);
        return; //lint !e527  unreachable code
    }
    mmc_drv = mmc->private_data;

    bool cur_int_status = (mmc_drv->flags & MMC_FLAG_NON_INT_HANDLE) != 0 ? false : true;
    if (cur_int_status != en) {
        if (en) {
            mmc_drv->flags &= ~MMC_FLAG_NON_OS_HANDLE;
            mmc_drv->flags &= ~MMC_FLAG_NON_INT_HANDLE;
            int_enable_irq((int)mmc_drv->irq_num);
        } else {
            mmc_drv->flags |= MMC_FLAG_NON_OS_HANDLE;
            mmc_drv->flags |= MMC_FLAG_NON_INT_HANDLE;
            int_disable_irq((int)mmc_drv->irq_num);
        }
    }
}

static void hi_mmc_finish_data(struct hi_mmc_drv *mmc_drv)
{
    struct rt_mmcsd_data *data = NULL;

    mmc_assert(mmc_drv->data);

    data = mmc_drv->data;

    if (data->err == 0) {
        data->bytes_xfered = data->blks * data->blksize;
    } else {
        data->bytes_xfered = 0;
    }

    if (DATA_CMD_STOP_ENABLE(data) != 0) {
        if (data->err != 0) {
            hal_mmc_clr_reg(mmc_drv, MSHC_RESET_CMD);
            hal_mmc_clr_reg(mmc_drv, MSHC_RESET_DATA);
        }
        hi_send_cmd(mmc_drv, data->stop, NULL);
    } else {
        hi_mmc_bottom_finish(mmc_drv);
    }
}

static void hi_mmc_get_dat_irq_err(uint32_t intmask, int *err)
{
    if ((intmask & MSHC_ERR_INT_MASK) != 0) {
        *err = (int)(intmask & MSHC_ERR_INT_MASK);
    } else {
        *err = 0;
    }
}

static void hi_mmc_data_irq(struct hi_mmc_drv *mmc_drv, uint32_t intmask)
{
    unsigned int command;

    mmc_assert(intmask);

    if (mmc_drv->data == NULL || (mmc_drv->cmd != NULL && mmc_drv->cmd->cmd_code == STOP_TRANSMISSION)) {
        if (mmc_drv->cmd != NULL && (mmc_drv->cmd->flags & RESP_R1B) != 0) {
            if ((intmask & MSHC_INT_DATA_TIMEOUT) != 0) {
                mmc_drv->cmd->err = -ETIMEDOUT;
                hi_mmc_bottom_finish(mmc_drv);
                return;
            }
            if ((intmask & MSHC_INT_DATA_END) != 0) {
                hi_mmc_finish_cmd(mmc_drv);
                return;
            }
        }
    }
    if (mmc_drv->data == NULL) {
        return;
    }
    hi_mmc_get_dat_irq_err(intmask, &(mmc_drv->data->err));
    if ((mmc_drv->data->err) != 0) {
        command = MSHC_GET_CMD(mmc_readw(mmc_drv, MSHC_COMMAND));
        if (command != SEND_TUNING_BLOCK && command != SEND_TUNING_BLOCK_HS200) {
            LOG_E("err = 0x%x, cmd = %d, interrupt = 0x%x", mmc_drv->data->err, command, intmask);
            hi_mmc_dumpregs(mmc_drv);
        }
        hi_mmc_finish_data(mmc_drv);
    } else {
        if ((intmask & MSHC_INT_DATA_END) != 0) {
            hi_mmc_finish_data(mmc_drv);
        }
    }
}

void hi_mmc_poll_handler_all(struct hi_mmc_drv *mmc_drv)
{
    uint16_t int_nor_mask = 0;
    uint16_t int_err_mask = 0;
    unsigned int intmask;
    uint32_t times = NON_OS_WAIT_SUM_TIMES;

    while ((mmc_readl(mmc_drv, MSHC_PRESENT_STATE) & MSHC_CMD_INHIBIT) != 0) {
        if (times == 0) {
            LOG_D("exec cmd %d timeout!", mmc_drv->cmd->cmd_code);
            hi_mmc_dumpregs(mmc_drv);
            mmc_drv->cmd->err = -EIO;
            g_mmc_err_info.mmc_present_stat = mmc_readl(mmc_drv, MSHC_PRESENT_STATE);
            g_mmc_err_info.mmc_cmd_err = mmc_drv->cmd->err;
            return;
        }
        times--;
        uapi_tcxo_delay_us((uint64_t)WAIT_RSP_PER_USEC);
    }

    // in initial, clock freq is 400k, need more time to wait int status.
    if (g_mmc_init_process == 1) { uapi_tcxo_delay_ms(1ULL); }

    times = NON_OS_WAIT_SUM_TIMES;
    while (times != 0) {
        int_nor_mask = mmc_readw(mmc_drv, MSHC_NOR_INT_STATUS);
        int_err_mask = mmc_readw(mmc_drv, MSHC_ERR_INT_STATUS);
        if (int_err_mask != 0) { g_mmc_err_info.mmc_err_int_stat = int_err_mask; }
        mmc_writew(mmc_drv, int_nor_mask, MSHC_NOR_INT_STATUS);
        mmc_writew(mmc_drv, int_err_mask, MSHC_ERR_INT_STATUS);
        intmask = (int_nor_mask | ((uint32_t)int_err_mask << MSHC_ERR_INT_OFFSET));

        LOG_D("poll status %#x", intmask);
        if ((intmask & MSHC_INT_CMD_MASK) != 0) {
            hi_mmc_cmd_irq(mmc_drv, (intmask & MSHC_INT_CMD_MASK), &intmask);
            if (mmc_drv->data == NULL) { break; }
        }

        if ((intmask & MSHC_INT_DATA_MASK) != 0) {
            hi_mmc_data_irq(mmc_drv, (intmask & MSHC_INT_DATA_MASK));
            LOG_I("data poll status %#x", intmask);
            break;
        }
        if ((intmask & MSHC_INT_BUS_POWER) != 0) { PRINT("host%d:card is consuming too much power!", mmc_drv->id); }
        intmask &= ~(CART_PLUG_STATE | MSHC_INT_CMD_MASK | MSHC_INT_DATA_MASK |
                     MSHC_INT_ERROR | MSHC_INT_BUS_POWER | MSHC_INT_CARD_INT);
        if (intmask != 0) { mmc_drv->err_int_count++; }

        uapi_tcxo_delay_us((uint64_t)WAIT_RSP_PER_USEC);
        times--;
    }
    if (times == 0) {
        hi_mmc_err_recovery();
        LOG_E("timeout, need check");
    }
}

static void hi_mmc_irq(struct hi_mmc_drv *mmc_drv)
{
    uint16_t int_nor_mask = 0;
    uint16_t int_err_mask = 0;
    unsigned int intmask;

    while ((int_nor_mask = mmc_readw(mmc_drv, MSHC_NOR_INT_STATUS)) != 0) {
        int_err_mask = mmc_readw(mmc_drv, MSHC_ERR_INT_STATUS);

        mmc_writew(mmc_drv, int_nor_mask, MSHC_NOR_INT_STATUS);
        mmc_writew(mmc_drv, int_err_mask, MSHC_ERR_INT_STATUS);

        intmask = int_nor_mask;
        intmask |= ((uint32_t)int_err_mask << MSHC_ERR_INT_OFFSET);
        LOG_I("OS IRQ %#x", intmask);
        if ((intmask & MSHC_INT_CMD_MASK) != 0) {
            hi_mmc_cmd_irq(mmc_drv, (intmask & MSHC_INT_CMD_MASK), &intmask);
        }

        if ((intmask & MSHC_INT_DATA_MASK) != 0) {
            hi_mmc_data_irq(mmc_drv, (intmask & MSHC_INT_DATA_MASK));
        }
        if ((intmask & MSHC_INT_BUS_POWER) != 0) {
            LOG_E("host%d:card is consuming too much power!", mmc_drv->id);
        }
        intmask &= ~(CART_PLUG_STATE | MSHC_INT_CMD_MASK | MSHC_INT_DATA_MASK |
                     MSHC_INT_ERROR | MSHC_INT_BUS_POWER | MSHC_INT_CARD_INT);
        if (intmask != 0) {
            mmc_drv->err_int_count++;
        }
    }
}

static uint32_t mmc_erase_param_align(struct rt_mmcsd_card *card, uint32_t *start, uint32_t *end, uint32_t nums)
{
    uint32_t from_new = *start;
    uint32_t nr_new = nums;
    uint32_t rem;
    LOG_I("erase form %#x to %#x, nr is %d, size is %d", *start, *end, nums, card->erase_size);
    rem = from_new % card->erase_size;
    if (rem != 0) {
        rem = card->erase_size - rem;
        from_new += rem;
        if (nr_new > rem) {
            nr_new -= rem;
        } else {
            return 0;
        }
    }

    rem = nr_new % card->erase_size;
    if (rem != 0) {
        nr_new -= rem;
    }
    if (nr_new == 0) {
        return 0;
    }
    *end = from_new + nr_new;
    *start = from_new;
    return nr_new;
}

static int hi_mmc_send_erase_cmd(struct rt_mmcsd_card *card, struct rt_mmcsd_cmd *cmd, uint32_t arg, uint32_t cmd_code)
{
    int err;
    (void)memset_s(cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    cmd->cmd_code = cmd_code;
    cmd->arg = arg;
    cmd->flags = RESP_R1 | CMD_AC;
    err = mmcsd_send_cmd(card->host, cmd, MMC_ERASE_CMD_RETRIES_TIMES);
    if (err != 0) {
        LOG_E("erase cmd_code=%d req failed ret=%d", cmd_code, err);
        err = -EIO;
    }
    return err;
}

static int hi_mmc_do_erase(struct rt_mmcsd_card *card, uint32_t start, uint32_t end, uint32_t type)
{
    struct rt_mmcsd_cmd cmd = { 0 };
    uint32_t busy_timeout = MMC_ERASE_MAX_TIME_MS;
    uint64_t timeout;
    int err;

    if (type == MMC_ERASE_TYPE_ERASE) {  // erase type is group erase
        if (end - start + 1 < card->erase_size) {
            err = EOPNOTSUPP;
            goto out;
        }
    }

    err = hi_mmc_send_erase_cmd(card, &cmd, start, ERASE_GROUP_START);
    if (err != 0) {
        goto out; /*lint !e801*/
    }
    err = hi_mmc_send_erase_cmd(card, &cmd, end, ERASE_GROUP_END);
    if (err != 0) {
        goto out; /*lint !e801*/
    }
    err = hi_mmc_send_erase_cmd(card, &cmd, type, ERASE);
    if (err != 0) {
        goto out; /*lint !e801*/
    }

    timeout = uapi_tcxo_get_ms() + busy_timeout;
    do {
        (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
        cmd.cmd_code = SEND_STATUS;
        cmd.arg = card->rca << MMC_RCA_OFFSET_VAL;
        cmd.flags = RESP_R1 | CMD_AC;
        err = mmcsd_send_cmd(card->host, &cmd, MMC_ERASE_CMD_RETRIES_TIMES);
        if (err != 0 || (cmd.resp[0] & R1_RSP_ERASE_STATE_ALL_ERR) != 0) {
            LOG_E("erase:  err = %d ,status = 0x%x", err, cmd.resp[0]);
            err = -EIO;
            goto out; /*lint !e801*/
        }
        if (uapi_tcxo_get_ms() >= timeout) {
            err = -EIO;
            goto out; /*lint !e801*/
        }
    } while ((cmd.resp[0] & R1_READY_FOR_DATA) == 0 || (R1_CURRENT_STATE(cmd.resp[0]) == R1_STATE_PRG));
    return 0;
out:
    LOG_E("do erase error ret=%d", err);
    return err;
}

int mmc_erase_arae(struct rt_mmcsd_card *card, const struct rt_mmcsd_req *req)
{
    uint32_t start = (uint32_t)req->erase->start_addr;
    uint32_t end = (uint32_t)(req->erase->start_addr + req->erase->len);
    uint32_t nums = (uint32_t)req->erase->len;
    uint32_t type = (uint32_t)req->erase->type;
    if ((card->csd.card_cmd_class & CCC_ERASE) == 0) {
        LOG_E("cmd_class=%x is not support CCC_ERASE", card->csd.card_cmd_class);
        return -EOPNOTSUPP;
    }
    if (card->erase_size == 0) {
        LOG_E("erase_size=%x error", card->erase_size);
        return -EOPNOTSUPP;
    }
    if (type == MMC_ERASE_TYPE_ERASE) {
        nums = mmc_erase_param_align(card, &start, &end, nums);
        if (nums == 0) {
            LOG_E("nums=%x 0 err", nums);
            return -EINVAL;
        }
    }
    if (end <= start) {
        LOG_E("end lower than start", end, start);
        return -EINVAL;
    }
    return hi_mmc_do_erase(card, start, end, type);
}

static void hi_mmc_irq_mmc0(void)
{
    hi_mmc_irq(g_mmc_ptr[SDIO_EMMC_ID]);
}

static void hi_mmc_irq_mmc1(void)
{
    hi_mmc_irq(g_mmc_ptr[SDIO_HOST_ID]);
}

static HWI_PROC_FUNC hi_mmc_get_int_func(uint32_t irq_num)
{
    if (irq_num == EMMC_IRQn) {
        return hi_mmc_irq_mmc0;
    } else {
        return hi_mmc_irq_mmc1;
    }
}

int hi_mmc_card_busy(struct rt_mmcsd_host *mmc)
{
    struct hi_mmc_drv *mmc_drv = (struct hi_mmc_drv *)mmc->private_data;
    uint32_t present_state;

    // Check whether DAT[0] is 0
    present_state = mmc_readl(mmc_drv, MSHC_PRESENT_STATE);

    return !(present_state & MSHC_DATA_0_LVL_MASK);
}

void hi_mmc_set_drv_bus_width_features(struct hi_mmc_drv *mmc_drv)
{
    if ((mmc_drv->host->flags & MMCSD_BUSWIDTH_1) != 0) {
        mmc_drv->bus_width = MMCSD_BUS_WIDTH_1;
    } else if ((mmc_drv->host->flags & MMCSD_BUSWIDTH_8) != 0) {
        mmc_drv->bus_width = MMCSD_BUS_WIDTH_8;
    } else {  // default bus width is 4bit
        mmc_drv->bus_width = MMCSD_BUS_WIDTH_4;
    }
}

void hi_mmc_set_drv_features(struct hi_mmc_drv *mmc_drv, uint8_t id)
{
    mmc_drv->id = id;
    g_mmc_ptr[id] = mmc_drv;
    switch (id) {
        case SDIO_EMMC_ID:
            mmc_drv->hw_base_addr  = SDIO_EMMC_BASE_ADDR;
            mmc_drv->irq_num = EMMC_IRQn;
            mmc_drv->type = MMC_TYPE_MMC;
            mmc_drv->freq_max = CONFIG_MMC0_CCLK_MAX;
            mmc_drv->freq_min = CONFIG_MMC0_CCLK_MIN;
            mmc_drv->flags |= MMC_FLAG_AUTO_CMD23;
            break;
#if (WITH_SDIO_HOST == YES)
        case SDIO_HOST_ID:
            mmc_drv->hw_base_addr  = SDIO_HOST_BASE_ADDR;
            mmc_drv->irq_num = SDIO_HOST_IRQn;
            mmc_drv->type = MMC_TYPE_SDIO;
            mmc_drv->freq_max = CONFIG_MMC1_CCLK_MAX;
            mmc_drv->freq_min = CONFIG_MMC1_CCLK_MIN;
            break;
#endif
        default:
            break;
    }

    mmc_drv->phy_driver_cap = mmc_drv->host->driver_cap;
    hi_mmc_set_drv_bus_width_features(mmc_drv);
    hal_mmc_deassert_status(mmc_drv);  // power on and dereset
    mmc_drv->version = (mmc_readw(mmc_drv, MSHC_HOST_VERSION) & MSHC_SPEC_VER_MASK);
    // in 1135, the eMMC and SDIO are unpluggable cards, chip is not supportted.
    mmc_drv->flags |= (MMC_FLAG_USE_ADMA | MMC_FLAG_REQ_USE_DMA | MMC_FLAG_NO_DETECT_INSERT);
    mmc_drv->adma_max_desc = MMC_ADMA_MAX_DESC;
    mmc_drv->adma_desc_line_sz = MMC_AMDA_32BIT_DESC_LINE_SIZE;
    mmc_drv->adma_desc_sz = mmc_drv->adma_max_desc * mmc_drv->adma_desc_line_sz;
    mmc_drv->adma_desc = (void *)g_adma_buf;

    if ((mmc_drv->host->flags & MMCSD_NON_OS_ENABLE) != 0) {
        mmc_drv->flags |= (MMC_FLAG_NON_OS_HANDLE | MMC_FLAG_NON_INT_HANDLE);
    }

    if (id == SDIO_EMMC_ID) {
        hal_mmc_set_emmc_ctrl(mmc_drv, (uint32_t)MSHC_EMMC_CTRL, (uint32_t)MSHC_CARD_IS_EMMC_START,
                              (uint32_t)MSHC_CARD_IS_EMMC_BITS, (uint32_t)MSHC_CARD_IS_EMMC);
        // set MBIU_CTRL_R 8burst
        uint32_t pvendor_spc_area = mmc_reg32_getbits(mmc_drv, SD_BUS_PVSA1, SD_BUS_PVSA1_POS, SD_BUS_PVSA1_LEN);
        // Offset: P_VENDOR_SPECIFIC_AREA[11:0] + 0x10
        mmc_reg32_setbits(mmc_drv, pvendor_spc_area + 0x10, SD_BUS_MBURST_POS, SD_BUS_MBURST_LEN, SD_BUS_MBURST_VAL);
    }
}

void hi_mmc_set_host_features(struct rt_mmcsd_host *mmc)
{
    struct hi_mmc_drv *mmc_drv = mmc->private_data;

    mmc_drv->irq_enable = MSHC_INT_BUS_POWER | MSHC_INT_DATA_END_ERR | MSHC_INT_DATA_CRC | MSHC_INT_DATA_TIMEOUT |
                          MSHC_INT_INDEX | MSHC_INT_END_BIT | MSHC_INT_CRC | MSHC_INT_TIMEOUT | MSHC_INT_DATA_END |
                          MSHC_INT_RESPONSE | MSHC_INT_AUTO_CMD_ERR;
    mmc_drv->pwr = 0;
    hal_mmc_enable_plug_irq(mmc_drv, mmc_drv->irq_enable);

    mmc->freq_max = mmc_drv->freq_max;
    mmc->freq_min = mmc_drv->freq_min;
// In the standard, the value is calculated based on the CAPABILITIES1_R field.
// in Hi1135 apply the fixed value.
    mmc->valid_ocr = VDD_29_30 | VDD_30_31 | VDD_32_33 | VDD_33_34 | VDD_165_195;
    mmc->max_blk_size = MAX_BLOCK_SIZE;
    mmc->max_blk_count = MAX_BLOCK_COUNT;
    mmc->max_seg_size = MAX_SEG_SIZE;
    mmc->max_dma_segs = mmc_drv->adma_max_desc * mmc->max_seg_size;
    if (mmc_drv->bus_width == MMCSD_BUS_WIDTH_4) {
        mmc->flags |= MMCSD_BUSWIDTH_4;
    } else if (mmc_drv->bus_width == MMCSD_BUS_WIDTH_8) {
        mmc->flags |= MMCSD_BUSWIDTH_8;
    } else {
        mmc->flags |= MMCSD_BUSWIDTH_1;
    }
    mmc->ops = &g_hi_ops;
}

int hi_mmc_set_int_info(struct rt_mmcsd_host *mmc, struct hi_mmc_drv *mmc_drv)
{
    HWI_PROC_FUNC irq_func;
    int ret;
    irq_func = (HWI_PROC_FUNC)hi_mmc_get_int_func(mmc_drv->irq_num);
    ret = (int)oal_int_create(mmc_drv->irq_num, irq_prio(mmc_drv->irq_num), (HWI_PROC_FUNC)irq_func, 0);
    if (ret != 0) {
        LOG_E("set irq for mmmc host err %d", ret);
        ret = -EACCES;
        return ret;
    }
    if ((mmc_drv->flags & MMC_FLAG_NON_INT_HANDLE) != 0) {
        ret = (int)oal_int_delete(mmc_drv->irq_num);
    }
    if (ret != 0) {
        LOG_E("unset irq for mmmc host err %d", ret);
        ret = -EACCES;
        return ret;
    }

    return ret;
}

#define PHASE_SCALE         16
#define NOT_FOUND          (-1)
#define MAX_TUNING_NUM      1
#define SAMP_PHASE_DEF      7

#define SAMP_PHASE_ENDPOINT 15 // Soc only support 16 phase

/* Tuning Block Pattern UHS-I */
static const uint8_t g_tuning_blk_4bit[] = {
    0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0xCC, 0xC3, 0xCC,
    0xC3, 0x3C, 0xCC, 0xFF, 0xFE, 0xFF, 0xFE, 0xEF,
    0xFF, 0xDF, 0xFF, 0xDD, 0xFF, 0xFB, 0xFF, 0xFB,
    0xBF, 0xFF, 0x7F, 0xFF, 0x77, 0xF7, 0xBD, 0xEF,
    0xFF, 0xF0, 0xFF, 0xF0, 0x0F, 0xFC, 0xCC, 0x3C,
    0xCC, 0x33, 0xCC, 0xCF, 0xFF, 0xEF, 0xFF, 0xEE,
    0xFF, 0xFD, 0xFF, 0xFD, 0xDF, 0xFF, 0xBF, 0xFF,
    0xBB, 0xFF, 0xF7, 0xFF, 0xF7, 0x7f, 0x7B, 0xDE,
};

/* Tuning block pattern for 8 bit mode for HS200 */
static const uint8_t g_tuning_blk_8bit[] = {
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC, 0xCC,
    0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF, 0xFF,
    0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE, 0xFF,
    0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD, 0xDD,
    0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF, 0xBB,
    0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF,
    0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC,
    0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF,
    0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE,
    0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD,
    0xDD, 0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF,
    0xBB, 0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF,
    0xFF, 0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE,
};

struct tuning_blk_info {
    uint8_t *blk;
    unsigned int blk_size;
};

static void hi_mmc_tune_out(struct hi_mmc_drv *mmc_drv, uint32_t start,
    uint32_t top, uint32_t bottom, uint32_t end)
{
    int phase;
    unsigned short val;

    if (top == (uint32_t)NOT_FOUND) {
        top = start;
    }

    if (bottom == (uint32_t)NOT_FOUND) {
        bottom = end;
    }
    if (bottom < top) {
        phase = (top + bottom) >> 1;
        phase = phase - (PHASE_SCALE >> 1);
        phase = (phase < 0) ? (PHASE_SCALE + phase) : phase;
    } else {
        phase = (uint32_t)((top + bottom) >> 1);
    }
    hal_mmc_set_sample_phase(mmc_drv, (uint32_t)phase);

    val = mmc_readw(mmc_drv, MSHC_HOST_CONTROL2);
    val |= MSHC_CTRL_TUNED_CLK;
    mmc_writew(mmc_drv, val, MSHC_HOST_CONTROL2);
    hal_mmc_enable_plug_irq(mmc_drv, mmc_drv->irq_enable);
    LOG_I("tuning  valid phase shift [%d, %d] Final Phase %d", top, bottom, phase);
}

static void mmc_tuning_config(struct rt_mmcsd_req *tuning_mrq, struct rt_mmcsd_cmd *tuning_cmd,
    struct rt_mmcsd_data *tuning_data, struct tuning_blk_info *tuning_info, struct rt_mmcsd_host *mmc)
{
    struct hi_mmc_drv *mmc_drv = mmc->private_data;

    if (mmc->io_cfg.bus_width == MMCSD_BUS_WIDTH_8) {
        tuning_info->blk = (uint8_t *)g_tuning_blk_8bit;
        tuning_info->blk_size = sizeof(g_tuning_blk_8bit);
    } else if (mmc->io_cfg.bus_width == MMCSD_BUS_WIDTH_4) {
        tuning_info->blk = (uint8_t *)g_tuning_blk_4bit;
        tuning_info->blk_size = sizeof(g_tuning_blk_4bit);
    } else {
        LOG_E("MMC tuning config error");
        return;
    }

    tuning_mrq->cmd = tuning_cmd;
    tuning_mrq->data = tuning_data;
    tuning_cmd->arg = 0;
    tuning_cmd->retries = 0;
    tuning_cmd->flags = RESP_R1 | CMD_ADTC;
    tuning_cmd->err = 0;
    tuning_data->err = 0;
    tuning_mrq->data->blksize = tuning_info->blk_size;
    tuning_mrq->data->blks = 1;
    tuning_mrq->data->flags = DATA_DIR_READ;

    mmc_drv->req = tuning_mrq;
    mmc_drv->cmd = tuning_cmd;
    mmc_drv->data = tuning_data;
}

static int mmc_send_tuning(struct rt_mmcsd_host *mmc, uint32_t opcode, int32_t *cmd_error)
{
    struct rt_mmcsd_req tuning_mrq = {0};
    struct rt_mmcsd_cmd tuning_cmd = {0};
    struct rt_mmcsd_data tuning_data = {0};
    struct tuning_blk_info tuning_info = {NULL, 0};
    struct hi_mmc_drv *mmc_drv = mmc->private_data;
    void *data_buf = NULL;
    int  err = 0;

    mmc_tuning_config(&tuning_mrq, &tuning_cmd, &tuning_data, &tuning_info, mmc);
    data_buf = (void *)irmalloc(mmc_align(tuning_info.blk_size, CACHE_ALIGNED_SIZE));
    if (data_buf == NULL) {
        return -ENOMEM;
    }
    (void)memset_s(data_buf, tuning_info.blk_size, 0, tuning_info.blk_size);
    tuning_cmd.cmd_code = opcode;
    tuning_mrq.data->buf = (rt_uint32_t *)data_buf;
    hal_mmc_set_card_clk(mmc_drv, true);

    hi_send_cmd(mmc_drv, &tuning_cmd, &tuning_data);

    if ((mmc_drv->flags & MMC_FLAG_NON_INT_HANDLE) != 0) {
        hi_mmc_poll_handler_all(mmc_drv);
    } else {
        hi_mmc_non_os_req();
    }

    hal_mmc_set_card_clk(mmc_drv, false);
    if (cmd_error != NULL) {
        *cmd_error = tuning_cmd.err;
        err = tuning_cmd.err;
        goto out; /*lint !e801*/
    }
    if (tuning_cmd.err != 0) {
        err = tuning_cmd.err;
        goto out; /*lint !e801*/
    }

    if (tuning_data.err != 0) {
        err = tuning_data.err;
        goto out; /*lint !e801*/
    }
    if (memcmp(tuning_info.blk, data_buf, tuning_info.blk_size) != 0) {
        err = -EIO;
    }
out:
    irfree(data_buf);
    return err;
}

int hi_mmc_exec_tuning(struct rt_mmcsd_host *mmc, uint32_t opcode)
{
    uint32_t hindex, val;
    uint32_t error = 0;
    uint32_t found = 0;
    uint32_t startPoint = 0;
    uint32_t endPoint = SAMP_PHASE_ENDPOINT;
    uint32_t prevError = (uint32_t)NOT_FOUND;
    uint32_t raisePoint = (uint32_t)NOT_FOUND;
    uint32_t fallPoint = (uint32_t)NOT_FOUND;
    struct hi_mmc_drv *drv = mmc->private_data;

    LOG_I("MMC_ExeTuning");

    val = mmc_readl(drv, MSHC_AUTO_TUNING_CTRL);
    val |= MSHC_SW_TUNING_EN;
    mmc_writel(drv, val, MSHC_AUTO_TUNING_CTRL);

    val = drv->irq_enable | MSHC_INT_DATA_AVAIL;
    hal_mmc_enable_plug_irq(drv, val);

    for (hindex = startPoint; hindex <= endPoint; hindex++) {
        hal_mmc_set_sample_phase(drv, hindex);
        if (mmc_send_tuning(drv->host, opcode, NULL) == 0) {
            found = 1;  /* found a valid phase */
        } else {
            error = 1;
        }

        if (hindex > startPoint) {
            if (error != 0 && prevError == 0) {
                fallPoint = hindex - 1;
            }
            if (error == 0 && prevError != 0) {
                raisePoint = hindex;
            }
        }

        if ((raisePoint != (uint32_t)NOT_FOUND) &&
                (fallPoint != (uint32_t)NOT_FOUND)) {
            break;
        }
        prevError = error;
        error = 0;
    }

    if (found == 0) {
        LOG_E("no valid phase shift! use default");
        hal_mmc_set_sample_phase(drv, (uint32_t)SAMP_PHASE_DEF);
    } else {
        LOG_I("Tuning finished!!");
        hi_mmc_tune_out(drv, startPoint, raisePoint, fallPoint, endPoint);
    }
    return 0;
}

void mmc_set_sdiom_clk_mode(int clk_type)
{
    hal_mmc_set_sdiom_clk_mode(g_mmc_ptr, clk_type);
}

void mmc_set_driver_capability(struct rt_mmcsd_host* mmc, uint32_t cap)
{
    mmc->driver_cap = cap;
}

bool mmc_detect_bus_error(struct rt_mmcsd_host* host)
{
    if ((host == NULL) || (host->private_data == NULL)) { return false; }
    struct hi_mmc_drv *mmc_drv = host->private_data;
    uint32_t cur_pstate = mmc_readl(mmc_drv, MSHC_PRESENT_STATE);
    if ((g_mmc_err_info.mmc_err_int_stat != 0) || (g_mmc_err_info.mmc_present_stat & MSHC_CMD_INHIBIT) != 0 ||
        (cur_pstate & MSHC_CMD_INHIBIT) != 0) {
        return true;
    }
    return false;
}

int hi_mmc_send_stop_cmd(struct rt_mmcsd_host *mmc, uint32_t *status)
{
    int err = 0;
    struct rt_mmcsd_card *card = mmc->card;
    struct rt_mmcsd_cmd cmd = { 0 };

    memset_s(&cmd, sizeof(cmd), 0, sizeof(cmd));
    cmd.cmd_code = STOP_TRANSMISSION;
    cmd.arg = 0;
    cmd.flags = RESP_R1B | CMD_AC;

    err = mmcsd_send_cmd(card->host, &cmd, TRY_TIMES);
    // if the status of emmc device is ok, it will not response to the cmd12.
    if (err != 0) { LOG_E("cmd12 err %x \n", err); }
    err = mmc_send_status(card, ((rt_uint32_t *)status));
    if (err != 0) { LOG_E("cmd13 after cmd12 err %x \n", err); }
    return err;
}

void hi_mmc_set_magic_to_aon_area(void)
{
#if ((defined EMMC_ABNORMAL_SAVE_TO_AON_AREA) && (EMMC_ABNORMAL_SAVE_TO_AON_AREA == YES))
    uint8_t *ptr = (uint8_t*)(uintptr_t)EMMC_ERROR_INFO;
    *ptr = EMMC_NORMAL_MAGIC_VAL;
#else  // Add different items as required.
#endif
}

// sum 8 byte: 1 magic + 1 cmd_code + 2 err_int_stat + 4 present_stat
void hi_mmc_set_err_to_aon_area(void)
{
#if ((defined EMMC_ABNORMAL_SAVE_TO_AON_AREA) && (EMMC_ABNORMAL_SAVE_TO_AON_AREA == YES))
    uint8_t *ptr = (uint8_t*)(uintptr_t)EMMC_ERROR_INFO;
    int32_t ret = 0;
    *ptr = EMMC_ERROR_MAGIC_VAL;
    ptr++;
    ret = memcpy_s((void *)ptr, sizeof(g_mmc_err_info.mmc_cmd_code), &(g_mmc_err_info.mmc_cmd_code),
                   sizeof(g_mmc_err_info.mmc_cmd_code));
    ptr += sizeof(g_mmc_err_info.mmc_cmd_code);
    ret = memcpy_s((void *)ptr, sizeof(g_mmc_err_info.mmc_err_int_stat), &(g_mmc_err_info.mmc_err_int_stat),
                   sizeof(g_mmc_err_info.mmc_err_int_stat));
    ptr += sizeof(g_mmc_err_info.mmc_err_int_stat);
    ret = memcpy_s((void *)ptr, sizeof(g_mmc_err_info.mmc_present_stat), &(g_mmc_err_info.mmc_present_stat),
                   sizeof(g_mmc_err_info.mmc_present_stat));
#else  // Add different items as required.
#endif
}

mmc_err_info_type_t *hi_mmc_get_err_from_aon_area(void)
{
#if ((defined EMMC_ABNORMAL_SAVE_TO_AON_AREA) && (EMMC_ABNORMAL_SAVE_TO_AON_AREA == YES))
    mmc_err_info_type_t *ptr = (mmc_err_info_type_t *)(uintptr_t)EMMC_ERROR_INFO;
    return ptr;
#else  // Add different items as required.
    return &g_mmc_err_info;
#endif
}

void hi_mmc_reset_cmd_data(struct rt_mmcsd_host* host)
{
    if ((host == NULL) || (host->private_data == NULL)) { return; }
    struct hi_mmc_drv *mmc_drv = host->private_data;
    hal_mmc_clr_reg(mmc_drv, MSHC_RESET_CMD);
    hal_mmc_clr_reg(mmc_drv, MSHC_RESET_DATA);
}


void init_emmc_pinmux(void)
{
    int n;

    for (n = 0; n < EMMC_PINMUX_NUM; n++) {
        writew(S_HGPIO + S_HGPIO6_OFFSET + n * 0x4, FUNC_EMMC);
    }

    for (n = 0; n < EMMC_PAD_PINMUX_NUM; n++) {
        writew(S_HGPIO_PAD + S_HGPIO6_PAD_OFFSET + n * 0x4, IE_PINMUX_EN);
    }

    // 配置EMMC时钟源为32M
    writew(SDIOM_CR_REG, SDIOM_CR_CTRL_XO_32M_CLK_EN);
}

int ssb_emmc_init(void)
{
    g_mmc = (struct rt_mmcsd_host *)irmalloc(sizeof(struct rt_mmcsd_host));
    if (g_mmc == NULL) {
        LOG_E("malloc failed, check\r\n");
        return -1; // -1 system error
    }
    memset_s(g_mmc, sizeof(struct rt_mmcsd_host), 0, sizeof(struct rt_mmcsd_host));
    g_mmc->id = 0;    // 0 -- MMC0
    g_mmc->flags |= MMCSD_NON_OS_ENABLE;
    g_mmc->flags |= MMCSD_BUSWIDTH_8;
    writew(EMMC_BASE_ADDR + 0x508, 0);
    init_emmc_pinmux();
    int ret = hi_mmc_init(g_mmc);
    PRINT("emmc init ret=%d\r\n", ret);
    PRINT("emmc msg:blksize=%d,card capacity=%dKB,earse size=%d\n", g_mmc->card->card_blksize,
        g_mmc->card->card_capacity, g_mmc->card->erase_size);
    return ret;
}

struct rt_mmcsd_host* get_ssb_emmc(void)
{
    return g_mmc;
}

int32_t ssb_emmc_read(const char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return hi_mmc_read(buffer, start_sector, nsectors);
}

int32_t ssb_emmc_write(const char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return hi_mmc_write(buffer, start_sector, nsectors);
}

int32_t ssb_emmc_erase(uint32_t start_sector, uint32_t nsectors)
{
    return hi_mmc_erase(start_sector, nsectors, ERASE_TYPE_ERASE_GROUP);
}