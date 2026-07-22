/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:   header file for the hal_sectors
 * Author: @CompanyNameTag
 * Create:  2018-10-15
 */

#ifndef HAL_SECTORS_H
#define HAL_SECTORS_H

#include "stdint.h"

/** @defgroup connectivity_drivers_hal_sectors SECTORS
  * @ingroup  connectivity_drivers_hal
  * @{
  */
/** he size of the RAM assigned to the heap in internal RAM (in octets) */
extern uint32_t g_hal_sectors_ram_heap_size;

/** The pointer to the start of the external RAM assigned to the internal RAM heap */
extern void *g_hal_sectors_ram_heap_start;

/** The total size of the RAM (in octets) */
extern uint32_t g_hal_sectors_ram_size;

/** The start address of RAM */
extern void *g_hal_sectors_ram_start;

#if (defined(BUILD_APPLICATION_SSB)) && (!defined(SUPPORT_PARTITION_INFO))
/** The number of flash pages */
extern const uint32_t g_hal_sectors_flash_region_pages;

/** The end of the flash region */
extern const void *g_hal_sectors_flash_region_end;
#endif /* BUILD_APPLICATION_SSB */

/**
  * @}
  */
#endif
