/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  lcd test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef TEST_SCREEN_N_KDK_H
#define TEST_SCREEN_N_KDK_H

#include "test_screen_lcd.h"

combo_dev_cfg_t *kdk_get_dev_cfg(int index);

test_drv_api *kdk_get_drv_api(void);

#endif