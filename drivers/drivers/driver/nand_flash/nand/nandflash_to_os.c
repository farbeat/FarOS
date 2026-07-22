/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "debug_print.h"
#include "dpal_mtd.h"
#include "nandflash_inner.h"
#include "nandflash.h"
#include "hal_nandflash.h"
#ifdef __LITEOS__
#include "los_misc_pri.h"
#endif
#include "nandflash_config.h"
#include "soc_osal.h"
#include "soc_errno.h"
#include "pinctrl_porting.h"
#include "hal_gpio.h"
#include "platform_core.h"
#include "pinctrl.h"
#include "nandflash_to_os.h"

#define NAND_FLASH_TYLE 4
#define NAND_FLASH_FLAGS 0x400
#define OS_DUMP_LEN 200
#define SUCCESS 0
#define FAILURE 1
#define MAX_RETRYS 3

extern uint8_t nand_read_buf[DEFALUT_BUFLENGTH] __attribute__((aligned(4)));
bool test_overturn = false;

void test_nand_io_init(void)
{
    test_overturn = false;
    uapi_pin_set_mode(S_AGPIO_R8, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(S_AGPIO_R8, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(S_AGPIO_R8, GPIO_LEVEL_HIGH);
}

void test_nand_io_overturn(void)
{
    if (test_overturn) {
        test_overturn = false;
        uapi_gpio_set_dir(S_AGPIO_R8, GPIO_DIRECTION_OUTPUT);
        uapi_gpio_set_val(S_AGPIO_R8, GPIO_LEVEL_LOW);
    } else {
        test_overturn = true; 
        uapi_gpio_set_dir(S_AGPIO_R8, GPIO_DIRECTION_OUTPUT);
        uapi_gpio_set_val(S_AGPIO_R8, GPIO_LEVEL_HIGH);
    }
}

extern uint8_t buffer_verify[DEFALUT_BUFLENGTH];
struct nand_driver_struct *nand_driver;
#ifdef __LITEOS__
extern char *OsCurTaskNameGet(void);
#endif
osal_mutex file_system_mux;

void nand_lock(void)
{
    if (get_trans_type() == TRANS_BY_CPU_SINGLE_LINE) {
        return;
    }
    osal_mutex_lock(&(file_system_mux));
}

void nand_unlock(void)
{
    if (get_trans_type() == TRANS_BY_CPU_SINGLE_LINE) {
        return;
    }
    osal_mutex_unlock(&(file_system_mux));
}

void nand_register(struct mtd_info *mtd)
{
    ext_errno ret;
    if (file_system_mux.mutex == NULL) {
        ret = osal_mutex_init(&(file_system_mux));
        if (ret != OSAL_SUCCESS) {
            return;
        }
    }
    nand_driver = get_nand_flash();
    mtd->size = nand_driver->pages_per_block * nand_driver->bytes_per_page * nand_driver->total_block;
    mtd->erasesize = nand_driver->pages_per_block * nand_driver->bytes_per_page;
    mtd->writesize = nand_driver->bytes_per_page;
#ifdef ENABLE_ECC
    mtd->oobsize = nand_driver->bytes_per_oob / 2; // bytes_per_oob / 2 is because half is data and half is oob
#else
    mtd->oobsize = nand_driver->bytes_per_oob;
#endif
    mtd->name = "nand";
    mtd->type = NAND_FLASH_TYLE;
    mtd->flags = NAND_FLASH_FLAGS;
    mtd->erase = os_nand_erase;
    mtd->read = os_nand_read;
    mtd->write = os_nand_write;
    mtd->read_oob = os_nand_read_oob;
    mtd->write_oob = os_nand_write_oob;
    mtd->block_isbad = os_nand_block_isbad;
    mtd->block_markbad = os_nand_block_markbad;
    NFPRINT("size = %ll \r\n", mtd->size);
    NFPRINT("erase size = %ll \r\n", mtd->erasesize);
    NFPRINT("write size = %ll \r\n", mtd->writesize);
    NFPRINT("oob size = %ll \r\n", mtd->oobsize);
}

int os_nand_erase(const struct mtd_info *mtd, const struct erase_info *instr)
{
#ifdef __LITEOS__
    NFPRINT("os_nand_erase  %s \r\n", OsCurTaskNameGet());
#endif
    nand_lock();
    uint32_t addr = (uint32_t)instr->addr;
    uint32_t len = (uint32_t)instr->len;
    NFPRINT("liteos_nand_erase addr=%u ,len=%u \r\n", addr, len);
    uint32_t start_block = addr / (uint32_t)mtd->erasesize;
    uint32_t block_num = len / (uint32_t)mtd->erasesize;
    NFPRINT("start block = %d\r\n", start_block);
    NFPRINT("block_num = %d \r\n", block_num);
    if ((addr & (mtd->erasesize - 1)) != 0) {
        PRINT("Unaligned erase address!\r\n");
        nand_unlock();
        return FAILURE;
    }
    if ((len + addr) > mtd->size) {
        PRINT("Out of range!\r\n");
        nand_unlock();
        return FAILURE;
    }
    if (len < mtd->erasesize) {
        PRINT("Erase size 0x%08x smaller than one erase block 0x%08x\r\n!", len, mtd->erasesize);
        nand_unlock();
        return FAILURE;
    }
    for (uint32_t i = 0; i < block_num; i++) {
        if (nand_driver->check_bad_block(start_block + i) == 1) {
            NFPRINT("block %d is bad block \r\n", start_block + i);
            continue;
        }
        uint32_t erase_result = MC_FAILURE;
        uint32_t retry = 0;
        while ((erase_result != MC_SUCCESS) && (retry <= MAX_RETRYS)) {
            erase_result = nand_driver->erase_block(start_block + i);
            NFPRINT("%d block erase_result = %d \r\n", start_block + i, erase_result);
            retry++;
        }
        if (erase_result != MC_SUCCESS) {
            nand_driver->mark_bad_block(start_block + i);
            NFPRINT("mark %d is bad block \r\n", start_block + i);
        }
    }
    NFPRINT("liteos_nand_erase SUCCESS \r\n");
    nand_unlock();
    return SUCCESS;
}

int os_nand_read_oob(struct mtd_info *mtd, size_t from, size_t len, size_t *retlen, char *buf)
{
    nand_lock();
    uint32_t *change = (uint32_t *)&from;
    uint32_t addr_from = change[0];
    *retlen = len;
    NFPRINT("read oob addr_from=%x len=%d \r\n", addr_from, len);
    if ((addr_from & (mtd->writesize - 1)) != 0) {
        PRINT("Attempt to read non page aligned data, addr_from %08x.\n", (uint32_t)addr_from);
        nand_unlock();
        return FAILURE;
    }
    if ((len % (mtd->writesize + mtd->oobsize)) != 0) {
        PRINT("Attempt to read non aligned data, read length should "
              "be aligned with (pagesize + oobsize), length:%d "
              "pagesize:%d oobsize:%d\n",
            (uint32_t)len,
            mtd->writesize,
            mtd->oobsize);
        nand_unlock();
        return FAILURE;
    }
    uint32_t len_data = len / (mtd->writesize + mtd->oobsize) * mtd->writesize;
    uint32_t frompage = addr_from / mtd->writesize;
    uint32_t ret;
    for (uint32_t i = 0; i < len_data / mtd->writesize; i++) {
        uint8_t status;
        NFPRINT("read oob len_data = %d , from_page = %d , currrent_page = %d\r\n", len_data, frompage, frompage + i);
        ret = nand_driver->read_page(frompage + i, (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize),
                                     mtd->writesize, (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize) +
                                     mtd->writesize, mtd->oobsize, &status, TRANS_DEFAULT);
        if (ret != MC_SUCCESS) {
            ret = nand_driver->read_page(frompage + i,
                (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize),
                mtd->writesize,
                (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize) + mtd->writesize,
                mtd->oobsize,
                &status,
                TRANS_BY_CPU_SINGLE_LINE);
        }
        if (ret != MC_SUCCESS) {
            PRINT("NAND read to from %08x failed %d \r\n", (uint32_t)addr_from + i * mtd->writesize, ret);
            nand_unlock();
            return FAILURE;
        }
        len -= mtd->writesize + mtd->oobsize;
    }
    nand_unlock();
    return SUCCESS;
}

int os_nand_write_oob(struct mtd_info *mtd, size_t to, size_t len, size_t *retlen, const char *buf)
{
#ifdef __LITEOS__
    NFPRINT("os_nand_write_oob  %s \r\n", OsCurTaskNameGet());
#endif
    nand_lock();
    uint32_t *change = (uint32_t *)&to;
    uint32_t from = change[0];
    if ((from & (mtd->writesize - 1)) != 0) {
        NFPRINT("Attempt to read non page aligned data, to %08x.\n", (uint32_t)from);
        nand_unlock();
        return FAILURE;
    }
    if ((len % (mtd->writesize + mtd->oobsize)) != 0) {
        NFPRINT("Attempt to read non aligned data, read length should "
                "be aligned with (pagesize + oobsize), length:%d "
                "pagesize:%d oobsize:%d\n",
                (uint32_t)len, mtd->writesize, mtd->oobsize);
        nand_unlock();
        return FAILURE;
    }
    uint32_t len_data = len / (mtd->writesize + mtd->oobsize) * mtd->writesize;
    uint32_t frompage = from / mtd->writesize;
    uint32_t write_ret;
    uint32_t read_ret;
    for (uint32_t i = 0; i < len_data / mtd->writesize; i++) {
        uint8_t status;
        write_ret = nand_driver->write_page(frompage + i,
            (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize),
            mtd->writesize,
            (uint8_t *)buf + i * (mtd->writesize + mtd->oobsize) + mtd->writesize,
            mtd->oobsize);
        memset_s(buffer_verify, OS_DUMP_LEN, 0, OS_DUMP_LEN);
        read_ret = nand_driver->read_page(frompage + i, (uint8_t *)buffer_verify, mtd->writesize,
                                          (uint8_t *)buffer_verify + mtd->writesize, mtd->oobsize,
                                          &status, TRANS_BY_CPU_SINGLE_LINE);
        if (write_ret != MC_SUCCESS) {
            PRINT("NAND write to %08x failed %d \r\n", (uint32_t)from + i * mtd->writesize, write_ret);
            nand_unlock();
            return FAILURE;
        }
        if (read_ret != MC_SUCCESS) {
            PRINT("NAND write read %08x failed %d \r\n", (uint32_t)from + i * mtd->writesize, read_ret);
            nand_unlock();
            return FAILURE;
        }
        for (uint32_t t = 0; t < (mtd->writesize + mtd->oobsize); t++) {
            if ((uint8_t)buffer_verify[t] != (uint8_t)(buf[i * (mtd->writesize + mtd->oobsize) + t])) {
#ifdef __LITEOS__
                PRINT("PAGE = %d\r\n", frompage + i);
                PRINT("origin-----------------------------------------, diff index = %d\r\n", t);
                OsDumpMemByte(OS_DUMP_LEN, (const char *)buf + i * (mtd->writesize + mtd->oobsize));
                PRINT("after-dma-nand_read_buf---------------------------, diff index = %d\r\n", t);
                OsDumpMemByte(OS_DUMP_LEN, nand_read_buf);
                PRINT("after-copy-----------------------------------------, diff index = %d\r\n", t);
                OsDumpMemByte(OS_DUMP_LEN, buffer_verify);

#endif
                PRINT("os_nand_write_oob check write fail, page = %d\r\n", frompage + i);
                nand_unlock();
                return FAILURE;
            }
        }
        len -= mtd->writesize + mtd->oobsize;
        *retlen = *retlen + mtd->writesize + mtd->oobsize;
    }
    nand_unlock();
    return SUCCESS;
}

int os_nand_block_isbad(const struct mtd_info *mtd, size_t ofs)
{
    nand_lock();
    uint32_t *change = (uint32_t *)&ofs;
    uint32_t addr = change[0];
    NFPRINT("check is bad addr = %x\r\n", addr);
    if ((addr & (mtd->erasesize - 1)) != 0) {
        NFPRINT("Unaligned checkbad address!\n");
        nand_unlock();
        return FAILURE;
    }
    int ret = (uint8_t)nand_driver->check_bad_block(addr / mtd->erasesize);
    NFPRINT("liteos_nand_block_isbad ret = %d \r\n", ret);
    nand_unlock();
    return ret;
}

int os_nand_block_markbad(const struct mtd_info *mtd, size_t ofs)
{
    nand_lock();
    uint32_t *change = (uint32_t *)&ofs;
    uint32_t addr = change[0];
    NFPRINT("mark bad addr = %x \r\n", addr);
    if ((addr & (mtd->erasesize - 1)) != 0) {
        NFPRINT("Unaligned markbad address!\n");
        nand_unlock();
        return FAILURE;
    }
    PRINT("mark bad block %d \r\n", addr / mtd->erasesize);
    int ret = (int)nand_driver->mark_bad_block(addr / mtd->erasesize);
    nand_unlock();
    return ret;
}

int os_nand_read(const struct mtd_info *mtd, size_t from, size_t len, const size_t *retlen, const char *buf)
{
    unused(mtd);
    unused(from);
    unused(len);
    unused(retlen);
    unused(buf);
    return SUCCESS;
}

int os_nand_write(const struct mtd_info *mtd, size_t to, size_t len, const size_t *retlen, const char *buf)
{
    unused(mtd);
    unused(to);
    unused(len);
    unused(retlen);
    unused(buf);
    return SUCCESS;
}
