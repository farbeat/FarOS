/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "nandflash_config.h"
#include "stdint.h"
#include "stdio.h"
#include "debug_print.h"
#include "chip_io.h"
#include "securec.h"
#include "nandflash.h"
#include "hal_nandflash.h"
#include "nandflash_ds35x1g.h"
#include "nandflash_w25n01g.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "hal_nandflash.h"

#define NAND_BYTE_ALIGN 4
#define ERASE_ALL_TIMES 10
#define NAND_PAGE_MAX_SIZE 0x100
#define MAX_NAND_BUFF_LEN 4224

uint32_t g_nandflash_init = 1;

struct nand_driver_struct *get_nand_flash(void)
{
    return &g_ds35x1g_driver;
}

void nandflash_qspi_init(void)
{
    struct nand_driver_struct *nandflash_drv = get_nand_flash();
    PRINT("\r\nnandflash init start.\r\n");
    nand_driver_init(SPEED_HIGH);
    PRINT("\r\nnandflash initing ...\r\n");
    g_nandflash_init = nandflash_drv->nand_flash_init();
    PRINT("\r\nnandflash init end. ret %s\r\n", (g_nandflash_init == 0 ? "success" : "fail"));
}

void erase_block(uint32_t start, uint32_t length)
{
    struct nand_driver_struct *driver = get_nand_flash();
    uint32_t ret;
    uint8_t status;
    for (uint32_t i = start; i < (start + length); i++) {
        ret = driver->erase_block(i);
        uint32_t times = 0;
        while (times < ERASE_ALL_TIMES && ret != 0) {
            PRINT("\r\n erase block fail try again %d \r\n", i);
            ret = driver->erase_block(i);
            times++;
        }
    }
}

/* check oob data is all FF */
bool nand_oob_check(uint8_t* buff, uint32_t page_len, uint32_t oob_len)
{
    uint32_t i;
    for (i = 0; i < oob_len; i++) {
        if (buff[page_len + i] != 0xff) {
            break;
        }
    }
    if (i == oob_len) {
        return true;
    } else {
        return false;
    }
}

/* nand write data and check data */
bool nand_write_and_check(struct nand_driver_struct *nand_drv, uint8_t *wbuf, uint32_t pos)
{
    uint32_t i;
    uint32_t len;
    uint32_t w_data, r_data;
    static uint8_t rbuf[MAX_NAND_BUFF_LEN] = {0xff};

    if (nand_drv->write_page(pos, wbuf, nand_drv->bytes_per_page,
        wbuf + nand_drv->bytes_per_page, nand_drv->bytes_per_oob) != 0) {
        return false;
    }
    if (nand_drv->read_page(pos, rbuf, nand_drv->bytes_per_page,
        rbuf + nand_drv->bytes_per_page, nand_drv->bytes_per_oob, NULL) != 0) {
        return false;
    }

    len = sizeof(uint32_t);
    for (i = 0; i < nand_drv->bytes_per_page / len; i++) {
        w_data = *(uint32_t*)&wbuf[i * len];
        // The endian mode of the read data is inconsistent with write data.
        r_data = (uint32_t)(rbuf[i * len + 0x3] | (rbuf[i * len + 0x2] << POS_8) |
            (rbuf[i * len + 0x1] << POS_16) | (rbuf[i * len] << POS_24));
        if (w_data != r_data) {
            PRINT("i=%d wd=%x rd=%x!\n", i, w_data, r_data);
            return false;
        }
    }
    return true;
}