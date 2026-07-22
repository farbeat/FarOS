/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm core adapter header file
 */

#ifndef SRC_LIB_PM_CORE_H
#define SRC_LIB_PM_CORE_H

#include "chip_io.h"
#include "securec.h"
#include "product.h"
#include "lpc_core.h"
#include "non_os.h"

#define PM_GET_LR    __return_address()

void lowpower_cpu_suspend(void);

void lowpower_cpu_resume(void);

void pm_core_set_reset_pc(uint32_t cb);

void pm_core_suspend(void);

void pm_core_resume(void);

uint64_t pm_core_get_current_ms(void);

void pm_core_close_all_irq(void);

#endif
