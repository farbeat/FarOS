/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Sdhci Runstop Implementation
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

#include "soc_mmc.h"
#include "mmc_type.h"
#include "host.h"
#include "hci.h"
#include "sdhci_private.h"
#ifdef DPALCFG_DRIVERS_SDIO
#include "sdio_reg.h"
#include "sdio_ops.h"
#endif
#include "sdhci.h"
#include "dpal.h"

int mmc_host_suspend(const void *data)
{
    int mmc_num;
    int i;
    struct hci_softc *sc;
    struct sdhci_slot *slot;
    bool mmc_need_suspend;
    device_t dev;
    const struct mmc_suspend_resume *mmc_data = (struct mmc_suspend_resume *)data;
    if (mmc_data == NULL) {
        mmc_num = MAX_MMC_NUM;
    } else {
        mmc_num = DPAL_MIN(mmc_data->mmc_max_num, MAX_MMC_NUM);
    }

    for (i = 0; i < mmc_num; i++) {
        mmc_need_suspend = 0;
        sc = get_mmc_resource(i);
        if (sc == NULL) {
            continue;
        }
        slot = (struct sdhci_slot *)sc->slot;
        if (slot == NULL) {
            continue;
        }
        SDHCI_LOCK(slot);
        dev = slot->dev;
        if (mmc_data != NULL) {
            if (mmc_data->flags[i] == MMC_DO_DETECT) {
                mmc_need_suspend = 1;
            }
        } else {
            if ((dev != NULL) && (slot->host.mode != mode_sdio)) {
                mmc_need_suspend = 1;
            }
        }
        SDHCI_UNLOCK(slot);
        if ((dev != NULL) && (mmc_need_suspend == 1)) {
            sdhci_card_detect(sc, HCI_EVENT_UNPLUGGED);
        }
    }
    dev_logd(slot->bus, "sdhci is suspend.\n");
    return (0);
}

int mmc_host_resume(const void *data)
{
    int mmc_num;
    int i;
    struct hci_softc *sc = NULL;
    struct sdhci_slot *slot = NULL;
    bool mmc_need_resume;
    device_t dev = NULL;
    const struct mmc_suspend_resume *mmc_data = (struct mmc_suspend_resume *)data;
    if (mmc_data == NULL) {
        mmc_num = MAX_MMC_NUM;
    } else {
        mmc_num = DPAL_MIN(mmc_data->mmc_max_num, MAX_MMC_NUM);
    }

    for (i = 0; i < mmc_num; i++) {
        mmc_need_resume = 1;
        sc = get_mmc_resource(i);
        if (sc == NULL) {
            continue;
        }
        slot = (struct sdhci_slot *)sc->slot;
        if (slot == NULL) {
            continue;
        }
        SDHCI_LOCK(slot);
        dev = slot->dev;
        if (mmc_data != NULL) {
            if (mmc_data->flags[i] == MMC_UNDO_DETECT) {
                mmc_need_resume = 0;
            }
        } else {
            if ((dev != NULL) && (slot->host.mode == mode_sdio)) {
                mmc_need_resume = 0;
            }
        }

        if (dev == NULL) {
            mmc_need_resume = 1;
        }
        SDHCI_UNLOCK(slot);
        if (mmc_need_resume) {
            sdhci_card_detect(sc, HCI_EVENT_PLUGGED);
        }
    }
    dev_logd(slot->bus, "sdhci is resume.\n");
    return (0);
}

#ifdef DPALCFG_DRIVERS_SDIO
int sdio_rescan(int index)
{
    struct sdhci_slot *slot = NULL;
    device_t card = NULL;
    struct hci_softc *sc = get_mmc_resource(index);
    int error;
    uint8_t val;

    if (sc == NULL || !sc->sdio_rescan_allow) {
        return (-EINVAL);
    }

    slot = sc->slot;
    if (slot == NULL) {
        return (-EINVAL);
    }

    card = get_any_card(sc->dev);
    if (card != NULL && slot->host.card_present == MMC_CAP_CARD_PRESENT) {
        if (slot->host.mode == mode_sdio) {
            error = sdio_rw_direct_acqr_bus(slot->dev, card, 0, SD_IO_CCCR_FN_ENABLE, false, &val);
            if (error != 0) {
                sdhci_card_detect(sc, HCI_EVENT_UNPLUGGED);
                sdhci_card_detect(sc, HCI_EVENT_PLUGGED);
            } else {
                dev_logd(slot->bus, "card found in slot %d val=0x%x\n", index, val);
                return (0);
            }
        } else {
            dev_logd(slot->bus, "sd/mmc found in host:%d, rescan not support.\n", index);
        }
    } else {
        sdhci_card_detect(sc, HCI_EVENT_PLUGGED);
    }
    card = get_any_card(sc->dev);
    if (card == NULL) {
        return (-EINVAL);
    }
    return (0);
}
#endif
