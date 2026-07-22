/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power manage sys header file
 */

#ifndef LIB_PM_SYS_H
#define LIB_PM_SYS_H

#include "pm.h"
#include "errcode.h"
#include "pm_definition.h"

/**
 * @defgroup connectivity_libs_pm PM_SERVICE
 * @ingroup  connectivity_libs
 * @{
 */

pm_mode_t pm_get_cur_mode(void);

void pm_enter(pm_mode_t mode);

#endif