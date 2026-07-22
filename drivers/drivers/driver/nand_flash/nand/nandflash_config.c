/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "nandflash_config.h"
#include <sys/mount.h>
#include "sys/stat.h"
#include "stdint.h"
#include "stdio.h"
#include "dpal_mtd.h"
#include "debug_print.h"
#include "fcntl.h"
#include "errno.h"
#include "chip_io.h"
#include "securec.h"
#include "mtd_list.h"
#include "mtd_partition.h"
#include "dpal.h"
#include "nandflash.h"
#include "chip_io.h"
#include "hal_nandflash.h"
#include "nandflash_tc58cyg.h"
#include "nandflash_ds35x1g.h"
#include "nandflash_w25n01g.h"
#include "nandflash_to_os.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "hal_nandflash.h"

#ifndef MEMORY_MINI
#define YF_VOLUMES_NUM 4
#else
#define YF_VOLUMES_NUM 1
#endif

#define NAND_BYTE_ALIGN 4
#define ERASE_ALL_TIMES 10
#define NAND_PAGE_MAX_SIZE 0x100

extern uint8_t nand_read_buf[DEFALUT_BUFLENGTH] __attribute__((aligned(4)));
uint8_t buffer_verify[DEFALUT_BUFLENGTH] __attribute__((aligned(4)));
uint8_t buffer_oob_verify[128] __attribute__((aligned(4))); // 128 is one max oob size
int has_nandflash_init = 0;

#ifndef MEMORY_MINI
#define YF_NAME_STRS "/system/", "/update/", "/music/", "/user/"
uint32_t yf_volume_percent[YF_VOLUMES_NUM] = { 10, 10, 10, 70 };
#else
#define YF_NAME_STRS "/user/"
uint32_t yf_volume_percent[YF_VOLUMES_NUM] = {100};
#endif

static const char * const g_volume_names[YF_VOLUMES_NUM] = { YF_NAME_STRS };

struct mtd_info nandflash_freertos_info;
bool get_exception_status(void);

struct nand_driver_struct *get_nand_flash(void)
{
    return &g_ds35x1g_driver;
}

static int formate_filesystem(uint32_t total_block_num)
{
    int ret;
    uint32_t index;
    size_t part_start_sector = 0;
    size_t part_count_sector = 0;
    uint32_t total_sector_num = total_block_num;
    char dev_name[64] = { 0 };
    for (index = 0; index < YF_VOLUMES_NUM; ++index) {
        part_count_sector = (total_sector_num * yf_volume_percent[index]) / 0x64;

        /* The number of blocks in the last area is calculated separately. */
        if (index == (YF_VOLUMES_NUM - 1)) {
            part_count_sector = total_sector_num - part_start_sector;
        }

        ret = add_mtd_partition("nand", (uint32_t)(part_start_sector * nandflash_freertos_info.erasesize),
                                (uint32_t)(part_count_sector * nandflash_freertos_info.erasesize), index);
        if (ret != 0) {
            PRINT("add partition %d fail\r\n", index);
            return -EIO;
        }
        ret = sprintf_s(dev_name, sizeof(dev_name), "/dev/nandblk%d", index);
        if (ret <= 0) {
            PRINT("formate dev %d name fail \r\n", index);
            return -EIO;
        }

        ret = mount(dev_name, g_volume_names[index], "yaffs", 0, NULL);
        PRINT("formate filesystem devinfo: index = %d , dev_name = %s, folder_name = %s, \
              part_start_sector = %d, part_count_sector = %d\r\n", index, dev_name,
              g_volume_names[index], part_start_sector, part_count_sector);
        if (ret != 0) {
            return ret;
        }
        part_start_sector += part_count_sector;

        ret = yaffs_sync(g_volume_names[index]);
        if (ret != 0) {
            PRINT("[%d] yaffs_sync fail, ret = %d", index, ret);
        }
    }
    return ret;
}

/* 使用约束：中断和异常中，禁止sync操作 */
void nandflash_yaffs_sync(void)
{
    if (osal_in_interrupt() != true && get_exception_status() != true) {
        for (uint8_t index = 0; index < YF_VOLUMES_NUM; index++) {
            int32_t ret = yaffs_sync(g_volume_names[index]);
            if (ret != 0) {
                PRINT("[%d] yaffs_sync fail, ret = %d", index, ret);
            }
        }
    }
}

void nandflash_yaffs_format(void)
{
    for (uint8_t index = 0; index < YF_VOLUMES_NUM; index++) {
        int32_t ret = yaffs_format(g_volume_names[index], 1, 1, 1);
        PRINT("[%s] yaffs_format ret = %d", g_volume_names[index], ret);
    }
}

void erase_all(struct nand_driver_struct *driver)
{
    uint32_t ret;
    uint8_t status;
    for (uint32_t j = 0; j < 1; j++) {
        for (uint32_t i = 0; i < driver->total_block; i++) {
            ret = driver->erase_block(i);
            uint32_t times = 0;
            while (times < ERASE_ALL_TIMES && ret != 0) {
                PRINT("\r\n erase block fail try again %d \r\n", i);
                ret = driver->erase_block(i);
                times++;
            }
            if (ret != 0) {
                PRINT("\r\n erase block fail%d !!!!!! \r\n", i);
            }
        }
    }
}

void test_nand_flash_read_8(struct nand_driver_struct  *test_nand_flash_driver, uint32_t nand_page)
{
    struct mtd_info *mtd;
    mtd = get_mtd("nand");
    if (mtd == NULL) {
        nand_register(mtd);
    }
    memset_s(buffer_verify, DEFALUT_BUFLENGTH, 0, DEFALUT_BUFLENGTH);
    PRINT("nandflash [READ][PAGE:%d]\r\n", nand_page);
    uint32_t retlen;
    writew(0x57000010, 0x5a);
    os_nand_read_oob(
        mtd, mtd->writesize * nand_page, mtd->writesize + mtd->oobsize, &retlen, (const char *)buffer_verify);
    writew(0x57000010, 0x0);
    for (uint32_t i = 0; i < test_nand_flash_driver->bytes_per_page; i++) {
        PRINT("[READ DATA INDEX:%d , VALUE:%d] \r\n", i, buffer_verify[i]);
    }
#ifdef ENABLE_ECC
    for (uint8_t i = 0; i < test_nand_flash_driver->bytes_per_oob / 2; i++) { // 2 is need by ecc
        PRINT("[READ DATA OOB:%d , VALUE:%d] \r\n", i, buffer_oob_verify[i]);
    }
#else
    for (uint8_t i = 0; i < test_nand_flash_driver->bytes_per_oob; i++) {
        PRINT("[READ DATA OOB:%d , VALUE:%d] \r\n", i, buffer_oob_verify[i]);
    }
#endif
    PRINT("[READ][READ][READ][READ][READ][READ][READ][READ][READ]\r\n", nand_page);
}

void test_nand_flash_write_width8(struct nand_driver_struct *test_nand_flash_driver, uint32_t nand_page)
{
    struct mtd_info *mtd;
    mtd = get_mtd("nand");
    if (mtd == NULL) {
        nand_register(mtd);
    }

    int j = 0;
    for (uint32_t i = 0; i < test_nand_flash_driver->bytes_per_page; i++) {
        buffer_verify[i] = i;
        j++;
        if (j == NAND_PAGE_MAX_SIZE) {
            j = 0;
        }
    }

#ifdef ENABLE_ECC
    for (uint8_t i = 0; i < test_nand_flash_driver->bytes_per_oob / 2; i++) { // 2 is need by ecc
        buffer_oob_verify[i] = i;
    }
#else
    for (uint8_t i = 0; i < test_nand_flash_driver->bytes_per_oob; i++) {
        buffer_oob_verify[i] = i;
    }
#endif
    buffer_oob_verify[0] = 0xff;
    buffer_oob_verify[1] = 0xff;
    PRINT("nandflash [READ][PAGE:%d]\r\n", nand_page);
    PRINT("[WRITE][WRITE][WRITE][WRITE][WRITE][WRITE][WRITE][WRITE]\r\n");
    uint32_t retlen;
    os_nand_write_oob(
        mtd, mtd->writesize * nand_page, mtd->writesize + mtd->oobsize, &retlen, (const char *)buffer_verify);
}

void nandflash_qspi_init(void)
{
    struct nand_driver_struct *frtos_nandflash_drv = get_nand_flash();
    uint32_t ret;
    PRINT("\r\nnandflash init start.\r\n");
    nand_register(&nandflash_freertos_info);

    ret = frtos_nandflash_drv->nand_flash_init();
    PRINT("\r\nnandflash_qspi_init %s\r\n", (ret == 0 ? "success" : "fail"));
}

int nandflash_filesystem_init(void)
{
    if (has_nandflash_init == 1) {
        PRINT("\r\nnandflash init ok.\r\n");
        return 0;
    }
    struct nand_driver_struct *freertos_nandflash_driver = get_nand_flash();
    nandflash_qspi_init();
    set_trans_type(TRANS_BY_DMA_QUAD);
    PRINT("\r\n nandflash init end.\r\n");
    mtd_init_list();
    add_mtd_list("nand", &nandflash_freertos_info);
    if (get_mtd_info("nand") != 0) {
        PRINT("get_mtd_info faild!\n");
    }
    PRINT("yaffs filesystem init start!\n");
    int ret = formate_filesystem(freertos_nandflash_driver->total_block);
    PRINT("yaffs filesystem init end!\n");
    has_nandflash_init = 1;
    return ret;
}

void nandflash_qspi_resume(void)
{
    nand_driver_init(SPEED_HIGH);
}
