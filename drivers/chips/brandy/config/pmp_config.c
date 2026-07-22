/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  chip pmp init config file.
 */
#include "pmp_config.h"
#include "stdbool.h"
#include "drv_pmp.h"
#include "memory_config.h"

#define L2MEM_DSP_ADDR_START    0x60100000
#define L2MEM_DSP_ADDR_LEN      0x100000

#define L2MEM_GPU_ADDR_START    0x60240000
#define L2MEM_GPU_ADDR_LEN      0x20000

#if PSRAM_DENSITY_MB == 16
#define PSRAM_ADDR_START        0x6C000000
#define PSRAM_ADDR_LEN          0x800000
#define PSRAM_GPU_ADDR_START    0x6CF80000
#define PSRAM_GPU_ADDR_LEN      0x40
#define PSRAM_SECOND_ADDR_START 0x6C800000
#define PSRAM_SECOND_ADDR_LEN   0x800000
#else  /* PSRAM_DENSITY_MB == 32 */
#define PSRAM_ADDR_START        0x6C000000
#define PSRAM_ADDR_LEN          0x1000000
#define PSRAM_GPU_ADDR_START    0X6DC00000
#define PSRAM_GPU_ADDR_LEN      0x40
#define PSRAM_SECOND_ADDR_START 0x6D000000
#define PSRAM_SECOND_ADDR_LEN   0x1000000
#endif

#define NOR_FLASH_ADDR_START    FLASH_START
#define NOR_FLASH_ADDR_LEN      FLASH_LENGTH

#define BOOTROM_PMP_LENGTH      0x10000
#define ITCM_PMP_LENGTH         0x10000
#define ITCM_PMP_SECOND_LENGTH  0x20000
#define DTCM_PMP_LENGTH         0x80000

typedef enum {
    BOOTROM_IDX                = 0,
    ITCM_IDX                   = 1,
    ITCM_SECOND_IDX            = 2,
    DTCM_IDX                   = 3,
    L2RAM_IDX                  = 4,
    SHARE_MEM_IDX              = 5,
    BT_RAM_APP_MAPPING_IDX     = 6,
    REG_ADDR_0_IDX             = 7,  /* reg 0x5x */
    REG_ADDR_1_IDX             = 8,  /* reg 0xa3 */
#if defined(CONFIG_PSRAM_SUPPORT) || defined(TEST_SUITE)
    PSRAM_DYN_LOAD_IDX,
    PSRAM_TEXT_IDX,
    PSRAM_IDX,
    PMP_DEBUG_IDX,  /* pmp调测预留idx */
    PSRAM_SECOND_IDX,
#endif
    NOR_FLASH_IDX,
    BACKGROUND_IDX             = 15,
    MAX_REGION_NUM
} region_index_t;

pmp_conf_t g_region_attr[MAX_REGION_NUM];

static void pmp_init_config(void)
{
    /* bootrom 可读不可写不可执行 */
    g_region_attr[BOOTROM_IDX].idx = BOOTROM_IDX;
    g_region_attr[BOOTROM_IDX].addr = ROM_START;
    g_region_attr[BOOTROM_IDX].size = 0x100;
    g_region_attr[BOOTROM_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[BOOTROM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_NEXECUTE;
    g_region_attr[BOOTROM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[BOOTROM_IDX].conf.lock = true;

    /* ITCM 第一段 可读可执行不可写 */
    g_region_attr[ITCM_IDX].idx = ITCM_IDX;
    g_region_attr[ITCM_IDX].addr = APP_ITCM_ORIGIN;
    g_region_attr[ITCM_IDX].size = ITCM_PMP_LENGTH;
    g_region_attr[ITCM_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[ITCM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_EXECUTE;
    g_region_attr[ITCM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[ITCM_IDX].conf.lock = true;

    /* ITCM 第二段 可读可执行不可写 */
    g_region_attr[ITCM_SECOND_IDX].idx = ITCM_SECOND_IDX;
    g_region_attr[ITCM_SECOND_IDX].addr = (APP_ITCM_ORIGIN + ITCM_PMP_LENGTH);
    g_region_attr[ITCM_SECOND_IDX].size = ITCM_PMP_SECOND_LENGTH;
    g_region_attr[ITCM_SECOND_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[ITCM_SECOND_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_EXECUTE;
    g_region_attr[ITCM_SECOND_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[ITCM_SECOND_IDX].conf.lock = true;

    /* DTCM 可读可写不可执行 */
    g_region_attr[DTCM_IDX].idx = DTCM_IDX;
    g_region_attr[DTCM_IDX].addr = APP_DTCM_ORIGIN;
    g_region_attr[DTCM_IDX].size = DTCM_PMP_LENGTH;
    g_region_attr[DTCM_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[DTCM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[DTCM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[DTCM_IDX].conf.lock = true;

    /* L2MEM 可读可执行可写 no cache属性 */
    g_region_attr[L2RAM_IDX].idx = L2RAM_IDX;
    g_region_attr[L2RAM_IDX].addr = 0x60000000;
    g_region_attr[L2RAM_IDX].size = 0x2000000;
    g_region_attr[L2RAM_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[L2RAM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_EXECUTE;
    g_region_attr[L2RAM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[L2RAM_IDX].conf.lock = true;

    /* SHARE_MEM_IDX 可读可写不可执行 不打开cache属性 */
    g_region_attr[SHARE_MEM_IDX].idx = SHARE_MEM_IDX;
    g_region_attr[SHARE_MEM_IDX].addr = 0x87000000;
    g_region_attr[SHARE_MEM_IDX].size = 0x20000;
    g_region_attr[SHARE_MEM_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[SHARE_MEM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_EXECUTE;
    g_region_attr[SHARE_MEM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[SHARE_MEM_IDX].conf.lock = true;

    /* BT_RAM_APP_MAPPING_IDX 可读可写不可执行 不打开cache属性 */
    g_region_attr[BT_RAM_APP_MAPPING_IDX].idx = BT_RAM_APP_MAPPING_IDX;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].addr = 0xA6000000;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].size = 0x200000;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].conf.pmp_attr = PMP_ATTR_DEVICE_NO_BUFFERABLE;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[BT_RAM_APP_MAPPING_IDX].conf.lock = true;

    /* REG_ADDR_0_IDX 可读可写不可执行 不打开cache属性 */
    g_region_attr[REG_ADDR_0_IDX].idx = REG_ADDR_0_IDX;
    g_region_attr[REG_ADDR_0_IDX].addr = 0x50000000;
    g_region_attr[REG_ADDR_0_IDX].size = 0x10000000;
    g_region_attr[REG_ADDR_0_IDX].conf.pmp_attr = PMP_ATTR_DEVICE_NO_BUFFERABLE;
    g_region_attr[REG_ADDR_0_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[REG_ADDR_0_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[REG_ADDR_0_IDX].conf.lock = true;

    /* REG_ADDR_1_IDX 可读可写不可执行 不打开cache属性 */
    g_region_attr[REG_ADDR_1_IDX].idx = REG_ADDR_1_IDX;
    g_region_attr[REG_ADDR_1_IDX].addr = 0xA0000000;
    g_region_attr[REG_ADDR_1_IDX].size = 0x20000000;
    g_region_attr[REG_ADDR_1_IDX].conf.pmp_attr = PMP_ATTR_DEVICE_NO_BUFFERABLE;
    g_region_attr[REG_ADDR_1_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[REG_ADDR_1_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[REG_ADDR_1_IDX].conf.lock = true;

#if defined(CONFIG_PSRAM_SUPPORT) || defined(TEST_SUITE)
    /* PSRAM syn load 可读可写可执行 打开cache属性 */
    g_region_attr[PSRAM_DYN_LOAD_IDX].idx = PSRAM_DYN_LOAD_IDX;
    g_region_attr[PSRAM_DYN_LOAD_IDX].addr = PSRAM_ADDR_START;
    g_region_attr[PSRAM_DYN_LOAD_IDX].size = PSRAM_APP_SYN_LOAD_LENGTH;
    g_region_attr[PSRAM_DYN_LOAD_IDX].conf.pmp_attr = PMP_ATTR_WRITEBACK_RWALLOCATE;
    g_region_attr[PSRAM_DYN_LOAD_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_EXECUTE;
    g_region_attr[PSRAM_DYN_LOAD_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[PSRAM_DYN_LOAD_IDX].conf.lock = true;

    /* PSRAM TEXT 可读不可写可执行 打开cache属性 */
    g_region_attr[PSRAM_TEXT_IDX].idx = PSRAM_TEXT_IDX;
    g_region_attr[PSRAM_TEXT_IDX].addr = PSRAM_ADDR_START;
    g_region_attr[PSRAM_TEXT_IDX].size = PSRAM_APP_CODE_LENGTH;
    g_region_attr[PSRAM_TEXT_IDX].conf.pmp_attr = PMP_ATTR_WRITEBACK_RWALLOCATE;
    g_region_attr[PSRAM_TEXT_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_EXECUTE;
    g_region_attr[PSRAM_TEXT_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[PSRAM_TEXT_IDX].conf.lock = true;

    /* PSRAM 第一段 可读可写不可执行 打开cache属性 */
    g_region_attr[PSRAM_IDX].idx = PSRAM_IDX;
    g_region_attr[PSRAM_IDX].addr = PSRAM_ADDR_START;
    g_region_attr[PSRAM_IDX].size = PSRAM_ADDR_LEN;
    g_region_attr[PSRAM_IDX].conf.pmp_attr = PMP_ATTR_WRITEBACK_RWALLOCATE;
    g_region_attr[PSRAM_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[PSRAM_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[PSRAM_IDX].conf.lock = true;

    /* PSRAM 第二段 可读可写不可执行 关闭cache属性 */
    g_region_attr[PSRAM_SECOND_IDX].idx = PSRAM_SECOND_IDX;
    g_region_attr[PSRAM_SECOND_IDX].addr = PSRAM_SECOND_ADDR_START;
    g_region_attr[PSRAM_SECOND_IDX].size = PSRAM_SECOND_ADDR_LEN;
    g_region_attr[PSRAM_SECOND_IDX].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    g_region_attr[PSRAM_SECOND_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_RW_NEXECUTE;
    g_region_attr[PSRAM_SECOND_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[PSRAM_SECOND_IDX].conf.lock = true;
#endif

    /* Flash 可读不可写可执行 打开cache属性 */
    g_region_attr[NOR_FLASH_IDX].idx = NOR_FLASH_IDX;
    g_region_attr[NOR_FLASH_IDX].addr = NOR_FLASH_ADDR_START;
    g_region_attr[NOR_FLASH_IDX].size = NOR_FLASH_ADDR_LEN;
    g_region_attr[NOR_FLASH_IDX].conf.pmp_attr = PMP_ATTR_WRITEBACK_RWALLOCATE;
    g_region_attr[NOR_FLASH_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_EXECUTE;
    g_region_attr[NOR_FLASH_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[NOR_FLASH_IDX].conf.lock = true;

    /* BACKGROUND_IDX 不可读不可写不可执行 */
    g_region_attr[BACKGROUND_IDX].idx = BACKGROUND_IDX;
    g_region_attr[BACKGROUND_IDX].addr = 0x0;
    g_region_attr[BACKGROUND_IDX].size = 0xFFFFFFFF; /* 4GB */
    g_region_attr[BACKGROUND_IDX].conf.pmp_attr = PMP_ATTR_DEVICE_NO_BUFFERABLE;
    g_region_attr[BACKGROUND_IDX].conf.rwx_permission = (rwx_permission_t)PMPCFG_NO_ACCESS;
    g_region_attr[BACKGROUND_IDX].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    g_region_attr[BACKGROUND_IDX].conf.lock = true;
}

void pmp_init(void)
{
    pmp_init_config();
    uapi_pmp_config(g_region_attr, MAX_REGION_NUM);
}

void pmp_gpu_psram_init(void)
{
#if defined(SUPPORT_PMP_DEBUG) && defined(CONFIG_PSRAM_SUPPORT)
    pmp_conf_t gpu_psram_pmp_region_attr[1] = {0};

    /* PSRAM 第GPU段 可读不可写不可执行 关闭cache属性 */
    gpu_psram_pmp_region_attr[0].idx = PMP_DEBUG_IDX;
    gpu_psram_pmp_region_attr[0].addr = PSRAM_GPU_ADDR_START;
    gpu_psram_pmp_region_attr[0].size = PSRAM_GPU_ADDR_LEN;
    gpu_psram_pmp_region_attr[0].conf.pmp_attr = PMP_ATTR_NO_BUFFERABLE_AND_NO_CACHEABLE;
    gpu_psram_pmp_region_attr[0].conf.rwx_permission = (rwx_permission_t)PMPCFG_READ_ONLY_NEXECUTE;
    gpu_psram_pmp_region_attr[0].conf.addr_match = PMPCFG_ADDR_MATCH_NAPOT;
    gpu_psram_pmp_region_attr[0].conf.lock = true;

    uapi_pmp_config(gpu_psram_pmp_region_attr, 1);
#endif
}
