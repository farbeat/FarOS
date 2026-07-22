/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  psram memory configurations
 * Author: @CompanyNameTag
 * Create:  2022-03-09
 */

#ifndef PSRAM_H
#define PSRAM_H

#include "product.h"

#if PSRAM_DENSITY_MB == 16
#include "psram_16.h"
#else /* PSRAM_DENSITY_MB == 32 */
#include "psram_32.h"
#endif

#endif // PSRAM_H
