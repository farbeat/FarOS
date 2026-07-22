/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-06-15     hichard      first version
 */

#include "mmcsd_core.h"
#include "drv_mmc_adapter.h"
#include "mshc_reg.h"
#include "mmc.h"

#define DBG_TAG               "SDIO"
#ifdef RT_SDIO_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* RT_SDIO_DEBUG */
#define EXT_CSD_LEN  512

static const uint32_t tran_unit[] = {
    10000, 100000, 1000000, 10000000,
    0,     0,      0,       0
};

static const uint8_t tran_value[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static const uint32_t tacc_uint[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
};

static const uint8_t tacc_value[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

rt_inline uint32_t GET_BITS(uint32_t *resp, uint32_t  start, uint32_t  size)
{
    const int32_t __size = size;
    const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1; // 32
    const int32_t __off = 3 - ((start) / 32); // 32 3
    const int32_t __shft = (start) & 31; // 31
    uint32_t __res;

    __res = resp[__off] >> __shft;
    if (__size + __shft > 32) { // 32
        __res |= resp[__off-1] << ((32 - __shft) % 32); // 32
    }

    return __res & __mask;
}

/*
 * Given a 128-bit response, decode to our card CSD structure.
 */
static int32_t mmcsd_parse_csd(struct rt_mmcsd_card *card)
{
    uint32_t a, b;
    struct rt_mmcsd_csd *csd = &card->csd;
    uint32_t *resp = card->resp_csd;

    /*
    * We only understand CSD structure v1.1 and v1.2.
    * v1.2 has extra information in bits 15, 11 and 10.
    * We also support eMMC v4.4 & v4.41.
    */
    csd->csd_structure = GET_BITS(resp, 126, 2); // 126 2
    if (csd->csd_structure == 0) {
        LOG_E("unrecognised CSD structure version %d!", csd->csd_structure);
        return -RT_ERROR;
    }
    csd->spec_vers = GET_BITS(resp, 122, 4); // 122 4
    csd->taac = GET_BITS(resp, 112, 8); // 112 8
    csd->nsac = GET_BITS(resp, 104, 8); // 104 8
    csd->tran_speed = GET_BITS(resp, 96, 8); // 96 8
    csd->card_cmd_class = GET_BITS(resp, 84, 12); // 84 12
    csd->rd_blk_len = GET_BITS(resp, 80, 4); // 80 4
    csd->rd_blk_part = GET_BITS(resp, 79, 1); // 79 1
    csd->wr_blk_misalign = GET_BITS(resp, 78, 1); // 78 1
    csd->rd_blk_misalign = GET_BITS(resp, 77, 1); // 77 1
    csd->dsr_imp = GET_BITS(resp, 76, 1); // 76 1
    csd->c_size = GET_BITS(resp, 62, 12); // 62 12
    csd->c_size_mult = GET_BITS(resp, 47, 3); // 47 3
    csd->r2w_factor = GET_BITS(resp, 26, 3); // 26 3
    csd->wr_blk_len = GET_BITS(resp, 22, 4); // 22 4
    csd->wr_blk_partial = GET_BITS(resp, 21, 1); // 21 1
    csd->csd_crc = GET_BITS(resp, 1, 7); // 1 7

    card->card_blksize = 1 << csd->rd_blk_len;
    card->tacc_clks = csd->nsac * 100; // 100
    card->tacc_ns = (tacc_uint[csd->taac&0x07] * tacc_value[(csd->taac&0x78)>>3] + 9) / 10; // 3 9 10
    card->max_data_rate = tran_unit[csd->tran_speed&0x07] * tran_value[(csd->tran_speed&0x78)>>3]; // 3
    if (csd->wr_blk_len >= 9) { // 9
        a = GET_BITS(resp, 42, 5); // 42 5
        b = GET_BITS(resp, 37, 5); // 37 5
        card->erase_size = (a + 1) * (b + 1);
        card->erase_size <<= csd->wr_blk_len - 9; // 9
    }
    return 0;
}

/*
 * Read extended CSD.
 */
static int mmc_get_ext_csd(struct rt_mmcsd_card *card, uint8_t **new_ext_csd)
{
    void *ext_csd;
    struct rt_mmcsd_req req;
    struct rt_mmcsd_cmd cmd;
    struct rt_mmcsd_data data;

    *new_ext_csd = RT_NULL;

    if (GET_BITS(card->resp_cid, 122, 4) < 4) { // 122 4
        return 0;
    }

    /*
    * As the ext_csd is so large and mostly unused, we don't store the
    * raw block in mmc_card.
    */
    ext_csd = irmalloc(EXT_CSD_LEN);
    if (!ext_csd) {
        LOG_E("alloc memory failed when get ext csd!");
        return -RT_ENOMEM;
    }
    (void)memset_s(ext_csd, EXT_CSD_LEN, 0, EXT_CSD_LEN);
    (void)memset_s(&req, sizeof(struct rt_mmcsd_req), 0, sizeof(struct rt_mmcsd_req));
    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    (void)memset_s(&data, sizeof(struct rt_mmcsd_data), 0, sizeof(struct rt_mmcsd_data));

    req.cmd = &cmd;
    req.data = &data;

    cmd.cmd_code = SEND_EXT_CSD;
    cmd.arg = 0;

    /* NOTE HACK:    the RESP_SPI_R1 is always correct here, but we
    * rely on callers to never use this with "native" calls for reading
    * CSD or CID.    Native versions of those commands use the R2 type,
    * not R1 plus a data block.
    */
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_ADTC;

    data.blksize = 512; // 512
    data.blks = 1;
    data.flags = DATA_DIR_READ;
    data.buf = ext_csd;

    /*
    * Some cards require longer data read timeout than indicated in CSD.
    * Address this by setting the read timeout to a "reasonably high"
    * value. For the cards tested, 300ms has proven enough. If necessary,
    * this value can be increased if other problematic cards require this.
    */
    data.timeout_ns = 300000000; // 300000000
    data.timeout_clks = 0;

    mmcsd_send_request(card->host, &req);
    if (cmd.err) {
        return cmd.err;
    }

    if (data.err) {
        return data.err;
    }

    *new_ext_csd = ext_csd;
    (void)memcpy_s((void *)card->ext_csd_val, EXT_CSD_LEN, ext_csd, EXT_CSD_LEN);
    return 0;
}

/*
 * Decode extended CSD.
 */
static int mmc_parse_ext_csd(struct rt_mmcsd_card *card, uint8_t *ext_csd)
{
    rt_uint64_t card_capacity = 0;
    int retval = 0;
    if (card == RT_NULL || ext_csd == RT_NULL) {
        LOG_E("emmc parse ext csd fail, invaild args");
        return -1;
    }
    struct rt_mmcsd_ext_csd *ext_csd_reg = RT_NULL;

    ext_csd_reg = &(card->ext_csd);

    ext_csd_reg->raw_ext_csd_structure = ext_csd[EXT_CSD_STRUCTURE];
    if (card->csd.csd_structure == 3) { // 3
        if (ext_csd_reg->raw_ext_csd_structure > 2) { // 2
            LOG_E("unrecognised EXT_CSD structure ersion %d", ext_csd_reg->raw_ext_csd_structure);
            retval = -EINVAL;
            return retval;
        }
    }
    ext_csd_reg->rev = ext_csd[EXT_CSD_REV];
    ext_csd_reg->raw_sectors[0] = ext_csd[EXT_CSD_SEC_CNT + 0]; // 0
    ext_csd_reg->raw_sectors[1] = ext_csd[EXT_CSD_SEC_CNT + 1]; // 1
    ext_csd_reg->raw_sectors[2] = ext_csd[EXT_CSD_SEC_CNT + 2]; // 2
    ext_csd_reg->raw_sectors[3] = ext_csd[EXT_CSD_SEC_CNT + 3]; // 3
    if (ext_csd_reg->rev >= 2) { // 2
        ext_csd_reg->sectors = (ext_csd[EXT_CSD_SEC_CNT + 0] << 0) |
            (ext_csd[EXT_CSD_SEC_CNT + 1] << 8) | /* 8 */
            (ext_csd[EXT_CSD_SEC_CNT + 2] << 16) | /* 2 16 */
            (ext_csd[EXT_CSD_SEC_CNT + 3] << 24); /* 3 24 */
    }

    ext_csd_reg->strobe_support = ext_csd[EXT_CSD_STROBE_SUPPORT];
    ext_csd_reg->raw_card_type = ext_csd[EXT_CSD_CARD_TYPE];
    switch (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_MASK) {
        case EXT_CSD_CARD_TYPE_ALL_1_8V:
        case EXT_CSD_CARD_TYPE_1_8V_HS200:
            ext_csd_reg->hs_max_dtr = EXT_CSD_CARD_MAX_RATE_200;
            ext_csd_reg->card_type = EXT_CSD_CARD_TYPE_HS200_1_8V;
            break;
        case EXT_CSD_CARD_TYPE_DDR_52 | EXT_CSD_CARD_TYPE_HS_52 | EXT_CSD_CARD_TYPE_HS_26:
            ext_csd_reg->hs_max_dtr = EXT_CSD_CARD_MAX_RATE_52;
            ext_csd_reg->card_type = EXT_CSD_CARD_TYPE_DDR_52;
            break;
        case EXT_CSD_CARD_TYPE_DDR_1_8V | EXT_CSD_CARD_TYPE_HS_52 | EXT_CSD_CARD_TYPE_HS_26:
            ext_csd_reg->hs_max_dtr = EXT_CSD_CARD_MAX_RATE_52;
            ext_csd_reg->card_type = EXT_CSD_CARD_TYPE_DDR_1_8V;
            break;
        case EXT_CSD_CARD_TYPE_HS_52 | EXT_CSD_CARD_TYPE_HS_26:
            ext_csd_reg->hs_max_dtr = EXT_CSD_CARD_MAX_RATE_52;
            break;
        default:
            LOG_E("card type:%#x, need to parse the type", ext_csd[EXT_CSD_CARD_TYPE]);
            break;
    }

    ext_csd_reg->raw_s_a_timeout = ext_csd[EXT_CSD_S_A_TIMEOUT];
    ext_csd_reg->raw_erase_timeout_mult = ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT];
    ext_csd_reg->raw_hc_erase_grp_size = ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];
    if (ext_csd_reg->rev >= 3) { // 3
        uint8_t shift = ext_csd[EXT_CSD_S_A_TIMEOUT];
        ext_csd_reg->part_config = ext_csd[EXT_CSD_PART_CONFIG];
        ext_csd_reg->part_time_ms = 10 * ext_csd[EXT_CSD_PART_SWITCH_TIME]; // 10
        if (shift > 0 && shift <= 0x17) {
            ext_csd_reg->sa_timeout_ns = 1 << ext_csd[EXT_CSD_S_A_TIMEOUT];
        }

        ext_csd_reg->erase_group_def = ext_csd[EXT_CSD_ERASE_GROUP_DEF];
        ext_csd_reg->hc_erase_timeout = 300 * ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT]; // 300
        ext_csd_reg->hc_erase_size = ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] << 10; // 10
        ext_csd_reg->rel_sectors = ext_csd[EXT_CSD_REL_WR_SEC_C];
        ext_csd_reg->boot_size = ext_csd[EXT_CSD_BOOT_MULT] << 17; // 17
    }

    ext_csd_reg->raw_hc_erase_gap_size = ext_csd[EXT_CSD_PARTITION_ATTRIBUTE];
    ext_csd_reg->raw_sec_trim_mult = ext_csd[EXT_CSD_SEC_TRIM_MULT];
    ext_csd_reg->raw_sec_erase_mult = ext_csd[EXT_CSD_SEC_ERASE_MULT];
    ext_csd_reg->raw_sec_feature_support = ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT];
    ext_csd_reg->raw_trim_mult = ext_csd[EXT_CSD_TRIM_MULT];
    if (ext_csd_reg->rev >= 4) { // 4
        if ((ext_csd[EXT_CSD_PARTITION_SUPPORT] & 0x2) && (ext_csd[EXT_CSD_PARTITION_ATTRIBUTE] & 0x1)) {
            uint8_t hc_erase_grp_sz = ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];
            uint8_t hc_wp_grp_sz = ext_csd[EXT_CSD_HC_WP_GRP_SIZE];
            ext_csd_reg->enhanced_area_en = 1;

            // 139 24 138 16 137 8 136
            ext_csd_reg->enhanced_area_offset = ((rt_uint64_t)ext_csd[139] << 24) +
                ((rt_uint64_t)ext_csd[138] << 16) + ((rt_uint64_t)ext_csd[137] << 8) + (rt_uint64_t)ext_csd[136];
            ext_csd_reg->enhanced_area_offset <<= 9;   // device is block device 9
            // 142 16 141 8 140
            ext_csd_reg->enhanced_area_size = (ext_csd[142] << 16) + (ext_csd[141] << 8) + ext_csd[140];
            ext_csd_reg->enhanced_area_size *= (uint32_t)(hc_erase_grp_sz * hc_wp_grp_sz);
            ext_csd_reg->enhanced_area_size <<= 9; // 9
        } else {
            ext_csd_reg->enhanced_area_offset = (uint64_t)(-EINVAL);/*lint !e570*/
            ext_csd_reg->enhanced_area_size = (uint32_t)(-EINVAL);/*lint !e570*/
        }
        ext_csd_reg->sec_trim_mult = ext_csd[EXT_CSD_SEC_TRIM_MULT];
        ext_csd_reg->sec_erase_mult = ext_csd[EXT_CSD_SEC_ERASE_MULT];
        ext_csd_reg->sec_feature_support = ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT];
        ext_csd_reg->trim_timeout = 300 * ext_csd[EXT_CSD_TRIM_MULT]; // 300
    }

    if (ext_csd_reg->rev >= 5) { // 5
        /* whether the eMMC card supports HPI */
        if (ext_csd[EXT_CSD_HPI_FEATURES] & 0x1) {
            ext_csd_reg->hpi = 1;
            if (ext_csd[EXT_CSD_HPI_FEATURES] & 0x2) {
                ext_csd_reg->hpi_cmd = STOP_TRANSMISSION;
            } else {
                ext_csd_reg->hpi_cmd = SEND_STATUS;
            }
            /*
            ** Indicate the maximum timeout to close
            ** a command interrupted by HPI
            **/
            ext_csd_reg->out_of_int_time = ext_csd[EXT_CSD_OUT_OF_INTERRUPT_TIME] * 10; // 10
        }
        ext_csd_reg->rel_param = ext_csd[EXT_CSD_WR_REL_PARAM];
    }

    card->hs_max_data_rate = ext_csd_reg->hs_max_dtr;
    card_capacity = *((uint32_t *)&ext_csd[EXT_CSD_SEC_CNT]);
    card_capacity *= card->card_blksize;
    card_capacity >>= 10; /* unit:KB 10 */
    card->card_capacity = card_capacity;
    LOG_D("emmc card capacity %d KB. card_type=%d", card->card_capacity, ext_csd_reg->card_type);
    return retval;
}

int32_t mmc_send_status(struct rt_mmcsd_card *card, uint32_t *status)
{
    int32_t err;
    struct rt_mmcsd_cmd cmd = {0};

    cmd.cmd_code = SEND_STATUS;
    cmd.arg = card->rca << 16; // 16
    cmd.flags = RESP_R1 | CMD_AC;
    err = mmcsd_send_cmd(card->host, &cmd, 5); // 5
    if (err) {
        LOG_E("cmd13 requesting status failed ret=%d", err);
    }
    if (status != NULL) {
       *status = cmd.resp[0];
    }
    return err;
}

/**
 *   mmc_switch - modify EXT_CSD register
 *   @card: the MMC card associated with the data transfer
 *   @set: cmd set values
 *   @fields: EXT_CSD register index
 *   @value: value to program into EXT_CSD register
 *
 *   Modifies the EXT_CSD register for selected card.
 */
int mmc_switch(struct rt_mmcsd_card *card, uint8_t set, uint8_t fields, uint8_t value)
{
    int err;
    struct rt_mmcsd_host *host = card->host;
    struct rt_mmcsd_cmd cmd = {0};

    cmd.cmd_code = SWITCH;
    cmd.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (fields << 16) | (value << 8) | set; // 24 16 8
    cmd.flags = RESP_SPI_R1 | RESP_R1 | CMD_AC;

    err = mmcsd_send_cmd(host, &cmd, 3); // 3
    if (err) {
        return err;
    }
    return 0;
}

static int mmc_compare_ext_csds(struct rt_mmcsd_card *card, uint8_t *ext_csd, uint32_t bus_width)
{
    uint8_t *bw_ext_csd;
    int err;

    if (bus_width == MMCSD_BUS_WIDTH_1) {
        return 0;
    }
    err = mmc_get_ext_csd(card, &bw_ext_csd);
    if (err || bw_ext_csd == RT_NULL) {
        err = -RT_ERROR;
        goto out;
    }

    /* only compare read only fields */
    err = !((ext_csd[EXT_CSD_PARTITION_SUPPORT] == bw_ext_csd[EXT_CSD_PARTITION_SUPPORT]) &&
          (ext_csd[EXT_CSD_ERASED_MEM_CONT] == bw_ext_csd[EXT_CSD_ERASED_MEM_CONT]) &&
          (ext_csd[EXT_CSD_REV] == bw_ext_csd[EXT_CSD_REV]) &&
          (ext_csd[EXT_CSD_STRUCTURE] == bw_ext_csd[EXT_CSD_STRUCTURE]) &&
          (ext_csd[EXT_CSD_CARD_TYPE] == bw_ext_csd[EXT_CSD_CARD_TYPE]) &&
          (ext_csd[EXT_CSD_S_A_TIMEOUT] == bw_ext_csd[EXT_CSD_S_A_TIMEOUT]) &&
          (ext_csd[EXT_CSD_HC_WP_GRP_SIZE] == bw_ext_csd[EXT_CSD_HC_WP_GRP_SIZE]) &&
          (ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT] == bw_ext_csd[EXT_CSD_ERASE_TIMEOUT_MULT]) &&
          (ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] == bw_ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE]) &&
          (ext_csd[EXT_CSD_SEC_TRIM_MULT] == bw_ext_csd[EXT_CSD_SEC_TRIM_MULT]) &&
          (ext_csd[EXT_CSD_SEC_ERASE_MULT] == bw_ext_csd[EXT_CSD_SEC_ERASE_MULT]) &&
          (ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT] == bw_ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT]) &&
          (ext_csd[EXT_CSD_TRIM_MULT] == bw_ext_csd[EXT_CSD_TRIM_MULT]) &&
          (ext_csd[EXT_CSD_SEC_CNT + 0] == bw_ext_csd[EXT_CSD_SEC_CNT + 0]) &&
          (ext_csd[EXT_CSD_SEC_CNT + 1] == bw_ext_csd[EXT_CSD_SEC_CNT + 1]) &&
          (ext_csd[EXT_CSD_SEC_CNT + 2] == bw_ext_csd[EXT_CSD_SEC_CNT + 2]) && /* 2 */
          (ext_csd[EXT_CSD_SEC_CNT + 3] == bw_ext_csd[EXT_CSD_SEC_CNT + 3]) && /* 3 */
          (ext_csd[EXT_CSD_PWR_CL_52_195] == bw_ext_csd[EXT_CSD_PWR_CL_52_195]) &&
          (ext_csd[EXT_CSD_PWR_CL_26_195] == bw_ext_csd[EXT_CSD_PWR_CL_26_195]) &&
          (ext_csd[EXT_CSD_PWR_CL_52_360] == bw_ext_csd[EXT_CSD_PWR_CL_52_360]) &&
          (ext_csd[EXT_CSD_PWR_CL_26_360] == bw_ext_csd[EXT_CSD_PWR_CL_26_360]) &&
          (ext_csd[EXT_CSD_PWR_CL_200_195] == bw_ext_csd[EXT_CSD_PWR_CL_200_195]) &&
          (ext_csd[EXT_CSD_PWR_CL_200_360] == bw_ext_csd[EXT_CSD_PWR_CL_200_360]) &&
          (ext_csd[EXT_CSD_PWR_CL_DDR_52_195] == bw_ext_csd[EXT_CSD_PWR_CL_DDR_52_195]) &&
          (ext_csd[EXT_CSD_PWR_CL_DDR_52_360] == bw_ext_csd[EXT_CSD_PWR_CL_DDR_52_360]) &&
          (ext_csd[EXT_CSD_PWR_CL_DDR_200_360] == bw_ext_csd[EXT_CSD_PWR_CL_DDR_200_360]));

    if (err) {
        err = -RT_ERROR;
    }

out:
    irfree(bw_ext_csd);
    return err;
}

/*
 * Select the bus width amoung 4-bit and 8-bit(SDR).
 * If the bus width is changed successfully, return the selected width value.
 * Zero is returned instead of error value if the wide width is not supported.
 */
static int mmc_select_bus_width(struct rt_mmcsd_card *card, uint8_t *ext_csd)
{
    uint32_t ext_csd_bits[] = {
        EXT_CSD_BUS_WIDTH_8,
        EXT_CSD_BUS_WIDTH_4,
        EXT_CSD_BUS_WIDTH_1
    };
    uint32_t bus_widths[] = {
        MMCSD_BUS_WIDTH_8,
        MMCSD_BUS_WIDTH_4,
        MMCSD_BUS_WIDTH_1
    };
    struct rt_mmcsd_host *host = card->host;
    unsigned idx, bus_width = 0;
    int err = 0;

    if (GET_BITS(card->resp_cid, 122, 4) < 4) { // 122 4
        return 0;
    }

    /*
    * Unlike SD, MMC cards dont have a configuration register to notify
    * supported bus width. So bus test command should be run to identify
    * the supported bus width or compare the ext csd values of current
    * bus width and ext csd values of 1 bit mode read earlier.
    */
    for (idx = 0; idx < sizeof(bus_widths) / sizeof(uint32_t); idx++) {
        /*
        * Host is capable of 8bit transfer, then switch
        * the device to work in 8bit transfer mode. If the
        * mmc switch command returns error then switch to
        * 4bit transfer mode. On success set the corresponding
        * bus width on the host.
        */
        err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, ext_csd_bits[idx]);
        if (err) {
            continue;
        }

        bus_width = bus_widths[idx];
        mmcsd_set_bus_width(host, bus_width);
        uapi_tcxo_delay_ms(20); // delay 20ms
        err = mmc_compare_ext_csds(card, ext_csd, bus_width);
        if (!err) {
            err = bus_width;
            break;
        } else {
            switch (ext_csd_bits[idx]) {
                case 0:
                    LOG_E("switch to bus width 1 bit failed!");
                    break;
                case 1:
                    LOG_E("switch to bus width 4 bit failed!");
                    break;
                case 2: // 2
                    LOG_E("switch to bus width 8 bit failed!");
                    break;
                default:
                    break;
            }
        }
    }
    return err;
}
rt_err_t mmc_send_op_cond(struct rt_mmcsd_host *host, uint32_t ocr, uint32_t *rocr)
{
    struct rt_mmcsd_cmd cmd;
    uint32_t i;
    rt_err_t err = RT_EOK;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SEND_OP_COND;
    cmd.arg = controller_is_spi(host) ? 0 : ocr;
    cmd.flags = RESP_SPI_R1 | RESP_R3 | CMD_BCR;

    for (i = 100; i; i--) { // 100
        err = mmcsd_send_cmd(host, &cmd, 3); // 3 times
        if (err) {
            break;
        }

        /* if we're just probing, do a single pass */
        if (ocr == 0) {
            break;
        }

        /* otherwise wait until reset completes */
        if (controller_is_spi(host)) {
            if (!(cmd.resp[0] & R1_SPI_IDLE)) {
                break;
            }
        } else {
            if (cmd.resp[0] & CARD_BUSY) {
                break;
            }
        }

        err = -RT_ETIMEOUT;
        uapi_tcxo_delay_ms(10); // delay 10ms
    }

    if (rocr && !controller_is_spi(host)) {
        *rocr = cmd.resp[0];
    }
    return err;
}

static rt_err_t mmc_set_card_addr(struct rt_mmcsd_host *host, uint32_t rca)
{
    rt_err_t err;
    struct rt_mmcsd_cmd cmd;

    (void)memset_s(&cmd, sizeof(struct rt_mmcsd_cmd), 0, sizeof(struct rt_mmcsd_cmd));
    cmd.cmd_code = SET_RELATIVE_ADDR;
    cmd.arg = rca << 16; // 16
    cmd.flags = RESP_R1 | CMD_AC;

    err = mmcsd_send_cmd(host, &cmd, 3); // 3 times
    if (err) {
        return err;
    }
    return 0;
}

static void mmc_set_erase_size(struct rt_mmcsd_card *cur_card)
{
    if (cur_card->ext_csd.erase_group_def & 1) {
        cur_card->erase_size = cur_card->ext_csd.hc_erase_size;
    }
}

static int32_t mmc_switch_status(struct rt_mmcsd_card *card)
{
    int32_t err = 0;
    uint32_t status = R1_RSP_UNEXP;

    err = mmc_send_status(card, &status);
    if (err) {
        return err;
    }

    if (status & R1_RSP_UNEXP) {
        LOG_E("unexpected status %#x after switch", status);
    }
    if (status & R1_RSP_BIT7_SWITCH_ERROR) {
        LOG_E("switch status failed");
        return -EBADMSG;
    }
    return 0;
}

static int mmc_select_hs200(struct rt_mmcsd_card *card, uint8_t *ext_csd)
{
    int err = 0;
    struct rt_mmcsd_host *host = card->host;

    mmcsd_select_voltage(host, 0);
    err = mmc_select_bus_width(card, ext_csd);
    if (err >= 0) {
        err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS200);
        if (err) {
            LOG_E("set hs timing failed ret=%d", err);
            return err;
        }
        hi_mmc_set_timing(host, TIMING_MMC_HS200);
        err = mmc_switch_status(card);
    } else {
        LOG_E("hs200 select bus width failed ret=%d", err);
    }
    return err;
}

static int mmc_select_highspeed(struct rt_mmcsd_card *card, uint8_t *ext_csd)
{
    int err = 0;
    struct rt_mmcsd_host *host = card->host;

    err = mmc_select_bus_width(card, ext_csd);
    if (err >= 0) {
        err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, EXT_CSD_TIMING_HS);
        if (err) {
            LOG_E("set highspeed timing failed ret=%d", err);
            return err;
        }
        hi_mmc_set_timing(host, TIMING_MMC_HS);
        err = mmc_switch_status(card);
    } else {
        LOG_E("high speed select bus width failed ret=%d", err);
    }
    return err;
}

static void mmc_set_speed_flag(struct rt_mmcsd_host *host)
{
    struct rt_mmcsd_card *card = host->card;
    host->flags &= ~MMCSD_SUP_HS200;
    host->flags &= ~MMCSD_SUP_HS400;
    host->flags &= ~MMCSD_SUP_HIGHSPEED;

    if (card->ext_csd.card_type == EXT_CSD_CARD_TYPE_HS200_1_8V) {
        host->flags |= MMCSD_SUP_HS200;
    } else if (card->ext_csd.card_type == EXT_CSD_CARD_TYPE_DDR_1_8V) {
        host->flags |= MMCSD_SUP_HIGHSPEED;
    }
    LOG_I("card_type = %x flags = %x", card->ext_csd.card_type, host->flags);
}

static int32_t mmcsd_mmc_init_card(struct rt_mmcsd_host *host, uint32_t ocr)
{
    int32_t err;
    uint32_t resp[4]; // 4
    uint32_t rocr = 0;
    uint32_t max_data_rate;
    uint8_t *ext_csd = RT_NULL;
    struct rt_mmcsd_card *card = RT_NULL;

    mmcsd_go_idle(host);
    /* The extra bit indicates that we support high capacity */
    err = mmc_send_op_cond(host, ocr | (1 << 30), &rocr); // 30
    if (err) {
        goto err;
    }

    if (controller_is_spi(host)) {
        err = mmcsd_spi_use_crc(host, 1);
        if (err) {
            goto err1;
        }
    }

    if (controller_is_spi(host)) {
        err = mmcsd_get_cid(host, resp);
    } else {
        err = mmcsd_all_get_cid(host, resp);
    }

    if (err) {
        goto err;
    }

    card = irmalloc(sizeof(struct rt_mmcsd_card));
    if (!card) {
        LOG_E("malloc card failed!");
        err = -RT_ENOMEM;
        goto err;
    }
    (void)memset_s(card, sizeof(struct rt_mmcsd_card), 0, sizeof(struct rt_mmcsd_card));

    host->card = card;
    card->card_type = CARD_TYPE_MMC;
    card->host = host;
    card->rca = 1;
    (void)memcpy_s(card->resp_cid, sizeof(card->resp_cid), resp, sizeof(card->resp_cid));

    /*
     * For native busses:  get card RCA and quit open drain mode.
     */
    if (!controller_is_spi(host)) {
        err = mmc_set_card_addr(host, card->rca);
        if (err) {
            goto err1;
        }

        mmcsd_set_bus_mode(host, MMCSD_BUSMODE_PUSHPULL);
    }

    err = mmcsd_get_csd(card, card->resp_csd);
    if (err) {
        goto err1;
    }

    err = mmcsd_parse_csd(card);
    if (err) {
        goto err1;
    }

    if (!controller_is_spi(host)) {
        err = mmcsd_select_card(card);
        if (err) {
            goto err1;
        }
    }
    
    /*
    * Fetch and process extended CSD.
    */
    
    err = mmc_get_ext_csd(card, &ext_csd);
    if (err) {
        goto err1;
    }

    err = mmc_parse_ext_csd(card, ext_csd);
    if (err) {
        goto err1;
    }

    mmc_set_erase_size(card);
    if (card->ext_csd.part_config & EXT_CSD_PART_CONFIG_ACC_MASK) {
        card->ext_csd.part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
        err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONFIG, card->ext_csd.part_config);
        if (err && err != -EBADMSG) {
            goto err1;
        }
    }
    mmc_set_speed_flag(host);
    if (host->flags & MMCSD_SUP_HS200) {
        err = mmc_select_hs200(card, ext_csd);
    } else if (host->flags & MMCSD_SUP_HIGHSPEED) {
        err = mmc_select_highspeed(card, ext_csd);
    }
    if (err) {
        goto err1;
    }

    /* If doing byte addressing, check if required to do sector
    * addressing.  Handle the case of <2GB cards needing sector
    * addressing.  See section 8.1 JEDEC Standard JED84-A441;
    * ocr register has bit 30 set for sector addressing.
    */
    if (!(card->flags & CARD_FLAG_SDHC) && (rocr & (1<<30))) {
        card->flags |= CARD_FLAG_SDHC;
    }

    /* set bus speed */
    max_data_rate = host->freq_max;

    if (max_data_rate > card->hs_max_data_rate) {
        max_data_rate = card->hs_max_data_rate;
    }
    LOG_I("flags=%x mdr=%d hsmdr=%d cmdr=%d", host->flags, max_data_rate, card->hs_max_data_rate, card->max_data_rate);
    mmcsd_set_clock(host, max_data_rate);
    if (host->flags & MMCSD_SUP_HS200) {
        err = hi_mmc_exec_tuning(card->host, SEND_TUNING_BLOCK_HS200);
        if (err) {
            LOG_W("tuning execution failed\n");
            goto err1;
        }
        /* switch bus width */
        mmc_select_bus_width(card, ext_csd);
    } else {
        /* switch bus width */
        mmc_select_bus_width(card, ext_csd);
    }
    host->card = card;
    LOG_I("emmc init finish success clock=%d\n", max_data_rate);
    irfree(ext_csd);
    return 0;

err1:
    irfree(card);
err:

    return err;
}

/*
 * Starting point for mmc card init.
 */
int32_t init_mmc(struct rt_mmcsd_host *host, uint32_t ocr)
{
    int32_t err;
    uint32_t  current_ocr;
    /*
     * We need to get OCR a different way for SPI.
     */

    if (controller_is_spi(host)) {
        err = mmcsd_spi_read_ocr(host, 0, &ocr);
        if (err) {
            goto err;
        }
    }

    current_ocr = mmcsd_select_voltage(host, ocr);
    if (!current_ocr) {
        err = -RT_ERROR;
        goto err;
    }

    /*
     * Detect and init the card.
     */
    err = mmcsd_mmc_init_card(host, current_ocr);
    if (err) {
        goto err;
    }

    mmcsd_host_unlock(host);

    err = rt_mmcsd_blk_probe(host->card);
    if (err) {
        goto remove_card;
    }

    mmcsd_host_lock(host);
    return 0;

remove_card:
    mmcsd_host_lock(host);
    rt_mmcsd_blk_remove(host->card);
    irfree(host->card);
    host->card = RT_NULL;

err:
    LOG_E("init MMC card failed!");
    return err;
}

int32_t deinit_mmc_free_mem(struct rt_mmcsd_host *host)
{
    if (host == RT_NULL || host->card == RT_NULL) {
        return -RT_EINVAL;
    }
    irfree(host->card);
    host->card = RT_NULL;
    return 0;
}