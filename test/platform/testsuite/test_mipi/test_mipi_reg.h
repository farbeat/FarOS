/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi_tx self test.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef __TEST_MIPI_REG_H__
#define __TEST_MIPI_REG_H__

int32_t mipi_screen_init_reg_cfg(int argc, char* argv[]);

int32_t mipi_colorbar_on(int argc, char* argv[]);

int32_t mipi_colorbar_off(int argc, char* argv[]);

int32_t dpu_colorbar_on(int argc, char* argv[]);

#endif