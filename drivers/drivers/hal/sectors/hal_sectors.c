/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  hal_sectors c file - get variables from the linker file.
 * Author: @CompanyNameTag
 * Create:  2018-10-15
 */

#include "hal_sectors.h"

#if defined(BUILD_APPLICATION_SSB)
#include "memory_config.h"
#define KV_PAGE_SIZE KV_PAGES
#endif

//lint -esym(526, g_intheap_size)
//lint -esym(526, g_intheap_begin)
// Declare the linker symbols
extern void *g_intheap_size;
extern void *g_intheap_begin;

/** The size of the RAM assigned to the heap in internal RAM (in octets) */
uint32_t g_hal_sectors_ram_heap_size = ((uint32_t)&g_intheap_size);

/** The pointer to the start of the external RAM assigned to the internal RAM heap */
#ifdef __ICCARM__
#pragma section = "HEAP"
void *g_hal_sectors_ram_heap_start = __section_begin("HEAP");
#else
void *g_hal_sectors_ram_heap_start = ((void *)&g_intheap_begin);
#endif
#if (defined(BUILD_APPLICATION_SSB)) && (!defined(SUPPORT_PARTITION_INFO))

#if BTH_WITH_SMART_WEAR == NO
/** The number of flash pages */
const uint32_t g_hal_sectors_flash_region_pages = SSB_FLASH_REGION_PAGES + KV_PAGE_SIZE + SSB_BACKUP_FLASH_REGION_PAGES;
#else
const uint32_t g_hal_sectors_flash_region_pages = SSB_FLASH_REGION_PAGES + KV_PAGE_SIZE;
#endif

/** The end of the flash region */
const void *g_hal_sectors_flash_region_end = (void *)(FLASH_CORE_IMAGES_START);
#endif
