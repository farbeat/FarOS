/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:  MMC BLOCK INTERFACE
 * Author:
 * Create:  2021-06
 */
#include "drv_mmc_adapter.h"
#include "mmc.h"
#include "mmcsd_card.h"
#include "mmcsd_core.h"
#include "mmcsd_host.h"
#include "mmc_block.h"

#define SECTOR_SIZE             512
#define PIO_SECTOR_SIZE         512
#define MMC_RW_RETRY_COUNTS     3
#define VENDOR_CMD              60
#define DATA_TRANSFER_TIMEOUT   300000000    // 300ms
#define MMC_DMA_ALIGN_SIZE      64
#define MMC_CURRENT_STATE_TRAN  0x4
#define MMC_RETRY_TIMES         500
#define MMC_RETRY_DELAY         1
#define PADING_LEN              196
#define MAC_KEY_LEN             32
#define DATA_LEN                256
#define RAND_LEN                16
#define WRITE_CNT_LEN           4
#define RESULT_LEN              2
#define HMAC_AUTH_DATA_LEN      284
#define SET_RELIABLE_WRITE      0x80000000UL
#define OFFSET_8                8
#define OFFSET_16               16
#define OFFSET_24               24
#define AUTH_KEY_REQ                    0x1
#define WRITE_COUNTER_READ_REQ          0x2
#define AUTH_DATA_WRITE_REQ             0x3
#define AUTH_DATA_READ_REQ              0x4
#define RESULT_READ_REQ                 0x5
#define AUTH_DEVICE_CONFIG_WRITE_REQ    0x6
#define AUTH_DEVICE_CONFIG_READ_REQ     0x7
#define AUTH_KEY_REPLY                  0x0100
#define WRITE_COUNTER_READ_REPLY        0x0200
#define AUTH_DATA_WRITE_REPLY           0x0300
#define AUTH_DATA_READ_REPLY            0x0400
#define AUTH_DEVICE_CONFIG_WRITE_RESP   0x0600
#define AUTH_DEVICE_CONFIG_READ_RESP    0x0700

typedef struct {
    uint8_t padding[PADING_LEN];
    uint8_t mac_key[MAC_KEY_LEN];
    uint8_t data[DATA_LEN];
    uint8_t rand[RAND_LEN];
    uint32_t write_cnt;
    uint16_t address;
    uint16_t block_cnt;
    uint16_t op_result;
    uint16_t req_resp;
}rpmb_data_t;

typedef struct {
    uint32_t sector;
    uint8_t rw_flag;
    uint32_t blks;
    uint8_t reliable_flag;
} ioctl_data_t;

typedef enum  {
    MMC_READ = 0,
    MMC_WRITE = 1,
} mmc_rw_flag_t;

int g_req_flag = 0;
rpmb_data_t g_rpmb_data __attribute__((__aligned__(64))) = { 0 };

static int mmcsd_req_block(struct rt_mmcsd_card *card, uint32_t sector, void *buf, uint32_t blks, mmc_rw_flag_t rw_flag)
{
    struct rt_mmcsd_cmd cmd, stop;
    struct rt_mmcsd_data data;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_host *host = card->host;
    uint32_t r_cmd, w_cmd;

    memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&stop, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));
    req.cmd = &cmd;
    req.data = &data;

    if (g_req_flag == 1) {
        req.cmd->err = -EBUSY;
        return -1;
    }
    g_req_flag = 1; // Avoid reentrant
    mmcsd_host_lock(host);
    cmd.arg = sector;
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;
    data.blksize = SECTOR_SIZE;
    data.blks  = blks;

    if (blks > 1) {
        req.stop = &stop;
        stop.cmd_code = STOP_TRANSMISSION;
        stop.flags = RESP_SPI_R1B | RESP_R1B | CMD_AC;
        data.flags |= DATA_CMD_AUTO_STOP;
        r_cmd = READ_MULTIPLE_BLOCK;
        w_cmd = WRITE_MULTIPLE_BLOCK;
    } else {
        req.stop = NULL;
        r_cmd = READ_SINGLE_BLOCK;
        w_cmd = WRITE_BLOCK;
    }
    if (rw_flag == MMC_READ) {
        cmd.cmd_code = r_cmd;
        data.flags |= DATA_DIR_READ;
    } else {
        cmd.cmd_code = w_cmd;
        data.flags |= DATA_DIR_WRITE;
    }
    mmcsd_set_data_timeout(&data, card);
    data.buf = buf;
    mmcsd_send_request(host, &req);

    mmcsd_host_unlock(host);
    g_req_flag = 0; // Avoid reentrant
    if ((cmd.err != 0) || (data.err != 0) || (stop.err != 0)) { return -1; }
    return 0;
}

static uint32_t mmc_dma_rw(const void *buf, uint32_t pos, uint32_t size, mmc_rw_flag_t rw_flag)
{
    int32_t err = 0;
    uint32_t offset = 0;
    uint32_t req_size = 0;
    uint32_t remain_size = size;
    if (buf == NULL) {
        return 0;
    }
    void *wr_ptr = (void *)buf;
    struct rt_mmcsd_host *mmc = NULL;
    uint8_t retries = MMC_RW_RETRY_COUNTS;

    if ((uint32_t)buf % EMMC_DMA_ALIGN) { return 0; }
    mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL) {
        panic(1, __LINE__);
        return 0; //lint !e527  unreachable code
    }
    do {
        while (remain_size != 0) {
            req_size = (remain_size > mmc->max_dma_segs) ? mmc->max_dma_segs : remain_size;
            if (rw_flag == MMC_READ) {
                err = mmcsd_req_block(mmc->card, pos + offset, wr_ptr, req_size, MMC_READ);
            } else {
                err = mmcsd_req_block(mmc->card, pos + offset, wr_ptr, req_size, MMC_WRITE);
            }
            if (err != 0) { break; }
            offset += req_size;
            wr_ptr = (void *)((uint8_t *)wr_ptr + (req_size << 9)); // 9
            remain_size -= req_size;
        }
        if (err != 0) {
            retries--;
        } else {
            break;
        }
        LOG_E("mmc_rw:start:%d,len:%d,rw_flag=%d,retries=%d,err_ret=%d", pos, size, rw_flag, retries, err);
    } while (retries != 0);

    if (err != 0) {
        return 0;
    }
    return size - remain_size;
}

uint32_t hi_mmc_read(void *buffer, uint32_t pos, uint32_t size)
{
    uint32_t ret;
    ret = mmc_dma_rw(buffer, pos, size, MMC_READ);
    return ret;
}

uint32_t hi_mmc_write(const void *buffer, uint32_t pos, uint32_t size)
{
    uint32_t ret;
    ret = mmc_dma_rw(buffer, pos, size, MMC_WRITE);
    return ret;
}

uint32_t hi_mmc_pio_read(void *buffer, uint32_t pos, uint32_t size)
{
    uint32_t ret;
    ret = mmc_dma_rw(buffer, pos, size, MMC_READ);
    return ret;
}

uint32_t hi_mmc_pio_write(const void *buffer, uint32_t pos, uint32_t size)
{
    uint32_t ret;
    ret = mmc_dma_rw(buffer, pos, size, MMC_WRITE);
    return ret;
}

int hi_mmc_erase(uint32_t start_sector, uint32_t nsectors, uint32_t type)
{
    struct rt_mmcsd_req req = {0};
    struct rt_mmcsd_erase erase = {0};
    uint32_t tmp_start = start_sector;
    uint32_t tmp_len = nsectors;
    int ret = 0;
    struct rt_mmcsd_host *mmc = NULL;
    struct rt_mmcsd_card *card = NULL;
    mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL || mmc->card == NULL) {
        panic(1, __LINE__);
        return 0; //lint !e527  unreachable code
    }
    mmcsd_host_lock(mmc);
    card = mmc->card;
    req.erase = &erase;
    erase.type = type;
    do {
        erase.start_addr = 0;
        erase.len = 0;
        if (tmp_len > CONFIG_ERASE_MAX_SECTOR) {
            erase.start_addr = tmp_start;
            erase.len = CONFIG_ERASE_MAX_SECTOR;
        } else {
            erase.start_addr = tmp_start;
            erase.len = tmp_len;
        }
        LOG_I("erase: sec str:%d, sec len:%d type %d", erase.start_addr, erase.len, type);
        ret = mmc_erase_arae(card, &req);
        if (ret != 0) {
            LOG_I("erase err ret=%d addr_len=%d type:%d", ret, erase.len, type);
            break;
        }
        tmp_len -= erase.len;
        tmp_start += erase.len;
    } while (tmp_len != 0);
    mmcsd_host_unlock(mmc);
    if (tmp_len == 0) {
        ret = 0;
    }
    return ret;
}

int hi_mmc_init(struct rt_mmcsd_host *mmc)
{
    struct hi_mmc_drv *mmc_drv = NULL;
    int ret = 0;

    if (mmc == NULL) {
        ret = -EINVAL;
        goto err; /*lint !e801*/
    }
    mmc_drv = irmalloc(sizeof(struct hi_mmc_drv));
    if (mmc_drv == NULL) {
        ret = -EINVAL;
        goto err; /*lint !e801*/
    }

    memset_s(mmc_drv, sizeof(struct hi_mmc_drv), 0, sizeof(struct hi_mmc_drv));
    mmc->private_data = mmc_drv;
    mmc_drv->host = mmc;
    hi_mmc_set_magic_to_aon_area();
    hi_mmc_set_drv_features(mmc_drv, mmc->id);
    hi_mmc_set_host_features(mmc);
    ret = hi_mmc_set_int_info(mmc, mmc_drv);
    if (ret != 0) {
        goto err; /*lint !e801*/
    }

    ret = hi_mmc_detect(mmc);
    if (ret != 0) {
        goto err; /*lint !e801*/
    }
    LOG_I("mmc version=%d drv_flags=%x host_flags=%x cap1=%x, cap2=%x\n",
          mmc_drv->version, mmc_drv->flags, mmc->flags,
          hal_mmc_get_cap1(mmc_drv), hal_mmc_get_cap2(mmc_drv));
    return 0;
err:
    if (mmc_drv != NULL) {
        irfree(mmc_drv);
        mmc_drv = NULL;
    }
    if (mmc != NULL) {
        LOG_E("MMC %d err while initialize ret=%d\n", mmc->id, ret);
        irfree(mmc);
    }
    UNUSED(mmc_drv);
    return ret;
}

int hi_mmc_get_cardstatus(void)
{
    int err;
    uint32_t status = 0;
    struct rt_mmcsd_host *mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL || mmc->card == NULL) {
        LOG_E("get mmc error for getcardstate");
        return -1;
    }
    err = mmc_send_status(mmc->card, &status);
    status = R1_CURRENT_STATE((unsigned int)status);
    if (err != 0) {
        LOG_E("get mmc status err=%d, status=%d", err, status);
        return -1;
    }
    return (int)status;
}

int hi_mmc_set_ext_csd_partition_config(PART_CONFIG_AREA area)
{
    int ret;
    uint32_t status;
    struct rt_mmcsd_host *mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL || mmc->card == NULL) {
        return -EPARAMERR;
    }
    if (area < PART_CONFIG_AREA_DEFAULT || area > PART_CONFIG_AREA_RPMB) {
        return EPARAMERR;
    }
    struct rt_mmcsd_card *card = mmc->card;
    LOG_I("set ext csd partition config %x\n", card->ext_csd.part_config);
    card->ext_csd.part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
    card->ext_csd.part_config |= (uint8_t)area;
    ret = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONFIG, card->ext_csd.part_config);
    if (ret != 0) {
        LOG_E("mmc set ext_csd partition config error, err=%d", ret);
        return ret;
    }
    do {
        ret = mmc_send_status(card, (uint32_t *)&status);
        if (ret != 0) {
            return ret;
        }
    } while (((status & R1_READY_FOR_DATA) == 0) || (hi_mmc_card_busy(card->host) != 0));
    return ret;
}

int hi_mmc_sleep_awake(emmc_sleep_mode_e_type sleep_enable)
{
    int err = 0;
    struct rt_mmcsd_host *mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    if (mmc == NULL || mmc->card == NULL) {
        return -EPARAMERR;
    }
    if ((sleep_enable != EMMC_SLEEP_DISABLE) && (sleep_enable != EMMC_SLEEP_ENABLE)) {
        return EPARAMERR;
    }
    struct rt_mmcsd_card *card = mmc->card;
    struct rt_mmcsd_cmd cmd = {0};
    uint32_t status;
    if (sleep_enable != EMMC_SLEEP_DISABLE) {
        err = mmcsd_deselect_cards(card);
        LOG_I("cmd7 from tran to standby status %x \n", err);
    }
    memset_s(&cmd, sizeof(cmd), 0, sizeof(cmd));
    cmd.cmd_code = SLEEP_AWAKE;
    cmd.arg = (card->rca << 16) | ((uint32_t)sleep_enable << 15); // 16 15
    cmd.flags = RESP_SPI_R1B | RESP_R1B | CMD_AC;
    err = mmcsd_send_cmd(card->host, &cmd, 3); // 3
    if (err != 0) {
        return err;
    }
    if (sleep_enable == EMMC_SLEEP_ENABLE) {
        return err;
    }
    if (sleep_enable == EMMC_SLEEP_DISABLE) {
        err = mmcsd_select_card(card);
        LOG_E("cmd7 from standby to trans status %x \n", err);
    }
    do {
        err = mmc_send_status(card, &status);
        if (err != 0) {
            return err;
        }
    } while (((status & R1_READY_FOR_DATA) == 0) || (hi_mmc_card_busy(card->host) != 0));
    LOG_I("cmd5 status %x \n", status);
    return err;
}

int sdio_read_fifo_block(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size)
{
    if (func == NULL || dst == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_extended_block(func, 0, addr, 0, (unsigned char *)dst, size);
}

int sdio_write_fifo_block(struct rt_sdio_function *func, uint32_t addr, void *src, uint32_t size)
{
    if (func == NULL || src == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_extended_block(func, 1, addr, 0, (unsigned char *)src, size);
}

int hi_sdio_io_rw_direct(struct rt_mmcsd_card *card, int writeflag, uint32_t fn,
                         uint32_t addr, uint8_t in, uint8_t *out)
{
    UNUSED(in);
    if (card == NULL || out == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_direct(card, writeflag, fn, addr, out, 0);
}

void sdio_writeb(struct rt_sdio_function *func, uint8_t byte, uint32_t addr, int *err)
{
    if (func == NULL) {
        *err = -EPARAMERR;
        return;
    }
    int ret = sdio_io_rw_direct(func->card, 1, func->num, addr, &byte, 0);
    if (err != NULL) {
        *err = ret;
    }
}

uint8_t sdio_readb(struct rt_sdio_function *func, uint32_t addr, int *err)
{
    uint8_t val = 0;
    if (func == NULL) {
        *err = -EPARAMERR;
        return 0xFF;
    }
    int ret = sdio_io_rw_direct(func->card, 0, func->num, addr, &val, 0);
    *err = ret;

    if (ret != 0) {
        return 0xFF;
    } else {
        return val;
    }
}

int sdio_writesb(struct rt_sdio_function *func, uint32_t addr, void *src, uint32_t size)
{
    if (func == NULL || src == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_extended_block(func, 1, addr, 0, (uint8_t *)src, size);
}

int sdio_readsb(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size)
{
    if (func == NULL || dst == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_extended_block(func, 0, addr, 0, (uint8_t *)dst, size);
}

int sdio_memcpy_fromio(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size)
{
    if (func == NULL || dst == NULL) {
        return -EPARAMERR;
    }
    return sdio_io_rw_extended_block(func, 0, addr, 1, (uint8_t *)dst, size);
}

void sdio_writew(struct rt_sdio_function *func, uint16_t byte, uint32_t addr, int *err)
{
    if (func == NULL) {
        *err = -EPARAMERR;
        return;
    }
    int ret = sdio_io_rw_extended_block(func, 1, addr, 1, (uint8_t *)&byte, 2); // 2 means 2byte
    if (err != NULL) {
        *err = ret;
    }
}

uint16_t sdio_readw(struct rt_sdio_function *func, uint32_t addr, int *err)
{
    uint16_t val = 0;
    if (func == NULL) {
        *err = -EPARAMERR;
        return 0xFFFF;
    }
    int ret = sdio_io_rw_extended_block(func, 0, addr, 1, (uint8_t *)&val, 2); // 2 means 2byte
    if (err != NULL) {
        *err = ret;
    }

    if (ret != 0) {
        return 0xFFFF;
    } else {
        return val;
    }
}

void sdio_writel(struct rt_sdio_function *func, uint32_t byte, uint32_t addr, int *err)
{
    if (func == NULL) {
        *err = -EPARAMERR;
        return;
    }
    int ret = sdio_io_rw_extended_block(func, 1, addr, 1, (uint8_t *)&byte, 4); // 4 means 4byte
    if (err != NULL) {
        *err = ret;
    }
}

uint32_t sdio_readl(struct rt_sdio_function *func, uint32_t addr, int *err)
{
    uint32_t val = 0;
    if (func == NULL) {
        *err = -EPARAMERR;
        return 0xFFFFFFFF;
    }
    int ret = sdio_io_rw_extended_block(func, 0, addr, 1, (uint8_t *)&val, 4); // 4 means 4byte
    if (err != NULL) {
        *err = ret;
    }

    if (ret != 0) {
        return 0xFFFFFFFF;
    } else {
        return val;
    }
}

int sdio_reset(struct rt_mmcsd_card *card)
{
    int error;
    uint8_t out = 0;
    if (card == NULL) {
        return -EPARAMERR;
    }
    error = sdio_io_rw_direct(card, 0, 0, SDIO_REG_CCCR_IO_ABORT, &out, 0);
    if (error != 0) {
        out = 0x08;
    } else {
        out |= 0x08;
    }
    error = sdio_io_rw_direct(card, 1, 0, SDIO_REG_CCCR_IO_ABORT, &out, 0);
    return error;
}

void hi_mmc_set_sdiom_clk_mode(sdiom_clk_cfg_e_type clk_type)
{
    if (clk_type < SDIO_HOST_HIGH_50MHZ_EMMC_HIGH_50MHZ || clk_type >= SDIOM_CLK_MODE_MAX_NUM) {
        return;
    }
    mmc_set_sdiom_clk_mode((int)clk_type);
}

int32_t hi_mmc_switch_cmd(struct rt_mmcsd_card *card, uint32_t cmd_arg_val)
{
    int32_t err;
    if (card == NULL || card->host == NULL) {
        return -EPARAMERR;
    }
    struct rt_mmcsd_host *host = card->host;
    struct rt_mmcsd_cmd cmd = { 0 };
    uint32_t val, status;
    uint32_t i = 0;

    cmd.cmd_code = SWITCH;
    cmd.arg = cmd_arg_val;
    cmd.flags = RESP_R1 | CMD_AC;

    err = mmcsd_send_cmd(host, &cmd, 1);
    if (err != 0) {
        return err;
    } else {
        for (i = 0; i < MMC_RETRY_TIMES; i++) {
            err = mmc_send_status(card, ((uint32_t *)&val));
            status = R1_CURRENT_STATE(val);
            if ((err == 0) && (status == MMC_CURRENT_STATE_TRAN)) {
                break;
            } else {
                LOG_E("get mmc status err=%d, status=%d", err, status);
                uapi_tcxo_delay_ms(MMC_RETRY_DELAY);
            }
        }
    }
    if (i == MMC_RETRY_TIMES) {
        return -1;
    }
    return 0;
}

int32_t hi_mmc_get_ext_csd(struct rt_mmcsd_card *card,  void *ext_csd)
{
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    if (card == NULL || card->host == NULL || ext_csd == NULL) {
        return -EPARAMERR;
    }
    memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;
    cmd.cmd_code = SEND_EXT_CSD;
    cmd.arg = 0;
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;
    data.blksize = SECTOR_SIZE;
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = (uint32_t *)ext_csd;
    data.timeout_ns = DATA_TRANSFER_TIMEOUT;
    data.timeout_clks = 0;

    mmcsd_send_request(card->host, &req);
    if (cmd.err != 0) {
        return cmd.err;
    }
    if (data.err != 0) {
        return data.err;
    }
    return 0;
}

int32_t hi_mmc_gen_cmd(struct rt_mmcsd_card *card,  uint32_t cmd_arg_val, uint32_t *data_buf)
{
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;
    if (card == NULL || card->host == NULL || data_buf == NULL) {
        return -EPARAMERR;
    }
    memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;
    cmd.cmd_code = GEN_CMD;
    cmd.arg = cmd_arg_val;
    cmd.flags = RESP_R1 | CMD_ADTC;
    data.blksize = SECTOR_SIZE;
    data.blks = 1;
    if ((cmd_arg_val & 0x1) != 0) {
        data.flags = DATA_DIR_READ;
    } else {
        data.flags = DATA_DIR_WRITE;
    }
    data.buf = (uint32_t *)data_buf;
    data.timeout_ns = DATA_TRANSFER_TIMEOUT;
    data.timeout_clks = 0;

    mmcsd_send_request(card->host, &req);
    if (cmd.err != 0) {
        return cmd.err;
    }
    if (data.err != 0) {
        return data.err;
    }
    return 0;
}

int32_t hi_mmc_vendor_cmd(struct rt_mmcsd_card *card,  uint32_t cmd_arg_val)
{
    int32_t err;
    if (card == NULL || card->host == NULL) {
        return -EPARAMERR;
    }
    struct rt_mmcsd_host *host = card->host;
    struct rt_mmcsd_cmd cmd = { 0 };

    cmd.cmd_code = VENDOR_CMD;
    cmd.arg = cmd_arg_val;
    cmd.flags = RESP_R1 | CMD_ADTC;

    err = mmcsd_send_cmd(host, &cmd, 1);
    if (err != 0) {
        return err;
    }
    return 0;
}

uint32_t* hi_mmc_get_cid(struct rt_mmcsd_card *card)
{
    if (card == NULL) {
        return NULL;
    }
    return (uint32_t*)(card->resp_cid);
}

void hi_mmc_set_driver_capability(struct rt_mmcsd_host* host, emmc_phy_driver_capability_e_type cap)
{
    if (host == NULL) {
        return;
    }
    mmc_set_driver_capability(host, (uint32_t)cap);
}

int32_t hi_mmc_set_block_count(struct rt_mmcsd_card *card, uint32_t blk_cnt, uint8_t reliable_write_flag)
{
    int32_t err;
    struct rt_mmcsd_cmd cmd = { 0 };

    cmd.cmd_code = SET_BLOCK_COUNT;
    if (reliable_write_flag == 1) {
        cmd.arg = blk_cnt | SET_RELIABLE_WRITE;
    } else {
        cmd.arg = blk_cnt;
    }
    cmd.flags = RESP_R1 | CMD_AC;
    err = mmcsd_send_cmd(card->host, &cmd, 3); // retry 3 times
    if (err) {
        LOG_E("cmd23 status err=%d, resp=%x", err, cmd.resp[0]);
    }
    return err;
}

int32_t hi_mmc_rpmb_ioctl(struct rt_mmcsd_card *card, rpmb_data_t *rpmb_data, ioctl_data_t io_data)
{
    struct rt_mmcsd_cmd cmd, stop;
    struct rt_mmcsd_data data;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_host *host = card->host;
    int32_t err;
    uint32_t status;

    memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&stop, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));
    req.cmd = &cmd;
    req.data = &data;

    mmcsd_host_lock(host);
    cmd.arg = io_data.sector;
    cmd.flags = RESP_R1 | CMD_ADTC;
    data.blksize = SECTOR_SIZE;
    data.blks  = io_data.blks;

    if (io_data.rw_flag == MMC_READ) {
        cmd.cmd_code = READ_MULTIPLE_BLOCK;
        data.flags |= DATA_DIR_READ;
    } else {
        cmd.cmd_code = WRITE_MULTIPLE_BLOCK;
        data.flags |= DATA_DIR_WRITE;
    }
    mmcsd_set_data_timeout(&data, card);
    data.buf = (uint32_t *)rpmb_data;
    if (io_data.reliable_flag == 1) {
        hi_mmc_set_block_count(card, 1, 1);
    } else {
        hi_mmc_set_block_count(card, 1, 0);
    }
    mmcsd_send_request(host, &req);
    mmcsd_host_unlock(host);
    if ((cmd.err != 0) || (data.err != 0) || (stop.err != 0)) {
        LOG_E("cmd.err=%x, data.err=%x, stop.err=%x\r\n", cmd.err, data.err, stop.err);
        return -1;
    }

    do {
        err = mmc_send_status(card, (uint32_t *)&status);
        if (err) {
            return err;
        }
    } while (((status & R1_READY_FOR_DATA) == 0) || (hi_mmc_card_busy(card->host) != 0));

    return 0;
}

int32_t hi_mmc_write_rpmb(struct rt_mmcsd_card *card, uint32_t sector, uint32_t blks,
                          rpmb_data_t *rpmb_data, uint8_t reliable_flag)
{
    int32_t err = 0;
    ioctl_data_t io_data;
    io_data.sector = sector;
    io_data.rw_flag = MMC_WRITE;
    io_data.blks = blks;
    io_data.reliable_flag = reliable_flag;
    err = hi_mmc_rpmb_ioctl(card, rpmb_data, io_data);
    return err;
}

int32_t hi_mmc_read_rpmb(struct rt_mmcsd_card *card, uint32_t sector, uint32_t blks, rpmb_data_t *rpmb_data)
{
    int32_t err = 0;
    ioctl_data_t io_data;
    io_data.sector = sector;
    io_data.rw_flag = MMC_READ;
    io_data.blks = blks;
    io_data.reliable_flag = 0;
    err = hi_mmc_rpmb_ioctl(card, rpmb_data, io_data);
    return err;
}

int32_t hi_mmc_rpmb_get_write_counter(struct rt_mmcsd_card *card,  uint32_t *counter, uint32_t *result)
{
    int err = 0;
    memset_s(&g_rpmb_data, sizeof(rpmb_data_t), 0, sizeof(rpmb_data_t));
    g_rpmb_data.req_resp = (WRITE_COUNTER_READ_REQ << OFFSET_8);

    err = hi_mmc_write_rpmb(card, 0, 1, &g_rpmb_data, 0);
    if (err != 0) {
        LOG_E("mmc write rpmb error, err=%d", err);
        return err;
    }

    err = hi_mmc_read_rpmb(card, 0, 1, &g_rpmb_data);
    if (err != 0) {
        LOG_E("mmc read rpmb error, err=%d", err);
        return err;
    }

    *counter = (g_rpmb_data.write_cnt & 0x000000FF) << OFFSET_24;
    *counter |= (g_rpmb_data.write_cnt & 0x0000FF00) << OFFSET_8;
    *counter |= (g_rpmb_data.write_cnt & 0x00FF0000) >> OFFSET_8;
    *counter |= (g_rpmb_data.write_cnt & 0xFF000000) >> OFFSET_24;
    *result = (((g_rpmb_data.op_result & 0xff) << OFFSET_8) | ((g_rpmb_data.op_result & 0xff00) >> OFFSET_8));
    LOG_I("mmc rpmb request=%d, writecounter=%x, result=%d",
          g_rpmb_data.req_resp, g_rpmb_data.write_cnt, g_rpmb_data.op_result);

    return err;
}

int32_t hi_mmc_rpmb_set_key(struct rt_mmcsd_card *card, uint8_t *key, uint32_t *result)
{
    int err = 0;
    memset_s(&g_rpmb_data, sizeof(rpmb_data_t), 0, sizeof(rpmb_data_t));

    for (uint8_t i = 0; i < MAC_KEY_LEN; i++) {
        g_rpmb_data.mac_key[i] = ((char *)key)[i];
    }

    g_rpmb_data.req_resp = (AUTH_KEY_REQ << OFFSET_8);

    err = hi_mmc_write_rpmb(card, 0, 1, &g_rpmb_data, 1);
    if (err != 0) {
        LOG_E("mmc write rpmb error, err=%d", err);
        return err;
    }

    memset_s(&g_rpmb_data, sizeof(rpmb_data_t), 0, sizeof(rpmb_data_t));
    g_rpmb_data.req_resp = (RESULT_READ_REQ << OFFSET_8);
    err = hi_mmc_write_rpmb(card, 0, 1, &g_rpmb_data, 0);
    if (err != 0) {
        LOG_E("mmc write rpmb error, err=%d", err);
        return err;
    }
    err = hi_mmc_read_rpmb(card, 0, 1, &g_rpmb_data);
    if (err != 0) {
        LOG_E("mmc read rpmb error, err=%d", err);
        return err;
    }

    *result = (((g_rpmb_data.op_result & 0xff) << OFFSET_8) | ((g_rpmb_data.op_result & 0xff00) >> OFFSET_8));
    LOG_I("mmc rpmb request=%d, result=%d", g_rpmb_data.req_resp, g_rpmb_data.op_result);

    return err;
}

int32_t hi_mmc_rpmb_cmp_key(uint8_t *key)
{
    UNUSED(key);
    uint8_t auth_data[HMAC_AUTH_DATA_LEN] = { 0 };
    int32_t err = 0;
    uint8_t hmac[MAC_KEY_LEN] = { 0 };
    uint8_t *tmp_data = (uint8_t *)g_rpmb_data.data;

    err = memcpy_s(auth_data, HMAC_AUTH_DATA_LEN, tmp_data, HMAC_AUTH_DATA_LEN);
    if (err != 0) {
        LOG_E("hi_mmc_rpmb_cmp_key memcpy error, err=%d", err);
        return -1;
    }
#ifdef MEDTLS_MD
    err = mbedtls_md_hmac(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                          key, MAC_KEY_LEN, auth_data, HMAC_AUTH_DATA_LEN, hmac);
#endif
    if (err != 0) {
        LOG_E("mbedtls hmac error, err=%d", err);
        return -1;
    }
    err = memcmp(g_rpmb_data.mac_key, hmac, MAC_KEY_LEN);
    if (err != 0) {
        LOG_E("rpmb key check error, err=%d", err);
        return -1;
    }
    return 0;
}

void hi_mmc_err_recovery(void)
{
    struct rt_mmcsd_host *mmc = hi_mmc_get_host(SDIO_EMMC_ID);
    int32_t non_recovery_flag = 0;
    int32_t ret = 0;
    uint32_t status, val, i;
    if (mmc == NULL || mmc->card == NULL) {
        LOG_E("get mmc error for getcardstate");
        return ;
    }
    g_req_flag = 0;
    // 1. make sure the data has been transfered completed.
    for (i = 0; i < MMC_RETRY_TIMES; i++) {
        ret = mmc_send_status(mmc->card, ((uint32_t *)&val));
        status = R1_CURRENT_STATE(val);
        if ((ret == 0) && (status == MMC_CURRENT_STATE_TRAN)) {
            return;
        } else {
            uapi_tcxo_delay_ms(MMC_RETRY_DELAY);
        }
    }
    LOG_E("wait to transfer the data, get mmc status err=%d, status=%d", ret, status);
    // 2. make the host to reset CMD and DATA
    if (mmc_detect_bus_error(mmc)) {
        hi_mmc_reset_cmd_data(mmc);  // reset the data and cmd line;
    }
    // 3. make the device to revovery to normal(CMD12)
    for (i = 0; i < MMC_RETRY_TIMES; i++) {
        ret = hi_mmc_send_stop_cmd(mmc, &status);   // send cmd12 to stop the transmission
        if ((ret == 0) && (R1_CURRENT_STATE(status) == MMC_CURRENT_STATE_TRAN)) {
            LOG_E("recovery send cmd12 err=%d, status=%d", ret, status);
            break;
        }
        uapi_tcxo_delay_ms(MMC_RETRY_DELAY);
    }
    LOG_E("wait to send cmd12, get mmc status err=%d, status=%d", ret, status);
    if (i == MMC_RETRY_TIMES) { non_recovery_flag = 1; }
    // 4. at last, judge host if it is ok, if not, record the flag to shareram.
    if (non_recovery_flag == 1) { // emmc cannot recovery, need re-initial the device
        hi_mmc_set_err_to_aon_area();
    }
}

mmc_err_info_type_t* hi_mmc_get_emmc_failed_info(void)
{
    return hi_mmc_get_err_from_aon_area();
}