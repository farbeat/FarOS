/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Main boot for Second Stage Bootloader (SSB).
 * Author: @CompanyNameTag
 * Create: 2021-03-09
 */

#ifndef MAIN_H
#define MAIN_H

typedef enum {
    PSRAM_DENSITY_16M = 0x5,
    PSRAM_DENSITY_64M = 0x6,
    PSRAM_DENSITY_32M = 0x7,

    PSRAM_DENSITY_MAX
} psram_density_t;

#endif