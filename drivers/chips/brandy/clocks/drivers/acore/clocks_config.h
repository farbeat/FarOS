/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CLOCKS CONFIG HEADER
 * Author: @CompanyNameTag
 * Create:2020-05-09
 */

#ifndef SRC_DRIVERS_NON_OS_SHARED_CLOCKS_CONFIG_H
#define SRC_DRIVERS_NON_OS_SHARED_CLOCKS_CONFIG_H

#include "clocks_config_priv.h"
#include "clocks_types.h"
#include "platform_core.h"
#include "chip_io.h"

/** @addtogroup connectivity_drivers_non_os_clocks
  * @{
  */

#define GLB_GP_D_REG2  0x18
#define GLB_GP_D_REG3  0x1C

#define STANDARD_32K_FREQUENCY 32768

#ifdef SUPPORT_RC_CALIBRATION
#define RC_CALIBRATION_ADDR (GLB_CTL_D_RB_BASE + GLB_GP_D_REG3)
#define CLOCK_32K_CALI_FREQ (readw(RC_CALIBRATION_ADDR) == 0 ? STANDARD_32K_FREQUENCY : readw(RC_CALIBRATION_ADDR))
#else
#define CLOCK_32K_CALI_FREQ 32768
#endif

/**
  * @}
  */
#endif
