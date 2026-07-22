/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: LiteOS Board.
 * Author: @CompanyNameTag
 * Create: 2021-09-03
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

#include <sys/mount.h>
#include "soc_mmc.h"
#include "mmcreg.h"
#include "block.h"
#include "disk.h"
#include "host.h"
#include "chip_io.h"
#include "mmc.h"
#include "dpal_driverbase.h"
#include "fs/fs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define FF_VOLUMES_NUM 4
#define EMMC_PINMUX_NUM 10
#define EMMC_PAD_PINMUX_NUM 9
#define SDIO_PINMUX_NUM 6
#define SDIO_PINMUX_PAD_NUM 5
#define EMMC_NUM100 100
#define EMMC_PART_MODE 0666
#define EMMC_MSLEEP_20 20
#define EMMC_ACCESS_OFFSET 20
#define EMMC_SECTOR_TO_KB 2
#define FF_VOLUME_NAME_STRS "/user/", "/system/", "/update/", "/music/"
uint32_t g_volume_percent[FF_VOLUMES_NUM] = { 11, 11, 8, 70 };
static const char * const g_volume_names[FF_VOLUMES_NUM] = { FF_VOLUME_NAME_STRS };
int32_t g_fail_mount_index = 0;

static struct dpal_resource sdmmc0_resources[] = {
    {
        .start  = BRANDY_EMMC_REG_BASE,
        .end    = BRANDY_EMMC_REG_END,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = BRANDY_INTERRUPT_EMMC,
        .end    = BRANDY_INTERRUPT_EMMC,
        .flags  = IORESOURCE_IRQ,
    },
    {
        .start  = PERI_EMMC_CRG_RST_CTL,
        .end    = PERI_EMMC_CRG_RST_CTL,
        .flags  = IORESOURCE_REG,
    },
};

struct dpal_platform_device device_sdmmc0 = {
    .name       = "sdhci0",
    .id     = 0,
    .resource   = sdmmc0_resources,
    .num_resources  = sizeof(sdmmc0_resources) / sizeof(sdmmc0_resources[0]),
};

#ifdef DPALCFG_DRIVERS_SDIO
static struct dpal_resource sdmmc1_resources[] = {
    {
        .start  = BRANDY_SDIO_REG_BASE,
        .end    = BRANDY_SDIO_REG_END,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = BRANDY_INTERRUPT_SDIO,
        .end    = BRANDY_INTERRUPT_SDIO,
        .flags  = IORESOURCE_IRQ,
    },
    {
        .start  = PERI_EMMC_CRG_RST_CTL,
        .end    = PERI_EMMC_CRG_RST_CTL,
        .flags  = IORESOURCE_REG,
    },
};

struct dpal_platform_device device_sdmmc1 = {
    .name       = "sdhci1",
    .id     = 1,
    .resource   = sdmmc1_resources,
    .num_resources  = sizeof(sdmmc1_resources) / sizeof(sdmmc1_resources[0]),
};
#endif

int emmc_drv_init(void)
{
    int ret;
    int index;

    size_t part_start_sector;
    size_t part_count_sector;
    uint64_t total_sector_num;

    total_sector_num = EMMC_CAPCITY_SIZE / MMC_SECTOR_SIZE;

    if (total_sector_num <= RESERVED_BOOT_SECTOR) {
        return -EINVAL;
    }
    total_sector_num -= RESERVED_BOOT_SECTOR;
    part_start_sector = RESERVED_BOOT_SECTOR;

    for (index = 0; index < FF_VOLUMES_NUM; ++index) {
        part_count_sector = (size_t)((total_sector_num * g_volume_percent[index]) / EMMC_NUM100);
        dpal_log_warn("mmc_partition[%d]:sector size=%d capacity=%dKB\n",
            index, part_count_sector, part_count_sector / EMMC_SECTOR_TO_KB);
        ret = add_mmc_partition(&emmc, part_start_sector, part_count_sector);
        if (ret != 0) {
            return -EIO;
        }

        part_start_sector += part_count_sector;
    }
    (void)dpal_platform_device_register(&device_sdmmc0);

    ret = MMC_HostInitById(0);
    if (ret != 0) {
        return -EIO;
    }

    return ENOERR;
}

int emmc_drv_try_mount(void)
{
    int ret;
    int index;
    int try_access_num = 0;
    char dev_name[64] = { 0 };

    for (index = 0; index < FF_VOLUMES_NUM; ++index) {
        ret = sprintf_s(dev_name, sizeof(dev_name), "/dev/mmcblk0p%d", index);
        if (ret <= 0) {
            return -EIO;
        }

        do {
            ret = los_part_access(dev_name, EMMC_PART_MODE);
            ++try_access_num;
            dpal_msleep(EMMC_MSLEEP_20);
        } while ((ret != ENOERR) && (try_access_num < EMMC_ACCESS_OFFSET));

        if (ret != ENOERR) {
            return -EACCES;
        }

        ret = mount(dev_name, g_volume_names[index], "vfat", 0, NULL);
        if (ret != OK) {
            g_fail_mount_index = index;
            printf("%s, mount failed, partition:[%d]\r\n", __FUNCTION__, index);
            return -ENODEV;
        }
    }

    return ENOERR;
}

int emmc_drv_format(void)
{
    int ret;
    int index = g_fail_mount_index;
    g_fail_mount_index = 0;
    char dev_name[64] = { 0 };

    for (; index < FF_VOLUMES_NUM; ++index) {
        ret = sprintf_s(dev_name, sizeof(dev_name), "/dev/mmcblk0p%d", index);
        if (ret <= 0) {
            continue;
        }

        umount(g_volume_names[index]);

        ret = format(dev_name, SECTOR_NUM_PER_CLUSTER, FMT_FAT32 | FMT_ERASE);
        if (ret != 0) {
            return -EIO;
        }

        ret = mount(dev_name, g_volume_names[index], "vfat", 0, NULL);
        if (ret != OK) {
            return -ENODEV;
        }
    }

    return ENOERR;
}

int emmc_drv_context_init(void)
{
    int ret;

    ret = emmc_drv_init();
    if (ret != ENOERR) {
        printf("[%s:%d] ret[%d]\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    ret = emmc_drv_try_mount();
    if (ret == ENOERR) {
        printf("[%s:%d] ret[%d]\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    return emmc_drv_format();
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

#ifdef DPALCFG_DRIVERS_SDIO
void sdio_drv_context_init(void)
{
    uint32_t ret;

    (void)dpal_platform_device_register(&device_sdmmc1);
    ret = (uint32_t)MMC_HostInitById(1);
    if (ret != 0) {
        printf("MMC_HostInitById init sdio error, ret = %d\r\n", ret);
    }
}

void init_sdio_pinmux(void)
{
    int n;

    for (n = 0; n < SDIO_PINMUX_NUM; n++) {
        writew(S_HGPIO + S_HGPIO0_OFFSET + n * 0x4, FUNC_SDIO);
    }

    for (n = 0; n < SDIO_PINMUX_PAD_NUM; n++) {
        writew(S_HGPIO_PAD + S_HGPIO0_PAD_OFFSET + n * 0x4, IE_PINMUX_EN);
    }
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
