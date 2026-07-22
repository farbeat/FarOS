/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef TEST_SCREEN_EDO_H
#define TEST_SCREEN_EDO_H

#include "soc_mipi_tx.h"
#include "test_screen_lcd.h"

combo_dev_cfg_t *edo_get_dev_cfg(int index);
test_drv_api *edo_get_drv_api(void);

#endif