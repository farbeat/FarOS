/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:   MMC BLOCK HEADER FILE
 * Author:
 * Create:  2020-04
 */
#ifndef MMC_BLOCK_H
#define MMC_BLOCK_H
#include "drv_mmc_adapter.h"

typedef enum {
    PART_CONFIG_AREA_DEFAULT    = 0x0,
    PART_CONFIG_AREA_BOOT1      = 0x1,
    PART_CONFIG_AREA_BOOT2      = 0x2,
    PART_CONFIG_AREA_RPMB       = 0x3,
} PART_CONFIG_AREA;

typedef enum {
    SDIO_HOST_HIGH_50MHZ_EMMC_HIGH_50MHZ,
    SDIO_HOST_HIGH_50MHZ_EMMC_LOW_32MHZ,
    SDIO_HOST_LOW_32MHZ_EMMC_HIGH_50MHZ,
    SDIO_HOST_LOW_32MHZ_EMMC_LOW_32MHZ,      // closed pll
    SDIOM_CLK_MODE_MAX_NUM = HAL_SDIOM_CLK_MODE_MAX_NUM
} sdiom_clk_cfg_e_type;

typedef enum {
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_0,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_1,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_2,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_3,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_4,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_5,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_6,
    EMMC_PHY_IOCTL_DRIVER_CAPABILITY_7,
} emmc_phy_driver_capability_e_type;

typedef enum {
    EMMC_SLEEP_DISABLE = 0,
    EMMC_SLEEP_ENABLE = 1,
} emmc_sleep_mode_e_type;

int hi_mmc_init(struct rt_mmcsd_host *mmc);

int hi_mmc_deinit(struct rt_mmcsd_host *mmc);

int hi_mmc_get_cardstatus(void);

int hi_mmc_erase(uint32_t start_sector, uint32_t nsectors, uint32_t type);

int mmc_erase_arae(struct rt_mmcsd_card *card, const struct rt_mmcsd_req *req);

int hi_mmc_set_ext_csd_partition_config(PART_CONFIG_AREA area);

int hi_mmc_sleep_awake(emmc_sleep_mode_e_type sleep_enable);

int sdio_read_fifo_block(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size);

int sdio_write_fifo_block(struct rt_sdio_function *func, uint32_t addr, void *src, uint32_t size);

int hi_sdio_io_rw_direct(struct rt_mmcsd_card *card, int writeflag, uint32_t fn,
                         uint32_t addr, uint8_t in, uint8_t *out);

void sdio_writeb(struct rt_sdio_function *func, uint8_t byte, uint32_t addr, int *err);

uint8_t sdio_readb(struct rt_sdio_function *func, uint32_t addr, int *err);

int sdio_writesb(struct rt_sdio_function *func, uint32_t addr, void *src, uint32_t size);

int sdio_readsb(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size);

int sdio_memcpy_fromio(struct rt_sdio_function *func, void *dst, uint32_t addr, uint32_t size);

void sdio_writew(struct rt_sdio_function *func, uint16_t byte, uint32_t addr, int *err);

uint16_t sdio_readw(struct rt_sdio_function *func, uint32_t addr, int *err);

void sdio_writel(struct rt_sdio_function *func, uint32_t byte, uint32_t addr, int *err);

uint32_t sdio_readl(struct rt_sdio_function *func, uint32_t addr, int *err);

int sdio_reset(struct rt_mmcsd_card *card);

void hi_mmc_set_sdiom_clk_mode(sdiom_clk_cfg_e_type clk_type);

int32_t hi_mmc_switch_cmd(struct rt_mmcsd_card *card, uint32_t cmd_arg_val);

int32_t hi_mmc_get_ext_csd(struct rt_mmcsd_card *card,  void *ext_csd);

int32_t hi_mmc_vendor_cmd(struct rt_mmcsd_card *card, uint32_t cmd_arg_val);

int32_t hi_mmc_gen_cmd(struct rt_mmcsd_card *card,  uint32_t cmd_arg_val, uint32_t *data_buf);

uint32_t* hi_mmc_get_cid(struct rt_mmcsd_card *card);

void hi_mmc_set_driver_capability(struct rt_mmcsd_host* host, emmc_phy_driver_capability_e_type cap);

int32_t hi_mmc_rpmb_get_write_counter(struct rt_mmcsd_card *card,  uint32_t *counter, uint32_t *result);

int32_t hi_mmc_rpmb_set_key(struct rt_mmcsd_card *card, uint8_t *key, uint32_t *result);

int32_t hi_mmc_rpmb_cmp_key(uint8_t *key);

void hi_mmc_err_recovery(void);

mmc_err_info_type_t* hi_mmc_get_emmc_failed_info(void);
#endif
