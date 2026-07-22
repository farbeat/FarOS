/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description: Driver of WEITAI lcd screen.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef TEST_SCREEN_WEITAI_H
#define TEST_SCREEN_WEITAI_H

#include "soc_mipi_tx.h"
#include "test_screen_lcd.h"

combo_dev_cfg_t *weitai_get_dev_cfg(int index);

test_drv_api *weitai_get_drv_api(void);

#endif