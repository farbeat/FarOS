/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power tickless header file
 */

#ifndef LIB_PM_TICKLESS_H
#define LIB_PM_TICKLESS_H

#include "chip_io.h"

/**
 * @defgroup connectivity_libs_pm PM_SERVICE
 * @ingroup  connectivity_libs
 * @{
 */

void pm_tickless_start(uint32_t sleep_ms);

void pm_tickless_stop(uint32_t sleep_ms);

#endif