/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Sdhci Tune Implementation
 * Author: @CompanyNameTag
 * Create: 2019-06-23
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
#include "sdhci.h"
#include "sdhci_private.h"
#include "hci_helper.h"

#define SAMP_PHASE_DEF        3
#define TUNE_EDGE_MAX        8 /* Active phase scale: 0 ~ TUNE_EDGE_PHASES-1 */
#define TUNE_EDGE_NOT_FUND    TUNE_EDGE_MAX

typedef struct {
    uint32_t left;
    uint32_t right;
    uint32_t phase;
} tune_result;

static const uint32_t tune_blk_4bits[] = {
    0x00ff0fff, 0xccc3ccff, 0xffcc3cc3, 0xeffefffe,
    0xddffdfff, 0xfbfffbff, 0xff7fffbf, 0xefbdf777,
    0xf0fff0ff, 0x3cccfc0f, 0xcfcc33cc, 0xeeffefff,
    0xfdfffdff, 0xffbfffdf, 0xfff7ffbb, 0xde7b7ff7
};

static const uint32_t tune_blk_8bits[] = {
    0xff00ffff, 0x0000ffff, 0xccccffff, 0xcccc33cc,
    0xcc3333cc, 0xffffcccc, 0xffffeeff, 0xffeeeeff,
    0xffddffff, 0xddddffff, 0xbbffffff, 0xbbffffff,
    0xffffffbb, 0xffffff77, 0x77ff7777, 0xffeeddbb,
    0x00ffffff, 0x00ffffff, 0xccffff00, 0xcc33cccc,
    0x3333cccc, 0xffcccccc, 0xffeeffff, 0xeeeeffff,
    0xddffffff, 0xddffffff, 0xffffffdd, 0xffffffbb,
    0xffffbbbb, 0xffff77ff, 0xff7777ff, 0xeeddbb77
};

static int tune_basic(struct sdhci_slot *slot, tune_result *t)
{
#define SDHCI_TUNINT_REQ_TIMEOUT  (DPALCFG_BASE_CORE_TICK_PER_SECOND / 5)
    uint32_t i;
    int tune_result;
    uint32_t ret;
    uint32_t reg;
    uint16_t host_ctrl2;
    uint32_t opcode;
    uint32_t success_idx = 0;
    uint32_t succeeded_first_phase = TUNE_EDGE_NOT_FUND;
    uint32_t fail_index = 0;
    uint32_t failed_first_phase = TUNE_EDGE_NOT_FUND;
    struct hci_softc *sc = device_get_softc(slot->bus);
    struct mmc_data *tune_data;
    struct mmc_request *tune_req;
    struct mmc_command *tune_cmd;
    uint8_t tune_data_buf[MMC_TUNING_LEN_HS200];
    const void *cmp_data_buf = NULL;
    struct mmc_ios *ios = &slot->host.ios;

    WR4(slot, SDHCI_INT_ENABLE, slot->intmask | SDHCI_INT_DATA_AVAIL);
    WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask | SDHCI_INT_DATA_AVAIL);
    host_ctrl2 = RD2(slot, SDHCI_HOST_CONTROL2);
    host_ctrl2 &= ~SDHCI_CTRL2_SAMPLING_CLOCK;
    WR2(slot, SDHCI_HOST_CONTROL2, host_ctrl2);

    reg = RD4(slot, SDHCI_AUTO_TUNING_CTRL);
    reg |= SDHCI_SW_TUNING_EN;
    WR4(slot, SDHCI_AUTO_TUNING_CTRL, reg);

    for (i = 0; i < TUNE_EDGE_MAX; i++) {
        if (slot->ops->select_sample_phase != NULL) {
            slot->ops->select_sample_phase(sc, i);
        }

        tune_cmd = slot->tune_cmd;
        opcode = tune_cmd->opcode;
        (void)memset_s(tune_cmd, sizeof(*tune_cmd), 0, sizeof(*tune_cmd));
        tune_cmd->opcode = opcode;
        tune_cmd->flags = MMC_RSP_R1 | MMC_CMD_ADTC;

        tune_data = slot->tune_data;
        tune_cmd->data = slot->tune_data;
        (void)memset_s(tune_data, sizeof(*tune_data), 0, sizeof(*tune_data));

        tune_data->len = (opcode == MMC_SEND_TUNING_BLOCK_HS200 &&
            ios->bus_width == bus_width_8) ? MMC_TUNING_LEN_HS200 :
            MMC_TUNING_LEN;
        cmp_data_buf = (opcode == MMC_SEND_TUNING_BLOCK_HS200 &&
            ios->bus_width == bus_width_8) ? (const void *)tune_blk_8bits :
            (const void *)tune_blk_4bits;

        tune_data->data = (void *)tune_data_buf;
        tune_data->flags = MMC_DATA_READ;

        tune_req = slot->tune_req;
        (void)memset_s(tune_req, sizeof(*tune_req), 0, sizeof(*tune_req));
        tune_req->cmd = tune_cmd;
        tune_req->done = sdhci_req_wakeup;
        tune_req->done_data = slot;

        slot->req = tune_req;
        slot->flags = 0;
        tune_result = 0;

        sdhci_start(slot);

        ret = dpal_event_read(&slot->req_event, SDHCI_ACQUIRE_HOST,
            DPAL_WAITMODE_AND | DPAL_WAITMODE_CLR, SDHCI_TUNINT_REQ_TIMEOUT);
        if (ret == DPAL_ERRNO_EVENT_READ_TIMEOUT) {
            tune_cmd->error = MMC_ERR_NO_REP;
            slot_printf(slot, "tuning timeout\n");
            sdhci_dump_regs(slot);
            sdhci_reset(slot, SDHCI_RESET_CMD | SDHCI_RESET_DATA);
            slot->req = NULL;
        }

        if (tune_cmd->error != 0) {
            tune_result = tune_cmd->error;
        }

        if (memcmp(cmp_data_buf, (void *)tune_data_buf, tune_data->len) != 0) {
            tune_result = -EIO;
        }

        dev_logt(slot->bus, "tune_result %d\n", tune_result);
        if (tune_result == 0) {
            if (success_idx == 0) {
                succeeded_first_phase = i;
            }
            success_idx++;
        } else {
            fail_index++;
            if ((fail_index != 0 && success_idx != 0) && succeeded_first_phase < i) {
                failed_first_phase = i;
                break;
            }
        }
    }

    if (success_idx > 0) {
        t->left = succeeded_first_phase;
        t->right = failed_first_phase - 1;
        t->phase = (t->left + t->right) >> 1 ;
        return (0);
    } else {
        return (-1);
    }
}

int tune_edge(device_t brdev, device_t reqdev)
{
    int status;
    uint16_t ctrl;
    tune_result t = {0};
    struct sdhci_slot *slot = device_get_ivars(reqdev);
    struct hci_softc *sc = device_get_softc(slot->bus);
    struct mmc_ios *ios = &slot->host.ios;
    if ((slot->opt & SDHCI_TUNING_SUPPORTED) == 0) {
        return (0);
    }

    SDHCI_LOCK(slot);
    slot->retune_ticks = slot->retune_count * SDHCI_HZ;

    /* Tell the host it's tuning failed by default */
    slot->opt &= ~SDHCI_TUNING_ENABLED;
    slot->tune_cmd->opcode = MMC_SEND_TUNING_BLOCK;
    switch (ios->timing) {
        case bus_timing_mmc_hs400:
            slot_printf(slot, "HS400 must be tuned in HS200 mode\n");
            SDHCI_UNLOCK(slot);
            return (EINVAL);
        case bus_timing_mmc_hs200:
            slot->tune_cmd->opcode = MMC_SEND_TUNING_BLOCK_HS200;
            break;
        case bus_timing_uhs_ddr50:
        case bus_timing_uhs_sdr104:
            break;
        case bus_timing_uhs_sdr50:
            if ((slot->opt & SDHCI_SDR50_NEEDS_TUNING) != 0) {
                break;
            }
            /* FALLTHROUGH, no break */
        default:
            SDHCI_UNLOCK(slot);
            return (0);
    }

    status = tune_basic(slot, &t);
    if (status < 0) {
        dev_loge(slot->bus, "Tuning failed, using fixed sampling clock\n");
        sc->tuning_phase = SAMP_PHASE_DEF;
    } else {
        dev_loge(slot->bus, "tuning done, phase[%u, %u] final:%u\n", t.left, t.right, t.phase);
        sc->tuning_phase = t.phase;
    }

    /* Tell the host it's tuning successful, we can do it in the future */
    slot->opt |= SDHCI_TUNING_ENABLED;

    if (slot->ops->select_sample_phase != NULL) {
        slot->ops->select_sample_phase(sc, sc->tuning_phase);
    } else {
        dev_loge(slot->bus, "cannot set tune, post tune call back is missing\n");
    }

    ctrl = RD2(slot, SDHCI_HOST_CONTROL2);
    ctrl |= SDHCI_CTRL2_SAMPLING_CLOCK; /* 0: fixed clock, 1: tuned clock for samling */
    WR2(slot, SDHCI_HOST_CONTROL2, ctrl);

    /*
     * Maybe SDHCI_INT_RESPONSE has been cleared by function sdhci_start_command,
     * so we restore interrupts here
     */
    dev_logt(slot->bus, "slot->intmask:0x%x, ctrl:0x%x,", slot->intmask, ctrl);
    slot->intmask |= (SDHCI_INT_RESPONSE | SDHCI_INT_DMA_END);
    WR4(slot, SDHCI_INT_ENABLE, slot->intmask);
    WR4(slot, SDHCI_SIGNAL_ENABLE, slot->intmask);

    SDHCI_UNLOCK(slot);
    return (status);
}
