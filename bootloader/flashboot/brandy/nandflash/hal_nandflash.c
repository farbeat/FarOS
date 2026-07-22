/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  NON-OS NANDFLASH DRIVER
 * Author: @CompanyNameTag
 * Create:  2020-10-15
 */

#include "hal_nandflash.h"
#include "securec.h"
#include "chip_io.h"
#include "tcxo.h"
#include "debug_print.h"
#include "soc_osal.h"
#include "hal_qspi.h"
#include "nandflash.h"
#include "nandflash_qspi.h"
#include "nandflash_config.h"
#include "hal_nandflash.h"
#include "pinctrl_porting.h"
#include "gpio.h"
#include "nandflash.h"

#define OFFSET 0
#define MAX_WAIT 10
#define HAL_NAND_WAIT_3US 3
#define HAL_NAND_BYTE_ALIGN 4
#define HAL_NAND_INS_OFFSET 24
#define NAND_ID_DATA_SIZE 2
#define NAND_W_DATA_CNT2 2
#define NAND_W_DATA_CNT15 15
#define DMA_TRANSFER_TIMEOUT_MS 1000

static volatile bool is_nandflash_working = false;

uint8_t nand_write_buf[DEFALUT_BUFLENGTH] __attribute__((aligned(4)));
uint8_t nand_read_buf[DEFALUT_BUFLENGTH] __attribute__((aligned(4)));

void cs_pulldown(void)
{
    if (SER_SELECT == 1) {
        uapi_pin_set_mode(S_HGPIO11, (pin_mode_t)HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_HGPIO11, GPIO_DIRECTION_OUTPUT);
        uapi_gpio_set_val(S_HGPIO11, GPIO_LEVEL_LOW);
    } else {
        uapi_pin_set_mode(S_HGPIO12, (pin_mode_t)HAL_PIO_FUNC_GPIO);
        uapi_gpio_set_dir(S_HGPIO12, GPIO_DIRECTION_OUTPUT);
        uapi_gpio_set_val(S_HGPIO12, GPIO_LEVEL_LOW);
    }
}

void cs_pullup(void)
{
    if (SER_SELECT == 1) {
        uapi_pin_set_mode(S_HGPIO11, (pin_mode_t)HAL_PIO_FUNC_QSPI2);
    } else {
        uapi_pin_set_mode(S_HGPIO12, (pin_mode_t)HAL_PIO_FUNC_QSPI2);
    }
}

uint8_t mc_flash_get_offset(void)
{
    return OFFSET;
}

uint16_t read_id(qspi_data_type_def *qspi_data)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(1);
    nand_qspi_enable();
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(DUMMY_CYCLE_BYTE);
    uint16_t temp_id;
    qspi_read_width_8((uint8_t *)&temp_id, NAND_ID_DATA_SIZE);
    is_nandflash_working = false;
    return SWAP_U16(temp_id);
}

uint32_t read_cell_array(qspi_data_type_def *qspi_data)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(qspi_data->data_ptr[1]); /* 1: bit24~bit16 */
    nand_qspi_write_data(qspi_data->data_ptr[2]); /* 2: bit15~bit8 */
    nand_qspi_write_data(qspi_data->data_ptr[3]); /* 3: bit7~bit0 */
    writel(SER, SER_SELECT);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
    NFPRINT("readcell array ins:%x, addr[0]:%x addr[1]:%x \r\n",
        qspi_data->instruction,
        qspi_data->data_ptr[0],
        qspi_data->data_ptr[1]);
    return MC_SUCCESS;
}

uint32_t program_execute(qspi_data_type_def *qspi_data)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    /* spi config */
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(qspi_data->data_ptr[1]); /* 1: bit24~bit16 */
    nand_qspi_write_data(qspi_data->data_ptr[2]); /* 2: bit15~bit8 */
    nand_qspi_write_data(qspi_data->data_ptr[3]); /* 3: bit7~bit0 */
    writel(SER, SER_SELECT);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
    return MC_SUCCESS;
}

uint32_t block_erase(qspi_data_type_def *qspi_data)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(qspi_data->data_ptr[1]); /* 1: bit24~bit16 */
    nand_qspi_write_data(qspi_data->data_ptr[2]); /* 2: bit15~bit8 */
    nand_qspi_write_data(qspi_data->data_ptr[3]); /* 3: bit7~bit0 */
    writel(SER, SER_SELECT);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
    return MC_SUCCESS;
}

uint8_t read_sr(qspi_data_type_def *qspi_data, uint8_t address)
{
    UNUSED(address);
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(qspi_data->address);
    writel(SER, SER_SELECT);
    uint8_t status;
    qspi_read_width_8(&status, qspi_data->data_size);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
    NFPRINT("read_sr,ins:0x%x,address:%x,status:%x\r\n", qspi_data->instruction, qspi_data->address, status);
    return status;
}

uint32_t write_sr(qspi_data_type_def *qspi_data, uint8_t addr, uint8_t data)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(addr);
    nand_qspi_write_data(data);
    writel(SER, SER_SELECT);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
    NFPRINT("write_sr,ins:0x%x,address:%x,status:%x\r\n", qspi_data->instruction, addr, data);
    return MC_SUCCESS;
}

void write_cmd(uint8_t command)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    unsigned int irq_sts = osal_irq_lock();
    is_nandflash_working = true;
    qspi_busy_wait();
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(command);
    writel(SER, SER_SELECT);
    is_nandflash_working = false;
    osal_irq_restore(irq_sts);
}

static void write_oob(uint8_t *oob, uint32_t oob_len)
{
    UNUSED(oob_len);
    struct nand_driver_struct *nandflash = (struct nand_driver_struct *)get_nand_flash();
    errno_t err_code;
    if (nandflash->enable_ecc == true) {
        if (nandflash->oob_total_len >= 1) { // 1 is ecc field count
            err_code = memcpy_s(nand_write_buf + nandflash->bytes_per_page + nandflash->oob1_offset,
                nandflash->oob1_per_size,
                oob,
                nandflash->oob1_per_size);
            if (err_code != EOK) {
                PRINT("write oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
        if (nandflash->oob_total_len >= 2) { // 2 is ecc field count
            err_code = memcpy_s(nand_write_buf + nandflash->bytes_per_page + nandflash->oob2_offset,
                nandflash->oob2_per_size,
                oob + nandflash->oob1_per_size,
                nandflash->oob2_per_size);
            if (err_code != EOK) {
                PRINT("write oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
        if (nandflash->oob_total_len >= 3) { // 3 is ecc field count
            err_code = memcpy_s(nand_write_buf + nandflash->bytes_per_page + nandflash->oob3_offset,
                nandflash->oob3_per_size,
                oob + nandflash->oob1_per_size + nandflash->oob2_per_size,
                nandflash->oob3_per_size);
            if (err_code != EOK) {
                PRINT("write oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }

        if (nandflash->oob_total_len >= 4) { // 4 is ecc field count
            err_code = memcpy_s(nand_write_buf + nandflash->bytes_per_page + nandflash->oob4_offset,
                nandflash->oob4_per_size,
                oob + nandflash->oob1_per_size + nandflash->oob2_per_size + nandflash->oob3_per_size,
                nandflash->oob4_per_size);
            if (err_code != EOK) {
                PRINT("write oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
    } else {
        err_code = memcpy_s(nand_write_buf + nandflash->bytes_per_page, nandflash->bytes_per_oob, oob,
            nandflash->bytes_per_oob);
        if (err_code != EOK) {
            PRINT("write oob memcpy_s fail, err ret = %d \r\n", err_code);
        }
    }
    return;
}

uint32_t program_load(qspi_data_type_def *qspi_data, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    is_nandflash_working = true;
    errno_t err_code;
    if ((data != NULL) && (data_len > 0)) {
        err_code = memcpy_s(nand_write_buf, data_len, data, data_len);
        if (err_code != EOK) {
            printf("[%s:%d]---nandflash copy fail---------", __FUNCTION__, __LINE__);
        }
    }
    write_oob(oob, oob_len);
    qspi_data->data_ptr = nand_write_buf;
    uint32_t result = write_by_cpu_single_line(qspi_data);
    is_nandflash_working = false;
    return result;
}

static void read_oob(uint8_t *oob, uint32_t oob_len)
{
    UNUSED(oob_len);
    struct nand_driver_struct *nandflash = (struct nand_driver_struct *)get_nand_flash();
    errno_t err_code;
    if (nandflash->enable_ecc == false) {
        err_code =
            memcpy_s(oob, nandflash->bytes_per_oob, nand_read_buf + nandflash->bytes_per_page,
                nandflash->bytes_per_oob);
        if (err_code != EOK) {
            PRINT("read oob memcpy_s fail, err ret = %d \r\n", err_code);
        }
    } else {
        if (nandflash->oob_total_len >= 1) { // 1 is ecc field count
            err_code = memcpy_s(oob,
                nandflash->oob1_per_size,
                nand_read_buf + nandflash->bytes_per_page + nandflash->oob1_offset,
                nandflash->oob1_per_size);
            if (err_code != EOK) {
                PRINT("read oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
        if (nandflash->oob_total_len >= 2) { // 2 is ecc field count
            err_code = memcpy_s(oob + nandflash->oob1_per_size,
                nandflash->oob2_per_size,
                nand_read_buf + nandflash->bytes_per_page + nandflash->oob2_offset,
                nandflash->oob2_per_size);
            if (err_code != EOK) {
                PRINT("read oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
        if (nandflash->oob_total_len >= 3) { // 3 is ecc field count
            err_code = memcpy_s(oob + nandflash->oob1_per_size + nandflash->oob2_per_size,
                nandflash->oob3_per_size,
                nand_read_buf + nandflash->bytes_per_page + nandflash->oob3_offset,
                nandflash->oob3_per_size);
            if (err_code != EOK) {
                PRINT("read oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
        if (nandflash->oob_total_len >= 4) { // 4 is ecc field count
            err_code = memcpy_s(oob + nandflash->oob1_per_size + nandflash->oob2_per_size + nandflash->oob3_per_size,
                nandflash->oob4_per_size,
                nand_read_buf + nandflash->bytes_per_page + nandflash->oob4_offset,
                nandflash->oob4_per_size);
            if (err_code != EOK) {
                PRINT("read oob memcpy_s fail, err ret = %d \r\n", err_code);
            }
        }
    }
    return;
}

uint32_t read_buffer(qspi_data_type_def *qspi_data, uint8_t *data, uint32_t data_len, uint8_t *oob, uint32_t oob_len)
{
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    is_nandflash_working = true;
    errno_t err_code;
    uint32_t result = read_buffer_single_line_width_32(qspi_data);
    if ((data != NULL) && (data_len > 0)) {
        err_code = memcpy_s(data, data_len, nand_read_buf, data_len);
        if (err_code != EOK) {
            printf("[%s:%d]---nandflash copy fail---------", __FUNCTION__, __LINE__);
        }
    }
    read_oob(oob, oob_len);
    is_nandflash_working = false;
    return result;
}

uint32_t read_buffer_single_line_width_8(qspi_data_type_def *qspi_data)
{
    qspi_busy_wait();
    uint16_t address = SWAP_U16(qspi_data->address);
    uint8_t *addr = (uint8_t *)&address;
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    unsigned int irq_sts = osal_irq_lock();
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(qspi_data->data_size - 1);
    nand_qspi_enable();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(addr[0]);
    nand_qspi_write_data(addr[1]);
    nand_qspi_write_data(DUMMY_CYCLE_BYTE);
    writel(SER, SER_SELECT);
    qspi_read_width_8(qspi_data->data_ptr, qspi_data->data_size);
    for (uint32_t i = 0; i < qspi_data->data_size; i++) {
        NFPRINT("read buff qspi_data->data_ptr[i] = %x \r\n", qspi_data->data_ptr[i]);
    }
    osal_irq_restore(irq_sts);
    return MC_SUCCESS;
}

uint32_t read_buffer_single_line_width_32(qspi_data_type_def *qspi_data)
{
    qspi_busy_wait();
    uint16_t address = SWAP_U16(qspi_data->address);
    uint8_t *addr = (uint8_t *)&address;
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_32BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg((qspi_data->data_size - 1) / HAL_NAND_BYTE_ALIGN);
    writel(DMACR, 1);
    writel(DMARDLR, 0x8);
    nand_qspi_enable();
    uint32_t ins = qspi_data->instruction;
    ins = ins << HAL_NAND_INS_OFFSET;
    uint64_t static_time_before;
    uint64_t static_time_after;
    if (SUPPORT_STATISTIC == 1) {
        static_time_before = uapi_tcxo_get_us();
    }
    unsigned int irq_sts = osal_irq_lock();
    writel(SER, 0x0);
    nand_qspi_write_data(ins);
    writel(SER, SER_SELECT);
    qspi_read_width_32((uint32_t *)nand_read_buf, qspi_data->data_size / HAL_NAND_BYTE_ALIGN);
    osal_irq_restore(irq_sts);
    if (SUPPORT_STATISTIC == 1) {
        static_time_after = uapi_tcxo_get_us();
        uint64_t use_count = static_time_after - static_time_before;
        uint32_t *count = (uint32_t *)&use_count;
        PRINT("[func:read_buffer_single_line_width_32] read qspi DR cost[%d us]\r\n", count[0]);
    }
    return MC_SUCCESS;
}

uint32_t mark_bad_block(qspi_data_type_def *qspi_data)
{
    qspi_busy_wait();
    uint16_t address = SWAP_U16(qspi_data->address);
    uint8_t *addr = (uint8_t *)&address;
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_TX_ONLY_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    uint64_t static_time_before;
    uint64_t static_time_after;
    if (SUPPORT_STATISTIC == 1) {
        static_time_before = uapi_tcxo_get_us();
    }
    unsigned int irq_sts = osal_irq_lock();
    writel(SER, 0x0);
    nand_qspi_write_data(qspi_data->instruction);
    nand_qspi_write_data(addr[0]);
    nand_qspi_write_data(addr[1]);
    for (uint32_t i = 0; i < 4; i++) { // 4 is data length
        nand_qspi_write_data(qspi_data->data_ptr[i]);
    }
    writel(SER, SER_SELECT);
    osal_irq_restore(irq_sts);
    if (SUPPORT_STATISTIC == 1) {
        static_time_after = uapi_tcxo_get_us();
        uint64_t use_count = static_time_after - static_time_before;
        uint32_t *count = (uint32_t *)&use_count;
        PRINT("[func:program_load_width_8] write [%d bytes] data into qspi DR cost[%d us]\r\n",
            qspi_data->data_size,
            count[0]);
    }
    while (is_trans_busy()) {
        NFPRINT("nandfash is sending data !!!\r\n");
    }
    return MC_SUCCESS;
}

uint32_t program_load_width_32(qspi_data_type_def *qspi_data)
{
    qspi_busy_wait();
    uint16_t address = SWAP_U16(qspi_data->address);
    uint8_t *addr = (uint8_t *)&address;
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_32BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    uint8_t first_byte = qspi_data->data_ptr[0];
    uint32_t *send_buffer = (uint8_t *)qspi_data->data_ptr + 1;
    for (uint32_t i = 0; i < qspi_data->data_size / HAL_NAND_BYTE_ALIGN; i++) {
        send_buffer[i] = SWAP_U32(send_buffer[i]);
    }
    uint32_t ins = qspi_data->instruction;
    ins = ins << HAL_NAND_INS_OFFSET;
    ins = (ins & 0xff000000) | first_byte;
    uint64_t static_time_before;
    uint64_t static_time_after;
    if (SUPPORT_STATISTIC == 1) {
        static_time_before = uapi_tcxo_get_us();
    }
    unsigned int irq_sts = osal_irq_lock();
    writel(SER, 0x0);
    nand_qspi_write_data(ins);
    for (uint8_t i = 0; i < NAND_W_DATA_CNT15; i++) {
        nand_qspi_write_data(send_buffer[i]);
    }
    writel(SER, SER_SELECT);
    for (uint32_t i = NAND_W_DATA_CNT15; i < qspi_data->data_size / HAL_NAND_BYTE_ALIGN; i++) {
        uint32_t timeout = 0;
        while (is_fifo_full() && (timeout < SEND_TIMEOUT)) {
            timeout = timeout + 1;
        }
        if (timeout == SEND_TIMEOUT) {
            NFPRINT("nandfash data writing timeout!!!\r\n");
        } else {
            nand_qspi_write_data(send_buffer[i]);
        }
    }
    osal_irq_restore(irq_sts);
    if (SUPPORT_STATISTIC == 1) {
        static_time_after = uapi_tcxo_get_us();
        uint64_t use_count = static_time_after - static_time_before;
        uint32_t *count = (uint32_t *)&use_count;
        PRINT("[func:program_load_width_32]  write [%d bytes] data into qspi DR cost[%d us]\r\n",
            qspi_data->data_size,
            count[0]);
    }
    while (is_trans_busy()) {
        NFPRINT("nandfash is sending data !!!\r\n");
    }
    return MC_SUCCESS;
}

uint32_t write_by_cpu_single_line(qspi_data_type_def *qspi_data)
{
    qspi_busy_wait();
    uint16_t address = SWAP_U16(qspi_data->address);
    uint8_t *addr = (uint8_t *)&address;
    hal_spi_interface_ctrlr0_reg ctrl0 = {0};
    nand_qspi_disable();
    ctrl0.d.spi_frf = SPI_STD_SPI_FRF;
    ctrl0.d.dfs_32 = DFR_32_FRAME_08BITS;
    ctrl0.d.tmod = SPI_EEPROM_TMOD;
    nand_qspi_ctlr0_cfg(ctrl0.d32);
    nand_qspi_ctlr1_cfg(0);
    nand_qspi_enable();
    uint32_t *send_buffer = (uint8_t *)qspi_data->data_ptr;
    uint8_t ins = qspi_data->instruction;
    uint64_t static_time_before;
    uint64_t static_time_after;
    if (SUPPORT_STATISTIC == 1) {
        static_time_before = uapi_tcxo_get_us();
    }
    unsigned int irq_sts = osal_irq_lock();
    cs_pulldown();
    writel(SER, 0x0);
    nand_qspi_write_data(ins);
    nand_qspi_write_data(0); // addr is 0
    nand_qspi_write_data(0); // addr is 0
    for (uint8_t i = 0; i < CPU_TRANS_BUFFER_DEEP; i++) {
        nand_qspi_write_data(qspi_data->data_ptr[i]);
    }
    writel(SER, SER_SELECT);
    for (uint32_t i = CPU_TRANS_BUFFER_DEEP; i < qspi_data->data_size; i++) {
        uint32_t timeout = 0;
        while (is_fifo_full() && (timeout < SEND_TIMEOUT)) {
            timeout = timeout + 1;
        }
        if (timeout == SEND_TIMEOUT) {
            NFPRINT("nandfash data writing timeout!!!\r\n");
        } else {
            nand_qspi_write_data(qspi_data->data_ptr[i]);
        }
    }
    cs_pullup();
    osal_irq_restore(irq_sts);
    if (SUPPORT_STATISTIC == 1) {
        static_time_after = uapi_tcxo_get_us();
        uint64_t use_count = static_time_after - static_time_before;
        uint32_t *count = (uint32_t *)&use_count;
        PRINT("[func:program_load_width_32]  write [%d bytes] data into qspi DR cost[%d us]\r\n",
            qspi_data->data_size,
            count[0]);
    }
    while (is_trans_busy()) {
        NFPRINT("nandfash is sending data !!!\r\n");
    }
    return MC_SUCCESS;
}

void nand_driver_init(nandflash_speed_type_t speed)
{
    UNUSED(speed);
    while (is_nandflash_working) {
        uapi_tcxo_delay_us(HAL_NAND_WAIT_3US);
    }
    writel(M_CTL_MTOP4_RST_SOFT_N_0, 0);
    writel(GLB_CTL_M_SOFT_RST, 0);
    writel(PMU_CTL_MCU_HS_PWR_EN, 1);
    uint32_t rdata = 0;
    while ((rdata & MCU_HS_EN_MASK) != MCU_HS_EN_MASK) {
        rdata = readl(PMU_CTL_MCU_HS_PWR_EN);
    }
    writel(M_CTL_MTOP4_RST_SOFT_N_0, 0x3);
    writel(GLB_CTL_M_SOFT_RST, 0x3);

    writew(HS_GPIO6_PINMUX, 0x1);
    writew(HS_GPIO6_PINMUX, 0x1);
    writew(HS_GPIO7_PINMUX, 0x1);
    writew(HS_GPIO8_PINMUX, 0x1);
    writew(HS_GPIO9_PINMUX, 0x1);
    writew(HS_GPIO11_PINMUX, 0x1);

    writew(HS_GPIO15_PINMUX, 0x1);
    writew(HS_GPIO6_PAD_CTRL, 0x2);
    writew(HS_GPIO7_PAD_CTRL, 0x2);
    writew(HS_GPIO8_PAD_CTRL, 0x7);
    writew(HS_GPIO9_PAD_CTRL, 0x7);

    writel(SSIENR, 0x0);
    writel(SER, 0x1);
    writel(BAUDR, DIV);
    writel(IMR, QSPI_INT_MASK);
    writel(TXFTLR, QSPI_FIFO_THRESHOLD);
    writel(RXFTLR, QSPI_FIFO_THRESHOLD);
    writel(SSIENR, 0x1);
    is_nandflash_working = false;
}