/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 * Description: LiteOS MMC Block Implementation
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

#include "disk.h"
#include "mmcsd.h"
#include "mmc_adapt.h"
#include "dpal.h"
#include "mmcbrvar.h"
#include "mmcvar.h"
#include "block_private.h"
#include "mmc_private.h"
#include "mmcreg.h"
#include "pm_veto.h"

static uint32_t part_node_cnt = 0;
#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
static bool rw_in_exception = false;
static dpal_spinlock rw_spinlock;
#endif

/* Block ioctl */
#define    MMC_CARD_AU_SIZE    0x2 /* Get card au size, in sector */
#define    MAX_PART_COUNT    32
#define BLOCK_MSLEEP10 10

struct mmcsd_part *g_local_part[MAX_PART_COUNT];
static void mmc_init_before_handler(void)
{
    if (!mmc_get_sleep_init_flag() && mmc_get_first_init_flag()) {
        emmc_recovery_init();
        mmc_set_sleep_init_flag(true);
    }
}

static inline void set_local_part_node(struct mmcsd_part *part, int index)
{
    g_local_part[index] = part;
}

static inline void clear_local_part_node_cnt(uint32_t index)
{
    g_local_part[index] = NULL;
}

static int get_part_node_cnt(void)
{
    uint32_t i;
    for (i = 0; i < MAX_PART_COUNT; i++) {
        if (mmc_isclr(&part_node_cnt, i)) {
            mmc_setbit(&part_node_cnt, i);
            return ((int)i);
        }
    }
    mmc_err("part number:%u run out!\n", part_node_cnt);
    return (-1);
}

static void clear_part_node_cnt(uint32_t bit)
{
    if (bit < MAX_PART_COUNT) {
        mmc_clrbit(&part_node_cnt, bit);
    } else {
        mmc_err("part number error:%u!\n", part_node_cnt);
    }
}

void deinit_part(struct mmcsd_part *part)
{
    MMCSD_DISK_LOCK_DESTROY(part);
    mmc_clrbit(&part_node_cnt, part->cnt);
    clear_local_part_node_cnt(part->cnt);
    if (part->buffer != NULL) {
        free(part->buffer);
        part->buffer = NULL;
    }
    free(part);
    return;
}

static uint32_t bio_start(struct mmcsd_part *part, struct bio *bp)
{
    uint32_t ret;

    dpal_dma_cache_flush(0, 0);
    dev_logt(part->sc->dev, "wait part->disk_mtx\n");
    MMCSD_DISK_LOCK(part);
    dev_logt(part->sc->dev, "got part->disk_mtx\n");
    if (part->bio_queue_state == BIO_QUEUE_NORMAL) {
        if (bp->bio_cmd == BIO_CMD0) {
            ret = dpal_queue_write_head(part->bio_queue_id, bp, sizeof(char *), DPAL_WAIT_FOREVER);
        } else {
            ret = dpal_queue_write(part->bio_queue_id, bp, sizeof(char *), DPAL_WAIT_FOREVER);
        }
        MMCSD_DISK_UNLOCK(part);
        if (ret != 0) {
            return (0);
        }
    } else {
        MMCSD_DISK_UNLOCK(part);
        return (0);
    }

    dev_logt(part->sc->dev, "wait for the collection of data.\n");
    ret = dpal_event_read(&bp->wait_event, REQUEST_EVENT_ANSWER,
        (DPAL_WAITMODE_OR | DPAL_WAITMODE_CLR), DPAL_WAIT_FOREVER);
    dpal_dma_cache_flush(0, 0);
    dev_logt(part->sc->dev, "got bp->wait_event and the data is collected.ret:%u\n", ret);
    if ((bp->bio_flags & BIO_ERROR) != 0) {
        dev_logt(part->sc->dev, "cmd:%u, wait event fail! ret = %u, (cmd %d: mmc_read, %d:mmc_write)\n",
                 bp->bio_cmd, ret, BIO_READ, BIO_WRITE);
        return (0);
    }
    if (bp->bio_error != 0) {
        dev_logt(part->sc->dev, "bp->sector_start = 0x%x, bp->sectorsize = 0x%x, bp->bio_flags = 0x%x\n",
                 bp->sector_start, bp->sectorsize, bp->bio_flags);
        part->errot_cnt++;
        return (0);
    }

    return (bp->numsectors);
}

ssize_t mmcsd_exit(struct mmcsd_part *part)
{
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    uint32_t ret;

    part->part_status = PART_ABNORMAL;
    while (1) {
        if (dpal_atomic_read(&part->atom_reqs) == 0) {
            break;
        } else {
            mmc_delay_ms(BLOCK_MSLEEP10);
        }
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));
    (void)dpal_event_init(&bp->wait_event);

    bp->bio_cmd |= BIO_CMD0;
    bp->numsectors = 0;
    ret = bio_start(part, bp);
    if (ret != 0) {
        dev_logd(part->sc->dev, "mmcsd_exit %u\n", ret);
    }
    (void)dpal_event_destroy(&bp->wait_event);
    return ((ssize_t)ret);
}

static int mmcsd_open(FAR struct inode *inode)
{
    (void)inode;
#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
    dpal_spin_lock_init(&rw_spinlock);
#endif
    return (0);
}

static int mmcsd_close(FAR struct inode *inode)
{
    (void)inode;
    return (0);
}

static ssize_t mmcsd_read(FAR struct inode *inode, FAR u_char *buffer,
    unsigned long long start_sector, uint32_t nsectors)
{
    struct mmcsd_part *part = (struct mmcsd_part *)(inode->i_private);
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    int ret;

    if (part == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }
    mmc_init_before_handler();
    uapi_pm_add_sleep_veto(PM_ID_STORAGE);
    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        ret = -EIO;
        goto out;
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));
    (void)dpal_event_init(&(bp->wait_event));

    bp->bio_cmd |= BIO_READ;
    bp->sector_start = (unsigned long)start_sector;
    bp->numsectors = nsectors;
    bp->sectorsize = MMC_CARDBUS_BLOCK_SIZE;
    bp->bio_data = buffer;

    ret = (ssize_t)bio_start(part, bp);
    (void)dpal_event_destroy(&bp->wait_event);
out:
    dpal_atomic_dec(&part->atom_reqs);
    uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
    return ((ret == 0) ? -EAGAIN : (ssize_t)ret);
}

static ssize_t mmcsd_write(FAR struct inode *inode, FAR const u_char *buffer,
    unsigned long long start_sector, uint32_t nsectors)
{
    struct mmcsd_part *part = (struct mmcsd_part *)(inode->i_private);
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    int ret;

    if (part == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }
    mmc_init_before_handler();
    uapi_pm_add_sleep_veto(PM_ID_STORAGE);
    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        ret = -EIO;
        goto out;
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));
    (void)dpal_event_init(&(bp->wait_event));

    bp->bio_cmd |= BIO_WRITE;
    bp->sector_start = (unsigned long)start_sector;
    bp->numsectors = nsectors;
    bp->sectorsize = MMC_CARDBUS_BLOCK_SIZE;
    bp->bio_data = (u_char *)buffer;

    ret = (ssize_t)bio_start(part, bp);
    (void)dpal_event_destroy(&bp->wait_event);
out:
    dpal_atomic_dec(&part->atom_reqs);
    uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
    return ((ret == 0) ? -EAGAIN : (ssize_t)ret);
}

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
static ssize_t
mmcsd_operate_in_irq(FAR struct inode *inode, FAR u_char *buffer,
    unsigned long long start_sector, uint32_t nsectors, bool wr)
{
    struct mmcsd_part *part = (struct mmcsd_part *)(inode->i_private);
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    struct mmcsd_softc *sc;
    unsigned long end;
    ssize_t ret = 0;

    if (part == NULL || part->sc == NULL || part->sc->mmcbus == NULL || part->sc->dev == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }

    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        ret = -EIO;
        goto out;
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));

    bp->in_exc = true;
    bp->bio_cmd |= wr ? BIO_WRITE : BIO_READ;
    bp->sector_start = (unsigned long)start_sector;
    bp->numsectors = nsectors;
    bp->sectorsize = MMC_CARDBUS_BLOCK_SIZE;
    bp->bio_data = (u_char *)buffer;

    sc = part->sc;
    MMCBUS_ACQUIRE_BUS(sc->mmcbus, sc->dev);
    end = mmcsd_rw(part, bp);
    if (bp->sector_start + bp->numsectors != end) {
        ret = -EIO;
    }
    MMCBUS_RELEASE_BUS(sc->mmcbus, sc->dev);

out:
    dpal_atomic_dec(&part->atom_reqs);
    return ((ret < 0) ? ret : bp->numsectors);
}
#endif

#ifdef LOSCFG_MMCSD_SG_SUPPORT
/* SG_MAX_CNT >= max transfer sector nums(default 4096) / CONFIG_FS_FAT_SECTOR_PER_BLOCK (vfs_config.h) */
#define SG_MAX_CNT  32
ssize_t mmcsd_writev(FAR struct inode *inode, FAR const struct iovec *iov, uint32_t cnt,
    unsigned long long start_sector)
{
    struct mmcsd_part *part = (struct mmcsd_part *)inode->i_private;
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    int ret;
    uint32_t nsectors = 0;
    struct mmc_sg sg[SG_MAX_CNT];

    if (part == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }

    if (cnt > SG_MAX_CNT) {
        mmc_err("iov cnt cannot be greater than %d\n", SG_MAX_CNT);
        return (-EINVAL);
    }

    for (uint32_t i = 0; i < cnt; i++) {
        if (iov[i].iov_base == NULL) {
            return (-EINVAL);
        }
        /* For segmented data, ensure that the each segment is cache aligned. */
        if ((uintptr_t)iov[i].iov_base & (CACHE_ALIGNED_SIZE - 1)) {
            mmc_err("segment data address mem is not cache(%d byte) aligned\n", CACHE_ALIGNED_SIZE);
            return (-EINVAL);
        }

        nsectors += iov[i].iov_len;
        sg[i].data = iov[i].iov_base;
        sg[i].length = (iov[i].iov_len << 9); /* every sector size is 512 (1 << 9) */
    }

    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        ret = -EIO;
        goto out;
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));
    (void)dpal_event_init(&(bp->wait_event));

    bp->bio_cmd |= BIO_WRITE;
    bp->sector_start = start_sector;
    bp->numsectors = nsectors;
    bp->sectorsize = MMC_CARDBUS_BLOCK_SIZE;
    bp->sgs = sg;
    bp->sg_count = cnt;
    bp->sg_mode = true;

    ret = bio_start(part, bp);
    (void)dpal_event_destroy(&bp->wait_event);
out:
    dpal_atomic_dec(&part->atom_reqs);
    return ((ret == 0) ? -EAGAIN : (ssize_t)ret);
}
#endif

static int mmcsd_geometry(FAR struct inode *inode, FAR struct geometry *geometry)
{
    struct mmcsd_part *part = (struct mmcsd_part *)inode->i_private;
    struct mmcsd_softc *sc;
    device_t dev;

    if (geometry == NULL) {
        return (-EINVAL);
    }

    if (part == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }
    sc = part->sc;
    dev = sc->dev;
    mmc_init_before_handler();
    uapi_pm_add_sleep_veto(PM_ID_STORAGE);
    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        dpal_atomic_dec(&part->atom_reqs);
        uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
        return (-EIO);
    }

    geometry->geo_available = true;
    geometry->geo_mediachanged = false;
    geometry->geo_writeenabled = true;

    if (mmc_get_card_type(dev) == mode_sd || mmc_get_card_type(dev) == mode_mmc) {
        geometry->geo_nsectors = mmc_get_media_size(dev);
    }
    geometry->geo_sectorsize = (size_t)mmc_get_sector_size(dev);
    dpal_atomic_dec(&part->atom_reqs);
    uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
    return (ENOERR);
}

/*
 * According Part 1: Physical Layer Specification Version3.00,
 * AU respresnts allocation unit which consists of one or mode blocks.
 * While nowdays one physical block may larger than AU size, while there's no mothod
 * to obtain the real block size.
 */

#define SC_1K    (0x00000400 / 512) /* sector count of 1k */
#define SC_1M    (0x00100000 / 512) /* sector count of 1M */

static const uint32_t au_size[] = {
              0,  16 * SC_1K,  32 * SC_1K, 64 * SC_1K,
    128 * SC_1K, 256 * SC_1K, 512 * SC_1K,  1 * SC_1M,
      2 * SC_1M,   4 * SC_1M,   8 * SC_1M, 12 * SC_1M,
     16 * SC_1M,  24 * SC_1M,  32 * SC_1M, 64 * SC_1M
};

static int mmcsd_ioctl(FAR struct inode *inode, int cmd, unsigned long arg)
{
    struct mmcsd_part *part = (struct mmcsd_part *)inode->i_private;
    struct mmcsd_softc *sc = part->sc;
    device_t dev = sc->dev;
    struct mmc_ivars *ivar = device_get_ivars(dev);
    uint32_t au_reg_value;

    mmc_init_before_handler();
    uapi_pm_add_sleep_veto(PM_ID_STORAGE);
    dpal_atomic_inc(&part->atom_reqs);
    if (part->part_status != PART_NORMAL) {
        dpal_atomic_dec(&part->atom_reqs);
        uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
        return (-EIO);
    }

    switch (cmd) {
    case MMC_CARD_AU_SIZE:
        au_reg_value = mmc_get_au_size(dev);
        if (au_reg_value > 9 && ivar->scr.sda_spec3 == 0) {
            au_reg_value = 9;
            dev_logw(dev, "spec2 does not support > 4MB au size, overitten to 4MB\n");
        }
        *(uint32_t *)arg = au_size[au_reg_value];
        break;
    default:
        break;
    }
    dpal_atomic_dec(&part->atom_reqs);
    uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
    return (ENOERR);
}

const static struct block_operations mmc_sd_ops = {
    .open = mmcsd_open,
    .close = mmcsd_close,
    .read = mmcsd_read,
    .write = mmcsd_write,
#ifdef LOSCFG_MMCSD_SG_SUPPORT
    .writev = mmcsd_writev,
#endif
    .geometry = mmcsd_geometry,
    .ioctl = mmcsd_ioctl,
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
    .unlink = NULL
#endif
};

const struct block_operations *mmcsd_block_get_bops(void)
{
    return &mmc_sd_ops;
}

struct disk_divide_info emmc = {.sector_count = 0xffffffff};

int mmcsd_part_ops_register(struct mmcsd_part *part, uint32_t card_mode)
{
    size_t name_size;
    int ret = 0;
    int disk_id;
    int cnt;
    struct disk_divide_info *info = ((card_mode == mode_sd) ? NULL : &emmc);
    if (info != NULL) {
        info->sector_count = part->card_capacity;
    }

    cnt = get_part_node_cnt();
    if (cnt < 0) {
        return (-ENOMEM);
    }
    set_local_part_node(part, cnt);
    part->cnt = (uint32_t)cnt;

    name_size = sizeof(part->name);
    ret = snprintf_s(part->name, name_size, name_size - 1, "/dev/mmcblk%0u", part->cnt);
    if (ret <= 0) {
        clear_part_node_cnt(part->cnt);
        clear_local_part_node_cnt(part->cnt);
        mmc_err("snprintf_s err=%d, part_no=%u\n", ret, part->cnt);
        return (-ENOMEM);
    }

    disk_id = los_alloc_diskid_byname(part->name);
    if (disk_id < 0) {
        clear_part_node_cnt(part->cnt);
        clear_local_part_node_cnt(part->cnt);
        dev_loge(part->sc->dev, "alloc disk id failed!\n");
        return (-EXDEV);
    }

    ret = los_disk_init(part->name, &mmc_sd_ops, (void *)part, disk_id, (void *)info);
    if (ret != ENOERR) {
        clear_part_node_cnt(part->cnt);
        clear_local_part_node_cnt(part->cnt);
        dev_loge(part->sc->dev, "disk init failure!\n");
    }
    return (ret);
}

ssize_t mmcsd_erase(FAR struct inode *inode, size_t start_sector, uint32_t nsectors)
{
#define    CONFIG_DEF_ERASE_SECTOR    0x100000U
    struct mmcsd_part *part;
    uint32_t sector_total = nsectors;
    device_t dev;
    struct bio qbp = {0};
    struct bio *bp = &qbp;
    ssize_t ret = 0;

    if (inode->u.i_bops != &mmc_sd_ops) {
        return (-EINVAL);
    }

    part = (struct mmcsd_part *)(inode->i_private);
    if (part == NULL) {
        mmc_err("failed to find mmc device!\n");
        return (-EINVAL);
    }

    dev = part->sc->dev;
    dpal_atomic_inc(&part->atom_reqs);
    if (mmc_get_card_present(dev) == MMC_CAP_CARD_ABSENT) {
        mmc_err("card is absent!\n");
        ret = -EIO;
        goto out;
    }

    if (part->part_status != PART_NORMAL) {
        ret = -EIO;
        goto out;
    }

    (void)memset_s(bp, sizeof(struct bio), 0x0, sizeof(struct bio));
    (void)dpal_event_init(&(bp->wait_event));

    bp->bio_cmd |= BIO_DELETE;
    bp->sectorsize = MMC_CARDBUS_BLOCK_SIZE;

    while (sector_total > 0) {
        if (sector_total > CONFIG_DEF_ERASE_SECTOR) {
            bp->sector_start = start_sector;
            bp->numsectors = CONFIG_DEF_ERASE_SECTOR;
        } else {
            bp->sector_start = start_sector;
            bp->numsectors = sector_total;
        }

        bp->bio_data = NULL;
        if (bio_start(part, bp) != bp->numsectors) {
            (void)dpal_event_destroy(&bp->wait_event);
            ret = -EIO;
            goto out;
        }

        sector_total -= bp->numsectors;
        start_sector += bp->numsectors;
    }
    (void)dpal_event_destroy(&bp->wait_event);
out:
    dpal_atomic_dec(&part->atom_reqs);
    return (ret);
}

#define    MMC_WRITE_FLAG    1
#define    MMC_READ_FLAG    0

static int buffer_valid_check(const char *buffer)
{
    if (buffer == NULL || ((uintptr_t)buffer & (CACHE_ALIGNED_SIZE - 1)) != 0) {
        mmc_err("buffer should be not NULL and cache align\n");
        return (-EINVAL);
    }
    return (0);
}

static ssize_t sector_rw(int index, char *buffer, size_t start_sector, uint32_t nsectors, int rw)
{
    struct mmcsd_part *part;
    struct inode local_node;
    ssize_t ret;

    part = g_local_part[index];
    if (part == NULL) {
        mmc_err("no device!\n");
        return (0);
    }

    local_node.i_private = part;

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
    if (rw_in_exception) {
        return (mmcsd_operate_in_irq(&local_node, (FAR u_char *)buffer, start_sector, nsectors, rw));
    }
#endif

    if (rw == MMC_READ_FLAG) {
        ret = mmcsd_read(&local_node, (FAR u_char *)buffer, start_sector, nsectors);
    } else {
        ret = mmcsd_write(&local_node, (FAR u_char *)buffer, start_sector, nsectors);
    }
    return (ret);
}

static ssize_t mmc_rw(int index, char *buffer, uint32_t start_sector, uint32_t nsectors, int flags)
{
#define    MMC_RAW_WRITE_MAX_SECTOR    2048
    uint32_t sector_now = nsectors;
    uint32_t sector_count;
    ssize_t ret;
    mmc_init_before_handler();
    uapi_pm_add_sleep_veto(PM_ID_STORAGE);
    do {
        sector_count = (sector_now >= MMC_RAW_WRITE_MAX_SECTOR) ? MMC_RAW_WRITE_MAX_SECTOR : sector_now;
        ret = sector_rw(index, buffer, start_sector, sector_count, flags);
        if (ret != (ssize_t)sector_count) {
            uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
            return (ret);
        }

        sector_now -= sector_count;
        start_sector += sector_count;
        buffer += sector_count << 9; /* every sector size is 512 (1 << 9) */
    } while (sector_now != 0);
    uapi_pm_remove_sleep_veto(PM_ID_STORAGE);
    return ((ssize_t)nsectors);
}

static int get_emmc_part_node_cnt(void)
{
    struct mmcsd_part *part;
    struct mmcsd_softc *sc;
    device_t mmcbus;
    int i;

    for (i = 0; i < MAX_PART_COUNT; i++) {
        part = g_local_part[i];
        if (part == NULL) {
            continue;
        }

        mmcbus = part->sc->mmcbus;
        sc = device_get_softc(mmcbus);
        if (mmcbr_get_mode(sc->dev) == mode_mmc) {
            break;
        }
    }

    if (i == MAX_PART_COUNT) {
        mmc_err("No eMMC found!\n");
        return (-1);
    }

    return (i);
}

static uint32_t emmc_raw_rw(char *buffer, uint32_t start_sector, uint32_t nsectors, int flags)
{
    int index;
    if (buffer_valid_check(buffer) < 0) {
        return (0);
    }

    index = get_emmc_part_node_cnt();
    if (index < 0) {
        return (0);
    }

    return ((uint32_t)mmc_rw(index, buffer, start_sector, nsectors, flags));
}
uint32_t emmc_raw_write(char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return (emmc_raw_rw(buffer, start_sector, nsectors, MMC_WRITE_FLAG));
}

uint32_t emmc_raw_read(char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return (emmc_raw_rw(buffer, start_sector, nsectors, MMC_READ_FLAG));
}

static bool mmc_direct_rw_check(uint32_t host_idx, const char *buffer, int *idx)
{
    struct mmcsd_part *part;
    struct mmcsd_softc *sc;
    device_t mmcbus;
    int i;

    if (buffer_valid_check(buffer) < 0) {
        return (false);
    }

    for (i = 0; i < MAX_PART_COUNT; i++) {
        part = g_local_part[i];
        if (part == NULL || part->sc == NULL || part->sc->mmcbus == NULL) {
            continue;
        }
        mmcbus = part->sc->mmcbus;
        sc = device_get_softc(mmcbus);
        if (host_idx == MMCBR_GET_HOST_ID(device_get_parent(sc->dev), sc->dev)) {
            break;
        }
    }

    if (i == MAX_PART_COUNT) {
        return (false);
    }

    *idx = i;

    return (true);
}

static uint32_t mmc_direct_rw(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors, int flags)
{
    int idx;
    if (mmc_direct_rw_check(host_idx, buffer, &idx) == false) {
        return (0);
    }

    return ((uint32_t)mmc_rw(idx, buffer, start_sector, nsectors, flags));
}

uint32_t mmc_direct_write(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return (mmc_direct_rw(host_idx, buffer, start_sector, nsectors, MMC_WRITE_FLAG));
}

uint32_t mmc_direct_read(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    return (mmc_direct_rw(host_idx, buffer, start_sector, nsectors, MMC_READ_FLAG));
}

#ifdef CONFIG_DRIVERS_MMC_RW_IN_EXC
uint32_t mmc_read_in_exception(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    uint32_t ret, int_save;

    dpal_spin_lock_irqsave(&rw_spinlock, &int_save);
    rw_in_exception = true;
    ret = mmc_direct_rw(host_idx, buffer, start_sector, nsectors, MMC_READ_FLAG);
    rw_in_exception = false;
    dpal_spin_unlock_irqrestore(&rw_spinlock, &int_save);

    return ret;
}

uint32_t mmc_write_in_exception(uint32_t host_idx, char *buffer, uint32_t start_sector, uint32_t nsectors)
{
    uint32_t ret, int_save;

    dpal_spin_lock_irqsave(&rw_spinlock, &int_save);
    rw_in_exception = true;
    ret = mmc_direct_rw(host_idx, buffer, start_sector, nsectors, MMC_WRITE_FLAG);
    rw_in_exception = false;
    dpal_spin_unlock_irqrestore(&rw_spinlock, &int_save);

    return ret;
}
#endif
