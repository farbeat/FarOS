/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: pm device manage header
 */

#ifndef SRC_LIB_PM_DEV_H
#define SRC_LIB_PM_DEV_H

#include "pm.h"
#include "errcode.h"
#include "chip_io.h"

enum {
    DEV_OSTIMER_ENABLE,
    DEV_OSTIMER_DISABLE,
};

enum {
    DEV_RTC_START,
};

enum {
    DEV_WDT_ENABLE,
    DEV_WDT_DISABLE,
};

errcode_t pm_dev_control(pm_dev_id_t dev_id, uint8_t type, uint8_t *data, uint8_t len);

errcode_t pm_dev_suspend(void);

void pm_dev_resume(void);

errcode_t pm_unregister_dev_ops(pm_dev_id_t dev_id);

#endif
