/**
 * Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: dump
 * Author:
 * Create:
 */
#include "exception_dump.h"
#include <stdio.h>
#include <string.h>
#include "exception.h"
#include "memory_config.h"
#include "time.h"
#include "sys/time.h"
#include "chip_io.h"
#include "flash.h"
#include "soc_osal.h"
#include "vectors.h"
#include "chip_exception_utils.h"

#define MAX_FILE_NAME_LENGTH 100
#define REGS_BUFF_SIZE 500

#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#ifdef CONFIG_PSRAM_SUPPORT
#define PSRAM_DUMP_TO_FILE_LENGTH   0x40000  // 256KB
#endif

void exception_dump_memory_to_file(char *file_name, uint32_t dump_time, uintptr_t startAddr, uint32_t length)
{
    char *str = (char *)malloc(MAX_FILE_NAME_LENGTH);
    memset_s(str, MAX_FILE_NAME_LENGTH, 0, MAX_FILE_NAME_LENGTH);
    sprintf_s(str, MAX_FILE_NAME_LENGTH, file_name, dump_time);
    exc_info_write(str, 0, (const uint8_t *)startAddr, length);
}

typedef struct {
    char *name;
    uintptr_t start_addr;
    uint32_t len;
} mem_dump_to_file_t;

static const mem_dump_to_file_t g_mem_dump_to_file_info[] = {
    {"/user/exc/APP_DTCM_ORIGIN_%d.bin", APP_DTCM_ORIGIN, APP_DTCM_LENGTH},
    {"/user/exc/MCPU_TRACE_MEM_REGION_%d.bin", MCPU_TRACE_MEM_REGION_START, CPU_TRACE_MEM_REGION_LENGTH},
#ifdef CONFIG_PSRAM_SUPPORT
    {"/user/exc/PSRAM_ORIGIN_%d.bin", PSRAM_APP_DATA_ADDR, PSRAM_DUMP_TO_FILE_LENGTH},
#endif
};

void dump_mem_to_file(uint32_t time_sec)
{
    uint16_t count = (uint16_t)(sizeof(g_mem_dump_to_file_info) / sizeof(mem_dump_to_file_t));
    for (uint8_t i = 0; i < count; i++) {
        exception_dump_memory_to_file(g_mem_dump_to_file_info[i].name,
            time_sec,
            g_mem_dump_to_file_info[i].start_addr,
            g_mem_dump_to_file_info[i].len);
    }
}
#endif

#ifdef CFG_DUMP_REG_SUPPORT
char *g_regist_str  = "[PMU1_B_SOC_PWR_EN_REG %x, %x]\r\n[PMU1_BT_CBB_PWR_EN_REG %x, %x]\r\n[PMU1_BMEM_PWR_EN_REG %x, %x]\r\n[PMU1_BT_EM_PWR_EN_REG %x, %x]\r\n[PMU1_MCU_HS_PWR_EN_REG %x, %x]\r\n[PMU1_DSUB_PWR_EN_REG %x, %x]\r\n[PMU1_ASUB_PWR_EN_REG %x, %x]\r\n[PMU1_S_EGPIO_PWR_EN_REG %x, %x]\r\n[PMU1_BT_RF_PWR_EN_REG %x, %x]\r\n[PMU1_MCU_PWR_EN_REG %x, %x]\r\n[PMU1_MEM_PWR_EN_REG %x, %x]\r\n[PMU1_COM_PWR_EN_REG %x, %x]";
static const char *g_reg_path = "/user/exc/reg_path%d.txt";

void dump_reg_to_file(uint32_t time_sec)
{
    char *file_name = (char *)malloc(MAX_FILE_NAME_LENGTH);
    sprintf_s(file_name, MAX_FILE_NAME_LENGTH, g_reg_path, time_sec);
    char *str = (char *)malloc(REGS_BUFF_SIZE);
    uint32_t ctrl_base = 0x57004000;
    sprintf_s(str, REGS_BUFF_SIZE, g_regist_str,
                  ctrl_base + 0x20,  readl(ctrl_base + 0x20),
                  ctrl_base + 0x24,  readl(ctrl_base + 0x24),
                  ctrl_base + 0x28,  readl(ctrl_base + 0x28),
                  ctrl_base + 0x2c,  readl(ctrl_base + 0x2c),
                  ctrl_base + 0x30,  readl(ctrl_base + 0x30),
                  ctrl_base + 0x34,  readl(ctrl_base + 0x34),
                  ctrl_base + 0x38,  readl(ctrl_base + 0x38),
                  ctrl_base + 0x5c,  readl(ctrl_base + 0x5c),
                  ctrl_base + 0x60,  readl(ctrl_base + 0x60),
                  ctrl_base + 0x64,  readl(ctrl_base + 0x64),
                  ctrl_base + 0x68,  readl(ctrl_base + 0x68),
                  ctrl_base + 0x78,  readl(ctrl_base + 0x78));
    exc_info_write(file_name, 0, (uint8_t *)str, strlen(str));
}
#endif

void dump_all_to_file(void)
{
    uint32_t timestamp = get_local_timestamp();
#ifdef CFG_DRIVERS_NANDFLASH
    dump_mem_to_file(timestamp);
#endif
#ifdef CFG_DUMP_REG_SUPPORT
    dump_reg_to_file(timestamp);
#endif
}

#define MAGIC_SIZE 8
#define NORFLASH_TRACE_START 0x3d000
#define NORFLASH_DEBUG_LENGTH 0x3000
#define NORFLASH_DEVIATION 0x70000000
#define FIRMWARE_VERSION APPLICATION_VERSION_STRING
static const char *g_trace_in_norflash = "/user/exc/norflash_trace_%s_%d.bin";
static const char *g_exc_in_norflash = "/user/exc/norflash_exc_%s_%d.bin";
void write_trace(exc_context_t *exc_buff_addr)
{
#if defined(BUILD_APPLICATION_STANDARD) && !defined(BUILD_RECOVERY_IMAGE)
    uint8_t magic_array[MAGIC_SIZE] = {0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a};
    uapi_flash_exit_from_xip_mode(CHIP_FLASH_ID);
    uapi_flash_write_data(CHIP_FLASH_ID, NORFLASH_TRACE_START, magic_array, MAGIC_SIZE);
    uapi_flash_write_data(CHIP_FLASH_ID,
        NORFLASH_TRACE_START + MAGIC_SIZE,
        (const uint8_t *)MCPU_TRACE_MEM_REGION_START,
        CPU_TRACE_MEM_REGION_LENGTH);
    uint32_t exc_buff_size = sizeof(exc_context_t);
    uapi_flash_write_data(
        CHIP_FLASH_ID, NORFLASH_TRACE_START + MAGIC_SIZE + CPU_TRACE_MEM_REGION_LENGTH, exc_buff_addr, exc_buff_size);
    uapi_flash_switch_to_xip_mode(CHIP_FLASH_ID);
#endif
}

void dump_norflash_trace_to_file(void)
{
#if defined(BUILD_APPLICATION_STANDARD) && !defined(BUILD_RECOVERY_IMAGE)
    if (readl(NORFLASH_DEVIATION + NORFLASH_TRACE_START) == 0x5a5a5a5a) {
        uint32_t current_time = get_local_timestamp();
        char file_name_buff[MAX_FILE_NAME_LENGTH];
        uint32_t exc_buff_size = sizeof(exc_context_t);
        sprintf_s(file_name_buff, MAX_FILE_NAME_LENGTH, g_trace_in_norflash, FIRMWARE_VERSION, current_time);
        write_fileinfo(file_name_buff,
            0,
            (const uint8_t *)(NORFLASH_DEVIATION + NORFLASH_TRACE_START + MAGIC_SIZE),
            CPU_TRACE_MEM_REGION_LENGTH);
        memset_s(file_name_buff, MAX_FILE_NAME_LENGTH, 0, MAX_FILE_NAME_LENGTH);
        sprintf_s(file_name_buff, MAX_FILE_NAME_LENGTH, g_exc_in_norflash, FIRMWARE_VERSION, current_time);
        write_fileinfo(file_name_buff,
            0,
            (const uint8_t *)(NORFLASH_DEVIATION + NORFLASH_TRACE_START + MAGIC_SIZE + CPU_TRACE_MEM_REGION_LENGTH),
            exc_buff_size);
        unsigned int irq_sts = osal_irq_lock();
        if (uapi_flash_exit_from_xip_mode(0) != ERRCODE_SUCC) {
            return;
        }
        uapi_flash_block_erase(CHIP_FLASH_ID, NORFLASH_TRACE_START, NORFLASH_DEBUG_LENGTH, true);
        if (uapi_flash_switch_to_xip_mode(0) != ERRCODE_SUCC) {
            return;
        }
        osal_irq_restore(irq_sts);
    }
#endif
}

void printcpu_tracemem(void)
{
    uint32_t colum = 16;
    uint32_t line = CPU_TRACE_MEM_REGION_LENGTH / colum;
    for (uint32_t j = 0; j < line; j++) {
        for (uint32_t i = 0; i < colum; i++) {
            uint32_t index = j * colum + i + MCPU_TRACE_MEM_REGION_START;
            printf("%02x  ", readb(index));
        }
        printf("\r\n");
    }
}