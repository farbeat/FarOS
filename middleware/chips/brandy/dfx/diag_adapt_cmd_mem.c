/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: diag mem adapt
 * This file should be changed only infrequently and with great care.
 */

#include "diag_cmd_mem_read_write.h"

typedef struct diag_mem_config {
    uintptr_t start_addr;
    uintptr_t end_addr;
} diag_mem_config_t;

const diag_mem_config_t g_mem_config[] = {
    { 0x00000000, 0x0000BFFF }, // MCPU_TCM BOOT_ROM
    { 0x00010000, 0x0003FFFF }, // MCPU_TCM ITCM_MEM
    { 0x20000000, 0x2006FFFF }, // MCPU_TCM DTCM_MEM

    { 0xA6000000, 0xA608FFFF }, // MCPU_AXIM BRAM
    { 0xA60A0000, 0xA617FFFF }, // MCPU_AXIM BROM
    { 0x87000000, 0x87017FFF }, // MCPU_AXIM COM_RAM
    { 0xA3000000, 0xA3000FFF }, // MCPU_AXIM MEM_X2P QSPI0_2CS
    { 0xA3004000, 0xA3008FFF }, // MCPU_AXIM MEM_X2P OPI
    { 0x60100000, 0x6039FFFF }, // MCPU_AXIM L2M_RAM
    { 0x603E0000, 0x603E095F }, // MCPU_AXIM GMMU_LUT
    { 0x61240000, 0x61FFFFFF }, // MCPU_AXIM VIR_ADDR
    { 0x68000000, 0x77FFFFFF }, // MCPU_AXIM XIP

    { 0x59000000, 0x590037FF }, // MCPU_AHBP BPERP_APB
    { 0x59004000, 0x59008FFF }, // MCPU_AHBP BPERP_APB
    { 0x5900C000, 0x59010FFF }, // MCPU_AHBP BPERP_APB
    { 0x59400000, 0x5941FFFF }, // MCPU_AHBP BT_SUB_AHB
    { 0x59800000, 0x59BFFFFF }, // MCPU_AHBP BDMA_CFG
    { 0x57000000, 0x57031FFF }, // MCPU_AHBP AON_APB
    { 0x57034000, 0x57039FFF }, // MCPU_AHBP AON_APB
    { 0x55000000, 0x55003FFF }, // MCPU_AHBP AON_APB
    { 0x52000000, 0x52002FFF }, // MCPU_AHBP MAIN_CFG_APB
    { 0x52004000, 0x52009FFF }, // MCPU_AHBP MAIN_CFG_APB
    { 0x52030000, 0x52033FFF }, // MCPU_AHBP SEC_MCU_AHB
    { 0x52061000, 0x52063FFF }, // MCPU_AHBP SDIO_CFG_BUS
    { 0x52065000, 0x52065FFF }, // MCPU_AHBP SDIO_CFG_BUS
    { 0x52070000, 0x52070FFF }, // MCPU_AHBP MDMA_CFG
    { 0x52080000, 0x5208AFFF }, // MCPU_AHBP M_PERP_APB
    { 0x52090000, 0x52090FFF }, // MCPU_AHBP M_PERP_APB
    { 0x520A0000, 0x520A0FFF }, // MCPU_AHBP SMDMA_CFG
    { 0x56000000, 0x56001FFF }, // MCPU_AHBP VAPB
    { 0x56010000, 0x5607FFFF }, // MCPU_AHBP VAPB
    { 0x58000000, 0x58FFFFFF }, // MCPU_AHBP AUDIO_SYS
};

static bool diag_permit_check(uintptr_t start_addr, uintptr_t end_addr)
{
    bool ret = false;
    uint32_t loop;

    for (loop = 0; loop < sizeof(g_mem_config) / sizeof(diag_mem_config_t); loop++) {
        if ((g_mem_config[loop].start_addr <= start_addr) && (g_mem_config[loop].end_addr >= end_addr)) {
            ret = true;
            break;
        }
    }
    return ret;
}

bool diag_cmd_permit_read(uintptr_t start_addr, uintptr_t end_addr)
{
    return diag_permit_check(start_addr, end_addr);
}

bool diag_cmd_permit_write(uintptr_t start_addr, uintptr_t end_addr)
{
    return diag_permit_check(start_addr, end_addr);
}
