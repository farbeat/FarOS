/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description:  mipi phy configuration.
 * Author: CompanyName
 * Create: 2020-12-11
 */

#ifndef __TEST_MIPI_PHY_H__
#define __TEST_MIPI_PHY_H__

int32_t hiwing_read_byte(uint32_t offset, uint32_t *read_val);

int32_t hiwing_write_byte(uint32_t offset, uint32_t reg_val);

int32_t mipi_phy_hiwing_write(int argc, char* argv[]);

int32_t mipi_phy_hiwing_read(int argc, char* argv[]);

int32_t mipi_phy_hiwing_init(int argc, char* argv[]);

void mipi_phy_hiwing_default_i2c(void);

#endif